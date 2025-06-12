#pragma once
#ifndef STM32F1
  #define STM32F1
#endif
#include "libopencm3/stm32/rcc.h"
#include "libopencm3/stm32/gpio.h"
#include "libopencm3/usb/usbd.h"
#include "libopencm3/usb/cdc.h"

#include "usb_iface_dscrptr.h"

static const struct usb_config_descriptor usb_conf_dscrptr = {
    .bLength             = USB_DT_CONFIGURATION_SIZE, // Size of Desciptor in Bytes
    .bDescriptorType     = USB_DT_CONFIGURATION,      // Configuration Desciptor (0x02)
    .wTotalLength        = 0,                         // Total length in bytes of data returned
    .bNumInterfaces      = 3,                         // Number of Interfaces
    .bConfigurationValue = 1, // Value to use as an argument to select this configuration
    .iConfiguration      = 0, // Index of String Descriptor describing this configuration
    // D7 Reserved, set to 1. (USB 1.0 Bus Powered)
    // D6 Self Powered
    // D5 Remote Wakeup
    // D4..0 Reserved, set to 0
    .bmAttributes = 0xC0,
    .bMaxPower    = 0x32, // Maximum Power Consumption in 2mA units
    .interface    = usb_iface,
};

// When the configuration descriptor is read, it returns the entire configuration which includes
// all related interface and endpoint descriptors. The wTotallength field reflects the number in the hierarchy

// bNumInterfaces specifies the number of interfaces present for this configuration

// bConfigurationValue is used by the SetConfiguration request to select this configuration

// iConfiguration is a index to a string descriptor describing the configuration in human readable form

// bmAttributes specify power parameters for the configuration.
// If a device is seld powered, it sets D6
// Bit D7 was used in USB 1.0 to indicate a bus powered device? but this is now done by bMaxPower.
// If device uses any power from the bus, whether it be as a bus powered device or as a self powered device
// it must report its power consumption in bMaxPower.
// Devices can also support remote wakeup which allows the the device to wake up the host
// when the host is in suspend

// bMaxPower defines the maximum power the device will drain from the bus.
// This is in 2mA units, thus a maximum of approximately 500mA can be specified.
// The specification allows a high powered device to drain no more then 500mA from Vbus.
// If a device loses external power, then it must not drain more than indicated in bMaxPower.
// It should fail any operation it cannot perform without external power
