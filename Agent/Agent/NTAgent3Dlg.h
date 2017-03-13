// NTAgent3Dlg.h : header file
//

#if !defined(AFX_NTAGENT3DLG_H__2E55BC2B_B073_4091_B4CC_5C1DC909119B__INCLUDED_)
#define AFX_NTAGENT3DLG_H__2E55BC2B_B073_4091_B4CC_5C1DC909119B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TrayIcon.h"
#include "Prop0.h"
#include "Prop1.h"
#include "Prop2.h"
#include "Prop3.h"
#include "TabSheet.h"
#include "NTTRACE.h"
#include "NTLogic.h"
/////////////////////////////////////////////////////////////////////////////
// CNTAgent3Dlg dialog

class CNTAgent3Dlg : public CDialog
{
// Construction
public:
	CNTAgent3Dlg(CWnd* pParent = NULL);	// standard constructor
	CNTTRACE *m_trace;
	NTLogic *m_ntlogic;
	CTrayIcon m_trayIcon;

// Dialog Data
	//{{AFX_DATA(CNTAgent3Dlg)
	enum { IDD = IDD_NTAGENT3_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CTabSheet m_sheet;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTAgent3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	HICON m_hIcon;
//	unsigned int uID;
//	CTrayIcon	m_trayIcon(IDI_ICON_NT);

	Prop0 m_prop0;
	Prop1 m_prop1;
	Prop2 m_prop2;
	Prop3 m_prop3;
// Implementation
protected:
	//HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNTAgent3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnOpen();
	afx_msg void OnCancel();
	afx_msg void OnExit();
	afx_msg void OnNcPaint();
	afx_msg void OnBnClickedConfirm();
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTAGENT3DLG_H__2E55BC2B_B073_4091_B4CC_5C1DC909119B__INCLUDED_)
