// PackXML.cpp: implementation of the PackXML class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NTAgent3.h"
#include "PackXML.h"

#include <MsXml2.h>
#import "MSXML2.dll" 
using namespace std ;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// char RecvRetDefine[][100] = {
// 							"60002|+0|3|", 
// 							""
// 						};

PackXML::PackXML(CNTTRACE *pTrace)
{
    m_trace=pTrace;
	//CoInitialize(NULL);
   // m_mydes =new CMyDes(pTrace);
}

PackXML::~PackXML()
{
	//delete m_mydes;
}


int PackXML::UnPack( char *unpackstr, int unpackstrlen, PVMSG pvmsg )
{
	int ret = -1;
	char msg[3072] = {0};
	char szValue[64] = {0};
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config", "KEY", szValue);
	
	//测试，暂时不解密
	//strcpy(msg, unpackstr);
	m_mydes->Dec(unpackstr, szValue, msg);  //解密报文

	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "收到报文, 明文=[%s]", msg);
    
	ret = UnPackMsg(msg, strlen(msg), pvmsg);

	return ret;
	
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

int PackXML::UnPackMsg(char *unpackstr, int unpackstrlen, PVMSG pvmsg)
{
	char szValue[128] = {0};
	char * Msgbuf = unpackstr;
	int iret = -1;
	try
	{
		if( GetFieldValue(Msgbuf, "DEVID", pvmsg->DEVID)<0 )
		{
			return -1;
		}
		if( GetFieldValue(Msgbuf, "TRNTYPE", pvmsg->TRNTYPE)<0 )
		{
			return -1;
		}

		if( GetFieldValue(Msgbuf, "TRNCODE", pvmsg->TRNCODE)<0 )
		{
			//return -1;
		}
	
		if( GetFieldValue(Msgbuf, "TRNEJ", pvmsg->TRNEJ)<0 )
		{
			//return -1;
		}
	
		if( GetFieldValue(Msgbuf, "PTIME", pvmsg->PTIME)<0 )
		{
			//return -1;
		}
		if( GetFieldValue(Msgbuf, "PARA1", pvmsg->PARA1)<0 )
		{
			//return -1;
		}
		if( GetFieldValue(Msgbuf, "PARA2", pvmsg->PARA2)<0 )
		{
			//return -1;
		}
		if( GetFieldValue(Msgbuf, "PARA3", pvmsg->PARA3)<0 )
		{
			//return -1;
		}
		if( GetFieldValue(Msgbuf, "PARA4", pvmsg->PARA4)<0 )
		{
			//return -1;
		}
		if( GetFieldValue(Msgbuf, "PARA5", pvmsg->PARA5)<0 )
		{
			//return -1;
		}		
		if( GetFieldValue(Msgbuf, "PARA6", pvmsg->PARA6)<0 )
		{
			//return -1;
		}

		//校验设备号是否正确
		if( !CheckDevID(pvmsg->DEVID) )
		{
			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "设备号校验错误,发送报文设备=[%s]", pvmsg->DEVID);
			return -6;
		}

		//判断是否为回执标志
		if( !strcmp(pvmsg->TRNTYPE, "RESP") )
		{
			GetFieldValue(Msgbuf, "RESPTYPE", pvmsg->RESPTYPE);
			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_INFO, "收到回执, RESPTYPE=[%s]",szValue);
			return 100;
		}

		memset(szValue,0,sizeof(szValue));
		GetNTReg("SOFTWARE\\Ebring\\Agent\\HostCmd", "CMD_FLAG", szValue);
		if( (!strcmp(szValue, "1")) && strcmp(pvmsg->TRNCODE, "60099") )
		{
			m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "有其他命令在执行, comd=[%s], CMD_FLAG=[%s]",pvmsg->TRNCODE, szValue);
			return -2;
		}

		return 0;

		

		if (!strcmp(pvmsg->TRNCODE, "60001"))		//更新配置
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			txtFieldGet(Msgbuf,"CTIMER",tmpbuf,sizeof(tmpbuf));
// 			if(atoi(tmpbuf)>30 && atoi(tmpbuf)<1200)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","TIMER",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"Check Timer error[%d]",atoi(tmpbuf));
// 				return -5;
// 			}
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"CFGVER",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","CFG_VER",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"SERIP",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_IP",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"SERPORT",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","SER_PORT",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"CLIPORT",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","CLI_PORT",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"JNLPRAR_C",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_PRAR_C",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"JNLPATH_P",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_PATH_P",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"JNLUPTIME",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Log","JNL_UPTIME",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"FTPIP",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_IP",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"FTPUSR",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_USER",tmpbuf);
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"FTPPASS",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config","FTP_PASS",tmpbuf);
// 			
// 			return 1;
		} 
		else if (!strcmp(pvmsg->TRNCODE, "60011"))	//强制软件更新
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_VER",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_NAME_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","SOF_PATH_C",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KSPROCESS",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT",tmpbuf);
// 			else
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","SREBOOT","1");

			return 11;
		}
		else if (!strcmp(pvmsg->TRNCODE,"60012"))	//广告更新
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_VER",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_NAME_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_C",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KAPROCESS",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT",tmpbuf);
// 			else
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT","0");

			return 12;
		}
		else if (!strcmp(pvmsg->TRNCODE,"60013"))	//强制广告更新
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_VER",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_NAME_P",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","ADV_PATH_C",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA5",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","KAPROCESS",tmpbuf);
// 			else
// 				return -1;
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA6",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT",tmpbuf);
// 			else
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","AREBOOT","0");

			return 13;
		}
		else if (!strcmp(pvmsg->TRNCODE,"60014"))	//上传目录明细
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_PATH_P",tmpbuf);
// 			
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRINFO_NAME_P",tmpbuf);
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","DIRPATH_C",tmpbuf);
			
			return 14;
		}
		else if (!strcmp(pvmsg->TRNCODE,"60018"))	//Agent更新
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_VER",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发现更新AGENT版本号，AGENT_VER域为空");
// 				return -19;
// 			}
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_PATH_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发现AGENT更新P端路径，AGENT_PATH_P域为空");
// 				return -20;
// 			}
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA3",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_NAME_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发现AGENT更新P端文件名，AGENT_NAME_P域为空");
// 				return -21;
// 			}
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA4",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\Config\\Ver","AGENT_PATH_C",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发现AGENT更新C端路径，AGENT_PATH_P域为空");
// 				return -22;
// 			}			
			return 18;
		}

		else if (!strcmp(pvmsg->TRNCODE,"60023"))//获取安装硬件信息
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端硬件信息P端文件路径，PATH_H域为空");
// 				return -23;
// 			}
// 
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端硬件信息P端文件名称，NAME_H域为空");
// 				return -24;
// 			}
			return 23;			
		}
		else if (!strcmp(pvmsg->TRNCODE,"60024"))//获取安装软件信息
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端软件信息P端文件路径，PATH_S域为空");
// 				return -31;
// 			}
// 			
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA2",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送C端软件信息P端文件名称，NAME_S域为空");
// 				return -32;
// 			}
			return 24;			
		}
		else if (!strcmp(pvmsg->TRNCODE,"60025"))//结束系统进程
		{
// 			memset(tmpbuf,0,sizeof(tmpbuf));
// 			if(txtFieldGet(Msgbuf,"PARA1",tmpbuf,sizeof(tmpbuf))>0)
// 				SetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","KILL_PROCESS",tmpbuf);
// 			else
// 			{
// 				m_trace->WTrace("EbringAgent","EbringAgent.log",LT_INFO,"未发送需要停止的进程名，KILL_PROCESS域为空");
// 				return -33;
// 			}		
			return 25;			
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

int PackXML::GetFieldValue(char *Msgbuf, char *FieldName, char *FieldValue)
{
	char tmpbuf[256];
	memset(tmpbuf, 0x0, sizeof(tmpbuf));
	if(txtFieldGet(Msgbuf, FieldName, tmpbuf, sizeof(tmpbuf))<0)
	{
		//m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "报文解包 [%s] 没找到", FieldName);
		return -1;
	}
	else
	{
		strcpy(FieldValue, tmpbuf);
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "报文解包 [%s]=[%s]", FieldName, tmpbuf);
	}
	return 0;
}

bool PackXML::CheckDevID(const char *MsgDevID)
{
	char szValue[128] = {0};
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO", szValue);
	
	if(strcmp(szValue, MsgDevID))
	{
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "设备号不一致 本机设备号=[%s], 对比设备号=[%s]", szValue, MsgDevID);
		return false;
	}
	return true;
}

bool PackXML::PackStat(char *PackStr,int *PackLen)
{
	int ret=0;
	
	int buflen=0;
	
	int tmpbuflen=0;
	char szValue[_MAX_PATH];
	LONG lRes=0;

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30)); 
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为root
	pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"STAT");//节点值
	xmlRoot->appendChild(childNode1);

	pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"40001");//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	GetTRNEJ(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	pDoc->raw_createElement((_bstr_t)(char*)"State_T", &childNode1);
	xmlRoot->raw_appendChild(childNode1, NULL);
	MSXML2::IXMLDOMElementPtr childNode2 ;
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","CFG_VER",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DCFG_VER_C", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","SOF_VER",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DSOF_VER_C", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","ADV_VER",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DADV_VER_C", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\VER","AGT_VER",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DAGENT_VER_C", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","REFTIME",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DREFTIME_C", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","RUNSTA",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PRUNST", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","COMSTA",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PCOMST", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	//添加对ATMC关键信息的监测
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","AtmNO",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_AtmNO", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","BranchID",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_BranchID", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","IPAddress",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_IPAddress", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","UnionpayAreaCode",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_AreaCode", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","CamType",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_CamType", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","DeviceType",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_DeviceType", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","Version",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_Version", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","MaintMode",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_MaintMode", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","Count",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_Count", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","CInterval",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_CInterval", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\INF","isJournal",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"KF_isJournal", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;
	
	char tmpbuff[2048];
	memset(tmpbuff,0,sizeof(tmpbuff));
	strncpy(tmpbuff,b,strlen(b));
//	tmpbuff[strlen(b)-2]='\0';
	int len=strlen(tmpbuff);
	//m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"PackStat,(报文前部)长度[%d],为其分配数组长度[%d],tmpbuff=%s",len,sizeof(tmpbuff),tmpbuff);


	char msg[6000];
	memset(msg,0,sizeof(msg));
	memcpy(msg,tmpbuff,strlen(tmpbuff)-9);
	len=strlen(msg);

    char mypackbuf[4096];
	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_SIU(mypackbuf,&buflen);
	strcat(msg,mypackbuf);
//	txtPackAdd(msgbuf,mypackbuf);

	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_PTR(mypackbuf,&buflen);
	strcat(msg,mypackbuf);    
//	txtPackAdd(msgbuf,mypackbuf);
	
	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_IDC(mypackbuf,&buflen);
	strcat(msg,mypackbuf);
//	txtPackAdd(msgbuf,mypackbuf);
	
	memset(szValue,0,sizeof(szValue));
	char devtyp[256];
	memset(devtyp,0,sizeof(devtyp));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","UNITCOUNT",szValue);
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","DEVTYPE",devtyp);
	if(!strcmp(szValue,"")==0&&!strcmp(devtyp,"")==0)
	{ 

		memset(mypackbuf,0,sizeof(mypackbuf));
		PackStat_CDM(mypackbuf,&buflen);
		strcat(msg,mypackbuf);
	//	txtPackAdd(msgbuf,mypackbuf);
	}
	
	memset(szValue,0,sizeof(szValue));
	memset(devtyp,0,sizeof(devtyp));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","UNITCOUNT",szValue);
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","DEVTYPE",devtyp);
	if(!strcmp(szValue,"")==0&&!strcmp(devtyp,"")==0)
	{
		memset(mypackbuf,0,sizeof(mypackbuf));
		PackStat_CIM(mypackbuf,&buflen);
		strcat(msg,mypackbuf);
	//	txtPackAdd(msgbuf,mypackbuf);
	}
	
	memset(mypackbuf,0,sizeof(mypackbuf));
	PackStat_PIN(mypackbuf,&buflen);

	strcat(msg,mypackbuf);
//	txtPackAdd(msgbuf,mypackbuf);


	strcat(msg,"</root>");

	//strncpy(PackStr,msg,strlen(msg)-2);

	//PackStr[strlen(msg)-2]='\0';
	strcpy(PackStr,msg);
	*PackLen=strlen(PackStr);
	//m_trace->info("报文长度[%d],数组长度[%d],PackLen=[%d]",strlen(PackStr),sizeof(msg),*PackLen);
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "状态打包信息=[%s]", PackStr);
	
	return true;
}

bool PackXML::GetTRNEJ(char *TrnEJ)
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

bool PackXML::GetCDMStat(char *statbuf)
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

	//默认为state值
	strcpy(statbuf, cdmdevstat);

	//如果cdmdispenser为1(钞少)，转换为1
	if( !strcmp(cdmdispenser,"1")  && !strcmp(cdmdevstat,"0") )
	{
		strcpy(statbuf,"1");
	}

	//如果cdmdispenser为2(缺钞)，转换为2
	if( !strcmp(cdmdispenser,"2")  && !strcmp(cdmdevstat,"0") )
	{
		strcpy(statbuf,"2");
	}


// 	DEVSTAT=1   result=4
// 		if ((!strncmp(cdmdevstat,"1",1)) || (!strncmp(cdmdevstat,"1",2)) || (!strncmp(cdmdevstat,"1",3)) )
// 		{
// 			strcpy(statbuf,"4");
// 		} 
// 		else if(strncmp(cdmdevstat,"0",1))   //DEVSTAT=0   result=3
// 		{
// 			//strcpy(statbuf,"3");
// 			strcpy(statbuf,"0");
// 		}
// 		else if(!strncmp(cdmdispenser,"0",1))
// 		{
// 			strcpy(statbuf,"0");
// 		}
// 		else if(!strncmp(cdmdispenser,"1",1))
// 		{
// 			strcpy(statbuf,"1");
// 		}
// 		else if(!strncmp(cdmdispenser,"2",1))
// 		{
// 			strcpy(statbuf,"2");
// 		}
// 		else
// 		{
// 			strcpy(statbuf,"7");
// 		}
	return true;
}

bool PackXML::GetCIMStat(char *statbuf)
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



bool PackXML::PackComdRet(char* PackStr,char* Trncode,char* Comd_Res, char *TRNEJ, char *errmsg, PVMSG vmsg)
{
	char packbuf[2048];
	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[256];
	memset(szValue,0,sizeof(szValue));

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30)); 
	if(!SUCCEEDED(hr))
	{
		m_trace->error("PackComdRet,无法创建DM对象");
		return false;
	}
	
	//根节点的名称为State_IDC
	pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	//设置号
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ;
	pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值

	//交易类型
	xmlRoot->appendChild(childNode1);
	pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	childNode1->Puttext((_bstr_t)("CRET"));//节点值
	xmlRoot->appendChild(childNode1);

	//交易码
	pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	childNode1->Puttext((_bstr_t)(Trncode));//节点值
	xmlRoot->appendChild(childNode1);

	//返回时间
	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	//原发送报文流水号
	pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	childNode1->Puttext((_bstr_t)(TRNEJ));//节点值
	xmlRoot->appendChild(childNode1);

	//返回状态 2正在执行, 3成功, 4失败
	pDoc->raw_createElement((_bstr_t)(char*)"CMDSTAT", &childNode1);
	childNode1->Puttext((_bstr_t)(Comd_Res));//节点值
	xmlRoot->appendChild(childNode1);

	//错误信息
	pDoc->raw_createElement((_bstr_t)(char*)"ERR_DETAIL", &childNode1);
	childNode1->Puttext((_bstr_t)(errmsg));//节点值
	xmlRoot->appendChild(childNode1);

	if(!strcmp(Trncode,"60009"))
	{
		
		pDoc->raw_createElement((_bstr_t)(char*)"COMD_PARA", &childNode1);
		xmlRoot->raw_appendChild(childNode1, NULL);
		MSXML2::IXMLDOMElementPtr childNode2 ;
		
// 		memset(szValue,0,sizeof(szValue));
// 		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPPATH_P",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)"PARA1",&childNode2);
		if(vmsg) childNode2->Puttext((_bstr_t)vmsg->PARA1);
		childNode1->appendChild(childNode2);
		
// 		memset(szValue,0,sizeof(szValue));
// 		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_P",szValue); //直接在上传文件时，将统配后的文件名写入注册表，此处不用再通配
		pDoc->raw_createElement((_bstr_t)(char*)"PARA2",&childNode2);
		if(vmsg) childNode2->Puttext((_bstr_t)vmsg->PARA2);
		childNode1->appendChild(childNode2);
		
// 		memset(szValue,0,sizeof(szValue));
// 		GetNTReg("SOFTWARE\\Ebring\\Agent\\TMP","UPFILE_PARA",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)"PARA3",&childNode2);
		if(vmsg) childNode2->Puttext((_bstr_t)vmsg->PARA3);
		childNode1->appendChild(childNode2);
	}
	
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	

	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	strncpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';

	return true;
}

bool PackXML::PackComdRet2(char* PackStr,char* Trncode,char* Comd_Res, char *TRNEJ)
{
	char packbuf[2048];
	memset(packbuf,0x00,sizeof(packbuf));
	char cmd_para[2048];
	memset(cmd_para,0,sizeof(cmd_para));
	char szValue[256];
	memset(szValue,0,sizeof(szValue));
	strcpy(cmd_para,"<root><DEVID>");
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	strcat(cmd_para,szValue);
	strcat(cmd_para,"</DEVID><TRNTYPE>CRET</TRNTYPE>");

	
	strcat(cmd_para,"<TRNCODE>");
	strcat(cmd_para,Trncode);
	strcat(cmd_para,"</TRNCODE>");
	
	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	strcat(cmd_para,"<CTIME>");
	strcat(cmd_para,szValue);
	strcat(cmd_para,"</CTIME>");


	strcat(cmd_para,"<TRNEJ>");
	strcat(cmd_para,TRNEJ);
	strcat(cmd_para,"</TRNEJ>");

	strcat(cmd_para,Comd_Res);
	strcat(cmd_para,"</root>");
	
	strncpy(PackStr,cmd_para,strlen(cmd_para));
	PackStr[strlen(cmd_para)]='\0';


	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackComdRet,PackStr=[%s]", PackStr);

	return true;
}

bool PackXML::PackTRN(int &pos,char *PackStr, int *PackLen)
{

	int buflen=0;
	//char tmpbuf[_MAX_PATH];
	int tmpbuflen=0;
	//char szValue[_MAX_PATH];
	char tmpbuf[8192];
	char szValue[8192];
	LONG lRes=0;
	
	memset(tmpbuf,0x00,sizeof(tmpbuf));

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\TRN","SIZE",szValue);
	int maxtrnnum=atoi(szValue);
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "SIZE=[%s], maxtrnnum=[%d]", szValue, maxtrnnum);
	int minnum=100;
	//memset(rpttimetmp,0,sizeof(rpttimetmp));
	for(int i=0;i<maxtrnnum;i++)
	{
		sprintf(tmpbuf,"SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\TRN\\%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmpbuf,"SIGNAL",szValue);
		if(!strcmp(szValue, "0")) minnum = i;
	}
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "minnum=[%d]", minnum);
	if(minnum==100) return false;
	//strcpy(rpttime,rpttimetmp);
	pos = minnum;
	//Sleep(500);//等待应用程序把信息写入注册表

	sprintf(tmpbuf,"SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\TRN\\%d",minnum);
	char buffer[8192] = { 0x00 };
	GetNTReg(tmpbuf, "PACKET", buffer);
	//m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "packet=[%s]", buffer);
	char msg[8192];
	memset(msg, 0, sizeof(msg));
	strcpy(msg, buffer);
	memcpy(PackStr, msg, strlen(msg));
	PackStr[strlen(msg)] = '\0';

	*PackLen = strlen(PackStr);

	//m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "打包交易报文, PackStr=[%s]", PackStr);

	return true;


	//CoInitialize(NULL) ;
	//MSXML2::IXMLDOMDocumentPtr pDoc; 
	//MSXML2::IXMLDOMElementPtr xmlRoot ;
	////创建DOMDocument对象
	//HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	//if(!SUCCEEDED(hr)) 
	//{ 
	//	m_trace->error("PackTRN,无法创建DM对象");
	//	return false;
	//} 
	//
	////根节点的名称为root
	//pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	//pDoc->raw_appendChild(xmlRoot, NULL);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	//MSXML2::IXMLDOMElementPtr childNode1 ; 
	//pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	//childNode1->Puttext((_bstr_t)(szValue));//节点值
	//xmlRoot->appendChild(childNode1);

	//pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	//childNode1->Puttext((_bstr_t)(char*)"TRAN");//节点值
	//xmlRoot->appendChild(childNode1);

	//pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	//childNode1->Puttext((_bstr_t)(char*)"40003");//节点值
	//xmlRoot->appendChild(childNode1);

	//memset(szValue,0,sizeof(szValue));
	//GetNowTime(szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	//childNode1->Puttext((_bstr_t)(szValue));//节点值
	//xmlRoot->appendChild(childNode1);
	//
	////memset(szValue,0,sizeof(szValue));
	////GetTRNEJ(szValue);
	////GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	//GetNTReg(tmpbuf,"TRNEJ",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	//childNode1->Puttext((_bstr_t)(szValue));//节点值
	//xmlRoot->appendChild(childNode1);

 //   //WANGBO ADD FOR RESP
	////SetNTReg(tmpbuf,"TRNEJ",szValue);
	////SetNTReg(tmpbuf,"RESP","0");

	//pDoc->raw_createElement((_bstr_t)(char*)"TranInfo", &childNode1);
	//xmlRoot->raw_appendChild(childNode1, NULL);
	//MSXML2::IXMLDOMElementPtr childNode2 ;
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNDATE",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TranDate", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNTIME",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TranTime", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TCliTraceNum", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//

	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNEJ_P",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TServTraceNum", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNCODE",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TranCode", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
 //
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNRET",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TRetCode", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNACC1",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TAccNo1", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);

	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNACC2",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TAccNo2", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);

	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNAMT",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TranAmt", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);
	//
	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNCURR",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TCurrency", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);

	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNFEE",szValue);
	//pDoc->raw_createElement((_bstr_t)(char*)"TFee", &childNode2);
	//childNode2->Puttext((_bstr_t)(szValue));//节点值
	//childNode1->appendChild(childNode2);

	//memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"REPORTTIME",szValue);
	//strcpy(rpttime,szValue);//返回型参数，记录本次交易信息上送时间

	
}
bool PackXML::PackSNR(int &pos, char *PackStr, int *PackLen)
{
	int buflen = 0;
	//char tmpbuf[_MAX_PATH];
	int tmpbuflen = 0;
	//char szValue[_MAX_PATH];
	char tmpbuf[8192];
	char szValue[8192];
	LONG lRes = 0;
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "POS=[%d],PackStr=[%s], PackLen=[%d]", pos, PackStr, PackLen);
	memset(tmpbuf, 0x00, sizeof(tmpbuf));

	memset(szValue, 0, sizeof(szValue));
	GetNTReg("SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\SNR", "SIZE", szValue);
	int maxtrnnum = atoi(szValue);
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "SIZE=[%s], maxtrnnum=[%d]", szValue, maxtrnnum);
	int minnum = 100;
	//memset(rpttimetmp,0,sizeof(rpttimetmp));
	for (int i = 0; i<maxtrnnum; i++)
	{
		sprintf(tmpbuf, "SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\SNR\\%d", i);
		memset(szValue, 0, sizeof(szValue));
		GetNTReg(tmpbuf, "SIGNAL", szValue);
		if (!strcmp(szValue, "0")) minnum = i;
	}
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "minnum=[%d]", minnum);
	if (minnum == 100) return false;
	//strcpy(rpttime,rpttimetmp);
	pos = minnum;
	//Sleep(500);//等待应用程序把信息写入注册表

	sprintf(tmpbuf, "SOFTWARE\\EBRING\\Agent\\Config\\FLOWA\\MESSAGES\\SNR\\%d", minnum);
	char buffer[8192] = { 0x00 };
	GetNTReg(tmpbuf, "PACKET", buffer);
	//m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "packet=[%s]", buffer);
	char msg[8192];
	memset(msg, 0, sizeof(msg));
	strcpy(msg, buffer);
	memcpy(PackStr, msg, strlen(msg));
	PackStr[strlen(msg)] = '\0';

	*PackLen = strlen(PackStr);

	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "打包交易报文, PackStr=[%s]", PackStr);

	return true;
}
bool PackXML::PackTRN_resp(int TranIndex,char *PackStr, int *PackLen)
{

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

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackTRN_resp,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为root
	pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"TRAN");//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"40003");//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	//GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	GetNTReg(tmpbuf,"TRNEJ",szValue);

	//memset(szValue,0,sizeof(szValue));
	//GetTRNEJ(szValue);
	//SetNTReg(tmpbuf,"TRNEJ",szValue);
	SetNTReg(tmpbuf,"RESP","0");
	pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);


	pDoc->raw_createElement((_bstr_t)(char*)"TranInfo", &childNode1);
	xmlRoot->raw_appendChild(childNode1, NULL);
	MSXML2::IXMLDOMElementPtr childNode2 ;
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNDATE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TranDate", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNTIME",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TranTime", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNEJ_C",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TCliTraceNum", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNEJ_P",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TServTraceNum", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCODE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TranCode", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNRET",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRetCode", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC1",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TAccNo1", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNACC2",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TAccNo2", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNAMT",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TranAmt", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNCURR",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TCurrency", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"TRNFEE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TFee", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;
	
	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	strcpy(PackStr,msg);
		
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "打包交易返回报文, PackStr=[%s]", PackStr);

	return true;
}
bool PackXML::PackRTC(char* rpttime,char *PackStr, int *PackLen)
{

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

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackTRN_resp,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为root
	pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"RTCA");//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"40002");//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetTRNEJ(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
    //WANGBO ADD FOR RESP
	SetNTReg(tmpbuf,"TRNEJ",szValue);
	SetNTReg(tmpbuf,"RESP","0");
	
	pDoc->raw_createElement((_bstr_t)(char*)"RTCInfo", &childNode1);
	xmlRoot->raw_appendChild(childNode1, NULL);
	MSXML2::IXMLDOMElementPtr childNode2 ;

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTDATE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTDate", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTTIME",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTTime", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCARD",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTCARD", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
 
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCODE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTCODE", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);

	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"REPORTTIME",szValue);
	strcpy(rpttime,szValue);

	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	

	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "打包吞卡报文,PackStr=[%s]",PackStr);

	return true;
}
bool PackXML::PackRTC_resp(int RTCIndex,char *PackStr, int *PackLen)
{
	int ret=0;
	
//	char msgbuf[2048];
//	memset(msgbuf,0x00,sizeof(msgbuf));
	
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
	
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "PackTRN_resp,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为root
	pDoc->raw_createElement((_bstr_t)(char*)"root", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\Config","ATMNO",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DEVID", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"TRNTYPE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"RTCA");//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"TRNCODE", &childNode1);
	childNode1->Puttext((_bstr_t)(char*)"40002");//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	GetNowTime(szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"CTIME", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	memset(szValue,0,sizeof(szValue));
	//if(!GetNTReg(tmpbuf,"TRNEJ_C",szValue))
	//{
	//	return false;
	//}
	GetTRNEJ(szValue);
	SetNTReg(tmpbuf,"TRNEJ",szValue);
	SetNTReg(tmpbuf,"RESP","0");
	pDoc->raw_createElement((_bstr_t)(char*)"TRNEJ", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	pDoc->raw_createElement((_bstr_t)(char*)"RTCInfo", &childNode1);
	xmlRoot->raw_appendChild(childNode1, NULL);
	MSXML2::IXMLDOMElementPtr childNode2 ;
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTDATE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTDate", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTTIME",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTTime", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCARD",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTCARD", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg(tmpbuf,"RTCODE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"TRTCODE", &childNode2);
	childNode2->Puttext((_bstr_t)(szValue));//节点值
	childNode1->appendChild(childNode2);
	
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	

	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';

	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "打包吞卡返回报文, 报文长度[%d],PackStr=[%s]",strlen(PackStr),PackStr);
	
	return true;
}
bool PackXML::PackStat_VDM(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_CAM(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_ALM(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_CEU(char * PackStr,int * PackLen)
{
	return true;
}

bool PackXML::PackStat_PTR(char * PackStr,int * PackLen)
{
	char szValue[260];

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_PTR,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_PTR
	pDoc->raw_createElement((_bstr_t)(char*)"State_PTR", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);
	
	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PTR","NUM",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DPTRNUM", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);
	
	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256];
	int partnum=atoi(szValue);
	//for(int i=0;i<partnum;i++)
	for(int i=1;i<=partnum;i++)
	{
		memset(szValue,0,strlen(szValue));
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PTR\\%d",i);
		GetNTReg(tmppath,"LOGNAME",szValue);
		if(szValue[0]==NULL)
			break;
		
			//sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PTR\\%d",i+1);
			//memset(szValue,0,sizeof(szValue));               //9.27 chehongyang
			//GetNTReg(tmppath,"DEVTYPE",szValue);
			//sprintf(tmpname1,"PPTRST%s",szValue);
			//sprintf(tmpname2,"PPTRTP%s",szValue);
			sprintf(tmpname1,"PPTRST%d",i);
			sprintf(tmpname2,"PPTRTP%d",i);
			sprintf(tmpname3,"PPTREC%d",i);
			
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"DEVTYPE",szValue);
			pDoc->raw_createElement((_bstr_t)(char*)tmpname2, &childNode1);
			childNode1->Puttext((_bstr_t)(szValue));//节点值
			xmlRoot->appendChild(childNode1);

			memset(szValue,0,sizeof(szValue));
 			GetNTReg(tmppath,"DEVSTAT",szValue);
			pDoc->raw_createElement((_bstr_t)(char*)tmpname1, &childNode1);
			childNode1->Puttext((_bstr_t)(szValue));//节点值
			xmlRoot->appendChild(childNode1);

			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"DEVERRCORE",szValue);
			pDoc->raw_createElement((_bstr_t)(char*)tmpname3, &childNode1);
			childNode1->Puttext((_bstr_t)(szValue));//节点值
			xmlRoot->appendChild(childNode1);
			
	}
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
	
	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_PTR,报文长度[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);
	return true;
}

bool PackXML::PackStat_IDC(char * PackStr,int * PackLen)
{

	char szValue[260];

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_IDC,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_IDC
	pDoc->raw_createElement((_bstr_t)(char*)"State_IDC", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\IDC","NUM",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DIDCNUM", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256];;
	int partnum=atoi(szValue);
	//for(int i=0;i<partnum;i++)
	for(int i = 1; i<= partnum; i++)
	{
		//sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\IDC\\%d",i);
		//memset(szValue,0,sizeof(szValue));        // 9.27 chehongyang
		//GetNTReg(tmppath,"DEVTYPE",szValue);
		//sprintf(tmpname1,"PIDCST%s",szValue);
		//sprintf(tmpname2,"PIDCTP%s",szValue);
		memset(szValue,0,strlen(szValue));
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\IDC\\%d",i);
		GetNTReg(tmppath,"LOGNAME",szValue);
		if(szValue[0]==NULL)
			break;
		sprintf(tmpname1,"PIDCST%d",i);
		sprintf(tmpname2,"PIDCTP%d",i);
		sprintf(tmpname3,"PIDCEC%d",i);
		

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVTYPE",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname2, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVSTAT",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname1, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVERRCORE",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname3, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

	}
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
	
	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);  //不复制根节点</root>，所以-2(字节)
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_IDC,报文长度为[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);
	return true;
}
bool PackXML::PackStat_CDM(char * PackStr,int * PackLen)
{
//	char packbuf[2048];
//	memset(packbuf,0x00,sizeof(packbuf));
	char szValue[260];

	char pcdmsttmp[10];
	memset(pcdmsttmp,0,sizeof(pcdmsttmp));

	memset(szValue,0,sizeof(szValue));
	GetCDMStat(szValue);

	//txtFieldAdd(packbuf,sizeof(packbuf),"PCDMST",szValue);
	strcpy(pcdmsttmp, szValue);

	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_CDM,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_CDM
	pDoc->raw_createElement((_bstr_t)(char*)"State_CDM", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);

	memset(szValue,0,sizeof(szValue));
	//GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","DEVSTAT",szValue);
	strcpy(szValue, pcdmsttmp);  //CDM整体状态，结合DISPENSER进行了缺纸、缺钞的转换
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"PCDMST", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","DEVERRCORE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DOEC", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1","UNITCOUNT",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"DONUM", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	int unitnum=atoi(szValue);
	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256],tmpname4[256],tmpname5[256],tmpname6[256];

	int sortindex[256];
	int j,k=1;
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
		//	break;
		if(strlen(szValue)!=NULL)
		{   memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"1"))//废钞箱
			{
				sortindex[j]=k;
				k++;
		    }
		}
	}
	/*  暂时去掉回收箱
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
			//break;
		if(strlen(szValue)!=NULL)
		{   memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"2"))//回收钞箱
			{
				sortindex[j]=k;
				k++;
			}
		}
	}
	*/
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
		//break;
		if(strlen(szValue)!=NULL)
		{   memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"3"))//取款钞箱
		{
			sortindex[j]=k;
			k++;
		}
		}
	}
	// 	for(j=0;j<unitnum;j++)
	// 	{
	// 		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
	// 		
	// 		memset(szValue,0,sizeof(szValue));
	// 		GetNTReg(tmppath,"TYPE",szValue);
	// 		if(!strcmp(szValue,"2"))//回收钞箱
	// 		{
	// 			sortindex[j]=k;
	// 			k++;
	// 		}
	// 	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
		//break;
		if(strlen(szValue)!=NULL)
		{   memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		if(!strcmp(szValue,"5"))//循环钞箱
		{
			sortindex[j]=k;
			k++;
		}
		}
	}

	int totalamount=0;
	int tmpval=0;
	int ivalue=0;
	int icount=0;
	char cxType[MAX_PATH];
	memset(cxType, 0, sizeof(cxType));
	for(int i=0;i<unitnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CDM\\1\\UNIT\\%d",i+1);


		//memset(szValue,0,strlen(szValue));
		//GetNTReg(tmppath,"UNITID",szValue);

		//if(szValue==NULL){
		//	continue;
		//}else if(strcmp(szValue,"")==0){
		//	continue;
		//}
	
		sprintf(tmpname1,"POTP%d",sortindex[i]);
		sprintf(tmpname2,"DOCURR%d",sortindex[i]);
		sprintf(tmpname3,"DOVL%d",sortindex[i]);
		sprintf(tmpname4,"DOCN%d",sortindex[i]);
		sprintf(tmpname5,"POST%d",sortindex[i]);
		sprintf(tmpname6,"DOVLT%d",sortindex[i]);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		strcpy(cxType, szValue);

		//只上送废钞箱和取款钞箱,添加上送回钞箱、循环钞箱
		//if((strcmp(szValue,"1"))&&(strcmp(szValue,"3"))&&(strcmp(szValue,"2"))&&(strcmp(szValue,"5")))
		if((strcmp(szValue,"1"))&&(strcmp(szValue,"3"))&&(strcmp(szValue,"5")))//先不送回收箱
			continue;

		pDoc->raw_createElement((_bstr_t)(char*)tmpname1, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);


		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"CURRENCY",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname2, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"VALUES",szValue);
		ivalue=atoi(szValue);
		//有的设备废钞箱会取出面值为-1，转换为0
		if(ivalue<0) ivalue = 0;
		//ivalue=ivalue/100;
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",ivalue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname3, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);
			
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"COUNT",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname4, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);
		icount=atoi(szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"STATUS",szValue);

		//如果是废钞箱，报出钞将尽、钞少的问题，将状态改为正常上送
		if( !strcmp(cxType, "1") )
		{
			if( !strcmp(szValue, "5") || !strcmp(szValue, "6") )
			{
				memset(szValue,0,sizeof(szValue));
				strcpy(szValue, "0");
			}
		}

		pDoc->raw_createElement((_bstr_t)(char*)tmpname5, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		//此钞箱的金额
		tmpval=ivalue*icount;

		//取款钞箱、循环钞箱才计入总额
		if( !strcmp(cxType, "3") || !strcmp(cxType, "5") )
		{
			totalamount+=tmpval;
		}
		

		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",tmpval);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname6, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

	}
	memset(szValue,0,sizeof(szValue));
	sprintf(szValue,"%d",totalamount);
	pDoc->raw_createElement((_bstr_t)(char*)"DOVLTT", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	

//CDM整体状态，在物理监测的基础上，对缺钞、钞少的业务逻辑转换  (暂时不做转换 by yfpeng)
// 	if((!strcmp(pcdmsttmp,"0")) || (!strcmp(pcdmsttmp,"1")) || (!strcmp(pcdmsttmp,"2")))
// 	{
// 		if(totalamount == 0)
// 		{
// 			strcpy(pcdmsttmp,"2");
// 		}
// 		else if(0<totalamount && totalamount<=40000)
// 		{
// 			strcpy(pcdmsttmp,"1");
// 		}
// 		else
// 			strcpy(pcdmsttmp,"0");
// 	}
	//pDoc->raw_createElement((_bstr_t)(char*)"PCDMST", &childNode1);
	//childNode1->Puttext((_bstr_t)(szValue));//节点值
	//xmlRoot->appendChild(childNode1);

	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
		
	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);

	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_CDM,报文长度[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);
	return true;
}

bool PackXML::PackStat_CIM(char * PackStr,int * PackLen)
{
//	char packbuf[2048];
//	memset(packbuf,0x00,sizeof(packbuf));
	char szValue[260];

	char pcimsttmp[10];
	memset(pcimsttmp,0,sizeof(pcimsttmp));

	memset(szValue,0,sizeof(szValue));
	GetCIMStat(szValue);
	//txtFieldAdd(packbuf,sizeof(packbuf),"PCIMST",szValue);
	strcpy(pcimsttmp,szValue);

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_CIM,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_IDC
	pDoc->raw_createElement((_bstr_t)(char*)"State_CIM", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","DEVSTAT",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"PCIMST", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","DEVERRCORE",szValue);
	//MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DIEC", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1","UNITCOUNT",szValue);
	//MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DINUM", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);


	int unitnum=atoi(szValue);
	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256],tmpname4[256],tmpname5[256],tmpname6[256];
	int sortindex[256];
	int j,k=1;

	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);

		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
			//break;
		if(strlen(szValue)!=NULL)
		{
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"1"))//废钞箱
			{
				sortindex[j]=k;
				k++;
			}
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
			//break;
		if(strlen(szValue)!=NULL)
		{
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"4"))//存款钞箱
			{
				sortindex[j]=k;
				k++;
			}
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
			//break;
		if(strlen(szValue)!=NULL)
		{
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"2"))//回收钞箱
			{
				sortindex[j]=k;
				k++;
			}
		}
	}
	for(j=0;j<unitnum;j++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",j+1);
		memset(szValue,0,strlen(szValue));
		GetNTReg(tmppath,"UNITID",szValue);
		//if(szValue[0]==NULL)
			//break;
		if(strlen(szValue)!=NULL)
		{
			memset(szValue,0,sizeof(szValue));
			GetNTReg(tmppath,"TYPE",szValue);
			if(!strcmp(szValue,"5"))//循环钞箱
			{
				sortindex[j]=k;
				k++;
			}
		}
	}

	int totalamount=0;
	int tmpval=0;
	int ivalue=0;
	int icount=0;
//	m_trace->info("The unitnum is %d,k= [%d]",unitnum,k);
    char cxType[MAX_PATH];
	memset(cxType, 0, sizeof(cxType));
	for(int i=0;i<unitnum;i++)
	{
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\CIM\\1\\UNIT\\%d",i+1);
		//memset(szValue,0,strlen(szValue));
		//GetNTReg(tmppath,"UNITID",szValue);

	//if(szValue==NULL){
		//continue;
	//}else if(strcmp(szValue,"")==0){
		//continue;
	//}	
		sprintf(tmpname1,"PITP%d",sortindex[i]);
		sprintf(tmpname2,"DICURR%d",sortindex[i]);
		sprintf(tmpname3,"DIVL%d",sortindex[i]);
		sprintf(tmpname4,"DICN%d",sortindex[i]);
		sprintf(tmpname5,"PIST%d",sortindex[i]);
		sprintf(tmpname6,"DIVLT%d",sortindex[i]);


		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"TYPE",szValue);
		strcpy(cxType, szValue);
		//只上送1废钞箱、2回钞箱、4存钞箱、5循环钞箱数据
		//if((strcmp(szValue,"1"))&&(strcmp(szValue,"4"))&&(strcmp(szValue,"2"))&&(strcmp(szValue,"5")))
		//只上送1废钞箱、4存钞箱
		if((strcmp(szValue,"1"))&&(strcmp(szValue,"4")))
		{
			continue;
		}
		pDoc->raw_createElement((_bstr_t)(char*)tmpname1, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"CURRENCY",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname2, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"VALUES",szValue);
		ivalue=atoi(szValue);
		//ivalue=ivalue/100;
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",ivalue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname3, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"COUNT",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname4, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);
		icount=atoi(szValue);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"STATUS",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname5, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		//这个钞箱的金额
		tmpval=ivalue*icount;

		//存款钞箱才计入总和
		if(!strcmp(cxType, "4"))
		{
			totalamount+=tmpval;
		}
		
		memset(szValue,0,sizeof(szValue));
		sprintf(szValue,"%d",tmpval);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname6, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);
	}
	memset(szValue,0,sizeof(szValue));
	sprintf(szValue,"%d",totalamount);
	pDoc->raw_createElement((_bstr_t)(char*)"DIVLTT", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	if((!strcmp(pcimsttmp,"0")) || (!strcmp(pcimsttmp,"1")))
	{
		if(totalamount >= 350000)
		{
			strcpy(pcimsttmp,"1");
		}
		else
			strcpy(pcimsttmp,"0");
	}
	//pDoc->raw_createElement((_bstr_t)(char*)"PCIMST", &childNode1);
	//childNode1->Puttext((_bstr_t)(szValue));//节点值
	//xmlRoot->appendChild(childNode1);

	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
	
	char msg[2048];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_CIM,报文长度[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);
	return true;
}

bool PackXML::PackStat_PIN(char * PackStr,int * PackLen)
{
//	char packbuf[2048];
//	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_PIN,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_IDC
	pDoc->raw_createElement((_bstr_t)(char*)"State_PIN", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\PIN","NUM",szValue);
	MSXML2::IXMLDOMElementPtr childNode1 ; 
	pDoc->raw_createElement((_bstr_t)(char*)"DPINNUM", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	char tmppath[256],tmpname1[256],tmpname2[256],tmpname3[256];
	int partnum=atoi(szValue);
	//for(int i=0;i<partnum;i++)
	for(int i=1;i<=partnum;i++)
	{
		//sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PIN\\%d",i+1);
		sprintf(tmppath,"SOFTWARE\\Ebring\\Agent\\DevStat\\PIN\\%d",i);
		//memset(szValue,0,sizeof(szValue));           //9.27 chehongyang
		//GetNTReg(tmppath,"DEVTYPE",szValue);
		//sprintf(tmpname1,"PPINST%s",szValue);
		//sprintf(tmpname2,"PPINTP%s",szValue);

		sprintf(tmpname1,"PPINST%d",i);
		sprintf(tmpname2,"PPINTP%d",i);
		sprintf(tmpname3,"PPINEC%d",i);
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVTYPE",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname2, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);
	
		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVSTAT",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname1, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

		memset(szValue,0,sizeof(szValue));
		GetNTReg(tmppath,"DEVERRCORE",szValue);
		pDoc->raw_createElement((_bstr_t)(char*)tmpname3, &childNode1);
		childNode1->Puttext((_bstr_t)(szValue));//节点值
		xmlRoot->appendChild(childNode1);

	}
	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
	
	char msg[1024];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);
	
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_PIN,报文长度[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);

	return true;
}
bool PackXML::PackStat_CHK(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_DEP(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_TTU(char * PackStr,int * PackLen)
{
	return true;
}
bool PackXML::PackStat_SIU(char * PackStr,int * PackLen)
{
//	char packbuf[2048];
//	memset(packbuf,0x00,sizeof(packbuf));

	char szValue[260];

	CoInitialize(NULL) ;
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	MSXML2::IXMLDOMElementPtr xmlRoot ;
	//创建DOMDocument对象
	HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30));
	if(!SUCCEEDED(hr)) 
	{ 
		m_trace->error("PackStat_SIU,无法创建DM对象");
		return false;
	} 
	
	//根节点的名称为State_IDC
	pDoc->raw_createElement((_bstr_t)(char*)"State_SIU", &xmlRoot);
	pDoc->raw_appendChild(xmlRoot, NULL);

 	MSXML2::IXMLDOMElementPtr childNode1 ;

// 	memset(szValue,0,sizeof(szValue));
// 	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\ETC","SAFEDOORSTA",szValue);
// 	pDoc->raw_createElement((_bstr_t)(char*)"PSAFEDOORST", &childNode1);
// 	childNode1->Puttext((_bstr_t)(szValue));//节点值
// 	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","SWITCH",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PSIUSWITCH", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","SAFEDOOR",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PSIUSAFEDOOR", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","UPSSTAT",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PSIUUPSST", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	memset(szValue,0,sizeof(szValue));
	GetNTReg("SOFTWARE\\Ebring\\Agent\\DevStat\\SIU","DEVERRCORE",szValue);
	pDoc->raw_createElement((_bstr_t)(char*)"PSIUEC", &childNode1);
	childNode1->Puttext((_bstr_t)(szValue));//节点值
	xmlRoot->appendChild(childNode1);

	BSTR aa;
	pDoc->get_xml(&aa);
	_bstr_t b=aa;	
		
	char msg[1024];
	memset(msg,0,sizeof(msg));
	strcpy(msg,b);
	memcpy(PackStr,msg,strlen(msg)-2);
	PackStr[strlen(msg)-2]='\0';
	*PackLen=strlen(PackStr);
		
	m_trace->WTrace(LOG_GRP, LOG_COMM, LT_DEBUG, "PackStat_SIU,报文长度[%d],为其分配数组长度[%d],PackStr=[%s]",strlen(PackStr),sizeof(msg),PackStr);
	return true;
}

int	PackXML::txtFieldGet( char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize )
{

	CoInitialize(NULL) ;                                                                                                                                                                                                                                                                                                       
	MSXML2::IXMLDOMDocumentPtr pDoc; 
	CString strName;
	try
	{
		HRESULT hr = pDoc.CreateInstance(_uuidof(DOMDocument30)); 
		if(!SUCCEEDED(hr)) 
		{  
			//MessageBox((LPCTSTR)"无法创建DOMDocument对象，请检查是否安装了MS XML Parser 运行库!"); 
			return -1;
		}
		
		VARIANT_BOOL bFlag; 
		BSTR inbuff=_bstr_t(FieldList);
		bFlag = pDoc->loadXML(inbuff);
		if(bFlag == S_FALSE)                                                                                                                                                                                                                                           
		{                                                                                                                                                                                                                                                                                                                          
			m_trace->error("txtFieldGet,打开XML对象失败!");
			return -1;	                                                                                                                                                                                                                                                                                                             
		} 
		MSXML2::IXMLDOMNodePtr  pNode=NULL;
		char nodename[30]={0};
		sprintf(nodename,"//%s",FieldName);

		BSTR node=_bstr_t(nodename);
		
		pNode=pDoc->selectSingleNode(node);
		//MSXML2::DOMNodeType nodeType; 
		
		//////得到节点类型 
		//pNode->get_nodeType(&nodeType); 
		
		////节点名称 
	
		
		strName=(char *)pNode->GetnodeName();
		
		//		BSTR variantValue;
		
		////取得节点的值
		CString value;
		value=(char *)pNode->Gettext();
		
        strcpy(ValueBuffer,value.GetBuffer(value.GetLength()));
		ValBufSize=value.GetLength();
		//m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,获取结点%s成功，nodevalue=%s",strName,ValueBuffer);
		pNode.Release();
	//	MessageBox(strName,value,MB_OK);
		//	pNode=NULL;
		
		
		//	pDoc=NULL;
	}
	catch (...)
	{
		//m_trace->WTrace(LOG_GRP, LOG_COMM, LT_ERROR, "txtFieldGet,获取结点%s失败",strName);
		return -1;
	}
	pDoc.Release();
	CoUninitialize();
	return ValBufSize;
// 	HRESULT hr = pDoc.CreateInstance(__uuidof(xml::DOMDocument)) ; //创建文档对象
// 	if(!SUCCEEDED(hr))
// 	{ 
// 		m_trace->error("txtFieldGet,无法创建DM对象");
// 		return -1;		                                                                                                                                                                                                                                                                                                           
// 	}                                                                                                                                                                                                                                                                                                                          
// 	VARIANT_BOOL bFlag; 
// 	BSTR inbuff=_bstr_t(FieldList);
// 	bFlag = pDoc->loadXML(inbuff);
// 	if(bFlag == S_FALSE)                                                                                                                                                                                                                                           
// 	{                                                                                                                                                                                                                                                                                                                          
// 		m_trace->error("txtFieldGet,打开XML对象失败!");
// 		return -1;	                                                                                                                                                                                                                                                                                                             
// 	}                                                                                                                                                                                                                                                                                                                          
// 	hr = pDoc->get_documentElement(&pElem) ; //获取根结点
// 	if(pElem == NULL)                                                                                                                                                                                                                                             
// 	{
// 		m_trace->error("txtFieldGet,获取根结点失败！");
// 		return -1;		                                                                                                                                                                                                                                                                                                             
// 	}                                                                                                                                                                                                                                                                                                                          
// 	BSTR strTagName = NULL;                                                                                                                                                                                                                                                                                                    
// 	hr = pElem->get_tagName(&strTagName) ;                                                                                                                                                                                                                                                                                     
// 	
// 	CString tagname(strTagName);                                                                                                                                                                                                                                                                                   
// 	SysFreeString(strTagName) ;                                                                                                                                                                                                                                                                                                
// 	hr = pElem->get_childNodes(&pNodeList) ; //获取子结点列表 
// 
// 	long lCount;                                                                                                                                                                                                                                              
// 	hr = pNodeList->get_length(&lCount) ;    
// 	m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,获取子结点列表成功！lCount=%d",lCount);
// 	for(long i=0; i<lCount; ++i)                                                                                                                                                                                                                                                                                               
// 	{                                                                                                                                                                                                                                                                                                                          
// 		xml::DOMNodeType NodeType ;                                                                                                                                                                                                                                                                                            
// 		xml::IXMLDOMNodeListPtr pChildNodeList ;                                                                                                                                                                                                                                                                               
// 		hr = pNodeList->get_item(i, &pNode) ; //获取结点 
// 		hr = pNode->get_nodeType(&NodeType) ; //获取结点信息的类型
// 	
// 		BSTR strValue ; 
// 		hr = pNode->get_nodeName(&strValue);
// 		
// 		_bstr_t tmp=strValue;
// 		char *nodename=tmp;
// 		SysFreeString(strValue);
// 		m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,获取结点%d成功，nodename=%s",i,nodename);
//  		if (strcmp(nodename,FieldName)==0)
//  		{			 
//  			hr = pNode->get_text(&strValue);
//  			_bstr_t tmpv=strValue;
// 			strcpy(ValueBuffer,tmpv);
// 			SysFreeString(strValue);
// 			m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,获取结点%d成功，nodetext=%s",i,ValueBuffer);
// 			pNode.Release() ;    
// 			//pChildNodeList.Release() ;
// 			pNodeList.Release() ; 
// 			pElem.Release() ;  
// 			pDoc.Release() ;   
// 			CoUninitialize() ;
// 			ValBufSize=strlen(ValueBuffer);
// 			return ValBufSize;
// 		}        
// 		
// 		if(xml::NODE_ELEMENT == NodeType)                                                                                                                                              
// 		{                                                                                                                                                                                                                                                                                                                      
// 			hr = pNode->get_childNodes(&pChildNodeList) ;                                                                                                                                                                                                                                                                      
// 			long childLen ;                                                                                                                                                                                                                                                                                                    
// 			pChildNodeList->get_length(&childLen) ;                                                                                                                                                                                                                                                                            
// 			//设置宽字符区域（类似于国域)chs代表简体中文..可以在msdn上查询 
// 			wcout.imbue(locale("chs")) ;                                                                                                                                                                                                         
// 			//	AfxMessageBox("------NodeList------" ) ;                                                                                                                                                                                                                                                                           
// 			for (int j=0; j<childLen; ++j)                                                                                                                                                                                                                                                                                     
// 			{                                                                                                                                                                                                                                                                                                                  
// 				xml::IXMLDOMNodePtr pChildNode ;                                                                                                                                                                                                                                                                               
// 				BSTR strValue ;                                                                                                                                                                                                                                                                                                
// 				hr = pChildNodeList->get_item(j, &pChildNode) ;                                                                                                                                                                                                                                                                
// 				hr = pChildNode->get_nodeName(&strValue) ; //获取结点名字 
// 			
// 				_bstr_t a1=strValue;
// 				char *nodename1=a1;
// 				SysFreeString(strValue);
// 				
// 				if (strcmp(nodename1,FieldName)==0)
// 				{
// 					hr = pChildNode->get_text(&strValue) ; //获取结点的值
// 					_bstr_t tmpv1=strValue;
// 					
// 					strcpy(ValueBuffer,tmpv1);
// 					SysFreeString(strValue) ;                                                                                                                                                                                                                                                                                      
// 					pChildNode.Release() ;
// 					ValBufSize=strlen(ValueBuffer);
// 					return ValBufSize;	
// 					m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,获取结点%d成功，nodename=%s,nodetext=%s",j,nodename1,ValueBuffer);
// 					pNode.Release() ;                                                                                                                                                                                                                                                                                                      
// 					//pChildNodeList.Release();
// 					pNodeList.Release() ;                                                                                                                                                                                                                                                                                                      
// 					pElem.Release() ;                                                                                                                                                                                                                                                                                                          
// 					pDoc.Release() ;                                                                                                                                                                                                                                                                                                           
// 					CoUninitialize() ;
// 					ValBufSize=strlen(ValueBuffer);
// 					return ValBufSize;					
// 				}
// 			}                                                                                                                                                                                                                                                                                                                  
// 		}                                                                                                                                                                                                                                                                                                                      
// 		pNode.Release();                                                                                                                                                                                                                                                                                                      
// 		pChildNodeList.Release();                                                                                                                                                                                                                                                                                             
// 	}                                                                                                                                                                                                                                                                                                                          
// 	pNodeList.Release();                                                                                                                                                                                                                                                                                                      
// 	pElem.Release();                                                                                                                                                                                                                                                                                                          
// 	pDoc.Release();                       
//     CoUninitialize();
// 	m_trace->WTrace("EbringAgent","EbringAgent.log",LT_DEBUG,"txtFieldGet,99999999999999999999999999999999999return -1");
// 	return -1;
}
