#ifndef __UTILS_H
#define __UTILS_H

//**********************************************************************************************
// Target device
//
//#define WRITER
//#define HI_VOLTAGE
//
//**********************************************************************************************
// Options
//
//#define WRITE_DEBUG
//#define LCD
//#define READ_WET_CODE
//
//**********************************************************************************************

#define BUZZ_1		LATCbits.LATC5
#define BUZZ_2		LATCbits.LATC7
//#define TM_VDD		LATBbits.LATB7

#define LED_2		LATBbits.LATB4
#define LED_1		LATBbits.LATB5
//#define REG_SCK		LATBbits.LATB6

#define LCD_E		LATBbits.LATB5
#define LCD_DAT		LATBbits.LATB4
#define LCD_CLK		LATBbits.LATB6

#define TM_IN		PORTAbits.RA3
#define TM_OUT		LATCbits.LATC4

#ifdef WRITE_DEBUG
	#define DEBUG		LATCbits.LATC6		// RF IN
#endif

#define EEPROM_COUNTER_ADDR		0

#define LED_OFF		0
#define LED_OK		1
#define LED_ERROR	2
#define LED_READ	4
#define LED_WRITE	8

#define NO_KEY		0
#define DS1990		1
#define CYFRAL		2
#define TM2002		4
#define EM4100		8


#define __OPTIMIZE_OFF	_asm nop _endasm

typedef union _WORD
{
    unsigned int w;
    struct
    {
        unsigned char l;
        unsigned char h;
    };
} word;


extern unsigned char tm_buff[8];
extern unsigned char wr_buff[8];
extern unsigned char LedData;

unsigned char Decode_Cyfral(void);

void LED(unsigned char led);

void StackPointerInit(void);

void LCDSend(unsigned char data, unsigned char rs);

void Tone(unsigned char freq, unsigned int duration);
void Beep(void);
void LongBeep(void);
void ErrorBeep(void);

void Encode_Cyfral(unsigned char CodeHi, unsigned char CodeLow);

unsigned char EEPROMRead(unsigned char Addr);
void EEPROMWrite(unsigned char Addr, unsigned char Byte);
void NonLinearInc(unsigned char *DataH, unsigned char *DataL);


#endif
