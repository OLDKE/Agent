// GetPCResours.cpp: implementation of the CGetPCResours class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTAgent3.h"
#include "GetPCResours.h"
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <shlwapi.h>
#include   <cfgmgr32.h>
#include   "hidsdi.h"

#include "LogicCommon.h"

#pragma comment(lib,"hid.lib")
#pragma comment(lib, "setupapi.lib")

	
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetPCResours::CGetPCResours(CNTTRACE *pTrace)
{
	m_trace=pTrace;
}

CGetPCResours::~CGetPCResours()
{

}
BOOL CGetPCResours::GetSoftWareInfo(char *filepath)
{
	char msg[256];
	char filename[256];
	memset(filename,0,sizeof(filename));
	//	strcpy(filename,filepath);
	char tmp[20];
	memset(tmp,0,sizeof(tmp));
	GetNTReg("Software\\Ebring\\Agent\\Config","ATMNO",tmp);
	sprintf(filename,"%s\\SoftwareInfo_%s.txt",filepath,tmp);
	
	m_trace->WTrace(LOG_GRP, LOG_CTRL, LT_DEBUG, "在C盘中生成的文件名字为[%s]", filename);
	
    if (PathFileExists(filename))	
	{
		DeleteFile(filename);
	}   
    char logname[256];
	memset(logname,0,sizeof(logname));
	sprintf(logname,"SoftwareInfo_%s",tmp);

	HKEY   m_key,m_key2;   
	char   m_SubKey[255]="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";   
	DWORD   m_index=0,count=0;   
	char   m_name[200],m_displayname[200],buff[200],uninstall[200],version[50],Publisher[50],InstallData[20];   
	DWORD   m_namelen=200,m_displaynamelen=200,   m_uninstalllen=200,Ver_len=50,p_len=50,data_len=20;   
	DWORD   m_attr=REG_BINARY|REG_DWORD|REG_EXPAND_SZ|REG_MULTI_SZ|REG_NONE|REG_SZ;   
//	CString  szTemp="",sziret="";
	
	if   (RegOpenKeyEx(HKEY_LOCAL_MACHINE,m_SubKey,0,KEY_ALL_ACCESS,&m_key)==ERROR_SUCCESS)   
	{   
		while   (RegEnumKeyEx(m_key,m_index,m_name,&m_namelen,0,NULL,NULL,0)!=ERROR_NO_MORE_ITEMS)   
		{   
			m_index++;   
			if   (strcmp(m_name,"")!=NULL)   
			{   //如果找到了，分别在枚举下面有没有DISPLAYNAME，以便得到软件名称   
				strcpy(uninstall,m_SubKey);   
				strcat(uninstall,"\\");   
				strcat(uninstall,m_name);       
				m_displaynamelen=200;     
				memset(m_displayname,0,200);				
				m_uninstalllen=200;   
				memset(buff,0,200);
				memset(version,0,sizeof(version));
				memset(InstallData,0,sizeof(InstallData));
				memset(Publisher,0,sizeof(Publisher));
				if   (RegOpenKeyEx(HKEY_LOCAL_MACHINE,uninstall,0,KEY_ALL_ACCESS,&m_key2)==ERROR_SUCCESS)   
				{   
					RegQueryValueEx(m_key2,"DisplayName",0,&m_attr,LPBYTE(m_displayname),&m_displaynamelen);   
					RegQueryValueEx(m_key2,"UninstallString",0,&m_attr,LPBYTE(buff),&m_uninstalllen);   
					
					if   (   (strcmp(m_displayname,"")!=NULL)   ||     (strcmp(buff,"")!=NULL)   )   
					{   
						count++;

						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "\nDisplayName=%s\n",m_displayname);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);
						
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "UninstallString=%s\n",buff);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);

						RegQueryValueEx(m_key2,"DisplayVersion",0,&m_attr,LPBYTE(version),&Ver_len);
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "DisplayVersion=%s\n",version);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);

						RegQueryValueEx(m_key2,"Publisher",0,&m_attr,LPBYTE(Publisher),&p_len);
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "Publisher=%s\n",Publisher);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);

						RegQueryValueEx(m_key2,"InstallDate",0,&m_attr,LPBYTE(InstallData),&data_len);
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "InstallDate=%s\n",InstallData);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);
						
						memset(buff,0,sizeof(buff));
						m_uninstalllen=200;
						RegQueryValueEx(m_key2,"DisplayIcon",0,&m_attr,LPBYTE(buff),&m_uninstalllen);
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "DisplayIcon=%s\n",buff);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);

						memset(buff,0,sizeof(buff));
						m_uninstalllen=200;
						RegQueryValueEx(m_key2,"InstallLocation",0,&m_attr,LPBYTE(buff),&m_uninstalllen);
						memset(msg,0,sizeof(msg));
						sprintf( (char *)msg,  "InstallLocation=%s\n",buff);
						m_trace->WriteTraceFile(logname,filepath,(const char*)msg,3);
						
					} 
					  
				}   
				
			}   
			m_namelen=200;   
			memset(m_name,0,200);   
		}//end   while   
		RegCloseKey(m_key); 
		m_trace->WTrace(LOG_GRP, LOG_CTRL,LT_INFO,"Get software info sucess. The No. Of Software is %d.",count);
		return TRUE;
	}   //end   if   
	return FALSE;

}
BOOL IsDeviceDisabled(DWORD dwDevID, HDEVINFO hDevInfo)
{
	SP_DEVINFO_DATA DevInfoData = {sizeof(SP_DEVINFO_DATA)};
	DWORD    dwDevStatus,dwProblem;
	if(!SetupDiEnumDeviceInfo(hDevInfo,dwDevID,&DevInfoData))
	{
		return FALSE;
	}
	
	//查询设备状态
	if(CM_Get_DevNode_Status(&dwDevStatus,&dwProblem,DevInfoData.DevInst,0)!=CR_SUCCESS)
	{
		return FALSE;
	}
	return ((dwDevStatus & DN_HAS_PROBLEM) && (dwProblem == CM_PROB_DISABLED));
	return TRUE;
}
BOOL CGetPCResours::GetHardWareInfo(char *filepath)
{

//	char msg[256];
	char filename[256];
	memset(filename,0,sizeof(filename));
	//	strcpy(filename,filepath);
	char tmp[20];
	memset(tmp,0,sizeof(tmp));
	GetNTReg("Software\\Ebring\\Agent\\Config","ATMNO",tmp);
	sprintf(filename,"%s\\HardWareInfo_%s.txt",filepath,tmp);
	
    if (PathFileExists(filename))
	{
		DeleteFile(filename);
	}   
    char logname[256];
	memset(logname,0,sizeof(logname));
	sprintf(logname,"HardWareInfo_%s",tmp);

	SP_DEVINFO_DATA        DeviceInfoData    =    {sizeof(SP_DEVINFO_DATA)}; 
	DWORD        BufSize                =    MAX_PATH;     
	DWORD        RetBufSize            =    0;
	DWORD        DeviceId            =    0;
	DWORD        dwIndex                =    0;
	CHAR        szDevName[MAX_PATH]    ;
	CHAR        szName[MAX_PATH] ;
	char		buff[MAX_PATH];
	CHAR        szDriver[MAX_PATH];
	CHAR        szDesc[MAX_PATH];    
	CHAR        szHDID[MAX_PATH];
	CHAR        szFrName[MAX_PATH] ;
//	CHAR        szMFG[MAX_PATH] ;
	HDEVINFO    hDevInfo;
	DWORD        Status, Problem; 
	
//	LVITEMA        lvi;
//	DWORD        dwRow;


    
    hDevInfo = SetupDiGetClassDevs(NULL,NULL,0,DIGCF_PRESENT|DIGCF_ALLCLASSES);
    /*
    GUID_DEVCLASS_FDC软盘控制器
    GUID_DEVCLASS_DISPLAY显示卡
    GUID_DEVCLASS_CDROM光驱
    GUID_DEVCLASS_KEYBOARD键盘
    GUID_DEVCLASS_COMPUTER计算机
    GUID_DEVCLASS_SYSTEM系统
    GUID_DEVCLASS_DISKDRIVE磁盘驱动器
    GUID_DEVCLASS_MEDIA声音、视频和游戏控制器
    GUID_DEVCLASS_MODEMMODEM
    GUID_DEVCLASS_MOUSE鼠标和其他指针设备
    GUID_DEVCLASS_NET网络设备器
    GUID_DEVCLASS_USB通用串行总线控制器
    GUID_DEVCLASS_FLOPPYDISK软盘驱动器
    GUID_DEVCLASS_UNKNOWN未知设备
    GUID_DEVCLASS_SCSIADAPTERSCSI 和 RAID 控制器
    GUID_DEVCLASS_HDCIDE ATA/ATAPI 控制器
    GUID_DEVCLASS_PORTS端口（COM 和 LPT）
    GUID_DEVCLASS_MONITOR监视器
   */

    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        DWORD dwError = GetLastError();
        printf("SetupDiGetClassDevs error=[%d]",dwError);
        return 1;
    }

	for (DeviceId=0;SetupDiEnumDeviceInfo( hDevInfo,DeviceId,&DeviceInfoData);DeviceId++) 
	{ 
		// 获得设备的状态
		if (CM_Get_DevNode_Status(&Status, &Problem, DeviceInfoData.DevInst ,0) != CR_SUCCESS) 
		{
			continue; 
		}
		
		RtlZeroMemory(szDevName,MAX_PATH);
		// 获取设备类名
		if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData,SPDRP_CLASS ,NULL,(PBYTE)szDevName,BufSize,&RetBufSize))
		{
			RtlZeroMemory(szName,MAX_PATH);
			if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData,SPDRP_ENUMERATOR_NAME ,NULL,(PBYTE)szName,BufSize,&RetBufSize) && lstrlenA( szName)>0) 
			{
				RtlZeroMemory(szDriver,MAX_PATH);
				// 获取设备驱动名称
				if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DRIVER ,NULL, (PBYTE)szDriver,BufSize,&RetBufSize)) 
				{
					RtlZeroMemory(szDesc,MAX_PATH);
					// 获取设备描述                    
					if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC,NULL,(PBYTE)szDesc,BufSize,&RetBufSize)) 
					{
						if (lstrlenA( szDevName) > 0 && lstrlenA( szName) > 0 && lstrlenA( szDriver) > 0 )
						{
							memset(buff,0,sizeof(buff));
							sprintf(buff,"\nDeviceClassName=%s\n",szDevName);
							m_trace->WriteTraceFile(logname,filepath,buff,3);

							memset(buff,0,sizeof(buff));
							sprintf(buff,"DeviceSZName=%s\n",szName);
							m_trace->WriteTraceFile(logname,filepath,buff,3);

							memset(buff,0,sizeof(buff));
							sprintf(buff,"DeviceInfo=%s\n",szDesc);
							m_trace->WriteTraceFile(logname,filepath,buff,3);

							memset(buff,0,sizeof(buff));
							//获取设备运行状态
							if (IsDeviceDisabled(DeviceId, hDevInfo)==FALSE)
							{
								sprintf(buff,"DeviceStat=%s\n","Disable");
							}
							else
							{
								sprintf(buff,"DeviceStat=%s\n","Normal");
							}							
							m_trace->WriteTraceFile(logname,filepath,buff,3);
							//获取设备显示名称
							RtlZeroMemory(szFrName,MAX_PATH);
							if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData,SPDRP_FRIENDLYNAME,NULL,(PBYTE)szFrName,BufSize,&RetBufSize)) 
							{
								memset(buff,0,sizeof(buff));
								sprintf(buff,"DeviceFriendlyName=%s\n",szFrName);
								m_trace->WriteTraceFile(logname,filepath,buff,3);
							}
							
							RtlZeroMemory(szHDID,MAX_PATH);
							if (SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData,SPDRP_HARDWAREID,NULL,(PBYTE)szHDID,BufSize,&RetBufSize)) 
							{
								memset(buff,0,sizeof(buff));
								sprintf(buff,"DeviceHARDWAREID=%s\n",szHDID);
								m_trace->WriteTraceFile(logname,filepath,buff,3);
							}
							else
							{
								memset(buff,0,sizeof(buff));
								sprintf(buff,"DeviceHARDWAREID=%s\n","");
								m_trace->WriteTraceFile(logname,filepath,buff,3);
							}						
							
						}
					}
				}
			}
		}
	} 
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return TRUE;
	 
}