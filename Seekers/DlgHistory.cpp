// DlgHistory.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgHistory.h"
#include "afxdialogex.h"


// CDlgHistory 对话框

IMPLEMENT_DYNAMIC(CDlgHistory, CDialogEx)

CDlgHistory::CDlgHistory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHistory::IDD, pParent)
{

}

CDlgHistory::~CDlgHistory()
{
}

void CDlgHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IMAGE, m_LC_Image);
	DDX_Control(pDX, IDC_BTN_DETECT, m_BTN_Detect);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BTN_Exit);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgHistory, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DETECT, &CDlgHistory::OnBnClickedBtnDetect)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgHistory::OnBnClickedBtnExit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMAGE, &CDlgHistory::OnNMClickListImage)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgHistory::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgHistory 消息处理程序


void CDlgHistory::OnBnClickedBtnDetect()
{
	DETECTINPUT tDetectInput;
	tDetectInput.hoDetectImage = m_hoDispImage;
	tDetectInput.nBreakType    = 0;
	
	 
	if (TRUE != g_vec_Algorithm[m_nGrabberSN]->DoSomething("SHOW",&tDetectInput,NULL))
	{
		CString strException;
		strException.Format("算法%d调用显示界面异常,请排查原因",m_nGrabberSN);
		AfxMessageBox(strException);
		return;
	}
}


void CDlgHistory::OnBnClickedBtnExit()
{
	CDialogEx::OnCancel();
}
void CDlgHistory::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_fontList.CreatePointFont(90,"Microsoft YaHei");

	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	m_BTN_Detect.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Detect.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_Detect.SetFont(&m_ftButton);
	
	m_BTN_Exit.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Exit.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_Exit.SetFont(&m_ftButton);
	
	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	//m_BTN_close.SetFont(&m_ftButton);
}
void CDlgHistory::InitListCtrl()
{
	m_LC_Image.SetFont(&m_fontList);
	m_LC_Image.SetColorHeader(TRUE);
	m_LC_Image.SetHeaderHeight(1.5);
	m_LC_Image.SetHeaderBKColor(255,255,255,0);
	m_LC_Image.DeleteAllItems();
	try
	{
		for (int i=0;i<m_hoRegion.CountObj();i++)
		{
			int nItemCount = m_LC_Image.GetItemCount();
			CString strText;
			strText.Format("%d",i+1);
			m_LC_Image.InsertItem(nItemCount,strText);
			HTuple Row1,Row2;
			SmallestRectangle1(m_hoRegion.SelectObj(i+1),&Row1,NULL,&Row2,NULL);
			if (Row2 - Row1 == 69 || 89 == Row2 - Row1)//OK
			{
				m_LC_Image.SetItemBkColor(nItemCount,1,RGB(0,255,0));
			}else if (Row2 - Row1 == 70 || 90 == Row2 - Row1)//NG
			{
				m_LC_Image.SetItemBkColor(nItemCount,1,RGB(255,0,0));
			}else /*(Row2 - Row1 == 80)*///尺寸错误
			{
				m_LC_Image.SetItemBkColor(nItemCount,1,RGB(255,165,0));
			} 
		}
	}
	catch(...)
	{
		AfxMessageBox("初始化出现问题");
	}
	
}
BOOL CDlgHistory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_LC_Image.SetGridLines(TRUE);
	m_LC_Image.InsertColumn(0,"序号",LVCFMT_LEFT,40);
	m_LC_Image.InsertColumn(1,"状态",LVCFMT_LEFT,80);

	/*CString strException;
	strException.Format("%s","步骤1");
	g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
*/
	InitListCtrl();
	InitUI();
	CRect rcImage;
	GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(rcImage);//获得当前控件大小
	SetWindowAttr("background_color","white");
	SetCheck("~father");
	OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_IMAGE)->m_hWnd,"visible","",&m_hvWindowID);
	SetCheck("father");
	SetDraw(m_hvWindowID, "margin");
	SetColor(m_hvWindowID, "red");
	set_display_font(m_hvWindowID, 16, "mono", "true", "false");
	SetWindowAttr("background_color","dim gray");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgHistory::OnNMClickListImage(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);//将pNMHDR类型转换为LPNMITEMACTIVATE类型
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nSelItem = pNMItemActivate->iItem;
	try
	{
		HTuple Width,Height;
		m_hoDispImage = m_hoImage.SelectObj(nSelItem+1);
		GetImageSize(m_hoDispImage,&Width,&Height);
		SetPart(m_hvWindowID,0,0,Height-1,Width-1);
		DispObj(m_hoDispImage,m_hvWindowID);
	}catch(HException)
	{

	}
}


void CDlgHistory::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg,rcTitle,rcClient,rcCtl;  
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

	GetDlgItem(IDC_LIST_IMAGE)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_STATIC_IMAGE)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
}


BOOL CDlgHistory::PreTranslateMessage(MSG* pMsg)
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


BOOL CDlgHistory::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgHistory::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgHistory::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
