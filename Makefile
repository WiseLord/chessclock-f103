PROJECT = chessclock

STM32_MCU = F103CB

DISPLAY = ILI9341
DISPVAR = SPI

DEBUG = 1
OPT = -Os


# Lowercase argument
lc = $(shell echo $1 | tr '[:upper:]' '[:lower:]')

ifeq "$(STM32_MCU)" "F103CB"
  STM32_FAMILY = STM32F1
  STM32_GROUP  = $(STM32_FAMILY)03xB
endif

TARGET = $(call lc, $(PROJECT)_$(STM32_MCU)_$(DISPLAY)_$(DISPVAR))

C_DEFS = -DUSE_FULL_LL_DRIVER -D$(STM32_GROUP) -D_$(STM32_MCU)

#C_DEFS += -D_DISP_READ_ENABLED
#C_DEFS += -D_DISP_RST_ENABLED
C_DEFS += -D_DISP_BCKL_ENABLED

C_SOURCES = main.c

C_SOURCES += actions.c
C_SOURCES += chess.c
C_SOURCES += debug.c
C_SOURCES += eemul.c
C_SOURCES += handlers.c
C_SOURCES += input.c
C_SOURCES += menu.c
C_SOURCES += pins.c
C_SOURCES += screen.c
C_SOURCES += settings.c
C_SOURCES += spi.c
C_SOURCES += swtimers.c
C_SOURCES += timers.c
C_SOURCES += usart.c
C_SOURCES += utils.c
C_SOURCES += tr/labels.c
C_SOURCES += $(wildcard tr/labels_*.c)

# Display source files
C_SOURCES += $(wildcard display/fonts/font*.c)

ifneq (,$(filter $(DISPLAY), \
  ILI9163   \
  S6D0144   \
  ST7735    \
))
  DISPSIZE = 160x128
endif

ifneq (,$(filter $(DISPLAY), \
  L2F50126  \
  LPH9157   \
  LS020     \
  SSD1286A  \
))
  DISPSIZE = 176x132
endif

ifneq (,$(filter $(DISPLAY), \
  HX8340    \
  ILI9225   \
  LGDP4524  \
  S6D0164   \
))
  DISPSIZE = 220x176
endif

ifneq (,$(filter $(DISPLAY), \
  HX8347A   \
  HX8347D   \
  ILI9320   \
  ILI9341   \
  MC2PA8201 \
  S6D0129   \
  S6D0139   \
  SPFD5408  \
  SSD1289   \
  SSD2119   \
))
  DISPSIZE = 320x240
endif

ifneq (,$(filter $(DISPLAY), \
  ILI9327   \
  S6D04D1   \
  ST7793    \
))
  DISPSIZE = 400x240
endif

ifneq (,$(filter $(DISPLAY), \
  ILI9481   \
  ILI9486   \
  R61581    \
  RM68140   \
))
  DISPSIZE = 480x320
endif

C_SOURCES += display/gc$(DISPSIZE)/$(call lc,$(DISPLAY)).c
C_SOURCES += display/dispdrv.c
C_SOURCES += display/glcd.c
C_DEFS += -D_$(DISPLAY)
C_DEFS += -D_DISP_$(DISPVAR)
C_DEFS += -D_DISP_$(DISPSIZE)

C_SOURCES += gui/canvas.c
C_SOURCES += gui/lt$(DISPSIZE).c
C_SOURCES += gui/palette.c
C_SOURCES += $(wildcard gui/fonts/font*.c)
C_SOURCES += $(wildcard gui/icons/icon*.c)
C_SOURCES += $(wildcard gui/widget/*.c)

C_INCLUDES += \
  -Idisplay/fonts \
  -I.

C_SOURCES += \
  drivers/$(STM32_FAMILY)xx_HAL_Driver/Src/$(call lc, $(STM32_FAMILY))xx_ll_gpio.c \
  drivers/$(STM32_FAMILY)xx_HAL_Driver/Src/$(call lc, $(STM32_FAMILY))xx_ll_rcc.c \
  drivers/$(STM32_FAMILY)xx_HAL_Driver/Src/$(call lc, $(STM32_FAMILY))xx_ll_usart.c \
  drivers/$(STM32_FAMILY)xx_HAL_Driver/Src/$(call lc, $(STM32_FAMILY))xx_ll_utils.c \
  system/system_$(call lc, $(STM32_FAMILY))xx.c

C_INCLUDES += \
-Idrivers/$(STM32_FAMILY)xx_HAL_Driver/Inc \
  -Idrivers/CMSIS/Device/ST/$(STM32_FAMILY)xx/Include \
  -Idrivers/CMSIS/Include \
  -Isystem

AS_DEFS +=

ASM_SOURCES += \
  system/startup_$(call lc, $(STM32_GROUP)).s

# Build directory
BUILD_DIR = build

ifeq "$(STM32_FAMILY)" "STM32F1"
  CPU = -mcpu=cortex-m3
  FPU =
  FLOAT-ABI =
endif

ifeq "$(STM32_FAMILY)" "STM32F3"
  CPU = -mcpu=cortex-m4
  FPU = -mfpu=fpv4-sp-d16
  FLOAT-ABI = -mfloat-abi=hard
endif

# Compiler
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
OPT_FLAGS = $(OPT) -fshort-enums -ffunction-sections -fdata-sections -ffreestanding
WARN = -Wall -Werror
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT_FLAGS) $(WARN)
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT_FLAGS) $(WARN)
ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif
# Dependency information
CFLAGS += -MMD -MP -MT $(BUILD_DIR)/$(*F).o -MF $(BUILD_DIR)/$(*D)/$(*F).d

LDSCRIPT = system/$(call lc, STM32$(STM32_MCU))_flash.ld
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nosys.specs -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections
#LDFLAGS = $(WARN) -mmcu=$(MCU) -Wl,--gc-sections -Wl,--relax

# Main definitions
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
OD = $(PREFIX)objdump
AR = $(PREFIX)ar
SZ = $(PREFIX)size

OPENOCD := openocd
OPENOCD_CFG := system/$(call lc, $(STM32_GROUP))_openocd.cfg

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

ELF = $(BUILD_DIR)/$(TARGET).elf
BIN = flash/$(TARGET).bin

all: $(BIN) size

$(BIN): $(ELF)
	$(CP) -O binary $(ELF) $(BIN)
	$(OD) -h -S $(ELF) > $(BUILD_DIR)/$(TARGET).lss

$(ELF): $(OBJECTS)
	@mkdir -p flash
	$(CC) $(LDFLAGS) -o $(ELF) $(OBJECTS)

size: $(ELF)
	$(SZ) $(ELF)

$(BUILD_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(C_DEFS) -o $@ $<

$(BUILD_DIR)/%.o: %.s Makefile
	@mkdir -p $(dir $@)
	$(AS) -c $(CFLAGS) $(AS_DEFS) -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)

clean_disp:
	@rm -f $(patsubst %.c,$(BUILD_DIR)/%.o, $(notdir $(wildcard display/gc*/*.c)))
	@rm -f $(BUILD_DIR)/dispdrv.o $(BUILD_DIR)/layout.o $(BUILD_DIR)/pins.o

.PHONY: flash
flash: $(BIN)
	$(OPENOCD) -f $(OPENOCD_CFG) -c "stm_flash $(BIN)" -c shutdown

.PHONY: erase
erase:
	$(OPENOCD) -f $(OPENOCD_CFG) -c "stm_erase" -c shutdown

.PHONY: ee_erase
ee_erase:
	$(OPENOCD) -f $(OPENOCD_CFG) -c "stm_ee_erase" -c shutdown

# Other dependencies
-include $(OBJECTS:.o=.d)
