#pragma once
#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"

static struct usb_device_descriptor usb_dev_dscrptr = {
    .bLength            = USB_DT_DEVICE_SIZE, // Size of the Descriptor in Bytes (18 bytes)
    .bDescriptorType    = USB_DT_DEVICE,      // Device Descriptor (0x01)
    .bcdUSB             = 0x0200,             // USB Specification Number which device complies too
    .bDeviceClass       = 0,                  // Class Code (Assigned by USB Org)
    .bDeviceSubClass    = 0,                  // Subclass Code (Assigned by USB Org)
    .bDeviceProtocol    = 0,                  // Protocol Code (Assigned by USB Org)
    .bMaxPacketSize0    = 64,     // Maximum Packet Size for Zero Endpoint. Valid: 8, 16, 32, 64
    .idVendor           = 0xFADE, // Vendor ID (Assigned by USB Org)
    .idProduct          = 0x0012, // Product ID (Assigned by Manufacturer)
    .bcdDevice          = 0x0000, // Device Release Number
    .iManufacturer      = 1,      // Index of Manufacturer String Descriptor
    .iProduct           = 2,      // Index of Product String Descriptor
    .iSerialNumber      = 3,      // Index of Serial Number String Descriptor
    .bNumConfigurations = 1,      // Number of Possible Configurations
};

// The bcdUSB field reports the highest version of USB the device supports.
// The value is in binary coded decimal with a format of 0xJJMN where
// JJ is the major version number
// M is the minor version number
// N is the sub minor version number
// e.g. USB 2.0 is reported as 0x0200
//      USB 1.1 as             0x0110
//      USB 1.0 as             0x0100

// The bDeviceClass, bDeviceSubClass and bDeviceProtocol
// are used by the operating system to find a class driver for your device.
// Typically only the bDeviceClass is set at the device level.
// Most class specifications choose to identfy itself at the
// interface level and as a result set the bDeviceClass as 0x00
// This allows for the one device to support multiple classes

// The bMaxPacketSize field reports the maximum packet size for endpoint zero.
// All deivices must support enpoint zero

// The idVendor andidProduct are used by the operating system
// to find a driver for your device
// The Vendor ID is assigned by the USB-IF

// The bsdDevice hase the same format than the bsdUSB
// and is used to provide a device version number.
// This value assigned by the developer

// Three string descriptors exist to provide details of
// the manufacturer, product and serial number.
// There i no requirement to have string descriptors.
// If no string descriptor is present, a index of zero should be used.

// bNummConfigurations defines the number of configuratinos
// the device supports at its current speed
