// TCPServer.cpp: implementation of the CTCPServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTAgent3.h"
#include "TCPServer.h"
#include "NTTRACE.h"
#include "LogicCommon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTCPServer::CTCPServer(CNTTRACE *pTrace,LPVOID ptr)
{
	m_trace=pTrace;
	m_pWnd = (CWnd*)ptr;
	Initialize();
}

CTCPServer::~CTCPServer()
{
	WSACleanup();
}

// 
// void handleError()
// {
// 	     LPVOID lpMsgBuf; 
//      FormatMessage( 
//      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
//      FORMAT_MESSAGE_FROM_SYSTEM | 
//      FORMAT_MESSAGE_IGNORE_INSERTS, 
//      NULL, 
//      GetLastError(), 
//      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language 
//      (LPTSTR) &lpMsgBuf, 
//      0, 
//      NULL 
//      ); 
// 
// 	 AfxMessageBox(  (LPCTSTR)lpMsgBuf); 
//      
//      // Free the buffer. 
//      LocalFree( lpMsgBuf ); 
// 
// 
// }

bool CTCPServer::Initialize()
{
	WSADATA wsd;
	int ret = WSAStartup(0x0202,&wsd);
	if(ret!=0)
	{
		WSACleanup();
		return false;
	}
	
	return true;
}

bool CTCPServer::SetSendPara(char * szremoteIP,int port)
{
	strcpy(m_szRemoteIP,szremoteIP);
	m_RemotePort = port;
	return true;
}

bool CTCPServer::SendMsg(char * szBuf, int length, char * szremoteIP,int port)
{

	if(szremoteIP!=NULL)
	{
		strcpy(m_szRemoteIP,szremoteIP);
		m_RemotePort = port;
	}

    SOCKET agentsocket;//创建socket套接字
	
	struct sockaddr_in server;
	int len =sizeof(server);
	server.sin_family=AF_INET;
	server.sin_port=htons(m_RemotePort);					//server的监听端口
	server.sin_addr.s_addr=inet_addr(m_szRemoteIP);			//server的地址 
	agentsocket=socket(AF_INET,SOCK_STREAM,0);
	
	//将发送的报文写入日志
	
	if (connect(agentsocket, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) != 0) 
	{ 
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "CTCPServer connect error ERRORCODE=[%d]",GetLastError());
		return false;
	}
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "CTCPServer SendMsg[%s][%d][%s]", m_szRemoteIP, m_RemotePort, szBuf);

	if(send(agentsocket, szBuf, length,0)!=SOCKET_ERROR)
	{
		//发送报文    成功之后关闭套接字
//         char   RecvMsg[1024]={0}; 
// 		int buflen;
//         buflen=recv(agentsocket,RecvMsg,sizeof(RecvMsg),0); //接收RDP返回信息
// 		if(buflen<=0) //接收0长度或者出错
// 		{
// 			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR,"CTCPServer RecvMsg error, ERRORCODE=[%d]]", GetLastError());
// 			closesocket(agentsocket);
// 			return false;
// 		}
// 		
//         m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "CTCPServer RecvMsg=[%s]", RecvMsg);
		closesocket(agentsocket);
		return true;
	}
	else
	{
		//发送失败将失败信息写入日志文件并关闭套接字
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "CTCPServer send error ERRORCODE=[%d]", GetLastError());
		closesocket(agentsocket);
		return false;
	}

	closesocket(agentsocket);
	return true;
}
