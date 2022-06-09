// DlgSet.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "LightControl.h"
#include "DlgSet.h"
#include "afxdialogex.h"

// CDlgSet �Ի���
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
	// TODO: �ڴ����ר�ô����/����û���
	return CDialogEx::PreTranslateMessage(pMsg);
}

// CDlgSet ��Ϣ�������
BOOL CDlgSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//EnableVisualManagerStyle(TRUE, TRUE);//��������ʹ�ó�����ÿ� ��������...���˵�����ӵ�ϵͳ�˵��С�
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitUI();
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
//��ʼ���Ի���
void CDlgSet::InitUI()
{
	m_Open.EnableWindow(TRUE);//�򿪰�ť����ʹ��
	m_Close.EnableWindow(FALSE);//�رհ�ť������ʹ��

	m_Combo_CommType.AddString("����ͨ��");
	m_Combo_CommType.AddString("����ͨ��");
	m_Combo_CommType.SetCurSel(g_This->GetLightParam().eCommType);
	OnCbnSelchangeComboComtype();
    
	CSerialPortExt Port;//���ڶ˿�
	Port.GetComList(m_PortList);//�����ʼ�����Զ���ȡ���ô��ڣ���ȡ�豸�����б�
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


	//����������Ĭ��ֵ
	m_LightType.SetCurSel(g_This->GetLightParam().eLightType);//��Դ�ͺ� 
	if (m_SerialPortList.GetCount() > 0)  //�����б�������
	{
		m_SerialPortList.SetCurSel(g_This->GetLightParam().nComIndex);//�����±�
	}
	OnCbnSelchangeComboLightType();
	m_ChannelList.SetCurSel(0);//Aͨ��

	
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

//���ӹ�Դ���򿪴��ڻ��ߴ����ڣ�
void CDlgSet::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�򿪴���ǰ���ȼ��ش���/����/ͨ�����Ƿ�Ϊ��
	GetDlgItem(IDC_BUTTON_READ)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(TRUE);
	int index1 = m_Combo_CommType.GetCurSel();
	g_This->GetLightParam().eCommType = (eLightCommType)index1;//��ö�ٺͶԻ���ı�����������
	if (index1 == 1) 
	{
		int index = m_SerialPortList.GetCurSel();
		if (index < 0)
		{
			AfxMessageBox("����ѡ����Ϊ��!");
			return;
		}

		m_SerialPortList.GetLBText(index, CurPort);
		g_This->GetLightParam().nComIndex = index;
		g_This->GetLightParam().sComName = CurPort;
		if (g_This->m_nCurrentStatus)
		{
			m_Open.EnableWindow(FALSE);
			m_Close.EnableWindow(TRUE);
			//�༭����ʾ�򿪽��
			CString str;
			str.Format("�ɹ��򿪴���:%s��\r\n", CurPort);
			m_PortRecored += TEXT(str);
		}
		else
		{
			m_PortRecored += TEXT("�򿪴���ʧ�ܣ�\r\n");
		}

		UpdateData(FALSE); // ���������ݣ�ͬ�����ؼ��У�
	}
	else 
	{
		CString strIp,strPort;
		m_Edit_Ip.GetWindowText(strIp);
		if (strIp.IsEmpty()) 
		{
			AfxMessageBox("������IP��ַ��");
		}
		m_Edit_Ip.GetWindowText(strPort);
		if (strPort.IsEmpty())
		{
			AfxMessageBox("������˿ڣ�");
		}

		if (g_This->m_nCurrentStatus)
		{
			m_Open.EnableWindow(FALSE);
			m_Close.EnableWindow(TRUE);
			//�༭����ʾ�򿪽��
			CString str;
			str.Format("�ɹ�������");
			

		}
		else
		{
			AfxMessageBox("������ʧ�ܣ�");
		}

		UpdateData(FALSE); // ���������ݣ�ͬ�����ؼ��У�
	}
}

//�Ͽ���Դ���رչ�Դ���ڻ��߹ر����ڣ�
void CDlgSet::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (g_This->Close())
	{
		m_ChannelList.SetCurSel(-1);
		m_Open.EnableWindow(TRUE);
		m_Close.EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_READ)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WRITE)->EnableWindow(FALSE);
	}	
	
}

//��Դͨ��������ѡ���¼���Aͨ����Bͨ����
void CDlgSet::OnCbnSelchangeComboChaanelList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index =  m_ChannelList.GetCurSel();
	if (index < 0)
	{
		AfxMessageBox("ͨ��ѡ���Ϊ�գ�");
		return;
	}
	m_ChannelList.GetLBText(index, CurChannel);
	//��ȡ��ǰͨ����Ĭ������ֵ������ʼ��������λ��
	int light;
	CString str;
	light = g_This->GetBrightnes(index + 1);
	if (light == -1)
	{
		str.Format("��ȡͨ��%d������ֵ�쳣:%d \r\n", index + 1, 0);
		m_PortRecored += str;
		UpdateData(FALSE); // ���������ݣ�ͬ�����ؼ��У�
		return;
	}
	m_Light_Value_Edit = light;
    str.Format("��ǰѡ��ͨ��:%c,����ֵ:%d \r\n",'A'+index, light);
	m_PortRecored += str;
	UpdateData(FALSE); // ���������ݣ�ͬ�����ؼ��У�
		
}

void CDlgSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	/*CDialogEx*/CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������

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

//��Դ�ͺţ�������Դ��DH-HAPS-48W800-1T��DH-HAPS-48W800-2T��CST_CPL48320CDP_2TD��
void CDlgSet::OnCbnSelchangeComboLightType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//ȷ����ť
void CDlgSet::OnBnClickedButton2()
{
	if (!m_bEnable)
	{
		AfxMessageBox("û��Ȩ�ޣ�");
		//((CSeekersDlg*)g_pMainDialog)->Login();
		return;
	}

	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//ͨ���ͺţ����ڻ�����
void CDlgSet::OnCbnSelchangeComboComtype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int index = m_Combo_CommType.GetCurSel();
	g_This->GetLightParam().eCommType = (eLightCommType)index;
	//CString temp_value = _T("");   //temp_value��������intֵ
	//temp_value.Format(_T("%d"), index);//�̶���ʽ
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

//����������ѡ���¼���COM1��COM2
void CDlgSet::OnCbnSelchangeComboSerialList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_SerialPortList.GetCurSel();
	m_SerialPortList.GetLBText(index, CurPort);

}
//��ȡ����
void CDlgSet::OnBnClickedButtonRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_ChannelList.GetCurSel();
	m_Light_Value_Edit = g_This->GetBrightnes( index+1);
	if (m_Light_Value_Edit == -1)
	{
		AfxMessageBox("��ȡ����ʧ�ܣ�");
	}
	CString str;
	str.Format("��ȡ���ȳɹ���ͨ��:%c,����ֵ:%d \r\n", 'A' + index, m_Light_Value_Edit);
	m_PortRecored += str;
	UpdateData(FALSE);
}
//��ȡ����
void CDlgSet::OnBnClickedButtonWrite()
{
	if (!m_bEnable)
	{
		AfxMessageBox("û��Ȩ�ޣ�");
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int index = m_ChannelList.GetCurSel();
	BOOL bRes =  g_This->SetBrightness(index + 1, m_Light_Value_Edit); 
	if ( bRes ==false) 
	{
		AfxMessageBox("д������ʧ�ܣ�");
	}
	CString str;
	str.Format("�������ȳɹ���ͨ��:%c,����ֵ:%d \r\n", 'A' + index, m_Light_Value_Edit);
	m_PortRecored += str;
	UpdateData(FALSE);
}

void CDlgSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Light_Value_Edit = 0;
	CDialogEx::OnCancel();
	UpdateData(FALSE);
}
