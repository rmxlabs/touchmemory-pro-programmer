// TMRF-FirmwareUpdateDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "USBBootloader.h"
#include "HEXEncoder.h"
#include "afxcmn.h"

// CFirmwareUpdateDlg dialog
class CFirmwareUpdateDlg : public CDialog
{
// Construction
public:
	CFirmwareUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TMRFFIRMWAREUPDATE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void DoEvents(void);


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD_PTR dwData);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonOpen();


	void AddMessageToLog(const CString& message, bool lines = false);
	void ClearLog();

private:
	CEdit m_editLog;
	CString m_strLog;

	///////////////////////////
	USBBootloader _bootloader;
	HEXEncoder	  _decoder;
	///////////////////////////
	CString m_strUsbState;
	CString m_strVersion;
	CButton m_btnOpenFile;
	CButton m_btnProgram;
	CString m_strFilePath;
	CProgressCtrl m_ctrlProgress;
};
