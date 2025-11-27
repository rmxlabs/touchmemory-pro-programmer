#pragma once

#include "HEXEncoder.h"

#define MY_DEVICE_ID  "Vid_04d8&Pid_553c"

// For device validation
#define ENCODED_NUM_XOR_PC			0xA3
#define ENCODED_NUM_XOR_BL			0x58

//*********************** BOOTLOADER COMMANDS ******************************
#define	CMD_VALIDATE				0x50    // Validate the device and version
#define CMD_ERASE_DEVICE			0x51	// Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define CMD_PROGRAM_DEVICE			0x52	// If host is going to send a full RequestDataBlockSize to be programmed, it uses this command.
#define	CMD_PROGRAM_COMPLETE		0x53	// If host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define CMD_GET_DATA				0x54	// The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	CMD_RESET_DEVICE			0x55	// Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

#define	CMD_LEAVEUSBMODE			0xF0	// END USB Mode, goto main app
//**************************************************************************

#define USER_ID_ADDRESS			0x200000
#define USER_ID_BLOCK_SIZE		8


#define CMD_PROG_BLOCK_SIZE		0x3A

#pragma pack(1)
typedef union _BOOTLOADER_COMMAND
{
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char Pad[63];
	} CmdWithoutParams;
	
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char RandNum;
		unsigned char EncodedNum;
		unsigned char Pad[61];
	} CmdValidate;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned char RandNum;
		unsigned char EncodedNum;
		unsigned short MajorVer;
		unsigned short MinorVer;
		unsigned int  ProgramMemoryStart;
		unsigned int  ProgramMemoryEnd;
		unsigned char Pad[49];
	} CmdValidateResult;

	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned int  Address;
		unsigned char BytesPerPacket;
		unsigned char Data[58];
	} CmdProgramDevice;

	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned int  Address;
		unsigned char BytesPerPacket;
		unsigned char Pad[58];
	} CmdGetData;
	struct
	{
		unsigned char WindowsReserved;
		unsigned char Command;
		unsigned int  Address;
		unsigned char BytesPerPacket;
		unsigned char Data[58];
	} CmdGetDataResults;

	struct
	{
		unsigned char WindowsReserved;
		unsigned char Data[64];
	} PacketData;

	unsigned char RawData[65];

} BOOTLOADER_COMMAND;
#pragma pack()

class USBBootloader
{
public:
	static GUID InterfaceClassGuid;

	USBBootloader(void);
	~USBBootloader(void);

	bool SearchForDevice(void);
	bool ValidateDevice(void);
	bool ProgramDevice(CProgressCtrl* pProgress);
	bool VerifyDevice(CProgressCtrl* pProgress);
	void ResetDevice(void);
	void SendLeaveUSBMode(void);

	bool IsConnected(void)	  { return _isConnected; }
	bool IsValid(void)		  { return _isValid; }
	

	bool FillBuffers( const CStringArray* pHEX );

	const CString GetVersion(void);

protected:
	unsigned long StringToHex(const CString& strData);


private:
	PSP_DEVICE_INTERFACE_DETAIL_DATA MyStructureWithDetailedInterfaceDataInIt;
	
	unsigned short  _majorVersion; //2 BYTES
	unsigned short  _minorVersion; //2 BYTES
	
	
	unsigned long	_addressStartOfProgramSpace;
	unsigned long	_addressEndOfProgramSpace;
	unsigned long	_sizeOfFirmware;

	unsigned char	*pBufferFirmware;
	unsigned char	*pBufferID;

	bool	_isConnected;
	bool	_isValid;


	DWORD ErrorStatusWrite;
	DWORD ErrorStatusRead;
};
