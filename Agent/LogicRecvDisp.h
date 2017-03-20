// LogicRecvDisp.h: interface for the CLogicRecvDisp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICRECVDISP_H__9D68B5B3_0E89_4EF8_9B5A_02E55506B9CD__INCLUDED_)
#define AFX_LOGICRECVDISP_H__9D68B5B3_0E89_4EF8_9B5A_02E55506B9CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogicCommon.h"
#include "MyFTP.h"
#include "PackXML.h"
#include "UDPServer.h"
#include "SysStatus.h"
#include "GetPCResours.h"
#include "LogicRecvCtrl.h"

/************************************************************************/
/* 处理接收逻辑，软件分发                                               */
/************************************************************************/
class CLogicRecvDisp  
{
public:
	CLogicRecvDisp();
	virtual ~CLogicRecvDisp();

	CNTTRACE *m_trace;
	CUDPServer *m_udpser;
	PackXML *m_packxml;
	MyFTP *m_ftp;
	CLogicRecvCtrl *m_logicRecvCtrl;
	UNZIP m_unzip;
	ZIP m_zip;

	//软件分发外部调用方法
	int CommonDisp(VMSG vmsg);

	//下载更新包
	int DownLoadUpdateFile(VMSG vmsg);
	//前置操作
	int CommonDispPre(VMSG vmsg);
	//执行更新操作
	int CommonDispDo(VMSG vmsg);
	//后置操作
	int CommonDispAft(VMSG vmsg);

	//软件更新60010,60011
	int DispSoft(VMSG vmsg);

	//广告更新60012,60013
	int DispAdv(VMSG vmsg);

	//Agent更新60018
	int DispAgent(VMSG vmsg);

private:
	

	//执行命令
	int ControlApp(char *cmd, char *currpath);






};

#endif // !defined(AFX_LOGICRECVDISP_H__9D68B5B3_0E89_4EF8_9B5A_02E55506B9CD__INCLUDED_)
