
CROSS_COMPILE   = arm-none-eabi-
CC              = $(CROSS_COMPILE)gcc
LD              = $(CROSS_COMPILE)gcc
OBJCOPY         = $(CROSS_COMPILE)objcopy

CFLAGS  = -Wall -Werror \
          -g -Os -mcpu=cortex-a7 \
          -Iinclude -ffreestanding \
          -nostdlib -ffunction-sections -fdata-sections \
		  -Isrc/include -DCFE_API_ALL

LDFLAGS = -T linker.ld -nodefaultlibs -nostdlib -Wl,--build-id=none,--no-warn-rwx-segments


SRCFILES = \
	src/main.c \
	src/printf.c \
	src/cfe_api.c \
	src/cfe_string.c

OUTPUT = output
TARGET_ELF = $(OUTPUT).elf
TARGET_BIN = $(OUTPUT).bin

OBJECTS = $(patsubst %.c, %.o, $(SRCFILES))

#
#   Begin compile
#

.PHONY: all clean $(TARGET_BIN)

$(TARGET_ELF): $(OBJECTS) | linker.ld
	@echo " [LD] $(TARGET_ELF)"
	@$(LD) -o $@ $(LDFLAGS) $(OBJECTS) 

$(TARGET_BIN): $(TARGET_ELF)
	@echo " [OBJCPY] $(TARGET_BIN)"
	@$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

%.o: %.c
	@echo " [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET_ELF) $(TARGET_BIN)

clean:
	@rm -rf $(TARGET_ELF) $(TARGET_BIN) $(OBJECTS)
