/************************************************************************
("INITIALIZE" == strMessage)//初始化
("ACTION" == strMessage)//检测
("SHOW" == strMessage)//调用对话框
("SETPARAM" == strMessage)//获取参数
("GETPARAM" == strMessage)//设置参数
/************************************************************************/
#pragma once
#include "stdafx.h"


enum e_ErrorType
{
	JieDai,         //胶带
	LouJinShu,      //漏金属
	TuoTan,         //脱碳或漏辊
	TiaoHen,        //条痕
	QiPao,          //气泡
	LiangDian,      //亮点
	HeiDian,        //黑点
	TuoWei,         //箔区缺陷

	BHXJDownSumSize,//B面模长NG
	DownJXSize,     //B面留白NG
	BHXJUpSumSize,  //A面模长
	UpJXSize,       //A面留白NG
	C_WSize,        //总宽NG
	C_SingleWidth,  //分条宽度NG
	HeadCuoWei,     //头部错位NG
	TailCuoWei,     //尾部错位NG

	//JiErQueXian,//极耳完整度
	//SumSize,//总长NG
	//C_WSize,//模切宽度NG
	//AT9Size,//涂膜上极耳(阴极AT9尺寸)NG、ACS1用
	//PoSun,//破损 W3测试用
	//AT9LouJinShu,//AT9漏金属
	//CaoWeiHNG,//槽位高度NG
	//CaoWeiWNG,//槽位宽度NG
	//CaoWeiMDNG,//槽位MDNG
	//CaoWeiTDNG,//槽位TDNG
	//CaoWeiMDCuoWeiNG,//槽位MD错位NG
	//CaoWeiTDCuoWeiNG,//槽位TD错位NG
	//BHXJSumSize,//不含间歇总长
	//UpJXSize,//正面间歇高度
	//DownJXSize,//背面间歇高度


	
	ErrorCount,

};
//const CString g_strErrorName[]={
//	_T("胶带"),
//	_T("漏金属"),
//	_T("脱碳"),
//	_T("条痕"),
//	_T("气泡"),
//	_T("亮点"),
//	_T("黑点"),
//	_T("极耳完整度"),
//	_T("总膜长"),
//	_T("极耳间距"),
//	_T("模切宽度"),
//	_T("涂膜上极耳"),
//	_T("破损"),
//	_T("AT9漏金属"),
//	_T("圆弧高度"),
//	_T("凹漏箔高度"),
//	_T("不含间歇总长"),
//	_T("AB错位高度"),
//	_T("正面间歇高度"),
//	_T("背面间歇高度"),
//	_T("Mean值NG"),
//	_T("拖尾"),
//};







//enum e_ErrorType
//{
//	///瑕疵错误项目
//	JieDai,//胶带
//	LouJinShu,//漏金属
//	TuoTan,//脱碳或漏辊
//	TiaoHen,//条痕
//	QiPao,//气泡
//	LiangDian,
//	HeiDian,
//	JiErQueXian,
//	EXCEPTION,
//	CO_WSIZE,
//	FO_WSIZE,
//	CE_WSIZE,
//	AB_DISLOCATION,//上下涂布错位
//
//};
//const CString g_strErrorName[]={
//
//	_T("胶带"),
//	_T("漏金属"),
//	_T("脱碳"),
//	_T("条痕"),
//	_T("气泡"),
//	_T("亮点"),
//	_T("黑点"),
//	_T("极耳完整度"),
//	_T("异常"),
//	_T("宽度不良"),
//	_T("极耳宽度不良"),
//	_T("陶瓷宽度不良"),
//	_T("AB面错位不良"),
//};

typedef struct  tagTSEPARATEINIT
{
	CString strProductName;//品种名称
	int nGrabberSN;//相机序号，默认为0
	PVOID   pLogView;
	int nProductType;
	int nCoatType;
}SEPARATEINIT,*PSEPARATEINIT;
typedef struct  tagTSEPARATEINPUT
{
	HObject hoGrabImage;
	HTuple  hvHomMat2D;//仿射变换矩阵
	int     nBreakType;//跳出类型
}SEPARATEINPUT,*PSEPARATEINPUT;
typedef struct tagTSEPARATEOUTPUT
{
	HTuple  hv_Flag;//TRUE是产品OK,FALSE产品为NG
	//连续涂布
	HTuple  hv_CoatWidth;           //输出的从左到右的涂布宽度
	HTuple  hv_FoilWidth;           //输出的从左到右的极耳宽度
	HTuple  hv_CeramicWidth;        //输出的从左到右的陶瓷宽度
	HTuple  hv_SortWidth;           //排序后显示的宽度顺序
	HTuple  hv_Name;                //依据宽度排序后的名字
	HObject ho_Coat;                //涂布区域
	HObject ho_Foil;                //极耳区域
	HObject ho_Ceramic;             //陶瓷区域
	HObject ho_DispRegion;          //测量产生的中间区域
	HObject ho_ErrorRegion;         //错误区域
	HTuple  hv_AT9EdageToProEdage;  //记录AT9边界距离产品边界的距离
	HTuple hv_CpkEdageToProEdage;	//连续涂布CPK计算,记录每条涂布的相邻边
	//间歇涂布
	HTuple  hv_LeftEadgePixel;      //左侧像素
	HTuple  hv_RightEadgePixel;     //右侧像素
	HTuple  hv_TopEdagePixel;       //顶部像素
	HTuple  hv_BotEdagePixel;       //底部像素

	HTuple  hv_TripSNTuple;         //槽位隶属于条数
	HTuple  hv_CaoWeiWidthTuple;    //槽位宽度
	HTuple  hv_CaoWeiHeightTuple;   //槽位高度
	HTuple  hv_CaoWeiXPositionTuple;//槽位X坐标
	HTuple  hv_CaoWeiYpositionTuple;//槽位Y坐标
	HObject ho_ExtractCaoWeiRegions;//提取的槽位区域

	HTuple  hv_JXHeight;            //间歇高度
	HTuple  hv_MPLength;            //膜长
	HTuple  hv_SumLength;           //总长
	HObject ho_JXRegion;            //间歇区域

}SEPARATEOUTPUT,*PSEPARATEOUTPUT;

