// NTAgent3.h : main header file for the NTAGENT3 application
//

#if !defined(AFX_NTAGENT3_H__DA1A0935_CDCD_4C20_B838_8CBE880B92F9__INCLUDED_)
#define AFX_NTAGENT3_H__DA1A0935_CDCD_4C20_B838_8CBE880B92F9__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3App:
// See NTAgent3.cpp for the implementation of this class
//

class CNTAgent3App : public CWinApp
{
public:
	CNTAgent3App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTAgent3App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNTAgent3App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CNTAgent3App theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTAGENT3_H__DA1A0935_CDCD_4C20_B838_8CBE880B92F9__INCLUDED_)
