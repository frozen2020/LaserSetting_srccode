/*! @file
********************************************************************************
<PRE>
Classname      : CDelphilaserComCommunity
fileName       : DelphilaserComCommunity.h
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
#pragma once
#include <assert.h>
#include <stdio.h>
#include <windows.h>
#include <process.h>
/* karen add 
事件 描述 
EV_BREAK 输入中发生中断 
EV_CTS CTS信号状态发生变化 
EV_DSR DSR信号状态发生变化 
EV_ERR 发生线路状态错误，线路状态错误包括CE_FRAME,CE_OVERRUN和CE_RXPARITY 
EV_RING 检测到振铃指示 
EV_RLSD 接收线路信号检测的信号状态发生变化 
EV_RXCHAR 接收到字符，并置于输入缓冲区中 
EV_RXFLAG 接收到事件字符，并置于输入缓冲区中 
EV_TXEMPTY 输出缓冲区中的最后一个字符已被发送 

*/
//WindowsMessage  WPARAM:Port
#define ON_COM_RECEIVE WM_USER + 618    
#define ON_COM_CTS     WM_USER + 619  
#define ON_COM_DSR     WM_USER + 621 
#define ON_COM_RING    WM_USER + 623
#define ON_COM_RLSD    WM_USER + 624
#define ON_COM_BREAK   WM_USER + 625
#define ON_COM_TXEMPTY WM_USER + 626
#define ON_COM_ERROR   WM_USER + 627 
#define DEFAULT_COM_MASK_EVENT  EV_RXCHAR | EV_ERR | EV_CTS | EV_DSR | EV_BREAK | EV_TXEMPTY | EV_RING | EV_RLSD


class CDelphilaserComCommunicate  
{
public:
	bool EndThread(DWORD dwWaitTime = 100);
	bool BeginThread();
	CDelphilaserComCommunicate();
	virtual ~CDelphilaserComCommunicate();
	virtual void Close();
	void InitCom(bool fAutoBeginThread = true, DWORD dwIOMode = FILE_FLAG_OVERLAPPED);

	//////////////////////////////////////////////////////////////////////////
	virtual void OnReceive()//EV_RXCHAR
	{   /*函数功能：该函数确定给定的窗口句柄是否标识一个已存在的窗口
		hWnd:被测试窗口的句柄。 　　
		返回值：如果窗口句柄标识了一个已存在的窗口，返回值为非零；
		如果窗口句柄未标识一个已存在窗口，返回值为零。此函数在WINUSER.H中定义*/
		if(::IsWindow(_hNotifyWnd))
			//函数功能：该函数将一个消息放入（寄送）到与指定窗口创建的线程相联系消息队列里，不等待线程处理消息就返回，是异步消息模式。
			::PostMessage(_hNotifyWnd, ON_COM_RECEIVE, WPARAM(_dwPort), LPARAM(0));
	}
	
	virtual void OnDSR()
	{
		if(::IsWindow(_hNotifyWnd))
		{
			DWORD Status;
			if(GetCommModemStatus(_hCommHandle, &Status))
				::PostMessage(_hNotifyWnd, ON_COM_DSR, WPARAM(_dwPort), LPARAM( (Status & MS_DSR_ON) ? 1 : 0));
		}
	}
	
	virtual void OnCTS()
	{
		if(::IsWindow(_hNotifyWnd))
		{
			DWORD Status;
			if(GetCommModemStatus(_hCommHandle, &Status))
				::PostMessage(_hNotifyWnd, ON_COM_CTS, WPARAM(_dwPort), LPARAM( (Status & MS_CTS_ON) ? 1 : 0));
		}
	}
	
	virtual void OnBreak()
	{
		if(::IsWindow(_hNotifyWnd))
		{
			::PostMessage(_hNotifyWnd, ON_COM_BREAK , WPARAM(_dwPort), LPARAM(0));
		}
	}
	
	virtual void OnTXEmpty()
	{
		if(::IsWindow(_hNotifyWnd))
			::PostMessage(_hNotifyWnd, ON_COM_TXEMPTY, WPARAM(_dwPort), LPARAM(0));
	}
	
	virtual void OnError()
	{
		DWORD dwError;
		::ClearCommError(_hCommHandle, &dwError, NULL);
		if(::IsWindow(_hNotifyWnd))
			::PostMessage(_hNotifyWnd, ON_COM_ERROR, WPARAM(_dwPort), LPARAM(dwError));
	}
	
	virtual void OnRing()
	{
		if(::IsWindow(_hNotifyWnd))
			::PostMessage(_hNotifyWnd, ON_COM_RING, WPARAM(_dwPort), LPARAM(0));
	}
	
	virtual void OnRLSD()
	{
		if(::IsWindow(_hNotifyWnd))
			::PostMessage(_hNotifyWnd, ON_COM_RLSD, WPARAM(_dwPort), LPARAM(0));
	}
	//////////////////////////////////////////////////////////////////////////
	
	void Lock()
	{
		/*
		多个线程操作相同的数据时，一般是需要按顺序访问的，否则会引导数据错乱，无法控制数据，
		变成随机变量。为解决这个问题，就需要引入互斥变量，让每个线程都按顺序地访问变量。
		这样就需要使用EnterCriticalSection和LeaveCriticalSection函数。
		WINAPI 函数  */
		::EnterCriticalSection(&_Mutex);
	}
	void Unlock()
	{
		::LeaveCriticalSection(&_Mutex);
	}
	
	//////////////////////////////////////////////////////////////////////////
	inline bool IsOpen()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//
	operator bool ()
	{
		return _hCommHandle != INVALID_HANDLE_VALUE;
	}
	//
	HANDLE GetHandle()
	{
		return _hCommHandle;
	}
	//
	operator HANDLE()
	{
		return _hCommHandle;
	}
	//
	const int GetPortID()
	{
		return _dwPort;
	}
	//
	const char *GetPortName()
	{
		return _szCommStr;
	}
	//
	DCB *GetState()
	{
		return IsOpen() && ::GetCommState(_hCommHandle, &_DCB) == TRUE ? &_DCB : NULL;
	}
	//
	bool SetState(DCB *pdcb = NULL)
	{
		return IsOpen() ? ::SetCommState(_hCommHandle, pdcb == NULL ? &_DCB : pdcb) == TRUE : false;
	}
	//
	bool SetState(char *szSetStr) 
	{
		if(IsOpen())
		{
			if(::GetCommState(_hCommHandle, &_DCB) != TRUE)
				return false;
			if(::BuildCommDCB(szSetStr, &_DCB) != TRUE)
				return false;
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}
	//
	bool SetState(DWORD dwBaudRate, DWORD dwByteSize = 8, DWORD dwParity = NOPARITY, DWORD dwStopBits = ONESTOPBIT)
	{
		if(IsOpen())
		{
			if(::GetCommState(_hCommHandle, &_DCB) != TRUE)
				return false;
			_DCB.BaudRate = dwBaudRate;
			_DCB.ByteSize = (unsigned char)dwByteSize;
			_DCB.Parity   = (unsigned char)dwParity;
			_DCB.StopBits = (unsigned char)dwStopBits;
			return ::SetCommState(_hCommHandle, &_DCB) == TRUE;
		}
		return false;
	}
	//
	LPCOMMTIMEOUTS GetTimeouts(void)
	{
		return IsOpen() && ::GetCommTimeouts(_hCommHandle, &_CO) == TRUE  ? &_CO : NULL;
	}
	//
	bool SetTimeouts(LPCOMMTIMEOUTS lpCO)
	{
		return IsOpen() ? ::SetCommTimeouts(_hCommHandle, lpCO) == TRUE : false;
	}
	//
	bool SetBufferSize(DWORD dwInputSize, DWORD dwOutputSize)
	{
		return IsOpen() ? ::SetupComm(_hCommHandle, dwInputSize, dwOutputSize) == TRUE : false; 
	}
	//
	void ClearInputBuffer()
	{
		if(IsOpen())
			::PurgeComm(_hCommHandle, PURGE_RXABORT | PURGE_RXCLEAR );
	}
	//
	void ClearOutputBuffer()
	{
		if(IsOpen())
			::PurgeComm(_hCommHandle, PURGE_TXABORT | PURGE_TXCLEAR );
	}
	//
	inline void SetWnd(HWND hWnd)//设置窗口接收处理消息
	{
		assert(::IsWindow(hWnd));
		_hNotifyWnd = hWnd;
	}
	//
	inline void SetNotifyNum(DWORD dwNum)
	{
		_dwNotifyNum = dwNum;
	}
	//
	//
	bool SetDTR(bool OnOrOff)
	{
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETDTR : CLRDTR) != 0 : false;
	}
	//RTS 
	bool SetRTS(bool OnOrOff)
	{
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETRTS : CLRRTS) != 0 : false;
	}
	//
	bool SetBreak(bool OnOrOff)
	{
		return IsOpen() ? EscapeCommFunction(_hCommHandle, OnOrOff ? SETBREAK : CLRBREAK) != 0 : false;
	}

	inline bool IsThreadRunning()
	{
		return _hThreadHandle != NULL;
	}
	//
	inline HANDLE GetThread()
	{
		return _hThreadHandle;
	}
	//
	void SetMaskEvent(DWORD dwEvent = DEFAULT_COM_MASK_EVENT)
	{
		_dwMaskEvent = dwEvent;
	}
	//
	int GetInputSize()
	{
		COMSTAT Stat;
		DWORD dwError;
		
		return ::ClearCommError(_hCommHandle, &dwError, &Stat) == TRUE  ? Stat.cbInQue : (DWORD)-1L;
	}
	//
	inline bool SuspendThread()
	{
		return IsThreadRunning() ? ::SuspendThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//
	inline bool ResumeThread()
	{
		return IsThreadRunning() ? ::ResumeThread(_hThreadHandle) != 0xFFFFFFFF : false;
	}
	//////////////////////////////////////////////////////////////////////////

	//Open COM
	bool Open(DWORD dwPort);
	bool Open(DWORD dwPort, DWORD dwBaudRate);
	bool Open(DWORD dwPort, char *szSetStr);
	
	//Read Char
	DWORD Read(LPVOID Buffer, DWORD dwBufferLength, DWORD dwWaitTime = 10);
	char * ReadString(char *szBuffer, DWORD dwBufferLength, DWORD dwWaitTime = 20);
	DWORD ReadSync(LPVOID Buffer, DWORD dwBufferLength);
	
	//write char
	DWORD Write(LPVOID Buffer, DWORD dwBufferLength);
	DWORD Write(const char *szBuffer);
	DWORD WriteSync(LPVOID Buffer, DWORD dwBufferLength);
	DWORD Write(char *szBuffer, DWORD dwBufferLength, char * szFormat, ...);
	DWORD Write(char *szBuffer, char * szFormat, ...);

protected:
	virtual DWORD ThreadFunc();
	virtual void Destroy();
	virtual bool SetupPort();
	virtual bool OpenCommPort();
	void BindCommPort(DWORD dwPort);

protected:
	volatile DWORD _dwPort;			//Com Port
	volatile HANDLE _hCommHandle;	//Com Handle
	char _szCommStr[20];			//Com Name:COM1
	//WINBASE.H中
	DCB _DCB;    
	COMMTIMEOUTS _CO;     
	
	DWORD _dwIOMode;  // 0/FILE_FLAG_OVERLAPPED 
	OVERLAPPED _ReadOverlapped, _WriteOverlapped; 
	
	volatile HANDLE _hThreadHandle; //work Thread
	volatile HWND _hNotifyWnd;		//Wnd
	volatile DWORD _dwNotifyNum;	//Receive How chars(>=_dwNotifyNum) to send message to Wnd
	volatile DWORD _dwMaskEvent;	//
	volatile bool _fRunFlag;		//flag
	bool _fAutoBeginThread;			//Open BeginThread();
	OVERLAPPED _WaitOverlapped;		//WaitCommEvent use

private:  //the function protected 
	CDelphilaserComCommunicate(const CDelphilaserComCommunicate&);
	CDelphilaserComCommunicate &operator = (const CDelphilaserComCommunicate&);
	CRITICAL_SECTION _Mutex;
	
	//base function for thread
	#ifdef _MT
	static UINT APIENTRY CommThreadProc(LPVOID lpPara)
	#else
	static DWORD WINAPI CommThreadProc(LPVOID lpPara)
	#endif
	{
		return ( (CDelphilaserComCommunicate *)lpPara )->ThreadFunc();
	}
};

struct InnerLock
{
	CDelphilaserComCommunicate* ptr;
	
	InnerLock(CDelphilaserComCommunicate* p) : ptr(p)
	{
		ptr->Lock();
	}
	~InnerLock()
	{
		ptr->Unlock();
	}
};