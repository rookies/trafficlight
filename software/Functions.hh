#ifndef Functions_hh
#define Functions_hh

#include "ExpTable.hh"

void setLights(byte red, byte yellow, byte green) {
  analogWrite(pinRed, pgm_read_byte(expTable + red));
  analogWrite(pinYellow, pgm_read_byte(expTable + yellow));
  analogWrite(pinGreen, pgm_read_byte(expTable + green));
}

#endif
