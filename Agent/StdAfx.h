// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0D0BF12E_30C2_4410_9700_AC9B4A2DA14B__INCLUDED_)
#define AFX_STDAFX_H__0D0BF12E_30C2_4410_9700_AC9B4A2DA14B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define WINVER 0x0501

#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <Shlwapi.h>
#include <imagehlp.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "general.h"

/*
#ifdef _DEBUG
#pragma comment(lib,"ZipArchive\\ZipArchive_d.lib")
#else
#pragma comment(lib,"ZipArchive\\ZipArchive.lib")
#endif
*/
#import "MSXML2.dll"
#pragma comment(lib,"ws2_32.lib")
//#import "MFC80.dll"
//#import "MSVCP80.dll"
//#import "MSVCR80.dll"


#define			WM_UDPRecv		WM_USER + 1000   	//收到报文
#define			WM_UPLOG		WM_USER + 1001   	//上传日志
#define			WM_UPDATASOF	WM_USER + 1002   	//软件更新
#define			WM_UPDATAADV	WM_USER + 1003   	//广告更新

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0D0BF12E_30C2_4410_9700_AC9B4A2DA14B__INCLUDED_)
