// PageControlParam.cpp : 实现文件
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "DlgLabelInfoDisp.h"
#include "PageControlParam.h"
#include "afxdialogex.h"


// CPageControlParam 对话框

IMPLEMENT_DYNAMIC(CPageControlParam, CDialogEx)

CPageControlParam::CPageControlParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageControlParam::IDD, pParent)
{
	m_LST_Font.CreatePointFont(100,"Microsoft YaHei");
}

CPageControlParam::~CPageControlParam()
{
}

void CPageControlParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_CONTROLLIST, m_LST_ControlList);
}


BEGIN_MESSAGE_MAP(CPageControlParam, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ENABLECONNECT, &CPageControlParam::OnBnClickedBtnEnableconnect)
	ON_BN_CLICKED(IDC_BTN_ENABLEMANNUAL, &CPageControlParam::OnBnClickedBtnEnablemannual)
//	ON_BN_CLICKED(IDC_BTN_ENABLELABEL, &CPageControlParam::OnBnClickedBtnEnablelabel)
ON_BN_CLICKED(IDC_BTN_ENABLESUMWARN, &CPageControlParam::OnBnClickedBtnEnablesumwarn)
ON_BN_CLICKED(IDC_BTN_ENABLEFOURLABEL, &CPageControlParam::OnBnClickedBtnEnablefourlabel)
ON_BN_CLICKED(IDC_BTN_ENABLECONLABELWARN, &CPageControlParam::OnBnClickedBtnEnableconlabelwarn)
ON_BN_CLICKED(IDC_BTN_ENABLELABEL, &CPageControlParam::OnBnClickedBtnEnablelabel)
ON_BN_CLICKED(IDC_BTN_ENABLEMARKUSE, &CPageControlParam::OnBnClickedBtnEnablemarkuse)
ON_BN_CLICKED(IDC_BTN_ENABLEDUANDAI, &CPageControlParam::OnBnClickedBtnEnableduandai)
END_MESSAGE_MAP()



void CPageControlParam::ResetSize()
{
	
	CRect rcGridBase, rcDlg;
	this->GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	int nWidth=300;
	m_LST_ControlList.SetExtendedStyle( LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER);
	m_LST_ControlList.InsertColumn(0,"项目",LVCFMT_LEFT,nWidth);
	m_LST_ControlList.InsertColumn(1,"值",LVCFMT_LEFT,nWidth);
	m_LST_ControlList.SetFont(&m_LST_Font);
}


void CPageControlParam::UpdateList(HTuple& hv_Name,HTuple& hv_Info)
{
	if (hv_Name.Length()==0)
	{
		return;
	}


	m_LST_ControlList.DeleteAllItems();
	CString strValue;
	int nItemCount;
	for (int k=0;k<hv_Name.Length();k++)
	{
		nItemCount=m_LST_ControlList.GetItemCount();
		m_LST_ControlList.InsertItem(nItemCount,hv_Name[k].S().Text());
		if (hv_Info[k].I()==0)
		{
			strValue="关闭";
			m_LST_ControlList.SetItemText(nItemCount,1,strValue);
			m_LST_ControlList.SetItemBkColor(nItemCount,1,RGB(255,0,0));

		}else
		{
			strValue="开启";
			m_LST_ControlList.SetItemText(nItemCount,1,strValue);
			m_LST_ControlList.SetItemBkColor(nItemCount,1,RGB(0,255,0));
		}
	}



}

void CPageControlParam::OnBnClickedBtnEnableconnect()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4750;///联网地址
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		 nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}


}


void CPageControlParam::OnBnClickedBtnEnablemannual()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4734;///手动模式
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}





void CPageControlParam::OnBnClickedBtnEnablesumwarn()
{
	// TODO: 在此添加控件通知处理程序代码

		if (m_pChildOwner->m_CFinsTcp.m_bConnected)
		{
			int nValue;
			int nDM=4754;///累计打标
			m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
			nValue=!nValue;
			m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
		}
}


void CPageControlParam::OnBnClickedBtnEnablefourlabel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4762;///四链表开关
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnableconlabelwarn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4758;///四链表开关
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnablelabel()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4212;///四链表开关
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}



}


void CPageControlParam::OnBnClickedBtnEnablemarkuse()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4760;///四链表开关
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnableduandai()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4769;///断带报警
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}
