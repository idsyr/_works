
_stack_base = 0x20004FFF; /* SRAM start addr is: 0x20000000 + 20Kb */
_boot_checksum = 0 - (_stack_base + main + 1 + (hang + 1) * 5);
_data_rom = LOADADDR(.data)

MEMORY
{  rom(rx)   : ORIGIN = 0x08000000, LENGTH = 128K
   ram(xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{  .text : ALIGN(4)
   {  FILL(0xFF)
      KEEP(*(.isr_vector))
      KEEP(*(.isr_vector_nvic))
   }  > rom

   .text : ALIGN(4)
   {  FILL(0xFF)
      *(.text*)
      *(.rodata .rodata.*) /* .rodata.str1.1 */
   }  > rom

   .data : ALIGN(4)
   {  _data = .;
      *(SORT_BY_ALIGNMENT(.data*)) /* magic */
      . = ALIGN(4);
      _edata = .;
   }  > ram AT>rom = 0xFF

   .bss : ALIGN(4)
   {  _bss = .;
      *(.bss*)
      *(COMMON) /* !static */
      . = ALIGN(4);
      _ebss = .;
   }  > ram

   /DISCARD/ :
   {  *(.glue_7*)
      *(.v4_bx)
      *(.vfp11_veneer)   /* no FPU */
      *(.iplt .igot.plt)
      *(.rel.*)
   }
}
 
