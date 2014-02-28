// LaserSetting.h : main header file for the LASERSETTING application
//

#if !defined(AFX_LASERSETTING_H__793C87D9_EB0A_4BAB_BDC0_6211D4D21A6B__INCLUDED_)
#define AFX_LASERSETTING_H__793C87D9_EB0A_4BAB_BDC0_6211D4D21A6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLaserSettingApp:
// See LaserSetting.cpp for the implementation of this class
//

class CLaserSettingApp : public CWinApp
{
public:
	CLaserSettingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserSettingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLaserSettingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CLaserSettingApp theApp;
extern COMCONFIG	m_ComConfig;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERSETTING_H__793C87D9_EB0A_4BAB_BDC0_6211D4D21A6B__INCLUDED_)
