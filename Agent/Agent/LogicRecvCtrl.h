// LogicRecvCtrl.h: interface for the CLogicRecvCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICRECVCTRL_H__176F0EBB_3C10_4B6A_982D_0B34DBF6E994__INCLUDED_)
#define AFX_LOGICRECVCTRL_H__176F0EBB_3C10_4B6A_982D_0B34DBF6E994__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogicCommon.h"
#include "NTTRACE.h"
#include "TCPServer.h"
#include "UDPServer.h"
#include "PackXML.h"

/************************************************************************/
/* 处理接收逻辑，远程控制                                               */
/************************************************************************/
class CLogicRecvCtrl  
{
public:
	CLogicRecvCtrl();
	virtual ~CLogicRecvCtrl();

	//启动应用60002
	int StartApp(VMSG vmsg);

	//暂停应用60003
	int PauseApp(VMSG vmsg);

	//重启系统60004
	int ResetOS(VMSG vmsg);

	//强制重启系统60005
	int ResetOSForc(VMSG vmsg);

	//关机60006
	int CloseOS(VMSG vmsg);

	//强制关机60007
	int CloseOSForc(VMSG vmsg);

	//同步时间60008
	int SyncTime(VMSG vmsg);

	//同步设备状态60030
	int SyncState(VMSG vmsg);

	//强制清除命令60099
	int ClearHostCmd(VMSG vmsg);

	//启动或暂停ATMC应用
	int WaitCmd(char * trncode);

	//组织控制ATMC的报文
	void GetRdpCmd(char * CmdFlag, char * StrCmd);

	//处理命令执行结果
	void CLogicRecvCtrl::DoRet(int ret, VMSG vmsg);

public:
	CNTTRACE *m_trace;
	CUDPServer *m_udpser;
	CTCPServer *m_tcpser;
	PackXML *m_packxml;

private:
	//设置操作权限
	BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege);
	//更新系统时间
	BOOL Upsystime();

};

#endif // !defined(AFX_LOGICRECVCTRL_H__176F0EBB_3C10_4B6A_982D_0B34DBF6E994__INCLUDED_)
