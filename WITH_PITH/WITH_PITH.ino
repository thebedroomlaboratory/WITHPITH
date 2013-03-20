//#include "IRremote.h"
#include "LowPower.h"

#define PIN_IR 3
#define PIN_DETECT 2
#define REC_POWER 4

int x = 0;
boolean lastPulseSuccess = true;
boolean thisPulse = false;
//IRsend irsend;

void setPinsLow(){
  for (byte i = 0; i <= 13; i++)
  {
    pinMode (i, OUTPUT);    // changed as per below
    digitalWrite (i, LOW);  //     ditto
  }
  pinMode(PIN_DETECT, INPUT);
//  Serial.println("Pins set as Low outputs");
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
  TCCR2A |= _BV(COM2B1);
  delayMicroseconds(350);
}

void disablePWM(){
//  Serial.println("Disabling PWM");
  TCCR2A &= ~(_BV(COM2B1));
//  delayMicroseconds(0);
  digitalWrite (REC_POWER, LOW);
  digitalWrite (PIN_IR, LOW);
}

void setup()
{
  Serial.begin(9600);
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
  Serial.print(millis());
  digitalWrite (REC_POWER, HIGH);
//  irsend.mark(0);
  enablePWM();
  thisPulse=digitalRead(PIN_DETECT);
//  delay(1000);
  disablePWM();
//  irsend.space(0);
//  Serial.print("Off at: ");
//  Serial.println(millis());
  if (thisPulse){
    if(lastPulseSuccess){
      x++;
      lastPulseSuccess=false;
//      Serial.print(millis());
//      Serial.print(" - Beam Broken: ");
//      Serial.print(x);
//      Serial.println(" times");
    }
//    else {
//      Serial.println("Still broken");
//    }
  }
  else {
//    Serial.println("Received Correctly");
    lastPulseSuccess=true;
  }
//  Serial.println(digitalRead(PIN_DETECT));
//  Serial.print("Off at: ");
  Serial.println(x);
  Serial.flush();
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
}
