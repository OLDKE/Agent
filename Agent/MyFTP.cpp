#include "StdAfx.h"
#include "MyFTP.h"

#include "LogicCommon.h"

MyFTP::MyFTP(CNTTRACE *pTrace)
{
	m_trace=pTrace;
	pInternetSession = new CInternetSession(NULL,INTERNET_OPEN_TYPE_PRECONFIG);
	if(!pInternetSession)
	{
		return;
	}
}

MyFTP::~MyFTP(void)
{
	pInternetSession->Close();
	if(pInternetSession != NULL)
		delete pInternetSession;
}

bool MyFTP::SetAccessRight(CString userName,
							CString userPass)
{
	strPass = userPass;
	strUser = userName;
	if( (strPass == "") || (strUser == ""))
		return false;

	return true;
}

bool MyFTP::OpenConnection(CString server, CString port)
{
	if(server == "")
		return 0;

	int iport = INTERNET_DEFAULT_FTP_PORT;
	if(port.GetLength() != 0) iport = atoi(port);
		
	strServerName = server;	
	//m_trace->WTrace(LOG_GRP,LOG_COMM,LT_INFO,"The server is %s",server);
	try 
	{
		//pFtpConnection = pInternetSession->GetFtpConnection(strServerName,strUser,strPass,iport,TRUE);
		pFtpConnection = pInternetSession->GetFtpConnection(strServerName,strUser,strPass,iport,FALSE);
	} 
	catch (CInternetException* pEx) 
	{
		TCHAR szErr[512];
		pEx->GetErrorMessage(szErr, 512);
		szErr[strlen(szErr)]='\0';
		m_trace->WTrace(LOG_GRP,LOG_COMM,LT_ERROR,"MYFTP GetFtpConnection error[%s]",szErr);
		pEx->Delete();
		return false;
	}
	m_trace->WTrace(LOG_GRP,LOG_COMM,LT_DEBUG,"OpenConnection Success!!!");
	return true;
}

bool MyFTP::GetFile(CString remoteFile,CString localFile)
{
	// Try to get the file
	m_trace->WTrace(LOG_GRP,LOG_COMM,LT_DEBUG,"GetFile [%s] begin", remoteFile);
	BOOL bGotFile;
	try
	{
		bGotFile = pFtpConnection->GetFile(remoteFile,localFile,FALSE,FILE_ATTRIBUTE_NORMAL,
		FTP_TRANSFER_TYPE_BINARY |INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD | INTERNET_FLAG_RESYNCHRONIZE);	
		
		if (bGotFile==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_COMM,LT_ERROR,"GetFile ERR=[%d]remoteFile=[%s],localFile=[%s]",GetLastError(),remoteFile,localFile);
			return false;
		}
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[512];
		pEx->GetErrorMessage(szErr, 512);
		szErr[strlen(szErr)]='\0';
		m_trace->WTrace(LOG_GRP,LOG_COMM,LT_ERROR,"MYFTP GetFile error[%s]",szErr);
		pEx->Delete();
		return false;
	}
	m_trace->WTrace(LOG_GRP,LOG_COMM,LT_DEBUG,"GetFile end");
	return true;
}

bool MyFTP::PutFile(CString localFile,CString remoteFile)
{
	// Try to get the file
	m_trace->WTrace(LOG_GRP,LOG_COMM,LT_DEBUG,"Localfile=[%s],Remotefile=[%s]",localFile,remoteFile);
	BOOL bGotFile;
	try
	{
		bGotFile = pFtpConnection->PutFile(localFile,remoteFile,
		FTP_TRANSFER_TYPE_BINARY |INTERNET_FLAG_TRANSFER_BINARY|INTERNET_FLAG_RELOAD | INTERNET_FLAG_RESYNCHRONIZE);
		if (bGotFile==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_COMM,LT_ERROR,"put file error=[%d]",GetLastError());
			return false;
		}
		//m_trace->WTrace(LOG_GRP,LOG_COMM,LT_INFO,"bgotfile=[%d]",bGotFile);
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[512];
		pEx->GetErrorMessage(szErr, 512);
		szErr[strlen(szErr)]='\0';
		m_trace->WTrace(LOG_GRP,LOG_COMM,LT_ERROR,"MYFTP PutFile error[%s]",szErr);
		pEx->Delete();
		return false;
	}
	//m_trace->WTrace(LOG_GRP,LOG_COMM,LT_INFO,"put file success!");
	return true;
	//return bGotFile ? 1 : 0 ;
}

bool MyFTP::CloseConnection()
{
	// close the connection to server, you can reconnect latter
	if(pFtpConnection == NULL)
		return 0;
	try
	{
		pFtpConnection->Close();
	}
	catch(...)
	{
		return 0;
	}
	if(pFtpConnection != NULL)
		delete pFtpConnection;

	return 1;
}