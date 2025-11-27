#pragma once



class HEXEncoder
{
	static const unsigned int MagicNumber;
public:
	HEXEncoder(void);
	~HEXEncoder(void);

	bool	DecodeToBuffer(const CString& strInFileName);
	bool	Encode(const CString& strOutFileName);

	CString	GetVersion(void);
	CString	GetVersionFromBuffer(void);

	void	SetInputFileName(const CString& name) { m_strInputFileName = name; }

	CStringArray*	GetBuffer(void) { return &m_arrOfHEXData; } 
private:
	CString			m_strInputFileName;
	CStringArray	m_arrOfHEXData;
	
protected:
	unsigned long StringToHex(const CString& strData);
};
