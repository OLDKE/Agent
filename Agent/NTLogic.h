#pragma once

#include <afx.h>
#include <imagehlp.h>
#include "NTTRACE.h"
#include "Pack.h"
#include "UDPServer.h"
#include "TCPServer.h"
#include "MyFTP.h"
//#include "MyZip.h"
#include "BrowseDir.h"
#include "PackXML.h"
#include "SysStatus.h"
#include "GetPCResours.h"
#include "MyDes.h"

#include "LogicCommon.h"
#include "LogicRecvCtrl.h"
#include "LogicRecvFuns.h"
#include "LogicRecvDisp.h"

//XFS
 #include ".\XFS\XFSADMIN.H"
 #include ".\XFS\XFSAPI.H"
 #include ".\XFS\XFSCAM.H"
 #include ".\XFS\XFSCDM.H"
 #include ".\XFS\XFSCHK.H"
 #include ".\XFS\Xfsconf.h"
 #include ".\XFS\XFSDEP.H"
 #include ".\XFS\XFSIDC.H"
 #include ".\XFS\XFSPIN.H"
 #include ".\XFS\XFSPTR.H"
 #include ".\XFS\XFSSIU.H"
 #include ".\XFS\XFSSPI.H"
 #include ".\XFS\XFSTTU.H"
 #include ".\XFS\XFSVDM.H"
 #include ".\XFS\XFSCIM.H"
 #include "NTXFS_ALL.h"

#define STATTIMER_EVENT 60
#define TRANTIMER_EVENT 61
#define RTCARDTIMER_EVENT 62
#define SNRTIMER_EVENT 63



//#define DES_KEY "12345678"

typedef struct
{
	char trncode[256];
	LPVOID m_ntlogic;
	LPVOID pvmsg;
}s_ThreadPara;


//1表示全局路径，2表示绝对路径，3表示相对路径，4表示不存路径
// NTLogic 对话框
class NTLogic : public CDialog
{
	DECLARE_DYNAMIC(NTLogic)


		//for XFS
public:
 	HSERVICE ptr_h_add[256];
 	HSERVICE idc_h_add[256];
 	HSERVICE cdm_h_add[256];
 	HSERVICE pin_h_add[256];
 	HSERVICE cim_h_add[256];

	HANDLE hThread[256];
 	NTXFS_CDM	Ntxfs_cdm;
 	NTXFS_IDC	Ntxfs_idc;
     NTXFS_PTR	Ntxfs_ptr;
     NTXFS_PIN	Ntxfs_pin;
 	NTXFS_CIM	Ntxfs_cim;

	bool InItXFSManager();
	void Updatatime();
	void StartXfs();
	//bool SNR_RESP();

	void Pin_timer();
	void Cdm_timer();
	void Idc_timer();
	void Ptr_timer();
	void Cim_timer();

	
	static DWORD WINAPI Check_Ptr(LPVOID lpParam);
	static DWORD WINAPI Check_Idc(LPVOID lpParam);
	static DWORD WINAPI Check_Cdm(LPVOID lpParam);
	static DWORD WINAPI Check_Pin(LPVOID lpParam);
	static DWORD WINAPI Check_Cim(LPVOID lpParam);
	static DWORD WINAPI Check_Xfs(LPVOID lpParam);
	   

	bool XFSStatus_PTR();
	bool XFSStatus_IDC();
	bool XFSStatus_CDM();
	bool XFSStatus_PIN();
	bool XFSStatus_CIM();


	bool InXFSStatus_PTR();
	bool InXFSStatus_IDC();
	bool InXFSStatus_CDM();
	bool InXFSStatus_PIN();
	bool InXFSStatus_CIM();

	void ConvertErrCode_CDM(char* name,int inerr,char* outerr);
	void ConvertErrCode_IDC(char* name,int inerr,char* outerr);
	void ConvertErrCode_PTR(char* name,int inerr,char* outerr);
	void ConvertErrCode_PIN(char* name,int inerr,char* outerr);	
	void ConvertErrCode_CIM(char* name,int inerr,char* outerr);

public:
	NTLogic(CNTTRACE *pTrace,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NTLogic();
	char AgentNo[20];//ATM设备号
	char AgentPath[60];  //Agent路径
	int LocalPos;	//SNR上送报文的注册表的位置。

	//Agent程序运行相关路径
	char m_DOWNLOAD_SOF[MAX_PATH];     //软件升级本地下载路径
	char m_DOWNLOAD_ADV[MAX_PATH];     //广告升级本地下载路径
	char m_DOWNLOAD_AGENT[MAX_PATH];   //Agent升级本地下载路径
	char m_UNZIP_PATH[MAX_PATH];       //解压后本地存放路径     
	char m_BACKUP_SOF[MAX_PATH];       //软件升级备份本地路径
	char m_BACKUP_ADV[MAX_PATH];       //广告升级备份本地路径
	char m_BACKUP_AGENT[MAX_PATH];     //Agent升级备份本地路径
	char m_UPFILE_PATH[MAX_PATH];      //上传文件压缩包本地存放路径

	bool match(char *model,char *src);
	Pack *m_pack;
	CNTTRACE *m_trace;
	CUDPServer *m_udpser;
	CTCPServer *m_tcpser;

	PackXML *m_packxml;
	CSysStatus *m_sys;
	MyFTP *m_ftp;
	CGetPCResours *m_resours;
	CMyDes *m_mydes;
	HINSTANCE m_hInstDLL;
	bool StartSer(char *Module);
	bool StopSer(char *Module);
	void Uplog();

UNZIP m_unzip;
ZIP m_zip;
//	void TimerSend_Stat();
	HANDLE m_cmdrun;
	static DWORD WINAPI CmdRUN(LPVOID lpParameter);   // 线程函数
	void tongpei(char * msgbuf,char *tongpeifu);

	CLogicCommon   m_logicCommon;
	CLogicRecvCtrl m_logicRecvCtrl;
	CLogicRecvFuns m_logicRecvFuns;
	CLogicRecvDisp m_logicRecvDisp;

private:
	char RptTrnTime[256];		//交易上送时间
	char RptRtcTime[256];		//吞卡上送时间
	char RptStaTime[256];		//状态上送时间

	char RefreshTime[256];		//状态刷新时间
	char RefreshTime_XFS[256];	//agent状态刷新时间
	char SendTime[256];			//状态发送时间
	char LogPath[256];			//日志路径
	char AppPath[256];			//程序路径
	HANDLE	hRevThread;			
	UINT Timer_Send;
	UINT Timer_Uplog;
	char CmdRES_curr[256];

	
	bool StartSer_Rev();
	bool StartSer_Snd();
	bool StartSer_Trn();
	bool StartSer_RtCard();
	bool StartSer_Uplog();
	bool StartSofUp();
	bool StartAdvUp();
	bool StartSer_SNR();

	bool CheckSndStat();//判断是否需要发送状态报文
	bool CheckSndComd();//判断是否需要发送命令返回报文
	bool CheckSndTran();//判断是否需要发送交易报文

	bool CheckUpVer_SOF();
	bool CheckUpVer_ADV();


	void TimerSend_Stat();
	void TimerSend_Comd();
	void TimerSend_Tran();
	void TimerSend_RtCard();
	void TimerSend_SNR();

	BOOL SetPrivilege(LPCTSTR lpszPrivilege,BOOL bEnablePrivilege);
	BOOL Upsystime();
	bool RespDo(char *resptype,char *respej);
	bool Chk_TRN_Resp();
	bool Chk_RTC_Resp();

	UINT_PTR StatTimer;
	UINT_PTR RtcaTimer;
	UINT_PTR TranTimer;

	bool ReplyFlag;		//通讯回执标志

	int UpLogHH;
	int UpLogMM;
	int oneflag;
	//服务启动标志
	bool Ser_Rev;
	bool Ser_Timer;
	bool Ser_Uplog;


	//别乱用这个函数
	bool MyCopyDirectory_t(LPCTSTR lpszFrom,LPCTSTR lpszTo)  
	{
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"MyCopyDirectory_t(%s,%s)",lpszFrom,lpszTo);

		ASSERT((lpszFrom != NULL) && (lpszTo != NULL));  
		if ((lpszFrom == NULL) || (lpszTo == NULL))  
			return  FALSE;  


		bool bRet = TRUE;  
		TCHAR szFrom[MAX_PATH], szTo[MAX_PATH];  
		memset(szFrom,0,sizeof(szFrom));  
		memset(szTo,0,sizeof(szTo));  
		lstrcpy(szFrom,lpszFrom);  
		lstrcpy(szTo,lpszTo);  

		SHFILEOPSTRUCT fileop;  
		fileop.hwnd = NULL;  
		fileop.wFunc = FO_COPY;  
		fileop.pFrom = szFrom;  
		fileop.pTo = szTo;  
		fileop.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;

		int ret = SHFileOperation(&fileop);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"MyCopyDirectory_t SHFileOperation return %d",ret);

		bRet = (ret == 0);    
		return  bRet;  
	}

	bool MyCopyDirectoryEx_t(LPCTSTR lpszFrom,LPCTSTR lpszTo,bool bRootIn)  //拷贝路径和文件
	{  
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"MyCopyDirectoryEx_t(%s,%s,%d)",lpszFrom,lpszTo,bRootIn);
		ASSERT((lpszFrom != NULL) &&  (lpszTo  !=  NULL));  
		if((lpszFrom == NULL) || (lpszTo == NULL))  
			return  FALSE;  

		CString tmpFrom,tmpTo;

		tmpFrom=lpszFrom;
		tmpFrom.TrimRight("\\");
		

		tmpTo=lpszTo;
		tmpTo.TrimRight("\\");
		

		CFileFind  ff;  
		if(!ff.FindFile(tmpFrom))
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Source Directory(%s) do not exsit",tmpFrom);
			return FALSE;
		}
		if(!ff.FindFile(tmpTo))
		{
			if(!CreateDirectory(tmpTo,NULL))
			{
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Create Dest Directory(%s) fail",tmpTo);
				return FALSE;
			}
		}
		if  (bRootIn)  
			return  MyCopyDirectory_t(tmpFrom,  tmpTo);  
		else  
		{  
			CString  strFindFile  =  lpszFrom;  
			strFindFile.TrimRight();  
			strFindFile.TrimRight(_T('\\'));  
			strFindFile  +=  _T("\\*.*");  

			BOOL  bWorking  =  ff.FindFile(strFindFile);  

			while(  bWorking  )  
			{  
				bWorking  =  ff.FindNextFile();  
				if(  ff.IsDots()  )  
					continue;  

				//  Only  process  the  file  
				if  (  !ff.IsDirectory()  )  
				{  
					//  Copy  file  
					CString  strTo  =  lpszTo;  
					strTo.TrimRight();  
					strTo.TrimRight(_T('\\'));  
					strTo  +=  _T("\\");  

					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CopyFile(%s,%s,FALSE)",ff.GetFilePath(),strTo + ff.GetFileName());
					if  (!CopyFile(ff.GetFilePath(),  strTo  +  ff.GetFileName(),  FALSE))
					{
						m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CopyFile false %u",GetLastError());
						return  FALSE;  
					}
				}  
				else  if  (!MyCopyDirectory_t(ff.GetFilePath(),tmpTo))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"MyCopyDirectoryEx_t false ");				
					return  FALSE;  
				}
			}  
			return  TRUE;  
		}  
	}

	bool DeleteDirectoryEx_t(char* sDirName)//删除目录
	{
		CFileFind tempFind;
		char sTempFileFind[200];
		CString tmpDir;

		tmpDir=sDirName;
		tmpDir.TrimRight("\\");
		
		if(!tempFind.FindFile(tmpDir))
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Directory(%s) do not exist",tmpDir);
			return FALSE;
		}
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"DeleteDirectoryEx_t(%s)",tmpDir);
		sprintf(sTempFileFind,"%s\\*.*",tmpDir);
		BOOL IsFinded = tempFind.FindFile(sTempFileFind);
		while (IsFinded)
		{
			IsFinded = tempFind.FindNextFile();
			if (!tempFind.IsDots())
			{
				char sFoundFileName[200];
				strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200));
				if (tempFind.IsDirectory())
				{
					char sTempDir[200];
					sprintf(sTempDir,"%s\\%s",tmpDir,sFoundFileName);
					DeleteDirectoryEx_t(sTempDir);
				}
				else
				{
					char sTempFileName[200];
					sprintf(sTempFileName,"%s\\%s",tmpDir,sFoundFileName);
					SetFileAttributes(sTempFileName,FILE_ATTRIBUTE_NORMAL);
					if(!DeleteFile(sTempFileName))
					{
						m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"DeleteFile(%s) false %u",sTempFileName,GetLastError());
					}
				}
			}
		}
		tempFind.Close();
		if(!RemoveDirectory(tmpDir))
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"RemoveDirectory(%s) false",tmpDir);
			return FALSE;
		}
		m_trace->WTrace(LOG_GRP, LOG_STEP,LT_INFO,"RemoveDirectory(%s) true",tmpDir);
		return TRUE;
	}



	// 对话框数据
	enum { IDD = IDD_NTLOGIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL PackMsg_Flag;
	//打包命令回执报文
	//bool PackComdRet(char* PackStr,char* Trncode,char* Comd_Res,int TranEJflag,char *ErrMsg); 
		//打包命令回执报文
	//bool PackComdRet2(char* PackStr,char* Trncode,char* Comd_Res,int TranEJflag); 
	//打包状态报文
	bool PackStat(char *PackStr,int *PackLen);
	//打包交易报文（首次发送未收到回执时再次发送）
	bool PackTRN_resp(int TranIndex,char *PackStr, int *PackLen);
	//打包交易报文
	bool PackTRN(int &pos,char *PackStr, int *PackLen);
	//打包冠字号报文
	bool PackSNR(int &pos, char *PackStr, int *PackLen);
	//打包吞卡报文（首次发送未收到回执时再次发送）
	bool PackRTC_resp(int RTCIndex,char *PackStr, int *PackLen);
	//打包吞卡报文
	bool PackRTC(char* rpttime,char *PackStr, int *PackLen);
	//解命令报文
	//int UnPack(char *unpackstr, int unpackstrlen, VMSG vmsg);
    //拷贝文件至指定路径
	int CopyFile_EX(char *filefrom,char *fileto,char *filefail);
	//删除指定文件
	bool DeletFile_EX(char *filename);
	//执行bat文件函数
	int ControlApp(char *cmd,char  *currpath);
	//重启Agent需要该函数
	void StartAgent();
	//关闭进程函数
	int ProcessKillEx(char *ProcessName,char * errmsg);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnRecvLogic(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpLog(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUpdata_SOF(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnUpdata_ADV(WPARAM wParam, LPARAM lParam);



};
