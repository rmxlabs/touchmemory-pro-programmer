#include "p18cxxx.h"
#include "utils.h"
#include "delay.h"
#include "rfid.h"
#include "rfid_tx.h"

#pragma udata

unsigned char TXBuff[8];

//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

void Delay(void)
{
	Delay1KTCYx(240);
	Delay1KTCYx(140);
//	Delay1KTCYx(240);
}


void RFRestart(void)
{
	PSTRCON = 0b00000001;
	Delay();
	PSTRCON = 0b00000101;
	Delay();
}

void WriteEM4100(void)
{
	PackEM();

	WriteRF(0x01);
	Delay();

	TXBuff[0] = TXBuff[4];
	TXBuff[1] = TXBuff[5];
	TXBuff[2] = TXBuff[6];
	TXBuff[3] = TXBuff[7];

	WriteRF(0x02);
	Delay();

	TXBuff[0] = 0b00010000;
	TXBuff[1] = 0b00010100;
	TXBuff[2] = 0b10000000;
	TXBuff[3] = 0b01000000; 

	WriteRF(0x00);
	Delay();
}

unsigned char RFIDWriteEM4100(void)
{
	unsigned char n, res;

//	RFRestart();
	WriteEM4100(); 
	RFRestart();
	n = 3;
	do
	{
		tm_buff[0] = 0;
		tm_buff[1] = 0;
		tm_buff[2] = 0;
		tm_buff[3] = 0;
		tm_buff[4] = 0;
		tm_buff[5] = 0;
		tm_buff[6] = 0;
		tm_buff[7] = 0;
		res = ReciveRFID();
		n--;
	} while ((n != 0) && (res != 0));
	if (res != 0) return(1);	
	if ((tm_buff[1] == wr_buff[1]) && (tm_buff[2] == wr_buff[2]) && (tm_buff[3] == wr_buff[3]) && (tm_buff[4] == wr_buff[4]) && (tm_buff[5] == wr_buff[5])) return(0);
	return(1);	
}


unsigned char WriteRFID(unsigned char time)
{
	do {
		if (RFIDWriteEM4100() == 0) return(0);
	} while (--time);
	return(2);
}

unsigned char ScanRFID(void)
{
	unsigned int speed;

	if (SCAN_RFID() != 0) return(1);
	return(0);
}


unsigned char ReadRFID(void)
{
	if (SCAN_RFID() != 0) return(NO_KEY);
	if(ReciveRFID() != 0) return(NO_KEY);
	tm_buff[0] = 0;
	tm_buff[6] = 0;
	tm_buff[7] = 0;
	return(EM4100);
}


void RFIDFree(unsigned char time)
{
	unsigned char i;

	i = time;
	do
	{
		if (ScanRFID() == 0) i = time;
		i--;
	} while (i != 0);
}


