
_stack_base = 0x20004FFF; /* SRAM start addr is: 0x20000000 + 20Kb */
_boot_checksum = 0 - (_stack_base + main + 1 + (hang + 1) * 5);

MEMORY
{  rom(rx)   : ORIGIN = 0x08000000, LENGTH = 128K
   ram(xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{  .text : 
   {  KEEP(*(.isr_vector))
      *(.text*) 
   } > rom
   .bss  : { *(.bss*) }  > ram
}
 
