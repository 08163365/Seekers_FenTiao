
// Seekers.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Seekers.h"
#include "SeekersDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeekersApp

BEGIN_MESSAGE_MAP(CSeekersApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSeekersApp ����

CSeekersApp::CSeekersApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSeekersApp ����

CSeekersApp theApp;


// CSeekersApp ��ʼ��
BOOL CSeekersApp::InitInstance()
{
	//����Ψһ��
	HANDLE hCurInstance = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 16, "CSeekersApp");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hCurInstance);
		AfxMessageBox("�Ѿ�����һ��δ�رյ�Seekers������ʵ��");
		return FALSE;
	}

	if (!AfxOleInit())
	{
		AfxMessageBox("��ʼ��COMʧ��!");
		return FALSE;
	}
	try
	{
		SetSystem("width", 30000);
		SetSystem("height", 30000);

		ResetObjDb(52000, 52000, 0);

	
		//SetSystem("max_connection",200);
		SetSystem("parallelize_operators","true");
		SetSystem("use_window_thread", "true");
		SetSystem("store_empty_region", "false");
		SetHcppInterfaceStringEncodingIsUtf8(FALSE);

		SetSystem("filename_encoding","locale");
		SetSystem("read_halcon_files_encoding_fallback","locale");
		SetSystem("read_halcon_files_encoding_fallback","locale");
		SetSystem("write_halcon_files_encoding","locale");
		SetSystem("tsp_tuple_string_operator_mode","byte");
		SetSystem("tuple_string_operator_mode","byte");
		SetSystem("clip_region", "false");
	}catch(HException& e)
	{
		CString strMessageBox;
		strMessageBox.Format("��������ѵ���,����ϵ��Ӧ��\r\n%s  [ %s() ]",e.ErrorMessage().Text(),e.ProcName().Text());
		AfxMessageBox(strMessageBox);
		return FALSE;
	}

    
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CSeekersDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CSeekersApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	return CWinApp::ExitInstance();
}
