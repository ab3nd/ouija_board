/* Control software for the Ouija Board. The board is an Arduino Nano with the old bootloader.
   By Ab3nd.
*/

#include <Servo.h>

//Limit switch connections
#define LIMIT_X 9
#define LIMIT_Y 10

//Stepper control
#define ENABLE 8
#define DIR_X 5
#define DIR_Y 6
#define STEP_X 2
#define STEP_Y 3

//Sensor
#define ENABLE_SENSE 4
#define S0 7
#define S1 11
#define S2 12
#define SENSE A0

#define SERVO_PIN 13

//Measurements, from home, of the working area, in steps
#define Y_MAX 30921
#define X_MAX 18995

/* "NO"   600   5500
   "YES"  600   24600
   A      8800  29000
   B      7200  26500
   C      6500  24000
   D      5500  22000
   E      5200  20000
   F      5100  17500
   G      5000  15500
   H      5000  13500
   I      5000  11000
   J      5500  9000
   K      6000  7000
   L      6800  4500
   M      8300  2300
   N      13200 28300
   O      11600 26400
   P      10600 24400
   Q      9600  22400
   R      8800  20200
   S      8600  17700
   T      8400  15700
   U      8100  13500
   V      8300  11000
   W      9000  8500
   X      10500 6100
   Y      11500 4000
   Z      12800 1800
*/
int current_x = 0;
int current_y = 0;

struct point {
  int x;
  int y;
};

//26 letters, in order a-z
struct point letters[26] = {
  {8800, 29000},
  {7200, 26500},
  {6500, 24000},
  {5500, 22000},
  {5200, 20000},
  {5100, 17500},
  {5000, 15500},
  {5000, 13500},
  {5000, 11000},
  {5500, 9000},
  {6000, 7000},
  {6800, 4500},
  {8300, 2300},
  {13200, 28300},
  {11600, 26400},
  {10600, 24400},
  {9600, 22400},
  {8800, 20200},
  {8600, 17700},
  {8400, 15700},
  {8100, 13500},
  {8300, 11000},
  {9000, 8500},
  {10500, 6100},
  {11500, 4000},
  {12800, 1800}
};

point no = {600, 5500};
point yes = {600, 24600};

int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int cal_readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* Map counts on the address lines of the mux to indices for readings
   so that the readings are sequential around the ring */
int addr_map[8] = {0, 1, 2, 7, 3, 6, 4, 5};

Servo mag_lift;

String serial_cmd;

unsigned long last_moved;

void setup() {

  //Set up the magnet height
  mag_lift.attach(SERVO_PIN);
  //90 is very low, 40 looks about good
  mag_lift.write(40);

  //Limit switches
  pinMode(LIMIT_X, INPUT);
  pinMode(LIMIT_Y, INPUT);

  //Set up the stepper control
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(STEP_X, OUTPUT);
  pinMode(STEP_Y, OUTPUT);

  // Hall ring sensor
  pinMode(ENABLE_SENSE, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(SENSE, INPUT);

  // Set up sensor
  digitalWrite(ENABLE_SENSE, HIGH);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);

  //Accumulate 10 samples
  for (int count = 0; count < 10; count++) {
    for (byte ii = 0; ii < 8; ii++)
    {
      //Disable the switch
      digitalWrite(ENABLE_SENSE, HIGH);
      //Set the address bits
      (ii & B00000001) ? digitalWrite(S0, HIGH) : digitalWrite(S0, LOW);
      (ii & B00000010) ? digitalWrite(S1, HIGH) : digitalWrite(S1, LOW);
      (ii & B00000100) ? digitalWrite(S2, HIGH) : digitalWrite(S2, LOW);
      digitalWrite(ENABLE_SENSE, LOW);
      cal_readings[addr_map[ii]] += analogRead(SENSE);
    }
  }

  //Get their average
  for (int idx = 0; idx < 8; idx++)
  {
    //Each reading should be around 512 with no magnet, so these
    //should all be around 5120, and so not have an underflow
    //dividing by 10
    cal_readings[idx] = cal_readings[idx] / 10;
  }

  //For debugging
  Serial.begin(9600);

  //Just for initialization
  last_moved = millis();
}


void move_home() {
  //Enable stepper drivers
  digitalWrite(ENABLE, LOW);

  //Home x axis first
  digitalWrite(DIR_X, LOW);
  digitalWrite(STEP_X, LOW);
  while (digitalRead(LIMIT_X) == 1) {
    digitalWrite(STEP_X, HIGH);
    delayMicroseconds(200);
    digitalWrite(STEP_X, LOW);
    delayMicroseconds(200);
  }

  //Home Y axis, it uses opposite DIR_Y setting
  digitalWrite(DIR_Y, HIGH);
  digitalWrite(STEP_Y, LOW);
  while (digitalRead(LIMIT_Y) == 1) {
    digitalWrite(STEP_Y, HIGH);
    delayMicroseconds(200);
    digitalWrite(STEP_Y, LOW);
    delayMicroseconds(200);
  }

  //Disable stepper drivers
  digitalWrite(ENABLE, HIGH);

  //Reset the position counters
  current_x = current_y = 0;
}

void single_step(int dir, int axis, int vel) {

  // Enable axis and maintain step counts
  digitalWrite(ENABLE, LOW);
  if (axis == STEP_X) {
    digitalWrite(DIR_X, dir);
    //High is away from home for X
    if (dir) {
      current_x++;
    }
    else {
      current_x--;
    }
  }
  else {
    digitalWrite(DIR_Y, dir);
    //HIGH is towards home for Y
    if (dir) {
      current_y--;
    }
    else {
      current_y++;
    }
  }

  // Single step
  digitalWrite(axis, HIGH);
  delayMicroseconds(vel);
  digitalWrite(axis, LOW);
  delayMicroseconds(vel);

  //Update last move
  last_moved = millis();
}

void move_to(int x, int y, int vel) {
  //In this house, we only move within the operating area of the gantry
  y = constrain(y, 0, Y_MAX);
  x = constrain(x, 0, X_MAX);

  //Figure out X and Y directions
  int x_dir = HIGH;  // Away from home
  if ((current_x - x) > 0) {
    x_dir = LOW;  // Towards home
  }

  int y_dir = LOW;  //Away from home
  if ((current_y - y) > 0) {
    y_dir = HIGH;  // Towards home
  }

  //Ratio of X to Y, so how many steps to take of each per cycle
  float ratio = float(abs(current_x - x)) / float(abs(current_y - y));

  int stepX, stepY;
  if (ratio >= 1) {
    stepX = int(ratio);
    stepY = 1;
  } else {
    stepX = 1;
    stepY = int(1.0 / ratio);
  }

  while ((current_y != y) && (current_x != x)) {
    //Do the x steps, if any
    for (int ii = 0; ii < stepX; ii++) {
      if (current_x == x) {
        break;
      }
      single_step(x_dir, STEP_X, vel);
    }
    //Do the y steps, if any
    for (int ii = 0; ii < stepY; ii++) {
      if (current_y == y) {
        break;
      }
      single_step(y_dir, STEP_Y, vel);
    }
  }

  while (current_y != y) {
    single_step(y_dir, STEP_Y, vel);
  }

  while (current_x != x) {
    single_step(x_dir, STEP_X, vel);
  }
}

void say_string(String msg) {
  int index = 0;
  struct point p;
  //For all the letters
  for (int ii = 0; ii < msg.length(); ii++) {
    //String is all lowercase, so subtracting 'a' converts from ascii
    //to a=0, b=1, c=2,..., which can then be used to
    //look up coordinates in a lookup table and move
    index = msg[ii] - 'a';
    p = letters[index];
    move_to(p.x, p.y, 100);
    //Delay to let people read it off
    delay(1000);
    //If the next letter is the same as this letter
    while ((ii != msg.length() - 1) && (msg[ii] == msg[ii + 1])) {
      //Skip the next letter
      ii++;
      //move off this letter and then back on
      move_to(current_x - 800, current_y - 800, 100);
      move_to(current_x + 800, current_y + 800, 100);
      delay(1000);
    }
  }
}

void update_sensor() {
  for (byte ii = 0; ii < 8; ii++)
  {
    //Disable the switch
    digitalWrite(ENABLE_SENSE, HIGH);
    //Set the address bits
    (ii & B00000001) ? digitalWrite(S0, HIGH) : digitalWrite(S0, LOW);
    (ii & B00000010) ? digitalWrite(S1, HIGH) : digitalWrite(S1, LOW);
    (ii & B00000100) ? digitalWrite(S2, HIGH) : digitalWrite(S2, LOW);
    digitalWrite(ENABLE_SENSE, LOW);
    readings[addr_map[ii]] = analogRead(SENSE) - cal_readings[addr_map[ii]];
  }
}

bool close_to(int a, int b, int thresh) {
  return b - thresh <= a <= b + thresh;
}

float distance(point p1, point p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

char closest_letter() {
  //Hacky, but the biggest distance between two points on the
  //board can't be more than the sum of the edges of the board
  unsigned long min_dist = X_MAX + Y_MAX;
  int min_idx = 0;
  float d = 0;
  point current = {current_x, current_y};
  for (int ii = 0; ii < 25; ii++) {
    d = distance(current, letters[ii]);
    if (d < min_dist)
    {
      min_idx = ii;
      min_dist = d;
    }
  }

  //Capital A is 65;
  char retval = (char)(min_idx + 65);

  //Check if the distance to "No" is even closer
  d = distance(no, current);
  if (d < min_dist) {
    retval = 0;
    min_dist = d;
  }
  
  //Finally, check if the distance to "Yes" is closer
  d = distance(no, current);
  if (d < min_dist) {
    retval = 1;
    min_dist = d;
  }

  //Too far from any letter to count
  if(min_dist > 2000)
  {
    retval = 2;
  }

  //Return is a char, but it's 0 for No, 1 for Yes, 2 for no letter, and the letter for A-Z
  return retval;
}

void follow_planchette() {
  int vel = 100;
  int move_dist = 200;

  //Get the maximum sensor reading
  byte max_idx = 0;
  int max_val = 0;
  for (byte ii = 0; ii < 8; ii++)
  {
    if (readings[ii] > max_val)
    {
      max_val = readings[ii];
      max_idx = ii;
    }
  }

  Serial.print("max_val: ");
  Serial.println(max_val);
  
  //If we need to get moving
  if (max_val > 10) {
    //Get the second max
    byte second_max_idx = 0;
    byte second_max = 0;

    if (max_idx == 0)
    {
      //Handle wrap at zero
      (readings[max_idx + 1] > readings[7]) ? second_max_idx = max_idx + 1 : second_max_idx = 7;
    }
    else if (max_idx == 7)
    {
      //Handle wrap at 7
      (readings[0] > readings[max_idx - 1]) ? second_max_idx = 0 : second_max_idx = max_idx - 1;
    }
    else
    {
      //Check sensors next to this sensor
      (readings[max_idx + 1] > readings[max_idx - 1]) ? second_max_idx = max_idx + 1 : second_max_idx = max_idx - 1;
    }

    //Figure out the move
    int target_x = current_x;
    int target_y = current_y;

    //Add a little bit to current postion to get new position
    if ((max_idx == 0 && second_max_idx == 1) || (max_idx == 1 && second_max_idx == 0))
    {
      //Down and Left
      target_x += move_dist;
      target_y += move_dist;
    }
    else if ((max_idx == 1 && second_max_idx == 2) || (max_idx == 2 && second_max_idx == 1))
    {
      //Left
      target_y += move_dist;
    }
    else if ((max_idx == 2 && second_max_idx == 3) || (max_idx == 3 && second_max_idx == 2))
    {
      //Up and Left
      target_x -= move_dist;
      target_y += move_dist;
    }
    else if ((max_idx == 3 && second_max_idx == 4) || (max_idx == 4 && second_max_idx == 3))
    {
      //Up
      target_x -= move_dist;
    }
    else if ((max_idx == 4 && second_max_idx == 5) || (max_idx == 5 && second_max_idx == 4))
    {
      //Up and Right
      target_x -= move_dist;
      target_y -= move_dist;
    }
    else if ((max_idx == 5 && second_max_idx == 6) || (max_idx == 6 && second_max_idx == 5))
    {
      //Right
      target_y -= move_dist;
    }
    else if ((max_idx == 6 && second_max_idx == 7) || (max_idx == 7 && second_max_idx == 6))
    {
      //Down and Right
      target_x += move_dist;
      target_y -= move_dist;
    }
    else if ((max_idx == 7 && second_max_idx == 0) || (max_idx == 0 && second_max_idx == 7))
    {
      //Down
      target_x += move_dist;
    }

    move_to(target_x, target_y, vel);
  }
}

void move_yes() 
{
  if ((current_y == 24600) && (current_x == 600)) {
    //move off and then back on
    move_to(current_x - 500, current_y - 800, 100);
    move_to(current_x + 500, current_y + 800, 100);
  }
  else {
    move_to(600, 24600, 100);
  }  
}

void move_no()
{
  if ((current_y == 5500) && (current_x == 600)) {
    //move off and then back on
    move_to(current_x - 500, current_y - 800, 100);
    move_to(current_x + 500, current_y + 800, 100);
  }
  else {
    move_to(600, 5500, 100);
  }  
}

void get_serial_cmd() {

  while (!Serial.available()) {} // wait for data to arrive

  int index = 0;
  char *tok;
  long x, y;

  if (Serial.available()) {
    serial_cmd = Serial.readStringUntil('\n');
    //Previous line would block until we got something
    //The returning command from the user is a string to say,
    //And then a yes/no answer string
    if (serial_cmd.startsWith("s")) {

      //Split on first space, command is of the form "s some string here"
      index = serial_cmd.indexOf(" ");
      String message = serial_cmd.substring(index + 1);
      //Send it back as confirmation
      say_string(message);

    }
    else if (serial_cmd.startsWith("a")) {

      //Answer, message is of the form "a yyny" or similar 
      index = serial_cmd.indexOf(" ");
      String answer = serial_cmd.substring(index + 1);
      for(int ii = 0; ii < answer.length(); ii++)
      {
        if(answer.charAt(ii) == "y")
        {
          move_yes();
        }
        else if(answer.charAt(ii) == "n")
        {
          move_no();
        }
      }
    }

    else {
      Serial.print("What even is a ");
      Serial.println(serial_cmd);
    }
  }
}

enum states {START, FOLLOW_MOVE, SEND_CHAR, DONE_WAIT, SHOW_RESULT};
uint8_t state = START;

void loop() {
  update_sensor();
  run_state_machine();
}

void run_state_machine()
{
  switch (state)
  {
    case START:
      //Home the axes
      move_home();
      Serial.println("start -> follow move");
      state = FOLLOW_MOVE;
      break;
    case FOLLOW_MOVE:
      follow_planchette();
      if ((millis() - last_moved) > 1500)
      {
        Serial.println("follow move -> send char");
        state = SEND_CHAR;
      }
      break;
    case SEND_CHAR:
      //Get the closest character
      char toSend = closest_letter();
      Serial.print("Send: ");
      Serial.println((int)toSend);
      
      if (toSend == 1)
      {
        //If it is "yes", next state is Done Wait
        Serial.println("send char -> done wait");
        state = DONE_WAIT;
        Serial.println("Yes");
      }
      else if (toSend == 2)
      {
        //It is a "No character", next state is FOLLOW_MOVE
        Serial.println("send char -> follow move");
        state = FOLLOW_MOVE;
      }
      else if (toSend >= 65 && toSend <= (65+26)) {
        //If it is a letter, next state is Follow Move
        Serial.println("send char -> follow move");
        state = FOLLOW_MOVE;
        Serial.println(toSend);
      }
      else{
        //We don't handle "no" yet, other cases as well...
        state = FOLLOW_MOVE;
      }
      break;
    case DONE_WAIT:
      get_serial_cmd();
      Serial.println("done wait -> follow move");
      state = FOLLOW_MOVE;
      break;
  }
}
