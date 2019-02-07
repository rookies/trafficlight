#include <DmxReceiver.h>
#include "Pins.hh"
#include "Functions.hh"
#include "ModeSwitcher.hh"
#include "Flash.hh"

/* Modes of the traffic light: */
enum Mode : byte {
  Dmx = 0,
  Random = 1
};
ModeSwitcher<Mode,2> mode;
/* State of the button: */
bool buttonPressed = false;
/* To receive DMX data: */
DmxReceiver dmx(pinDmx, 3);
/* To flash the lights: */
Flash flash(500, 200, [](bool turnOn) {
 if (turnOn) {
   setLights(255, 255, 255);
 } else {
    setLights(0, 0, 0);
 }
});

void setup() {
  /* Init serial port: */
  Serial.begin(86400);
  Serial.println(F("Hello, I'm trafficlight v0.1!"));
  /* Define lights as output: */
  pinMode(pinRed, OUTPUT);
  pinMode(pinYellow, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  /* Define button as input with pullup resistor: */
  pinMode(pinButton, INPUT_PULLUP);
  /* Flash once on startup: */
  flash.counter = 1;
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
  /* If lights are not flashing, set them according
     to the mode we are currently in: */
  switch (mode.mode) {
    case Mode::Dmx:
      /* DMX mode */
      if (dmx.poll()) {
        setLights(dmx.getValue(0), dmx.getValue(1), dmx.getValue(2));
      }
      break;
    case Mode::Random:
      /* Random mode */
      setLights(127, 127, 127);
      /* TODO */
      break;
    default:
      setLights(0, 0, 0);
  }
}
