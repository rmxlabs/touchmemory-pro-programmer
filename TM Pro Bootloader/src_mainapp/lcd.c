
#include "p18cxxx.h"
#include "delay.h"
#include "lcd.h"
#include "utils.h"
#include "analog.h"

#ifdef LCD

#pragma udata

char LCD_buffer[40];

unsigned char HiCifr;					// старшая цифра
unsigned char LowCifr;              	// младшая цифра

extern unsigned char DataCod[4];


//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

// ------------------------------------------------------------------------
void InitLCD(void) 
{
	unsigned char i;

	LCD_E = 0;
	LCD_DAT = 0;
	LCD_CLK = 0;

	TRISBbits.TRISB4 = 0;
	TRISBbits.TRISB5 = 0;
	TRISBbits.TRISB6 = 0;

 	for (i = 0; i < 32; i++) LCD_buffer[i] = 0;

	Delay1KTCYx(20*8/4);			// задержка 20мс

	LCD_Command(LCD_2L4B);			// set 2-line mode LCD
	LCD_Command(LCD_2L4B);			// set 2-line mode LCD
	LCD_Command(LCD_2L4B);			// set 2-line mode LCD
	LCD_Command(LCD_OFF);			// off LCD
	LCD_Command(LCD_RESET);			// reset LCD
	Delay1KTCYx(2*8/4);			// задержка 2мс
	LCD_Command(LCD_ON);			// ON LCD
	Delay1KTCYx(20*8/4);			// задержка 20мс
	Delay1KTCYx(20*8/4);			// задержка 20мс
	LCD_Command(LCD_LINE1);;  // set line_2 LCD с начала;
	LCD_RAM(LCD_buffer);
	LCD_Command(LCD_LINE2);;  // set line_2 LCD с начала;
	LCD_RAM(LCD_buffer);
}

// ------------------------------------------------------------------------
void LCD_Command(unsigned char cc)
{
	LCDSend(cc >> 4, 0);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0; 
	// младший полубайт
	LCDSend(cc & 0x0F, 0);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0;
	Delay10TCYx(64);				// задержка ~ 50 мкс
}

// ------------------------------------------------------------------------
void LCD_setCOMMAND(unsigned char cc)
{
	// старший полубайт
	LCDSend(cc >> 4, 0);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0;
	// младший полубайт
	LCDSend(cc & 0x0F, 0);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0;
}

// ------------------------------------------------------------------------
void LCD_Data(unsigned char cc)
{		
	// старший полубайт
	LCDSend(cc >> 4, 1);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0;
	// младший полубайт
	LCDSend(cc & 0x0F, 1);
	LCD_E = 1; Delay10TCYx(4); LCD_E = 0;
	Delay10TCYx(100);
}


// ------------------------------------------------------------------------
// --- вывод строки из ПЗУ на LCD  -----
//  -- строка д. заканчиваться нулевым символом (0x00)
void LCD_POS(unsigned char pos)
{
	if (pos < 16) LCD_Command(LCD_LINE1+pos); else LCD_Command(LCD_LINE2+pos-16);
//	LCD_Command(0x00);		
}

// ------------------------------------------------------------------------
// --- вывод строки из ПЗУ на LCD  -----
//  -- строка д. заканчиваться нулевым символом (0x00)
void LCD_ROM(const rom char *s)
{
 	unsigned char i;
  
 	i = 0; while (s[i] != 0) LCD_Data(s[i++]);
}

// ------------------------------------------------------------------------
// --- вывод строки из ОЗУ на LCD  -----
//  -- строка д. заканчиваться нулевым символом (0x00)
void LCD_RAM(char *s)
{
 	unsigned char i;
  
	i = 0; while (s[i] != 0) LCD_Data(s[i++]);
}

// ------------------------------------------------------------------------
void Dec_Hex(unsigned char c)	// дешифратор Hex-кода числа в 2 выводимые цифры
{    
    HiCifr = c >> 4 & 0x0F;
	if (HiCifr > 9) HiCifr += 7;

	LowCifr = c &0x0F;
	if (LowCifr > 9) LowCifr += 7;

	LowCifr += 0x30;
	HiCifr += 0x30;
}

// ------------------------------------------------------------------------
void LCD_CLR(void)
{
	unsigned char i;

	for (i = 0; i < 16; i++) LCD_buffer[i]=' ';
	LCD_buffer[16] = 0x00;
	LCD_Command(LCD_LINE1);
	LCD_RAM(LCD_buffer);
	LCD_Command(LCD_LINE2);
	LCD_RAM(LCD_buffer);
}

// ------------------------------------------------------------------------
void LCD_HEX(unsigned char data, unsigned char pos)
{
	unsigned char i, c;

	c = data >> 4;   if (c > 9) c = c + 7; LCD_buffer[pos] = c + 0x30;
	c = data & 0x0F; if (c > 9) c = c + 7; LCD_buffer[pos+1] = c + 0x30;

	LCD_buffer[32] = 0;
	LCD_POS(16);
	LCD_RAM(&LCD_buffer[16]);

	LCD_buffer[16] = 0;
	LCD_POS(0);
	LCD_RAM(LCD_buffer);
}

// ------------------------------------------------------------------------
// вывод на LCD прочитанного кода
void LCD_code(unsigned char mode)
{
    unsigned char i;

	for (i = 0; i < 32; i++) LCD_buffer[i]=' ';

	if (mode == EM4100) 
	{
		LCD_buffer[0]='E'; 
		LCD_buffer[1]='m'; 
		LCD_buffer[2]='M'; 
		LCD_buffer[3]='a';
		LCD_buffer[4]='r'; 
		LCD_buffer[5]='i'; 
		LCD_buffer[6]='n'; 
						
   		Dec_Hex(tm_buff[5]);
   		LCD_buffer[16]=HiCifr; LCD_buffer[17]=LowCifr;	
   		Dec_Hex(tm_buff[4]);
   		LCD_buffer[18]=HiCifr; LCD_buffer[19]=LowCifr;	
   		Dec_Hex(tm_buff[3]);
		LCD_buffer[20]=HiCifr; LCD_buffer[21]=LowCifr;	
   		Dec_Hex(tm_buff[2]);
		LCD_buffer[22]=HiCifr; LCD_buffer[23]=LowCifr;	
   		Dec_Hex(tm_buff[1]);
		LCD_buffer[24]=HiCifr; LCD_buffer[25]=LowCifr;	
	}

	if (mode == DS1990) 
	{
		LCD_buffer[0]='D'; 
		LCD_buffer[1]='a'; 
		LCD_buffer[2]='l'; 
		LCD_buffer[3]='l';
		LCD_buffer[4]='a'; 
		LCD_buffer[5]='s'; 
						
   		Dec_Hex(tm_buff[0]);
   		LCD_buffer[16]=HiCifr; LCD_buffer[17]=LowCifr;	
   		Dec_Hex(tm_buff[6]);
   		LCD_buffer[18]=HiCifr; LCD_buffer[19]=LowCifr;	
   		Dec_Hex(tm_buff[5]);
		LCD_buffer[20]=HiCifr; LCD_buffer[21]=LowCifr;	
   		Dec_Hex(tm_buff[4]);
		LCD_buffer[22]=HiCifr; LCD_buffer[23]=LowCifr;	
   		Dec_Hex(tm_buff[3]);
		LCD_buffer[24]=HiCifr; LCD_buffer[25]=LowCifr;	
   		Dec_Hex(tm_buff[2]);
		LCD_buffer[26]=HiCifr; LCD_buffer[27]=LowCifr;	
   		Dec_Hex(tm_buff[1]);
		LCD_buffer[28]=HiCifr; LCD_buffer[29]=LowCifr;	
   		Dec_Hex(tm_buff[7]);
		LCD_buffer[30]=HiCifr; LCD_buffer[31]=LowCifr;	
	}

	if (mode == TM2002) 
	{
		LCD_buffer[0]='M'; 
		LCD_buffer[1]='e'; 
		LCD_buffer[2]='t'; 
		LCD_buffer[3]='a';
		LCD_buffer[4]='c'; 
		LCD_buffer[5]='o'; 
		LCD_buffer[6]='m';

   		Dec_Hex(DataCod[0]);
   		LCD_buffer[16]=HiCifr; LCD_buffer[17]=LowCifr;	
   		Dec_Hex(DataCod[1]);
   		LCD_buffer[18]=HiCifr; LCD_buffer[19]=LowCifr;	
		Dec_Hex(DataCod[2]);
		LCD_buffer[20]=HiCifr; LCD_buffer[21]=LowCifr;	
		Dec_Hex(DataCod[3]);
		LCD_buffer[22]=HiCifr; LCD_buffer[23]=LowCifr;	
	}

	if (mode == CYFRAL)
	{
		LCD_buffer[0]='C'; 
		LCD_buffer[1]='y'; 
		LCD_buffer[2]='f'; 
		LCD_buffer[3]='r';
		LCD_buffer[4]='a'; 
		LCD_buffer[5]='l'; 

   		Dec_Hex(DataCod[0]);
   		LCD_buffer[16]=HiCifr; LCD_buffer[17]=LowCifr;	
   		Dec_Hex(DataCod[1]);
   		LCD_buffer[18]=HiCifr; LCD_buffer[19]=LowCifr;	
		Dec_Hex(DataCod[2]);
		LCD_buffer[20]=HiCifr; LCD_buffer[21]=LowCifr;	
	}	
	LCD_buffer[32] = 0;
	LCD_POS(0);
	LCD_RAM(LCD_buffer);
	LCD_POS(16);
	LCD_RAM(&LCD_buffer[16]);
}

// ------------------------------------------------------------------------

void LCD_Error(unsigned char err)
{
//rr = '1';
	if (err == 0)
	{
		LCD_buffer[0]='O';
		LCD_buffer[1]='K';
		LCD_buffer[2]='.';
		LCD_buffer[3]=' ';
		LCD_buffer[4]=' ';
	}
	else
	{
		LCD_buffer[0]='E';
		LCD_buffer[1]='r';
		LCD_buffer[2]='r';
		LCD_buffer[3]=':';
		LCD_buffer[4]=err;
	}
	LCD_buffer[5]=' ';
	LCD_buffer[6]=' ';
	LCD_buffer[7]=' ';
	LCD_buffer[8]=' ';
	LCD_buffer[9]=' ';
	LCD_buffer[10]=' ';
	LCD_buffer[11]=' ';
	LCD_buffer[12]=' ';
	LCD_buffer[13]=' ';
	LCD_buffer[14]=' ';
	LCD_buffer[15]=' ';
	LCD_buffer[16]=' ';
	LCD_buffer[17]=0x00;        
	LCD_POS(16);
	LCD_RAM(LCD_buffer);
}

// ------------------------------------------------------------------------


void LCD_Display(rom char *text)
{
	LCD_POS(0);
	LCD_ROM(text);
}

#endif
//=============================================================================================================
