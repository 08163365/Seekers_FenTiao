// DlgRegister.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgRegister.h"
#include "afxdialogex.h"
// CDlgRegister 对话框

IMPLEMENT_DYNAMIC(CDlgRegister, CDialogEx)

	CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRegister::IDD, pParent)
	, m_strRegisterCode(_T(""))
	, m_strUserName(_T(""))
	, m_strPassword(_T(""))
	, m_COM_StrPowerType(_T(""))
{

}

CDlgRegister::~CDlgRegister()
{
}

void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REGISTERCODE, m_strRegisterCode);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_CHNAME, m_STATIC_chName);
	DDX_Control(pDX, IDC_STATIC_CHPASSWORD, m_STATIC_chPassword);
	DDX_Control(pDX, IDC_STATIC_CHREGISTER, m_STATIC_chRegister);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_EDIT_password);
	DDX_Control(pDX, IDC_EDIT_REGISTERCODE, m_EDIT_registerCode);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_EDIT_userName);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_LST_INFO, m_LST_CPowerInfo);
	DDX_Text(pDX, IDC_COM_POWERTYPE, m_COM_StrPowerType);
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgRegister::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgRegister::OnBnClickedButtonCancel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_STN_CLICKED(IDC_STATIC_TITLE, &CDlgRegister::OnStnClickedStaticTitle)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgRegister::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgRegister::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgRegister::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_LOOK, &CDlgRegister::OnBnClickedBtnLook)
END_MESSAGE_MAP()


// CDlgRegister 消息处理程序


BOOL CDlgRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	CRect rcRect;
	m_LST_CPowerInfo.GetWindowRect(rcRect);
	int nWidth=rcRect.Width()/3 - 1;
	m_LST_CPowerInfo.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_LST_CPowerInfo.InsertColumn(0,"工号",LVCFMT_LEFT,nWidth);
	m_LST_CPowerInfo.InsertColumn(1,"密码",LVCFMT_LEFT,nWidth);
	m_LST_CPowerInfo.InsertColumn(2,"权限类型",LVCFMT_LEFT,nWidth);
	/*int nItemCount;
	for (int nPL = 0; nPL < g_CPowerLimit.m_vec_EngineerPowerList.size(); nPL++)
	{
	nItemCount=m_LST_CPowerInfo.GetItemCount();
	m_LST_CPowerInfo.InsertItem(nItemCount,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strJobNum);
	m_LST_CPowerInfo.SetItemText(nItemCount,1,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strJobPassWord);
	m_LST_CPowerInfo.SetItemText(nItemCount,2,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strPowerType);
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgRegister::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_STATIC_chRegister.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chName.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chPassword.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_STATIC_chName.SetTransparent(TRUE);
	m_STATIC_chPassword.SetTransparent(TRUE);
	m_STATIC_chRegister.SetTransparent(TRUE);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);

	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_btnOK.SetFont(&m_ftButton);
	m_btnCancel.SetFont(&m_ftButton);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);

	//m_btnOK.SetIcon(IDI_ICON_OK);
	//m_btnCancel.SetIcon(IDI_ICON_CANCEL);
	m_EDIT_userName.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_userName.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
	m_EDIT_password.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_password.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
	m_EDIT_registerCode.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_registerCode.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
}

void CDlgRegister::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strRegisterCode != "dahengimage")
	{
		AfxMessageBox("授权码输入错误");
		return;
	}

	if (m_strUserName.GetLength() == 0)
	{
		AfxMessageBox("用户名不能为空");
		return;
	}
	if (m_strUserName == "root")
	{
		AfxMessageBox("不能注册用户名为root的用户");
		return;
	}
	if (0 == g_nDatabaseType)
	{
		AfxMessageBox("没有使用数据库");
		return;
	}
	

	CString strSql;
	strSql.Format("SELECT * FROM [dahengimage].[dbo].[user]");
	_RecordsetPtr pset = g_AdoDatabase.GetRecordSet((_bstr_t)strSql);
	if (pset != NULL)
	{
		_variant_t Value;
		for (int i=0;i<pset->GetRecordCount();i++)//校验用户名是否存在
		{
			Value = pset->GetCollect("USERNAME");
			if (Value.bstrVal == m_strUserName)
			{
				AfxMessageBox("该用户名已经存在");
				return;
			}
			pset->MoveNext();
		}

		strSql.Format("INSERT INTO [dahengimage].[dbo].[user]"
			"([USERNAME],[PASSWORD])"
			"VALUES"
			"('%s','%s')",m_strUserName,m_strPassword);


		if(g_AdoDatabase.ExecuteSQL((_bstr_t)strSql))
		{
			AfxMessageBox("注册成功");
			CDialogEx::OnOK();
		}else
		{
			AfxMessageBox("写入数据库失败,注册失败");
		}
	}else
	{
		AfxMessageBox("写入数据库失败,注册失败");
	}

}


void CDlgRegister::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgRegister::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);
}


BOOL CDlgRegister::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgRegister::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgRegister::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgRegister::OnStnClickedStaticTitle()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgRegister::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CDlgRegister::SetList()
{

	m_LST_CPowerInfo.DeleteAllItems();
	int nItemCount;
	for (int nPL = 0; nPL < g_CPowerLimit.m_vec_EngineerPowerList.size(); nPL++)
	{
		nItemCount=m_LST_CPowerInfo.GetItemCount();
		m_LST_CPowerInfo.InsertItem(nItemCount,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strJobNum);
		m_LST_CPowerInfo.SetItemText(nItemCount,1,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strJobPassWord);
		m_LST_CPowerInfo.SetItemText(nItemCount,2,g_CPowerLimit.m_vec_EngineerPowerList[nPL].strPowerType);
	}
}
void CDlgRegister::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strRegisterCode != "dahengimage")
	{
		AfxMessageBox("授权码输入错误");
		return;
	}
	if (m_COM_StrPowerType== "")
	{
		AfxMessageBox("权限类型不能为空");
		return;
	}

	if (m_strUserName.GetLength() == 0)
	{
		AfxMessageBox("用户名不能为空");
		return;
	}
	if (m_strUserName == "root")
	{
		AfxMessageBox("不能注册用户名为root的用户");
		return;
	}
	if (m_strPassword == " ")
	{
		AfxMessageBox("密码不能为空");
		return;
	}

	PowerInfo  tPowerInfo;
	tPowerInfo.strJobNum=m_strUserName;
	tPowerInfo.strPowerType=m_COM_StrPowerType;
	tPowerInfo.strJobPassWord=m_strPassword;
	if (FALSE==g_CPowerLimit.addData(tPowerInfo))
	{
		AfxMessageBox("该用户名已存在!");
	}
	//SetList();
}


void CDlgRegister::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strRegisterCode != "dahengimage")
	{
		AfxMessageBox("授权码输入错误");
		return;
	}
	POSITION tPos = m_LST_CPowerInfo.GetFirstSelectedItemPosition();
	int nItem = m_LST_CPowerInfo.GetNextSelectedItem(tPos);
	if (nItem >= 0)
	{
		m_LST_CPowerInfo.DeleteItem(nItem);
	}
	g_CPowerLimit.m_vec_EngineerPowerList.clear();
	for (int i=0;i<m_LST_CPowerInfo.GetItemCount();i++)
	{
		PowerInfo tPowerInfo;
		tPowerInfo.strJobNum=m_LST_CPowerInfo.GetItemText(i,0);
		tPowerInfo.strJobPassWord=m_LST_CPowerInfo.GetItemText(i,1);
		tPowerInfo.strPowerType=m_LST_CPowerInfo.GetItemText(i,2);
		g_CPowerLimit.addData(tPowerInfo);
	}



}


void CDlgRegister::OnBnClickedBtnLook()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strRegisterCode != "dahengimage")
	{
		AfxMessageBox("授权码输入错误");
		return;
	}
	SetList();
}
