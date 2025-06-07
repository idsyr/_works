_stack_base = 0x20004FFF; /* SRAM start addr is: 0x20000000 + 20Kb */
_data_rom = LOADADDR(.data);

ENTRY(main)

MEMORY
{  rom(rx)   : ORIGIN = 0x08000000, LENGTH = 128K
   ram(xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{  .text : ALIGN(4)
   {  FILL(0xFF)
      KEEP(*(.vector_table_common))
      KEEP(*(.vector_table_concrete))
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
   {  *(.glue_7*) /* only thumb2 in cm3 -> no glue instrs required */
      *(.v4_bx)   /* only thumb2 in cm3 -> no branch exchange required */
      *(.vfp11_veneer)   /* no FPU */
      *(.iplt .igot.plt) /* no dynamic linkage -> deiscard tables */
      *(.rel.*) /* relocations tables */
   }
}
 
