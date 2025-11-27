// TMRF_FirmwareEncoderDlg.h : header file
//

#pragma once

#include "hexencoder.h"
#include "afxwin.h"

// CFirmwareEncoderDlg dialog
class CFirmwareEncoderDlg : public CDialog
{
// Construction
public:
	CFirmwareEncoderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TMRF_FIRMWAREENCODER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonFindfile();
	afx_msg void OnBnClickedButtonEncode();

private:
	HEXEncoder	_encoder;
public:
	CString m_strFileName;
	CString m_strVersion;
	CButton m_btnEncode;
};
