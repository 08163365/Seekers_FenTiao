#pragma once
#include "stdafx.h"

typedef struct tagTLIGHTINIT//初始化光源控制器
{
	CString strProductName; //配方名
	DHInterface* Log;		//写日志回调函数
	int nProductType;		//产品类型
}LIGHTINIT,*PLIGHTINIT;

enum eLightControlType	//光源控制器型号
{
	DH_HAPS_48W800_1T = 0,
	DH_HAPS_48W800_2T,
	CST_CPL48320CDP_2TD,
};

enum eLightCommType
{
	
	CT_Tcp = 0,						//网口Tcp
	CT_SerialPort ,		//串口
};