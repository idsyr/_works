#pragma once
#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"

static const struct {
  struct usb_cdc_header_descriptor          header;
  struct usb_cdc_call_management_descriptor call_mgmt;
  struct usb_cdc_acm_descriptor             acm;
  struct usb_cdc_union_descriptor           cdc_union;
} __attribute__( ( packed ) ) usb_cdcacm_func_dscrptr = {
    .header =
        {
            .bFunctionLength    = sizeof( struct usb_cdc_header_descriptor ),
            .bDescriptorType    = CS_INTERFACE,
            .bDescriptorSubtype = USB_CDC_TYPE_HEADER,
            .bcdCDC             = 0x0110,
        },
    .call_mgmt =
        {
            .bFunctionLength    = sizeof( struct usb_cdc_call_management_descriptor ),
            .bDescriptorType    = CS_INTERFACE,
            .bDescriptorSubtype = USB_CDC_TYPE_CALL_MANAGEMENT,
            .bmCapabilities     = 0,
            .bDataInterface     = 1,
        },
    .acm =
        {
            .bFunctionLength    = sizeof( struct usb_cdc_acm_descriptor ),
            .bDescriptorType    = CS_INTERFACE,
            .bDescriptorSubtype = USB_CDC_TYPE_ACM,
            .bmCapabilities     = 0,
        },
    .cdc_union =
        {
            .bFunctionLength        = sizeof( struct usb_cdc_union_descriptor ),
            .bDescriptorType        = CS_INTERFACE,
            .bDescriptorSubtype     = USB_CDC_TYPE_UNION,
            .bControlInterface      = 0,
            .bSubordinateInterface0 = 1,
        },
};
