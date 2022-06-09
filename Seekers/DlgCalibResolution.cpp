// DlgCalibResolution.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgCalibResolution.h"
#include "afxdialogex.h"


// CDlgCalibResolution �Ի���

IMPLEMENT_DYNAMIC(CDlgCalibResolution, CDialogEx)

CDlgCalibResolution::CDlgCalibResolution(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibResolution::IDD, pParent)
{

	m_EDT_dFirstMeasureLength = 0.0;
	m_EDT_dFirstMeasureWidth = 0.0;
	m_EDT_dFirstColumnResolution = 0.0;
	m_EDT_dRealMeasureLength = 0.0;
	m_EDT_dRealMeasureWidth = 0.0;
	m_EDT_dSecondMeasureWidth = 0.0;
	m_EDT_dSecondColumnResolution = 0.0;
	m_EDT_dVerticalResolution = 0.0;
	m_dFirstCalibColumnResolution=0;
	m_dSecondCalibColumnResolution=0;
	m_dCalibRowResolution=0;
	m_EDT_dOmmValue1 =120;
	m_EDT_dOmmValue2 = 120;
	m_EDT_dOmmValue3 = 120;
	m_EDT_dOmmValue4 = 120;
	m_EDT_dOmmValue5 = 120;
	m_EDT_dOmmValue6 = 120;
	m_EDT_dOmmValue7 = 120;
	m_EDT_dOmmValue8 = 120;
	m_EDT_dCCDMeasureValue1 = 120;
	m_EDT_dCCDMeasureValue2 = 120;
	m_EDT_dCCDMeasureValue3 = 120;
	m_EDT_dCCDMeasureValue4 = 120;
	m_EDT_dCCDMeasureValue5 = 120;
	m_EDT_dCCDMeasureValue6 = 120;
	m_EDT_dCCDMeasureValue7 = 120;
	m_EDT_dCCDMeasureValue8 = 120;
	m_EDT_dCurrent_K = 0.0;
	m_EDT_dCurrentThickness = 0.001;
	m_EDT_dEncoderDiameter = 120;
	m_EDT_dStandard_K0 = 0.0;
	m_EDT_dStandardThickness = 0.001;
	m_COM_nEnableThickness = 0;
}

CDlgCalibResolution::~CDlgCalibResolution()
{
}

void CDlgCalibResolution::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FIRSTCAMERALENGTH, m_EDT_dFirstMeasureLength);
	DDX_Text(pDX, IDC_EDT_FIRSTCAMERAWIDTH, m_EDT_dFirstMeasureWidth);
	DDX_Text(pDX, IDC_EDT_FIRSTRESOLUTION, m_EDT_dFirstColumnResolution);
	DDX_Text(pDX, IDC_EDT_REALLENGTH, m_EDT_dRealMeasureLength);
	DDX_Text(pDX, IDC_EDT_REALWIDTH, m_EDT_dRealMeasureWidth);
	DDX_Text(pDX, IDC_EDT_SECONDCAMERAWIDTH, m_EDT_dSecondMeasureWidth);
	DDX_Text(pDX, IDC_EDT_SECONDRESOLUTION, m_EDT_dSecondColumnResolution);
	DDX_Text(pDX, IDC_EDT_VERTICALRESOLUTION, m_EDT_dVerticalResolution);
	DDX_Control(pDX, IDC_STC_TITLE, m_STC_CTitle);
	DDX_Control(pDX, IDC_BTN_JUSTCALIBBWIDTH, m_BTN_CJustCalibWidth);
	DDX_Control(pDX, IDC_BTN_JUSTCALIBLENGTH, m_BTN_CJustCalibLength);
	DDX_Control(pDX, IDC_BTN_ONEKEYCALIB, m_BTN_CCalibALL);
	DDX_Control(pDX, IDC_BTN_SAVE, m_BTN_CWriteCalibResult);
	DDX_Control(pDX, IDC_BTN_RETURN, m_BTN_CReturn);
	DDX_Text(pDX, IDC_EDT_OMM1, m_EDT_dOmmValue1);
	DDX_Text(pDX, IDC_EDT_OMM2, m_EDT_dOmmValue2);
	DDX_Text(pDX, IDC_EDT_OMM3, m_EDT_dOmmValue3);
	DDX_Text(pDX, IDC_EDT_OMM4, m_EDT_dOmmValue4);
	DDX_Text(pDX, IDC_EDT_OMM5, m_EDT_dOmmValue5);
	DDX_Text(pDX, IDC_EDT_OMM6, m_EDT_dOmmValue6);
	DDX_Text(pDX, IDC_EDT_OMM7, m_EDT_dOmmValue7);
	DDX_Text(pDX, IDC_EDT_OMM8, m_EDT_dOmmValue8);
	DDX_Text(pDX, IDC_CCD_MESURE1, m_EDT_dCCDMeasureValue1);
	DDX_Text(pDX, IDC_CCD_MESURE2, m_EDT_dCCDMeasureValue2);
	DDX_Text(pDX, IDC_CCD_MESURE3, m_EDT_dCCDMeasureValue3);
	DDX_Text(pDX, IDC_CCD_MESURE4, m_EDT_dCCDMeasureValue4);
	DDX_Text(pDX, IDC_CCD_MESURE5, m_EDT_dCCDMeasureValue5);
	DDX_Text(pDX, IDC_CCD_MESURE6, m_EDT_dCCDMeasureValue6);
	DDX_Text(pDX, IDC_CCD_MESURE7, m_EDT_dCCDMeasureValue7);
	DDX_Text(pDX, IDC_CCD_MESURE8, m_EDT_dCCDMeasureValue8);
	DDX_Text(pDX, IDC_EDT_CURRENTMOVEDIRECTION_K, m_EDT_dCurrent_K);
	DDX_Text(pDX, IDC_EDT_CURRENTYHICKNESS, m_EDT_dCurrentThickness);
	DDX_Text(pDX, IDC_EDT_ENCODERDIAMETER, m_EDT_dEncoderDiameter);
	DDX_Text(pDX, IDC_EDT_STANDARDMOVEDIRECT_K0, m_EDT_dStandard_K0);
	DDX_Text(pDX, IDC_EDT_STANDARDTHICKNESS, m_EDT_dStandardThickness);
	DDX_CBIndex(pDX, IDC_COM_ENABLEUSETHICKNESS, m_COM_nEnableThickness);
}


BEGIN_MESSAGE_MAP(CDlgCalibResolution, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_ONEKEYCALIB, &CDlgCalibResolution::OnBnClickedBtnOnekeycalib)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgCalibResolution::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_RETURN, &CDlgCalibResolution::OnBnClickedBtnReturn)
	ON_BN_CLICKED(IDC_BTN_JUSTCALIBBWIDTH, &CDlgCalibResolution::OnBnClickedBtnJustcalibbwidth)
	ON_BN_CLICKED(IDC_BTN_JUSTCALIBLENGTH, &CDlgCalibResolution::OnBnClickedBtnJustcaliblength)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CALCULATE_K0, &CDlgCalibResolution::OnBnClickedBtnCalculateK0)
	ON_BN_CLICKED(IDC_BTN_SAVE_K0, &CDlgCalibResolution::OnBnClickedBtnSaveK0)
//	ON_EN_CHANGE(IDC_EDT_CURRENTYHICKNESS, &CDlgCalibResolution::OnEnChangeEdtCurrentyhickness)
	ON_EN_KILLFOCUS(IDC_EDT_CURRENTYHICKNESS, &CDlgCalibResolution::OnEnKillfocusEdtCurrentyhickness)
END_MESSAGE_MAP()


// CDlgCalibResolution ��Ϣ�������


BOOL CDlgCalibResolution::OnInitDialog()
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
	m_BTN_font.CreatePointFont(100,"Microsoft YaHei");

	SetButtonColors(m_BTN_CJustCalibWidth,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CJustCalibLength,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CCalibALL,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CWriteCalibResult,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CReturn,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	m_BTN_CReturn.SetIcon(IDI_ICON_W_UNCHECK);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_EDT_dFirstMeasureLength=g_tStoreInfo.dSumLength;
	m_EDT_dFirstMeasureWidth=g_tStoreInfo.dFirstWidth;
	m_EDT_dSecondMeasureWidth=g_tStoreInfo.dSecondWidth;


	m_EDT_dFirstColumnResolution=g_vec_ColumnResolution[0];
	m_EDT_dSecondColumnResolution=g_vec_ColumnResolution[1];
	m_EDT_dVerticalResolution=g_vec_RowResolution[0];


	CString  strSection;
	strSection.Format("GRABBER%d",0);
	
	m_COM_nEnableThickness=GetPrivateProfileInt("��ֵ����","EnableThickness",0,g_strConfigCommonFile);
	m_EDT_dStandardThickness=GetPrivateProfileDouble("��ֵ����","StandardThickness",0.01,g_strConfigCommonFile);
	m_EDT_dCurrentThickness=GetPrivateProfileDouble("��ֵ����","CurrentThickness",0.01,g_strConfigCommonFile);
	m_EDT_dEncoderDiameter=GetPrivateProfileDouble("��ֵ����","EncoderDiameter",120,g_strConfigCommonFile);
	m_EDT_dStandard_K0=GetPrivateProfileDouble(strSection,"StandardK0",0.1036,g_strConfigCommonFile);
	if (g_dCoatThickness!=-1&& m_COM_nEnableThickness==2)
	{

		m_EDT_dCurrentThickness=g_dCoatThickness*1000;//ת��Ϊ΢��

	}


	m_EDT_dCurrent_K=m_EDT_dVerticalResolution;
	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgCalibResolution::OnBnClickedBtnOnekeycalib()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	////У׼ǰ�������ֱ���
	double dTempResolution=m_EDT_dFirstColumnResolution;
	if (m_EDT_dFirstMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dFirstColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dFirstMeasureWidth*g_vec_ColumnResolution[0];

	}else
	{
		AfxMessageBox("�˹��������ֵ��ǰCCD�������ֵ������Ϊ0������������");
		return;
	}
	CString strValue;
	strValue.Format("ǰ�����ˮƽKֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dFirstColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	////У׼���������ֱ���
	dTempResolution=m_EDT_dSecondColumnResolution;
	if (m_EDT_dSecondMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dSecondColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dSecondMeasureWidth*g_vec_ColumnResolution[1];

	}else
	{
		AfxMessageBox("�˹��������ֵ�ͺ�CCD�������ֵ������Ϊ0������������");
		return;
	}
	strValue.Format("�������ˮƽKֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dSecondColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);


	////У׼ǰ���������ֱ���
	dTempResolution=m_EDT_dVerticalResolution;
	if (m_EDT_dFirstMeasureLength!=0 && m_EDT_dRealMeasureLength!=0)
	{

		m_EDT_dVerticalResolution=m_EDT_dRealMeasureLength/m_EDT_dFirstMeasureLength*g_vec_RowResolution[0];

	}else
	{
		AfxMessageBox("�˹���������ֵ��CCD�����ܳ�������Ϊ0������������");
		return;
	}
	strValue.Format("���������Kֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dVerticalResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);




	if (m_COM_nEnableThickness!=0)
	{
		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter*1000+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter*1000+m_EDT_dStandardThickness);
	}


	UpdateData(FALSE);










}
void CDlgCalibResolution::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_BTN_font);
}

void CDlgCalibResolution::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	CString strSection;
	CString strValue;
	

	if (m_COM_nEnableThickness!=0)//ȷ�Ͽ�����ȱ궨
	{
		strValue.Format("%.8f",m_EDT_dCurrent_K);
		WritePrivateProfileInt("��ֵ����","EnableThickness",m_COM_nEnableThickness,g_strConfigCommonFile);
		WritePrivateProfileDouble("��ֵ����","CurrentThickness",m_EDT_dCurrentThickness,g_strConfigCommonFile);
	}else
	{
		strValue.Format("%.8f",m_EDT_dVerticalResolution);
	}


	for (int i=0;i<g_nGrabberCount;i++)
	{


		if (i==0)
		{
			strSection.Format("GRABBER%d",i);
			WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dFirstColumnResolution,g_strConfigCommonFile);
		

		}else
		{
			strSection.Format("GRABBER%d",i);
			WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dSecondColumnResolution,g_strConfigCommonFile);
			
		}

	
		WritePrivateProfileString(strSection,"ROWRESOLUTION",strValue,g_strConfigCommonFile);;
	}

	SaveRecord();



    AfxMessageBox("д��ɹ�,�����´����");
}


void CDlgCalibResolution::SaveRecord()
{

	CString strFilePath,strFileDirecor,strItem;
	SYSTEMTIME tSystemTime;
	GetLocalTime(&tSystemTime);
	strFileDirecor.Format("D:\\ccd_data\\%02d-%d",tSystemTime.wYear,tSystemTime.wMonth);
	strFilePath.Format("%s\\�ֱ��ʵ�����¼.csv",strFileDirecor);

	HTuple bExists;
	FileExists(HTuple(strFilePath),&bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strFileDirecor);
		ofstream outFile;
		outFile.open(strFilePath,  ios::out |ios::app);
		strItem.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s","�궨ʱ��","��ʼ�궨���(um)","��ʼ����ϵ��k0","����������ֱ��(um)","��ǰ���Ϻ��(um)","��ǰ����ϵ��K1","ǰ����ֱ���","�����ֱ���","����ֱ���");
		outFile << strItem <<endl;
		outFile.close();
	}
	try
	{
		CString strTime;
		strTime.Format("%02d%02d%02d %02d:%02d:%02d:%03d",tSystemTime.wYear,tSystemTime.wMonth,tSystemTime.wDay,tSystemTime.wHour,tSystemTime.wMinute,tSystemTime.wSecond,tSystemTime.wMilliseconds);
		strItem.Format("%s,%.3f,%.8f,%.3f,%.3f,%.8f,%.8f,%.8f,%.8f",strTime,m_EDT_dStandardThickness,m_EDT_dStandard_K0,m_EDT_dEncoderDiameter*1000,m_EDT_dCurrentThickness,m_EDT_dCurrent_K,m_EDT_dFirstColumnResolution,m_EDT_dSecondColumnResolution,m_EDT_dVerticalResolution);
		ofstream outFile;
		CString strLeftInfo,StrRightInfo;
		outFile.open(strFilePath,  ios::out |ios::app);
		outFile << strItem <<endl;
		outFile.close();
	}catch(HException& e)
	{

	}




}

void CDlgCalibResolution::OnBnClickedBtnReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnCancel();



}


void CDlgCalibResolution::OnBnClickedBtnJustcalibbwidth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	////У׼ǰ�������ֱ���
	double dTempResolution=m_EDT_dFirstColumnResolution;
	if (m_EDT_dFirstMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dFirstColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dFirstMeasureWidth*g_vec_ColumnResolution[0];

	}else
	{
		AfxMessageBox("�˹��������ֵ��ǰCCD�������ֵ������Ϊ0������������");
		return;
	}
	CString strValue;
	strValue.Format("ǰ�����ˮƽKֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dFirstColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	////У׼���������ֱ���
	dTempResolution=m_EDT_dSecondColumnResolution;
	if (m_EDT_dSecondMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dSecondColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dSecondMeasureWidth*g_vec_ColumnResolution[1];

	}else
	{
		AfxMessageBox("�˹��������ֵ�ͺ�CCD�������ֵ������Ϊ0������������");
		return;
	}
	strValue.Format("�������ˮƽKֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dSecondColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	UpdateData(FALSE);
}


void CDlgCalibResolution::OnBnClickedBtnJustcaliblength()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	////У׼ǰ���������ֱ���
	double dTempResolution;
	CString strValue;
	dTempResolution=m_EDT_dVerticalResolution;
	if (m_EDT_dFirstMeasureLength!=0 && m_EDT_dRealMeasureLength!=0)
	{

		m_EDT_dVerticalResolution=m_EDT_dRealMeasureLength/m_EDT_dFirstMeasureLength*g_vec_RowResolution[0];

	}else
	{
		AfxMessageBox("�˹���������ֵ��CCD�����ܳ�������Ϊ0������������");
		return;
	}
	strValue.Format("���������Kֵ��%.8f����Ϊ%.8f",dTempResolution,m_EDT_dVerticalResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);


	UpdateData(FALSE);
}


void CDlgCalibResolution::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()	
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STC_TITLE)->GetClientRect(rcTitle); 
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


HBRUSH CDlgCalibResolution::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CDlgCalibResolution::OnBnClickedBtnCalculateK0()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	double dSumOmmValue=m_EDT_dOmmValue1+m_EDT_dOmmValue2+m_EDT_dOmmValue3+m_EDT_dOmmValue4+m_EDT_dOmmValue5+m_EDT_dOmmValue6+m_EDT_dOmmValue7+m_EDT_dOmmValue8;
	double dSumCCDMeasureValue=m_EDT_dCCDMeasureValue1+m_EDT_dCCDMeasureValue2+m_EDT_dCCDMeasureValue3+m_EDT_dCCDMeasureValue4+m_EDT_dCCDMeasureValue5+m_EDT_dCCDMeasureValue6+m_EDT_dCCDMeasureValue7+m_EDT_dCCDMeasureValue8;

	if (dSumOmmValue!=0 && dSumCCDMeasureValue!=0)
	{
		m_EDT_dStandard_K0=g_vec_RowResolution[0]*dSumOmmValue/dSumCCDMeasureValue;
		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter*1000+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter*1000+m_EDT_dStandardThickness);
	}else
	{

		AfxMessageBox("OMM����ֵ�ĺͻ���CCD�Ĳ���ֵ֮�Ͳ���Ϊ0����ȷ��");
	}
	
	UpdateData(FALSE);



}


void CDlgCalibResolution::OnBnClickedBtnSaveK0()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString  strSection;
	CString  strValue;

	WritePrivateProfileInt("��ֵ����","EnableThickness",m_COM_nEnableThickness,g_strConfigCommonFile);
	WritePrivateProfileDouble("��ֵ����","StandardThickness",m_EDT_dStandardThickness,g_strConfigCommonFile);
	WritePrivateProfileDouble("��ֵ����","EncoderDiameter",m_EDT_dEncoderDiameter,g_strConfigCommonFile);



	for (int i=0;i<g_nGrabberCount;i++)
	{
		strSection.Format("GRABBER%d",i);
		strValue.Format("%.8f",m_EDT_dStandard_K0);
		WritePrivateProfileString(strSection,"StandardK0",strValue,g_strConfigCommonFile);
	}





}


//void CDlgCalibResolution::OnEnChangeEdtCurrentyhickness()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	UpdateData(TRUE);
//	if(m_EDT_dCurrentThickness!=0)
//	{
//
//
//		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter+m_EDT_dStandardThickness);
//
//
//	}
//	UpdateData(FALSE);
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CDlgCalibResolution::OnEnKillfocusEdtCurrentyhickness()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_EDT_dCurrentThickness!=0)
	{
		if (g_dCoatThickness!=-1 && m_COM_nEnableThickness==2)
		{

			m_EDT_dCurrentThickness=g_dCoatThickness*1000;//ת��Ϊ΢��

		}

		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter*1000+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter*1000+m_EDT_dStandardThickness);


	}
	UpdateData(FALSE);

}
