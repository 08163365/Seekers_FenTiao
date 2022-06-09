// SCIDetect.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SCIDetect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE g_hMeasureDll;
e_GrabberType g_GrabberType;
e_ProductType g_ProductType;
e_CoatType g_emCoatType;
CString   g_strConfigCommonFile;//Common.dh
//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CSCIDetectApp

BEGIN_MESSAGE_MAP(CSCIDetectApp, CWinApp)
END_MESSAGE_MAP()


// CSCIDetectApp 构造

CSCIDetectApp::CSCIDetectApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSCIDetectApp 对象

CSCIDetectApp theApp;


// CSCIDetectApp 初始化

BOOL CSCIDetectApp::InitInstance()
{
	CWinApp::InitInstance();
	
	g_strConfigCommonFile.Format("%s/Config/Common.dh",GetCurrentAppPath());//获取配置文件名称
	CString strFileName,strProductName;
	strFileName.Format("%s//Config//System.vis",GetCurrentAppPath());
	GetPrivateProfileString("INFO","PRODUCTNAME","null",strProductName.GetBuffer(MAX_PATH),MAX_PATH,strFileName);
	strProductName.ReleaseBuffer();
	g_emCoatType=(e_CoatType)GetPrivateProfileInt(strProductName,"CoatType",0,strFileName);
	g_ProductType=(e_ProductType)GetPrivateProfileInt(strProductName,"ProductType",0,strFileName);
	g_GrabberType=(e_GrabberType)GetPrivateProfileInt(strProductName,"GrabberType",0,strFileName);

	return TRUE;
}


int CSCIDetectApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类

	FreeLibrary(g_hMeasureDll);
	return CWinApp::ExitInstance();
}
