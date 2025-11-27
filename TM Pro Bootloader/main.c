
/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h"                   
#include "usb.h"                         
#include "io_cfg.h"                     
#include "BootPIC18NonJ.h"

#include "interrupt.h"
#include "BL_LED.h"
#include "utils.h"

/** V A R I A B L E S ********************************************************/
#pragma udata

extern void BL_Delay1KTCYx(unsigned char);
extern void _GOTO_REMAPPED_RESET_VECTOR_ADDRESS(void);
extern void MainApp(void);

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void main(void);
void BlinkUSBStatus(void);

/** D E C L A R A T I O N S **************************************************/
#pragma code RESET_VECTOR=0x000000
void reset(void)
{
	_asm goto main _endasm
}

//******************************************************************************

#pragma code USB_BOOTLOADER

//******************************************************************************

unsigned int  counterTry;
unsigned char isConnectedToPC;

void StackInit(void)
{
	_asm
	lfsr 1, _stack
    lfsr 2, _stack
    _endasm
}

void main(void)
{
	INTCON = 0b00000000;
	OSCCON = 0b01100000;	// 48.0MHz

	//BL_LED INIT
	LATBbits.LATB4 = 0; TRISBbits.TRISB4 = 0;
	LATBbits.LATB5 = 0; TRISBbits.TRISB5 = 0;
//	LATBbits.LATB6 = 0; TRISBbits.TRISB6 = 0;
	// BUZZ INIT
	LATCbits.LATC5 = 0; TRISCbits.TRISC5 = 0;
	LATCbits.LATC7 = 0; TRISCbits.TRISC7 = 0;

	BL_LedSave = 0xFF;
	BL_LedData = BL_LED_OFF; 
	BLED_1 = 1;
	BLED_2 = 0;

	StackInit();


    mInitializeUSBDriver();         // See usbdrv.h
    BootloaderInit();  


	counterTry = 0;
	isConnectedToPC = 0;

    while(1)
    {
//		ClrWdt();	
	    USBCheckBusStatus();
    	USBDriverService();

		BL_LedData = BL_LED_READ;

	    if ((usb_device_state == CONFIGURED_STATE) && (UCONbits.SUSPND != 1))
	    {
			ProcessIO();
 	    }
    
		if (usb_device_state > POWERED_STATE) 
		{
			isConnectedToPC = 1;
		}

		if (BootState == BootNotFound)
		{
			BL_Delay1KTCYx(1);
			counterTry++;
			
			if (isConnectedToPC == 1)
			{
				BL_LedData = BL_LED_WRITE;
//#pragma CHANGE "USB 'counterTry' is changed."
				if (counterTry == 50000)
				{
					_GOTO_REMAPPED_RESET_VECTOR_ADDRESS();
				}
			}
			else
			{
//#pragma CHANGE "USB 'counterTry' is changed."
				if(counterTry == 5000)
				{
					_GOTO_REMAPPED_RESET_VECTOR_ADDRESS();
				}
			}
		}
		else
		{
			BL_LedData = BL_LED_OK;
		}
//		BL_LED();
	}
}

//******************************************************************************
