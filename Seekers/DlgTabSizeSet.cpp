// DlgTabSizeSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgTabSizeSet.h"
#include "afxdialogex.h"
#include "SeekersDlg.h"


// CDlgTabSizeSet �Ի���

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


// CDlgTabSizeSet ��Ϣ�������


BOOL CDlgTabSizeSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_nTripNum = m_pOwner->m_tStandardSize.hvTripNum;
    m_Tab_Common.InsertItem(0, "�ߴ���");
	m_Tab_Common.InsertItem(1, "覴ù��");
	m_Tab_Common.SetFont(&m_TabFont);
	CRect rcTabItem;
	m_Tab_Common.GetClientRect(&rcTabItem);
	//�����ӶԻ����ڸ������е�λ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	SwitchDispPage(m_Tab_Common.GetCurSel());
}


void CDlgTabSizeSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (g_CPowerLimit.m_bFactoryPower==FALSE && g_CPowerLimit.m_bSuperEngineerPower==FALSE && g_CPowerLimit.m_bEngineerPower==FALSE)
	{
		AfxMessageBox("���������Ҫ����ʦ����Ȩ��");
		return;
	}


	m_pPageStandardSize->GetValue();
	m_pPageStandardSurface->GetSurfaceList();

	if (m_nTripNum != m_pOwner->m_tStandardSize.hvTripNum)
	{
		AfxMessageBox("�޸���������Ҫ�������,�뱣���������");
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

	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
	WritePrivateProfileInt(g_strProductName, "CoatType", g_ProductParam.emCoatType, g_strConfigSystemFile);
	WritePrivateProfileInt(g_strProductName, "ProductType", g_ProductParam.emProductType, g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ
	CDialogEx::OnOK();
}


void CDlgTabSizeSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(255,255,255));
}


HBRUSH CDlgTabSizeSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
