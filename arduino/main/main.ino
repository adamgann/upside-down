/*

Play the main sequence on the wall. 

Adam Gannon - October 2019. 

 */
#include <avr/sleep.h>

// The first LED (A) is at register value 6
// The last LED (Z) is at register value 32
const int REG_START = 6;
const int REG_STOP = 32;


// The external button
const int buttonPin = 2;     // the number of the pushbutton pin
// The onboard LED for status
const int ledPin =  13;      // the number of the LED pin
//Pin connected to ST_CP of 74HC595
int latchPin = 5;
//Pin connected to SH_CP of 74HC595
int clockPin = 6;
////Pin connected to DS of 74HC595
int dataPin = 4;
// Pin connected to OE of 74HC595
int outputEnablePin = 3;

// Globals for LED registers 
int numOfRegisters = 4;
byte* registerState;


void setup() {

  // Seed our RNG by reading noise
  randomSeed(analogRead(0));
  
  //Initialize array
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(outputEnablePin, OUTPUT);

  // LEDs can be *really* bright, particularly at a dark party. Tone it down a little
  setBrightness(125);

  // initialize the onboard LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  // Start with nothing displayed on the LEDs
  clear_reg();
  write_all(LOW);
  
}

void loop() 
{

  // Wait on button press to start sequence.
  // Use the onboard LED for status
  wait_on_button();
  digitalWrite(ledPin,HIGH);
  run_once();
  digitalWrite(ledPin,LOW);
  
 

}

// Blocking call that waits until the state of the button changes 
void wait_on_button()
{
  EIFR = _BV (INTF0);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(buttonPin),wake_up,CHANGE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu(); //Stays here until interrupt is fired
}
void wake_up()
{
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(buttonPin));
}


// Run the whole sequence once
void run_once()
{
  
  // "Okay. Okay baby talk to me"
  delay(5000);
  int right_delay[] = {5000,2000,1200,1500,1500};
  int here_delay[] = {2000,1000,1000,1000};
  flash_phrase("RIGHT",right_delay);
  flash_phrase("HERE",here_delay);

  // "Right here? I don't know what that means?
  // Lights are all off during this
  delay(13000);
  
  // "What should I do?"
  int run_delay[] = {2000,3000,3500};
  flash_phrase("RUN",run_delay);
  delay(200);

  // Oh shit!
  flash_all(15000);
}


// The demogorgon sequence. Flash a random chunk of LEDs for flash_time
// Randomize the time on and off between flashes. 
void flash_all(int flash_time)
{
  unsigned long start_time = millis();
  while( (millis() - start_time) < flash_time )
  {
    long rand_on_time = random(40,80);
    long rand_off_time = random(40,80);
    
    rand_on();
    delay(rand_on_time);
    write_all(LOW);
    clear_reg();
    delay(rand_off_time);
  }
  
}

void rand_on()
{
  // Number of simultaneous LEDs that are on
  long rand_leds = random(4,26); 

  for (int iled=0;iled<rand_leds;iled++)
  {
    long rand_letter = random(REG_START,REG_STOP+1); //Register values of A-Z
    regWrite(rand_letter,HIGH);    
  }

}


// Clear the register state vector
void clear_reg()
{
  for (int ii=0;ii<numOfRegisters;ii++)
  {
    registerState[ii] = 0;
  }
}


// Letters flash one after another, without pause.
// Each letter is up for about 2 seconds
void flash_phrase(String phrase, int delay_vec[])
{
  for (int ii=0;ii<phrase.length();ii++)
  {
    write_letter(phrase[ii],delay_vec[ii]);
  }  
}



// Flash a single letter for delay_val milliseconds
// This function accepts capital letters only (65-90)
void write_letter(char letter, int delay_val)
{
    // ALPHA_START (6) is the start of the register.
    // 65 is the ASCII value of 'A', the first letter
    int reg_val = letter - 65 + REG_START;

    // Turn on and hold
    regWrite(reg_val,HIGH);
    delay(delay_val);
    regWrite(reg_val,LOW);
}

void write_all(bool state)
{
  byte states;
  if (state == LOW)
  {
    states = 0;
  }
  else
  {
    states = 255;
  }
  
  digitalWrite(latchPin, LOW);
  for (int i = 0; i < numOfRegisters; i++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, states);
  }
  digitalWrite(latchPin, HIGH);
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
