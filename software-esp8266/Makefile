SKETCH    := software-esp8266.ino
LIBRARIES :=
BAUDRATE  := 115200
BOARD     ?= esp8266:esp8266:d1_mini
ARDUINO   ?= arduino-cli
PICOCOM   ?= picocom
PORT      ?= /dev/ttyUSB0
LIB_DIR   ?= ~/Arduino/libraries
TARGETEXT ?= bin

SRC    := $(SKETCH) \
          $(wildcard *.hh) \
          $(foreach lib,$(LIBRARIES),$(LIB_DIR)/$(lib) $(wildcard $(LIB_DIR)/$(lib)/*))
TARGET := ..$(subst $(noop) $(noop),.,$(filter-out cpu=%,$(subst :, ,$(BOARD))))

upload : $(TARGET).$(TARGETEXT)
	$(ARDUINO) upload -p $(PORT) --fqbn $(BOARD) .

compile : $(TARGET).$(TARGETEXT)

console :
	@echo "NOTE: Press Ctrl+A Ctrl+X to exit."
	$(PICOCOM) -b $(BAUDRATE) $(PORT)

$(TARGET).$(TARGETEXT) : $(SRC)
	$(ARDUINO) compile --fqbn $(BOARD) .

.PHONY : clean
clean :
	$(RM) $(TARGET).$(TARGETEXT) $(TARGET).elf
