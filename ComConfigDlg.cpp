// ComConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LaserSetting.h"
#include "ComConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg dialog


CComConfigDlg::CComConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CComConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComConfigDlg)
	//}}AFX_DATA_INIT
}


void CComConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComConfigDlg)
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_cbStopbits);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_cbParity);
	DDX_Control(pDX, IDC_COMBO_DATACONTROL, m_cbDataControl);
	DDX_Control(pDX, IDC_COMBO_DATABITS, m_cbDataBits);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_cbBaudRate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CComConfigDlg)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg message handlers
BOOL CComConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	int nTemp = atoi(m_ComConfig.strBaudRate);//���ַ���תΪ����
	switch( nTemp )
	{
	case 110:
		m_cbBaudRate.SetCurSel(0);
		break;
	case 300:
		m_cbBaudRate.SetCurSel(1);
		break;
	case 1200:
		m_cbBaudRate.SetCurSel(2);
		break;
	case 2400:
		m_cbBaudRate.SetCurSel(3);
		break;
	case 4800:
		m_cbBaudRate.SetCurSel(4);
		break;
	case 9600:
		m_cbBaudRate.SetCurSel(5);
		break;
	case 19200:
		m_cbBaudRate.SetCurSel(6);
		break;
	case 38400:
		m_cbBaudRate.SetCurSel(7);
		break;
	case 57600:
		m_cbBaudRate.SetCurSel(8);
		break;
	case 115200:
		m_cbBaudRate.SetCurSel(9);
		break;
	case 230400:
		m_cbBaudRate.SetCurSel(10);
		break;
	case 460800:
		m_cbBaudRate.SetCurSel(11);
		break;
	case 921600:
		m_cbBaudRate.SetCurSel(12);
		break;
	default:
		m_cbBaudRate.SetCurSel(5);
		break;
	}
	
	char chTemp[128];
	strcpy(chTemp,m_ComConfig.strParity);
	switch( chTemp[0] )
	{
	case 'e':
		m_cbParity.SetCurSel(0);
		break;
	case 'o':
		m_cbParity.SetCurSel(1);
		break;
	case 'n':
		m_cbParity.SetCurSel(2);
		break;
	case 'm':
		m_cbParity.SetCurSel(3);
		break;
	case 's':
		m_cbParity.SetCurSel(4);
		break;
	default:
		m_cbParity.SetCurSel(2);
		break;
	}
	
	nTemp = atoi( m_ComConfig.strDataBits );
	switch( nTemp )
	{
	case 5:
		m_cbDataBits.SetCurSel(0);
		break;
	case 6:
		m_cbDataBits.SetCurSel(1);
		break;
	case 7:
		m_cbDataBits.SetCurSel(2);
		break;
	case 8:
		m_cbDataBits.SetCurSel(3);
		break;
	default:
		m_cbDataBits.SetCurSel(3);
		break;
	}
	
	double fTemp = atof(m_ComConfig.strStopbits);
	if( fTemp == 1.0 )
		m_cbStopbits.SetCurSel(0);
	else if( fTemp == 1.5 )
		m_cbStopbits.SetCurSel(1);
	else if( fTemp == 2.0 )
		m_cbStopbits.SetCurSel(2);
	else
		m_cbStopbits.SetCurSel(0);
	
	chTemp[128];
	strcpy(chTemp,m_ComConfig.strDataControl);
	switch( chTemp[0] )
	{
	case 'X':
		m_cbDataControl.SetCurSel(0);
		break;
	case 'h':
		m_cbDataControl.SetCurSel(1);
		break;
	case 'N':
		m_cbDataControl.SetCurSel(2);
		break;
	default:
		m_cbDataControl.SetCurSel(2);
		break;
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CComConfigDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CComConfigDlg::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	m_cbBaudRate.GetWindowText(m_ComConfig.strBaudRate);
	
	int nIndex = m_cbParity.GetCurSel();//��żУ��
	switch(nIndex)
	{
	case 0:
		m_ComConfig.strParity = _T("e");
		break;
	case 1:
		m_ComConfig.strParity = _T("o");
		break;
	case 2:
		m_ComConfig.strParity = _T("n");
		break;
	case 3:
		m_ComConfig.strParity = _T("m");
		break;
	case 4:
		m_ComConfig.strParity = _T("s");
		break;
	default:
		break;
	}
	m_cbDataBits.GetWindowText(m_ComConfig.strDataBits);
	m_cbStopbits.GetWindowText(m_ComConfig.strStopbits);
	m_cbDataControl.GetWindowText(m_ComConfig.strDataControl);
	CDialog::OnOK();
}