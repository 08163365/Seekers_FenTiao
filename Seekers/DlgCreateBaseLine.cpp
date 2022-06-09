// DlgCreateBaseLine.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgCreateBaseLine.h"
#include "afxdialogex.h"


// CDlgCreateBaseLine 对话框

IMPLEMENT_DYNAMIC(CDlgCreateBaseLine, CDialogEx)

CDlgCreateBaseLine::CDlgCreateBaseLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCreateBaseLine::IDD, pParent)

{
	m_vecPages.clear();
}

CDlgCreateBaseLine::~CDlgCreateBaseLine()
{
}

void CDlgCreateBaseLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_TITLE, m_STC_CTitle);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_BTN_CReturn);
	DDX_Control(pDX, IDC_STC_GRAYVALUE, m_STC_CGrayValue);
	DDX_Control(pDX, IDC_TAB_SETTING, m_Tab_Set);
}


BEGIN_MESSAGE_MAP(CDlgCreateBaseLine, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgCreateBaseLine::OnBnClickedBtnCancel)
//	ON_BN_CLICKED(IDC_BTN_DRAWFIRSTMODEL, &CDlgCreateBaseLine::OnBnClickedBtnDrawfirstmodel)
//	ON_BN_CLICKED(IDC_BTN_DRAWSECONDMODEL, &CDlgCreateBaseLine::OnBnClickedBtnDrawsecondmodel)
//	ON_BN_CLICKED(IDC_BTN_ONEKEY, &CDlgCreateBaseLine::OnBnClickedBtnOnekey)
//	ON_BN_CLICKED(IDC_BTN_SETGRRPARAM, &CDlgCreateBaseLine::OnBnClickedBtnSetgrrparam)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgCreateBaseLine::OnBnClickedBtnSave)
//	ON_CBN_SELCHANGE(IDC_COM_CAMREAINDEX, &CDlgCreateBaseLine::OnCbnSelchangeComCamreaindex)
//	ON_BN_CLICKED(IDC_BTN_SPOTTEST, &CDlgCreateBaseLine::OnBnClickedBtnSpottest)
//	ON_BN_CLICKED(IDC_BTN_OPENTESTIMAGE, &CDlgCreateBaseLine::OnBnClickedBtnOpentestimage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING, &CDlgCreateBaseLine::OnSelchangeTabSetting)
END_MESSAGE_MAP()


// CDlgCreateBaseLine 消息处理程序



// Procedures 
// Local procedures 
void CDlgCreateBaseLine:: FindCoatRegion (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
	HTuple hv_nLowThresh,HTuple hv_nHighThresh)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_RegionOpening;
	HObject  ho_RegionClosing, ho_ConnectedRegions;

	// Local control variables
	HTuple  hv_HeightValue;

	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region, MIN(hv_nLowThresh,hv_nHighThresh),  MAX(hv_nLowThresh,hv_nHighThresh));
	ClosingRectangle1(ho_Region, &ho_RegionClosing, 1, 51);
	RegionFeatures(ho_RegionClosing, "height", &hv_HeightValue);
	OpeningRectangle1(ho_RegionClosing, &ho_RegionOpening, 1, hv_HeightValue/3);
	Connection(ho_RegionOpening, &ho_ConnectedRegions);
	SelectShapeStd(ho_ConnectedRegions, &(*ho_SelectedRegions), "max_area", 70);
	return;
}
void CDlgCreateBaseLine::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_BTN_font);
}
BOOL CDlgCreateBaseLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	m_BTN_font.CreatePointFont(100,"Microsoft YaHei");
	m_STC_CTitle.SetTextColor(RGB(255,255,255));
	m_STC_CTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	SetButtonColors(m_BTN_CReturn,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	m_BTN_CReturn.SetIcon(IDI_ICON_W_UNCHECK);
	CRect rcClient;
	GetDlgItem(IDC_STC_FIRST)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	m_Wnd_FirstImage.Create("", GetDlgItem(IDC_STC_FIRST)->GetStyle(), rcClient, this,IDC_STC_FIRST); 
	m_Wnd_FirstImage.DispImage(m_hoFirstImage);
	GetDlgItem(IDC_STC_SECOND)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	m_Wnd_SecondImage.Create("", GetDlgItem(IDC_STC_SECOND)->GetStyle(), rcClient, this,IDC_STC_SECOND); 
	m_Wnd_SecondImage.DispImage(m_hoSecondImage);

	SetDraw(m_Wnd_FirstImage.GetDispWnd(),"margin");
	SetDraw(m_Wnd_SecondImage.GetDispWnd(),"margin");


	//

	m_Tab_Set.SetItemSize(CSize(80,20));
	//m_Tab_Set.InsertItem(0,  "建模设置");
	m_Tab_Set.InsertItem(0,  "点检设置");
	m_Tab_Set.InsertItem(1,  "GRR设置" );


	//m_Tab_Set.SetFont("Microsoft Yahei",19,0);

	CRect rcSubDlg,rcTabItem,rcSource;
	GetDlgItem(IDC_TAB_SETTING)->GetWindowRect(&rcSubDlg);

   
	rcSubDlg.top=rcSubDlg.top+30;
	//m_pPageBaseLine = new CPageBaseLine;
	//m_pPageBaseLine->Create(CPageBaseLine::IDD,this);
	//m_pPageBaseLine->m_pParentDlg=this;
	//m_pPageBaseLine->GetClientRect(&rcSource);


	//m_pPageBaseLine->MoveWindow(rcSubDlg);
	//m_pPageBaseLine->ShowWindow(SW_NORMAL);
	//m_pPageBaseLine->ResetSize(rcSource);
	//m_vecPages.push_back(m_pPageBaseLine);


	m_pPageSpotDetectSet = new CPageSpotDetectSet;
	m_pPageSpotDetectSet->Create(CPageSpotDetectSet::IDD,this);
	m_pPageSpotDetectSet->m_pParentDlg=this;
	m_pPageSpotDetectSet->MoveWindow(rcSubDlg);
	m_pPageSpotDetectSet->ShowWindow(SW_NORMAL);
	m_vecPages.push_back(m_pPageSpotDetectSet);

	

	m_pPageGRRSet = new CPageGRRSet;
	m_pPageGRRSet->Create(CPageGRRSet::IDD,this);
	m_pPageGRRSet->m_pParentDlg=this;
	m_pPageGRRSet->MoveWindow(rcSubDlg);
	m_pPageGRRSet->ShowWindow(SW_HIDE);
	m_vecPages.push_back(m_pPageGRRSet);
	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCreateBaseLine::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


//void CDlgCreateBaseLine::OnBnClickedBtnDrawfirstmodel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	try
//	{
//		UpdateData(TRUE);
//		m_Wnd_FirstImage.m_bDrawing=TRUE;
//		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
//		HObject hoROI,hoSelectRegion;
//		m_Wnd_FirstImage.DispImage(m_hoFirstImage);
//		DrawRectangle1(m_Wnd_FirstImage.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//		GenRectangle1(&hoROI,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//		FindCoatRegion(m_hoFirstImage,hoROI,&hoSelectRegion,m_EDT_nFirstCoatLowThresh,m_EDT_nFirstCoatHighThresh);
//		if (hoSelectRegion.CountObj()>0)
//		{
//			m_Wnd_FirstImage.DispImage(m_hoFirstImage);
//			m_Wnd_FirstImage.DispRegion(hoSelectRegion,"yellow","margin",2,TRUE);
//			HTuple hv_LeftColumn,hv_RightColumn,hv_LeftRow,hv_RightRow;
//			SmallestRectangle1(hoSelectRegion,&hv_LeftRow,&hv_LeftColumn,&hv_RightRow,&hv_RightColumn);
//			SetTposition(m_Wnd_FirstImage.GetDispWnd(),50,50);
//			WriteString(m_Wnd_FirstImage.GetDispWnd(),"寻边成功");
//			g_tCreateBaseLineInfo.nFirstLeftPosition=hv_LeftColumn;
//			g_tCreateBaseLineInfo.nFirstRightPosition=hv_RightColumn;
//			g_tCreateBaseLineInfo.nFirstLeftPositionRow=hv_LeftRow;
//			g_tCreateBaseLineInfo.nFirstRightPositionRow=hv_RightRow;
//		}
//
//		m_Wnd_FirstImage.m_bDrawing=FALSE;
//	}catch(HException& e)
//	{
//		
//	}
//
//
//
//
//
//
//
//
//}










void CDlgCreateBaseLine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STC_TITLE)->GetClientRect(rcTitle); 
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


HBRUSH CDlgCreateBaseLine::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDlgCreateBaseLine::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{


		CPoint   pt;
		GetCursorPos(&pt);
		GetDlgItem(IDC_STC_FIRST)->ScreenToClient(&pt);
		CRect WndRect;
		GetDlgItem(IDC_STC_FIRST)->GetClientRect(&WndRect);

		// 还原显示（不缩放）
		if (WndRect.PtInRect(pt))
		{
			try
			{
				HTuple Row,Column,Value,Area,Circularity;
				GetMposition(m_Wnd_FirstImage.GetDispWnd(),&Row,&Column,NULL);
				GetGrayval(m_hoFirstImage,Row,Column,&Value);
				CString strDisp;
				strDisp.Format("坐标:  X:%3d Y:%3d\r\n \r\n灰度值: %3d",Column[0].I(),Row[0].I(),Value[0].I());
				m_STC_CGrayValue.SetWindowTextA(strDisp);
				return TRUE;
			}catch(HException e)
			{
				CString strDisp;
				strDisp.Format("坐标:  X:-- Y:--\r\n \r\n灰度值: --");
				m_STC_CGrayValue.SetWindowTextA(strDisp);
			}

		}else
		{
			CPoint   pt;
			GetCursorPos(&pt);
			GetDlgItem(IDC_STC_SECOND)->ScreenToClient(&pt);
			CRect WndRect;
			GetDlgItem(IDC_STC_SECOND)->GetClientRect(&WndRect);
			if (WndRect.PtInRect(pt))
			{
				try
				{
					HTuple Row,Column,Value,Area,Circularity;
					GetMposition(m_Wnd_SecondImage.GetDispWnd(),&Row,&Column,NULL);
					GetGrayval(m_hoSecondImage,Row,Column,&Value);
					CString strDisp;
					strDisp.Format("坐标:  X:%3d Y:%3d\r\n \r\n灰度值: %3d",Column[0].I(),Row[0].I(),Value[0].I());
					m_STC_CGrayValue.SetWindowTextA(strDisp);
					return TRUE;
				}catch(HException e)
				{
					CString strDisp;
					strDisp.Format("坐标:  X:-- Y:--\r\n \r\n灰度值: --");
					m_STC_CGrayValue.SetWindowTextA(strDisp);
				}
			}
		}
	}
		

	return CDialogEx::PreTranslateMessage(pMsg);
}




void CDlgCreateBaseLine::OnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
    SwitchDispPage(m_Tab_Set.GetCurSel());
}


void CDlgCreateBaseLine::SwitchDispPage(const int& nSN)
{
	if (nSN >= m_vecPages.size())return;

	for (int i = 0; i < m_vecPages.size(); i++)
	{
		if (nSN>0)
		{
			if (FALSE == g_CPowerLimit.IsEngineAndFactoryContinue())
				break;
		}
		if (nSN == i)
			m_vecPages[i]->ShowWindow(SW_NORMAL);
		else
			m_vecPages[i]->ShowWindow(SW_HIDE);
	}
}