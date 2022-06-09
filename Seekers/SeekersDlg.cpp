
// SeekersDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibResolution.h"
#include "Seekers.h"
#include "SeekersDlg.h"
#include "afxdialogex.h"
#include "DlgSwitchBatch.h"
#include "DlgProductManager.h"
#include "DlgLogin.h"
#include "DlgHelp.h"
#include "DlgDatabaseHistory.h"
#include "DlgHistory.h"
#include "DlgProductParam.h"
#include "DlgSystemSet.h"
#include "DlgTcpConnect.h"
#include "DlgCreateBaseLine.h"
#include "DlgDailyButton.h"
HHOOK g_hook = NULL;
HHOOK g_hookMenu = NULL;

static LRESULT CALLBACK WindowsHook(int code, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
WNDPROC oldMenuWndProc = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static UINT indicators[] =
{
	IDS_STRING_READY,
	IDS_STRING_GRAB1,
	IDS_STRING_GRAB2,
	IDS_STRING_DETECT1,
	IDS_STRING_DETECT2,
	IDS_STRING_QUEUEINFO,
	IDS_STRING_USERNAME,
	IDS_STRING_TIME
};
// CSeekersDlg 对话框
const int SUB_WINDOW_COUNT = 5;//2019.8.4修改
const int SUB_COUNT = 5;
const int CROP_SIZE = 200;



static BOOL SystemCall(const CString& strMessage, void* wParam, void* lParam)
{

	CString strParam;
	strParam.Format("%s", (char*)wParam);

	if (strMessage == "GETPARAM")
	{
		if (strParam == "StandardLabelLocation")//贴标位置
		{
			g_pCommunicate->DoSomething("GETPARAM", "StandardLabelLocation", lParam);

		}
		else if (strParam == "LabelOffsetDev")//贴标偏移公差
		{
			g_pCommunicate->DoSomething("GETPARAM", "LabelLocationDev", lParam);

		}
		else if (strParam == "LabelLocationOffset")//贴标补偿
		{
			g_pCommunicate->DoSomething("GETPARAM", "LabelLocationOffset", lParam);

		}
		else if (strParam == "LabelRealLocation")//监控到的贴标位置
		{
			g_pCLabeller->DoSomething("GETPARAM", "LabelRealLocation", lParam);


		}

	}
	else   if (strMessage == "SETPARAM")
	{
		if (strParam == "StandardLabelLocation")//贴标位置
		{
			g_pCLabeller->DoSomething("SETPARAM", "StandardLabelLocation", lParam);

		}
		else if (strParam == "LabelOffsetDev")//贴标偏移公差
		{
			g_pCLabeller->DoSomething("SETPARAM", "LabelOffsetDev", lParam);

		}
		else if (strParam == "LabelLocationOffset")//贴标补偿
		{
			g_pCLabeller->DoSomething("SETPARAM", "LabelLocationOffset", lParam);

		}


	}

	return TRUE;
}
CSeekersDlg::CSeekersDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSeekersDlg::IDD, pParent)
	, m_hvec_MainWindowsHandle(1)
	, m_hvec_HistroyWindowsHandle(1)
	, m_hvec_SubWindowsHandle(1)
	, m_hvec_ResultSizeHandle(1)
	, m_hovec_ResultGrayImage(1)
	, m_hovec_ResultRGBImage(1)
	, m_hovec_Histroy(1)
	, m_hovec_Histroy_Image(1)
	, m_hovec_ResultRegion(1)
	, m_pSubSizeDlg(NULL)
	, m_nCurrentDisplayView(0)
	, m_nSubPage(2)//2019.8
	, m_pStartUp(NULL)
	, m_bRunning(TRUE)
	, m_hvec_PerAtCuoWei(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pMenuTools = NULL;
	m_ListLabelFont.CreatePointFont(90, "Microsoft YaHei");
	m_hvExitTimeCount.Clear();
	m_hvExitLightTimeCount.Clear();
}

CSeekersDlg::~CSeekersDlg()
{
	if (m_pMenuTools)
	{
		delete m_pMenuTools;
	}

	m_ListLabelFont.DeleteObject();

}
void CSeekersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_staticTitle);
	//  DDX_Control(pDX, IDC_STATIC_YIELD, m_STC_CountTrip1);
	//  DDX_Control(pDX, IDC_STATIC_MACHINESPEED, m_staticMachineSpeed);
	DDX_Control(pDX, IDC_STATIC_BATCH, m_staticBatch);
	DDX_Control(pDX, IDC_STATIC_PRODUCTNAME, m_staticProductName);
	//  DDX_Control(pDX, IDC_STATIC_C_YIELD, m_cstaticYield);
	//  DDX_Control(pDX, IDC_STATIC_C_MACHINESPEED, m_cstaticMachineSpeed);
	DDX_Control(pDX, IDC_STATIC_C_BATCH, m_cstaticBatch);
	DDX_Control(pDX, IDC_STATIC_C_PRODUCTNAME, m_cstaticProductName);
	DDX_Control(pDX, IDC_BUTTON_EXIT_APPLICATION, m_btnExitAppication);
	DDX_Control(pDX, IDC_BUTTON_SUBBACK, m_btnSubBack);
	DDX_Control(pDX, IDC_BUTTON_SUBNEXT, m_btnSubNext);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_SWITCH_BATCH, m_btnSwitchBatch);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btnLogin);
	//  DDX_Control(pDX, IDC_BUTTON_MAP, m_btnMap);
	DDX_Control(pDX, IDC_BUTTON_PRODUCT_MANAGER, m_btnProductManager);
	DDX_Control(pDX, IDC_BUTTON_SWITCH_VIEWS, m_btnSwitchViews);
	DDX_Control(pDX, IDC_BUTTON_TOOLS, m_btnTools);
	DDX_Control(pDX, IDC_BTN_DRAWLEFTMODEL, m_BTN_DrawLeftModel);
	//  DDX_Control(pDX, IDC_BTN_DRAWRIGHTMODEL, m_BTN_DrawRightModel);
	DDX_Control(pDX, IDC_STC_METER, m_STC_CMeter);
	DDX_Control(pDX, IDC_STC_METERNAME, m_STC_CMeterName);
	DDX_Control(pDX, IDC_LIST_LABELCOUNT, m_LST_LabelCount);
	DDX_Control(pDX, IDC_BTN_CLEARLABELCOUNT, m_BTN_ClearLabeCount);
	//  DDX_Control(pDX, IDC_STC_COUNT0, m_STC_CountTrip0);
	//  DDX_Control(pDX, IDC_STC_COUNT1, m_STC_CountTrip1);
	DDX_Control(pDX, IDC_BTN_CALIBSET, m_btnCalibResolution);
	DDX_Control(pDX, IDC_BTN_FINDEDAGE, m_BTN_CFindEdage);
	DDX_Control(pDX, IDC_BTN_LIGHTONOFF, m_BTN_CLightOnOff);
	DDX_Control(pDX, IDC_BTN_CLEARDISK, m_btn_ClearDisk);
	DDX_Control(pDX, IDC_LIST_LABLECAOWEI, m_LST_LableCaoWei);
	DDX_Control(pDX, IDC_BTN_DAILYUSE, m_BTN_DailyUse);
}

BEGIN_MESSAGE_MAP(CSeekersDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CSeekersDlg::OnBnClickedButtonStart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_EXIT_APPLICATION, &CSeekersDlg::OnBnClickedButtonExitApplication)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH_VIEWS, &CSeekersDlg::OnBnClickedButtonSwitchViews)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH_BATCH, &CSeekersDlg::OnBnClickedButtonSwitchBatch)
	//	ON_BN_CLICKED(IDC_BUTTON_MAP, &CSeekersDlg::OnBnClickedButtonMap)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PRODUCT_MANAGER, &CSeekersDlg::OnBnClickedButtonProductManager)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CSeekersDlg::OnBnClickedButtonLogin)
	ON_REGISTERED_MESSAGE(WM_DISP, &CSeekersDlg::OnDisp)
	ON_REGISTERED_MESSAGE(WM_GRABBER, &CSeekersDlg::OnGrabber)
	ON_REGISTERED_MESSAGE(WM_DETECT, &CSeekersDlg::OnDetect)
	ON_REGISTERED_MESSAGE(WM_RESTARTPROCESS, &CSeekersDlg::OnRestartProcess)
	ON_BN_CLICKED(IDC_BUTTON_SUBNEXT, &CSeekersDlg::OnBnClickedButtonSubnext)
	ON_BN_CLICKED(IDC_BUTTON_SUBBACK, &CSeekersDlg::OnBnClickedButtonSubback)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_STN_CLICKED(IDC_STATIC_BATCH, &CSeekersDlg::OnStnClickedStaticBatch)
	ON_STN_DBLCLK(IDC_STATIC_TITLE, &CSeekersDlg::OnStnDblclickStaticTitle)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_TOOLS, &CSeekersDlg::OnBnClickedButtonTools)
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	//ON_COMMAND(ID_MENU_LANESET, &CSeekersDlg::OnMenuLaneset)
	//ON_COMMAND(ID_MENU_LINEMATCHSET, &CSeekersDlg::OnMenuLinematchset)
	ON_COMMAND(ID_MENU_PROSET, &CSeekersDlg::OnMenuProset)
	ON_COMMAND(ID_MENU_ALGPARAM, &CSeekersDlg::OnMenuAlgparam)
	ON_COMMAND(ID_MENU_RESTART, &CSeekersDlg::OnMenuRestart)
	ON_BN_CLICKED(IDC_BTN_DRAWLEFTMODEL, &CSeekersDlg::OnBnClickedBtnDrawleftmodel)
	//	ON_BN_CLICKED(IDC_BTN_DRAWRIGHTMODEL, &CSeekersDlg::OnBnClickedBtnDrawrightmodel)
	ON_COMMAND(ID_MENU_TCPCONNECT, &CSeekersDlg::OnMenuTcpconnect)
	ON_BN_CLICKED(IDC_BTN_CLEARLABELCOUNT, &CSeekersDlg::OnBnClickedBtnClearlabelcount)

	ON_BN_CLICKED(IDC_BTN_CALIBSET, &CSeekersDlg::OnBnClickedBtnCalibset)
	ON_BN_CLICKED(IDC_BTN_FINDEDAGE, &CSeekersDlg::OnBnClickedBtnFindedage)
	ON_BN_CLICKED(IDC_BTN_LIGHTONOFF, &CSeekersDlg::OnBnClickedBtnLightonoff)
	ON_BN_CLICKED(IDC_BTN_CLEARDISK, &CSeekersDlg::OnBnClickedBtnCleardisk)
	ON_COMMAND(ID_MENU_GUIGESHEZHI, &CSeekersDlg::OnMenuGuigeshezhi)
	ON_COMMAND(ID_MENU_PLC, &CSeekersDlg::OnMenuPlc)
	ON_BN_CLICKED(IDC_BTN_DAILYUSE, &CSeekersDlg::OnBnClickedBtnDailyuse)
END_MESSAGE_MAP()


// CSeekersDlg 消息处理程序
LRESULT CALLBACK keyFunc(int nCode, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = ::CallNextHookEx(g_hook, nCode, wParam, lParam);
	if (nCode == HC_ACTION)
	{
		if (wParam == VK_SNAPSHOT)
		{
			CString strFolder;
			strFolder.Format("%s\\Screen", GetCurrentAppPath());
			CreateDirectory(strFolder, NULL);
			int nFileCount = 0;
			CFileFind finder;
			BOOL bworking = finder.FindFile(strFolder + "\\*.*");
			while (bworking)
			{
				bworking = finder.FindNextFile();
				if (finder.IsDots())
					continue;
				if (!finder.IsDirectory())
					nFileCount++;
			}
			CString strFileName;
			strFileName.Format("%s\\%d.bmp", strFolder, ++nFileCount);
			CDC *pDC;//屏幕DC
			pDC = CDC::FromHandle(GetDC(NULL));//获取当前整个屏幕DC
			int BitPerPixel = pDC->GetDeviceCaps(BITSPIXEL);//获得颜色模式
			int Width = pDC->GetDeviceCaps(HORZRES);
			int Height = pDC->GetDeviceCaps(VERTRES);
			CDC memDC;//内存DC
			memDC.CreateCompatibleDC(pDC);
			CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
			memBitmap.CreateCompatibleBitmap(pDC, Width, Height);
			oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
			memDC.BitBlt(0, 0, Width, Height, pDC, 0, 0, SRCCOPY);//复制屏幕图像到内存DC
			//以下代码保存memDC中的位图到文件
			BITMAP bmp;
			memBitmap.GetBitmap(&bmp);//获得位图信息
			FILE *fp;
			fopen_s(&fp, strFileName, "w+b");
			BITMAPINFOHEADER bih = { 0 };//位图信息头
			bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
			bih.biCompression = BI_RGB;
			bih.biHeight = bmp.bmHeight;//高度
			bih.biPlanes = 1;
			bih.biSize = sizeof(BITMAPINFOHEADER);
			bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
			bih.biWidth = bmp.bmWidth;//宽度
			BITMAPFILEHEADER bfh = { 0 };//位图文件头
			bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
			bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
			bfh.bfType = (WORD)0x4d42;
			fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头 
			fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头
			byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
			GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, Height, p,
				(LPBITMAPINFO)&bih, DIB_RGB_COLORS);//获取位图数据
			fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
			delete[] p;
			fclose(fp);
			memDC.SelectObject(oldmemBitmap);
			//客户说图片太卡,采用Halcon保存为JPG的
			HObject SaveImage;
			ReadImage(&SaveImage, HTuple(strFileName));
			DeleteFile(HTuple(strFileName));
			strFileName.Delete(strFileName.GetLength() - 4, 4);
			WriteImage(SaveImage, "jpeg", NULL, HTuple(strFileName));
			return TRUE;
		}
		else if ((wParam == VK_C || wParam == VK_c) && lParam >> 30 & 1 && lParam >> 29 & 1)
		{
			((CSeekersDlg*)g_pMainDialog)->OnBnClickedButtonSwitchViews();
			return TRUE;
		}
	}
	return lResult;
}

static LRESULT CALLBACK WindowsHook(int code, WPARAM wParam, LPARAM lParam)
{
	CWPSTRUCT* pStruct = (CWPSTRUCT*)lParam;

	while (code == HC_ACTION)
	{
		HWND hWnd = pStruct->hwnd;

		//捕捉菜单创建WM_CREATE, 0x01E2是什么消息我不知道,呵呵,这一段参考了别人的代码
		if (pStruct->message != WM_CREATE /*&& pStruct->message != 0x01E2*/)
			break;
		TCHAR sClassName[10];
		int Count = ::GetClassName(hWnd, sClassName, sizeof(sClassName) / sizeof(sClassName[0]));

		//检查是否菜单窗口
		if (Count != 6 || _tcscmp(sClassName, _T("#32768")) != 0)
		{
			break;
		}

		WNDPROC lastWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		if (lastWndProc != WndProc)
		{
			//替换菜单窗口过程函数
			SetWindowLongPtrA(hWnd, GWLP_WNDPROC, (LONG64)WndProc);

			//保留旧的窗口过程
			oldMenuWndProc = lastWndProc;
		}
		break;
	}

	return CallNextHookEx((HHOOK)WH_CALLWNDPROC, code, wParam, lParam);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	switch (message)
	{

		//首先要去掉菜单窗口的WS_BORDER风格和WS_EX_DLGMODALFRAME , WS_EX_WINDOWEDGE扩展风格
	case WM_CREATE:
	{
		lResult = CallWindowProcA(oldMenuWndProc, hWnd, message, wParam, lParam);
		DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
		DWORD dwNewStyle = (dwStyle & ~WS_BORDER);
		::SetWindowLong(hWnd, GWL_STYLE, dwNewStyle);
		DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
		DWORD dwNewExStyle = (dwExStyle & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE));
		::SetWindowLong(hWnd, GWL_EXSTYLE, dwNewExStyle);
		return lResult;
		break;
	}

	//这里/阻止非客户区的绘制
	case WM_PRINT:
		return CallWindowProc(oldMenuWndProc, hWnd, WM_PRINTCLIENT, wParam, lParam);
		break;

		//最后, 因为你在MeasureItem里指定的菜单大小，系统会自动替你加上边框，我们必须去掉这部分额外的尺寸
	case WM_WINDOWPOSCHANGING:
	{
		//将菜单大小改小
		LPWINDOWPOS lpPos = (LPWINDOWPOS)lParam;
		lpPos->cx -= 2 * GetSystemMetrics(SM_CXBORDER) + 4;
		lpPos->cy -= 2 * GetSystemMetrics(SM_CYBORDER) + 4;
		lResult = CallWindowProc(oldMenuWndProc, hWnd, message, wParam, lParam);
		return 0;
	}
	case WM_GETICON:

		return 0;
		break;
	default:
		return CallWindowProcA(oldMenuWndProc, hWnd, message, wParam, lParam);
	}
}
void CSeekersDlg::Verify()
{
	DHInterface* pVerify = LoadDHInterFaceDll("LCDVerify");
	if (NULL == pVerify)
	{
		exit(0);
	}
	if (TRUE != pVerify->DoSomething("Verify", NULL, NULL))
	{
		exit(0);
	}
	return;
}

void CSeekersDlg::CrashRecord()
{
	m_pCrash = LoadDHInterFaceDll("CrashRecord");
	m_pCrash->DoSomething("CrashRecord", NULL, NULL);
}










BOOL CSeekersDlg::OnInitDialog()
{

	//Verify();

	LoadVersionInfoList();//加载版本信息
	CrashRecord();
	ShowWindow(SW_HIDE);
	InitStartUp();
	/////////////////////////////////////////////////////////////////////////
	g_hook = ::SetWindowsHookEx(WH_KEYBOARD, keyFunc, AfxGetInstanceHandle(), GetCurrentThreadId());
	g_hookMenu = ::SetWindowsHookEx(WH_CALLWNDPROC, WindowsHook, AfxGetInstanceHandle(), GetCurrentThreadId());
	CDialogEx::OnInitDialog();
	SetWindowText("Seekers分条机全质量检测软件");
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	AdaptiveSize(m_hWnd);
	m_pStartUp->SetText("正在初始化异常提示窗口...");
	m_BrushBlack.CreateSolidBrush(RGB(0, 0, 0));
	ShowWindow(SW_SHOWMAXIMIZED);
	InitStatusBar();
	ShowWindow(SW_HIDE);
	//读取参数
	m_pStartUp->SetText("正在初始化全局变量...");
	InitGlobalVariables();
	Sleep(10);


	//初始化相机
	m_pStartUp->SetText("正在打开CCD...");
	InitGrabber();
	Sleep(10);

	//初始化队列
	m_pStartUp->SetText("正在初始化数据队列...");
	InitQueues();
	Sleep(300);

	//初始化窗口
	m_pStartUp->SetText("正在初始化显示窗口...");
	InitWindowShow();
	Sleep(300);
	//初始化队列
	m_pStartUp->SetText("正在初始化UI尺寸...");
	InitUI();
	Sleep(300);
	//初始化子窗口
	m_pStartUp->SetText("正在初始化子窗口...");
	InitSubDlg();
	Sleep(300);
	//初始化线程
	m_pStartUp->SetText("正在开启运转线程...");
	InitThread();
	Sleep(300);
	//开启定时器
	m_pStartUp->SetText("正在开启定时器...");
	ShowButton(0);
	InitLabelCountList();
	InitLabelCaoWeiList();
	InitializeCriticalSection(&g_CsClearLabelCount);
	InitializeCriticalSection(&g_CsPopWindowChange);



	SetTimer(UM_TIMER_QUEUEINFO, 500, NULL);

	SetTimer(UM_TIMER_STARTGRAB, 2000, NULL);
	SetTimer(UM_TIMER_TIME, 5000, NULL);
	//显示
	m_pStartUp->SetText("启动完成 √");
	Sleep(300);
	m_pStartUp->PostMessageA(WM_CLOSE);
	m_pStartUp->ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOWMAXIMIZED);
	g_pLog->DoSomething("CREATEEXCEPTION", NULL, NULL);
	BOOL bShow = TRUE;
	g_pLog->DoSomething("SETPARAM", "SHOWEXCEPTION", &bShow);



	//版本确认
	CString strLastVersion;
	GetPrivateProfileString("INFO", "SeekersVersion", "Ver1.0.0.1", strLastVersion.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	strLastVersion.ReleaseBuffer();
	if (g_StrCurrentVersion != strLastVersion)
	{
		CString strException;
		strException.Format("版本更新确认,当前机台上版本是%s,目前打开的版本是%s,若确认更新请在常用-Seekers版本号界面中更新版本号", strLastVersion, g_StrCurrentVersion);
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}

	g_pCLabeller->DoSomething("SETPARAM", "YELLOW_LIGHT", NULL);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSeekersDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialogEx::OnPaint();
		COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
		CRect   rcFullDlg, rcTitle, rcCtl, rcClient;
		CPaintDC   dc(this);
		GetClientRect(rcFullDlg);
		GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle);
		rcTitle.right = rcFullDlg.right;
		rcClient = rcFullDlg;
		rcClient.top = rcTitle.bottom + 3;
		dc.FillSolidRect(rcClient, RGB(255, 255, 255));
		dc.FillSolidRect(rcTitle, MainColor);

		int nSubR, nSubG, nSubB;
		nSubR = max(g_Blendent.m_Blendent.Dlg.nR - 30, 0);
		nSubG = max(g_Blendent.m_Blendent.Dlg.nG - 30, 0);
		nSubB = max(g_Blendent.m_Blendent.Dlg.nB - 30, 0);
		COLORREF SubColor = RGB(nSubR, nSubG, nSubB);

		//画标题下边的线
		CPen pen, pen1, pen2, pen3;
		pen.CreatePen(PS_SOLID, 1, SubColor);//109,177,61 
		dc.SelectObject(pen);
		dc.MoveTo(rcTitle.left, rcTitle.bottom);
		dc.LineTo(rcTitle.right, rcTitle.bottom);
		pen1.CreatePen(PS_SOLID, 1, RGB(180, 180, 180));//94,162,46
		dc.SelectObject(pen1);
		dc.MoveTo(rcTitle.left, rcTitle.bottom + 1);
		dc.LineTo(rcTitle.right, rcTitle.bottom + 1);
		pen2.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));//154,222,106
		dc.SelectObject(pen2);
		dc.MoveTo(rcTitle.left, rcTitle.bottom + 2);
		dc.LineTo(rcTitle.right, rcTitle.bottom + 2);


		//状态栏上面画线
		CRect rcStatusBar;
		m_wndStatusBar.GetWindowRect(rcStatusBar);
		dc.SelectObject(pen);
		dc.MoveTo(rcStatusBar.left, rcStatusBar.top);
		dc.LineTo(rcStatusBar.right, rcStatusBar.top);
		dc.SelectObject(pen1);
		dc.MoveTo(rcStatusBar.left, rcStatusBar.top - 1);
		dc.LineTo(rcStatusBar.right, rcStatusBar.top - 1);
		dc.SelectObject(pen2);
		dc.MoveTo(rcStatusBar.left, rcStatusBar.top - 2);
		dc.LineTo(rcStatusBar.right, rcStatusBar.top - 2);




		pen3.CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
		dc.SelectObject(pen3);
		GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(rcCtl);
		ScreenToClient(rcCtl);
		dc.MoveTo(rcCtl.left - 1, rcCtl.top - 1);
		dc.LineTo(rcCtl.right + 1, rcCtl.top - 1);
		dc.LineTo(rcCtl.right + 1, rcCtl.bottom + 1);
		dc.LineTo(rcCtl.left - 1, rcCtl.bottom + 1);
		dc.LineTo(rcCtl.left - 1, rcCtl.top - 1);

		GetDlgItem(IDC_PICTURE_SUB)->GetWindowRect(rcCtl);
		ScreenToClient(rcCtl);
		dc.MoveTo(rcCtl.left - 1, rcCtl.top - 1);
		dc.LineTo(rcCtl.right + 1, rcCtl.top - 1);
		dc.LineTo(rcCtl.right + 1, rcCtl.bottom + 1);
		dc.LineTo(rcCtl.left - 1, rcCtl.bottom + 1);
		dc.LineTo(rcCtl.left - 1, rcCtl.top - 1);

		CDialogEx::OnPaint();
		return;
		CString strLogo;
		strLogo.Format("%s\\res\\logo.png", GetCurrentAppPath());
		CRect rcLogo;
		rcLogo.left = 0;
		rcLogo.top = 0;
		rcLogo.bottom = rcTitle.bottom;//标题高39
		rcLogo.right = rcLogo.left + LOGO_WIDTH;//logo宽163
		CImage Pic;
		Pic.Load(strLogo);
		if (Pic.IsNull())
		{
			return;
		}
		if (Pic.GetBPP() == 32)
		{
			for (int i = 0; i < Pic.GetWidth(); i++)
			{
				for (int j = 0; j < Pic.GetHeight(); j++)
				{
					unsigned char* pucColor = reinterpret_cast<unsigned char *>(Pic.GetPixelAddress(i, j));
					pucColor[0] = pucColor[0] * pucColor[3] / 255;
					pucColor[1] = pucColor[1] * pucColor[3] / 255;
					pucColor[2] = pucColor[2] * pucColor[3] / 255;
				}
			}
		}
		::SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);
		::SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);
		Pic.Draw(dc.GetSafeHdc(), rcLogo);


		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSeekersDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSeekersDlg::InitGrabber()//初始化采集端信息
{

	CString strSection;

	for (int i = 0; i < g_nGrabberCount; i++)//读取相机的分辨率
	{
		strSection.Format("GRABBER%d", i);
		int nGrabberType = GetPrivateProfileInt(strSection, "TYPE", 0, g_strConfigSystemFile);
		int nCameraMode = GetPrivateProfileInt(strSection, "CameraMode", 2, g_strConfigSystemFile);
		int nCameraLinkSN = GetPrivateProfileInt(strSection, "CameraLinkSN", 0, g_strConfigSystemFile);
		g_nGrabberType = nGrabberType;
		CString strDeviceMark, strDeviceFile, strDeviceName;
		if (0 == nGrabberType)
		{
			GetPrivateProfileString(strSection, "模拟采集初始化文件", "(local)", strDeviceFile.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			strDeviceFile.ReleaseBuffer();

		}
		else if (1 == nGrabberType)
		{
			GetPrivateProfileString(strSection, "Dalsa采集初始化文件", "(local)", strDeviceFile.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			GetPrivateProfileString(strSection, "Dalsa相机序列号", "(local)", strDeviceMark.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			strDeviceMark.ReleaseBuffer();
			strDeviceFile.ReleaseBuffer();
		}
		else if (2 == nGrabberType)
		{
			GetPrivateProfileString(strSection, "AVT采集初始化文件", "(local)", strDeviceFile.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			GetPrivateProfileString(strSection, "AVT相机序列号", "(local)", strDeviceMark.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			strDeviceMark.ReleaseBuffer();
			strDeviceFile.ReleaseBuffer();
		}
		else if (3 == nGrabberType)
		{
			GetPrivateProfileString(strSection, "MER采集初始化文件", "(local)", strDeviceFile.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			GetPrivateProfileString(strSection, "MER相机序列号", "(local)", strDeviceMark.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
			strDeviceMark.ReleaseBuffer();
			strDeviceFile.ReleaseBuffer();
		}
		int nGrabberSN = GetPrivateProfileInt(strSection, "SN", 0, g_strConfigSystemFile);
		CString strDeviceNameDef;
		strDeviceNameDef.Format("%d相机", nGrabberSN + 1);//默认值
		GetPrivateProfileString(strSection, "NAME", strDeviceNameDef, strDeviceName.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
		strDeviceName.ReleaseBuffer();
		g_vec_GrabberName.push_back(strDeviceName);
		s_GBINITSTRUCT InitParam;
		InitParam.nGrabberSN = nGrabberSN;
		InitParam.CallBackFunc = HDevExportCpp::_hcppthread_acquisition_thread;
		InitParam.Context = this;
		/*InitParam.iReserve1 = CL_Full;
		InitParam.iReserve2 = 0;*/
		InitParam.iReserve1 = nCameraMode;//0是Base 1：medium 2：full
		InitParam.iReserve2 = nCameraLinkSN;// 
		InitParam.iGrabberTypeSN = nGrabberType;
		sprintf_s(InitParam.strDeviceMark, "%s", strDeviceMark);//序列号
		sprintf_s(InitParam.strDeviceName, "%s", strDeviceName);
		sprintf_s(InitParam.strGrabberFile, "%s\\Config\\Products\\%s\\SCIGrabber\\%s", GetCurrentAppPath(), g_strProductName, strDeviceFile);//设备初始化文件名
		//相机类型:0为模拟采集卡，1 dalsa 线阵2 为AVT相机， 3  MER
		//显示加载日志类
		DHInterface* pGrabber = NULL;
		switch (InitParam.iGrabberTypeSN)
		{
		case 0:
			pGrabber = LoadDHInterFaceDll("SCIGrabberSG");
			break;
		case 1:
			pGrabber = LoadDHInterFaceDll("SCIGrabberDS");
			break;
		case 2:
			pGrabber = LoadDHInterFaceDll("SCIGrabberAVT");
			break;
		case 3:
			pGrabber = LoadDHInterFaceDll("SCIGrabberMER");
			break;
		}
		if (TRUE == pGrabber->DoSomething("INITIALIZE", &InitParam, NULL))//pGrabber->Init(&InitParam)
		{
			g_vec_Grabber.push_back(pGrabber);
		}
		else
		{
			CString strMessageBox;
			strMessageBox.Format("相机%2d初始化失败\r\n", i);
			AfxMessageBox(strMessageBox);
			CDialogEx::OnCancel();
		}
	}


	/////相机设置防呆 防止前后相机设置的高度不一致
	if (g_nGrabberCount == 2)
	{

		GBParamID Param = GBImageHeight;
		__int64 nHeight1(0), nHeight2(0);
		g_vec_Grabber[0]->DoSomething("GETPARAM", &Param, &nHeight1);
		g_vec_Grabber[1]->DoSomething("GETPARAM", &Param, &nHeight2);
		if (nHeight1 != nHeight2)
		{

			AfxMessageBox("前后相机采集图像高度不一致，请确认！");
			return;
		}

		g_pCommunicate->DoSomething("SETPARAM", "SetParam63", &nHeight1);


	}






}

void CSeekersDlg::Release()
{
	for (UINT i = 0; i < g_vec_Grabber.size(); i++)
	{
		//  g_vec_Grabber[i]->DoSomething("STOPGRAB",NULL,NULL);

		g_vec_Grabber[i]->DoSomething("CLOSE", NULL, NULL);
	}
}
void CSeekersDlg::InitStartUp()
{

	m_pStartUp = new CDlgStartUp;
	m_pStartUp->Create(CDlgStartUp::IDD, this);
	m_pStartUp->CenterWindow();
	m_pStartUp->ShowWindow(SW_NORMAL);
	m_pStartUp->UpdateWindow();

}
CString CSeekersDlg::GetDataBaseConnection()
{
	CString strConnect, strServer, strUID, strPWD;
	GetPrivateProfileString("DATABASE", "SERVER", "daheng-PC", strServer.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	strServer.ReleaseBuffer();
	GetPrivateProfileString("DATABASE", "USERID", "sa", strUID.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	strUID.ReleaseBuffer();
	GetPrivateProfileString("DATABASE", "PASSWORD", "daheng", strPWD.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	strPWD.ReleaseBuffer();
	//strConnect = "Driver=SQL Server;Database=dahengimage;Server=daheng-PC;UID=sa;PWD=daheng";
	strConnect.Format("Driver=SQL Server;Database=dahengimage;Server=%s;UID=%s;PWD=%s", strServer, strUID, strPWD);
	return strConnect;
}


void CSeekersDlg::InitMenu()
{
	m_pMenuTools = new CMenuExt;
	m_pMenuTools->SetAttributes(95, "Microsoft YaHei", 40, 35,
		g_Blendent.m_Blendent.Menu.BKColorNormal,
		g_Blendent.m_Blendent.Menu.BKColorFocus,
		g_Blendent.m_Blendent.Menu.BDColorFocus,
		g_Blendent.m_Blendent.Menu.TextColorNormal,
		g_Blendent.m_Blendent.Menu.TextColorFocus,
		g_Blendent.m_Blendent.Menu.IconBKColor,
		g_Blendent.m_Blendent.Menu.SPColor);
	m_pMenuTools->LoadMenu(IDR_MENU_TOOLS);
	m_pMenuTools->ChangeMenuItem(m_pMenuTools);
}

void CSeekersDlg::Reset()
{

	g_pLog->DoSomething("EXCEPTION", "正在关闭相机...", NULL);
	for (int i = 0; i < g_nGrabberCount; i++)
	{
		if (TRUE != g_vec_Grabber[i]->DoSomething("STOPGRAB", NULL, NULL))
		{
			CString strError;
			strError.Format("相机%d停止采集失败", i);
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strError, NULL);
		}
	}
	g_pLog->DoSomething("EXCEPTION", "正在清空队列...", NULL);
	try
	{
		for (int i = 0; i < g_hvec_QueueImages.Length(); i++)
		{
			SetMessageQueueParam(g_hvec_QueueImages[i].T(), "flush_queue", 1);
		}
		for (int i = 0; i < g_hvec_QueueResults.Length(); i++)
		{
			SetMessageQueueParam(g_hvec_QueueResults[i].T(), "flush_queue", 1);
		}
		for (int i = 0; i < g_hvec_QueueDisplay.Length(); i++)
		{
			SetMessageQueueParam(g_hvec_QueueDisplay[i].T(), "flush_queue", 1);
		}
	}
	catch (HException e)
	{
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)e.ErrorMessage().Text(), NULL);
	}
	g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)"正在重置接口卡...", NULL);


	//判断是否重新打开相机
	CString strBtnText;
	GetDlgItemTextA(IDC_BUTTON_START, strBtnText);
	if ("停止" == strBtnText)//如果当前是启动状态
	{
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)"正在重新打开相机...", NULL);
		for (int i = g_nGrabberCount - 1; i >= 0; i--)
		{
			if (TRUE != g_vec_Grabber[i]->DoSomething("STARTGRAB", NULL, NULL))//从后向前依次打开,防止后面相机该采集了但是还没有打开
			{
				CString strError;
				strError.Format("相机%d打开采集失败", i);
				g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strError, NULL);
			}
		}
	}
	g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)"重置完成", NULL);
}

void CSeekersDlg::ClearQueues()//清空队列
{


	for (int i = 0; i < g_nGrabberCount; i++)
	{
		SetMessageQueueParam(g_hvec_QueuePintu[i].T(), "flush_queue", 1);
		SetMessageQueueParam(g_hvec_QueueImages[i].T(), "flush_queue", 1);
		SetMessageQueueParam(g_hvec_QueueResults[i].T(), "flush_queue", 1);

	}

	for (int i = 0; i < 1; i++)
	{
		SetMessageQueueParam(g_hvec_QueueDisplay[i].T(), "flush_queue", 1);
	}
}

void CSeekersDlg::InitGlobalVariables()
{
	//初始化全局文件名称
	g_strConfigSystemFile.Format("%s/Config/System.vis", GetCurrentAppPath());//获取配置文件名称
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(g_strConfigSystemFile))
	{
		CString strMessageBox;
		strMessageBox.Format("%s\r\n文件不存在,无法正常运行程序", g_strConfigSystemFile);
		AfxMessageBox(strMessageBox);
		exit(1);
	}
	g_strConfigCommonFile.Format("%s/Config/Common.dh", GetCurrentAppPath());//获取配置文件名称
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(g_strConfigCommonFile))
	{
		CString strMessageBox;
		strMessageBox.Format("%s\r\n文件不存在,无法正常运行程序", g_strConfigCommonFile);
		AfxMessageBox(strMessageBox);
		exit(1);
	}
	//读取参数
	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读属性
	//启动信息
	GetPrivateProfileString("INFO", "PRODUCTNAME", "SBP_CP", g_strProductName.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strProductName.ReleaseBuffer();//读取产品名称
	GetPrivateProfileString("INFO", "BATCH", "早班", g_strBatch.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strBatch.ReleaseBuffer();//读取班次
	g_dwYield = GetPrivateProfileDouble("INFO", "YIELD", 0, g_strConfigSystemFile);//产量
	g_dCoatThickness = GetPrivateProfileDouble("INFO", "g_dCoatThickness", -1, g_strConfigSystemFile);//冷压厚度值
	GetPrivateProfileString("INFO", "g_strLabelTypeName", " ", g_strLabelTypeName.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strLabelTypeName.ReleaseBuffer();
	GetPrivateProfileString("INFO", "g_strBatchStartTime", "888888", g_strBatchStartTime.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strBatchStartTime.ReleaseBuffer();//换卷起始时间
	g_bUseAutoFlag = GetPrivateProfileInt("INFO", "g_bUseAutoFlag", 0, g_strConfigSystemFile);//允许使用自动换型
	g_bUseSeekersWithoutServer = GetPrivateProfileInt("INFO", "g_bUseSeekersWithoutServer", 0, g_strConfigSystemFile);//是否隐藏掉本地换卷和登陆
	//相机个数
	GetPrivateProfileString("INFO", "OperatorPassWord", "888888", g_strOperatorPassWord.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strOperatorPassWord.ReleaseBuffer();//操作员密码
	GetPrivateProfileString("INFO", "EngineerPassWord", "ATL2020", g_strEngineerPassWord.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strEngineerPassWord.ReleaseBuffer();//工程师密码
	GetPrivateProfileString("INFO", "FactoryPassWord", "daheng", g_strFactoryPassWord.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
	g_strFactoryPassWord.ReleaseBuffer();//厂家密码

	g_bNGLabelTwoEA = GetPrivateProfileInt("INFO", "g_bNGLabelTwoEA", 0, g_strConfigSystemFile);//首间距NG2个EA都贴标


	CString strDefaultValue;
	strDefaultValue.Format("%s\\Run-x64\\Map\\ccd_data", GetCurrentAppPath());
	g_strSaveFullPath = GetPrivateProfileCString("StorgePath", "g_strSaveFullPath", strDefaultValue, g_strConfigSystemFile);
	strDefaultValue.Format("%s\\Run-x64\\Map\\ccd_data\\瑕疵数据", GetCurrentAppPath());
	g_strSaveBasePath = GetPrivateProfileCString("StorgePath", "g_strSaveBasePath", strDefaultValue, g_strConfigSystemFile);



	CString strPath;
	strPath.Format("%s\\Config\\Public\\g_hvXTuple.tup", GetCurrentAppPath());
	try
	{
		ReadTuple((HTuple)strPath, &g_hvXTuple);
	}
	catch (HException& e)
	{
		g_hvXTuple.Clear();
	}
	strPath.Format("%s\\Config\\Public\\g_hvYTuple.tup", GetCurrentAppPath());
	try
	{
		ReadTuple((HTuple)strPath, &g_hvYTuple);
	}
	catch (HException& e)
	{
		g_hvYTuple.Clear();
	}


	//系统配置


	g_tSystemSet.nHistoryCount = GetPrivateProfileInt("System", "HistoryCount", 20, g_strConfigSystemFile);//保留历史图片个数
	g_tSystemSet.nHistoryType = GetPrivateProfileInt("System", "HistoryType", 2, g_strConfigSystemFile);//保留历史图片类型
	g_tSystemSet.nRejectPoint = GetPrivateProfileInt("System", "RejectPoint", 9, g_strConfigSystemFile);//剔废显示点
	g_tSystemSet.nMergeResults = GetPrivateProfileInt("System", "MergeResults", 0, g_strConfigSystemFile);
	g_tSystemSet.nMergeParam = GetPrivateProfileInt("System", "MergeParam", 1, g_strConfigSystemFile);//默认正反相机使用相同的参数
	g_tSystemSet.bMirrorRow = GetPrivateProfileInt("System", "bMirrorRow", 0, g_strConfigSystemFile);

	g_tSystemSet.nOverTimeExit = GetPrivateProfileInt("System", "nOverTimeExit", 2, g_strConfigSystemFile);//超时退出
	g_tSystemSet.bEnableReloadParam = GetPrivateProfileInt("System", "bEnableReloadParam", 0, g_strConfigSystemFile);


	g_tSystemSet.bEnablePopWindowToWarn = GetPrivateProfileInt("System", "bEnablePopWindowToWarn", 0, g_strConfigSystemFile);
	g_tSystemSet.nMaxPopWindowNum = GetPrivateProfileInt("System", "nMaxPopWindowNum", 5, g_strConfigSystemFile);

	g_tSystemSet.bTileImage = GetPrivateProfileInt("System", "TileImage", 1, g_strConfigSystemFile);//默认正反相机使用相同的参数

	g_tSystemSet.nDistanceCamera = GetPrivateProfileInt("System", "DistanceCamera", 1500, g_strConfigSystemFile);
	g_tSystemSet.bAlignedTwoCamera = GetPrivateProfileInt("System", "AlignedTwoCamera", 1, g_strConfigSystemFile);
	g_tSystemSet.nCameraModel = GetPrivateProfileInt("System", "nCameraModel", 0, g_strConfigSystemFile);//写入历史回看类型


	g_tSystemSet.nOverTimeFrameValue = GetPrivateProfileInt("System", "nOverTimeFrameValue", 3, g_strConfigSystemFile);
	g_tSystemSet.nOverTimeValue = GetPrivateProfileInt("System", "nOverTimeValue", 400, g_strConfigSystemFile);
	g_tSystemSet.nOverTimeLimitLoginOff = GetPrivateProfileInt("System", "nOverTimeLimitLoginOff", 5, g_strConfigSystemFile);

	//产品配置
	g_ProductParam.emCoatType = (e_CoatType)GetPrivateProfileInt(g_strProductName, "CoatType", 0, g_strConfigSystemFile);
	g_ProductParam.emProductType = (e_ProductType)GetPrivateProfileInt(g_strProductName, "ProductType", 0, g_strConfigSystemFile);




	g_tCreateBaseLineInfo.nFirstCoatLowThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstCoatLowThresh", 10, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nFirstCoatHighThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstCoatHighThresh", 30, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nFirstFoilLowThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstFoilLowThresh", 100, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nFirstFoilHighThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstFoilHighThresh", 255, g_strConfigSystemFile);

	g_tCreateBaseLineInfo.nSecondCoatLowThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondCoatLowThresh", 10, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondCoatHighThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondCoatHighThresh", 30, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondFoilLowThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondFoilLowThresh", 100, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondFoilHighThresh = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondFoilHighThresh", 255, g_strConfigSystemFile);





	g_tCreateBaseLineInfo.nFirstLeftPosition = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstLeftPosition", -1, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nFirstRightPosition = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstRightPosition", -1, g_strConfigSystemFile);

	g_tCreateBaseLineInfo.nFirstRightPositionRow = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstRightPositionRow", -1, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nFirstLeftPositionRow = GetPrivateProfileInt("CreateBaseLineInfo", "nFirstLeftPositionRow", -1, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondLeftPositionRow = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondLeftPositionRow", -1, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondRightPositionRow = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondRightPositionRow", -1, g_strConfigSystemFile);



	g_tCreateBaseLineInfo.nSecondLeftPosition = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondLeftPosition", -1, g_strConfigSystemFile);
	g_tCreateBaseLineInfo.nSecondRightPosition = GetPrivateProfileInt("CreateBaseLineInfo", "nSecondRightPosition", -1, g_strConfigSystemFile);



	g_Count.nType = GetPrivateProfileInt("ERROR", "COUNTTYPE", 0, g_strConfigCommonFile);//缺陷种类数目

	g_Count.strAClassName.RemoveAll();
	g_Count.veCount[0].clear();
	g_Count.veCount[1].clear();
	g_nGrabberCount = GetPrivateProfileInt("GRABBER", "COUNT", 2, g_strConfigSystemFile);//读取相机的个数

	g_nDatabaseType = GetPrivateProfileInt("DATABASE", "TYPE", 0, g_strConfigSystemFile);
	g_pMainDialog = this;//当前对话框赋值
	for (int i = 0; i < g_nGrabberCount; i++)
	{
		double dwRow, dwColumn;
		CString strSection;
		strSection.Format("GRABBER%d", i);
		dwRow = GetPrivateProfileDouble(strSection, "ROWRESOLUTION", 0.1, g_strConfigCommonFile);
		dwColumn = GetPrivateProfileDouble(strSection, "COLUMNRESOLUTION", 0.1, g_strConfigCommonFile);
		g_vec_RowResolution.push_back(dwRow);
		g_vec_ColumnResolution.push_back(dwColumn);
	}
	for (int i = 0; i < 10; i++)//2019.8
	{

		SUBELEMENT tSubElement;
		tSubElement.nGrabberSN = 0;
		GenImageConst(&tSubElement.hoImage, "byte", 4096, 3000);
		GenImageConst(&tSubElement.hoSubImage, "byte", 200, 200);
		m_lstSubImage.push_back(tSubElement);
	}

	CString strValue;
	INITJUDGE tInitJude;
	for (int i = 0; i < g_nGrabberCount; i++)
	{
		tInitJude.nSN = i;
		tInitJude.strProductName = g_strProductName;
		g_Jugde[i].Init(tInitJude);

	}

	g_tSystemSet.nLaneCount = g_Jugde[0].m_tStandardSize.hvTripNum.I();
	g_ProductParam.emCoatType = (e_CoatType)g_Jugde[0].m_tStandardSize.hv_CoatType.I();
	g_ProductParam.emProductType = (e_ProductType)g_Jugde[0].m_tStandardSize.hv_ProductType.I();





	//加载各个模块动态库
	g_pLog = LoadDHInterFaceDll("LogViewer");
	g_pLog->DoSomething("SETPARAM", "OperatePath", g_strSaveFullPath.GetBuffer(MAX_PATH));
	g_CUpLoad.Init();///初始化客户端


	g_pLog->DoSomething("SETPARAM", "GONGWEI", g_CUpLoad.m_StrGongWeiName.GetBuffer(MAX_PATH));
	g_CUpLoad.m_StrGongWeiName.ReleaseBuffer();
	g_strSaveFullPath.ReleaseBuffer();
	g_pLog->DoSomething("SETPARAM", "Batch", g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();



	g_pStorage = LoadDHInterFaceDll("SCIStorage");
	g_pCommunicate = LoadDHInterFaceDll("SCICommunicate");

	CString strLastPath;
	STORAGEINIT tInit;
	tInit.pLogView = g_pLog;
	tInit.strProductName.Format("%s", g_strProductName);
	tInit.nProductType = g_ProductParam.emProductType;
	tInit.nCoatType = g_ProductParam.emCoatType;

	g_pStorage->DoSomething("SETPARAM", "GONGWEI", g_CUpLoad.m_StrGongWeiName.GetBuffer(MAX_PATH));
	g_pStorage->DoSomething("INITIALIZE", &tInit, NULL);
	g_pStorage->DoSomething("GETPARAM", "WRITEFLAG", &g_bWriteFlag);
	g_pStorage->DoSomething("SETPARAM", "SAVEPATH", g_strSaveBasePath.GetBuffer(MAX_PATH));
	g_strSaveBasePath.ReleaseBuffer();
	g_pStorage->DoSomething("SETPARAM", "BATCH", g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();

	COMMUNICATEINIT tInitCom;
	tInitCom.pLogView = g_pLog;
	tInitCom.strProductName.Format("%s", g_strProductName);
	tInitCom.pDlg = this;
	tInitCom.pSysCall = SystemCall;
	tInitCom.nCoatType = g_ProductParam.emCoatType;
	tInitCom.strGongWei = g_CUpLoad.m_StrGongWeiName;
	g_pCommunicate->DoSomething("SETPARAM", "BATCH", g_strBatch.GetBuffer(MAX_PATH));
	int nTripNum = g_Jugde[0].m_tStandardSize.hvTripNum;
	g_pCommunicate->DoSomething("SETPARAM", "TRIPNUM", &nTripNum);
	g_strBatch.ReleaseBuffer();
	g_pCommunicate->DoSomething("INITIALIZE", &tInitCom, NULL);
	g_pCommunicate->DoSomething("GETPARAM", "GRABBERDIVIDENUM", &g_nGrabberLineDivideNum);//读取采集分聘数目
	g_pCommunicate->DoSomething("SETPARAM", "SAVEPATH", g_strSaveFullPath.GetBuffer(MAX_PATH));
	g_strSaveFullPath.ReleaseBuffer();


	g_pCLabeller = LoadDHInterFaceDll("SCILabeller");

	////PLC初始化
	LABELLERINIT tInitLaber;
	tInitLaber.pLogView = g_pLog;
	tInitLaber.pSysCall = SystemCall;
	g_pCLabeller->DoSomething("SETPARAM", "BATCH", g_strBatch.GetBuffer(MAX_PATH));
	tInitLaber.strProductName.Format("%s", g_strProductName);
	g_pCLabeller->DoSomething("INITIALIZE", &tInitLaber, NULL);



	for (int i = 0; i < g_nGrabberCount; i++)
	{
		DETECTINIT InitParam;//初始化算法结构体
		InitParam.strProductName.Format("%s", g_strProductName);
		InitParam.nGrabberSN = i;
		InitParam.pLogView = g_pLog;
		InitParam.nProductType = g_ProductParam.emProductType;//正极 负极
		InitParam.nCoatType = g_ProductParam.emCoatType;

		DHInterface* t = LoadDHInterFaceDll("SCIDetect");// 调用导出函数获得抽象类指针
		if (TRUE != t->DoSomething("INITIALIZE", &InitParam, NULL))//INITIALIZE
		{
			CString strException;
			strException.Format("相机%d算法无定位", i);
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)(strException), NULL);
		}
		g_vec_Algorithm.push_back(t);

		NS_SCIFindMark::INITPARAM tInitParam;
		tInitParam.strProductName.Format("%s", g_strProductName);
		tInitParam.nCameraSN = i;
		tInitParam.nProductType = g_ProductParam.emProductType;
		tInitParam.nCoatType = g_ProductParam.emCoatType;
		t = LoadDHInterFaceDll("SCIFindMark");// 调用导出函数获得抽象类指针
		if (TRUE != t->DoSomething("INITIALIZE", &tInitParam, NULL))//INITIALIZE
		{
			CString strException;
			strException.Format("相机%d算法无定位", i);
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)(strException), NULL);
		}
		g_vec_FindMark.push_back(t);
	}

	int nLocationY = 0;

	g_vec_FindMark[g_tSystemSet.nCameraModel]->DoSomething("GETPARAM", "LocatePositionY", &nLocationY);
	g_Jugde[0].m_tStandardSize.hvQiTouXianChu = nLocationY;


	for (int i = 0; i < g_nGrabberCount; i++)
	{
		g_vec_Algorithm[i]->DoSomething("SETPARAM", "tSizeParam", &g_Jugde[0].m_tStandardSize);
		g_CInJetProcess[i].Init(i);
		g_CSpotSizeCheck[i].m_nSide = i;
		g_CSpotSizeCheck[i].ReadParam();
		g_CAutoGRR[i].m_nSide = i;
		g_CAutoGRR[i].ReadParam();
	}




	///加载光源控制模块
	g_pLightControl = LoadDHInterFaceDll("LightControl");
	LIGHTINIT tInitLC;
	tInitLC.Log = g_pLog;
	g_pLightControl->DoSomething("INITIALIZE", &tInitLC, NULL);
	g_pLightControl->DoSomething("OPENLIGHT", "ON", NULL);



	double dEALength;
	dEALength = g_Jugde[0].m_tStandardSize.dTotalLength.D();
	g_pCommunicate->DoSomething("SETPARAM", "SandardEALength0", &dEALength);
	dEALength = g_Jugde[1].m_tStandardSize.dTotalLength.D();
	g_pCommunicate->DoSomething("SETPARAM", "SandardEALength1", &dEALength);


	STANDARDSURFACE tStandSurface;
	tStandSurface = g_Jugde[0].m_tStandardSurface;
	g_pStorage->DoSomething("SETPARAM", "SurfaceParam", &tStandSurface);
	STANDARDSIZE tStandSize;
	tStandSize = g_Jugde[0].m_tStandardSize;
	g_pStorage->DoSomething("SETPARAM", "SizeParam", &tStandSize);

	g_CPowerLimit.ReadParam();
	TupleGenConst(2, 0, &g_hvPopWindowNum);



	for (int i = 0; i < g_Count.nType; i++)
	{
		CString strKey, strValue;
		strKey.Format("ERROR%dNAME", i);
		GetPrivateProfileString("ERRORINFO", strKey, "漏箔", strValue.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigCommonFile);
		g_Count.strAClassName.Add(strValue);
	}
	for (int k = 0; k < g_nGrabberCount; k++)
	{
		for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
		{
			s_SingleCount sc;
			CString strKey, strSection;
			strSection.Format("COUNT%d", k);
			for (int j = 0; j < g_Count.nType; j++)
			{
				strKey.Format("ERRC%d", i*g_Count.nType + j);
				sc.nClassError.push_back(GetPrivateProfileInt(strSection, strKey, 0, g_strConfigSystemFile));
			}
			strKey.Format("ERR%d", i);
			sc.nError = GetPrivateProfileInt(strSection, strKey, 0, g_strConfigSystemFile);
			strKey.Format("TOTAL%d", i);
			sc.nTotal = GetPrivateProfileInt(strSection, strKey, 0, g_strConfigSystemFile);
			g_Count.veCount[k].push_back(sc);
		}

	}




}

void CSeekersDlg::InitSubDlg()
{

	m_pSubSizeDlg = new CSubDlgSize;
	m_pSubSizeDlg->Create(CSubDlgSize::IDD, this);
	CRect rcMainPicture;
	GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(rcMainPicture);
	m_pSubSizeDlg->MoveWindow(rcMainPicture);
	m_pSubSizeDlg->ShowWindow(SW_HIDE);
	m_pSubSizeDlg->ResetSize(0);
	m_nCurrentDisplayView = VIEW_PICTURE;
	GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(rcMainPicture);




	m_pFinalOutResult = new CDlgFinalOutResult;
	m_pFinalOutResult->Create(CDlgFinalOutResult::IDD, this);
	GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(rcMainPicture);
	m_pFinalOutResult->MoveWindow(rcMainPicture);
	m_pFinalOutResult->ShowWindow(SW_HIDE);
	m_pFinalOutResult->ResetSize(0);


	m_pDlgScatterPlot = new CDlgScatterPlot;
	m_pDlgScatterPlot->Create(CDlgScatterPlot::IDD, this);//非模态对话框
	GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(rcMainPicture);
	m_pDlgScatterPlot->MoveWindow(rcMainPicture);
	m_pDlgScatterPlot->ShowWindow(SW_HIDE);
	m_pDlgScatterPlot->ResetSize();
}
void CSeekersDlg::InitStatusBar()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	RECT RectStatusBar;
	GetClientRect(&RectStatusBar); //获取对话框的矩形区域
	RectStatusBar.top = RectStatusBar.bottom - 25; //设置状态栏的矩形区域

	m_wndStatusBar.Create(this);
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
	m_wndStatusBar.MoveWindow(&RectStatusBar);
	m_wndStatusBar.SetBackgroundColor(MainColor);
	m_wndStatusBar.SetTextColor(RGB(255, 255, 255));

	m_wndStatusBar.SetPaneInfo(0, IDS_STRING_READY, SBPS_NORMAL, 80);//返回值存nID中
	m_wndStatusBar.SetPaneInfo(1, IDS_STRING_GRAB1, SBPS_STRETCH, 200);
	m_wndStatusBar.SetPaneInfo(2, IDS_STRING_GRAB2, SBPS_STRETCH, 200);
	m_wndStatusBar.SetPaneInfo(3, IDS_STRING_DETECT1, SBPS_STRETCH, 1);//返回值存nID中
	m_wndStatusBar.SetPaneInfo(4, IDS_STRING_DETECT2, SBPS_STRETCH, 1);//返回值存nID中
	m_wndStatusBar.SetPaneInfo(5, IDS_STRING_QUEUEINFO, SBPS_STRETCH, 1150);
	m_wndStatusBar.SetPaneInfo(6, IDS_STRING_USERNAME, SBPS_STRETCH, 150);
	m_wndStatusBar.SetPaneInfo(7, IDS_STRING_TIME, SBPS_NORMAL, 100);

	m_wndStatusBar.SetPaneText(0, " UNREADY");
	m_wndStatusBar.SetPaneText(6, "用户：无");
	CString strTime;
	CTime time = CTime::GetCurrentTime();
	strTime.Format("%02d:%02d", time.GetHour(), time.GetMinute());
	m_wndStatusBar.SetPaneText(7, strTime);
}


void CSeekersDlg::ShowButton(int nType)
{

}


void CSeekersDlg::GenDispImage(HObject *ho_Image)
{

	// Local iconic variables
	HObject  ho_Rectangle, ho_Rectangle1, ho_Rectangle2;

	GenImageConst(&(*ho_Image), "byte", 440, 512);
	GenRectangle1(&ho_Rectangle, 180, 0, 512, 512);
	GenRectangle1(&ho_Rectangle1, 0, 50, 150, 150);
	GenRectangle1(&ho_Rectangle2, 150, 50, 180, 150);
	OverpaintRegion((*ho_Image), ho_Rectangle, 90, "fill");
	OverpaintRegion((*ho_Image), ho_Rectangle1, 255, "fill");
	OverpaintRegion((*ho_Image), ho_Rectangle2, 120, "fill");
	return;
}

void CSeekersDlg::InitWindowShow()//初始化显示句柄
{
	CRect rcMainPicture;
	GetDlgItem(IDC_PICTURE_MAIN)->GetClientRect(rcMainPicture);//获得当前控件大小
	CRect rcHalfMainPicture(rcMainPicture);
	CRect rcImage(rcHalfMainPicture);
	CRect rcHistory;
	GetDlgItem(IDC_PICTURE_HISTORY)->GetClientRect(rcHistory);//获得当前控件大小
	CRect rcSub;
	GetDlgItem(IDC_PICTURE_SUB)->GetClientRect(rcSub);
	const int nSubWidth = rcSub.Width() / SUB_WINDOW_COUNT;
	SetCheck("~father");
	///////两个窗体//
	SetWindowAttr("background_color", "white");
	const int nSegWidth = 20;
	{
		rcHalfMainPicture.right = rcHalfMainPicture.right / 2;
		rcImage = rcHalfMainPicture;
		OpenWindow(rcImage.top, rcImage.left, rcImage.Width(), rcImage.Height(),
			(Hlong)GetDlgItem(IDC_PICTURE_MAIN)->m_hWnd, "visible", "", &m_hvec_MainWindowsHandle[0].T());
		rcImage.OffsetRect(rcHalfMainPicture.Width() + 4, 0);
		OpenWindow(rcImage.top, rcImage.left, rcImage.Width(), rcImage.Height(),
			(Hlong)GetDlgItem(IDC_PICTURE_MAIN)->m_hWnd, "visible", "", &m_hvec_MainWindowsHandle[1].T());

		OpenWindow(rcHistory.top, rcHistory.left, rcHistory.Width() / 2 - nSegWidth, rcHistory.Height(),
			(Hlong)GetDlgItem(IDC_PICTURE_HISTORY)->m_hWnd, "visible", "", &m_hvec_HistroyWindowsHandle[0].T());
		OpenWindow(rcHistory.top, rcHistory.left + rcHistory.Width() / 2 + nSegWidth, rcHistory.Width() / 2 - nSegWidth, rcHistory.Height(),
			(Hlong)GetDlgItem(IDC_PICTURE_HISTORY)->m_hWnd, "visible", "", &m_hvec_HistroyWindowsHandle[1].T());
	}

	SetWindowAttr("background_color", "black");
	for (int i = 0; i < SUB_WINDOW_COUNT; i++)
	{

		OpenWindow((rcSub.Height() - nSubWidth) / 2, rcSub.left + nSubWidth * i, nSubWidth - 5, nSubWidth - 5,
			(Hlong)GetDlgItem(IDC_PICTURE_SUB)->m_hWnd, "visible", "", &m_hvec_SubWindowsHandle[i].T());
		SetPart(m_hvec_SubWindowsHandle[i].T(), 0, 0, 200 - 1, 200 - 1);
		set_display_font(m_hvec_SubWindowsHandle[i].T(), 16, "mono", "true", "false");
		SetColor(m_hvec_SubWindowsHandle[i].T(), "green");
	}
	SetWindowAttr("background_color", "white");
	HTuple hvSub;
	OpenWindow(rcSub.top, rcSub.left, rcSub.Width(), rcSub.Height(),
		(Hlong)GetDlgItem(IDC_PICTURE_SUB)->m_hWnd, "visible", "", &hvSub);

	SetCheck("father");

	SetDraw(m_hvec_MainWindowsHandle[0].T(), "margin");
	SetDraw(m_hvec_MainWindowsHandle[1].T(), "margin");
	SetColor(m_hvec_MainWindowsHandle[0].T(), "red");
	SetColor(m_hvec_MainWindowsHandle[1].T(), "red");
	set_display_font(m_hvec_MainWindowsHandle[0].T(), 16, "mono", "true", "false");
	set_display_font(m_hvec_MainWindowsHandle[1].T(), 16, "mono", "true", "false");

	CString strBG;
	strBG.Format("%s\\res\\bg.png", GetCurrentAppPath());
	HObject hoBG;
	HTuple hvW, hvH, hvNum;
	ReadImage(&hoBG, (HTuple)strBG);
	GenImageConst(&hoBG, "byte", 400, 200);
	CountObj(hoBG, &hvNum);
	if (hvNum == 0)
	{
		return;
	}
	GetImageSize(hoBG, &hvW, &hvH);

	SetPart(m_hvec_MainWindowsHandle[0].T(), 0, 0, hvH, hvW);
	SetPart(m_hvec_MainWindowsHandle[1].T(), 0, 0, hvH, hvW);
	DispObj(hoBG, m_hvec_MainWindowsHandle[0].T());
	DispObj(hoBG, m_hvec_MainWindowsHandle[1].T());
}


void CSeekersDlg::InitStatic()
{
	COLORREF BK(RGB(255, 255, 255));
	COLORREF Text(RGB(0, 0, 0));
	m_staticTitle.SetTextColor(RGB(255, 255, 255));
	m_staticProductName.SetTextColor(Text);
	m_staticBatch.SetTextColor(Text);
	m_STC_CMeter.SetTextColor(Text);
	m_STC_CMeterName.SetTextColor(Text);





	m_staticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_staticProductName.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_staticBatch.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CMeter.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CMeterName.SetFont("Microsoft YaHei", 20, FW_NORMAL);




	m_cstaticProductName.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticBatch.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CMeter.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CMeterName.SetFont("Microsoft YaHei", 20, FW_NORMAL);

	m_staticProductName.SetBkColor(BK, BK, CStaticExt::Normal);
	m_staticBatch.SetBkColor(BK, BK, CStaticExt::Normal);

	m_cstaticProductName.SetBkColor(BK, BK, CStaticExt::Normal);
	m_cstaticBatch.SetBkColor(BK, BK, CStaticExt::Normal);

	m_STC_CMeterName.SetBkColor(BK, BK, CStaticExt::Normal);
	m_STC_CMeter.SetBkColor(BK, BK, CStaticExt::Normal);


	m_staticProductName.Format("%s", g_strProductName);
	m_staticBatch.Format("%s", g_strBatch);


}

void CSeekersDlg::SetButtonColors(CButtonExt & btn, short btnoffset, COLORREF bkin, COLORREF bkout, COLORREF bkfoucus, COLORREF btfin, COLORREF btfout, COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_ftButton);
}
void CSeekersDlg::InitButton()
{
	m_ftButton.CreatePointFont(110, "Microsoft YaHei");

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮

	SetButtonColors(m_btnExitAppication, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnStart, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnSwitchBatch, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnLogin, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnProductManager, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnSwitchViews, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnTools, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_BTN_DrawLeftModel, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_BTN_DailyUse, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);

	SetButtonColors(m_BTN_ClearLabeCount, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btnCalibResolution, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_BTN_CFindEdage, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);

	SetButtonColors(m_BTN_CLightOnOff, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);
	SetButtonColors(m_btn_ClearDisk, 0, BKIN, BKOUT, BKFOCUS, FG, FG, FG);



	m_btnExitAppication.SetIcon(IDI_ICON_W_LEAVE);
	m_btnExitAppication.SetTooltipText(_T("退出程序"));

	m_btnStart.SetIcon(IDI_ICON_W_PLAY);
	m_btnSwitchBatch.SetIcon(IDI_ICON_W_FILEADD);
	m_btnLogin.SetIcon(IDI_ICON_W_KEY);
	m_btnProductManager.SetIcon(IDI_ICON_W_LAYER);
	m_btnSwitchViews.SetIcon(IDI_ICON_W_REFRESH);
	m_btnTools.SetIcon(IDI_ICON_W_LINE);
	m_BTN_DrawLeftModel.SetIcon(IDI_ICON_DRAWMODEL);

	////前进后退
	m_btnSubBack.SetIcon(IDI_ICON_W_LEFT);
	m_btnSubBack.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, 1);
	m_btnSubBack.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnSubBack.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnSubBack.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);

	m_btnSubNext.SetIcon(IDI_ICON_W_RIGHT);//,(int)BTNST_AUTO_GRAY
	m_btnSubNext.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, 1);
	m_btnSubNext.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnSubNext.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnSubNext.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);


}

void CSeekersDlg::InitQueues()//初始化队列
{

	for (int i = 0; i < g_nGrabberCount; i++)
	{
		CreateMessageQueue(&g_hvec_QueueImages[i].T());//创建和相机个数相等的采集队列
		SetMessageQueueParam(g_hvec_QueueImages[i].T(), "max_message_num", 30);
		CreateMessageQueue(&g_hvec_QueueResults[i].T());//创建一个结果队列
		SetMessageQueueParam(g_hvec_QueueResults[i].T(), "max_message_num", 30);
		CreateMessageQueue(&g_hvec_QueuePintu[i].T());//创建一个结果队列
		SetMessageQueueParam(g_hvec_QueuePintu[i].T(), "max_message_num", 30);

	}

	CreateMessageQueue(&g_hvec_QueueDisplay[0].T());//创建一个显示队列
	SetMessageQueueParam(g_hvec_QueueDisplay[0].T(), "max_message_num", 40);
}

void CSeekersDlg::InitUI()
{
	InitStatic();
	InitButton();
	InitMenu();
}

void CSeekersDlg::InitLabelCountList()
{
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	m_LST_LabelCount.SetHeaderHeight(1.5);          //设置头部高度
	m_LST_LabelCount.SetHeaderFontHW(15, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_LST_LabelCount.SetHeaderTextColor(RGB(255, 255, 255)); //设置头部字体颜色
	m_LST_LabelCount.SetHeaderBKColor(70, 95, 120, 0); //设置头部背景色
	m_LST_LabelCount.SetBkColor(RGB(255, 255, 255));        //设置背景色
	m_LST_LabelCount.SetRowHeigt(23);               //设置行高度
	m_LST_LabelCount.SetFontHW(15, 0);               //设置字体高度，和宽度,0表示缺省宽度

	while (m_LST_LabelCount.GetHeaderCtrl()->GetItemCount())
	{
		m_LST_LabelCount.DeleteColumn(0);
	}
	m_LST_LabelCount.DeleteAllItems();
	CString strIndex;
	COLORREF  BkColor = RGB(100, 100, 100);
	COLORREF  NameBkColor = RGB(0, 180, 180);
	COLORREF  CountBkColor = RGB(0, 180, 180);
	CRect rcRect;
	m_LST_LabelCount.GetWindowRect(rcRect);
	int nWidth = rcRect.Width() / (3 * g_Jugde[0].m_tStandardSize.hvTripNum.I()) - 2;
	m_LST_LabelCount.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	for (int i = 0; i < g_Jugde[0].m_tStandardSize.hvTripNum; i++)
	{
		strIndex.Format("%d条缺陷类别", i + 1);
		m_LST_LabelCount.InsertColumn(i * 3, strIndex, LVCFMT_LEFT, nWidth);
		m_LST_LabelCount.InsertColumn(i * 3 + 1, _T("数目"), LVCFMT_LEFT, nWidth);
		m_LST_LabelCount.InsertColumn(i * 3 + 2, _T(" "), LVCFMT_LEFT, nWidth);
		//m_LST_LabelCount.SetColColor(i*3,NameBkColor); //设置列背景色
		//m_LST_LabelCount.SetColColor(i*3+1,CountBkColor); //设置列背景色
		//m_LST_LabelCount.SetColColor(i*3+2,BkColor); //设置列背景色
		if (i == 0)
		{
			for (int j = 0; j < g_Count.nType; j++)
			{
				m_LST_LabelCount.InsertItem(j + 1, g_Count.strAClassName.GetAt(j));//128
			}
			/*m_LST_LabelCount.InsertItem(g_Count.nType+1, _T("错误片数"));*/
			m_LST_LabelCount.InsertItem(g_Count.nType + 1, _T("总数"));
			m_LST_LabelCount.InsertItem(g_Count.nType + 2, _T("废品率%"));
		}
		else
		{
			for (int j = 0; j < g_Count.nType; j++)
			{
				m_LST_LabelCount.SetItemText(j, i * 3, g_Count.strAClassName.GetAt(j));//128
			}
			/*m_LST_LabelCount.SetItemText(g_Count.nType, i*3,_T("错误片数"));	*/
			m_LST_LabelCount.SetItemText(g_Count.nType, i * 3, _T("总数"));
			m_LST_LabelCount.SetItemText(g_Count.nType + 1, i * 3, _T("废品率%"));
		}
	}
}

void CSeekersDlg::InitLabelCaoWeiList()
{
	return;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	m_LST_LableCaoWei.SetHeaderHeight(1.5);          //设置头部高度
	m_LST_LableCaoWei.SetHeaderFontHW(15, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_LST_LableCaoWei.SetHeaderTextColor(RGB(255, 255, 255)); //设置头部字体颜色
	m_LST_LableCaoWei.SetHeaderBKColor(70, 95, 120, 0); //设置头部背景色
	m_LST_LableCaoWei.SetBkColor(RGB(255, 255, 255));        //设置背景色
	m_LST_LableCaoWei.SetRowHeigt(23);               //设置行高度
	m_LST_LableCaoWei.SetFontHW(15, 0);               //设置字体高度，和宽度,0表示缺省宽度


	int n = m_LST_LableCaoWei.GetHeaderCtrl()->GetItemCount();
	while (m_LST_LableCaoWei.GetHeaderCtrl()->GetItemCount())
	{
		m_LST_LableCaoWei.DeleteColumn(0);
	}
	m_LST_LableCaoWei.DeleteAllItems();
	CString strIndex;
	COLORREF  BkColor = RGB(100, 100, 100);
	COLORREF  NameBkColor = RGB(0, 180, 180);
	COLORREF  CountBkColor = RGB(0, 180, 180);
	CRect rcRect;
	m_LST_LableCaoWei.GetWindowRect(rcRect);
	int nWidth = rcRect.Width() / (g_Jugde[0].m_tStandardSize.hvTripNum.I() * 2 + 1);
	m_LST_LableCaoWei.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);


	strIndex.Format("-CPK-");
	m_LST_LableCaoWei.InsertColumn(0, strIndex, LVCFMT_CENTER, nWidth);
	for (int i = 0; i < g_Jugde[0].m_tStandardSize.hvTripNum * 2; i++)
	{
		if (i < g_Jugde[0].m_tStandardSize.hvTripNum)
		{
			strIndex.Format("涂布错位%d", i + 1);
			m_LST_LableCaoWei.InsertColumn(i, strIndex, LVCFMT_CENTER, nWidth + 8);
		}
		else
		{
			strIndex.Format("AT9错位%d", i + 1);
			m_LST_LableCaoWei.InsertColumn(i, strIndex, LVCFMT_CENTER, nWidth + 8);
		}

	}

	for (int i = 0; i < 20; i++)
	{
		strIndex.Format("第%d组", i + 1);
		m_LST_LableCaoWei.InsertItem(i, strIndex);
	}
	int nCount = m_LST_LableCaoWei.GetItemCount();
	strIndex.Format("CPK:");
	m_LST_LableCaoWei.InsertItem(nCount, strIndex);

	int  nColCount = m_LST_LableCaoWei.GetHeaderCtrl()->GetItemCount();

	for (int i = 0; i < m_LST_LableCaoWei.GetItemCount(); i++)
	{
		strIndex.Format("-");
		for (int j = 1; j < nColCount; j++)
		{
			m_LST_LableCaoWei.SetItemText(i, j, strIndex);
		}
	}
}

void CSeekersDlg::UpdateLabelCaoWeiList(C_ALLOCATORWORKSHOPOUTPUT& Output)
{
	try
	{
		return;
		HWND hwndList;
		hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_LABLECAOWEI);
		SNDMSG(hwndList, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

		CString strCount;
		m_hvec_PerAtCuoWei[m_hvec_PerAtCuoWei.Length()].T() = g_Jugde[0].m_hvPerATCuoWei;
		if (m_hvec_PerAtCuoWei.Length() >= m_LST_LableCaoWei.GetItemCount() - 1)
		{

			for (int i = 0; i < m_LST_LableCaoWei.GetItemCount() - 1; i++)
			{
				for (int j = 1; j < m_LST_LableCaoWei.GetHeaderCtrl()->GetItemCount(); j++)
				{
					strCount.Format("%0.4f", m_hvec_PerAtCuoWei[i].T()[j - 1].D());
					m_LST_LableCaoWei.SetItemText(i, j, strCount);
				}
			}
			m_hvec_PerAtCuoWei.Clear();
		}

		if (g_Jugde[0].m_hvCPK.Length() > 0)
		{
			//每20组写入一次CPK
			for (int j = 0; j < g_Jugde[0].m_hvCPK.Length(); j++)
			{
				strCount.Format("%0.4f", g_Jugde[0].m_hvCPK[j].D());
				m_LST_LableCaoWei.SetItemText(m_LST_LableCaoWei.GetItemCount() - 1, j + 1, strCount);
			}
			g_Jugde[0].m_hvCPK.Clear();
		}

		SNDMSG(hwndList, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
	}
	catch (HException & e)
	{
		CString strMessage;
		strMessage.Format("UpdateLabelCaoWeiList刷新错位值列表异常：%s函数:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
	}

}

void CSeekersDlg::InitThread()//初始化线程
{
	int nStartThread = 0;//已经启动了多少个线程
	HDevThread*         hcppthread_handle;
	HDevThreadContext   hcppthread_context;


	for (int i = 0; i < g_nGrabberCount; i++)//开启拼图线程和处理线程
	{
		hcppthread_handle = new HDevThread(hcppthread_context,
			(void*)HDevExportCpp::_hcppthread_pintu_thread, 1, 0);//开启一个具有3个输入的线程
		hcppthread_handle->SetInputCtrlParamTuple(0, i);//线程编号也就是相机序号
		hcppthread_handle->ParStart(&g_hvec_VProcThreads[nStartThread].T());
		nStartThread++;

		hcppthread_handle = new HDevThread(hcppthread_context,
			(void*)HDevExportCpp::_hcppthread_processing_thread, 1, 0);//开启一个具有3个输入的线程
		hcppthread_handle->SetInputCtrlParamTuple(0, i);//线程编号也就是相机序号
		hcppthread_handle->ParStart(&g_hvec_VProcThreads[nStartThread].T());
		nStartThread++;
	}

	for (int nGrabberSN = 0; nGrabberSN < 1; nGrabberSN++)//创建综合线程，综合线程的数目始终是和产品的条数是相等的
	{
		hcppthread_handle = new HDevThread(hcppthread_context,
			(void*)HDevExportCpp::_hcppthread_allocator_thread, 1, 0);//开启一个具有相机个数外加3个队列的输入
		hcppthread_handle->SetInputCtrlParamTuple(0, nGrabberSN);//
		hcppthread_handle->ParStart(&g_hvec_VProcThreads[nStartThread].T());
		nStartThread++;
	}


	//for (int nGrabberSN=0;nGrabberSN<1;nGrabberSN++)//创建监听线程 监听OUT口的变化
	//{
	//	hcppthread_handle = new HDevThread(hcppthread_context,
	//		(void*)HDevExportCpp::_hcppthread_Listen_thread,1,0);//开启一个具有相机个数外加3个队列的输入
	//	hcppthread_handle->SetInputCtrlParamTuple(0,nGrabberSN);//
	//	hcppthread_handle->ParStart(&g_hvec_VProcThreads[nStartThread].T());
	//	nStartThread++;
	//}
}

void CSeekersDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strBtnText;
	GetDlgItemTextA(IDC_BUTTON_START, strBtnText);
	if ("启动" == strBtnText)
	{
		m_bRunning = TRUE;
		strBtnText.Format("停止");
		SetDlgItemTextA(IDC_BUTTON_START, strBtnText);
		m_wndStatusBar.SetPaneText(0, "  READY");
		m_btnStart.SetIcon(IDI_ICON_W_PAUSE);
		g_pCommunicate->DoSomething("READY", "ON", NULL);
		if (0 == g_nGrabberType)
		{
			for (int i = g_nGrabberCount - 1; i >= 0; i--)
			{
				g_vec_Grabber[i]->DoSomething("STARTGRAB", NULL, NULL);
			}
		}
		{

			CString strValue;
			strValue = "程序开始检测";
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
		}


	}
	else if ("停止" == strBtnText)
	{
		if (g_bChangeProduct == FALSE)
		{
			if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
			{
				return;
			}
		}

		m_bRunning = FALSE;
		strBtnText.Format("启动");
		SetDlgItemTextA(IDC_BUTTON_START, strBtnText);
		m_wndStatusBar.SetPaneText(0, " UNREADY");
		m_btnStart.SetIcon(IDI_ICON_W_PLAY);
		g_pCommunicate->DoSomething("READY", "OFF", NULL);

		if (0 == g_nGrabberType)
		{
			for (int i = 0; i < g_nGrabberCount; i++)
			{
				g_vec_Grabber[i]->DoSomething("STOPGRAB", NULL, NULL);
			}
		}
		{

			CString strValue;
			strValue = "程序停止检测";
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
		}



	}
}

LRESULT CSeekersDlg::OnRestartProcess(WPARAM wParam, LPARAM lParam)
{
	ExitDo();
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset((void*)&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	CString strApp;
	strApp.Format("%s\\Seekers.exe", GetCurrentAppPath());
	CreateProcess(NULL, strApp.GetBuffer(0), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	::TerminateProcess(::GetCurrentProcess(), NULL);//终止原先程序
	return 0l;
}

LRESULT CSeekersDlg::OnDetect(WPARAM wParam, LPARAM lParam)
{
	static int nCount[2] = { 0 };
	int nGrabberSN = (int)wParam;
	CString strDisp;
	strDisp.Format("G%d-%d", nGrabberSN, ++nCount[nGrabberSN]);
	m_wndStatusBar.SetPaneText(nGrabberSN + 3, strDisp);
	return 0L;
}

LRESULT CSeekersDlg::OnGrabber(WPARAM wParam, LPARAM lParam)
{
	int nGrabberSN = LOWORD(wParam);
	int nStatus = HIWORD(wParam);
	int nFrameSN = LOWORD(lParam);
	int nImageSN = HIWORD(lParam);
	CString strDisp;
	strDisp.Format("相机%d-%d-%d-图像号:%d", nGrabberSN, nStatus, nFrameSN, nImageSN);
	m_wndStatusBar.SetPaneText(nGrabberSN + 1, strDisp);
	if (nStatus != 0 && nStatus != 19)//如果采集状态不为0,记录异常日志
	{
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)(strDisp), NULL);
	}
	return 0L;
}
void CSeekersDlg::Sub(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	BOOL bUpdate = FALSE;
	HTuple hv_GrabberSN = 0;
	int nInsertCount;
	if (1 == g_nGrabberCount)
	{
		nInsertCount = 1;
	}
	else
	{
		nInsertCount = 2;
	}
	for (int i = 0; i < nInsertCount; i++)
	{
		B_DETECTWORKSHOPOUTPUT* pDetectOutput = &tOutput.B[i];
		HTuple hvErrorStrip;
		TupleFind(pDetectOutput->tDetectOutput.hv_Flag, FALSE, &hvErrorStrip);//查看有没有错误的
		if (-1 != hvErrorStrip)//如果存在错误Region
		{
			HObject ConnectedRegions, RegionUnion, RegionClosing;
			Union1(pDetectOutput->tDetectOutput.ho_ErrorRegion, &RegionUnion);
			ClosingRectangle1(RegionUnion, &RegionClosing, CROP_SIZE, CROP_SIZE);
			Connection(RegionClosing, &ConnectedRegions);
			HTuple hv_Min;
			/*	TupleMin2(ConnectedRegions.CountObj(), 2, &hv_Min);*/
			TupleMin2(ConnectedRegions.CountObj(), 1, &hv_Min);
			for (int j = 0; j < hv_Min; j++)
			{
				HRegion ObjectSelected;
				SelectObj(ConnectedRegions, &ObjectSelected, j + 1);
				if (ObjectSelected.Area() > 0)//如果面积大于0
				{
					HTuple CenterRow, CenterColumn, Row1, Column1;
					HObject ImagePart;
					AreaCenter(ObjectSelected, NULL, &CenterRow, &CenterColumn);
					TupleMax2(CenterRow - CROP_SIZE / 2, 0, &Row1);
					TupleMax2(CenterColumn - CROP_SIZE / 2, 0, &Column1);
					SUBELEMENT tSubElement;
					tSubElement.nGrabberSN = pDetectOutput->A.hvGrabberSN;
					CopyImage(pDetectOutput->A.hoGrabImage, &tSubElement.hoImage);
					CropRectangle1(pDetectOutput->A.hoGrabImage, &tSubElement.hoSubImage, Row1, Column1, Row1 + CROP_SIZE, Column1 + CROP_SIZE);
					m_lstSubImage.push_back(tSubElement);
					m_lstSubImage.pop_front();
					bUpdate = TRUE;
				}
			}
		}
	}
	if (TRUE == bUpdate)
	{
		SetSystem("flush_graphic", "false");
		for (int i = 0; i < SUB_WINDOW_COUNT; i++)
		{
			ClearWindow(m_hvec_SubWindowsHandle[i].T());
		}
		SetSystem("flush_graphic", "true");

		m_nSubPage = (int)m_lstSubImage.size() / SUB_WINDOW_COUNT;
		for (int i = 0; i < SUB_WINDOW_COUNT; i++)
		{
			size_t n = m_lstSubImage.size();
			std::list<SUBELEMENT>::iterator iter = m_lstSubImage.begin();
			advance(iter, i + SUB_WINDOW_COUNT*(m_nSubPage - 1));
			HObject hoSubImage = iter->hoSubImage;
			DispObj(hoSubImage, m_hvec_SubWindowsHandle[i].T());
			SetTposition(m_hvec_SubWindowsHandle[i].T(), 12, 12);
			WriteString(m_hvec_SubWindowsHandle[i].T(), HTuple(iter->nGrabberSN));
		}
	}

}

void CSeekersDlg::ExitDo()//程序退出时候需要做的事情
{


	OnBnClickedButtonStart();

	g_pStorage->DoSomething("ExitDo", NULL, NULL);
	g_bExitThread = TRUE;
	g_pLightControl->DoSomething("CLOSE", NULL, NULL);
	g_pCommunicate->DoSomething("SETPARAM", "EXIT", NULL);
	g_pCLabeller->DoSomething("SETPARAM", "ExitThread", NULL);
	SaveCurrentStatus();
	Sleep(500);
	KillTimer(UM_TIMER_QUEUEINFO);
	KillTimer(UM_TIMER_TIME);
	DeleteCriticalSection(&g_CsClearLabelCount);
	DeleteCriticalSection(&g_CsPopWindowChange);

	Release();
	g_pCommunicate->DoSomething("READY", "OFF", NULL);
	g_pCLabeller->DoSomething("SETPARAM", "RED_LIGHT", NULL);
	if (g_hook != NULL)
	{
		UnhookWindowsHookEx(g_hook);
		g_hook = NULL;
	}
	if (g_hookMenu != NULL)
	{
		UnhookWindowsHookEx(g_hookMenu);
		g_hookMenu = NULL;
	}
}

void CSeekersDlg::History(C_ALLOCATORWORKSHOPOUTPUT& Output)
{

	const int HISTORT_COUNT = g_tSystemSet.nHistoryCount;
	const int HISTORY_TYPE = g_tSystemSet.nHistoryType;//0是不保留 1是只保留错误 2是保留全部
	const int REC_WIDTH = 80;

	for (int i = 0; i < g_nGrabberCount; i++)
	{
		//去除非本次循环
		B_DETECTWORKSHOPOUTPUT* pDetectOutput;
		pDetectOutput = &Output.B[i];

		HTuple hv_GrabberSN = pDetectOutput->A.hvGrabberSN;
		SetPart(m_hvec_HistroyWindowsHandle[hv_GrabberSN].T(), 0, 0, 100, HISTORT_COUNT * 100);
		HObject Rectangle;
		if (HISTORY_TYPE != 0)
		{
			HTuple hvError;
			TupleFind(pDetectOutput->tDetectOutput.hv_Flag, FALSE, &hvError);//-1
			if (-1 != Output.B[0].tDetectOutput.hv_JuanRaoMarkRow)//如果有Mark点
			{
				if (hvError != -1)//如果尺寸或者表面出错高度变为90 ok高度变为89
				{
					GenRectangle1(&Rectangle, 0, (HISTORT_COUNT + 1) * 100 - REC_WIDTH, 90, (HISTORT_COUNT + 1) * 100);
				}
				else
				{
					GenRectangle1(&Rectangle, 1, (HISTORT_COUNT + 1) * 100 - REC_WIDTH, 90, (HISTORT_COUNT + 1) * 100);
				}
			}
			else
			{
				if (hvError != -1)//如果没有mark点的极片尺寸或者表面出现问题，则高度变为70 ok的高度为69
				{
					GenRectangle1(&Rectangle, 20, (HISTORT_COUNT + 1) * 100 - REC_WIDTH, 90, (HISTORT_COUNT + 1) * 100);
				}
				else
				{
					GenRectangle1(&Rectangle, 21, (HISTORT_COUNT + 1) * 100 - REC_WIDTH, 90, (HISTORT_COUNT + 1) * 100);
				}
			}


			if ((HISTORY_TYPE == 1 && hvError != -1) || HISTORY_TYPE == 2)//全部保留错误，或者是部分保留
			{
				ConcatObj(m_hovec_Histroy[hv_GrabberSN].O(), Rectangle, &m_hovec_Histroy[hv_GrabberSN].O());//加入Region
				ConcatObj(m_hovec_Histroy_Image[hv_GrabberSN].O(), pDetectOutput->A.hoGrabImage, &m_hovec_Histroy_Image[hv_GrabberSN].O());//加入Image
				if (m_hovec_Histroy[hv_GrabberSN].O().CountObj() == HISTORT_COUNT + 1)//如果里面已经满了
				{
					HObject TempRegion; TempRegion.GenEmptyObj();
					for (int i = 2; i <= m_hovec_Histroy[hv_GrabberSN].O().CountObj(); i++)//2
					{
						ConcatObj(TempRegion, m_hovec_Histroy[hv_GrabberSN].O().SelectObj(i), &TempRegion);
					}
					m_hovec_Histroy[hv_GrabberSN].O().GenEmptyObj();
					ConcatObj(m_hovec_Histroy[hv_GrabberSN].O(), TempRegion, &m_hovec_Histroy[hv_GrabberSN].O());

					//image
					HObject TempImage; TempImage.GenEmptyObj();
					for (int i = 2; i <= m_hovec_Histroy_Image[hv_GrabberSN].O().CountObj(); i++)//2
					{
						ConcatObj(TempImage, m_hovec_Histroy_Image[hv_GrabberSN].O().SelectObj(i), &TempImage);
					}
					m_hovec_Histroy_Image[hv_GrabberSN].O().GenEmptyObj();
					ConcatObj(m_hovec_Histroy_Image[hv_GrabberSN].O(), TempImage, &m_hovec_Histroy_Image[hv_GrabberSN].O());

				}
				else if (m_hovec_Histroy[hv_GrabberSN].O().CountObj() > HISTORT_COUNT + 1)//如果里面已经超了
				{
					m_hovec_Histroy[hv_GrabberSN].O().GenEmptyObj();
					m_hovec_Histroy_Image[hv_GrabberSN].O().GenEmptyObj();
				}
				SetSystem("flush_graphic", "false");
				ClearWindow(m_hvec_HistroyWindowsHandle[hv_GrabberSN].T());

				MoveRegion(m_hovec_Histroy[hv_GrabberSN].O(), &m_hovec_Histroy[hv_GrabberSN].O(), 0, -100);//修改2016.5.18

				for (int i = 0; i < m_hovec_Histroy[hv_GrabberSN].O().CountObj(); i++)
				{

					HTuple Row1, Row2, Column1, Column2;
					SmallestRectangle1(m_hovec_Histroy[hv_GrabberSN].O().SelectObj(i + 1), &Row1, &Column1, &Row2, &Column2);

					if (Row2 - Row1 == 69 || Row2 - Row1 == 89)//OK
					{

						SetColor(m_hvec_HistroyWindowsHandle[hv_GrabberSN].T(), "green");

					}
					else if (Row2 - Row1 == 70 || Row2 - Row1 == 90)//表面错误
					{
						SetColor(m_hvec_HistroyWindowsHandle[hv_GrabberSN].T(), "red");
					}
					DispObj(m_hovec_Histroy[hv_GrabberSN].O().SelectObj(i + 1), m_hvec_HistroyWindowsHandle[hv_GrabberSN].T());
					if (g_tSystemSet.nRejectPoint == i + 1 && 0 == hv_GrabberSN)
					{
						HObject CirCle;
						GenCircle(&CirCle, 95, Column1 + (Column2 - Column1) / 2, 5);
						DispObj(CirCle, m_hvec_HistroyWindowsHandle[hv_GrabberSN].T());
					}
				}
				SetSystem("flush_graphic", "true");
				DispPolygon(m_hvec_HistroyWindowsHandle[hv_GrabberSN].T(), -1, -1);//防止闪烁
			}
		}
	}
}









void CSeekersDlg::Size(C_ALLOCATORWORKSHOPOUTPUT& Output)//尺寸显示
{

	m_pSubSizeDlg->Insert(Output);
	//m_pDlgScatterPlot->InsertData(Output);
	if (Output.bIsFinalResult == TRUE)//必须是连续的单双侧产品才能用到此功能
	{
		m_pFinalOutResult->Insert(Output);

	}
}




void CSeekersDlg::IntervalTimeSave(int nIntervalMinites)
{

	static HTuple hvLastSaveTime = 0;
	HTuple hvRealTime;
	CountSeconds(&hvRealTime);
	if ((hvRealTime - hvLastSaveTime) > nIntervalMinites * 60)
	{
		SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
		hvLastSaveTime = hvRealTime;
		for (int k = 0; k < g_nGrabberCount; k++)
		{
			CString strSection;
			strSection.Format("COUNT%d", k);
			for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
			{
				s_SingleCount sc = g_Count.veCount[k].at(i);
				CString strKey;
				for (int j = 0; j < g_Count.nType; j++)
				{
					strKey.Format("ERRC%d", i*g_Count.nType + j);
					WritePrivateProfileInt(strSection, strKey, sc.nClassError[j], g_strConfigSystemFile);
				}
				strKey.Format("ERR%d", i);
				WritePrivateProfileInt(strSection, strKey, sc.nError, g_strConfigSystemFile);
				strKey.Format("TOTAL%d", i);
				WritePrivateProfileInt(strSection, strKey, sc.nTotal, g_strConfigSystemFile);
			}
		}


	}
}


void CSeekersDlg::UpdateLabelCount()
{
	HWND hwndList;
	hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_COUNT);
	SNDMSG(hwndList, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

	for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
	{
		CString strIndex;
		int nCount = 0;
		CString strCount;
		for (int j = 0; j < g_Count.nType; j++)
		{
			strCount.Format("%d", g_Count.veCount[0][i].nClassError[j]);
			m_LST_LabelCount.SetItemText(j, 3 * i + 1, strCount);
			nCount += g_Count.veCount[0][i].nClassError[j];
		}
		g_Count.veCount[0].at(i).nError = nCount;
		/*strCount.Format("%d",nCount);
		m_LST_LabelCount.SetItemText(g_Count.nType,3*i+1,strCount);*/
		strCount.Format("%d", g_Count.veCount[0][0].nTotal);
		m_LST_LabelCount.SetItemText(g_Count.nType, 3 * i + 1, strCount);
		strCount.Format("%.2f", nCount*1.0 / MAX(1, g_Count.veCount[0][0].nTotal) * 100);
		m_LST_LabelCount.SetItemText(g_Count.nType + 1, 3 * i + 1, strCount);
	}
	SNDMSG(hwndList, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
}




void CSeekersDlg::Count(C_ALLOCATORWORKSHOPOUTPUT& Output)
{
	try
	{
		HTuple hv_SortIndex, hv_BelongTrips, hv_ErrorIndex;
		if (TRUE == Output.bIsFinalResult)
		{
			g_Count.veCount[0].at(0).nTotal++;
			int nSide = Output.hvPackageType.I();
			if (Output.tFinalResult.hv_SurfaceFlag == FALSE)
			{
				TupleSortIndex(Output.tFinalResult.hv_SurfaceErrorIndex, &hv_SortIndex);
				TupleSelect(Output.tFinalResult.hv_SurfaceErrorIndex, hv_SortIndex, &hv_ErrorIndex);
				TupleSelect(Output.tFinalResult.hv_SurfaceErrorBelongTrips, hv_SortIndex, &hv_BelongTrips);
				HTuple hv_TempIndex, hv_TempErrorIndex;
				for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
				{
					TupleFind(hv_BelongTrips, i, &hv_TempIndex);
					if (hv_TempIndex.Length() > 0 && hv_TempIndex != -1)
					{

						TupleSelect(hv_ErrorIndex, hv_TempIndex, &hv_TempErrorIndex);
						g_Count.veCount[0].at(i).nClassError[hv_TempErrorIndex[0].I()]++;
					}
				}
			}
			else if (Output.tFinalResult.hv_SizeFlag == FALSE)
			{

				TupleSortIndex(Output.tFinalResult.hv_SizeErrorIndex, &hv_SortIndex);
				TupleSelect(Output.tFinalResult.hv_SizeErrorIndex, hv_SortIndex, &hv_ErrorIndex);
				TupleSelect(Output.tFinalResult.hv_SizeErrorBelongTrips, hv_SortIndex, &hv_BelongTrips);
				HTuple hv_TempIndex, hv_TempErrorIndex;
				for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
				{
					TupleFind(hv_BelongTrips, i, &hv_TempIndex);
					if (hv_TempIndex.Length() > 0 && hv_TempIndex != -1)
					{
						TupleSelect(hv_ErrorIndex, hv_TempIndex, &hv_TempErrorIndex);
						g_Count.veCount[0].at(i).nClassError[hv_TempErrorIndex[0].I()]++;
					}
				}
			}

			UpdateLabelCount();
			if (g_ProductParam.emCoatType == M_TwoCam_Continue)
			{
				UpdateLabelCaoWeiList(Output);
			}
		}
	}
	catch (HException & e)
	{
		CString strMessage;
		strMessage.Format("Count计数异常：%s函数:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
	}



}

void CSeekersDlg::DispImage(C_ALLOCATORWORKSHOPOUTPUT& tOutput)//历史记录
{

	if (VIEW_PICTURE != m_nCurrentDisplayView)
	{
		return;
	}
	HTuple Width, Height;
	HTuple hv_TimeStart;
	CountSeconds(&hv_TimeStart);//开始记时
	for (int i = 0; i < g_nGrabberCount; i++)
	{
		GetImageSize(tOutput.B[i].A.hoGrabImage, &Width, &Height);
		SetPart(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), 0, 0, Height - 1, Width - 1);
		DispObj(tOutput.B[i].A.hoGrabImage, m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());

		SetColor(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), "blue");
		DispObj(tOutput.B[i].tDetectOutput.ho_JXRegion, m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());
		SetColor(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), "green");
		DispObj(tOutput.B[i].tDetectOutput.ho_Coat, m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());
		DispObj(tOutput.B[i].tDetectOutput.ho_Foil, m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());

		CString strMessage;
		CString strName;
		strName = GetGongWeiName(g_CUpLoad.m_StrGongWeiName, tOutput.B[i].A.hvGrabberSN.I());
		HObject hoCutRegionLine;
		HTuple hvMean;
		Intensity(tOutput.B[i].tDetectOutput.ho_Coat, tOutput.B[i].A.hoGrabImage, &hvMean, NULL);
		if (TRUE == tOutput.bIsFinalResult)
		{

			GenRegionLine(&hoCutRegionLine, MIN(Height.I() - 1, g_Jugde[0].m_tStandardSize.hvCutPosition / g_vec_RowResolution[0]), 0, MIN(Height.I() - 1, g_Jugde[0].m_tStandardSize.hvCutPosition / g_vec_RowResolution[0]), Width - 1);
			CString strSize, strSurface;
			if (TRUE == tOutput.tFinalResult.hv_SurfaceFlag)
			{
				strSurface = "OK";
			}
			else
			{
				strSurface = "NG";
			}
			if (TRUE == tOutput.tFinalResult.hv_SizeFlag)
			{
				strSize = "OK";
			}
			else
			{
				strSize = "NG";
			}


			strMessage.Format("%s—耗时: %.1f :%.1f :%.1f 灰度均值:%.1f 当前组产品表面:%s,尺寸:%s", strName, tOutput.B[i].A.hvTimeStamp.D(), tOutput.B[i].hvTimeStamp.TupleSum().D()
				, tOutput.hvTimeStamp.D(), hvMean.D(), strSurface, strSize);
		}
		else
		{
			hoCutRegionLine.GenEmptyObj();
			strMessage.Format("%s—耗时: %.1f :%.1f :%.1f 灰度均值:%.1f ", strName, tOutput.B[i].A.hvTimeStamp.D(), tOutput.B[i].hvTimeStamp.TupleSum().D()
				, tOutput.hvTimeStamp.D(), hvMean.D());
		}

		SetTposition(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), 25, 25);
		WriteString(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), HTuple(strMessage));
		SetColor(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), "yellow");
		DispObj(hoCutRegionLine, m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());
		SetColor(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), "red");
		DispObj(HRegion(tOutput.B[i].tDetectOutput.ho_ErrorRegion).Union1(), m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());

		//try
		//{
		//	if (g_Jugde[0].m_tStandardSize.hv_CoatType == M_TwoCam_Intermission)
		//	{
		//		if (i == 0 && (g_Jugde[0].m_tStandardSize.hv_CaoWeiType != 0 && g_Jugde[0].m_tStandardSize.hv_CaoWeiType != 2) || i == 1 && (g_Jugde[0].m_tStandardSize.hv_CaoWeiType != 0 && g_Jugde[0].m_tStandardSize.hv_CaoWeiType != 1))
		//		{

		//			set_display_font(m_hvec_MainWindowsHandle[i].T(), 12, "mono", "true", "false");


		//			for (int k = 0; k<tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)
		//			{

		//				DispObj(tOutput.B[i].tDetectOutput.ho_ExtractCaoWeiRegions.SelectObj(k + 1), m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T());
		//				HTuple hv_Row, hv_Column, hv_WidthValue, hv_HeightValue,hv_CaoweiMD,hv_CaoweiTD, hv_CaoweiTDCuoWei, hv_CaoweiMDCuoWei;
		//				hv_Row = tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple[k];
		//				hv_Column =tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple[k];
		//				hv_WidthValue = tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple[k];
		//				hv_HeightValue = tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple[k];
		//				hv_CaoweiMD = MIN(tOutput.B[0].tDetectOutput.hv_BotEdagePixel, tOutput.B[1].tDetectOutput.hv_BotEdagePixel)-tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple[k];
		//				hv_CaoweiTD = tOutput.B[i].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple[k].D();

		//				hv_CaoweiTDCuoWei = (tOutput.B[0].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*g_vec_ColumnResolution[0]
		//					- (tOutput.B[1].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[1].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*g_vec_ColumnResolution[1];

		//				hv_CaoweiMDCuoWei = tOutput.B[0].tDetectOutput.hv_CaoWeiYpositionTuple[k].D()*g_vec_RowResolution[0] - tOutput.B[1].tDetectOutput.hv_CaoWeiYpositionTuple[k].D()*g_vec_RowResolution[1];
		//				CString strValue;
		//				if (i==0)
		//				{
		//					strValue.Format("宽度%.2f\r\n高度%.2f\r\nMD:%.2f\r\nTD:%.2f\r\nTD错位:%0.2f\r\nMD错位:%0.2f", hv_WidthValue.D(), hv_HeightValue.D(), hv_CaoweiMD.D()*g_vec_RowResolution[i], hv_CaoweiTD.D()*g_vec_ColumnResolution[i], hv_CaoweiTDCuoWei.D(), hv_CaoweiMDCuoWei.D());
		//				}
		//				else
		//				{
		//					strValue.Format("宽度%.2f\r\n高度%.2f\r\nMD:%.2f\r\nTD:%.2f", hv_WidthValue.D(), hv_HeightValue.D(), hv_CaoweiMD.D()*g_vec_RowResolution[i], hv_CaoweiTD.D()*g_vec_ColumnResolution[i]);
		//				}
		//				

		//				HTuple hv_Index;
		//				TupleFind(tOutput.B[i].tDetectOutput.hv_ErrorBelongStrip, k, &hv_Index);
		//				if (hv_Index.Length()>0 && hv_Index != -1)
		//				{

		//					HTuple hv_Index1, hv_Index2, hv_Index3, hv_Index4;
		//					HTuple hv_ErrorIndex;
		//					TupleSelect(tOutput.B[i].tDetectOutput.hv_ErrorType, hv_Index, &hv_ErrorIndex);
		//					TupleFind(hv_ErrorIndex, CaoWeiHNG, &hv_Index1);
		//					TupleFind(hv_ErrorIndex, CaoWeiWNG, &hv_Index2);
		//					TupleFind(hv_ErrorIndex, CaoWeiMDNG, &hv_Index3);
		//					TupleFind(hv_ErrorIndex, CaoWeiTDNG, &hv_Index4);
		//					if (hv_Index1.Length()>0 && hv_Index1 != -1 || hv_Index2.Length()>0 && hv_Index2 != -1 || hv_Index3.Length()>0 && hv_Index3 != -1 || hv_Index4.Length()>0 && hv_Index4 != -1)
		//					{
		//						disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hv_Row - 200, hv_Column, "red", "false");
		//					}
		//					else
		//					{
		//						disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hv_Row - 200, hv_Column, "green", "false");
		//					}

		//				}
		//				else
		//				{
		//					disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hv_Row - 200, hv_Column, "green", "false");
		//				}


		//			}

		//			set_display_font(m_hvec_MainWindowsHandle[i].T(), 16, "mono", "true", "false");

		//		}


		//	}
		//	else
		//	{

		//		HObject hoFoil;
		//		hoFoil.GenEmptyObj();
		//		Connection(tOutput.B[i].tDetectOutput.ho_Foil, &hoFoil);
		//		double dOffset = 10;
		//		set_display_font(m_hvec_MainWindowsHandle[i].T(), 12, "mono", "true", "false");
		//		HTuple hvCuoweiTuple;
		//		TupleSub(tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage, tOutput.B[1].tDetectOutput.hv_AT9EdageToProEdage, &hvCuoweiTuple);
		//		for (int k = 0; k<hoFoil.CountObj(); k++)
		//		{
		//			HTuple hv_Row, hv_Column, hvArea;
		//			HTuple hvPositionX, hvPositionY;
		//			AreaCenter(hoFoil.SelectObj(k + 1), &hvArea, &hv_Row, &hv_Column);
		//			hvPositionY = hv_Row;
		//			CString strValue;











		//			if (k == 0)
		//			{
		//				hvPositionX = hv_Column + dOffset / g_vec_ColumnResolution[i];
		//				strValue.Format("条数%d \r\nAT9尺寸%.2f\r\nMD尺寸%.2f\r\nMD错位尺寸%.2f\r\n", k+1, tOutput.B[i].tDetectOutput.hv_CeramicWidth[k].D(), tOutput.B[i].tDetectOutput.hv_AT9EdageToProEdage[k].D(), hvCuoweiTuple[MIN(hvCuoweiTuple.Length() - 1, k)].D());
		//				disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hvPositionY, hvPositionX, "green", "false");
		//			}
		//			else if (k == hoFoil.CountObj() - 1)
		//			{

		//				hvPositionX = hv_Column - dOffset / g_vec_ColumnResolution[i];
		//				strValue.Format("条数%d \r\nAT9尺寸%.2f\r\nMD尺寸%.2f\r\nMD错位尺寸%.2f\r\n", 2 * k - 1+1, tOutput.B[i].tDetectOutput.hv_CeramicWidth[2 * k - 1].D(), tOutput.B[i].tDetectOutput.hv_AT9EdageToProEdage[2 * k - 1].D(), hvCuoweiTuple[MIN(hvCuoweiTuple.Length() - 1, 2 * k - 1)].D());
		//				disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hvPositionY, hvPositionX, "green", "false");
		//			}
		//			else
		//			{

		//				hvPositionX = hv_Column - dOffset / g_vec_ColumnResolution[i];
		//				strValue.Format("条数%d \r\nAT9尺寸%.2f\r\nMD尺寸%.2f\r\nMD错位尺寸%.2f\r\n", 2 * k - 1+1, tOutput.B[i].tDetectOutput.hv_CeramicWidth[2 * k - 1].D(), tOutput.B[i].tDetectOutput.hv_AT9EdageToProEdage[2 * k - 1].D(), hvCuoweiTuple[MIN(hvCuoweiTuple.Length() - 1, 2 * k - 1)].D());
		//				disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hvPositionY / 2, hvPositionX, "green", "false");

		//				hvPositionX = hv_Column + dOffset / g_vec_ColumnResolution[i];
		//				strValue.Format("条数%d \r\nAT9尺寸%.2f\r\nMD尺寸%.2f\r\nMD错位尺寸%.2f\r\n", 2 * k+1, tOutput.B[i].tDetectOutput.hv_CeramicWidth[2 * k].D(), tOutput.B[i].tDetectOutput.hv_AT9EdageToProEdage[2 * k].D(), hvCuoweiTuple[MIN(hvCuoweiTuple.Length() - 1, 2 * k)].D());
		//				disp_message(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), (HTuple)strValue, "image", hvPositionY * 3 / 2, hvPositionX, "green", "false");
		//			}






		//		}

		//		set_display_font(m_hvec_MainWindowsHandle[i].T(), 16, "mono", "true", "false");

		//	}
		//}
		//catch (HException& e)
		//{



		//}



		if (tOutput.B[i].tDetectOutput.hv_ErrorType.Length() > 0)
		{
			strMessage.Format("%s", g_Count.strAClassName.GetAt(tOutput.B[i].tDetectOutput.hv_ErrorType[0].I()));
			SetTposition(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), 300, 25);
			WriteString(m_hvec_MainWindowsHandle[tOutput.B[i].A.hvGrabberSN].T(), HTuple(strMessage));
		}





	}





}



BOOL CSeekersDlg::GetSingleFrameSurfaceResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut)
{
	///////对各种缺陷进行分类统计	
	if (g_tSystemSet.bEnablePopWindowToWarn == FALSE)
	{
		return TRUE;
	}

	BOOL bRet = TRUE;
	HTuple hvIndex, hv_SortTuple;
	HObject hoSingelSumErrorRegion;
	HTuple  hvSingelSumErrorType;
	int nGrabberSN;
	int nSide = tOutPut.hvPackageType.I();
	for (int k = 0; k < 2; k++)
	{
		nGrabberSN = tOutPut.B[k].A.hvGrabberSN;
		double dwRowResolution = g_vec_RowResolution[nGrabberSN];
		double dwColumnResolution = g_vec_ColumnResolution[nGrabberSN];
		hvSingelSumErrorType = tOutPut.B[k].tDetectOutput.hv_ErrorType;
		hoSingelSumErrorRegion = tOutPut.B[k].tDetectOutput.ho_ErrorRegion;
		HObject hoSingleTypeError;
		HTuple hvSingleTypeSumArea;
		try
		{

			if (tOutPut.bIsFinalResult == TRUE)
			{

				g_Jugde[nSide].m_nContinuAt9NGNum = 0;
			}


			if (hvSingelSumErrorType.Length() > 0)
			{
				for (int j = 0; j < g_Jugde[nSide].m_tStandardSurface.hv_ErrorIndex.Length(); j++)
				{
					if (g_Jugde[nSide].m_tStandardSurface.hv_EnablePopWindow[j].I() == FALSE)
					{
						continue;
					}
					HTuple hv_ErrorDiameter, hv_SumArea, hv_ErrorNum;
					TupleFind(hvSingelSumErrorType, g_Jugde[nSide].m_tStandardSurface.hv_ErrorIndex[j], &hvIndex);
					hv_ErrorNum = hvIndex.Length();
					if (0 != hv_ErrorNum && -1 != hvIndex)
					{
						HTuple hv_HorizonWidth;
						SelectObj(hoSingelSumErrorRegion, &hoSingleTypeError, hvIndex + 1);
						AreaCenter(hoSingleTypeError, &hvSingleTypeSumArea, NULL, NULL);//该类缺陷的全部单个面积
						RegionFeatures(hoSingleTypeError, "width", &hv_HorizonWidth);
						hv_SumArea = hvSingleTypeSumArea.TupleSum()*g_vec_RowResolution[nGrabberSN] * g_vec_ColumnResolution[nGrabberSN];
						HTuple hv_JudageArea, hv_JudageNum, hv_JudageAreaTwo, hv_JudageNumTwo;
						HTuple hv_Result0Index, hv_Result1Index, hv_ResultWidthIndex;
						TupleGreaterEqualElem(hvSingleTypeSumArea*g_vec_RowResolution[nGrabberSN] * g_vec_ColumnResolution[nGrabberSN], g_Jugde[nSide].m_tStandardSurface.hv_TotalMaxArea[j].D(), &hv_Result0Index);
						TupleGreaterEqualElem(hvSingleTypeSumArea*g_vec_RowResolution[nGrabberSN] * g_vec_ColumnResolution[0], g_Jugde[nSide].m_tStandardSurface.hv_TotalMaxAreaTwo[j].D(), &hv_Result1Index);
						TupleGreaterEqualElem(hv_HorizonWidth*g_vec_ColumnResolution[nGrabberSN], g_Jugde[nSide].m_tStandardSurface.hv_SingleMaxWidth[j].D(), &hv_ResultWidthIndex);

						HTuple hv_Result0, hv_Result1;
						TupleFind(hv_Result0Index, 1, &hv_Result0);
						TupleFind(hv_Result1Index, 1, &hv_Result1);


						/*	if (g_Jugde[nSide].m_tStandardSurface.hv_ErrorIndex[j]==AT9Size)
							{

								g_Jugde[nSide].m_nContinuAt9NGNum++;

							}
	*/

						if (g_Jugde[nSide].m_nContinuAt9NGNum > g_Jugde[nSide].m_tStandardSurface.hv_TotalMaxNumber[j] || (hv_Result0 != -1 && hv_Result0.Length() >= g_Jugde[nSide].m_tStandardSurface.hv_TotalMaxNumber[j] || hv_Result1 != -1 && hv_Result1.Length() >= g_Jugde[nSide].m_tStandardSurface.hv_TotalMaxNumberTwo[j]) && hv_ResultWidthIndex.TupleSum() > 0)
						{
							////存在超标的缺陷
							g_Jugde[nSide].m_nContinuAt9NGNum = 0;
							if (g_hvPopWindowNum[nSide] > g_tSystemSet.nMaxPopWindowNum)
							{
								return TRUE;
							}
							CString strValue;
							CString strLocation;
							if (nSide == 0)
								strLocation = "内侧";
							else
								strLocation = "外侧";
							strValue.Format(" %s错误缺陷名称:%s 面积:%.2fmm^2  弹窗数%d", strLocation, g_Count.strAClassName.GetAt(g_Jugde[nSide].m_tStandardSurface.hv_ErrorIndex[j].I()), hv_SumArea.D(), g_hvPopWindowNum[nSide].I());
							PopErrorInfo tPopErrorInfo;
							tPopErrorInfo.StrErrorInfo = strValue;
							tPopErrorInfo.nSide = nSide;
							tPopErrorInfo.hoImage = tOutPut.B[k].A.hoGrabImage;
							tPopErrorInfo.hoErrorRegion = tOutPut.B[k].tDetectOutput.ho_ErrorRegion;
							tPopErrorInfo.hvGrabberSN = nGrabberSN;
							m_pDlgPopError = new CDlgPopError;
							m_pDlgPopError->m_tPopErrorInfo = tPopErrorInfo;
							m_pDlgPopError->Create(CDlgPopError::IDD, this);
							m_pDlgPopError->CenterWindow();
							m_pDlgPopError->ShowWindow(SW_SHOWNOACTIVATE);
							m_pDlgPopError->Disp();

							bRet = FALSE;
							g_CUpLoad.SendExceptionWarn(nSide, strValue);
							break;
						}
					}

				}
			}
		}
		catch (HException e)
		{
			CString strException;
			strException.Format("GetSingleFrameSurfaceResult函数:%s:%s ", e.ProcName().Text(), e.ErrorMessage().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
		}


	}
	return bRet;
}


LRESULT CSeekersDlg::OnDisp(WPARAM wParam, LPARAM lParam)
{


	HTuple hv_MessageRecv, hv_StopProcInfo;
	HTuple hvMessagenum;
	HTuple hv_TimeStart, hv_TimeEnd;
	HTuple hvUpFlag(0), hvDownFlag(0), hvUpIndex, hvDownIndex;
	CountSeconds(&hv_TimeStart);//开始记时
	try
	{
		GetMessageQueueParam(g_hvec_QueueDisplay[0].T(), "message_num", &hvMessagenum);
		for (int i = 0; i < hvMessagenum; i++)
		{
			DequeueMessage(g_hvec_QueueDisplay[0].T(), "timeout", "infinite", &hv_MessageRecv);
			GetMessageParam(hv_MessageRecv, "key_exists", "stop_processing", &hv_StopProcInfo);
			if (0 != (HTuple(hv_StopProcInfo[0])))
			{
				return 0L;
			}
			C_ALLOCATORWORKSHOPOUTPUT tOutput;
			tOutput.ReadMessage(hv_MessageRecv);
			ClearMessage(hv_MessageRecv);

			DispImage(tOutput);//显示区域和图片


			/////测试错误弹窗
			GetSingleFrameSurfaceResult(tOutput);//测试

			History(tOutput);


			Sub(tOutput);

			Size(tOutput);

			Count(tOutput);

			LightAdjust(tOutput);


		}

	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("OnDisp出现异常\r\n异常信息%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
	CountSeconds(&hv_TimeEnd);//执行结束
	CString strTime;
	strTime.Format("OnDisp:%05.03f", (hv_TimeEnd.D() - hv_TimeStart.D()) * 1000);
	g_pLog->DoSomething("LOG", (void*)(LPCSTR)(strTime), NULL);
	return 0L;
}

HBRUSH CSeekersDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{

	}

	return hbr;
}

void CSeekersDlg::LightAdjust(C_ALLOCATORWORKSHOPOUTPUT& OutPut)
{
	HTuple hv_MeanValue;
	int nMeanValue, nSide, nCameraIndex;
	nSide = OutPut.hvPackageType.I();
	if (nSide == 0)
	{

		try
		{

			Intensity(OutPut.B[0].tDetectOutput.ho_Coat, OutPut.B[0].A.hoGrabImage, &hv_MeanValue, NULL);
			nMeanValue = (int)hv_MeanValue.D();
		}
		catch (HException& e)
		{
			nMeanValue = 0;
		}
		nCameraIndex = 1;
		g_pLightControl->DoSomething("AdaptLightBright", &nMeanValue, &nCameraIndex);
		try
		{

			Intensity(OutPut.B[1].tDetectOutput.ho_Coat, OutPut.B[1].A.hoGrabImage, &hv_MeanValue, NULL);
			nMeanValue = (int)hv_MeanValue.D();
		}
		catch (HException& e)
		{
			nMeanValue = 0;
		}
		nCameraIndex = 2;
		g_pLightControl->DoSomething("AdaptLightBright", &nMeanValue, &nCameraIndex);
	}




	//////超时判断///
	{

		static int s_nOverCount[4] = { 0,0,0,0 };

		for (int k = 0; k<g_nGrabberCount; k++)
		{
			if (OutPut.B[k].hvTimeStamp.TupleSum().D()>g_tSystemSet.nOverTimeValue)
			{

				s_nOverCount[k * 2 + nSide] = s_nOverCount[k * 2 + nSide] + 1;
				if (s_nOverCount[k * 2 + nSide] >= g_tSystemSet.nOverTimeFrameValue)
				{
					//报警停机
					s_nOverCount[k * 2 + nSide] = 0;
					CString  strException;
					strException.Format("相机%d连续%d张检测超时%d ", k * 2 + nSide, g_tSystemSet.nOverTimeFrameValue, g_tSystemSet.nOverTimeValue);
					g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
					g_CUpLoad.SendExceptionWarn(nSide, strException);
				}



			}
			else
			{
				s_nOverCount[k * 2 + nSide] = 0;
			}
		}








	}






}

void CSeekersDlg::SaveCurrentStatus()
{



	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileString("INFO", "BATCH", g_strBatch, g_strConfigSystemFile);
	WritePrivateProfileString("INFO", "PRODUCTNAME", g_strProductName, g_strConfigSystemFile);
	WritePrivateProfileDouble("INFO", "YIELD", g_dwYield, g_strConfigSystemFile);//产量
	WritePrivateProfileString("INFO", "OperatorPassWord", g_strOperatorPassWord, g_strConfigSystemFile);//写入操作员密码
	WritePrivateProfileString("INFO", "EngineerPassWord", g_strEngineerPassWord, g_strConfigSystemFile);//写入工程师密码
	WritePrivateProfileString("INFO", "FactoryPassWord", g_strFactoryPassWord, g_strConfigSystemFile);//写入厂家密码
	WritePrivateProfileInt("INFO", "g_bUseAutoFlag", g_bUseAutoFlag, g_strConfigSystemFile);//允许使用自动换型
	WritePrivateProfileString("StorgePath", "g_strSaveFullPath", g_strSaveFullPath, g_strConfigSystemFile);
	WritePrivateProfileString("StorgePath", "g_strSaveBasePath", g_strSaveBasePath, g_strConfigSystemFile);
	WritePrivateProfileInt("INFO", "g_bUseSeekersWithoutServer", g_bUseSeekersWithoutServer, g_strConfigSystemFile);//是否隐藏掉本地换卷和登陆


	WritePrivateProfileInt("INFO", "g_bNGLabelTwoEA", g_bNGLabelTwoEA, g_strConfigSystemFile);//报存标志



	WritePrivateProfileString("INFO", "g_strLabelTypeName", g_strLabelTypeName, g_strConfigSystemFile);

	CString strPath;
	strPath.Format("%s\\Config\\Public\\g_hvXTuple.tup", GetCurrentAppPath());
	try
	{
		WriteTuple(g_hvXTuple, (HTuple)strPath);
	}
	catch (HException& e)
	{

	}
	strPath.Format("%s\\Config\\Public\\g_hvYTuple.tup", GetCurrentAppPath());
	try
	{
		WriteTuple(g_hvYTuple, (HTuple)strPath);
	}
	catch (HException& e)
	{

	}
	///保存卷绕的次品数和良品数
	for (int k = 0; k < g_nGrabberCount; k++)
	{
		CString strSection;
		strSection.Format("COUNT%d", k);
		for (int i = 0; i < g_tSystemSet.nLaneCount; i++)
		{
			s_SingleCount sc = g_Count.veCount[k].at(i);
			CString strKey;
			for (int j = 0; j < g_Count.nType; j++)
			{
				strKey.Format("ERRC%d", i*g_Count.nType + j);
				WritePrivateProfileInt(strSection, strKey, sc.nClassError[j], g_strConfigSystemFile);
			}
			strKey.Format("ERR%d", i);
			WritePrivateProfileInt(strSection, strKey, sc.nError, g_strConfigSystemFile);
			strKey.Format("TOTAL%d", i);
			WritePrivateProfileInt(strSection, strKey, sc.nTotal, g_strConfigSystemFile);
		}
	}

	for (int k = 0; k < 2; k++)
	{
		g_Jugde[k].WriteParam();
	}

	g_CPowerLimit.WriteParam();

	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
}
void CSeekersDlg::OnBnClickedButtonExitApplication()
{

	//记录当前信息
	if (g_bChangeProduct)//换型标志位强制重启
	{
		g_CPowerLimit.m_bFactoryPower = TRUE;
	}

	if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())//三者满足一即可切换查看错误视图
	{
		return;
	}
	else
	{
		if (g_bChangeProduct || IDOK == MessageBox(_T("确认需要关闭软件吗！"), _T("警告!"), MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
		{

			CString strValue;
			strValue = "程序退出";
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
			PostMessageA(WM_CLOSE);
		}
		else
		{
			return;
		}

	}

}


void CSeekersDlg::OnBnClickedButtonSwitchViews()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nCurrentDisplayView++;

	if (VIEW_PICTURE == m_nCurrentDisplayView)
	{
		GetDlgItem(IDC_PICTURE_MAIN)->ShowWindow(SW_NORMAL);
		//m_pSubCountDlg->ShowWindow(SW_HIDE);
		m_pSubSizeDlg->ShowWindow(SW_HIDE);
		m_pFinalOutResult->ShowWindow(SW_HIDE);
		m_pDlgScatterPlot->ShowWindow(SW_HIDE);

	}
	else if (VIEW_SIZE == m_nCurrentDisplayView)
	{
		GetDlgItem(IDC_PICTURE_MAIN)->ShowWindow(SW_HIDE);
		m_pSubSizeDlg->ShowWindow(SW_NORMAL);
		m_pFinalOutResult->ShowWindow(SW_HIDE);
		m_pDlgScatterPlot->ShowWindow(SW_HIDE);

	}
	else if (VIEW_FINAL == m_nCurrentDisplayView)
	{
		GetDlgItem(IDC_PICTURE_MAIN)->ShowWindow(SW_HIDE);
		m_pFinalOutResult->ShowWindow(SW_NORMAL);
		m_pSubSizeDlg->ShowWindow(SW_HIDE);
		m_pDlgScatterPlot->ShowWindow(SW_HIDE);

	}
	/*	else if (VIEW_LINECHART == m_nCurrentDisplayView)
		{
			GetDlgItem(IDC_PICTURE_MAIN)->ShowWindow(SW_HIDE);
			m_pFinalOutResult->ShowWindow(SW_HIDE);
			m_pSubSizeDlg->ShowWindow(SW_HIDE);
			m_pDlgScatterPlot->ShowWindow(SW_NORMAL);

		}*/



	if (m_nCurrentDisplayView >= VIEW_FINAL)
	{
		m_nCurrentDisplayView = -1;
	}
}
void CSeekersDlg::OnBnClickedButtonSwitchBatch()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (FALSE==g_CPowerLimit.IsAllContinue())
	//{
	//	return;
	//}
	CDlgSwitchBatch dlg;
	dlg.DoModal();
}





BOOL CSeekersDlg::DispHistoryAlgorithmDlg(const int &nGrabberSN, const int &nObjectIndex)
{
	HObject ImageSelected;
	SelectObj(m_hovec_Histroy_Image[nGrabberSN].O(), &ImageSelected, nObjectIndex + 1);
	if (nObjectIndex + 1 < 1)
	{
		GenImageConst(&ImageSelected, "byte", 4096, 3000);
	}
	DETECTINPUT tDetectInput;
	//tDetectInput.hoDetectImage = ImageSelected;
	CopyImage(ImageSelected, &tDetectInput.hoDetectImage);
	tDetectInput.nBreakType = 0;
	if (TRUE != g_vec_Algorithm[nGrabberSN]->DoSomething("SHOW", &tDetectInput, NULL))
	{
		CString strException;
		strException.Format("算法%d调用显示界面异常,请排查原因", nGrabberSN);
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
		return FALSE;
	}
	int nDestGrabberSN;
	switch (nGrabberSN)
	{
	case 0:
		nDestGrabberSN = 1;
		break;
	case 1:
		nDestGrabberSN = 0;
		break;
	}
	if (TRUE == g_tSystemSet.nMergeResults)
	{
		g_vec_Algorithm[nDestGrabberSN]->DoSomething("COPYPARAM", "ALGPARAM", g_vec_Algorithm[nGrabberSN]);
	}
	return TRUE;
}

BOOL CSeekersDlg::DispSubWinAlgorithmDlg(const int &nSubWindowsHandleSN)
{
	int nIndex = nSubWindowsHandleSN + SUB_WINDOW_COUNT*(m_nSubPage - 1);

	std::list<SUBELEMENT>::iterator iter = m_lstSubImage.begin();
	advance(iter, nIndex);
	DETECTINPUT tDetectInput;
	//tDetectInput.hoDetectImage = iter->hoImage;
	CopyImage(iter->hoImage, &tDetectInput.hoDetectImage);
	tDetectInput.nBreakType = 0;
	if (TRUE != g_vec_Algorithm[iter->nGrabberSN]->DoSomething("SHOW", &tDetectInput, NULL))
	{
		CString strException;
		strException.Format("算法%d调用显示界面异常,请排查原因", iter->nGrabberSN);
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
		return FALSE;
	}
	return TRUE;
}

BOOL CSeekersDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//屏蔽回车
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)//屏蔽ESC
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_SPACE)//
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//屏蔽ALT+F4
		{
			return TRUE;
		}
		else if ((pMsg->wParam == VK_S || pMsg->wParam == VK_s) && HIBYTE(::GetKeyState(VK_MENU)))
		{
			m_btnTools.SetFocus();
			OnBnClickedButtonTools();
			return TRUE;
		}
		else if ((pMsg->wParam == VK_Q || pMsg->wParam == VK_q) && HIBYTE(::GetKeyState(VK_MENU)))
		{
			BOOL bShow = TRUE;
			g_pLog->DoSomething("SETPARAM", "SHOWEXCEPTION", &bShow);
			return TRUE;
		}
		else if ((pMsg->wParam == VK_H || pMsg->wParam == VK_h) && HIBYTE(::GetKeyState(VK_MENU)))
		{
			CDlgDatabaseHistory dlg;
			dlg.DoModal();
			return TRUE;
		}
		else if (pMsg->wParam == VK_F10)
		{
			CDlgDatabaseHistory dlg;
			dlg.DoModal();
			return TRUE;
		}
	}
	else if (pMsg->message == WM_LBUTTONDBLCLK)//如果双击了屏幕
	{

		CRect rcHistory, rcImage;
		GetDlgItem(IDC_PICTURE_HISTORY)->GetWindowRect(&rcHistory);
		GetDlgItem(IDC_PICTURE_MAIN)->GetWindowRect(&rcImage);
		if (PtInRect(rcHistory, pMsg->pt))
		{
			HTuple Button, Row, Column;
			for (int i = 0; i < m_hvec_HistroyWindowsHandle.Length(); i++)//此时两个句柄
			{
				try
				{
					GetMposition(m_hvec_HistroyWindowsHandle[i].T(), &Row, &Column, &Button);
					if (1 == Button)//左键
					{
						HObject ObjectSelected;
						HTuple Inside;
						for (int j = 0; j < m_hovec_Histroy[i].O().CountObj(); j++)
						{
							SelectObj(m_hovec_Histroy[i].O(), &ObjectSelected, j + 1);
							TestRegionPoint(ObjectSelected, Row, Column, &Inside);
							if (1 == Inside)
							{
								DispHistoryAlgorithmDlg(i, j);
								break;
							}
						}

					}
					else if (33 == Button)
					{
						CDlgHistory dlg;
						dlg.m_nGrabberSN = i;
						dlg.m_hoImage = m_hovec_Histroy_Image[i].O();
						dlg.m_hoRegion = m_hovec_Histroy[i].O();
						dlg.DoModal();
					}
				}
				catch (HException e)
				{
					/*CString strValue;
					strValue.Format("异常函数:%s 错误信息%s",e.ProcName().Text(),e.ErrorMessage().Text());
					AfxMessageBox(strValue);*/

				}
			}
			return TRUE;
		}
		else
		{
			CRect rcSub;
			GetDlgItem(IDC_PICTURE_SUB)->GetWindowRect(rcSub);
			if (PtInRect(rcSub, pMsg->pt))
			{
				HTuple Button, Row, Column;
				for (int i = 0; i < m_hvec_SubWindowsHandle.Length(); i++)
				{
					try
					{
						GetMposition(m_hvec_SubWindowsHandle[i].T(), &Row, &Column, &Button);
						if (1 == Button)//左键
						{
							DispSubWinAlgorithmDlg(i);
							break;
						}
					}
					catch (HException)
					{

					}
				}
			}
			return TRUE;
		}


	}
	else if (pMsg->message == WM_RBUTTONDBLCLK)//如果右键双击了屏幕
	{


		CRect rcHistory;
		GetDlgItem(IDC_PICTURE_HISTORY)->GetWindowRect(&rcHistory);
		if (PtInRect(rcHistory, pMsg->pt))
		{

			HTuple Button, Row, Column;
			for (int i = 0; i < m_hvec_HistroyWindowsHandle.Length(); i++)
			{
				try
				{
					GetMposition(m_hvec_HistroyWindowsHandle[i].T(), &Row, &Column, &Button);
					if (4 == Button)//右键
					{
						HObject ObjectSelected;
						HObject ImageSelected;
						HTuple Inside;
						for (int j = 0; j < m_hovec_Histroy[i].O().CountObj(); j++)
						{
							SelectObj(m_hovec_Histroy[i].O(), &ObjectSelected, j + 1);
							TestRegionPoint(ObjectSelected, Row, Column, &Inside);
							if (1 == Inside)
							{
								SelectObj(m_hovec_Histroy_Image[i].O(), &ImageSelected, j + 1);
								if (g_tSystemSet.nCameraModel == i)
								{
									g_vec_FindMark[i]->DoSomething("SHOW", &ImageSelected, NULL);
								}
								break;
							}

						}
					}
				}
				catch (HException)
				{

				}
			}

		}
		return TRUE;

	}
	else if (pMsg->message == WM_LBUTTONUP)//如果左键单击了屏幕
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(&rcTitle);
		rcTitle.right = rcTitle.left + LOGO_WIDTH;
		if (PtInRect(rcTitle, pMsg->pt))
		{
			CDlgHelp dlg;
			dlg.DoModal();
			return TRUE;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void  CSeekersDlg::WriteOutHistory(int& nOut0, int &nOut1)
{
	static int s_nOut0 = 0;
	static int s_nOut1 = 0;
	if (nOut0 == s_nOut0 && nOut1 == s_nOut1)
	{
		return;
	}
	else
	{
		s_nOut0 = nOut0;
		s_nOut1 = nOut1;

	}
	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_strProductName, g_strBatch);

	strSizeRecord.Format("%s\\OUT口计数-%s.csv.", strSizePath, g_strBatch);
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("时间,品种,速度,标机1,标机2,");
		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d日%02d:%02d:%02d", CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond);
	HTuple hv_FileHandle;
	ofstream outFile;
	CString strCount;
	outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
	strCount.Format("%s,%s,%.1f,%d,%d,", strTime, g_strProductName, g_dRealSpeed, nOut0, nOut1);
	outFile << strCount << endl;
	outFile.close();
}




void CSeekersDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == UM_TIMER_QUEUEINFO)
	{

		//更新状态栏
		HTuple hv_Pintu[4], hv_Tuxiang[4], hv_Jieguo[4], hv_Xianshi, hv_Tongxin, hv_Cunchu;
		for (int i = 0; i < g_nGrabberCount; i++)
		{
			try
			{
				GetMessageQueueParam(g_hvec_QueuePintu[i].T(), "message_num", &hv_Pintu[i]);
			}
			catch (HException)
			{
				hv_Pintu[i] = -1;
			}
			try
			{
				GetMessageQueueParam(g_hvec_QueueImages[i].T(), "message_num", &hv_Tuxiang[i]);
			}
			catch (HException)
			{
				hv_Tuxiang[i] = -1;
			}
			try
			{
				GetMessageQueueParam(g_hvec_QueueResults[i].T(), "message_num", &hv_Jieguo[i]);
			}
			catch (HException)
			{
				hv_Jieguo[i] = -1;
			}

		}
		GetMessageQueueParam(g_hvec_QueueDisplay[0].T(), "message_num", &hv_Xianshi);
		g_pCommunicate->DoSomething("GETPARAM", "MESSAGE_NUM", &hv_Tongxin);
		g_pStorage->DoSomething("GETPARAM", "MESSAGE_NUM", &hv_Cunchu);
		int nReadCount, nLineCount(0);
		nReadCount = 16;
		g_pCommunicate->DoSomething("READCOUNT", &nReadCount, &nLineCount);
		double dwSpeed = nLineCount*g_vec_RowResolution[0] / 1000 * 60 * 2 / g_nGrabberLineDivideNum;
		g_dRealSpeed = dwSpeed;
		static double dLastSpeed = 0;

		/////////////////////////////////////////////////////////////
		int nFlag;
		g_pCommunicate->DoSomething("GETPARAM", "LabelWarnStaus", &nFlag);
		g_pLog->DoSomething("GETPARAM", "SoftStatus", &g_bExceptionSoftStatus);//显示异常对话框
		BOOL bFlag = FALSE;
		g_pCLabeller->DoSomething("GETPARAM", "PLCWarnStatus", &bFlag);


		if (g_nGrabberType == 1)//测试
		{
			if (nFlag == 1)//表示存在没清空的报警
			{

				if (g_bExceptionSoftStatus == FALSE)
				{
					CString strException;
					strException.Format("存在未处理的贴标机报警，请先复位处理再开机");
					g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
				}

				g_pCommunicate->DoSomething("SETPARAM", "BJTJ", NULL);
			}
			else if (bFlag)//表示存在没清空的PLC报警
			{

				if (g_bExceptionSoftStatus == FALSE)
				{
					CString strException;
					strException.Format("存在未处理的PLC报警信息，请先复位处理再开机");
					g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
				}

				g_pCommunicate->DoSomething("SETPARAM", "BJTJ", NULL);
			}
			else if (g_bExceptionSoftStatus)//当前存在异常对话框就得报警停机
			{
				g_pCommunicate->DoSomething("SETPARAM", "BJTJ", NULL);
			}


		}



		/////////////////////////////////////////////////////////////

		if (dLastSpeed == 0 && dwSpeed > 0)//获取一下贴标状态
		{
			dLastSpeed = dwSpeed;
			LabelControlInfo tLabelControlInfo;
			g_pCommunicate->DoSomething("GETPARAM", "LabelControlInfo", &tLabelControlInfo);
			CString strLabelStatus;
			int nEnabelMarkStatus(0);

			if (tLabelControlInfo.bEnableFirstLabel == 1)
			{
				m_staticTitle.SetBkColor(RGB(0, 255, 0));
				strLabelStatus.Format(" 正常贴标状态:贴标开启");
				m_staticTitle.SetWindowText(strLabelStatus);
				m_staticTitle.SetTextColor(RGB(0, 0, 0));
			}
			else
			{
				strLabelStatus.Format(" 异常贴标状态:贴标关闭");
				m_staticTitle.SetBkColor(RGB(255, 0, 0));
				m_staticTitle.SetWindowText(strLabelStatus);
				m_staticTitle.SetTextColor(RGB(0, 0, 0));
			}

		}
		else if (dwSpeed == 0)
		{
			dLastSpeed = 0;
		}


		//if (dwSpeed==0)
		//{
		//	IntervalTimeSave();
		//}






		CString strMeter, strLabelConnectStatus, strServerConnectStatus, StrDianYanStatus;
		strMeter.Format("%.2fm", g_dwYield / 1000);
		m_STC_CMeter.SetWindowTextA(strMeter);

		//int nOut0Count = 0;
		//int nOut1Count = 0;
		//int nOut0nIndex = 4;
		//g_pCommunicate->DoSomething("READCOUNT", &nOut0nIndex, &nOut0Count);
		//int nOut1nIndex = 5;
		//g_pCommunicate->DoSomething("READCOUNT", &nOut1nIndex, &nOut1Count);

		//int nLabelStatusFlag=0;
		//g_pCommunicate->DoSomething("GETPARAM","LabelStatus",&nLabelStatusFlag);
		//if (nLabelStatusFlag==1)
		//{
		//	strServerConnectStatus.Format("贴标机已连接 报警Out0:%d 心跳Out1:%d", nOut0Count, nOut1Count);
		//}else
		//{
		//	strServerConnectStatus.Format("贴标机已连接 报警Out0:%d 心跳Out1:%d", nOut0Count, nOut1Count);
		//}
		strServerConnectStatus = "";

		CString strPLCStatus = "";
		g_pCLabeller->DoSomething("GETPARAM", "StrLabellerStatus", strPLCStatus.GetBuffer(MAX_PATH));
		strPLCStatus.ReleaseBuffer();

		CString strLightStatus;
		g_pLightControl->DoSomething("GetConnection", NULL, strLightStatus.GetBuffer(MAX_PATH));
		strLightStatus.ReleaseBuffer();

		CString strCunchu = "";
		CString strTemp;
		for (int k = 0; k < hv_Cunchu.Length(); k++)
		{
			if (k == 0)
				strTemp.Format("%d", hv_Cunchu[k].I());
			else
				strTemp.Format("-%d", hv_Cunchu[k].I());

			strCunchu += strTemp;
		}



		CString strExportStatus;
		g_pStorage->DoSomething("GETPARAM", "ExportStatus", strExportStatus.GetBuffer(100));
		strExportStatus.ReleaseBuffer();
		CString strDisplay;
		strDisplay.Format("拼图剩余:%d-%d  处理剩余:%d-%d 综合结果剩余:%d-%d  \
						   显示剩余:%d  通信剩余:%d  存:%s      %s     %s    %s  导出状态:%s             ",
			hv_Pintu[0].I(), hv_Pintu[1].I(),
			hv_Tuxiang[0].I(), hv_Tuxiang[1].I(),
			hv_Jieguo[0].I(), hv_Jieguo[1].I(),
			hv_Xianshi[0].I(), hv_Tongxin[0].I(), strCunchu, strServerConnectStatus, strLightStatus, strPLCStatus, strExportStatus);
		m_wndStatusBar.SetPaneText(5, strDisplay);
		//更新当前信息
	}
	else if (UM_TIMER_STARTGRAB == nIDEvent)
	{
		UpdateLabelCount();
		if (g_nGrabberType != 0)
		{
			for (int i = 0; i < g_nGrabberCount; i++)//打开相机
			{
				g_vec_Grabber[i]->DoSomething("STARTGRAB", NULL, NULL);
			}
			OnBnClickedButtonStart();
			g_pCommunicate->DoSomething("ENABLE", NULL, NULL);
			g_pCommunicate->DoSomething("READY", "ON", NULL);
			KillTimer(nIDEvent);
		}
	}
	else if (UM_TIMER_TIME == nIDEvent)
	{
		CString strTime;
		CTime time = CTime::GetCurrentTime();
		strTime.Format("%02d:%02d", time.GetHour(), time.GetMinute());

		/////确认是否自动退出当前登陆
		if (g_bExitLogin == TRUE)
		{
			if (m_hvExitTimeCount.Length() == 0)
			{
				CountSeconds(&m_hvExitTimeCount);
			}
			else
			{
				HTuple hv_CurrentTime;
				int nTimeMinite = MAX(1, g_tSystemSet.nOverTimeLimitLoginOff);//假如设置时间是5分钟
				CountSeconds(&hv_CurrentTime);
				double dTempTimeCount = hv_CurrentTime.D() - m_hvExitTimeCount.D();
				CString strInfo;
				strInfo.Format("%s\\%.1f", strTime, dTempTimeCount);
				m_wndStatusBar.SetPaneText(7, strInfo);
				if ((hv_CurrentTime - m_hvExitTimeCount) > nTimeMinite * 60)//这个时候系统需要判断是否需要退出
				{
					m_hvExitTimeCount.Clear();
					CString strBtnText;
					GetDlgItem(IDC_BUTTON_LOGIN)->GetWindowTextA(strBtnText);
					if ("注销" == strBtnText)
					{
						g_bExitLogin = FALSE;
						g_CPowerLimit.ClearFlag();
						CString str;
						str.Format("%s", "无");
						((CSeekersDlg*)g_pMainDialog)->m_wndStatusBar.SetPaneText(6, "用户：" + str);
						strBtnText = "登录";
						GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowTextA(strBtnText);
						m_btnLogin.SetIcon(IDI_ICON_W_KEY);
						m_btnLogin.SetTooltipText(_T("用户登录"));

						CString strValue;
						strValue.Format("停机超过%d分钟执行自动注销操作", nTimeMinite);
						g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);

					}

				}
			}

		}
		else
		{
			m_hvExitTimeCount.Clear();
			m_wndStatusBar.SetPaneText(7, strTime);
		}

		//////光源自动退出功能
		static BOOL S_bOff = FALSE;
		if (g_dRealSpeed == 0 && g_nGrabberType == 1)
		{
			if (m_hvExitLightTimeCount.Length() == 0)
			{
				CountSeconds(&m_hvExitLightTimeCount);
			}
			else
			{
				HTuple hv_CurrentTime;
				int nTimeMinite = MAX(1, g_tSystemSet.nOverTimeExit);//假如设置时间是5分钟
				CountSeconds(&hv_CurrentTime);
				double dTime = hv_CurrentTime.D() - m_hvExitLightTimeCount.D();
				CString strInfo;
				strInfo.Format("%s\\%.1f", strTime, dTime);
				m_wndStatusBar.SetPaneText(7, strInfo);
				if ((hv_CurrentTime - m_hvExitLightTimeCount) > nTimeMinite * 60 && S_bOff == FALSE)//这个时候系统需要判断是否需要退出
				{
					S_bOff = TRUE;
					g_pLightControl->DoSomething("SETPARAM", "OFF", NULL);
					CString strValue;
					strValue.Format("停机超过%d分钟执行关闭光源节能", nTimeMinite);
					g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
				}
			}

		}
		else if (S_bOff == TRUE)
		{
			S_bOff = FALSE;
			CString strValue;
			strValue.Format("速度%.1fm/min,执行打开光源,结束休眠", g_dRealSpeed);
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
			g_pLightControl->DoSomething("SETPARAM", "ON", NULL);
			m_hvExitLightTimeCount.Clear();
			m_wndStatusBar.SetPaneText(7, strTime);
		}
		else
		{
			m_hvExitLightTimeCount.Clear();
			//m_wndStatusBar.SetPaneText(7,strTime);
		}

		///////监控回调是否停止
		static HTuple s_hvStartTime(0), s_hvEndTime(0);
		static double s_dEncoderValue[2] = { 0 };
		if (g_dRealSpeed > 1)
		{
			CountSeconds(&s_hvEndTime);
			int  nSecondTime = 30;
			if ((s_hvEndTime.D() - s_hvStartTime.D()) > nSecondTime)//30s判定回调累积的长度是否产生变化
			{


				if (g_dEncoderLocation[0] != s_dEncoderValue[0] && g_dEncoderLocation[1] != s_dEncoderValue[1])//报警停机
				{
					CountSeconds(&s_hvStartTime);
					CountSeconds(&s_hvEndTime);

					s_dEncoderValue[0] = g_dEncoderLocation[0];
					s_dEncoderValue[1] = g_dEncoderLocation[1];
				}
				else
				{

					CString strException;
					strException.Format("回调在速度%.1fm/min的速度下,%d时间内计数没有发生变化,请确认", g_dRealSpeed, nSecondTime);
					g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

				}

			}
		}
		else
		{
			CountSeconds(&s_hvStartTime);
			CountSeconds(&s_hvEndTime);

		}

		////////////////////////////////////////////
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CSeekersDlg::OnBnClickedButtonProductManager()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
	//{
	//	return;
	//}
	CDlgProductManager dlg;
	dlg.SetOwner(&g_Jugde[0]);
	dlg.DoModal();
}

BOOL CSeekersDlg::Login()
{
	CDlgLogin dlg;
	if (IDOK == dlg.DoModal())
	{
		ShowButton(1);
		CString strBtnText = "注销";
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowTextA(strBtnText);
		m_btnLogin.SetIcon(IDI_ICON_W_LOCK);
		m_btnLogin.SetTooltipText(_T("用户注销"));
		return TRUE;
	}
	return FALSE;
}
void CSeekersDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strBtnText;
	GetDlgItem(IDC_BUTTON_LOGIN)->GetWindowTextA(strBtnText);
	if ("登录" == strBtnText)
	{
		Login();
	}
	else if ("注销" == strBtnText)
	{
		g_bExitLogin = FALSE;
		g_CPowerLimit.ClearFlag();
		CString str;
		str.Format("%s", "无");
		((CSeekersDlg*)g_pMainDialog)->m_wndStatusBar.SetPaneText(6, "用户：" + str);
		ShowButton(0);
		strBtnText = "登录";
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowTextA(strBtnText);
		m_btnLogin.SetIcon(IDI_ICON_W_KEY);
		m_btnLogin.SetTooltipText(_T("用户登录"));

	}

}

void CSeekersDlg::OnBnClickedButtonSubnext()
{
	// TODO: 在此添加控件通知处理程序代码
	++m_nSubPage;
	if (m_nSubPage >= 2)//2019.8
	{
		m_nSubPage = 2;
		m_btnSubNext.SetIcon(IDI_ICON_W_RIGHT);
	}
	else
	{
		m_btnSubBack.SetIcon(IDI_ICON_W_LEFT);
	}
	for (int i = 0; i < SUB_WINDOW_COUNT; i++)
	{
		try
		{
			int nIndex = i + SUB_WINDOW_COUNT*(m_nSubPage - 1);
			std::list<SUBELEMENT>::iterator iter = m_lstSubImage.begin();
			advance(iter, nIndex);
			DispObj(iter->hoSubImage, m_hvec_SubWindowsHandle[i].T());
		}
		catch (HException) {}
	}
}


void CSeekersDlg::OnBnClickedButtonSubback()
{
	// TODO: 在此添加控件通知处理程序代码
	--m_nSubPage;
	if (m_nSubPage <= 1)
	{
		m_nSubPage = 1;
		m_btnSubBack.SetIcon(IDI_ICON_W_LEFT);
	}
	else
	{
		m_btnSubNext.SetIcon(IDI_ICON_W_RIGHT);
	}
	for (int i = 0; i < SUB_WINDOW_COUNT; i++)
	{
		try
		{
			int nIndex = i + SUB_WINDOW_COUNT*(m_nSubPage - 1);
			std::list<SUBELEMENT>::iterator iter = m_lstSubImage.begin();
			advance(iter, nIndex);
			DispObj(iter->hoSubImage, m_hvec_SubWindowsHandle[i].T());
		}
		catch (HException) {}
	}
}


void CSeekersDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ExitDo();
	CDialogEx::OnClose();
}


BOOL CSeekersDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDlgHelp dlg;
	dlg.DoModal();
	return TRUE;
	return CDialogEx::OnHelpInfo(pHelpInfo);
}


void CSeekersDlg::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::WinHelp(dwData, nCmd);
}


void CSeekersDlg::OnStnClickedStaticBatch()
{
	OnBnClickedButtonSwitchBatch();
}

void CSeekersDlg::OnBnClickedButtonTools()
{
	//if (FALSE==g_CPowerLimit.IsEngineAndFactoryContinue())
	//{
	//	return;
	//}
	CRect rct;
	GetDlgItem(IDC_BUTTON_TOOLS)->GetWindowRect(&rct);
	CPoint point(rct.left, rct.bottom);
	CMenu* m_tempmenu = m_pMenuTools->GetSubMenu(0);
	m_tempmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
}

void CSeekersDlg::OnStnDblclickStaticTitle()
{

}

void CSeekersDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle, rcBatch;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(&rcTitle);
	GetDlgItem(IDC_STATIC_BATCH)->GetWindowRect(&rcBatch);
	rcTitle.right = rcTitle.left + LOGO_WIDTH;
	if (rcTitle.PtInRect(point) || rcBatch.PtInRect(point))//鼠标在图标上
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
		::SetCursor(hCur);
	}
	else
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
		::SetCursor(hCur);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CSeekersDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}



int CSeekersDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetFocus();
	ShowWindow(FALSE);
	return 0;
}


void CSeekersDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDCtl == 0)
	{
		m_pMenuTools->DrawItem(lpDrawItemStruct);
	}
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CSeekersDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == 0)
	{
		m_pMenuTools->MeasureItem(lpMeasureItemStruct);
	}
	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


BOOL CSeekersDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT uID = (UINT)wParam;//所选择的项的ID
	switch (uID)
	{
	case ID_MENU_HISTORYIMAGE0:
	{
		//历史图像相机0
		CDlgHistory dlg;
		dlg.m_nGrabberSN = 0;
		dlg.m_hoImage = m_hovec_Histroy_Image[0].O();
		dlg.m_hoRegion = m_hovec_Histroy[0].O();
		dlg.DoModal();
		break;
	}
	case ID_MENU_HISTORYIMAGE1:
	{
		//历史图像相机1
		CDlgHistory dlg;
		dlg.m_nGrabberSN = 1;
		dlg.m_hoImage = m_hovec_Histroy_Image[1].O();
		dlg.m_hoRegion = m_hovec_Histroy[1].O();
		dlg.DoModal();
		break;
	}
	case ID_MENUE_LIGHTCONTROL:
	{


		BOOL bEnable = TRUE;
		g_pLightControl->DoSomething("SHOW", NULL, &bEnable);
		break;
	}
	case ID_MENU_ALGORITHM0:
	{
		//算法界面0
		HTuple hvNum;
		CountObj(m_hovec_Histroy_Image[0].O(), &hvNum);
		DispHistoryAlgorithmDlg(0, hvNum - 1);

		break;
	}
	case ID_MENU_ALGORITHM1:
	{
		//算法界面1
		HTuple hvNum;
		CountObj(m_hovec_Histroy_Image[1].O(), &hvNum);
		DispHistoryAlgorithmDlg(1, hvNum - 1);
		break;
	}

	case ID_MENU_COUNTRECORD:
	{
		//统计记录
		CDlgDatabaseHistory dlg;
		dlg.DoModal();
		break;
	}
	case ID_MENU_EXCEPTION:
	{
		//异常日志
		BOOL bRet = TRUE;
		g_pLog->DoSomething("SETPARAM", "SHOWEXCEPTION", &bRet);
		break;
	}

	case ID_MENU_HELP:
	{
		//帮助
		CString strHelpFile;
		strHelpFile.Format("%s\\Seekers软件使用手册.pdf", GetCurrentAppPath());
		if (!PathFileExists(strHelpFile))
		{
			CString strMessage;
			strMessage.Format("无法打开帮助文档，请检查");
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
			break;
		}
		CStdioFile	 helpFile;
		helpFile.Open(_T(strHelpFile), CFile::modeWrite/*|CFile::typeText*/, NULL);

		if (helpFile.m_hFile == CFile::hFileNull)
		{
			CString strMessage;
			strMessage.Format("帮助文档已经打开，请检查");
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
			break;
		}
		helpFile.Close();
		HINSTANCE hInstance = ShellExecute(NULL, _T("open"), _T(strHelpFile), NULL, NULL, SW_SHOW);
		ASSERT(hInstance != NULL);
		break;
	}
	case ID_MENU_ABOUT:
	{
		//关于
		CDlgHelp dlg;
		dlg.DoModal();
		break;
	}
	case ID_MENU_CAMERASET0:
	{
		if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			break;
		}

		if (IDOK == MessageBox(_T("确认需要需要打开相机调试软件吗，调试完毕必须重启软件，否则影响贴标！"), _T("严重警告!"), MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
		{

			//相机0设置
			g_vec_Grabber[0]->DoSomething("SHOW", NULL, NULL);
		}


		break;
	}
	case ID_MENU_CAMERASET1:
	{
		if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			break;
		}
		//相机1设置


		if (IDOK == MessageBox(_T("确认需要需要打开相机调试软件吗，调试完毕必须重启软件，否则影响贴标！"), _T("严重警告!"), MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
		{
			if (1 == g_nGrabberCount)
			{
				g_vec_Grabber[0]->DoSomething("SHOW", NULL, NULL);
			}
			else
			{
				g_vec_Grabber[1]->DoSomething("SHOW", NULL, NULL);
			}
		}
		break;
	}
	case ID_MENU_IOSET:
	{
		if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			break;
		}
		//IO设置
		g_pCommunicate->DoSomething("SHOW", NULL, NULL);
		break;
	}
	case ID_MENU_SYSETEM:
	{
		if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			break;
		}
		//系统设置
		CDlgSystemSet dlg;
		dlg.DoModal();
		break;
	}
	case ID_MENU_STORAGE:
	{
		if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			break;
		}
		//存储设置
		g_pStorage->DoSomething("SHOW_PARAMSET", NULL, NULL);
		break;
	}
	default:
	{
		break;
	}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}




void CSeekersDlg::OnMenuProset()
{
	//产品设置
	CDlgProductParam dlg;
	dlg.DoModal();
}


void CSeekersDlg::OnMenuAlgparam()
{

}


void CSeekersDlg::OnMenuRestart()
{
	if (IDOK == MessageBox("是否重新启动检测系统？", "提示", MB_OKCANCEL))
	{
		ExitDo();
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		memset((void*)&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.wShowWindow = SW_SHOW;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		CString strApp;
		strApp.Format("%s\\Seekers.exe", GetCurrentAppPath());
		CreateProcess(NULL, strApp.GetBuffer(0), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
		::TerminateProcess(::GetCurrentProcess(), NULL);//终止原先程序
	}
}

void CSeekersDlg::OnBnClickedBtnDrawleftmodel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_tSystemSet.nCameraModel == 0)
	{
		HObject ImageSelected;
		if (m_hovec_Histroy_Image[0].O().CountObj() > 0)
		{
			SelectObj(m_hovec_Histroy_Image[0].O(), &ImageSelected, 1);
		}
		else
		{
			GenImageConst(&ImageSelected, "byte", 4096, 3000);
		}

		g_vec_FindMark[0]->DoSomething("SHOW", &ImageSelected, NULL);
	}
	else
	{
		HObject ImageSelected;
		if (m_hovec_Histroy_Image[1].O().CountObj() > 0)
		{
			SelectObj(m_hovec_Histroy_Image[1].O(), &ImageSelected, 1);
		}
		else
		{
			GenImageConst(&ImageSelected, "byte", 4096, 3000);
		}

		g_vec_FindMark[1]->DoSomething("SHOW", &ImageSelected, NULL);

	}


	///获取了间歇位置 


	int nLocationY = 0;
	g_vec_FindMark[g_tSystemSet.nCameraModel]->DoSomething("GETPARAM", "LocatePositionY", &nLocationY);
	g_Jugde[0].m_tStandardSize.hvQiTouXianChu = nLocationY;
	g_vec_Algorithm[0]->DoSomething("SETPARAM", "tSizeParam", &g_Jugde[0].m_tStandardSize);
	g_vec_Algorithm[1]->DoSomething("SETPARAM", "tSizeParam", &g_Jugde[0].m_tStandardSize);





}


//void CSeekersDlg::OnBnClickedBtnDrawrightmodel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//if (FALSE==g_CPowerLimit.IsEngineAndFactoryContinue())
//	//{
//	//	return ;
//	//}
//
//
//
//
//}


void CSeekersDlg::OnMenuTcpconnect()
{
	// TODO: 在此添加命令处理程序代码
	CDlgTcpConnect dlg;

	dlg.DoModal();

}


void CSeekersDlg::OnBnClickedBtnClearlabelcount()
{
	//	// TODO: 在此添加控件通知处理程序代码
	//	EnterCriticalSection(&g_CsClearLabelCount);
	//	for (int i=0;i<g_tSystemSet.nLaneCount;i++)
	//	{
	//		for(int j=0;j<g_Count.nType;j++)
	//		{
	//			g_LabelCauseCount.vec_LabelCause[i].at(0).vecErrorClassify[j]=0;
	//		}
	//		
	//	}
	//	for (int nRow=0;nRow<g_tSystemSet.nLaneCount;nRow++)
	//	{
	//		for (int nColumn=0;nColumn<g_Count.nType;nColumn++)//列数
	//		{
	//			CString strValue;
	//			strValue.Format("%d",g_LabelCauseCount.vec_LabelCause[nRow].at(0).vecErrorClassify[nColumn]);
	//			m_LST_LabelCount.SetItemText(nRow,nColumn+1,strValue);
	//		}
	//	}
	//LeaveCriticalSection(&g_CsClearLabelCount);
	//

}




void CSeekersDlg::OnBnClickedBtnCalibset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
	{
		return;
	}
	CDlgCalibResolution dlg;
	dlg.DoModal();
}


void CSeekersDlg::OnBnClickedBtnFindedage()
{


	// TODO: 在此添加控件通知处理程序代码
	HObject hoImageSelected1, hoImageSelected2;
	if (m_hovec_Histroy_Image[0].O().CountObj() > 0)
	{
		SelectObj(m_hovec_Histroy_Image[0].O(), &hoImageSelected1, m_hovec_Histroy_Image[0].O().CountObj());
	}
	else
	{
		GenImageConst(&hoImageSelected1, "byte", 4096, 3000);
	}
	if (m_hovec_Histroy_Image[1].O().CountObj() > 0)
	{
		SelectObj(m_hovec_Histroy_Image[1].O(), &hoImageSelected2, m_hovec_Histroy_Image[1].O().CountObj());
	}
	else
	{
		GenImageConst(&hoImageSelected2, "byte", 4096, 3000);
	}

	CDlgCreateBaseLine dlg;

	dlg.m_hoFirstImage = hoImageSelected1;
	dlg.m_hoSecondImage = hoImageSelected2;
	dlg.DoModal();



}
void CSeekersDlg::LoadVersionInfoList()//版本信息刷新
{

	CString strValue;
	int nItemCount;

	//格式:正式_主程序版本号.子版本号.修正版本号.日期
	g_StrCurrentVersion = "V01.01.2.220531";
	strValue = "V01.01.2.220406";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220406 删除多余缺陷类型，删除贴标机，修改算法模块";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220407";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220407 优化启动bug";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220508";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220508 联动尺寸和修改尺寸规格";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220512";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220512 规范尺寸判断A面为正面，B面为反面";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220512 优化换型过程";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220514";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220514 优化换型过程2.0";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220518";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220518 开放点检时间";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220524";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220524 尺寸数据报表优化";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220524 PDF报表优化";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220524 规格设置移到换型界面";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "V01.01.2.220531";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220531 解决切换配方右侧参数不刷新的问题";
	g_hvVertionInfoTuple.Append((HTuple)strValue);

	strValue = "";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
	strValue = " V01.01.2.220531 优化自动换型配置测量";
	g_hvVertionInfoTuple.Append((HTuple)strValue);
}

void CSeekersDlg::OnBnClickedBtnLightonoff()
{
	// TODO: 在此添加控件通知处理程序代码


	CString strBtnText;
	GetDlgItemTextA(IDC_BTN_LIGHTONOFF, strBtnText);
	if ("打开光源" == strBtnText)
	{

		strBtnText.Format("关闭光源");
		SetDlgItemTextA(IDC_BTN_LIGHTONOFF, strBtnText);

		g_pLightControl->DoSomething("OPENLIGHT", "ON", NULL);
		CString strValue;
		strValue = "执行打开光源按钮";
		g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);



	}
	else if ("关闭光源" == strBtnText)
	{


		if (IDOK == MessageBox(_T("确认需要关闭光源吗！"), _T("严重警告!"), MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
		{
			strBtnText.Format("打开光源");
			SetDlgItemTextA(IDC_BTN_LIGHTONOFF, strBtnText);

			g_pLightControl->DoSomething("CLOSELIGHT", "OFF", NULL);
			CString strValue;
			strValue = "执行关闭光源按钮";
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
		}
	}




}


void CSeekersDlg::OnBnClickedBtnCleardisk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_dRealSpeed > 0)
	{
		AfxMessageBox("速度值不为0,禁用手动清理硬盘");
		return;
	}
	g_pStorage->DoSomething("SETPARAM", "ClearDisk", NULL);



}


void CSeekersDlg::OnMenuGuigeshezhi()
{
	// TODO: 在此添加命令处理程序代码

	if (TRUE == g_Jugde[0].ShowParam())
	{


	}
}


void CSeekersDlg::OnMenuPlc()
{
	// TODO: 在此添加命令处理程序代码

	g_pCLabeller->DoSomething("SHOWLABELLERDLG", NULL, NULL);

}


void CSeekersDlg::OnBnClickedBtnDailyuse()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgDailyButton dlg;
	dlg.DoModal();



}
