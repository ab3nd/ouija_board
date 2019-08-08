/* Control software for the Ouija Board. The board is an Arduino Nano with the old bootloader.
   By Ab3nd.
*/

#include <Servo.h>

#define LIMIT_X 9
#define LIMIT_Y 10

#define ENABLE 8
#define DIR_X 5
#define DIR_Y 6
#define STEP_X 2
#define STEP_Y 3

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

Servo mag_lift;

String serial_cmd;

void setup() {
  //Limit switches
  pinMode(LIMIT_X, INPUT);
  pinMode(LIMIT_Y, INPUT);

  //Set up the stepper control
  pinMode(ENABLE, OUTPUT);
  pinMode(DIR_X, OUTPUT);
  pinMode(DIR_Y, OUTPUT);
  pinMode(STEP_X, OUTPUT);
  pinMode(STEP_Y, OUTPUT);

  //For debugging
  Serial.begin(9600);

  //Home the axes
  move_home();

  //Set up the magnet height
  mag_lift.attach(13);
  //90 is very low, 40 looks about good
  mag_lift.write(40);
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
    move_to(p.x, p.y, 50);
    //Delay to let people read it off
    delay(700);
    //If the next letter is the same as this letter
    while ((ii != msg.length() - 1) && (msg[ii] == msg[ii + 1])) {
      //Skip the next letter
      ii++;
      //move off this letter and then back on
      move_to(current_x - 800, current_y - 800, 50);
      move_to(current_x + 800, current_y + 800, 50);
      delay(1000);
    }
  }
}

bool close_to(int a, int b, int thresh){
  return b-thresh <= a <= b+thresh;  
}

void loop() {
  // TODO: Velocity curves/trapezoidal acceleration
  // proportional speed based on distance, can treat X and Y seperately
  // Figure out top speed

  while (!Serial.available()) {} // wait for data to arrive

  int index = 0;
  char *tok;
  long x, y;

  if (Serial.available()) {
    serial_cmd = Serial.readStringUntil('\n');
    //Previous line would block until we got something, now parse it
    if (serial_cmd.startsWith("s")) {

      //Split on first space, command is of the form "s some string here"
      index = serial_cmd.indexOf(" ");
      String message = serial_cmd.substring(index + 1);
      //Send it back as confirmation
      Serial.println(message);
      say_string(message);

    }
    else if (serial_cmd.startsWith("m")) {

      //Split on spaces, command is of the form "m xx yy"
      //second and third entries are x and y
      index = serial_cmd.indexOf(" ");
      x = serial_cmd.substring(index + 1, serial_cmd.indexOf(" ", index + 1)).toInt();
      y = serial_cmd.substring(serial_cmd.indexOf(" ", index + 1)).toInt();

      //Send it back as confirmation
      Serial.print("Move to ");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(" from ");
      Serial.print(current_x);
      Serial.print(" ");
      Serial.println(current_y);
      move_to(x, y, 100);
    }

    else if (serial_cmd.startsWith("h")) {
      move_home();
      Serial.println("Moved home");
    }

    else if (serial_cmd.startsWith("y")) {
      if ((current_y == 24600) && (current_x == 600)) {
        //move off and then back on
        move_to(current_x + 800, current_y - 800, 100);
        move_to(current_x - 800, current_y + 800, 100);
      }
      else {
        move_to(600, 24600, 100);
      }
      Serial.println("Move to Yes");
      
    }

    else if (serial_cmd.startsWith("n")) {
      if ((current_y == 5500) && (current_x == 600)) {
        //move off and then back on
        move_to(current_x + 800, current_y - 800, 100);
        move_to(current_x - 800, current_y + 800, 100);
      }
      else {
        move_to(600, 5500, 100);
      }
      Serial.println("Move to No");
    }
    else {
      Serial.print("What even is a ");
      Serial.println(serial_cmd);
    }
  }
}
