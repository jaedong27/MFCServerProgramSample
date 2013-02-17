
// MFCServerProgramSampleDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CMFCServerProgramSampleDlg dialog
class CMFCServerProgramSampleDlg : public CDialogEx
{
// Construction
public:
	CMFCServerProgramSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCSERVERPROGRAMSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CSocket m_Socket_Server;
	CSocket m_Socket_Accepted;
	UINT m_Handle_AcceptedSocket;

public:
	UINT static RecvThread(LPVOID pParam);
	
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonSend();
	CEdit m_Edit_Port;
	CEdit m_Edit_SendData;
	CEdit m_Edit_RecvData;
};
