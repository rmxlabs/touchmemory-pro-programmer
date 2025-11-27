#ifndef USB_H
#define USB_H

/*
 * usb.h provides a centralize way to include all files
 * required by Microchip USB Firmware.
 *
 * The order of inclusion is important.
 * Dependency conflicts are resolved by the correct ordering.
 */

#include "usbcfg.h"
#include "usbdefs_std_dsc.h"
#include "usbdsc.h"

#include "usbdefs_ep0_buff.h"
#include "usbmmap.h"

#include "usbdrv.h"
#include "usbctrltrf.h"
#include "usb9.h"

#if defined(USB_USE_HID)                // See autofiles\usbcfg.h
#include "hid.h"
#endif

#endif //USB_H
