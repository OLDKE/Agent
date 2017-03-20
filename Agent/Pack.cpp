#include "StdAfx.h"
#include "Pack.h"


Pack::Pack(CNTTRACE *pTrace)
{
	m_trace=pTrace;


// 	char szValue[_MAX_PATH];
// 	GetNTReg("SOFTWARE\\Ebring\\ATM\\Config","LOGPATH",szValue);
// 	strcpy(LogPath,szValue);
// 
// 	GetNTReg("SOFTWARE\\Ebring\\ATM\\Config","APPPATH",szValue);
// 	strcpy(AppPath,szValue);

	char tmpbuf[32];
	GetNowTime(tmpbuf);
	strcpy(CardTime,tmpbuf);
}

Pack::~Pack(void)
{
}

bool Pack::PackStat(char *PackStr,int *PackLen)
{
	int ret=0;

	char msgbuf[4096],mypackbuf[2048];
	memset(msgbuf,0x00,sizeof(msgbuf));

	int buflen=0;
	char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	memset(tmpbuf,0x00,sizeof(tmpbuf));

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DEVID",szValue);

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNTYPE","STAT");

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNCODE","40001");

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	GetTRNEJ(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNEJ",szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","CFG_VER",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DCFG_VER_C",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","SOF_VER",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DSOF_VER_C",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","ADV_VER",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DADV_VER_C",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DREFTIME_C",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"PRUNST",szValue);


	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","COMSTA",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"PCOMST",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","SAFEDOORSTA",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"PSAFEDOORST",szValue);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_PTR(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_IDC(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_CDM(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_CIM(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_PIN(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_SIU(mypackbuf,&buflen);
	txtPackAdd(msgbuf,mypackbuf);

	strcpy(PackStr,msgbuf);
	*PackLen=strlen(msgbuf);

	return true;
}
bool Pack::PackComd(char *PackStr, int *PackLen)
{
	return true;
}
bool Pack::PackTRN(char* rpttime,char *PackStr, int *PackLen)
{
	int ret=0;

	char msgbuf[4096];
	memset(msgbuf,0x00,sizeof(msgbuf));

	int buflen=0;
	char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	memset(tmpbuf,0x00,sizeof(tmpbuf));

	char rpttimeall[_MAX_PATH],rpttimetmp[_MAX_PATH];
	memset(rpttimeall,0,sizeof(rpttimeall));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TRN","REPORTTIME",rpttimeall);
	if((strcmp(rpttimeall,"20080101010101")<0) || (strlen(rpttimeall) !=14))
		return false;
	if(strcmp(rpttime,rpttimeall)>=0)
		return false;

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TRN","TRNNUM",szValue);
	int maxtrnnum=atoi(szValue);
	int minnum=0;
	memset(rpttimetmp,0,sizeof(rpttimetmp));
	for(int i=1;i<=maxtrnnum;i++)
	{
		sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmpbuf,"REPORTTIME",szValue);

		if(strcmp(szValue,rpttime)>0 && (strlen(szValue)==14))
		{
			if( (strlen(rpttimetmp)==0) || (strcmp(szValue,rpttimetmp)<0) )
			{
				minnum=i;
				strcpy(rpttimetmp,szValue);
			}
		}		
	}
	if(minnum==0)
		return false;

	Sleep(500);//等待应用程序把信息写入注册表

	sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d",minnum);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DEVID",szValue);

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNTYPE","TRAN");

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNCODE","40003");

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	GetTRNEJ(szValue);
	SetNTReg(tmpbuf,"TRNEJ",szValue);	//WANGBO ADD FOR RESP
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNEJ",szValue);

	SetNTReg(tmpbuf,"RESP","0");		//WANGBO ADD FOR RESP

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNDATE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranDate",szValue);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNTIME",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranTime",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TCliTraceNum",szValue);

	memset(szValue,0,sizeof(szValue));
//	GetNTReg(tmpbuf,"TRNEJ_P",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TServTraceNum",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCODE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranCode",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNRET",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRetCode",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC1",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TAccNo",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC2",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TAccNo2",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNAMT",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranAmt",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCURR",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TCurrency",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNFEE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TFee",szValue);

	strcpy(PackStr,msgbuf);
	*PackLen=strlen(msgbuf);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"REPORTTIME",szValue);
	strcpy(rpttime,szValue);

	return true;
}
bool Pack::PackTRN_resp(int TranIndex,char *PackStr, int *PackLen)
{
	int ret=0;

	char msgbuf[4096];
	memset(msgbuf,0x00,sizeof(msgbuf));

	int buflen=0;
	char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\TRN","TRNNUM",szValue);
	int maxtrnnum=atoi(szValue);
	if((TranIndex>maxtrnnum) || (TranIndex<1))
		return false;
		
	sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\TRN\\%d",TranIndex);
		
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DEVID",szValue);

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNTYPE","TRAN");

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNCODE","40003");

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	if(!GetNTReg(tmpbuf,"TRNEJ",szValue))
		return false;
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNEJ",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNDATE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranDate",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNTIME",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranTime",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TCliTraceNum",szValue);

	memset(szValue,0,sizeof(szValue));
//	GetNTReg(tmpbuf,"TRNEJ_P",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TServTraceNum",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCODE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranCode",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNRET",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRetCode",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC1",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TAccNo",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC2",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TAccNo2",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNAMT",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TTranAmt",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCURR",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TCurrency",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNFEE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TFee",szValue);

	strcpy(PackStr,msgbuf);
	*PackLen=strlen(msgbuf);

	return true;
}
bool Pack::PackRTC(char* rpttime,char *PackStr, int *PackLen)
{
	int ret=0;

	char msgbuf[4096];
	memset(msgbuf,0x00,sizeof(msgbuf));

	int buflen=0;
	char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	memset(tmpbuf,0x00,sizeof(tmpbuf));

	char rpttimeall[_MAX_PATH],rpttimetmp[_MAX_PATH];
	memset(rpttimeall,0,sizeof(rpttimeall));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\RTC","REPORTTIME",rpttimeall);
	if((strcmp(rpttimeall,"20080101010101")<0) || (strlen(rpttimeall) !=14))
		return false;
	if(strcmp(rpttime,rpttimeall)>=0)
		return false;

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\RTC","RTCNUM",szValue);
	int maxtrnnum=atoi(szValue);
	int minnum=0;
	memset(rpttimetmp,0,sizeof(rpttimetmp));
	for(int i=1;i<=maxtrnnum;i++)
	{
		sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\RTC\\%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmpbuf,"REPORTTIME",szValue);

		if(strcmp(szValue,rpttime)>0 && (strlen(szValue)==14))
		{
			if( (strlen(rpttimetmp)==0) || (strcmp(szValue,rpttimetmp)<0) )
			{
				minnum=i;
				strcpy(rpttimetmp,szValue);
			}
		}		
	}
	if(minnum==0)
		return false;

	Sleep(500);//等待应用程序把信息写入注册表

	sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\RTC\\%d",minnum);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DEVID",szValue);

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNTYPE","RTCA");

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNCODE","40002");

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	GetTRNEJ(szValue);
	SetNTReg(tmpbuf,"TRNEJ",szValue);	//WANGBO ADD FOR RESP
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNEJ",szValue);

	SetNTReg(tmpbuf,"RESP","0");		//WANGBO ADD FOR RESP

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTDATE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTDate",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTTIME",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTTime",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCARD",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTCARD",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCODE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTCODE",szValue);

	strcpy(PackStr,msgbuf);
	*PackLen=strlen(msgbuf);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"REPORTTIME",szValue);
	strcpy(rpttime,szValue);

	return true;
}
bool Pack::PackRTC_resp(int RTCIndex,char *PackStr, int *PackLen)
{
	int ret=0;

	char msgbuf[4096];
	memset(msgbuf,0x00,sizeof(msgbuf));

	int buflen=0;
	char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\RTC","RTCNUM",szValue);
	int maxtrnnum=atoi(szValue);
	if((RTCIndex>maxtrnnum) || (RTCIndex<1))
		return false;

	sprintf(tmpbuf,"SOFTWARE\\Ebring\\Agent\\RTC\\%d",RTCIndex);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"DEVID",szValue);

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNTYPE","RTCA");

	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNCODE","40002");

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	if(!GetNTReg(tmpbuf,"TRNEJ",szValue))
		return false;
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRNEJ",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTDATE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTDate",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTTIME",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTTime",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCARD",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTCARD",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCODE",szValue);
	txtFieldAdd(msgbuf,sizeof(msgbuf),"TRTCODE",szValue);

	strcpy(PackStr,msgbuf);
	*PackLen=strlen(msgbuf);

	return true;
}

/*UnPack
*return	0
		-1	效验报文错
		-2	已经有命令在执行，且现有命令不是强制清楚
		-3	现有命令执行完毕，但还未发送结果
		2	启动应用
		3	暂停服务
		4	重启设备
		5	强制重启
		6	设备关机
		7	强制关机
		8	更新时间
		9	上传日志
		10	软件更新
		11	强制软件更新
		12	广告更新
		13	强制广告更新
		99	强制清除命令
		100 回执报文
*/

int Pack::UnPack(char *unpackstr, int unpackstrlen)
{
	char szValue[256],tmpbuf[256];
	char Msgbuf[4096];
	memset(Msgbuf,0,sizeof(Msgbuf));
	int iret=0;

	try
	{
		memset(tmpbuf,0,sizeof(tmpbuf));
		
		if(unpackstrlen<=4)
			return -1;
		memcpy(tmpbuf,unpackstr,4);
		
		//if(atoi(tmpbuf) > (unpackstrlen-4))
		if(atoi(tmpbuf) != (unpackstrlen-4))
			return -1;

		memcpy(Msgbuf,unpackstr+4,atoi(tmpbuf));

		memset(tmpbuf,0,sizeof(tmpbuf));
		if(txtFieldGet(Msgbuf,"DEVID",tmpbuf,sizeof(tmpbuf))<0)
			return -1;

		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);

		if(strcmp(szValue,tmpbuf))
		{
			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"设备号效验错 L[%s],R[%s]",szValue,tmpbuf);
			return -6;
		}

		memset(tmpbuf,0,sizeof(tmpbuf));
		txtFieldGet(Msgbuf,"TRNTYPE",tmpbuf,sizeof(tmpbuf));

		if((strcmp(tmpbuf,"COMD")) && (strcmp(tmpbuf,"RESP")))
		{
			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"报文类型效验错 L[%s],R[%s]",szValue,tmpbuf);
			return -1;
		}

		if(!strcmp(tmpbuf,"RESP"))
		{
			memset(szValue,0,sizeof(szValue));
			GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","RESP_FLAG",szValue);
			if(strcmp(szValue,"1"))
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"回执标志为0，无需回执处理");
				return -1;
			}

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"TRNEJ",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_RESP_TMP",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"RESPTYPE",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","RESPTYPE_RESP_TMP",tmpbuf);
			return 100;
		}

		memset(tmpbuf,0,sizeof(tmpbuf));
		if(txtFieldGet(Msgbuf,"TRNEJ",tmpbuf,sizeof(tmpbuf))>0)
			SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_P_TMP",tmpbuf);


		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd","CMD_FLAG",szValue);

		memset(tmpbuf,0,sizeof(tmpbuf));
		if(txtFieldGet(Msgbuf,"TRNCODE",tmpbuf,sizeof(tmpbuf))>0)
			SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNCODE_P_TMP",tmpbuf);

		if( (!strcmp(szValue,"1")) && strcmp(tmpbuf,"60099"))
		{
			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"有其他命令在执行,comd=[%s],CMD_FLAG=[%s]",tmpbuf,szValue);
			return -2;
		}

		char trncode[256];
		strcpy(trncode,tmpbuf);

		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","CMD_DELAY",szValue);
		if(!strcmp(szValue,"1") &&  strcmp(tmpbuf,"60099") )
		{
			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"有强制或长时间命令在执行,CMD_DELAY=[%s]",szValue);
			return -2;
		}
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_P_TMP",szValue);
		SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_P",szValue);

		if (!strcmp(trncode,"60001"))		//更新配置
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			txtFieldGet(Msgbuf,"CTIMER",tmpbuf,sizeof(tmpbuf));
			if(atoi(tmpbuf)>30 && atoi(tmpbuf)<1200)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","TIMER",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"Check Timer error[%d]",atoi(tmpbuf));
				return -1;
			}

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"CFGVER",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","CFG_VER",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"SERIP",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_IP",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"SERPORT",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_PORT",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"CLIPORT",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","CLI_PORT",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"JNLPRAR_C",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_PRAR_C",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"JNLPATH_P",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_PATH_P",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"JNLUPTIME",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_UPTIME",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"FTPIP",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"FTPUSR",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",tmpbuf);

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"FTPPASS",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf);
			
			return 1;
		} 
		else if (!strcmp(trncode,"60002"))	//启动应用
		{
			return 2;
		}
		else if (!strcmp(trncode,"60003"))	//暂停服务
		{
			return 3;
		}
		else if (!strcmp(trncode,"60004"))	//重启设备
		{
			return 4;
		}
		else if (!strcmp(trncode,"60005"))	//强制重启
		{
			return 5;

		}
		else if (!strcmp(trncode,"60006"))	//设备关机
		{
			return 6;
		}
		else if (!strcmp(trncode,"60007"))	//强制关机
		{
			return 7;
		}
		else if (!strcmp(trncode,"60008"))	//更新时间
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PTIME",tmpbuf,sizeof(tmpbuf))>0)
			{
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","PTIME",tmpbuf);
				return 8;
			}
			else
				return -1;

		}
		else if (!strcmp(trncode,"60009"))	//上传文件
		{

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",tmpbuf);
			else
				return -1;


			return 9;
		}
		else if (!strcmp(trncode,"60010"))	//软件更新
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_VER",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_NAME_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_C",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KSPROCESS",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT",tmpbuf);
			else
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT","1");

			return 10;
		}
		else if (!strcmp(trncode,"60011"))	//强制软件更新
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_VER",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_NAME_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_C",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KSPROCESS",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT",tmpbuf);
			else
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT","1");

			return 11;
		}
		else if (!strcmp(trncode,"60012"))	//广告更新
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_VER",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_NAME_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_C",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KAPROCESS",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT",tmpbuf);
			else
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT","0");

			return 12;
		}
		else if (!strcmp(trncode,"60013"))	//强制广告更新
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_VER",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_NAME_P",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_C",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KAPROCESS",tmpbuf);
			else
				return -1;

			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT",tmpbuf);
			else
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT","0");

			return 13;
		}
		else if (!strcmp(trncode,"60014"))	//上传文件明细
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_PATH_P",tmpbuf);
			
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_NAME_P",tmpbuf);
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRPATH_C",tmpbuf);
			
			return 14;
		}
		else if (!strcmp(trncode,"60018"))	//Agent更新
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_VER",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送更新AGENT版本号，AGENT_VER域为空");
				return -19;
			}
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_PATH_P",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送AGENT更新P端路径，AGENT_PATH_P域为空");
				return -20;
			}
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_NAME_P",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送AGENT更新P端文件名，AGENT_NAME_P域为空");
				return -21;
			}
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_PATH_C",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送AGENT更新C端路径，AGENT_PATH_C域为空");
				return -22;
			}			
			
			return 18;
		}
		else if (!strcmp(trncode,"60019"))//上传系统运行状态
		{
			return 19;
		}
		else if (!strcmp(trncode,"60024"))//获取安装软件信息
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端软件信息P端文件路径，PATH_P域为空");
				return -31;
			}
			
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端软件信息P端文件名称，NAME_P域为空");
				return -32;
			}
			return 24;			
		}
		else if (!strcmp(trncode,"60025"))//结束系统进程
		{
			memset(tmpbuf,0,sizeof(tmpbuf));
			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","KILL_PROCESS",tmpbuf);
			else
			{
				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送需要停止的进程名，KILL_PROCESS域为空");
				return -33;
			}		
			return 25;			
		}
		else if (!strcmp(trncode,"60099"))	//强制清除命令
		{
			return 99;
		}
		else
		{
			return -1;
		}
	}
	catch (...)
	{		
		return -1;
	}
	return -1;
}

bool Pack::PackStat_PTR(char * PackStr,int * PackLen)
{
	char packbuf[1024];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PTR","NUM",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DPTRNUM",szValue);

	char tmppath[256],tmpname1[256],tmpname2[256];
	int partnum=atoi(szValue);
	for(int i=0;i<partnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PTR\\%d",i+1);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVTYPE",szValue);
		sprintf(tmpname1,"PPTRST%s",szValue);
		sprintf(tmpname2,"PPTRTP%s",szValue);

		txtFieldAdd(packbuf,sizeof(packbuf),tmpname2,szValue);



		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVSTAT",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname1,szValue);
	}
	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;
}
bool Pack::PackStat_IDC(char * PackStr,int * PackLen)
{
	char packbuf[1024];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\IDC","NUM",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DIDCNUM",szValue);

	char tmppath[256],tmpname1[256],tmpname2[256];
	int partnum=atoi(szValue);
	for(int i=0;i<partnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\IDC\\%d",i+1);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVTYPE",szValue);
		sprintf(tmpname1,"PIDCST%s",szValue);
		sprintf(tmpname2,"PIDCTP%s",szValue);

		txtFieldAdd(packbuf,sizeof(packbuf),tmpname2,szValue);


		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVSTAT",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname1,szValue);
	}
	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;
}
bool Pack::PackStat_CDM(char * PackStr,int * PackLen)
{
	char packbuf[2048];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];
	
	char pcdmsttmp[10];
	memset(pcdmsttmp,0,sizeof(pcdmsttmp));

	memset(szValue,0,sizeof(szValue));
	GetCDMStat(szValue);
	//txtFieldAdd(packbuf,sizeof(packbuf),"PCDMST",szValue);
	strcpy(pcdmsttmp,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","UNITCOUNT",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DONUM",szValue);

	int unitnum=atoi(szValue);
	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256],tmpname4[256],tmpname5[256],tmpname6[256];

	int sortindex[256];
	int j,k=1;
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"1"))//费钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"3"))//取款钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"2"))//回收钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"5"))//循环钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
			
	int totalamount=0;
	int tmpval=0;
	int ivalue=0;
	int icount=0;
	for(int i=0;i<unitnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",i+1);
		sprintf(tmpname1,"POTP%d",sortindex[i]);
		sprintf(tmpname2,"DOCURR%d",sortindex[i]);
		sprintf(tmpname3,"DOVL%d",sortindex[i]);
		sprintf(tmpname4,"DOCN%d",sortindex[i]);
		sprintf(tmpname5,"POST%d",sortindex[i]);
		sprintf(tmpname6,"DOVLT%d",sortindex[i]);
		


		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if((strcmp(szValue,"1"))&&(strcmp(szValue,"3")))
			continue;
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname1,szValue);


		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"CURRENCY",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname2,szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"VALUES",szValue);
		ivalue=atoi(szValue);
		//ivalue=ivalue/100;
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",ivalue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname3,szValue);
		

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"COUNT",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname4,szValue);
		icount=atoi(szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"STATUS",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname5,szValue);

		tmpval=ivalue*icount;
		totalamount+=tmpval;


		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",tmpval);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname6,szValue);

	}
	memset(szValue,0,sizeof(szValue));
	sprintf(szValue,"%d",totalamount);
	txtFieldAdd(packbuf,sizeof(packbuf),"DOVLTT",szValue);
	

	if((!strcmp(pcdmsttmp,"0")) || (!strcmp(pcdmsttmp,"1")) || (!strcmp(pcdmsttmp,"2")))
	{
		if(totalamount == 0)
		{
			strcpy(pcdmsttmp,"2");
		}
		else if(0<totalamount && totalamount<=40000)
		{
			strcpy(pcdmsttmp,"1");
		}
		else
			strcpy(pcdmsttmp,"0");
	}
	txtFieldAdd(packbuf,sizeof(packbuf),"PCDMST",pcdmsttmp);



	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;
}
bool Pack::PackStat_PIN(char * PackStr,int * PackLen)
{
	char packbuf[2048];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PIN","NUM",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DPINNUM",szValue);

	char tmppath[256],tmpname1[256],tmpname2[256];
	int partnum=atoi(szValue);
	for(int i=0;i<partnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PIN\\%d",i+1);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVTYPE",szValue);
		sprintf(tmpname1,"PPINST%s",szValue);
		sprintf(tmpname2,"PPINTP%s",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname2,szValue);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVSTAT",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname1,szValue);
	}
	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;
}
bool Pack::PackStat_CHK(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_DEP(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_TTU(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_SIU(char * PackStr,int * PackLen)
{
	char packbuf[1024];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","SWITCH",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"PSIUSWITCH",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","SAFEDOOR",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"PSIUSAFEDOOR",szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","UPSSTAT",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"PSIUUPSST",szValue);

	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;


}
bool Pack::PackStat_VDM(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_CAM(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_ALM(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_CEU(char * PackStr,int * PackLen)
{
	return true;
}
bool Pack::PackStat_CIM(char * PackStr,int * PackLen)
{
	char packbuf[2048];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	char pcimsttmp[10];
	memset(pcimsttmp,0,sizeof(pcimsttmp));

	memset(szValue,0,sizeof(szValue));
	GetCIMStat(szValue);
	//txtFieldAdd(packbuf,sizeof(packbuf),"PCIMST",szValue);
	strcpy(pcimsttmp,szValue);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","UNITCOUNT",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DINUM",szValue);

	int unitnum=atoi(szValue);
	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256],tmpname4[256],tmpname5[256],tmpname6[256];
	int sortindex[256];
	int j,k=1;
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"1"))//费钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"4"))//存款钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"2"))//回收钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"5"))//循环钞箱
		{
			sortindex[j]=k;
			k++;
		}
	}


	int totalamount=0;
	int tmpval=0;
	int ivalue=0;
	int icount=0;
	for(int i=0;i<unitnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",i+1);
		sprintf(tmpname1,"PITP%d",sortindex[i]);
		sprintf(tmpname2,"DICURR%d",sortindex[i]);
		sprintf(tmpname3,"DIVL%d",sortindex[i]);
		sprintf(tmpname4,"DICN%d",sortindex[i]);
		sprintf(tmpname5,"PIST%d",sortindex[i]);
		sprintf(tmpname6,"DIVLT%d",sortindex[i]);



		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if((strcmp(szValue,"1"))&&(strcmp(szValue,"4")))
			continue;
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname1,szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"CURRENCY",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname2,szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"VALUES",szValue);
		ivalue=atoi(szValue);
		//ivalue=ivalue/100;
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",ivalue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname3,szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"COUNT",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname4,szValue);
		icount=atoi(szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"STATUS",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname5,szValue);

		tmpval=ivalue*icount;
		totalamount+=tmpval;

		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",tmpval);
		txtFieldAdd(packbuf,sizeof(packbuf),tmpname6,szValue);
	}
	memset(szValue,0,sizeof(szValue));
	sprintf(szValue,"%d",totalamount);
	txtFieldAdd(packbuf,sizeof(packbuf),"DIVLTT",szValue);

	if((!strcmp(pcimsttmp,"0")) || (!strcmp(pcimsttmp,"1")))
	{
		if(totalamount >= 350000)
		{
			strcpy(pcimsttmp,"1");
		}
		else
			strcpy(pcimsttmp,"0");
	}
	txtFieldAdd(packbuf,sizeof(packbuf),"PCIMST",pcimsttmp);


	strcpy(PackStr,packbuf);
	*PackLen=strlen(packbuf);
	return true;
}

bool Pack::GetTRNEJ(char *TrnEJ)
{

//	CriticalSection.Lock();

	char szValue[128];
	memset(szValue,0,sizeof(szValue));

	if(GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","TRNEJ",szValue))
	{
		strcpy(TrnEJ,szValue);
		int tmpint=atoi(szValue);
		tmpint ++;
		sprintf(szValue,"%08d",tmpint);
		SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","TRNEJ",szValue);
	}
//	CriticalSection.Unlock();
	return true;
}

bool Pack::GetCDMStat(char *statbuf)
{
	char szValue[256];
	memset(szValue,0,sizeof(szValue));

	if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","DEVSTAT",szValue))
		return false;
	char cdmdevstat[16];
	strcpy(cdmdevstat,szValue);

	if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","DISPENSER",szValue))
		return false;
	char cdmdispenser[16];
	strcpy(cdmdispenser,szValue);

	if ((!strncmp(cdmdevstat,"1",1)) || (!strncmp(cdmdevstat,"1",2)) || (!strncmp(cdmdevstat,"1",3)) )
	{
		strcpy(statbuf,"4");
	} 
	else if(strncmp(cdmdevstat,"0",1))
	{
		strcpy(statbuf,"3");
	}
	else if(!strncmp(cdmdispenser,"0",1))
	{
		strcpy(statbuf,"0");
	}
	else if(!strncmp(cdmdispenser,"1",1))
	{
		strcpy(statbuf,"1");
	}
	else if(!strncmp(cdmdispenser,"2",1))
	{
		strcpy(statbuf,"2");
	}
	else
	{
		strcpy(statbuf,"7");
	}
	return true;
}

bool Pack::GetCIMStat(char *statbuf)
{
	char szValue[256];
	memset(szValue,0,sizeof(szValue));

	if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","DEVSTAT",szValue))
		return false;
	char cimdevstat[16];
	strcpy(cimdevstat,szValue);

	if(!GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","ACCEPTOR",szValue))
		return false;
	char cdmacceptor[16];
	strcpy(cdmacceptor,szValue);

	if ((!strncmp(cimdevstat,"1",1)) || (!strncmp(cimdevstat,"1",2)) || (!strncmp(cimdevstat,"1",3)) )
	{
		strcpy(statbuf,"4");
	} 
	else if(strncmp(cimdevstat,"0",1))
	{
		strcpy(statbuf,"3");
	}
	else if(!strncmp(cdmacceptor,"0",1))
	{
		strcpy(statbuf,"0");
	}
	else if(!strncmp(cdmacceptor,"1",1))
	{
		strcpy(statbuf,"1");
	}
	else if(!strncmp(cdmacceptor,"2",1))
	{
		strcpy(statbuf,"2");
	}
	else
	{
		strcpy(statbuf,"7");
	}
	return true;
}



bool Pack::PackComdRet(char* PackStr,char* Trncode,char* Comd_Res,int TranEJflag)
{
	char packbuf[1024];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[256];

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"DEVID",szValue);

	txtFieldAdd(packbuf,sizeof(packbuf),"TRNTYPE","CRET");

	if(TranEJflag == 0)
	{
		txtFieldAdd(packbuf,sizeof(packbuf),"TRNCODE",Trncode);
	}
	else
	{
		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNCODE_P_TMP",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),"TRNCODE",szValue);
	}

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	txtFieldAdd(packbuf,sizeof(packbuf),"CTIME",szValue);

	memset(szValue,0,sizeof(szValue));
	if(TranEJflag == 0)
	{
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_P",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),"TRNEJ",szValue);
	}
	else
	{
		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","TRNEJ_P_TMP",szValue);
		txtFieldAdd(packbuf,sizeof(packbuf),"TRNEJ",szValue);
	}

	txtFieldAdd(packbuf,sizeof(packbuf),"CMDSTAT",Comd_Res);

	strcpy(PackStr,packbuf);

	return true;
}




int	Pack::txtFieldSeek( char *FieldList, char *FieldName )
{
	char	*p, *b, *s;
	int	l;

	if ( FieldList == NULL || FieldName == NULL )
		return __FLDRC_PARAM;
	b = (char *) FieldList;
	l = strlen( FieldName );
	do {
		while ( *b == __FLDSEP )
			b ++;
		if ( ( p = strchr( b, __FLDEQU ) ) == NULL )
			break;
		s = strchr( b, __FLDAPT );

		if ( s == NULL || s > p ) s = p;

		if ( l == s - b && memcmp( FieldName, b, l ) == 0 )
			return ( b - FieldList );
	} while ( ( b = strchr( p + 1, __FLDSEP ) ) != NULL );
	return __FLDRC_NFND;
}

int	Pack::txtFieldGet( char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize )
{
	int	s, l;
	char	*p, *b;

	if ( ValueBuffer == NULL )
		return __FLDRC_PARAM;
	s = txtFieldSeek( FieldList, FieldName );
	if ( s < __FLDRC_OK )
		return s;
	if( *( FieldList + s + strlen( FieldName )) == __FLDAPT ) {
		b = strchr( FieldList + s, __FLDEQU );
		s = b - FieldList + 1;
	}
	else
		s += strlen( FieldName ) + 1;

	p = strchr( FieldList + s, __FLDSEP );
	if ( p == NULL )
		l = strlen( FieldList + s );
	else
		l = p - ( FieldList + s );
	if ( ValBufSize <= l )
		return __FLDRC_SIZE;
	memcpy( ValueBuffer, FieldList + s, l );
	ValueBuffer[l] = 0;
	return l;
}

int	Pack::txtFieldPut( char *FieldList, int LstBufSize,char *FieldName,char *FieldValue )
{
	int s, l, n, o, t;
	char	*p;

	if ( FieldValue == NULL )
		return __FLDRC_PARAM;
	s = txtFieldSeek( FieldList, FieldName );
	if ( s == __FLDRC_PARAM )
		return __FLDRC_PARAM;
	n = strlen( FieldName );
	if ( s == __FLDRC_NFND ) {
		s = strlen( FieldList );
		l = n + 1 + strlen( FieldValue );
		if ( s > 0 ) {
			if ( LstBufSize <= (s+1+l) )
				return __FLDRC_SIZE;
			FieldList[s] = __FLDSEP;
			s ++;
		}
		else {
			if ( LstBufSize <= l )
				return __FLDRC_SIZE;
		}
		memcpy( FieldList + s, FieldName, n );
		s += n;
		FieldList[s] = __FLDEQU;
		strcpy( FieldList + s + 1, FieldValue );
	}
	else {
		s += n + 1;
		l = strlen( FieldValue );
		t = strlen( FieldList );
		p = strchr( FieldList + s, __FLDSEP );
		if ( p == NULL )
			o = t - s;
		else
			o = p - ( FieldList + s );
		if ( LstBufSize <= (strlen( FieldList ) - o + l) )
			return __FLDRC_SIZE;
		memmove( FieldList + s + l, FieldList + s + o, t - s - o + 1 );
		memcpy( FieldList + s, FieldValue, l );
	}
	return __FLDRC_OK;
}

int	Pack::txtFieldClear( char *FieldList, char *FieldName )
{
	int	s;
	char	*p;

	s = txtFieldSeek( FieldList, FieldName );
	if ( s < __FLDRC_OK )
		return s;
	p = strchr( FieldList + s + strlen( FieldName ) + 1, __FLDSEP );
	if ( p != NULL )
		strcpy( FieldList + s, p + 1 );
	else if ( s == 0 )
		FieldList[0] = 0;
	else
		FieldList[s - 1] = 0;
	return __FLDRC_OK;
}

int	Pack::txtFieldTake( char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize )
{
	int	s, l, n;
	char	*p;

	if ( ValueBuffer == NULL )
		return __FLDRC_PARAM;
	s = txtFieldSeek( FieldList, FieldName );
	if ( s < __FLDRC_OK )
		return s;
	n = strlen( FieldName ) + 1;
	p = strchr( FieldList + s + n, __FLDSEP );
	if ( p == NULL )
		l = strlen( FieldList + s + n );
	else
		l = p - ( FieldList + s + n );
	if ( ValBufSize <= l )
		return __FLDRC_SIZE;
	memcpy( ValueBuffer, FieldList + s + n, l );
	ValueBuffer[l] = 0;
	if ( p != NULL )
		strcpy( FieldList + s, p + 1 );
	else if ( s == 0 )
		FieldList[0] = 0;
	else
		FieldList[s - 1] = 0;
	return l;
}

int	Pack::txtFieldAdd( char *FieldList, int LstBufSize,const char *FieldName, char *FieldValue )
{
	int	s, n, l;
	if ( FieldList == NULL || FieldName == NULL || FieldValue == NULL )
		return __FLDRC_PARAM;
	s = strlen( FieldList );
	n = strlen( FieldName );
	l = n + 1 + strlen( FieldValue );
	if ( s > 0 ) 
	{
		if ( LstBufSize <= s + 1 + l )
			return __FLDRC_SIZE;
		FieldList[s] = __FLDSEP;
		s ++;
	}
	else 
	{
		if ( LstBufSize <= l )
			return __FLDRC_SIZE;
	}
	memcpy( FieldList + s, FieldName, n );
	s += n;
	FieldList[s] = __FLDEQU;
	FieldList[s+1] = 0x00;
	strcpy( FieldList + s + 1, FieldValue );
	return __FLDRC_OK;
}

int	Pack::txtFieldFirst( char *FieldList, char *NameBuffer, int NamBufSize,char *ValueBuffer, int ValBufSize )
{
	int	s, l;
	char	*p;

	if ( FieldList == NULL || NameBuffer == NULL || ValueBuffer == NULL )
		return __FLDRC_PARAM;
	p = strchr( FieldList, __FLDEQU );
	if ( p == NULL )
		return __FLDRC_NFND;
	s = p - FieldList;
	if ( NamBufSize <= s )
		return __FLDRC_SIZE;
	p = strchr( p + 1, __FLDSEP );
	if ( p == NULL )
		l = strlen( FieldList + s + 1 );
	else
		l = p - ( FieldList + s + 1 );
	if ( ValBufSize <= l )
		return __FLDRC_SIZE;
	memcpy( NameBuffer, FieldList, s );
	NameBuffer[s] = 0;
	memcpy( ValueBuffer, FieldList + s + 1, l );
	ValueBuffer[l] = 0;
	if ( p == NULL )
		FieldList[0] = 0;
	else
		strcpy( FieldList,p + 1 );
	return l;
}

int	Pack::txtFieldSkip( char *FieldList )
{
	char	*p;

	if ( FieldList == NULL )
		return __FLDRC_PARAM;
	p = strchr( FieldList, __FLDEQU );
	if ( p == NULL )
		return __FLDRC_NFND;
	p = strchr( p + 1, __FLDSEP );
	if ( p == NULL )
		FieldList[0] = 0;
	else
		strcpy( FieldList, p + 1 );
	return __FLDRC_OK;
}

int	Pack::txtParseValue( char *Value, char *CodeBuffer, int CodBufSize,char *DescBuffer, int DscBufSize )
{
	int	c, d, l;
	char	*p;

	if ( Value == NULL )
		return __FLDRC_PARAM;
	l = strlen( Value );
	p = strchr( Value, __FLDAPT );
	if ( p == NULL ) {
		c = l;
		d = 0;
	}
	else {
		c = p - Value;
		d = l - c - 1;
	}
	if ( CodeBuffer != NULL && CodBufSize <= c
		|| DescBuffer != NULL && DscBufSize <= d )
		return __FLDRC_SIZE;
	if ( CodeBuffer != NULL ) {
		memcpy( CodeBuffer, Value, c );
		CodeBuffer[c] = 0;
	}
	if ( DescBuffer != NULL ) {
		if ( p == NULL )
			DescBuffer[0] = 0;
		else
			strcpy(DescBuffer,p + 1);
	}
	return __FLDRC_OK;
}

int	Pack::txtPackAdd( char *FieldList,char *Packbuf )
{
	int	s, n;

	if ( FieldList == NULL || Packbuf == NULL )
		return __FLDRC_PARAM;
	
	s = strlen( FieldList );
	n = strlen( Packbuf );
	FieldList[s] = __FLDSEP;
	memcpy( FieldList + s+1, Packbuf, n );
	FieldList[n + s + 2] = 0x00;
	return __FLDRC_OK;
}