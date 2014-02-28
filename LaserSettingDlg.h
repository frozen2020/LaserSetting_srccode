// LaserSettingDlg.h : header file
//

#if !defined(AFX_LASERSETTINGDLG_H__B12CFA51_608C_4C06_95A1_911B0EA9A85A__INCLUDED_)
#define AFX_LASERSETTINGDLG_H__B12CFA51_608C_4C06_95A1_911B0EA9A85A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLaserSettingDlg dialog

class CLaserSettingDlg : public CDialog
{
// Construction
public:
	CLaserSettingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLaserSettingDlg)
	enum { IDD = IDD_LASERSETTING_DIALOG };
	CListCtrl	m_ListCommad;
	CComboBox	m_cbCom;
	BOOL	m_bDIOOn;
	float	m_fIDI;
	BOOL	m_bSHTOn;
	BOOL	m_bQSOn;
	BOOL	m_bQSEXT;
	UINT	m_nDelay;
	CString	m_strSend;
	float	m_fPWM;
	float	m_fPWM_QUERY;
	float	m_lPRF_QUERY;
	long	m_lPRF;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLaserSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonComset();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonSend();
	afx_msg void OnCheckDio();
	afx_msg void OnChangeEditIdi();
	afx_msg void OnChangeEditPrf();
	afx_msg void OnCheckQs();
	afx_msg void OnCheckExt();
	afx_msg void OnButtonGet();
	afx_msg void OnChangeEditPwm();
	afx_msg void OnClosedig();
	afx_msg void OnKillfocusEditIdi();
	afx_msg void OnKillfocusEditPrf();
	afx_msg void OnKillfocusEditPwm();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnComRecv(WPARAM wParam, LPARAM lParam);
public:
	void SendCommon(char* strCommon,int nlength);
	void GetCom();
	void SetListCommad(LPCTSTR strListCmd);
	unsigned long StringToLong(char* p);
	void longToString(ULONG itemp,char *p);
	CDelphilaserComCommunicate		m_ComLaser;
	int								m_nComIndex;

private:
	CString				m_strReceive;
	CManualResetEvent	m_waitEvent;
	BOOL				m_bIDIChange;
	BOOL				m_bPRFChange;
	UINT				m_nListCount;

	BOOL				m_bPWMChange;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERSETTINGDLG_H__B12CFA51_608C_4C06_95A1_911B0EA9A85A__INCLUDED_)
