// LaserSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LaserSetting.h"
#include "LaserSettingDlg.h"
#include "string.h"
#include "ComConfigDlg.h"
#include "windows.h" 
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
		m_nDelay = 2;
	m_strSend = _T("");
	m_fWPWM = 100.0f;
	m_fRPWM = 0.0f;
	m_lRPRF = 0.0f;
	m_lWPRF = 20;
	m_QS = 0;
	m_ESH = 0;
	m_DIO = 0;
	m_fRSHG = 0.0f;
	m_fRTDI = 0.0f;
	m_fRTHG = 0.0f;
	m_fRIDI = 0.0f;
	m_fWIDI = 0.0f;
	m_fWSHG = 0.0f;
	m_fWTDI = 0.0f;
	m_fWTHG = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconRED = AfxGetApp()->LoadIcon(IDI_ICON2);
	m_hIconGREEN=AfxGetApp()->LoadIcon(IDI_ICON3);
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
	nComIndex=0;
}

void CLaserSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserSettingDlg)
	DDX_Control(pDX, IDC_STATIC_QS, m_CSQS);
	DDX_Control(pDX, IDC_STATIC_ESH, m_CSESH);
	DDX_Control(pDX, IDC_STATIC_DIO, m_CSDIO);
	DDX_Control(pDX, IDC_STATIC_COM, m_CSCOM);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_cButton);
	DDX_Text(pDX, IDC_EDIT_WPWM, m_fWPWM);
	DDX_Text(pDX, IDC_EDIT_RPWM, m_fRPWM);
	DDX_Text(pDX, IDC_EDIT_RPRF, m_lRPRF);
	DDX_Text(pDX, IDC_EDIT_WPRF, m_lWPRF);
	DDX_CBIndex(pDX, IDC_COMBO_QS, m_QS);
	DDX_CBIndex(pDX, IDC_COMBO_ESH, m_ESH);
	DDX_CBIndex(pDX, IDC_COMBO_DIO, m_DIO);
	DDX_Text(pDX, IDC_EDIT_RSHG, m_fRSHG);
	DDX_Text(pDX, IDC_EDIT_RTDI, m_fRTDI);
	DDX_Text(pDX, IDC_EDIT_RTHG, m_fRTHG);
	DDX_Text(pDX, IDC_EDIT_RIDI, m_fRIDI);
	DDX_Text(pDX, IDC_EDIT_WIDI, m_fWIDI);
	DDX_Text(pDX, IDC_EDIT_WSHG, m_fWSHG);
	DDX_Text(pDX, IDC_EDIT_WTDI, m_fWTDI);
	DDX_Text(pDX, IDC_EDIT_WTHG, m_fWTHG);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_cButton);
}

BEGIN_MESSAGE_MAP(CLaserSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserSettingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_EN_CHANGE(IDC_EDIT_PWM, OnChangeEditPwm)
	ON_BN_CLICKED(IDC_CLOSEDIG, OnClosedig)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DIO, OnSelchangeComboDio)
	ON_CBN_SELCHANGE(IDC_COMBO_ESH, OnSelchangeComboEsh)
	ON_CBN_SELCHANGE(IDC_COMBO_QS, OnSelchangeComboQs)
	ON_EN_KILLFOCUS(IDC_EDIT_WIDI, OnKillfocusEditWidi)
	ON_EN_KILLFOCUS(IDC_EDIT_WPRF, OnKillfocusEditWprf)
	ON_EN_KILLFOCUS(IDC_EDIT_WPWM, OnKillfocusEditWpwm)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRecv)
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
	GetCom();
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

//	m_cbCom.SetCurSel(0);
	m_ComLaser.InitCom();

	//m_ListCommad.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	SetTimer(1,5000,NULL);//定义时钟1，时间间隔为500ms

	//m_ListCommad.InsertColumn(0,_T("ID"),LVCFMT_LEFT,100);
	//m_ListCommad.InsertColumn(1,_T("List Command"),LVCFMT_LEFT,250);
	SkinH_Attach();
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

//DEL void CLaserSettingDlg::OnButtonComset() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CComConfigDlg	dlg;
//DEL 	dlg.DoModal();
//DEL }

void CLaserSettingDlg::OnButtonConnect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char strConbuf[256];
	CString		strConnect;
	strConnect.Format(_T("%s,%s,%s,%s"),m_ComConfig.strBaudRate,m_ComConfig.strParity,m_ComConfig.strDataBits,m_ComConfig.strStopbits);

	strcpy(strConbuf,strConnect);

	if( m_ComLaser.Open(nComIndex+1,strConbuf) )
	{	
		m_nComIndex = nComIndex +1;
		GetDlgItem(IDC_BUTTON_CANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
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
				m_DIO = FALSE;
			}
			else if( m_strReceive.CompareNoCase("000001") == 0  )//Karen 这个函数使用lstrcmpi函数对一个CString和另一个CString进行比较
			{
				m_DIO = TRUE;
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
				m_ESH = FALSE;
			}
			else if( m_strReceive.CompareNoCase("000001") == 0  )
			{
				m_ESH = TRUE;
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
		float ntemp = (m_fWIDI * 10.0);
		longToString(ntemp,chTemp);
		strSend.Format(_T("IDI=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);
		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
	
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
		longToString((m_lWPRF*1000),chTemp);
		strSend.Format(_T("PRF=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);

		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
	
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
		longToString((420-(long)m_fWPWM*4),chTemp);
		strSend.Format(_T("PWM=%s\n"),chTemp);
		//m_ComLaser.Write(strSend);

		memcpy(chTemp,strSend,11);
		SendCommon(chTemp,11);
	
		m_bPWMChange = FALSE;

		m_waitEvent.Reset();
		if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )//等待命令响应完成
		{	
			//AfxMessageBox(m_strReceive);
		}
	}
}

//DEL void CLaserSettingDlg::OnCheckDio() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	m_ComLaser.ClearOutputBuffer();
//DEL 	m_ComLaser.ClearInputBuffer();
//DEL 	m_strReceive = _T("");
//DEL 
//DEL 	if( m_DIO )
//DEL 	{
//DEL 		//m_ComLaser.Write(_T("DIO=000001\n"));
//DEL 		SendCommon(_T("DIO=000001\n"),11);
//DEL 	}
//DEL 	else
//DEL 	{	
//DEL 		//m_ComLaser.Write(_T("DIO=000000\n"));
//DEL 		SendCommon(_T("DIO=000000\n"),11);
//DEL 	}
//DEL 	
//DEL 	m_waitEvent.Reset();
//DEL 	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
//DEL 	{	
//DEL 		//AfxMessageBox(m_strReceive);
//DEL 	}
//DEL }

//DEL void CLaserSettingDlg::OnChangeEditIdi() 
//DEL {
//DEL 	// TODO: If this is a RICHEDIT control, the control will not
//DEL 	// send this notification unless you override the CDialog::OnInitDialog()
//DEL 	// function and call CRichEditCtrl().SetEventMask()
//DEL 	// with the ENM_CHANGE flag ORed into the mask.
//DEL 	
//DEL 	// TODO: Add your control notification handler code here
//DEL }

//DEL void CLaserSettingDlg::OnChangeEditPrf() 
//DEL {
//DEL 	// TODO: If this is a RICHEDIT control, the control will not
//DEL 	// send this notification unless you override the CDialog::OnInitDialog()
//DEL 	// function and call CRichEditCtrl().SetEventMask()
//DEL 	// with the ENM_CHANGE flag ORed into the mask.
//DEL 	
//DEL 	// TODO: Add your control notification handler code here
//DEL }
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

void CLaserSettingDlg::GetCom()
{
    //程序启动时获取全部可用串口
    HANDLE  hCom;
    int i,num,k;
    CString str;
    BOOL flag;
    
    ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->ResetContent();
    flag = FALSE;
    num = 0;
    for (i = 1;i <= 16;i++)
    {//此程序支持16个串口
        str.Format("\\\\.\\COM%d",i);
        hCom = CreateFile(str, 0, 0, 0, 
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if(INVALID_HANDLE_VALUE != hCom )
        {//能打开该串口，则添加该串口
            CloseHandle(hCom);
            str = str.Mid(4);
            ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->AddString(str);
            if (flag == FALSE)
            {
                flag = TRUE;
                num = i;
            }
        }
    }
    i = ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->GetCount();
    if (i == 0)
    {//若找不到可用串口则禁用“打开串口”功能
        ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->EnableWindow(FALSE);
    }
    else
    {
        k = ((CComboBox *)GetDlgItem((IDC_COMBO_COM)))->GetCount();
        ((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(k - 1);
        nComIndex=i;
    }
}


//DEL void CLaserSettingDlg::OnCheckQs() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	m_ComLaser.ClearOutputBuffer();
//DEL 	m_ComLaser.ClearInputBuffer();
//DEL 	m_strReceive = _T("");
//DEL 
//DEL 	if( m_ESH )
//DEL 	{
//DEL 		//m_ComLaser.Write(_T("QSW=000001\n"));
//DEL 		SendCommon(_T("QSW=000001\n"),11);
//DEL 	}
//DEL 	else
//DEL 	{	
//DEL 		//m_ComLaser.Write(_T("QSW=000000\n"));
//DEL 		SendCommon(_T("QSW=000000\n"),11);	}
//DEL 
//DEL 	m_waitEvent.Reset();
//DEL 	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
//DEL 	{	
//DEL 		//AfxMessageBox(m_strReceive);
//DEL 	}
//DEL }

//DEL void CLaserSettingDlg::OnCheckExt() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	UpdateData(TRUE);
//DEL 	m_ComLaser.ClearOutputBuffer();
//DEL 	m_ComLaser.ClearInputBuffer();
//DEL 	m_strReceive = _T("");
//DEL 	if( m_ESH )
//DEL 	{
//DEL 		//m_ComLaser.Write(_T("EXT=000001\n"));
//DEL 		SendCommon(_T("EXT=000001\n"),11);
//DEL 	}
//DEL 	else
//DEL 	{	
//DEL 		//m_ComLaser.Write(_T("EXT=000000\n"));
//DEL 		SendCommon(_T("EXT=000000\n"),11);
//DEL 	}
//DEL 
//DEL 	m_waitEvent.Reset();
//DEL 	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
//DEL 	{	
//DEL 		//AfxMessageBox(m_strReceive);
//DEL 	}
//DEL }



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
	m_ComLaser.Close();
    ExitProcess(0);
} 


void CLaserSettingDlg::OnTimer(UINT nIDEvent) 
{
	char	chTemp[16];
	if(m_cButton.EnableWindow() != 0)	
 {	UpdateData(TRUE);
	m_cButton.EnableWindow(false);
	//DIO
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");
	SendCommon("?DIO\n",5);
	m_waitEvent.Reset();
	long  lValue = 0;
	float  fValue = 0.0;//karen add 2012-02-20
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		if ((BOOL)lValue==true)
		m_CSDIO.SetIcon(m_hIconRED);
		else
		m_CSDIO.SetIcon(m_hIconGREEN);
	}
	UpdateData(FALSE);
	
	//QSW
	Sleep(10);

	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?QSW\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);
		if ((BOOL)lValue==true)
		m_CSQS.SetIcon(m_hIconRED);
		else
		m_CSQS.SetIcon(m_hIconGREEN);
		
	}
	UpdateData(FALSE);
	
	//EXT
	Sleep(10);

	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?EXT\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		lValue = atol(m_strReceive);	
		if ((BOOL)lValue==true)
		m_CSQS.SetIcon(m_hIconRED);
		else
		m_CSQS.SetIcon(m_hIconGREEN);
	}
	UpdateData(FALSE);
	
//TK1
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?TK1\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp)/10;		
		m_fRSHG = lValue;
	}
	UpdateData(FALSE);
	
//TK2
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?TK2\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp)/10;		
		m_fRTHG = lValue;
	}
	UpdateData(FALSE);
	
	
	//TDI
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?TDI\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp)/10;		
		m_fRTDI = lValue;
	}
	UpdateData(FALSE);
	
	//IDI


	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?IDI\n",5);
	m_waitEvent.Reset();
	char   bufIDI[6];
	CString tempbufIDI;
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
			lValue = StringToLong(chTemp)/10;		
		m_fRIDI = lValue;
		//lValue = StringToLong(chTemp);		
		
		//tempbufIDI.Format(_T("%.1f"),lValue/10.0);        
        //strncpy(bufIDI,(LPCTSTR)tempbufIDI,sizeof(bufIDI));
		//GetDlgItem(IDC_EDIT_RIDI)->SetWindowText(bufIDI);
	
	}
	UpdateData(FALSE);

	//TEV
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TEV\n",5);	m_waitEvent.Reset();
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
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TLA\n",5);
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
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	SendCommon("?TDI\n",5);
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
        GetDlgItem(IDC_EDIT_RTDI)->SetWindowText(bufTDI);
        
	}
	UpdateData(FALSE);
	//karen add 2012-02-20 begin

	//?PRF
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?PRF\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);	
		lValue = StringToLong(chTemp)/1000;		
		m_lRPRF = lValue;
	}
	UpdateData(FALSE);
	
	//?PWM
	Sleep(10);
	ZeroMemory(chTemp,16);
	m_ComLaser.ClearOutputBuffer();
	m_strReceive = _T("");
	lValue =0;
	SendCommon("?PWM\n",5);
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 1000) == TRUE )
	{	
		ZeroMemory(chTemp,16);
		memcpy(chTemp,m_strReceive,6);		
		lValue = StringToLong(chTemp);		
		m_fRPWM = lValue;
	}
	UpdateData(FALSE);
	//karen add 2012-02-20 end
	}
	CDialog::OnTimer(nIDEvent);
	
}


void CLaserSettingDlg::OnSelchangeComboDio() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	if( m_DIO )
	{
		//m_ComLaser.Write(_T("DIO=000001\n"));
		SendCommon(_T("DIO=000001\n"),11);
	}
	else
	{	
		//m_ComLaser.Write(_T("DIO=000000\n"));
		SendCommon(_T("DIO=000000\n"),11);
	}
	
	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
	
}

void CLaserSettingDlg::OnSelchangeComboEsh() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");
	if( m_ESH)
	{
		//m_ComLaser.Write(_T("EXT=000001\n"));
		SendCommon(_T("EXT=000001\n"),11);
	}
	else
	{	
		//m_ComLaser.Write(_T("EXT=000000\n"));
		SendCommon(_T("EXT=000000\n"),11);
	}

	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
	
}

void CLaserSettingDlg::OnSelchangeComboQs() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_ComLaser.ClearOutputBuffer();
	m_ComLaser.ClearInputBuffer();
	m_strReceive = _T("");

	if( m_QS )
	{
		//m_ComLaser.Write(_T("QSW=000001\n"));
		SendCommon(_T("QSW=000001\n"),11);
	
	}
	else
	{	
		//m_ComLaser.Write(_T("QSW=000000\n"));
		SendCommon(_T("QSW=000000\n"),11);
	
	}

	m_waitEvent.Reset();
	if( WaitWithMessageLoop(m_waitEvent.GetHANDLE(), 8000) == TRUE )
	{	
		//AfxMessageBox(m_strReceive);
	}
}

void CLaserSettingDlg::OnKillfocusEditWidi() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
		if ((m_fWIDI>=0) && (m_fWIDI<=50))
    m_bIDIChange = TRUE;
	else
	{
		m_bIDIChange = FALSE;
		AfxMessageBox("IDI 范围应在 0~50");
	}
	UpdateData(FALSE);
}

void CLaserSettingDlg::OnKillfocusEditWprf() 
{
	// TODO: Add your control notification handler code here
		UpdateData(TRUE);
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	if ((m_lWPRF>=20)&&(m_lWPRF<=150))
	m_bPRFChange = TRUE;
	else
	{m_bPRFChange = FALSE;
		AfxMessageBox("prf 范围应在 20~150");
	}
	UpdateData(FALSE);
}

void CLaserSettingDlg::OnKillfocusEditWpwm() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if ((m_fWPWM>=10)&&(m_fWPWM<=100))
	m_bPWMChange = TRUE;
	else
	{m_bPWMChange = FALSE;
	AfxMessageBox("PWM 范围应在 10~100");
	}
	UpdateData(FALSE);
	
}
