// DlgParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "FindMark.h"
#include "DlgParamSet.h"
#include "afxdialogex.h"


// CDlgParamSet 对话框

IMPLEMENT_DYNAMIC(CDlgParamSet, CDialogEx)

	CDlgParamSet::CDlgParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamSet::IDD, pParent)
	, m_nSearchCoefficient_Column(0)
	, m_strMarkType(_T(""))
	, m_strDrawType(_T(""))
	, m_strMarkName(_T(""))
	, m_COM_nType(1)
	, m_EDT_dMiniScore(0.8)
	, m_EDT_dIgnoreDist(0)
	, m_EDT_nBackLowThresh(0)
	, m_EDT_nBackHighThresh(0)
	, m_EDT_dFoilSaveHeight(0)
	, m_EDT_dJXEADev(0)
	, m_COM_nLocateJXUpOrDown(0)
	, m_EDT_dLocateOffset(0)
	, m_COM_nJXGray(0)
{
	// 滚轮缩放
	m_hvDispImageWidth = 0;			// 显示图像宽
	m_hvDispImageHeight = 0;		// 显示图像高
	m_dDispImagePartRow0 = 0;
	m_dDispImagePartCol0 = 0;
	m_dDispImagePartRow1 = 0;
	m_dDispImagePartCol1 = 0;
	m_hv_WindowID = NULL;
	m_bDrawing = FALSE;
	m_nSearchCoefficient_Column = 1;
	GenEmptyObj(&m_hoCurrentDispImage);
	m_pOwner=new CFindMark;
	m_EDT_nMaxThresh = 0;
	m_bChangeFlag=FALSE;
}

CDlgParamSet::~CDlgParamSet()
{
	if (m_pOwner!=NULL)
	{
		delete m_pOwner;
	}
}

void CDlgParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_MarkTree);
	DDX_Text(pDX, IDC_EDIT_CREATESEARCHCOEFFICIENT_COLUMN, m_nSearchCoefficient_Column);
	DDV_MinMaxInt(pDX, m_nSearchCoefficient_Column, -1, 2046);
	DDX_CBString(pDX, IDC_COMBO_CREATETYPE, m_strMarkType);
	DDX_CBString(pDX, IDC_COMBO_DRAWTYPE, m_strDrawType);
	DDX_Text(pDX, IDC_EDIT_CREATEMARKNAME, m_strMarkName);
	DDX_Control(pDX, IDC_BUTTON_OPENIMAGE, m_BTN_OpenImage);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_BTN_AddMark);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_EDIT_CREATEMARKNAME, m_EDT_MarkName);
	DDX_Control(pDX, IDC_EDIT_CREATESEARCHCOEFFICIENT_COLUMN, m_EDT_DilaColumn);
	DDX_Control(pDX, IDC_BUTTON_ADDLINE, m_BTN_ModifyLine);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_BTN_Test);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_COM_nType);
	DDX_Text(pDX, IDC_EDIT_MINISCORE, m_EDT_dMiniScore);
	DDX_Control(pDX, IDC_EDIT_MINISCORE, m_EDT_MinScore);
	//  DDX_Control(pDX, IDC_BTN_AUTOMODEL, m_BTN_AutoModel);
	//  DDX_CBIndex(pDX, IDC_COM_FINDSEARCHTYPE, m_COM_nFindSearchType);
	//  DDX_Text(pDX, IDC_EDT_GRAYVALUE, m_EDT_nGrayValue);
	DDX_Control(pDX, IDC_BTN_SAVEPARAM, m_BTN_CSaveParam);
	//  DDX_Text(pDX, IDC_EDT_LOWTHRESH, m_EDT_nLowThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nLowThresh, 0, 255);
	//  DDX_Text(pDX, IDC_EDT_HIGHTHRESH, m_EDT_nHighThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nHighThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_MAXTHRESH, m_EDT_nMaxThresh);
	DDV_MinMaxInt(pDX, m_EDT_nMaxThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_IGNOREDIST, m_EDT_dIgnoreDist);
	DDX_Text(pDX, IDC_EDT_BALCKLOWTHRESH, m_EDT_nBackLowThresh);
	DDV_MinMaxInt(pDX, m_EDT_nBackLowThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_BACKHIGHTHRESH, m_EDT_nBackHighThresh);
	DDV_MinMaxInt(pDX, m_EDT_nBackHighThresh, 0, 255);
	DDX_Control(pDX, IDC_STC_IMAGEGRAYVALUE, m_STC_CImageGrayValue);
	DDX_Text(pDX, IDC_EDT_SAVEFOILHEIGHT, m_EDT_dFoilSaveHeight);
	DDV_MinMaxDouble(pDX, m_EDT_dFoilSaveHeight, 1, 100);
	DDX_Text(pDX, IDC_EDT_JXEADEV, m_EDT_dJXEADev);
	DDV_MinMaxDouble(pDX, m_EDT_dJXEADev, 3, 30);
	DDX_CBIndex(pDX, IDC_COM_LOCATEUPORDOWN, m_COM_nLocateJXUpOrDown);
	DDX_Text(pDX, IDC_EDT_LOCATEOFFSET, m_EDT_dLocateOffset);
	DDX_CBIndex(pDX, IDC_COM_JXGRAY, m_COM_nJXGray);
}


BEGIN_MESSAGE_MAP(CDlgParamSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgParamSet::OnBnClickedButtonAdd)
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_OPENIMAGE, &CDlgParamSet::OnBnClickedButtonOpenimage)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CDlgParamSet::OnNMDblclkTree)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &CDlgParamSet::OnNMRClickTree)
	ON_COMMAND(ID_MENU_DELETE, &CDlgParamSet::OnMenuDelete)
	ON_COMMAND(ID_MENU_RENAME, &CDlgParamSet::OnMenuRename)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgParamSet::OnBnClickedButtonCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADDLINE, &CDlgParamSet::OnBnClickedButtonAddline)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgParamSet::OnBnClickedButtonTest)
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgParamSet::OnCbnSelchangeComboType)
//	ON_BN_CLICKED(IDC_BTN_AUTOMODEL, &CDlgParamSet::OnBnClickedBtnAutomodel)
//	ON_CBN_SELCHANGE(IDC_COM_FINDSEARCHTYPE, &CDlgParamSet::OnCbnSelchangeComFindsearchtype)
//	ON_EN_CHANGE(IDC_EDT_GRAYVALUE, &CDlgParamSet::OnEnChangeEdtGrayvalue)
	ON_BN_CLICKED(IDC_BTN_SAVEPARAM, &CDlgParamSet::OnBnClickedBtnSaveparam)
	ON_EN_CHANGE(IDC_EDT_MAXTHRESH, &CDlgParamSet::OnEnChangeEdtMaxthresh)
	ON_EN_CHANGE(IDC_EDT_IGNOREDIST, &CDlgParamSet::OnEnChangeEdtIgnoredist)
	ON_BN_CLICKED(IDC_BTN_GETINTERMISSIONHEIGHT, &CDlgParamSet::OnBnClickedBtnGetintermissionheight)
	ON_EN_CHANGE(IDC_EDT_BALCKLOWTHRESH, &CDlgParamSet::OnChangeEdtBalcklowthresh)
	ON_EN_CHANGE(IDC_EDT_BACKHIGHTHRESH, &CDlgParamSet::OnChangeEdtBackhighthresh)
	ON_BN_CLICKED(IDC_BTN_GETMEAN, &CDlgParamSet::OnBnClickedBtnGetmean)
	ON_EN_CHANGE(IDC_EDT_SAVEFOILHEIGHT, &CDlgParamSet::OnChangeEdtSavefoilheight)
	ON_EN_CHANGE(IDC_EDT_JXEADEV, &CDlgParamSet::OnEnChangeEdtJxeadev)
	ON_EN_CHANGE(IDC_EDT_LOCATEOFFSET, &CDlgParamSet::OnEnChangeEdtLocateoffset)
	ON_CBN_SELCHANGE(IDC_COM_LOCATEUPORDOWN, &CDlgParamSet::OnCbnSelchangeComLocateupordown)
	ON_CBN_SELCHANGE(IDC_COM_JXGRAY, &CDlgParamSet::OnCbnSelchangeComJxgray)
END_MESSAGE_MAP()


// CDlgParamSet 消息处理程序


//void CDlgParamSet::PostNcDestroy()
//{
//	// TODO: 在此添加专用代码和/或调用基类

//	CDialogEx::PostNcDestroy();
//	//delete this;
//}



//void CDlgParamSet::OnCbnSelchangeComboLineSelect()
//{
//	UpdateData(TRUE);
//	int n = m_CB_nLineSelect;
//	m_EDT_nMeasureLinesNum = m_pOwner->m_sLineTool[n].m_nMeasureNum;
//	m_EDT_dMeasureSigma    = m_pOwner->m_sLineTool[n].m_dSigma;
//	m_EDT_dMeasureLength   = m_pOwner->m_sLineTool[n].m_dHeight;
//	m_EDT_nMeasureThres    = m_pOwner->m_sLineTool[n].m_nThr;
//	m_EDT_dMeasureWidth    = m_pOwner->m_sLineTool[n].m_dWidth;
//	m_CB_nSelect           = m_pOwner->m_sLineTool[n].m_nSelect;
//	m_CB_nDirect           = m_pOwner->m_sLineTool[n].m_nDirect;
//	m_CB_nChoice           = m_pOwner->m_sLineTool[n].m_nChoice;
//	/*double dStartY,dStartX,dEndY,dEndX;
//	m_pOwner->FitLine(m_hoCurrentDispImage,m_pOwner->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pDetectInput->hvHomMat2D),
//	&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,1);
//	GenLineArrow(m_pOwner->m_sLineTool[m_CB_nLineSelect],80);
//	DispImage(m_hoCurrentDispImage);*/
//	UpdateData(FALSE);
//}


//void CDlgParamSet::OnEnChangeEditMeasurelinesnum()
//{
//	FreshNewParam();
//}
//void CDlgParamSet::OnBnClickedButtonLineTest()
//{
//	UpdateData(TRUE);
//	m_hoDispRegion.GenEmptyObj();
//	for (int i=0;i<LINE_COUNT;i++)
//	{
//		HObject hoDispRegion;
//		double dStartY,dStartX,dEndY,dEndX;
//		m_pOwner->FitLine(m_hoCurrentDispImage,m_pOwner->m_sLineTool[i],&hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX);
//		ConcatObj(m_hoDispRegion,hoDispRegion,&m_hoDispRegion);
//		GenLineArrow(m_pOwner->m_sLineTool[i],80);
//	}
//	DispImage(m_hoCurrentDispImage);
//	
//}
//void CDlgParamSet::OnBnClickedButtonModLine()
//{
//	m_bDrawing = TRUE;
//	SetButtonStaus(FALSE);
//	HTuple	tRow0,tCol0,tRow1,tCol1,Phi,Length1,Length2;
//	if (FALSE == m_hoCurrentDispImage.IsInitialized())
//	{
//		SetButtonStaus(TRUE);
//		m_bDrawing = FALSE;
//		return;
//	}
//	m_hoDispRegion.GenEmptyObj();
//	DrawLineMod(m_hv_WindowID,m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartRow,
//		m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartCol,
//		m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndRow,
//		m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndCol,
//		&tRow0,&tCol0,&tRow1,&tCol1);
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartRow=tRow0[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartCol=tCol0[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndRow=tRow1[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndCol=tCol1[0].D();
//	double dStartY,dStartX,dEndY,dEndX;
//	m_pOwner->FitLine(m_hoCurrentDispImage,m_pOwner->m_sLineTool[m_CB_nLineSelect],
//		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,1);
//
//	GenLineArrow(m_pOwner->m_sLineTool[m_CB_nLineSelect],80);
//	DispImage(m_hoCurrentDispImage);
//
//
//	SetButtonStaus(TRUE);
//	m_bDrawing = FALSE;
//	SetFocus();
//}

//void CDlgParamSet::OnBnClickedButtonDrawLine()
//{
//	m_bDrawing = TRUE;
//	SetButtonStaus(FALSE);
//
//	HTuple	tRow0,tCol0,tRow1,tCol1,Phi,Length1,Length2;
//	if (FALSE == m_hoCurrentDispImage.IsInitialized())
//	{
//		SetButtonStaus(TRUE);
//		m_bDrawing = FALSE;
//		return;
//	}
//	m_hoDispRegion.GenEmptyObj();
//	DrawLine(m_hv_WindowID,&tRow0,&tCol0,&tRow1,&tCol1);
//
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartRow=tRow0[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dStartCol=tCol0[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndRow=tRow1[0].D();
//	m_pOwner->m_sLineTool[m_CB_nLineSelect].m_dEndCol=tCol1[0].D();
//	double dStartY,dStartX,dEndY,dEndX;
//	m_pOwner->FitLine(m_hoCurrentDispImage,m_pOwner->m_sLineTool[m_CB_nLineSelect],
//		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,1);
//	GenLineArrow(m_pOwner->m_sLineTool[m_CB_nLineSelect],80);
//	DispImage(m_hoCurrentDispImage);
//	SetButtonStaus(TRUE);
//	m_bDrawing = FALSE;
//	SetFocus();
//}
//void CDlgParamSet::FreshNewParam()
//{
//	UpdateData(TRUE);
//	int n = m_CB_nLineSelect;
//	m_pOwner->m_sLineTool[n].m_nMeasureNum = m_EDT_nMeasureLinesNum;
//	m_pOwner->m_sLineTool[n].m_dSigma      = m_EDT_dMeasureSigma;
//	m_pOwner->m_sLineTool[n].m_dHeight     = m_EDT_dMeasureLength;
//	m_pOwner->m_sLineTool[n].m_nThr        = m_EDT_nMeasureThres;
//	m_pOwner->m_sLineTool[n].m_dWidth      = m_EDT_dMeasureWidth;
//	m_pOwner->m_sLineTool[n].m_nSelect     = m_CB_nSelect;
//	m_pOwner->m_sLineTool[n].m_nDirect     = m_CB_nDirect;
//	m_pOwner->m_sLineTool[n].m_nChoice     = m_CB_nChoice;
//	double dStartY,dStartX,dEndY,dEndX;
//	m_pOwner->FitLine(m_hoCurrentDispImage,m_pOwner->m_sLineTool[m_CB_nLineSelect],
//		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,1);
//	DispImage(m_hoCurrentDispImage);
//}
//void CDlgParamSet::OnEnChangeEditMeasurelength()
//{
//	FreshNewParam();
//}


//void CDlgParamSet::OnEnChangeEditMeasurewidth()
//{
//	FreshNewParam();
//}


//void CDlgParamSet::OnEnChangeEditMeasuresigma()
//{
//	FreshNewParam();
//}


//void CDlgParamSet::OnEnChangeEditMeasurethres()
//{
//	FreshNewParam();
//}


//void CDlgParamSet::OnCbnSelchangeComboMeasuretransition()
//{
//	FreshNewParam();
//}

//void CDlgParamSet::OnCbnSelchangeComboLineChoice()
//{
//	FreshNewParam();
//}

//void CDlgParamSet::OnCbnSelchangeComboMeasureselect()
//{
//	FreshNewParam();
//}
void CDlgParamSet::DispMark(const SINGLEMARKINFO&  tMarkDisp)
{
	//显示原图
	HObject ho_Image = tMarkDisp.ho_ModleInfo.SelectObj(1);
	HObject ho_Mark  = tMarkDisp.ho_ModleInfo.SelectObj(2);
	HObject ho_Search = tMarkDisp.ho_ModleInfo.SelectObj(3);
	DispImage(ho_Image);
	SetColor(m_hv_WindowID,"red");
	DispObj(ho_Mark,m_hv_WindowID);
	DispObj(tMarkDisp.ho_Line,m_hv_WindowID);
	
	if ("形状匹配" == tMarkDisp.hv_ModelParam[1])
	{
		HObject ho_ModelContours,ho_ContoursAffinTrans;
		HTuple hv_Row,hv_Column,hvHomMat2D;
		AreaCenter(ho_Mark,NULL,&hv_Row,&hv_Column);
		GetShapeModelContours(&ho_ModelContours, tMarkDisp.hv_ModelID, 1);
		VectorAngleToRigid(0, 0, 0, hv_Row, hv_Column, 0, &hvHomMat2D);
		AffineTransContourXld(ho_ModelContours, &ho_ContoursAffinTrans, hvHomMat2D);
		DispObj(ho_ContoursAffinTrans,m_hv_WindowID);
	}else if ("阈值分割匹配"==tMarkDisp.hv_ModelParam[1])
	{
		HTuple hv_Width,hv_Height;
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		GetImageSize(tMarkDisp.ho_ModleInfo.SelectObj(1),&hv_Width,&hv_Height);
		SmallestRectangle1(ho_Mark, &hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
		GenRectangle1(&ho_Search,0,hv_Column0,hv_Height-1,hv_Column1);
		SetColor(m_hv_WindowID,"yellow");
		DispObj(ho_Search,m_hv_WindowID);

	}else
	{
		SetColor(m_hv_WindowID,"yellow");
		DispObj(ho_Search,m_hv_WindowID);
	}


	try
	{
		m_strMarkName.Format("%s",tMarkDisp.hv_ModelParam[0].S().Text());
		m_strMarkType.Format("%s",tMarkDisp.hv_ModelParam[1].S().Text());
		m_strDrawType = "矩形";
		m_nSearchCoefficient_Column = tMarkDisp.hv_ModelParam[2];
		m_EDT_dMiniScore            = tMarkDisp.hv_ModelParam[3];
		m_COM_nType                 =tMarkDisp.hv_ModelParam[4];
		m_hoCurrentDispImage=ho_Image;
	}catch(HException e)
	{

	}

	UpdateData(FALSE);
}
void CDlgParamSet::DispImage(const HObject& hoImage)
{
	try
	{
		if(NULL == m_hv_WindowID)
		{	
			CWnd *pWnd;	
			CRect rect;
			pWnd = GetDlgItem(IDC_STATIC_PIC);
			pWnd->GetWindowRect(rect);	
			SetCheck("~father");
			OpenWindow(0, 0, rect.Width(), rect.Height(),(Hlong)pWnd->m_hWnd, "visible", "", &m_hv_WindowID);
			SetDraw(m_hv_WindowID,"margin");
			SetCheck("father");
		}

		HTuple tmpWidth;
		HTuple tmpHeight;
		GetImageSize(hoImage, &tmpWidth, &tmpHeight);
		if (tmpWidth != m_hvDispImageWidth || tmpHeight != m_hvDispImageHeight)
		{		
			m_dDispImagePartRow0 = 0;
			m_dDispImagePartCol0 = 0;
			m_dDispImagePartRow1 = tmpHeight - 1; 
			m_dDispImagePartCol1 = tmpWidth - 1;	

			m_hvDispImageWidth = tmpWidth;
			m_hvDispImageHeight = tmpHeight;
		}		
		SetPart(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
		SetSystem("flush_graphic","false");
		ClearWindow(m_hv_WindowID);
		SetSystem("flush_graphic","true");
		DispObj(hoImage, m_hv_WindowID);	
		if (m_hoDispRegion.IsInitialized())
		{
			DispObj(m_hoDispRegion,m_hv_WindowID);
		}
	}catch(HException)
	{
		ClearWindow(m_hv_WindowID);
	}
}

BOOL CDlgParamSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	try
	{
		DispImage(m_hoCurrentDispImage);
		m_hoImage=m_hoCurrentDispImage;//备用
		m_EDT_nMaxThresh=m_pOwner->m_nMaxThresh;
		m_EDT_dIgnoreDist=m_pOwner->m_dIgnoreDist;
		m_EDT_nBackLowThresh=m_pOwner->m_nBackLowThresh;
		m_EDT_nBackHighThresh=m_pOwner->m_nBackHighThresh;
		m_EDT_dFoilSaveHeight=m_pOwner->m_dFoilSaveHeight;
		m_EDT_dJXEADev=m_pOwner->m_dJXEADev;
		m_COM_nJXGray = m_pOwner->m_nJXGrayType;
		m_COM_nLocateJXUpOrDown=m_pOwner->m_nLocateUpOrDown;
		m_EDT_dLocateOffset=m_pOwner->m_dLocateOffset;


		{
			GetDlgItem(IDC_STC_LT_BACKGROUNDPARAM)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDT_BALCKLOWTHRESH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_BACKLOWTHRESH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STC_BACKHIGHTHRESH)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDT_BACKHIGHTHRESH)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_STC_LT_FOILBOTTOMSIZE)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDT_SAVEFOILHEIGHT)->ShowWindow(SW_HIDE);

			if (m_pOwner->m_tInitParam.nProductType==positive)
			{
				GetDlgItem(IDC_STC_LJ_EXTRACTFOILORJXTHRESH)->SetWindowText("间歇提取阈值");

			}
			else
			{
				GetDlgItem(IDC_STC_LJ_EXTRACTFOILORJXTHRESH)->SetWindowText("间歇提取对比度");
			}
			
			GetDlgItem(IDC_STC_LJ_IGNAORORJXHEIGHT)->SetWindowText("间歇标准高度");
			
			

		}

		



		UpdateData(FALSE);
	}catch(HException)
	{
	}
	InitUI();

	HTuple Width, Height;
	GetImageSize(m_hoCurrentDispImage, &Width, &Height);
	m_dDispImagePartRow0 = 0;
	m_dDispImagePartCol0 = 0;
	m_dDispImagePartRow1 = Height - 1;
	m_dDispImagePartCol1 = Width - 1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgParamSet::SetOwner(CFindMark* pOwner)
{
	/*m_pOwner = pOwner;*/
	m_pRealDetect=pOwner;
}


void CDlgParamSet::InitUI()
{

	UpdateData(FALSE);

	((CComboBox*)GetDlgItem(IDC_COMBO_CREATETYPE))->SetCurSel(3);
	((CComboBox*)GetDlgItem(IDC_COMBO_DRAWTYPE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_COMBO_TYPE))->SetCurSel(m_pOwner->m_nType);
	GetDlgItem(IDC_EDIT_CREATEMARKNAME)->SetWindowTextA("名称");
	
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	//设置背景色 共7个按钮
	SetButton(m_BTN_AddMark);
	SetButton(m_BTN_ModifyLine);
	SetButton(m_BTN_Test);
	SetButton(m_BTN_OpenImage);
	SetButton(m_BTN_Cancel);
	SetButton(m_BTN_CSaveParam);
	
	m_BTN_Cancel.SetIcon(IDI_ICON_W_UNCHECK);
	COLORREF MainColor = m_tBlendent.m_Blendent.Dlg.MainColor;
	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);
	m_EDT_MarkName.SetBkGndColor(m_tBlendent.m_Blendent.Edit.BKColor,m_tBlendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_MarkName.SetBorderColor(m_tBlendent.m_Blendent.Edit.Normal,
		m_tBlendent.m_Blendent.Edit.Hover,m_tBlendent.m_Blendent.Edit.Focus);
	m_EDT_DilaColumn.SetBkGndColor(m_tBlendent.m_Blendent.Edit.BKColor,m_tBlendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_DilaColumn.SetBorderColor(m_tBlendent.m_Blendent.Edit.Normal,
		m_tBlendent.m_Blendent.Edit.Hover,m_tBlendent.m_Blendent.Edit.Focus);

	m_EDT_MinScore.SetBkGndColor(m_tBlendent.m_Blendent.Edit.BKColor,m_tBlendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_MinScore.SetBorderColor(m_tBlendent.m_Blendent.Edit.Normal,
		m_tBlendent.m_Blendent.Edit.Hover,m_tBlendent.m_Blendent.Edit.Focus);
	m_STC_CImageGrayValue.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	InitTree();

	//UpdateData(FALSE);
}

BOOL CDlgParamSet::IsDrawing()
{
	return m_bDrawing;
}
void CDlgParamSet::MyFitImage()
{
	if (NULL != m_hv_WindowID)
	{
		CPoint   pt;
		GetCursorPos(&pt);
		GetDlgItem(IDC_STATIC_PIC)->ScreenToClient(&pt);
		CRect WndRect;
		GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&WndRect);

		// 还原显示（不缩放）
		if (WndRect.PtInRect(pt))
		{	
			HTuple tmpWidth;
			HTuple tmpHeight;

			GetImageSize(m_hoCurrentDispImage, &tmpWidth, &tmpHeight);

			m_dDispImagePartRow0 = 0;
			m_dDispImagePartCol0 = 0;
			m_dDispImagePartRow1 = tmpHeight - 1;
			m_dDispImagePartCol1 = tmpWidth - 1;	

			m_hvDispImageWidth = tmpWidth;
			m_hvDispImageHeight = tmpHeight;

			SetPart(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);

			ClearWindow(m_hv_WindowID);
			DispObj(m_hoCurrentDispImage, m_hv_WindowID);	
		}
	}

}
void CDlgParamSet::MyMouseMove()
{
	CPoint   pt;
	GetCursorPos(&pt);
	GetDlgItem(IDC_STATIC_PIC)->ScreenToClient(&pt);
	CRect WndRect;
	GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&WndRect);

	if (WndRect.PtInRect(pt))
	{
		try
		{
			HTuple hvRow,hvColumn,hvValue;
			GetMposition(m_hv_WindowID,&hvRow,&hvColumn,NULL);
			GetGrayval(m_hoCurrentDispImage,hvRow,hvColumn,&hvValue);
			//DispImage(m_hoCurrentDispImage);
			SetTposition(m_hv_WindowID,hvRow,hvColumn+10);
			HTuple Row1,Column1,Row2,Column2;
			GetPart(m_hv_WindowID,&Row1,&Column1,&Row2,&Column2);

			/*CString strCoordinate;HTuple Message;
			strCoordinate.Format("Row:%.2f",hvRow[0].D());
			Message.Append(HTuple(strCoordinate));
			strCoordinate.Format("Column:%.2f",hvColumn[0].D());
			Message.Append(HTuple(strCoordinate));
			CString strValue;
			strValue.Format("灰度值:%d",hvValue[0].I());*/
			/*	Message.Append(HTuple(strValue));
			disp_message(m_hv_WindowID,Message,"image",hvRow,hvColumn,"black","true");*/
			CString strValue;
			strValue.Format("Row:%.1f,Column:%.2f 灰度值:%d",hvRow[0].D(),hvColumn[0].D(),hvValue[0].I());
			m_STC_CImageGrayValue.SetWindowText(strValue);


		}catch(HException e)
		{
			m_STC_CImageGrayValue.SetWindowText("Row:-,Column:- 灰度值:-");
		}
	}
}

BOOL CDlgParamSet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		if (m_hv_WindowID != NULL && m_hoCurrentDispImage.CountObj())
		{
			CPoint   pt;
			GetCursorPos(&pt);
			GetDlgItem(IDC_STATIC_PIC)->ScreenToClient(&pt);
			CRect WndRect;
			GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&WndRect);
			if (WndRect.PtInRect(pt))
			{
				double	ImagePtX, ImagePtY;
				double	Row0_1, Col0_1, Row1_1, Col1_1;
				double	Scale = 0.1;

				if (zDelta>0)
				{
					ImagePtX = m_dDispImagePartCol0 + (pt.x - WndRect.left) / (WndRect.Width() - 1.0)*(m_dDispImagePartCol1 - m_dDispImagePartCol0);
					ImagePtY = m_dDispImagePartRow0 + (pt.y - WndRect.top) / (WndRect.Height() - 1.0)*(m_dDispImagePartRow1 - m_dDispImagePartRow0);
					Row0_1 = ImagePtY - 1 / (1 - Scale)*(ImagePtY - m_dDispImagePartRow0);
					Row1_1 = ImagePtY - 1 / (1 - Scale)*(ImagePtY - m_dDispImagePartRow1);
					Col0_1 = ImagePtX - 1 / (1 - Scale)*(ImagePtX - m_dDispImagePartCol0);
					Col1_1 = ImagePtX - 1 / (1 - Scale)*(ImagePtX - m_dDispImagePartCol1);
					m_dDispImagePartRow0 = Row0_1;
					m_dDispImagePartCol0 = Col0_1;
					m_dDispImagePartRow1 = Row1_1;
					m_dDispImagePartCol1 = Col1_1;
				}
				else
				{
					ImagePtX = m_dDispImagePartCol0 + (pt.x - WndRect.left) / (WndRect.Width() - 1.0)*(m_dDispImagePartCol1 - m_dDispImagePartCol0);
					ImagePtY = m_dDispImagePartRow0 + (pt.y - WndRect.top) / (WndRect.Height() - 1.0)*(m_dDispImagePartRow1 - m_dDispImagePartRow0);
					Row0_1 = ImagePtY - 1 / (1 + Scale)*(ImagePtY - m_dDispImagePartRow0);
					Row1_1 = ImagePtY - 1 / (1 + Scale)*(ImagePtY - m_dDispImagePartRow1);
					Col0_1 = ImagePtX - 1 / (1 + Scale)*(ImagePtX - m_dDispImagePartCol0);
					Col1_1 = ImagePtX - 1 / (1 + Scale)*(ImagePtX - m_dDispImagePartCol1);
					m_dDispImagePartRow0 = Row0_1;
					m_dDispImagePartCol0 = Col0_1;
					m_dDispImagePartRow1 = Row1_1;
					m_dDispImagePartCol1 = Col1_1;
				}


				SetPart(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
				SetSystem("flush_graphic", "false");
				ClearWindow(m_hv_WindowID);
				SetSystem("flush_graphic", "true");
				DispObj(m_hoCurrentDispImage, m_hv_WindowID);
			}
		}
	}
	catch (HException)
	{

	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


//
//void CDlgParamSet::MyMouseWheel(short zDelta)
//{
//	if (NULL != m_hv_WindowID)
//	{
//		CPoint   pt;
//		GetCursorPos(&pt);
//		GetDlgItem(IDC_STATIC_PIC)->ScreenToClient(&pt);
//		CRect WndRect;
//		GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&WndRect);
//		if (WndRect.PtInRect(pt))
//		{			
//			double	ImagePtX,ImagePtY;
//			double	Row0_1,Col0_1,Row1_1,Col1_1;
//			double	Scale=0.1;
//
//			if (zDelta>0)
//			{
//				ImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
//				ImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
//				Row0_1=ImagePtY-1/(1-Scale)*(ImagePtY-m_dDispImagePartRow0);
//				Row1_1=ImagePtY-1/(1-Scale)*(ImagePtY-m_dDispImagePartRow1);
//				Col0_1=ImagePtX-1/(1-Scale)*(ImagePtX-m_dDispImagePartCol0);
//				Col1_1=ImagePtX-1/(1-Scale)*(ImagePtX-m_dDispImagePartCol1);
//				m_dDispImagePartRow0=Row0_1;
//				m_dDispImagePartCol0=Col0_1;
//				m_dDispImagePartRow1=Row1_1;
//				m_dDispImagePartCol1=Col1_1;
//			}
//			else
//			{
//				ImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
//				ImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
//				Row0_1=ImagePtY-1/(1+Scale)*(ImagePtY-m_dDispImagePartRow0);
//				Row1_1=ImagePtY-1/(1+Scale)*(ImagePtY-m_dDispImagePartRow1);
//				Col0_1=ImagePtX-1/(1+Scale)*(ImagePtX-m_dDispImagePartCol0);
//				Col1_1=ImagePtX-1/(1+Scale)*(ImagePtX-m_dDispImagePartCol1);
//				m_dDispImagePartRow0=Row0_1;
//				m_dDispImagePartCol0=Col0_1;
//				m_dDispImagePartRow1=Row1_1;
//				m_dDispImagePartCol1=Col1_1;
//			}
//
//			SetPart(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
//			SetSystem("flush_graphic", "false");
//			ClearWindow(m_hv_WindowID);
//			SetSystem("flush_graphic", "true");
//			DispObj(m_hoCurrentDispImage, m_hv_WindowID);
//			//DispImage(m_hoCurrentDispImage);
//		}
//	}
//}
void CDlgParamSet::InitTree()
{
	m_MarkTree.DeleteAllItems();
	m_TreeImages.DeleteImageList();
	m_TreeImages.Create(32,32,ILC_COLOR32|ILC_MASK,0,0);
	m_TreeImages.Add(AfxGetApp()->LoadIconA(IDI_ICON_MARK));
	m_MarkTree.SetImageList(&m_TreeImages, LVSIL_NORMAL);
	int MarkCount = m_pOwner->m_tMarkList.size(); 
	CString strInsertName="";  
	for(int i=0;i<MarkCount;i++)
	{
		strInsertName.Format("%s",m_pOwner->m_tMarkList[i].hv_ModelParam[0].S().Text());
		m_MarkTree.InsertItem(strInsertName,0,0);  
	}
}
void CDlgParamSet::SetButton(CButtonExt& tButton)
{

	short	shBtnColor = m_tBlendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = m_tBlendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = m_tBlendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = m_tBlendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = m_tBlendent.m_Blendent.Button.FGColorOut;


	tButton.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	tButton.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	tButton.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	tButton.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	tButton.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	tButton.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	tButton.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	tButton.SetFont(&m_ftButton);

}
BOOL CDlgParamSet::Create_ShapeModel(HObject& ho_ImageReduced)
{
	HObject ho_Domain,ho_Search;
	GetDomain(ho_ImageReduced,&ho_Domain);
	DilationRectangle1(ho_Domain,&ho_Search,m_nSearchCoefficient_Column,m_nSearchCoefficient_Column);
	SINGLEMARKINFO tMarkAdd;
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkName));
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkType));
	tMarkAdd.hv_ModelParam.Append(HTuple(m_nSearchCoefficient_Column));
	tMarkAdd.hv_ModelParam.Append(HTuple(m_EDT_dMiniScore));
	tMarkAdd.hv_ModelParam.Append(HTuple(m_COM_nType));
	try
	{
		CreateShapeModel(ho_ImageReduced, "auto", HTuple(-15).TupleRad(), HTuple(15).TupleRad(), "auto", "auto", "use_polarity", 
		"auto", "auto", &tMarkAdd.hv_ModelID);
		ConcatObj(m_hoCurrentDispImage,ho_Domain,&tMarkAdd.ho_ModleInfo);
		ConcatObj(tMarkAdd.ho_ModleInfo,ho_Search,&tMarkAdd.ho_ModleInfo);


		HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
		SmallestRectangle1(ho_Domain,&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
		GenRegionLine(&tMarkAdd.ho_Line,(hv_Row2+hv_Row1)/2,hv_Column1,(hv_Row2+hv_Row1)/2,hv_Column2);
		m_pOwner->m_tMarkList.push_back(tMarkAdd);
	}catch(HException e)
	{
		AfxMessageBox("添加形状模板失败");
		return FALSE;
	}
	return TRUE;

}

void CDlgParamSet::SetButtonStaus(BOOL bStaus)
{
	HWND  hwndChild=::GetWindow(GetSafeHwnd(),GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	int woc; 
	CRect Rect;
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
		switch (woc)
		{
		case IDC_STATIC_PIC:
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}
}
BOOL CDlgParamSet::Create_NCCModel(HObject& ho_ImageReduced)
{
	HObject ho_Domain,ho_Search;

	GetDomain(ho_ImageReduced,&ho_Domain);
	DilationRectangle1(ho_Domain,&ho_Search,m_nSearchCoefficient_Column,m_nSearchCoefficient_Column);
	SINGLEMARKINFO tMarkAdd;
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkName));//模板名称
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkType));//灰度匹配还是形状匹配
	tMarkAdd.hv_ModelParam.Append(HTuple(m_nSearchCoefficient_Column));//列宽
	tMarkAdd.hv_ModelParam.Append(HTuple(m_EDT_dMiniScore));//最小匹配率
	tMarkAdd.hv_ModelParam.Append(HTuple(m_COM_nType));//定位类型是极耳还是R角
	try
	{
		CreateNccModel(ho_ImageReduced,"auto", HTuple(-5).TupleRad(), HTuple(10).TupleRad(), "auto", "use_polarity", &tMarkAdd.hv_ModelID);
		ConcatObj(m_hoCurrentDispImage,ho_Domain,&tMarkAdd.ho_ModleInfo);
		ConcatObj(tMarkAdd.ho_ModleInfo,ho_Search,&tMarkAdd.ho_ModleInfo);
		HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
		SmallestRectangle1(ho_Domain,&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
		GenRegionLine(&tMarkAdd.ho_Line,(hv_Row2+hv_Row1)/2,hv_Column1,(hv_Row2+hv_Row1)/2,hv_Column2);
		m_pOwner->m_tMarkList.push_back(tMarkAdd);
	}catch(HException e)
	{
		AfxMessageBox("添加灰度模板失败");
		return FALSE;
	}
	return TRUE;
}
void CDlgParamSet::Draw(CString strDrawType,HObject* Region)
{
	GenEmptyObj(Region);
	HTuple tmpButton = 1;
	//while(1 == tmpButton)
	{
		try
		{
			SetDraw(m_hv_WindowID,"margin");
			HObject tmpRegion;
			HTuple tmpRow, tmpColumn;
			DispImage(m_hoCurrentDispImage);
			SetColor(m_hv_WindowID, "green");
			SetLineWidth(m_hv_WindowID, 2);
			DispObj(*Region, m_hv_WindowID);

			SetTposition(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0);
			SetColor(m_hv_WindowID, "blue");
			SetLineWidth(m_hv_WindowID, 2);
			WriteString(m_hv_WindowID, "请画区域，点右键确认。");

			SetColor(m_hv_WindowID, "yellow");
			SetLineWidth(m_hv_WindowID, 1);
			if ("矩形" == strDrawType)
			{
				HTuple Row1, Column1, Row2, Column2;
				DrawRectangle1(m_hv_WindowID, &Row1, &Column1, &Row2, &Column2);
				GenRectangle1(&tmpRegion,Row1, Column1, Row2, Column2);
			}else if ("带方向矩形" == strDrawType)
			{
				HTuple Row, Column, Phi, Length1, Length2;
				DrawRectangle2(m_hv_WindowID, &Row, &Column, &Phi, &Length1, &Length2);
				GenRectangle2(&tmpRegion,Row, Column, Phi, Length1, Length2);
			}else if ("圆形" == strDrawType)
			{
				HTuple Row, Column, Radius;
				DrawCircle(m_hv_WindowID, &Row, &Column, &Radius);
				GenCircle(&tmpRegion,Row, Column, Radius);
			}else if ("椭圆" == strDrawType)
			{
				HTuple Row, Column, Phi, Radius1, Radius2;
				DrawEllipse(m_hv_WindowID, &Row, &Column, &Phi, &Radius1, &Radius2);
				GenEllipse(&tmpRegion, Row, Column, Phi, Radius1, Radius2);
			}else if ("多边形" == strDrawType)
			{
				DrawPolygon(&tmpRegion,m_hv_WindowID);
				FillUp(tmpRegion,&tmpRegion);
			}else if ("任意形状" == strDrawType)
			{
				DrawRegion(&tmpRegion, m_hv_WindowID);
			}


			Union2(*Region, tmpRegion, Region);

			DispImage(m_hoCurrentDispImage);
			SetColor(m_hv_WindowID, "green");
			SetLineWidth(m_hv_WindowID, 2);
			DispObj(*Region, m_hv_WindowID);

			SetTposition(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0);
			SetColor(m_hv_WindowID, "red");
			SetLineWidth(m_hv_WindowID, 2);
		 
		//	WriteString(m_hv_WindowID, "左键继续添加... / 右键结束。");
	//		GetMbutton(m_hv_WindowID, &tmpRow, &tmpColumn, &tmpButton);		

		}catch(HException &e)
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

	}	
}


BOOL CDlgParamSet::CreatThreshModel(HObject& ho_ImageReduced)
{
	HObject ho_Domain,ho_Search;
	GetDomain(ho_ImageReduced,&ho_Domain);
	DilationRectangle1(ho_Domain,&ho_Search,3,3);
	SINGLEMARKINFO tMarkAdd;
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkName));//模板名称
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkType));//灰度匹配还是形状匹配
	tMarkAdd.hv_ModelParam.Append(HTuple(m_nSearchCoefficient_Column));//列宽
	tMarkAdd.hv_ModelParam.Append(HTuple(m_EDT_dMiniScore));//最小匹配率
	tMarkAdd.hv_ModelParam.Append(HTuple(m_COM_nType));//定位类型是极耳还是R角
	try
	{
		HObject hoRectangle,ho_ImageReduced1;
		GenRectangle1(&hoRectangle,0,0,50,50);
		ReduceDomain(m_hoCurrentDispImage,hoRectangle,&ho_ImageReduced1);
		CreateNccModel(ho_ImageReduced1,"auto", HTuple(-5).TupleRad(), HTuple(10).TupleRad(), "auto", "use_polarity", &tMarkAdd.hv_ModelID);
		ConcatObj(m_hoCurrentDispImage,ho_Domain,&tMarkAdd.ho_ModleInfo);
		ConcatObj(tMarkAdd.ho_ModleInfo,ho_Search,&tMarkAdd.ho_ModleInfo);
		HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
		SmallestRectangle1(ho_Domain,&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
		GenRegionLine(&tMarkAdd.ho_Line,(hv_Row2+hv_Row1)/2,hv_Column1,(hv_Row2+hv_Row1)/2,hv_Column2);
		m_pOwner->m_tMarkList.push_back(tMarkAdd);
	}catch(HException e)
	{
		AfxMessageBox("添加灰度模板失败");
		return FALSE;
	}
	return TRUE;


}
BOOL CDlgParamSet::Create_ThreshIntermisssModel(HObject& ho_ImageReduced)
{
	HObject ho_Domain,ho_Search;
	GetDomain(ho_ImageReduced,&ho_Domain);
	DilationRectangle1(ho_Domain,&ho_Search,1,1);
	SINGLEMARKINFO tMarkAdd;
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkName));//模板名称
	tMarkAdd.hv_ModelParam.Append(HTuple(m_strMarkType));//灰度匹配还是形状匹配
	tMarkAdd.hv_ModelParam.Append(HTuple(m_nSearchCoefficient_Column));//列宽
	tMarkAdd.hv_ModelParam.Append(HTuple(m_EDT_dMiniScore));//最小匹配率
	tMarkAdd.hv_ModelParam.Append(HTuple(m_COM_nType));//定位类型是极耳还是R角
	try
	{
		HObject hoRegion1,ho_ImageReduced1;
		GenRectangle1(&hoRegion1,0,0,100,100);
		ReduceDomain(m_hoCurrentDispImage,hoRegion1,&ho_ImageReduced1);
		CreateNccModel(ho_ImageReduced1,"auto", HTuple(-5).TupleRad(), HTuple(10).TupleRad(), "auto", "use_polarity", &tMarkAdd.hv_ModelID);
		ConcatObj(m_hoCurrentDispImage,ho_Domain,&tMarkAdd.ho_ModleInfo);
		ConcatObj(tMarkAdd.ho_ModleInfo,ho_Search,&tMarkAdd.ho_ModleInfo);
		HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
		SmallestRectangle1(ho_Domain,&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
		GenRegionLine(&tMarkAdd.ho_Line,(hv_Row2+hv_Row1)/2,hv_Column1,(hv_Row2+hv_Row1)/2,hv_Column2);
		m_pOwner->m_tMarkList.push_back(tMarkAdd);
	}catch(HException e)
	{
		AfxMessageBox("添加阈值分割模板失败");
		return FALSE;
	}
	return TRUE;
}

void CDlgParamSet::OnBnClickedButtonAdd()
{
	SetButtonStaus(FALSE);
	UpdateData(TRUE);
	try
	{
		for(int i=0;i<m_pOwner->m_tMarkList.size();i++)
		{
			CString strInsertName;
			strInsertName.Format("%s",m_pOwner->m_tMarkList[i].hv_ModelParam[0].S().Text());
			if (m_pOwner->m_tMarkList.size()>2)
			{
				AfxMessageBox("拼图模板的最大数目是3个，请删除已有模板后再进行绘制！");
				SetButtonStaus(TRUE);
				return;
			}
			if(strInsertName ==  m_strMarkName)
			{
				AfxMessageBox("不允许存在名称相同的拼图定位Mark");
				SetButtonStaus(TRUE);
				return;
			}
		}
		m_bDrawing = TRUE;
		BOOL bReturn;;
		/////////////////画模版区域
		HObject ho_Region;
		Draw(m_strDrawType,&ho_Region);
		HObject ho_ImageReduced;
		ReduceDomain(m_hoCurrentDispImage,ho_Region,&ho_ImageReduced);
		try
		{
			if ("形状匹配" == m_strMarkType)
			{
				bReturn = Create_ShapeModel(ho_ImageReduced);
			}else if ("灰度匹配" == m_strMarkType)
			{
				bReturn = Create_NCCModel(ho_ImageReduced);
			}else if ("阈值分割匹配" == m_strMarkType)
			{
				/*bReturn = Create_NCCModel(ho_ImageReduced);*/
				bReturn = CreatThreshModel(ho_ImageReduced);
			}else if ("间歇分割" == m_strMarkType)
			{

				bReturn = Create_ThreshIntermisssModel(ho_ImageReduced);
			}
		}catch(HException)
		{
			bReturn = FALSE;
		}
		if (FALSE == bReturn)
		{
			AfxMessageBox("模板区域画得不好，请重新创建！");
			m_bDrawing = FALSE;
			SetButtonStaus(TRUE);
			return;
		}
		InitTree();
		m_bDrawing = FALSE;
		DispMark(m_pOwner->m_tMarkList[m_pOwner->m_tMarkList.size()-1]);
		AfxMessageBox("新的拼图定位Mark点添加完成");
		SetButtonStaus(TRUE);

	}catch(HException& e)
	{
		SetButtonStaus(TRUE);
	}

}


BOOL CDlgParamSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//屏蔽回车
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)//屏蔽ESC
		{
			return TRUE;
		}else if(pMsg->wParam == VK_SPACE)//
		{
			return TRUE;
		}else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//屏蔽ALT+F4
		{
			return TRUE;
		}

	}
	else if (pMsg->message == WM_MOUSEMOVE/* && GetAsyncKeyState(VK_LCONTROL)*/)
	{
		try
		{
			HTuple Row,Column,Value,Area,Circularity;
			GetMposition(m_hv_WindowID,&Row,&Column,NULL);
			GetGrayval(m_hoCurrentDispImage,Row,Column,&Value);
			CString strValue;
			strValue.Format("Row:%.1f,Column:%.2f 灰度值:%d",Row[0].D(),Column[0].D(),Value[0].I());
			m_STC_CImageGrayValue.SetWindowText(strValue);
			return TRUE;
		}catch(HException e)
		{
			CString strValue;
			strValue.Format("坐标:  X:-- Y:-- 灰度值: --");
			m_STC_CImageGrayValue.SetWindowText(strValue);
		}
	}
	else if (pMsg->message == WM_MOUSEWHEEL)
	{
		short zDelta = HIWORD(pMsg->wParam);
		UINT nFlags = LOWORD(pMsg->wParam);
		OnMouseWheel(nFlags, zDelta, pMsg->pt);
		return TRUE;
	}
	else if (pMsg->message == WM_MBUTTONDOWN)
	{
		if (NULL != m_hv_WindowID)
		{
			CPoint   pt;
			GetCursorPos(&pt);
			GetDlgItem(IDC_STATIC_PIC)->ScreenToClient(&pt);
			CRect WndRect;
			GetDlgItem(IDC_STATIC_PIC)->GetClientRect(&WndRect);

			// 还原显示（不缩放）
			if (WndRect.PtInRect(pt))
			{
				HTuple tmpWidth;
				HTuple tmpHeight;

				GetImageSize(m_hoCurrentDispImage, &tmpWidth, &tmpHeight);

				m_dDispImagePartRow0 = 0;
				m_dDispImagePartCol0 = 0;
				m_dDispImagePartRow1 = tmpHeight - 1;
				m_dDispImagePartCol1 = tmpWidth - 1;
				SetPart(m_hv_WindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);

				ClearWindow(m_hv_WindowID);
				DispObj(m_hoCurrentDispImage, m_hv_WindowID);
			}
		}
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}






void CDlgParamSet::OnBnClickedButtonOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this); 
	if(IDOK == FileDlg.DoModal())
	{
		CString lpstrFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_hoCurrentDispImage,HTuple(lpstrFilename));
			DispImage(m_hoCurrentDispImage);
		}catch(HException )
		{
			AfxMessageBox("打开文件失败");
		}

	}

}


void CDlgParamSet::OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	//显示选中名称的Mark
	HTREEITEM SelectItem = m_MarkTree.GetSelectedItem();

	static bool bClicked=FALSE;
	if (FALSE==bClicked)//查看当前模板
	{
		for(int i=0;i<m_pOwner->m_tMarkList.size();i++)
		{
			CString strInsertName;
			strInsertName.Format("%s",m_pOwner->m_tMarkList[i].hv_ModelParam[0].S().Text());
			if(strInsertName ==  m_MarkTree.GetItemText(SelectItem))
			{
				DispMark(m_pOwner->m_tMarkList[i]);
				break;
			}
		}
		bClicked=TRUE;
	}else
	{
		m_hoCurrentDispImage=m_hoImage;//显示非模板图像
		DispImage(m_hoCurrentDispImage);
		bClicked=FALSE;
	}
	
}


void CDlgParamSet::OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CPoint  RClickPoint; 
	GetCursorPos(&RClickPoint); //获取当前光标的位置，以便使得菜单可以跟随光标  
	m_MarkTree.ScreenToClient(&RClickPoint);  //m_TREE是CTreeCtrl控件变量。
	HTREEITEM  SelItem = m_MarkTree.HitTest(RClickPoint,NULL); //获取当前鼠标右键单击的位置下的item
	m_MarkTree.SelectItem(SelItem);

	m_MarkTree.ClientToScreen(&RClickPoint);
	CMenu menu;//定义下面要用到的cmenu对象  
	menu.LoadMenu(IDR_MENU_RBUTTON_MARKTREE);//装载自定义的右键菜单  
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,RClickPoint.x,RClickPoint.y,this);  //在指定位置显示弹出菜单
	*pResult = 0;
}


void CDlgParamSet::OnMenuDelete()
{
	HTREEITEM SelectItem = m_MarkTree.GetSelectedItem();
	std::vector<SINGLEMARKINFO>::iterator it;
	for(it=m_pOwner->m_tMarkList.begin();it!=m_pOwner->m_tMarkList.end();it++)
	{
		CString strInsertName;
		strInsertName.Format("%s",it->hv_ModelParam[0].S().Text());
		if(strInsertName ==  m_MarkTree.GetItemText(SelectItem))
		{
			m_pOwner->m_tMarkList.erase(it);
			break;
		}
	}
	InitTree();
	//m_pOwner->WriteMark();
}


void CDlgParamSet::OnMenuRename()
{
	// TODO: 在此添加命令处理程序代码
}


void CDlgParamSet::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


void CDlgParamSet::OnPaint()
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


HBRUSH CDlgParamSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor==CTLCOLOR_STATIC) 
	{ 
		pDC-> SetBkMode(TRANSPARENT); 
		return (HBRUSH)::GetStockObject(NULL_BRUSH); 
	} 

	return hbr;
}


void CDlgParamSet::OnBnClickedButtonAddline()
{
	UpdateData(TRUE);
	HTREEITEM SelectItem = m_MarkTree.GetSelectedItem();
	std::vector<SINGLEMARKINFO>::iterator it;
	for(it=m_pOwner->m_tMarkList.begin();it!=m_pOwner->m_tMarkList.end();it++)
	{
		CString strInsertName;
		strInsertName.Format("%s",it->hv_ModelParam[0].S().Text());
		if(strInsertName ==  m_MarkTree.GetItemText(SelectItem))//修改线的位置
		{
			m_bDrawing = TRUE;
			/////////////////画模版区域
			HObject ho_Region;
			Draw(m_strDrawType,&ho_Region);
			HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
			SmallestRectangle1(ho_Region,&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
			GenRegionLine(&it->ho_Line,(hv_Row2+hv_Row1)/2,hv_Column1,(hv_Row2+hv_Row1)/2,hv_Column2);
			m_pOwner->WriteMark();
			m_bDrawing = FALSE;

			DispMark(*it);
			AfxMessageBox("追踪行修改完成");
			break;
		}
	}


}


void CDlgParamSet::OnBnClickedButtonTest()
{
	try
	{

	
	HTuple hv_Start,hv_End;
	CountSeconds(&hv_Start);
	NS_SCIFindMark::MARKRESULT tMarkResult;
	m_pOwner->DoSomething("FIND",&m_hoCurrentDispImage,&tMarkResult);
	DispImage(m_hoCurrentDispImage);
	SetColor(m_hv_WindowID,"red");
	DispObj(tMarkResult.ho_DispObj,m_hv_WindowID);

	CountSeconds(&hv_End);
	if (m_pOwner->m_tMarkList.size() == 0)
	{
		return;

	}
	////////////////////////////
	HTuple hv_Height;
	GetImageSize(m_hoCurrentDispImage, NULL, &hv_Height);
	HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	HObject ho_Search;
	SmallestRectangle1(m_pOwner->m_tMarkList.at(0).ho_ModleInfo.SelectObj(3), &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	GenRectangle1(&ho_Search, 0, hv_Column1, hv_Height - 1, hv_Column2);
	SetColor(m_hv_WindowID, "green");
	DispObj(ho_Search, m_hv_WindowID);
	if (m_pOwner->m_tMarkList.at(0).hv_ModelParam[1]=="间歇分割" && tMarkResult.hv_Row.Length()>0)
	{
		DispObj(tMarkResult.ho_DispObj,m_hv_WindowID);
		HTuple hvIntermissionHeight;
		RegionFeatures(tMarkResult.ho_DispObj,"height",&hvIntermissionHeight);
		for (int i=0;i<tMarkResult.hv_Row.Length();i++)
		{
			SetTposition(m_hv_WindowID,tMarkResult.hv_Row[i],tMarkResult.hv_Column[i]);
			CString strValue;
			strValue.Format("%.1fmm",hvIntermissionHeight[i].D()*m_pOwner->m_dRowResolution);
			WriteString(m_hv_WindowID,(HTuple)strValue);
		}
	}
	//////////////////////////

	if (tMarkResult.ho_DispObj.CountObj()>1)
	{
		SetColor(m_hv_WindowID, "blue");
		DispObj(tMarkResult.ho_DispObj.SelectObj(tMarkResult.ho_DispObj.CountObj()), m_hv_WindowID);
	}

	HTuple hv_Message;
	HObject hoCross;
	for (int i=0;i<tMarkResult.hv_AllScore.Length();i++)
	{
		CString strScore;
		strScore.Format("%d:匹配率 %.6f",i+1,tMarkResult.hv_AllScore[i].D());
		hv_Message.Append(HTuple(strScore));
		SetColor(m_hv_WindowID,"yellow");
		GenCrossContourXld(&hoCross,tMarkResult.hv_Row[i],tMarkResult.hv_Column[i],160,0);
        DispObj(hoCross,m_hv_WindowID);



	}
	CString strSpeedTime;
	strSpeedTime.Format("匹配耗时:%05.03f ms",(hv_End.D() - hv_Start.D())*1000);
	hv_Message.Append(HTuple(strSpeedTime));
	disp_message(m_hv_WindowID,hv_Message,"image",m_dDispImagePartRow0, m_dDispImagePartCol0,"red","false");
	}
	catch (HException & e)
	{

	}
}


LRESULT CDlgParamSet::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
}


void CDlgParamSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*85)/100,LWA_ALPHA);
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


BOOL CDlgParamSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CDlgParamSet::OnCbnSelchangeComboType()
{
	UpdateData(TRUE);
	m_pOwner->m_nType = m_COM_nType;
	m_pOwner->m_nMaxThresh=m_EDT_nMaxThresh;
	WritePrivateProfileInt("Mark","Type",m_pOwner->m_nType,m_pOwner->m_strConfigFile);
	WritePrivateProfileInt("Mark","m_nMaxThresh",m_pOwner->m_nMaxThresh,m_pOwner->m_strConfigFile);



}



void CDlgParamSet::OnBnClickedBtnSaveparam()
{
	// TODO: 在此添加控件通知处理程序代码
	{
	
		m_bChangeFlag=TRUE;
		m_pRealDetect->DoSomething("COPYOBJECTS",m_pOwner,NULL);
		m_pRealDetect->WriteMark();
	}
	AfxMessageBox("保存成功！");

}


void CDlgParamSet::OnEnChangeEdtMaxthresh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_nMaxThresh=m_EDT_nMaxThresh;
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnEnChangeEdtIgnoredist()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_dIgnoreDist=m_EDT_dIgnoreDist;
	
}


void CDlgParamSet::OnBnClickedBtnGetintermissionheight()
{
	// TODO: 在此添加控件通知处理程序代码

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_RegionFillUp;
	HObject  ho_ConnectedRegions, ho_SelectedRegions;

	// Local control variables
	HTuple  hv_nJianXie, hv_Number;
	HObject ho_DetetctRegion;
	hv_nJianXie = m_EDT_dIgnoreDist/m_pOwner->m_dRowResolution;
	//SetButtonStaus(FALSE);
	try
	{
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		{
			SetTposition(m_hv_WindowID,12,12);
			CString strValue;
			strValue.Format("请绘制包含间歇高度区域，以便获取间歇区域高度特征");
			WriteString(m_hv_WindowID,(HTuple)strValue);
		}
		DrawRectangle1(m_hv_WindowID,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
		GenRectangle1(&ho_DetetctRegion, hv_Row0,hv_Column0,hv_Row1,hv_Column1);

		ReduceDomain(m_hoCurrentDispImage, ho_DetetctRegion, &ho_ImageReduced);


		if (m_pOwner->m_tInitParam.nProductType==positive)
		{
			Threshold(ho_ImageReduced, &ho_Region, m_EDT_nMaxThresh,255);
		}else
		{

			HTuple hvAbsHisto, hv_RelateHisto;
			HTuple hv_SortIndex;
			GrayHisto(ho_DetetctRegion, m_hoCurrentDispImage, &hvAbsHisto, &hv_RelateHisto);
			TupleSortIndex(hvAbsHisto, &hv_SortIndex);
			if (hv_SortIndex.Length()>0)
			{
				Threshold(ho_ImageReduced, &ho_Region, 0, MAX(15, hv_SortIndex[hv_SortIndex.Length() - 1] - m_EDT_nMaxThresh));
			}
			else
			{
				GenEmptyObj(&ho_Region);
			}
			//Threshold(ho_ImageReduced, &ho_Region, 0, m_EDT_nMaxThresh);
		}

		FillUp(ho_Region, &ho_RegionFillUp);
		OpeningRectangle1(ho_RegionFillUp, &ho_RegionFillUp, 11, 1);
		ClosingRectangle1(ho_RegionFillUp, &ho_RegionFillUp, 33, 31);//中间有白条会找不到
		OpeningRectangle1(ho_RegionFillUp, &ho_RegionFillUp, 11, 1);
		Connection(ho_RegionFillUp, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("convexity").Append("rectangularity")), 
			"and", (HTuple(0.85).Append(0.9)), (HTuple(1).Append(1)));
		SelectShapeStd(ho_SelectedRegions,&ho_SelectedRegions,"max_area", 70);
		CountObj(ho_SelectedRegions, &hv_Number);
		
		if (0 != (int(hv_Number==1)))
		{
			HTuple hv_HeightValue;
			HTuple hv_RowCenter,hv_ColumnCenter;
			SetColor(m_hv_WindowID,"yellow");
			DispObj(ho_SelectedRegions,m_hv_WindowID);
			SetColor(m_hv_WindowID,"green");
			RegionFeatures(ho_SelectedRegions,"height",&hv_HeightValue);
			AreaCenter(ho_SelectedRegions,NULL,&hv_RowCenter,&hv_ColumnCenter);
			SetTposition(m_hv_WindowID,hv_RowCenter,hv_ColumnCenter);
			CString strValue;
			strValue.Format("%.1fmm",hv_HeightValue.D()*m_pOwner->m_dRowResolution);
			WriteString(m_hv_WindowID,(HTuple)strValue);
		}
		//SetButtonStaus(TRUE);
	}catch(HException& e)
	{
		//SetButtonStaus(TRUE);
	}


	return;
}


void CDlgParamSet::OnChangeEdtBalcklowthresh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_nBackLowThresh=m_EDT_nBackLowThresh;

	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnChangeEdtBackhighthresh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_nBackHighThresh=m_EDT_nBackHighThresh;
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnBnClickedBtnGetmean()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		SetButtonStaus(FALSE);
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_COlumn1,hv_Mean;
		HObject hoRectangle;
		DrawRectangle1(m_hv_WindowID,&hv_Row0,&hv_Column0,&hv_Row1,&hv_COlumn1);
		GenRectangle1(&hoRectangle,hv_Row0,hv_Column0,hv_Row1,hv_COlumn1);
		Intensity(hoRectangle,m_hoCurrentDispImage,&hv_Mean,NULL);
		SetTposition(m_hv_WindowID,hv_Row0,hv_Column0);
		CString strValue;
		strValue.Format("灰度均值:%.1f",hv_Mean.D());
		WriteString(m_hv_WindowID,(HTuple)strValue);

		SetButtonStaus(TRUE);
	}catch(HException& e)
	{
		SetButtonStaus(TRUE);
	}

}


void CDlgParamSet::OnChangeEdtSavefoilheight()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_dFoilSaveHeight=m_EDT_dFoilSaveHeight;
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnEnChangeEdtJxeadev()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_dJXEADev=m_EDT_dJXEADev;
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnEnChangeEdtLocateoffset()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pOwner->m_dLocateOffset=m_EDT_dLocateOffset;
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnCbnSelchangeComLocateupordown()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pOwner->m_nLocateUpOrDown=m_COM_nLocateJXUpOrDown;
	
}


void CDlgParamSet::OnCbnSelchangeComJxgray()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pOwner->m_nJXGrayType = m_COM_nJXGray;
}
