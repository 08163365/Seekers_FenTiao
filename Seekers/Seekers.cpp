
// Seekers.cpp : 定义应用程序的类行为。
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


// CSeekersApp 构造

CSeekersApp::CSeekersApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSeekersApp 对象

CSeekersApp theApp;


// CSeekersApp 初始化
BOOL CSeekersApp::InitInstance()
{
	//进程唯一性
	HANDLE hCurInstance = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 16, "CSeekersApp");
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hCurInstance);
		AfxMessageBox("已经存在一个未关闭的Seekers检测软件实例");
		return FALSE;
	}

	if (!AfxOleInit())
	{
		AfxMessageBox("初始化COM失败!");
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
		strMessageBox.Format("运行许可已到期,请联系供应商\r\n%s  [ %s() ]",e.ErrorMessage().Text(),e.ProcName().Text());
		AfxMessageBox(strMessageBox);
		return FALSE;
	}

    
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CSeekersDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CSeekersApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	return CWinApp::ExitInstance();
}
