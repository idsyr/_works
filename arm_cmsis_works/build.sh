arm-none-eabi-as boot.s -o boot.o
arm-none-eabi-gcc -g -nostdlib -nostartfiles -ffreestanding -Wall -mthumb -mcpu=cortex-m3 -ICPU -IMCU -c systick_blink.c -o main-c3.o
arm-none-eabi-ld boot.o main-c3.o -o blink-c3.elf -T mem.ld  
arm-none-eabi-objdump -D blink-c3.elf > blink-c3.lst
arm-none-eabi-objcopy blink-c3.elf blink-c3.bin -O binary
