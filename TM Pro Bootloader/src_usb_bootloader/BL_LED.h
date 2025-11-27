#ifndef __BL_LED_H
#define __BL_LED_H

#define BLED_2		LATBbits.LATB4
#define BLED_1		LATBbits.LATB5
//#define REG_SCK		LATBbits.LATB6

#define BL_LED_OFF		0
#define BL_LED_OK		4
#define BL_LED_ERROR	8
#define BL_LED_READ		1
#define BL_LED_WRITE	2


extern unsigned char BL_LedData;
extern unsigned char BL_LedSave;


extern void BL_LED(void);

#endif
