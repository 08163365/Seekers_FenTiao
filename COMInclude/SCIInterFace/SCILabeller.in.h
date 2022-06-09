#include "stdafx.h"
typedef BOOL(WINAPI *SystemCallBack)(const CString& strMessage, void* wParam, void* lParam);//系统回调函数
typedef struct tagTLABELLERINIT//初始化存储类
{
	CString strProductName;
	PVOID   pLogView; 
	SystemCallBack	pSysCall;  //回调,模块和主程序通讯可以通过这个函数互相访问
	
}LABELLERINIT,*PLABELLERINIT;
