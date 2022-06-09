
#include <afxtempl.h>
#include <vector>
#ifndef _DEFINE_GEICHECK
#define _DEFINE_GEICHECK
#define LINE_COUNT 8
#define MAX_LANE  2//单侧最多个数
#define MAX_LINEMATCH 8

enum e_GrabberType
{
	LINE=0,
	AREA,
};
enum e_ProductType
{
	positive=0,
	negative
};
enum e_CoatType
{
	D_TwoCam_OneTrip,//叠片机 双相机单条
	M_OneCam_OneTrip,//多极耳 单相机单条
	M_OneCam_TwoTrip,//多极耳 单相机双条
	M_TwoCam_OneTrip,//多极耳 双相机单条
	M_TwoCam_TwoTrip,//多极耳 双相机双条
	M_TwoCam_Intermission
};
typedef struct  tagTPARAMETERS
{
	BOOL bPretreat;//是否进行预处理
	int  nSizePretreat;//预处里尺寸大小
	int  nXSizePretreat;//预处里水平尺寸大小
	int nHcalibFlag;//0关闭 1开启
	int nFitRegionMode;//拟合点生成区域
	e_GrabberType emGrabberType;
	e_CoatType  emCoatType;
	e_ProductType emProductType;
	int nLaneCount;
	int nMaxDiffColumn;//起始点容忍的列差值

	int    nSelectValueMode;//取点方式
	double dLocateMissingOffset;//定位不齐补偿值
	double dMaxAoBanDeviation;//使用阈值分割后的最大切除深度允许偏差

	BOOL  bUseGRR;//使用GRR
	int   nSelectNum;//选取点数

	//////换型设立基准
	//double  dStandardWidth;//记录当前的基准
	double  dStandardLeftLocation;//记录当前的左侧基准
	double  dStandardRightLocation;//记录当前的右侧基准
	HTuple  hv_Mat2D;
    ///////提取凹版的二次阈值//////
	int     bEnaleUseSegAoBan;//
    int     bFindManualOrAuto;//手动还是自动
	double  dExtractFoilRegionWidth;//极耳提取区域宽度
	int     nExtractFoilThresh;//极耳提取阈值
	double  dExtractAobanRegionWidth;//凹版区域宽度
	int     nExtractAobanThresh;//凹版提取阈值

	///变宽使能
	BOOL   bEnableVariableWidth;//变宽检测
	double  dRelateOffset;//相对偏移
}PARAMETERS,*PPARAMETERS;

struct AoBanInfo//凹版提取信息
{
	HObject hoFoilDetectRegion;//提取极耳区域
	HObject hoExtractFoilRegion;//提取到的极耳区域
	HObject hoAoBanRegion;//凹版区域
	HObject hoLineBetAoBanAndFoil;//凹版和极耳交界处
	HTuple  hvLocateDistance;
	AoBanInfo()
	{
		hoFoilDetectRegion.GenEmptyObj();
		hoExtractFoilRegion.GenEmptyObj();
		hoAoBanRegion.GenEmptyObj();
		hoLineBetAoBanAndFoil.GenEmptyObj();
		hvLocateDistance.Clear();
	}
};

struct Monitor//稳定性监控
{
	HObject hoLeftXld;
	HObject hoRightXld;
	double dLeftHeight;
	double dLeftWidth;
	double dRightHeight;
	double dRightWidth;
	void ClearAll()
	{
		hoLeftXld.GenEmptyObj();
		hoRightXld.GenEmptyObj();
		dLeftHeight=0;
		dLeftWidth=0;
		dRightHeight=0;
		dRightWidth=0;
	}
};

typedef struct tagTLANESET
{
	double  dAllLength; //总箔材的长度
	double  dLeftStart;       //左侧起始点距离第一条线的距离
	double  dRightStart;      //右侧起始点距离最后一条线的距离
	int     nLeftCount;    //以左侧为基准的通道个数
	int     nRightCount;   //以右侧为基准的通道个数
	std::vector<double>  vLeftPos;     //以左侧起点为基准的所有通道位置
	std::vector<double>  vRightPos;    //以右侧起点为基准的所有通道位置
}LANESET,*PLANESET;

typedef struct tagTLINEMATCH
{
	int    nLineUsed;//使用的线个数
	int    nCoatCount;
	int    nFoilCount;
	std::vector<int> vCoat1;
	std::vector<int> vCoat2;//1和2是一对 代表是涂布区域
	std::vector<int> vFoil1;
	std::vector<int> vFoil2;//1和2是一对 代表是极耳区域

	//进行LineMatch后进行尺寸判定
	std::vector<double> vCoatWidthMin;
	std::vector<double> vCoatWidthMax;
	std::vector<double> vCoatWidthOffset;

	std::vector<double> vCoatHeightMin;
	std::vector<double> vCoatHeightMax;
	std::vector<double> vCoatHeightOffset;

	std::vector<double> vFoilWidthMin;
	std::vector<double> vFoilWidthMax;
	std::vector<double> vFoilWidthOffset;

	std::vector<double> vFoilHeightMin;
	std::vector<double> vFoilHeightMax;
	std::vector<double> vFoilHeightOffset;

}LINEMATCH,*PLINEMATCH;



//try的宏定义
#define _TRY \
	try \
{
#define _CATCH_HALCON(str) \
} \
	catch(HException e) \
{ \
	CString strMessageBox; \
	strMessageBox.Format("%s\r\n%s",str,e.ErrorMessage().Text()); \
	AfxMessageBox(strMessageBox); \
}
#define _CATCH_HALCON_NULL \
} \
	catch(HException e) \
{ \
}
#define _CATCH_HALCON_RETURN(str) \
} \
	catch(HException e) \
{ \
	CString strMessageBox; \
	strMessageBox.Format("%s\r\n%s",str,e.ErrorMessage().Text()); \
	AfxMessageBox(strMessageBox); \
	return; \
}
#endif _DEFINE_GEICHECK