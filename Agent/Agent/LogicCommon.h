// LogicCommon.h: interface for the CLogicCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGICCOMMON_H__EC5A437F_4693_4787_8E96_8B344E4E3D2C__INCLUDED_)
#define AFX_LOGICCOMMON_H__EC5A437F_4693_4787_8E96_8B344E4E3D2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//ע������·��
#define REG_CONFIG "SOFTWARE\\Ebring\\Agent\\Config"
#define REG_TMP "SOFTWARE\\Ebring\\Agent\\Tmp"
//�ļ���Ż���·��
#define DOWNLOAD_SOF "\\DownLoad\\SOF"
#define DOWNLOAD_ADV "\\DownLoad\\ADV"
#define DOWNLOAD_AGENT "\\DownLoad\\AGENT"

#define UNZIP_PATH "\\UnZip"
#define BACKUP_SOF "\\BackUp\\SOF"
#define BACKUP_ADV "\\BackUp\\ADV"
#define BACKUP_AGENT "\\BackUp\\AGENT"
#define UPFILE_PATH "\\UPFILE"

//����RDP�Ľ�����
#define STARTRDP  "301001"
#define PAUSERDP  "301002"
#define RESTARTOS "301003"

//���ر���״̬����
#define RET_DOIN "2"	//����ִ��
#define RET_SUCC "3"	//ִ�гɹ�
#define RET_FAIL "4"	//ִ��ʧ��

//RUNSTӦ������״̬
//#define RUNST_START   "1"	//����״̬
//#define RUNST_PAUSE   "2"	//��ͣ״̬
//#define RUNST_PROTECT "3"	//ά��״̬

//RUNSTӦ������״̬
#define RUNST_RUN    "1"	//���ڽ���
#define RUNST_IDEL   "2"	//����״̬
#define RUNST_STOP   "3"	//ֹͣ����
#define RUNST_MANG   "4"	//ά��״̬


//��־����
#define LOG_GRP		"EbringAgent"			//�����ļ���־�����
#define LOG_STEP	"EbringAgent.log"				//״̬������־�ļ���
#define LOG_STAT	"EbringAgent.log"				//״̬������־�ļ���
#define LOG_TRAN	"EbringAgent.log"				//���׷�����־�ļ���
#define LOG_CTRL	"EbringAgent.log"			//����������־�ļ���
#define LOG_COMM	"EbringAgent.log"			//����������־�ļ���

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
/* �����߼���ͨ��״̬����                                               */
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
