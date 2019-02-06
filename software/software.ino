#include <avr/pgmspace.h>
#include <DmxReceiver.h>

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

DmxReceiver dmx(pinDmx, 3);

byte flashCounter = 0;
byte flashingState = 0;
unsigned long flashTime = 0;
const unsigned long flashDuration = 100;
bool buttonPressed = false;

void setLights(byte red, byte yellow, byte green) {
  analogWrite(pinRed, pgm_read_byte(expTable + red));
  analogWrite(pinYellow, pgm_read_byte(expTable + yellow));
  analogWrite(pinGreen, pgm_read_byte(expTable + green));
}

void flash() {
  if (flashCounter == 0) return;
  if (flashingState == 0) {
    setLights(0, 0, 0);
    flashingState = 1;
    flashTime = millis();
  } else if (flashingState == 1 && (millis() - flashTime) > flashDuration) {
    setLights(255, 255, 255);
    flashingState = 2;
    flashTime = millis();
  } else if (flashingState == 2 && (millis() - flashTime) > flashDuration) {
    setLights(0, 0, 0);
    flashingState = 3;
    flashTime = millis();
  } else if (flashingState == 3 && (millis() - flashTime) > flashDuration) {
    flashingState = 0;
    flashCounter--;
  }
}

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
  flashCounter = 3;
}

void loop() {
  /* Flash the lights: */
  if (flashCounter != 0) {
    flash();
    return;
  }
  /* Check if button is pressed: */
  byte button = digitalRead(pinButton);
  if (button == 1 && buttonPressed) {
    buttonPressed = false;
  } else if (button == 0 && !buttonPressed) {
    Serial.println("Button pressed!");
    flashCounter = 1;
    buttonPressed = true;
  };
  /* Receive DMX data: */
  if (dmx.poll()) {
    setLights(dmx.getValue(0), dmx.getValue(1), dmx.getValue(2));
  }
}
