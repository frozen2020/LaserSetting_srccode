// stdafx.cpp : source file that includes just the standard includes
//	LaserSetting.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CManualResetEvent::CManualResetEvent()
   :  m_hEvent(INVALID_HANDLE_VALUE)
{
   m_strEventName = _T("");
}

CManualResetEvent::~CManualResetEvent()
{
   if (m_hEvent != INVALID_HANDLE_VALUE)
   {
      ::CloseHandle(m_hEvent);
   }
}

bool CManualResetEvent::Create(TCHAR *f_pName)
{
	if( m_hEvent == INVALID_HANDLE_VALUE )
		m_hEvent = ::CreateEvent(0, TRUE, FALSE, f_pName);

   return (0 != m_hEvent);
}

bool CManualResetEvent::Set()
{
   return (0 != ::SetEvent(m_hEvent));
}

bool CManualResetEvent::Reset()
{
   return (0 != ::ResetEvent(m_hEvent));
}

bool CManualResetEvent::Pulse()
{
   return (0 != ::PulseEvent(m_hEvent));
}

HANDLE CManualResetEvent::GetHANDLE() const
{
   return m_hEvent;
}

void CManualResetEvent::Wait() const
{
   if( !Wait(INFINITE) )
	  TRACE("Warning : Unexpected timeout on infinite wait - CManualResetEvent::Wait(), name is %s", m_strEventName);
}

bool CManualResetEvent::Wait(DWORD timeoutMillis) const
{
   bool ok;

   DWORD result = ::WaitForSingleObject(m_hEvent, timeoutMillis);

   if (result == WAIT_TIMEOUT)
   {
      ok = false;
   }
   else if (result == WAIT_OBJECT_0)
   {
      ok = true;
   }
   else
   {
	  TRACE("Warning : WaitForSingleObject(%s) get error %d - CManualResetEvent::Wait()", m_strEventName, ::GetLastError());
   }
    
   return ok;
}

/********************************************************************/
/*																	*/
/* Function name : WaitWithMessageLoop								*/
/* Description   : Pump messages while waiting for event			*/
/*																	*/
/********************************************************************/
BOOL WaitWithMessageLoop(HANDLE hEvent, int nTimeout)
{   
	DWORD dwRet;

    DWORD dwMaxTick = GetTickCount() + nTimeout;

	while (1)
	{
		// wait for event or message, if it's a message, process it and return to waiting state
		dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, dwMaxTick - GetTickCount(), QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0)
		{
			TRACE0("WaitWithMessageLoop() event triggered.\n");
			return TRUE;      
		}   
		else
		if (dwRet == WAIT_OBJECT_0 + 1)
		{
			// process window messages
			MSG msg;
			// window message         
			while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
			{            
				TranslateMessage(&msg);            
				DispatchMessage(&msg);         
			}      
		}  
		else
		{
			// timed out !
			return FALSE;
		}
	}
}