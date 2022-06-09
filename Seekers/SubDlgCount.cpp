// SubDlgCount.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "SubDlgCount.h"
#include "afxdialogex.h"
#include "DlgDatabaseHistory.h"

// CSubDlgCount 对话框

IMPLEMENT_DYNAMIC(CSubDlgCount, CDialogEx)

CSubDlgCount::CSubDlgCount(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSubDlgCount::IDD, pParent)
{
	//m_pSubDlgHistogram = NULL;

}

CSubDlgCount::~CSubDlgCount()
{
}

void CSubDlgCount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COUNTCALSS, m_listCountClass);
	DDX_Control(pDX, IDC_LIST_COUNT, m_listCount);
	DDX_Control(pDX, IDC_BTN_HISTOGRAM, m_btnHistogram);
	DDX_Control(pDX, IDC_BTN_CLEARCOUNT, m_btnClearCount);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistory);
}


BEGIN_MESSAGE_MAP(CSubDlgCount, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_HISTOGRAM, &CSubDlgCount::OnBnClickedBtnHistogram)
	ON_BN_CLICKED(IDC_BTN_CLEARCOUNT, &CSubDlgCount::OnBnClickedBtnClearcount)
	ON_BN_CLICKED(IDC_BTN_HISTORY, &CSubDlgCount::OnBnClickedBtnHistory)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSubDlgCount 消息处理程序

void CSubDlgCount::InsertData()
{

	const int nColumCount = 16;
	for (int nGrabberSN = 0; nGrabberSN < g_nGrabberCount; nGrabberSN++)
	{
		CListCtrlExt *pList;
		HWND hwnd;
		if (nGrabberSN == 0)
		{
			pList = &m_listCountClass;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNTCALSS);
		}
		else
		{
			pList = &m_listCount;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNT);
		}
		SNDMSG(hwnd, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

		while (pList->GetHeaderCtrl()->GetItemCount())
		{
			pList->DeleteColumn(0);
		}

		pList->DeleteAllItems();
		pList->InsertColumn(0, _T("条号/相机"), LVCFMT_CENTER, 50);//50
		for (int i = 0; i < MIN(nColumCount, g_Count.strAClassName.GetSize()); i++)//插入错误类项目
		{
			pList->InsertColumn(i + 1, g_Count.strAClassName.GetAt(i), LVCFMT_CENTER, 80);//128
		}
		int nItemCount = pList->GetItemCount();
		for (int i = 0; i < m_nLaneCount; i++)//插入统计数目
		{
			CString strIndex;
			strIndex.Format("%d/%d", i, nGrabberSN);
			pList->InsertItem(nItemCount + i, strIndex);
			CString strCount;
			for (int j = 0; j < MIN(nColumCount, g_Count.strAClassName.GetSize()); j++)
			{
				strCount.Format("%d", g_Count.veCount[nGrabberSN][i].nClassError[j]);
				pList->SetItemText(nItemCount + i, j + 1, strCount);
			}
		}


		pList->SetColColor(0, RGB(10, 150, 20)); //设置列背景色
		pList->SetColTextColor(0, RGB(255, 255, 255));
		for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); i++)
		{
			if (0 == i % 2)
			{
				pList->SetColColor(i, RGB(30, 100, 90)); //设置列背景色
				pList->SetColTextColor(i, RGB(255, 255, 0));
			}
			else
			{
				pList->SetColColor(i, RGB(50, 120, 110)); //设置列背景色
				pList->SetColTextColor(i, RGB(0, 255, 255));
			}
		}
		//设置控件无闪烁
		SNDMSG(hwnd, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
	}
	for (int nGrabberSN = 0; nGrabberSN < g_nGrabberCount; nGrabberSN++)
	{
		CListCtrlExt *pList;
		HWND hwnd;
		if (nGrabberSN == 0)
		{
			pList = &m_listCountClass;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNTCALSS);
		}
		else
		{
			pList = &m_listCount;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNT);
		}
		SNDMSG(hwnd, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

		int nItemCount = pList->GetItemCount();
		pList->InsertItem(nItemCount, "条号/相机");
		pList->SetItemColor(0, nItemCount, RGB(255, 255, 255));
		pList->SetItemTextColor(0, nItemCount, RGB(0, 0, 0));
		for (int i = 0; i < MAX(0, g_Count.strAClassName.GetSize() - (nColumCount + 1)); i++)//插入错误类项目
		{

			pList->SetItemColor(i + 1, nItemCount, RGB(255, 255, 255));
			pList->SetItemTextColor(i + 1, nItemCount, RGB(0, 0, 0));
			pList->SetItemText(nItemCount, i + 1, g_Count.strAClassName.GetAt(nColumCount + 1 + i));//128

		}
		nItemCount = pList->GetItemCount();
		for (int i = 0; i < m_nLaneCount; i++)//插入统计数目
		{
			CString strIndex;
			strIndex.Format("%d/%d", i, nGrabberSN);
			pList->InsertItem(nItemCount + i, strIndex);
			CString strCount;
			for (int j = 0; j < MAX(0, g_Count.strAClassName.GetSize() - (nColumCount + 1)); j++)
			{
				strCount.Format("%d", g_Count.veCount[nGrabberSN][i].nClassError[nColumCount + 1 + j]);
				pList->SetItemText(nItemCount + i, j + 1, strCount);
			}
		}
		//设置控件无闪烁
		SNDMSG(hwnd, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
	}

}


BOOL CSubDlgCount::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//AdaptiveSize(m_hWnd);
	// TODO:  在此添加额外的初始化
	m_nLaneCount = g_tSystemSet.nLaneCount;//共分为几条
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSubDlgCount::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


void CSubDlgCount::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


void CSubDlgCount::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}


void CSubDlgCount::InitUI()
{

	m_ftButton.CreatePointFont(100, "Microsoft YaHei");
	m_ftList.CreatePointFont(80, "Microsoft YaHei");

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

	m_btnHistogram.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnHistogram.SetFont(&m_ftButton);
	m_btnHistogram.SetIcon(IDI_ICON_W_123);
	m_btnClearCount.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnClearCount.SetFont(&m_ftButton);
	m_btnClearCount.SetIcon(IDI_ICON_W_CLEAR);
	m_btnHistory.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnHistory.SetFont(&m_ftButton);
	m_btnHistory.SetIcon(IDI_ICON_W_PAPER);


	m_listCountClass.SetHeaderHeight(1.5);          //设置头部高度
	m_listCountClass.SetHeaderFontHW(13, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listCountClass.SetHeaderTextColor(RGB(0, 0, 0)); //设置头部字体颜色 255,200,100
	m_listCountClass.SetHeaderBKColor(255, 255, 255, 0); //设置头部背景色 100,255,100,8
	m_listCountClass.SetBkColor(RGB(255, 255, 255));        //设置背景色 50,10,10
	m_listCountClass.SetRowHeigt(30);               //设置行高度
	m_listCountClass.SetFontHW(12, 0);              //设置字体高度，和宽度,0表示缺省宽度

	m_listCount.SetHeaderHeight(1.5);          //设置头部高度
	m_listCount.SetHeaderFontHW(13, 0);         //设置头部字体高度,和宽度,0表示缺省，自适应 
	m_listCount.SetHeaderTextColor(RGB(0, 0, 0)); //设置头部字体颜色
	m_listCount.SetHeaderBKColor(255, 255, 255, 0); //设置头部背景色
	m_listCount.SetBkColor(RGB(255, 255, 255));        //设置背景色
	m_listCount.SetRowHeigt(30);               //设置行高度
	m_listCount.SetFontHW(12, 0);               //设置字体高度，和宽度,0表示缺省宽度

	m_listCountClass.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //设置扩展风格为网格
	m_listCount.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //设置扩展风格为网格
	InsertData();
}

void CSubDlgCount::ResetSize()
{
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);

	LONG nSrcX(987), nSrcY(408);
	LONG nDstX = rtSize.Width();
	LONG nDstY = rtSize.Height();
	float fsp[2];
	fsp[0] = (float)nDstX / nSrcX;
	fsp[1] = (float)nDstY / nSrcY;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		pWnd->GetDlgItem(woc)->GetWindowRect(Rect);
		pWnd->ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		pWnd->GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}

}

void CSubDlgCount::OnBnClickedBtnHistogram()
{

}


void CSubDlgCount::OnBnClickedBtnClearcount()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(TEXT("是否清空?"), TEXT("提示"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	for (int k = 0; k < g_nGrabberCount; k++)//多少个相机
	{

		for (int j = 0; j < m_nLaneCount; j++)//每个相机下有几条
		{
			for (int i = 0; i < g_Count.nType; i++)//每一条的错误种类数目
			{
				g_Count.veCount[k][j].nClassError[i] = 0;//每种错误的计数
			}
			g_Count.veCount[k][j].nError = 0;
			g_Count.veCount[k][j].nTotal = 0;
		}
	}


	InsertData();
}


void CSubDlgCount::OnBnClickedBtnHistory()
{
	CDlgDatabaseHistory dlg;
	dlg.DoModal();
}


void CSubDlgCount::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcFullDlg, rcTitle;
	GetClientRect(rcFullDlg);
	dc.FillSolidRect(rcFullDlg, RGB(255, 255, 255));
}


BOOL CSubDlgCount::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
