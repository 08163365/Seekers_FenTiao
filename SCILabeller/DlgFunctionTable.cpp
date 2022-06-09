// DlgFunctionTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SCILabeller.h"
#include "DlgFunctionTable.h"
#include "afxdialogex.h"


// CDlgFunctionTable �Ի���

IMPLEMENT_DYNAMIC(CDlgFunctionTable, CDialogEx)

CDlgFunctionTable::CDlgFunctionTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFunctionTable::IDD, pParent)
{
		m_vecPages.clear();
}

CDlgFunctionTable::~CDlgFunctionTable()
{
}

void CDlgFunctionTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_LABELFUNCTION, m_TAB_CLabelFunction);
}


BEGIN_MESSAGE_MAP(CDlgFunctionTable, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LABELFUNCTION, &CDlgFunctionTable::OnTcnSelchangeTabLabelfunction)
END_MESSAGE_MAP()


// CDlgFunctionTable ��Ϣ�������

void CDlgFunctionTable::ResetSize()
{
	int nSrcX, nSrcY;
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);
	GetDlgItem(IDC_TAB_LABELFUNCTION)->MoveWindow(rtSize);

	m_TAB_CLabelFunction.InsertItem(0, "��������ͼ");
	m_TAB_CLabelFunction.InsertItem(1, "���ݱ��");
	m_TAB_CLabelFunction.InsertItem(2, "EA���");

	m_TAB_CLabelFunction.SetTextFont("Microsoft Yahei", 19, 0);
	m_TAB_CLabelFunction.SetColor(RGB(220, 224, 236), RGB(253, 244, 191), RGB(253, 244, 191), RGB(220, 224, 236), RGB(0, 0, 0), RGB(0, 0, 0));
	m_TAB_CLabelFunction.SetItemSize(CSize(140, 30));
	m_TAB_CLabelFunction.SetMinTabWidth(100);
	CRect rcSubDlg, rcTabItem;
	this->GetClientRect(rcSubDlg);
	m_TAB_CLabelFunction.GetItemRect(0, rcTabItem);
	rcSubDlg.top = rcSubDlg.top + rcTabItem.Height() + 3;
	rcSubDlg.left += 1;
	rcSubDlg.right -= 1;
	rcSubDlg.bottom -= 1;
}
BOOL CDlgFunctionTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgFunctionTable::SwitchDispPage(const int & nSN)
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
void CDlgFunctionTable::OnTcnSelchangeTabLabelfunction(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	SwitchDispPage(m_TAB_CLabelFunction.GetCurSel());



}
