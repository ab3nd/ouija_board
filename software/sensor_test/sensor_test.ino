
#define ENABLE 2
#define S0 3
#define S1 4
#define S2 5
#define SENSE A0

int readings[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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
    readings[ii] = analogRead(SENSE);
  }

  for(byte ii = 0; ii < 8; ii++)
  {
    Serial.print(readings[ii]);
    if(ii < 7)
    {
      Serial.print(",");
    }
  }
  Serial.println();
}
