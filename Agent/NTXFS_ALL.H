/******************************************************************************
*                                                                             *
* NTXFS_ALL.h      XFS - NanTian XFS  definitions                        *
*                                                                             *
*               Version 3.00 (5/8/08)                                       *
* Author:lxf     2008-5-8                                                                       *
******************************************************************************/
/*=================================================================*/
/* CDM Info Command Structures */
/*=================================================================*/

//#define MANAGE_REQUIRED_VERSIONS			0x20002 
//#define SERVICE_REQUIRED_VERSIONS			0x20992 


#define MANAGE_VERSIONS     0x00030303//       1100000000001100000011
#define SERVICE_VERSIONS    0x3020303//110000001000000011000000110011

typedef struct _ntxfs_cdm_unit
{
	char	Type[2];
	char	UnitID[16];
	char	Currency[16];
	char	Values[16];
	char	Count[16];
	char	Status[4];
	char	Statustime[16];		//add by weinv
} NTXFS_CDM_UNIT, * LPNTXFS_CDM_UNIT;

typedef struct _ntxfs_cdm_log
{
	char	LogName[32];
	char	DevStat[2];
	char	DevStattime[16];	//add by weinv
	char	DevType[2];
	char	DevErrCode[5];
	char	SafeDoor[2];
	char	SafeDoortime[16];	//add by weinv
	char	Dispenser[2];
	char	Dispensertime[16];	//add by weinv
	short	UnitCount;
	NTXFS_CDM_UNIT ntxfs_cdm_unit[10];

} NTXFS_CDM_LOG, * LPNTXFS_CDM_LOG;

typedef struct _ntxfs_cdm
{
	short	LogCount;
	NTXFS_CDM_LOG ntxfs_cdm_log[5];
} NTXFS_CDM, * LPNTXFS_CDM;

typedef struct _ntxfs_idc_log
{
	char	LogName[32];
	char	DevStat[2];
	char	DevStattime[16];	//add by weinv	
	char	DevType[2];
	char	DevErrCode[5];
	
} NTXFS_IDC_LOG, * LPNTXFS_IDC_LOG;

typedef struct _ntxfs_idc
{
	short	LogCount;
	NTXFS_IDC_LOG ntxfs_idc_log[5];
} NTXFS_IDC, * LPNTXFS_IDC;

typedef struct _ntxfs_ptr_log
{
	char	LogName[32];
	char	DevStat[2];
	char	DevStattime[16];	//add by weinv
	char	DevType[2];
	char	DevErrCode[5];
	
} NTXFS_PTR_LOG, * LPNTXFS_PTR_LOG;

typedef struct _ntxfs_ptr
{
	short	LogCount;
	NTXFS_PTR_LOG  ntxfs_ptr_log[5];
} NTXFS_PTR, * LPNTXFS_PTR;

typedef struct _ntxfs_pin_log
{
	char	LogName[32];
	char	DevStat[2];
	char	DevStattime[16];	//add by weinv
	char	DevType[2];
	char	DevErrCode[5];
	
} NTXFS_PIN_LOG, * LPNTXFS_PIN_LOG;

typedef struct _ntxfs_pin
{
	short	LogCount;
	NTXFS_PIN_LOG  ntxfs_pin_log[5];
} NTXFS_PIN, * LPNTXFS_PIN;

typedef struct _ntxfs_cim_unit
{
	char	Type[2];
	char	UnitID[16];
	char	Currency[16];
	char	Values[16];
	char	Count[16];
	char	Status[4];
	char	Statustime[16];		//add by weinv
} NTXFS_CIM_UNIT, * LPNTXFS_CIM_UNIT;

typedef struct _ntxfs_cim_log
{
	char	LogName[32];
	char	DevStat[2];
	char	DevStattime[16];	//add by weinv
	char	DevType[2];
	char	DevErrCode[5];
	char	SafeDoor[2];
	char	SafeDoortime[16];	//add by weinv
	char	Acceptor[2];
	char	Dispensertime[16];	//add by weinv
	short	UnitCount;
	char	Dispenser[2];
	NTXFS_CIM_UNIT ntxfs_cim_unit[10];
	
} NTXFS_CIM_LOG, * LPNTXFS_CIM_LOG;

typedef struct _ntxfs_cim
{
	short	LogCount;
	NTXFS_CIM_LOG ntxfs_cim_log[5];
} NTXFS_CIM, * LPNTXFS_CIM;
