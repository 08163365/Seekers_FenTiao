// DlgHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "SCIDetect.h"
#include "DlgHelp.h"
#include "afxdialogex.h"


// CDlgHelp 对话框

IMPLEMENT_DYNAMIC(CDlgHelp, CDialogEx)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgHelp::IDD, pParent)
{

}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CDlgHelp::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgHelp 消息处理程序


BOOL CDlgHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//FullScreenAdaptive(this->GetSafeHwnd());
	
	CRect rcClient;
	GetDlgItem(IDC_STC_IMAGE)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	m_Wnd_Image.Create("", GetDlgItem(IDC_STC_IMAGE)->GetStyle(), rcClient, this,IDC_STC_IMAGE);


	//HObject hoImage;
	//CString strImageFile;
	//strImageFile.Format("%s\\res\\参数说明.bmp",GetCurrentAppPath());
	//try
	//{
	//	ReadImage(&hoImage,(HTuple)strImageFile);
	//	m_Wnd_Image.DispImage(hoImage);
	//}catch(HException& e)
	//{
	//	CString strMessage;
	//	strMessage.Format("%s文件不存在",strImageFile);
	//	AfxMessageBox(strMessage);
	//}
	CString strFilepath;
	strFilepath.Format("%s\\res\\线位置示意图.png", GetCurrentAppPath());
	HObject hoPicture;
	HTuple hvWidth, hvHeight, hvNum;
	ReadImage(&hoPicture, (HTuple)strFilepath);
	CountObj(hoPicture, &hvNum);
	if (hvNum == 0)
	{
		AfxMessageBox("缺少划线示意图");
	}

	m_Wnd_Image.DispImage(hoPicture);






	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgHelp::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
