SKETCH    := software.ino
LIBRARIES := DmxReceiver
BAUDRATE  := 86400
TARGET    ?= ..arduino.avr.nano
BOARD     ?= arduino:avr:nano:cpu=atmega328old
ARDUINO   ?= arduino-cli
PICOCOM   ?= picocom
PORT      ?= /dev/ttyUSB0
LIB_DIR   ?= ~/Arduino/libraries

SRC = $(SKETCH) $(foreach lib,$(LIBRARIES),$(wildcard $(LIB_DIR)/$(lib)/*))

upload : $(TARGET).hex
	$(ARDUINO) upload -p $(PORT) --fqbn $(BOARD) .

compile : $(TARGET).hex

console :
	@echo "NOTE: Press Ctrl+A Ctrl+X to exit."
	$(PICOCOM) -b $(BAUDRATE) $(PORT)

$(TARGET).hex : $(SRC)
	$(ARDUINO) compile --fqbn $(BOARD) .

.PHONY : clean
clean :
	$(RM) $(TARGET).hex $(TARGET).elf
