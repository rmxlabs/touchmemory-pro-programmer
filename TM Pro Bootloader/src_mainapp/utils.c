
#include "p18cXXX.h"
#include "delay.h"
#include "utils.h"
#include "analog.h"

#pragma udata

unsigned char LedData;

//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

void StackPointerInit(void)
{
	_asm
	    lfsr 1, _stack			// Initialize the stack pointer.
	    lfsr 2, _stack
	_endasm;
}


#ifdef LCD
	
void LCDSend(unsigned char data, unsigned char rs)
{
	unsigned char n;

	if (rs != 0) LCD_DAT = 1; else LCD_DAT = 0;
	LCD_CLK = 1;
	Delay10TCYx(1);
	LCD_CLK = 0;
	n = 6;
	do {
		if ((data & 0x08) != 0) LCD_DAT = 1; else LCD_DAT = 0;
		LCD_CLK = 1;
		data <<= 1;
		LCD_CLK = 0;
	} while(--n);
}

#endif

/*
void REG_Send(unsigned char data)
{
#ifndef LCD
	unsigned char n;

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
#endif
}
*/

void LED(unsigned char led)
{
#ifndef LCD
	LED_1 = 0; LED_2 = 0;
	if ((led & LED_READ)  != 0) LED_1 = 1;
	if ((led & LED_WRITE) != 0) LED_2 = 1;
//	if ((led & LED_OK) 	  != 0) LED_1 = 1;
//	if ((led & LED_ERROR) != 0) LED_2 = 1;
#endif
}


// ------------------------------------------------------------------------

void Gen(unsigned char freq,  unsigned int duration)
{
	while (duration !=0 )
	{
		BUZZ_1 = 1; BUZZ_2 = 0;
		Delay10TCYx(freq);
		BUZZ_1 = 0; BUZZ_2 = 1;
		Delay10TCYx(freq);
		duration--;
	}
	BUZZ_1 = 0; BUZZ_2 = 0;

}

void LongBeep(void)
{
   Gen(33, 1000);
}

void Beep(void)
{
   Gen(33, 128);
}

void ErrorBeep(void)
{
   Gen(100, 60);
   Delay1KTCYx(30);
   Delay1KTCYx(30);
   Gen(150, 105);
}

#ifdef WRITER

unsigned char EncodeTwoBits(unsigned char c)
{    
   	switch (c & 0x03) 
	{
		case 0x00:	return(0x7);
		case 0x01:	return(0xB);
		case 0x02:	return(0xD);
   		case 0x03:	return(0xE);
   	} 
}

void Encode_Cyfral(unsigned char CodeHi, unsigned char CodeLow)
{    
	DataCodTemp[3]  = EncodeTwoBits(CodeHi);
	CodeHi >>= 2;
  	DataCodTemp[3] |= EncodeTwoBits(CodeHi) << 4;
	CodeHi >>= 2;
	DataCodTemp[2]  = EncodeTwoBits(CodeHi);
	CodeHi >>= 2;
  	DataCodTemp[2] |= EncodeTwoBits(CodeHi) << 4;

	DataCodTemp[1]  = EncodeTwoBits(CodeLow);
	CodeLow >>= 2;
  	DataCodTemp[1] |= EncodeTwoBits(CodeLow) << 4;
	CodeLow >>= 2;
	DataCodTemp[0]  = EncodeTwoBits(CodeLow);
	CodeLow >>= 2;
  	DataCodTemp[0] |= EncodeTwoBits(CodeLow) << 4;
}


unsigned char EEPROMRead(unsigned char Addr)
{
  	EEADR = Addr;          
  	EECON1bits.EEPGD = 0;  
  	EECON1bits.CFGS  = 0;  
  	EECON1bits.RD    = 1;  
  	_asm  
		nop 	
		nop
	_endasm
  	return(EEDATA);    	 
}


void EEPROMWrite(unsigned char Addr, unsigned char Byte)
{
	unsigned char GIE_Flag;

  	EECON1bits.EEPGD = 0;
  	EECON1bits.CFGS  = 0;
  	EECON1bits.WREN  = 1;
  	EEADR = Addr;
  	EEDATA = Byte;
	GIE_Flag = INTCON & 0x80;
  	INTCONbits.GIE = 0;
	EECON2 = 0x55;
  	EECON2 = 0xAA;
  	EECON1bits.WR = 1;
	INTCON |= GIE_Flag;
  	while (!PIR2bits.EEIF);
  	PIR2bits.EEIF = 0;
  	EECON1bits.WREN = 0;
}


void Ror(unsigned char *data)
{
	unsigned char n;

	n = *data >> 1;
	if ((*data & 0x01) != 0) n |= 0x80;
	*data = n;
}

void Rol(unsigned char *data)
{
	unsigned char n;

	n = *data << 1;
	if ((*data & 0x80) != 0) n |= 0x01;
	*data = n;
}

void NonLinearInc(unsigned char *DataH, unsigned char *DataL)
{
	word n, t;
	unsigned char bl, bh;

	n.h = *DataH;
	n.l = *DataL;

	bl = (n.l & 0b01010101) | (n.h & 0b10101010); 
	bh = (n.l & 0b10101010) | (n.h & 0b01010101);
	n.l = bl; n.h = bh;

	n.w ^= 0x6EB4;

	Ror(&n.l); Ror(&n.l); Ror(&n.l);
	Rol(&n.h); Rol(&n.h); Rol(&n.h);

	
	n.w += 0x0001;


	Rol(&n.l); Rol(&n.l); Rol(&n.l);
	Ror(&n.h); Ror(&n.h); Ror(&n.h);

	n.w ^= 0x6EB4;

	*DataL = (n.l & 0b01010101) | (n.h & 0b10101010); 
	*DataH = (n.l & 0b10101010) | (n.h & 0b01010101); 
}

#endif
