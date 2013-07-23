//#include "IRremote.h"
#include "LowPower.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

// pin 7 - Serial clock out (SCLK) 9
// pin 6 - Serial data out (DIN) 8
// pin 5 - Data/Command select (D/C) 7
// pin 8 - LCD chip select (CS) 6
// pin 9 - LCD reset (RST) 5
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 8, 7, 6, 5);

#define PIN_DETECT 12
#define PIN_IR 3
#define REC_POWER 13
#define SCREEN_POWER 10
#define BUT1 2
#define BUT2 4

int x = 0;
boolean lastPulseSuccess = true;
boolean thisPulse = false;
boolean screenChecked = false;
//IRsend irsend;

void setPinsLow(){
  for (byte i = 0; i <= 13; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }
  pinMode(PIN_DETECT, INPUT);
  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
//  Serial.println("Pins set as Low outputs");
}

void turnOffScreen(){
  digitalWrite(SCREEN_POWER, LOW);
  for (byte i = 5; i <= 9; i++)
  {
    digitalWrite (i, LOW);  //     ditto
  }
}

boolean screenCheck(){
  // Enable pullup for open connection
  digitalWrite(BUT1, HIGH);
  // Test to see if button connection closed
  screenChecked=digitalRead(BUT1);
//  Serial.println("oops");
  if(screenChecked==LOW){
    // Configure LCD Screen and enable power to it.
//    Serial.println("checking screen");
    digitalWrite(SCREEN_POWER, HIGH);
    display.begin();
    display.clearDisplay();
    display.setTextColor(BLACK);
    display.setCursor(0,0);
    display.setTextSize(2);
    display.print("Passes: ");
    display.println(x);
    display.display();
    delay(5000);
    resetCheck();
    // Turn screen off again
//    turnOffScreen();
  }
  // Disable pullup resistor
  digitalWrite(BUT1, LOW);
  return screenChecked;
}

void resetCheck(){
  // Check if reset button is pressed
  digitalWrite(BUT2, HIGH);
  if(digitalRead(BUT2)==LOW){
    display.clearDisplay();
    display.setTextSize(1);
    display.println("Reset in: 3s");
    display.display();
    delay(1000); 
    display.clearDisplay();
    if(digitalRead(BUT2)==LOW){
      display.println("Reset in: 2s");
      display.display();
      delay(1000); 
      display.clearDisplay();
      if(digitalRead(BUT2)==LOW){
        display.println("Reset in: 1s");
        display.display();
        delay(1000); 
        display.clearDisplay();
        if(digitalRead(BUT2)==LOW){
          x=0;
          display.println("Reset success!");
          display.display();
          delay(1000);
        }
      }
    }
  }
  // Disable pullup resistor again & print current count
  digitalWrite(BUT2, LOW);
}

void enableIROut(int val){
  TIMSK2 = 0;
  const uint8_t pwmval = F_CPU / 2000 / (val); \
  TCCR2A = _BV(WGM20); \
  TCCR2B = _BV(WGM22) | _BV(CS20); \
  OCR2A = pwmval; \
  OCR2B = pwmval / 3; 
//  Serial.println("PWM Setup"); 
}

void enablePWM(){
//  Serial.println("Enabling PWM");
  digitalWrite (REC_POWER, HIGH);
  TCCR2A |= _BV(COM2B1);
  delayMicroseconds(500);
}

void disablePWM(){
//  Serial.println("Disabling PWM");
  TCCR2A &= ~(_BV(COM2B1));
//  delayMicroseconds(0);
  digitalWrite (PIN_IR, LOW);
  digitalWrite (REC_POWER, LOW);
}

void setup()
{
//  Serial.begin(9600);
  setPinsLow();
  
  ADCSRA &= ~(1<<ADEN); //Disable ADC
  ACSR = (1<<ACD); //Disable the analog comparator
  DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
  DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0
  enableIROut(38);
//  irsend.enableIROut(38);
}

void loop() {
//  Serial.print("On at: ");
//  Serial.print(millis());
  // Display current pass count on screen and check to see if reset required
  if(screenCheck()){
//    irsend.mark(0);
    enablePWM();
    thisPulse=digitalRead(PIN_DETECT);
//    delay(1000);
    disablePWM();
//    irsend.space(0);
//    Serial.print("Off at: ");
//    Serial.println(millis());
    display.clearDisplay();
    if (thisPulse){
      if(lastPulseSuccess){
        x++;
        lastPulseSuccess=false;
        display.print("Break: ");
//        Serial.print(millis());
//        Serial.print(" - Beam Broken: ");
        display.println(x);
//        Serial.println(" times");
       }
       else {
         display.println("Still broken");
       }
    }
    else {
      display.println("Received");
      lastPulseSuccess=true;
    }
    display.display();
  }
//  Serial.println(digitalRead(PIN_DETECT));
//  Serial.print("Off at: ");
//  Serial.println(x);
//  Serial.flush();
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
