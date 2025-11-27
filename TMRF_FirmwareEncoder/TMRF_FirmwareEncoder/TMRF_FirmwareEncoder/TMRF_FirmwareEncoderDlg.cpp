// TMRF_FirmwareEncoderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMRF_FirmwareEncoder.h"
#include "TMRF_FirmwareEncoderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFirmwareEncoderDlg dialog




CFirmwareEncoderDlg::CFirmwareEncoderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFirmwareEncoderDlg::IDD, pParent)
	, m_strFileName(_T(""))
	, m_strVersion(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFirmwareEncoderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INPUT_FILE, m_strFileName);
	DDX_Control(pDX, IDC_BUTTON_ENCODE, m_btnEncode);
}

BEGIN_MESSAGE_MAP(CFirmwareEncoderDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CFirmwareEncoderDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_FINDFILE, &CFirmwareEncoderDlg::OnBnClickedButtonFindfile)
	ON_BN_CLICKED(IDC_BUTTON_ENCODE, &CFirmwareEncoderDlg::OnBnClickedButtonEncode)
END_MESSAGE_MAP()


// CFirmwareEncoderDlg message handlers

BOOL CFirmwareEncoderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.




	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFirmwareEncoderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFirmwareEncoderDlg::OnPaint()
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
HCURSOR CFirmwareEncoderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFirmwareEncoderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CFirmwareEncoderDlg::OnBnClickedButtonFindfile()
{
	CFileDialog dlg(TRUE, L"hex", NULL, OFN_FILEMUSTEXIST, L"Microchip HEX (*.hex)|*.hex||");
	if(dlg.DoModal() == IDOK)
	{
		m_strFileName = dlg.GetPathName();
		_encoder.SetInputFileName(m_strFileName);
		m_strVersion = _encoder.GetVersion();

		if(m_strVersion.GetLength() != 0)
		{
			m_btnEncode.EnableWindow(TRUE);
		}
		else
		{
			m_btnEncode.EnableWindow(FALSE);
			m_strFileName = _T("Файл не содержит версии прошивки!");
		}
		UpdateData(FALSE);
	}
}

void CFirmwareEncoderDlg::OnBnClickedButtonEncode()
{
	CString strDefaultName = L"firmwareTMRF" + m_strVersion;
	
	CFileDialog dlg(FALSE, L"tmfw", strDefaultName, OFN_OVERWRITEPROMPT, L"Файлы прошивок (*.tmfw)|*.tmfw||");
	if(dlg.DoModal() == IDOK)
	{
		if(_encoder.Encode(dlg.GetPathName()) == true)
		{
			AfxMessageBox(_T("Файл зашифрован и сохранён"), MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox(_T("Ошибка при сохранении файла"), MB_OK | MB_ICONERROR);
		}
	}
}
