.cpu cortex-m3
.thumb

.word _stack_base
.word main // reset 
.word hang // NMI
.word hang // HardFault
.word hang // MemManageFault
.word hang // BusFault
.word hang // UsageFault
.word _boot_checksum
.word hang // reserved
.word hang // reserved
.word hang // reserved
.word hang // Supervisor call
.word hang // Debug monitor (__asm volatile ("BKPT #0");)
.word hang // reserved
.word hang // Pendable service call
.word hang // SysTick
.word hang // EXTI0
// +35...

.thumb_func 
hang: b . 
.global hang

