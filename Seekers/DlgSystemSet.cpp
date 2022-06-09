// DlgSystemSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgSystemSet.h"
#include "afxdialogex.h"


// CDlgSystemSet 对话框

IMPLEMENT_DYNAMIC(CDlgSystemSet, CDialogEx)

CDlgSystemSet::CDlgSystemSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSystemSet::IDD, pParent)
	, m_nHistoryType(0)
	, m_nHistoryCount(0)
	, m_CHECK_bTileImage(FALSE)
	, m_CHECK_bSaveTitleImage(FALSE)
	, m_EDT_nDistanceCamera(0)
	, m_EDT_nOverTimeExit(0)
	, m_COM_nCameraModel(0)
	, m_CHK_bEnableReLoadParam(FALSE)
	, m_CHK_bPopWindowToWarn(FALSE)
	, m_EDT_nMaxPopWindowNum(0)
	, m_EDT_nOverTimeFrames(0)
	, m_EDT_nTimeLimitValue(0)
{


	m_EDT_nOverTimeLimitLoginOff = 0;
}

CDlgSystemSet::~CDlgSystemSet()
{
}

void CDlgSystemSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLUMNRESOLUTION0, m_cstaticColumnResolution0);
	DDX_Control(pDX, IDC_STATIC_COLUMNRESOLUTION1, m_cstaticColumnResolution1);
	DDX_Control(pDX, IDC_STATIC_ROWRESOLUTION0, m_cstaticRowResolution0);
	DDX_Control(pDX, IDC_STATIC_ROWRESOLUTION1, m_cstaticRowResolution1);
	DDX_CBIndex(pDX, IDC_COMBO_HISTORY_TYPE, m_nHistoryType);
	DDX_Text(pDX, IDC_EDIT_HISTORY_COUNT, m_nHistoryCount);
	DDV_MinMaxInt(pDX, m_nHistoryCount, 1, 30);
	DDX_Control(pDX, IDC_EDIT_HISTORY_COUNT, m_EDIT_historyCount);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_BTN_cancel);
	DDX_Control(pDX, IDC_BUTTON_OK, m_BTN_ok);
	DDX_Check(pDX, IDC_CHECK_TILEIMAGE, m_CHECK_bTileImage);
	DDX_Check(pDX, IDC_CHECK_SAVETILEIMAGE_BEFORE, m_CHECK_bSaveTitleImage);
	DDX_Text(pDX, IDC_EDIT_DISTANCECAMERS, m_EDT_nDistanceCamera);
	DDX_Control(pDX, IDC_EDIT_DISTANCECAMERS, m_EDT_DistanceCamera);
	DDX_Text(pDX, IDC_EDT_OVERTIMEEXIT, m_EDT_nOverTimeExit);
	DDX_Control(pDX, IDC_EDT_OVERTIMEEXIT, m_EDT_COverTimeExit);
	DDX_CBIndex(pDX, IDC_COM_MODELCAMERA, m_COM_nCameraModel);
	DDX_Check(pDX, IDC_CHK_RELOADPARAMBYBATCH, m_CHK_bEnableReLoadParam);
	DDX_Check(pDX, IDC_CHK_POPWINDOWTOWARN, m_CHK_bPopWindowToWarn);
	DDX_Text(pDX, IDC_EDT_MAXNUMLIMIT, m_EDT_nMaxPopWindowNum);
	DDX_Check(pDX, IDC_CHECK_MERGEPARAM, m_CHK_bEnableMergeParam);
	DDX_Check(pDX, IDC_CHECK_MERGERESULT, m_CHK_bEnableMergeResult);
	DDX_Text(pDX, IDC_EDT_OVERTIMEFRAMES, m_EDT_nOverTimeFrames);
	DDX_Text(pDX, IDC_EDT_OVERTIMEVALUE, m_EDT_nTimeLimitValue);
	DDX_Text(pDX, IDC_EDT_LOGIOFFTIMELIMIT, m_EDT_nOverTimeLimitLoginOff);
}


BEGIN_MESSAGE_MAP(CDlgSystemSet, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgSystemSet::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgSystemSet::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgSystemSet::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgSystemSet 消息处理程序

BOOL CDlgSystemSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nHistoryCount = g_tSystemSet.nHistoryCount;
	m_nHistoryType = g_tSystemSet.nHistoryType;

	m_CHECK_bTileImage = g_tSystemSet.bTileImage;
	m_CHECK_bSaveTitleImage = g_tSystemSet.bSaveTileImage;
	m_EDT_nDistanceCamera = g_tSystemSet.nDistanceCamera;
	
	m_EDT_nOverTimeExit=g_tSystemSet.nOverTimeExit;
	m_COM_nCameraModel=g_tSystemSet.nCameraModel;
    m_CHK_bEnableReLoadParam=g_tSystemSet.bEnableReloadParam;

	m_CHK_bPopWindowToWarn=g_tSystemSet.bEnablePopWindowToWarn;
	m_EDT_nMaxPopWindowNum=g_tSystemSet.nMaxPopWindowNum;


	m_CHK_bEnableMergeResult=g_tSystemSet.nMergeResults;
	m_CHK_bEnableMergeParam=g_tSystemSet.nMergeParam ;
	
	m_EDT_nOverTimeFrames=g_tSystemSet.nOverTimeFrameValue;
	m_EDT_nTimeLimitValue=g_tSystemSet.nOverTimeValue;

	m_EDT_nOverTimeLimitLoginOff=g_tSystemSet.nOverTimeLimitLoginOff;

	InitStatic();
	//((CButton*)GetDlgItem(IDC_CHECK_MERGERESULT))->SetCheck(g_tSystemSet.nMergeResults);
	//((CButton*)GetDlgItem(IDC_CHECK_MERGEPARAM))->SetCheck(g_tSystemSet.nMergeParam);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}





void CDlgSystemSet::InitStatic()
{
	m_FontButton.CreatePointFont(100,"Microsoft YaHei");

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;


	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);


	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);

	m_BTN_ok.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	
	m_BTN_cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	



	m_BTN_ok.SetFont(&m_FontButton);
	m_BTN_cancel.SetFont(&m_FontButton);

	m_cstaticColumnResolution0.SetTextColor(RGB(0,255,0));
	m_cstaticColumnResolution1.SetTextColor(RGB(0,255,0));
	m_cstaticRowResolution0.SetTextColor(RGB(0,255,0));
	m_cstaticRowResolution1.SetTextColor(RGB(0,255,0));

	m_cstaticColumnResolution0.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticColumnResolution1.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticRowResolution0.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticRowResolution1.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);

	
	m_FontEdit.CreateFontA(20,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"华文细黑");
	GetDlgItem(IDC_EDIT_HISTORY_COUNT)->SetFont(&m_FontEdit);

	m_FontCombo.CreateFontA(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"华文细黑");
	GetDlgItem(IDC_COMBO_HISTORY_TYPE)->SetFont(&m_FontCombo);


	try
	{

		{
			m_cstaticColumnResolution0.Format("%.5f",g_vec_ColumnResolution[0]);
			m_cstaticColumnResolution1.Format("%.5f",g_vec_ColumnResolution[1]);
			m_cstaticRowResolution0.Format("%.5f",g_vec_RowResolution[0]);
			m_cstaticRowResolution1.Format("%.5f",g_vec_RowResolution[1]);
		}


	}catch(std::out_of_range)
	{

	}
	m_EDIT_historyCount.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_historyCount.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);



	m_EDT_DistanceCamera.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_DistanceCamera.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);

	
	


	m_EDT_COverTimeExit.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_COverTimeExit.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
	

}

void CDlgSystemSet::ComPareChange(int LastValue,int  CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("系统设置参数%s由%d更改为%d",strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}
BOOL CDlgSystemSet::Save()
{
	if (UpdateData(TRUE))
	{
		ComPareChange(g_tSystemSet.nHistoryCount,m_nHistoryCount,"缓存图像数目");
		ComPareChange(g_tSystemSet.nHistoryType,m_nHistoryType,"缓存图像类型");
		ComPareChange(g_tSystemSet.bTileImage,m_CHECK_bTileImage,"拼图使能");
		ComPareChange(g_tSystemSet.bSaveTileImage,m_CHECK_bSaveTitleImage,"报存拼图前图像使能");
		ComPareChange(g_tSystemSet.nDistanceCamera,m_EDT_nDistanceCamera,"前后相机像素距离");
		ComPareChange(g_tSystemSet.nCameraModel,m_COM_nCameraModel,"定位相机");
		ComPareChange(g_tSystemSet.bEnableReloadParam,m_CHK_bEnableReLoadParam,"按卷还原使能");
		
		ComPareChange(g_tSystemSet.bEnablePopWindowToWarn,m_CHK_bPopWindowToWarn,"弹窗使能");
		ComPareChange(g_tSystemSet.nMaxPopWindowNum,m_EDT_nMaxPopWindowNum,"最大弹窗限制");
		ComPareChange(g_tSystemSet.nMergeResults,m_CHK_bEnableMergeResult,"表面检测使用相同参数使能");
		ComPareChange(g_tSystemSet.nMergeParam,m_CHK_bEnableMergeParam,"尺寸检测使用相同参数使能");
		ComPareChange(g_tSystemSet.nOverTimeExit,m_EDT_nOverTimeExit,"超时光源关闭设置");

		ComPareChange(g_tSystemSet.nOverTimeFrameValue,m_EDT_nOverTimeFrames,"连续超时帧数");
		ComPareChange(g_tSystemSet.nOverTimeValue,m_EDT_nTimeLimitValue,"超时设置");

		ComPareChange(g_tSystemSet.nOverTimeLimitLoginOff,m_EDT_nOverTimeLimitLoginOff,"登陆超时退出设置");

	
		


		g_tSystemSet.nHistoryCount   = m_nHistoryCount;
		g_tSystemSet.nHistoryType    = m_nHistoryType;
		
		g_tSystemSet.bTileImage      = m_CHECK_bTileImage;
		g_tSystemSet.bSaveTileImage  = m_CHECK_bSaveTitleImage;
		g_tSystemSet.nDistanceCamera = m_EDT_nDistanceCamera;
	
		g_tSystemSet.nOverTimeExit=m_EDT_nOverTimeExit;
		g_tSystemSet.nCameraModel=m_COM_nCameraModel;
		g_tSystemSet.bEnableReloadParam=m_CHK_bEnableReLoadParam;
	
		g_tSystemSet.bEnablePopWindowToWarn=m_CHK_bPopWindowToWarn;
		g_tSystemSet.nMaxPopWindowNum=m_EDT_nMaxPopWindowNum;
		g_tSystemSet.nMergeResults = m_CHK_bEnableMergeResult;
		g_tSystemSet.nMergeParam = m_CHK_bEnableMergeParam;

		g_tSystemSet.nOverTimeFrameValue=m_EDT_nOverTimeFrames;
		g_tSystemSet.nOverTimeValue=m_EDT_nTimeLimitValue;

		g_tSystemSet.nOverTimeLimitLoginOff=m_EDT_nOverTimeLimitLoginOff;


		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
		WritePrivateProfileInt("System","MergeResults",g_tSystemSet.nMergeResults,g_strConfigSystemFile);//
		WritePrivateProfileInt("System","MergeParam",g_tSystemSet.nMergeParam,g_strConfigSystemFile);//写入历史回看类型
		WritePrivateProfileInt("System","HistoryCount",g_tSystemSet.nHistoryCount,g_strConfigSystemFile);//写入历史回看个数
		WritePrivateProfileInt("System","HistoryType",g_tSystemSet.nHistoryType,g_strConfigSystemFile);//写入历史回看类型
		WritePrivateProfileInt("System","bEnableReloadParam",g_tSystemSet.bEnableReloadParam ,g_strConfigSystemFile);
		WritePrivateProfileInt("System","nOverTimeLimitLoginOff",g_tSystemSet.nOverTimeLimitLoginOff ,g_strConfigSystemFile);


		WritePrivateProfileInt("System","bEnablePopWindowToWarn",g_tSystemSet.bEnablePopWindowToWarn ,g_strConfigSystemFile);


		WritePrivateProfileInt("System","nMaxPopWindowNum",g_tSystemSet.nMaxPopWindowNum ,g_strConfigSystemFile);

		
		WritePrivateProfileInt("System","nOverTimeExit",g_tSystemSet.nOverTimeExit,g_strConfigSystemFile);//超时退出

		WritePrivateProfileInt("System","TileImage",g_tSystemSet.bTileImage,g_strConfigSystemFile);//写入历史回看个数
		WritePrivateProfileInt("System","DistanceCamera",g_tSystemSet.nDistanceCamera,g_strConfigSystemFile);//写入历史回看类型
		WritePrivateProfileInt("System","nCameraModel",g_tSystemSet.nCameraModel,g_strConfigSystemFile);//写入历史回看类型

		WritePrivateProfileInt("System","nOverTimeFrameValue",g_tSystemSet.nOverTimeFrameValue ,g_strConfigSystemFile);
		WritePrivateProfileInt("System","nOverTimeValue",g_tSystemSet.nOverTimeValue ,g_strConfigSystemFile);


		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
		
		
		CDialogEx::OnOK();
		return TRUE;
	}else
	{
		return FALSE;
	}


}


void CDlgSystemSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
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
}


HBRUSH CDlgSystemSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDlgSystemSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CMFCPropertyPage::OnEraseBkgnd(pDC);
}


void CDlgSystemSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*85)/100,LWA_ALPHA);
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


LRESULT CDlgSystemSet::OnNcHitTest(CPoint point)
{
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);

	//return CDialogEx::OnNcHitTest(point);
}


BOOL CDlgSystemSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//屏蔽回车
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)//屏蔽ESC
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSystemSet::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgSystemSet::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	Save();
	CDialogEx::OnOK();
}


void CDlgSystemSet::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

