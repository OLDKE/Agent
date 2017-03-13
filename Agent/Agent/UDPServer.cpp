#include "stdafx.h"
#include "UDPServer.h"
#include "LogicCommon.h"

CUDPServer::CUDPServer(CNTTRACE *pTrace,LPVOID ptr)
{
	m_trace=pTrace;
	m_pWnd = (CWnd*)ptr;
	m_mydes =new CMyDes(pTrace);
	
	Initialize();
}
CUDPServer::~CUDPServer()
{
//	Destoryme();
//	if(m_bReciveFlag)
//	{
//		StopRecv();
//	}
//	CleanSocketLib();
	delete m_mydes;
	WSACleanup();

}
void CUDPServer::Initialize()
{
	InitSocketLib(0x0202);
	InitMemberVariable();
}
void CUDPServer::Destoryme()
{
	if(m_bReciveFlag)
	{
		StopRecv();
	}
	CleanSocketLib();
}
void CUDPServer::InitMemberVariable()
{
	strcpy(m_szFTPIP,"127.0.0.1");
	strcpy(m_szFTPUsr,"anonymous");
	strcpy(m_szFTPPass,"");

	strcpy(m_szLocalIP,"127.0.0.1");
	memset(m_szHostName,0,sizeof(m_szHostName));
	strcpy(m_szRemoteIP,"127.0.0.1");

	m_RecvPort = 9800;
//	m_SckRecive = INVALID_SOCKET;
//	m_SckSend = INVALID_SOCKET;
	memset(m_szRemoteIP,0,20);
	m_RemotePort = 12000;
	m_RecvThread = NULL;
	m_ThreadID = 0;
	m_bReciveFlag = false;
}
bool CUDPServer::InitSocketLib(WORD wVersion)
{
	WSADATA wsd;
	int ret = WSAStartup(wVersion,&wsd);
	if(ret!=0)
	{
		WSACleanup();
		return false;
	}
	return true;
}
bool CUDPServer::SetSendPara(char * szremoteIP,int port)
{
	strcpy(m_szRemoteIP,szremoteIP);
	m_RemotePort = port;
	return true;
}

bool CUDPServer::SetFTPPara(char* FTPIP,char* FTPUSR,char* FTPPASS)
{
	strcpy(m_szFTPIP,FTPIP);
	strcpy(m_szFTPUsr,FTPUSR);
	strcpy(m_szFTPPass,FTPPASS);
	return true;
}

void CUDPServer::CleanSocketLib()
{
	WSACleanup();
}
bool CUDPServer::CreateSendSocket()
{
/*
	int ret;
	bool flag;
	SOCKADDR_IN addr;

	CLOSE_SOCKET(m_SckSend);
	m_SckSend = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP); 
	if(m_SckSend==INVALID_SOCKET)
	{
		return false;
	}
	flag = true;
	//设置允许地址复用
	ret = setsockopt(m_SckSend,SOL_SOCKET,SO_REUSEADDR,(char *)&flag,sizeof(flag));
	if(ret!=0)
	{
		CLOSE_SOCKET(m_SckSend);
		return false;
	}
	//绑定
	ZeroMemory(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	//addr.sin_port = htons(m_RecvPort);
	//addr.sin_port = m_RecvPort;
	ret = bind(m_SckSend,(struct sockaddr *)&addr,sizeof(addr));
	if(ret!=0)
	{
		CLOSE_SOCKET(m_SckSend);
		return false;
	}
*/
	return true;
}
bool CUDPServer::CreateRecvSocket(int RecvPort)
{
	m_RecvPort = RecvPort;

/*
	int ret;
	bool flag;
	SOCKADDR_IN addr;

	CLOSE_SOCKET(m_SckRecive);
	m_RecvPort = RecvPort;
	//创建socket
	m_SckRecive = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP); 
	if(m_SckRecive==INVALID_SOCKET)
	{
		return false;
	}
	flag = true;
	//设置允许地址复用
	ret = setsockopt(m_SckRecive,SOL_SOCKET,SO_REUSEADDR,(char *)&flag,sizeof(flag));
	if(ret!=0)
	{
		CLOSE_SOCKET(m_SckRecive);
		return false;
	}

	//绑定
	ZeroMemory(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(m_RecvPort);
	ret = bind(m_SckRecive,(struct sockaddr *)&addr,sizeof(addr));
	if(ret!=0)
	{
		CLOSE_SOCKET(m_SckRecive);
		return false;
	}
*/
	return true;
}
//使用udp协议作为报文的发送基础
bool CUDPServer::SendMsg(char * szBuf,int length,char * szremoteIP,int port)
{

	if(szremoteIP!=NULL)
	{
		strcpy(m_szRemoteIP,szremoteIP);
		m_RemotePort = port;
	}

	char szValue[256];
	memset(szValue,0x0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","KEY",szValue);	
	char outbuf[8000]={0};
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "发送报文 明文=[%s]", szBuf);

	m_mydes->Enc(szBuf,szValue,outbuf);  //报文加密
	//strcpy(outbuf, szBuf);
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "加密后=[%s]", "");

	int tmplen=strlen(outbuf);
	//m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"SendMsg中,分配数组长度[%d],加密后报文长度[%d]=%s",sizeof(outbuf),strlen(outbuf),outbuf);
	memset(szValue,0x0,sizeof(szValue));
	//m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"sendbuf=[%s]",outbuf);

	SOCKET agentsocket;

	int Error;
	WORD VersionRequested;
	WSADATA WsaData;
	VersionRequested=MAKEWORD(2,2);
	Error=WSAStartup(VersionRequested,&WsaData); //启动WinSock2
	if(Error!=0)
	{
		return false;
	}
	else
	{
		if(LOBYTE(WsaData.wVersion)!=2||HIBYTE(WsaData.wHighVersion)!=2)
		{
			WSACleanup();
			return false;
		}

	}

	struct sockaddr_in server;
	int len =sizeof(server);
	server.sin_family=AF_INET;
	server.sin_port=htons(m_RemotePort);					//server的监听端口
	server.sin_addr.s_addr=inet_addr(m_szRemoteIP);			//server的地址 
	agentsocket=socket(AF_INET,SOCK_DGRAM,0);
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "发送报文密文[%s][%d][%s]", m_szRemoteIP, m_RemotePort, outbuf);
	if(sendto(agentsocket,outbuf,strlen(outbuf),0,(struct sockaddr*)&server,len)!=SOCKET_ERROR)
	{
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "发送报文成功");
		closesocket(agentsocket);
		return true;
	}
	else
	{
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "发送报文错误[%d]",GetLastError());
		closesocket(agentsocket);
		return false;
	}
	return true;	
}
bool CUDPServer::StartRecv(int RecvPort)
{
	m_RecvPort = RecvPort;

	//创建线程，在线程中启动服务
	m_RecvThread = CreateThread(NULL,0,OnReceiveThread,this,0,&m_ThreadID);
	
	if(m_RecvThread==NULL)
	{
		return false;
	}
	return true;
}
bool CUDPServer::StopRecv()
{
/*
	if(!m_bReciveFlag)
	{
		return true;
	}
	CLOSE_SOCKET(m_SckRecive);
	if(m_RecvThread!=NULL)
	{
		WaitForSingleObject(m_RecvThread,INFINITE);
		m_RecvThread = NULL;
		m_ThreadID = 0;
	}
*/
	return true;
}

/*
DWORD WINAPI CUDPServer::OnReceiveThread(LPVOID lpParameter)
{
	CUDPServer * pUDPServer;
	pUDPServer = (CUDPServer *)lpParameter;
	pUDPServer->m_bReciveFlag = true;
	pUDPServer->m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"Receive Thread Started.");

	char pbuf[4096];
	int addr_len,iRecLen;;
	struct sockaddr_in addr;
	while (1)
	{
		memset(pbuf,0,sizeof(pbuf));
		addr_len = sizeof(addr);
		iRecLen = recvfrom(pUDPServer->m_SckRecive,pbuf,sizeof(pbuf),0,(struct sockaddr *)&addr,&addr_len);
		if(iRecLen==SOCKET_ERROR)
		{
			pUDPServer->m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"Receive Failed!");
		}
		else
		{
//			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"Receive from [%s]:[%s]",inet_ntoa(addr.sin_addr),pbuf);
			pUDPServer->m_pWnd->SendMessage(WM_UDPRecv,(WPARAM)pbuf,(LPARAM)(inet_ntoa(addr.sin_addr)));
		}
		Sleep(500);
	}
}

*/
//用于接收命令的线程
DWORD WINAPI CUDPServer::OnReceiveThread(LPVOID lpParameter)
{
	CUDPServer * pUDPServer;
	pUDPServer = (CUDPServer *)lpParameter;
	pUDPServer->m_bReciveFlag = true;

	int ret;
	char recvbuf[2048];

	int Error;
	WORD VersionRequested;
	WSADATA WsaData;
	VersionRequested=MAKEWORD(2,2);

	Error=WSAStartup(VersionRequested,&WsaData); //启动WinSock2
	if(Error!=0)
	{
		return 1;
	}
	else
	{
		if(LOBYTE(WsaData.wVersion)!=2||HIBYTE(WsaData.wHighVersion)!=2)
		{
			WSACleanup();
			return 1;
		}
	}
	SOCKET recvsocket;
	struct	sockaddr_in  local;
	local.sin_family=AF_INET;
	local.sin_port=htons(pUDPServer->m_RecvPort);			//监听端口
	local.sin_addr.s_addr=INADDR_ANY;		//本机
	struct sockaddr_in from;
	int fromlen =sizeof(from);

	recvsocket=socket(AF_INET,SOCK_DGRAM,0);
	int bindresult=bind(recvsocket,(struct sockaddr*)&local,sizeof local);

	if(bindresult==0){
		pUDPServer->m_trace->info("UDP Server[%d]启动成功！",pUDPServer->m_RecvPort);
	}else if(bindresult==-1){
		pUDPServer->m_trace->error("UDP Server[%d]启动失败，端口已经被占用！",pUDPServer->m_RecvPort);
		exit(0);
	}else{
		pUDPServer->m_trace->error("UDP Server[%d]启动失败！",pUDPServer->m_RecvPort);
		exit(0);
	}
	while(1) 
	{
		memset(recvbuf,0,sizeof(recvbuf));
		ret=recvfrom(recvsocket,recvbuf,sizeof recvbuf,0,(struct sockaddr*)&from,&fromlen);
		pUDPServer->m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "收到报文, 密文[IP=%s][LEN=%d][%s]", inet_ntoa(from.sin_addr), ret, recvbuf);
		if(ret==SOCKET_ERROR)
		{
			pUDPServer->m_trace->WTrace(LOG_GRP, LOG_COMM ,LT_ERROR, "接收报文错误, ret=[%d]", ret);
		}
		else
		{
			pUDPServer->m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "接收报文成功");
			pUDPServer->m_pWnd->SendMessage(WM_UDPRecv,(WPARAM)recvbuf,(LPARAM)(inet_ntoa(from.sin_addr)));
		}

		Sleep(200);
	}
	closesocket(recvsocket);
	return true;
}


BOOL CUDPServer::FTPGetFile(char * path_H,char * filename_H,char * path_L,char * filename_L,char* FTPIP,char* FTPUSR,char* FTPPASS)
{
	BOOL ret=FALSE;

	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "FTP下载文件,本地路径=[%s],本地文件名=[%s],远程路径=[%s],远程文件名=[%s],IP=[%s]", 
		path_L, filename_L, path_H, filename_H, FTPIP);

	CString m_ftpip;
	if(FTPIP != NULL)
		m_ftpip=FTPIP;
	else
	{
		if(m_szFTPIP == NULL)
			return FALSE;
		m_ftpip=m_szFTPIP;
	}

	CString m_ftpusr;
	if(FTPUSR != NULL)
		m_ftpusr=FTPUSR;
	else
	{
		if(m_szFTPUsr == NULL)
			return FALSE;
		m_ftpusr=m_szFTPUsr;
	}

	CString m_ftppass;
	if(FTPUSR != NULL)
		m_ftppass=FTPPASS;
	else
	{
		if(m_szFTPPass == NULL)
			return FALSE;
		m_ftppass=m_szFTPPass;
	}
	
	CString m_remoteFile;
	m_remoteFile.Format(_T("%s/%s"),path_H,filename_H);

	CString m_localFile;
	m_localFile.Format(_T("%s\\%s"),path_L,filename_L);
	

	CInternetSession ftpsess(_T("NTAGENT/3.0 FTP GET"));

	CFtpConnection* pftpConnect = NULL;

	try
	{
		pftpConnect = ftpsess.GetFtpConnection(m_ftpip,m_ftpusr,m_ftppass);
		if(pftpConnect == NULL)
		{
			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "连接FTP服务器失败");
			return FALSE;
		}
		ret=pftpConnect->GetFile(m_remoteFile,m_localFile,FALSE,FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD | INTERNET_FLAG_RESYNCHRONIZE);
		
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "FTP下载文件返回值=[%d]",ret);

		if(pftpConnect != NULL)
		{
			pftpConnect->Close();
			delete pftpConnect;
		}
	}
	catch (CInternetException* pEx) 
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "FTP下载文件失败[%s]",szErr);
		AfxMessageBox(szErr);
		pEx->Delete();

		if(pftpConnect != NULL)
		{
			pftpConnect->Close();
			delete pftpConnect;
		}
		return FALSE;
	}

	return ret;
}
BOOL CUDPServer::FTPPutFile(char * path_L,char * filename_L,char * path_H,char * filename_H,char* FTPIP,char* FTPUSR,char* FTPPASS)
{
	BOOL ret=FALSE;

	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "FTP上传文件,本地路径=[%s],本地文件名=[%s],远程路径=[%s],远程文件名=[%s],IP=[%s]", 
		path_L, filename_L, path_H, filename_H, FTPIP);

	CString m_ftpip;
	if(FTPIP != NULL)
		m_ftpip=FTPIP;
	else
	{
		if(m_szFTPIP == NULL)
			return FALSE;
		m_ftpip=m_szFTPIP;
	}

	CString m_ftpusr;
	if(FTPUSR != NULL)
		m_ftpusr=FTPUSR;
	else
	{
		if(m_szFTPUsr == NULL)
			return FALSE;
		m_ftpusr=m_szFTPUsr;
	}

	CString m_ftppass;
	if(FTPUSR != NULL)
		m_ftppass=FTPPASS;
	else
	{
		if(m_szFTPPass == NULL)
			return FALSE;
		m_ftppass=m_szFTPPass;
	}

	CString m_remoteFile;
	m_remoteFile.Format(_T("%s/%s"),path_H,filename_H);

	CString m_localFile;
	m_localFile.Format(_T("%s\\%s"),path_L,filename_L);


	CInternetSession ftpsess(_T("NTAGENT/3.0 FTP GET"));

	CFtpConnection* pftpConnect = NULL;

	try
	{
		pftpConnect = ftpsess.GetFtpConnection(m_ftpip,m_ftpusr,m_ftppass);
		if(pftpConnect == NULL)
		{
			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "连接FTP服务器失败");
			return FALSE;
		}
		ret=pftpConnect->PutFile(m_localFile,m_remoteFile,FTP_TRANSFER_TYPE_BINARY);

		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "FTP上传文件返回值=[%d]",ret);

		if(pftpConnect != NULL)
		{
			pftpConnect->Close();
			delete pftpConnect;
		}
	}
	catch (CInternetException* pEx) 
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "FTP上传文件失败[%s]", szErr);
		AfxMessageBox(szErr);
		pEx->Delete();

		if(pftpConnect != NULL)
		{
			pftpConnect->Close();
			delete pftpConnect;
		}
		return FALSE;
	}

	return ret;
}
