// TCPServer.h: interface for the CTCPServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSERVER_H__019DF490_CB9A_4A49_A7A0_DA622B7BACFB__INCLUDED_)
#define AFX_TCPSERVER_H__019DF490_CB9A_4A49_A7A0_DA622B7BACFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>
#include <Winsock2.h>   // Ws2_32.lib
#include "NTTRACE.h"
#include <afxinet.h>

class CTCPServer  
{

public:
	
    CTCPServer(CNTTRACE *pTrace,LPVOID ptr);

	bool SetSendPara(char * szremoteIP,int port);

	bool SendMsg(char * szBuf,int length,char * szremoteIP=NULL,int port=0);

	virtual ~CTCPServer();

private:

	CWnd* m_pWnd;
	CNTTRACE *m_trace;

	char m_szRemoteIP[20];
	int  m_RemotePort;

	bool Initialize();

};

#endif // !defined(AFX_TCPSERVER_H__019DF490_CB9A_4A49_A7A0_DA622B7BACFB__INCLUDED_)
