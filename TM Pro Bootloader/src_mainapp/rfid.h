#ifndef __RFID_H
#define __RFID_H


unsigned char ReadRFID(void);
unsigned char WriteRFID(unsigned char time);
void RFIDFree(unsigned char time);
unsigned char ReciveRFID(void);
unsigned char SCAN_RFID(void);
#endif
