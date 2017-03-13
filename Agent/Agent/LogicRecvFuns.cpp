// LogicRecvFuns.cpp: implementation of the CLogicRecvFuns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicRecvFuns.h"

#include "BrowseDir.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicRecvFuns::CLogicRecvFuns()
{
	char szValue[_MAX_PATH];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent","AGENTPATH",szValue);
	strcpy(AgentPath,szValue);

	memset(m_UPFILE_PATH,0,sizeof(m_UPFILE_PATH));
	strcpy(m_UPFILE_PATH,AgentPath);
	strcat(m_UPFILE_PATH,UPFILE_PATH);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	strcpy(AgentNo,szValue);
	
}

CLogicRecvFuns::~CLogicRecvFuns()
{
	
}

//�ϴ��ļ�60009
int CLogicRecvFuns::UploadFile(VMSG vmsg)
{
	char sendmsg[2048];
	char pfilepath[256] = {0};
	char pfilename[256] = {0};
	int iret = DoUploadFile(pfilename, pfilepath, vmsg);
	if(0==iret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		strcpy(vmsg.PARA1, pfilepath);
		strcpy(vmsg.PARA2, pfilename);
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_SUCC, vmsg.TRNEJ, "", &vmsg);
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));			

		
// 		char szValue[512]={0};
// 		strcpy(szValue,"<CMDSTAT>3</CMDSTAT><ERR_DETAIL></ERR_DETAIL>");	
// 		strcat(szValue,"<COMD_PARA>");
// 		char tmp[256];
// 		memset(tmp,0,sizeof(tmp));
// 		char uppara[256];
// 		memset(uppara,0,sizeof(uppara));
// 		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",uppara);
// 		sprintf(tmp,"<PARA1>%s</PARA1><PARA2>%s</PARA2><PARA3>%s</PARA3>",pfilepath,pfilename,uppara);
// 		strcat(szValue,tmp);
// 		strcat(szValue,"</COMD_PARA>");
// 		memset(sendmsg,0,sizeof(sendmsg));
// 		m_packxml->PackComdRet2(sendmsg,"60009",szValue,vmsg.TRNEJ);
// 		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if (-1==iret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����δ����", &vmsg);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if (-2==iret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����FTPʧ��",  &vmsg);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if(-3==iret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�ϴ��ļ�ʧ��",  &vmsg);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if(-4==iret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�����ļ�������",  &vmsg);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	return iret;
}

//�ϴ�Ŀ¼�ṹ60014
int CLogicRecvFuns::UploadDirInfo(VMSG vmsg)
{
	char sendmsg[4096];
	char szValue[256];
	char batfile[256];
	char command[256];
	char tmpfile[256];
	char localpath[256];
	
	memset(command,0,256);
	memset(batfile,0,256);
	memset(szValue,0,256);
	memset(tmpfile,0,256);
	
	
	char remoteFile[256];
	memset(remoteFile,0,sizeof(remoteFile));
	strcpy(remoteFile, vmsg.PARA1);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_PATH_P",remoteFile);// �õ�·����
	
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue, vmsg.PARA2);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_NAME_P",szValue);
	strcat(remoteFile,"/");
	strcat(remoteFile,szValue);
	
	memset(localpath,0,sizeof(localpath));
	strcpy(localpath, vmsg.PARA3);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRPATH_C",localpath);
	
	m_trace->CreateFilePath(m_UPFILE_PATH);
	strcpy(batfile,m_UPFILE_PATH);
	strcat(batfile,"\\dirinfo.bat");
	DeleteFile(batfile);               //�ڴ���bat�ļ�ǰ��ɾ��
	strcpy(tmpfile,m_UPFILE_PATH);
	
	strcat(tmpfile,"\\dirinfo.txt");
	DeleteFile(tmpfile);
	FILE *file=fopen(batfile,"a+");
	if(file==NULL)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�����������ļ�ʧ��!: %u\n", GetLastError() );
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;
		
	}
	
	char *temp=localpath;		  
	if (_access(temp,00)==0)
	{
		strcpy(command,"dir \"");
		strcat(command,localpath);
		strcat(command,"\" > ");
		strcat(command,tmpfile);
	}
	else
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"4:��ǰĿ¼�����ļ���������");
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"��ǰĿ¼�����ļ���������");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));	
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		fclose(file);
		return 0;
	}		
	
	int len=0;
	len=fwrite(command,sizeof(char),strlen(command),file);
	fclose(file);
	if(len==0)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"д���������ļ�ʧ��!: %u\n", GetLastError() );
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;
	}
	
	
	STARTUPINFO   si;   
	memset(&si,0,sizeof(si));   
	si.dwFlags   =   STARTF_USESHOWWINDOW;   
	si.wShowWindow   =   SW_HIDE;   
	
	PROCESS_INFORMATION pinfo; 
	
	int bret=CreateProcess(batfile,NULL,NULL,NULL,1,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pinfo);
	if(!bret)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"ִ���������ļ�ʧ��!: %u\n", GetLastError() );
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		
		return 0;
	}
	::Sleep(1800);
	DWORD pid=pinfo.dwProcessId;
	HANDLE process=OpenProcess(PROCESS_ALL_ACCESS|PROCESS_QUERY_INFORMATION|SYNCHRONIZE,FALSE,pid); 
	WaitForSingleObjectEx(process,5000,TRUE);
	
	//	char szValue[_MAX_PATH];
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",szValue);
	CString ftpip;
	ftpip.Format(_T("%s"),szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PORT",szValue);
	CString ftpport;
	ftpport.Format(_T("%s"),szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",szValue);
	CString ftpusr;
	ftpusr.Format(_T("%s"),szValue);
	
	char tmpbuf2[256];
	memset(tmpbuf2,0,sizeof(tmpbuf2));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf2);
	CString ftppass;
	ftppass.Format(_T("%s"),tmpbuf2);
	
	m_ftp->SetAccessRight(ftpusr,ftppass);
	BOOL ret= m_ftp->OpenConnection(ftpip, ftpport);
	if(!ret)
	{
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�����������ʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�����������ʧ�ܣ�:%u\n",GetLastError());
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		//	m_ntlogic->m_ftp->CloseConnection();
		return 0;
		//return 
	}
	memset(szValue,0,sizeof(szValue));
	CString RemoteFile;
	RemoteFile.Format(_T("%s"),remoteFile);
	
	ret=m_ftp->PutFile(tmpfile,RemoteFile);
	m_ftp->CloseConnection();
	if(!ret)
	{
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�ϴ�Ŀ¼��ϸʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�ϴ�Ŀ¼��ϸʧ��!:%u\n",GetLastError());
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;
	}
	else
	{
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_SUCC,vmsg.TRNEJ,"");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�ϴ�Ŀ¼��ϸ�ɹ�");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;
	}			
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	return 0;
}

// int CLogicRecvFuns::DoUploadFile(char *pfilename,char *pfilepath, VMSG vmsg)
// {
// 	
// 	char szValue[_MAX_PATH];
// 	
// 	memset(szValue,0,sizeof(szValue));
// 	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",szValue);
// 	CString ftpip;
// 	ftpip.Format(_T("%s"),szValue);
// 	
// 	memset(szValue,0,sizeof(szValue));
// 	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PORT",szValue);
// 	CString ftpport;
// 	ftpport.Format(_T("%s"),szValue);
// 	
// 	memset(szValue,0,sizeof(szValue));
// 	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",szValue);
// 	CString ftpusr;
// 	ftpusr.Format(_T("%s"),szValue);
// 	
// 	char tmpbuf2[256];
// 	memset(tmpbuf2,0,sizeof(tmpbuf2));
// 	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf2);
// 	CString ftppass;
// 	ftppass.Format(_T("%s"),tmpbuf2);
// 	
// 	//�ϴ���V�˵�·��
// 	memset(szValue,0,sizeof(szValue));
// 	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",szValue);
// 	strcpy(szValue, vmsg.PARA1);
// 	tongpei(szValue,"$YYYY");
// 	tongpei(szValue,"$YY");
// 	tongpei(szValue,"$MM");
// 	tongpei(szValue,"$DD");
// 	tongpei(szValue,"$hh");
// 	tongpei(szValue,"$mm");
// 	tongpei(szValue,"$ss");
// 	tongpei(szValue,"$DEVID");
// 	strcpy(pfilepath,szValue);
// 	strcpy(vmsg.PARA1, szValue);
// 	
// 	//�ϴ���V�˵�ѹ���ļ���
// 	memset(szValue,0,sizeof(szValue));
// 	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",szValue);
// 	strcpy(szValue, vmsg.PARA2);
// 	tongpei(szValue,"$YYYY");
// 	tongpei(szValue,"$YY");
// 	tongpei(szValue,"$MM");
// 	tongpei(szValue,"$DD");
// 	tongpei(szValue,"$hh");
// 	tongpei(szValue,"$mm");
// 	tongpei(szValue,"$ss");
// 	tongpei(szValue,"$DEVID");
// 	strcpy(pfilename,szValue);
// 	strcpy(vmsg.PARA2, szValue);
// 	
// 
// 	//V��ָ����Ҫ�ϴ����ļ�
// 	char cfilepara[4096];
// 	memset(cfilepara,0,sizeof(cfilepara));
// 	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",uppara);
// 	strcpy(cfilepara, vmsg.PARA3);
// 
// 	//strcpy(cfilepara, "C:\\EbringAgent\\logs");
// 
// 	CString remotefile,localfile;
// 	CString strTmp;
// 	strTmp.Format("%s", pfilepath);
// 	if(strTmp.Right(1) == "\\" || strTmp.Right(1) == "/") 
// 	{
// 		remotefile.Format(_T("%s%s"),pfilepath,pfilename);
// 	}
// 	else
// 	{
// 		remotefile.Format(_T("%s/%s"),pfilepath,pfilename);
// 	}
// 	
// 	
// 	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "IP=[%s], PORT=[%s], Remotefile=[%s]", ftpip, ftpport, remotefile);
// 
// 	char cfile[256];
// 	strcpy(cfile,m_UPFILE_PATH);
// 	m_trace->CreateFilePath(m_UPFILE_PATH);
// 	strcat(cfile,"\\upfile.zip");
// 	localfile.Format(_T("%s"),cfile);
// 	
// 	try
// 	{
// 		DeleteFile(_T(cfile));
// 		
// 		char t[256];
// 		memset(t,0,sizeof(t));
// 		int i=0;
// 		CBrowseDir DirTest;
// 		
// 		char cfilepath1[256]={0};
// 		char *cfilepath=cfilepath1;
// 		
// 		char model1[256]={0};
// 		char *model=model1;
// 		char tmppath[256];
// 		int m_LostFileNum=0;
// 		CString strPath;
// 		bool created=false;
// 		//		char *cfilename;
// 		//	bool bret=m_myzip->zippara(cfile,cfilepara);
// 		char * ptmp = t;
// 		strcpy(ptmp,cfilepara);
// 		cfilepath=strtok(cfilepara,"~");
// 		
// 		while(1)
// 		{
// 			
// 			if(cfilepath == NULL)
// 				break;
// 			ptmp = ptmp + strlen(cfilepath)+1;			
// 			//	m_trace->WTrace(LT_INFO,"cfilepath=%s",cfilepath);
// 			model=strrchr(cfilepath,'\\');
// 			if(model == NULL)
// 			{
// 				cfilepath=strtok(NULL,"~");
// 				continue;
// 			}
// 			model++;
// 			strncpy(tmppath,cfilepath,model-cfilepath);
// 			tmppath[model-cfilepath]='\0';
// 			strPath=tmppath;
// 			strPath.TrimRight(_T('\\'));
// 			cfilepath=(LPSTR)(LPCTSTR)strPath;
// 			
// 			char basepath1[256]={0};
// 			char *basepath=basepath1;
// 			basepath=strrchr(cfilepath,'\\');
// 			
// 			if(basepath != NULL)
// 			{
// 				basepath++;
// 			}
// 			
// 			if(::GetFileAttributes(cfilepath) & FILE_ATTRIBUTE_DIRECTORY)
// 			{
// 				
// 				if(!DirTest.SetInitDir(cfilepath))
// 				{
// 					(char *)cfilepath=strtok(NULL,"~");
// 					continue;
// 				}
// 				DirTest.BeginBrowseA(model);//ƥ���ļ����ļ���  
// 				int filenum= DirTest.fileArray.GetSize();
// 				int foldernum= DirTest.folderArray.GetSize();
// 				if (filenum!=0)// �ж��ϴ��ļ�����Ϊ��
// 				{
// 					if(!created)
// 					{
// 						m_zip(cfilepath,basepath,cfile,true,4);
// 						created=true;
// 					}
// 					else
// 						m_zip(cfilepath,basepath,cfile,false,4);
// 					CString str;
// 					int i=0;				
// 					char *f=t;
// 					for(i=0;i<foldernum;i++)
// 					{
// 						str=DirTest.folderArray.GetAt(i);
// 						str.TrimRight(_T('\\'));
// 						f=(LPSTR)(LPCTSTR)str;
// 						//	m_zip(f,basepath,cfile,false,4);
// 						
// 					}					
// 					for(i=0;i<filenum;i++)
// 					{
// 						str=DirTest.fileArray.GetAt(i);
// 						str.TrimRight(_T('\\'));
// 						f=(LPSTR)(LPCTSTR)str;						
// 						m_zip(f,basepath,cfile,false,4);
// 					}
// 				}
// 				//���ļ�������
// 				else
// 				{
// 					m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "cannot find %s",model);
// 					m_LostFileNum++;
// 					
// 					cfilepath=strtok(ptmp,"~");
// 					continue;					
// 				}
// 				
// 			}
// 			if(strlen(ptmp)== 0)
// 				break;
// 			cfilepath=strtok(ptmp,"~");
// 		}
// 		
// 		if (m_LostFileNum!=0)
// 		{
// 			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "��%d���ļ�������", m_LostFileNum);
// 		}
// 		//�����ļ�ʧ�ܣ������ļ�������
// 		if (created==false)
// 		{
// 			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "�ϴ��ļ�ʧ�ܣ������ļ�������");
// 			throw(-4);		
// 		}
// 		
// 		m_ftp->SetAccessRight(ftpusr,ftppass);			
// 		
// 		if(!m_ftp->OpenConnection(ftpip, ftpport))
// 		{
// 			//m_ftp->CloseConnection();
// 			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "OpenConnection Error");
// 			throw (-2);
// 		}
// 		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFile,RemoteFile=[%s],LocalFile=[%s]",remotefile,localfile);	
// 		if(!m_ftp->PutFile(localfile,remotefile))
// 		{
// 			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PutFile Error");
// 			m_ftp->CloseConnection();
// 			throw (-3);
// 		}
// 		m_ftp->CloseConnection();
// 		
// 		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ZIP END");
// 	}
// 	catch(int iret)
// 	{
// 		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "UpFILE ERROR catch2[%d]",iret);
// 		//m_ftp->CloseConnection();
// 		return iret;
// 	}
// 	catch (...)
// 	{
// 		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "UpFILE ERROR catch");
// 		//m_ftp->CloseConnection();
// 		return -1;
// 	}
// 	m_trace->WTrace(LOG_GRP, LOG_CTRL ,LT_INFO, "UpFILE SUCCESS ");
// 	//m_ftp->CloseConnection();
// 	return 0;
// }

//�ϴ�ϵͳ����״̬60019
int CLogicRecvFuns::UploadSysState(VMSG vmsg)
{
	char sendmsg[4096];
	char szValue[1024];	
	memset(szValue,0,sizeof(szValue));
	BOOL bret=m_sys->GetallInfo2();
	if (!bret)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"��ȡϵͳ����״̬ʧ��");
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"��ȡϵͳ����״̬ʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;
		
	}

	CString aa(m_sys->SysStatus_all);
	strcpy(szValue,"<CMDSTAT>3</CMDSTAT><ERR_DETAIL></ERR_DETAIL>");	
	strcat(szValue,"<COMD_PARA>");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"SysStatus_all=[%s]",aa);
	strcat(szValue,aa.GetBuffer(aa.GetLength()));
	strcat(szValue,"</COMD_PARA>");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"��ȡϵͳ����״̬�ɹ���[%s]",szValue);
	m_packxml->PackComdRet2(sendmsg,"60019",szValue,vmsg.TRNEJ);
	m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	
	return 0;
}

int CLogicRecvFuns::DoUploadFile(char *pfilename,char *pfilepath, VMSG vmsg)
{

	char szValue[_MAX_PATH];
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",szValue);
	CString ftpip;
	ftpip.Format(_T("%s"),szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PORT",szValue);
	CString ftpport;
	ftpport.Format(_T("%s"),szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",szValue);
	CString ftpusr;
	ftpusr.Format(_T("%s"),szValue);
	
	char tmpbuf2[256];
	memset(tmpbuf2,0,sizeof(tmpbuf2));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf2);
	CString ftppass;
	ftppass.Format(_T("%s"),tmpbuf2);
	
	//�ϴ���V�˵�·��
	memset(szValue,0,sizeof(szValue));
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",szValue);
	strcpy(szValue, vmsg.PARA1);
	tongpei(szValue,"$YYYY");
	tongpei(szValue,"$YY");
	tongpei(szValue,"$MM");
	tongpei(szValue,"$DD");
	tongpei(szValue,"$hh");
	tongpei(szValue,"$mm");
	tongpei(szValue,"$ss");
	tongpei(szValue,"$DEVID");
	strcpy(pfilepath,szValue);
	
	//�ϴ���V�˵�ѹ���ļ���
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue, vmsg.PARA2);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",szValue);
	tongpei(szValue,"$YYYY");
	tongpei(szValue,"$YY");
	tongpei(szValue,"$MM");
	tongpei(szValue,"$DD");
	tongpei(szValue,"$hh");
	tongpei(szValue,"$mm");
	tongpei(szValue,"$ss");
	tongpei(szValue,"$DEVID");
	strcpy(pfilename,szValue);
	

	//V��ָ����Ҫ�ϴ����ļ�
	char cfilepara[4096];
	memset(cfilepara,0,sizeof(cfilepara));
	strcpy(cfilepara, vmsg.PARA3);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",uppara);
	
	CString remotefile,localfile;
	CString strTmp;
	strTmp.Format("%s", pfilepath);
	if(strTmp.Right(1) == "\\" || strTmp.Right(1) == "/") 
	{
		remotefile.Format(_T("%s%s"),pfilepath,pfilename);
	}
	else
	{
		remotefile.Format(_T("%s/%s"),pfilepath,pfilename);
	}
	
	
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "IP=[%s], PORT=[%s], Remotefile=[%s]", ftpip, ftpport, remotefile);

	char cfile[256];
	strcpy(cfile,m_UPFILE_PATH);
	m_trace->CreateFilePath(m_UPFILE_PATH);
	strcat(cfile,"\\upfile.zip");
	localfile.Format(_T("%s"),cfile);
	
	bool created = false;

	try
	{
		DeleteFile(_T(cfile));
		
		CBrowseDir DirTest;
		
		char cfilepath1[256]={0};
		char *cfilepath = cfilepath1;
		cfilepath=strtok(cfilepara,"~");
		
		char *ptmp = cfilepara;

		while(1)
		{
			
			if(cfilepath == NULL)
				break;
			
			ptmp = ptmp + strlen(cfilepath)+1;

			 
			if( DirTest.IsExistDir(cfilepath) ) //�����Ŀ¼
			{
				CStringArray array;
				DirTest.GetAllDirFile(cfilepath, array);
				for(int i=0; i<array.GetSize(); i++)
				{
					CString strFile;
					strFile.Format("%s", cfilepath);
					CString strTmp = strFile;
					if( (strFile.Right(1) != "\\") && (strFile.Right(1) != "/") )
					{
						strTmp.Format("%s\\", strFile);
					}
					strFile.Format("%s%s", strTmp, array.GetAt(i));

					m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "upload file[%d]=%s", i, strFile);
					
					if( DirTest.IsExistFile(strFile) ) //������ļ�
					{
						strFile.MakeUpper();

						if( strFile.Find("NTTRACE", 0) > 0 ) 
						{
							char szTemp[300] = {0};
							sprintf(szTemp, "%s\\%s", m_UPFILE_PATH, GetFileName(strFile) );
							CopyFile(strFile, szTemp, 0);
							if(!created)
							{
								m_zip(szTemp, "", cfile, true, 4);
								created=true;
							}
							else
								m_zip(szTemp, "", cfile, false, 4);
							DeleteFile(szTemp);
						}
						else
						{
							if(!created)
							{
								m_zip(strFile, "", cfile, true, 4);
								created=true;
							}
							else
								m_zip(strFile, "", cfile, false, 4);
						}
						
					}
				}
				
			}
			else if( DirTest.IsExistFile(cfilepath) ) //������ļ�
			{
// 				char szTemp[600]={0};
// 				sprintf(szTemp, "%s\\%s", m_UPFILE_PATH, cfilepath);
// 				CopyFile(cfilepath,szTemp,0);
// 				DeleteFile(szTemp);

				m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "upload file=%s", cfilepath);

				if( strstr(strupr(cfilepath), "NTTRACE") ) 
				{
					char szTemp[300] = {0};
					sprintf(szTemp, "%s\\%s", m_UPFILE_PATH, GetFileName(cfilepath) );
					CopyFile(cfilepath, szTemp, 0);
					if(!created)
					{
						m_zip(szTemp, "", cfile, true, 4);
						created=true;
					}
					else
						m_zip(szTemp, "", cfile, false, 4);
					DeleteFile(szTemp);
				}
				else
				{
					if(!created)
					{
						m_zip(cfilepath, "", cfile, true, 4);
						created=true;
					}
					else
					m_zip(cfilepath, "", cfile, false, 4);
				}
	
			}
			else
			{
				m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "file no exist filename=[%s]", cfilepath);
			}

			cfilepath=strtok(ptmp,"~");

		}
		

		

		//�����ļ�ʧ�ܣ������ļ�������
		if (created==false)
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "�ϴ��ļ�ʧ�ܣ������ļ�������");
			throw(-4);		
		}
		
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "zip file gen successful, begin upload to ftp server");

		m_ftp->SetAccessRight(ftpusr,ftppass);			
		
		if(!m_ftp->OpenConnection(ftpip, ftpport))
		{
			//m_ftp->CloseConnection();
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "OpenConnection Error");
			throw (-2);
		}
		//m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFile,RemoteFile=[%s],LocalFile=[%s]",remotefile,localfile);	
		if(!m_ftp->PutFile(localfile,remotefile))
		{
			m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "PutFile Error");
			m_ftp->CloseConnection();
			throw (-3);
		}
		m_ftp->CloseConnection();
		
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "zip file upload to ftp server successful");

		//m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_INFO, "ZIP END");
	}
	catch(int iret)
	{
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "UpFILE ERROR catch2[%d]",iret);
		//m_ftp->CloseConnection();
		return iret;
	}
	catch (...)
	{
		m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_ERROR, "UpFILE ERROR catch");
		//m_ftp->CloseConnection();
		return -1;
	}
	m_trace->WTrace(LOG_GRP, LOG_CTRL ,LT_INFO, "UpFILE SUCCESS ");
	//m_ftp->CloseConnection();
	return 0;
}

//�ϴ���װӲ����Ϣ60023
int CLogicRecvFuns::UploadHardwareInfo(VMSG vmsg)
{
	char sendmsg[4096];
	char filepath[256];
	memset(filepath,0,sizeof(filepath));
	GetNTReg("SOFTWARE\\Ebring\\Agent","AGENTPATH",filepath);
	strcat(filepath,"\\Files");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"FilePath=[%s]",filepath);
	BOOL ret=m_resours->GetHardWareInfo(filepath);
	if (!ret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"��ȡ�豸Ӳ����Ϣʧ��");
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"��ȡ�豸Ӳ����Ϣʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;			
	}
	char filename[MAX_PATH];
	memset(filename,0,sizeof(filename));
	sprintf(filename,"%s\\HardWareInfo_%s.txt",filepath,AgentNo);
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",filename);
	
	char pfilepath[256];
	char pfilename[256];
	memset(pfilename,0,sizeof(pfilename));
	memset(pfilepath,0,sizeof(pfilepath));
	int	iret=UpFILE(pfilename,pfilepath, vmsg, filename);
	if(iret == 0)
	{
		char szValue[512]={0};
		strcpy(szValue,"<CMDSTAT>3</CMDSTAT><ERR_DETAIL></ERR_DETAIL>");	
		strcat(szValue,"<COMD_PARA>");
		char tmp[256];
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"<PARA1>%s</PARA1><PARA2>%s</PARA2>",pfilepath,pfilename);
		strcat(szValue,tmp);
		strcat(szValue,"</COMD_PARA>");
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet2(sendmsg,"60023",szValue,vmsg.TRNEJ);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-1) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����δ����");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"����δ����");
		//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-2) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����FTPʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"����FTPʧ��");
		//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-3) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�ϴ��ļ�ʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�ϴ��ļ�ʧ��");
		//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-4) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�����ļ�������");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�����ļ�������");
		//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	
	//SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");

	return 0;
}

//�ϴ���װ�����Ϣ60024
int CLogicRecvFuns::UploadSoftwareInfo(VMSG vmsg)
{
	char sendmsg[4096];
	char filepath[256];
	memset(filepath,0,sizeof(filepath));
	GetNTReg("SOFTWARE\\Ebring\\Agent","AGENTPATH",filepath);
	strcat(filepath,"\\Files");
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"FilePath=[%s]",filepath);
	BOOL ret=m_resours->GetSoftWareInfo(filepath);
	if (!ret)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"��ȡ�豸�����Ϣʧ��");
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"��ȡ�豸�����Ϣʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
		return 0;			
	}
	char filename[MAX_PATH];
	memset(filename,0,sizeof(filename));
	sprintf(filename,"%s\\SoftwareInfo_%s.txt",filepath,AgentNo);
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",filename);
	
	char pfilepath[256];
	char pfilename[256];
	memset(pfilename,0,sizeof(pfilename));
	memset(pfilepath,0,sizeof(pfilepath));
	int	iret=UpFILE(pfilename,pfilepath, vmsg, filename);
	if(iret == 0)
	{
		char szValue[512]={0};
		strcpy(szValue,"<CMDSTAT>3</CMDSTAT><ERR_DETAIL></ERR_DETAIL>");	
		strcat(szValue,"<COMD_PARA>");
		char tmp[256];
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"<PARA1>%s</PARA1><PARA2>%s</PARA2>",pfilepath,pfilename);
		strcat(szValue,tmp);
		strcat(szValue,"</COMD_PARA>");
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet2(sendmsg,"60024",szValue,vmsg.TRNEJ);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-1) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����δ����");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"����δ����");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-2) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"����FTPʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"����FTPʧ��");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-3) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�ϴ��ļ�ʧ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�ϴ��ļ�ʧ��");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	else if(iret==-4) 
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"�����ļ�������");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"�����ļ�������");
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	}
	
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");

	return 0;
}

//����ϵͳ����
int CLogicRecvFuns::StopProcess(VMSG vmsg)
{
	char sendmsg[1024] = {0};
	char ProcessName[_MAX_PATH];
	char szValue[_MAX_PATH];
	memset(szValue,0,sizeof(szValue));
	memset(ProcessName,0,sizeof(ProcessName));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","KILL_PROCESS",szValue);
	strcpy(ProcessName,szValue);
	char err[_MAX_PATH];
	memset(err,0,sizeof(err));
	int ret=ProcessKillEx(ProcessName,err);
	if (ret==0)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_SUCC,vmsg.TRNEJ, "");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));		
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
	} 
	else if (ret==-1)
	{
		char tmp[256];
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"�رղ��ֽ���ʧ��,ʧ�ܽ���Ϊ[%s]",err);
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,tmp);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if(ret==-2)
	{
		char tmp[256];
		memset(tmp,0,sizeof(tmp));
		sprintf(tmp,"�رս���ʧ��,ʧ�ܽ���Ϊ[%s]",err);
		
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,tmp);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	else if(ret==-3)
	{
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"4:������Ϊ��");
		memset(sendmsg,0,sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE,RET_FAIL,vmsg.TRNEJ,"������Ϊ��");
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
	}
	return ret;
}

int CLogicRecvFuns::UpFILE(char *pfilename,char *pfilepath, VMSG vmsg, CString localfile)
{
	char szValue[_MAX_PATH];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",szValue);
	CString ftpip;
	ftpip.Format(_T("%s"),szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PORT",szValue);
	CString ftpport;
	ftpport.Format(_T("%s"),szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",szValue);
	CString ftpusr;
	ftpusr.Format(_T("%s"),szValue);

	char tmpbuf2[128];
	memset(tmpbuf2,0,sizeof(tmpbuf2));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf2);
	CString ftppass;
	ftppass.Format(_T("%s"),tmpbuf2);

	memset(szValue,0,sizeof(szValue));
	strcpy(szValue, vmsg.PARA1);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",szValue);
	//char pfilepath[256];
	tongpei(szValue,"$YYYY");
	tongpei(szValue,"$YY");
	tongpei(szValue,"$MM");
	tongpei(szValue,"$DD");
	tongpei(szValue,"$hh");
	tongpei(szValue,"$mm");
	tongpei(szValue,"$ss");
	tongpei(szValue,"$DEVID");

	strcpy(pfilepath,szValue);
	
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue, vmsg.PARA2);
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",szValue);

	tongpei(szValue,"$YYYY");
	tongpei(szValue,"$YY");
	tongpei(szValue,"$MM");
	tongpei(szValue,"$DD");
	tongpei(szValue,"$hh");
	tongpei(szValue,"$mm");
	tongpei(szValue,"$ss");
	tongpei(szValue,"$DEVID");

//	char pfilename[256];
	strcpy(pfilename,szValue);

	
	CString remotefile;
	if(strchr(pfilepath,'\\') == NULL)
		remotefile.Format(_T("%s/%s"),pfilepath,pfilename);
	else
		remotefile.Format(_T("%s\\%s"),pfilepath,pfilename);

	try
	{
		m_ftp->SetAccessRight(ftpusr,ftppass);			
		
		if(!m_ftp->OpenConnection(ftpip, ftpport))
		{
			//m_ftp->CloseConnection();
			throw (-2);
		}
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFile,RemoteFile=[%s],LocalFile=[%s]",remotefile,localfile);	
		if(!m_ftp->PutFile(localfile,remotefile))
		{
			m_ftp->CloseConnection();
			throw (-3);
		}
		m_ftp->CloseConnection();

		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"upfile end ");
	}
	catch (CMemoryException* e)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFILE ERROR ,a CMemoryException catch");
		m_ftp->CloseConnection();
		return -1;
	}
	catch (CFileException* e)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFILE ERROR ,a CFileException catch");
		m_ftp->CloseConnection();
		return -1;
	}
	catch (CException* e)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFILE ERROR ,a CException catch");
		m_ftp->CloseConnection();
		return -1;
	}
	catch(int iret)
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFILE ERROR catch1[%d]",iret);
		//m_ftp->CloseConnection();
		return iret;
	}
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"UpFILE SUCCESS ");
	//m_ftp->CloseConnection();
	return 0;	
}



void CLogicRecvFuns::tongpei(char * msgbuf,char *tongpeifu)
{
	char *tmpbuf;
	char tmpmsg[2048];
	char nowtime[64];
	GetNowTime(nowtime);
	int tplen=0,i=0,j=0;
	
	while(1) 
	{	
		tmpbuf=strstr(msgbuf,tongpeifu);
		if(tmpbuf == NULL)
			break;
		memcpy(tmpmsg,msgbuf,tmpbuf - msgbuf);
		if(!strcmp(tongpeifu,"$YYYY"))
		{
			tplen=4;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime,tplen);
		}
		else if(!strcmp(tongpeifu,"$YY"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+2,tplen);
		}
		else if(!strcmp(tongpeifu,"$MM"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+4,tplen);
		}
		else if(!strcmp(tongpeifu,"$DD"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+6,tplen);
		}
		else if(!strcmp(tongpeifu,"$hh"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+8,tplen);
		}
		else if(!strcmp(tongpeifu,"$mm"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+10,tplen);
		}
		else if(!strcmp(tongpeifu,"$ss"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+12,tplen);
		}
		else if(!strcmp(tongpeifu,"$DEVID"))
		{
			char szValue[_MAX_PATH];
			if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue))
				return;
			
			tplen=strlen(szValue);
			memcpy(tmpmsg+(tmpbuf - msgbuf),szValue,tplen);
		}
		memcpy(tmpmsg + (tmpbuf - msgbuf) + tplen,tmpbuf+strlen(tongpeifu),strlen(tmpbuf)-strlen(tongpeifu));
		tmpmsg[strlen(msgbuf)-strlen(tongpeifu)+tplen]='\0';
		strcpy(msgbuf,tmpmsg);
	}
}


int CLogicRecvFuns::ProcessKillEx(char *ProcessName,char * errmsg)
{
	char szValue[_MAX_PATH];
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue,ProcessName);
	
	char err[_MAX_PATH];
	memset(err,0,sizeof(err));
	int count=0;
	int errNo=0;
	if(strlen(szValue)!=0)
	{
		char propara[256];
		memset(propara,0,sizeof(propara));
		strcpy(propara,szValue);
		
		char t[256];
		memset(t,0,sizeof(t));
		char *process=t;
		process=strtok(propara,"~");
		char *tmpbuf=t;
		strcpy(tmpbuf,propara);
		while (1)
		{
			if(process == NULL )
				break;
			tmpbuf = tmpbuf + strlen(process)+1;
			count++;
			int bret=MyKillProcess(process);
			if (bret!=0)
			{
				m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"ֹͣ����[%s]ʧ�ܣ�",process);
				strcat(err,process);
				strcat(err," ");
				errNo++;
			}
			
			::Sleep(500);
			if (strlen(tmpbuf)==0)
			{
				break;
			}
			process = strtok(tmpbuf,"~");
		}	
		if (count==errNo && count!=0)
		{
			strcpy(errmsg,err);
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"ֹͣ���н���[%s]ʧ��",errmsg);
			return -2;
		}
		else if (errNo!=0&&count>errNo)
		{
			strcpy(errmsg,err);
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"ֹͣ���ֽ���[%s]ʧ��",errmsg);
			return -1;
		}
		else if (errNo==0 && count!=0)
		{
			m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"ֹͣ���н��̳ɹ�");
			return 0;
		}
		else
			return -1;
		
	}
	else
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"������Ϊ��!");
		return -3;
	}
}

//��·�����ļ����У�ȡ���ļ�������
CString CLogicRecvFuns::GetFileName(CString filePath)
{
	int pos = filePath.ReverseFind('\\');
	return filePath.Right(filePath.GetLength() - pos - 1);
}