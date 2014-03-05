#if !defined(AFX_DLGPASS_H__83340EE1_9281_452C_BC33_4DE14BED0EA9__INCLUDED_)
#define AFX_DLGPASS_H__83340EE1_9281_452C_BC33_4DE14BED0EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPass dialog

class CDlgPass : public CDialog
{
// Construction
public:
	CDlgPass(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPass)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPass)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPASS_H__83340EE1_9281_452C_BC33_4DE14BED0EA9__INCLUDED_)
