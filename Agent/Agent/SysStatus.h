// SysStatus.h: interface for the CSysStatus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSSTATUS_H__D21014BE_57F4_4AC0_8395_3EDFA7DBF2AD__INCLUDED_)
#define AFX_SYSSTATUS_H__D21014BE_57F4_4AC0_8395_3EDFA7DBF2AD__INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <winbase.h>
#include <shlwapi.h>
#include "NTTRACE.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSysStatus  
{
public:
	CSysStatus(CNTTRACE *pTrace);
	virtual ~CSysStatus();

	CNTTRACE *m_trace;
	
public:
	BOOL Initialize();
    int nNewUsges;
	int GetCpuUsgesNt();
	int GetCpuUsges9X();
	static UINT GetCpuUsage(LPVOID lpParameter);
	HANDLE hThread;
	LARGE_INTEGER   m_liOldIdleTime;
    LARGE_INTEGER   m_liOldSystemTime;
	 int	 m_CPUUsges;
	CString    DiskInfo_all1;
	CString    DiskInfo_all2;
	CString    SysInfo;
	CString    MemInfo1;
	CString    MemInfo2;
	char filename[256];
	char agentpath[256];
    
	BOOL       GetallInfo1();
	BOOL       GetallInfo2();
	
    BOOL       GetSysInfo();	
	BOOL       GetDiskInfo();
	BOOL       GetMemoryInfo();
	
	
	CString  SysStatus_all;

};

#endif // !defined(AFX_SYSSTATUS_H__D21014BE_57F4_4AC0_8395_3EDFA7DBF2AD__INCLUDED_)
