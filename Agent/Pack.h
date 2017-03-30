#pragma once
#define	__FLDSEP	';'
#define	__FLDEQU	'='
#define	__FLDAPT	':'

#define	__FLDRC_OK	0
#define	__FLDRC_PARAM	-1
#define	__FLDRC_NFND	-2
#define	__FLDRC_SIZE	-3

#include <afxmt.h>
#include "NTTRACE.h"
class Pack
{
public:
	Pack(CNTTRACE *pTrace);
	virtual ~Pack(void);

	CNTTRACE *m_trace;

	int UnPack(char *unpackstr, int unpackstrlen);
	bool PackStat(char *PackStr,int *PackLen);
	bool PackComd(char *PackStr, int *PackLen);
	bool PackTRN( char* rpttime,char *PackStr, int *PackLen);
	bool PackSNR(char* rpttime, char *PackStr, int *PackLen);
	bool PackTRN_resp(int TranIndex,char *PackStr, int *PackLen);
	bool PackRTC(char* rpttime,char *PackStr, int *PackLen);
	bool PackRTC_resp(int RtcIndex,char *PackStr, int *PackLen);
	bool PackComdRet(char* PackStr,char* Trncode,char* Comd_Res,int TranEJflag);
	char * GetCmdCode_curr();
	char * GetCmdCode_tmp();
	char * GetCmdRes();
	int checksendcmd();
	char SofVer[10];            //ATM软件版本
private:
	char LogPath[64];			//日志路径
	char AppPath[64];			//程序路径
	char CardTime[15];			//吞卡上送时间
	char CmdCode_curr[32];		//当前命令
	char CmdCode_tmp[32];		//临时命令
	char CmdRes[32];			//命令结果
	bool GetTRNEJ(char *PackStr);
	CCriticalSection CriticalSection;
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

//for txtfld报文 begin
	int	txtFieldSeek( char *FieldList, char *FieldName );
	int	txtFieldGet(  char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize );
	int	txtFieldPut(  char *FieldList, int LstBufSize,char *FieldName, char *FieldValue );
	int	txtFieldClear(char *FieldList, char *FieldName );
	int	txtFieldTake( char *FieldList, char *FieldName,char *ValueBuffer, int ValBufSize );
	int	txtFieldAdd(  char *FieldList, int LstBufSize,const char *FieldName, char *FieldValue );
	int	txtFieldFirst(char *FieldList, char *NameBuffer,int NamBufSize, char *ValueBuffer, int ValBufSize );
	int	txtFieldSkip( char *FieldList );
	int	txtParseValue(char *Value, char *CodeBuffer,int CodBufSize, char *DescBuffer, int DscBufSize );
	int	txtPackAdd( char *FieldList,char *Packbuf );
//for txtfld报文 end
};
