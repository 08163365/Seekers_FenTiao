// DlgInput.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgInput.h"
#include "afxdialogex.h"


// CDlgInput 对话框

IMPLEMENT_DYNAMIC(CDlgInput, CDialogEx)

CDlgInput::CDlgInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgInput::IDD, pParent)
	, m_strInput(_T(""))
{

}

CDlgInput::~CDlgInput()
{
}

void CDlgInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_strInput);
	DDX_Control(pDX, IDC_BTN_OK, m_btnOK);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_EDIT_input);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgInput, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgInput::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgInput::OnBnClickedBtnOk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgInput::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgInput 消息处理程序

void CDlgInput::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.Format("  %s",m_strTitle);
	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);


	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);

	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
	
	m_btnOK.SetFont(&m_ftButton);
	m_btnCancel.SetFont(&m_ftButton);
	//m_btnOK.SetIcon(IDI_ICON_LOGINLOGIN);
	//m_btnCancel.SetIcon(IDI_ICON_CANCEL);

	m_EDIT_input.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_input.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
}


void CDlgInput::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CDlgInput::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}


BOOL CDlgInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgInput::OnPaint()
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
}


BOOL CDlgInput::PreTranslateMessage(MSG* pMsg)
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


BOOL CDlgInput::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgInput::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgInput::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
