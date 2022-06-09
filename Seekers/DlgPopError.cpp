// DlgPopError.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SeekersDlg.h"
#include "DlgPopError.h"
#include "afxdialogex.h"


// CDlgPopError �Ի���

IMPLEMENT_DYNAMIC(CDlgPopError, CDialogEx)

CDlgPopError::CDlgPopError(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPopError::IDD, pParent)
	, m_STC_StrErrorName(_T(""))
{
	m_Font.CreatePointFont(120,"Microsoft YaHei");
	InitializeCriticalSection(&m_csAddSubChange);
}

CDlgPopError::~CDlgPopError()
{
	DeleteCriticalSection(&m_csAddSubChange);
	m_Font.DeleteObject();
}

void CDlgPopError::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STC_ERRORNAME, m_STC_StrErrorName);
	DDX_Control(pDX, IDC_STC_ERRORNAME, m_STC_CErrorName);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_MANUALLABEL, m_btn_ManulLabelControl);
	DDX_Control(pDX, IDC_BTN_ANALYSIS, m_btn_Analysis);
}


BEGIN_MESSAGE_MAP(CDlgPopError, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CDlgPopError::OnBnClickedCancel)
//	ON_BN_CLICKED(IDC_BUTTON, &CDlgPopError::OnBnClickedButton)
	ON_BN_CLICKED(IDC_BTN_MANUALLABEL, &CDlgPopError::OnBnClickedBtnManuallabel)
	ON_BN_CLICKED(IDC_BTN_ANALYSIS, &CDlgPopError::OnBnClickedBtnAnalysis)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgPopError ��Ϣ�������


void CDlgPopError::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	SubPopWindowNum(m_tPopErrorInfo.nSide);
	delete this;
	CDialogEx::PostNcDestroy();
}

void CDlgPopError::Disp()
{
	try
	{

		CRect rcImage;
		GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(rcImage);
		SetCheck("~father");
		OpenWindow(rcImage.top, rcImage.left, rcImage.Width(), rcImage.Height(),
			(Hlong)GetDlgItem(IDC_STATIC_IMAGE)->m_hWnd, "visible", "", &m_hvWindowHandle);
		SetDraw(m_hvWindowHandle, "margin");
		HTuple Width, Height;
		GetImageSize(m_tPopErrorInfo.hoImage, &Width, &Height);
		SetPart(m_hvWindowHandle, 0, 0, Height - 1, Width - 1);
		SetCheck("father");
		DispObj(m_tPopErrorInfo.hoImage, m_hvWindowHandle);
		SetColor(m_hvWindowHandle, "red");
		DispObj(m_tPopErrorInfo.hoErrorRegion, m_hvWindowHandle);

		CString strInsert;
		strInsert.Format(_T("��������:%s"),m_STC_StrErrorName);
		m_STC_CErrorName.SetFont(&m_Font);
		m_STC_CErrorName.SetTextColor(COLORREF(RGB(255, 0, 0)));
		m_STC_CErrorName.SetWindowText(strInsert);
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("Disp�����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}
BOOL CDlgPopError::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
		AddPopWindowNumm(m_tPopErrorInfo.nSide);
	m_STC_StrErrorName=m_tPopErrorInfo.StrErrorInfo;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgPopError::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
PostNcDestroy();
	//CDialogEx::OnCancel();

}

void  CDlgPopError::AddPopWindowNumm(int& nTrip)
{
	EnterCriticalSection(&m_csAddSubChange);
	g_hvPopWindowNum[nTrip]=g_hvPopWindowNum[nTrip]+1;
	LeaveCriticalSection(&m_csAddSubChange);
}
void  CDlgPopError::SubPopWindowNum(int& nTrip)
{

	EnterCriticalSection(&m_csAddSubChange);
	g_hvPopWindowNum[nTrip]=g_hvPopWindowNum[nTrip]-1;
	LeaveCriticalSection(&m_csAddSubChange);
}
//void CDlgPopError::OnBnClickedButton()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//	DETECTINPUT tDetectInput;
//	CopyImage(m_tPopErrorInfo.hoImage, &tDetectInput.hoDetectImage);
//	tDetectInput.nBreakType = 0;
//	if (TRUE != g_vec_Algorithm[m_tPopErrorInfo.hvGrabberSN.I()]->DoSomething("SHOW", &tDetectInput, NULL))
//	{
//		CString strException;
//		strException.Format("�㷨%d������ʾ�����쳣,���Ų�ԭ��", m_tPopErrorInfo.hvGrabberSN.I());
//		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
//	}
//}


void CDlgPopError::OnBnClickedBtnManuallabel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (FALSE==g_CPowerLimit.IsEngineAndFactoryContinue())//��������һ�����л��鿴������ͼ
	{
		return ;
	}
	CString strValue;
	strValue.Format("%sȷ�ϲ�����",g_CPowerLimit.GetStatus());
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);
	g_Jugde[m_tPopErrorInfo.nSide].m_bCancelLabelFlag=1;
	OnBnClickedCancel();

}


void CDlgPopError::OnBnClickedBtnAnalysis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DETECTINPUT tDetectInput;
	CopyImage(m_tPopErrorInfo.hoImage, &tDetectInput.hoDetectImage);
	tDetectInput.nBreakType = 0;
	if (TRUE != g_vec_Algorithm[m_tPopErrorInfo.hvGrabberSN.I()]->DoSomething("SHOW", &tDetectInput, NULL))
	{
		CString strException;
		strException.Format("�㷨%d������ʾ�����쳣,���Ų�ԭ��", m_tPopErrorInfo.hvGrabberSN.I());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}


void CDlgPopError::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


BOOL CDlgPopError::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnEraseBkgnd(pDC);
}
