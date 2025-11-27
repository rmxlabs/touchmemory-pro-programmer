#ifndef __RW1990_H
#define __RW1990_H


unsigned char ReadDS1990(void);
void WaitNoDS1990(unsigned char wait_time);
unsigned char OneReadDS1990(unsigned char wait_time);
unsigned char DetectDS1990(unsigned int time);

unsigned char WaitDS1990(unsigned int time, unsigned int max);
unsigned char Write_DS(void);
unsigned char Detect_TM(void);
unsigned char TM2004Write(void);


unsigned char TmReset(void);
void Tm_Read(void);
unsigned char CheckCRC(void);

unsigned char RW_Write(void);
unsigned char RW_WF(unsigned char flag);

unsigned char Detect_IZ(void);

unsigned char IZ_Write(void);
unsigned char IZ_WF(unsigned char flag);


#endif
