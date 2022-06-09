// DlgDailyButton.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgDailyButton.h"
#include "afxdialogex.h"


// CDlgDailyButton �Ի���

IMPLEMENT_DYNAMIC(CDlgDailyButton, CDialogEx)

CDlgDailyButton::CDlgDailyButton(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDailyButton::IDD, pParent)
{

	m_EDT_dLabelLoaction = 0.0;
}

CDlgDailyButton::~CDlgDailyButton()
{
}

void CDlgDailyButton::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LABELLOCATION, m_EDT_dLabelLoaction);
}


BEGIN_MESSAGE_MAP(CDlgDailyButton, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_XIANGJICHUCHEN, &CDlgDailyButton::OnBnClickedBtnXiangjichuchen)
	ON_BN_CLICKED(IDC_BTN_BAOJINGFUWEI, &CDlgDailyButton::OnBnClickedBtnBaojingfuwei)
	ON_BN_CLICKED(IDC_BTN_BIANMAGUNYAXIA, &CDlgDailyButton::OnBnClickedBtnBianmagunyaxia)
	ON_BN_CLICKED(IDC_BTN_BIANMAGUNTAIQI, &CDlgDailyButton::OnBnClickedBtnBianmaguntaiqi)
	ON_BN_CLICKED(IDC_BTN_SDLQGYX, &CDlgDailyButton::OnBnClickedBtnSdlqgyx)
	ON_BN_CLICKED(IDC_BTN_SDLQGTQ, &CDlgDailyButton::OnBnClickedBtnSdlqgtq)
	ON_BN_CLICKED(IDC_BTN_XDLQGYX, &CDlgDailyButton::OnBnClickedBtnXdlqgyx)
	ON_BN_CLICKED(IDC_BTN_XDLQGTQ, &CDlgDailyButton::OnBnClickedBtnXdlqgtq)
	ON_BN_CLICKED(IDOK, &CDlgDailyButton::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgDailyButton ��Ϣ�������


void CDlgDailyButton::OnBnClickedBtnXiangjichuchen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	g_pCLabeller->DoSomething("SETPARAM","XJCC",NULL);


}


void CDlgDailyButton::OnBnClickedBtnBaojingfuwei()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCommunicate->DoSomething("SETPARAM","BJFW",NULL);//������豸������λ
	g_pCLabeller->DoSomething("SETPARAM","BJFW",NULL);//PLC��λ������λ

}


void CDlgDailyButton::OnBnClickedBtnBianmagunyaxia()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","BMGQGYX",NULL);

}


void CDlgDailyButton::OnBnClickedBtnBianmaguntaiqi()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","BMGQGTTQ",NULL);
}


void CDlgDailyButton::OnBnClickedBtnSdlqgyx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","SDLQGYX",NULL);

}


void CDlgDailyButton::OnBnClickedBtnSdlqgtq()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","SDLQGTQ",NULL);
}


void CDlgDailyButton::OnBnClickedBtnXdlqgyx()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","XDLQGYX",NULL);
}


void CDlgDailyButton::OnBnClickedBtnXdlqgtq()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	g_pCLabeller->DoSomething("SETPARAM","XDLQGTQ",NULL);
}


BOOL CDlgDailyButton::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LabelControlInfo tLabelControlInfo;
	g_pCommunicate->DoSomething("GETPARAM","LabelControlInfo",&tLabelControlInfo);
	m_EDT_dLabelLoaction=tLabelControlInfo.dLabelActionPosition;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgDailyButton::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	LabelControlInfo tLabelControlInfo;
	g_pCommunicate->DoSomething("GETPARAM","LabelControlInfo",&tLabelControlInfo);
	tLabelControlInfo.dLabelActionPosition=m_EDT_dLabelLoaction;
	g_pCommunicate->DoSomething("SETPARAM","LabelControlInfo",&tLabelControlInfo);
	CDialogEx::OnOK();
}
