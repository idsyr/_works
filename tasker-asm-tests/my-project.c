#ifndef STM32F1
  #define STM32F1
#endif

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/stm32/f1/nvic.h"
#include "libopencm3/stm32/common/timer_common_all.h"
#include "libopencm3/cm3/nvic.h"

static void led_setup( void ) {
  rcc_periph_clock_enable( RCC_GPIOC );
  gpio_set_mode( GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 );
}

static void tim2_setup( void ) {
  rcc_periph_clock_enable( RCC_TIM2 );
  nvic_enable_irq( NVIC_TIM2_IRQ );
  rcc_periph_reset_pulse( RST_TIM2 );
  timer_set_mode( TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP );
  timer_set_prescaler( TIM2, ( ( rcc_apb1_frequency * 2 ) / 5000 ) );
  timer_disable_preload( TIM2 );
  timer_continuous_mode( TIM2 );
  timer_set_period( TIM2, /*6*/ 5535 );
  timer_set_oc_value( TIM2, TIM_OC1, 100 );
  timer_enable_counter( TIM2 );
  timer_enable_irq( TIM2, TIM_DIER_CC1IE );
}

void tim2_isr( void ) {
  timer_clear_flag( TIM2, TIM_SR_CC1IF );
  *( uint32_t * )0xE000ED04 |= 1 << 28;
}

void sv_call_handler( void ) {
  __asm__( "ADD SP,SP,#8*4	\n\t"
           "BX	LR		\n\t" );
}

__attribute__( ( naked ) ) void schedule_return( void ) {
  __asm__( "CPSIE I	\n\t"
           "DSB		\n\t"
           "ISB		\n\t"
           "SVC	#0	\n\t" );
}

// xPSR PC LR R12 R3 R2 R1 R0
void pend_sv_handler( void ) {
  __asm__( "CPSID I		\n\t"
           "LDR	R3,=0xE000ED04	\n\t"
           "LDR	R1,=1<<27	\n\t"
           "STR	R1,[R3]		\n\t"

           "LDR	R3,=1<<24		\n\t" // xPSR. always set to 1 (refman)
           "LDR	R2,=schedule		\n\t" // PC
           "LDR	R1,=schedule_return	\n\t" // LR
           "SUB	SP,SP,#8*4		\n\t" // new register pack
           "ADD	R0,SP,#5*4		\n\t" // position of the last three
           "STM	R0!,{R1-R3}		\n\t" // save them
           "LDR	R0,=0xFFFFFFF9		\n\t"
           "BX	R0			\n\t" );
}

void schedule( void ) { gpio_toggle( GPIOC, GPIO13 ); }

int main( void ) {
  rcc_clock_setup_pll( &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ] );
  led_setup();

  tim2_setup();
  nvic_set_priority( NVIC_PENDSV_IRQ, 0xFF );

  for ( ;; ) {
    for ( int i = 0; i < 0x800000; ++i ) {
      __asm__( "nop" );
    }
    // *( uint32_t * )0xE000ED04 |= 1 << 28;
  }
}
