// DlgStartUp.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgStartUp.h"
#include "afxdialogex.h"


// CDlgStartUp 对话框

IMPLEMENT_DYNAMIC(CDlgStartUp, CDialogEx)

CDlgStartUp::CDlgStartUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStartUp::IDD, pParent)
{

}

CDlgStartUp::~CDlgStartUp()
{
}

void CDlgStartUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROCESS, m_StaticProcess);
	DDX_Control(pDX, IDC_STATIC_CHEEKERS, m_STATIC_chEekers);
	DDX_Control(pDX, IDC_STATIC_CHS, m_STATIC_chS);
	DDX_Control(pDX, IDC_STATIC_CHCOMPANY, m_STATIC_chCompany);
	DDX_Control(pDX, IDC_STATIC_CHNAME, m_STATIC_chName);
}


BEGIN_MESSAGE_MAP(CDlgStartUp, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgStartUp 消息处理程序

void CDlgStartUp::SetText(CString strText)
{
	//CString strCurText;
	//m_StaticProcess.GetWindowTextA(strCurText);
	//strCurText+="\r\n";
	//strCurText +=strText;
	m_StaticProcess.SetWindowTextA(strText);

}
BOOL CDlgStartUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//CImage ImageStart;
	//ImageStart.LoadFromResource(AfxGetInstanceHandle(),IDB_BITMAP_STARTUP);
	//MoveWindow(0,0,ImageStart.GetWidth(),ImageStart.GetHeight());
	CenterWindow();
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgStartUp::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	m_StaticProcess.SetFont("Microsoft YaHei light", 15, FW_NORMAL);
	m_StaticProcess.SetTransparent(TRUE);
	m_StaticProcess.SetTextColor(RGB(120,120,120));
	//m_StaticProcess.SetBkColor( RGB(255,255,255),RGB(255,255,255),CxStatic::Normal);
	m_STATIC_chS.SetFont("Microsoft YaHei", 140, FW_NORMAL);
	//m_STATIC_chS.SetTransparent(TRUE);
	m_STATIC_chS.SetTextColor(RGB(255,255,255));
	m_STATIC_chS.SetBkColor( MainColor,0,CStaticExt::Normal);

	m_STATIC_chEekers.SetFont("Microsoft YaHei", 50, FW_NORMAL);
	//m_STATIC_chEekers.SetTransparent(TRUE);
	m_STATIC_chEekers.SetTextColor(RGB(255,255,255));
	m_STATIC_chEekers.SetBkColor( MainColor,0,CStaticExt::Normal);

	m_STATIC_chCompany.SetFont("Microsoft YaHei", 19, FW_NORMAL);
	m_STATIC_chCompany.SetTransparent(TRUE);
	m_STATIC_chCompany.SetTextColor(RGB(255,255,255));
	//m_STATIC_chCompany.SetBkColor( RGB(124,192,76),RGB(124,192,76),CxStatic::Normal);

	m_STATIC_chName.SetFont("Microsoft YaHei", 28, FW_NORMAL);
	m_STATIC_chName.SetTransparent(TRUE);
	m_STATIC_chName.SetTextColor(RGB(0,0,0));

	CRect   rcFullDlg,rcText;  
	GetClientRect(rcFullDlg);
	rcText.left = 10;
	rcText.top = rcFullDlg.bottom - 30;
	rcText.right = 200;
	rcText.bottom = rcFullDlg.bottom - 0;

	m_StaticProcess.MoveWindow(rcText);
}


void CDlgStartUp::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


void CDlgStartUp::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgStartUp::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


HBRUSH CDlgStartUp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here 
	

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgStartUp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle;
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	GetClientRect(rcDlg);  
	dc.FillSolidRect(rcDlg,RGB(255,255,255));
	rcTitle = rcDlg;
	rcTitle.bottom = rcDlg.Height() - 150;
	//dc.FillSolidRect(rcTitle,RGB(124,192,76));
	dc.FillSolidRect(rcTitle,MainColor);

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,MainColor);//200，200，200
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);

	//CPen pen,pen1,pen2,pen3;
	//pen.CreatePen(PS_SOLID,1,RGB(180,180,180));//109,177,61
	//dc.SelectObject(pen);
	//dc.MoveTo(rcDlg.left+3,rcDlg.top-3);
	//dc.LineTo(rcDlg.right-3,rcDlg.top-3);
	//dc.LineTo(rcDlg.right-3,rcDlg.bottom-3);
	//dc.LineTo(rcDlg.left+3,rcDlg.bottom-3);
	//dc.LineTo(rcDlg.left+3,rcDlg.top-3);
	//pen1.CreatePen(PS_SOLID,1,RGB(200,200,200));//94,162,46
	//dc.SelectObject(pen1);
	//dc.MoveTo(rcDlg.left+2,rcDlg.top-2);
	//dc.LineTo(rcDlg.right-2,rcDlg.top-2);
	//dc.LineTo(rcDlg.right-2,rcDlg.bottom-2);
	//dc.LineTo(rcDlg.left+2,rcDlg.bottom-2);
	//dc.LineTo(rcDlg.left+2,rcDlg.top-2);
	//pen2.CreatePen(PS_SOLID,1,RGB(220,220,220));//154,222,106
	//dc.SelectObject(pen2);
	//dc.MoveTo(rcDlg.left+1,rcDlg.top+1);
	//dc.LineTo(rcDlg.right-1,rcDlg.top+1);
	//dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	//dc.LineTo(rcDlg.left+1,rcDlg.bottom-1);
	//dc.LineTo(rcDlg.left+1,rcDlg.top+1);
	//pen3.CreatePen(PS_SOLID,1,RGB(240,240,240));//154,222,106
	//dc.SelectObject(pen3);
	//dc.MoveTo(rcDlg.left,rcDlg.top);
	//dc.LineTo(rcDlg.right,rcDlg.top);
	//dc.LineTo(rcDlg.right,rcDlg.bottom);
	//dc.LineTo(rcDlg.left,rcDlg.bottom);
	//dc.LineTo(rcDlg.left,rcDlg.top);


	//HICON HIcon = LoadIconA(,IDI_ICON_LOGO_S_WHITE);
	//GetDlgItem(IDC_STATIC_PIC)->SetIcon(HIcon,TRUE);
	
	CDialogEx::OnPaint();
}



BOOL CDlgStartUp::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgStartUp::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}
