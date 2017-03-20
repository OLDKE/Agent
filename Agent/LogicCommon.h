// LogicCommon.h: interface for the CLogicCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICCOMMON_H__EC5A437F_4693_4787_8E96_8B344E4E3D2C__INCLUDED_)
#define AFX_LOGICCOMMON_H__EC5A437F_4693_4787_8E96_8B344E4E3D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//注册表基本路径
#define REG_CONFIG "SOFTWARE\\Ebring\\Agent\\Config"
#define REG_TMP "SOFTWARE\\Ebring\\Agent\\Tmp"
//文件存放基本路径
#define DOWNLOAD_SOF "\\DownLoad\\SOF"
#define DOWNLOAD_ADV "\\DownLoad\\ADV"
#define DOWNLOAD_AGENT "\\DownLoad\\AGENT"

#define UNZIP_PATH "\\UnZip"
#define BACKUP_SOF "\\BackUp\\SOF"
#define BACKUP_ADV "\\BackUp\\ADV"
#define BACKUP_AGENT "\\BackUp\\AGENT"
#define UPFILE_PATH "\\UPFILE"

//控制RDP的交易码
#define STARTRDP  "301001"
#define PAUSERDP  "301002"
#define RESTARTOS "301003"

//返回报文状态定义
#define RET_DOIN "2"	//正在执行
#define RET_SUCC "3"	//执行成功
#define RET_FAIL "4"	//执行失败

//RUNST应用启动状态
//#define RUNST_START   "1"	//启动状态
//#define RUNST_PAUSE   "2"	//暂停状态
//#define RUNST_PROTECT "3"	//维护状态

//RUNST应用启动状态
#define RUNST_RUN    "1"	//正在交易
#define RUNST_IDEL   "2"	//空闲状态
#define RUNST_STOP   "3"	//停止服务
#define RUNST_MANG   "4"	//维护状态


//日志配置
#define LOG_GRP		"EbringAgent"			//配置文件日志相关域
#define LOG_STEP	"EbringAgent.log"				//状态发送日志文件名
#define LOG_STAT	"EbringAgent.log"				//状态发送日志文件名
#define LOG_TRAN	"EbringAgent.log"				//交易发送日志文件名
#define LOG_CTRL	"EbringAgent.log"			//控制命令日志文件名
#define LOG_COMM	"EbringAgent.log"			//控制命令日志文件名

typedef BOOL(_stdcall *UNZIP)(LPCTSTR, LPCTSTR);//Ex_UnZip
typedef BOOL(_stdcall *ZIP)(LPCTSTR,LPCTSTR, LPCTSTR,bool create,int pathlevel);//Ex_Zip

typedef struct _V_MSG
{
	char DEVID[8+1];
	char TRNTYPE[16+1];
	char TRNCODE[16+1];
	char TRNEJ[40+1];
	char PTIME[40+1];
	char PARA1[128+1];
	char PARA2[128+1];
	char PARA3[128+1];
	char PARA4[128+1];
	char PARA5[128+1];
	char PARA6[128+1];
	char RESPTYPE[128+1];
}VMSG, *PVMSG;


#define NCR_STARTSERVICE "C:\\install\\Update\\StartService"
#define NCR_STOPSERVICE "C:\\install\\Update\\StopService"
#define NCR_REBOOTSERVICE "C:\\install\\Update\\Reboot"
#define NCR_SHUTDOWNSERVICE "C:\\install\\Update\\Shutdown"


/************************************************************************/
/* 处理逻辑，通用状态处理                                               */
/************************************************************************/
class CLogicCommon  
{
public:
	CLogicCommon();
	virtual ~CLogicCommon();

	static BOOL CreateDirByPath(CString path, BOOL hasFileName=FALSE);
	static BOOL CreateFile(CString strFileName);
};

#endif // !defined(AFX_LOGICCOMMON_H__EC5A437F_4693_4787_8E96_8B344E4E3D2C__INCLUDED_)
