// LogicRecvCtrl.cpp: implementation of the CLogicRecvCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicRecvCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicRecvCtrl::CLogicRecvCtrl()
{

}

CLogicRecvCtrl::~CLogicRecvCtrl()
{

}

//60002����Ӧ��
int CLogicRecvCtrl::StartApp(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�����Ӧ������");

	if(CLogicCommon::CreateFile(NCR_STARTSERVICE)) return 0;

	return -1;

	//ԭ����RDP��Ͻ��е���������ȴ�ATMC�Ƿ��Ѿ���������ƽ̨���ͽ��
	//int ret = -1;
	//char runsta[10] = {0};
	////Ϊ0Ӧ��״̬Ϊ��������Ϊ1״̬Ϊ��ͣ����
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",runsta);

	//if( !strcmp(runsta, RUNST_RUN) || !strcmp(runsta, RUNST_IDEL) )
	//{
	//	//���Ŀǰ����״̬Ϊ������ֱ�ӷ��سɹ�
	//	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "Ӧ���Ѿ�������״̬��ֱ�ӷ��سɹ�");
	//	ret = 0;
	//}
	//else
	//{
	//	ret = WaitCmd("60002");
	//}

	//return ret;
}

//60003��ͣӦ��
int CLogicRecvCtrl::PauseApp(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ���ͣӦ������");

	if(CLogicCommon::CreateFile(NCR_STOPSERVICE)) return 0;

	return -1;

	/*
	int ret = -1;
	char runsta[10] = {0};
	//Ϊ0Ӧ��״̬Ϊ��������Ϊ1״̬Ϊ��ͣ����
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",runsta);

	if( !strcmp(runsta, RUNST_STOP) )
	{
		//���Ŀǰ����״̬Ϊ��ͣ��ֱ�ӷ��سɹ�
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "Ӧ���Ѿ�����ͣ״̬��ֱ�ӷ��سɹ�");
		ret = 0;
	}
	else
	{
		ret = WaitCmd("60003");
	}

	return ret;
	*/
}

//����ϵͳ60004
int CLogicRecvCtrl::ResetOS(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�����ϵͳ����");


	if(CLogicCommon::CreateFile(NCR_REBOOTSERVICE)) return 0;

	return -1;

//	int ret = -1;
//
//	//����ͣӦ��
//	//ret = PauseApp(vmsg);
//
//	char SendMsg[128] = {0};
//	GetRdpCmd(RESTARTOS, SendMsg);//���������
//	//��RDP���������  
//    if( !m_tcpser->SendMsg(SendMsg, strlen(SendMsg)) )
//	{
//		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "��RDP������������ʧ��");
//		return -3;
//	}
//	else
//	{
//		ret = 0;
//	}
//
//	//������ͣ�����������ͣ�豸����ִ��������������ô������ٴ�����
//	DoRet(ret, vmsg);
//
//	//�����ͣ�ɹ�������������
//// 	if(ret == 0)
//// 	{
//// 		SetPrivilege(SE_SHUTDOWN_NAME,1);
//// 		ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
//// 	}
//	
//	return ret;
}

//ǿ������ϵͳ60005
int CLogicRecvCtrl::ResetOSForc(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�ǿ������ϵͳ����");
	int ret = 0;
	
	//ֱ�ӷ��سɹ�����ִ����������
	DoRet(ret, vmsg);
	
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");

	SetPrivilege(SE_SHUTDOWN_NAME,1);
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
	
	return ret;

}

//�ػ�60006
int CLogicRecvCtrl::CloseOS(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ��ر�ϵͳ����");

	if( CLogicCommon::CreateFile(NCR_SHUTDOWNSERVICE) ) return 0;

	return -1;

	//int ret = -1;
	//
	////����ͣӦ��
	//ret = PauseApp(vmsg);
	//
	////������ͣ�����������ͣ�豸����ִ��������������ô������ٴ�����
	//DoRet(ret, vmsg);
	//
	////�����ͣ�ɹ�������������
	//if(ret == 0)
	//{
	//	SetPrivilege(SE_SHUTDOWN_NAME,1);
	//	ExitWindowsEx(EWX_POWEROFF|EWX_FORCE,0);
	//}
	//
	//return ret;
}

//ǿ�ƹػ�60007
int CLogicRecvCtrl::CloseOSForc(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�ǿ�ƹر�ϵͳ����");
	int ret = 0;
	
	//ֱ�ӷ��سɹ�����ִ����������
	DoRet(ret, vmsg);
	
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");

	SetPrivilege(SE_SHUTDOWN_NAME,1);
	ExitWindowsEx(EWX_POWEROFF|EWX_FORCE,0);
	
	return ret;
}

//ͬ��ʱ��60008
int CLogicRecvCtrl::SyncTime(VMSG vmsg)
{

	int ret = -1;
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","PTIME",vmsg.PTIME);
			
	ret = Upsystime() ? 0 : -3;

	return ret;

}

//ͬ���豸״̬60030
int CLogicRecvCtrl::SyncState(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�ͬ��ϵͳ״̬����");

	char SendMsg[6000];
	memset(SendMsg,0,sizeof(SendMsg));
	int SendMsgLen=0;

	if(!m_packxml->PackStat(SendMsg,&SendMsgLen))
	{
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "���״̬��Ϣ����");
		goto errtag;
	}
	SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat","RESP","0");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"ͬ���豸״̬, ����ǰ����=[%s]", SendMsg);
	if(!m_udpser->SendMsg(SendMsg,SendMsgLen))
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"����״̬��Ϣʧ��");
		goto errtag;
	}

	//������˷��͸��³ɹ�
	memset(SendMsg, 0, sizeof(SendMsg));
	m_packxml->PackComdRet(SendMsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "");
	m_udpser->SendMsg(SendMsg, strlen(SendMsg));
	
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ִ��ͬ��ϵͳ״̬����ɹ�");

	return 0;

errtag:
	//������˷���ʧ����Ϣ
	memset(SendMsg, 0, sizeof(SendMsg));
	m_packxml->PackComdRet(SendMsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "");
	m_udpser->SendMsg(SendMsg, strlen(SendMsg));
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ִ��ͬ��ϵͳ״̬����ʧ��");
	return -1;
	
}

//ǿ���������60099
int CLogicRecvCtrl::ClearHostCmd(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�յ�ǿ���������");

	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_START","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_PAUSE","0");
	Sleep(500);

	return 0;
}

//��������ͣATMCӦ��
int CLogicRecvCtrl::WaitCmd(char * trncode)
{
	int waitmax=1000;
	char szValue[_MAX_PATH] = {0};
	char trncodename[_MAX_PATH] = {0};
	char CmdFlag[20] = {0};
	
	//ֻ������������ͣ��������ĵȴ�
	if( !strcmp(trncode, "60002") ) 
	{
		strcpy(trncodename, "CMD_START");
		strcpy(CmdFlag, STARTRDP);	
	}
	else if( !strcmp(trncode, "60003") )
	{
		strcpy(trncodename, "CMD_PAUSE");
		strcpy(CmdFlag, PAUSERDP);
	}
	else
	{
		return -1;
	}
	
	//��������������ִ��
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "1");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "1");
	
	char SendMsg[128] = {0};
	GetRdpCmd(CmdFlag, SendMsg);//���������
	//��RDP���������  
    if( !m_tcpser->SendMsg(SendMsg, strlen(SendMsg)) )
	{
		SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "0");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "0");
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "��RDP�����������[%s]", SendMsg);
		return -3;
	}
	
	int waitnum=0;
	while (1)
	{
		Sleep(1500);
		waitnum++;
		if(waitnum>waitmax)
		{
			//�ȴ���ʱ(25����)������������
			SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "0");
			SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "0");
			return -2;	
		}
		memset(szValue, 0, sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG",szValue);
		if(!strcmp(szValue,"3")||!strcmp(szValue,"4")||!strcmp(szValue,"0"))	//3��ʾ�ɹ���4��ʾʧ��
		{
			Sleep(2000);
			
			memset(szValue,0,sizeof(szValue));
			GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd",trncodename,szValue);
			if(!strcmp(szValue,"3"))
			{	
				//�ɹ�
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd",trncodename,"0");
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");
				
				//����״̬����
				int msglen=0;
				char sendmsg[6000];
				memset(sendmsg,0,sizeof(sendmsg));
				if(!strcmp(trncode,"60002"))
					SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA", RUNST_IDEL);
				else if(!strcmp(trncode,"60003"))
					SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA", RUNST_STOP);
				
				return 0;				
			}
			else
			{
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd",trncodename,"0");
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");
				return -3;
			}
		}
		else
			continue;
	}
	return 0;
}

void CLogicRecvCtrl::GetRdpCmd(char * CmdFlag, char * StrCmd)
{
	sprintf(StrCmd, "%s|%s|%s", CmdFlag,"", "");
}

void CLogicRecvCtrl::DoRet(int cmdret, VMSG vmsg)
{
	char sendmsg[1024] = {0};
	//m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"PackMsg_Flag=[%d]",PackMsg_Flag);
	if(cmdret==0)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "") )
		{
			m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		}
		else
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PackComdRet error");
		}
	}
	else if (cmdret==-1)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "���������") )
		{
			m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		}
		else
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PackComdRet error");
		}
	}
	else if (cmdret==-2)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "�ȴ�ִ�����ʱ") )
		{
			m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		}
		else
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PackComdRet error");
		}
	}
	else if (cmdret==-3)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "ִ������ʧ��") )
		{
			m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		}
		else
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PackComdRet error");
		}
	}
}


BOOL CLogicRecvCtrl::SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	
	//���Ӧ�õ�TOKEN�����Priviliegeֵ
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"OpenProcessToken error: %u\n", GetLastError() ); 
		return FALSE;
	}
	
	if( !LookupPrivilegeValue(  NULL,           // lookup privilege on local system
		lpszPrivilege,  // privilege to lookup 
		&luid ) )		// receives LUID of privilege
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"LookupPrivilegeValue error: %u\n", GetLastError() ); 
		return FALSE; 
	}
	
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	
	if(bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;
	
	// ʹ�� SE_SHUTDOWN_NAMEֵ
	AdjustTokenPrivileges(  hToken, 
		FALSE, 
		&tp, 
		sizeof(TOKEN_PRIVILEGES), 
		(PTOKEN_PRIVILEGES) NULL, 
		(PDWORD) NULL); 
	
	DWORD dwRet ;
	if((dwRet = GetLastError()) != ERROR_SUCCESS)
	{ 
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"AdjustTokenPrivileges failed: %u\n", GetLastError() ); 
		return FALSE; 
	}
	return TRUE;
}

BOOL CLogicRecvCtrl::Upsystime()
{
	char systime[256];
	SYSTEMTIME st;
	GetLocalTime(&st);
	char YY[5];
	char MM[5];
	char DD[5];
	char hh[5];
	char mm[5];
	char ss[5];

	memset(YY,0,sizeof(YY));
	memset(MM,0,sizeof(MM));
	memset(DD,0,sizeof(DD));
	memset(hh,0,sizeof(hh));
	memset(mm,0,sizeof(mm));
	memset(ss,0,sizeof(ss));
	
	memset(systime,0,sizeof(systime));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","PTIME",systime);
	
	if(strlen(systime)<14)
		return FALSE;
	
	memcpy(YY,systime,4);
	memcpy(MM,systime+4,2);
	memcpy(DD,systime+4+2,2);
	memcpy(hh,systime+4+2+2,2);
	memcpy(mm,systime+4+2+2+2,2);
	memcpy(ss,systime+4+2+2+2+2,2);
	
	st.wYear = atoi(YY);
	if(st.wYear>2050 ||st.wYear <1990)
		return FALSE;
	
	st.wMonth = atoi(MM);
	if(st.wMonth>12 ||st.wMonth <1)
		return FALSE;
	
	st.wDay = atoi(DD);
	if(st.wDay>31 ||st.wDay <1)
		return FALSE;
	
	st.wHour = atoi(hh); 
	if(st.wHour>23 ||st.wHour <0)
		return FALSE;
	
	st.wMinute = atoi(mm);
	if(st.wMinute>59 ||st.wMinute <0)
		return FALSE;
	
	st.wSecond = atoi(ss);
	if(st.wSecond>59 ||st.wSecond <0)
		return FALSE;
	
	SetLocalTime(&st);
	return TRUE;
}
