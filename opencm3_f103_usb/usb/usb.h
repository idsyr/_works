#ifndef USB_H
#define USB_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "libopencm3/stm32/f1/nvic.h"

#include "usb_dev_dscrptr.h"
#include "usb_conf_dscrptr.h"
#include "usb_iface_dscrptr.h"
#include "usb_func_dscrptr.h"
#include "usb_endp_dscrptr.h"
#include "usb_hid_report_dscrptr.h"

void cdcacm_data_rx_cb( usbd_device * usbd_dev, uint8_t ep );

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

static enum usbd_request_return_codes
hid_control_request( usbd_device * usbd_dev, struct usb_setup_data * req, uint8_t ** buf,
                     uint16_t * len,
                     void ( **complete )( usbd_device *, struct usb_setup_data * ) ) {
  ( void )complete;
  ( void )usbd_dev;
  if ( ( req->bmRequestType != 0x81 ) || ( req->bRequest != USB_REQ_GET_DESCRIPTOR ) ||
       ( req->wValue != 0x2200 ) )
    return USBD_REQ_NOTSUPP;

  // woooo
  *buf = ( uint8_t * )usb_hid_report_dscrptr;
  *len = sizeof( usb_hid_report_dscrptr );
  return USBD_REQ_HANDLED;
}

static void hid_set_config( usbd_device * usbd_dev, uint16_t wValue ) {
  ( void )wValue;
  ( void )usbd_dev;
  usbd_ep_setup( usbd_dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 4, NULL );
  usbd_register_control_callback( usbd_dev, USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
                                  USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, hid_control_request );
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

static void usb_set_config( usbd_device * usbd_dev, uint16_t wValue ) {
  hid_set_config( usbd_dev, wValue );
  cdcacm_set_config( usbd_dev, wValue );
}

void configure_usb( void ) {
  usb_dev_dscrptr.idVendor  = 0xFADE; // Vendor ID (Assigned by USB Org)
  usb_dev_dscrptr.idProduct = 0x0012; // Product ID (Assigned by Manufacturer)
  usb_dev_dscrptr.bcdDevice = 0x0000; // Device Release Number

  usb_conf_dscrptr.bNumInterfaces = 3; // Number of Interfaces
  usb_conf_dscrptr.bmAttributes   = 0xC0;
  usb_conf_dscrptr.bMaxPower      = 0x32; // Maximum Power Consumption in 2mA units

  usb_hid_endp_dscrptr->bInterval =
      0x0A; // Interval for polling endpoint for data transfers. Expressed in milliseconds
}

void init_usb( void ) {
  usbd_dev = usbd_init( &st_usbfs_v1_usb_driver, &usb_dev_dscrptr, &usb_conf_dscrptr, usb_strings,
                        3, usbd_control_buffer, sizeof( usbd_control_buffer ) );
  usbd_register_set_config_callback( usbd_dev, usb_set_config );

  nvic_enable_irq( NVIC_USB_LP_CAN_RX0_IRQ );
  nvic_enable_irq( NVIC_USB_WAKEUP_IRQ );
}

void usb_wakeup_isr( void ) { usbd_poll( usbd_dev ); }

void usb_lp_can_rx0_isr( void ) { usbd_poll( usbd_dev ); }

#endif
