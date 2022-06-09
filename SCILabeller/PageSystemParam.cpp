// PageSystemParam.cpp : 实现文件
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "PageSystemParam.h"
#include "afxdialogex.h"


// CPageSystemParam 对话框

IMPLEMENT_DYNAMIC(CPageSystemParam, CDialogEx)

CPageSystemParam::CPageSystemParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageSystemParam::IDD, pParent)
{
	m_LST_Font.CreatePointFont(100,"Microsoft YaHei");
}

CPageSystemParam::~CPageSystemParam()
{
}

void CPageSystemParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_PARAMSET, m_LST_ParamList);
}


BEGIN_MESSAGE_MAP(CPageSystemParam, CDialogEx)
END_MESSAGE_MAP()
void CPageSystemParam::ResetSize()
{

	CRect rcGridBase, rcDlg;
	this->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);

	int nWidth=300;
	m_LST_ParamList.SetExtendedStyle( LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER );
	m_LST_ParamList.InsertColumn(0,"参数项目",LVCFMT_LEFT,nWidth);
	m_LST_ParamList.InsertColumn(1,"参数值",LVCFMT_LEFT,nWidth/2);
	m_LST_ParamList.SetFont(&m_LST_Font);


}
void CPageSystemParam::UpdateList(HTuple& hv_Name,HTuple& hv_Info)
{
	if (hv_Name.Length()==0)
	{
		return;
	}
	m_LST_ParamList.DeleteAllItems();
	CString strValue;
	int nItemCount;
	HTuple hvTrue=FALSE;
	for (int k=0;k<hv_Name.Length();k++)
	{
		nItemCount=m_LST_ParamList.GetItemCount();
		m_LST_ParamList.InsertItem(nItemCount,hv_Name[k].S().Text());
		TupleIsInt(hv_Info[k],&hvTrue);
		if (hvTrue.I()==1)
		{
			strValue.Format("%d",hv_Info[k].I());
		}else
		{
			strValue.Format("%.1f",hv_Info[k].D());
		}
		
		m_LST_ParamList.SetItemText(nItemCount,1,strValue);

	}



}

// CPageSystemParam 消息处理程序
