
#include <afxtempl.h>
#ifndef _DEFINE_GEICHECK
#define _DEFINE_GEICHECK
#include <vector>
#include "LineToolPara.h"

#define LINE_COUNT  22

const int MAX_LANE  = 1; //最多分切16条
#define M_PI       3.14159265358979323846


// 分类器的枚举
enum CLASSTYPE
{
	JIAODAI_C,
	LOUBO_C,
	TUOTAN_C,
	TIAOHEN_C,
	QIPAO_C,
	BAIDIAN_C,
	HEIDIAN_C,
	OK_C,
	ClassTotalNum,
};
const CString g_strClassName[] =
{
	"胶带",
	"漏箔",
	"脱碳",
	"条痕",
	"气泡",
	"白点",
	"黑点",
	"OK",
};
struct MLPParam
{
	HTuple hvTrainDataDict;//训练数据集合
	HTuple hvMLPHandle;//分类器处理
	HTuple hvRegionFeature;//区域特征
	HTuple hvGrayFeature;//灰度特征
	HTuple hvFeatureName;//特征名称
	HTuple hvFeatureLength;//特征长度
	int    nLeftColumn;
	int    nRightColumn;
};



struct ExmaraParam
{
	double dAlpha;
	int nLowThresh;
	int nHighThresh;
	double dMinArea;
};
typedef struct tagTBlackRegion
{
	HObject ho_JXRegion;
	HObject hoTripRegion;//分区示意图
	HObject hoYaHenPingBiRegion;//大间歇背后的压痕
	void InitObject()
	{
		GenEmptyObj(&hoTripRegion);
		GenEmptyObj(&ho_JXRegion);
		GenEmptyObj(&hoYaHenPingBiRegion);
	}
}BlackRegion,*PBlackRegion;

	typedef struct tagTMYREGION
	{
		HObject ho_CoatUnion;
		HObject ho_CerimicUnion;
		HObject ho_FoilUnion;

		void Clear()
		{
			GenEmptyObj(&ho_CoatUnion);
			GenEmptyObj(&ho_CerimicUnion);
			GenEmptyObj(&ho_FoilUnion);
		}
	}MYREGION,*PMYREGION;
typedef struct tagTSINGLEMODEL
{
	HObject hoModelInfo;//0 原图 1 模板区域 2 搜索区域
	HTuple  hvModelID;

}SINGLEMODEL,*PSINGLEMODEL;
typedef std::vector<SINGLEMODEL> MARKVECTOR;
typedef struct tagTLINEMATCH
{
	int    nLineUsed;//画线总数
	int    nCoatCount;//涂布区域测量需要线的个数
	int    nFoilCount;//极耳区域测量需要线的个数
	int    nCeramicCount;//陶瓷区域测量需要线的个数
	int    nStaggerCount;//错位区域测量需要线的个数

	int     nCoatLeftStartIndex;
	int     nFoilLeftStartIndex;
	int     nCeramicLeftStartIndex;


	std::vector<int> vCoat1;
	std::vector<int> vCoat2;//1和2是一对 代表是涂布区域
	std::vector<int> vFoil1;
	std::vector<int> vFoil2;//1和2是一对 代表是极耳区域
	std::vector<int> vCeramic1;
	std::vector<int> vCeramic2;//1和2是一对 代表是陶瓷区域


	std::vector<int> vStagger1;
	std::vector<int> vStagger2;//1和2是一对 代表是检测错位区域

	//进行LineMatch后进行尺寸判定
	std::vector<double> vCoatWidthMin;
	std::vector<double> vCoatWidthMax;
	std::vector<double> vCoatWidthOffset;

	std::vector<double> vFoilWidthMin;
	std::vector<double> vFoilWidthMax;
	std::vector<double> vFoilWidthOffset;

	std::vector<double> vCeramicWidthMin;
	std::vector<double> vCeramicWidthMax;
	std::vector<double> vCeramicWidthOffset;

	std::vector<double> vStaggerOffset;//错位补偿



}LINEMATCH,*PLINEMATCH;


typedef struct  tagTMEASUREPARAM
{
	//BOOL bPretreat;//是否进行预处理
	//int  nSizePretreat;//预处里尺寸大小
	//BOOL bUseLineRefer;//使用参考线，适用于单个相机拍不全涂布的情况
	//int  nLineReferSN;//将几线设置为参考线
	//BOOL bJudgeCoat;//判断涂布是否涂了
	int  nCoatThres;//判断涂布阈值
	//int nCameraSize;//几K的相机
	//HTuple hv_ColResolution;//所有的分辨率
	//int nCalibType;
	HObject hoCaoweiRegions;//槽位提取区域

}MEASUREPARAM,*PMEASUREPARAM;


class CriticalSectionHelper
{
public:
	CriticalSectionHelper(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}

	~CriticalSectionHelper()
	{
		LeaveCriticalSection(m_pcs);
		m_pcs = NULL;
	}

private:
	CRITICAL_SECTION *m_pcs;
};


#endif _DEFINE_GEICHECK



struct TuoWeiParam//脱碳参数
{

	///间歇检测脱碳
	BOOL   bUseDetect;//启用
	int    nJXTapeLTMinThresh;//间歇区域脱碳的最小阈值
	int    nJXTapeLTMaxThresh;//间歇区域脱碳的最大阈值
	double dJXTapeLTMinArea;//间歇区域脱碳的最小面积
	double  dJXTuoWeiMaxHeight;//拖尾最大高度
};
struct AT9Param//AT9参数
{

	BOOL   bUseDetect;//启用
	////漏箔////
	int    nAT9LMinThresh;//AT9最小阈值
	double dAT9LMinWidth;//AT9最小宽度
	double dAT9LMinHeight;//AT9最小高度
	/////AT9区域暗缺陷
	int    nAT9DarkMaxThresh;//最小灰度
	double dAT9DarkMinWidth;//AT9最小暗缺陷灰度
	double dAT9DarkMinHeight;//最小高度
};


struct PretreatParam//预处理
{
	double dBorderWidth;//边界宽度mm
	double dRightBorderWidth;//右侧边界宽度mm
	double dScaleFactor;//图像拉伸系数 
	double  dMidLightCof;//中心亮区域提取系数取值 0-1
	double dMidDarkCof;//中心暗区域提取系数 0-1
	double dBorderLightCof;//中心亮区域提取系数0-1
	double dBorderDarkCof;//中心暗区域提取系数0-1
	double dYaHenPingBiKuanDu;//压痕屏蔽宽度
	BOOL   bEnbleUseClassyfy;//启用分类器
	double dMLPSongjindu;//MLP松紧度
};

struct JiaoDaiParam
{
	BOOL   bUseDetect;//启用
	int    nDarkThresh;//暗提取阈值
	int    nLightThresh;//亮阈值
	double dMinArea;//最小面积
};
struct LeakFoilParam//漏箔参数
{
	BOOL bUseDetect;//启用
	///回路1
	int nThresh;
	double dMinArea;



};
struct BubbleParam//气泡参数
{
	BOOL   bUseDetect;//启用
	int    nDynThresh;//对比度
	double dMinArea;//最小面积
	double dMinCircularity;//最小圆度
};


struct DarkLineParam//黑线参数
{
	BOOL   bUseDetect;//启用
	int    nDynThresh;//对比度
	double dMinWidth;//最小宽度
	double dMinLengthWidthRatio;//最小长宽比
};
struct LightDotParam//白斑参数
{
	BOOL   bUseDetect;//启用
	int    nLightDotDynThresh;//阈值下限
	double dMinArea;//最小面积
};

struct DarkDotParam//黑斑参数
{
	BOOL   bUseDetect;//启用
	int    nDarkDotDynThresh;//阈值上限
	double dMinArea;//最小面积
};

struct TapeParam//胶带参数
{
	BOOL   bUseDetect;//启用
	int    nTapeDynThresh;//脱碳对比度
	double  dDevFactor;//偏差因子
	double dLTMinArea;//亮胶带的最小面积

	int    nTapeThresh;//阈值分割
	double dTapeThreshMinArea;//最小面积
};

struct FoilParam//极耳检测参数
{
	BOOL   bUseDetect;//启用
	int    nMaxDarkThresh;//阈值上限
	double dMinDarkArea;//最小面积
};


struct  SingleFeature//自定义缺陷组
{
	CString StrSingleFeatureName;
	double dMin;
	double dMax;

};

struct  SingleErrorGroup//自定义缺陷组
{
	CString strErrorName;//错误项目名称
	int    nSingleGroupNum;//当前项目中有多少项筛选特征项
	std::vector<SingleFeature>Vec_tSingleGroupFeatures;

};
struct  FeatureSelect//自定义缺陷组
{
	std::vector<SingleErrorGroup> Vec_FSelects;//自定义缺陷特征数组
	int nGroupNum;//自定义缺陷数目
};
typedef struct  tagTPARAMETERS
{
	BOOL tUseDefectMasterSwitch;
	PretreatParam tPretreatParam;
	LeakFoilParam tLeakFoilParam;
	BubbleParam   tBubbleParam;
	DarkLineParam tDarkLineParam;
	LightDotParam tLightDotParam;
	DarkDotParam  tDarkDotParam;
	TapeParam     tTapeParam;
	JiaoDaiParam  tJiaoDaiParam;
	FoilParam     tFoilParam;
	TuoWeiParam   tTuoWeiParam;
	AT9Param      tAT9Param;
	FeatureSelect tFeatureSelect;
}PARAMETERS, *PPARAMETERS;

