// TMRF-FirmwareUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMRF-FirmwareUpdate.h"
#include "TMRF-FirmwareUpdateDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//Register for WM_DEVICECHANGE notifications.  We want windows messages for
	//plug and play events, that could signal attachment or detachment of our USB device.
	DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
	MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
	MyDeviceBroadcastHeader.dbcc_classguid = USBBootloader::InterfaceClassGuid;
	RegisterDeviceNotification(this->GetSafeHwnd(), &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CFirmwareUpdateDlg dialog




CFirmwareUpdateDlg::CFirmwareUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirmwareUpdateDlg::IDD, pParent)
	, m_strLog(_T(""))
	, m_strUsbState(_T(""))
	, m_strVersion(_T(""))
	, m_strFilePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFirmwareUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_editLog);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLog);
	DDX_Text(pDX, IDC_USB_STATE, m_strUsbState);
	DDX_Text(pDX, IDC_VERSION, m_strVersion);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpenFile);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btnProgram);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFilePath);
	DDX_Control(pDX, IDC_PROGRESS, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CFirmwareUpdateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DEVICECHANGE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CFirmwareUpdateDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CFirmwareUpdateDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CFirmwareUpdateDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CFirmwareUpdateDlg message handlers

BOOL CFirmwareUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);

	AddMessageToLog(_T("Старт программы"));


	_bootloader.SearchForDevice();
	if(_bootloader.IsConnected())
	{
		if(_bootloader.ValidateDevice())
		{			
			m_strUsbState = _T(" Связь установлена. Версия: ") + _bootloader.GetVersion();
			AddMessageToLog(_T("Связь установлена. Версия ") + _bootloader.GetVersion());
			m_btnOpenFile.EnableWindow(TRUE);
		}
		else
		{
			m_strUsbState = _T(" Ошибка установки связи!");
		}
	}
	else
	{
		m_strUsbState =  _T(" Устройство не найдено!");
	}
	UpdateData(FALSE);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

HBRUSH CFirmwareUpdateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

   if (pWnd->GetDlgCtrlID() == IDC_USB_STATE)
   {
	   if(_bootloader.IsValid())
	   {
			pDC->SetTextColor(RGB(0, 0, 255));
	   }
	   else
	   {
			pDC->SetTextColor(RGB(255, 0, 0));
	   }
	   pDC->SetBkMode(TRANSPARENT);
   }
   

   return hbr;
}

void CFirmwareUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFirmwareUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFirmwareUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFirmwareUpdateDlg::DoEvents()
{
   MSG stMsg = { 0 };
   while( ::PeekMessage( &stMsg, this->GetSafeHwnd(), 0, 0, PM_REMOVE ))
   {
       ::TranslateMessage( &stMsg );
	   ::DispatchMessage( &stMsg );
    }
}

void CFirmwareUpdateDlg::AddMessageToLog(const CString& message, bool lines)
{
	CString strNew;
	CTime t(time(NULL));
	
	
	strNew.Format(L"%.2d:%.2d:%.2d  ", t.GetHour(), t.GetMinute(), t.GetSecond());
	strNew = strNew + message + CString(L"\r\n");
	
	if(lines)
		m_strLog = CString(L"\t==============\r\n") + strNew + 
				   CString(L"\t==============\r\n") + m_strLog;
	else
		m_strLog = strNew + m_strLog;


	UpdateData(FALSE);
	DoEvents();
}

void CFirmwareUpdateDlg::ClearLog()
{
	m_editLog.SetWindowTextW(L"");
}


BOOL CFirmwareUpdateDlg::OnDeviceChange(UINT nEventType, DWORD_PTR dwData)
{
	if(nEventType == DBT_DEVICEARRIVAL)
	{
		TRACE("WM_DEVICECHANGE: DBT_DEVICEARRIVAL");
	}
	if(nEventType == DBT_DEVICEREMOVECOMPLETE)
	{
		TRACE("WM_DEVICECHANGE: DBT_DEVICEREMOVECOMPLETE");
	}
	if(nEventType == DBT_DEVICEREMOVEPENDING)
	{
		TRACE("WM_DEVICECHANGE: DBT_DEVICEREMOVEPENDING");
	}

	bool wasConnected = _bootloader.IsConnected();

	_bootloader.SearchForDevice();
	if(_bootloader.IsConnected())
	{
		if(_bootloader.IsValid() == false)
		{
			if(_bootloader.ValidateDevice())
			{
				if(!wasConnected)
				{
					m_strUsbState = _T(" Связь установлена. Версия: ") + _bootloader.GetVersion();
					AddMessageToLog(_T("Связь установлена. Версия ") + _bootloader.GetVersion());

					m_btnOpenFile.EnableWindow(TRUE);
				}
			}
			else
			{
				m_btnOpenFile.EnableWindow(FALSE);
				m_btnProgram.EnableWindow(FALSE);

				m_strUsbState = _T(" Ошибка установки связи!");
			}
		}
	}
	else
	{
		m_btnOpenFile.EnableWindow(FALSE);
		m_btnProgram.EnableWindow(FALSE);

		m_strUsbState = _T(" Устройство не найдено!");
	}
	UpdateData(FALSE);
	Invalidate();

	return CDialog::OnDeviceChange(nEventType, dwData);
}

void CFirmwareUpdateDlg::OnClose()
{	
	if(_bootloader.IsConnected() && _bootloader.IsValid())
	{
		_bootloader.SendLeaveUSBMode();
	}
	CDialog::OnClose();
}


void CFirmwareUpdateDlg::OnBnClickedCancel()
{
	if(_bootloader.IsConnected() && _bootloader.IsValid())
	{
		_bootloader.SendLeaveUSBMode();
	}
	OnCancel();
}

void CFirmwareUpdateDlg::OnBnClickedButtonLoad()
{
	AddMessageToLog(_T("Запуск процесса обновления"), true);
	UpdateData(FALSE);
	bool bErr = true;

	if(_bootloader.ProgramDevice(&m_ctrlProgress))
	{
		AddMessageToLog(_T("Запись -> OK!"));
		if(_bootloader.VerifyDevice(&m_ctrlProgress))
		{
			AddMessageToLog(_T("Проверка записи -> OK!"));
			bErr = false;
		}
		else
		{
			AddMessageToLog(_T("Проверка записи -> ERROR!"));
		}
	}
	else
	{
		AddMessageToLog(_T("Запись -> ERROR!"));
	}

	if(bErr == false)
	{
		AddMessageToLog(_T("ОБНОВЛЕНИЕ ЗАВЕРШЕНО УСПЕШНО!!!"), true);
	}
	else
	{
		AddMessageToLog(_T("ОБНОВЛЕНИЕ ЗАВЕРШЕНО С ОШИБКОЙ!!!"), true);
	}

	_bootloader.ResetDevice();

}

void CFirmwareUpdateDlg::OnBnClickedButtonOpen()
{
	CFileDialog dlg(TRUE, L"tmfw", NULL, OFN_FILEMUSTEXIST, L"Файлы прошивок (*.tmfw)|*.tmfw||");
	if(dlg.DoModal() == IDOK)
	{
		bool isError = false;

		if(_decoder.DecodeToBuffer(dlg.GetPathName()))
		{
			if(_bootloader.FillBuffers(_decoder.GetBuffer()))
			{
				m_strFilePath = dlg.GetPathName();
				m_strVersion = _decoder.GetVersionFromBuffer();
				m_btnProgram.EnableWindow(TRUE);

				ClearLog();
				AddMessageToLog(_T("Файл открыт..."));

				UpdateData(FALSE);
			}
			else
			{
				AddMessageToLog(_T("Подготовка буффера данных -> ERROR!"));
				m_btnProgram.EnableWindow(FALSE);

			}
		}
		else
		{
			AddMessageToLog(_T("Декодирование файла прошивки -> ERROR!"));
			m_btnProgram.EnableWindow(FALSE);
		}
	}
}
