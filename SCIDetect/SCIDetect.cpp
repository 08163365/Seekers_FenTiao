// SCIDetect.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CSCIDetectApp

BEGIN_MESSAGE_MAP(CSCIDetectApp, CWinApp)
END_MESSAGE_MAP()


// CSCIDetectApp ����

CSCIDetectApp::CSCIDetectApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSCIDetectApp ����

CSCIDetectApp theApp;


// CSCIDetectApp ��ʼ��

BOOL CSCIDetectApp::InitInstance()
{
	CWinApp::InitInstance();
	
	g_strConfigCommonFile.Format("%s/Config/Common.dh",GetCurrentAppPath());//��ȡ�����ļ�����
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
	// TODO: �ڴ����ר�ô����/����û���

	FreeLibrary(g_hMeasureDll);
	return CWinApp::ExitInstance();
}
