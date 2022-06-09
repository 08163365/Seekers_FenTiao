#pragma once
#include "stdafx.h"
#include <vector>
#define PIO24_PORTS_MAX 32
#define PIO24_COUNT 1
const int  ConstFenpinNum = 2;

//const int  Const_nChannel[]=
//{
//	//初始化打标
//1	0,
//2	4,
//3	5,
//4	6,
//5	7,
//6	8,
//7	3,
//8	9,
//9	10,
//10	11,
//11	12,
//12	13,
//};
///////上面1-6 下面7-12////////////////
//const int  Const_nChannel[] =
//{
//	//初始化打标
//	0,
//	4,
//	5,
//	6,
//	7,
//	8,
//	3,
//	9,
//	10,
//	11,
//	12,
//	13,
//};
/////1-2贴标机对应的通道输出计数
//const int  Const_nChannelOutCounter[] =
//{
//	///////输出计数器//////
//	45,
//	60,
//	61,
//	62,
//	63,
//	64,
//	46,
//	65,
//	66,
//	67,
//	68,
//	69,
//};
/////手动控制打标
//const int Const_TestIndexParam[] =
//{
//	169,170,171,172,173,174,175,176,177,178,179,180
//
//};
/////////////////////////////////////////////

/////上面1-5 下面7-11/////去除6通道不使用///////////
const int  Const_nChannel[] =
{
	//初始化打标
	0,
	4,
	5,
	6,
	7,
	3,
	9,
	10,
	11,
	12,
	8,//原来的6
	13,//原来的12
};
///1-2贴标机对应的通道输出计数
const int  Const_nChannelOutCounter[] =
{
	///////输出计数器//////
	45,
	60,
	61,
	62,
	63,
	46,
	65,
	66,
	67,
	68,
	64,
	69,
};
///手动控制打标
const int Const_TestIndexParam[] =
{
	169,170,171,172,173,175,176,177,178,179,174,180

};
///////////////////////////////////////////






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

enum e_CoatType
{
	M_TwoCam_Continue,//连续涂布
	M_TwoCam_Intermission,//间歇涂布

};
#define  MAX_LALBELNUM 12  //最多支持贴标机数目
enum e_RejectType
{
	REJECT_DEFULT,//正常贴标
	REJECT_NONE,//停止贴标
};
typedef struct tagTALARMS//报警灯响起
{
	BOOL		bEffective;//是否生效
	//int			nMAXContinuousError;//最大错误个数
	//int			nCurrentContinuousError;//当前连续错误个数
}ALARMS,*PALARMS;
typedef struct tagTCOMMUICATEPARAM
{
   int			nRejectType;//剔废类型 如果为1是间隔剔废 如果为2为连续剔废 默认为0 根据结果好坏进行剔废
   double       dFirstLabelLength;//内贴标机的物理距离
   double       dSecondLabelLength;//外贴标机的物理距离

   int       nXLengthInPixel;//打标队列X设置行数
   int       n2ndLengthInPixel;//打标队列2nd设置行数

   int          nBuChangHangShu;//变速贴标补偿行数
   int          nTiFeiPulse;//踢废脉冲
   double       dLabelPosition;//贴标位置
   //////////////////////////////产品和贴标机设置使能////////////////////////////////////////////

   HTuple       hvChannelChoice;//通道选择

   double       dVirtualEALength;//连续料虚拟EA长度

   BOOL         bMachineLengthLabel;//设备总长打标
   ////
   BOOL     bUseLabelOffset;//使用贴标偏移监控
   double   dLabelOffsetDev;//公差
   double   dUpLabelOffsetCompensate;//上带路偏移补偿
   double   dDownLabelOffsetCompensate;//下带路偏移补偿
   HTuple   hvLabelOffsetCompensateTuple;//贴标偏移补偿

}COMMUICATEPARAM,*PCOMMUICATEPARAM;
typedef struct tagTCOMMONPARAM
{
	e_CoatType  emCoatType;//设备类型
	double		dRowResolution[2];//纵向分辨率，0 相机0，1 相机1
	double		dColumnResolution[2];//横向分辨率，0 相机0，1 相机1
}COMMONPARAM,*PCOMMONPARAM;


