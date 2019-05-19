/* Control software for the Ouija Board. The board is an Arduino Nano with the old bootloader. 
 * By Ab3nd.
 */

#include <Servo.h>
 
#define LIMIT_X 9
#define LIMIT_Y 10

#define ENABLE 8
#define DIR_X 5
#define DIR_Y 6
#define STEP_X 2
#define STEP_Y 3

Servo mag_lift;

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

void move_home(){
  //Enable stepper drivers
  digitalWrite(ENABLE, LOW);

  //Home x axis first
  digitalWrite(DIR_X, LOW);
  digitalWrite(STEP_X, LOW);
  while(digitalRead(LIMIT_X) == 1) {
    digitalWrite(STEP_X,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(STEP_X,LOW); 
    delayMicroseconds(500); 
  }

  //Home Y axis, it uses opposite DIR_Y setting
  digitalWrite(DIR_Y, HIGH);
  digitalWrite(STEP_Y, LOW);
  while(digitalRead(LIMIT_Y) == 1){
    digitalWrite(STEP_Y,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(STEP_Y,LOW); 
    delayMicroseconds(500); 
  }

  //Enable stepper drivers
  digitalWrite(ENABLE, HIGH);

}

int lx = 0;
int ly = 0;

void loop() {
  lx = digitalRead(LIMIT_X);
  ly = digitalRead(LIMIT_Y);

  Serial.print(lx);
  Serial.print(", ");
  Serial.print(ly);
  Serial.println();

}
