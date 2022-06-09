// DlgParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SCIStorage.h"
#include "DlgParamSet.h"
#include "afxdialogex.h"


// CDlgParamSet 对话框

IMPLEMENT_DYNAMIC(CDlgParamSet, CDialogEx)

CDlgParamSet::CDlgParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamSet::IDD, pParent)
	, m_EDT_nCacheDays(0)
	, m_CHK_bSaveInLocation(FALSE)
	, m_CHK_bSaveMarkImage(FALSE)
	, m_COM_nImageType(0)
	, m_MFCBROWSE_StrImageSavePath(_T(""))
	, m_EDT_StrMachineSN(_T(""))
	, m_CHK_bEnableUpLoadImage(FALSE)
	, m_EDT_StrUpLoadImagePath(_T(""))
{
	m_pOwner = NULL;
	//  m_EDT_StrSavePath = _T("");
	m_COM_StrImageFormat = _T("");
	m_CHK_bSaveAllImage = 0;
}

CDlgParamSet::~CDlgParamSet()
{
}

void CDlgParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WRITE_TIME, m_checkWriteTime);
	//  DDX_Control(pDX, IDC_CHECK_DISK_IMAGE, m_checkDiskImage);
	DDX_Control(pDX, IDC_CHECK_DISK_ERRORREGION, m_checkDiskErrorRegion);
	DDX_Control(pDX, IDC_CHECK_DISK_SIZE, m_checkDiskSize);
	DDX_Control(pDX, IDC_CHECK_DISK_DELETEOK, m_checkDiskDeleteOK);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDOK, m_BTN_OK);
	//  DDX_Text(pDX, IDC_EDT_DISKVOLUM, m_EDT_nDiskRemainVolume);
	//  DDV_MinMaxInt(pDX, m_EDT_nDiskRemainVolume, 1, 1000);
	DDX_Text(pDX, IDC_EDT_CACHEDAYS, m_EDT_nCacheDays);
	//  DDX_Text(pDX, IDC_EDT_SAVEPATH, m_EDT_StrSavePath);
	DDX_Control(pDX, IDC_CHECK_DISK_PROCESSINFO, m_checkDiskProcessInfo);
	DDX_Control(pDX, IDC_CHEK_DISK_SMALLIMAGE, m_checkDiskSmallImage);
	DDX_CBString(pDX, IDC_COM_IMAGEFORMAT, m_COM_StrImageFormat);
	DDX_Check(pDX, IDC_CHK_SAVEALLIMAGE, m_CHK_bSaveAllImage);
	DDX_Control(pDX, IDC_CHK_SAVEALLIMAGE, m_CHK_SaveAllImage);
	DDX_Check(pDX, IDC_CHK_SAVELOCATION, m_CHK_bSaveInLocation);
	DDX_Control(pDX, IDC_CHK_SAVELOCATION, m_CHK_CSaveLocation);
	DDX_Check(pDX, IDC_CHK_SAVEMARKIMAGE, m_CHK_bSaveMarkImage);
	DDX_Control(pDX, IDC_CHK_SAVEMARKIMAGE, m_CHK_CSaveMarkImage);
	DDX_CBIndex(pDX, IDC_COM_OKNG, m_COM_nImageType);
	DDX_Text(pDX, IDC_MFCBROWSE_IMAGESAVEPATH, m_MFCBROWSE_StrImageSavePath);
	//  DDX_Control(pDX, IDC_CHECK_CHOICEPATH, m_checDefinedPath);
	DDX_Text(pDX, IDC_EDT_MACHINESN, m_EDT_StrMachineSN);
	DDX_Check(pDX, IDC_CHK_ENABLEUPLOADIMAGE, m_CHK_bEnableUpLoadImage);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_UPLOADIMAGEPATH, m_EDT_StrUpLoadImagePath);
}


BEGIN_MESSAGE_MAP(CDlgParamSet, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CDlgParamSet::OnBnClickedOk)
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
//	ON_BN_CLICKED(IDC_CHECK_CHOICEPATH, &CDlgParamSet::OnBnClickedCheckChoicepath)
END_MESSAGE_MAP()


// CDlgParamSet 消息处理程序
void CDlgParamSet::InitCheck()
{
	COLORREF MainColor = m_tBlendent.m_Blendent.Dlg.MainColor;

	
	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_checkWriteTime.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_checkDiskProcessInfo.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_checkDiskErrorRegion.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_checkDiskSize.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_checkDiskDeleteOK.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_checkDiskSmallImage.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);

	m_CHK_SaveAllImage.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_CHK_CSaveLocation.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);
	m_CHK_CSaveMarkImage.SetIcon(IDI_ICON_W_CHECK, IDI_ICON_W_UNCHECK);

	short	shBtnColor = m_tBlendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = m_tBlendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = m_tBlendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = m_tBlendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = m_tBlendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_checkWriteTime.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkWriteTime.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_checkDiskProcessInfo.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkDiskProcessInfo.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_checkDiskSmallImage.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkDiskSmallImage.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);




	m_checkDiskErrorRegion.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkDiskErrorRegion.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_checkDiskSize.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkDiskSize.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_CHK_SaveAllImage.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_CHK_SaveAllImage.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	m_checkDiskDeleteOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_checkDiskDeleteOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	m_CHK_CSaveLocation.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_CHK_CSaveLocation.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	
	m_CHK_CSaveMarkImage.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_CHK_CSaveMarkImage.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	m_BTN_OK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_OK.SetIcon(IDI_ICON_W_UNCHECK);

	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


}
void CDlgParamSet::OnPaint()
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

BOOL CDlgParamSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BOOL bWriteLog = FALSE;
	m_pOwner->m_tInit.pLogView->DoSomething("GETPARAM","WRITELOG",&bWriteLog);

	m_checkWriteTime.SetCheck(bWriteLog);
	m_checkDiskProcessInfo.SetCheck(m_pOwner->m_tWriteDisk.bSaveProcessInfo);
	m_checkDiskSmallImage.SetCheck(m_pOwner->m_tWriteDisk.bSmallImage);
	m_checkDiskErrorRegion.SetCheck(m_pOwner->m_tWriteDisk.bErrorRegion);
	m_checkDiskSize.SetCheck(m_pOwner->m_tWriteDisk.bSize);
	m_checkDiskDeleteOK.SetCheck(m_pOwner->m_tWriteDisk.bDeleteOK);

	m_EDT_nCacheDays=m_pOwner->m_tWriteDisk.nSaveDays;
	m_CHK_bSaveAllImage=m_pOwner->m_tWriteDisk.bSaveAllGrabberImage;
	m_COM_StrImageFormat=m_pOwner->m_tWriteDisk.strImageFormat;
	m_CHK_bSaveInLocation=m_pOwner->m_tWriteDisk.bWriteInLocation;
    m_CHK_bSaveMarkImage=m_pOwner->m_tWriteDisk.bSaveMarkImage;
	m_COM_nImageType=m_pOwner->m_tWriteDisk.nSaveImageType;
	m_MFCBROWSE_StrImageSavePath=m_pOwner->m_tWriteDisk.StrImageSavePath;
	m_EDT_StrMachineSN=m_pOwner->m_tWriteDisk.StrMachineSN;

	m_EDT_StrUpLoadImagePath=m_pOwner->m_tWriteDisk.StrUpLoadImagePath;
	m_CHK_bEnableUpLoadImage=m_pOwner->m_tWriteDisk.bEnableUpLoadImage;


	UpdateData(FALSE);
	InitCheck();
	return TRUE;  // return TRUE unless you set the focus to a control
	
}


void CDlgParamSet::OnBnClickedOk()
{
	UpdateData(TRUE);
	BOOL bWriteLog = m_checkWriteTime.GetCheck();
	m_pOwner->m_tInit.pLogView->DoSomething("SETPARAM","WRITELOG",&bWriteLog);//获取是否写入时间日志
	m_pOwner->m_tWriteDisk.bSaveProcessInfo       =  m_checkDiskProcessInfo.GetCheck();//是否写入过程信息
	m_pOwner->m_tWriteDisk.bSmallImage       =  m_checkDiskSmallImage.GetCheck();//是否写入小图
	m_pOwner->m_tWriteDisk.bErrorRegion = m_checkDiskErrorRegion.GetCheck();//是否写入错误区域
	m_pOwner->m_tWriteDisk.bSize        = m_checkDiskSize.GetCheck();//是否写入尺寸
	m_pOwner->m_tWriteDisk.bDeleteOK    = m_checkDiskDeleteOK.GetCheck();//是否删除正常片图片

	m_pOwner->m_tWriteDisk.nSaveDays=m_EDT_nCacheDays;
	m_pOwner->m_tWriteDisk.bSaveAllGrabberImage=m_CHK_bSaveAllImage;
	m_pOwner->m_tWriteDisk.strImageFormat=m_COM_StrImageFormat;
	m_pOwner->m_tWriteDisk.bWriteInLocation=m_CHK_bSaveInLocation;
	m_pOwner->m_tWriteDisk.bSaveMarkImage=m_CHK_bSaveMarkImage;
	m_pOwner->m_tWriteDisk.nSaveImageType=m_COM_nImageType;
	m_pOwner->m_tWriteDisk.StrImageSavePath=m_MFCBROWSE_StrImageSavePath;
	m_pOwner->m_tWriteDisk.StrMachineSN=m_EDT_StrMachineSN;

	m_pOwner->m_tWriteDisk.StrUpLoadImagePath=m_EDT_StrUpLoadImagePath;
	m_pOwner->m_tWriteDisk.bEnableUpLoadImage=m_CHK_bEnableUpLoadImage;



	

	m_pOwner->WriteParam();
	CDialogEx::OnOK();
}


BOOL CDlgParamSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgParamSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
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


LRESULT CDlgParamSet::OnNcHitTest(CPoint point)
{
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		return HTCAPTION;
	}

	return CDialogEx::OnNcHitTest(point);
}


BOOL CDlgParamSet::PreTranslateMessage(MSG* pMsg)
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


//void CDlgParamSet::OnBnClickedCheckChoicepath()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//UpdateData(TRUE);
//	//if (m_checDefinedPath.GetCheck() && IDOK == MessageBox(_T("确认自定义路径吗？可能会误删除图片，请确认路径！"), _T("警告!"), MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
//	//{
//
//	//}
//	//else
//	//{
//
//	//}
//	//UpdateData(FALSE);
//
//}
