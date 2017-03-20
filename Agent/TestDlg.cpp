// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "TestDlg.h"


//XFS
#include ".\XFS\XFSADMIN.H"
#include ".\XFS\XFSAPI.H"
#include ".\XFS\XFSCAM.H"
#include ".\XFS\XFSCDM.H"
#include ".\XFS\XFSCHK.H"
#include ".\XFS\Xfsconf.h"
#include ".\XFS\XFSDEP.H"
#include ".\XFS\XFSIDC.H"
#include ".\XFS\XFSPIN.H"
#include ".\XFS\XFSPTR.H"
#include ".\XFS\XFSSIU.H"
#include ".\XFS\XFSSPI.H"
#include ".\XFS\XFSTTU.H"
#include ".\XFS\XFSVDM.H"
#include ".\XFS\XFSCIM.H"
 #include "NTXFS_ALL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers

void CTestDlg::OnOK() 
{
	HRESULT hr;
	WFSVERSION ver;
	hr = WFSStartUp(MANAGE_VERSIONS,&ver);
}
