// DlgHelp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SCIDetect.h"
#include "DlgHelp.h"
#include "afxdialogex.h"


// CDlgHelp �Ի���

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


// CDlgHelp ��Ϣ�������


BOOL CDlgHelp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//FullScreenAdaptive(this->GetSafeHwnd());
	
	CRect rcClient;
	GetDlgItem(IDC_STC_IMAGE)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	m_Wnd_Image.Create("", GetDlgItem(IDC_STC_IMAGE)->GetStyle(), rcClient, this,IDC_STC_IMAGE);


	//HObject hoImage;
	//CString strImageFile;
	//strImageFile.Format("%s\\res\\����˵��.bmp",GetCurrentAppPath());
	//try
	//{
	//	ReadImage(&hoImage,(HTuple)strImageFile);
	//	m_Wnd_Image.DispImage(hoImage);
	//}catch(HException& e)
	//{
	//	CString strMessage;
	//	strMessage.Format("%s�ļ�������",strImageFile);
	//	AfxMessageBox(strMessage);
	//}
	CString strFilepath;
	strFilepath.Format("%s\\res\\��λ��ʾ��ͼ.png", GetCurrentAppPath());
	HObject hoPicture;
	HTuple hvWidth, hvHeight, hvNum;
	ReadImage(&hoPicture, (HTuple)strFilepath);
	CountObj(hoPicture, &hvNum);
	if (hvNum == 0)
	{
		AfxMessageBox("ȱ�ٻ���ʾ��ͼ");
	}

	m_Wnd_Image.DispImage(hoPicture);






	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgHelp::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
