#include <stdint.h>

extern uint32_t _data_rom;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;

void load_data ()
{  uint32_t * data_in_rom_p = &_data_rom;
   for (uint32_t * data_in_ram_p = &_data; data_in_ram_p < &_edata; )
   {  *data_in_ram_p++ = *data_in_ram_p++;
   }
}

void load_bss ()
{  for (uint32_t * bss_in_ram_p = &_bss; bss_in_ram_p < &_ebss;)
   {  *bss_in_ram_p++ = 0;
   }
}

