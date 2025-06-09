#pragma once

#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"

static const struct usb_endpoint_descriptor usb_comm_endp_dscrptr[] = {
    {
        .bLength          = USB_DT_ENDPOINT_SIZE, // Size of Descriptor in Bytes (7 bytes)
        .bDescriptorType  = USB_DT_ENDPOINT,      // Endpoint Desciptor (0x05)
        .bEndpointAddress = 0x83,                 // Endpoint Address
        .bmAttributes     = USB_ENDPOINT_ATTR_INTERRUPT,
        .wMaxPacketSize =
            16,           // Maximum Packet Size this endpoint is capable of sending or receiving
        .bInterval = 255, // Interval for polling endpoint data transfers. Value in frame counts.
                          // Ignored for Bulk & Conitrol Endpoints. Isochronous must equal 1 and
                          // field may range from 1 to 255 for interrupt endpoints
    },
};

// bEndpointAddres indicates what endpoint this descriptor is describing

// bmAttributes spucifies the transfer type.
// This cam either be Control, Interrupt, Isochronous, or Bulk Transfers
// If an Isochronous endpoint is specified, additional attributes can be selected such as
// the Synchronisation and usage types
// Bits 0..1 Transfer Type
//	00 = Control
//	01 = Isochronous
//	10 = Bulk
//	11 = Interrupt
// Bits 2..7 are reserved. If Isochronous endpoint
// Bits 3..2 = Synchronisation Type (Iso Mode)
//	00 = No Synchronisation
//	01 = Asynchonous
//	10 = Adaptive
//	11 = Synchronous
// Bits 5..4 = Usage Type (Iso Mode)
//	00 = Data Endpoint
//	01 = Feedback Endpoint
//	10 = Explicit Feedbask Data Endpoint
//	11 = Reserved

// wMaxPacketSize indicates the maximum payload size for this endpoint

// bInterval is used to specify the polling interval of certain transfers.
// The unis are expressed in frames, thus this equates to either 1ms for
// low/full speed devices and 125us for high speed devices

static const struct usb_endpoint_descriptor usb_data_endp_dscrptr[] = {
    {
        .bLength          = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType  = USB_DT_ENDPOINT,
        .bEndpointAddress = 0x01,
        .bmAttributes     = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize   = 64,
        .bInterval        = 1,
    },
    {
        .bLength          = USB_DT_ENDPOINT_SIZE,
        .bDescriptorType  = USB_DT_ENDPOINT,
        .bEndpointAddress = 0x82,
        .bmAttributes     = USB_ENDPOINT_ATTR_BULK,
        .wMaxPacketSize   = 64,
        .bInterval        = 1,
    } };
