// DlgNewProductName.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgNewProductName.h"
#include "afxdialogex.h"


// CDlgNewProductName 对话框

IMPLEMENT_DYNAMIC(CDlgNewProductName, CDialogEx)

CDlgNewProductName::CDlgNewProductName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgNewProductName::IDD, pParent)
	, m_strNewProduct(_T(""))
{

}

CDlgNewProductName::~CDlgNewProductName()
{
}

void CDlgNewProductName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_NEWPRODUCTNAME, m_strNewProduct);
	DDX_Control(pDX, IDC_STATIC_CHPRODUCT, m_STATIC_chNewProduct);
	DDX_Control(pDX, IDC_EDIT_NEWPRODUCTNAME, m_EDIT_newProductName);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgNewProductName, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgNewProductName::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgNewProductName::OnBnClickedButtonCancel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgNewProductName::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgNewProductName 消息处理程序


BOOL CDlgNewProductName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitStatic();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgNewProductName::InitStatic()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_STATIC_chNewProduct.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chNewProduct.SetTransparent(TRUE);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);

	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_btnOK.SetFont(&m_ftButton);
	m_btnCancel.SetFont(&m_ftButton);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);

	//m_btnOK.SetIcon(IDI_ICON_OK);
	//m_btnCancel.SetIcon(IDI_ICON_CANCEL);

	m_EDIT_newProductName.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_newProductName.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
}

void CDlgNewProductName::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (0 == m_strNewProduct.GetLength())
	{
		AfxMessageBox("配方名称不能为空");
		return;
	}
	CDialogEx::OnOK();
}


void CDlgNewProductName::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgNewProductName::OnPaint()
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


BOOL CDlgNewProductName::PreTranslateMessage(MSG* pMsg)
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


BOOL CDlgNewProductName::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgNewProductName::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgNewProductName::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
