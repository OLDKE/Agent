// NTLogic.cpp : 实现文件
//

#include "stdafx.h"
#include "NTAgent3.h"
#include "NTLogic.h"

#include "LogicCommon.h"

/************************************************************************/
/* 构造及初始化                                                         */
/************************************************************************/

IMPLEMENT_DYNAMIC(NTLogic, CDialog)
NTLogic::NTLogic(CNTTRACE *pTrace,CWnd* pParent /*=NULL*/)
	: CDialog(NTLogic::IDD, pParent)
{
	m_hInstDLL=NULL;
	m_unzip=NULL;
	m_zip=NULL;
	Ser_Rev=false;

	m_trace=pTrace;
	m_pack = new Pack(pTrace);
	m_packxml = new PackXML(pTrace);
	m_udpser = new CUDPServer(pTrace,this);
	m_tcpser = new CTCPServer(pTrace,this);
	m_sys = new CSysStatus(pTrace);
	m_ftp = new MyFTP(pTrace);
	m_resours = new CGetPCResours(pTrace);
	m_mydes =new CMyDes(pTrace);
//	m_myftp = new MyFTP(pTrace);

	m_hInstDLL=LoadLibrary("ZipDll.dll");
	
	
	if(m_hInstDLL!=NULL)
	{
		if(m_unzip==NULL)
			m_unzip=(UNZIP)GetProcAddress(m_hInstDLL,"Ex_UnZip");
		if(m_zip==NULL)
			m_zip=(ZIP)GetProcAddress(m_hInstDLL,"Ex_Zip");
	}
	else
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "加载动态库ZipDll.dll失败! ERRORCODE=[%d]", GetLastError());
	}

	char szValue[_MAX_PATH];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent","AGENTPATH",szValue);
	strcpy(AgentPath,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	strcpy(AgentNo,szValue);

	memset(szValue,0,sizeof(szValue));
    GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","PACK_TYPE",szValue);
	if (!strcmp(szValue,"XML"))
	{
		PackMsg_Flag=TRUE;
	}
	else
	{
		PackMsg_Flag=FALSE;
	}

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_IP",szValue);
	char sendip[256];
	strcpy(sendip,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_PORT",szValue);
	int sendport=atoi(szValue);

	//RDP IP地址和端口
	char sendiprdp[64] = {0};
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","CLI_IP", szValue);//获取RDP的端口号
	strcpy(sendiprdp, szValue);
	char sport[64] = {0};//用于保存读取的RDP的端口号(字符串格式)
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","RDP_PORT", szValue);//获取RDP的端口号
	int sendportrdp=atoi(szValue);


	m_logicRecvCtrl.m_packxml = m_packxml;
	m_logicRecvCtrl.m_trace = m_trace;
	m_logicRecvCtrl.m_tcpser = m_tcpser;
	m_logicRecvCtrl.m_udpser = m_udpser;

	m_logicRecvFuns.m_ftp = m_ftp;
	m_logicRecvFuns.m_packxml = m_packxml;
	m_logicRecvFuns.m_trace = m_trace;
	m_logicRecvFuns.m_udpser = m_udpser;
	m_logicRecvFuns.m_resours = m_resours;
	m_logicRecvFuns.m_sys = m_sys;
	m_logicRecvFuns.m_zip = m_zip;
	m_logicRecvFuns.m_unzip = m_unzip;

	m_logicRecvDisp.m_packxml = m_packxml;
	m_logicRecvDisp.m_trace = m_trace;
	m_logicRecvDisp.m_ftp = m_ftp;
	m_logicRecvDisp.m_udpser = m_udpser;
	m_logicRecvDisp.m_logicRecvCtrl = &m_logicRecvCtrl;
	m_logicRecvDisp.m_zip = m_zip;
	m_logicRecvDisp.m_unzip = m_unzip;

	m_packxml->m_mydes = m_mydes;


	memset(m_DOWNLOAD_SOF,0,sizeof(m_DOWNLOAD_SOF));
	strcpy(m_DOWNLOAD_SOF,AgentPath);
	strcat(m_DOWNLOAD_SOF,DOWNLOAD_SOF);
    
	memset(m_DOWNLOAD_ADV,0,sizeof(m_DOWNLOAD_ADV));
	strcpy(m_DOWNLOAD_ADV,AgentPath);
	strcat(m_DOWNLOAD_ADV,DOWNLOAD_ADV);
	
	memset(m_DOWNLOAD_AGENT,0,sizeof(m_DOWNLOAD_AGENT));
	strcpy(m_DOWNLOAD_AGENT,AgentPath);
	strcat(m_DOWNLOAD_AGENT,DOWNLOAD_AGENT);
	
    memset(m_UNZIP_PATH,0,sizeof(m_UNZIP_PATH));
	strcpy(m_UNZIP_PATH,AgentPath);
	strcat(m_UNZIP_PATH,UNZIP_PATH);
	
    memset(m_BACKUP_SOF,0,sizeof(m_BACKUP_SOF));
	strcpy(m_BACKUP_SOF,AgentPath);
	strcat(m_BACKUP_SOF,BACKUP_SOF);
    
	memset(m_BACKUP_ADV,0,sizeof(m_BACKUP_ADV));
	strcpy(m_BACKUP_ADV,AgentPath);
	strcat(m_BACKUP_ADV,BACKUP_ADV);
	
	memset(m_BACKUP_AGENT,0,sizeof(m_BACKUP_AGENT));
	strcpy(m_BACKUP_AGENT,AgentPath);
	strcat(m_BACKUP_AGENT,BACKUP_AGENT);
	
	memset(m_UPFILE_PATH,0,sizeof(m_UPFILE_PATH));
	strcpy(m_UPFILE_PATH,AgentPath);
	strcat(m_UPFILE_PATH,UPFILE_PATH);

	m_udpser->SetSendPara(sendip,sendport);
	m_tcpser->SetSendPara(sendiprdp,sendportrdp);
	

	StatTimer=0;
	RtcaTimer=0;
	TranTimer=0;

	strcpy(SendTime,"20080101010101");

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TIME_RPTTRN",szValue);
	if(strlen(szValue)==0)
		strcpy(RptTrnTime,"20080101010101");
	else
		strcpy(RptTrnTime,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TIME_RPTRTC",szValue);
	if(strlen(szValue)==0)
		strcpy(RptRtcTime,"20080101010101");
	else
		strcpy(RptRtcTime,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME",szValue);
	strcpy(RefreshTime,szValue);

	strcpy(RptStaTime,"20080101010101");


	/*
	char dllpath[64];
	memset(dllpath,0,sizeof(dllpath));
	strcpy(dllpath,szValue);
	strcat(dllpath,"\\");
	strcat(dllpath,"ZipDllwedf.dll");
	m_hInstDLL=LoadLibrary(dllpath);
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"加载%s",dllpath);*/
}

NTLogic::~NTLogic()
{
	/*
	HRESULT hr;
	for(int i=0;i<Ntxfs_ptr.LogCount;i++)
	{
		hr=WFSClose(ptr_h_add[i]);
		//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose\t hService\t%d return %d",ptr_h_add[i],hr);
	}
	for(i=0;i<Ntxfs_idc.LogCount;i++)
	{
		hr=WFSClose(idc_h_add[i]);
		//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose\t hService\t%d return %d",idc_h_add[i],hr);
	}
	for(i=0;i<Ntxfs_cdm.LogCount;i++)
	{
		hr=WFSClose(cdm_h_add[i]);
		//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose:\t hService\t%d return %d",cdm_h_add[i],hr);
	}
	for(i=0;i<Ntxfs_pin.LogCount;i++)
	{
		hr=WFSClose(pin_h_add[i]);
		//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose\t hService\t%d return %d",pin_h_add[i],hr);
	}
	for(i=0;i<Ntxfs_cim.LogCount;i++)
	{
		hr=WFSClose(cim_h_add[i]);
		//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose\t hService\t%d return %d",cim_h_add[i],hr);
	}
	hr=WFSCleanUp();
	//m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCleanUp\t  return %d",hr);
	*/
	delete m_pack;
	delete m_packxml;
	delete m_udpser;
	delete m_tcpser;
	delete m_ftp;
//	delete m_mydes;
	this->DestroyWindow();
}

void NTLogic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NTLogic, CDialog)
	ON_WM_TIMER()
	ON_MESSAGE(WM_UDPRecv, OnRecvLogic)  //收到命令后调用 OnRecvLogic函数
	ON_MESSAGE(WM_UPLOG, OnUpLog)
//	ON_MESSAGE(WM_UPDATASOF, OnUpdata_SOF)
//	ON_MESSAGE(WM_UPDATAADV, OnUpdata_ADV)

END_MESSAGE_MAP()

/************************************************************************/
/* 启动各种处理逻辑                                                     */
/************************************************************************/
//代码注释 任康 2017/03/13
//这个函数可以在将来自定义模块加功能的时候使用
bool NTLogic::StartSer(char *Module)
{
	char module_name[32] = {0};
	bool ret = false;
	strcpy(module_name,Module);
	m_trace->WTrace(LOG_GRP, LOG_STEP, LT_INFO, "StartSer module=[%s]", module_name);
	

	if(!strcmp(module_name, "ALL"))
	{
		//启动所有服务
		ret=StartSer_Rev();
		ret=StartSer_Snd();
		ret=StartSer_Uplog();
	}	
	else if(!strcmp(module_name,"Rev"))
	{
		//接收控制命令
		ret=StartSer_Rev();
	}
	else if(!strcmp(module_name,"Snd"))
	{
		m_sys->Initialize(); //初始化系统cpu等利用率
		//发送状态信息
		ret=StartSer_Snd();
	}
	else if(!strcmp(module_name,"Trn"))
	{
		//发送交易信息
		ret=StartSer_Trn();
	}
	else if(!strcmp(module_name,"RtC"))
	{
		//发送吞卡信息
		ret=StartSer_RtCard();
	}
	else if(!strcmp(module_name,"Uplog"))
	{
		//上传日志
		ret=StartSer_Uplog();
	}
	else
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "StartSer module no found. module=[%s]", module_name);
		ret=false;
	}
	
	return ret;
}


/************************************************************************/
/* 接收逻辑                                                             */
/************************************************************************/
bool NTLogic::StartSer_Rev()
{
	//判断接收服务是否已经启动，如果已经启动，不再重复启动。初始为 False
	if(Ser_Rev)
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_INFO, "StartSer_Rev service already started");
		return true;
	}
	
	//判断agent启动接收服务的端口，如果不在100-32768之间，不启动服务
	char tmpval[256] = {0};
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","CLI_PORT",tmpval);
	if((atoi(tmpval)<100) || (atoi(tmpval)>32768))
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "StartSer_Rev port=[%s] error, must in [100-32768]", tmpval);
		return false;
	}
	
	//通过CUDPServer启动UDP服务，用于接收控制命令, 方法中启动线程，接收到报文后通过消息触发OnRecvLogic
	if(false==m_udpser->StartRecv(atoi(tmpval)))
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "StartSer_Rev start udp server error");
		return false;
	}
	
	//标记服务已经启动
	Ser_Rev=true;
	
	return true;
}

//接收到v端发来的控制命令后回调
LRESULT NTLogic::OnRecvLogic(WPARAM wParam, LPARAM lParam)
{
	//收到的报文
	char recvbuf[2048] = {0};
	strcpy(recvbuf, (char *)wParam);

	//发送报文的IP,这里指的是发送命令报文的v端的ip
	char recvip[128] = {0};
	strcpy(recvip, (char *)lParam);

	char sendmsg[2048] = {0};
	//char sendmsg[1024] = {0};
	
	//m_trace->WTrace(LOG_GRP, LOG_STEP, LT_DEBUG, "recv ip=[%s], buf=[%s]", recvip, recvbuf);

	if(strlen(recvbuf)<=4)
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "OnRecvLogic msg length less 4");
		return 0;
	}

	//报文解包，并根据返回码进行处理
	int ret = -1;
	VMSG vmsg;
	memset(&vmsg, 0x0, sizeof(VMSG));

	ret = m_packxml->UnPack(recvbuf, strlen(recvbuf), &vmsg); //对命令报文解包,并根据返回码做相应处理

	m_trace->WTrace(LOG_GRP, LOG_STEP, LT_DEBUG, "OnRecvLogic unpack return=[%d], trantype=[%s], trancode=[%s]", ret, vmsg.TRNTYPE, vmsg.TRNCODE);

	if( -6 == ret )
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "设备号校验错"); //打包命令返回报文
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return 0;
	}
	if (-1 == ret)
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "解包错，不做任何处理");
		return 0;
	} 
	else if( -2 == ret)
	{
		memset(sendmsg, 0, sizeof(sendmsg));
		m_packxml->PackComdRet(sendmsg,vmsg.TRNCODE, RET_FAIL, vmsg.TRNEJ, "有其他命令正在执行");
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		return 0;
	}
	else if( 100 == ret)		//回执报文
	{	
		ret = RespDo(vmsg.RESPTYPE, vmsg.TRNEJ);
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_DEBUG,"RespDo(%s, %s) return=[%d]",vmsg.TRNTYPE, vmsg.TRNEJ, ret);
		return 0;
	}

	//回复服务端命令正在执行
	memset(sendmsg,0,sizeof(sendmsg));
	if( m_packxml->PackComdRet(sendmsg, vmsg.TRNCODE, RET_DOIN, vmsg.TRNEJ, "") )
	{
		m_udpser->SendMsg(sendmsg, strlen(sendmsg));
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_DEBUG, "response server doin...");
	}
	else
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "PackComdRet error");
	}

	//启动线程，开始执行命令
	s_ThreadPara *m_threadpara = new s_ThreadPara;
	m_threadpara->m_ntlogic = this;
	strcpy(m_threadpara->trncode, vmsg.TRNCODE);
	m_threadpara->pvmsg = &vmsg;

	m_cmdrun = CreateThread(NULL, 0, CmdRUN, (LPVOID)m_threadpara, 0, NULL);
	if(m_cmdrun==NULL)
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_ERROR, "CreateThread CmdRUN error!!!");
		return 0;
	}
	else
	{
		m_trace->WTrace(LOG_GRP, LOG_STEP, LT_INFO, "CreateThread CmdRUN success!!!");
	}	
	return 0;
}

 void NTLogic::StartXfs()
 {
 	HANDLE hThread_Xfs=CreateThread(NULL,0,Check_Xfs,this,0,NULL);
 	if(hThread_Xfs==NULL)
 	{
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Xfs  Error !");
 	}
 	else
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Xfs  OK !");
 	CloseHandle(hThread_Xfs);
 }


//FOR XFS
 
 DWORD WINAPI NTLogic::Check_Xfs(LPVOID lpParam)
 {	
 		NTLogic *m_xfs=(NTLogic *)lpParam;
 		if(m_xfs->InItXFSManager())
 		{
 			
 			m_xfs->hThread[0]=CreateThread(NULL,0,Check_Ptr,m_xfs,0,NULL);
 			if(m_xfs->hThread[0]==NULL)
 			{
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Ptr  Error !");
 			}
 			else
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Ptr  OK !");
 			
 			
 
 			//建立 读卡器线程时判断是否是OMRON,是用WOSA否则不建立线程
 			//char szCfgName[256];
 			//char szModuleName[256];
 			//char tmpbuf[64];
 			//memset(szCfgName,0,sizeof(szCfgName));
 			//memset(szModuleName,0,sizeof(szModuleName));
 			//strcpy(szCfgName,"c:\\ntbst\\SIP20.cfg");
 			//strcpy(szModuleName,"CARDREADER");
 			//memset(tmpbuf,0,sizeof(tmpbuf));
 			//GetPrivateProfileStringA(szModuleName,"CardReaderType",NULL,tmpbuf,sizeof(tmpbuf),szCfgName);
 		//if(!strcmp(tmpbuf,"OMRON"))
 			//{
 				m_xfs->hThread[1]=CreateThread(NULL,0,Check_Idc,m_xfs,0,NULL);
 				if(m_xfs->hThread[1]==NULL)
 				{
 					m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Idc  Error !");
 				}
 				else
 					m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Idc  OK !");
 			//}
 			
 			m_xfs->hThread[2]=CreateThread(NULL,0,Check_Cdm,m_xfs,0,NULL);
 			if(m_xfs->hThread[2]==NULL)
 			{
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Cdm  Error !");
 			}
 			else
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Cdm  OK !");
 			
 			
 			m_xfs->hThread[3]=CreateThread(NULL,0,Check_Pin,m_xfs,0,NULL);
 			if(m_xfs->hThread[3]==NULL)
 			{
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Pin  Error !");
 			}
 			else
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Pin  OK !");
 			
 			
 			m_xfs->hThread[4]=CreateThread(NULL,0,Check_Cim,m_xfs,0,NULL);
 			if(m_xfs->hThread[4]==NULL)
 			{
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Cim  Error !");
 			}
 			else
 				m_xfs->m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"CreateThread Check_Cim  OK !");
 			
 			
 		}
		
 		return 0;
 }


 bool NTLogic::InItXFSManager()
 {
 
 	NTXFS_PTR_LOG Myntxfs_ptr_log;
 	NTXFS_IDC_LOG Myntxfs_idc_log;
 	NTXFS_CDM_LOG Myntxfs_cmd_log;
 	NTXFS_PIN_LOG Myntxfs_pin_log;
 	NTXFS_CIM_LOG Myntxfs_cim_log;
 
    WFSRESULT *pwfsResult =NULL; 
 	
 	WFSVERSION v1,v2;
 	
 	
 	TCHAR szValue[_MAX_PATH];
 	LONG lRes=0;
 	
 	int i,LogCount=0;
 	char	tmppath[_MAX_PATH];
 	bool ret=false;
 	char tmptime[64];
 
 	GetNowTime(tmptime);
 	
 	HRESULT hr;
 	WFSVERSION ver;
 	DWORD version = 0x0000ffff;
 	HAPP happ;
 
 
 	hr = -100;
 	hr = WFSStartUp(MANAGE_VERSIONS,&ver);
 	/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSStartUp(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwVersionsRequired: [0x%x],",MANAGE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpWFSVersion: [0x%x],",ver);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",ver.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",ver.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",ver.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",ver.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",ver.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t }");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 	if (( hr!= WFS_SUCCESS ) && ( hr!= WFS_ERR_ALREADY_STARTED))
 	{
 		hr = WFSCleanUp();
 		/**********************log begin***********************/
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCleanUp() return: [%d]",hr);
 		/***********************log end***********************/
 		return false;
 	}		
 	
 	int test=0;
 	int count=0;
 	char lasttime[36];
 	memset(lasttime,0,sizeof(lasttime));
 	while(1)
 	{
 

 		if(ret)
 			break;
//modify by yfpeng 2011.11.14
//  		//通过应用程序的刷新时间判断应用是否启动，如果刷新时间改变三次则视为启动
//  		Sleep(10000);
//  		memset(szValue,0,sizeof(szValue));
//  		lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME",szValue);
//  		if(strcmp(szValue,lasttime))
//  		{
//  			strcpy(lasttime,szValue);
//  			count++;
//  		}
//  
//  		if(count<3)//最好判断超过三次
//  			continue;
//modify by yfpeng 2011.11.14 end

 		//应用程序启动后，尝试打开所有模块，尝试10次，如果都没有成功则退出
 		if(test > 10)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"打开WOSA失败");
 			return false;
 		}
 		test++;
 
 
 
 
 
 
 
 	//PTR
 	memset(szValue,0,sizeof(szValue));
 	lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PTR","NUM",szValue);
 	LogCount=atoi(szValue);
 	Ntxfs_ptr.LogCount=LogCount;
 
 	for(i=0;i<LogCount;i++)
 	{
 		memset(tmppath,0x00,sizeof(tmppath));
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PTR\\%d",i+1);
 		memset(szValue,0,sizeof(szValue));
 		lRes=GetNTReg(tmppath,"LOGNAME",szValue);
 		strcpy(Myntxfs_ptr_log.LogName,szValue);
 		hr=WFSCreateAppHandle(&happ);
 		if(hr != WFS_SUCCESS)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCreateAppHandle() return: [%d]",hr);
 			return false;
 		}
 
 		
 		
 
 		hr = WFSOpen(Myntxfs_ptr_log.LogName,happ,Myntxfs_ptr_log.LogName,NULL,WFS_INDEFINITE_WAIT,SERVICE_VERSIONS,&v1,&v2,&ptr_h_add[i]);
 		/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSOpen(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszLogicalName: [%s],",Myntxfs_ptr_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t hApp: [%d],",happ);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszAppID: [%s],",Myntxfs_ptr_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTraceLevel: [%d],",NULL);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTimeOut: [%d],",WFS_INDEFINITE_WAIT);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwSrvcVersionsRequired: [%x],",SERVICE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSrvcVersion: [0x%x]",v1);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v1.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v1.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v1.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v1.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v1.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSPIVersion: [0x%x]",v2);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v2.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v2.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v2.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v2.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v2.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lphService: [%d],",ptr_h_add[i]);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		
 		
 		if ( hr != WFS_SUCCESS )
 		{
 			strcpy(Ntxfs_ptr.ntxfs_ptr_log[i].DevStat,"7");
 			hr = WFSClose(ptr_h_add[i]);
 		/**********************log begin***********************/
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose() return: [%d]",hr);
 		/***********************log end***********************/
 			continue;	
 		}
 		else
 			ret=true;
 	}
 	//IDC
 	memset(szValue,0,sizeof(szValue));
 	lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\IDC","NUM",szValue);
 	LogCount=atoi(szValue);
 	
 	Ntxfs_idc.LogCount=LogCount;
 	
 	for(i=0;i<LogCount;i++)
 	{
 		memset(tmppath,0x00,sizeof(tmppath));
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\IDC\\%d",i+1);
 		memset(szValue,0,sizeof(szValue));
 		lRes=GetNTReg(tmppath,"LOGNAME",szValue);
 		strcpy(Myntxfs_idc_log.LogName,szValue);
 
 		hr=WFSCreateAppHandle(&happ);
 		if(hr != WFS_SUCCESS)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCreateAppHandle() return: [%d]",hr);
 			return false;
 		}
 
 
 
 		hr = WFSOpen(Myntxfs_idc_log.LogName,happ,Myntxfs_idc_log.LogName,NULL,WFS_INDEFINITE_WAIT,SERVICE_VERSIONS,&v1,&v2,&idc_h_add[i]);
 	/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSOpen(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszLogicalName: [%s],",Myntxfs_idc_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t hApp: [%d],",happ);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszAppID: [%s],",Myntxfs_idc_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTraceLevel: [%d],",NULL);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTimeOut: [%d],",WFS_INDEFINITE_WAIT);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwSrvcVersionsRequired: [%x],",SERVICE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSrvcVersion: [0x%x]",v1);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v1.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v1.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v1.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v1.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v1.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSPIVersion: [0x%x]",v2);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v2.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v2.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v2.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v2.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v2.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lphService: [%d],",idc_h_add[i]);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if ( hr != WFS_SUCCESS )
 		{
 			strcpy(Ntxfs_idc.ntxfs_idc_log[i].DevStat,"7");
 			hr = WFSClose(idc_h_add[i]);
 		/**********************log begin***********************/
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose() return: [%d]",hr);
 		/***********************log end***********************/	
 			continue;	
 		}
 		else
 			ret=true;
 	}
 
 	//CDM
 	memset(szValue,0,sizeof(szValue));
 	lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM","NUM",szValue);
 	LogCount=atoi(szValue);
 	Ntxfs_cdm.LogCount=LogCount;
 	
 	for(i=0;i<LogCount;i++)
 	{
 		memset(tmppath,0x00,sizeof(tmppath));
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\%d",i+1);
 		memset(szValue,0,sizeof(szValue));
 		lRes=GetNTReg(tmppath,"LOGNAME",szValue);
 		strcpy(Myntxfs_cmd_log.LogName,szValue);
 		hr=WFSCreateAppHandle(&happ);
 		if(hr != WFS_SUCCESS)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCreateAppHandle() return: [%d]",hr);
 			return false;
 		}
 
 
 
 		hr = WFSOpen(Myntxfs_cmd_log.LogName,happ,Myntxfs_cmd_log.LogName,NULL,WFS_INDEFINITE_WAIT,SERVICE_VERSIONS,&v1,&v2,&cdm_h_add[i]);
 	/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSOpen(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszLogicalName: [%s],",Myntxfs_cmd_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t hApp: [%d],",happ);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszAppID: [%s],",Myntxfs_cmd_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTraceLevel: [%d],",NULL);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTimeOut: [%d],",WFS_INDEFINITE_WAIT);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwSrvcVersionsRequired: [%x],",SERVICE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSrvcVersion: [0x%x]",v1);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v1.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v1.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v1.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v1.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v1.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSPIVersion: [0x%x]",v2);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v2.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v2.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v2.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v2.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v2.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lphService: [%d],",cdm_h_add[i]);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if ( hr != WFS_SUCCESS )
 		{
 			strcpy(Ntxfs_cdm.ntxfs_cdm_log[i].DevStat,"7");
 			hr = WFSClose(cdm_h_add[i]);	
 		/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose() return: [%d]",hr);
 		/***********************log end***********************/
 			continue;	
 		}
 		else
 			ret=true;
 	}
 	//PIN
 	memset(szValue,0,sizeof(szValue));
 	lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PIN","NUM",szValue);
 	LogCount=atoi(szValue);
 	
 	Ntxfs_pin.LogCount=LogCount;
 	
 	for(i=0;i<LogCount;i++)
 	{
 		memset(tmppath,0x00,sizeof(tmppath));
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PIN\\%d",i+1);
 		memset(szValue,0,sizeof(szValue));
 		lRes=GetNTReg(tmppath,"LOGNAME",szValue);
 		strcpy(Myntxfs_pin_log.LogName,szValue);
 		hr=WFSCreateAppHandle(&happ);
 		if(hr != WFS_SUCCESS)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCreateAppHandle() return: [%d]",hr);
 			return false;
 		}
 		hr = WFSOpen(Myntxfs_pin_log.LogName,happ,Myntxfs_pin_log.LogName,NULL,WFS_INDEFINITE_WAIT,SERVICE_VERSIONS,&v1,&v2,&pin_h_add[i]);		
 	//**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSOpen(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszLogicalName: [%s],",Myntxfs_pin_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t hApp: [%d],",happ);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszAppID: [%s],",Myntxfs_pin_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTraceLevel: [%d],",NULL);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTimeOut: [%d],",WFS_INDEFINITE_WAIT);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwSrvcVersionsRequired: [%x],",SERVICE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSrvcVersion: [0x%x]",v1);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v1.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v1.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v1.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v1.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v1.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSPIVersion: [0x%x]",v2);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v2.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v2.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v2.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v2.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v2.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lphService: [%d],",pin_h_add[i]);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	//***********************log end***********************/
 		if ( hr != WFS_SUCCESS )
 		{
 			strcpy(Ntxfs_pin.ntxfs_pin_log[i].DevStat,"7");
 			hr = WFSClose(pin_h_add[i]);
 		/**********************log begin***********************/
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose() return: [%d]",hr);
 		/***********************log end***********************/
 			continue;	
 		}
 		else
 			ret=true;
 	}
 
 	//CIM
 	memset(szValue,0,sizeof(szValue));
 	lRes=GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM","NUM",szValue);
 	LogCount=atoi(szValue);
 	Ntxfs_cim.LogCount=LogCount;
 	
 	for(i=0;i<LogCount;i++)
 	{
 		memset(tmppath,0x00,sizeof(tmppath));
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\%d",i+1);
 		memset(szValue,0,sizeof(szValue));
 		lRes=GetNTReg(tmppath,"LOGNAME",szValue);
 		strcpy(Myntxfs_cim_log.LogName,szValue);
 		hr=WFSCreateAppHandle(&happ);
 		if(hr != WFS_SUCCESS)
 		{
 			m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSCreateAppHandle() return: [%d]",hr);
 			return false;
 		}
 		
 
 		hr = WFSOpen(Myntxfs_cim_log.LogName,happ,Myntxfs_cim_log.LogName,NULL,WFS_INDEFINITE_WAIT,SERVICE_VERSIONS,&v1,&v2,&cim_h_add[i]);//SERVICE_REQUIRED_VERSIONS
 	/**********************log begin***********************/
 	m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSOpen(");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszLogicalName: [%s],",Myntxfs_cim_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t hApp: [%d],",happ);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpszAppID: [%s],",Myntxfs_cim_log.LogName);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTraceLevel: [%d],",NULL);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwTimeOut: [%d],",WFS_INDEFINITE_WAIT);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t dwSrvcVersionsRequired: [%x],",SERVICE_VERSIONS);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSrvcVersion: [0x%x]",v1);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v1.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v1.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v1.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v1.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v1.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lpSPIVersion: [0x%x]",v2);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t {");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wVersion: [%d]",v2.wVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wLowVersion: [%d]",v2.wLowVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t wHighVersion: [%d]",v2.wHighVersion);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szDescription: [%s]",v2.szDescription);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t\t szSystemStatus: [%s]",v2.szSystemStatus);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t },");
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t lphService: [%d],",cim_h_add[i]);
 	m_trace->WTraceNoTime("EAXFS","EAXFS.LOG",LT_INFO,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if ( hr != WFS_SUCCESS )
 		{
 			strcpy(Ntxfs_cim.ntxfs_cim_log[i].DevStat,"7");
 			hr = WFSClose(cim_h_add[i]);
 		/**********************log begin***********************/
 		m_trace->WTrace("EAXFS","EAXFS.LOG",LT_INFO,"WFSClose() return: [%d]",hr);
 		/***********************log end***********************/
 			continue;	
 		}
 		else
 			ret=true;
 	}
 
 	}
 	return ret;
 }
 
 DWORD WINAPI NTLogic::Check_Ptr(LPVOID lpParam)
 {
 	NTLogic *m_xfs=(NTLogic *)lpParam;
 	m_xfs->Ptr_timer();
 	return 0;
 }

 DWORD WINAPI NTLogic::Check_Idc(LPVOID lpParam)
 {
 	NTLogic *m_xfs=(NTLogic *)lpParam;
 	m_xfs->Idc_timer();
 	return 0;
 }
 
 DWORD WINAPI NTLogic::Check_Cdm(LPVOID lpParam)
 {
 	NTLogic *m_xfs=(NTLogic *)lpParam;
 	m_xfs->Cdm_timer();
 	return 0;
 }
 
 
 DWORD WINAPI NTLogic::Check_Pin(LPVOID lpParam)
 {
 	NTLogic *m_xfs=(NTLogic *)lpParam;
 	m_xfs->Pin_timer();
 	return 0;
 }
 
 DWORD WINAPI NTLogic::Check_Cim(LPVOID lpParam)
 {
 	NTLogic *m_xfs=(NTLogic *)lpParam;
 	m_xfs->Cim_timer();
 	return 0;
 }
 
 
 void NTLogic::Ptr_timer()
 {
 	while(1)
 	{
 		if(XFSStatus_PTR())
 		{
 			if(InXFSStatus_PTR())
 				Updatatime();
 		}
 		Sleep(10000);
 	}
 }
 
 void NTLogic::Idc_timer()
 {
 	while(1)
 	{
 		if(XFSStatus_IDC())
 		{
 			if(InXFSStatus_IDC())
 				Updatatime();
 		}
 		Sleep(10000);
 	}
 }
 
 void NTLogic::Cdm_timer()
 {
 	while(1)
 	{
 		if(XFSStatus_CDM())
 		{
 			if(InXFSStatus_CDM())
 				Updatatime();
 		}
 		Sleep(10000);
 	}
 }
 
 void NTLogic::Pin_timer()
 {
 	while(1)
 	{
 		if(XFSStatus_PIN())
 		{
 			if(InXFSStatus_PIN())
 				Updatatime();
 		}
 		Sleep(10000);
 	}
 }
 
 void NTLogic::Cim_timer()
 {
 	while(1)
 	{
 		if(XFSStatus_CIM())
 		{
 			if(InXFSStatus_CIM())
 				Updatatime();
 		}
 		Sleep(10000);
 	}
 }

 void NTLogic::ConvertErrCode_CIM(char *name, int inerr, char *outerr)
 {
 	if(!strcmp(name,"DEVSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_CIM_DEVONLINE:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CIM_DEVBUSY:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CIM_DEVHWERROR:
 			sprintf(outerr,"3");
 			break;
 		case WFS_CIM_DEVOFFLINE:
 			sprintf(outerr,"7");		//change by xiongjian
 			break;
 		case WFS_CIM_DEVPOWEROFF:
 			sprintf(outerr,"7");		//change by xiongjian
 			break;			
 		case WFS_CIM_DEVNODEVICE:
 			sprintf(outerr,"4");
 			break;			
 		case WFS_CIM_DEVUSERERROR:
 			sprintf(outerr,"3");
 			break;
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"SAFEDOOR"))
 	{
 		switch(inerr)
 		{
 		case WFS_CIM_DOORNOTSUPPORTED:
 			sprintf(outerr,"4");
 			break;
 		case WFS_CIM_DOOROPEN:
 			sprintf(outerr,"1");
 			break;
 		case WFS_CIM_DOORCLOSED:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CIM_DOORUNKNOWN:
 			sprintf(outerr,"7");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"ACCEPTOR"))
 	{
 		switch(inerr)
 		{
 		case WFS_CIM_ACCOK:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CIM_ACCCUSTATE:
 			sprintf(outerr,"1");
 			break;
 		case WFS_CIM_ACCCUSTOP:
 			sprintf(outerr,"2");
 			break;
 		case WFS_CIM_ACCCUUNKNOWN:
 			sprintf(outerr,"7");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"TYPE"))
 	{
 		switch(inerr)
 		{
 		case WFS_CIM_TYPECASHIN://普通存款钞箱
 			sprintf(outerr,"4");
 			break;			
 		case WFS_CIM_TYPEREPCONTAINER:////废钞箱
 			sprintf(outerr,"1");
 			break;
 		case WFS_CIM_TYPERETRACTCASSETTE://回收钞箱
 			sprintf(outerr,"2");
 			break;			
 		case WFS_CIM_TYPERECYCLING://循环钞箱
 			sprintf(outerr,"5");
 			break;
 		default:
 			sprintf(outerr,"0");
 		}
 	}
 	else if(!strcmp(name,"STATUS"))
 	{
 		switch(inerr)
 		{
 		case WFS_CIM_STATCUOK:
 			sprintf(outerr,"0");//正常
 			break;			
 		case WFS_CIM_STATCUHIGH:
 			sprintf(outerr,"1");//钞箱快满
 			break;			
 		case WFS_CIM_STATCUFULL://已满
 			sprintf(outerr,"2");
 			break;
 		//未激活
 		case WFS_CIM_STATCUINOP://故障
 			sprintf(outerr,"3");
 			break;
 		case WFS_CIM_STATCUNOREF://没有初始化（故障）		
 			sprintf(outerr,"3");
 			break;			
  		case WFS_CIM_STATCUMISSING://没有设备
 			sprintf(outerr,"4");
 			break;
 		case WFS_CIM_STATCUNOVAL://钞箱不可用
 			sprintf(outerr,"4");
 			break;			
 		case WFS_CIM_STATCULOW://缺钞
 			sprintf(outerr,"5");
 			break;			
 		case WFS_CIM_STATCUEMPTY://空
 			sprintf(outerr,"6");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 
 }
 
 
 void NTLogic::ConvertErrCode_CDM(char *name, int inerr, char *outerr)
 {
 	if(!strcmp(name,"DEVSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_CDM_DEVONLINE:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CDM_DEVBUSY:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CDM_DEVHWERROR:
 			sprintf(outerr,"3");
 			break;
 		case WFS_CDM_DEVOFFLINE:
 			sprintf(outerr,"7");		//change by xiongjian
 			break;	
 		case WFS_CDM_DEVPOWEROFF:
 			sprintf(outerr,"7");		//change by xiongjian
 			break;			
 		case WFS_CDM_DEVNODEVICE:
 			sprintf(outerr,"4");
 			break;			
 		case WFS_CDM_DEVUSERERROR:
 			sprintf(outerr,"3");
 			break;
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"SAFEDOOR"))
 	{
 		switch(inerr)
 		{
 		case WFS_CDM_DOORNOTSUPPORTED:
 			sprintf(outerr,"4");
 			break;
 		case WFS_CDM_DOOROPEN:
 			sprintf(outerr,"1");
 			break;
 		case WFS_CDM_DOORCLOSED:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CDM_DOORUNKNOWN:
 			sprintf(outerr,"7");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"DISPENSER"))
 	{
 		switch(inerr)
 		{
 		case WFS_CDM_DISPOK:
 			sprintf(outerr,"0");
 			break;
 		case WFS_CDM_DISPCUSTATE:
 			sprintf(outerr,"1");
 			break;
 		case WFS_CDM_DISPCUSTOP:
 			sprintf(outerr,"2");
 			break;
 		case WFS_CDM_DISPCUUNKNOWN:
 			sprintf(outerr,"7");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 	else if(!strcmp(name,"TYPE"))
 	{
 		switch(inerr)
 		{
 		case WFS_CDM_TYPEREJECTCASSETTE:
 			sprintf(outerr,"1");
 			break;			
 		case WFS_CDM_TYPEBILLCASSETTE:
 			sprintf(outerr,"3");
 			break;
 		case WFS_CDM_TYPERETRACTCASSETTE:
 			sprintf(outerr,"2");
 			break;			
 		case WFS_CDM_TYPERECYCLING:
 			sprintf(outerr,"5");
 			break;	
 /*		case WFS_CDM_TYPENA:
 		case WFS_CDM_TYPECOINCYLINDER:
 		case WFS_CDM_TYPECOINDISPENSER:
 		case WFS_CDM_TYPECOUPON:
 		case WFS_CDM_TYPEDOCUMENT:
 	
 			sprintf(outerr,"5");
 			break;*/
 		default:
 			sprintf(outerr,"0");
 		}
 	}
 	else if(!strcmp(name,"STATUS"))
 	{
 		switch(inerr)
 		{
 		case WFS_CDM_STATCUOK:
 			sprintf(outerr,"0");
 			break;			
 		case WFS_CDM_STATCUHIGH:
 			sprintf(outerr,"1");
 			break;			
 		case WFS_CDM_STATCUFULL:
 			sprintf(outerr,"2");
 			break;
 		//未激活
 		case WFS_CDM_STATCUINOP:
 			sprintf(outerr,"3");
 			break;	
 		case WFS_CDM_STATCUNOREF:
 			sprintf(outerr,"3");
 			break;			
  		case WFS_CDM_STATCUMISSING:
 			sprintf(outerr,"4");
 			break;
 		case WFS_CDM_STATCUNOVAL:
 			sprintf(outerr,"4");
 			break;			
 		case WFS_CDM_STATCULOW:
 			sprintf(outerr,"5");
 			break;			
 		case WFS_CDM_STATCUEMPTY:
 			sprintf(outerr,"6");
 			break;			
 		default:
 			sprintf(outerr,"3");
 		}
 	}
 
 }
 void NTLogic::ConvertErrCode_IDC(char *name, int inerr, char *outerr)
 {
 	if(!strcmp(name,"REBINSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_IDC_RETAINBINOK :
 			sprintf(outerr,"0");
 			break ;
 			
 		case WFS_IDC_RETAINNOTSUPP ://change by xiongjian
 			sprintf(outerr,"4");
 			break ;
 			
 		case WFS_IDC_RETAINBINFULL :
 			sprintf(outerr,"2");
 			break ;
 		case WFS_IDC_RETAINBINHIGH :
 			sprintf(outerr,"1");
 			break ;
 			
 		default:
 			sprintf(outerr,"3");
 		}	
 	}
 	else if(!strcmp(name,"DEVSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_IDC_DEVNODEVICE :
 			sprintf(outerr,"4");
 			break ;
 		case WFS_IDC_DEVUSERERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_IDC_DEVHWERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_IDC_DEVONLINE : 
 			sprintf(outerr,"0");
 			break ;
 		case WFS_IDC_DEVOFFLINE :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_IDC_DEVPOWEROFF :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_IDC_DEVBUSY:
 			sprintf(outerr,"0");
 			break ;
 			
 		default:
 			sprintf(outerr,"3");
 		}	
 	}
 	else if(!strcmp(name,"TYPE"))
 	{
 		switch(inerr)
 		{
 		case WFS_IDC_TYPEMOTOR  :
 			sprintf(outerr,"1");
 			break ;
 			
 		case WFS_IDC_TYPESWIPE :
 			sprintf(outerr,"4");
 			break ;
 			
 		case WFS_IDC_TYPEDIP :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_IDC_TYPECONTACTLESS :
 			sprintf(outerr,"2");
 			break ;
 			
 		default:
 			sprintf(outerr,"1");
 		}	
 	}
 }
 void NTLogic::ConvertErrCode_PIN(char *name, int inerr, char *outerr)
 {
 	
 	if(!strcmp(name,"DEVSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_PIN_DEVNODEVICE :
 			sprintf(outerr,"4");
 			break ;
 		case WFS_PIN_DEVUSERERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_PIN_DEVHWERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_PIN_DEVONLINE : 
 			sprintf(outerr,"0");
 			break ;
 		case WFS_PIN_DEVOFFLINE :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_PIN_DEVPOWEROFF :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_PIN_DEVBUSY:
 			sprintf(outerr,"0");
 			break ;
 			
 		default:
 			sprintf(outerr,"3");
 		}	
 	}
 	
 	
 }
 void NTLogic::ConvertErrCode_PTR(char *name, int inerr, char *outerr)
 {
 	
     if(!strcmp(name,"DEVSTAT"))
 	{
 		switch(inerr)
 		{
 		case WFS_PTR_DEVNODEVICE :
 			sprintf(outerr,"4");
 			break ;
 		case WFS_PTR_DEVUSERERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_PTR_DEVHWERROR :
 			sprintf(outerr,"3");
 			break ;
 		case WFS_PTR_DEVONLINE : 
 			sprintf(outerr,"0");
 			break ;
 		case WFS_PTR_DEVOFFLINE :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_PTR_DEVPOWEROFF :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_PTR_DEVBUSY:
 			sprintf(outerr,"0");
 			break ;
 			
 		default:
 			sprintf(outerr,"3");
 		}	
 	}
 	else if(!strcmp(name,"PAPER"))
 	{
 		switch(inerr)
 		{
 		case WFS_PTR_PAPERFULL :
 			sprintf(outerr,"0");
 			break ;
 		case WFS_PTR_PAPERUNKNOWN :
 			sprintf(outerr,"0");
 			break ;		
 		case WFS_PTR_PAPERLOW :
 			sprintf(outerr,"1");
 			break ;			
 		case WFS_PTR_PAPEROUT :
 			sprintf(outerr,"2");
 			break ;
 		case WFS_PTR_PAPERNOTSUPP :
 			sprintf(outerr,"7");
 			break ;
 		case WFS_PTR_PAPERJAMMED:
 			sprintf(outerr,"5");//卡纸状态 modify for 3.0
 			break;
 		default:
 			sprintf(outerr,"7");
 		}	
 	}
 	else if(!strcmp(name,"TYPE"))
 	{
 		switch(inerr)
 		{
 		case WFS_PTR_TYPERECEIPT  :
 			sprintf(outerr,"2");
 			break ;
 			
 		case WFS_PTR_TYPEPASSBOOK :
 			sprintf(outerr,"3");
 			break ;
 			
 		case WFS_PTR_TYPEJOURNAL :
 			sprintf(outerr,"1");
 			break ;
 		case WFS_PTR_TYPEDOCUMENT :
 			sprintf(outerr,"4");
 			break ;
 			
 		default:
 			sprintf(outerr,"6");
 		}	
 	}
 }
void NTLogic::Updatatime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	
	char yyyymmddhhmmss[128];
	memset(yyyymmddhhmmss,0,sizeof(yyyymmddhhmmss));
	sprintf(yyyymmddhhmmss, "%04d%02d%02d%02d%02d%02d",
		st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);	
	
	DWORD size=strlen(yyyymmddhhmmss);
	
	char tmp[256];
	sprintf(tmp,"%s",yyyymmddhhmmss);
	SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME_XFS",tmp);
}


 bool NTLogic::InXFSStatus_PTR()
 {
     int i,result;
 	char tmppath[128];
 	for(i=0;i<Ntxfs_ptr.LogCount;i++)
 	{
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PTR\\%d",i+1);
 		result=SetNTReg(tmppath,"DEVSTAT",Ntxfs_ptr.ntxfs_ptr_log[i].DevStat);
 		result=SetNTReg(tmppath,"DEVTYPE",Ntxfs_ptr.ntxfs_ptr_log[i].DevType);
 		result=SetNTReg(tmppath,"DEVERRCORE",Ntxfs_ptr.ntxfs_ptr_log[i].DevErrCode);
 		
 	}
 	return true;
 	
 
 }
 bool NTLogic::InXFSStatus_IDC()
 {
 	int i,result;
 	char tmppath[128];
 	for(i=0;i<Ntxfs_idc.LogCount;i++)
 	{
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\IDC\\%d",i+1);
 		result=SetNTReg(tmppath,"DEVSTAT",Ntxfs_idc.ntxfs_idc_log[i].DevStat);
 		result=SetNTReg(tmppath,"DEVTYPE",Ntxfs_idc.ntxfs_idc_log[i].DevType);
 		result=SetNTReg(tmppath,"DEVERRCORE",Ntxfs_idc.ntxfs_idc_log[i].DevErrCode);
 		
 	}
 	return true;
 
 
 }
 bool NTLogic::InXFSStatus_CDM()
 {
 	int i,j,result;
 	char tmpbuf[128],tmppath[128],tmppath2[128];
 	for(i=0;i<Ntxfs_cdm.LogCount;i++)
 	{
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\%d",i+1);
 		result=SetNTReg(tmppath,"DEVSTAT",Ntxfs_cdm.ntxfs_cdm_log[i].DevStat);
 		result=SetNTReg(tmppath,"DEVTYPE",Ntxfs_cdm.ntxfs_cdm_log[i].DevType);
 		result=SetNTReg(tmppath,"DEVERRCORE",Ntxfs_cdm.ntxfs_cdm_log[i].DevErrCode);
 		result=SetNTReg(tmppath,"SAFEDOOR",Ntxfs_cdm.ntxfs_cdm_log[i].SafeDoor);
 		
 		result=SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","SAFEDOORSTA",Ntxfs_cdm.ntxfs_cdm_log[i].SafeDoor);
 
 
 
 		result=SetNTReg(tmppath,"DISPENSER",Ntxfs_cdm.ntxfs_cdm_log[i].Dispenser);
 		sprintf(tmpbuf,"%d",Ntxfs_cdm.ntxfs_cdm_log[i].UnitCount);
 		result=SetNTReg(tmppath,"UNITCOUNT",tmpbuf);
 		for(j=0;j<Ntxfs_cdm.ntxfs_cdm_log[i].UnitCount;j++)
 		{
 			sprintf(tmppath2,"%s\\UNIT\\%d",tmppath,j+1);
 			result=SetNTReg(tmppath2,"TYPE",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].Type);
 			result=SetNTReg(tmppath2,"UNITID",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].UnitID);
 			result=SetNTReg(tmppath2,"CURRENCY",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].Currency);
 			result=SetNTReg(tmppath2,"VALUES",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].Values);
 			result=SetNTReg(tmppath2,"COUNT",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].Count);
 			result=SetNTReg(tmppath2,"STATUS",Ntxfs_cdm.ntxfs_cdm_log[i].ntxfs_cdm_unit[j].Status);
 		}
 	}
 	return true;
 }
 bool NTLogic::InXFSStatus_PIN()
 {
      
 	int i,result;
 	char tmppath[128];
 	for(i=0;i<Ntxfs_pin.LogCount;i++)
 	{
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PIN\\%d",i+1);
 		result=SetNTReg(tmppath,"DEVSTAT",Ntxfs_pin.ntxfs_pin_log[i].DevStat);
 		result=SetNTReg(tmppath,"DEVTYPE",Ntxfs_pin.ntxfs_pin_log[i].DevType);
 		result=SetNTReg(tmppath,"DEVERRCORE",Ntxfs_pin.ntxfs_pin_log[i].DevErrCode);
 		
 	}
 	return true;
 
 }
 
 bool NTLogic::InXFSStatus_CIM()
 {
 	int i,j,result;
 	char tmpbuf[128],tmppath[128],tmppath2[128];
 	for(i=0;i<Ntxfs_cim.LogCount;i++)
 	{
 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\%d",i+1);
 		result=SetNTReg(tmppath,"DEVSTAT",Ntxfs_cim.ntxfs_cim_log[i].DevStat);
 		result=SetNTReg(tmppath,"DEVTYPE",Ntxfs_cim.ntxfs_cim_log[i].DevType);
 		result=SetNTReg(tmppath,"DEVERRCORE",Ntxfs_cim.ntxfs_cim_log[i].DevErrCode);
 		result=SetNTReg(tmppath,"SAFEDOOR",Ntxfs_cim.ntxfs_cim_log[i].SafeDoor);
 
 
 		result=SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","SAFEDOORSTA",Ntxfs_cim.ntxfs_cim_log[i].SafeDoor);
 
 
 		result=SetNTReg(tmppath,"ACCEPTOR",Ntxfs_cim.ntxfs_cim_log[i].Acceptor);
 		sprintf(tmpbuf,"%d",Ntxfs_cim.ntxfs_cim_log[i].UnitCount);
 		result=SetNTReg(tmppath,"UNITCOUNT",tmpbuf);
 		for(j=0;j<Ntxfs_cim.ntxfs_cim_log[i].UnitCount;j++)
 		{
 			sprintf(tmppath2,"%s\\UNIT\\%d",tmppath,j+1);
 			result=SetNTReg(tmppath2,"TYPE",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].Type);
 			result=SetNTReg(tmppath2,"UNITID",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].UnitID);
 			result=SetNTReg(tmppath2,"CURRENCY",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].Currency);
 			result=SetNTReg(tmppath2,"VALUES",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].Values);
 			result=SetNTReg(tmppath2,"COUNT",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].Count);
 			result=SetNTReg(tmppath2,"STATUS",Ntxfs_cim.ntxfs_cim_log[i].ntxfs_cim_unit[j].Status);
 		}
 	}
 	return true;
 
 }
 
 
 
 bool NTLogic::XFSStatus_PTR()
 {
 	WFSPTRCAPS   PTRCaps;
 	char tmptime[64];
 	GetNowTime(tmptime);
 	NTXFS_PTR_LOG Myntxfs_ptr_log;
 	char tmpbuf[128];
 	WFSPTRSTATUS PTRStatus;
 	HRESULT hr;
 	WFSRESULT *pwfsResult =NULL;
 
 
 	for(int i=0;i<Ntxfs_ptr.LogCount;i++)
 	{
 		///取打印机类型
 		hr = WFSGetInfo(ptr_h_add[i],WFS_INF_PTR_CAPABILITIES,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("PTR","PTR.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t hService: [%d],",ptr_h_add[i]);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_PTR_CAPABILITIES);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&PTRCaps,pwfsResult->lpBuffer,sizeof(WFSPTRCAPS));	
 	/**********************log begin***********************/
 	m_trace->WTrace("PTR","PTR.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t WFS_INF_PTR_CAPABILITIES");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t {");
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t wClass=[%d]",PTRCaps.wClass);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwType=[%d]",PTRCaps.fwType);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t bCompound=[%d]",PTRCaps.bCompound);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t wResolution=[%d]",PTRCaps.wResolution);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwReadForm=[%d]",PTRCaps.fwReadForm);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwWriteForm=[%d]",PTRCaps.fwWriteForm);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwExtents=[%d]",PTRCaps.fwExtents);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwControl=[%d]",PTRCaps.fwControl);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t usMaxMediaOnStacker=[%d]",PTRCaps.usMaxMediaOnStacker);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t bAcceptMedia=[%d]",PTRCaps.bAcceptMedia);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t bMultiPage=[%d]",PTRCaps.bMultiPage);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwPaperSources=[%d]",PTRCaps.fwPaperSources);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t bMediaTaken=[%d]",PTRCaps.bMediaTaken);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t usRetractBins=[%d]",PTRCaps.usRetractBins);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t lpusMaxRetract=[%s]",PTRCaps.lpusMaxRetract);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwImageType=[%d]",PTRCaps.fwImageType);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwFrontImageColorFormat=[%d]",PTRCaps.fwFrontImageColorFormat);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwBackImageColorFormat=[%d]",PTRCaps.fwBackImageColorFormat);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwCodelineFormat=[%d]",PTRCaps.fwCodelineFormat);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwImageSource=[%d]",PTRCaps.fwImageSource);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwCharSupport=[%d]",PTRCaps.fwCharSupport);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t bDispensePaper=[%d]",PTRCaps.bDispensePaper);
 m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%s]",PTRCaps.lpszExtra);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		hr = WFSFreeResult(pwfsResult);
 		ConvertErrCode_PTR("TYPE",PTRCaps.fwType,tmpbuf);
 		strcpy(Myntxfs_ptr_log.DevType,tmpbuf);
 
 
 		//取打印机状态
 		hr = WFSGetInfo(ptr_h_add[i],WFS_INF_PTR_STATUS,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("PTR","PTR.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t hService: [%d],",ptr_h_add[i]);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_PTR_STATUS);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&PTRStatus,pwfsResult->lpBuffer,sizeof(WFSPTRSTATUS));
 	/**********************log begin***********************/
 	m_trace->WTrace("PTR","PTR.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t WFS_INF_PTR_STATUS");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwDevice=[%d]",PTRStatus.fwDevice);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwMedia=[%d]",PTRStatus.fwMedia);
 	for(int k=0;k<WFS_PTR_SUPPLYUPPER;k++)
 	{
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwPaper[k]=[%d]",PTRStatus.fwPaper[k]);
 	}
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwToner=[%d]",PTRStatus.fwToner);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwInk=[%d]",PTRStatus.fwInk);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t fwLamp=[%d]",PTRStatus.fwLamp);
 
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t lppRetractBins");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t {");
 				LPWFSPTRRETRACTBINS * lppRetractBins = PTRStatus.lppRetractBins;
 				WFSPTRRETRACTBINS m_RetractBins[20];
 				int j = 0;
 				memset(m_RetractBins, 0, sizeof(m_RetractBins));
 				if(lppRetractBins != NULL)
 				{
 					while(lppRetractBins[j] != NULL)
 					{
 						memcpy(&m_RetractBins[j], lppRetractBins[j], sizeof(WFSPTRRETRACTBINS));
 						
 						m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t\t RetractBins[%d]",j);
 						m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t\t wRetractBin=[%d]",m_RetractBins[j].wRetractBin);
 						m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t\t usRetractCount=[%d]",m_RetractBins[j].usRetractCount);
 						j++;
 					}
 				}
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t }");
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t usMediaOnStacker=[%d]",PTRStatus.usMediaOnStacker);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%d]",PTRStatus.lpszExtra);
 	m_trace->WTraceNoTime("PTR","PTR.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		ConvertErrCode_PTR("DEVSTAT",PTRStatus.fwDevice,tmpbuf);
 		sprintf(Myntxfs_ptr_log.DevErrCode,"%04d",PTRStatus.fwDevice);
 		
 		if(!strcmp(tmpbuf,"0") && strcmp(Myntxfs_ptr_log.DevType,"3"))                 //打印机状态为正常时，再取纸的状态
 		{
 			ConvertErrCode_PTR("PAPER",PTRStatus.fwPaper[WFS_PTR_SUPPLYUPPER],tmpbuf);//WFS_PTR_SUPPLYSIZE
 		}
 		strcpy(Myntxfs_ptr_log.DevStat,tmpbuf);
 		strcpy(Myntxfs_ptr_log.DevStattime,tmptime);
 		hr = WFSFreeResult(pwfsResult);
 		
 		
 
 
 		memcpy(&Ntxfs_ptr.ntxfs_ptr_log[i],&Myntxfs_ptr_log,sizeof(NTXFS_PTR_LOG));
 	}
 	return true;
 }
 
 bool NTLogic::XFSStatus_IDC()
 {
 	WFSIDCCAPS   IDCCaps;
 	NTXFS_IDC_LOG Myntxfs_idc_log;
 	char tmptime[64];
 	GetNowTime(tmptime);
 	char tmpbuf[128];
 	WFSIDCSTATUS IDCStatus;
 	HRESULT hr;
 	WFSRESULT *pwfsResult =NULL;
 	for(int i=0;i<Ntxfs_idc.LogCount;i++)
 	{
 		hr = WFSGetInfo(idc_h_add[i],WFS_INF_IDC_STATUS,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("IDC","IDC.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t hService: [%d],",idc_h_add[i]);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_IDC_STATUS);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&IDCStatus,pwfsResult->lpBuffer,sizeof(WFSIDCSTATUS));					
 	/**********************log begin***********************/
 	m_trace->WTrace("IDC","IDC.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t WFS_INF_IDC_STATUS");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwDevice=[%d]",IDCStatus.fwDevice);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwMedia=[%d]",IDCStatus.fwMedia);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwRetainBin=[%d]",IDCStatus.fwRetainBin);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwSecurity=[%d]",IDCStatus.fwSecurity);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwChipPower=[%d]",IDCStatus.fwChipPower);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%s]",IDCStatus.lpszExtra);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		ConvertErrCode_IDC("DEVSTAT",IDCStatus.fwDevice,tmpbuf);	
 		sprintf(Myntxfs_idc_log.DevErrCode,"%04d",IDCStatus.fwDevice);
 		if(!strcmp(tmpbuf,"0"))                 ///卡箱状态
 			ConvertErrCode_IDC("REBINSTAT",IDCStatus.fwRetainBin,tmpbuf);
 		strcpy(Myntxfs_idc_log.DevStat,tmpbuf);
 		strcpy(Myntxfs_idc_log.DevStattime,tmptime);
 		hr = WFSFreeResult(pwfsResult);
 		///取读卡器类型
 		hr = WFSGetInfo(idc_h_add[i],WFS_INF_IDC_CAPABILITIES,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("IDC","IDC.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t hService: [%d],",idc_h_add[i]);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_IDC_CAPABILITIES);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&IDCCaps,pwfsResult->lpBuffer,sizeof(WFSIDCCAPS));	
 	/**********************log begin***********************/
 	m_trace->WTrace("IDC","IDC.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t WFS_INF_IDC_CAPABILITIES");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t wClass=[%d]",IDCCaps.wClass);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwType=[%d]",IDCCaps.fwType);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t bCompound=[%d]",IDCCaps.bCompound);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwReadTracks=[%d]",IDCCaps.fwReadTracks);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwWriteTracks=[%d]",IDCCaps.fwWriteTracks);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwChipProtocols=[%d]",IDCCaps.fwChipProtocols);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t usCards=[%d]",IDCCaps.usCards);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwSecType=[%d]",IDCCaps.fwSecType);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwPowerOnOption=[%d]",IDCCaps.fwPowerOnOption);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwPowerOffOption=[%d]",IDCCaps.fwPowerOffOption);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t bFluxSensorProgrammable=[%d]",IDCCaps.bFluxSensorProgrammable);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t bReadWriteAccessFollowingEject=[%d]",IDCCaps.bReadWriteAccessFollowingEject);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwWriteMode=[%d]",IDCCaps.fwWriteMode);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t fwChipPower=[%d]",IDCCaps.fwChipPower);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%d]",IDCCaps.lpszExtra);
 	m_trace->WTraceNoTime("IDC","IDC.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		hr = WFSFreeResult(pwfsResult);
 		ConvertErrCode_IDC("TYPE",IDCCaps.fwType,tmpbuf);
 		strcpy(Myntxfs_idc_log.DevType,tmpbuf);		
 		memcpy(&Ntxfs_idc.ntxfs_idc_log[i],&Myntxfs_idc_log,sizeof(NTXFS_IDC_LOG));
 	}
 	return true;
 }
 
 bool NTLogic::XFSStatus_CDM()
 {
 	WFSCDMCASHUNIT CMDCashunit;
 	WFSCDMSTATUS CDMStatus;
 	WFSCDMCUINFO CDMCuinfo;
 	WFSRESULT *pwfsResult =NULL; 
 	HRESULT hr;
 	NTXFS_CDM_LOG Myntxfs_cmd_log;
 	NTXFS_CDM_UNIT Myntxfs_cdm_unit;
 	LONG lRes=0;
 	char tmpbuf[128];
 	int j = 0;
 
 
 	char tmptime[64];
 	GetNowTime(tmptime);
 	for(int i=0;i<Ntxfs_cdm.LogCount;i++)
 	{	
 		hr = WFSGetInfo(cdm_h_add[i],WFS_INF_CDM_STATUS,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("CDM","CDM.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t hService: [%d],",cdm_h_add[i]);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_CDM_STATUS);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&CDMStatus,pwfsResult->lpBuffer,sizeof(WFSCDMSTATUS));	
 	/**********************log begin***********************/
 	m_trace->WTrace("CDM","CDM.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t WFS_INF_CDM_STATUS");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t fwDevice=[%d]",CDMStatus.fwDevice);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t fwSafeDoor=[%d]",CDMStatus.fwSafeDoor);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t fwDispenser=[%d]",CDMStatus.fwDispenser);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t fwIntermediateStacker=[%d]",CDMStatus.fwIntermediateStacker);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t lppPositions");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t {");
 				LPWFSCDMOUTPOS * lppPositions = CDMStatus.lppPositions;
 				WFSCDMOUTPOS m_Positions[20];	
 				memset(m_Positions, 0, sizeof(m_Positions));
 				if(lppPositions != NULL)
 				{
 					while(lppPositions[j] != NULL)
 					{
 						memcpy(&m_Positions[j], lppPositions[j], sizeof(WFSCDMOUTPOS));					
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t Positions[%d]",j);
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t fwPosition=[%d]",m_Positions[j].fwPosition);
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t fwShutter=[%d]",m_Positions[j].fwShutter);
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t fwPositionStatus=[%d]",m_Positions[j].fwPositionStatus);
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t fwTransport=[%d]",m_Positions[j].fwTransport);
 						m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t fwTransportStatus=[%d]",m_Positions[j].fwTransportStatus);
 						j++;
 					}
 				}
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t }");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%s]",CDMStatus.lpszExtra);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		hr = WFSFreeResult(pwfsResult);		
 
 		strcpy(Myntxfs_cmd_log.DevType,"1");
 		sprintf(Myntxfs_cmd_log.DevErrCode,"%04d",CDMStatus.fwDevice);
 		ConvertErrCode_CDM("DEVSTAT",CDMStatus.fwDevice,tmpbuf);
 		strcpy(Myntxfs_cmd_log.DevStat,tmpbuf);
 		strcpy(Myntxfs_cmd_log.DevStattime,tmptime);
 		ConvertErrCode_CDM("SAFEDOOR",CDMStatus.fwSafeDoor,tmpbuf);
 		strcpy(Myntxfs_cmd_log.SafeDoor,tmpbuf);
 		strcpy(Myntxfs_cmd_log.SafeDoortime,tmptime);				
 		ConvertErrCode_CDM("DISPENSER",CDMStatus.fwDispenser,tmpbuf);
 		strcpy(Myntxfs_cmd_log.Dispenser,tmpbuf);
 		strcpy(Myntxfs_cmd_log.Dispensertime,tmptime);			
 
 		hr = WFSGetInfo(cdm_h_add[i],WFS_INF_CDM_CASH_UNIT_INFO,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("CDM","CDM.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t hService: [%d],",cdm_h_add[i]);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_CDM_CASH_UNIT_INFO);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&CDMCuinfo,pwfsResult->lpBuffer,sizeof(WFSCDMCUINFO)) ;
 		Myntxfs_cmd_log.UnitCount=CDMCuinfo.usCount;
 		for( j=0;j<CDMCuinfo.usCount;j++)
 		{
 		/**********************log begin***********************/
 	m_trace->WTrace("CDM","CDM.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t WFS_INF_CDM_CASH_UNIT_INFO");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t lppList[%d]",j);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t {");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t usType=[%d]",CDMCuinfo.lppList[j]->usType);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t cUnitID=[%d]",CDMCuinfo.lppList[j]->cUnitID);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t cCurrencyID=[%d]",CDMCuinfo.lppList[j]->cCurrencyID);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t ulValues=[%d]",CDMCuinfo.lppList[j]->ulValues);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t usStatus=[%d]",CDMCuinfo.lppList[j]->usStatus);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t\t ulCount=[%d]",CDMCuinfo.lppList[j]->ulCount);
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t\t }");
 	m_trace->WTraceNoTime("CDM","CDM.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 			memset(&Myntxfs_cdm_unit,0x00,sizeof(NTXFS_CDM_UNIT));
 		    memcpy(&CMDCashunit,CDMCuinfo.lppList[j],sizeof(WFSCDMCASHUNIT));
 
 			ConvertErrCode_CDM("TYPE",CMDCashunit.usType,tmpbuf);			
 			strcpy(Myntxfs_cdm_unit.Type,tmpbuf);
 			memcpy(Myntxfs_cdm_unit.UnitID,CMDCashunit.cUnitID,5);
 			memcpy(Myntxfs_cdm_unit.Currency,CMDCashunit.cCurrencyID,3);			
 			sprintf(tmpbuf,"%d",CMDCashunit.ulValues);
 			strcpy(Myntxfs_cdm_unit.Values,tmpbuf);
 			sprintf(tmpbuf,"%d",CMDCashunit.ulCount);
 			strcpy(Myntxfs_cdm_unit.Count,tmpbuf);
 			ConvertErrCode_CDM("STATUS",CMDCashunit.usStatus,tmpbuf);
 			strcpy(Myntxfs_cdm_unit.Status,tmpbuf);
 			strcpy(Myntxfs_cdm_unit.Statustime,tmptime);			
 
 			memcpy(&Myntxfs_cmd_log.ntxfs_cdm_unit[j],&Myntxfs_cdm_unit,sizeof(NTXFS_CDM_UNIT));
         }
 	
 		memcpy(&Ntxfs_cdm.ntxfs_cdm_log[i],&Myntxfs_cmd_log,sizeof(NTXFS_CDM_LOG));
 
 		hr = WFSFreeResult(pwfsResult);
 	}
 	return true;
 }
 
 bool NTLogic::XFSStatus_PIN()
 {
 	WFSPINSTATUS PINStatus;
 	NTXFS_PIN_LOG Myntxfs_pin_log;
     WFSRESULT *pwfsResult =NULL; 
 	LONG lRes=0;
 	int i,LogCount=0;
 	char tmpbuf[128];
 	
 	char tmptime[64];
 	GetNowTime(tmptime);
 	
 	HRESULT hr;		
 	
 	for(i=0;i<Ntxfs_pin.LogCount;i++)
 	{
 		hr = WFSGetInfo(pin_h_add[i],WFS_INF_PIN_STATUS,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("PIN","PIN.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t hService: [%d],",pin_h_add[i]);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_PIN_STATUS);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{
 			continue;
 		}
 		memcpy(&PINStatus,pwfsResult->lpBuffer,sizeof(WFSPINSTATUS));	
 	/**********************log begin***********************/
 	m_trace->WTrace("PIN","PIN.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t WFS_INF_PIN_STATUS");
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t\t fwDevice=[%d]",PINStatus.fwDevice);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t\t fwEncStat=[%d]",PINStatus.fwEncStat);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%s]",PINStatus.lpszExtra);
 	m_trace->WTraceNoTime("PIN","PIN.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		ConvertErrCode_PIN("DEVSTAT",PINStatus.fwDevice,tmpbuf);
 		sprintf(Myntxfs_pin_log.DevErrCode,"%04d",PINStatus.fwDevice);
 		strcpy(Myntxfs_pin_log.DevType,"1");
 		strcpy(Myntxfs_pin_log.DevStat,tmpbuf);
 		strcpy(Myntxfs_pin_log.DevStattime,tmptime);					
 		hr = WFSFreeResult(pwfsResult);		
 		memcpy(&Ntxfs_pin.ntxfs_pin_log[i],&Myntxfs_pin_log,sizeof(NTXFS_PIN_LOG));
 	}
 	return true;
 }
 
 bool NTLogic::XFSStatus_CIM()
 {
 	WFSCIMCASHIN CIMCashin;
 	WFSCIMCASHINFO CIMCashinfo;	
 	WFSCIMSTATUS CIMStatus;
 	NTXFS_CIM_LOG Myntxfs_cim_log;
 	NTXFS_CIM_UNIT Myntxfs_cim_unit;
     WFSRESULT *pwfsResult =NULL; 
 	int j=0;
 	LONG lRes=0;
 	int i,LogCount=0;
 	char tmpbuf[128];
 	HRESULT hr;
 
 	for(i=0;i<Ntxfs_cim.LogCount;i++)
 	{
 	
 		hr = WFSGetInfo(cim_h_add[i],WFS_INF_CIM_STATUS,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("CIM","CIM.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t hService: [%d],",cim_h_add[i]);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_CIM_STATUS);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{			
 			continue;
 		}
 		memcpy(&CIMStatus,pwfsResult->lpBuffer,sizeof(WFSCIMSTATUS));		
 	/**********************log begin***********************/
 	m_trace->WTrace("CIM","CIM.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t WFS_INF_CIM_STATUS");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwDevice=[%d]",CIMStatus.fwDevice);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwSafeDoor=[%d]",CIMStatus.fwSafeDoor);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwAcceptor=[%d]",CIMStatus.fwAcceptor);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwIntermediateStacker=[%d]",CIMStatus.fwIntermediateStacker);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwStackerItems=[%d]",CIMStatus.fwStackerItems);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t fwBanknoteReader=[%d]",CIMStatus.fwBanknoteReader);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t bDropBox=[%d]",CIMStatus.bDropBox);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t lppPositions");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t {");
 				LPWFSCIMINPOS * lppPositions = CIMStatus.lppPositions;
 				WFSCIMINPOS m_Positions[20];
 				int j = 0;
 				memset(m_Positions, 0, sizeof(m_Positions));
 				if(lppPositions != NULL)
 				{
 					while(lppPositions[j] != NULL)
 					{
 						memcpy(&m_Positions[j], lppPositions[j], sizeof(WFSCIMINPOS));
 						
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t Positions[%d]",j);
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwPosition=[%d]",m_Positions[j].fwPosition);
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwShutter=[%d]",m_Positions[j].fwShutter);
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwPositionStatus=[%d]",m_Positions[j].fwPositionStatus);
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwTransport=[%d]",m_Positions[j].fwTransport);
 						m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwTransportStatus=[%d]",m_Positions[j].fwTransportStatus);
 						j++;
 					}
 				}
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t }");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t lpszExtra=[%s]",CIMStatus.lpszExtra);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 		hr = WFSFreeResult(pwfsResult);
 
 		ConvertErrCode_CIM("DEVSTAT",CIMStatus.fwDevice,tmpbuf);
 		strcpy(Myntxfs_cim_log.DevStat,tmpbuf);
 		strcpy(Myntxfs_cim_log.DevType,"1");
 		sprintf(Myntxfs_cim_log.DevErrCode,"%04d",CIMStatus.fwDevice);
 
 		ConvertErrCode_CIM("SAFEDOOR",CIMStatus.fwSafeDoor,tmpbuf);
 		strcpy(Myntxfs_cim_log.SafeDoor,tmpbuf);
 
 		ConvertErrCode_CIM("ACCEPTOR",CIMStatus.fwAcceptor,tmpbuf);
 		strcpy(Myntxfs_cim_log.Acceptor,tmpbuf);
 		
 		hr = WFSGetInfo(cim_h_add[i],WFS_INF_CIM_CASH_UNIT_INFO,NULL,3000,&pwfsResult) ;
 	/**********************log begin***********************/
 	m_trace->WTrace("CIM","CIM.LOG",LT_DEBUG,"WFSGetInfo(");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t hService: [%d],",cim_h_add[i]);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t dwCategory: [%d],",WFS_INF_CIM_CASH_UNIT_INFO);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t lpQueryDetails: [%s],",NULL);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t dwTimeOut: [%d],",3000);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t lppResult: [%d],",pwfsResult);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t ) return: [%d]",hr);
 	/***********************log end***********************/
 		if(hr != WFS_SUCCESS)
 		{			
 			continue;
 		}
 		memcpy(&CIMCashinfo,pwfsResult->lpBuffer,sizeof(WFSCIMCASHINFO)) ;
 		Myntxfs_cim_log.UnitCount=CIMCashinfo.usCount;
 		for(j=0;j<CIMCashinfo.usCount;j++)
 		{
 	/**********************log begin***********************/
 	m_trace->WTrace("CIM","CIM.LOG",LT_DEBUG,"WFSGetInfo");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t WFS_INF_CIM_CASH_UNIT_INFO");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t {");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t lppCashIn[%d]",j);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t {");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t fwType=[%d]",CIMCashinfo.lppCashIn[j]->fwType);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t cUnitID=[%d]",CIMCashinfo.lppCashIn[j]->cUnitID);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t cCurrencyID=[%d]",CIMCashinfo.lppCashIn[j]->cCurrencyID);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t ulValues=[%d]",CIMCashinfo.lppCashIn[j]->ulValues);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t\t usStatus=[%d]",CIMCashinfo.lppCashIn[j]->usStatus);
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t\t }");
 	m_trace->WTraceNoTime("CIM","CIM.LOG",LT_DEBUG,"\t\t }");
 	/***********************log end***********************/
 			memset(&Myntxfs_cim_unit,0x00,sizeof(NTXFS_CIM_UNIT));
 		    memcpy(&CIMCashin,CIMCashinfo.lppCashIn[j],sizeof(WFSCIMCASHIN));
 
 			ConvertErrCode_CIM("TYPE",CIMCashin.fwType,tmpbuf);
 			strcpy(Myntxfs_cim_unit.Type,tmpbuf);
 			memcpy(Myntxfs_cim_unit.UnitID,CIMCashin.cUnitID,5);
 			memcpy(Myntxfs_cim_unit.Currency,CIMCashin.cCurrencyID,3);			
 			sprintf(tmpbuf,"%d",CIMCashin.ulValues);
 			strcpy(Myntxfs_cim_unit.Values,tmpbuf);
 			sprintf(tmpbuf,"%d",CIMCashin.ulCount);
 			strcpy(Myntxfs_cim_unit.Count,tmpbuf);
 			ConvertErrCode_CIM("STATUS",CIMCashin.usStatus,tmpbuf);
 			strcpy(Myntxfs_cim_unit.Status,tmpbuf);
 			memcpy(&Myntxfs_cim_log.ntxfs_cim_unit[j],&Myntxfs_cim_unit,sizeof(NTXFS_CIM_UNIT));
         }
 		memcpy(&Ntxfs_cim.ntxfs_cim_log[i],&Myntxfs_cim_log,sizeof(NTXFS_CIM_LOG));
 
 		hr = WFSFreeResult(pwfsResult);
 	}
 	return true;
 }
 

//开启发送报文的定时器，每隔20秒发送一次 STATTIMER_EVENT 信号
bool NTLogic::StartSer_Snd()
{
	StatTimer=SetTimer(STATTIMER_EVENT,20000,NULL);
	if(StatTimer == 0)
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer STATTIMER  fail !");
	else
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer STATTIMER  success !");

	Sleep(200);

	return true;
}
//开启交易处理额定时器，默认每20秒发送一次 TRANTIMER_EVENT 信号
bool NTLogic::StartSer_Trn()
{

	//20秒发送一次交易60000
	TranTimer=SetTimer(TRANTIMER_EVENT, 20000, NULL);
	if(TranTimer == 0)
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer TranTimer  fail !");
	else
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer TranTimer  success !");

	Sleep(200);
	return true;
}

//开启吞卡处理的定时器，默认每20秒发送一次 RTCARDTIMER_EVENT 信号
bool NTLogic::StartSer_RtCard()
{

	//20秒发送一次吞卡120000
	RtcaTimer=SetTimer(RTCARDTIMER_EVENT, 20000, NULL);
	if(RtcaTimer == 0)
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer RtCardTimer  fail !");
	else
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer RtCardTimer  success !");

	Sleep(200);
	return true;
}

//开启上传日志的定时器，默认每119秒处理一次 59 信号
bool NTLogic::StartSer_Uplog()
{
	if(Ser_Uplog)
		return true;
	SetTimer(59,119000,NULL);
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CreateTimer Service_Uplog  success !");
	Ser_Uplog=true;
	return true;
}

//定时器回调处理函数
void NTLogic::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == STATTIMER_EVENT)		//如果是发送状态报文的信号
	{
		TimerSend_Stat();	//只发送状态信息？
	}
	else if(nIDEvent == RTCARDTIMER_EVENT)		//如果是发送吞卡报文的信号
	{
		TimerSend_RtCard();	//发送吞卡报文
	}
	else if(nIDEvent == TRANTIMER_EVENT)	//如果是发送交易信息的信号
	{
		TimerSend_Tran();	//发送交易信息
	}


	CDialog::OnTimer(nIDEvent);
}

//检查发送报文的状态，由发送报文处理函数内部调用

bool NTLogic::CheckSndStat()
{
	char szValue[_MAX_PATH];

	char nowtime[256];
	GetNowTime(nowtime); //获取当前时间

	if(oneflag==0)
	{
		oneflag = 1;

	}
	char RefreshTime_now[256];
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME",RefreshTime_now);
	if((strcmp(RefreshTime_now,"20070101010101")<0) || (strcmp(RefreshTime_now,"20200101010101")>0))
	{
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"效验DevStat\\ETC\\REFTIME错误，状态不发送");
		return false;
	}

/*	if(strcmp(RefreshTime_now,RefreshTime))
	{
		strcpy(RefreshTime,RefreshTime_now);
		strcpy(RptStaTime,nowtime);
		return true;
	}*/

	char RespFlag[256];
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","RESP_FLAG",RespFlag);
	if(!strcmp(RespFlag,"1")) //如果当前有报文需要（回执）发送？
	{
		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat","RESP",szValue);	//如果当前有状态报文没有发送
		if(!strcmp(szValue,"0"))
		{
			strcpy(RptStaTime,nowtime);//将本次状态报文发送时间，计入成员变量
			return true;	//返回true表示校验通过，需要发送状态报文
		}
	}
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","TIMER",szValue);
	int Ctimer=atoi(szValue);
	if((Ctimer < 29) || (Ctimer > 601))
		Ctimer=590;
	int timeamount=TimeCount(RptStaTime,nowtime);
	if(timeamount>=Ctimer) //如果上次发送状态报文的时间间隔，大于agent的定时发送间隔，表示需要上送报文，返回true校验通过。
	{
		strcpy(RptStaTime,nowtime);
		return true;
	}
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"上次上送时间[%s],CheckSndStat [%d]<[%d]效验false，状态不发送",RptStaTime,timeamount,Ctimer);
	return false;
}
bool NTLogic::CheckSndComd()
{
	return false;
}

bool NTLogic::CheckSndTran()
{
	return false;
}

//由定时器回调，调用处理发送状态报文的信号
void NTLogic::TimerSend_Stat()
{
	if(!CheckSndStat()) //检查发送状态
		return;

	char SendMsg[6000];
	memset(SendMsg,0,sizeof(SendMsg));
	int SendMsgLen=0;

	if(!PackStat(SendMsg,&SendMsgLen))	//打包状态报文
	{
		m_trace->error("PackStat return fail,not send the stat msg");	
		return;
	}
	//将发送过后的状态报文，回执标志改为未发送，以便在下一次继续发送？
	SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat","RESP","0");
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_DEBUG,"TimerSend_Stat,发送前数据:%s",SendMsg);
	if(!m_udpser->SendMsg(SendMsg,SendMsgLen))
	{
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Stat Send fail");
		return;
	}

//	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"3**********");
	//回执机制待确定
	//可以加到CheckSndStat里面，若没有收到回执，下次检查

}

void NTLogic::TimerSend_Comd()
{

	if(!CheckSndComd())
		return;

	char SendMsg[4096];
	memset(SendMsg,0,sizeof(SendMsg));
	int SendMsgLen=0;

	if(!m_pack->PackComd(SendMsg,&SendMsgLen))
	{
		m_trace->error("PackComd return fail,not send the stat msg");	
		return;
	}
    int sendret=m_udpser->SendMsg(SendMsg,SendMsgLen);
	if(sendret==SOCKET_ERROR || sendret<SendMsgLen)
	{
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_DEBUG,"Comd Send fail,return=[%d]",sendret);
		return;
	}

	//回执机制待确定
	//可以加到CheckSndStat里面，若没有收到回执，下次检查

}

//发送交易信息，由定时器回调
void NTLogic::TimerSend_Tran()
{
	char SendMsg[4096];
	int SendMsgLen=0;
	memset(SendMsg,0,sizeof(SendMsg));
	/*char RptTime[256];
	memset(RptTime,0,sizeof(RptTime));
	strcpy(RptTime,RptTrnTime);
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The RptTime is [%s],RptTrnTime is [%s]",RptTime,RptTrnTime);*/

	//char RespFlag[256];
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","RESP_FLAG",RespFlag);
	//if(!strcmp(RespFlag,"1"))
	//{
	//	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Chk_TRN_Resp");
	//	Chk_TRN_Resp();
	//}
	int pos = 0;	//返回打包的位置，此位置在注册表中，相当于读写者模式中的共享仓库的下标
	//打包交易信息，使用udp进行发送。
	while(PackTRN(pos,SendMsg,&SendMsgLen)) //如果队列中还有没发送的报文（resp 等于0），则一直发送，直到返回false，队列里没有交易报文要发送为止
	{
		if(!m_udpser->SendMsg(SendMsg,SendMsgLen))
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Tran Send fail");
			return;
		}
		//发送成功后，设置共享队列pos制定的交易报文发送标志，进行下一个交易报文的发送
		char tmpbuf[_MAX_PATH];
		memset(tmpbuf,0x00,sizeof(tmpbuf));
		sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d", pos);
		SetNTReg(tmpbuf,"RESP", "1"); //讲交易队列里已发送的交易，发送标志设置为1
		memset(SendMsg,0,sizeof(SendMsg));
		SendMsgLen=0;	
	/*	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The RptTime is[%s]",RptTime);
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TIME_RPTTRN",RptTime);*/
	}
	//strcpy(RptTrnTime,RptTime);
	//回执机制待确定
	//可以加到CheckSndStat里面，若没有收到回执，下次检查
}

void NTLogic::TimerSend_RtCard()
{
	char SendMsg[4096];
	int SendMsgLen=0;
	memset(SendMsg,0,sizeof(SendMsg));
	char RptTime[256];
	strcpy(RptTime,RptRtcTime);

	char RespFlag[256];
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","RESP_FLAG",RespFlag);
	if(!strcmp(RespFlag,"1"))
	{
		Chk_RTC_Resp();
	}
	//while(PackRTC(RptTime,SendMsg,&SendMsgLen))
	//{

	//	if(!m_udpser->SendMsg(SendMsg,strlen(SendMsg)))
	//	{
	//		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"RtCard Send fail");
	//		return;
	//	}		
	//	memset(SendMsg,0,sizeof(SendMsg));
	//	SendMsgLen=0;		
	//	SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TIME_RPTRTC",RptTime);
	//}
	strcpy(RptRtcTime,RptTime);
}

BOOL NTLogic::Upsystime()
{
	char systime[256];
	SYSTEMTIME st;
	GetLocalTime(&st);
	char YY[5];
	char MM[5];
	char DD[5];
	char hh[5];
	char mm[5];
	char ss[5];

	memset(YY,0,sizeof(YY));
	memset(MM,0,sizeof(MM));
	memset(DD,0,sizeof(DD));
	memset(hh,0,sizeof(hh));
	memset(mm,0,sizeof(mm));
	memset(ss,0,sizeof(ss));

	memset(systime,0,sizeof(systime));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","PTIME",systime);
	
	if(strlen(systime)<14)
		return FALSE;

	memcpy(YY,systime,4);
	memcpy(MM,systime+4,2);
	memcpy(DD,systime+4+2,2);
	memcpy(hh,systime+4+2+2,2);
	memcpy(mm,systime+4+2+2+2,2);
	memcpy(ss,systime+4+2+2+2+2,2);

	st.wYear = atoi(YY);
	if(st.wYear>2050 ||st.wYear <1990)
		return FALSE;
	
	st.wMonth = atoi(MM);
	if(st.wMonth>12 ||st.wMonth <1)
		return FALSE;
	
	st.wDay = atoi(DD);
	if(st.wDay>31 ||st.wDay <1)
		return FALSE;
	
	st.wHour = atoi(hh); 
	if(st.wHour>23 ||st.wHour <0)
		return FALSE;

	st.wMinute = atoi(mm);
	if(st.wMinute>59 ||st.wMinute <0)
		return FALSE;

	st.wSecond = atoi(ss);
	if(st.wSecond>59 ||st.wSecond <0)
		return FALSE;

	SetLocalTime(&st);
	return TRUE;
}

bool NTLogic::CheckUpVer_SOF()
{
	char ver_c[_MAX_PATH];
	memset(ver_c,0,sizeof(ver_c));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","SOF_VER",ver_c);

	char ver_p[_MAX_PATH];
	memset(ver_p,0,sizeof(ver_p));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\VER","SOF_VER",ver_p);

	if(strcmp(ver_p,ver_c) != 0)
		return true;
	return false;
}

bool NTLogic::CheckUpVer_ADV()
{
	char ver_c[_MAX_PATH];
	memset(ver_c,0,sizeof(ver_c));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","ADV_VER",ver_c);

	char ver_p[_MAX_PATH];
	memset(ver_p,0,sizeof(ver_p));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\VER","ADV_VER",ver_p);

	if(strcmp(ver_p,ver_c) != 0)
		return true;
	return false;
}

LRESULT NTLogic::OnUpLog(WPARAM wParam, LPARAM lParam)
{
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"UPLOG BEGIN");








	return 0;
}
/*
LRESULT NTLogic::OnUpdata_SOF(WPARAM wParam, LPARAM lParam)
{
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"UPDATA_SOF BEGIN");

	CString remotefile,localfile;
	char sendmsg[4096];
	char tmpbuf[256];
	char sofpathp[256];
	memset(sofpathp,0,sizeof(sofpathp));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_P",sofpathp);
	strcpy_s(sofpathp,sizeof(sofpathp),tmpbuf);

	memset(tmpbuf,0,sizeof(tmpbuf));
	SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_C",tmpbuf);
	char sofpathc[256];
	strcpy_s(sofpathc,sizeof(sofpathc),tmpbuf);

	memset(tmpbuf,0,sizeof(tmpbuf));
	SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_NAME_P",tmpbuf);

	try
	{
		remotefile.Format(_T("%s%s"),sofpathp,tmpbuf);
		localfile.Format(_T("%s%s"),sofpathc,tmpbuf);

		if(!MyFTPOpen())
			throw(-1);
		if(!m_myftp->GetFile(remotefile,localfile))
			throw(-2);

	}
	catch (CException* e)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_pack->PackComdRet(sendmsg,"60009","4",0);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));
		
	}
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"UPDATA_SOF END!!");

	return 0;
}
LRESULT NTLogic::OnUpdata_ADV(WPARAM wParam, LPARAM lParam)
{
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"UPDATA_ADV BEGIN");
	CString remotefile,localfile;
	char sendmsg[4096];
	char tmpbuf[256];
	char sofpathp[256];
	memset(sofpathp,0,sizeof(sofpathp));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_P",sofpathp);
	strcpy_s(sofpathp,sizeof(sofpathp),tmpbuf);

	memset(tmpbuf,0,sizeof(tmpbuf));
	SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_C",tmpbuf);
	char sofpathc[256];
	strcpy_s(sofpathc,sizeof(sofpathc),tmpbuf);

	memset(tmpbuf,0,sizeof(tmpbuf));
	SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_NAME_P",tmpbuf);

	try
	{
		remotefile.Format(_T("%s%s"),sofpathp,tmpbuf);
		localfile.Format(_T("%s%s"),sofpathc,tmpbuf);

		if(!MyFTPOpen())
			throw(-1);
		if(!m_myftp->GetFile(remotefile,localfile))
			throw(-2);

	}
	catch (CException* e)
	{
		memset(sendmsg,0,sizeof(sendmsg));
		m_pack->PackComdRet(sendmsg,"60009","4",0);
		m_udpser->SendMsg(sendmsg,strlen(sendmsg));

	}
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"UPDATA_ADV END!!");
	return 0;
}

*/
//命令执行的线程,由接收到命令报文后接收处理逻辑调用
DWORD WINAPI NTLogic::CmdRUN(LPVOID lpParameter)
{
	s_ThreadPara * m_threadpara=(s_ThreadPara *)lpParameter;
	char trncode[32] = {0};
	strcpy(trncode, m_threadpara->trncode);
	NTLogic* m_ntlogic=(NTLogic *)m_threadpara->m_ntlogic;
	PVMSG pvmsg = (PVMSG)m_threadpara->pvmsg;

	int waitnum=0,waitmax=500;
	char sendmsg[2048];
	int ret = -1;

	//设置有命令正在执行
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "1");

	if(!strcmp(trncode,"60002"))	//启动应用
	{
		ret = m_ntlogic->m_logicRecvCtrl.StartApp(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);
	}
	else if(!strcmp(trncode,"60003"))	//暂停服务
	{
		ret = m_ntlogic->m_logicRecvCtrl.PauseApp(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);//根据命令执行的结果，做相应的返回报文处理
	}
	else if(!strcmp(trncode,"60004"))	//重启设备
	{
		ret = m_ntlogic->m_logicRecvCtrl.ResetOS(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);
	}
	else if(!strcmp(trncode,"60005"))	//强制重启设备
	{
		ret = m_ntlogic->m_logicRecvCtrl.ResetOSForc(*pvmsg);
	}
	else if(!strcmp(trncode,"60006"))	//设备关机
	{
		ret = m_ntlogic->m_logicRecvCtrl.CloseOS(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);
	}
	else if(!strcmp(trncode,"60007"))	//强制设备关机
	{
		ret = m_ntlogic->m_logicRecvCtrl.CloseOSForc(*pvmsg);
	}
	else if(!strcmp(trncode,"60008"))	//同步设备时间
	{
		ret = m_ntlogic->m_logicRecvCtrl.SyncTime(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);
	}
	else if(!strcmp(trncode,"60030"))	//同步设备状态
	{
		ret = m_ntlogic->m_logicRecvCtrl.SyncState(*pvmsg);
	}
	else if(!strcmp(trncode,"60099"))	//清除命令执行状态
	{
		ret = m_ntlogic->m_logicRecvCtrl.ClearHostCmd(*pvmsg);
		m_ntlogic->m_logicRecvCtrl.DoRet(ret, *pvmsg);
	}
	else if(!strcmp(trncode,"60009"))	//上传文件
	{
		ret = m_ntlogic->m_logicRecvFuns.UploadFile(*pvmsg);
	}
	else if(!strcmp(trncode,"60014"))   //上传目录明细
	{
		ret = m_ntlogic->m_logicRecvFuns.UploadDirInfo(*pvmsg);
	}
	else if (!strcmp(trncode,"60019"))//获取系统运行状态
	{
		ret = m_ntlogic->m_logicRecvFuns.UploadSysState(*pvmsg);
	}
	else if (!strcmp(trncode,"60023"))//获取C端安装硬件信息
	{
		ret = m_ntlogic->m_logicRecvFuns.UploadHardwareInfo(*pvmsg);
	}
	else if (!strcmp(trncode,"60024"))//获取C端安装软件信息
	{
		ret = m_ntlogic->m_logicRecvFuns.UploadSoftwareInfo(*pvmsg);
	}
	else if (!strcmp(trncode,"60025"))//结束系统进程
	{
		ret = m_ntlogic->m_logicRecvFuns.StopProcess(*pvmsg);
	}
	else if( (!strcmp(trncode,"60010")) || (!strcmp(trncode,"60011")) )	//软件更新
	{
		ret = m_ntlogic->m_logicRecvDisp.CommonDisp(*pvmsg);
	}
	else if( (!strcmp(trncode,"60012")) || (!strcmp(trncode,"60013")) )	//广告更新
	{
		ret = m_ntlogic->m_logicRecvDisp.CommonDisp(*pvmsg);
	}
	else if (!strcmp(trncode,"60018")) //Agent升级
	{
		ret = m_ntlogic->m_logicRecvDisp.CommonDisp(*pvmsg);
	}

	//清除有命令正在执行的标志
	SetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", "0");

	delete m_threadpara;
	m_threadpara = NULL;

	return 0;
}


bool NTLogic::RespDo(char *resptype,char *respej)
{
	char szValue[_MAX_PATH];
	char tmpbuf[_MAX_PATH];

	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"RespDo.......");

	if(!strcmp(resptype,"TRAN"))
	{
		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TRN","TRNNUM",szValue);
		int maxtrnnum=atoi(szValue);
		for(int i=1;i<=maxtrnnum;i++)
		{
			sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d",i);
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,tmpbuf);
			memset(szValue,0,sizeof(szValue));
			//GetNTReg(tmpbuf,"TRNEJ_C",szValue);
			GetNTReg(tmpbuf,"TRNEJ",szValue);
			if(!strcmp(respej,szValue))
			{
				SetNTReg(tmpbuf,"RESP","1");
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"TRAN RESP set 1");
			}
		}
	}
	else if(!strcmp(resptype,"RTCA"))
	{
		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\RTC","RTCNUM",szValue);
		int maxtrnnum=atoi(szValue);
		for(int i=1;i<=maxtrnnum;i++)
		{
			sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\RTC\\%d",i);
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmpbuf,"TRNEJ",szValue);
			if(!strcmp(respej,szValue))
				SetNTReg(tmpbuf,"RESP","1");
		}
	}
	else if(!strcmp(resptype,"STAT"))
	{
		SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat","RESP","1");
	}
	else 
		return false;
	return true;
}

bool NTLogic::Chk_TRN_Resp()
{
	char szValue[_MAX_PATH];
	char szValue_T[MAX_PATH];
	char tmpbuf[_MAX_PATH];
	char SendMsg[4096];
	int SendMsgLen=0;

	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TRN","TRNNUM",szValue);
	int maxtrnnum=atoi(szValue);
	for(int i=1;i<=maxtrnnum;i++)
	{
		sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmpbuf,"RESP",szValue);
		memset(szValue_T,0,sizeof(szValue_T));
		GetNTReg(tmpbuf,"REPORTTIME",szValue_T);
		if((!strcmp(szValue,"0")||strlen(szValue)==0)&&strlen(szValue_T)!=0)
			if(PackTRN_resp(i,SendMsg,&SendMsgLen))
			{				
				if(!m_udpser->SendMsg(SendMsg,strlen(SendMsg)))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Tran Send fail");
					return false;
				}
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Tran Send OK");
				Sleep(2000);
			}
	}
	return true;
}
bool NTLogic::Chk_RTC_Resp()
{
	char szValue[_MAX_PATH];
	char szValue_T[MAX_PATH];
	char tmpbuf[_MAX_PATH];
	char SendMsg[4096];
	int SendMsgLen=0;


	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\RTC","RTCNUM",szValue);
	int maxtrnnum=atoi(szValue);
	for(int i=1;i<=maxtrnnum;i++)
	{
		sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\RTC\\%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmpbuf,"RESP",szValue);
		memset(szValue_T,0,sizeof(szValue_T));
		GetNTReg(tmpbuf,"REPORTTIME",szValue_T);
		if((!strcmp(szValue,"0")||strlen(szValue)==0)&&strlen(szValue_T)!=0)
			if(PackRTC_resp(i,SendMsg,&SendMsgLen))
			{
				if(!m_udpser->SendMsg(SendMsg,strlen(SendMsg)))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"Tran Send fail");
					return false;
				}
				Sleep(2000);
			}
	}
	return true;
}
int NTLogic::CopyFile_EX(char *filefrom,char *fileto,char *filefail)
{
	char szValue[_MAX_PATH];
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue,filefrom);
	
	char msg[256];
	memset(msg,0,sizeof(msg));
	
	char err[_MAX_PATH];
	memset(err,0,sizeof(err));
	int count=0;
	int errNo=0;
	if(strlen(szValue)!=0)
	{
		char propara[256];
		memset(propara,0,sizeof(propara));
		strcpy(propara,szValue);
		
		char t[256];
		memset(t,0,sizeof(t));
		char* tmp=t;
		char* filewithpath=t;
		filewithpath=strtok(propara,"~");
		strcpy(tmp,propara);
		char rootpath[5];
		memset(rootpath,0,sizeof(rootpath));
		strncpy(rootpath,tmp,2);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The filewithpath is [%s]",filewithpath);

		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The tmp is [%s]",tmp);

		rootpath[2]='\0';
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The rootpath is [%s]",rootpath);

		/*		strcpy(fileto_path,rootpath);*/
		while (1)
		{
			if(filewithpath == NULL )
				break;
			
			tmp = tmp + strlen(filewithpath)+2;
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The filewithpath is[%s]!",filewithpath);

			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The tmp is %d!",tmp);
			count++;
			char *filename=t;
			filename=strrchr(filewithpath,'\\');
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The filename is[%s]!",filename);

			if(filename == NULL)
			{
				filewithpath=strtok(NULL,"~");
				continue;
			}
			filename++;
			
			char filewithpathto[256];
			sprintf(filewithpathto,"%s\\%s",rootpath,filename);

			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"The filewithpathto is %s",filewithpathto);

			BOOL bret=CopyFile(filewithpath,filewithpathto,FALSE);
			if (bret==FALSE)
			{
				errNo++;
				strcat(filefail,filewithpath);
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CopyFile (filefrom=[%s],fileto=[%s]) failed!",filewithpath,filewithpathto);
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"CopyFile error,  errno=[%d]，errNo=[%d]",GetLastError(),errNo);
			}
			else
			{
				strcat(fileto,filewithpathto);
				strcat(msg,filewithpath);	
			}			
			::Sleep(500);
			if (strlen(tmp)==0)
			{
				break;
			}
			strcat(fileto,"~");
			filewithpath = strtok(tmp,"~");
		}	
		if (count==errNo)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"copyfile_ex(%s,%s) failed",filefrom,fileto);
			return -1;			
		}
		if (errNo!=0&&count>errNo)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"copyfile_ex(%s,%s) success",msg,fileto);
			return 1;
		}
		if (errNo==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"copyfile_ex(%s,%s) success",filefrom,fileto);
			return 0;
		}
		
	}
	return 0;
    
}
bool NTLogic::DeletFile_EX(char *filename)
{
	char szValue[_MAX_PATH];
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue,filename);
	
	char err[_MAX_PATH];
	memset(err,0,sizeof(err));
	char t[256];
	memset(t,0,sizeof(t));
	int count=0;
	int errNo=0;
	if(strlen(szValue)!=0)
	{
		char propara[256];
		memset(propara,0,sizeof(propara));
		strcpy(propara,szValue);
		
		char* filewithpath=t;
		filewithpath=strtok(propara,"~");
		char* tmpbuf=propara;
		char rootpath[5];
		strncpy(rootpath,tmpbuf,2);
		rootpath[2]='\0';
		while (1)
		{
			if(filewithpath == NULL )
				break;
			
			tmpbuf = tmpbuf + strlen(filewithpath)+1;
			count++;
			
			BOOL bret=DeleteFile(filewithpath);
			
			if (bret==FALSE)
			{
				errNo++;
			}
			
			::Sleep(500);
			if (strlen(tmpbuf)==0)
			{
				break;
			}
			filewithpath = strtok(tmpbuf,"~");
		}	
		if (count==errNo)
		{
			return false;
			
		}
		if (errNo!=0&&count>errNo)
		{
			return false;
		}
		if (errNo==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"deletefile(%s) success",filename);
			return true;
		}
		return true;
	}
	return false;
    
}

int NTLogic::ProcessKillEx(char *ProcessName,char * errmsg)
{
	char szValue[_MAX_PATH];
	memset(szValue,0,sizeof(szValue));
	strcpy(szValue,ProcessName);
	
	char err[_MAX_PATH];
	memset(err,0,sizeof(err));
	int count=0;
	int errNo=0;
	if(strlen(szValue)!=0)
	{
		char propara[256];
		memset(propara,0,sizeof(propara));
		strcpy(propara,szValue);
		
		char t[256];
		memset(t,0,sizeof(t));
		char *process=t;
		process=strtok(propara,"~");
		char *tmpbuf=t;
		strcpy(tmpbuf,propara);
		while (1)
		{
			if(process == NULL )
				break;
			tmpbuf = tmpbuf + strlen(process)+1;
			count++;
			int bret=MyKillProcess(process);
			if (bret!=0)
			{
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"停止进程[%s]失败！",process);
				strcat(err,process);
				strcat(err," ");
				errNo++;
			}
			
			::Sleep(500);
			if (strlen(tmpbuf)==0)
			{
				break;
			}
			process = strtok(tmpbuf,"~");
		}	
		if (count==errNo && count!=0)
		{
			strcpy(errmsg,err);
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"停止所有进程[%s]失败",errmsg);
			return -2;
		}
		else if (errNo!=0&&count>errNo)
		{
			strcpy(errmsg,err);
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"停止部分进程[%s]失败",errmsg);
			return -1;
		}
		else if (errNo==0 && count!=0)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"停止所有进程成功");
			return 0;
		}
		else
			return -1;
		
	}
	else
	{
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"进程名为空!");
		return -3;
	}
}



void NTLogic::tongpei(char * msgbuf,char *tongpeifu)
{
	char *tmpbuf;
	char tmpmsg[2048];
	char nowtime[64];
	GetNowTime(nowtime);
	int tplen=0,i=0,j=0;

	while(1) 
	{	
		tmpbuf=strstr(msgbuf,tongpeifu);
		if(tmpbuf == NULL)
			break;
		memcpy(tmpmsg,msgbuf,tmpbuf - msgbuf);
		if(!strcmp(tongpeifu,"$YYYY"))
		{
			tplen=4;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime,tplen);
		}
		else if(!strcmp(tongpeifu,"$YY"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+2,tplen);
		}
		else if(!strcmp(tongpeifu,"$MM"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+4,tplen);
		}
		else if(!strcmp(tongpeifu,"$DD"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+6,tplen);
		}
		else if(!strcmp(tongpeifu,"$hh"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+8,tplen);
		}
		else if(!strcmp(tongpeifu,"$mm"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+10,tplen);
		}
		else if(!strcmp(tongpeifu,"$ss"))
		{
			tplen=2;
			memcpy(tmpmsg+(tmpbuf - msgbuf),nowtime+12,tplen);
		}
		else if(!strcmp(tongpeifu,"$DEVID"))
		{
			char szValue[_MAX_PATH];
			if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue))
				return;

			tplen=strlen(szValue);
			memcpy(tmpmsg+(tmpbuf - msgbuf),szValue,tplen);
		}
		memcpy(tmpmsg + (tmpbuf - msgbuf) + tplen,tmpbuf+strlen(tongpeifu),strlen(tmpbuf)-strlen(tongpeifu));
		tmpmsg[strlen(msgbuf)-strlen(tongpeifu)+tplen]='\0';
		strcpy(msgbuf,tmpmsg);
	}
}


bool NTLogic::PackRTC(char* rpttime,char *PackStr, int *PackLen)
{
	bool bret=false;
	char msg[3072];
	memset(msg,0,sizeof(msg));
	if (PackMsg_Flag==TRUE)
	{
		bret=m_packxml->PackRTC(rpttime,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
	else
	{
		bret=m_pack->PackRTC(rpttime,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
}

bool NTLogic::PackRTC_resp(int RTCIndex, char *PackStr, int *PackLen)
{
	bool bret=false;
	char msg[3072];
	memset(msg,0,sizeof(msg));
	if (PackMsg_Flag==TRUE)
	{
		bret=m_packxml->PackRTC_resp(RTCIndex,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
	else
	{
		bret=m_pack->PackRTC_resp(RTCIndex,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
}

//打包交易报文
/*******
*参数 出参 pos 注册表 SOFTWARE\Ebring\Agent\TRN\1, 2, 3....下的位置
*参数 出参 交易报文
*参数 出参 报文长度
*******/
bool NTLogic::PackTRN(int &pos, char *PackStr, int *PackLen)
{
	bool bret=false;
	char msg[2048];
	memset(msg,0,sizeof(msg));
	//注册表SOFTWARE\Ebring\Agent\Config\PACK_TYPE里设置的 xml打包标志， ‘XML’时为真
	if (PackMsg_Flag==TRUE) 
	{
		//这里使用的是xml打包，交易队列有要处理的报文返回 true，没有返回false
		bret=m_packxml->PackTRN(pos,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
	else
	{
		//bret=m_pack->PackTRN(pos,msg,PackLen);
		//strcpy(PackStr,msg);
		return bret;
	}
}

bool NTLogic::PackTRN_resp(int TranIndex, char *PackStr, int *PackLen)
{
	bool bret=false;
	char msg[5120];
	memset(msg,0,sizeof(msg));
	if (PackMsg_Flag==TRUE)
	{		
		bret=m_packxml->PackTRN_resp(TranIndex,msg,PackLen);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"PackTRN_resp,接收组报后数组长度[%d],报文长度[%d],报文为 %s",sizeof(msg),strlen(msg),msg);
		strcpy(PackStr,msg);
		return bret;
	}
	else
	{
		bret=m_pack->PackTRN_resp(TranIndex,msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
}
//打包状态报文信息，由定时器回调
bool NTLogic::PackStat(char *PackStr, int *PackLen)
{
	bool bret=false;
	char msg[6000];

	memset(msg,0,sizeof(msg));
	if (PackMsg_Flag==TRUE)		//如果使用的是xml报文 
	{		
		bret=m_packxml->PackStat(msg,PackLen);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_DEBUG,"PackStat中,为其分配数组长度[%d]",sizeof(msg));
		strcpy(PackStr,msg);
		return bret;
	}
	else
	{
		bret=m_pack->PackStat(msg,PackLen);
		strcpy(PackStr,msg);
		return bret;
	}
}
// 
// bool NTLogic::PackComdRet(char *PackStr, char *Trncode, char *Comd_Res, int TranEJflag,char *ErrMsg)
// {
// 	bool bret=false;
// 	char msg[2048];
// 	memset(msg,0,sizeof(msg));
// 	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_DEBUG,"PackMsg_Flag=[%d]",PackMsg_Flag);
// 	if (PackMsg_Flag==TRUE)
// 	{		
// 		bret=m_packxml->PackComdRet(msg,Trncode,Comd_Res,"",ErrMsg);
// 		strcpy(PackStr,msg);
// 		return bret;
// 	}
// 	else
// 	{
// 		strcat(Comd_Res,":");
// 		strcat(Comd_Res,ErrMsg);
// 		bret=m_pack->PackComdRet(msg,Trncode,Comd_Res,TranEJflag);
// 		strcpy(PackStr,msg);
// 		return bret;
// 	}
// }
// bool NTLogic::PackComdRet2(char *PackStr, char *Trncode, char *Comd_Res, int TranEJflag)
// {
// 	bool bret=false;
// 	char msg[2048];
// 	memset(msg,0,sizeof(msg));
// 	if (PackMsg_Flag==TRUE)
// 	{		
// 		bret=m_packxml->PackComdRet2(msg,Trncode,Comd_Res,TranEJflag);
// 		strcpy(PackStr,msg);
// 		return bret;
// 	}
// 	else
// 	{
// 		bret=m_pack->PackComdRet(msg,Trncode,Comd_Res,TranEJflag);
// 		strcpy(PackStr,msg);
// 		return bret;
// 	}
// }

void NTLogic::StartAgent()
{
	char exeFullPath[MAX_PATH];
	char exePath[MAX_PATH];
	char exeTmpFullPath[MAX_PATH];
	int iret=0;
	GetModuleFileNameA(NULL,exeFullPath,MAX_PATH);
	strcpy(exePath,exeFullPath);	
	size_t i;
	for(i=strlen(exePath)-1; i>0 && exePath[i]!='\\'; i--);
	exePath[i]='\0';
	sprintf(exeTmpFullPath,"%s\\Agent.exe",exePath);
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"start new agent");
	WinExec(exeTmpFullPath, SW_HIDE);
}

int NTLogic::ControlApp(char *cmd, char *currpath)
{
	char oldcurrentdir[256];
	GetCurrentDirectory(256,oldcurrentdir);
	::SetCurrentDirectory(currpath);
	m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"SetCurrentDirectory:[%s]",currpath);
	int iret=-1;	
	BOOL bret=false;
	
	char cmd1[512];
	memset(cmd1,0,sizeof(cmd1));
	strcpy(cmd1,cmd);
	strcat(cmd1," >");
	strcat(cmd1,currpath);
	strcat(cmd1,"cmdret.txt");
	char filename[256];
	strcpy(filename,currpath);
	strcat(filename,"cmdret.txt");
	char ApplicationName[512];
	strcpy(ApplicationName,currpath);
	strcat(ApplicationName,"\\cmd.bat");
	DeleteFile(ApplicationName);
	FILE* file1;
	try
	{
		file1=fopen(ApplicationName,"a+");
		if(file1==NULL)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"创建批处理文件失败!: %u\n", GetLastError() );
			throw (-1);
			
		}
		int len1=0;
		len1=fwrite(cmd1,sizeof(char),strlen(cmd1),file1);
		fclose(file1);
		if(len1==0)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"写入批处理命令失败!: %d\n", GetLastError() );			
			throw (-1);
			
		}
		STARTUPINFO   si;   
		memset(&si,0,sizeof(si));   
		si.dwFlags   =   STARTF_USESHOWWINDOW;   
		si.wShowWindow   =   SW_HIDE;   
		
		PROCESS_INFORMATION pinfo; 
		
        bret=CreateProcess(ApplicationName,NULL,NULL,NULL,1,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pinfo);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"创建程序%s",ApplicationName);
//		bret=CreateProcess(ApplicationName,NULL,NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,NULL,&info,&pinfo);
		if(!bret)
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"创建程序%s 失败，返回码%d",ApplicationName,GetLastError());
			throw (-1);
		}
		::Sleep(2000);
		DWORD pid=pinfo.dwProcessId;
		HANDLE process=OpenProcess(PROCESS_ALL_ACCESS|PROCESS_QUERY_INFORMATION|SYNCHRONIZE,FALSE,pid); 
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"打开程序%s",ApplicationName);
		if (WaitForSingleObjectEx(process,60000,TRUE)==WAIT_OBJECT_0) //考虑超时
		{
			char buffer[256];
			memset(buffer,0,sizeof(buffer));
			FILE* file;
			file=fopen(filename,"r");
			if(file==NULL)
			{
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"打开命令行结果文本 失败！ 返回%d",GetLastError());
				throw (-1);
			}
			int len=0;
			len=fread(buffer,sizeof(char),255,file);
			fclose(file);
			if(len==0)
			{
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"读取命令行结果失败！ 返回%d",GetLastError());
				throw (-1);
			}
			buffer[len]='\0';
			if (!strcmp(cmd,"c:\\SSTMANAGE OOS 30")||!strcmp(cmd,"c:\\SSTMANAGE GIS 30"))
			{
				if (!strcmp(buffer,"0"))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"SSTManage success! return %s",buffer);
					iret=0;				
				}
				if (!strcmp(buffer,"1"))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"SSTManage failed! return %s",buffer);
					iret=1;
				}
				if (!strcmp(buffer,"2"))
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"SSTManage failed! return %s",buffer);
					iret=2;
				}
				else
				{
					m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"SSTManage failed! return %s",buffer);
					iret=-1;
				}
			}
			else
				m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"执行命令行%s,return %s",cmd,buffer);
			    iret=1;
		}
		else
		{
			m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"等待失败!");
			throw (-1);
		}

		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,_T("执行命令[%s],iret=%d"),cmd,iret);
		return iret;

	}
	catch (...)
	{
		::SetCurrentDirectory(oldcurrentdir);
		m_trace->WTrace(LOG_GRP,LOG_STEP,LT_INFO,"controlapp失败");
		return -1;
	}
}
