// DlgProductParam.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgProductParam.h"
#include "afxdialogex.h"
#include "SeekersDlg.h"

// CDlgProductParam 对话框

IMPLEMENT_DYNAMIC(CDlgProductParam, CDialogEx)

CDlgProductParam::CDlgProductParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProductParam::IDD, pParent)
	, m_COM_emCoatType(0)
	, m_COM_emProductType(0)
{

}

CDlgProductParam::~CDlgProductParam()
{
}

void CDlgProductParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OK, m_BTN_OK);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_BTN_cancel);
	DDX_CBIndex(pDX, IDC_COMBO_COATTYPE, m_COM_emCoatType);
	DDX_CBIndex(pDX, IDC_COMBO_PRODUTTYPE, m_COM_emProductType);
	DDX_Control(pDX, IDC_COMBO_COATTYPE, m_COM_CCoatType);
}


BEGIN_MESSAGE_MAP(CDlgProductParam, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgProductParam::OnBnClickedButtonOk)
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgProductParam::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgProductParam::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgProductParam 消息处理程序
void CDlgProductParam::InitUI()
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;


	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);

	m_BTN_OK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_OK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_BTN_cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_BTN_OK.SetFont(&m_ftButton);
	m_BTN_cancel.SetFont(&m_ftButton);



	m_FontCombo.CreateFontA(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"华文细黑");
	GetDlgItem(IDC_COMBO_COATTYPE)->SetFont(&m_FontCombo);
	GetDlgItem(IDC_COMBO_PRODUTTYPE)->SetFont(&m_FontCombo);




}
BOOL CDlgProductParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	InitUI();
	/*		M_TwoCam_Continue,//连续涂布
	M_TwoCam_Intermission,//间歇涂布*/	
	m_COM_CCoatType.InsertString(0,"连续涂布");
	m_COM_CCoatType.InsertString(1,"间歇涂布");





	m_COM_emCoatType = g_ProductParam.emCoatType;
	m_COM_emProductType = g_ProductParam.emProductType;//产品极性


	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgProductParam::OnPaint()
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


HBRUSH CDlgProductParam::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDlgProductParam::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


void CDlgProductParam::OnBnClickedButtonOk()
{

	UpdateData(TRUE);
	CString strMessage;
	strMessage.Format("更换此配置,应用程序将会重启,确定更换并重启?");
	if (IDYES == AfxMessageBox(strMessage,MB_YESNO))
	{
		g_ProductParam.emCoatType = (e_CoatType)m_COM_emCoatType;
		g_ProductParam.emProductType = (e_ProductType)m_COM_emProductType;
		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
		WritePrivateProfileInt(g_strProductName,"CoatType",g_ProductParam.emCoatType,g_strConfigSystemFile);
		WritePrivateProfileInt(g_strProductName,"ProductType",g_ProductParam.emProductType,g_strConfigSystemFile);

		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式

		((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
		CDialogEx::OnOK();
		
	}



}


LRESULT CDlgProductParam::OnNcHitTest(CPoint point)
{
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgProductParam::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
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


BOOL CDlgProductParam::PreTranslateMessage(MSG* pMsg)
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


void CDlgProductParam::OnBnClickedButtonCancel()
{
    CDialogEx::OnCancel();
}

void CDlgProductParam::OnBnClickedButtonClose()
{
	CDialogEx::OnCancel();
}
