// SubDlgControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "SubDlgControl.h"
#include "afxdialogex.h"
#include "SeekersDlg.h"


// CSubDlgControl �Ի���

IMPLEMENT_DYNAMIC(CSubDlgControl, CDialogEx)

CSubDlgControl::CSubDlgControl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSubDlgControl::IDD, pParent)
{

}

CSubDlgControl::~CSubDlgControl()
{
}

void CSubDlgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_btnReset);
	DDX_Control(pDX, IDC_BUTTON_RESTARTPROCESS, m_btnReStartProcess);
	DDX_Control(pDX, IDC_BUTTON_SHUTDOWN, m_btnShutDown);
}


BEGIN_MESSAGE_MAP(CSubDlgControl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSubDlgControl::OnBnClickedButtonReset)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_RESTARTPROCESS, &CSubDlgControl::OnBnClickedButtonRestartprocess)
	ON_BN_CLICKED(IDC_BUTTON_SHUTDOWN, &CSubDlgControl::OnBnClickedButtonShutdown)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSubDlgControl ��Ϣ�������


BOOL CSubDlgControl::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AdaptiveSize(m_hWnd);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSubDlgControl::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


void CSubDlgControl::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::DestroyWindow();
}

VOID CSubDlgControl::InitUI()
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");


	COLORREF BKOUT(RGB(255,240,150));
	COLORREF BKIN(RGB(255,240,150));
	m_btnReset.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnReset.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnReset.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnReset.SetFont(&m_ftButton);
	//m_btnReset.SetIcon(IDI_ICON_RESET);
	m_btnReset.SetTooltipText("����ϵͳ");

	m_btnReStartProcess.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnReStartProcess.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnReStartProcess.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnReStartProcess.SetFont(&m_ftButton);
	//m_btnReStartProcess.SetIcon(IDI_ICON_RESTART);
	m_btnReStartProcess.SetTooltipText("����ϵͳ");

	m_btnShutDown.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnShutDown.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnShutDown.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnShutDown.SetFont(&m_ftButton);
	//m_btnShutDown.SetIcon(IDI_ICON_SHUTDOWN);
	m_btnShutDown.SetTooltipText("�رռ����");
	
}
void CSubDlgControl::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::DestroyWindow();
}


void CSubDlgControl::OnBnClickedButtonReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    ((CSeekersDlg*)GetParent())->Reset();

}


void CSubDlgControl::OnPaint()
{
	CRect   rcFullDlg;  
	CPaintDC   dc(this);  
	GetClientRect(rcFullDlg);  
	dc.FillSolidRect(rcFullDlg,RGB(255,255,255));   //����Ϊ��ɫ����255,240,150
	CDialogEx::OnPaint();
}


void CSubDlgControl::OnBnClickedButtonRestartprocess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox("��������?","��ʾ",MB_YESNO) == IDYES)
	{
		((CSeekersDlg*)g_pMainDialog)->ExitDo();
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		memset((void*)&si,0,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.wShowWindow = SW_SHOW;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		CString strApp;
		strApp.Format("%s\\Seekers.exe",GetCurrentAppPath());
		CreateProcess(NULL,strApp.GetBuffer(0),NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
		::TerminateProcess(::GetCurrentProcess(),NULL);//��ֹԭ�ȳ���
	}


}


void CSubDlgControl::OnBnClickedButtonShutdown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox("�رռ����?","��ʾ",MB_YESNO) == IDYES)
	{
		ExitWindowsEx(EWX_SHUTDOWN,0);
	}
}

BOOL CSubDlgControl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
