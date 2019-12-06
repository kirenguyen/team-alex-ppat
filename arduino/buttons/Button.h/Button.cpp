#include "Arduino.h"
#include "TFT_eSPI.h"
#include "mpu9255_esp32.h"
#include "SPI.h"
#include "math.h"
#include "Button.h"

/*
* Constructor for a button
*/
Button::Button(int p) {
  // based on the button pin number, initialize the Button
  flag = 0;
  state = 0;
  pin = p;
  // the time to decide whether or not something is a long press 
  t_of_state_2 = millis(); 
  // keeping track of the last time the button has been pressed 
  t_of_button_change = millis();
  // set a debounce time to prevent debouncing due to the mechanics of the button
  debounce_time = 10;
  long_press_time = 1000;
  button_pressed = 0;
}

/*
*based on the reading of the button, update the state and flag (ie/ short or long press)
*/
int Button::update() {
  read();
  // for some reason, without resetting these values here, these two variables were
  // automatically reset to 0 so we are reinitializing the values 
  long_press_time = 1000;
  debounce_time = 10;
  // flag indicates whether or not its a long press or short press 
  flag = 0;
  // state 0 = rest state: the button is unpressed and has been unpressed for a time > debounce_time
  if (state == 0) {
    if (button_pressed) {
      state = 1;
      t_of_button_change = millis();
    }
  // state 1 = start debounce state: the button has just been pressed and this is a tentative pressed state to prevent debounce
  } else if (state == 1) {
    if (!button_pressed) {
      state = 0;
      t_of_button_change = millis();
    }
    else {
      // we will move to a presistent pressed state after debounce_time elapses
      if (button_pressed && millis() - t_of_button_change >= debounce_time) {
        state = 2;
        t_of_state_2 = millis();
      }
      else if (button_pressed && millis() - t_of_button_change < debounce_time) {
        state = 1;
      }
    }
  // state 2 = short press state: the button has been pressed long enoungh to be a short press but not long enough to be a long press
  } else if (state == 2) {
    if (button_pressed && millis() - t_of_state_2 >= long_press_time) {
      state = 3;
    }
    else if (!button_pressed) {
      state = 4;
      t_of_button_change = millis();
    }
  // state 4 = long press state: the button has been pressed long enough to be a long press
  } else if (state == 3) {
    if (!button_pressed) {
      state = 4;
      t_of_button_change = millis();
    }
  // state 4 = ending debounce state: the button has just been released from short/long press. this is a tentative unpressed state
  } else if (state == 4) {
    // return to state 2 if the button is repressed based on the previous state
    if (button_pressed && millis() - t_of_state_2 < long_press_time) {
      flag = 0;
      state = 2;
      t_of_button_change = millis();
    }
    // return to state 3 if the button is repressed baed on the previous state
    else if (button_pressed && millis() - t_of_state_2 >= long_press_time) {
      flag = 0;
      state = 3;
      t_of_button_change = millis();
    }
    // depending on the time, flag it as 1 (short press) or 2 (long press)
    else if (!button_pressed && millis() - t_of_button_change >= debounce_time) {
      if (millis() - t_of_state_2 < long_press_time) {
        flag = 1;
      }
      else if (millis() - t_of_state_2 >= long_press_time) {
        flag = 2;
      }
      state = 0;
    }
  }
//  Serial.println(flag);
  return flag;
}

/*
* read the button from digitalRead
*/
void Button::read() {
  // based on the digitalRead signal, decide the state of the button
  uint8_t button_state = digitalRead(pin);
  button_pressed = !button_state;
}
