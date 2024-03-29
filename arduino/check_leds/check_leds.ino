/*

 Runs through each letter of the alphabet, illuminating each in turn. 

 */

 
//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 6;
////Pin connected to DS of 74HC595
int dataPin = 4;

int outputEnablePin = 3;


int numOfRegisters = 4;
byte* registerState;

long effectId = 0;
long prevEffect = 0;
long effectRepeat = 0;
long effectSpeed = 30;

void setup() {
  //Initialize array
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  setBrightness(25);
}

void loop() {
  for (int ii=6;ii<32;ii++)
  {
    regWrite(ii,HIGH);
    delay(500);
    regWrite(ii,LOW);
  }

  
}

void effectA(int speed){
  for (int i = 0; i < 16; i++){
    for (int k = i; k < 16; k++){
      regWrite(k, HIGH);
      delay(speed);
      regWrite(k, LOW);
    }

    //regWrite(i, HIGH);
  }
}

void effectB(int speed){
  for (int i = 15; i >= 0; i--){
    for (int k = 0; k < i; k++){
      regWrite(k, HIGH);
      delay(speed);
      regWrite(k, LOW);
    }

    regWrite(i, HIGH);
  }
}

void effectC(int speed){
  int prevI = 0;
  for (int i = 0; i < 16; i++){
    regWrite(prevI, LOW);
    regWrite(i, HIGH);
    prevI = i;

    delay(speed);
  }

  for (int i = 15; i >= 0; i--){
    regWrite(prevI, LOW);
    regWrite(i, HIGH);
    prevI = i;

    delay(speed);
  }
}

void effectD(int speed){
  for (int i = 0; i < 8; i++){
    for (int k = i; k < 8; k++)
    {
      regWrite(k, HIGH);
      regWrite(15 - k, HIGH);
      delay(speed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }

    regWrite(i, HIGH);
    regWrite(15 - i, HIGH);
  }
}

void effectE(int speed){
  for (int i = 7; i >= 0; i--){
    for (int k = 0; k <= i; k++)
    {
      regWrite(k, HIGH);
      regWrite(15 - k, HIGH);
      delay(speed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }

    regWrite(i, HIGH);
    regWrite(15 - i, HIGH);
  }
}

void regWrite(int pin, bool state){
  /* https://github.com/janisrove/Arduino-74HC595-shift-registers */
  //Determines register
  int reg = pin / 8;
  //Determines pin for actual register
  int actualPin = pin - (8 * reg);

  //Begin session
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < numOfRegisters; i++){
    //Get actual states for register
    byte* states = &registerState[i];

    //Update state
    if (i == reg){
      bitWrite(*states, actualPin, state);
    }

    //Write
    shiftOut(dataPin, clockPin, LSBFIRST, *states);
  }

  //End session
  digitalWrite(latchPin, HIGH);
}

/* https://learn.adafruit.com/adafruit-arduino-lesson-4-eight-leds/brightness-control */
void setBrightness(byte brightness) // 0 to 255
{
  analogWrite(outputEnablePin, 255-brightness);
}
