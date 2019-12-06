#ifndef Button_h
#define Button_h
#include "Arduino.h"
#include "TFT_eSPI.h"
#include "mpu9255_esp32.h"
#include "SPI.h"
#include "math.h"

class Button 
{
  private:
    uint32_t t_of_state_2;
    uint32_t t_of_button_change;
    uint32_t debounce_time;
    uint32_t long_press_time;
    uint8_t pin;
    uint8_t flag;
    bool button_pressed;
    uint8_t state;
    void read(); 
  public:
    Button(int p);
    int update();
};
#endif
