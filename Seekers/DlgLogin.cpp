// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgLogin.h"
#include "afxdialogex.h"
#include "DlgRegister.h"
#include "SeekersDlg.h"

// CDlgLogin �Ի���

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogin::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
	, m_EDT_StrJobNum(_T(""))
{

	m_COM_StrUserName = _T("");
}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btnLogin);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_REGISTER, m_btnRegister);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_STATIC_CHNAME, m_STATIC_chName);
	DDX_Control(pDX, IDC_STATIC_CHPASSWORD, m_STATIC_chPassword);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EDIT_password);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_COM_USERNAME, m_COM_CUserName);
	DDX_CBString(pDX, IDC_COM_USERNAME, m_COM_StrUserName);
	DDX_Text(pDX, IDC_EDT_JOBNUM, m_EDT_StrJobNum);
	DDX_Control(pDX, IDC_EDT_JOBNUM, m_EDT_CJobNum);
	DDX_Control(pDX, IDC_STC_JOBNUM, m_STC_CJobNum);
	//  DDX_Control(pDX, IDC_STC_CapslockStatus, m_STC_CCapLockStatus);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CDlgLogin::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgLogin::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CDlgLogin::OnBnClickedButtonRegister)
	ON_WM_PAINT()
//	ON_WM_ERASEBKGND()
//	ON_WM_ACTIVATE()
//	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgLogin::OnBnClickedButtonClose)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgLogin ��Ϣ�������


BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_COM_CUserName.InsertString(0,"����Ա");
	m_COM_CUserName.InsertString(1,"����ʦ");
	m_COM_CUserName.InsertString(2,"��������ʦ");
	m_COM_CUserName.InsertString(3,"����");
	m_COM_CUserName.SetCurSel(3);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CDlgLogin::Checkin()//У���û��������Ƿ���ȷ
{
	UpdateData(TRUE);
	return g_CPowerLimit.GetUserPower(m_COM_StrUserName,m_EDT_StrJobNum,m_strPassword);
	if (0 == g_nDatabaseType)
	{
		return FALSE;
	}
	CString strSql;
	strSql.Format("SELECT * FROM [dahengimage].[dbo].[user]");
	_RecordsetPtr pset = g_AdoDatabase.GetRecordSet((_bstr_t)strSql);
	if (pset != NULL)
	{
		_variant_t Value;
		for (int i=0;i<pset->GetRecordCount();i++)
		{
			Value = pset->GetCollect("USERNAME");
			if (Value.bstrVal == m_strUserName)
			{
				Value = pset->GetCollect("PASSWORD");
				if (Value.bstrVal == m_strPassword)
				{
					return TRUE;
				}
			}
			pset->MoveNext();
		}
	}else
	{
		AfxMessageBox(g_AdoDatabase.GetLastErrorText());
	}
	
	return FALSE;
}
void CDlgLogin::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_ftComBox.CreatePointFont(130,"Microsoft YaHei");
	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);


	m_STATIC_chName.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chPassword.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chName.SetTransparent(TRUE);
	m_STATIC_chPassword.SetTransparent(TRUE);
	
	m_STC_CJobNum.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STC_CJobNum.SetTransparent(TRUE);



	//short	shBtnColor = 0;
	//COLORREF BKOUT(RGB(129,197,81));
	//COLORREF BKIN(RGB(114,182,66));//124,192,76
	//COLORREF BKFOCUS(RGB(104,172,56));
	//COLORREF FG(RGB(255,255,255));

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//���ñ���ɫ ��7����ť
	m_btnLogin.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnRegister.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	
	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);

	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	

	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);

	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);

	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);


	m_btnLogin.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, FG);
	m_btnRegister.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, FG);

	m_btnLogin.SetFont(&m_ftButton);
	m_btnCancel.SetFont(&m_ftButton);
	m_btnRegister.SetFont(&m_ftButton);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
	m_COM_CUserName.SetFont(&m_ftComBox);
	m_EDIT_password.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_password.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);

	//m_EDT_CJobNum.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	//m_EDT_CJobNum.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
	//	g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);


	m_EDT_CJobNum.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CJobNum.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);

}

void CDlgLogin::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (Checkin())
	{

		if (g_CPowerLimit.IsEngineAndFactoryContinue())
		{
			g_bExitLogin=TRUE;

			if (g_CPowerLimit.m_bSuperEngineerPower ||g_CPowerLimit.m_bFactoryPower)
			{
				for (int i=0;i<g_nGrabberCount;i++)
				{
					int nPowerFlag=1;
					g_vec_Algorithm[i]->DoSomething("SETPARAM","SetPowerFlag",&nPowerFlag);
				}
			}
			
			
		}
		CString str;
		str.Format("%s",m_COM_StrUserName);
		((CSeekersDlg*)g_pMainDialog)->m_wndStatusBar.SetPaneText(6,"�û���" + str);
		CDialogEx::OnOK();
	}	
	
	
}


void CDlgLogin::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDlgLogin::OnBnClickedButtonRegister()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgRegister dlg;
	dlg.DoModal();

}


void CDlgLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
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


//BOOL CDlgLogin::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	return TRUE;
//	//return CDialogEx::OnEraseBkgnd(pDC);
//}


//void CDlgLogin::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
//{
//	CDialogEx::OnActivate(nState, pWndOther, bMinimized);
//
//	// TODO: �ڴ˴������Ϣ����������
//	if (WA_INACTIVE == nState)  
//	{  
//		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
//			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
//		this->SetLayeredWindowAttributes(0,(255*85)/100,LWA_ALPHA);
//	}else if (WA_CLICKACTIVE == nState)
//	{
//		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
//			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
//		this->RedrawWindow();
//	}else if (WA_ACTIVE == nState)
//	{
//		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
//			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
//		this->RedrawWindow();
//	}
//}


//LRESULT CDlgLogin::OnNcHitTest(CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	CRect rcTitle;
//	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
//
//	return rcTitle.PtInRect(point)
//		? HTCAPTION : CDialogEx::OnNcHitTest(point);
//	//return CDialogEx::OnNcHitTest(point);
//}


void CDlgLogin::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


//HBRUSH CDlgLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	// TODO:  �ڴ˸��� DC ���κ�����
//	if (nCtlColor==CTLCOLOR_STATIC)
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		return (HBRUSH)::GetStockObject(NULL_BRUSH);
//	}
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
//}


BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
		if(pMsg->message==WM_KEYDOWN)
		{
			if (pMsg->wParam == VK_RETURN)//����ǻس�
			{
				OnBnClickedButtonLogin();
				return TRUE;
			}else if (pMsg->wParam == VK_ESCAPE)
			{
				return TRUE;

			}else if (pMsg->wParam == 'V' && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
			{
				return TRUE;
			}
				 
		}
		else if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
		{
			CRect rcTitle;
			GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
			if (rcTitle.PtInRect(pMsg->pt))
			{
				PostMessage(WM_NCLBUTTONDOWN, HTCAPTION,  pMsg->lParam);
			}
		}  else if(pMsg->message == WM_RBUTTONUP)			
		{
			return TRUE;
		}
		
	return CDialogEx::PreTranslateMessage(pMsg);
}
