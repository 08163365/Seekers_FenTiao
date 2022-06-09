// DlgDailyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgDailyButton.h"
#include "afxdialogex.h"


// CDlgDailyButton 对话框

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


// CDlgDailyButton 消息处理程序


void CDlgDailyButton::OnBnClickedBtnXiangjichuchen()
{
	// TODO: 在此添加控件通知处理程序代码


	g_pCLabeller->DoSomething("SETPARAM","XJCC",NULL);


}


void CDlgDailyButton::OnBnClickedBtnBaojingfuwei()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCommunicate->DoSomething("SETPARAM","BJFW",NULL);//贴标机设备报警复位
	g_pCLabeller->DoSomething("SETPARAM","BJFW",NULL);//PLC点位报警复位

}


void CDlgDailyButton::OnBnClickedBtnBianmagunyaxia()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","BMGQGYX",NULL);

}


void CDlgDailyButton::OnBnClickedBtnBianmaguntaiqi()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","BMGQGTTQ",NULL);
}


void CDlgDailyButton::OnBnClickedBtnSdlqgyx()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","SDLQGYX",NULL);

}


void CDlgDailyButton::OnBnClickedBtnSdlqgtq()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","SDLQGTQ",NULL);
}


void CDlgDailyButton::OnBnClickedBtnXdlqgyx()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","XDLQGYX",NULL);
}


void CDlgDailyButton::OnBnClickedBtnXdlqgtq()
{
	// TODO: 在此添加控件通知处理程序代码
	g_pCLabeller->DoSomething("SETPARAM","XDLQGTQ",NULL);
}


BOOL CDlgDailyButton::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LabelControlInfo tLabelControlInfo;
	g_pCommunicate->DoSomething("GETPARAM","LabelControlInfo",&tLabelControlInfo);
	m_EDT_dLabelLoaction=tLabelControlInfo.dLabelActionPosition;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDailyButton::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	LabelControlInfo tLabelControlInfo;
	g_pCommunicate->DoSomething("GETPARAM","LabelControlInfo",&tLabelControlInfo);
	tLabelControlInfo.dLabelActionPosition=m_EDT_dLabelLoaction;
	g_pCommunicate->DoSomething("SETPARAM","LabelControlInfo",&tLabelControlInfo);
	CDialogEx::OnOK();
}
