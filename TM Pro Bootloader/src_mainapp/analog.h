#ifndef __ANALOG_H
#define __ANALOG_H


// ------------------------------------------------------------------------
// пусто : 0xAD
// TM2002: 0x53-0x36
// Zag   : 0x81

#define  READ_DET_VALUE		0x98
#define  READ_MIN_VALUE		0x10

// ------------------------------------------------------------------------

extern unsigned char DataCodMem[4];
extern unsigned char DataCodTemp[4];

extern unsigned char KeyMode;

unsigned char ReadCyfral(unsigned char rep);
unsigned char ReadMetacom(unsigned char rep);

void WaitNoAnaolog(unsigned char time);
unsigned char WaitAnaolog(unsigned char time, unsigned int max);

unsigned char ScanKey(void);

unsigned char ReadAnalog(void);
unsigned char WriteAnalog(unsigned char key);
unsigned char WrAnalog(unsigned char key);


#endif
