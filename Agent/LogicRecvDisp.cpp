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


//下载更新包
int CLogicRecvDisp::DownLoadUpdateFile(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "开始从FTP服务器下载文件...");

	char szValue[_MAX_PATH] = {0};

	//FTP地址
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_IP", szValue);
	CString ftpip;
	ftpip.Format(_T("%s"), szValue);

	//FTP端口
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_PORT", szValue);
	CString ftpport;
	ftpport.Format(_T("%s"), szValue);

	//FTP用户名
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_USER", szValue);
	CString ftpusr;
	ftpusr.Format(_T("%s"), szValue);

	//FTP密码
	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "FTP_PASS", szValue);
	CString ftppass;
	ftppass.Format(_T("%s"), szValue);

	//更新文件版本
	char softver[128] = {0};
	strcpy(softver, vmsg.PARA1);

	//更新文件路径
	char sofpathp[256] = {0};
	strcpy(sofpathp, vmsg.PARA2);

	//更新文件名
	char sofname[128] = {0};
	strcpy(sofname, vmsg.PARA3);

	//远程数据文件名和批处理文件名
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
	//下载到本地后的数据文件名和批处理文件名
	localfile.Format(_T("%s"), "c:\\EbringAgent\\Files\\sofupdata.zip");
	localfile_bat.Format(_T("%s"), "c:\\EbringAgent\\Files\\sofupdata_bat.bat");

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "准备下载的文件=[%s], 下载后存为本地文件=[%s]", remotefile, localfile);
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "准备下载的批处理文件=[%s], 下载后存为本地批处理文件=[%s]", remotefile_bat, localfile_bat);

	try
	{
		DeleteFile(_T(localfile));
		DeleteFile(_T(localfile_bat));

		//从FTP服务器上下载更新文件和批处理文件
		if(!m_ftp->SetAccessRight(ftpusr,ftppass))
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "设置FTP用户名密码错误，user=[%s]", ftpusr);
			throw (-1);
		}

		if(!m_ftp->OpenConnection(ftpip, ftpport))
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "连接FTP服务器失败，ip=[%s]，port=[%s]", ftpip, ftpport);
			throw (-2);
		}

		if(!m_ftp->GetFile(remotefile,localfile))
		{
			m_ftp->CloseConnection();
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "下载数据文件失败，准备下载文件=[%s], 存为本地文件=[%s]", remotefile, localfile);
			throw(-3);
		}
		
		if(!m_ftp->GetFile(remotefile_bat,localfile_bat))
		{
			m_ftp->CloseConnection();
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "下载批处理文件失败，准备下载文件=[%s], 存为本地文件=[%s]", remotefile_bat, localfile_bat);
			throw(-3);
		}

		m_ftp->CloseConnection();

		//解压更新数据文件
		if(!m_unzip("C:\\EbringAgent\\Files\\", localfile))//m_zip->unzip 
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "解压下载的文件失败，file=[%s]", localfile);
			throw(-4);
		}
			
	}
	catch (CException* e)
	{
		char errbuff[1024] = {0};
		e->GetErrorMessage(errbuff, 256, NULL);
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "下载文件失败, 错误信息=[%s]", errbuff);
		return -1;
	}
	catch (int iret)
	{
		return iret;
	}

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "下载文件成功!!");
	return 0;
}

//前处理
int CLogicRecvDisp::CommonDispPre(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "软件分发前处理开始...");

	//char szValue[_MAX_PATH];
	
	//根据监控平台要求，杀指定进程
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KSPROCESS",szValue);
	char kills[256] = {0};
	strcpy(kills, vmsg.PARA5);
	
	char* tmpbuf = strtok(kills,"~");
	while (1)
	{
		if(tmpbuf == NULL )
			break;

		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "结束进程=[%s]", tmpbuf);
		MyKillProcess(tmpbuf);
		Sleep(1000);
		tmpbuf = strtok(NULL,"~");
	}
	
	Sleep(500);

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "软件分发前处理完成!!");

	return 0;
}

//执行更新操作
int CLogicRecvDisp::CommonDispDo(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "执行更新操作开始...");

	char sendmsg[1024] = {0};

	//执行批处理文件，拷贝更新文件到指定位置
	int iret=ControlApp("c:\\EbringAgent\\Files\\sofupdata_bat.bat", "c:\\EbringAgent\\Files\\");
	if(iret != 0)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "执行软件下发批处理文件失败");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	/*
	//atmc版本号，广告版本号直接从RDP的fdp中获取
	//agent版本号是安装包直接更新AGT_VER不再从下发报文中获取(防止ATMV管理员把版本设置错误)

	//根据下发类型，更新版本号
	char sztype[32] = {0};
	char szver[64] = {0};
	//版本
	strcpy(szver, vmsg.PARA1);
	//类型
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
	//发送更新成功消息
	//memset(sendmsg,0,sizeof(sendmsg));
	//m_packxml->PackComdRet(sendmsg,"","3",0,"");
	//m_udpser->SendMsg(sendmsg,strlen(sendmsg));

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "执行更新操作完成!!");

	return 0;
}

//后处理
int CLogicRecvDisp::CommonDispAft(VMSG vmsg)
{
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "CommonDispAft begin...");

	int ret = -1;
	//后操作标志
	char rebootflag[32] = {0};
	strcpy(rebootflag, vmsg.PARA6);

	if( !strncmp(rebootflag, "1", 1) )
	{
		//启动应用
		ret = m_logicRecvCtrl->StartApp(vmsg);
	}
	else if( !strncmp(rebootflag, "2", 1) )
	{
		//重启机器
		ret = m_logicRecvCtrl->ResetOS(vmsg);
	}
	else if( !strncmp(rebootflag, "3", 1) )
	{
		//强制重启
		ret = m_logicRecvCtrl->ResetOSForc(vmsg);
	}
	else
	{
		ret = 0;
	}

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "CommonDispAft end!!");

	return ret;
}

//更新主方法
int CLogicRecvDisp::CommonDisp(VMSG vmsg)
{

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "软件更新开始");

	char sendmsg[1024] = {0};
	int iret = -1;

	//1、下载更新包
	iret = DownLoadUpdateFile(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "下载更新文件失败");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	//2、前处理
	iret = CommonDispPre(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "前置处理失败");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}

	//3、执行更新操作
	iret = CommonDispDo(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "执行更新操作失败");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}

	//4、后处理
	iret = CommonDispAft(vmsg);
	if( iret != 0 )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "后置处理失败");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return -1;
	}
	
	//给服务端发送更新成功
	memset(sendmsg, 0, sizeof(sendmsg));
	m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "");
	m_udpser->SendMsg(sendmsg, strlen(sendmsg));

	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "软件更新正常结束");
	
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
			m_trace->WTrace(LOG_GRP, LOG_CTRL,LT_ERROR,"创建批处理文件失败!: %u\n", GetLastError() );
			throw (-1);
			
		}
		int len1=0;
		len1=fwrite(cmd1,sizeof(char),strlen(cmd1),file1);
		fclose(file1);
		if(len1==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"写入批处理命令失败!: %d\n", GetLastError() );			
			throw (-1);
			
		}
		STARTUPINFO   si;   
		memset(&si,0,sizeof(si));   
		si.dwFlags   =   STARTF_USESHOWWINDOW;   
		si.wShowWindow   =   SW_HIDE;   
		
		PROCESS_INFORMATION pinfo; 
		
        bret=CreateProcess(ApplicationName,NULL,NULL,NULL,1,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pinfo);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"创建程序%s",ApplicationName);
//		bret=CreateProcess(ApplicationName,NULL,NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,NULL,&info,&pinfo);
		if(!bret)
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"创建程序%s 失败，返回码%d",ApplicationName,GetLastError());
			throw (-1);
		}
		::Sleep(2000);
		DWORD pid=pinfo.dwProcessId;
		HANDLE process=OpenProcess(PROCESS_ALL_ACCESS|PROCESS_QUERY_INFORMATION|SYNCHRONIZE,FALSE,pid); 
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"打开程序%s",ApplicationName);
		if (WaitForSingleObjectEx(process,60000,TRUE)==WAIT_OBJECT_0) //考虑超时
		{
			char buffer[256];
			memset(buffer,0,sizeof(buffer));
			FILE* file;
			file=fopen(filename,"r");
			if(file==NULL)
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"打开命令行结果文本 失败！ 返回%d",GetLastError());
				throw (-1);
			}
			int len=0;
			len=fread(buffer,sizeof(char),255,file);
			fclose(file);
			if(len==0)
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_ERROR,"读取命令行结果失败！ 返回%d",GetLastError());
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
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,"执行命令行%s,return [%s]",cmd,buffer);
				iret = 0;
			}

		}
		else
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"等待失败!");
			throw (-1);
		}

		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_DEBUG,_T("执行命令[%s],iret=[%d]"),cmd,iret);
		return iret;

	}
	catch (...)
	{
		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"controlapp失败");
		return -1;
	}
}


//软件更新60010,60011
int CLogicRecvDisp::DispSoft(VMSG vmsg)
{
	return 0;
}