#pragma once
#if !defined(UDPSERVER_H_INCLUDED_)
#define UDPSERVER_H_INCLUDED_

#if _MSC_VER > 1000
#endif // _MSC_VER > 1000


#include <stdio.h>
#include <Winsock2.h>   // Ws2_32.lib
#include "NTTRACE.h"
#include <afxinet.h>
#include "MyDes.h"

#define CLOSE_SOCKET(X) if(X!=INVALID_SOCKET){closesocket(X);X=INVALID_SOCKET;}
class NTLogic;
class CUDPServer  
{
public:
	CUDPServer(CNTTRACE *pTrace,LPVOID ptr);
	virtual ~CUDPServer();

	CNTTRACE *m_trace;
	CMyDes *m_mydes;

private:
	char m_szFTPIP[20];
	char m_szFTPUsr[30];
	char m_szFTPPass[30];


	char m_szLocalIP[20];
	char m_szHostName[30];
	int  m_RecvPort;
	char m_szRemoteIP[20];
	int  m_RemotePort;
	HANDLE m_RecvThread;
	DWORD  m_ThreadID;
	void InitMemberVariable();

	//窗口指针
	CWnd* m_pWnd;
public:
//	SOCKET m_SckRecive;
//	SOCKET m_SckSend;
	bool m_bReciveFlag;

	void Initialize();
	void Destoryme();
	//初始化socket lib
	bool InitSocketLib(WORD wVersion=0x0202);
	void CleanSocketLib();
	//创建发送socket
	bool CreateSendSocket();
	//创建接收socket
	bool CreateRecvSocket(int RecvPort=9527);
	//发送
	bool SendMsg(char * szBuf,int length,char * szremoteIP=NULL,int port=0);
	//启动接收线程
	bool StartRecv(int RecvPort);
	//停止接收线程
	bool StopRecv();

	bool SetSendPara(char * szremoteIP=NULL,int port=0);
	bool SetFTPPara(char* FTPIP,char* FTPUSR,char* FTPPASS);

	static DWORD WINAPI OnReceiveThread(LPVOID lpParameter);   // 线程函数

	BOOL FTPGetFile(char* path_H,char* filename_H,char* path_L,char* filename_L,char* FTPIP=NULL,char* FTPUSR=NULL,char* FTPPASS=NULL);
	BOOL FTPPutFile(char* path_L,char* filename_L,char* path_H,char* filename_H,char* FTPIP=NULL,char* FTPUSR=NULL,char* FTPPASS=NULL);
private:
	NTLogic* mpNtlogic;
};

#endif