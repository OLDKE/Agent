#ifndef __CNTTRACE_H_
#define __CNTTRACE_H_

#define LT_INFO  1
#define LT_DEBUG 2
#define LT_ERROR 4

typedef struct
{
	CString logPath[256];
	CString logModule[256];
	CString logCfgName;
	unsigned  long logSize[256];
	int logLevel[256];
	unsigned long logPos[256];
	int logDay[256];
	int count;
}NTLOG;
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <time.h>
#include <io.h>
#include <string.h>

#include <direct.h>

using namespace std;

class CNTTRACE
{
public:
	CNTTRACE();
	~CNTTRACE();
	void ClearLog(char *pGrp);
	int FindPathIndex(char *pGrp);
	void WTrace(char *pGrp,char *pFileName,int iLevel, char *fmt, ... );
	void WTraceNoTime(char *pGrp,char *pFileName,int iLevel, char *fmt, ... );
	void WriteTraceFile(char *filename, char *filepath, LPCSTR lpszTraceData, int TraceType);
	int CreateFilePath(char * pFilePath);
	int DeleteFile(char * pFileName);
	int GetTraceCFG(char* CfgName,char* GrpName);
	/**
	* 打印日志
	* iLevel:日志级别
	* fmt:日志内容
	*/
	void CNTTRACE::WTrace(int iLevel, char *fmt, ... );

	/**
	* 打印日志 info 日志
	* iLevel:日志级别
	* fmt:日志内容
	*/
	void CNTTRACE::info(char *fmt, ... );

	/**
	* 打印日志 DEBUG 日志
	* fmt:日志内容
	*/
	void CNTTRACE::debug(char *fmt, ... );

	/**
	* 打印日志 error 日志
	* fmt:日志内容
	*/
	void CNTTRACE::error(char *fmt, ... );

	NTLOG ntlog;
private:
	void GetLocalTimeForStr(char * pTime);
	ofstream fout;
	CRITICAL_SECTION   m_critical;

};
#endif