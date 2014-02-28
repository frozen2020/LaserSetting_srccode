// LaserSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LaserSetting.h"
#include "LaserSettingDlg.h"
#include "string.h"
#include "ComConfigDlg.h"
#include "windows.h" 
#include "SkinH.h"
#pragma comment (lib,"SkinHu.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ZERO 48
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserSettingDlg dialog

CLaserSettingDlg::CLaserSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserSettingDlg)
	m_bDIOOn = FALSE;
	m_fIDI = 0.0f;
	m_bSHTOn = FALSE;
	m_bQSOn = FALSE;
	m_bQSEXT = FALSE;
	m_nDelay = 150;
	m_strSend = _T("");
	m_fPWM = 100.0f;
	m_fPWM_QUERY = 0.0f;
	m_lPRF_QUERY = 0.0f;
	m_lPRF = 20;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_ComConfig.strBaudRate = _T("9600");
	m_ComConfig.strParity = _T("e");
	m_ComConfig.strDataBits = _T("8");
	m_ComConfig.strStopbits = _T("1");
	m_ComConfig.strDataControl = _T("n");
	m_nComIndex = 0;
	m_strReceive = _T("");
	m_waitEvent.Create();
	m_bIDIChange = false;
	m_bPRFChange = false;
	m_bPWMChange = false;
	m_nListCount = 0;
}

void CLaserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserSettingDlg)
	DDX_Control(pDX, IDC_LIST_COMMAD, m_ListCommad);
	DDX_Control(pDX, IDC_COMBO_COM, m_cbCom);
	DDX_Check(pDX, IDC_CHECK_DIO, m_bDIOOn);
	DDX_Text(pDX, IDC_EDIT_IDI, m_fIDI);
	DDX_Check(pDX, IDC_CHECK_SHT, m_bSHTOn);
	DDX_Check(pDX, IDC_CHECK_QS, m_bQSOn);
	DDX_Check(pDX, IDC_CHECK_EXT, m_bQSEXT);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_nDelay);
	DDV_MinMaxUInt(pDX, m_nDelay, 0, 3000);
	DDX_Text(pDX, IDC_EDIT_PWM, m_fPWM);
	DDX_Text(pDX, IDC_EDIT_PWM2, m_fPWM_QUERY);
	DDX_Text(pDX, IDC_EDIT_PRF2, m_lPRF_QUERY);
	DDX_Text(pDX, IDC_EDIT_PRF, m_lPRF);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLaserSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserSettingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_COMSET, OnButtonComset)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_CHECK_DIO, OnCheckDio)
	ON_EN_CHANGE(IDC_EDIT_IDI, OnChangeEditIdi)
	ON_EN_CHANGE(IDC_EDIT_PRF, OnChangeEditPrf)
	ON_BN_CLICKED(IDC_CHECK_SHT, OnCheckSht)
	ON_BN_CLICKED(IDC_CHECK_QS, OnCheckQs)
	ON_BN_CLICKED(IDC_CHECK_EXT, OnCheckExt)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	ON_EN_CHANGE(IDC_EDIT_PWM, OnChangeEditPwm)
	ON_BN_CLICKED(IDC_CLOSEDIG, OnClosedig)
	ON_EN_KILLFOCUS(IDC_EDIT_IDI, OnKillfocusEditIdi)
	ON_EN_KILLFOCUS(IDC_EDIT_PRF, OnKillfocusEditPrf)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRecv)
	ON_EN_KILLFOCUS(IDC_EDIT_PWM, OnKillfocusEditPwm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserSettingDlg message handlers

BOOL CLaserSettingDlg::OnInitDialog()//窗体初始化函数
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
	
	// TODO: Add extra initialization here

	m_cbCom.SetCurSel(0);
	m_ComLaser.InitCom();
	
	m_ListCommad.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	m_ListCommad.InsertColumn(0,_T("ID"),LVCFMT_LEFT,100);
	m_ListCommad.InsertColumn(1,_T("List Command"),LVCFMT_LEFT,250);
	//SkinH_Attach();
	SkinH_AttachEx("E:\tang\LaserSetting_srccode\Aero.she", NULL);
	SkinH_SetAero(TRUE);
	UpdateData(FALSE);
	return TRUE;  
}

void CLaserSettingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLaserSettingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLaserSettingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLaserSettingDlg::OnButtonComset() 
{
	// TODO: Add your control notification handler code here
	CComConfigDlg	dlg;
	dlg.DoModal();
}

void CLaserSettingDlg::OnButtonConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nComIndex = m_cbCom.GetCurSel();
	char strConbuf[256];
	CString		strConnect;
	strConnect.Format(_T("%s,%s,%s,%s"),m_ComConfig.strBaudRate,m_ComConfig.strParity,m_ComConfig.strDataBits,m_ComConfig.strStopbits);

	strcpy(strConbuf,strConnect);

	if( m_ComLaser.Open(nComIndex+1,strConbuf) )
	{	
		m_nComIndex = nComIndex +1;
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
		m_ComLaser.SetWnd(m_hWnd); 
			
		//m_ComLaser.Write("PROMPT=ON\n");
		/*m_ComLaser.ClearInputBuffer();
		m_strReceive = _T("");
		m_ComLaser.Write("?DIO\n");
		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
		{
			if( m_strReceive.CompareNoCase("000000") == 0 )
			{
				m_bDIOOn = FALSE;
			}
			else if( m_strReceive.CompareNoCase("000001") == 0  )//Karen 这个函数使用lstrcmpi函数对一个CString和另一个CString进行比较
			{
				m_bDIOOn = TRUE;
			}
			else if( m_strReceive.CompareNoCase("ERROR ") == 0 )
			{
				;
			}
		}*/
		/*m_ComLaser.ClearOutputBuffer();
		m_ComLaser.Write("?QSW\n");
		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
		{
			if( m_strReceive.CompareNoCase("000000") == 0 )
			{
				m_bQSOn = FALSE;
			}
			else if( m_strReceive.CompareNoCase("000001") == 0  )
			{
				m_bQSOn = TRUE;
			}
			else if( m_strReceive.CompareNoCase("ERROR ") == 0 )
			{
				;
			}
		}*/
	}
	else
	{
		CString		strCaption;
		strCaption.Format(_T("COM%d:%s,%s,%s,%s connect error!"),nComIndex+1,m_ComConfig.strBaudRate,m_ComConfig.strParity,m_ComConfig.strDataBits,m_ComConfig.strStopbits);
		AfxMessageBox(strCaption);
	}

	UpdateData(FALSE);
}

void CLaserSettingDlg::OnButtonCancel()
{
	// TODO: Add your control notification handler code here
	m_ComLaser.Close();
	GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_GET)->EnableWindow(FALSE);
}

LRESULT CLaserSettingDlg::OnComRecv(WPARAM wParam, LPARAM lParam)
{	
	CString		strread;
	if( wParam == m_nComIndex )
	{	
		char buf[1024];
		ZeroMemory(buf,sizeof(buf));
		strread = m_ComLaser.ReadString(buf, 1024);

		m_strReceive += strread;
		UpdateData(false);
		m_waitEvent.Set();
	}
	return 0;                                                                                    
}

void CLaserSettingDlg::OnButtonSend()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	CString		strSend;
	char chTemp[11];
	ZeroMemory(chTemp,11);
	m_strReceive = _T("");  
	if( m_bIDIChange )
	{	
		float ntemp = (m_fIDI * 10.0);
		longToString(ntemp,chTemp);
		strSend.Format(_T("IDI=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);
		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
		SetListCommad(strSend);
		m_bIDIChange = FALSE;

		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
		{	
			//AfxMessageBox(m_strReceive);
		}
	}
	
	ZeroMemory(chTemp,11);

	if( m_bPRFChange )
	{	
		longToString((m_lPRF*1000),chTemp);
		strSend.Format(_T("PRF=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);

		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
		SetListCommad(strSend);
		m_bPRFChange = FALSE;

		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
		{	
			//AfxMessageBox(m_strReceive);
		}
	}


	ZeroMemory(chTemp,11);

	if( m_bPWMChange )
	{	
		longToString((420-(long)m_fPWM*4),chTemp);
		strSend.Format(_T("PWM=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);

		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
		SetListCommad(strSend);
		m_bPWMChange = FALSE;

		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )//等待命令响应完成
		{	
			//AfxMessageBox(m_strReceive);
		}
	}
}

void CLaserSettingDlg::OnCheckDio() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	if( m_bDIOOn )
	{
		//m_ComLaser.Write(_T("DIO=000001\n"));
		SendCommon(_T("DIO=000001\n"),11);
		SetListCommad(_T("DIO=000001\n"));
	}
	else
	{	
		//m_ComLaser.Write(_T("DIO=000000\n"));
		SendCommon(_T("DIO=000000\n"),11);
		SetListCommad(_T("DIO=000000\n"));
	}
	
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
}

void CLaserSettingDlg::OnChangeEditIdi() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CLaserSettingDlg::OnChangeEditPrf() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}
void CLaserSettingDlg::longToString(ULONG itemp, char *p)
{
	*p = (unsigned char)(itemp >> 20 ) + ZERO;
	*(p+1) = (unsigned char)(( itemp & 0x0F0000 ) >> 16 ) + ZERO;
	*(p+2) = (unsigned char)(( itemp & 0x00F000 ) >> 12 ) + ZERO;
	*(p+3) = (unsigned char)(( itemp & 0x000F00 ) >> 8 ) + ZERO;
	*(p+4) = (unsigned char)(( itemp & 0x0000F0 ) >> 4 ) + ZERO;
	*(p+5) = (unsigned char)( itemp & 0x00000F ) + ZERO;
}

unsigned long CLaserSettingDlg::StringToLong(char *p)
{
	unsigned long temp,value;
	temp = ( p[0]-ZERO );
	temp = temp<<20;
	value = temp;
	
	temp = ( p[1]-ZERO );
	temp = temp<<16;
	value += temp;

	temp = ( p[2]-ZERO );
	temp = temp<<12;
	value += temp;
	
	temp = ( p[3]-ZERO );
	temp = temp<<8;
	value += temp;
	
	temp = ( p[4]-ZERO );
	temp = temp<<4;
	value += temp;

	temp = ( p[5]-ZERO );
	value += temp;

	return value;
}

void CLaserSettingDlg::OnCheckSht() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	if( m_bSHTOn )
	{
		//m_ComLaser.Write(_T("SHT=000001\n"));
		SendCommon(_T("SHT=000001\n"),11);
		SetListCommad(_T("SHT=000001\n"));
	}
	else
	{	
		//m_ComLaser.Write(_T("SHT=000000\n"));
		SendCommon(_T("SHT=000000\n"),11);
		SetListCommad(_T("SHT=000000\n"));
	}	

	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
}

void CLaserSettingDlg::OnCheckQs() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	if( m_bQSOn )
	{
		//m_ComLaser.Write(_T("QSW=000001\n"));
		SendCommon(_T("QSW=000001\n"),11);
		SetListCommad(_T("QSW=000001\n"));
	}
	else
	{	
		//m_ComLaser.Write(_T("QSW=000000\n"));
		SendCommon(_T("QSW=000000\n"),11);
		SetListCommad(_T("QSW=000000\n"));
	}

	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
}

void CLaserSettingDlg::OnCheckExt() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");
	if( m_bQSEXT )
	{
		//m_ComLaser.Write(_T("EXT=000001\n"));
		SendCommon(_T("EXT=000001\n"),11);
		SetListCommad(_T("EXT=000001\n"));
	}
	else
	{	
		//m_ComLaser.Write(_T("EXT=000000\n"));
		SendCommon(_T("EXT=000000\n"),11);
		SetListCommad(_T("EXT=000000\n"));
	}

	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
}

void CLaserSettingDlg::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	//DIO
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");
	SendCommon("?DIO\n",5);
	SetListCommad("?DIO\n");
	m_waitEvent.Reset();
	long  lValue = 0;
	float  fValue = 0.0;//karen add 2012-02-20
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		m_bDIOOn = (BOOL)lValue;
	}
	UpdateData(FALSE);
	
	//QSW
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?QSW\n",5);
	SetListCommad("?QSW\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		m_bQSOn = (BOOL)lValue;
	}
	UpdateData(FALSE);
	
	//EXT
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?EXT\n",5);
	SetListCommad("?EXT\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		m_bQSEXT = (BOOL)lValue;
	}
	UpdateData(FALSE);
	
	//SHT
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?SHT\n",5);
	SetListCommad("?SHT\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		m_bSHTOn = (BOOL)lValue;
	}
	UpdateData(FALSE);

	//IDI
	char	chTemp[16];

	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?IDI\n",5);
	SetListCommad("?IDI\n");
	m_waitEvent.Reset();
	char   bufIDI[6];
	CString tempbufIDI;
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp);		
		
		tempbufIDI.Format(_T("%.1f"),lValue/10.0);        
        strncpy(bufIDI,(LPCTSTR)tempbufIDI,sizeof(bufIDI));
		GetDlgItem(IDC_EDIT_IDIVALUE)->SetWindowText(bufIDI);
	
	}
	UpdateData(FALSE);

	//TEV
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TEV\n",5);
	SetListCommad("?TEV\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp);
		
		CString tempbufTEV;
		char bufTEV[6];
		tempbufTEV.Format(_T("%.1f"),lValue/10.0);
		strncpy(bufTEV,(LPCTSTR)tempbufTEV,sizeof(bufTEV));
		GetDlgItem(IDC_EDIT_TEV)->SetWindowText(bufTEV);       
	}
	UpdateData(FALSE);

	//TLA
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TLA\n",5);
	SetListCommad("?TLA\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp);	

		CString tempbufTLA;
		char bufTLA[6];
		tempbufTLA.Format(_T("%.1f"),lValue/10.0);
		strncpy(bufTLA,(LPCTSTR)tempbufTLA,sizeof(bufTLA));
		GetDlgItem(IDC_EDIT_TLA)->SetWindowText(bufTLA);
    
	}
	UpdateData(FALSE);

	//TDI
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TDI\n",5);
	SetListCommad("?TDI\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp);
        
		CString tempbufTDI;
		char bufTDI[6];
		tempbufTDI.Format(_T("%.1f"),lValue/10.0);
		strncpy(bufTDI,(LPCTSTR)tempbufTDI,sizeof(bufTDI));
        GetDlgItem(IDC_EDIT_TDI)->SetWindowText(bufTDI);
        
	}
	UpdateData(FALSE);
	//karen add 2012-02-20 begin

	//?PRF
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?PRF\n",5);
	SetListCommad("?PRF\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp);		
		m_lPRF_QUERY = lValue;
	}
	UpdateData(FALSE);
	
	//?PWM
	Sleep(100);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?PWM\n",5);
	SetListCommad("?PWM\n");
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);		
		lValue = StringToLong(chTemp);		
		m_fPWM_QUERY = lValue;
	}
	UpdateData(FALSE);
	//karen add 2012-02-20 end
}


void CLaserSettingDlg::SetListCommad(LPCTSTR strListCmd)//更新列表控件
{
	if( m_nListCount >= 100 )
	{
		m_ListCommad.DeleteAllItems();
		m_nListCount = 0;
	}

	CString		strTemp;
	m_nListCount++;
	strTemp.Format(_T("%d"),m_nListCount);
	int nindex = m_ListCommad.InsertItem(0,strTemp);

	strTemp = strListCmd;
	m_ListCommad.SetItemText(nindex,1,strTemp);
}

void CLaserSettingDlg::SendCommon(char *strCommon,int nlength)
{
	char strSend[11];
	
	memcpy(strSend,strCommon,nlength);

	for( int loop = 0; loop< nlength; loop++ )//一个一个字符的发送，中间有延时
	{	
		char send[1];
		send[0] = strSend[loop];

		m_ComLaser.Write(send);
		Sleep(m_nDelay);
	}
}


void CLaserSettingDlg::OnChangeEditPwm() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}







void CLaserSettingDlg::OnClosedig() 
{
	// TODO: Add your control notification handler code here	
    ExitProcess(0);
} 

void CLaserSettingDlg::OnKillfocusEditIdi() 
{  UpdateData(TRUE);
	// TODO: Add your control notification handler code here
		if ((m_fIDI>=0) && (m_fIDI<=50))
    m_bIDIChange = TRUE;
	else
	{
		m_bIDIChange = FALSE;
		AfxMessageBox("IDI 范围应在 0~50");
	}
	UpdateData(FALSE);
}

void CLaserSettingDlg::OnKillfocusEditPrf() 
{		UpdateData(TRUE);
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if ((m_lPRF>=20)&&(m_lPRF<=150))
	m_bPRFChange = TRUE;
	else
	{m_bPRFChange = FALSE;
		AfxMessageBox("prf 范围应在 20~150");
	}
	UpdateData(FALSE);
}

void CLaserSettingDlg::OnKillfocusEditPwm() 
{	
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if ((m_fPWM>=10)&&(m_fPWM<=100))
	m_bPWMChange = TRUE;
	else
	{m_bPWMChange = FALSE;
	AfxMessageBox("PWM 范围应在 10~100");
	}
	UpdateData(FALSE);
}
