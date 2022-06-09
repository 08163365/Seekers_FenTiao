// CDlgTcpConnect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgTcpConnect.h"
#include "afxdialogex.h"


// CDlgTcpConnect �Ի���

IMPLEMENT_DYNAMIC(CDlgTcpConnect, CDialogEx)

CDlgTcpConnect::CDlgTcpConnect(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTcpConnect::IDD, pParent)
	, m_EDT_StrComIP(_T(""))
	, m_EDT_nComPort(0)
	, m_COM_StrGongWeiName(_T(""))
	, m_EDT_nStopNum(0)

{

}

CDlgTcpConnect::~CDlgTcpConnect()
{
}

void CDlgTcpConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_CTitle);
	DDX_Control(pDX, ID_OK, m_BTN_Save);
	DDX_Control(pDX, IDCANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDC_BTN_RETURN, m_BTN_Return);
	DDX_Text(pDX, IDC_EDT_COMMONIP, m_EDT_StrComIP);
	DDX_Text(pDX, IDC_EDT_COMMONCOM, m_EDT_nComPort);
	DDX_CBString(pDX, IDC_COM_COMUTERLOCATOPNNAME, m_COM_StrGongWeiName);
	DDX_Text(pDX, IDC_EDT_WARNMACHINE, m_EDT_nStopNum);
}


BEGIN_MESSAGE_MAP(CDlgTcpConnect, CDialogEx)
	ON_BN_CLICKED(ID_OK, &CDlgTcpConnect::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_RETURN, &CDlgTcpConnect::OnBnClickedBtnReturn)
END_MESSAGE_MAP()


// CDlgTcpConnect ��Ϣ�������


BOOL CDlgTcpConnect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;


	m_STC_CTitle.SetTextColor(RGB(255,255,255));
	m_STC_CTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CTitle.SetBkColor(MainColor,0,CStaticExt::Normal);


	m_BTN_Save.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Save.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	m_BTN_Cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	m_BTN_Return.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Return.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_Return.SetIcon(IDI_ICON_W_UNCHECK);

	if (g_CUpLoad.m_StrGongWeiName=="UpOffset")
	{
		m_COM_StrGongWeiName="�Ϲ�λ";
	}else
	{
		m_COM_StrGongWeiName="�¹�λ";
	}

	m_EDT_StrComIP=g_CUpLoad.m_tParam.strAddressCom;
	m_EDT_nComPort=g_CUpLoad.m_tParam.nPortCom;
	m_EDT_nStopNum=g_CUpLoad.m_tParam.nNGNumToStopMachine;
	
	UpdateData(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgTcpConnect::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ����
	UpdateData(TRUE);
	g_CUpLoad.m_tParam.strAddressCom=m_EDT_StrComIP;
	g_CUpLoad.m_tParam.nPortCom=m_EDT_nComPort;
	g_CUpLoad.m_tParam.nNGNumToStopMachine=m_EDT_nStopNum;
	if (m_COM_StrGongWeiName=="�Ϲ�λ")
	{
		g_CUpLoad.m_StrGongWeiName="UpOffset";
	}else
	{
		g_CUpLoad.m_StrGongWeiName="DownOffset";
	}

	g_CUpLoad.WriteParam();
	CDialogEx::OnOK();

}


void CDlgTcpConnect::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//���߿���
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);




}


HBRUSH CDlgTcpConnect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgTcpConnect::OnBnClickedBtnReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}



