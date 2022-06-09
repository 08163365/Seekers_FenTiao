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
	CString strAddressData;//���ݶ˿�
	CString strAddressCom;//ͨ�ÿ���Э��
	CString strOffsetServerIP;//���۷�������Э��
	int     nOffsetServerPort;//���ۿ��ƶ˿�
	int     nBackDataSourceIndex;//����ǰ�ջ����ݷ�������Դ
	int     nPortData[CAMERA_COUNT];//2�����,���������������˿ڷ���ͼ��
	int     nPortCom;
	int     bUseJeket;
	double  dMaxDataLimit;
	int     nNGNumToStopMachine;//ͣ��������
	BOOL    bEnableDYOffset;//�Ƿ����õ��۱ջ�
}UploadParam;


class CUpload //�ϴ����������˿�
{
public:
	CUpload();
	~CUpload();

public:
	CString GetFileVersion(CString strFileName);
	static void* HThreadUpload(void *hcthread);//io�߳�
	void   GetGongWeiName(const C_ALLOCATORWORKSHOPOUTPUT &tOutput,CString& strComputerName);
	void   T_Upload(HTuple hvQueue);
	BOOL   Init();
	void   Upload(C_ALLOCATORWORKSHOPOUTPUT& C);
	BOOL   ReadParam();
	BOOL   WriteParam();
	void   SetWarnToServer();//����NG����
	void   SendClosedData(C_ALLOCATORWORKSHOPOUTPUT &Output);//���ͱջ�����
	void   SendExceptionWarn(int& nGrabberSN,CString strException);//�쳣����ͣ��
	void   CalculateCPK(int& nTrip,double& dRealValue,double dStandarValueValue,double dStandarDeviationValue,HTuple* hv_MeanValue,HTuple* hv_DeviationValue,HTuple* hv_CpkValue);
	void   CountContinueErrorNum(C_ALLOCATORWORKSHOPOUTPUT &Output);//ͳ������������Ŀ

	BOOL   IsInSpotTime();//�Ƿ��ڵ��ʱ��
public:

	HTuple m_hvQueue;
	UploadParam m_tParam;
	CString m_strConfigPath;
	CString m_StrGongWeiName;//��λ����
	HTuple   m_hvErrorCount[2];//���������ۼ�
	CRITICAL_SECTION m_csChange;
};

