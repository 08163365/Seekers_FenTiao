#ifndef P_DEFINE_P
#define P_DEFINE_P
#include "stdafx.h"
#include "HalconCpp.h"
using namespace HalconCpp;

const int ExternalTrigger=0;//外触发
const int Internal=1;//内触发
const int Intermittent=0;//间歇式
const int Continuous=1;//连续式
const int SpeedSection=2;//高速切片

#define UM_TIMER_QUEUEINFO WM_USER+1
#define UM_TIMER_STARTGRAB WM_USER+2
#define UM_TIMER_TIME WM_USER+3



#define LOGO_WIDTH 163		//LOGO的宽度
#define MAX_LANE 12


enum e_ProductType
{
	positive=0,//正极
	negative//负极
};
enum e_CoatType
{
	M_TwoCam_Continue,//连续涂布
	M_TwoCam_Intermission,//间歇涂布
};

enum e_GongYiNameType
{
	GongYi_ASC1,//双侧产品
	GongYi_M6S,//正常间歇料
	GongYi_MMT,//间歇有槽位
	GongYi_STS1,//A/B面均有空箔含OAP
	GongYi_STS2,//B面均有空箔含OAP
};


enum e_FoilInSideType
{
	TwoSideProduct,//双侧产品
	LeftSideProduct,//左侧产品
	RightSideProduct,//右侧产品

};
//喷码机错误类型映射表
const CString g_strLetterErrorName[]={
	_T("a"),
	_T("b"),
	_T("c"),
	_T("d"),
	_T("e"),
	_T("f"),
	_T("g"),
	_T("h"),
	_T("i"),
	_T("j"),
	_T("k"),
	_T("l"),
	_T("m"),
	_T("o"),
	_T("p"),
	_T("q"),
	_T("r"),
	_T("s"),
	_T("t"),
	_T("u"),
	_T("v"),
};
typedef struct tagTSYSTEMSET
{
	int nHistoryCount;//保留的历史图片的个数
	int nHistoryType;//仅保留错误的还是全部保留,还是不保留
	int nRejectPoint;//剔废点
	
	int  nMergeResults;//是否综合结果
	int  nMergeParam;//正反相机使用相同的参数
	BOOL bMirrorRow;//是否垂直镜像


	BOOL bTileImage;//是否拼图
	//int  nStepLength;
	//double dwMaxFoilDistance;//最大极耳间距
	BOOL bSaveTileImage;//是否保存拼之前的图片
	int  nDistanceCamera;//相机之间的距离
	BOOL bAlignedTwoCamera;//两相机排整齐,后相机是否使用前相机的mark位置
	int nLaneCount;//分条数
	int nOverTimeExit;//超时退出设置
	BOOL bEnableReloadParam;//启用按卷还原参数
	int  nCameraModel;//相机定位选择
	//BOOL bEnableLabelCheck;//启用贴标机防呆
	BOOL bEnablePopWindowToWarn;//启用弹窗报警
	int  nMaxPopWindowNum;//最大弹窗数目
	//int  nPushType;//0是正常入队 1是反向入队


	/////
	int  nOverTimeFrameValue;///超时帧数
	int  nOverTimeValue;//超时设置
	//超时自动退出
	int nOverTimeLimitLoginOff;


}SYSTEMSET,*PSYSTEMSET;




struct CreateBaseLineInfo//CreateBaseLineInfo
{

	int nFirstCoatLowThresh;//涂布提取低阈值
	int nFirstCoatHighThresh;//涂布提取高阈值h
	int nFirstFoilLowThresh;//涂布提取阈值
	int nFirstFoilHighThresh;//涂布提取阈值

	int nSecondCoatLowThresh;//涂布提取阈值
	int nSecondCoatHighThresh;//涂布提取阈值h
	int nSecondFoilLowThresh;//涂布提取阈值
	int nSecondFoilHighThresh;//涂布提取阈值


	int nFirstLeftPosition;//前相机左侧涂布位置
	int nFirstRightPosition;//前相机右侧涂布位置
	int nFirstLeftPositionRow;//前相机左侧涂布位置
	int nFirstRightPositionRow;//前相机右侧涂布位置

	int nSecondLeftPosition;//后相机左侧涂布位置
	int nSecondRightPosition;//后相机右侧涂布位置
	int nSecondLeftPositionRow;//后相机左侧涂布位置
	int nSecondRightPositionRow;//后相机右侧涂布位置


	/////////////////////////////////////////////////////

};
struct s_ProductParam//产品配置信息
{
	e_CoatType emCoatType;
	e_ProductType emProductType;
};
struct  s_TempStoreInfo//缓存数据
{
	double dFirstWidth;
	double dSecondWidth;
	double dSumLength;
   s_TempStoreInfo()
	{
		dFirstWidth=0;
		dSecondWidth=0;
		dSumLength=0;
	}
};

struct s_SingleCount//单条的统计
{
	std::vector<int> nClassError;//分类,错误的个数
	int nError;//错误片数
	int nTotal;//总片数
};

typedef struct tagTRESULTDATA
{
	int nFoilSN;
	int nCoatWidth;
	int nCoatHeight;
	int nFoilWidth;
	int nFoilHeight;
	int nTotalLength;

}RESULTDATA,*PRESULTDATA;

struct s_Count
{
	int nType;//几种错误类型
	std::vector<s_SingleCount> veCount[2];//有多少条，对应多少个
	CStringArray strAClassName;//分类错误的称呼
}; 


struct s_ErrorClassify
{
	std::vector<int> vecErrorClassify;//分类,错误的个数
};

struct s_LabelCause
{
	std::vector<s_ErrorClassify> vec_LabelCause[2];//标签统计
};


typedef struct  tagTSUBELEMENT
{
	HObject hoImage;
	HObject hoSubImage;
	int     nGrabberSN;
}SUBELEMENT,*PSUBELEMENT;

//尺寸判断结构体
typedef struct tagTJUDGECONMENT  ///每一项的判断
{
	bool bIsDecter;      //该项是否检测
	bool bIsSecond;      //该项是否判断个数和面积
	int  nMaxNumber;        //个数
	double dMaxDiameter;    //最大直径
	double dMaxArea;        //最大面积
	double dMaxLength;      //最大长度
	double dMaxWidth;		 //最大宽度
}JUDGECONMENT,*PJUDGECONMENT;
typedef struct tagTSIZECONMENT
{
	double dMaxWidth;		 //宽度上限
	double dMinWidth;		 //宽度下限
	double dMaxHeight;       //高度上限
	double dMinHeight;		 //高度下限


	double dwHeightStandard;//高度标准值
	double dwWidthStandard;//宽度标准值
	double dwHeightDeviation;//高度偏差
	double dwWidthDeviation;//宽度偏差
	double dwHeightCorrect;//高度修正
	double dwWidthCorrect;//宽度修正
	double dwTsizeMax;//T槽最大值
	double dwBaibianMax;//白边最大值


	double m_dCorrectMax;
	double m_dCorrectMin;
	double m_dwDiePianMarkWidth;//叠片U形切口宽度上限--
	double m_dwDiePianMarkWidthDev;//叠片U形切口宽度下限--
	double m_dwDiePianMarkHeight;//叠片U形切口高度上限--
	double m_dwDiePianMarkHeightDev;//叠片U形切口高度下限--
	double m_dwDiePianMarkDiffMax;//叠片U形口的对齐度最大值--
	double m_dwDistanceBorder;//极边距标准值
	double m_dwDistanceBorderDev;//极边距偏差
	double m_dwLocateMissing;//定位不齐标准值
	double m_dwLocateMissingDev;//定位不齐偏差
}SIZECONMENT,*PSIZECONMENT;


struct PowerInfo//权限结构体
{
	CString strPowerType;
	CString strJobNum;
	CString strJobPassWord;

};




//enum e_ErrorType
//{
//	C_WSIZE,//涂布宽度NG
//	C_HSIZE,//涂布高度NG
//	F_WSIZE,//极耳宽度NG
//	F_HSIZE,//极耳高度NG
//	DINGWEIBUQI,//定位不齐NG
//	SUMSIZE,//多极耳总长NG
//	BAIBIAN,//AT9尺寸NG
//	JIERYUCHICUNBUFU,//极耳数目与尺寸不符合
//	LOUJINSHU,//漏金属
//	QIPAO,//气泡
//	HEIBAN,//黑斑
//	BAIBAN,//白斑
//	HEIXIAN,//暗痕
//	BAIXIAN,//刮痕
//	BIANYUANQUEXIAN,//边缘缺陷
//	BIANJIEQUEXIAN,//边界缺陷
//	AT9QUEXIAN,//AT9缺陷
//	TUOTANHUOLOUGUN,//脱碳或漏辊
//	JIERDAILIAO,//极耳带料
//	JIEANCECHAOSHI,//检测超时
//	CHICUNYICHANG,//尺寸检测异常
//	BIAOMIANYICHANG//表面检测异常
//};


#define   VK_0  0x30   
#define   VK_1  0x31   
#define   VK_2  0x32   
#define   VK_3  0x33   
#define   VK_4  0x34   
#define   VK_5  0x35   
#define   VK_6  0x36   
#define   VK_7  0x37   
#define   VK_8  0x38   
#define   VK_9  0x39  
  
//定义数据字符A~Z  
#define   VK_A  0x41   
#define   VK_B  0x42   
#define   VK_C  0x43   
#define   VK_D  0x44   
#define   VK_E  0x45   
#define   VK_F  0x46   
#define   VK_G  0x47   
#define   VK_H  0x48   
#define   VK_I  0x49   
#define   VK_J  0x4A   
#define   VK_K  0x4B   
#define   VK_L  0x4C   
#define   VK_M  0x4D   
#define   VK_N  0x4E   
#define   VK_O  0x4F   
#define   VK_P  0x50   
#define   VK_Q  0x51   
#define   VK_R  0x52   
#define   VK_S  0x53   
#define   VK_T  0x54   
#define   VK_U  0x55   
#define   VK_V  0x56   
#define   VK_W  0x57   
#define   VK_X  0x58   
#define   VK_Y  0x59   
#define   VK_Z  0x5A   
  
//定义数据字符a~z  
#define   VK_a  0x61   
#define   VK_b  0x62   
#define   VK_c  0x63   
#define   VK_d  0x64   
#define   VK_e  0x65   
#define   VK_f  0x66   
#define   VK_g  0x67   
#define   VK_h  0x68   
#define   VK_i  0x69   
#define   VK_j  0x6A   
#define   VK_k  0x6B   
#define   VK_l  0x6C   
#define   VK_m  0x6D   
#define   VK_n  0x6E   
#define   VK_o  0x6F   
#define   VK_p  0x70   
#define   VK_q  0x71   
#define   VK_r  0x72   
#define   VK_s  0x73   
#define   VK_t  0x74   
#define   VK_u  0x75   
#define   VK_v  0x76   
#define   VK_w  0x77   
#define   VK_x  0x78   
#define   VK_y  0x79   
#define   VK_z  0x7A

#endif //P_DEFINE_P