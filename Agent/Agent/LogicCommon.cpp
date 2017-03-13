// LogicCommon.cpp: implementation of the CLogicCommon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogicCommon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicCommon::CLogicCommon()
{

}

CLogicCommon::~CLogicCommon()
{

}

BOOL CLogicCommon::CreateDirByPath(CString strPath, BOOL hasFileName)
{

	if(hasFileName) {
		strPath = strPath.Mid(0, strPath.ReverseFind('\\'));
	}

	CString strWPath = strPath;
	CString strTemp;

	if(!PathFileExists(strPath))//文件夹不存在则创建
	{

		strPath.TrimLeft(strPath.Left(3));
		int i = strPath.Find("\\");
		if(i>0)
		{
			strTemp = strWPath.Left(3) + strPath.Left(i);
		}
		else
		{
			strTemp = strWPath;
		}
		strPath.TrimLeft(strPath.Left(i));
		if(!PathFileExists(strTemp))
			CreateDirectory(strTemp,NULL);

		while(strPath.Find("\\") == 0)
		{
			strPath.TrimLeft(strPath.Left(1));
			int j = strPath.Find("\\");
			if(j > 0)
			{
				strTemp = strTemp + "\\" + strPath.Left(j);
				strPath.TrimLeft(strPath.Left(j));
			}
			else
				strTemp = strTemp + "\\" + strPath;
			if(!PathFileExists(strTemp))
				CreateDirectory(strTemp, NULL);
		}
	}
	return TRUE;
}

BOOL CLogicCommon::CreateFile(CString strFileName)
{
	CLogicCommon::CreateDirByPath(strFileName, TRUE);
	CFile file;
	file.Open(strFileName, CFile:: modeCreate);    
	file.Close();
	return TRUE;
}
 