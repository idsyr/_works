#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"
#include "libopencm3/stm32/f1/nvic.h"
#include "libopencm3/stm32/common/timer_common_all.h"

#include "usb_dev_dscrptr.h"
#include "usb_conf_dscrptr.h"
#include "usb_iface_dscrptr.h"
#include "usb_func_dscrptr.h"
#include "usb_endp_dscrptr.h"

static const char * usb_strings[] = { "BigBoobaLovers", "CDC_ACM_DEMO", "SERIAL_NUM.PASTE.HERE." };
uint8_t             usbd_control_buffer[128];
usbd_device *       usbd_dev;

static enum usbd_request_return_codes cdcacm_control_request(
    usbd_device * usbd_dev, struct usb_setup_data * req, uint8_t ** buf, uint16_t * len,
    void ( **complete )( usbd_device * usbd_dev, struct usb_setup_data * req ) ) {
  ( void )usbd_dev;
  ( void )buf;
  ( void )complete;
  switch ( req->bRequest ) {
    case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
      return USBD_REQ_HANDLED;
    case USB_CDC_REQ_SET_LINE_CODING:
      if ( *len < sizeof( struct usb_cdc_line_coding ) )
        return USBD_REQ_NOTSUPP;
      return USBD_REQ_HANDLED;
  }
  return USBD_REQ_NOTSUPP;
}

static void cdcacm_data_rx_cb( usbd_device * usbd_dev, uint8_t ep ) {
  ( void )ep;
  char buf[64];
  int  len = usbd_ep_read_packet( usbd_dev, 0x01, buf, 64 );
  sprintf( buf, "America yaaa\n" );
  if ( len )
    usbd_ep_write_packet( usbd_dev, 0x82, buf, 13 );
}

static void cdcacm_set_config( usbd_device * usbd_dev, uint16_t wValue ) {
  ( void )wValue;
  usbd_ep_setup( usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, 64, cdcacm_data_rx_cb );
  usbd_ep_setup( usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, 64, NULL );
  usbd_ep_setup( usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 16, NULL );

  usbd_register_control_callback( usbd_dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
                                  cdcacm_control_request );
}

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
  timer_set_period( TIM2, 65535 );
  timer_set_oc_value( TIM2, TIM_OC1, 100 );
  timer_enable_counter( TIM2 );
  timer_enable_irq( TIM2, TIM_DIER_CC1IE );
}

void tim2_isr( void ) {
  timer_clear_flag( TIM2, TIM_SR_CC1IF );
  gpio_toggle( GPIOC, GPIO13 );
}

void usb_wakeup_isr( void ) { usbd_poll( usbd_dev ); }

void usb_lp_can_rx0_isr( void ) { usbd_poll( usbd_dev ); }

int main( void ) {
  led_setup();
  tim2_setup();

  rcc_clock_setup_pll( &rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ] );

  rcc_periph_clock_enable( RCC_GPIOA );

  usbd_dev = usbd_init( &st_usbfs_v1_usb_driver, &usb_dev_dscrptr, &usb_conf_dscrptr, usb_strings,
                        3, usbd_control_buffer, sizeof( usbd_control_buffer ) );
  usbd_register_set_config_callback( usbd_dev, cdcacm_set_config );

  nvic_enable_irq( NVIC_USB_LP_CAN_RX0_IRQ );
  nvic_enable_irq( NVIC_USB_WAKEUP_IRQ );
  for ( ;; ) {
  }
}
