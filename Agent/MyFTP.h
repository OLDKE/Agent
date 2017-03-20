#pragma once
#include <afxinet.h>
#include "NTTRACE.h"
class MyFTP
{
public:
	MyFTP(CNTTRACE *pTrace);
	~MyFTP(void);

	bool SetAccessRight(CString userName,CString userPass);
	bool OpenConnection(CString server, CString port="");
	bool GetFile(CString remoteFile,CString localFile);
	bool PutFile(CString localFile,CString remoteFile);
	bool CloseConnection();

	CNTTRACE *m_trace;

private:
	CInternetSession *pInternetSession;
	CFtpConnection *pFtpConnection;
	CString strServerName;
	CString strPass;
	CString strUser;
};
