#include "p18cxxx.h"
#include "delay.h"
#include "lcd.h"
#include "rfid.h"
#include "rw1990.h"
#include "analog.h"
#include "utils.h"
#include "interrupt.h"

#pragma udata

unsigned char i, res, key;

unsigned char GenL, GenH, gh, gl;

extern unsigned char DataCod[4];
extern unsigned char DataCodMem[4];

//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

#ifdef LCD

rom char txt_read[] ={"Read...         "};         
rom char txt_write[]={"Write...        "};         

#endif


void MainApp(void)
{
	STKPTR = 0;
	StackPointerInit();
	OSCCON = 0b01100010;	// 8.0MHz
	OSCTUNEbits.SPLLEN = 1;

	INTCON  = 0b00000000;
	INTCON2 = 0b1111000;
	INTCON3 = 0b0000000;
    ADCON0  = 0x00;
	CM1CON0 = 0x00;
	CM2CON0 = 0x00;
	PIE1 = 0;
	PIE2 = 0;
	IPR1 = 0;
	IPR2 = 0;
	RCONbits.IPEN = 0;

	WPUA = 0x00;
	WPUB = 0x00;
	ANSEL = 0b01111000;
	ANSELH = 0b00000000;

	LATCbits.LATC0 = 1;
	LATCbits.LATC1 = 1;
	LATCbits.LATC2 = 1;


#ifdef WRITE_DEBUG
	TRISCbits.TRISC6 = 0; LATCbits.LATC6 = 0;
#else
	TRISCbits.TRISC6 = 1;
#endif

	LATCbits.LATC3 = 0; TRISCbits.TRISC3 = 0;

	LATCbits.LATC5 = 0; TRISCbits.TRISC5 = 0;

 	LATCbits.LATC4 = 0; TRISCbits.TRISC4 = 0;
 	LATBbits.LATB7 = 0; TRISBbits.TRISB7 = 0;
 	LATCbits.LATC7 = 0; TRISCbits.TRISC7 = 0;

	LATBbits.LATB4 = 0; TRISBbits.TRISB4 = 0;
	LATBbits.LATB5 = 0; TRISBbits.TRISB5 = 0;
	LATBbits.LATB6 = 0; TRISBbits.TRISB6 = 0;



//	ADCON2 = 0b00111110;
//	ADCON1 = 0b00000000;
//	ADCON0 = 0b00011000;
//	ADCON0bits.ADON = 1;


	T0CON = 0b10010000;		// Enabled, 16bit, 1:2
	T1CON = 0b10100101;		// Enabled, 16bit, 1:4
	T2CON = 0b00000100;		// Enabled, 1:1, 1:1
	T3CON = 0b10101001;		// Enabled, 16bit, 1:4
	PIR2bits.TMR3IF = 0;
	PIE2bits.TMR3IE = 1;


	ECCP1AS = 0b00000000;
	PWM1CON = 0b00000000;
	PSTRCON = 0b00000101;
//	CCP1CON = 0b00111100;

	PR2 = 0xFF;
	CCPR1L = 0x80;

//	TM_VPP = 0;
//	TM_VDD = 0;

#ifdef LCD
	InitLCD();
	LCD_setCOMMAND(LCD_RESET);
	Delay1KTCYx(12);
	LCD_CLR();
#endif
	Beep();

	do {
		LED(LED_READ);

		#ifdef LCD
			LCD_CLR();
			LCD_Display(txt_read);
		#endif

		OSCCON = 0b01100010;	// 8.0MHz
		PSTRCON = 0b00000101;
		CCP1CON = 0b00001100;
		PR2 = 0x0F;
		CCPR1L = 0x08;
		Delay10TCYx(10);
		do {
			key = NO_KEY; 
			if (key == NO_KEY) key = ReadRFID();
			if (key == NO_KEY) key = OneReadDS1990(1);
//			if (key == NO_KEY) key = ReadAnalog();
		} while (key == NO_KEY);
		LED(LED_WRITE);
		Beep(); 

		if (key == EM4100) 
		{
			#ifdef LCD
				LCD_code(EM4100);
			#endif
			RFIDFree(5);
//			LED(LED_OFF);

			for(i = 0; i < 8; i++) wr_buff[i] = tm_buff[i];
			res = WriteRFID(10);
			#ifdef LCD
				LCD_Error(res);
			#endif
			if (res == 0)
			{
				LED(LED_OFF);
				Beep();
				RFIDFree(10);
			}
			if (res == 2)
			{
				LongBeep(); 
				RFIDFree(20);
			}
		}
		if (key == DS1990) 
		{
			#ifdef LCD
				LCD_code(DS1990);
			#endif
			WaitNoDS1990(10);
			LED(LED_WRITE);

			for(i = 0; i < 8; i++) wr_buff[i] = tm_buff[i];
			if (WaitDS1990(20, 400) == 0)
			{
				#ifdef LCD
					LCD_Display(txt_write);
				#endif

				res = Write_DS();

				#ifdef LCD
					LCD_Error(res);
				#endif
				if (res == 0)
				{	
					LED(LED_OFF);
					Beep();
				}
				else
				{
					LED(LED_ERROR);
					ErrorBeep();
				}
				WaitNoDS1990(20);
			} 
			else 
			{ 
				LongBeep(); 
				WaitNoDS1990(20);
			}
		}
/*
		if ((key == CYFRAL) || (key == TM2002))
		{
			#ifdef LCD
				LCD_code(key);
			#endif
			WaitNoAnaolog(10);
			LED(LED_WRITE);

			if (WaitAnaolog(20, 400) == 0)
			{
				#ifdef LCD
					LCD_Display(txt_write);
				#endif

				res = WrAnalog(key);

				#ifdef LCD
					LCD_Error(res);
				#endif
				if (res == 0)
				{
					LED(LED_OK);
					Beep();
				}
				else
				{
					LED(LED_ERROR);
					ErrorBeep();
				}
				WaitNoAnaolog(20);
			} 
			else 
			{ 
				LongBeep(); 
			}
		} 
*/
	} while(1);	

}

