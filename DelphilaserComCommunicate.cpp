/*! @file
********************************************************************************
<PRE>
Classname      : CDelphilaserComCommunity
fileName       : DelphilaserComCommunity.cpp
Function       : Com Community 
Author         : Jack.Gu
Ver			   : V1.0
--------------------------------------------------------------------------------
Remark       : 
--------------------------------------------------------------------------------
Modify: 
Date        Ver     Author              Modify Note

  </PRE>
*******************************************************************************/
#include "stdafx.h"
#include "DelphilaserComCommunicate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDelphilaserComCommunicate::CDelphilaserComCommunicate()
{
	
}

CDelphilaserComCommunicate::~CDelphilaserComCommunicate()
{
	Close(); 
	Destroy();
}

void CDelphilaserComCommunicate::InitCom(bool fAutoBeginThread, DWORD dwIOMode)
{	
	_dwIOMode = dwIOMode;
	_fAutoBeginThread = fAutoBeginThread;

	memset(_szCommStr, 0, 20);
	memset(&_DCB, 0, sizeof(_DCB));
	_DCB.DCBlength = sizeof(_DCB);
	_hCommHandle = INVALID_HANDLE_VALUE;
	
	memset(&_ReadOverlapped, 0, sizeof(_ReadOverlapped));
	memset(&_WriteOverlapped, 0, sizeof(_WriteOverlapped));
	
	_ReadOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
	assert(_ReadOverlapped.hEvent != NULL); 
	
	_WriteOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
	assert(_WriteOverlapped.hEvent != NULL);
	
	_hNotifyWnd = NULL;
	_dwNotifyNum = 0;
	_dwMaskEvent = DEFAULT_COM_MASK_EVENT;
	_hThreadHandle = NULL;
	
	memset(&_WaitOverlapped, 0, sizeof(_WaitOverlapped));
	_WaitOverlapped.hEvent = ::CreateEvent(NULL, true, false, NULL);
	assert(_WaitOverlapped.hEvent != NULL); 
	
	::InitializeCriticalSection(&_Mutex);
}

DWORD CDelphilaserComCommunicate::ThreadFunc()
{
	if(!::SetCommMask(_hCommHandle, _dwMaskEvent))
	{
		char szBuffer[256];
		_snprintf(szBuffer, 255, "%s(%d) : COM%d Call WINAPI SetCommMask(%x, %x) Fail, thread work invalid! GetLastError() = %d;",
			__FILE__, __LINE__, _dwPort, _hCommHandle, _dwMaskEvent, GetLastError());
		MessageBox(NULL, szBuffer, "Class cnComm", MB_OK);
		return 1;
	}
	
	COMSTAT Stat;
	DWORD dwError;
	
	for(DWORD dwLength, dwMask = 0; _fRunFlag && IsOpen(); dwMask = 0)
	{
		if(!::WaitCommEvent(_hCommHandle, &dwMask, &_WaitOverlapped))
		{
			if(::GetLastError() == ERROR_IO_PENDING)// asynchronous
				::GetOverlappedResult(_hCommHandle, &_WaitOverlapped, &dwLength, TRUE);
			else
				continue;
		}
		
		if(dwMask == 0)
			continue;
		
		switch(dwMask)
		{
		case EV_RXCHAR :
			::ClearCommError(_hCommHandle, &dwError, &Stat);
			if(Stat.cbInQue >= _dwNotifyNum)
				OnReceive();
			break;
			
		case EV_TXEMPTY :
			OnTXEmpty();
			break;
			
		case EV_CTS :
			OnCTS();
			break;
			
		case EV_DSR :
			OnDSR();
			break;
			
		case EV_RING :
			OnRing();
			break;
			
		case EV_RLSD :
			OnRLSD();
			break;
			
		case EV_BREAK:
			OnBreak();
			break;
			
		case EV_ERR :
			OnError();
			break;
			
		}//case
	}//for
	return 0;
}

//open  default:9600, 8, n, 1
bool CDelphilaserComCommunicate::Open(DWORD dwPort)
{
	return Open(dwPort, 9600);
}

//open default baud_rate, 8, n, 1
bool CDelphilaserComCommunicate::Open(DWORD dwPort,DWORD dwBaudRate)
{
	if(dwPort < 1 || dwPort > 1024)
		return false;
	
	BindCommPort(dwPort);
	
	if(!OpenCommPort())
		return false;
	
	if(!SetupPort())
		return false;
	
	return SetState(dwBaudRate);
}

//open EG:"9600, e, 8, 1"
bool CDelphilaserComCommunicate::Open(DWORD dwPort, char *szSetStr)
{
	if(dwPort < 1 || dwPort > 1024)
		return false;
	
	BindCommPort(dwPort);
	
	if(!OpenCommPort())
		return false;
	
	if(!SetupPort())
		return false;
	
	return SetState(szSetStr);
}

//Read char to Buffer 
DWORD CDelphilaserComCommunicate::Read(LPVOID Buffer, DWORD dwBufferLength, DWORD dwWaitTime)
{
	if(!IsOpen())
		return 0;
	
	COMSTAT  Stat;
	DWORD dwError;
	
	if(::ClearCommError(_hCommHandle, &dwError, &Stat) && dwError > 0)
	{
		::PurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR);
		return 0;
	}
	
	if(!Stat.cbInQue) 
		return 0;
	
	unsigned long uReadLength = 0;
	
	dwBufferLength = dwBufferLength > Stat.cbInQue ? Stat.cbInQue : dwBufferLength;
	
	if(!::ReadFile(_hCommHandle, Buffer, dwBufferLength, &uReadLength, &_ReadOverlapped)) 
	{
		if(::GetLastError() == ERROR_IO_PENDING) 
		{
			WaitForSingleObject(_ReadOverlapped.hEvent, dwWaitTime); // 结束异步I/O
			if(!::GetOverlappedResult(_hCommHandle, &_ReadOverlapped, &uReadLength, false))
			{
				if(::GetLastError() != ERROR_IO_INCOMPLETE)
					uReadLength = 0;
			}
		}
		else
			uReadLength = 0;
	}
	
	return uReadLength;
}
//Read String 
char * CDelphilaserComCommunicate::ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime)
{
	unsigned long uReadLength = Read(szBuffer, dwBufferLength - 1, dwWaitTime);
	szBuffer[uReadLength] = '\0';
	return szBuffer;
}

//Read Sync
DWORD CDelphilaserComCommunicate::ReadSync(LPVOID Buffer, DWORD dwBufferLength)
{
	if(!IsOpen())
		return 0;
	
	DWORD dwError;
	if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0)
	{
		::PurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR);
		return 0;
	}
	
	DWORD uReadLength = 0;
	::ReadFile(_hCommHandle, Buffer, dwBufferLength, &uReadLength, NULL);
	return uReadLength;
}

//Write any char "abcd" or "\x0\x1\x2"
DWORD CDelphilaserComCommunicate::Write(LPVOID Buffer, DWORD dwBufferLength)
{
	if(!IsOpen())
		return 0;
	
	DWORD dwError;
	
	if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0) 
		::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR); 
	
	unsigned long uWriteLength = 0;
	
	if(!::WriteFile(_hCommHandle, Buffer, dwBufferLength, &uWriteLength, &_WriteOverlapped))
		if(::GetLastError() != ERROR_IO_PENDING)
			uWriteLength = 0;
		
		return uWriteLength;
}
//write ANSI Char
DWORD CDelphilaserComCommunicate::Write(const char *szBuffer)
{
	assert(szBuffer);
	
	return Write((void *)szBuffer, strlen(szBuffer));
}

//write Sync
DWORD CDelphilaserComCommunicate::WriteSync(LPVOID Buffer, DWORD dwBufferLength)
{
	if(!IsOpen())
		return 0;
	
	DWORD dwError;
	if(::ClearCommError(_hCommHandle, &dwError, NULL) && dwError > 0) 
		::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR); 
	
	unsigned long uWriteLength = 0;
	::WriteFile(_hCommHandle, Buffer, dwBufferLength, &uWriteLength, NULL);
	return uWriteLength;
}
//write char format
DWORD CDelphilaserComCommunicate::Write(char *szBuffer, DWORD dwBufferLength, char * szFormat, ...)
{
	if(!IsOpen())
		return 0;
	
	va_list va;
	va_start(va, szFormat);
	_vsnprintf(szBuffer, dwBufferLength, szFormat, va);
	va_end(va);
	
	return Write(szBuffer);
}
//write char format check the buffer size
DWORD CDelphilaserComCommunicate::Write(char *szBuffer, char * szFormat, ...)
{
	if(!IsOpen())
		return 0;
	
	va_list va;
	va_start(va, szFormat);
	vsprintf(szBuffer, szFormat, va);
	va_end(va);
	
	return Write(szBuffer);
}

void CDelphilaserComCommunicate::BindCommPort(DWORD dwPort)
{
	assert(dwPort >= 1 && dwPort <= 1024);
	
	char p[5];
	
	_dwPort = dwPort;
	strcpy(_szCommStr, "\\\\.\\COM"); 
	ltoa(_dwPort, p, 10);
	strcat(_szCommStr, p);
}

bool CDelphilaserComCommunicate::OpenCommPort()
{
	if(IsOpen())
		Close();
	
	_hCommHandle = ::CreateFile(
		_szCommStr,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | _dwIOMode, 
		NULL
		);
	
	if(_fAutoBeginThread)
	{
		if(IsOpen() && BeginThread())
			return true;
		else
		{
			Close();  //创建线程失败
			return false;
		}
	}
	return IsOpen();
}

bool CDelphilaserComCommunicate::SetupPort()
{
	if(!IsOpen())
		return false;
	
	
	if(!::SetupComm(_hCommHandle, 4096, 4096))
		return false; 
	
	
	if(!::GetCommTimeouts(_hCommHandle, &_CO))
		return false;
	_CO.ReadIntervalTimeout = 0;
	_CO.ReadTotalTimeoutMultiplier = 1;
	_CO.ReadTotalTimeoutConstant = 1000;
	_CO.WriteTotalTimeoutMultiplier = 1;
	_CO.WriteTotalTimeoutConstant = 1000;
	if(!::SetCommTimeouts(_hCommHandle, &_CO))
		return false; 
	
	
	if(!::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
		return false; 
	
	return true;
}

void CDelphilaserComCommunicate::Close()
{
	if(IsOpen())  
	{
		PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR); 
		
		EndThread();
		::CloseHandle(_hCommHandle);
		
		_hCommHandle = INVALID_HANDLE_VALUE;
	}
}

void CDelphilaserComCommunicate::Destroy()
{
	if(_ReadOverlapped.hEvent != NULL)
		CloseHandle(_ReadOverlapped.hEvent);
	
	if(_WriteOverlapped.hEvent != NULL)
		CloseHandle(_WriteOverlapped.hEvent);
	
	if(_WaitOverlapped.hEvent != NULL)
		CloseHandle(_WaitOverlapped.hEvent);
	
	::DeleteCriticalSection(&_Mutex);
}

bool CDelphilaserComCommunicate::BeginThread()
{
	if(!IsThreadRunning()) 
	{
		_fRunFlag = true;
		_hThreadHandle = NULL;
		
#ifdef _MT
		unsigned int id;
		_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, CommThreadProc, this, 0, &id);
#else
		DWORD id;
		_hThreadHandle = ::CreateThread(NULL, 0, CommThreadProc, this, 0, &id); 
#endif
		
		return (_hThreadHandle != NULL); 
	}
	return false;
}

bool CDelphilaserComCommunicate::EndThread(DWORD dwWaitTime)
{
	if(IsThreadRunning()) 
	{
		_fRunFlag = false;
		::SetCommMask(_hCommHandle, 0);
		::SetEvent(_WaitOverlapped.hEvent);
		if(::WaitForSingleObject(_hThreadHandle, dwWaitTime) != WAIT_OBJECT_0)
			if(!::TerminateThread(_hThreadHandle, 0))
				return false;
			
			::CloseHandle(_hThreadHandle);
			::ResetEvent(_WaitOverlapped.hEvent);
			
			_hThreadHandle = NULL;
			
			return true;
	}
	return false;
}
