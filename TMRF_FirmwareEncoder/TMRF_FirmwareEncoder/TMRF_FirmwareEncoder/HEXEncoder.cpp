#include "StdAfx.h"
#include "HEXEncoder.h"

#include <string>
#include <sstream>

#include <cmath>

#define SIZE_OF_WRITE_BUFFER	150
const unsigned int HEXEncoder::MagicNumber = 0xAA550584;

const unsigned char RandomBlock[SIZE_OF_WRITE_BUFFER] = {
	38, 39, 246, 133, 151, 21, 173, 29, 210, 148, 221, 196, 118, 25, 57, 49, 
	241, 173, 181, 88, 240, 147, 151, 50, 25, 43, 209, 192, 253, 22, 142, 78, 
	72, 155, 11, 245, 59, 73, 168, 99, 93, 222, 63, 223, 109, 104, 180, 135, 
	154, 170, 205, 220, 247, 193, 68, 129, 41, 8, 27, 64, 98, 56, 48, 78, 
	148, 212, 17, 208, 222, 196, 17, 157, 75, 63, 156, 70, 187, 239, 199, 84, 
	33, 80, 43, 208, 239, 90, 244, 9, 207, 95, 53, 145, 148, 54, 127, 137, 
	112, 153, 177, 30, 103, 204, 17, 84, 3, 127, 156, 3, 74, 246, 155, 30, 
	237, 103, 119, 59, 194, 164, 206, 80, 116, 249, 198, 187, 122, 88, 162, 134, 
	69, 179, 147, 232, 190, 170, 208, 15, 239, 102, 232, 28, 0, 197, 87, 112, 
	102, 183, 88, 38, 87, 232  
};


HEXEncoder::HEXEncoder(void) :
	m_strInputFileName(_T(""))
{
}

HEXEncoder::~HEXEncoder(void)
{
}

bool HEXEncoder::DecodeToBuffer(const CString& strInFileName)
{
	CStdioFile fileInput;
	
	bool result = false;

	UINT magicNumReaded;
	
	int bytesReaded;
	unsigned char* buffer;
	int bufferSize;

	if(!fileInput.Open(strInFileName, CFile::modeRead | CFile::typeBinary))
	{
		return false;
	}

	bufferSize = (int) fileInput.GetLength() - SIZE_OF_WRITE_BUFFER - sizeof(HEXEncoder::MagicNumber);
	if(bufferSize < 0)
	{
		fileInput.Close();
		return false;
	}
	fileInput.Read(&magicNumReaded, 4);

	if(magicNumReaded == HEXEncoder::MagicNumber)
	{
		buffer = new unsigned char[bufferSize];

		bytesReaded = fileInput.Read(buffer, SIZE_OF_WRITE_BUFFER);

		if(bytesReaded == SIZE_OF_WRITE_BUFFER)
		{			
			unsigned char *pCurr = buffer;
			while((bytesReaded = fileInput.Read(pCurr, SIZE_OF_WRITE_BUFFER)) > 0)
			{
				for(int i = 0; i < bytesReaded; i++)
				{
					pCurr[i] = pCurr[i] ^ RandomBlock[i];
				}
				pCurr += bytesReaded;
				if(bytesReaded < SIZE_OF_WRITE_BUFFER) //EOF
					break;
			}
			result = true;
		}

		m_arrOfHEXData.RemoveAll();

		std::istringstream str;
		str.rdbuf()->pubsetbuf((char *) buffer, bufferSize);
		std::string line;
		while(std::getline(str, line))
		{
			if (*line.rbegin() == '\r') line.erase(line.end() - 1);
			m_arrOfHEXData.Add(CString(line.c_str()));
		}

		delete [] buffer;
	}

	
	fileInput.Close();
	return result;
}

bool HEXEncoder::Encode(const CString& strOutFileName)
{
	CStdioFile fileInput;
	CStdioFile fileOutput;
	 

	int bytesReaded;
	unsigned char* buffer;

	if(!fileInput.Open(m_strInputFileName, CFile::modeRead | CFile::typeBinary))
	{
		return false;
	}
	if(!fileOutput.Open(strOutFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		fileInput.Close();
		return false;
	}
	//WRITE MAGIC NUM
	fileOutput.Write(&HEXEncoder::MagicNumber, 4);

	buffer = new unsigned char[SIZE_OF_WRITE_BUFFER];
	srand(clock());
	for(int i = 0; i < SIZE_OF_WRITE_BUFFER; i++)
	{
		buffer[i] = rand()%256;
	}
	//WRITE RANDOM BLOCK
	fileOutput.Write(buffer, SIZE_OF_WRITE_BUFFER);

	//WRITE HEX DATA
	while((bytesReaded = fileInput.Read(buffer, SIZE_OF_WRITE_BUFFER)) > 0)
	{
		for(int i = 0; i < bytesReaded; i++)
		{
			buffer[i] = buffer[i] ^ RandomBlock[i];
		}
		fileOutput.Write(buffer, bytesReaded);
		if(bytesReaded < SIZE_OF_WRITE_BUFFER) //EOF
			break;
	}

	delete [] buffer;

	fileInput.Close();
	fileOutput.Close();
	return true;
}

CString HEXEncoder::GetVersion(void)
{
	CString strVersion(_T(""));
	
	CStdioFile inputFile;
	
	if(!inputFile.Open(m_strInputFileName, CFile::modeRead))
	{
		return strVersion;
	}

	CString strOfData;

	while(inputFile.ReadString(strOfData) != FALSE)
	{
		if(strOfData.GetAt(0) != L':')
			break;

		if(strOfData.Find(L":020000040020DA") != -1) //Extended address for UserID
		{
			//Read next line
			if(inputFile.ReadString(strOfData) != FALSE)
			{
				if(strOfData.GetAt(0) != L':')
					break;

				unsigned long recordLength;
				unsigned long addressField;
				unsigned long recordType;
				unsigned long checksum;
				CString strUID;
				
				strOfData = strOfData.TrimLeft(':');


				//get the record length, address, record type, data, and checksum of the line
				recordLength = StringToHex(strOfData.Mid(0,2));
				CString temp = strOfData.Mid(2,4);
				addressField = StringToHex(strOfData.Mid(2,4));
				recordType = StringToHex(strOfData.Mid(6,2));
				strUID = strOfData.Mid(8,recordLength*2);
				checksum = StringToHex(strOfData.Mid((recordLength*2)+8,2));

				unsigned char j;
				unsigned checksumCalculated;

				//Initialize the checksum value to 0
				checksumCalculated = 0;

				//For each byte in the data payload length
				for(j=0;j<(recordLength+4);j++)
				{
					//Calculate it into the checksum
					checksumCalculated += StringToHex(strOfData.Mid(j*2,2));
				}
				
				//complete the checksum calculation
				checksumCalculated = (~checksumCalculated) + 1;

				//If the calculated checksum doesn't match the checksum read
				//  from the file
				if((checksumCalculated & 0x000000FF) != checksum)
				{
					break;					
				}

				unsigned char majorVersionB1 = (unsigned char)StringToHex(strUID.Mid(0,2));
				unsigned char majorVersionB2 = (unsigned char)StringToHex(strUID.Mid(2,2));
				unsigned short majorVersion = majorVersionB1 | (majorVersionB2 << 8);

				unsigned char minorVersionB1 = (unsigned char)StringToHex(strUID.Mid(4,2));
				unsigned char minorVersionB2 = (unsigned char)StringToHex(strUID.Mid(6,2));
				unsigned short minorVersion = minorVersionB1 | (minorVersionB2 << 8);

				strVersion.Format(L"_%u_%u", majorVersion, minorVersion);
			}
			else
			{
				break; // Error goto end
			}
		}
	}

	inputFile.Close();
	
	return strVersion;
}

CString HEXEncoder::GetVersionFromBuffer(void)
{
	CString strVersion(_T(""));
	CString strOfData;

	m_arrOfHEXData.GetSize();
	for(int i = 0; i < m_arrOfHEXData.GetSize(); i++)
	{
		strOfData = m_arrOfHEXData.GetAt(i);

		if(strOfData.GetAt(0) != L':')
			break;

		if(strOfData.Find(L":020000040020DA") != -1) //Extended address for UserID
		{
			//Read next line
			if(i < m_arrOfHEXData.GetSize() - 1)
			{			
				strOfData = m_arrOfHEXData.GetAt(i + 1);
				i = i + 1;

				if(strOfData.GetAt(0) != L':')
					break;

				unsigned long recordLength;
				unsigned long addressField;
				unsigned long recordType;
				unsigned long checksum;
				CString strUID;
				
				strOfData = strOfData.TrimLeft(':');


				//get the record length, address, record type, data, and checksum of the line
				recordLength = StringToHex(strOfData.Mid(0,2));
				CString temp = strOfData.Mid(2,4);
				addressField = StringToHex(strOfData.Mid(2,4));
				recordType = StringToHex(strOfData.Mid(6,2));
				strUID = strOfData.Mid(8,recordLength*2);
				checksum = StringToHex(strOfData.Mid((recordLength*2)+8,2));

				unsigned char j;
				unsigned checksumCalculated;

				//Initialize the checksum value to 0
				checksumCalculated = 0;

				//For each byte in the data payload length
				for(j=0;j<(recordLength+4);j++)
				{
					//Calculate it into the checksum
					checksumCalculated += StringToHex(strOfData.Mid(j*2,2));
				}
				
				//complete the checksum calculation
				checksumCalculated = (~checksumCalculated) + 1;

				//If the calculated checksum doesn't match the checksum read
				//  from the file
				if((checksumCalculated & 0x000000FF) != checksum)
				{
					break;					
				}

				unsigned char majorVersionB1 = (unsigned char)StringToHex(strUID.Mid(0,2));
				unsigned char majorVersionB2 = (unsigned char)StringToHex(strUID.Mid(2,2));
				unsigned short majorVersion = majorVersionB1 | (majorVersionB2 << 8);

				unsigned char minorVersionB1 = (unsigned char)StringToHex(strUID.Mid(4,2));
				unsigned char minorVersionB2 = (unsigned char)StringToHex(strUID.Mid(6,2));
				unsigned short minorVersion = minorVersionB1 | (minorVersionB2 << 8);

				strVersion.Format(L"_%u_%u", majorVersion, minorVersion);
			}
			else
			{
				break; // Error goto end
			}
		}
	}
	
	return strVersion;
}

unsigned long HEXEncoder::StringToHex(const CString& strData)
{
	unsigned long result;
	unsigned long placeMultiplier;
	unsigned char i;
	wchar_t c;

	result = 0;
	placeMultiplier = 1;

	for(i=0; i < strData.GetLength(); i++)
	{
		c = strData[strData.GetLength() - 1 - i];
		if((c >= 'A') && (c <= 'F'))
		{
			c = 10 + (c - 'A');
		}
		else if((c >= 'a') && (c <= 'f'))
		{
			c = 10 + (c - 'a');
		}
		else
		{
			c = c - '0';
		}

		result += (c * placeMultiplier);
		placeMultiplier *= 16;
	}

	return result;
}
