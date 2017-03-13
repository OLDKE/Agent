#include "stdafx.h"
#include "NTTRACE.H"
#include "LogicCommon.h"

CNTTRACE::CNTTRACE()
//:fout()			
{
	ntlog.count=0;
	InitializeCriticalSection(&m_critical);  
}

CNTTRACE::~CNTTRACE()
{
	if(fout.is_open())
	{
		fout.close();
	}
	DeleteCriticalSection(&m_critical);  
}
int CNTTRACE::FindPathIndex(char *pGrp)
{
	char tmp[36];
	int i=-1;
	memset(tmp,0,sizeof(tmp));
	strcpy(tmp,pGrp);
	CString strGrp=tmp;
	for(i=0;i<ntlog.count;i++)
	{
		if(ntlog.logModule[i] == strGrp)
			break;
	}
	if((i==ntlog.count)||(ntlog.count==0))
		return -1;
	else
		return i;
}
void CNTTRACE::ClearLog(char *pGrp)
{
	int index=FindPathIndex(pGrp);
	if(index == -1)
		return;
	CFileFind tempFind;
	char sTempFileFind[256];
	CString tmpDir;
	tmpDir=ntlog.logPath[index];
	tmpDir.TrimRight("\\");	

	sprintf(sTempFileFind,"%s\\*.*",tmpDir);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{
		IsFinded = tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			char sFoundFileName[256];
			strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(256));
			if (!tempFind.IsDirectory())
			{
				char sTempFileName[256];
				char szTemp[300]; 
				char year[6];
				char month[6];
				char day[6];
				char hour[6];
				char minute[6];
				char seconds[6];
				struct tm * p;
				time_t Currenttime;
				struct tm  p1;
				time_t thentime;
				char szNewFile[300];



				time(&Currenttime);
				p = localtime(&Currenttime);
				memset(szNewFile, 0, sizeof(szNewFile));
				sprintf(szNewFile, "%04d%02d%02d%02d%02d%02d-%s",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,sFoundFileName);
				memset(szTemp, 0, sizeof(szTemp));
				sprintf(szTemp,"%s\\%s",tmpDir,szNewFile);
				memset(sTempFileName,0,sizeof(sTempFileName));
				sprintf(sTempFileName,"%s\\%s",tmpDir,sFoundFileName);
				SetFileAttributes(sTempFileName,FILE_ATTRIBUTE_NORMAL);
				if(strlen(sFoundFileName)<14)
				{
					CopyFile(sTempFileName,szTemp,0);
					DeleteFile(sTempFileName);
					continue;
				}
				memset(year,0,sizeof(year));
				memset(month,0,sizeof(month));
				memset(day,0,sizeof(day));
				memset(hour,0,sizeof(hour));
				memset(minute,0,sizeof(minute));
				memset(seconds,0,sizeof(seconds));

				strncpy(year,sFoundFileName,4);
				strncpy(month,sFoundFileName+4,2);
				strncpy(day,sFoundFileName+6,2);
				strncpy(hour,sFoundFileName+8,2);
				strncpy(minute,sFoundFileName+10,2);
				strncpy(seconds,sFoundFileName+12,2);
				int iyear=atoi(year);
				int imonth=atoi(month);
				int iday=atoi(day);
				int ihour=atoi(hour);
				int iminute=atoi(minute);
				int iseconds=atoi(seconds);
				if((iyear<1900)||(imonth<1)||(imonth>12)||(iday<1)||(iday>31)||(ihour<0)||(ihour>23)
					||(iminute<1)||(iminute>59)||(iseconds<1)||(iseconds>59))
				{
					CopyFile(sTempFileName,szTemp,0);
					DeleteFile(sTempFileName);
					continue;
				}
				p1.tm_year=iyear-1900;
				p1.tm_mon=imonth-1;
				p1.tm_mday=iday;
				p1.tm_hour=ihour;
				p1.tm_min=iminute;
				p1.tm_sec=iseconds;
				thentime=mktime(&p1);
				int daydiff=(Currenttime-thentime)/(24*60*60);
				if(daydiff<ntlog.logDay[index])
				{
					continue;
				}
				DeleteFile(sTempFileName);

			}
		}
	}
	tempFind.Close();

}


/*
trace级别的定义如下
INFO --- 1
DEBUG ---- 2
ERROR ----- 4
*/

void CNTTRACE::WTrace(char *pGrp,char *pFileName,int iLevel, char *fmt, ... )
{	
	  
	int index=FindPathIndex(pGrp);
	if(index == -1)
		return ;
	/*if((ntlog.logLevel[index] & iLevel) == 0) // 判断是否记录该记录项
	{
		return ; //不记录直接返回成功
	}
	*/
	if(ntlog.logLevel[index]==LT_INFO) // 日志级别为 INFO ,打印INFO和ERROR的日志
	{
		if(iLevel!=LT_INFO&&iLevel!=LT_ERROR)
			return ; //不记录直接返回成功
	}
	else if(ntlog.logLevel[index]==LT_DEBUG) // 日志级别为 DEBUG ,打印所有日志
	{
		;
	}
	else if(ntlog.logLevel[index]==LT_ERROR) // 日志级别为 ERROR ,打印ERROR日志
	{
		if(iLevel!=LT_ERROR)
		return;
	}
	
	if((pFileName == NULL)||(!strcmp(pFileName,"")))
		return;

	char szTemp[1024];
	memset(szTemp, 0, sizeof(szTemp));
	strcat(szTemp, ntlog.logPath[index]);
	strcat(szTemp,"\\");
	strcat(szTemp, pFileName);

//	fout.open(szTemp, ios::out | ios::ate , filebuf::operator | filebuf::sh_write);
//	fout.open(szTemp,ios::app);
//	fout.close();
//	fout.open(szTemp,ios::in|ios::out|ios::ate);
	EnterCriticalSection(&m_critical); 
	if(_access(szTemp,0) == -1)
	{
		fout.open(szTemp,ios::ate|ios::out);
	}
	else
		fout.open(szTemp,ios::ate|ios::in);
	
	if(fout.rdstate() != ios::goodbit)
	{
		LeaveCriticalSection(&m_critical);
		return;
	}
	
	//dFixedFileSize小于等于0时认为是没有文件大小限制的
	streampos current_pos;
	//current_pos = fout.tellp();
	current_pos = ntlog.logPos[index];
	if((current_pos > 0) && ((DWORD)current_pos >= ntlog.logSize[index])) 
	{		
		current_pos = 0;
	}
	
	if(current_pos > 0)
	{
		fout.seekp(current_pos, ios::beg);
	}
	else
	{
		fout.seekp(0, ios::beg);
	}


	char long_type[50];
	memset(long_type,0,sizeof(long_type));
	if(iLevel==LT_INFO)
	{
		strcpy(long_type," INFO  ");
	}
	else if(iLevel==LT_DEBUG)
	{
		strcpy(long_type," DEBUG ");
	}
	else if(iLevel==LT_ERROR)
	{
		strcpy(long_type," ERROR ");
	}

	char szTime[50];
	memset(szTime, 0, sizeof(szTime));

	GetLocalTimeForStr(szTime);
	szTime[strlen(szTime)]='\0';
	strcat(szTime,long_type);

	fout.write(szTime, (int)strlen(szTime));

	char logbuf[9000];

	va_list ap;
	va_start ( ap, fmt );
	vsprintf(logbuf,fmt,ap);
	va_end ( ap );

	
	fout.write(logbuf, (int)strlen(logbuf));
	fout.write("\n", 1);
	fout.flush();
	current_pos = fout.tellp(); //current_pos保留的只要写入的位置（相对于文件开始位置）
	ntlog.logPos[index]=current_pos;
	fout.close();
	LeaveCriticalSection(&m_critical);
	
	
}

/**
* 打印日志
* iLevel:日志级别
* fmt:日志内容
*/
void CNTTRACE::WTrace(int iLevel, char *fmt, ... )
{
    va_list ptList;
    char    achBuf[6000] = {0};
    int     nBufLen;

    va_start(ptList, fmt);
    nBufLen = vsprintf(achBuf, fmt, ptList);
    va_end(ptList);

	WTrace(LOG_GRP,LOG_STEP,iLevel, achBuf);
}
/**
* 打印日志 info 日志
* iLevel:日志级别
* fmt:日志内容
*/
void CNTTRACE::info(char *fmt, ... )
{
	va_list ptList;
    char    achBuf[6000] = {0};

    int     nBufLen;

    va_start(ptList, fmt);
    nBufLen = vsprintf(achBuf, fmt, ptList);
    va_end(ptList);
	//WTrace(LOG_GRP,LOG_STEP,LT_INFO, fmt);
	//WTrace(LOG_GRP,LOG_STEP,LT_INFO, achBuf);
	WTrace(LT_INFO, achBuf );
}
/**
* 打印日志 DEBUG 日志
* fmt:日志内容
*/
void CNTTRACE::debug(char *fmt, ... )
{
	va_list ptList;
    char    achBuf[6000] = {0};
    int     nBufLen;

    va_start(ptList, fmt);
    nBufLen = vsprintf(achBuf, fmt, ptList);
    va_end(ptList);
	WTrace(LT_DEBUG, achBuf );
}

/**
* 打印日志 error 日志
* fmt:日志内容
*/
void CNTTRACE::error(char *fmt, ... )
{
	va_list ptList;
    char    achBuf[6000] = {0};
    int     nBufLen;

    va_start(ptList, fmt);
    nBufLen = vsprintf(achBuf, fmt, ptList);
    va_end(ptList);
	WTrace(LT_ERROR, achBuf );
}


void CNTTRACE::WTraceNoTime(char *pGrp,char *pFileName,int iLevel, char *fmt, ... )
{
	
	int index=FindPathIndex(pGrp);
	if(index == -1)
		return ;
	if((ntlog.logLevel[index] & iLevel) == 0) // 判断是否记录该记录项
	{
		return ; //不记录直接返回成功
	}
	if((pFileName == NULL)||(!strcmp(pFileName,"")))
		return;

	char szTemp[300];
	memset(szTemp, 0, sizeof(szTemp));
	strcat(szTemp, ntlog.logPath[index]);
	strcat(szTemp,"\\");
	strcat(szTemp, pFileName);

//	fout.open(szTemp, ios::out | ios::ate , filebuf::operator | filebuf::sh_write);
//	fout.open(szTemp,ios::app);
//	fout.close();
//	fout.open(szTemp,ios::in|ios::out|ios::ate);

	EnterCriticalSection(&m_critical);   
	if(_access(szTemp,0) == -1)
	{
		fout.open(szTemp,ios::ate|ios::out);
	}
	else
		fout.open(szTemp,ios::ate|ios::in);
	
	if(fout.rdstate() != ios::goodbit)
	{
		//fout.clear(ios::goodbit);
		LeaveCriticalSection(&m_critical);
		return;
	}
	
	//dFixedFileSize小于等于0时认为是没有文件大小限制的
	streampos current_pos;
	//current_pos = fout.tellp();
	current_pos = ntlog.logPos[index];
	if((current_pos > 0) && ((DWORD)current_pos >= ntlog.logSize[index])) 
	{		
		current_pos = 0;
	}
	
	if(current_pos > 0)
	{
		fout.seekp(current_pos, ios::beg);
	}
	else
	{
		fout.seekp(0, ios::beg);
	}



	fout.write("\t\t",2);
	char logbuf[8196];
	va_list ap;
	va_start ( ap, fmt );
	vsprintf(logbuf,fmt,ap);
	va_end ( ap );
	
	fout.write(logbuf, (int)strlen(logbuf));
	fout.write("\n", 1);
	fout.flush();
	current_pos = fout.tellp(); //current_pos保留的只要写入的位置（相对于文件开始位置）
	ntlog.logPos[index]=current_pos;
	fout.close();
	LeaveCriticalSection(&m_critical);

	
	
}


int CNTTRACE::CreateFilePath(char * pFilePath)
{
	if((pFilePath == NULL) || (*pFilePath == 0x00))
	{
		return 0;
	}

	char szDirName[300];
	memset(szDirName, 0, sizeof(szDirName));
	char szTemp[300]; //有待于创建的路径名（文件夹）
	memset(szTemp, 0, sizeof(szTemp));
	strcpy(szTemp, pFilePath);
	char * token;
	int iret = 0;

	token = strtok(szTemp,"\\");
	strcpy(szDirName,token);

	while(token != NULL)
	{
		token= strtok(NULL, "\\");
		if(token == NULL)
		{
			break;
		}
		strcat(szDirName, "\\");
		strcat(szDirName, token);
		iret = _mkdir(szDirName);
		if(iret == ENOENT)
		{
			return -5;
		}
		else if (iret == EEXIST)
		{
			continue;
		}
	}

	return 0;
}

int CNTTRACE::DeleteFile(char * pFileName)
{
	if((pFileName == NULL) || (*pFileName == 0x00))
	{
		return 0;
	}

	int iRet = 0;
	iRet = _unlink(pFileName);

	return iRet;
}

void CNTTRACE::GetLocalTimeForStr(char * pTime)
{
	time_t Currenttime;
	struct tm * p;

	time(&Currenttime);
	p = localtime(&Currenttime);

	char szTmp[300];
	memset(szTmp, 0, sizeof(szTmp));
	sprintf(szTmp, "[%d/%d/%d %02d:%02d:%02d]",
		(1900+p->tm_year),(1+p->tm_mon),p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

	strcpy(pTime, szTmp);
	return ;
}
int CNTTRACE::GetTraceCFG(char* CfgName,char* GrpName)
{
	int ret;
	char tmpbuf1[256];
	unsigned long tmpint;
	memset(tmpbuf1,0,sizeof(tmpbuf1));
	strcpy(tmpbuf1,CfgName);
	ntlog.logCfgName=tmpbuf1;
	

	memset(tmpbuf1,0,sizeof(tmpbuf1));
	strcpy(tmpbuf1,GrpName);
	ntlog.logModule[ntlog.count]=tmpbuf1;


	memset(tmpbuf1,0,sizeof(tmpbuf1));
	ret=GetPrivateProfileStringA(ntlog.logModule[ntlog.count],"LogPath",NULL,tmpbuf1,sizeof(tmpbuf1),ntlog.logCfgName);
	if(strlen(tmpbuf1)==0)
	{
		strcpy(tmpbuf1,"D:\\NT_LOG");
	}
	CreateFilePath(tmpbuf1);
	
	ntlog.logPath[ntlog.count]=tmpbuf1;


	memset(tmpbuf1,0,sizeof(tmpbuf1));
	
	ret=GetPrivateProfileStringA(ntlog.logModule[ntlog.count],"LogSize",NULL,tmpbuf1,sizeof(tmpbuf1),ntlog.logCfgName);
	if(strlen(tmpbuf1)==0)
	{
		strcpy(tmpbuf1,"1073741824");
	}	
	tmpint=atol(tmpbuf1);
	if(tmpint == 0)
		tmpint=1073741824;
	ntlog.logSize[ntlog.count]=tmpint;

	memset(tmpbuf1,0,sizeof(tmpbuf1));
	
	ret=GetPrivateProfileStringA(ntlog.logModule[ntlog.count],"LogLevel",NULL,tmpbuf1,sizeof(tmpbuf1),ntlog.logCfgName);
	if(strlen(tmpbuf1)==0)
	{
		strcpy(tmpbuf1,"0");
	}
	tmpint=atoi(tmpbuf1);
	ntlog.logLevel[ntlog.count]=tmpint;


	ntlog.logPos[ntlog.count]=0;

	memset(tmpbuf1,0,sizeof(tmpbuf1));
	ret=GetPrivateProfileStringA(ntlog.logModule[ntlog.count],"LogDay",NULL,tmpbuf1,sizeof(tmpbuf1),ntlog.logCfgName);
	if(strlen(tmpbuf1)==0)
	{
		ntlog.logDay[ntlog.count]=10;
	}
	else
		ntlog.logDay[ntlog.count]=atoi(tmpbuf1);



	ntlog.count++;
	return ret;

}
void CNTTRACE::WriteTraceFile(char *filename, char *filepath, LPCSTR lpszTraceData, int TraceType)
{
	CTime t ;
	int CurrentYear,FileYear ;
	int Ret;
	CFileStatus FileStatus ;
	BOOL OpenFileResult ;
	CStdioFile TraceFile ;
	char TempBuff[255],TraceFileName[60] ;
	
	
	if( !strlen(filename) || !strlen(lpszTraceData)||!strlen(filepath) )
		printf("input buff is illegal!\n");
	Ret = CreateFilePath(filepath);
	if(Ret != 0)
	{
		printf("Can not creat the specific path [%s]\n",filepath);		
	}	
	t = CTime::GetCurrentTime();
	sprintf(TraceFileName,"%s\\%s.txt",filepath,filename) ;
	OpenFileResult = TraceFile.Open(TraceFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText,NULL) ; 
	if ( !OpenFileResult ) 
		//return false ;
		printf("can not open the specific file!\n");
	else
	{
		
		TraceFile.GetStatus(FileStatus) ;
		if ( FileStatus.m_size )
		{
			FileYear = FileStatus.m_mtime.GetYear() ;
			CurrentYear = t.GetYear() ;
			if ( FileYear != CurrentYear ) 
				TraceFile.SetLength(0) ;
		}
		
		TraceFile.SeekToEnd() ;
		if ( !TraceType ) 
		{
			sprintf(TempBuff,"Command:%s \n",lpszTraceData);
			TraceFile.WriteString(TempBuff) ;
		}
		else if ( TraceType == 3 ) TraceFile.WriteString(lpszTraceData) ;
		else if ( TraceType == 4 )
		{
			sprintf(TempBuff,"日期:%d年%d月%d日 时间:%02d:%02d:%02d  %s\n",t.GetYear(),t.GetMonth(),t.GetDay(),
				t.GetHour(),t.GetMinute(),t.GetSecond(),lpszTraceData);
			
			TraceFile.WriteString(TempBuff) ;
		}
		else 
		{
			sprintf(TempBuff,"\t日期: %d年%d月%d日   时间: %02d:%02d:%02d\n",t.GetYear(),t.GetMonth(),t.GetDay(),
				t.GetHour(),t.GetMinute(),t.GetSecond());
			
			TraceFile.WriteString(TempBuff) ;
			
			if(TraceType == 1)
				sprintf(TempBuff,"\tSend = %s\n",lpszTraceData);
			else if(TraceType == 2) sprintf(TempBuff,"\tReceive = %s\n",lpszTraceData);
			else sprintf(TempBuff,"No Used") ;
			TraceFile.WriteString(TempBuff) ;
			TraceFile.Close() ;
		}
		//return NULL ;
		filename=TraceFileName;
	}
}