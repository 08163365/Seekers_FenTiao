// DlgHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgHelp.h"
#include "afxdialogex.h"

#include "SeekersDlg.h"
// CDlgHelp 对话框

IMPLEMENT_DYNAMIC(CDlgHelp, CDialogEx)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHelp::IDD, pParent)
{

}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_staticVersion);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_LST_VERINFO, m_Lst_CVersionInfo);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgHelp::OnBnClickedBtnCancel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgHelp::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BTN_REFRESHVERSION, &CDlgHelp::OnBnClickedBtnRefreshversion)
END_MESSAGE_MAP()


// CDlgHelp 消息处理程序


void CDlgHelp::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgHelp::OnCancel();
}
CString CDlgHelp::GetFileVersion(CString strFileName)
{
	#pragma comment(lib, "Version.lib ")
	//获取当前文件的版本信息
	DWORD dwLen = GetFileVersionInfoSize(strFileName,NULL); 
	char *pszAppVersion = new char[dwLen+1];
	CString strVersion;
	if(pszAppVersion)
	{
		memset(pszAppVersion,0,sizeof(char)*(dwLen+1));
		GetFileVersionInfo(strFileName,NULL,dwLen,pszAppVersion);
		
		UINT nLen(0);
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		VerQueryValue(pszAppVersion,"\\",(LPVOID*)&pFileInfo,&nLen);
		if(pFileInfo)
		{
			//获取版本号
			strVersion.Format("%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strVersion;
}

void CDlgHelp::InitUI()
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
	m_staticVersion.SetFont("Microsoft YaHei", 20, FW_NORMAL);//
	m_staticVersion.SetTextColor(RGB(0,0,0));
	m_staticVersion.SetTransparent(TRUE);


	CString strMachineID,strSystemVis;
	strSystemVis.Format("%s\\Config\\System.vis",GetCurrentAppPath());
	GetPrivateProfileString("INFO","ID","未指定",strMachineID.GetBuffer(MAX_PATH),MAX_PATH,strSystemVis);
	strMachineID.ReleaseBuffer();
	

	CString strSeekers,strDetect,strSeparate,strCommunicate,strStorage,strAVT,strDS,strMER;
	strSeekers.Format("%s\\Seekers.exe",GetCurrentAppPath());
	strDetect.Format("%s\\SCIDetect.dll",GetCurrentAppPath());
	strSeparate.Format("%s\\SCISeparate.dll",GetCurrentAppPath());
	strCommunicate.Format("%s\\SCICommunicate.dll",GetCurrentAppPath());
	strStorage.Format("%s\\SCIStorage.dll",GetCurrentAppPath());
	strAVT.Format("%s\\SCIGrabberAVT.dll",GetCurrentAppPath());
	strDS.Format("%s\\SCIGrabberDS.dll",GetCurrentAppPath());
	strMER.Format("%s\\SCIGrabberMER.dll",GetCurrentAppPath());

	//m_staticVersion.Format(
	//	"   机台编号:%s\r\n\r\n"
	//	"   Seekers:%s\r\n\r\n"
 //       "   SCIDetect:%s\r\n\r\n"
 //       "   SCISeparate:%s\r\n\r\n"
	//	"   SCICommunicate:%s\r\n\r\n"
 //       "   SCIStorage:%s\r\n\r\n"
	//	"   SCIGrabberDS:%s\r\n\r\n"
	//	"   SCIGrabberMER:%s\r\n\r\n"
	//	"   SCIGrabberAVT:%s\r\n\r\n"
	//	"   公司:  Seekers\r\n"
	//	"   开发者: 新能源产品部\r\n"
	//	"   电话:  86-10-62969288\r\n"
	//	"   地址:  北京市海淀区上地七街\r\n",
	//	strMachineID,GetFileVersion(strSeekers),GetFileVersion(strDetect),
	//	GetFileVersion(strSeparate),GetFileVersion(strCommunicate),GetFileVersion(strStorage),
	//	GetFileVersion(strDS),GetFileVersion(strAVT),GetFileVersion(strMER));

	m_staticVersion.Format(
		"   Seekers:%s\r\n\r\n"
		"   公司:  北京大恒\r\n"
		"   开发者: 新能源产品部\r\n"
		"   电话:  86-10-62969288\r\n"
		"   地址:  北京市海淀区上地七街\r\n",
		g_StrCurrentVersion
		);

	m_Lst_CVersionInfo.DeleteAllColumns();
	m_Lst_CVersionInfo.DeleteAllItems();
	m_Lst_CVersionInfo.SetExtendedStyle(LVS_EX_GRIDLINES);
	int nWidth=150;
	m_Lst_CVersionInfo.InsertColumn(0,"版本号",LVCFMT_LEFT,nWidth);
	nWidth=2000;
    m_Lst_CVersionInfo.InsertColumn(1,"更新内容",LVCFMT_LEFT,nWidth);

	

//	CString strValue;
	int nItemCount;
	//
	//strValue="10, 64, 18, 523";
	//m_Lst_CVersionInfo.InsertItem(nItemCount,strValue);
	//strValue=" 10, 64, 18, 523 Seekers增加光源自适应模块和校准bug";
	//m_Lst_CVersionInfo.SetItemText(nItemCount,1,strValue);

	for (int i = 0; i < g_hvVertionInfoTuple.TupleLength(); i = i + 2)
	{
		nItemCount = m_Lst_CVersionInfo.GetItemCount();
		m_Lst_CVersionInfo.InsertItem(nItemCount, g_hvVertionInfoTuple[i].S());;
		m_Lst_CVersionInfo.SetItemText(nItemCount, 1, g_hvVertionInfoTuple[i + 1].S());
	}





}

CString CDlgHelp::GetDesignatedDiskFreeSpace()
{
	DWORD dwTotalDiskSpace,dwFreeDiskSpace,dwUsedDiskSpace;    

	ULARGE_INTEGER uiFreeBytesAvailableToCaller;
	ULARGE_INTEGER uiTotalNumberOfBytes;
	ULARGE_INTEGER uiTotalNumberOfFreeBytes;

	CString strPath;
	GetModuleFileName(NULL,strPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	strPath.ReleaseBuffer();
	int pos = strPath.Find('\\');
	strPath = strPath.Left(pos);
	CString strFreeDiskSpace;
	if(GetDiskFreeSpaceEx(strPath, &uiFreeBytesAvailableToCaller,
		&uiTotalNumberOfBytes,
		&uiTotalNumberOfFreeBytes))
	{
		dwTotalDiskSpace = (DWORD)(uiTotalNumberOfBytes.QuadPart / 1024 / 1024);
		dwFreeDiskSpace  = (DWORD)(uiFreeBytesAvailableToCaller.QuadPart >> 20);
		dwUsedDiskSpace     = dwTotalDiskSpace - dwFreeDiskSpace;
		TRACE("硬盘%s::总空间%dMB, 已用%dMB, 可用%dMB\n", strPath,
			dwTotalDiskSpace, dwUsedDiskSpace, dwFreeDiskSpace);
		float fds = (float)dwFreeDiskSpace / 1024;
		strFreeDiskSpace.Format("磁盘可用空间:%.1fGB",fds);
	}
	return strFreeDiskSpace;
}
BOOL CDlgHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgHelp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);

	/*GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	ScreenToClient(rcTitle);
	dc.MoveTo(rcTitle.left,rcTitle.bottom);
	dc.LineTo(rcTitle.right-1,rcTitle.bottom);
	CPen pen2;
	pen2.CreatePen(PS_SOLID,1,RGB(220,220,220));
	dc.SelectObject(pen2);
	dc.MoveTo(rcTitle.left,rcTitle.bottom+1);
	dc.LineTo(rcTitle.right-1,rcTitle.bottom+1);*/
}


BOOL CDlgHelp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	/*if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
		if (rcTitle.PtInRect(pMsg->pt))
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION,  pMsg->lParam);
		}
	}*/
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgHelp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgHelp::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
}


void CDlgHelp::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*90)/100,LWA_ALPHA);
	}else if (WA_CLICKACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}else if (WA_ACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
}


HBRUSH CDlgHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgHelp::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgHelp::OnBnClickedBtnRefreshversion()
{
	// TODO: 在此添加控件通知处理程序代码

	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//存储卷号避免异常丢失
	WritePrivateProfileString("INFO","SeekersVersion",g_StrCurrentVersion,g_strConfigSystemFile);

	CString strValue;
	strValue.Format("%s版本号更新成功!!!",g_StrCurrentVersion);
	AfxMessageBox(strValue);

}
