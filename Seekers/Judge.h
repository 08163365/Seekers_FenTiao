#pragma once
#include "StdAfx.h"
#include "DlgPopError.h"
//������Ҫ����C�ṹ��ı���ͳߴ���ۺ��ж��Լ��ߴ�ṹ�����ʾ
const int NO_MARK   = 0;
const int UP_MARK   = 1;
const int DOWN_MARK = 2;
enum AT9Type
{
	NormalDataBack,//�����ջ�
	MeanDataBack,//��ֵ�ջ�
};

const CString STR_TestResult[]={"NG","OK"};
class CJudge
{
public:
	CJudge(void);
	~CJudge(void);

public:
	void Init(const INITJUDGE& tInit);
	BOOL ShowParam();
	void WriteParam();
	void Action(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);//ÿ�ι���һ��C�ṹ�幩�������ս��
	void CorrectSizeResult (HTuple hv_SizeDetect, HTuple hv_SizeStandard, HTuple hv_CorrectThresLow,HTuple hv_CorrectThresHigh, HTuple *hv_SizeCorrect);
private:
	void ReadParam();
	//�жϳߴ�ͱ���ʹ�õĺ���
	BOOL GetSurfaceResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);
	void SegErrorRegion(HObject hoImage,HTuple hvMarkRow,HObject hoErrorRegion,HTuple hvErrorType,HTuple hvErrorBelongTrips,HObject* ho_UpErrorRegion,HObject* ho_DownErrorRegion,HTuple* hvUpErrorType,HTuple*hvDownErrorType,HTuple* hvUpErrorBelongTrips,HTuple*hvDownErrorBelongTrips);
	void ConcatErrorRegion(HObject ho_UpErrorRegion,HTuple hv_UpErrorType,HTuple hv_UpErrorBelongtrips);
	void ComPareSize(HTuple hv_StandardSize,HTuple hv_StandardDev,HTuple hv_RealDetectSize,e_ErrorType ErrorType,C_ALLOCATORWORKSHOPOUTPUT& tOutput,BOOL& bRet,int nSN,int nTrip);
	
	void PreClear();

	void JustWarnOrLabel(BOOL bEnable,BOOL bEnableLabel,HTuple hv_StandardSize,HTuple hv_StandardDev,HTuple hv_RealDetectSize,e_ErrorType ErrorType,C_ALLOCATORWORKSHOPOUTPUT& tOutput,BOOL& bRet,int nSN,BOOL bAllNGFlag=FALSE);
	void GetCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut,BOOL& bRet);
	void GetCoatAndFoiliSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, BOOL& bRet);

	void CalCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, CAOWEISIZE& tCaoWeiStandardParam, int nSN, BOOL& bRet);
	void CalCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, STANDARDSIZE& tCoatAndFoilStandardParam,int nSN,BOOL& bRet);


	void CalculateCPK(int& nTrip,HTuple hv_RealValue,double dStandarValueValue,double dStandarDeviationValue,HTuple* hv_MeanValue,HTuple* hv_DeviationValue,HTuple* hv_CpkValue);
private:
	SIZERESULT    m_tSizeResult;   //�ߴ���
	FINALRESULT   m_tFinalResult;  //���ս��
	SURFACERESULT m_tSurfaceResult; //�洢�����ı���ȱ�ݺ�����
	int  m_nLastImageSN,m_nExeptLastSN;
	double m_dLastCropRow,m_dExeptLastCropRow;
	int           m_nFoilSN;           
public:
	BOOL m_bCancelLabelFlag;//�ֶ�ȷ��ȡ����ǰEA����
	int  m_nContinueEANGNum;//��������EA������Ŀ
	CString       m_strConfigFile; //�����ļ�
	INITJUDGE       m_tInit;           //������
	double          m_dMinDist;           
	STANDARDSIZE    m_tStandardSize; //��׼�ߴ�
	STANDARDSURFACE m_tStandardSurface;//��׼�ı�������ж�
	HTuple          m_hvErrorTypeCount;
	HTuple          m_hvComPareTuple;//�Ƚ����� �Ա����� ��������EA
	int              m_nContinuAt9NGNum;//ATNG��Ŀ
	//CPK���
	HTupleVector m_hvec_CPK;
	HTuple m_hvPerATCuoWei;
	HTuple m_hvCPK;
};

