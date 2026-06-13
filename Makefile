
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
	src/start.S \
	src/main.c \
	src/printf.c \
	src/cfe_api.c \
	src/cfe_string.c

OUTPUT = output
TARGET_ELF = $(OUTPUT).elf
TARGET_BIN = $(OUTPUT).bin
TARGET_SREC = $(OUTPUT).srec

INSTALLDIR = /srv/tftp

OBJECTS = 	$(patsubst %.S,%.o,$(filter %.S,$(SRCFILES))) \
			$(patsubst %.c,%.o,$(filter %.c,$(SRCFILES)))

#
#   Begin compile
#

.PHONY: all clean $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_ELF)
	@echo " [OBJCPY] $(TARGET_BIN)"
	@$(OBJCOPY) -O binary $(TARGET_ELF) $(TARGET_BIN)

$(TARGET_SREC): $(TARGET_ELF)
	@echo " [SREC] $(TARGET_SREC)"
	@$(OBJCOPY) -O srec $(TARGET_ELF) $(TARGET_SREC)

$(TARGET_ELF): $(OBJECTS) | linker.ld
	@echo " [LD] $(TARGET_ELF)"
	@$(LD) -o $@ $(LDFLAGS) $(OBJECTS)

%.o: %.c
	@echo " [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	@echo " [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET_ELF) $(TARGET_BIN) $(TARGET_SREC)

install: $(TARGET_BIN) $(TARGET_ELF) $(TARGET_SREC)
	@echo " [MKDIR] $(INSTALLDIR)"
	@mkdir -p "$(INSTALLDIR)"

	@echo " [INSTALL] $(INSTALLDIR)/$(TARGET_ELF)"
	@rm -f $(INSTALLDIR)/$(TARGET_ELF)
	@cp $(TARGET_ELF) $(INSTALLDIR)/$(TARGET_ELF)

	@echo " [INSTALL] $(INSTALLDIR)/$(TARGET_BIN)"
	@rm -f $(INSTALLDIR)/$(TARGET_BIN)
	@cp $(TARGET_BIN) $(INSTALLDIR)/$(TARGET_BIN)

	@echo " [INSTALL] $(INSTALLDIR)/$(TARGET_SREC)"
	@rm -f $(INSTALLDIR)/$(TARGET_SREC)
	@cp $(TARGET_SREC) $(INSTALLDIR)/$(TARGET_SREC)

clean:
	@rm -rf $(TARGET_ELF) $(TARGET_BIN) $(TARGET_SREC) $(OBJECTS)
	@echo "+------------------------------------------------------+"
	@echo "| There may still be installed files in $(INSTALLDIR)!     |"
	@echo "+------------------------------------------------------+"
