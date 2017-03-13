// LogicRecvDisp.cpp: implementation of the CLogicRecvDisp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicRecvDisp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicRecvDisp::CLogicRecvDisp()
{
	
}

CLogicRecvDisp::~CLogicRecvDisp()
{

}


//���ظ��°�
int CLogicRecvDisp::DownLoadUpdateFile(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "��ʼ��FTP�����������ļ�...");

	char szValue[_MAX_PATH] = {0};

	//FTP��ַ
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_IP", szValue);
	CString ftpip;
	ftpip.Format(_T("%s"), szValue);

	//FTP�˿�
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_PORT", szValue);
	CString ftpport;
	ftpport.Format(_T("%s"), szValue);

	//FTP�û���
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_USER", szValue);
	CString ftpusr;
	ftpusr.Format(_T("%s"), szValue);

	//FTP����
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_PASS", szValue);
	CString ftppass;
	ftppass.Format(_T("%s"), szValue);

	//�����ļ��汾
	char softver[128] = {0};
	strcpy(softver, vmsg.PARA1);

	//�����ļ�·��
	char sofpathp[256] = {0};
	strcpy(sofpathp, vmsg.PARA2);

	//�����ļ���
	char sofname[128] = {0};
	strcpy(sofname, vmsg.PARA3);

	//Զ�������ļ������������ļ���
	CString remotefile,localfile,remotefile_bat,localfile_bat;
	if(strchr(sofpathp,'\\') == NULL)
	{
		remotefile.Format( _T("%s/%s"), sofpathp, sofname );
		remotefile_bat.Format( _T("%s/%s.bat"), sofpathp, softver );
	}
	else
	{
		remotefile.Format( _T("%s\\%s"), sofpathp, sofname );
		remotefile_bat.Format( _T("%s\\%s.bat"), sofpathp, softver );
	}
	//���ص����غ�������ļ������������ļ���
	localfile.Format(_T("%s"), "c:\\EbringAgent\\Files\\sofupdata.zip");
	localfile_bat.Format(_T("%s"), "c:\\EbringAgent\\Files\\sofupdata_bat.bat");

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "׼�����ص��ļ�=[%s], ���غ��Ϊ�����ļ�=[%s]", remotefile, localfile);
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "׼�����ص��������ļ�=[%s], ���غ��Ϊ�����������ļ�=[%s]", remotefile_bat, localfile_bat);

	try
	{
		DeleteFile(_T(localfile));
		DeleteFile(_T(localfile_bat));

		//��FTP�����������ظ����ļ����������ļ�
		if(!m_ftp->SetAccessRight(ftpusr,ftppass))
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "����FTP�û����������user=[%s]", ftpusr);
			throw (-1);
		}

		if(!m_ftp->OpenConnection(ftpip, ftpport))
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "����FTP������ʧ�ܣ�ip=[%s]��port=[%s]", ftpip, ftpport);
			throw (-2);
		}

		if(!m_ftp->GetFile(remotefile,localfile))
		{
			m_ftp->CloseConnection();
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "���������ļ�ʧ�ܣ�׼�������ļ�=[%s], ��Ϊ�����ļ�=[%s]", remotefile, localfile);
			throw(-3);
		}
		
		if(!m_ftp->GetFile(remotefile_bat,localfile_bat))
		{
			m_ftp->CloseConnection();
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "�����������ļ�ʧ�ܣ�׼�������ļ�=[%s], ��Ϊ�����ļ�=[%s]", remotefile_bat, localfile_bat);
			throw(-3);
		}

		m_ftp->CloseConnection();

		//��ѹ���������ļ�
		if(!m_unzip("C:\\EbringAgent\\Files\\", localfile))//m_zip->unzip 
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "��ѹ���ص��ļ�ʧ�ܣ�file=[%s]", localfile);
			throw(-4);
		}
			
	}
	catch (CException* e)
	{
		char errbuff[1024] = {0};
		e->GetErrorMessage(errbuff, 256, NULL);
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "�����ļ�ʧ��, ������Ϣ=[%s]", errbuff);
		return -1;
	}
	catch (int iret)
	{
		return iret;
	}

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "�����ļ��ɹ�!!");
	return 0;
}

//ǰ����
int CLogicRecvDisp::CommonDispPre(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "����ַ�ǰ����ʼ...");

	//char szValue[_MAX_PATH];
	
	//���ݼ��ƽ̨Ҫ��ɱָ������
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KSPROCESS",szValue);
	char kills[256] = {0};
	strcpy(kills, vmsg.PARA5);
	
	char* tmpbuf = strtok(kills,"~");
	while (1)
	{
		if(tmpbuf == NULL )
			break;

		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "��������=[%s]", tmpbuf);
		MyKillProcess(tmpbuf);
		Sleep(1000);
		tmpbuf = strtok(NULL,"~");
	}
	
	Sleep(500);

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "����ַ�ǰ�������!!");

	return 0;
}

//ִ�и��²���
int CLogicRecvDisp::CommonDispDo(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ִ�и��²�����ʼ...");

	char sendmsg[1024] = {0};

	//ִ���������ļ������������ļ���ָ��λ��
	int iret=ControlApp("c:\\EbringAgent\\Files\\sofupdata_bat.bat", "c:\\EbringAgent\\Files\\");
	if(iret != 0)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "ִ������·��������ļ�ʧ��");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	/*
	//atmc�汾�ţ����汾��ֱ�Ӵ�RDP��fdp�л�ȡ
	//agent�汾���ǰ�װ��ֱ�Ӹ���AGT_VER���ٴ��·������л�ȡ(��ֹATMV����Ա�Ѱ汾���ô���)

	//�����·����ͣ����°汾��
	char sztype[32] = {0};
	char szver[64] = {0};
	//�汾
	strcpy(szver, vmsg.PARA1);
	//����
	strcpy(sztype, vmsg.PARA4);
	
	if( !strcmp(sztype, "1") )
		SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","SOF_VER",szver);
	else if( !strcmp(sztype, "2") )
		SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","ADV_VER",szver);
	else if( !strcmp(sztype, "3") )
		SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","CFG_VER",szver);
	else if( !strcmp(sztype, "4") )
		SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","AGT_VER",szver);
	*/
	//���͸��³ɹ���Ϣ
	//memset(sendmsg,0,sizeof(sendmsg));
	//m_packxml->PackComdRet(sendmsg,"","3",0,"");
	//m_udpser->SendMsg(sendmsg,strlen(sendmsg));

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ִ�и��²������!!");

	return 0;
}

//����
int CLogicRecvDisp::CommonDispAft(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "CommonDispAft begin...");

	int ret = -1;
	//�������־
	char rebootflag[32] = {0};
	strcpy(rebootflag, vmsg.PARA6);

	if( !strncmp(rebootflag, "1", 1) )
	{
		//����Ӧ��
		ret = m_logicRecvCtrl->StartApp(vmsg);
	}
	else if( !strncmp(rebootflag, "2", 1) )
	{
		//��������
		ret = m_logicRecvCtrl->ResetOS(vmsg);
	}
	else if( !strncmp(rebootflag, "3", 1) )
	{
		//ǿ������
		ret = m_logicRecvCtrl->ResetOSForc(vmsg);
	}
	else
	{
		ret = 0;
	}

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "CommonDispAft end!!");

	return ret;
}

//����������
int CLogicRecvDisp::CommonDisp(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "������¿�ʼ");

	char sendmsg[1024] = {0};
	int iret = -1;

	//1�����ظ��°�
	iret = DownLoadUpdateFile(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "���ظ����ļ�ʧ��");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	//2��ǰ����
	iret = CommonDispPre(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "ǰ�ô���ʧ��");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}

	//3��ִ�и��²���
	iret = CommonDispDo(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "ִ�и��²���ʧ��");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}

	//4������
	iret = CommonDispAft(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "���ô���ʧ��");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	//������˷��͸��³ɹ�
	memset(sendmsg, 0, sizeof(sendmsg));
	m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "");
	m_udpser->SendMsg(sendmsg, strlen(sendmsg));

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "���������������");
	
	return 0;
}

int CLogicRecvDisp::ControlApp(char *cmd, char *currpath)
{
	char oldcurrentdir[256];
	GetCurrentDirectory(256,oldcurrentdir);
	::SetCurrentDirectory(currpath);
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"SetCurrentDirectory:[%s]",currpath);
	int iret=-1;	
	BOOL bret=false;
	
	char cmd1[512];
	memset(cmd1,0,sizeof(cmd1));
	strcpy(cmd1,cmd);
	strcat(cmd1," >");
	strcat(cmd1,currpath);
	strcat(cmd1,"cmdret.txt");
	char filename[256];
	strcpy(filename,currpath);
	strcat(filename,"cmdret.txt");
	char ApplicationName[512];
	strcpy(ApplicationName,currpath);
	strcat(ApplicationName,"\\cmd.bat");
	DeleteFile(ApplicationName);
	FILE* file1;
	try
	{
		file1=fopen(ApplicationName,"a+");
		if(file1==NULL)
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL,LT_ERROR,"�����������ļ�ʧ��!: %u\n", GetLastError() );
			throw (-1);
			
		}
		int len1=0;
		len1=fwrite(cmd1,sizeof(char),strlen(cmd1),file1);
		fclose(file1);
		if(len1==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"д������������ʧ��!: %d\n", GetLastError() );			
			throw (-1);
			
		}
		STARTUPINFO   si;   
		memset(&si,0,sizeof(si));   
		si.dwFlags   =   STARTF_USESHOWWINDOW;   
		si.wShowWindow   =   SW_HIDE;   
		
		PROCESS_INFORMATION pinfo; 
		
        bret=CreateProcess(ApplicationName,NULL,NULL,NULL,1,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pinfo);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"��������%s",ApplicationName);
//		bret=CreateProcess(ApplicationName,NULL,NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,NULL,&info,&pinfo);
		if(!bret)
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"��������%s ʧ�ܣ�������%d",ApplicationName,GetLastError());
			throw (-1);
		}
		::Sleep(2000);
		DWORD pid=pinfo.dwProcessId;
		HANDLE process=OpenProcess(PROCESS_ALL_ACCESS|PROCESS_QUERY_INFORMATION|SYNCHRONIZE,FALSE,pid); 
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"�򿪳���%s",ApplicationName);
		if (WaitForSingleObjectEx(process,60000,TRUE)==WAIT_OBJECT_0) //���ǳ�ʱ
		{
			char buffer[256];
			memset(buffer,0,sizeof(buffer));
			FILE* file;
			file=fopen(filename,"r");
			if(file==NULL)
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"�������н���ı� ʧ�ܣ� ����%d",GetLastError());
				throw (-1);
			}
			int len=0;
			len=fread(buffer,sizeof(char),255,file);
			fclose(file);
			if(len==0)
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"��ȡ�����н��ʧ�ܣ� ����%d",GetLastError());
				throw (-1);
			}
			buffer[len]='\0';
			if (!strcmp(cmd,"c:\\SSTMANAGE OOS 30")||!strcmp(cmd,"c:\\SSTMANAGE GIS 30"))
			{
				if (!strcmp(buffer,"0"))
				{
					m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"SSTManage success! return [%s]",buffer);
					iret=0;				
				}
				if (!strcmp(buffer,"1"))
				{
					m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"SSTManage failed! return [%s]",buffer);
					iret=1;
				}
				if (!strcmp(buffer,"2"))
				{
					m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"SSTManage failed! return [%s]",buffer);
					iret=2;
				}
				else
				{
					m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"SSTManage failed! return [%s]",buffer);
					iret=-1;
				}
			}
			else
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"ִ��������%s,return [%s]",cmd,buffer);
				iret = 0;
			}

		}
		else
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�ȴ�ʧ��!");
			throw (-1);
		}

		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,_T("ִ������[%s],iret=[%d]"),cmd,iret);
		return iret;

	}
	catch (...)
	{
		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"controlappʧ��");
		return -1;
	}
}


//�������60010,60011
int CLogicRecvDisp::DispSoft(VMSG vmsg)
{
	return 0;
}