// PageWarn.cpp : 实现文件
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "PageWarn.h"
#include "afxdialogex.h"


// CPageWarn 对话框

IMPLEMENT_DYNAMIC(CPageWarn, CDialogEx)

CPageWarn::CPageWarn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageWarn::IDD, pParent)
{
	m_LST_Font.CreatePointFont(100,"Microsoft YaHei");
}

CPageWarn::~CPageWarn()
{
}

void CPageWarn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LST_ErrorInfo);
}


BEGIN_MESSAGE_MAP(CPageWarn, CDialogEx)
END_MESSAGE_MAP()

void CPageWarn::ResetSize()
{

	CRect rcGridBase, rcDlg;
	this->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	int nWidth=300;
	m_LST_ErrorInfo.SetExtendedStyle( LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);
	m_LST_ErrorInfo.InsertColumn(0,"报警项目",LVCFMT_LEFT,nWidth);
	nWidth=200;
	m_LST_ErrorInfo.InsertColumn(1,"状态",LVCFMT_LEFT,nWidth);
	m_LST_ErrorInfo.SetFont(&m_LST_Font);

}
// CPageWarn 消息处理程序
void CPageWarn::UpdateList(HTuple& hv_Name,HTuple& hv_Info)
{

	if (hv_Name.Length()==0)
	{
		return;
	}


HWND hwndList = m_LST_ErrorInfo.GetSafeHwnd();
SNDMSG(hwndList,WM_SETREDRAW,FALSE,0);
	m_LST_ErrorInfo.DeleteAllItems();
	CString strValue;
	int nItemCount;
	for (int k=0;k<hv_Name.Length();k++)
	{
		nItemCount=m_LST_ErrorInfo.GetItemCount();
		m_LST_ErrorInfo.InsertItem(nItemCount,hv_Name[k].S().Text());
		if (hv_Info[k].D()==1)
		{
			m_LST_ErrorInfo.SetItemText(nItemCount,1,"NG");
			m_LST_ErrorInfo.SetItemBkColor(nItemCount,1,RGB(255,0,0));
		}else
		{
			m_LST_ErrorInfo.SetItemText(nItemCount,1,"OK");
			m_LST_ErrorInfo.SetItemBkColor(nItemCount,1,RGB(0,255,0));
		}
		

	}
	SNDMSG(hwndList,WM_SETREDRAW,TRUE,0);
}
