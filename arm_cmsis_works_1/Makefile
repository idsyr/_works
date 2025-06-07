PROJECT   = arm_cmsis_works

LD_SCRIPT = linker.ld
VECTOR    = core/vector_table_m3.c mcu/vector_table_stm32f103xb.c
SOURCES   = app/main.c
INCLUDES  = -I./app -I./mcu -I./core -I.
OUTPATH   = $(shell pwd)/build


PREFIX  = arm-none-eabi
CC      = $(PREFIX)-gcc
AS      = $(PREFIX)-as
AR      = $(PREFIX)-ar
LD      = $(PREFIX)-ld
NM      = $(PREFIX)-nm
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump
READELF = $(PREFIX)-readelf
SIZE    = $(PREFIX)-size


CONCRETE_MCU_FLAGS=\
	-mcpu=cortex-m3\
	-mlittle-endian\
	-mfloat-abi=soft\
	-mthumb\
	-mno-unaligned-access\

COMMON_MCU_FLAGS=\
	-nostdlib\
	-nostartfiles\
	-ffreestanding\
	-fdata-sections\
	-ffunction-sections\

CC_FLAGS = $(COMMON_MCU_FLAGS) $(CONCRETE_MCU_FLAGS)


LD_FLAGS=\
	-static\
	--gc-sections\


SRCS = $(SOURCES) $(VECTOR)
OBJS = $(addprefix $(OUTPATH)/,$(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)


.PHONY: all flash dump

all: $(OUTPATH)/$(PROJECT).bin

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.elf: $(OBJS)
	$(LD) -o $@ $(LD_FLAGS) -T $(LD_SCRIPT) $(OBJS)

$(OBJS): $(OUTPATH)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CC_FLAGS) -c $(INCLUDES) $< -o $@

$(DEPS): $(OUTPATH)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) $(INCLUDES) -E $(CC_FLAGS) $< -MM -MT $(@:.d=.o) > $@

flash: 
	st-flash write $(OUTPATH)/$(PROJECT).bin 0x8000000

dump: $(OUTPATH)/dump.elf
	$(OBJDUMP) -D $<

include $(DEPS)
