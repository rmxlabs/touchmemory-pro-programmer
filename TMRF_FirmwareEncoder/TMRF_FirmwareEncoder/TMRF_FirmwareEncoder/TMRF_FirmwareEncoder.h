// TMRF_FirmwareEncoder.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CFirmwareEncoderApp:
// See TMRF_FirmwareEncoder.cpp for the implementation of this class
//

class CFirmwareEncoderApp : public CWinApp
{
public:
	CFirmwareEncoderApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFirmwareEncoderApp theApp;