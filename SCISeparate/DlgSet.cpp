// DlgSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SCISeparate.h"
#include "DlgSet.h"
#include "afxdialogex.h"


// CDlgSet 对话框

IMPLEMENT_DYNAMIC(CDlgSet, CDialogEx)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSet::IDD, pParent)
	, m_COM_bPretreat(0)
	, m_EDT_nDiffColumn(0)
	, m_EDT_nLineNum(0)
	, m_EDT_nTreatSize(0)

	, m_COM_nFitRegionMode(0)

	, m_COM_nSelectValueMode(0)
{

	m_COM_nHcalib = 0;
	m_EDT_dLocateMissingOffset = 0.0;
	m_EDT_dAoBanMaxDeviation = 0.0;
	m_EDT_nXTreatSize = 0;
	m_EDT_nSelectNum = 0;
}

CDlgSet::~CDlgSet()
{
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_PREPROCESS, m_COM_bPretreat);
	DDX_Control(pDX, IDOK, m_BTN_OK);
	DDX_Control(pDX, IDCANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_CBIndex(pDX, IDC_COM_HCALIB, m_COM_nHcalib);
	DDX_Control(pDX, IDC_STC_CALIBWIDTH, m_STC_Hcalib);
	DDX_Text(pDX, IDC_EDT_DIFFCOLUMN, m_EDT_nDiffColumn);
	DDX_Control(pDX, IDC_STC_DIFFCOLUMN, m_STC_DiffColumn);
	DDX_Text(pDX, IDC_EDT_LINENUM, m_EDT_nLineNum);
	DDX_Text(pDX, IDC_EDT_LOCATEMISSINGOFFSET, m_EDT_dLocateMissingOffset);
	//  DDX_Text(pDX, IDC_EDT_MAXAOBANDEVIATION, m_nMaxAobanDevition);
	//  DDX_Check(pDX, IDC_CHK_USETHRESHAOBAN, m_CHK_bUseThreshAoBan);
	DDX_Text(pDX, IDC_EDT_MAXAOBANDEVIATION, m_EDT_dAoBanMaxDeviation);
	DDX_Text(pDX, IDC_EDT_TREATSIZE, m_EDT_nTreatSize);
	DDX_Text(pDX, IDC_EDT_XTREATSIZE, m_EDT_nXTreatSize);
	DDX_Text(pDX, IDC_EDT_SELECTNUM, m_EDT_nSelectNum);
	DDV_MinMaxInt(pDX, m_EDT_nSelectNum, 1, 10);
	DDX_Check(pDX, IDC_CHK_USEGRR, m_CHK_bUseGRR);
	//  DDX_Text(pDX, IDC_EDT_AOBANTHRESH, m_EDT_nAoBanThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nAoBanThresh, 0, 255);
	//  DDX_CBIndex(pDX, IDC_COM_AUTOFIND, m_COM_BEnableFindLastFoil);
	//  DDX_Text(pDX, IDC_EDT_EXTRACTFOILTHRESH, m_EDT_nFoilThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nFoilThresh, 0, 255);
	//  DDX_Text(pDX, IDC_EDT_REGIONWIDTH, m_EDT_dRegionWidthSet);
	DDX_CBIndex(pDX, IDC_COM_FITREGIONMODE, m_COM_nFitRegionMode);
	//  DDX_CBIndex(pDX, IDC_COM_ENABLEMONITOR, m_COM_bUseMonitor);
	//  DDX_CBIndex(pDX, IDC_COM_EXTRACTCOLOR, m_COM_bExtractColor);
	//  DDX_Text(pDX, IDC_EDIT3, m_EDT_nLeftExtractThresh);
	//  DDX_Text(pDX, IDC_EDIT8, m_EDT_dLeftWidthOffset);
	//  DDX_Text(pDX, IDC_EDIT9, m_EDT_dLeftHeightOffset);
	//  DDX_Text(pDX, IDC_EDIT7, m_EDT_dRightHeightOffset);
	//  DDX_Text(pDX, IDC_EDIT6, m_EDT_dRightWidthOffset);
	//  DDX_Text(pDX, IDC_EDIT5, m_EDT_nRightExtractThresh);
	DDX_CBIndex(pDX, IDC_COM_SELECTVALUEMODE, m_COM_nSelectValueMode);
}


BEGIN_MESSAGE_MAP(CDlgSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSet::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgSet 消息处理程序
void CDlgSet::SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
	COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG)
{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
}
void CDlgSet::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_STC_Hcalib.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STC_Hcalib.SetTransparent(TRUE);
	m_STC_DiffColumn.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STC_DiffColumn.SetTransparent(TRUE);
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	SetButtonColcor(m_BTN_OK,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_Cancel,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	m_BTN_OK.SetFont(&m_ftButton);
	m_BTN_Cancel.SetFont(&m_ftButton);
}


BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitUI();
	m_COM_bPretreat = m_pOwner->m_tParameters.bPretreat;
	m_EDT_nTreatSize = m_pOwner->m_tParameters.nSizePretreat;
	m_EDT_nXTreatSize=m_pOwner->m_tParameters.nXSizePretreat;
	m_CHK_bUseGRR=m_pOwner->m_tParameters.bUseGRR;
	m_EDT_nSelectNum=m_pOwner->m_tParameters.nSelectNum;
	m_COM_nHcalib=m_pOwner->m_tParameters.nHcalibFlag;
	m_COM_nFitRegionMode=m_pOwner->m_tParameters.nFitRegionMode;
	m_EDT_nDiffColumn=m_pOwner->m_tParameters.nMaxDiffColumn;
	m_EDT_nLineNum=m_pOwner->m_tLineMatch.nLineUsed;
	m_EDT_dLocateMissingOffset=m_pOwner->m_tParameters.dLocateMissingOffset;
	m_EDT_dAoBanMaxDeviation=m_pOwner->m_tParameters.dMaxAoBanDeviation;

	m_COM_nSelectValueMode=m_pOwner->m_tParameters.nSelectValueMode;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSet::OnBnClickedOk()
{
	if (UpdateData(TRUE))
	{
		m_pOwner->m_tParameters.bPretreat = m_COM_bPretreat;
		m_pOwner->m_tLineMatch.nLineUsed=m_EDT_nLineNum;
		m_pOwner->m_tParameters.nSizePretreat = m_EDT_nTreatSize;
		m_pOwner->m_tParameters.nXSizePretreat=m_EDT_nXTreatSize;

	    m_pOwner->m_tParameters.bUseGRR=m_CHK_bUseGRR;
	    m_pOwner->m_tParameters.nSelectNum=m_EDT_nSelectNum;
		
		m_pOwner->m_tParameters.nFitRegionMode=m_COM_nFitRegionMode;
		m_pOwner->m_tParameters.nHcalibFlag = m_COM_nHcalib;
		m_pOwner->m_tParameters.nMaxDiffColumn = m_EDT_nDiffColumn;
		m_pOwner->m_tParameters.dLocateMissingOffset=m_EDT_dLocateMissingOffset;
		m_pOwner->m_tParameters.dMaxAoBanDeviation=m_EDT_dAoBanMaxDeviation;

		m_pOwner->m_tParameters.nSelectValueMode=m_COM_nSelectValueMode;


		
	}
	CDialogEx::OnOK();
}


void CDlgSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	//GetClientRect(rcDlg);  
	dc.FillSolidRect(rcDlg,RGB(255,255,255));

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


BOOL CDlgSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

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


LRESULT CDlgSet::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		return HTCAPTION;
	}
	return CDialogEx::OnNcHitTest(point);
}
