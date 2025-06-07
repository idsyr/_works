#include "../mcu/stm32f103xb.h"
#include "../core/core_cm3.h"

void enable_hse();
void config_pll_with_with_hse_72mhz();
void enable_pll();
void config_bus_clock();
void setclock_pll();

void enable_hse() {
  RCC->CR |= RCC_CR_HSEON;
  while ( !( RCC->CR & RCC_CR_HSERDY ) )
    ;
}

void enable_pll() {
  RCC->CR &= ~RCC_CR_HSEBYP;
  RCC->CR |= RCC_CR_PLLON;
  while ( !( RCC->CR & RCC_CR_PLLRDY ) )
    ;
}

void config_pll_with_hse_72mhz() {
  RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
  RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;
  RCC->CFGR &= ~RCC_CFGR_PLLMULL;
  RCC->CFGR |= RCC_CFGR_PLLMULL9;
  RCC->CFGR &= ~RCC_CFGR_PLLSRC;
  RCC->CFGR |= RCC_CFGR_PLLSRC;
}

void config_bus_clock() {
  RCC->CFGR &= ~RCC_CFGR_HPRE;
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
}

void setclock_pll() {
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  while ( !( RCC->CFGR & RCC_CFGR_SWS_PLL ) )
    ;
}

void set_optimal_clock() {
  enable_hse();
  config_pll_with_hse_72mhz();
  enable_pll();
  config_bus_clock();
  setclock_pll();
}
