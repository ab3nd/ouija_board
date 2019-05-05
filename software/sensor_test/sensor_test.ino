#include <math.h>

#define ENABLE 2
#define S0 3
#define S1 4
#define S2 5
#define SENSE A0

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

void setup() {
  pinMode(ENABLE, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(SENSE, INPUT);

  digitalWrite(ENABLE, HIGH);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);

  //Accumulate 10 samples
  for(int count = 0; count < 10; count++){
    for(byte ii = 0; ii < 8; ii++)
    {
      //Disable the switch
      digitalWrite(ENABLE, HIGH);
      //Set the address bits
      (ii & B00000001)? digitalWrite(S0, HIGH): digitalWrite(S0, LOW);
      (ii & B00000010)? digitalWrite(S1, HIGH): digitalWrite(S1, LOW);
      (ii & B00000100)? digitalWrite(S2, HIGH): digitalWrite(S2, LOW);
      digitalWrite(ENABLE, LOW);
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
  
  //For debugging
  Serial.begin(9600);
}

void loop() {
  for(byte ii = 0; ii < 8; ii++)
  {
    //Disable the switch
    digitalWrite(ENABLE, HIGH);
    //Set the address bits
    (ii & B00000001)? digitalWrite(S0, HIGH): digitalWrite(S0, LOW);
    (ii & B00000010)? digitalWrite(S1, HIGH): digitalWrite(S1, LOW);
    (ii & B00000100)? digitalWrite(S2, HIGH): digitalWrite(S2, LOW);
    digitalWrite(ENABLE, LOW);
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

//  Serial.print(sumX);
//  Serial.print(",");
//  Serial.println(sumY);

  //Might not need polar conversion, since the system is in X and Y
  Serial.print(sumX);
  Serial.print(",");
  Serial.println(sumY);

//  //Convert to polar
//  float r = sqrt(pow(sumX, 2) + pow(sumY, 2));
//  float theta = atan2(sumY, sumX);
//  //Serial.print(r);
//  //Serial.print(",");
//  Serial.println(theta);
  
}
