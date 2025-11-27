
#include "p18cxxx.h"
#include "delays.h"
#include "rw1990.h"
#include "utils.h"


//////////////////////////////////////////////////////
#pragma code MAINAPP
//////////////////////////////////////////////////////

unsigned char Compare(unsigned char *buff1, unsigned char *buff2, unsigned char len)
{
	unsigned char pos;

	pos = 0;
	while(len > 0)
	{
		if (buff1[pos] != buff2[pos]) return(1); 
		len--; pos++;
	}
	return(0);
}

void WaitNoDS1990(unsigned char wait_time)
{
	unsigned char n;

	n = wait_time;
	do {
		Delay10KTCYx(5);
        if (TmReset() != 0) n--; else n = wait_time;
	} while(n != 0);
}


unsigned char ReadDS1990(void)
{
	unsigned char i, buff[10];

	if (TmReset() == 0)
	{
		Tm_Read();
		if (CheckCRC() == 0)
		{
			for(i = 0; i < 8; i++) buff[i] = tm_buff[i];
			if (TmReset() == 0)
			{
				Tm_Read();
				if (CheckCRC() == 0) 
				{
					for(i = 0; i < 8; i++) { if (buff[i] != tm_buff[i]) return(2); }
					return(0);
				}
			}
		}
		return(2);
	}
	return(1);
}

unsigned char DetectDS1990(unsigned int time)
{
	unsigned int count;

	count = time;
	do {
		if (TmReset() == 0) count--; else count = time;
	} while (count > 0);
}

unsigned char WaitDS1990(unsigned int time, unsigned int max)
{
	unsigned int count;

	count = time;
	do {
		Delay10KTCYx(5);
		if (TmReset() == 0) count--; else count = time;
		max--;
		if (max == 0) return(1);
	} while (count > 0);
	return(0);
}


unsigned char OneReadDS1990(unsigned char wait_time)
{
	do {
		if (ReadDS1990() == 0) return(DS1990);
 	} while (--wait_time);
	return(NO_KEY); 
}


unsigned char Write_DS(void)
{
	unsigned char WrRes, WriteCount;

	WriteCount = 5;
	do {
		WrRes = 0xFF;

 		if (Detect_IZ() == 0)
		{
			WrRes = IZ_WF(1);
			if (WrRes == 0) WrRes = IZ_Write();
			if (WrRes == 0) WrRes = IZ_WF(0);
		}
		else
		{
			if (Detect_TM() == 0)
			{
				WrRes = TM2004Write();
			}
			else
			{
				WrRes = RW_WF(1);
				if (WrRes == 0) WrRes = RW_Write();
				if (WrRes == 0) WrRes = RW_WF(0);
			}
		}
		if (WrRes == 0)
		{ 	
			WrRes = 2;
			if (OneReadDS1990(20) == DS1990) 
			{ 
				if (Compare(tm_buff, wr_buff, 8) == 0) WrRes = 0;
			}
		}
		if (WrRes == 1) return(1);
		WriteCount--; 
	} while ((WriteCount != 0) && (WrRes != 0));
	return(WrRes);
}
