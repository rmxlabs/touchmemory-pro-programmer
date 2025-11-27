/** I N C L U D E S *************************************************/
#include "typedefs.h"
#include "usb.h"

/** C O N S T A N T S ************************************************/
#pragma romdata

/* Device Descriptor */
rom USB_DEV_DSC device_dsc=
{
    sizeof(USB_DEV_DSC),    // Size of this descriptor in bytes
    DSC_DEV,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    EP0_BUFF_SIZE,          // Max packet size for EP0, see usbcfg.h
    0x04D8,                 // Vendor ID: Microchip
    0x553C,                 // Product ID: HID Bootloader
    0x0002,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
CFG01={
    /* Configuration Descriptor */
    sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    DSC_CFG,                // CONFIGURATION descriptor type
    sizeof(cfg01),          // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT|_SELF,    		// Attributes, see usbdefs_std_dsc.h
    50,                     // Max power consumption (2X mA)

    /* Interface Descriptor */
    sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    DSC_INTF,               // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    HID_INTF,               // Class code
    0,     					// Subclass code, no subclass
    0,     					// Protocol code, no protocol
    0,                      // Interface string index

    /* HID Class-Specific Descriptor */
    sizeof(USB_HID_DSC),    // Size of this descriptor in bytes
    DSC_HID,                // HID descriptor type
    0x0111,                 // HID Spec Release Number in BCD format
    0x00,                   // Country Code (0x00 for Not supported)
    HID_NUM_OF_DSC,         // Number of class descriptors, see usbcfg.h
    DSC_RPT,                // Report descriptor type
    sizeof(hid_rpt01),      // Size of the report descriptor

    /* Endpoint Descriptor */
    sizeof(USB_EP_DSC),DSC_EP,_EP01_IN,_INT,HID_INT_IN_EP_SIZE,0x01,
    sizeof(USB_EP_DSC),DSC_EP,_EP01_OUT,_INT,HID_INT_OUT_EP_SIZE,0x01
};

rom struct{byte bLength;byte bDscType;word string[1];}sd000={
sizeof(sd000),DSC_STR,0x0409};

rom struct{byte bLength;byte bDscType;word string[25];}sd001={
sizeof(sd001),DSC_STR,
'M','i','c','r','o','c','h','i','p',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'};

rom struct{byte bLength;byte bDscType;word string[18];}sd002={
sizeof(sd002),DSC_STR,
'H','I','D',' ','U','S','B',' ','B','o','o',
't','l','o','a','d','e','r'};

rom struct{byte report[HID_RPT01_SIZE];}hid_rpt01={
//	First byte is the "Item".  First byte's two LSbs are the number of data bytes that
//  follow, but encoded (0=0, 1=1, 2=2, 3=4 bytes).
//  bSize should match number of bytes that follow, or REPORT descriptor parser won't work.  The bytes
//  that follow in each item line are data bytes

{
    0x06, 0x00, 0xFF,       // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,             // Usage (Vendor Usage 1)
    0xA1, 0x01,             // Collection (Application)
    0x19, 0x01,             //      Usage Minimum 
    0x29, 0x40,             //      Usage Maximum 	//64 input usages total (0x01 to 0x40)
    0x15, 0x00,             //      Logical Minimum (data bytes in the report may have minimum value = 0x00)
    0x26, 0xFF, 0x00, 	  	//      Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
    0x75, 0x08,             //      Report Size: 8-bit field size
    0x95, 0x40,             //      Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x81, 0x00,             //      Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
    0x19, 0x01,             //      Usage Minimum 
    0x29, 0x40,             //      Usage Maximum 	//64 output usages total (0x01 to 0x40)
    0x91, 0x00,             //      Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.
    0xC0}                   // End Collection
};    

rom const unsigned char *rom USB_CD_Ptr[]=
{
    (rom const unsigned char *rom)&cfg01,
    (rom const unsigned char *rom)&cfg01
};
rom const unsigned char *rom USB_SD_Ptr[]=
{
    (rom const unsigned char *rom)&sd000,
    (rom const unsigned char *rom)&sd001,
    (rom const unsigned char *rom)&sd002
};

rom pFunc ClassReqHandler[1]=
{
    &USBCheckHIDRequest
};

#pragma code USB_BOOTLOADER

/** EOF usbdsc.c ****************************************************/
