// PageControlParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "DlgLabelInfoDisp.h"
#include "PageControlParam.h"
#include "afxdialogex.h"


// CPageControlParam �Ի���

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
	m_LST_ControlList.InsertColumn(0,"��Ŀ",LVCFMT_LEFT,nWidth);
	m_LST_ControlList.InsertColumn(1,"ֵ",LVCFMT_LEFT,nWidth);
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
			strValue="�ر�";
			m_LST_ControlList.SetItemText(nItemCount,1,strValue);
			m_LST_ControlList.SetItemBkColor(nItemCount,1,RGB(255,0,0));

		}else
		{
			strValue="����";
			m_LST_ControlList.SetItemText(nItemCount,1,strValue);
			m_LST_ControlList.SetItemBkColor(nItemCount,1,RGB(0,255,0));
		}
	}



}

void CPageControlParam::OnBnClickedBtnEnableconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4750;///������ַ
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		 nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}


}


void CPageControlParam::OnBnClickedBtnEnablemannual()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4734;///�ֶ�ģʽ
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}





void CPageControlParam::OnBnClickedBtnEnablesumwarn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

		if (m_pChildOwner->m_CFinsTcp.m_bConnected)
		{
			int nValue;
			int nDM=4754;///�ۼƴ��
			m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
			nValue=!nValue;
			m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
		}
}


void CPageControlParam::OnBnClickedBtnEnablefourlabel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4762;///��������
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnableconlabelwarn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4758;///��������
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnablelabel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4212;///��������
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}



}


void CPageControlParam::OnBnClickedBtnEnablemarkuse()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4760;///��������
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}


void CPageControlParam::OnBnClickedBtnEnableduandai()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pChildOwner->m_CFinsTcp.m_bConnected)
	{
		int nValue;
		int nDM=4769;///�ϴ�����
		m_pChildOwner->m_CFinsTcp.ReadInt16(nDM,nValue);
		nValue=!nValue;
		m_pChildOwner->m_CFinsTcp.WriteInt16(nDM,nValue);
	}
}
