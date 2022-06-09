#pragma once
#include "StdAfx.h"
#include "DlgPopError.h"
//该类主要用于C结构体的表面和尺寸的综合判断以及尺寸结构体的显示
const int NO_MARK   = 0;
const int UP_MARK   = 1;
const int DOWN_MARK = 2;
enum AT9Type
{
	NormalDataBack,//正常闭环
	MeanDataBack,//均值闭环
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
	void Action(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);//每次过来一个C结构体供分析最终结果
	void CorrectSizeResult (HTuple hv_SizeDetect, HTuple hv_SizeStandard, HTuple hv_CorrectThresLow,HTuple hv_CorrectThresHigh, HTuple *hv_SizeCorrect);
private:
	void ReadParam();
	//判断尺寸和表面使用的函数
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
	SIZERESULT    m_tSizeResult;   //尺寸结果
	FINALRESULT   m_tFinalResult;  //最终结果
	SURFACERESULT m_tSurfaceResult; //存储单条的表面缺陷和类型
	int  m_nLastImageSN,m_nExeptLastSN;
	double m_dLastCropRow,m_dExeptLastCropRow;
	int           m_nFoilSN;           
public:
	BOOL m_bCancelLabelFlag;//手动确认取消当前EA贴标
	int  m_nContinueEANGNum;//连续超长EA报警数目
	CString       m_strConfigFile; //配置文件
	INITJUDGE       m_tInit;           //类的序号
	double          m_dMinDist;           
	STANDARDSIZE    m_tStandardSize; //标准尺寸
	STANDARDSURFACE m_tStandardSurface;//标准的表面二次判断
	HTuple          m_hvErrorTypeCount;
	HTuple          m_hvComPareTuple;//比较数组 对比数组 用来区分EA
	int              m_nContinuAt9NGNum;//ATNG数目
	//CPK相关
	HTupleVector m_hvec_CPK;
	HTuple m_hvPerATCuoWei;
	HTuple m_hvCPK;
};

