// GetPCResours.h: interface for the CGetPCResours class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETPCRESOURS_H__7543C30D_4E40_4367_84C9_4FCBD11027C5__INCLUDED_)
#define AFX_GETPCRESOURS_H__7543C30D_4E40_4367_84C9_4FCBD11027C5__INCLUDED_
#include "NTTRACE.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGetPCResours  
{
public:
	CGetPCResours(CNTTRACE *pTrace);
	virtual ~CGetPCResours();

	CNTTRACE *m_trace;
	BOOL GetSoftWareInfo(char *filepath);
	BOOL GetHardWareInfo(char *filepath);

};

#endif // !defined(AFX_GETPCRESOURS_H__7543C30D_4E40_4367_84C9_4FCBD11027C5__INCLUDED_)
