// DlgSet.cpp : 实现文件
//
#include "stdafx.h"
#include "LightControl.h"
#include "DlgSet.h"
#include "afxdialogex.h"

// CDlgSet 对话框
IMPLEMENT_DYNAMIC(CDlgSet, CDialogEx)

CDlgSet::CDlgSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSet::IDD, pParent)
	, m_PortRecored(_T(""))
	, m_Light_Value_Edit(0)
	, m_EDIT_strIp(_T(""))
	, m_EDIT_nPort(0)
{

	m_EDT_nGoalGray = 0;
	m_EDT_nGrayDev = 0;
	m_EDT_nSampleFrames = 0;
	m_EDT_nLightUp = 200;
	m_EDT_nLightDown = 100;
}

CDlgSet::~CDlgSet()
{
}

void CDlgSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_SERIAL_LIST, m_SerialPortList);
	DDX_Control(pDX, IDC_COMBO_CHAANEL_LIST, m_ChannelList);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_Open);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_Close);
	DDX_Text(pDX, IDC_EDIT_SERIAL_RECORED, m_PortRecored);
	DDX_Control(pDX, IDC_COMBO_LIGHT_TYPE, m_LightType);
	DDX_Text(pDX, IDC_LIGHT_VALUE_EDIT, m_Light_Value_Edit);
	DDX_Control(pDX, IDC_COMBO_COMTYPE, m_Combo_CommType);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Edit_Port);
	DDX_Control(pDX, IDC_EDIT_IP, m_Edit_Ip);
	DDX_Text(pDX, IDC_EDIT_IP, m_EDIT_strIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_EDIT_nPort);
	DDX_Check(pDX, IDC_CHK_ENABLELIGHTADJUST, m_CHK_bUseLightAdjust);
	DDX_Text(pDX, IDC_EDT_GOALGRAY, m_EDT_nGoalGray);
	//DDV_MinMaxInt(pDX, m_EDT_nGoalGray, 20, 150);
	DDX_Text(pDX, IDC_EDT_GRAYDEV, m_EDT_nGrayDev);
	DDX_Text(pDX, IDC_EDT_SAMPLEFRAME, m_EDT_nSampleFrames);
	//DDV_MinMaxInt(pDX, m_EDT_nSampleFrames, 2, 30);
	DDX_Text(pDX, IDC_EDIT_LIGHTUP, m_EDT_nLightUp);
	DDX_Text(pDX, IDC_EDIT_LIGHTDOWN, m_EDT_nLightDown);
}

BEGIN_MESSAGE_MAP(CDlgSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL_LIST, &CDlgSet::OnCbnSelchangeComboSerialList)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CDlgSet::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgSet::OnBnClickedButtonClose)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_CHAANEL_LIST, &CDlgSet::OnCbnSelchangeComboChaanelList)
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_LIGHT_TYPE, &CDlgSet::OnCbnSelchangeComboLightType)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgSet::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO_COMTYPE, &CDlgSet::OnCbnSelchangeComboComtype)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CDlgSet::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CDlgSet::OnBnClickedButtonWrite)
	                                    
	ON_BN_CLICKED(IDCANCEL, &CDlgSet::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CDlgSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CDialogEx::PreTranslateMessage(pMsg);
}

// CDlgSet 消息处理程序
BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//EnableVisualManagerStyle(TRUE, TRUE);//美化程序，使得程序更好看 将“关于...”菜单项添加到系统菜单中。
	// TODO: 在此添加额外的初始化代码
	InitUI();
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
//初始化对话框
void CDlgSet::InitUI()
{
	m_Open.EnableWindow(TRUE);//打开按钮可以使用
	m_Close.EnableWindow(FALSE);//关闭按钮不可以使用

	m_Combo_CommType.AddString("网口通信");
	m_Combo_CommType.AddString("串口通信");
	m_Combo_CommType.SetCurSel(g_This->GetLightParam().eCommType);
	OnCbnSelchangeComboComtype();
    
	CSerialPortExt Port;//串口端口
	Port.GetComList(m_PortList);//程序初始化，自动获取可用串口，获取设备串口列表
	for (int i = 0; i < m_PortList.size(); i++)
	{
		m_SerialPortList.AddString(TEXT(m_PortList[i]));
	}

	m_EDIT_strIp = g_This->GetLightParam().sIP;
	m_EDIT_nPort = g_This->GetLightParam().nTcpPort;
	m_CHK_bUseLightAdjust=g_This->GetLightParam().bUseLightAdjust;
	m_EDT_nGrayDev=g_This->GetLightParam().nGraySafeDev;
	m_EDT_nGoalGray=g_This->GetLightParam().nGoalGray;
	m_EDT_nSampleFrames=g_This->GetLightParam().nSampleFrames;
	m_EDT_nLightUp = g_This->GetLightParam().nLightUp;
	m_EDT_nLightDown = g_This->GetLightParam().nLightDown;


	//设置下拉框默认值
	m_LightType.SetCurSel(g_This->GetLightParam().eLightType);//光源型号 
	if (m_SerialPortList.GetCount() > 0)  //串口列表下拉框
	{
		m_SerialPortList.SetCurSel(g_This->GetLightParam().nComIndex);//串口下标
	}
	OnCbnSelchangeComboLightType();
	m_ChannelList.SetCurSel(0);//A通道

	
	if(g_This->m_nCurrentStatus)
	{
		m_Open.EnableWindow(FALSE);
		m_Close.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
	}
	else
	{
		m_Open.EnableWindow(TRUE);
		m_Close.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IP)->EnableWindow(FALSE);
	}
		
	UpdateData(FALSE);
}

//连接光源（打开串口或者打开网口）
void CDlgSet::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开串口前，先加载串口/波特/通道等是否为空
	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(TRUE);
	int index1 = m_Combo_CommType.GetCurSel();
	g_This->GetLightParam().eCommType = (eLightCommType)index1;//将枚举和对话框的变量关联起来
	if (index1 == 1) 
	{
		int index = m_SerialPortList.GetCurSel();
		if (index < 0)
		{
			AfxMessageBox("串口选择不能为空!");
			return;
		}

		m_SerialPortList.GetLBText(index, CurPort);
		g_This->GetLightParam().nComIndex = index;
		g_This->GetLightParam().sComName = CurPort;
		if (g_This->m_nCurrentStatus)
		{
			m_Open.EnableWindow(FALSE);
			m_Close.EnableWindow(TRUE);
			//编辑框显示打开结果
			CString str;
			str.Format("成功打开串口:%s！\r\n", CurPort);
			m_PortRecored += TEXT(str);
		}
		else
		{
			m_PortRecored += TEXT("打开串口失败！\r\n");
		}

		UpdateData(FALSE); // 将变量内容，同步到控件中；
	}
	else 
	{
		CString strIp,strPort;
		m_Edit_Ip.GetWindowText(strIp);
		if (strIp.IsEmpty()) 
		{
			AfxMessageBox("请输入IP地址！");
		}
		m_Edit_Ip.GetWindowText(strPort);
		if (strPort.IsEmpty())
		{
			AfxMessageBox("请输入端口！");
		}

		if (g_This->m_nCurrentStatus)
		{
			m_Open.EnableWindow(FALSE);
			m_Close.EnableWindow(TRUE);
			//编辑框显示打开结果
			CString str;
			str.Format("成功打开网口");
			

		}
		else
		{
			AfxMessageBox("打开网口失败！");
		}

		UpdateData(FALSE); // 将变量内容，同步到控件中；
	}
}

//断开光源（关闭光源串口或者关闭网口）
void CDlgSet::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_This->Close())
	{
		m_ChannelList.SetCurSel(-1);
		m_Open.EnableWindow(TRUE);
		m_Close.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(FALSE);
	}	
	
}

//光源通道下拉框选择事件（A通道，B通道）
void CDlgSet::OnCbnSelchangeComboChaanelList()
{
	// TODO: 在此添加控件通知处理程序代码
	int index =  m_ChannelList.GetCurSel();
	if (index < 0)
	{
		AfxMessageBox("通道选项不能为空！");
		return;
	}
	m_ChannelList.GetLBText(index, CurChannel);
	//读取当前通道的默认亮度值，并初始化滑动条位置
	int light;
	CString str;
	light = g_This->GetBrightnes(index + 1);
	if (light == -1)
	{
		str.Format("获取通道%d的亮度值异常:%d \r\n", index + 1, 0);
		m_PortRecored += str;
		UpdateData(FALSE); // 将变量内容，同步到控件中；
		return;
	}
	m_Light_Value_Edit = light;
    str.Format("当前选中通道:%c,亮度值:%d \r\n",'A'+index, light);
	m_PortRecored += str;
	UpdateData(FALSE); // 将变量内容，同步到控件中；
		
}

void CDlgSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	/*CDialogEx*/CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码

	if (WA_INACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE
			, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0, (255 * 90) / 100, LWA_ALPHA);
	}
	else if (WA_CLICKACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE
			, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
	else if (WA_ACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE
			, ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
}

//光源型号（三个光源：DH-HAPS-48W800-1T、DH-HAPS-48W800-2T、CST_CPL48320CDP_2TD）
void CDlgSet::OnCbnSelchangeComboLightType()
{
	// TODO: 在此添加控件通知处理程序代码
	int index =  m_LightType.GetCurSel();
	m_LightType.GetLBText(index, CurLightType);
	int num = m_LightType.GetCount();
	for(int i = 0;i<num;i++)
	{
		m_ChannelList.DeleteString(num-i-1);
	}	
	if("DH-HAPS-48W800-1T" == CurLightType)
	{
		m_ChannelList.AddString("A");
	}
	else if("DH-HAPS-48W800-2T" == CurLightType)
	{
		m_ChannelList.AddString("A");
		m_ChannelList.AddString("B");
	}
	else if ("CST-CPL48320CDP-2TD" == CurLightType)
	{
		m_ChannelList.AddString("A");
		m_ChannelList.AddString("B");
	}

	m_ChannelList.SetCurSel(0);
	UpdateData(0);
}

//确定按钮
void CDlgSet::OnBnClickedButton2()
{
	if (!m_bEnable)
	{
		AfxMessageBox("没有权限！");
		//((CSeekersDlg*)g_pMainDialog)->Login();
		return;
	}

	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	g_This->Close();
	int index = m_SerialPortList.GetCurSel();	
	g_This->GetLightParam().nComIndex = (eLightCommType)index;
	index = m_SerialPortList.GetCurSel();
	g_This->GetLightParam().nComIndex = index;

	if (index > 0)
		m_SerialPortList.GetLBText(index, g_This->GetLightParam().sComName);
	else
		g_This->GetLightParam().sComName = "";

	    g_This->GetLightParam().eLightType = (eLightControlType)m_LightType.GetCurSel();
		g_This->GetLightParam().sIP = m_EDIT_strIp;
		g_This->GetLightParam().nTcpPort = m_EDIT_nPort;

		g_This->GetLightParam().bUseLightAdjust=m_CHK_bUseLightAdjust;
		g_This->GetLightParam().nGraySafeDev=m_EDT_nGrayDev;
		g_This->GetLightParam().nGoalGray=m_EDT_nGoalGray;
		g_This->GetLightParam().nSampleFrames=m_EDT_nSampleFrames;

		g_This->GetLightParam().eCommType =(eLightCommType)m_Combo_CommType.GetCurSel();
		g_This->GetLightParam().nLightUp = m_EDT_nLightUp;
		g_This->GetLightParam().nLightDown = m_EDT_nLightDown;
	    g_This->SaveParams();

	
	   CDialogEx::OnOK();
	
}

//通信型号：串口或网口
void CDlgSet::OnCbnSelchangeComboComtype()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index = m_Combo_CommType.GetCurSel();
	g_This->GetLightParam().eCommType = (eLightCommType)index;
	//CString temp_value = _T("");   //temp_value用来处理int值
	//temp_value.Format(_T("%d"), index);//固定格式
    // AfxMessageBox(temp_value);
	if (index ==0 )
	{
        m_SerialPortList.EnableWindow(FALSE);
		m_Edit_Ip.EnableWindow(TRUE);
		m_Edit_Port.EnableWindow(TRUE);
	}
	else
	{
		m_Edit_Ip.EnableWindow(FALSE);
		m_Edit_Port.EnableWindow(FALSE);
		m_SerialPortList.EnableWindow(TRUE);
	}
}

//串口下拉框选择事件，COM1、COM2
void CDlgSet::OnCbnSelchangeComboSerialList()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_SerialPortList.GetCurSel();
	m_SerialPortList.GetLBText(index, CurPort);

}
//读取亮度
void CDlgSet::OnBnClickedButtonRead()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ChannelList.GetCurSel();
	m_Light_Value_Edit = g_This->GetBrightnes( index+1);
	if (m_Light_Value_Edit == -1)
	{
		AfxMessageBox("读取亮度失败！");
	}
	CString str;
	str.Format("读取亮度成功，通道:%c,亮度值:%d \r\n", 'A' + index, m_Light_Value_Edit);
	m_PortRecored += str;
	UpdateData(FALSE);
}
//获取亮度
void CDlgSet::OnBnClickedButtonWrite()
{
	if (!m_bEnable)
	{
		AfxMessageBox("没有权限！");
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int index = m_ChannelList.GetCurSel();
	BOOL bRes =  g_This->SetBrightness(index + 1, m_Light_Value_Edit); 
	if ( bRes ==false) 
	{
		AfxMessageBox("写入亮度失败！");
	}
	CString str;
	str.Format("设置亮度成功，通道:%c,亮度值:%d \r\n", 'A' + index, m_Light_Value_Edit);
	m_PortRecored += str;
	UpdateData(FALSE);
}

void CDlgSet::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Light_Value_Edit = 0;
	CDialogEx::OnCancel();
	UpdateData(FALSE);
}
