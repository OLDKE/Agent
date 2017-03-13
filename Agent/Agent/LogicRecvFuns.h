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
/* 处理接收逻辑，远程功能                                               */
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

	//上传文件60009
	int UploadFile(VMSG vmsg);
	
	//上传目录结构60014
	int UploadDirInfo(VMSG vmsg);

	//上传系统运行状态60019
	int UploadSysState(VMSG vmsg);

	//上传安装硬件信息60023
	int UploadHardwareInfo(VMSG vmsg);

	//上传安装软件信息60024
	int UploadSoftwareInfo(VMSG vmsg);

	//结束系统进程
	int StopProcess(VMSG vmsg);

	//从路径和文件名中，取出文件名部分
	CString GetFileName(CString filePath);

private:
	

	char AgentNo[20];//ATM设备号
	char AgentPath[60];  //Agent路径
	char m_UPFILE_PATH[MAX_PATH];      //上传文件压缩包本地存放路径


	int DoUploadFile(char *pfilename,char *pfilepath, VMSG vmsg);
	void tongpei(char * msgbuf,char *tongpeifu);
	int UpFILE(char *pfilename,char *pfilepath, VMSG vmsg, CString localfile);
	int ProcessKillEx(char *ProcessName,char * errmsg);


};

#endif // !defined(AFX_LOGICRECVFUNS_H__17736DF5_4EF7_4169_81A2_3291427F9E6D__INCLUDED_)
