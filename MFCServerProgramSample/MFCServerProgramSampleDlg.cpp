
// MFCServerProgramSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCServerProgramSample.h"
#include "MFCServerProgramSampleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCServerProgramSampleDlg dialog




CMFCServerProgramSampleDlg::CMFCServerProgramSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCServerProgramSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCServerProgramSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Edit_Port);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_Edit_SendData);
	DDX_Control(pDX, IDC_EDIT_RECV_DATA, m_Edit_RecvData);
}

BEGIN_MESSAGE_MAP(CMFCServerProgramSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCServerProgramSampleDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMFCServerProgramSampleDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CMFCServerProgramSampleDlg message handlers

BOOL CMFCServerProgramSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCServerProgramSampleDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCServerProgramSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCServerProgramSampleDlg::OnBnClickedButtonOpen()
{
	CString stringport;
	m_Edit_Port.GetWindowText(stringport);
	UINT port = _ttoi(stringport); 

	// TODO: Add your control notification handler code here
	if( !m_Socket_Server.Create(port) )
	{
		TRACE("Socket Create Fail");
		return;
	}

	if( !m_Socket_Server.Listen())
	{
		TRACE("Socket Create OK");
		return;
	}

	if( !m_Socket_Server.Accept(m_Socket_Accepted))
	{
		return;
	}
	m_Handle_AcceptedSocket = m_Socket_Accepted.Detach();
	CWinThread* pThread = AfxBeginThread(RecvThread, (LPVOID)this);
	m_Socket_Accepted.Attach(m_Handle_AcceptedSocket);
}


void CMFCServerProgramSampleDlg::OnBnClickedButtonSend()
{
	CString str;
	int length;

	m_Edit_SendData.GetWindowText(str);
	byte* send_buffer = (byte *)(LPCTSTR(str));
	length = str.GetLength();

	m_Socket_Accepted.Send(send_buffer, length);
}

UINT CMFCServerProgramSampleDlg::RecvThread (LPVOID pParam)
{
	CSocket recv_socket;
	BYTE recv_buffer[100];
	CString text_output, add_string;
	CEdit* edit_recv = &(((CMFCServerProgramSampleDlg *)pParam)->m_Edit_RecvData);
	UINT recv_data_size;

	//여기에서 사용할 Packet을 불러옴
	recv_socket.Attach((SOCKET) ((CMFCServerProgramSampleDlg *)pParam)->m_Handle_AcceptedSocket);

	while(1)
	{
		memset(recv_buffer, 0, 100);
		recv_data_size = recv_socket.Receive(recv_buffer,100);

		// 원래 있던 Window 뒤에다가 받은 데이터를 Append
		edit_recv->GetWindowText(text_output);
		add_string.Format(_T("%s"),(char *)recv_buffer);
		text_output.Append(add_string);

		edit_recv->SetWindowText(text_output);
		Sleep(1);
	}

	return 0;
}