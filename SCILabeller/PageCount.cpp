// PageCount.cpp : 实现文件
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "PageCount.h"
#include "afxdialogex.h"


// CPageCount 对话框

IMPLEMENT_DYNAMIC(CPageCount, CDialogEx)

CPageCount::CPageCount(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageCount::IDD, pParent)
{
	m_LST_Font.CreatePointFont(100,"Microsoft YaHei");
}

CPageCount::~CPageCount()
{
}

void CPageCount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COUNT, m_LST_LabelCountList);
	//  DDX_Control(pDX, IDC_LIST_WARN, m_LST_ErrorInfo);
}


BEGIN_MESSAGE_MAP(CPageCount, CDialogEx)
END_MESSAGE_MAP()

void CPageCount::ResetSize()
{

	int nSrcX,nSrcY;
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);
	nSrcX = rtSize.Width();
	nSrcY = rtSize.Height();





	int nWidth=300;
	m_LST_LabelCountList.SetExtendedStyle( LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);
	m_LST_LabelCountList.InsertColumn(0,"统计项目",LVCFMT_LEFT,nWidth);
	m_LST_LabelCountList.InsertColumn(1,"值",LVCFMT_LEFT,nWidth);
	m_LST_LabelCountList.SetFont(&m_LST_Font);

}
// CPageCount 消息处理程序
void CPageCount::UpdateList(HTuple& hv_Name,HTuple& hv_Info)
{
	if (hv_Name.Length()==0)
	{
		return;
	}
	m_LST_LabelCountList.DeleteAllItems();
	CString strValue;
	int nItemCount;
	for (int k=0;k<hv_Name.Length();k++)
	{
		nItemCount=m_LST_LabelCountList.GetItemCount();
		m_LST_LabelCountList.InsertItem(nItemCount,hv_Name[k].S().Text());
		m_LST_LabelCountList.SetItemText(nItemCount,1,hv_Info[k].S().Text());

	}



}
