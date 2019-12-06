// 
// Simple example showing how to set the Sleepy Pi to wake on button press
// and then power up the Raspberry Pi. To switch the RPi off press the button
// again. If the button is held dwon the Sleepy Pi will cut the power to the
// RPi regardless of any handshaking.
//

// **** INCLUDES *****
#include "SleepyPi.h"
#include <Time.h>
#include <LowPower.h>
#include <DS1374RTC.h>
#include <Wire.h>

// States
typedef enum {
  eWAIT = 0,
  eBUTTON_PRESSED,
  eBUTTON_HELD,
  eBUTTON_RELEASED
}eBUTTONSTATE;

typedef enum {
   ePI_OFF = 0,
   ePI_BOOTING,
   ePI_ON,
   ePI_SHUTTING_DOWN
}ePISTATE;

const int LED_PIN_ON = 13;
const int LED_PIN_OFF = 14;

volatile bool  buttonOnPressed = false;
volatile bool  buttonOffPressed = false;
eBUTTONSTATE   buttonOnState = eBUTTON_RELEASED;
eBUTTONSTATE   buttonOffState = eBUTTON_RELEASED;
ePISTATE       pi_state = ePI_OFF;
bool state = LOW;
unsigned long  time, timePress;

void button_on_isr()
{
    // A handler for the Button interrupt.
    buttonOnPressed = true;
}

void button_off_isr()
{
    // A handler for the Button interrupt.
    buttonOffPressed = true;
}

void setup()
{
  SleepyPi.simulationMode = false;  // Don't actually shutdown
  
  // Configure "Standard" LED pin
  pinMode(LED_PIN_ON, OUTPUT);    
  digitalWrite(LED_PIN_ON,LOW);    // Switch off LED
  pinMode(LED_PIN_OFF, OUTPUT);
  digitalWrite(LED_PIN_OFF,LOW);

  SleepyPi.enablePiPower(false);  
  SleepyPi.enableExtPower(false);
  
   // Allow wake up triggered by button press
  attachInterrupt(1, button_on_isr, LOW);    // button pin 
  attachInterrupt(2, button_off_isr, HIGH);
  
  // initialize serial communication: In Arduino IDE use "Serial Monitor"
  //Serial.begin(9600);
  
  // SleepyPi.simulationMode = true;  // Don't actually shutdown
  
}

void loop() 
{
    bool pi_running;
  
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake button is pressed.
    // Once button is pressed stay awake.
    pi_running = SleepyPi.checkPiStatus(true);  // Cut Power if we detect Pi not running
    if(pi_running == false){ 
       SleepyPi.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    }
    
    // Button State changed
    if(buttonOnPressed == true){
        detachInterrupt(1);
        buttonOnPressed = false;
        switch(buttonOnState) { 
          case eBUTTON_RELEASED:
              // Button pressed           
              timePress = millis();             
              pi_running = SleepyPi.checkPiStatus(false);
              if (!pi_running) {  
                  // Switch on the Pi
                  SleepyPi.enablePiPower(true);
                  SleepyPi.enableExtPower(true);   
              }          
              buttonOnState = eBUTTON_PRESSED;
              digitalWrite(LED_PIN_ON,HIGH);           
              attachInterrupt(1, button_on_isr, HIGH);                    
              break;
          case eBUTTON_PRESSED:
              // Button Released
              buttonOnState = eBUTTON_RELEASED;
              digitalWrite(LED_PIN_ON,LOW);            
              attachInterrupt(1, button_on_isr, LOW);    // button pin       
              break;
           default:
              break;
        }                
    }

    if (buttonOffPressed) {
      detachInterrupt(2);
      buttonOffPressed = false;
      switch (buttonOffState) {
        case eBUTTON_RELEASED:
          // button pressed
          timePress = millis();
          pi_running = SleepyPi.checkPiStatus(true);
          if (pi_running) {
            // switch off the pi
            SleepyPi.enablePiPower(false);
            SleepyPi.enableExtPower(false);
          }
          buttonOffState = eBUTTON_PRESSED;
          digitalWrite(LED_PIN_OFF, HIGH);
          attachInterrupt(2, button_off_isr, HIGH);
          break;
        case eBUTTON_PRESSED:
          // button released
          unsigned long buttonOffTime;
          buttonOffState = eBUTTON_RELEASED;
          digitalWrite(LED_PIN_OFF, LOW);
          attachInterrupt(2, button_off_isr, LOW);
          break;
        default:
          break;
      }
    }
}
