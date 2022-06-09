#pragma once
#include "stdafx.h"
typedef BOOL(WINAPI *SystemCallBack)(const CString& strMessage, void* wParam, void* lParam);//系统回调函数
typedef struct tagTCOMMUNICATEINIT//初始化通信类
{
	CDialogEx* pDlg;//主界面指针
	CString strProductName;
	DHInterface* pLogView;
	CString strGongWei;
	SystemCallBack	pSysCall;  //回调,模块和主程序通讯可以通过这个函数互相访问
	int nCoatType;//0是叠片 4卷绕
}COMMUNICATEINIT,*PCOMMUNICATEINIT;
typedef struct tagTLabelControl//初始化通信类
{
	///GET_TBCZ 工位一距离 工一使能  工位二距离 工位二使能 贴标位置

	double  dFirstLabelLength;
	BOOL    bEnableFirstLabel;

	double  dSecondLabelLength;
	BOOL    bEnableSecondLabel;

	double dLabelActionPosition;//贴标位置


}LabelControlInfo,*pLabelControlInfo;


