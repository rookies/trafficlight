SKETCH    := software
LIBRARIES := DmxReceiver
BOARD     ?= arduino:avr:nano:cpu=atmega328old
ARDUINO   ?= arduino-cli
PORT      ?= /dev/ttyUSB0
LIB_DIR   ?= ~/Arduino/libraries

SRC=$(SKETCH).ino $(foreach lib,$(LIBRARIES),$(wildcard $(LIB_DIR)/$(lib)/*))

upload : ..arduino.avr.nano.hex
	$(ARDUINO) upload -p $(PORT) --fqbn $(BOARD) .

compile : ..arduino.avr.nano.hex

..arduino.avr.nano.hex : $(SRC)
	arduino-cli compile --fqbn $(BOARD) .

.PHONY : clean
clean :
	$(RM) ..arduino.avr.nano.hex ..arduino.avr.nano.elf
