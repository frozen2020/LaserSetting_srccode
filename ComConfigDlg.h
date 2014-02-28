#if !defined(AFX_COMCONFIGDLG_H__FE0C8339_2C9E_4404_98E5_CCF8CB685B17__INCLUDED_)
#define AFX_COMCONFIGDLG_H__FE0C8339_2C9E_4404_98E5_CCF8CB685B17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComConfigDlg dialog

class CComConfigDlg : public CDialog
{
// Construction
public:
	CComConfigDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComConfigDlg)
	enum { IDD = IDD_DIALOG_COM };
	CComboBox	m_cbStopbits;//停止位
	CComboBox	m_cbParity;
	CComboBox	m_cbDataControl;
	CComboBox	m_cbDataBits;//数据位
	CComboBox	m_cbBaudRate;//波特率
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMCONFIGDLG_H__FE0C8339_2C9E_4404_98E5_CCF8CB685B17__INCLUDED_)
