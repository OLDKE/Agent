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

//60002启动应用
int CLogicRecvCtrl::StartApp(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到启动应用命令");

	if(CLogicCommon::CreateFile(NCR_STARTSERVICE)) return 0;

	return -1;

	//原来与RDP配合进行的重启，会等待ATMC是否已经重启再向平台发送结果
	//int ret = -1;
	//char runsta[10] = {0};
	////为0应用状态为启动服务，为1状态为暂停服务
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",runsta);

	//if( !strcmp(runsta, RUNST_RUN) || !strcmp(runsta, RUNST_IDEL) )
	//{
	//	//如果目前运行状态为启动，直接返回成功
	//	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "应用已经在启动状态，直接返回成功");
	//	ret = 0;
	//}
	//else
	//{
	//	ret = WaitCmd("60002");
	//}

	//return ret;
}

//60003暂停应用
int CLogicRecvCtrl::PauseApp(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到暂停应用命令");

	if(CLogicCommon::CreateFile(NCR_STOPSERVICE)) return 0;

	return -1;

	/*
	int ret = -1;
	char runsta[10] = {0};
	//为0应用状态为启动服务，为1状态为暂停服务
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",runsta);

	if( !strcmp(runsta, RUNST_STOP) )
	{
		//如果目前运行状态为暂停，直接返回成功
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "应用已经在暂停状态，直接返回成功");
		ret = 0;
	}
	else
	{
		ret = WaitCmd("60003");
	}

	return ret;
	*/
}

//重启系统60004
int CLogicRecvCtrl::ResetOS(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到重启系统命令");


	if(CLogicCommon::CreateFile(NCR_REBOOTSERVICE)) return 0;

	return -1;

//	int ret = -1;
//
//	//先暂停应用
//	//ret = PauseApp(vmsg);
//
//	char SendMsg[128] = {0};
//	GetRdpCmd(RESTARTOS, SendMsg);//整理命令报文
//	//向RDP发送命令报文  
//    if( !m_tcpser->SendMsg(SendMsg, strlen(SendMsg)) )
//	{
//		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "给RDP发送重启命令失败");
//		return -3;
//	}
//	else
//	{
//		ret = 0;
//	}
//
//	//根据暂停结果，先于暂停设备返回执行情况，函数调用处不用再处理返回
//	DoRet(ret, vmsg);
//
//	//如果暂停成功，重新启机器
//// 	if(ret == 0)
//// 	{
//// 		SetPrivilege(SE_SHUTDOWN_NAME,1);
//// 		ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
//// 	}
//	
//	return ret;
}

//强制重启系统60005
int CLogicRecvCtrl::ResetOSForc(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到强制重启系统命令");
	int ret = 0;
	
	//直接返回成功，并执行重启操作
	DoRet(ret, vmsg);
	
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");

	SetPrivilege(SE_SHUTDOWN_NAME,1);
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0);
	
	return ret;

}

//关机60006
int CLogicRecvCtrl::CloseOS(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到关闭系统命令");

	if( CLogicCommon::CreateFile(NCR_SHUTDOWNSERVICE) ) return 0;

	return -1;

	//int ret = -1;
	//
	////先暂停应用
	//ret = PauseApp(vmsg);
	//
	////根据暂停结果，先于暂停设备返回执行情况，函数调用处不用再处理返回
	//DoRet(ret, vmsg);
	//
	////如果暂停成功，重新启机器
	//if(ret == 0)
	//{
	//	SetPrivilege(SE_SHUTDOWN_NAME,1);
	//	ExitWindowsEx(EWX_POWEROFF|EWX_FORCE,0);
	//}
	//
	//return ret;
}

//强制关机60007
int CLogicRecvCtrl::CloseOSForc(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到强制关闭系统命令");
	int ret = 0;
	
	//直接返回成功，并执行重启操作
	DoRet(ret, vmsg);
	
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");

	SetPrivilege(SE_SHUTDOWN_NAME,1);
	ExitWindowsEx(EWX_POWEROFF|EWX_FORCE,0);
	
	return ret;
}

//同步时间60008
int CLogicRecvCtrl::SyncTime(VMSG vmsg)
{

	int ret = -1;
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","PTIME",vmsg.PTIME);
			
	ret = Upsystime() ? 0 : -3;

	return ret;

}

//同步设备状态60030
int CLogicRecvCtrl::SyncState(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到同步系统状态命令");

	char SendMsg[6000];
	memset(SendMsg,0,sizeof(SendMsg));
	int SendMsgLen=0;

	if(!m_packxml->PackStat(SendMsg,&SendMsgLen))
	{
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "打包状态信息错误");
		goto errtag;
	}
	SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat","RESP","0");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"同步设备状态, 发送前数据=[%s]", SendMsg);
	if(!m_udpser->SendMsg(SendMsg,SendMsgLen))
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"发送状态信息失败");
		goto errtag;
	}

	//给服务端发送更新成功
	memset(SendMsg, 0, sizeof(SendMsg));
	m_packxml->PackComdRet(SendMsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "");
	m_udpser->SendMsg(SendMsg, strlen(SendMsg));
	
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "执行同步系统状态命令成功");

	return 0;

errtag:
	//给服务端发送失败消息
	memset(SendMsg, 0, sizeof(SendMsg));
	m_packxml->PackComdRet(SendMsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "");
	m_udpser->SendMsg(SendMsg, strlen(SendMsg));
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "执行同步系统状态命令失败");
	return -1;
	
}

//强制清除命令60099
int CLogicRecvCtrl::ClearHostCmd(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "收到强制清除命令");

	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_START","0");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_PAUSE","0");
	Sleep(500);

	return 0;
}

//启动或暂停ATMC应用
int CLogicRecvCtrl::WaitCmd(char * trncode)
{
	int waitmax=1000;
	char szValue[_MAX_PATH] = {0};
	char trncodename[_MAX_PATH] = {0};
	char CmdFlag[20] = {0};
	
	//只接受启动、暂停两种命令的等待
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
	
	//设置有命令正在执行
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "1");
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "1");
	
	char SendMsg[128] = {0};
	GetRdpCmd(CmdFlag, SendMsg);//整理命令报文
	//向RDP发送命令报文  
    if( !m_tcpser->SendMsg(SendMsg, strlen(SendMsg)) )
	{
		SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "0");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "0");
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "向RDP发送命令错误[%s]", SendMsg);
		return -3;
	}
	
	int waitnum=0;
	while (1)
	{
		Sleep(1500);
		waitnum++;
		if(waitnum>waitmax)
		{
			//等待超时(25分钟)，清除命令并返回
			SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", trncodename, "0");
			SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "0");
			return -2;	
		}
		memset(szValue, 0, sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG",szValue);
		if(!strcmp(szValue,"3")||!strcmp(szValue,"4")||!strcmp(szValue,"0"))	//3表示成功，4表示失败
		{
			Sleep(2000);
			
			memset(szValue,0,sizeof(szValue));
			GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd",trncodename,szValue);
			if(!strcmp(szValue,"3"))
			{	
				//成功
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd",trncodename,"0");
				SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG","0");
				
				//发送状态报文
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
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "交易码错误") )
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
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "等待执行命令超时") )
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
		if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "执行命令失败") )
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
	
	//获得应用的TOKEN句柄和Priviliege值
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
	
	// 使能 SE_SHUTDOWN_NAME值
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
