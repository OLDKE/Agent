// NTAgent3Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "NTAgent3Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3Dlg dialog

CNTAgent3Dlg::CNTAgent3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNTAgent3Dlg::IDD, pParent),m_trayIcon(IDR_TRAYICON)
{
	//{{AFX_DATA_INIT(CNTAgent3Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

//	m_trayIcon = new CTrayIcon(IDR_TRAYICON);
//	char tmpbuf[128];

	m_trace= new CNTTRACE;

	char LogCFG[256];
	int ret=GetNTReg("SOFTWARE\\Ebring\\AGENT","AGENTLOG",LogCFG);
	if(strlen(LogCFG)<= 0)
		strcpy(LogCFG,"c:\\NTLOG.CFG");
	m_trace->GetTraceCFG(LogCFG,LOG_GRP);
	m_trace->ClearLog(LOG_GRP);
	/*m_trace->GetTraceCFG(LogCFG,"EAXFS");
	m_trace->ClearLog("EAXFS");
	m_trace->GetTraceCFG(LogCFG,"CDM");
	m_trace->ClearLog("CDM");
	m_trace->GetTraceCFG(LogCFG,"CIM");
	m_trace->ClearLog("CIM");
	m_trace->GetTraceCFG(LogCFG,"IDC");
	m_trace->ClearLog("IDC");
	m_trace->GetTraceCFG(LogCFG,"PIN");
	m_trace->ClearLog("PIN");
	m_trace->GetTraceCFG(LogCFG,"PTR");
	m_trace->ClearLog("PTR");*/

	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"=====LogLevel=[INFO]======");
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	/*m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	m_trace->WTrace("CDM","CDM.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("CDM","CDM.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("CDM","CDM.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	m_trace->WTrace("CIM","CIM.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("CIM","CIM.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("CIM","CIM.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	m_trace->WTrace("IDC","IDC.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("IDC","IDC.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("IDC","IDC.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	m_trace->WTrace("PIN","PIN.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("PIN","PIN.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("PIN","PIN.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");

	m_trace->WTrace("PTR","PTR.LOG",LT_INFO,"====================LogLevel=[INFO]====================");
	m_trace->WTrace("PTR","PTR.LOG",LT_ERROR,"====================LogLevel=[ERROR]====================");
	m_trace->WTrace("PTR","PTR.LOG",LT_DEBUG,"====================LogLevel=[DEBUG]====================");*/

	m_ntlogic = new NTLogic(m_trace);
	if(m_ntlogic != NULL)
	{
		m_ntlogic->Create(IDD,this);
		m_ntlogic->ShowWindow(SW_HIDE);
	}

}

void CNTAgent3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_sheet);
	//{{AFX_DATA_MAP(CNTAgent3Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNTAgent3Dlg, CDialog)
	//{{AFX_MSG_MAP(CNTAgent3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_COMMAND(IDM_OPEN, CNTAgent3Dlg::OnOpen)
	ON_COMMAND(IDM_CANCEL, CNTAgent3Dlg::OnCancel)
	ON_COMMAND(IDM_EXIT, CNTAgent3Dlg::OnExit)
	ON_BN_CLICKED(IDB_CONFIRM, CNTAgent3Dlg::OnBnClickedConfirm)
	ON_BN_CLICKED(IDB_CANCEL, CNTAgent3Dlg::OnBnClickedCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3Dlg message handlers

BOOL CNTAgent3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_sheet.AddPage(_T("系统信息"), &m_prop0, IDD_PROP0);
	//m_sheet.AddPage(_T("系统常规设置"), &m_prop1, IDD_PROP1);
	//m_sheet.AddPage(_T("系统增强设置"), &m_prop2, IDD_PROP2);
	//m_sheet.AddPage(_T("系统测试"), &m_prop3, IDD_PROP3);
	m_sheet.Show();

	//清空命令标志
	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY","0");
 	//m_ntlogic->StartSer("CopyTrace");
 	//m_ntlogic->StartSer("Trn");	//交易信息//测试屏蔽
 	//m_ntlogic->StartSer("Snd");	//设备状态//测试屏蔽
 	//m_ntlogic->StartSer("RtC");
	m_ntlogic->StartSer("Rev");//测试屏蔽
	m_ntlogic->StartSer("SNR");//冠字号信息

    //m_ntlogic->StartXfs();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNTAgent3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNTAgent3Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNTAgent3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CNTAgent3Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_trayIcon.SetNotificationWnd(this, WM_USER+0);

	//m_trayIcon.SetIcon(IDI_ICON_NT);
	m_trayIcon.SetIcon( MAKEINTRESOURCE(IDI_ICON_NT), "金融自助设备监控客户端");


	return 0;
}

void CNTAgent3Dlg::OnOpen()
{
	//ShowWindow(SW_NORMAL);
}

void CNTAgent3Dlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CNTAgent3Dlg::OnExit()
{
	m_trayIcon.DeleteIcon();
	OnOK();
}

void CNTAgent3Dlg::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnNcPaint()
	static int i = 2; 
	if(i > 0) 
	{ 
		i --; 
		ShowWindow(SW_HIDE); 
	} 
	else 
		CDialog::OnNcPaint(); 
}

void CNTAgent3Dlg::OnBnClickedConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CNTAgent3Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CNTAgent3Dlg::OnDestroy()
{
	CDialog::OnDestroy();
	
	delete m_ntlogic;
	delete m_trace;
//	delete m_trayIcon;
}

