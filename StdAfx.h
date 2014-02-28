// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3E353627_609D_4497_BFAA_3A033552417C__INCLUDED_)
#define AFX_STDAFX_H__3E353627_609D_4497_BFAA_3A033552417C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "SkinH.h"
#pragma comment (lib,"SkinH.lib")
#include "DelphilaserComCommunicate.h"

typedef struct _tagComConfig
{
	CString		strBaudRate;
	CString		strParity;
	CString		strDataBits;
	CString		strStopbits;
	CString		strDataControl;
}COMCONFIG;

class CManualResetEvent 
{
   public :

      CManualResetEvent();
      ~CManualResetEvent();

      bool Create(TCHAR *f_pName = NULL);

      bool Set();
	  bool Reset();
	  bool Pulse();

      HANDLE GetHANDLE() const;

      void Wait() const;

      bool Wait(
         DWORD timeoutMillis) const;

	  CString m_strEventName;

   private :

      HANDLE m_hEvent;
};


BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3E353627_609D_4497_BFAA_3A033552417C__INCLUDED_)
