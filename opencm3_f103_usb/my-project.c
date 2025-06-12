#ifndef STM32F1
  #define STM32F1
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"
#include "libopencm3/stm32/f1/nvic.h"
#include "libopencm3/stm32/common/timer_common_all.h"
#include "libopencm3/usb/usbstd.h"
#include "libopencm3/cm3/systick.h"
#include "libopencm3/usb/hid.h"

#include "usb/usb.h"

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

static void tim3_setup( void ) {
  rcc_periph_clock_enable( RCC_TIM3 );
  nvic_enable_irq( NVIC_TIM3_IRQ );
  rcc_periph_reset_pulse( RST_TIM3 );
  timer_set_mode( TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP );
  timer_set_prescaler( TIM3, ( ( rcc_apb1_frequency * 2 ) / 500 ) );
  timer_disable_preload( TIM3 );
  timer_one_shot_mode( TIM3 );
  timer_set_period( TIM3, 5535 );
  timer_set_oc_value( TIM3, TIM_OC1, 100 );
  timer_enable_irq( TIM3, TIM_DIER_CC1IE );
}

void cdcacm_data_rx_cb( usbd_device * usbd_dev, uint8_t ep ) {
  ( void )ep;
  char buf[64];
  int  len = usbd_ep_read_packet( usbd_dev, 0x01, buf, 64 );
  sprintf( buf, "pressed\n" );
  if ( len )
    usbd_ep_write_packet( usbd_dev, 0x82, buf, 8 );
}

void tim2_isr( void ) {
  timer_clear_flag( TIM2, TIM_SR_CC1IF );
  gpio_toggle( GPIOC, GPIO13 );

  char buf[64];
  int  len = usbd_ep_read_packet( usbd_dev, 0x01, buf, 64 );
  sprintf( buf, "America yaaa\n" );
  usbd_ep_write_packet( usbd_dev, 0x82, buf, 13 );

  static int8_t counting_byte = 0x04;
  ++counting_byte;
  uint8_t press_report[] = {
      0x00, 0x00, counting_byte, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  usbd_ep_write_packet( usbd_dev, 0x81, press_report, sizeof( press_report ) );

  usbd_poll( usbd_dev );

  timer_enable_counter( TIM3 );
}

void tim3_isr( void ) {
  timer_clear_flag( TIM3, TIM_SR_CC1IF );
  uint8_t release_report[] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  usbd_ep_write_packet( usbd_dev, 0x81, release_report, sizeof( release_report ) );
  usbd_poll( usbd_dev );
}

int main( void ) {
  rcc_clock_setup_pll( &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ] );
  led_setup();
  rcc_periph_clock_enable( RCC_GPIOA );

  configure_usb();
  init_usb();
  tim3_setup();
  tim2_setup();
  for ( ;; ) {
  }
}
