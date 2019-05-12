#include <math.h>

#define ENABLE_SENSE 4
#define S0 7
#define S1 11
#define S2 12
#define SENSE A0

#define LIMIT_X 9
#define LIMIT_Y 10

#define ENABLE_MOTOR 8
#define DIR_X 5
#define DIR_Y 6
#define STEP_X 2
#define STEP_Y 3

/* Lookup table for the sensors' component calculations. Since the 
 * angles of all the sensors are fixed, this table was precomputed. 
 */
float sin_lut[8] = {-0.48717, -0.99902, -0.56244, 0.4081, 0.9912, 0.63331, -0.32582, -0.97563};
float cos_lut[8] = {-0.8733, -0.04423, 0.82684, 0.91294, 0.13234, -0.7739, -0.94543, -0.21941};

int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int cal_readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* Map counts on the address lines of the mux to indices for readings
 * so that the readings are sequential around the ring */
int addr_map[8] = {0, 1, 2, 7, 3, 6, 4, 5};

void move_home(){
  //Enable stepper drivers
  digitalWrite(ENABLE_MOTOR, LOW);

  //Home x axis first
  digitalWrite(DIR_X, LOW);
  digitalWrite(STEP_X, LOW);
  while(digitalRead(LIMIT_X) == 0) {
    digitalWrite(STEP_X,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(STEP_X,LOW); 
    delayMicroseconds(500); 
  }

  //Home Y axis, it uses opposite DIR_Y setting
  digitalWrite(DIR_X, LOW);
  digitalWrite(STEP_Y, LOW);
  while(digitalRead(LIMIT_Y) == 0){
    digitalWrite(STEP_Y,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(STEP_Y,LOW); 
    delayMicroseconds(500); 
  }

  //Enable stepper drivers
  digitalWrite(ENABLE_MOTOR, HIGH);
}

void setup() {

  //Limit switches
  pinMode(LIMIT_X, INPUT);
  pinMode(LIMIT_Y, INPUT);

  //Set up the stepper control
  pinMode(ENABLE_MOTOR, OUTPUT);
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
  for(int count = 0; count < 10; count++){
    for(byte ii = 0; ii < 8; ii++)
    {
      //Disable the switch
      digitalWrite(ENABLE_SENSE, HIGH);
      //Set the address bits
      (ii & B00000001)? digitalWrite(S0, HIGH): digitalWrite(S0, LOW);
      (ii & B00000010)? digitalWrite(S1, HIGH): digitalWrite(S1, LOW);
      (ii & B00000100)? digitalWrite(S2, HIGH): digitalWrite(S2, LOW);
      digitalWrite(ENABLE_SENSE, LOW);
      cal_readings[addr_map[ii]] += analogRead(SENSE);
    }
  }

  //Get their average
  for(int idx = 0; idx < 8; idx++)
  {
    //Each reading should be around 512 with no magnet, so these
    //should all be around 5120, and so not have an underflow 
    //dividing by 10
    cal_readings[idx] = cal_readings[idx]/10;
  }

  //Home the axes
  move_home();
    
  //For debugging
  Serial.begin(9600);
}

void loop() {
  for(byte ii = 0; ii < 8; ii++)
  {
    //Disable the switch
    digitalWrite(ENABLE_SENSE, HIGH);
    //Set the address bits
    (ii & B00000001)? digitalWrite(S0, HIGH): digitalWrite(S0, LOW);
    (ii & B00000010)? digitalWrite(S1, HIGH): digitalWrite(S1, LOW);
    (ii & B00000100)? digitalWrite(S2, HIGH): digitalWrite(S2, LOW);
    digitalWrite(ENABLE_SENSE, LOW);
    readings[addr_map[ii]] = analogRead(SENSE)-cal_readings[addr_map[ii]];
  }

//  //Print readings for debug
//  for(byte ii = 0; ii < 8; ii++)
//  {
//    Serial.print(readings[ii]);
//    if(ii < 7)
//    {
//      Serial.print(",");
//    }
//  }
//  Serial.println();

  //Calculate the desired angle and speed
  float sumX = 0;
  float sumY = 0;

  //First, vector sums of all the components
  for(byte ii = 0; ii < 8; ii++)
  {
    sumX += float(readings[ii]) * cos_lut[ii];
    sumY += float(readings[ii]) * sin_lut[ii];
  }

  //Might not need polar conversion, since the system is in X and Y
  Serial.print(sumX);
  Serial.print(",");
  Serial.println(sumY);

  //Set the motor directions (These may need reversing)
  sumX > 0 ? digitalWrite(DIR_X, HIGH): digitalWrite(DIR_X, LOW);
  sumY > 0 ? digitalWrite(DIR_Y, HIGH): digitalWrite(DIR_Y, LOW);
  
//  //Do some steps
//  if(sumX > 10 || sumY > 10){
//    // The change is big enough that we should react to it
//    int ratio = 0;
//    if(sumX > sumY){
//      // Do one step in the Y direction and the ratio steps in X
//      ratio = int(abs(sumX/sumY));
//      Serial.print("X larger: ");
//      Serial.println(ratio);
//
//      // Y step
//      if(digitalRead(LIMIT_Y) == 0){
//        digitalWrite(STEP_Y,HIGH); 
//        delayMicroseconds(500); 
//        digitalWrite(STEP_Y,LOW); 
//        delayMicroseconds(500); 
//      }
//
//      // X steps
//      while(ratio > 0 && (digitalRead(LIMIT_X) == 0)){
//        ratio--;
//        digitalWrite(STEP_X,HIGH); 
//        delayMicroseconds(500); 
//        digitalWrite(STEP_X,LOW); 
//        delayMicroseconds(500);                 
//      }
//    }
//    else{
//      // Do one step in the X direction and the ratio steps in Y
//      ratio = int(abs(sumY/sumX));
//      Serial.print("Y larger: ");
//      Serial.println(ratio);
//      
//      // Single X step
//      if(digitalRead(LIMIT_X) == 0){
//        digitalWrite(STEP_X,HIGH); 
//        delayMicroseconds(500); 
//        digitalWrite(STEP_X,LOW); 
//        delayMicroseconds(500); 
//      }
//      // Y step for ratio
//      while(ratio > 0 && (digitalRead(LIMIT_Y) == 0)){
//        ratio--;
//        digitalWrite(STEP_Y,HIGH); 
//        delayMicroseconds(500); 
//        digitalWrite(STEP_Y,LOW); 
//        delayMicroseconds(500);                 
//      }
//    }
//  }

//  //Convert to polar
//  float r = sqrt(pow(sumX, 2) + pow(sumY, 2));
//  float theta = atan2(sumY, sumX);
//  //Serial.print(r);
//  //Serial.print(",");
//  Serial.println(theta);

  
  
}
