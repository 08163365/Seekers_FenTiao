// DlgTabSizeSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgTabSizeSet.h"
#include "afxdialogex.h"
#include "SeekersDlg.h"


// CDlgTabSizeSet 对话框

IMPLEMENT_DYNAMIC(CDlgTabSizeSet, CDialogEx)

	CDlgTabSizeSet::CDlgTabSizeSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTabSizeSet::IDD, pParent)
{
	m_TabFont.CreatePointFont(100,"Microsoft YaHei");
}

CDlgTabSizeSet::~CDlgTabSizeSet()
{

	m_TabFont.DeleteObject();

}

void CDlgTabSizeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SET, m_Tab_Common);
}


BEGIN_MESSAGE_MAP(CDlgTabSizeSet, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SET, &CDlgTabSizeSet::OnSelchangeTabSet)
	ON_BN_CLICKED(IDOK, &CDlgTabSizeSet::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgTabSizeSet 消息处理程序


BOOL CDlgTabSizeSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_nTripNum = m_pOwner->m_tStandardSize.hvTripNum;
    m_Tab_Common.InsertItem(0, "尺寸规格");
	m_Tab_Common.InsertItem(1, "瑕疵规格");
	m_Tab_Common.SetFont(&m_TabFont);
	CRect rcTabItem;
	m_Tab_Common.GetClientRect(&rcTabItem);
	//调整子对话框在父窗口中的位置
	rcTabItem.top += 30;
	rcTabItem.bottom -= 1;
	rcTabItem.left += 1;
	rcTabItem.right -= 2;

	m_pPageStandardSize = new CDlgStandardSetting;
	m_pPageStandardSize->m_pOwner = m_pOwner;
	m_pPageStandardSize->Create(CDlgStandardSetting::IDD, &m_Tab_Common);
	m_pPageStandardSize->MoveWindow(rcTabItem);
	m_pPageStandardSize->ShowWindow(SW_SHOW);
	m_vecPages.push_back(m_pPageStandardSize);



	m_pPageStandardSurface = new CDlgStandardSurfaceSetting;
	m_pPageStandardSurface->m_pOwner = m_pOwner;
	m_pPageStandardSurface->Create(CDlgStandardSurfaceSetting::IDD, &m_Tab_Common);
	m_pPageStandardSurface->MoveWindow(rcTabItem);
	m_pPageStandardSurface->ShowWindow(SW_HIDE);
	m_vecPages.push_back(m_pPageStandardSurface);

	SwitchDispPage(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgTabSizeSet::SwitchDispPage(const int & nSN)
{
	if (nSN >= m_vecPages.size())return;

	for (int i = 0; i<m_vecPages.size(); i++)
	{
		if (nSN == i)
			m_vecPages[i]->ShowWindow(SW_NORMAL);
		else
			m_vecPages[i]->ShowWindow(SW_HIDE);
	}
}


void CDlgTabSizeSet::OnSelchangeTabSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	SwitchDispPage(m_Tab_Common.GetCurSel());
}


void CDlgTabSizeSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (g_CPowerLimit.m_bFactoryPower==FALSE && g_CPowerLimit.m_bSuperEngineerPower==FALSE && g_CPowerLimit.m_bEngineerPower==FALSE)
	{
		AfxMessageBox("保存参数需要工程师以上权限");
		return;
	}


	m_pPageStandardSize->GetValue();
	m_pPageStandardSurface->GetSurfaceList();

	if (m_nTripNum != m_pOwner->m_tStandardSize.hvTripNum)
	{
		AfxMessageBox("修改条数后需要重启软件,请保存软件重启");
	}


	int nTripNum=g_Jugde[0].m_tStandardSize.hvTripNum;
	g_pCommunicate->DoSomething("SETPARAM","TRIPNUM",&nTripNum);
	g_vec_Algorithm[0]->DoSomething("SETPARAM","tSizeParam",&m_pOwner->m_tStandardSize);
	g_vec_Algorithm[1]->DoSomething("SETPARAM","tSizeParam",&m_pOwner->m_tStandardSize);

	HTuple hvInfo;
	hvInfo.Clear();
	
	int nLocationY = g_Jugde[0].m_tStandardSize.hvQiTouXianChu;
	hvInfo.Append(nLocationY).Append(m_pOwner->m_tStandardSize.dUpIntermissionHeight);
	g_vec_FindMark[0]->DoSomething("SETPARAM", "LocatePositionY", &hvInfo);
	g_vec_FindMark[1]->DoSomething("SETPARAM", "LocatePositionY", &hvInfo);

	m_pOwner->WriteParam();


	g_ProductParam.emCoatType = (e_CoatType)g_Jugde[0].m_tStandardSize.hv_CoatType.I();
	g_ProductParam.emProductType = (e_ProductType)g_Jugde[0].m_tStandardSize.hv_ProductType.I();

	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileInt(g_strProductName, "CoatType", g_ProductParam.emCoatType, g_strConfigSystemFile);
	WritePrivateProfileInt(g_strProductName, "ProductType", g_ProductParam.emProductType, g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
	CDialogEx::OnOK();
}


void CDlgTabSizeSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(255,255,255));
}


HBRUSH CDlgTabSizeSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
