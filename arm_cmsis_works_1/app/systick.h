#include "stm32f103xb.h"
#include "core_cm3.h"

void systick_setup() {
  unsigned int load = 12000000;
  SysTick->CTRL     = 0x04;
  SysTick->LOAD     = load < 0xFFFFFF ? load : 0xFFFFFF;
  SysTick->VAL      = 0;
  SysTick->CTRL     = 0x05;
}

void systick_wait ()
{  volatile int i;
   i = SysTick->CTRL;
   while ((i & 0x00010000) == 0)
   {  i = SysTick->CTRL;
   }
}

void systick_wait_sec ()
{  for (int i = 0, trgt = 72000000 / 12000000; i < trgt; ++i)
   {  systick_wait();
   }
}
