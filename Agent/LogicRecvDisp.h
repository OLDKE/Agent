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
/* ��������߼�������ַ�                                               */
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

	//����ַ��ⲿ���÷���
	int CommonDisp(VMSG vmsg);

	//���ظ��°�
	int DownLoadUpdateFile(VMSG vmsg);
	//ǰ�ò���
	int CommonDispPre(VMSG vmsg);
	//ִ�и��²���
	int CommonDispDo(VMSG vmsg);
	//���ò���
	int CommonDispAft(VMSG vmsg);

	//�������60010,60011
	int DispSoft(VMSG vmsg);

	//������60012,60013
	int DispAdv(VMSG vmsg);

	//Agent����60018
	int DispAgent(VMSG vmsg);

private:
	

	//ִ������
	int ControlApp(char *cmd, char *currpath);






};

#endif // !defined(AFX_LOGICRECVDISP_H__9D68B5B3_0E89_4EF8_9B5A_02E55506B9CD__INCLUDED_)
