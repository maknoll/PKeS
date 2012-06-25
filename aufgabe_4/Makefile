TARGET  = main
CTRL    = at90can128
INCLUDE = ../include
STK	= avr911

CC      = avr-gcc
OC      = avr-objcopy
LDFLAGS = -j .text -j .data

AVRDUDE = avrdude
AVRDUDE_FLAGS = -P/dev/cu.SLAB_USBtoUART
AVRDUDE_FLAGS += -b 19200 
AVRDUDE_FLAGS += -c $(STK)
AVRDUDE_FLAGS += -p $(CTRL)
AVRDUDE_FLAGS += -U f:w:$(TARGET).hex:a

.PHONY: all

all: 
	@echo "compiling"
	$(CC) -c -o $(TARGET).o -g -O2 -Wall -I $(INCLUDE) -mmcu=$(CTRL) $(TARGET).c
	@echo "linking"
	@$(CC) -g -Wall -O2 -mmcu=$(CTRL) -o $(TARGET).elf $(TARGET).o
	@echo "generate properly output format"
	@$(OC) $(LDFLAGS) -O ihex $(TARGET).elf $(TARGET).hex

program: all
	@echo "Programing the device"
	$(AVRDUDE) $(AVRDUDE_FLAGS)

asm:	all
	@echo "Generating assembler code"
	avr-objdump -S $(TARGET).elf >  $(TARGET).asm
clean:
	@echo "Cleaning"
	@rm -rf $(TARGET).hex $(TARGET).o $(TARGET).elf *~
