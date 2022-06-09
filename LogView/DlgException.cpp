// DlgException.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LogView.h"
#include "DlgException.h"
#include "afxdialogex.h"


// CDlgException �Ի���

IMPLEMENT_DYNAMIC(CDlgException, CDialogEx)

CDlgException::CDlgException(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgException::IDD, pParent)
	,m_bShow(FALSE)
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_ftList.CreatePointFont(120,"Microsoft YaHei");
}

CDlgException::~CDlgException()
{
	m_ftButton.DeleteObject();
	m_ftList.DeleteObject();
}

void CDlgException::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_LIST_EXCEPTION, m_listException);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgException, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDCANCEL, &CDlgException::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgException::OnBnClickedButtonCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgException::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgException ��Ϣ�������


BOOL CDlgException::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitUI();
	ShowWindow(SW_HIDE);
	//m_tipExcepion.Create(this);
	//CWnd* pwnd = GetDlgItem(IDC_LIST_EXCEPTION);
	//m_tipExcepion.AddTool(pwnd, "This is ToolTip");
	//pwnd = GetDlgItem(IDC_BUTTON_CANCEL);
	//m_tipExcepion.AddTool(pwnd, "ȡ��");
	//m_tipExcepion.SetTipTextColor(RGB(0,0,0));
	//m_tipExcepion.SetTipBkColor(RGB(240,240,240));
	//m_tipExcepion.SetFont(&m_ftList);
	////m_tipExcepion.SetDelayTime(150);
	//m_tipExcepion.SetMaxTipWidth(123);
	//m_tipExcepion.Activate(TRUE);
	//EnableToolTips(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CDlgException::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//���λس�
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)//����ESC
		{
			return TRUE;
		}else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//����ALT+F4
		{
			return TRUE;
		}
	}else if (pMsg->wParam & MK_LBUTTON)
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
		if (rcTitle.PtInRect(pMsg->pt))
		{
			this->SetFocus();
		}
	}else if (pMsg->wParam == WM_MOUSEMOVE)
	{
		//if(m_tipExcepion.m_hWnd!=NULL)
		/*{
		m_tipExcepion.RelayEvent(pMsg);
		}*/
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgException::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgException::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient,rcCtl;  
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

	GetDlgItem(IDC_LIST_EXCEPTION)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
}


BOOL CDlgException::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgException::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		return HTCAPTION;
	}
	return CDialogEx::OnNcHitTest(point);
}

void CDlgException::InitUI()
{
	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(RGB(248,92,76),0,CxStatic::Normal);

	COLORREF BKOUT(RGB(248,92,76));
	COLORREF BKIN(RGB(231,76,60));
	COLORREF BKFOCUS(RGB(192,57,42));
	COLORREF FG(RGB(255,255,255));

	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCancel.SetFont(&m_ftButton);

	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);

	m_BTN_close.SetFont(&m_ftButton);

	m_listException.SetFont(&m_ftList);
}

void CDlgException::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*90)/100,LWA_ALPHA);
	}else if (WA_CLICKACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}else if (WA_ACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
}


void CDlgException::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnCancel();
	CDialogEx::DestroyWindow();
}


void CDlgException::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
	 CriticalSectionHelper Lock(&m_pLog->m_OperateChange);
	m_pLog->m_bSoftExceptionStauts=FALSE;
	//CDialogEx::OnCancel();
}

BOOL CDlgException::ReadException(const CString &strFile,CStringArray* strArrayExcepion)
{
	CStdioFile fileException;
	CString strExceptionFileName = strFile;
	try
	{
		fileException.Open(strExceptionFileName,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
		CString strRead;
		while(fileException.ReadString(strRead))
		{
			strArrayExcepion->Add(strRead);
		}
	}catch(...)
	{
		fileException.Close();
		return FALSE;
	}
	try
	{
		fileException.Close();
	}catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

void CDlgException::ShowException()
{
	m_bShow = TRUE;
	CStringArray strArrayException;
	if (m_pLog == NULL)
	{
		return;
	}
	m_pLog->ReadException(&strArrayException);
	m_listException.ResetContent();
	for (int i=0;i<strArrayException.GetCount();i++)
	{
		m_listException.InsertString(0,strArrayException.GetAt(i));
	}
	if (m_bShow && strArrayException.GetCount()>0)
	{
		ShowWindow(SW_NORMAL);
	}
	//SetFocus();
}

void CDlgException::ShowException(const CString& strExceptionFile)
{
	//m_bShow = TRUE;
	CStringArray strArrayException;
	/*if (m_pLog == NULL)
	{
	return;
	}*/
	//m_pLog->ReadException(&strArrayException);
	ReadException(strExceptionFile,&strArrayException);
	m_listException.ResetContent();
	for (int i=0;i<strArrayException.GetCount();i++)
	{
		m_listException.InsertString(0,strArrayException.GetAt(i));
	}
	if (m_bShow && strArrayException.GetCount()>0)
	{
		ShowWindow(SW_NORMAL);
	}
}

void CDlgException::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void CDlgException::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowWindow(SW_HIDE);
	 CriticalSectionHelper Lock(&m_pLog->m_OperateChange);
	m_pLog->m_bSoftExceptionStauts=FALSE;
}

