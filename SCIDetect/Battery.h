

#include "Judge.in.h"
#include "Detect.h"
#pragma once
extern CBlendent g_Blendent;
extern int g_nLaneCount;
extern CStringArray g_StrArrayErrorName;

class CBattery : public DHInterface
{
public:
	CBattery(void);
	~CBattery(void);
	CBattery& operator= (const CBattery& in);

public:
	void InitDetect();//ִ�м�⺯��֮ǰ�Ĳ���
public: 

	BOOL Separate();
	void CrudeBreak();

public:
	PARAMETERS m_tParameters;
	virtual int DoSomething(void* message,void* wParam,void* lParam);//�������
	int  Init(DETECTINIT &In) ;//��ʼ���㷨
	int  Detect(DETECTINPUT &tIn,DETECTOUTPUT *tOut,int nBreakType = 0); //����㷨                      
	int  DisplaySetDialog(DETECTINPUT & tIn);//��ʾ�㷨�Ի���
	void DIYErrorOut();//�Զ���ȱ�ݵ������Ŀ
	void SaveException(HException e,CString strFuncName);//�����쳣
	void ReadDeFaultParameters();//By��ԭ����
public:
	//Ԥ������
	BOOL MeasureSize();//��ȡ����ͼ�������ϱ߽磬�����ϱ߽磬ͭ���ߴ磬Ϳ���ߴ�
public:
	
	void RectyfyIntermission(HTuple hv_LocationInfo);//���ݴ�������Ϣ���е�����Ъ�ĸ߶�
	//void WriteFirstData(InterOutPut& tInterOutPut);





public:

	MYREGION   m_tRegion;
	BOOL       m_bPoweFlag;//Ȩ�޲���

	MARKVECTOR m_ModelInfoVector;
	SEPARATEOUTPUT    m_tSeparateOut;//�ָ������
	SEPARATEINPUT     m_tSeparateIn;//�ָ�������
	DETECTOUTPUT*     m_ptDetectOut;
	DETECTINPUT*      m_ptDetectIn;

	
	BOOL ReadParameters();
	BOOL WriteParameters();
	CRITICAL_SECTION m_csChange;//������Ķ�������������ĵ�ʱ����Ҫ��ס����ֹ����ϵͳ�Ǳ����ڼ�飬�������ȥ���Ĳ���
	HObject m_hoMeanMidImage;
	HTuple m_hvHomMat2D;
	CString m_strModelPath;
	DETECTINIT m_tInit;

public:
	BOOL          m_bExit;
	HObject m_hoAdaptedImage;//�Ҷ�����Ӧ��ͼ
public:
	CString m_strSystemVis;
	
public:
	CString m_strParamFile;//覴ò���Ŀ¼
	double m_dRowResolution,m_dColResolution ;
	HObject m_hoAt9Region;
	HObject m_hoCoatMid;//Ϳ������
	HObject m_hoBoderRegion;//�߽�����
	HObject m_hoSegRegions;
	HTuple  m_hvPeakGrayMid;
	HObject m_hoLEnableChoiceRegions;
	HObject m_hoDEnableChoiceRegions;
	HTuple m_hvErrorName,m_hvStrFeature;
	BlackRegion m_tBlackRegion;//��ɫ����
	STANDARDSIZE m_tStandardSize;
	HTuple   m_hvReverseTimeCount;
	double   m_dMeanValue;
public:
	BOOL GetDynRegion (HObject& ho_Image);
	void GetChoiceRegions(HObject& hoLChoiceRegions, HObject& hoDChoiceRegions);
	void GetDetetcRegion();//��ȡ���ɵ�����
	void AT9Detect();
	void TuoWeiDetect();//��β���


	void LeakFoilDetect();
	void BubbleDetect();
	void LightLineDetect();
	void BlackLineDetect();
	void LightDotDetect();
	void BlackDotDetect();
	void TapeDetect();
	void FoilDetect();
	BOOL JiaoDai();
	void SelectLine (HObject ho_AllLineRegion, HObject *ho_SelectLineRegion, HTuple hv_StandarWidth,HTuple hv_StandarLength, HTuple *hv_WidthValue, HTuple *hv_LengthValue);
	BOOL AddError(HObject ho_ErrorObject,e_ErrorType nErrorCode,int nType=0);//0��Ϳ�� 1�Ǽ��� 2��AT9
	BOOL AddCalssfiyError(HObject ho_ErrorObject,e_ErrorType nErrorCode,int nType=0);//0��Ϳ�� 1�Ǽ��� 2��AT9



public:
	////��������غ���//////////////////////////////////////////////////////////////////////////
	void classify(HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName,
		HTuple hv_RegionFeatureName, HTuple *hv_Classes, HTuple *hv_Confidence);//ʹ�÷�����

	void get_features(HObject ho_Region, HObject ho_Image, HTuple hv_GrayFeaturesName,
		HTuple hv_RegionFeaturesName, HTuple *hv_Features);//��ȡ����
public:
	CLineToolPara m_sLineTool[LINE_COUNT];//�ֱ�Ѱ��
	CSeparate  m_CSeperate;

public:
	HObject m_hoTempErrorRegion;
	CString  m_strMLPPath;
	MLPParam  m_tMLPParam;//����������
public:
	void FlushFlag();
	BOOL    m_bExistJXFlag;
	BOOL    m_bExistOAPFlag;
	BOOL    m_bExistCaoWeiFlag;
	BOOL    m_bExistJXUpDownFlag;
};

