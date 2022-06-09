// DlgLabelInfoDisp.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLabelInfoDisp.h"
#include "afxdialogex.h"


// CDlgLabelInfoDisp 对话框
CBlendent g_Blendent;
IMPLEMENT_DYNAMIC(CDlgLabelInfoDisp, CDialogEx)
//int        = RegisterWindowMessage (_T("WM_LISTMESSGE"));

CDlgLabelInfoDisp::CDlgLabelInfoDisp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLabelInfoDisp::IDD, pParent)
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_bConnected = false;
	m_COM_nLabelName = 0;
	m_EDT_nTestAddress = 0;
	m_EDT_dTestValue = 0;
	m_vecPages.clear();
}

CDlgLabelInfoDisp::~CDlgLabelInfoDisp()
{
	
	m_ftButton.DeleteObject();
}

void CDlgLabelInfoDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_IPINFO, m_STC_strIp);
	//  DDX_Control(pDX, IDC_LST_ERRORINFO, m_LST_ErrorInfo);
	DDX_CBIndex(pDX, IDC_COM_LABELENAME, m_COM_nLabelName);
	DDX_Text(pDX, IDC_EDT_TESTADDRESS, m_EDT_nTestAddress);
	DDX_Text(pDX, IDC_EDT_TESTVALUE, m_EDT_dTestValue);
	//  DDX_Control(pDX, IDC_LST_PARAMSET, m_LST_ParamList);
	//  DDX_Control(pDX, IDC_LST_LABELCOUNT, m_LST_LabelCountList);
	//  DDX_Control(pDX, IDC_LST_CONTROLLIST, m_LST_ControlList);
	DDX_Control(pDX, IDC_TAB_LABELFUNCTION, m_TAB_CLabelFunction);
	//  DDX_Control(pDX, IDC_STC_SEBIAOGANYING, m_STC_CSeBiao);
	//  DDX_Control(pDX, IDC_STC_DUISHEGUANGQIAN, m_STC_CDuiShe);
	DDX_Control(pDX, IDC_STC_OAPSEBIAO1, m_STC_COAPSebiao1);
	//  DDX_Control(pDX, IDC_STC_OAPSEBIAO2, m_STC_CSeBiao2);
	DDX_Control(pDX, IDC_STC_OAPSEBIAO2, m_STC_COAPSebiao2);
}


BEGIN_MESSAGE_MAP(CDlgLabelInfoDisp, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgLabelInfoDisp::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CDlgLabelInfoDisp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgLabelInfoDisp::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLEARCOUNT, &CDlgLabelInfoDisp::OnBnClickedBtnClearcount)
	ON_BN_CLICKED(IDC_BUTTONONEMARK, &CDlgLabelInfoDisp::OnBnClickedButtononemark)
	ON_BN_CLICKED(IDC_BUTTONTWOMARK, &CDlgLabelInfoDisp::OnBnClickedButtontwomark)
	ON_BN_CLICKED(IDC_BUTTONONEZD, &CDlgLabelInfoDisp::OnBnClickedButtononezd)
	ON_BN_CLICKED(IDC_BUTTONTWOZD, &CDlgLabelInfoDisp::OnBnClickedButtontwozd)
	ON_BN_CLICKED(IDC_BTN_READ, &CDlgLabelInfoDisp::OnBnClickedBtnRead)
	ON_BN_CLICKED(IDC_BTN_WRITE, &CDlgLabelInfoDisp::OnBnClickedBtnWrite)
	ON_BN_CLICKED(IDC_BTN_HELP, &CDlgLabelInfoDisp::OnBnClickedBtnHelp)
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_BTN_READSPEED, &CDlgLabelInfoDisp::OnBnClickedBtnReadspeed)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_LABELFUNCTION, &CDlgLabelInfoDisp::OnSelchangeTabLabelfunction)
	ON_BN_CLICKED(IDC_BTN_QGYX, &CDlgLabelInfoDisp::OnBnClickedBtnQgyx)
	ON_BN_CLICKED(IDC_BTN_TQQG, &CDlgLabelInfoDisp::OnBnClickedBtnTqqg)
	ON_MESSAGE(WM_DISPLISTMESSGE, &CDlgLabelInfoDisp::OnDisplistmessge)
	ON_BN_CLICKED(IDC_BTN_READSPEED, &CDlgLabelInfoDisp::OnBnClickedBtnReadspeed)
	ON_BN_CLICKED(IDC_BTN_XDLQGYX, &CDlgLabelInfoDisp::OnBnClickedBtnXdlqgyx)
	ON_BN_CLICKED(IDC_BTN_XDLTQQG, &CDlgLabelInfoDisp::OnBnClickedBtnXdltqqg)
END_MESSAGE_MAP()


// CDlgLabelInfoDisp 消息处理程序

void CDlgLabelInfoDisp::SetLightColor(CButtonExt& btn,int nFlag)
{
	if (nFlag==1)
	{
	
		btn.SetIcon(IDI_ICON_REDLIGHT);
	}else
	{
		btn.SetIcon(IDI_ICON_GREENLIGHT);
		

	}
	
}

void CDlgLabelInfoDisp::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)
{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_ftButton);
}
BOOL CDlgLabelInfoDisp::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
  	//EnableVisualManagerStyle(TRUE,TRUE);
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;


//	FlushList();
	m_TAB_CLabelFunction.InsertItem(0, "系统参数");
	m_TAB_CLabelFunction.InsertItem(1, "报警信息");
	m_TAB_CLabelFunction.SetTextFont("Microsoft Yahei", 19, 0);
	m_TAB_CLabelFunction.SetColor(RGB(220, 224, 236), RGB(253, 244, 191), RGB(253, 244, 191), RGB(220, 224, 236), RGB(0, 0, 0), RGB(0, 0, 0));
	m_TAB_CLabelFunction.SetItemSize(CSize(140, 30));
	m_TAB_CLabelFunction.SetMinTabWidth(100);
	CRect rcSubDlg, rcTabItem;
	this->GetClientRect(rcSubDlg);
	m_TAB_CLabelFunction.GetItemRect(0, rcTabItem);
	rcSubDlg.top = rcSubDlg.top + rcTabItem.Height() + 3;
	rcSubDlg.left += 1;
	rcSubDlg.right -= 1;
	rcSubDlg.bottom -= 1;



	m_pPageSystemParam = new CPageSystemParam;//数据信息
	m_pPageSystemParam->Create(CPageSystemParam::IDD, &m_TAB_CLabelFunction);
	m_pPageSystemParam->MoveWindow(rcSubDlg);
	m_pPageSystemParam->ShowWindow(SW_NORMAL);
	m_pPageSystemParam->ResetSize();
	m_vecPages.push_back(m_pPageSystemParam);

	//m_pPageCount = new CPageCount;//计数
	//m_pPageCount->Create(CPageCount::IDD, &m_TAB_CLabelFunction);
	//m_pPageCount->MoveWindow(rcSubDlg);
	//m_pPageCount->ShowWindow(SW_HIDE);
	//m_pPageCount->ResetSize();
	//m_vecPages.push_back(m_pPageCount);

	//m_pPageControlParam = new CPageControlParam;//控制开关
	//m_pPageControlParam->Create(CPageControlParam::IDD, &m_TAB_CLabelFunction);
	//m_pPageControlParam->MoveWindow(rcSubDlg);
	//m_pPageControlParam->ShowWindow(SW_HIDE);
	//m_pPageControlParam->m_pChildOwner=m_pOwner;
	//m_pPageControlParam->ResetSize();
	//m_vecPages.push_back(m_pPageControlParam);

	m_pPageWarn = new CPageWarn;//控制开关
	m_pPageWarn->Create(CPageWarn::IDD, &m_TAB_CLabelFunction);
	m_pPageWarn->MoveWindow(rcSubDlg);
	m_pPageWarn->ShowWindow(SW_HIDE);
	m_pPageWarn->ResetSize();
	m_vecPages.push_back(m_pPageWarn);


	m_COM_nLabelName=m_pOwner->m_tLabellerInfo.nEnableLabelName;
	SetTimer(UM_TIMER_FLUSH,2000,NULL);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLabelInfoDisp::ResetSize()
{
	int nSrcX, nSrcY;
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);
	GetDlgItem(IDC_TAB_LABELFUNCTION)->MoveWindow(rtSize);
	m_TAB_CLabelFunction.InsertItem(0, "系统参数");
	m_TAB_CLabelFunction.InsertItem(1, "控制参数");
	m_TAB_CLabelFunction.InsertItem(2, "打标统计");
	m_TAB_CLabelFunction.InsertItem(3, "报警信息");
	m_TAB_CLabelFunction.SetTextFont("Microsoft Yahei", 19, 0);
	m_TAB_CLabelFunction.SetColor(RGB(220, 224, 236), RGB(253, 244, 191), RGB(253, 244, 191), RGB(220, 224, 236), RGB(0, 0, 0), RGB(0, 0, 0));
	m_TAB_CLabelFunction.SetItemSize(CSize(140, 30));
	m_TAB_CLabelFunction.SetMinTabWidth(100);
	CRect rcSubDlg, rcTabItem;
	this->GetClientRect(rcSubDlg);
	m_TAB_CLabelFunction.GetItemRect(0, rcTabItem);
	rcSubDlg.top = rcSubDlg.top + rcTabItem.Height() + 3;
	rcSubDlg.left += 1;
	rcSubDlg.right -= 1;
	rcSubDlg.bottom -= 1;
}



void CDlgLabelInfoDisp::FlushList()
{

	
	int nDM=62;//OAP色标
    float dValue=0;//默认低电平
	m_pOwner->m_CFinsTcp.ReadFloat32(nDM,dValue);
	if(dValue==0)
	{
		m_STC_COAPSebiao1.SetBkColor(COLORREF(RGB(255,0,0)));
	}else
	{
		m_STC_COAPSebiao1.SetBkColor(COLORREF(RGB(0,255,0)));
	}

	nDM=64;//对射传感器
	m_pOwner->m_CFinsTcp.ReadFloat32(nDM,dValue);
	if(dValue==0)
	{
		m_STC_COAPSebiao2.SetBkColor(COLORREF(RGB(255,0,0)));
	}else
	{
		m_STC_COAPSebiao2.SetBkColor(COLORREF(RGB(0,255,0)));
	}

	////计数
	HTuple hv_Name,hv_Value;




	/////Param
	hv_Name=m_pOwner->m_hvSystemParamName;
	hv_Value=m_pOwner->m_hvSystemParam;
	m_pOwner->m_hvSystemParamName.Clear();
	m_pOwner->m_hvSystemParam.Clear();
	m_pPageSystemParam->UpdateList(hv_Name,hv_Value);


	//////报警信息标
	HTuple hv_ResultFlag,hv_WarnClass;
	hv_ResultFlag=m_pOwner->m_hvStatusFlag;
	hv_WarnClass=m_pOwner->m_hvWarnNames;
	m_pOwner->m_hvStatusFlag.Clear();
	m_pOwner->m_hvWarnNames.Clear();
	m_pPageWarn->UpdateList(hv_WarnClass,hv_ResultFlag);


}






void CDlgLabelInfoDisp::InsertMessage(CString strWarnMessage, int nLightColor )
{
	CString strValue;
	SYSTEMTIME tSystemTime;
	GetLocalTime(&tSystemTime);
	strValue.Format("%02d:%02d:%02d.%03d :%s",tSystemTime.wHour,tSystemTime.wMinute,tSystemTime.wSecond,tSystemTime.wMilliseconds,strWarnMessage);
}



void CDlgLabelInfoDisp::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if(UM_TIMER_FLUSH==nIDEvent)
	{
		CString strValue;
		HTuple hv_ResultFlag,hv_WarnClass;
		strValue.Format("贴标机IP:%s   端口:%d",m_pOwner->m_tLabellerInfo.strLabelIpAddress,m_pOwner->m_tLabellerInfo.nLabelPort);
		if (m_pOwner->m_CFinsTcp.m_bConnected)
		{
			m_STC_strIp.SetBkColor(COLORREF(RGB(0,255,0)));
			m_STC_strIp.SetWindowText(strValue);
		}
		else
		{
			m_bConnected = FALSE;
			InsertMessage("贴标机连接断开");
			m_STC_strIp.SetBkColor(COLORREF(RGB(255,0,0)));
		    m_STC_strIp.SetWindowText(strValue);
		}
	
       PostMessageA(WM_DISPLISTMESSGE,0,0);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDlgLabelInfoDisp::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pOwner->LabelReset();
	m_pOwner->m_bWarnStaus=FALSE;
	InsertMessage("贴标机复位");
}


void CDlgLabelInfoDisp::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pOwner->m_tLabellerInfo.nEnableLabelName=m_COM_nLabelName;
	m_pOwner->WriteParam();
	CDialogEx::OnOK();
}


void CDlgLabelInfoDisp::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


//void CDlgLabelInfoDisp::OnBnClickedBtnReturn()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnCancel();
//}


void CDlgLabelInfoDisp::OnBnClickedBtnClearcount()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{
		int nDM=128;
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDM,dValue);
		InsertMessage("编码器辊抬起");
	}
}


void CDlgLabelInfoDisp::OnBnClickedButtononemark()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{
		int nDM=132;
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDM,dValue);

	}

}
void CDlgLabelInfoDisp::OnBnClickedButtontwomark()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		m_pOwner->m_CFinsTcp.WriteInt32(m_pOwner->m_tLabellerInfo.nManual1_ZD_SD_Address,102);


	}
}

//	编码器压辊气缸上升_CCD写入	D128
//	编码器压辊气缸下降_CCD写入	D130
void CDlgLabelInfoDisp::OnBnClickedButtononezd()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{
		int nDM=130;
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDM,dValue);
	
	}
}


void CDlgLabelInfoDisp::OnBnClickedButtontwozd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		m_pOwner->m_CFinsTcp.WriteInt32(m_pOwner->m_tLabellerInfo.nManual1_ZD_SD_Address,2);


	}
}


void CDlgLabelInfoDisp::OnBnClickedBtnRead()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{


		m_pOwner->m_CFinsTcp.ReadFloat32(m_EDT_nTestAddress,m_EDT_dTestValue);
		UpdateData(FALSE);
		AfxMessageBox("读取成功");
	}
}


void CDlgLabelInfoDisp::OnBnClickedBtnWrite()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		m_pOwner->m_CFinsTcp.WriteFloat32(m_EDT_nTestAddress,m_EDT_dTestValue);
		AfxMessageBox("写入成功");
	}
}


void CDlgLabelInfoDisp::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码

		CString strHelpFile;
		strHelpFile.Format("%s\\分条机PLC地址.pdf", GetCurrentAppPath());
		if (!PathFileExists(strHelpFile))
		{
			CString strMessage;
			strMessage.Format("无法打开贴标机地址.pdf，请检查");
			
			return;
		}
		CStdioFile	 helpFile;
		helpFile.Open(_T(strHelpFile), CFile::modeWrite/*|CFile::typeText*/, NULL);
	
		if (helpFile.m_hFile == CFile::hFileNull)
		{
			CString strMessage;
			strMessage.Format("贴标机地址已经打开，请检查");
			
			return;
		}
		helpFile.Close();
		HINSTANCE hInstance = ShellExecute(NULL, _T("open"), _T(strHelpFile), NULL, NULL, SW_SHOW);
		ASSERT(hInstance != NULL);


}


void CDlgLabelInfoDisp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


//void CDlgLabelInfoDisp::OnBnClickedBtnReadspeed()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	if (m_pOwner->m_CFinsTcp.m_bConnected)
//	{
//		int  nAddress = 3008;//
//		float fspeed = 0;
//		m_pOwner->m_CFinsTcp.ReadFloat32(nAddress, fspeed);
//		CString strMessage;
//		strMessage.Format("当前贴标机的编码器速度为:%.1fm/mmin", fspeed);
//		BCGPMessageBox(strMessage);
//	}
//}

void CDlgLabelInfoDisp::SwitchDispPage(const int & nSN)
{
	if (nSN >= m_vecPages.size())return;

	for (int i = 0; i<m_vecPages.size(); i++)
	{
		if (nSN == i)
			m_vecPages[i]->ShowWindow(SW_NORMAL);
		else
			m_vecPages[i]->ShowWindow(SW_HIDE);
	}
}
void CDlgLabelInfoDisp::OnSelchangeTabLabelfunction(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	SwitchDispPage(m_TAB_CLabelFunction.GetCurSel());
}






void CDlgLabelInfoDisp::OnBnClickedBtnQgyx()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{
		int nDMSQGYX=122;//
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDMSQGYX,dValue);
		
	}



}


void CDlgLabelInfoDisp::OnBnClickedBtnTqqg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		int nDMSQGTQ=120;//
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDMSQGTQ,dValue);


	}
}


afx_msg LRESULT CDlgLabelInfoDisp::OnDisplistmessge(WPARAM wParam, LPARAM lParam)
{
	static BOOL s_bFlag=FALSE;
	if (s_bFlag==TRUE)
	{
		return 0;
	}
	s_bFlag=TRUE;
	FlushList();
	s_bFlag=FALSE;
	return 0;
}


void CDlgLabelInfoDisp::OnBnClickedBtnReadspeed()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{
		int  nAddress = 20;//
		float fspeed = 0;
		m_pOwner->m_CFinsTcp.ReadFloat32(nAddress, fspeed);
		CString strMessage;
		strMessage.Format("当前贴标机的编码器速度为:%.2f m/mmin", fspeed);
		AfxMessageBox(strMessage);
	}
}

//上带路打标气缸上升_CCD写入	D120
//	上带路打标气缸下降_CCD写入	D122
//	下带路打标气缸上升_CCD写入	D124
//	下带路打标气缸下降_CCD写入	D126
//	编码器压辊气缸上升_CCD写入	D128
//	编码器压辊气缸下降_CCD写入	D130
void CDlgLabelInfoDisp::OnBnClickedBtnXdlqgyx()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		int nDMXQGYX=126;//
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDMXQGYX,dValue);


	}
}


void CDlgLabelInfoDisp::OnBnClickedBtnXdltqqg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_pOwner->m_CFinsTcp.m_bConnected)
	{

		int nDMXQGYX=124;//
		float dValue=1;
		m_pOwner->m_CFinsTcp.WriteFloat32(nDMXQGYX,dValue);


	}
}
