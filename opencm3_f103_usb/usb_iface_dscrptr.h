#pragma once
#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"

#include "usb_endp_dscrptr.h"
#include "usb_func_dscrptr.h"

static const struct usb_interface_descriptor usb_comm_iface_sdcrptr[] = { {
    .bLength            = USB_DT_INTERFACE_SIZE, // Size of Descriptor in bytes (9 Bytes)
    .bDescriptorType    = USB_DT_INTERFACE,      // Interface Descriptor (0x04)
    .bInterfaceNumber   = 0,                     // Number of Interface
    .bAlternateSetting  = 0,                     // Value used to select alternative setting
    .bNumEndpoints      = 1,                     // Number of Endpoints used for this interface
    .bInterfaceClass    = USB_CLASS_CDC,         // Class Code (Assigned by USB Org)
    .bInterfaceSubClass = USB_CDC_SUBCLASS_ACM,  // Subclasss Code (Assigned by USB Org)
    .bInterfaceProtocol = USB_CDC_PROTOCOL_AT,   // Protocol Code (Assigned by USB Org)
    .iInterface         = 0, // Index of String Descriptor Described this interface
    .endpoint           = usb_comm_endp_dscrptr,
    .extra              = &usb_cdcacm_func_dscrptr,
    .extralen           = sizeof( usb_cdcacm_func_dscrptr ),
} };

static const struct usb_interface_descriptor usb_data_iface_sdcrptr[] = { {
    .bLength            = USB_DT_INTERFACE_SIZE, // Size of Descriptor in bytes (9 Bytes)
    .bDescriptorType    = USB_DT_INTERFACE,      // Interface Descriptor (0x04)
    .bInterfaceNumber   = 1,                     // Number of Interface
    .bAlternateSetting  = 0,                     // Value used to select alternative setting
    .bNumEndpoints      = 2,                     // Number of Endpoints used for this interface
    .bInterfaceClass    = USB_CLASS_DATA,        // Class Code (Assigned by USB Org)
    .bInterfaceSubClass = 0,                     // Subclasss Code (Assigned by USB Org)
    .bInterfaceProtocol = 0,                     // Protocol Code (Assigned by USB Org)
    .iInterface         = 0, // Index of String Descriptor Described this interface
    .endpoint           = usb_data_endp_dscrptr,
} };

static const struct usb_interface usb_iface[] = { {
                                                      .num_altsetting = 1,
                                                      .altsetting     = usb_comm_iface_sdcrptr,
                                                  },
                                                  {
                                                      .num_altsetting = 1,
                                                      .altsetting     = usb_data_iface_sdcrptr,
                                                  } };
