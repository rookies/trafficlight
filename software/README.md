Software
========

This is the software for the traffic light.

It uses my [DmxReceiver](https://github.com/rookies/DmxReceiver) library, so
you need to have it installed.
You can compile and upload this sketch using the
[Arduino IDE](https://www.arduino.cc/en/main/software).

However, if you like to work on the command line, I provided a Makefile that
allows you to do so. It uses
[arduino-cli](https://github.com/arduino/arduino-cli)
to compile and upload the sketch and
[picocom](https://github.com/npat-efault/picocom)
to communicate with the serial port.

To compile and upload, just execute `make`.
If you only want to compile, use `make compile`.
To delete the created elf- & hex-files, type `make clean`.
To start a serial console, use `make console`.

By default, the Makefile compiles for an Arduino Nano with an ATmega328P and
the old bootloader (`arduino:avr:nano:cpu=atmega328old`) and tries to upload
to `/dev/ttyUSB0`.

You can either change these settings directly in the Makefile or add options
to the call to make, e.g.
```bash
make BOARD=arduino:avr:uno PORT=/dev/ttyUSB2
```
