// DlgCalibResolution.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgCalibResolution.h"
#include "afxdialogex.h"


// CDlgCalibResolution 对话框

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


// CDlgCalibResolution 消息处理程序


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
	// TODO:  在此添加额外的初始化
	m_EDT_dFirstMeasureLength=g_tStoreInfo.dSumLength;
	m_EDT_dFirstMeasureWidth=g_tStoreInfo.dFirstWidth;
	m_EDT_dSecondMeasureWidth=g_tStoreInfo.dSecondWidth;


	m_EDT_dFirstColumnResolution=g_vec_ColumnResolution[0];
	m_EDT_dSecondColumnResolution=g_vec_ColumnResolution[1];
	m_EDT_dVerticalResolution=g_vec_RowResolution[0];


	CString  strSection;
	strSection.Format("GRABBER%d",0);
	
	m_COM_nEnableThickness=GetPrivateProfileInt("赋值参数","EnableThickness",0,g_strConfigCommonFile);
	m_EDT_dStandardThickness=GetPrivateProfileDouble("赋值参数","StandardThickness",0.01,g_strConfigCommonFile);
	m_EDT_dCurrentThickness=GetPrivateProfileDouble("赋值参数","CurrentThickness",0.01,g_strConfigCommonFile);
	m_EDT_dEncoderDiameter=GetPrivateProfileDouble("赋值参数","EncoderDiameter",120,g_strConfigCommonFile);
	m_EDT_dStandard_K0=GetPrivateProfileDouble(strSection,"StandardK0",0.1036,g_strConfigCommonFile);
	if (g_dCoatThickness!=-1&& m_COM_nEnableThickness==2)
	{

		m_EDT_dCurrentThickness=g_dCoatThickness*1000;//转换为微米

	}


	m_EDT_dCurrent_K=m_EDT_dVerticalResolution;
	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgCalibResolution::OnBnClickedBtnOnekeycalib()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	////校准前相机横向分辨率
	double dTempResolution=m_EDT_dFirstColumnResolution;
	if (m_EDT_dFirstMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dFirstColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dFirstMeasureWidth*g_vec_ColumnResolution[0];

	}else
	{
		AfxMessageBox("人工测量宽度值和前CCD测量宽度值均不可为0，请重新输入");
		return;
	}
	CString strValue;
	strValue.Format("前相机的水平K值由%.8f更改为%.8f",dTempResolution,m_EDT_dFirstColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	////校准后相机横向分辨率
	dTempResolution=m_EDT_dSecondColumnResolution;
	if (m_EDT_dSecondMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dSecondColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dSecondMeasureWidth*g_vec_ColumnResolution[1];

	}else
	{
		AfxMessageBox("人工测量宽度值和后CCD测量宽度值均不可为0，请重新输入");
		return;
	}
	strValue.Format("后相机的水平K值由%.8f更改为%.8f",dTempResolution,m_EDT_dSecondColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);


	////校准前后相机纵向分辨率
	dTempResolution=m_EDT_dVerticalResolution;
	if (m_EDT_dFirstMeasureLength!=0 && m_EDT_dRealMeasureLength!=0)
	{

		m_EDT_dVerticalResolution=m_EDT_dRealMeasureLength/m_EDT_dFirstMeasureLength*g_vec_RowResolution[0];

	}else
	{
		AfxMessageBox("人工测量长度值和CCD测量总长均不可为0，请重新输入");
		return;
	}
	strValue.Format("相机的纵向K值由%.8f更改为%.8f",dTempResolution,m_EDT_dVerticalResolution);
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
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CString strSection;
	CString strValue;
	

	if (m_COM_nEnableThickness!=0)//确认开启厚度标定
	{
		strValue.Format("%.8f",m_EDT_dCurrent_K);
		WritePrivateProfileInt("赋值参数","EnableThickness",m_COM_nEnableThickness,g_strConfigCommonFile);
		WritePrivateProfileDouble("赋值参数","CurrentThickness",m_EDT_dCurrentThickness,g_strConfigCommonFile);
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



    AfxMessageBox("写入成功,请重新打开软件");
}


void CDlgCalibResolution::SaveRecord()
{

	CString strFilePath,strFileDirecor,strItem;
	SYSTEMTIME tSystemTime;
	GetLocalTime(&tSystemTime);
	strFileDirecor.Format("D:\\ccd_data\\%02d-%d",tSystemTime.wYear,tSystemTime.wMonth);
	strFilePath.Format("%s\\分辨率调整记录.csv",strFileDirecor);

	HTuple bExists;
	FileExists(HTuple(strFilePath),&bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strFileDirecor);
		ofstream outFile;
		outFile.open(strFilePath,  ios::out |ios::app);
		strItem.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s","标定时间","初始标定厚度(um)","初始比例系数k0","编码器辊子直径(um)","当前材料厚度(um)","当前比例系数K1","前横向分辨率","后横向分辨率","纵向分辨率");
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
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();



}


void CDlgCalibResolution::OnBnClickedBtnJustcalibbwidth()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	////校准前相机横向分辨率
	double dTempResolution=m_EDT_dFirstColumnResolution;
	if (m_EDT_dFirstMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dFirstColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dFirstMeasureWidth*g_vec_ColumnResolution[0];

	}else
	{
		AfxMessageBox("人工测量宽度值和前CCD测量宽度值均不可为0，请重新输入");
		return;
	}
	CString strValue;
	strValue.Format("前相机的水平K值由%.8f更改为%.8f",dTempResolution,m_EDT_dFirstColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	////校准后相机横向分辨率
	dTempResolution=m_EDT_dSecondColumnResolution;
	if (m_EDT_dSecondMeasureWidth!=0 && m_EDT_dRealMeasureWidth!=0)
	{

		m_EDT_dSecondColumnResolution=m_EDT_dRealMeasureWidth/m_EDT_dSecondMeasureWidth*g_vec_ColumnResolution[1];

	}else
	{
		AfxMessageBox("人工测量宽度值和后CCD测量宽度值均不可为0，请重新输入");
		return;
	}
	strValue.Format("后相机的水平K值由%.8f更改为%.8f",dTempResolution,m_EDT_dSecondColumnResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);

	UpdateData(FALSE);
}


void CDlgCalibResolution::OnBnClickedBtnJustcaliblength()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	////校准前后相机纵向分辨率
	double dTempResolution;
	CString strValue;
	dTempResolution=m_EDT_dVerticalResolution;
	if (m_EDT_dFirstMeasureLength!=0 && m_EDT_dRealMeasureLength!=0)
	{

		m_EDT_dVerticalResolution=m_EDT_dRealMeasureLength/m_EDT_dFirstMeasureLength*g_vec_RowResolution[0];

	}else
	{
		AfxMessageBox("人工测量长度值和CCD测量总长均不可为0，请重新输入");
		return;
	}
	strValue.Format("相机的纵向K值由%.8f更改为%.8f",dTempResolution,m_EDT_dVerticalResolution);
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strValue,NULL);


	UpdateData(FALSE);
}


void CDlgCalibResolution::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()	
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STC_TITLE)->GetClientRect(rcTitle); 
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


HBRUSH CDlgCalibResolution::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgCalibResolution::OnBnClickedBtnCalculateK0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	double dSumOmmValue=m_EDT_dOmmValue1+m_EDT_dOmmValue2+m_EDT_dOmmValue3+m_EDT_dOmmValue4+m_EDT_dOmmValue5+m_EDT_dOmmValue6+m_EDT_dOmmValue7+m_EDT_dOmmValue8;
	double dSumCCDMeasureValue=m_EDT_dCCDMeasureValue1+m_EDT_dCCDMeasureValue2+m_EDT_dCCDMeasureValue3+m_EDT_dCCDMeasureValue4+m_EDT_dCCDMeasureValue5+m_EDT_dCCDMeasureValue6+m_EDT_dCCDMeasureValue7+m_EDT_dCCDMeasureValue8;

	if (dSumOmmValue!=0 && dSumCCDMeasureValue!=0)
	{
		m_EDT_dStandard_K0=g_vec_RowResolution[0]*dSumOmmValue/dSumCCDMeasureValue;
		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter*1000+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter*1000+m_EDT_dStandardThickness);
	}else
	{

		AfxMessageBox("OMM测量值的和或者CCD的测量值之和不能为0，请确认");
	}
	
	UpdateData(FALSE);



}


void CDlgCalibResolution::OnBnClickedBtnSaveK0()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString  strSection;
	CString  strValue;

	WritePrivateProfileInt("赋值参数","EnableThickness",m_COM_nEnableThickness,g_strConfigCommonFile);
	WritePrivateProfileDouble("赋值参数","StandardThickness",m_EDT_dStandardThickness,g_strConfigCommonFile);
	WritePrivateProfileDouble("赋值参数","EncoderDiameter",m_EDT_dEncoderDiameter,g_strConfigCommonFile);



	for (int i=0;i<g_nGrabberCount;i++)
	{
		strSection.Format("GRABBER%d",i);
		strValue.Format("%.8f",m_EDT_dStandard_K0);
		WritePrivateProfileString(strSection,"StandardK0",strValue,g_strConfigCommonFile);
	}





}


//void CDlgCalibResolution::OnEnChangeEdtCurrentyhickness()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
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
//	// TODO:  在此添加控件通知处理程序代码
//}


void CDlgCalibResolution::OnEnKillfocusEdtCurrentyhickness()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_EDT_dCurrentThickness!=0)
	{
		if (g_dCoatThickness!=-1 && m_COM_nEnableThickness==2)
		{

			m_EDT_dCurrentThickness=g_dCoatThickness*1000;//转换为微米

		}

		m_EDT_dCurrent_K=m_EDT_dStandard_K0*(m_EDT_dEncoderDiameter*1000+m_EDT_dCurrentThickness)/(m_EDT_dEncoderDiameter*1000+m_EDT_dStandardThickness);


	}
	UpdateData(FALSE);

}
