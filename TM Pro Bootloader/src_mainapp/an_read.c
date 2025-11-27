#include "p18cxxx.h"
#include "delay.h"
#include "utils.h"
#include "lcd.h"
#include "rw1990.h"
#include "analog.h"

#pragma udata

#define READ_TIME_OUT  0x40

unsigned char DataCod[4];
unsigned char DataCodMem[4];
unsigned char DataCodTemp[4];

//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

unsigned char ScanKey(void)
{
	ADCON2 = 0b00111110;	// Left, Fosc/4, 20Tad.
	ADCON1 = 0b00000000;	// VDD/VSS.
	ADCON0 = 0b00011000;	// RC2 (AN6).
	ADCON0bits.ADON = 1;
	Delay10TCYx(120);
	ADCON0bits.GO = 1;
	while (ADCON0bits.GO == 1);
	return(ADRESH);
}

void WaitNoAnaolog(unsigned char time)
{
	unsigned char n, DataValue;

	n = time;
	do {
		Delay10KTCYx(5);	// 15
		DataValue = ScanKey();
        if (DataValue > READ_DET_VALUE) n--; else n = time;
	} while(n != 0);
}

unsigned char WaitAnaolog(unsigned char time, unsigned int max)
{
	unsigned char n, DataValue, i;

	TMR0H = 0; TMR0L = 0;
	n = time; i = 0;
	do {
		WREG = TMR0L ;
		if ((TMR0H & 0x80) == 0) i++;
		if ((i & 0x08) == 0) LED(LED_WRITE); else LED(LED_OFF);

		Delay10KTCYx(5);	// 15
		DataValue = ScanKey();
        if ((DataValue < READ_DET_VALUE) && (DataValue > READ_MIN_VALUE)) n--; else n = time;
/*
		LCD_CLR();
		LCD_HEX(DataValue, 0);
*/
		if (max != 0)
		{
			max--;
			if (max == 0) return(1);
		}
	} while(n != 0);
	return(0);
}

unsigned char Decode_TM2002(void)
{    
	unsigned char d, i, n, c;

  	for (i = 0; i < 4; i++) 
	{ 	
		c = 0; n = 0x80;
  		for (d = 0; d < 8; d++) 
		{ 	
    		if ((DataCod[i] & n) != 0) c++;
			n >>= 1;
		}
		if ((c & 0x01) != 0) return(1);
	}
	return(0);
}

unsigned char Decode_Cyfral(void)
{    
	unsigned char i, c, k;

 	k = 0;
  	for (i = 0; i < 4; i++) 
	{ 	
    	c = DataCod[i] & 0x0F;
    	switch (c) 
		{
       		case 0xE:   k = k ^0x03;
						break;
			case 0xD:   k = k ^0x02;
						break;
			case 0xB:   k = k ^0x01;
						break;
			case 0x7:   k = k ^0x00;
						break;
			default:   	return(1);
    	} 

    	c = DataCod[i] & 0xF0;
    	switch (c) 
		{
       		case 0xE0:  k = k ^0x0C;
						break;
       		case 0xD0:  k = k ^0x08;
						break;
	       	case 0xB0:  k = k ^0x04;
						break;
    	   	case 0x70:  k = k ^0x00;
						break;
      		default:   	return(1);
    	} 
    	if ((i == 0) || (i == 2)) k = k << 4; else { DataCod[i] = k; k = 0; }
	}

 	DataCod[0] = 0x01;   
	DataCod[2] = DataCod[3];
	DataCod[3] = 0x00;
	return(0);
}

unsigned int WAIT_P0(void)
{
	static word per;

    while (CM1CON0bits.C1OUT == 0) 
	{ 
		per.l = TMR0L;
		if ((TMR0H & READ_TIME_OUT) != 0) return(0);
	}
	per.h = TMR0H;
	TMR0H = 0; TMR0L = 0;
	if (per.w == 0) per.w++;
	return(per.w);
}

unsigned int WAIT_P1(void)
{
	static word per;

    while (CM1CON0bits.C1OUT == 1) 
	{ 
		per.l = TMR0L;
		if ((TMR0H & READ_TIME_OUT) != 0) return(0);
	}
	per.h = TMR0H;
	TMR0H = 0; TMR0L = 0;
	if (per.w == 0) per.w++;
	return(per.w);
}

unsigned char ReadCyfral(unsigned char rep)	
{    
	static word period, period1;
	static unsigned char num, i, j, counter, cod, vbit, start, DataValue;

	CM1CON0 = 0b11011001;	// IN+, IN1- (RC1), Invert input
	CM2CON0 = 0b00000000;	// CM2 - Disabled
	CM2CON1 = 0b00000000;	// Hysteresis, Synchronius

	TMR0H = 0; TMR0L = 0;

	if (WAIT_P0() == 0) return(1);

	if (WAIT_P1() == 0) return(1);

	num = 0;
	do {
		start = 0x0F; j = 0; i = 0; DataValue = 0; counter = 0;
		DataCod[0] = 0; DataCod[1] = 0; DataCod[2] = 0; DataCod[3] = 0;

		period1.w = WAIT_P0();
		if (period1.w == 0) return(1);

		period1.w = WAIT_P1();
		if (period1.w == 0) return(1);

		do {
			period.w = WAIT_P0();
			if (period.w == 0) return(1);

			period1.w = WAIT_P1();
			if (period1.w == 0) return(1);

			vbit = 0;
			if (period.w < period1.w) vbit++;
			if ((start & 0x80) == 0)
			{
				start |= vbit;
				if ((start ^ 0x01) == 0)
				{	
					start = 0x80;	
				}	
	            else 
				{ 
					start <<= 1;
					start &= 0x0F; 
				}	
			}           	
			else 
			{
				DataValue |= vbit;
				i++;
				if (i == 8)
				{
					DataCod[j] = DataValue;
					j++; DataValue = 0; i = 0;
				}
				else DataValue <<= 1;
			}   
			counter++;
			if (counter == 80) return(1);
		} while (j != 4);

		if (num == 0) 
		{
    		DataCodTemp[0] = DataCod[0]; 
			DataCodTemp[1] = DataCod[1]; 
			DataCodTemp[2] = DataCod[2]; 
			DataCodTemp[3] = DataCod[3]; 
		}
		else
		{
			if (DataCodTemp[0] != DataCod[0]) return(1);
			if (DataCodTemp[1] != DataCod[1]) return(1);
			if (DataCodTemp[2] != DataCod[2]) return(1);
			if (DataCodTemp[3] != DataCod[3]) return(1);
		}
		num++;
	} while (num != rep);

#ifdef READ_WET_CODE
	LCD_CLR();
	LCD_HEX(DataCod[0], 0);
	LCD_HEX(DataCod[1], 4);
	LCD_HEX(DataCod[2], 8);
	LCD_HEX(DataCod[3], 12);
//	while(1);
#endif

	return(Decode_Cyfral());
}

unsigned char ReadMetacom(unsigned char rep)	
{    
	static word period, period1;
	static unsigned int lastperiod, periodsum;
	static unsigned char num, i, j, counter, cod, vbit, start, DataValue;

	CM1CON0 = 0b11011001;	// IN+, IN1- (RC1), Invert input
	CM2CON0 = 0b00000000;	// CM2 - Disabled
	CM2CON1 = 0b00000000;	// Hysteresis, Synchronius

	TMR0H = 0; TMR0L = 0;

	period1.w = WAIT_P1();
	if (period1.w == 0) return(1);

	period1.w = WAIT_P0();
	if (period1.w == 0) return(1);

	num = 0;
	do {
		start = 0; DataValue = 0; counter = 0; lastperiod = 0;
		DataCod[0] = 0; DataCod[1] = 0; DataCod[2] = 0; DataCod[3] = 0;

		period1.w = WAIT_P1();
		if (period1.w == 0) return(1);

		period1.w = WAIT_P0();
		if (period1.w == 0) return(1);

		do {
			period.w = WAIT_P1();
			if (period.w == 0) return(1);

			period1.w = WAIT_P0();
			if (period1.w == 0) return(1);

			periodsum = period.w + period1.w;
			if (lastperiod != 0)
			{
				if (period1.w > lastperiod) start = 1;
			}
			lastperiod = periodsum;

			if (++counter == 80) return(1);
		} while (start == 0);	

		start = 0; i = 0; j = 0; 
		do {
			period.w = WAIT_P1();
			if (period.w == 0) return(1);

			period1.w = WAIT_P0();
			if (period1.w == 0) return(1);

			if (start != 0xFF)
			{
				start <<= 1;
				if (period.w > period1.w) start |= 0x01;
				i++;	
				if (i == 3) 
				{
					if (start != 0x02) return(1);
					i = 0; start = 0xFF; 
				}
			}
			else
			{
				if (period.w > period1.w) DataValue |= 0x01;
				i++;
				if (i == 8) 
				{
					i = 0; 
					DataCod[j++] = DataValue;
					DataValue = 0;
				}
   				else 
				{
					DataValue <<= 1;
				}
			}

		} while (j != 4);	

		if (num == 0) 
		{
    		DataCodTemp[0] = DataCod[0]; 
			DataCodTemp[1] = DataCod[1]; 
			DataCodTemp[2] = DataCod[2]; 
			DataCodTemp[3] = DataCod[3];
		}
		else
		{
			if (DataCodTemp[0] != DataCod[0]) return(1);
			if (DataCodTemp[1] != DataCod[1]) return(1);
			if (DataCodTemp[2] != DataCod[2]) return(1);
			if (DataCodTemp[3] != DataCod[3]) return(1);
		}
		num++;
	} while (num != rep);

#ifdef READ_WET_CODE
	LCD_CLR();
	LCD_HEX(DataCod[0], 0);
	LCD_HEX(DataCod[1], 4);
	LCD_HEX(DataCod[2], 8);
	LCD_HEX(DataCod[3], 12);
//	while(1);
#endif

	if (Decode_TM2002() == 0) return(0);
	if ((DataCod[3] 	& 0x01) == 0) DataCod[3] 	 |= 0x01; else DataCod[3] 	  &= 0xFE; 
	if ((DataCodTemp[3] & 0x01) == 0) DataCodTemp[3] |= 0x01; else DataCodTemp[3] &= 0xFE; 
	if (Decode_TM2002() == 0) return(0);
	return(1);
}

unsigned char ReadAnalog(void)	
{
	INTCON = 0; 
	OSCCON = 0b01100000;	// 48.0MHz
	T0CON = 0b10010000;		// Enabled, 16bit, 1:2
	if (ReadCyfral(2) == 0) 
	{
		DataCodMem[0] = DataCodTemp[0]; 
		DataCodMem[1] = DataCodTemp[1]; 
		DataCodMem[2] = DataCodTemp[2]; 
		DataCodMem[3] = DataCodTemp[3];
		OSCCON = 0b01100010;	// 8.0MHz
		return(CYFRAL);
	}
	if (ReadMetacom(2) == 0)
	{
		DataCodMem[0] = DataCodTemp[0]; 
		DataCodMem[1] = DataCodTemp[1]; 
		DataCodMem[2] = DataCodTemp[2]; 
		DataCodMem[3] = DataCodTemp[3];
		OSCCON = 0b01100010;	// 8.0MHz
		return(TM2002);
	}
	OSCCON = 0b01100010;	// 8.0MHz
	return(NO_KEY);
}


