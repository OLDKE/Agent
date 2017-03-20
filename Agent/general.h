#include <Tlhelp32.h>
#include "PSAPI.H"

static int GetNTReg(char* RegPath,char* RegName,char* RegVal)
{
	bool ret=TRUE;
	HKEY hKey;
	DWORD dwLength=8192;
	DWORD dwType=REG_SZ;

	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,RegPath,0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueExA(hKey,RegName,NULL,&dwType,(unsigned char *)RegVal,&dwLength)!=ERROR_SUCCESS)
		{
			strcpy(RegVal,"");
			ret=FALSE;
		}
		RegCloseKey(hKey);
	}
	else
	{
		strcpy(RegVal,"");
		ret=FALSE;
	}
	return ret;

};

static int SetNTReg(char* RegPath,char* RegName,char* RegVal)
{
	bool ret=TRUE;
	HKEY hKey;
	DWORD dwLength=256;
	DWORD dwType=REG_SZ;

	if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,RegPath,0,KEY_WRITE,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueExA(hKey,RegName,NULL,REG_SZ,(unsigned char *)RegVal,(ULONG)strlen(RegVal))!=ERROR_SUCCESS)
		{
			strcpy(RegVal,"");
			ret=FALSE;
		}
		RegCloseKey(hKey);
	}
	else
	{
		strcpy(RegVal,"");
		ret=FALSE;
	}
	return ret;
};

static void GetNowTime(char * nowtime)
{
	char tmpbuf[32];
	SYSTEMTIME localtime;

	memset(tmpbuf,0x00,sizeof(tmpbuf));
	GetLocalTime(&localtime);

	sprintf(tmpbuf,"%04d%02d%02d%02d%02d%02d",
		localtime.wYear,localtime.wMonth,localtime.wDay,
		localtime.wHour,localtime.wMinute,localtime.wSecond);

	strcpy(nowtime,tmpbuf);
};

static void GetAheadTime(char * mytime,int myday,int myhour,int mymin,int mysec)
{
	struct tm *when;
	time_t now, result;

	time(&now);
	when = localtime( &now ); 
	when->tm_mday = when->tm_mday + myday;
	when->tm_hour = when->tm_hour + myhour;
	when->tm_min = when->tm_min + mymin;
	when->tm_sec = when->tm_sec + mysec;
	if( (result = mktime( when )) != (time_t)-1 )
	{
		sprintf(mytime,"%04d%02d%02d%02d%02d%02d",(when->tm_year)+1900,(when->tm_mon)+1,
			when->tm_mday,when->tm_hour,when->tm_min,when->tm_sec);

	}
	else
		sprintf(mytime,"20071231235959");

};
static long TimeCount(char *starttime, char *endtime)
{// 返回 starttime和endtime的时间隔，位秒 

	struct tm tm_start,tm_end;
	time_t ltm_start,ltm_end;

	long tmcount=0;
	char tmpbuf[15];

	memset(tmpbuf,0,sizeof(tmpbuf));
	memcpy(tmpbuf,starttime,4);
	tmpbuf[4]='\0';
	tm_start.tm_year=atoi(tmpbuf)-1900;

	memcpy(tmpbuf,starttime+4,2);
	tmpbuf[2]='\0';
	tm_start.tm_mon=atoi(tmpbuf)-1;

	memcpy(tmpbuf,starttime+6,2);
	tmpbuf[2]='\0';
	tm_start.tm_mday=atoi(tmpbuf);

	memcpy(tmpbuf,starttime+8,2);
	tmpbuf[2]='\0';
	tm_start.tm_hour=atoi(tmpbuf);

	memcpy(tmpbuf,starttime+10,2);
	tmpbuf[2]='\0';
	tm_start.tm_min=atoi(tmpbuf);

	memcpy(tmpbuf,starttime+12,2);
	tmpbuf[2]='\0';
	tm_start.tm_sec=atoi(tmpbuf);

	ltm_start=mktime(&tm_start);

	memcpy(tmpbuf,endtime,4);
	tmpbuf[4]='\0';
	tm_end.tm_year=atoi(tmpbuf)-1900;

	memcpy(tmpbuf,endtime+4,2);
	tmpbuf[2]='\0';
	tm_end.tm_mon=atoi(tmpbuf)-1;

	memcpy(tmpbuf,endtime+6,2);
	tmpbuf[2]='\0';
	tm_end.tm_mday=atoi(tmpbuf);

	memcpy(tmpbuf,endtime+8,2);
	tmpbuf[2]='\0';
	tm_end.tm_hour=atoi(tmpbuf);

	memcpy(tmpbuf,endtime+10,2);
	tmpbuf[2]='\0';
	tm_end.tm_min=atoi(tmpbuf);

	memcpy(tmpbuf,endtime+12,2);
	tmpbuf[2]='\0';
	tm_end.tm_sec=atoi(tmpbuf);

	ltm_end=mktime(&tm_end);
	tmcount=ltm_end-ltm_start;
	return tmcount;
};


static bool MyCopyDirectory(LPCTSTR lpszFrom,LPCTSTR lpszTo)  
{  
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
	bRet = (SHFileOperation(&fileop) == 0);    
	return  bRet;  
}

static bool MyCopyDirectoryEx(LPCTSTR lpszFrom,LPCTSTR lpszTo,bool bRootIn)  
{  
	ASSERT((lpszFrom != NULL) &&  (lpszTo  !=  NULL));  
	if((lpszFrom == NULL) || (lpszTo == NULL))  
		return  FALSE;  

	if  (bRootIn)  
		return  MyCopyDirectory(lpszFrom,  lpszTo);  
	else  
	{  
		CString  strFindFile  =  lpszFrom;  
		strFindFile.TrimRight();  
		strFindFile.TrimRight(_T('\\'));  
		strFindFile  +=  _T("\\*.*");  

		CFileFind  ff;  
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

				if  (!CopyFile(ff.GetFilePath(),  strTo  +  ff.GetFileName(),  FALSE))  
					return  FALSE;  
			}  
			else  if  (!MyCopyDirectory(ff.GetFilePath(),  lpszTo))  
				return  FALSE;  
		}  
		return  TRUE;  
	}  
}


static bool DeleteDirectory(char* sDirName)
{
	CFileFind tempFind;
	char sTempFileFind[200];

	sprintf(sTempFileFind,"%s\\*.*",sDirName);
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
				sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				char sTempFileName[200];
				sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName);
				SetFileAttributes(sTempFileName,FILE_ATTRIBUTE_NORMAL);
				DeleteFile(sTempFileName);
			}
		}
	}
	tempFind.Close();
	if(!RemoveDirectory(sDirName))
	{
		return FALSE;
	}
	return TRUE;
}

static bool MyKillProcess(char* filenameex)
{
/*	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		return false;
	}

	SHFILEINFO shSmall;
	char filename[256];
	char myfile[256];
	strcpy(myfile,filenameex);

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize=sizeof(ProcessInfo);

	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0;
	while(Status)
	{
		m_nProcess++;
		ZeroMemory(&shSmall,sizeof(shSmall));
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,
			sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);

		sprintf(filename,"%s",ProcessInfo.szExeFile);
		if(!strcmp(filename,myfile))
		{
			HANDLE hProcess= OpenProcess( PROCESS_ALL_ACCESS, FALSE,ProcessInfo.th32ProcessID);
			if(::TerminateProcess(hProcess,1))
			{

				break;
			}
			break;
		}
		Status=Process32Next(SnapShot,&ProcessInfo);
	}		
*/
	  char myfile[256];
	  int   nCount,i; 
	  DWORD   pProcessID[1024];
	  char   szName[256];  
	char fullPath[256];
	DWORD   dwSize;   
	HANDLE   hProcess;
	HMODULE   hModule;
	strcpy(myfile,filenameex);

	DWORD curProcessID=GetCurrentProcessId();
	EnumProcesses(pProcessID,sizeof(pProcessID),&dwSize);   
	nCount=dwSize/sizeof(DWORD);   
	for(i=0;i<nCount;i++)   
	{   
		if(curProcessID == pProcessID[i])
			continue;
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pProcessID[i]);   
		if(hProcess)   
		{   
			if(EnumProcessModules(hProcess,&hModule,sizeof(hModule),&dwSize))   
			{   
				GetModuleFileNameEx(hProcess,hModule,fullPath,sizeof(fullPath));   
				char *p=strrchr(fullPath,'\\');
				p++;
				strcpy(szName,p);
				if(!stricmp(szName,myfile))
				{
					::TerminateProcess(hProcess,1);
				}
			}   
		}
		CloseHandle(hProcess); 
	} 	


	return true;
}

static bool CheckProcess(char* filenameex)
{
	bool ret=false;
	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		return ret;
	}

	SHFILEINFO shSmall;
	char filename[256];
	char myfile[256];
	strcpy(myfile,filenameex);

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize=sizeof(ProcessInfo);

	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0;
	while(Status)
	{
		m_nProcess++;
		ZeroMemory(&shSmall,sizeof(shSmall));
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,
			sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);

		sprintf(filename,"%s",ProcessInfo.szExeFile);
		if(!strcmp(filename,myfile))
		{
			ret=true;
			break;
		}
		Status=Process32Next(SnapShot,&ProcessInfo);
	}		
	return ret;
}
