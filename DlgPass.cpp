// DlgPass.cpp : implementation file
//

#include "stdafx.h"
#include "LaserSetting.h"
#include "DlgPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPass dialog


CDlgPass::CDlgPass(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPass, CDialog)
	//{{AFX_MSG_MAP(CDlgPass)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPass message handlers

void CDlgPass::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}
