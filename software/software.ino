#include <avr/pgmspace.h>
#include <DmxReceiver.h>
#include "ModeSwitcher.h"
#include "Flash.h"

/* Pin Configuration: */
const byte pinRed = 10;
const byte pinYellow = 11;
const byte pinGreen = 9;
const byte pinDmx = 6;
const byte pinButton = A1;

/**
 * A pre-computed table of function values for
 *   f(x) = a * e^(x/b) with a = f(0) and b = 255/ln(255/a)
 *
 * Generated with python:
 *   import math
 *   for i in range(256):
 *     print("%3d" % int(0.99*math.exp(i/255*math.log(255/0.99))), end=", ")
*/
const byte expTable[256] PROGMEM = {
    0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   3,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,
    5,   5,   5,   5,   5,   5,   5,   5,   6,   6,   6,   6,   6,   6,   6,
    7,   7,   7,   7,   7,   7,   8,   8,   8,   8,   8,   8,   9,   9,   9,
    9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  12,  13,
   13,  13,  14,  14,  14,  15,  15,  15,  16,  16,  16,  17,  17,  17,  18,
   18,  19,  19,  19,  20,  20,  21,  21,  22,  22,  23,  23,  24,  24,  25,
   25,  26,  27,  27,  28,  28,  29,  30,  30,  31,  32,  32,  33,  34,  35,
   35,  36,  37,  38,  39,  40,  40,  41,  42,  43,  44,  45,  46,  47,  48,
   49,  50,  52,  53,  54,  55,  56,  58,  59,  60,  61,  63,  64,  66,  67,
   69,  70,  72,  73,  75,  77,  78,  80,  82,  84,  85,  87,  89,  91,  93,
   95,  97,  99, 102, 104, 106, 109, 111, 113, 116, 119, 121, 124, 127, 129,
  132, 135, 138, 141, 144, 147, 151, 154, 157, 161, 164, 168, 172, 176, 179,
  183, 188, 192, 196, 200, 205, 209, 214, 218, 223, 228, 233, 238, 244, 249,
  255
};

enum Mode : byte {
  Dmx = 0,
  Chaser = 1,
  Test2 = 2,
  Test3 = 3
};

void setLights(byte red, byte yellow, byte green) {
  analogWrite(pinRed, pgm_read_byte(expTable + red));
  analogWrite(pinYellow, pgm_read_byte(expTable + yellow));
  analogWrite(pinGreen, pgm_read_byte(expTable + green));
}

DmxReceiver dmx(pinDmx, 3);
ModeSwitcher<Mode,4> mode;
Flash flash(500, 200, [](bool turnOn) {
 if (turnOn) {
   setLights(255, 255, 255);
 } else {
    setLights(0, 0, 0);
 }
});

bool buttonPressed = false;

void setup() {
  /* Init serial port: */
  Serial.begin(86400);
  Serial.println(F("Hello!"));
  /* Define lights as output: */
  pinMode(pinRed, OUTPUT);
  pinMode(pinYellow, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  /* Define button as input with pullup resistor: */
  pinMode(pinButton, INPUT_PULLUP);
  /* Flash three times on startup: */
  flash.counter = 3;
}

void loop() {
  /* Check if button is pressed: */
  byte button = digitalRead(pinButton);
  if (button == 1 && buttonPressed) {
    buttonPressed = false;
  } else if (button == 0 && !buttonPressed) {
    Serial.println("Button pressed!");
    buttonPressed = true;
    mode.next();
    flash.reset();
    flash.counter = mode.mode + 1;
  };
  /* Flash the lights: */
  if (flash.run()) return;
  /* Receive DMX data: */
  if (dmx.poll()) {
    setLights(dmx.getValue(0), dmx.getValue(1), dmx.getValue(2));
  }
}
