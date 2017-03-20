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
/* ��������߼���Զ�̿���                                               */
/************************************************************************/
class CLogicRecvCtrl  
{
public:
	CLogicRecvCtrl();
	virtual ~CLogicRecvCtrl();

	//����Ӧ��60002
	int StartApp(VMSG vmsg);

	//��ͣӦ��60003
	int PauseApp(VMSG vmsg);

	//����ϵͳ60004
	int ResetOS(VMSG vmsg);

	//ǿ������ϵͳ60005
	int ResetOSForc(VMSG vmsg);

	//�ػ�60006
	int CloseOS(VMSG vmsg);

	//ǿ�ƹػ�60007
	int CloseOSForc(VMSG vmsg);

	//ͬ��ʱ��60008
	int SyncTime(VMSG vmsg);

	//ͬ���豸״̬60030
	int SyncState(VMSG vmsg);

	//ǿ���������60099
	int ClearHostCmd(VMSG vmsg);

	//��������ͣATMCӦ��
	int WaitCmd(char * trncode);

	//��֯����ATMC�ı���
	void GetRdpCmd(char * CmdFlag, char * StrCmd);

	//��������ִ�н��
	void CLogicRecvCtrl::DoRet(int ret, VMSG vmsg);

public:
	CNTTRACE *m_trace;
	CUDPServer *m_udpser;
	CTCPServer *m_tcpser;
	PackXML *m_packxml;

private:
	//���ò���Ȩ��
	BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege);
	//����ϵͳʱ��
	BOOL Upsystime();

};

#endif // !defined(AFX_LOGICRECVCTRL_H__176F0EBB_3C10_4B6A_982D_0B34DBF6E994__INCLUDED_)
