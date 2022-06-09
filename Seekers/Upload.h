#include "StdAfx.h"
#pragma once

#define HALCON_TRY(__FUNC__) \
try \
{ \
(__FUNC__); \
} \
catch(HException&){ \
}

#define CAMERA_COUNT 2

void T_Connect();
BOOL IsValidFileName(const std::string& strFileName);
struct tagTSENDDATA
{
	//double
};
typedef struct tagTUploadParam
{
	CString strAddressData;//数据端口
	CString strAddressCom;//通用控制协议
	CString strOffsetServerIP;//电眼反馈控制协议
	int     nOffsetServerPort;//电眼控制端口
	int     nBackDataSourceIndex;//激光前闭环数据反馈数据源
	int     nPortData[CAMERA_COUNT];//2个相机,所以这里是两个端口发送图像
	int     nPortCom;
	int     bUseJeket;
	double  dMaxDataLimit;
	int     nNGNumToStopMachine;//停机报警数
	BOOL    bEnableDYOffset;//是否启用电眼闭环
}UploadParam;


class CUpload //上传给服务器端口
{
public:
	CUpload();
	~CUpload();

public:
	CString GetFileVersion(CString strFileName);
	static void* HThreadUpload(void *hcthread);//io线程
	void   GetGongWeiName(const C_ALLOCATORWORKSHOPOUTPUT &tOutput,CString& strComputerName);
	void   T_Upload(HTuple hvQueue);
	BOOL   Init();
	void   Upload(C_ALLOCATORWORKSHOPOUTPUT& C);
	BOOL   ReadParam();
	BOOL   WriteParam();
	void   SetWarnToServer();//连续NG报警
	void   SendClosedData(C_ALLOCATORWORKSHOPOUTPUT &Output);//发送闭环数据
	void   SendExceptionWarn(int& nGrabberSN,CString strException);//异常报警停机
	void   CalculateCPK(int& nTrip,double& dRealValue,double dStandarValueValue,double dStandarDeviationValue,HTuple* hv_MeanValue,HTuple* hv_DeviationValue,HTuple* hv_CpkValue);
	void   CountContinueErrorNum(C_ALLOCATORWORKSHOPOUTPUT &Output);//统计连续报警数目

	BOOL   IsInSpotTime();//是否处在点检时间
public:

	HTuple m_hvQueue;
	UploadParam m_tParam;
	CString m_strConfigPath;
	CString m_StrGongWeiName;//工位名称
	HTuple   m_hvErrorCount[2];//错误类型累计
	CRITICAL_SECTION m_csChange;
};

