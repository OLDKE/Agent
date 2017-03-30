// PackXML.h: interface for the PackXML class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKXML_H__7DBAFD2B_9296_4559_93A7_D7870774460B__INCLUDED_)
#define AFX_PACKXML_H__7DBAFD2B_9296_4559_93A7_D7870774460B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "NTTRACE.h" 
#include "LogicCommon.h"
#include "MyDes.h"

//#include "MyDes.h"
#include <MsXml2.h>

class PackXML  
{
public:
	PackXML(CNTTRACE *pTrace);
	virtual ~PackXML();

	CNTTRACE *m_trace;
	CMyDes *m_mydes;

//	CMyDes *m_mydes;
	int UnPack( char *unpackstr, int unpackstrlen, PVMSG pvmsg );
	int UnPackMsg(char *unpackstr, int unpackstrlen, PVMSG vmsg);

	int	txtFieldGet( char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize );
	bool PackStat(char *PackStr,int *PackLen);
	bool PackComd(char *PackStr, int *PackLen);
	bool PackTRN( int &pos,char *PackStr, int *PackLen);
	bool PackSNR(int &pos, char *PackStr, int *PackLen);
	bool PackTRN_resp(int TranIndex,char *PackStr, int *PackLen);
	bool PackRTC(char* rpttime,char *PackStr, int *PackLen);
	bool PackRTC_resp(int RtcIndex,char *PackStr, int *PackLen);
	bool PackComdRet(char* PackStr,char* Trncode,char* Comd_Res,char *TRNEJ,char *errmsg, PVMSG vmsg=NULL);
	bool PackComdRet2(char* PackStr,char* Trncode,char* Comd_Res,char *TRNEJ);

	bool GetTRNEJ(char *PackStr);
	//CCriticalSection CriticalSection;
	bool PackStat_PTR(char * PackStr,int * PackLen);
	bool PackStat_IDC(char * PackStr,int * PackLen);
	bool PackStat_CDM(char * PackStr,int * PackLen);
	bool PackStat_PIN(char * PackStr,int * PackLen);
	bool PackStat_CHK(char * PackStr,int * PackLen);
	bool PackStat_DEP(char * PackStr,int * PackLen);
	bool PackStat_TTU(char * PackStr,int * PackLen);
	bool PackStat_SIU(char * PackStr,int * PackLen);
	bool PackStat_VDM(char * PackStr,int * PackLen);
	bool PackStat_CAM(char * PackStr,int * PackLen);
	bool PackStat_ALM(char * PackStr,int * PackLen);
	bool PackStat_CEU(char * PackStr,int * PackLen);
	bool PackStat_CIM(char * PackStr,int * PackLen);
	bool GetCDMStat(char *statbuf);
	bool GetCIMStat(char *statbuf);

	//ͨ���ı�������ʽ�����ݱ����е��ֶ�������ȡ�ֶ�ֵ
	int GetFieldValue(char *Msgbuf, char *FieldName, char *FieldValue);

	//У�鱨���е��豸���뱾���豸���Ƿ�һ��
	bool CheckDevID(const char *MsgDevID);

};

#endif // !defined(AFX_PACKXML_H__7DBAFD2B_9296_4559_93A7_D7870774460B__INCLUDED_)
