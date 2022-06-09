

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
	void InitDetect();//执行检测函数之前的操作
public: 

	BOOL Separate();
	void CrudeBreak();

public:
	PARAMETERS m_tParameters;
	virtual int DoSomething(void* message,void* wParam,void* lParam);//函数入口
	int  Init(DETECTINIT &In) ;//初始化算法
	int  Detect(DETECTINPUT &tIn,DETECTOUTPUT *tOut,int nBreakType = 0); //检测算法                      
	int  DisplaySetDialog(DETECTINPUT & tIn);//显示算法对话框
	void DIYErrorOut();//自定义缺陷的输出项目
	void SaveException(HException e,CString strFuncName);//保存异常
	void ReadDeFaultParameters();//By卷还原参数
public:
	//预处理函数
	BOOL MeasureSize();//获取整张图的左余料边界，右余料边界，铜箔尺寸，涂布尺寸
public:
	
	void RectyfyIntermission(HTuple hv_LocationInfo);//根据传进来信息进行调整间歇的高度
	//void WriteFirstData(InterOutPut& tInterOutPut);





public:

	MYREGION   m_tRegion;
	BOOL       m_bPoweFlag;//权限操作

	MARKVECTOR m_ModelInfoVector;
	SEPARATEOUTPUT    m_tSeparateOut;//分割后的输出
	SEPARATEINPUT     m_tSeparateIn;//分割后的输入
	DETECTOUTPUT*     m_ptDetectOut;
	DETECTINPUT*      m_ptDetectIn;

	
	BOOL ReadParameters();
	BOOL WriteParameters();
	CRITICAL_SECTION m_csChange;//当该类的对象参数发生更改的时候，需要锁住，防止出现系统那边正在检查，这边正好去更改参数
	HObject m_hoMeanMidImage;
	HTuple m_hvHomMat2D;
	CString m_strModelPath;
	DETECTINIT m_tInit;

public:
	BOOL          m_bExit;
	HObject m_hoAdaptedImage;//灰度自适应的图
public:
	CString m_strSystemVis;
	
public:
	CString m_strParamFile;//瑕疵参数目录
	double m_dRowResolution,m_dColResolution ;
	HObject m_hoAt9Region;
	HObject m_hoCoatMid;//涂布中心
	HObject m_hoBoderRegion;//边界区域
	HObject m_hoSegRegions;
	HTuple  m_hvPeakGrayMid;
	HObject m_hoLEnableChoiceRegions;
	HObject m_hoDEnableChoiceRegions;
	HTuple m_hvErrorName,m_hvStrFeature;
	BlackRegion m_tBlackRegion;//黑色区域
	STANDARDSIZE m_tStandardSize;
	HTuple   m_hvReverseTimeCount;
	double   m_dMeanValue;
public:
	BOOL GetDynRegion (HObject& ho_Image);
	void GetChoiceRegions(HObject& hoLChoiceRegions, HObject& hoDChoiceRegions);
	void GetDetetcRegion();//获取生成的区域
	void AT9Detect();
	void TuoWeiDetect();//拖尾检测


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
	BOOL AddError(HObject ho_ErrorObject,e_ErrorType nErrorCode,int nType=0);//0是涂布 1是极耳 2是AT9
	BOOL AddCalssfiyError(HObject ho_ErrorObject,e_ErrorType nErrorCode,int nType=0);//0是涂布 1是极耳 2是AT9



public:
	////分类器相关函数//////////////////////////////////////////////////////////////////////////
	void classify(HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName,
		HTuple hv_RegionFeatureName, HTuple *hv_Classes, HTuple *hv_Confidence);//使用分类器

	void get_features(HObject ho_Region, HObject ho_Image, HTuple hv_GrayFeaturesName,
		HTuple hv_RegionFeaturesName, HTuple *hv_Features);//获取特征
public:
	CLineToolPara m_sLineTool[LINE_COUNT];//分别寻边
	CSeparate  m_CSeperate;

public:
	HObject m_hoTempErrorRegion;
	CString  m_strMLPPath;
	MLPParam  m_tMLPParam;//分类器参数
public:
	void FlushFlag();
	BOOL    m_bExistJXFlag;
	BOOL    m_bExistOAPFlag;
	BOOL    m_bExistCaoWeiFlag;
	BOOL    m_bExistJXUpDownFlag;
};

