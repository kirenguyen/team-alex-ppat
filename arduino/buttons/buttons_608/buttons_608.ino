#include "SleepyPi.h"
#include <Time.h>
#include <LowPower.h>
#include <DS1374RTC.h>
#include <Wire.h>
#include "Button.h"

#define ON 1
#define OFF 2

const int BUTTON_PIN_ON = 13;
const int BUTTON_PIN_OFF = 14;

Button button_on(BUTTON_PIN_ON);
Button button_off(BUTTON_PIN_OFF);

void setup() {
  pinMode(BUTTON_PIN_ON, INPUT_PULLUP);
  pinMode(BUTTON_PIN_OFF, INPUT_PULLUP);
}

void loop() {
  uint8_t input_on = digitalRead(BUTTON_PIN_ON);
  uint8_t input_off = digitalRead(BUTTON_PIN_OFF);
  bool pi_running = SleepyPi.checkPiStatus(true);
  if (pi_running) {
    int bv_off = button_off.update();
    if (bv_off == 1 || bv_off == 2) {
      SleepyPi.enablePiPower(false);
      SleepyPi.enableExtPower(false);
    }
  } else {
    int bv_on = button_on.update();
    if (bv_on == 1 || bv_on == 2) {
      SleepyPi.enablePiPower(true);
      SleepyPi.enableExtPower(true);
    }
  }
}
