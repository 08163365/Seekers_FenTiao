/************************************************************************
("INITIALIZE" == strMessage)//初始化
("ACTION" == strMessage)//检测
("SHOW" == strMessage)//调用对话框
("SETPARAM" == strMessage)//获取参数
("GETPARAM" == strMessage)//设置参数
/************************************************************************/
#pragma once
#include "stdafx.h"
///
enum e_ErrorType
{
	JieDai,//胶带
	LouJinShu,//漏金属
	TuoTan,//脱碳或漏辊
	TiaoHen,//条痕
	QiPao,//气泡
	LiangDian,//亮点
	HeiDian,//黑点
	JiErQueXian,//极耳完整度
	SumSize,//总长NG
	C_HSize,//极耳间距NG
	C_WSize,//模切宽度NG
	AT9Size,//涂膜上极耳(阴极AT9尺寸)NG
	PoSun,//破损 W3测试用
	AT9LouJinShu,//AT9漏金属
	CircleHeight,//圆弧高度
	AoLouBoHeight,//凹漏箔高度
	BHXJSumSize,//不含间歇总长
	ABSize,//AB错位高度
	UpJXSize,//正面间歇高度
	DownJXSize,//背面间歇高度
	FirstMeanValueSize,//Mean值NG
	TuoWei,//拖尾

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







typedef struct  tagTSEPARATEINIT
{
	CString strProductName;//品种名称
	int nGrabberSN;//相机序号，默认为0
	PVOID   pLogView;
	int nProductType;
	int nCoatType;//涂布类型
}SEPARATEINIT,*PSEPARATEINIT;
typedef struct  tagTSEPARATEINPUT
{
	HObject hoGrabImage;
	HTuple  hvHomMat2D;//仿射变换矩阵
	int     nBreakType;//跳出类型
}SEPARATEINPUT,*PSEPARATEINPUT;
typedef struct tagTSEPARATEOUTPUT
{
	HTuple  hvCoatWidth;
	HTuple  hvCoatHeight;
	HTuple  hvFoilWidth;
	HTuple  hvFoilHeight;
	HTuple  hvLocateMiss;//定位不齐值
	HTuple  hv_BaiBian;//白边宽度
	///传出区域
	HObject hoCoat;//涂布区域
	HObject hoFoil;//极耳区域
	HObject hoDispRegion;//测量产生的中间区域
	HObject hoTDispRegion;//AT9区域
	HObject hoLane;
	HObject hoErrorRegion;//错误区域
	HTuple  hvErrorType;//错误类型
}SEPARATEOUTPUT,*PSEPARATEOUTPUT;



