// LaserSettingDlg.h : header file
//

#include "afxwin.h"
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
	CStatic	m_CSQS;
	CStatic	m_CSESH;
	CStatic	m_CSDIO;
	CStatic	m_CSCOM;
	CButton	m_cButton;
	CComboBox	m_cbSH;
	CListCtrl	m_ListCommad;
	float	m_fIDI;
	BOOL	m_bSHTOn;
	UINT	m_nDelay;
	CString	m_strSend;
	float m_fWPWM ;
	float m_fRPWM ;
	long m_lRPRF ;
	long m_lWPRF ;
	float	m_fPWM_QUERY;
	float	m_lPRF_QUERY;
	long	m_lPRF;
	int		m_QS;
	int		m_ESH;
	int		m_DIO;
	float	m_fRSHG;
	float	m_fRTDI;
	float	m_fRTHG;
	float	m_fRIDI;
	float	m_fWIDI;
	float	m_fWSHG;
	float	m_fWTDI;
	float	m_fWTHG;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	HICON m_hIconGREEN;
	HICON m_hIconRED;
	// Generated message map functions
	//{{AFX_MSG(CLaserSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonSend();
	afx_msg void OnChangeEditPwm();
	afx_msg void OnClosedig();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboDio();
	afx_msg void OnSelchangeComboEsh();
	afx_msg void OnSelchangeComboQs();
	afx_msg void OnKillfocusEditWidi();
	afx_msg void OnKillfocusEditWprf();
	afx_msg void OnKillfocusEditWpwm();
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
	int                             nComIndex;
private:
	CString				m_strReceive;
	CManualResetEvent	m_waitEvent;
	BOOL				m_bIDIChange;
	BOOL				m_bPRFChange;
	UINT				m_nListCount;

	BOOL				m_bPWMChange;
public:
//	CButton m_cButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERSETTINGDLG_H__B12CFA51_608C_4C06_95A1_911B0EA9A85A__INCLUDED_)
