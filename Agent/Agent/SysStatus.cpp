// SysStatus.cpp: implementation of the CSysStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTAgent3.h"
#include "SysStatus.h"

#include "LogicCommon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SystemBasicInformation       0
#define SystemPerformanceInformation 2
#define SystemTimeInformation        3

#define Li2Double(x) ((double)((x).HighPart) * 4.294967296E9 + (double)((x).LowPart))

typedef struct
{
    DWORD   dwUnknown1;
    ULONG   uKeMaximumIncrement;
    ULONG   uPageSize;
    ULONG   uMmNumberOfPhysicalPages;
    ULONG   uMmLowestPhysicalPage;
    ULONG   uMmHighestPhysicalPage;
    ULONG   uAllocationGranularity;
    PVOID   pLowestUserAddress;
    PVOID   pMmHighestUserAddress;
    ULONG   uKeActiveProcessors;
    BYTE    bKeNumberProcessors;
    BYTE    bUnknown2;
    WORD    wUnknown3;
} SYSTEM_BASIC_INFORMATION;

typedef struct
{
    LARGE_INTEGER   liIdleTime;
    DWORD           dwSpare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
    LARGE_INTEGER liKeBootTime;
    LARGE_INTEGER liKeSystemTime;
    LARGE_INTEGER liExpTimeZoneBias;
    ULONG         uCurrentTimeZoneId;
    DWORD         dwReserved;
} SYSTEM_TIME_INFORMATION;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysStatus::CSysStatus(CNTTRACE *pTrace)
{
	m_trace=pTrace;
	m_liOldIdleTime.QuadPart   = 0;
    m_liOldSystemTime.QuadPart = 0;
	
	memset(agentpath,0,sizeof(agentpath));
	GetNTReg("\\SOFTWARE\\Ebring\\Agent\\Config","AGENT_PATH",agentpath);

}

CSysStatus::~CSysStatus()
{

}
BOOL CSysStatus::Initialize()
{
	m_CPUUsges=10;
	nNewUsges=10;
	hThread=AfxBeginThread(GetCpuUsage,this);
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The hThread is %d",hThread);
	
	return TRUE;
}
//******************************************��ȡϵͳΪNT��CPU������
int  CSysStatus::GetCpuUsgesNt()
{
	
	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
    SYSTEM_TIME_INFORMATION        SysTimeInfo;
    SYSTEM_BASIC_INFORMATION       SysBaseInfo;
    double                         dbIdleTime;
    double                         dbSystemTime;
    typedef LONG (WINAPI *PROCNTQSI)(UINT,PVOID,ULONG,PULONG);
    PROCNTQSI NtQuerySystemInformation;
	
    if (!(NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"), "NtQuerySystemInformation")))
        return 0;
	
    // get number of processors in the system
    if (NtQuerySystemInformation(SystemBasicInformation, &SysBaseInfo,sizeof(SysBaseInfo),NULL) != NO_ERROR)
        return 0;	
	if (NtQuerySystemInformation(SystemTimeInformation, &SysTimeInfo,sizeof(SysTimeInfo),0) != NO_ERROR)
		return 0;
	
	// get new CPU's idle time
	if (NtQuerySystemInformation(SystemPerformanceInformation, &SysPerfInfo,sizeof(SysPerfInfo),NULL) != NO_ERROR)
		return 0;
	
	// if it's a first call - skip it
	if (m_liOldIdleTime.QuadPart != 0)
	{
	// CurrentValue = NewValue - OldValue
		dbIdleTime = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(m_liOldIdleTime);
		dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(m_liOldSystemTime);
	
	// CurrentCpuIdle = IdleTime / SystemTime
		dbIdleTime = dbIdleTime / dbSystemTime;
	
	// CurrentCpuUsage% = 100 - (CurrentCpuIdle * 100) / NumberOfProcessors
		dbIdleTime = 100.0 - dbIdleTime * 100.0 / (double)SysBaseInfo.bKeNumberProcessors + 0.5;		
		nNewUsges = (UINT)dbIdleTime;
	}
	
	// store new CPU's idle and system time
	m_liOldIdleTime = SysPerfInfo.liIdleTime;
	m_liOldSystemTime = SysTimeInfo.liKeSystemTime;
	
	return nNewUsges;
	
}

//******************************************��ȡϵͳΪ9X��CPU������
int CSysStatus::GetCpuUsges9X()
{
	HKEY hkey;
    DWORD dwDataSize;
    DWORD dwType;
    DWORD dwCpuUsage;
	int nNewUsges=10;
	
    // starting the counter
    if (RegOpenKeyEx( HKEY_DYN_DATA, "PerfStats\\StartStat", 0, KEY_ALL_ACCESS, &hkey ) != ERROR_SUCCESS)
		return 0;	
    dwDataSize=sizeof(DWORD);
    if (RegQueryValueEx( hkey, "KERNEL\\CPUUsage", NULL, &dwType, (LPBYTE)&dwCpuUsage, &dwDataSize) != ERROR_SUCCESS)
		return 0;
    RegCloseKey(hkey);
    
    // geting current counter's value
    if (RegOpenKeyEx( HKEY_DYN_DATA, "PerfStats\\StatData", 0,KEY_READ, &hkey ) != ERROR_SUCCESS)	
		return 0;	
    dwDataSize=sizeof(DWORD);
    if (RegQueryValueEx(hkey, "KERNEL\\CPUUsage", NULL,&dwType, (LPBYTE)&dwCpuUsage, &dwDataSize)  != ERROR_SUCCESS)
		return 0;	
    nNewUsges = dwCpuUsage;
    RegCloseKey(hkey);
    
    // stoping the counter
    if (RegOpenKeyEx( HKEY_DYN_DATA, "PerfStats\\StopStat", 0,KEY_ALL_ACCESS, &hkey ) != ERROR_SUCCESS)
		return 0;	
    dwDataSize=sizeof(DWORD);
    if (RegQueryValueEx(hkey, "KERNEL\\CPUUsage", NULL,&dwType, (LPBYTE)&dwCpuUsage, &dwDataSize) != ERROR_SUCCESS)
		return 0;
    RegCloseKey(hkey);
	
	return nNewUsges;
}

//******************************************��ȡCPU�������̺߳���
UINT CSysStatus::GetCpuUsage(LPVOID lpParameter)
{
	CSysStatus * pSysSta;
	pSysSta = (CSysStatus *)lpParameter;

	DWORD winVer;		
	winVer = GetVersion();
 	if(winVer<0x80000000)
 	{
		while(1)
		{
			::Sleep(1000);
 			pSysSta->m_CPUUsges=pSysSta->GetCpuUsgesNt();//  NT
		}
	}
	else
	{
		while(1)
		{
			::Sleep(1000);
			pSysSta->m_CPUUsges=pSysSta->GetCpuUsges9X();//  9X ϵͳCPU������
		}		
	}	
    return 1;
}

//******************************************��ȡ����ϵͳ�汾�����͵���Ϣ
BOOL CSysStatus::GetSysInfo()
{
	char   windir[MAX_PATH];  
    
	//���WindowsĿ¼   
	GetWindowsDirectory(windir,MAX_PATH);  
    
	//���WindowsϵͳĿ¼   
	GetSystemDirectory(windir,MAX_PATH); 
    
	//��ò���ϵͳ��Ϣ   
	OSVERSIONINFO   osvi;   
	CString   os;   
   
	osvi.dwOSVersionInfoSize   =   sizeof(OSVERSIONINFO);   
	GetVersionEx   (&osvi);   
    
	switch(osvi.dwPlatformId)   
	{   
	case   0:   
		os   =   "Windows   3.x";   
		break;   
	case   1:   
		os   =   "Windows   95/98/Me";   
		break;   
	case   2:   
		os   =   "Windows   NT/2000";   
		break;   
	} 
	if (osvi.dwMajorVersion==5&&osvi.dwMinorVersion==1)
	{
		os=  "Windows XP";
	}
    if (osvi.dwMajorVersion==5&&osvi.dwMinorVersion==0)
	{
		os=  "Windows 2000";
	}
	SysInfo.Format(_T("\tSystem Version=%d.%d \n\tOpretion System=%s \n\tMore detail=Sever pack %d"),osvi.dwMajorVersion,osvi.dwMinorVersion,os,osvi.dwPlatformId); 
	return true;
}

//******************************************��ȡϵͳ������Ϣ��ʹ���ʣ����̴�С��
BOOL CSysStatus::GetDiskInfo()
{
	//��ʾ���̴洢��Ϣ
	DWORD dwNumBytesForDriveStrings;//ʵ�ʴ洢�������ŵ��ַ�������
	HANDLE hHeap;
	LPSTR lp;
	CString DiskInfo1,DiskInfo2;

	//���ʵ�ʴ洢�������ŵ��ַ�������
	dwNumBytesForDriveStrings=GetLogicalDriveStrings(0,NULL)*sizeof(TCHAR);

	//����ַ�����Ϊ�գ����ʾ������������������
	if (dwNumBytesForDriveStrings!=0) 
	{
		//�����ַ����ռ�
		hHeap=GetProcessHeap();
		lp=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,dwNumBytesForDriveStrings);

		//��ñ����������������ַ���
		GetLogicalDriveStrings(HeapSize(hHeap,0,lp),lp);
        
		//��������һ�����ŵ���������
		while (*lp!=0)
		{
		//���ѡ���������
			CString Driver;
			Driver=*(new CString (lp));

			//�жϴ����Ƿ��ǹ���
			 UINT   IsCDRom;   
			 IsCDRom=GetDriveType(Driver);   
			 if(IsCDRom==DRIVE_CDROM)                  //�ǹ���
			 {
				 lp=_tcschr(lp,0)+1;
				 continue;
			 } 
			 else
			 {
				 //��ô��̿ռ���Ϣ
				 DWORD dwSectorsPerCluster=0;//ÿ����������
				 DWORD dwBytesPerSector=0;//ÿ�������ֽ���
				 DWORD dwFreeClusters=0;//ʣ�����
				 DWORD dwTotalClusters=0;//�ܴ���
				 double strTotalBytes,strFreeBytes,UseAge;
				 if(GetDiskFreeSpace(Driver,&dwSectorsPerCluster,&dwBytesPerSector,&dwFreeClusters,&dwTotalClusters))
				 {
					 double dd=dwSectorsPerCluster*dwBytesPerSector/(1024.*1024.);
					 dd=dd/1024.;
					 strTotalBytes=dwTotalClusters*dd;//D���������ܴ�С
					 					 
					 dd=dwSectorsPerCluster*dwBytesPerSector/(1024.*1024.);
					 dd=dd/1024.;
					 strFreeBytes=dwFreeClusters*dd;//D���������ܴ�С
					 
					 UseAge=100.0-(double)strFreeBytes/(double)strTotalBytes*100;
					 char diskid[2];
					 memset(diskid,0,2);
					 strncpy(diskid,Driver,strlen(Driver)-2);
					 DiskInfo2.Format(_T("<DiskTotal_%s>%04.3fGB</DiskTotal_%s><DiskLoad_%s>%02.2f%%</DiskLoad_%s>"),diskid,strTotalBytes,diskid,diskid,UseAge,diskid);
					 DiskInfo1.Format(_T("\t[%s Disk]\n\t\ttotalspace=%04.3fGB\n\t\tFreeSpace=%04.3fGB\n\t\tUsage=%02.2f%%\n"),diskid,strTotalBytes,strFreeBytes,UseAge);
				 }
			 }
			 DiskInfo_all1=DiskInfo_all1+DiskInfo1;
			 DiskInfo_all2=DiskInfo_all2+DiskInfo2;
			 lp=_tcschr(lp,0)+1;
		}
		return true;
	}
	else 
	{		
		return false;
	}
}

//******************************************��ȡ�ڴ���Ϣ���������ڴ桢�������ڴ棬�ڴ�ʹ���ʣ�
BOOL CSysStatus::GetMemoryInfo()
{
	//��ʾ�ڴ���Ϣ
	MEMORYSTATUS   memstatus;
	double m_dmem_AvailPhys;
	double m_dmem_AviaiVir;
	double m_dmem_totalphys;
    double m_dmem_totalvir;
	double m_dmem_Load;

	memstatus.dwLength   =sizeof(MEMORYSTATUS);   
	GlobalMemoryStatus(&memstatus);
	
	double Mem_buffer=memstatus.dwAvailPhys/(1024.*1024.);
	m_dmem_AvailPhys=Mem_buffer;//���������ڴ� 
	
	
	Mem_buffer=memstatus.dwAvailVirtual/(1024.*1024.);
	m_dmem_AviaiVir=Mem_buffer;//���������ڴ�
		
	Mem_buffer=memstatus.dwTotalPhys/(1024.*1024.);
	m_dmem_totalphys=Mem_buffer;//�������ڴ�
		
	Mem_buffer=memstatus.dwTotalVirtual/(1024.*1024.);
	m_dmem_totalvir=Mem_buffer;//�������ڴ�

	m_dmem_Load=memstatus.dwMemoryLoad;//�ڴ�ʹ����
   	MemInfo1.Format(_T("\tTotal PhsMem=%04.3fMB\n\tTotal VirMem=%04.3fMB\n\tAviailable PhsMem=%04.3fMB\n\tAviailable VirMem=%04.3fMB\n\tMemery Load=%d%%\n"),m_dmem_totalphys,m_dmem_totalvir,m_dmem_AvailPhys,m_dmem_AviaiVir,memstatus.dwMemoryLoad);
	MemInfo2.Format(_T("<MEMERYPHS>%04.3fMB</MEMERYPHS><MEMERYVIR>%04.3fMB</MEMERYVIR><MemeryLoad>%d%%</MemeryLoad>"),m_dmem_totalphys,m_dmem_totalvir,memstatus.dwMemoryLoad);
	return TRUE;	
}

//******************************************��ȡϵͳ��Ϣ��д���ļ�SysInfo.txt��
BOOL CSysStatus::GetallInfo1()
{
	BOOL iret;
	
	//::GetCurrentDirectory(256,filename);
	memset(filename,0,sizeof(filename));
	strcpy(filename,agentpath);
	strcat(filename,"\\UPFILE");
	m_trace->CreateFilePath(filename);
	strcat(filename,"\\SysInfo.txt");
	DeleteFile(filename);
	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"����SYSINFO.txt�ļ��ɹ�");
	iret=GetSysInfo();
	if (iret==TRUE)
	{
		CString SysStatus;
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get Agent System Information success!");
		FILE *stream;
		stream=fopen(filename,"a");
		SysStatus.Format("[system Information]\n%s\n",SysInfo);
		fwrite(SysStatus,sizeof(char),strlen(SysStatus),stream);//д��Agent��ϵͳ��Ϣ
		fclose(stream);
	}
	else
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get Agent System Information failed!");			
	}
	if (hThread==NULL)
	{
		CString SysStatus;
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get Agent CPU Usage success!");
//		FILE *stream;
//		stream=fopen(filename,"a");
		SysStatus.Format("\n[CPU Infomation]\n\tUsage=%d%%\n",m_CPUUsges);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The m_CPUUsges success is %d",m_CPUUsges);
//		fwrite(SysStatus,sizeof(char),strlen(SysStatus),stream);//д��Agent��ϵͳ��Ϣ
//		fclose(stream);	
	}
	else
	{
		CString SysStatus;
		SysStatus.Format("\n[CPU Infomation]\n\tUsage=%d%%\n",m_CPUUsges);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The m_CPUUsges is %d",m_CPUUsges);
	}
	iret=GetMemoryInfo();
	if (iret==TRUE)
	{
		CString SysStatus;
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get Agent Memory Usage success!");
		FILE *stream;
		stream=fopen(filename,"a");
		SysStatus.Format("\n[Memory Status]\n%s\n",MemInfo1);
		fwrite(SysStatus,sizeof(char),strlen(SysStatus),stream);//д��Agent��ϵͳ��Ϣ
		fclose(stream);
	}
		
	iret=GetDiskInfo();
	if (iret==TRUE)
	{
		CString SysStatus;
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get Agent DISKs Information success!");
		FILE *stream;
		stream=fopen(filename,"a");
		SysStatus.Format("[DISK Status]\n%s\n",DiskInfo_all1);
		fwrite(SysStatus,sizeof(char),strlen(SysStatus),stream);//д��Agent��ϵͳ��Ϣ
		fclose(stream);
	}
    char *lpStr1=filename;
	if (!PathFileExists(lpStr1))
	{
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"Get SysInfo Failed!");
		return FALSE;
	}			
	return TRUE;	
}
BOOL CSysStatus::GetallInfo2()
{
	BOOL bret;
	SysStatus_all="";
    MemInfo2="";
    DiskInfo_all2="";
//	m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The hThread is  %s",hThread);	
	if (hThread==NULL)
	{
		SysStatus_all.Format("<CPUUSAGE>%d%%</CPUUSAGE>",m_CPUUsges);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The m_CPUusg(hThread=NULL) is %d",m_CPUUsges);
	}else{
		SysStatus_all.Format("<CPUUSAGE>%d%%</CPUUSAGE>",m_CPUUsges);
		m_trace->WTrace(LOG_GRP,LOG_CTRL,LT_INFO,"The m_CPUusg(hThread!=NULL) is %d",m_CPUUsges);		
	}
	bret=GetMemoryInfo();
	if (bret==TRUE)
	{		
		SysStatus_all=SysStatus_all+MemInfo2;		
	}	
	bret=GetDiskInfo();
	if (bret==TRUE)
	{		
		SysStatus_all=SysStatus_all+DiskInfo_all2;
	}	
	m_trace->WTrace(LOG_GRP, LOG_CTRL,LT_INFO,"Get FTPSever's Information success!");
	return TRUE;	
	
}