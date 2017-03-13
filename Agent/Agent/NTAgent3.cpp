// NTAgent3.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "NTAgent3Dlg.h"
#include <AFXSOCK.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3App

BEGIN_MESSAGE_MAP(CNTAgent3App, CWinApp)
	//{{AFX_MSG_MAP(CNTAgent3App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3App construction

CNTAgent3App::CNTAgent3App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNTAgent3App object

CNTAgent3App theApp;

/////////////////////////////////////////////////////////////////////////////
// CNTAgent3App initialization

BOOL CNTAgent3App::InitInstance()
{
	AfxOleInit();
	CoInitialize(NULL);
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();


	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//脱壳程序，在正式发布时打开，DEBUG时注释掉
	
	HANDLE mutex;
	mutex=CreateMutex(0,false,"EbringAgent");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		mutex=OpenMutex(MUTEX_ALL_ACCESS,false,"EbringAgent");

	LPWSTR *szArglist;
	int nArgs;
	//int i;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if( NULL == szArglist )
	{
		CloseHandle(mutex);
		return FALSE;
	}
	else
	{
		if(nArgs > 1)
		{
			LocalFree(szArglist);
		}
		else
		{
			char exeFullPath[MAX_PATH];
			char exePath[MAX_PATH];
			char exeTmpFullPath[MAX_PATH];
//			char exeTmpName[MAX_PATH];
//			memset(exeTmpName,0,sizeof(exeTmpName));
//			strcpy(exeTmpName,"NTAgent3_tmp.exe");
			//char exeName[MAX_PATH];
			//char *p;

			GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
			strcpy(exePath, exeFullPath);

			size_t i;
			for(i=strlen(exePath)-1; i>0 && exePath[i]!='\\'; i--);
			//memset(exeName,0,sizeof(exeName));
			//strcpy(exeName,exePath+i+1);
			exePath[i]='\0';

			//p=strstr(exeName,".exe");
			//exeName[p-exeName]='\0';


			//sprintf(exeTmpFullPath,"%s\\%s_tmp.exe",exePath,exeName);
			sprintf(exeTmpFullPath,"%s\\EbringAgent_tmp.exe",exePath);
			//如果有副本在执行，则结束掉
			WaitForSingleObject(mutex,INFINITE);
			////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////
			//Sleep(1000);
			int count=0;
			while(1)
			{
				MyKillProcess("EbringAgent_tmp.exe");
				if(!strcmp(exeFullPath,exeTmpFullPath))
					break;
				if(count > 300)
					break;
				CopyFileA(exeFullPath,exeTmpFullPath,FALSE);
				i=GetLastError();

				//char s[10];
				//sprintf(s,"%d",i);
				//::MessageBox(NULL,s,"info",MB_OK);
				if(i == 0)
				{
					break;
				}
				count++;
				::Sleep(1000);
			}
			sprintf(exeTmpFullPath,"\"%s\\EbringAgent_tmp.exe\" -2",exePath);
			//::MessageBox(NULL,exeTmpFullPath,"note",MB_OK);
			WinExec(exeTmpFullPath, SW_HIDE);
			LocalFree(szArglist);
			ReleaseMutex(mutex);
			CloseHandle(mutex);
			return FALSE;
		}

	}
	CloseHandle(mutex);
	//脱壳程序，在正式发布时打开，DEBUG时注释掉
//end 生成可执行程序的副本  end//////


	//::MessageBox(NULL,"new process","note",MB_OK);
	CNTAgent3Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
