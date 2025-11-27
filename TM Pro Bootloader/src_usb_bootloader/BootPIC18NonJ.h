#ifndef BOOTPIC18NONJ_H
#define BOOTPIC18NONJ_H

/** P U B L I C  P R O T O T Y P E S *****************************************/
void BootloaderInit(void);
void ProcessIO(void);

//BootState Variable States
#define	BootNotFound				0x00
#define	Idle						0x01
#define NotIdle						0x02

extern unsigned char BootState;

#endif //BOOTPIC18NONJ_H
