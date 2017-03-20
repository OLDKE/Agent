// LogicRecvFuns.h: interface for the CLogicRecvFuns class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICRECVFUNS_H__17736DF5_4EF7_4169_81A2_3291427F9E6D__INCLUDED_)
#define AFX_LOGICRECVFUNS_H__17736DF5_4EF7_4169_81A2_3291427F9E6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyFTP.h"
#include "PackXML.h"
#include "UDPServer.h"
#include "SysStatus.h"
#include "GetPCResours.h"

/************************************************************************/
/* ��������߼���Զ�̹���                                               */
/************************************************************************/
class CLogicRecvFuns  
{
public:
	CLogicRecvFuns();
	virtual ~CLogicRecvFuns();

	CNTTRACE *m_trace;
	CUDPServer *m_udpser;
	PackXML *m_packxml;
	MyFTP *m_ftp;
	CSysStatus *m_sys;
	CGetPCResours *m_resours;
	UNZIP m_unzip;
	ZIP m_zip;

	//�ϴ��ļ�60009
	int UploadFile(VMSG vmsg);
	
	//�ϴ�Ŀ¼�ṹ60014
	int UploadDirInfo(VMSG vmsg);

	//�ϴ�ϵͳ����״̬60019
	int UploadSysState(VMSG vmsg);

	//�ϴ���װӲ����Ϣ60023
	int UploadHardwareInfo(VMSG vmsg);

	//�ϴ���װ�����Ϣ60024
	int UploadSoftwareInfo(VMSG vmsg);

	//����ϵͳ����
	int StopProcess(VMSG vmsg);

	//��·�����ļ����У�ȡ���ļ�������
	CString GetFileName(CString filePath);

private:
	

	char AgentNo[20];//ATM�豸��
	char AgentPath[60];  //Agent·��
	char m_UPFILE_PATH[MAX_PATH];      //�ϴ��ļ�ѹ�������ش��·��


	int DoUploadFile(char *pfilename,char *pfilepath, VMSG vmsg);
	void tongpei(char * msgbuf,char *tongpeifu);
	int UpFILE(char *pfilename,char *pfilepath, VMSG vmsg, CString localfile);
	int ProcessKillEx(char *ProcessName,char * errmsg);


};

#endif // !defined(AFX_LOGICRECVFUNS_H__17736DF5_4EF7_4169_81A2_3291427F9E6D__INCLUDED_)
