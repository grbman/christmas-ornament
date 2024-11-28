# Path you your toolchain installation, leave empty if already in system PATH
TOOLCHAIN_ROOT =

###############################################################################

# Project specific
PROJNAME = ornament
TARGET = $(PROJNAME).hex
SRC_DIR = src/
INC_DIR = inc/

# Toolchain
CC = $(TOOLCHAIN_ROOT)avr-gcc
HEX = $(TOOLCHAIN_ROOT)avr-objcopy
FLASHER = avrdude

# Project sources
SRC_FILES = $(wildcard $(SRC_DIR)*.c) $(wildcard $(SRC_DIR)*/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)*.s) $(wildcard $(SRC_DIR)*/*.s)

# Project includes
INCLUDES   = -I$(INC_DIR)

# Vendor includes


# Compiler Flags
CFLAGS = -Os -mmcu=attiny404
CFLAGS += $(INCLUDES)


###############################################################################

# This does an in-source build. An out-of-source build that places all object
# files into a build directory would be a better solution, but the goal was to
# keep this file very simple.

CXX_OBJS = $(SRC_FILES:.c=.o)
ASM_OBJS = $(ASM_FILES:.s=.o)
ALL_OBJS = $(ASM_OBJS) $(CXX_OBJS)

.PHONY: clean

all: $(TARGET)

# Compile
$(CXX_OBJS): %.o: %.c
$(ASM_OBJS): %.o: %.s
$(ALL_OBJS):
	@echo "[CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@

# Link
%.elf: $(ALL_OBJS)
	@echo "[LD] $@"
	@$(CC) $(CFLAGS) $(ALL_OBJS) -o $@

# Create Hex        
%.hex: %.elf
	@echo "[HEX] $@"
	@$(HEX) $< -O ihex $(TARGET)

#  avrdude  -p t404 -c serialupdi -b 115200 -U flash:w:"blink.hex":a -P /dev/tty.usbserial-AB0O4T03
flash: %.hex
	@echo "[FLASH] $@"
	@$(FLASHER) -p t404 -c serialupdi -b 230400 -U flash:w:"$(TARGET)":a -P /dev/tty.usbserial-AB0O4T03
# Clean
clean:
	@rm -f $(ALL_OBJS) $(TARGET)
