#include "MCU/stm32f103xb.h"

void led_setup ()
{  RCC->APB2ENR |= (0b1 << RCC_APB2ENR_IOPCEN_Pos); // enable clock gpio_c
   GPIOC->CRH |= (0b10 << GPIO_CRH_MODE13_Pos);     // output pushpull 2MHz
}

void led_toggle (int on)
{  GPIOC->ODR ^= (1 << GPIO_ODR_ODR13_Pos);
}
