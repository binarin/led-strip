#ifndef __LED_STRIP_HPP__
#define __LED_STRIP_HPP__

#include <led_strip/led_strip.h>

namespace LedStrip {
  void start();
  bool isOn();
  void turnOn();
  void turnOff();
  void setColor(uint8_t r, uint8_t g, uint8_t b);
}


#endif // __LED_STRIP_HPP__
