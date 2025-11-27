#include "p18cXXX.h"
#include "BL_LED.h"

#pragma udata

unsigned char BL_LedData;
unsigned char BL_LedSave;

#pragma code USB_BOOTLOADER

/*
void BL_REG_Send(unsigned char data)
{
	static unsigned char n;

	n = 8;
	do {
		REG_DAT = 0;
		if ((data & 0x80) != 0) REG_DAT = 1;
		REG_RCK = 1;
		data <<= 1;
		REG_RCK = 0;
	} while(--n);
	REG_RCK = 1;
	REG_RCK = 0;
	REG_SCK = 1;
	REG_SCK = 0;
}

void BL_LED(void)
{

	if (BL_LedData != BL_LedSave)
	{
		BL_REG_Send(BL_LedData);
		BL_LedSave = BL_LedData;
	}
}
*/
