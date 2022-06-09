// DlgParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "LineToolPara.h"
#include "DlgAlgorithm.h"
#include "DlgCalibrate.h"
#include "PageLineSet.h"
#include "afxdialogex.h"


// CPageLineSet 对话框
void gen_arrow_contour_xld(HObject *ho_Arrow, HTuple hv_Row1, HTuple hv_Column1,
	HTuple hv_Row2, HTuple hv_Column2, HTuple hv_HeadLength, HTuple hv_HeadWidth)
{

	// Local iconic variables
	HObject  ho_TempArrow;

	// Local control variables
	HTuple  hv_Length, hv_ZeroLengthIndices, hv_DR;
	HTuple  hv_DC, hv_HalfHeadWidth, hv_RowP1, hv_ColP1, hv_RowP2;
	HTuple  hv_ColP2, hv_Index;

	//This procedure generates arrow shaped XLD contours,
	//pointing from (Row1, Column1) to (Row2, Column2).
	//If starting and end point are identical, a contour consisting
	//of a single point is returned.
	//
	//input parameteres:
	//Row1, Column1: Coordinates of the arrows' starting points
	//Row2, Column2: Coordinates of the arrows' end points
	//HeadLength, HeadWidth: Size of the arrow heads in pixels
	//
	//output parameter:
	//Arrow: The resulting XLD contour
	//
	//The input tuples Row1, Column1, Row2, and Column2 have to be of
	//the same length.
	//HeadLength and HeadWidth either have to be of the same length as
	//Row1, Column1, Row2, and Column2 or have to be a single element.
	//If one of the above restrictions is violated, an error will occur.
	//
	//
	//Init
	GenEmptyObj(&(*ho_Arrow));
	//
	//Calculate the arrow length
	DistancePp(hv_Row1, hv_Column1, hv_Row2, hv_Column2, &hv_Length);
	//
	//Mark arrows with identical start and end point
	//(set Length to -1 to avoid division-by-zero exception)
	hv_ZeroLengthIndices = hv_Length.TupleFind(0);
	if (0 != (hv_ZeroLengthIndices != -1))
	{
		hv_Length[hv_ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	hv_DR = (1.0*(hv_Row2 - hv_Row1)) / hv_Length;
	hv_DC = (1.0*(hv_Column2 - hv_Column1)) / hv_Length;
	hv_HalfHeadWidth = hv_HeadWidth / 2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) + (hv_HalfHeadWidth*hv_DC);
	hv_ColP1 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) - (hv_HalfHeadWidth*hv_DR);
	hv_RowP2 = (hv_Row1 + ((hv_Length - hv_HeadLength)*hv_DR)) - (hv_HalfHeadWidth*hv_DC);
	hv_ColP2 = (hv_Column1 + ((hv_Length - hv_HeadLength)*hv_DC)) + (hv_HalfHeadWidth*hv_DR);
	//
	//Finally create output XLD contour for each input point pair
	{
		HTuple end_val45 = (hv_Length.TupleLength()) - 1;
		HTuple step_val45 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
		{
			if (0 != (HTuple(hv_Length[hv_Index]) == -1))
			{
				//Create_ single points for arrows with identical start and end point
				GenContourPolygonXld(&ho_TempArrow, HTuple(hv_Row1[hv_Index]), HTuple(hv_Column1[hv_Index]));
			}
			else
			{
				//Create arrow contour
				GenContourPolygonXld(&ho_TempArrow, ((((HTuple(hv_Row1[hv_Index]).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP1[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index]))).TupleConcat(HTuple(hv_RowP2[hv_Index]))).TupleConcat(HTuple(hv_Row2[hv_Index])),
					((((HTuple(hv_Column1[hv_Index]).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP1[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index]))).TupleConcat(HTuple(hv_ColP2[hv_Index]))).TupleConcat(HTuple(hv_Column2[hv_Index])));
			}
			ConcatObj((*ho_Arrow), ho_TempArrow, &(*ho_Arrow));
		}
	}
	return;
}
void GetEachStartAndEndPoint(HTuple hv_Row, HTuple hv_Column, HTuple hv_Phi, HTuple hv_Length1,
	HTuple hv_Length2, HTuple *hv_StartRow, HTuple *hv_StartColumn, HTuple *hv_EndRow,
	HTuple *hv_EndColumn)
{

	// Local iconic variables
	HObject  ho_Rectangle, ho_RegionAffineTrans, ho_Contours;
	HObject  ho_ContoursSplit, ho_RegionLines1, ho_ObjectSelected;
	HObject  ho_FinalRegion, ho_ObjectSelected1, ho_ObjectSelected2;
	HObject  ho_ObjectSelected3, ho_ObjectSelected4, ho_RegionAffineTrans1;
	HObject  ho_line2, ho_line1, ho_line3, ho_line4, ho_startpoint1;
	HObject  ho_endpoint1, ho_startpoint2, ho_endpoint2;

	// Local control variables
	HTuple  hv_HomMat2D, hv_RowBegin, hv_ColBegin;
	HTuple  hv_RowEnd, hv_ColEnd, hv_Nr, hv_Nc, hv_Dist, hv_Number;
	HTuple  hv_meanrow, hv_meancolumn, hv_i, hv_Rows1, hv_Columns1;
	HTuple  hv_Mean1, hv_Mean2, hv_Indices, hv_Number1, hv_Indices1;
	HTuple  hv_Number2, hv_HomMat2D1, hv_null, hv_line1StartRow;
	HTuple  hv_line1StartColumn, hv_line1EndRow, hv_line1EndColumn;
	HTuple  hv_line2StartRow, hv_line2StartColumn, hv_line2EndRow;
	HTuple  hv_line2EndColumn;

	GenRectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2);
	VectorAngleToRigid(hv_Row, hv_Column, hv_Phi, hv_Row, hv_Column, 0, &hv_HomMat2D);
	AffineTransRegion(ho_Rectangle, &ho_RegionAffineTrans, hv_HomMat2D, "constant");
	GenContourRegionXld(ho_RegionAffineTrans, &ho_Contours, "border");
	SegmentContoursXld(ho_Contours, &ho_ContoursSplit, "lines", 5, 4, 2);
	FitLineContourXld(ho_ContoursSplit, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColBegin,
		&hv_RowEnd, &hv_ColEnd, &hv_Nr, &hv_Nc, &hv_Dist);
	GenRegionLine(&ho_RegionLines1, hv_RowBegin, hv_ColBegin, hv_RowEnd, hv_ColEnd);

	//****对旋转后的矩形进行排序
	CountObj(ho_RegionLines1, &hv_Number);
	hv_meanrow = HTuple();
	hv_meancolumn = HTuple();
	{
		HTuple end_val12 = hv_Number;
		HTuple step_val12 = 1;
		for (hv_i = 1; hv_i.Continue(end_val12, step_val12); hv_i += step_val12)
		{
			SelectObj(ho_RegionLines1, &ho_ObjectSelected, hv_i);
			GetRegionPoints(ho_ObjectSelected, &hv_Rows1, &hv_Columns1);
			TupleMean(hv_Rows1, &hv_Mean1);
			TupleMean(hv_Columns1, &hv_Mean2);
			TupleConcat(hv_meanrow, hv_Mean1, &hv_meanrow);
			TupleConcat(hv_meancolumn, hv_Mean2, &hv_meancolumn);
		}
	}
	TupleSortIndex(hv_meanrow, &hv_Indices);
	TupleLength(hv_Indices, &hv_Number1);
	TupleSortIndex(hv_meancolumn, &hv_Indices1);
	TupleLength(hv_Indices1, &hv_Number2);
	GenEmptyObj(&ho_FinalRegion);
	SelectObj(ho_RegionLines1, &ho_ObjectSelected1, HTuple(hv_Indices[0]) + 1);
	SelectObj(ho_RegionLines1, &ho_ObjectSelected2, HTuple(hv_Indices1[0]) + 1);
	SelectObj(ho_RegionLines1, &ho_ObjectSelected3, HTuple(hv_Indices[hv_Number1 - 1]) + 1);
	SelectObj(ho_RegionLines1, &ho_ObjectSelected4, HTuple(hv_Indices1[hv_Number2 - 1]) + 1);
	//***得到拍好顺序的矩形各个边，concat一下，旋转回去
	ConcatObj(ho_FinalRegion, ho_ObjectSelected1, &ho_FinalRegion);
	ConcatObj(ho_FinalRegion, ho_ObjectSelected3, &ho_FinalRegion);
	ConcatObj(ho_FinalRegion, ho_ObjectSelected2, &ho_FinalRegion);
	ConcatObj(ho_FinalRegion, ho_ObjectSelected4, &ho_FinalRegion);
	VectorAngleToRigid(hv_Row, hv_Column, 0, hv_Row, hv_Column, hv_Phi, &hv_HomMat2D1);
	AffineTransRegion(ho_FinalRegion, &ho_RegionAffineTrans1, hv_HomMat2D1, "constant");
	//**求四个角的坐标
	SelectObj(ho_RegionAffineTrans1, &ho_line2, 1);
	SelectObj(ho_RegionAffineTrans1, &ho_line1, 2);
	SelectObj(ho_RegionAffineTrans1, &ho_line3, 3);
	SelectObj(ho_RegionAffineTrans1, &ho_line4, 4);
	DilationCircle(ho_line1, &ho_line1, 6);
	DilationCircle(ho_line2, &ho_line2, 6);
	DilationCircle(ho_line3, &ho_line3, 6);
	DilationCircle(ho_line4, &ho_line4, 6);
	Intersection(ho_line1, ho_line4, &ho_startpoint1);
	AreaCenter(ho_startpoint1, &hv_null, &hv_line1StartRow, &hv_line1StartColumn);
	Intersection(ho_line1, ho_line3, &ho_endpoint1);
	AreaCenter(ho_endpoint1, &hv_null, &hv_line1EndRow, &hv_line1EndColumn);
	Intersection(ho_line2, ho_line3, &ho_startpoint2);
	AreaCenter(ho_startpoint2, &hv_null, &hv_line2StartRow, &hv_line2StartColumn);
	Intersection(ho_line2, ho_line4, &ho_endpoint2);
	AreaCenter(ho_endpoint2, &hv_null, &hv_line2EndRow, &hv_line2EndColumn);

	(*hv_StartRow) = HTuple();
	(*hv_StartColumn) = HTuple();
	(*hv_EndRow) = HTuple();
	(*hv_EndColumn) = HTuple();
	//**按照由内向外的搜索方向确定，各个边的起始坐标和终止坐标
	TupleConcat((*hv_StartRow), hv_line1StartRow, &(*hv_StartRow));
	TupleConcat((*hv_StartRow), hv_line2StartRow, &(*hv_StartRow));
	TupleConcat((*hv_StartRow), hv_line1EndRow, &(*hv_StartRow));
	TupleConcat((*hv_StartRow), hv_line2EndRow, &(*hv_StartRow));

	TupleConcat((*hv_StartColumn), hv_line1StartColumn, &(*hv_StartColumn));
	TupleConcat((*hv_StartColumn), hv_line2StartColumn, &(*hv_StartColumn));
	TupleConcat((*hv_StartColumn), hv_line1EndColumn, &(*hv_StartColumn));
	TupleConcat((*hv_StartColumn), hv_line2EndColumn, &(*hv_StartColumn));

	TupleConcat((*hv_EndRow), hv_line1EndRow, &(*hv_EndRow));
	TupleConcat((*hv_EndRow), hv_line2EndRow, &(*hv_EndRow));
	TupleConcat((*hv_EndRow), hv_line2StartRow, &(*hv_EndRow));
	TupleConcat((*hv_EndRow), hv_line1StartRow, &(*hv_EndRow));

	TupleConcat((*hv_EndColumn), hv_line1EndColumn, &(*hv_EndColumn));
	TupleConcat((*hv_EndColumn), hv_line2EndColumn, &(*hv_EndColumn));
	TupleConcat((*hv_EndColumn), hv_line2StartColumn, &(*hv_EndColumn));
	TupleConcat((*hv_EndColumn), hv_line1StartColumn, &(*hv_EndColumn));

	return;
}
IMPLEMENT_DYNAMIC(CPageLineSet, CDialogEx)

CPageLineSet::CPageLineSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageLineSet::IDD, pParent)
	, m_CB_nChoice(0)
	, m_EDT_nCoatGrayValue(0)
	, m_CB_nCalChoose(0)
{
	m_EDT_dMeasureLength = 0.0;
	m_EDT_nMeasureLinesNum = 0;
	m_EDT_dMeasureSigma = 0;
	m_EDT_nMeasureThres = 0;
	m_EDT_dMeasureWidth = 0;
	m_CB_nLineSelect = 0;
	m_CB_nSelect = 0;
	m_CB_nDirect = 0;
	m_hoDispRegion.GenEmptyObj();
	m_hoLine.GenEmptyObj();
}

CPageLineSet::~CPageLineSet()
{


}

void CPageLineSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEASURELENGTH, m_EDT_dMeasureLength);
	DDX_Text(pDX, IDC_EDIT_MEASURELINESNUM, m_EDT_nMeasureLinesNum);
	DDX_Text(pDX, IDC_EDIT_MEASURESIGMA, m_EDT_dMeasureSigma);
	DDX_Text(pDX, IDC_EDIT_MEASURETHRES, m_EDT_nMeasureThres);
	DDX_Text(pDX, IDC_EDIT_MEASUREWIDTH, m_EDT_dMeasureWidth);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_SELECT, m_CB_nLineSelect);
	DDX_CBIndex(pDX, IDC_COMBO_MEASURESELECT, m_CB_nSelect);
	DDX_CBIndex(pDX, IDC_COMBO_MEASURETRANSITION, m_CB_nDirect);
	//  DDX_Control(pDX, IDC_BNT_PARAMSET, m_BTN_ParamSet);
	//  DDX_Control(pDX, IDC_BNT_OPENIMAGE, m_BTN_OpenImage);
	DDX_Control(pDX, IDC_BNT_TEST, m_BTN_Test);
	//  DDX_Control(pDX, IDC_BNT_SAVE, m_BTN_Save);
	DDX_Control(pDX, IDC_BUTTON_DRAW_LINE, m_BTN_DrawLine);
	DDX_Control(pDX, IDC_BUTTON_LINE_TEST, m_BTN_LineTest);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_CHOICE, m_CB_nChoice);
	DDX_Control(pDX, IDC_BUTTON_MOD_LINE, m_BTN_ModLine);
	DDX_Text(pDX, IDC_EDT_COATGRAY, m_EDT_nCoatGrayValue);
	DDX_Control(pDX, IDC_STC_EXAMPLEIMAGE, m_STC_CExampleImage);
	DDX_CBIndex(pDX, IDC_COMBO_CALCULAT_CHOICE2, m_CB_nCalChoose);
}


BEGIN_MESSAGE_MAP(CPageLineSet, CDialogEx)
	ON_BN_CLICKED(IDC_BNT_TEST, &CPageLineSet::OnBnClickedButtonTest)
	//	ON_BN_CLICKED(IDC_BNT_SAVE, &CPageLineSet::OnBnClickedButtonSave)
	//	ON_BN_CLICKED(IDC_BNT_EXIT, &CPageLineSet::OnBnClickedButtonExit)
	//	ON_BN_CLICKED(IDC_BNT_OPENIMAGE, &CPageLineSet::OnBnClickedBntOpenimage)
	//	ON_BN_CLICKED(IDC_BNT_SAVEIMAGE, &CPageLineSet::OnBnClickedBntSaveimage)
	//	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_DRAW_LINE, &CPageLineSet::OnBnClickedButtonDrawLine)
	ON_BN_CLICKED(IDC_BUTTON_LINE_TEST, &CPageLineSet::OnBnClickedButtonLineTest)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_SELECT, &CPageLineSet::OnCbnSelchangeComboLineSelect)
	ON_EN_CHANGE(IDC_EDIT_MEASURELINESNUM, &CPageLineSet::OnEnChangeEditMeasurelinesnum)
	ON_EN_CHANGE(IDC_EDIT_MEASURELENGTH, &CPageLineSet::OnEnChangeEditMeasurelength)
	ON_EN_CHANGE(IDC_EDIT_MEASUREWIDTH, &CPageLineSet::OnEnChangeEditMeasurewidth)
	ON_EN_CHANGE(IDC_EDIT_MEASURESIGMA, &CPageLineSet::OnEnChangeEditMeasuresigma)
	ON_EN_CHANGE(IDC_EDIT_MEASURETHRES, &CPageLineSet::OnEnChangeEditMeasurethres)
	ON_CBN_SELCHANGE(IDC_COMBO_MEASURETRANSITION, &CPageLineSet::OnCbnSelchangeComboMeasuretransition)
	ON_CBN_SELCHANGE(IDC_COMBO_MEASURESELECT, &CPageLineSet::OnCbnSelchangeComboMeasureselect)
	//	ON_BN_CLICKED(IDC_BNT_PARAMSET, &CPageLineSet::OnBnClickedBntParamset)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//	ON_WM_ERASEBKGND()
	//	ON_WM_NCHITTEST()
	//	ON_WM_ACTIVATE()
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_CHOICE, &CPageLineSet::OnCbnSelchangeComboLineChoice)
	//	ON_BN_CLICKED(IDC_BNT_CANCEL, &CPageLineSet::OnBnClickedBntCancel)
	//	ON_BN_CLICKED(IDC_BNT_LANESET, &CPageLineSet::OnBnClickedBntLaneset)
	//	ON_BN_CLICKED(IDC_BUTTON_LINEMATCH, &CPageLineSet::OnBnClickedButtonLinematch)
	ON_BN_CLICKED(IDC_BUTTON_MOD_LINE, &CPageLineSet::OnBnClickedButtonModLine)

	//	ON_WM_MEASUREITEM()
	//	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_ONEKEYDRAW, &CPageLineSet::OnBnClickedBtnOnekeydraw)
	ON_EN_CHANGE(IDC_EDT_COATGRAY, &CPageLineSet::OnEnChangeEdtCoatgray)
	ON_BN_CLICKED(IDC_CALIB, &CPageLineSet::OnBnClickedCalib)
	ON_CBN_SELCHANGE(IDC_COMBO_CALCULAT_CHOICE2, &CPageLineSet::OnCbnSelchangeComboCalculatChoice2)
END_MESSAGE_MAP()


// CPageLineSet 消息处理程序
BOOL CPageLineSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitUI();
	try
	{

		m_hoPretreatImage = m_pParentDlg->m_hoCurrentDispImage;


		int n = m_CB_nLineSelect;
		m_EDT_nMeasureLinesNum = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nMeasureNum;
		m_EDT_dMeasureSigma = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dSigma;
		m_EDT_dMeasureLength = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dHeight;
		m_EDT_nMeasureThres = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nThr;
		m_EDT_dMeasureWidth = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dWidth;
		m_CB_nSelect = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nSelect;
		m_CB_nDirect = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nDirect;
		m_CB_nChoice = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nChoice;
		m_EDT_nCoatGrayValue = m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.nCoatThres;
		m_CB_nCalChoose = m_pParentDlg->m_pDetect->m_CSeperate.m_nCalculateChoose;
		InitCom();


		/*CRect rcImage;
		SetCheck("~father");
		m_STC_CExampleImage.GetClientRect(rcImage);
		OpenWindow(rcImage.top, rcImage.left, rcImage.Width(), rcImage.Height(), (Hlong)m_STC_CExampleImage.m_hWnd, "visible", "", &m_hv_ExampleWindowID);
		SetCheck("father");


		CString strFilepath;
		strFilepath.Format("%s\\res\\线位置示意图.png", GetCurrentAppPath());
		HObject hoPicture;
		HTuple hvWidth, hvHeight, hvNum;
		ReadImage(&hoPicture, (HTuple)strFilepath);
		CountObj(hoPicture, &hvNum);
		if (hvNum == 0)
		{
			AfxMessageBox("缺少划线示意图");
		}
		GetImageSize(hoPicture, &hvWidth, &hvHeight);
		SetPart(m_hv_ExampleWindowID, 0, 0, hvHeight, hvWidth);
		DispObj(hoPicture, m_hv_ExampleWindowID);*/
	}
	catch (HException e)
	{
		AfxMessageBox(e.ErrorMessage().Text());
	}

	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



// Procedures 
// Local procedures 
void CPageLineSet::GetFoilEdage(HObject ho_Image, HObject ho_Rectangle, HTuple *hv_LTRow, HTuple *hv_LTColumn,
	HTuple *hv_RBRow, HTuple *hv_RBColumn)
{

	HObject  ho_ImageReduced, ho_RegionClosing, ho_ConnectedRegions1;
	HObject  ho_FoilSelectedRegions;

	// Local control variables
	HTuple  hv_HeightValue;


	RegionFeatures(ho_Rectangle, "height", &hv_HeightValue);
	ReduceDomain(ho_Image, ho_Rectangle, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Rectangle, 150, 255);
	ClosingRectangle1(ho_Rectangle, &ho_RegionClosing, 5, 55);
	Connection(ho_RegionClosing, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_FoilSelectedRegions, (HTuple("height").Append("width")),
		"and", (hv_HeightValue / 2).TupleConcat(50), (HTuple(999999).Append(99999)));
	SmallestRectangle1(ho_FoilSelectedRegions, &(*hv_LTRow), &(*hv_LTColumn), &(*hv_RBRow),
		&(*hv_RBColumn));
	return;
}

void CPageLineSet::OnBnClickedButtonTest()
{
	try
	{
		if (TRUE == UpdateData(TRUE))
		{
			HTuple hvTimestart, hvTimeend;
			HTuple nWidth, nStraggerWidth;
			CountSeconds(&hvTimestart);
			HTuple hvHomMat2D;
			m_tDetectInput.hoGrabImage = m_hoPretreatImage;
			HomMat2dIdentity(&m_tDetectInput.hvHomMat2D);
			SEPARATEOUTPUT Output;
			m_pParentDlg->m_pDetect->m_CSeperate.Detect(m_tDetectInput, &Output);


			CountSeconds(&hvTimeend);
			SetColored(m_pParentDlg->m_hvWindowHandle, 12);
			DispObj(m_hoPretreatImage, m_pParentDlg->m_hvWindowHandle);
			DispObj(Output.ho_DispRegion, m_pParentDlg->m_hvWindowHandle);
			SetColor(m_pParentDlg->m_hvWindowHandle, "red");
			DispObj(Output.ho_Coat, m_pParentDlg->m_hvWindowHandle);
			SetColor(m_pParentDlg->m_hvWindowHandle, "blue");
			DispObj(Output.ho_Foil, m_pParentDlg->m_hvWindowHandle);

			for (int k = 0; k < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed; k++)
			{
				SetColor(m_pParentDlg->m_hvWindowHandle, "green");
				SetTposition(m_pParentDlg->m_hvWindowHandle, 100 * k, (m_pParentDlg->m_pDetect->m_CSeperate.m_hvColBegin[k] + m_pParentDlg->m_pDetect->m_CSeperate.m_hvColEnd[k]) / 2);
				CString strValue;
				strValue.Format("%d", k);
				WriteString(m_pParentDlg->m_hvWindowHandle, (HTuple)strValue);

			}

			if (m_pParentDlg->m_pDetect->m_tInit.nCoatType == M_TwoCam_Continue)
			{
				nWidth = Output.hv_SortWidth;
				m_pParentDlg->m_listInfo.DeleteAllItems();
				while (m_pParentDlg->m_listInfo.GetColumnCount()>0) m_pParentDlg->m_listInfo.DeleteColumn(0);

				for (int i = 0; i < nWidth.Length(); i++)
				{
					m_pParentDlg->m_listInfo.InsertColumn(i, Output.hv_Name[i].S(), LVCFMT_CENTER, 70);
				}
				m_pParentDlg->m_listInfo.InsertItem(0, "");
				for (int i = 0; i < nWidth.Length(); i++)
				{
					CString strWidth;
					strWidth.Format("%.3f", nWidth[i].D());
					m_pParentDlg->m_listInfo.SetItemText(0, i, strWidth);
				}
			}
			else
			{
				TupleInverse(Output.hv_SortWidth, &Output.hv_SortWidth);
				TupleInverse(Output.hv_Name, &Output.hv_Name);

				nWidth = Output.hv_SortWidth;
				m_pParentDlg->m_listInfo.DeleteAllItems();
				while (m_pParentDlg->m_listInfo.GetColumnCount()>0) m_pParentDlg->m_listInfo.DeleteColumn(0);

				for (int i = 0; i < nWidth.Length(); i++)
				{
					m_pParentDlg->m_listInfo.InsertColumn(i, Output.hv_Name[i].S(), LVCFMT_CENTER, 200);
				}
				m_pParentDlg->m_listInfo.InsertItem(0, "");
				for (int i = 0; i < nWidth.Length(); i++)
				{
					CString strWidth;
					strWidth.Format("%.3f", nWidth[i].D());
					m_pParentDlg->m_listInfo.SetItemText(0, i, strWidth);
				}
				for (int k = 0; k < m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->ho_ExtractCaoWeiRegions.CountObj(); k++)
				{

					DispObj(m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->ho_ExtractCaoWeiRegions.SelectObj(k + 1), m_pParentDlg->m_hvWindowHandle);
					HTuple hv_Row, hv_Column, hv_WidthValue, hv_HeightValue;
					hv_Row = m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->hv_CaoWeiYpositionTuple[k];
					hv_Column = m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->hv_CaoWeiXPositionTuple[k];
					hv_WidthValue = m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->hv_CaoWeiWidthTuple[k];
					hv_HeightValue = m_pParentDlg->m_pDetect->m_CSeperate.m_ptOut->hv_CaoWeiHeightTuple[k];
					CString strValue;
					strValue.Format("槽位%d \r\n宽度%.2f \r\n高度%.2f", k, hv_WidthValue.D(), hv_HeightValue.D());
					set_display_font(m_pParentDlg->m_hvWindowHandle, 12, "mono", "true", "false");
					disp_message(m_pParentDlg->m_hvWindowHandle, (HTuple)strValue, "image", hv_Row, hv_Column, "green", "false");

				}

			}









		}
	}
	catch (HException e)
	{

	}



}

//void CPageLineSet::OnBnClickedButtonSave()
//{
//	if(TRUE == UpdateData(TRUE))
//	{
//	
//		m_pParentDlg->m_pDetect->m_CSeperate.SaveParam();
//		AfxMessageBox("保存完毕");
//	}
//}

//void CPageLineSet::OnBnClickedBntOpenimage()
//{
//
//	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
//	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
//	if(IDOK == FileDlg.DoModal())
//	{
//		CString strFilename = FileDlg.GetPathName(); 
//		try
//		{
//			ReadImage(&m_tDetectInput.hoGrabImage,(HTuple)strFilename);
//		    m_hoPretreatImage = m_tDetectInput.hoGrabImage;
//
//
//
//			HTuple Width,Height;
//			GetImageSize(m_hoPretreatImage,&Width,&Height);
//			SetPart(m_pParentDlg->m_hvWindowHandle,0,0,Height-1,Width-1);
//			DispObj(m_hoPretreatImage,m_pParentDlg->m_hvWindowHandle);
//			
//
//		}catch(HException )
//		{
//			AfxMessageBox("读取图片失败");
//		}
//
//	}
//}




void CPageLineSet::SetButtonStaus(BOOL bStaus)
{
	HWND  hwndChild = ::GetWindow(GetSafeHwnd(), GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		switch (woc)
		{
		case IDC_STC_IMAGE:
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}
void CPageLineSet::OnBnClickedButtonDrawLine()
{

	SetButtonStaus(FALSE);

	HTuple	tRow0, tCol0, tRow1, tCol1, Phi, Length1, Length2;
	if (FALSE == m_hoPretreatImage.IsInitialized())
	{
		SetButtonStaus(TRUE);
		return;
	}
	m_hoDispRegion.GenEmptyObj();
	HTuple hv_Width(0), hv_Height(0);
	GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
	DrawLine(m_pParentDlg->m_hvWindowHandle, &tRow0, &tCol0, &tRow1, &tCol1);
	/*if (m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseReferSN >= 0)
	{
		if (0==m_pParentDlg->m_pDetect->m_CSeperate.m_tInit.nGrabberSN%2)
		{
			if (tCol0+100>m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose[m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseReferSN]||tCol1+100>m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose[m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseReferSN])
			{
				AfxMessageBox("坐标越界，请在分界线左侧画线！");
				SetButtonStaus(TRUE);
				return;
			}
		}
		else
		{
			if (tCol0-100<m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose[m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseReferSN]||tCol1-100<m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose[m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseReferSN])
			{
				AfxMessageBox("坐标越界，请在分界线右侧画线！");
				SetButtonStaus(TRUE);
				return;
			}
		}
	}*/

	//if (m_CB_nLineSelect==0||m_CB_nLineSelect==2||m_CB_nLineSelect==3||m_CB_nLineSelect==7||m_CB_nLineSelect==9||m_CB_nLineSelect==8)//极耳改为相对坐标
	//{
	//	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartRow=hv_Height[0].D()-tRow0[0].D();
	//	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartCol=tCol0[0].D();
	//	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndRow=hv_Height[0].D()-tRow1[0].D();
	//	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndCol=tCol1[0].D();
	//}else
	{
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartRow = tRow0[0].D();
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartCol = tCol0[0].D();
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndRow = tRow1[0].D();
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndCol = tCol1[0].D();
	}


	double dStartY, dStartX, dEndY, dEndX;
	m_pParentDlg->m_pDetect->m_CSeperate.FitLine(m_hoPretreatImage, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].AffineTrans(),
		&m_hoDispRegion, &dStartY, &dStartX, &dEndY, &dEndX, m_CB_nLineSelect, 1);
	GenLineArrow(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect], 80);
	FreshImage();
	SetButtonStaus(TRUE);
}
void CPageLineSet::FreshNewParam()
{
	UpdateData(TRUE);
	int n = m_CB_nLineSelect;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nMeasureNum = m_EDT_nMeasureLinesNum;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dSigma = m_EDT_dMeasureSigma;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dHeight = m_EDT_dMeasureLength;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nThr = m_EDT_nMeasureThres;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dWidth = m_EDT_dMeasureWidth;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nSelect = m_CB_nSelect;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nDirect = m_CB_nDirect;
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nChoice = m_CB_nChoice;
	m_pParentDlg->m_pDetect->m_CSeperate.m_nCalculateChoose = m_CB_nCalChoose;

	double dStartY, dStartX, dEndY, dEndX;
	m_pParentDlg->m_pDetect->m_CSeperate.FitLine(m_hoPretreatImage, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].AffineTrans(),
		&m_hoDispRegion, &dStartY, &dStartX, &dEndY, &dEndX, m_CB_nLineSelect, 1);
	FreshImage();
}
void CPageLineSet::FreshImage()
{
	SetColored(m_pParentDlg->m_hvWindowHandle, 12);
	DispObj(m_hoPretreatImage, m_pParentDlg->m_hvWindowHandle);

	if (m_hoDispRegion.IsInitialized())
	{
		DispObj(m_hoDispRegion, m_pParentDlg->m_hvWindowHandle);
	}

	SetColor(m_pParentDlg->m_hvWindowHandle, "yellow");
	DispObj(m_hoLine, m_pParentDlg->m_hvWindowHandle);
}
void CPageLineSet::OnBnClickedButtonLineTest()
{
	UpdateData(TRUE);
	m_hoDispRegion.GenEmptyObj();
	for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed; i++)
	{
		HObject hoDispRegion;
		double dStartY, dStartX, dEndY, dEndX;
		hoDispRegion.GenEmptyObj();
		m_pParentDlg->m_pDetect->m_CSeperate.FitLine(m_hoPretreatImage, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[i].AffineTrans(),
			&hoDispRegion, &dStartY, &dStartX, &dEndY, &dEndX, i);
		ConcatObj(m_hoDispRegion, hoDispRegion, &m_hoDispRegion);
		GenLineArrow(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[i], 80);
	}
	FreshImage();
}


void CPageLineSet::OnCbnSelchangeComboLineSelect()
{
	UpdateData(TRUE);
	int n = m_CB_nLineSelect;
	m_EDT_nMeasureLinesNum = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nMeasureNum;
	m_EDT_dMeasureSigma = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dSigma;
	m_EDT_dMeasureLength = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dHeight;
	m_EDT_nMeasureThres = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nThr;
	m_EDT_dMeasureWidth = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_dWidth;
	m_CB_nSelect = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nSelect;
	m_CB_nDirect = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nDirect;
	m_CB_nChoice = m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[n].m_nChoice;
	double dStartY, dStartX, dEndY, dEndX;
	m_pParentDlg->m_pDetect->m_CSeperate.FitLine(m_hoPretreatImage, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].AffineTrans(),
		&m_hoDispRegion, &dStartY, &dStartX, &dEndY, &dEndX, m_CB_nLineSelect, 1);
	GenLineArrow(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect], 80);
	FreshImage();
	UpdateData(FALSE);

}


void CPageLineSet::OnEnChangeEditMeasurelinesnum()
{
	FreshNewParam();
}


void CPageLineSet::OnEnChangeEditMeasurelength()
{
	FreshNewParam();
}


void CPageLineSet::OnEnChangeEditMeasurewidth()
{
	FreshNewParam();
}


void CPageLineSet::OnEnChangeEditMeasuresigma()
{
	FreshNewParam();
}


void CPageLineSet::OnEnChangeEditMeasurethres()
{
	FreshNewParam();
}


void CPageLineSet::OnCbnSelchangeComboMeasuretransition()
{
	FreshNewParam();
}


void CPageLineSet::OnCbnSelchangeComboMeasureselect()
{
	FreshNewParam();
}
void CPageLineSet::SetButtonColcor(CButtonExt& btn, short	shBtnColor, COLORREF BKOUT,
	COLORREF BKIN, COLORREF BKFOCUS, COLORREF FG)
{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	//btn.SetFont(&font);
}
void CPageLineSet::InitCom()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_LINE_SELECT))->ResetContent();
	CString strInsert;
	for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed; i++)
	{
		strInsert.Format("%d", i);
		((CComboBox*)GetDlgItem(IDC_COMBO_LINE_SELECT))->AddString(strInsert);
	}
}


void CPageLineSet::GenLineArrow(const CLineToolPara& Line, const HTuple& Size)
{
	try
	{
		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
		HTuple  hv_Rad, hv_RowCenter, hv_ColCenter, hv_Length, hv_Phi1;
		HTuple  hv_DiffRow, hv_DiffColumn, hv_DiffRowabs, hv_DiffColumnabs, hv_RowBegain, hv_ColumnBegain;
		HTuple  hv_RowEnd, hv_ColumnEnd;
		HTuple hv_Height;
		GetImageSize(m_hoPretreatImage, NULL, &hv_Height);
		hv_RowBegain = Line.m_dStartRow;
		hv_ColumnBegain = Line.m_dStartCol;
		hv_RowEnd = Line.m_dEndRow;
		hv_ColumnEnd = Line.m_dEndCol;
		hv_DiffRow = hv_RowEnd - hv_RowBegain;
		hv_DiffColumn = hv_ColumnEnd - hv_ColumnBegain;



		TupleAbs(hv_DiffRow, &hv_DiffRowabs);
		TupleAbs(hv_DiffColumn, &hv_DiffColumnabs);
		LinePosition(hv_RowBegain, hv_ColumnBegain, hv_RowEnd, hv_ColumnEnd, &hv_RowCenter, &hv_ColCenter,
			NULL, NULL);
		HObject hoArrowXLD;
		if (hv_DiffColumnabs >= hv_DiffRowabs)
		{
			if (hv_DiffColumn > 0)
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter + 300, hv_ColCenter, hv_RowCenter - 300, hv_ColCenter, Size, Size);
			}
			else
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter - 300, hv_ColCenter, hv_RowCenter + 300, hv_ColCenter, Size, Size);
			}

		}
		else
		{
			if (hv_DiffRow > 0)
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter, hv_ColCenter - 300, hv_RowCenter, hv_ColCenter + 300, Size, Size);
			}
			else
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter, hv_ColCenter + 300, hv_RowCenter, hv_ColCenter - 300, Size, Size);
			}
		}
		SetColor(m_pParentDlg->m_hvWindowHandle, "red");

		if (m_hoDispRegion.IsInitialized())
		{
			ConcatObj(m_hoDispRegion, hoArrowXLD, &m_hoDispRegion);
		}
		else
		{
			GenEmptyObj(&m_hoDispRegion);
			ConcatObj(m_hoDispRegion, hoArrowXLD, &m_hoDispRegion);
		}
	}
	catch (HException e)
	{
		CString strMessageBox;
		strMessageBox.Format("生成方向时候出现异常!\r\n异常信息：%s", e.ErrorMessage().Text());
		AfxMessageBox(strMessageBox);
	}

}
void CPageLineSet::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100, "Microsoft YaHei");



	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	SetButtonColcor(m_BTN_Test, shBtnColor, BKOUT, BKIN, BKFOCUS, FG);

	SetButtonColcor(m_BTN_DrawLine, shBtnColor, BKOUT, BKIN, BKFOCUS, FG);
	SetButtonColcor(m_BTN_LineTest, shBtnColor, BKOUT, BKIN, BKFOCUS, FG);
	SetButtonColcor(m_BTN_ModLine, shBtnColor, BKOUT, BKIN, BKFOCUS, FG);

	m_BTN_Test.SetFont(&m_ftButton);
	m_BTN_DrawLine.SetFont(&m_ftButton);
	m_BTN_LineTest.SetFont(&m_ftButton);
	m_BTN_ModLine.SetFont(&m_ftButton);





}

//void CPageLineSet::OnBnClickedBntParamset()
//{
//	//CDlgSet dlg;
//	//dlg.m_pOwner = m_pParentDlg->m_pDetect;
//	//if (IDOK == dlg.DoModal())
//	//{
//	//	m_hoPretreatImage = m_pDetectInput->hoGrabImage;
//	//}
//}


void CPageLineSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg, rcTitle, rcCtl;
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	//GetClientRect(rcDlg);  
	dc.FillSolidRect(rcDlg, RGB(200, 200, 200));


}


HBRUSH CPageLineSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}







void CPageLineSet::OnCbnSelchangeComboLineChoice()
{
	FreshNewParam();
}






void CPageLineSet::OnBnClickedButtonModLine()
{
	SetButtonStaus(FALSE);
	HTuple	tRow0, tCol0, tRow1, tCol1, Phi, Length1, Length2;
	if (FALSE == m_hoPretreatImage.IsInitialized())
	{
		SetButtonStaus(TRUE);
		return;
	}
	m_hoDispRegion.GenEmptyObj();


	HTuple hv_Width, hv_Height;
	GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
	DrawLineMod(m_pParentDlg->m_hvWindowHandle, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartRow,
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartCol,
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndRow,
		m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndCol,
		&tRow0, &tCol0, &tRow1, &tCol1);
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartRow = tRow0[0].D();
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dStartCol = tCol0[0].D();
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndRow = tRow1[0].D();
	m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].m_dEndCol = tCol1[0].D();
	double dStartY, dStartX, dEndY, dEndX;
	m_pParentDlg->m_pDetect->m_CSeperate.FitLine(m_hoPretreatImage, m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect].AffineTrans(),
		&m_hoDispRegion, &dStartY, &dStartX, &dEndY, &dEndX, m_CB_nLineSelect, 1);

	GenLineArrow(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[m_CB_nLineSelect], 80);
	FreshImage();


	SetButtonStaus(TRUE);
}

void CPageLineSet::GetCoatAndFoilRegions(HObject ho_Image, HObject ho_ROI_0, HObject *ho_FoilSortedRegions,
	HObject *ho_CoatSortedRegions)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_RegionClosing;
	HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionUnion;
	HObject  ho_RegionTrans, ho_RegionDifference, ho_ConnectedRegions1;
	HObject  ho_SelectedRegions1;

	// Local control variables
	HTuple  hv_HeightValue;

	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced);
	//****获取极耳区域
	Threshold(ho_ImageReduced, &ho_Region, 160, 255);
	RegionFeatures(ho_ROI_0, "height", &hv_HeightValue);
	ClosingRectangle1(ho_Region, &ho_RegionClosing, 15, hv_HeightValue / 2);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	ShapeTrans(ho_ConnectedRegions, &ho_RegionClosing, "rectangle1");
	SelectShape(ho_RegionClosing, &ho_SelectedRegions, "height", "and", hv_HeightValue / 2,
		hv_HeightValue);
	SortRegion(ho_SelectedRegions, &(*ho_FoilSortedRegions), "first_point", "true",
		"column");
	//****获取涂布区域
	Union1((*ho_FoilSortedRegions), &ho_RegionUnion);
	ShapeTrans(ho_RegionUnion, &ho_RegionTrans, "rectangle1");
	Difference(ho_RegionTrans, (*ho_FoilSortedRegions), &ho_RegionDifference);
	Connection(ho_RegionDifference, &ho_ConnectedRegions1);
	SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions1, "width", "and", 100, 99999);
	SortRegion(ho_SelectedRegions1, &(*ho_CoatSortedRegions), "first_point", "true",
		"column");
	return;
}

void CPageLineSet::SetDefaultLineParam(CLineToolPara& LineStruct)
{
	LineStruct.m_dWidth = 20;
	LineStruct.m_dHeight = 200;
	LineStruct.m_nMeasureNum = 20;
	LineStruct.m_nThr = 15;
	LineStruct.m_dSigma = 2;
}
//void CPageLineSet::OnMenuDrawcoat()
//{
//
//	if (m_pParentDlg->m_pDetect->m_tInit.nCoatType == M_TwoCam_Continue)
//	{
//		try
//		{
//			HTuple LTRow, LTColumn, RBRow, RBColumn;
//			HObject hoRectangle;
//			HTuple Row1, Column1, Row2, Column2;
//			HTuple hv_Height, hv_Width;
//			GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
//
//			SetButtonStaus(FALSE);
//			DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);
//			HObject hoRoI;
//			GenRectangle1(&hoRoI, Row1, Column1, Row2, Column2);
//			HObject hoFoilRegions, hoCoatRegions;
//			GetCoatAndFoilRegions(m_hoPretreatImage, hoRoI, &hoFoilRegions, &hoCoatRegions);
//			{
//				int nFoilNum, nCoatNum, nAT9Num;
//				nFoilNum = hoFoilRegions.CountObj();
//				nCoatNum = hoCoatRegions.CountObj();
//				nAT9Num = 2 * nCoatNum;
//				HTuple hvFLRow, hvFLColumn, hvFBRow, hv_FBColumn;
//				SmallestRectangle1(hoFoilRegions, &hvFLRow, &hvFLColumn, &hvFBRow, &hv_FBColumn);
//				HTuple hvCLRow, hvCLColumn, hvCBRow, hv_CBColumn;
//				SmallestRectangle1(hoCoatRegions, &hvCLRow, &hvCLColumn, &hvCBRow, &hv_CBColumn);
//				HTuple hv_StartAllRows, hv_StartAllColumns, hv_EndAllRows, hv_EndAllColumns;
//				hv_StartAllRows.Clear();
//				hv_StartAllColumns.Clear();
//				hv_EndAllRows.Clear();
//				hv_EndAllColumns.Clear();
//				hv_StartAllRows.Append(hvFLRow).Append(hvFLRow).Append(hvCLRow).Append(hvCLRow);
//				hv_StartAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
//				hv_EndAllRows.Append(hvFBRow).Append(hvFBRow).Append(hvCBRow).Append(hvCBRow);
//				hv_EndAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
//
//
//				HTuple hv_Index, hv_LineTuple;
//				TupleSortIndex(hv_StartAllColumns, &hv_Index);
//				TupleSelect(hv_StartAllRows, hv_Index, &hv_StartAllRows);
//				TupleSelect(hv_StartAllColumns, hv_Index, &hv_StartAllColumns);
//				TupleSelect(hv_EndAllRows, hv_Index, &hv_EndAllRows);
//				TupleSelect(hv_EndAllColumns, hv_Index, &hv_EndAllColumns);
//				TupleGenSequence(0, hv_StartAllRows.Length() - 1, 1, &hv_LineTuple);
//
//
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed = nFoilNum + nCoatNum + nAT9Num + 1;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount = nCoatNum;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount = nFoilNum;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount = nAT9Num;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nStaggerCount = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatLeftStartIndex = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilLeftStartIndex = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicLeftStartIndex = 0;
//
//
//
//
//				/*
//				double	m_dSigma;			//对应measure_pos中的Sigma
//				int		m_nThr;				//对应measure_pos中的Threshold
//				int		m_nDirect;			//对应measure_pos中的Transition:0-'all', 1-'positive', 2-'negative'
//				int		m_nSelect;			//对应measure_pos中的Select:0-'all', 1-'first', 2-'last'
//
//				double	m_dWidth;			//单个检测框的宽度
//				double	m_dHeight;			//单个检测框的高度
//				int		m_nMeasureNum;		//检测框的个数
//				double	m_dStartRow;		//起始点的Row坐标
//				double	m_dStartCol;		//起始点的Col坐标
//				double	m_dEndRow;			//终止点的Row坐标
//				double	m_dEndCol;			//终止点的Col坐标
//				*/
//
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount; i++)
//				{
//					int nLeft, nRight;
//					nLeft = 2 + i * 4;
//					nRight = nLeft + 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat1[i] = nLeft;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat2[i] = nRight;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 1;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 1;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 20;//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向
//
//
//				}
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount; i++)
//				{
//
//
//					int nLeft, nRight;
//					nLeft = i * 4;
//					nRight = nLeft + 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil1[i] = nLeft;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil2[i] = nRight;
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 2;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 80;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 2;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 80;//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向
//
//
//
//
//
//				}
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount; i++)
//				{
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic1[i] = 1 + 2 * i;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic2[i] = 1 + 2 * i + 1;
//
//				}
//
//			}
//
//
//			InitCom();
//			m_pParentDlg->m_pDetect->m_CSeperate.SaveParam();
//			m_pParentDlg->m_pDetect->m_CSeperate.SaveLineMatch();
//
//
//
//
//		}
//		catch (HException& e)
//		{
//
//			CString strValue;
//			strValue.Format("自动画线出现问题，错误函数:%s 错误内容:%s", e.ProcName().Text(), e.ErrorMessage().Text());
//			AfxMessageBox(strValue);
//		}
//
//		SetButtonStaus(TRUE);
//		OnBnClickedButtonLineTest();
//	}
//	else
//	{
//
//
//
//
//
//		HTuple hv_Height, hv_Width;
//		GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
//		HTuple Row, Column, Phi, Length1, Length2;
//		
//		HTuple LTRow, LTColumn, RBRow, RBColumn;
//		HObject hoRegion,hoConnections,hoFillUp,hoCoat;
//		try
//		{
//			//DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &LTRow, &LTColumn, &RBRow, &RBColumn);
//	
//			Threshold(m_hoPretreatImage,&hoRegion,MAX(0,m_EDT_nCoatGrayValue-10),MIN(m_EDT_nCoatGrayValue+10,255));
//			Connection(hoRegion,&hoConnections);
//			FillUp(hoConnections,&hoFillUp);
//			SelectShapeStd(hoFillUp,&hoFillUp,"max_area",70);
//			OpeningRectangle1(hoFillUp,&hoFillUp,101,101);
//			ClosingRectangle1(hoFillUp,&hoCoat,101,101);
//			if (hoCoat.CountObj()==1)
//			{
//				SmallestRectangle1(hoCoat,&LTRow, &LTColumn, &RBRow, &RBColumn);
//			}
//
//
//		}
//		catch (HException & e)
//		{
//		
//			AfxMessageBox("绘制有误");
//		}
//
//		{
//
//			try
//			{
//
//				double dOffsetPixel;
//				if (m_pParentDlg->m_pDetect->m_tInit.nGrabberSN==0)//前相机
//				{
//					if (m_pParentDlg->m_pDetect->m_tStandardSize.hvExistJXType==1)//前相机没有间歇
//					{
//						dOffsetPixel=0;
//
//						if (m_pParentDlg->m_pDetect->m_tStandardSize.hvQiTouXianChu==1)//间歇在底部
//						{
//
//
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice=2;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice=3;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice=3;
//
//						}else
//
//						{
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice=3;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice=2;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice=2;
//
//						}
//						
//
//					}else
//					{
//						
//						dOffsetPixel=m_pParentDlg->m_pDetect->m_tStandardSize.dUpIntermissionHeight.D() / m_pParentDlg->m_pDetect->m_dRowResolution;
//					}
//					
//
//				}
//				else
//				{
//					if (m_pParentDlg->m_pDetect->m_tStandardSize.hvExistJXType==0)//背面没有间歇
//					{
//						dOffsetPixel=1;
//						if (m_pParentDlg->m_pDetect->m_tStandardSize.hvQiTouXianChu==1)//间歇在底部
//						{
//
//
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice=2;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice=3;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice=3;
//
//						}else
//
//						{
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice=3;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice=2;
//							m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice=2;
//
//						}
//
//
//
//					}
//					else
//					{
//						dOffsetPixel=m_pParentDlg->m_pDetect->m_tStandardSize.dDownIntermissionHeight.D() / m_pParentDlg->m_pDetect->m_dRowResolution;
//					}
//					
//
//				}
//
//				if (m_pParentDlg->m_pDetect->m_tStandardSize.hvQiTouXianChu==1)//间歇在底部
//				{
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_nDirect = 2;
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nDirect = 2;
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartRow = MAX(0, hv_Height.D() - dOffsetPixel);
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndRow = MAX(1, hv_Height.D() - dOffsetPixel);
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nDirect = 1;
//
//				
//
//				}else
//				{
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_nDirect = 2;
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nDirect = 1;
//
//
//			
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartRow = MAX(0, dOffsetPixel);
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndRow = MAX(1, dOffsetPixel);
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nDirect = 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nDirect = 1;
//
//
//				}
//
//	
//
//				for (int i=0;i<m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed;i++)
//				{
//					SetDefaultLineParam(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[i]);
//				}
//
//			
//				OnBnClickedButtonLineTest();
//
//				/////继续寻找槽位制作模板  
//				if (m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType==3||(m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType==2 && m_pParentDlg->m_pDetect->m_tInit.nGrabberSN==1)||(m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType==1 && m_pParentDlg->m_pDetect->m_tInit.nGrabberSN==0))
//				{
//					//生成检测区域
//					HObject hoReducedImage,hoRegions;
//					ErosionCircle(hoCoat, &hoCoat, MAX(11,2*m_pParentDlg->m_pDetect->m_tParameters.tPretreatParam.dBorderWidth/m_pParentDlg->m_pDetect->m_dColResolution));
//					ReduceDomain(m_hoPretreatImage, hoCoat, &hoReducedImage);
//					Threshold(hoReducedImage, &hoRegions, 128, 255);
//					Connection(hoRegions, &hoRegions);
//					FillUp(hoRegions, &hoRegions);
//					SelectShape(hoRegions, &hoRegions, "rectangularity", "and", 0.8, 1);
//					SelectShape(hoRegions, &hoRegions, "inner_radius", "and", 20, 99999);
//					SortRegion(hoRegions, &hoRegions, "upper_left", "true", "column");
//					GenContourRegionXld(hoRegions, &hoRegions, "border");
//					HTuple Row, Column, Phi, Length1, Length2, PointOrder;
//					FitRectangle2ContourXld(hoRegions, "regression", -1, 0, 0, 3, 2, &Row, &Column, &Phi, &Length1, &Length2, &PointOrder);
//					GenRectangle2(&hoRegions, Row, Column, Phi, Length1, Length2);
//		
//					if (hoRegions.CountObj() ==m_pParentDlg->m_pDetect->m_tStandardSize.hvTripNum )
//					{
//						m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions.GenEmptyObj();
//						for (int i = 0; i < hoRegions.CountObj(); i++)
//						{
//							HObject hreg;
//							GenRectangle2(&hreg, Row[i], Column[i], Phi[i], Length1[i]+100, Length2[i]+100);
//							disp_message(m_pParentDlg->m_hvWindowHandle, i, "image", Row[i], Column[i], "yellow", "false");
//							ConcatObj(m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions, hreg, &m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions);
//						
//						   
//						}
//						SetColor(m_pParentDlg->m_hvWindowHandle,"green");
//						DispObj(m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions,m_pParentDlg->m_hvWindowHandle);
//					}else
//					{
//
//						AfxMessageBox("槽位数目和设置的槽位数目不符，请确认");
//					}
//
//
//				}
//			}
//			catch (HException& e)
//			{
//
//				
//			}
//
//
//
//		}
//		
//	}
//	
//
//}

//void CPageLineSet::OnMenuDrawcoat()
//{
//
//	if (m_pParentDlg->m_pDetect->m_tInit.nCoatType == M_TwoCam_Continue)
//	{
//		try
//		{
//			HTuple LTRow, LTColumn, RBRow, RBColumn;
//			HObject hoRectangle;
//			HTuple Row1, Column1, Row2, Column2;
//			HTuple hv_Height, hv_Width;
//			GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
//
//			SetButtonStaus(FALSE);
//			//DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);
//
//			HObject hoRegion, hoConnections, hoFillUp, hoCoat;
//			Threshold(m_hoPretreatImage, &hoRegion, MAX(0, m_EDT_nCoatGrayValue - 10), 255);
//			Connection(hoRegion, &hoConnections);
//			FillUp(hoConnections, &hoFillUp);
//			SelectShapeStd(hoFillUp, &hoFillUp, "max_area", 70);
//			OpeningRectangle1(hoFillUp, &hoFillUp, 101, 101);
//			ClosingRectangle1(hoFillUp, &hoCoat, 101, 101);
//			if (hoCoat.CountObj() != 1)
//			{
//				AfxMessageBox("自动绘制失败，请继续手动绘制");
//				DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);
//
//			}
//			else
//			{
//				HTuple LTRow, LTColumn, RBRow, RBColumn;
//				SmallestRectangle1(hoCoat, &LTRow, &LTColumn, &RBRow, &RBColumn);
//
//				Column2 = RBColumn + 30 / m_pParentDlg->m_pDetect->m_dColResolution;
//				Column1 = LTColumn - 30 / m_pParentDlg->m_pDetect->m_dColResolution;
//				Row1 = hv_Height / 2 - 30 / m_pParentDlg->m_pDetect->m_dRowResolution;
//				Row2 = hv_Height / 2 + 30 / m_pParentDlg->m_pDetect->m_dRowResolution;
//			}
//
//
//			HObject hoRoI;
//			GenRectangle1(&hoRoI, Row1, Column1, Row2, Column2);
//			HObject hoFoilRegions, hoCoatRegions;
//			GetCoatAndFoilRegions(m_hoPretreatImage, hoRoI, &hoFoilRegions, &hoCoatRegions);
//			{
//				int nFoilNum, nCoatNum, nAT9Num;
//				nFoilNum = hoFoilRegions.CountObj();
//				nCoatNum = hoCoatRegions.CountObj();
//				nAT9Num = 2 * nCoatNum;
//				HTuple hvFLRow, hvFLColumn, hvFBRow, hv_FBColumn;
//				SmallestRectangle1(hoFoilRegions, &hvFLRow, &hvFLColumn, &hvFBRow, &hv_FBColumn);
//				HTuple hvCLRow, hvCLColumn, hvCBRow, hv_CBColumn;
//				SmallestRectangle1(hoCoatRegions, &hvCLRow, &hvCLColumn, &hvCBRow, &hv_CBColumn);
//				HTuple hv_StartAllRows, hv_StartAllColumns, hv_EndAllRows, hv_EndAllColumns;
//				hv_StartAllRows.Clear();
//				hv_StartAllColumns.Clear();
//				hv_EndAllRows.Clear();
//				hv_EndAllColumns.Clear();
//				hv_StartAllRows.Append(hvFLRow).Append(hvFLRow).Append(hvCLRow).Append(hvCLRow);
//				hv_StartAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
//				hv_EndAllRows.Append(hvFBRow).Append(hvFBRow).Append(hvCBRow).Append(hvCBRow);
//				hv_EndAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
//
//
//				HTuple hv_Index, hv_LineTuple;
//				TupleSortIndex(hv_StartAllColumns, &hv_Index);
//				TupleSelect(hv_StartAllRows, hv_Index, &hv_StartAllRows);
//				TupleSelect(hv_StartAllColumns, hv_Index, &hv_StartAllColumns);
//				TupleSelect(hv_EndAllRows, hv_Index, &hv_EndAllRows);
//				TupleSelect(hv_EndAllColumns, hv_Index, &hv_EndAllColumns);
//				TupleGenSequence(0, hv_StartAllRows.Length() - 1, 1, &hv_LineTuple);
//
//
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed = nFoilNum + nCoatNum + nAT9Num + 1;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount = nCoatNum;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount = nFoilNum;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount = nAT9Num;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nStaggerCount = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatLeftStartIndex = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilLeftStartIndex = 0;
//				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicLeftStartIndex = 0;
//
//
//
//
//				/*
//				double	m_dSigma;			//对应measure_pos中的Sigma
//				int		m_nThr;				//对应measure_pos中的Threshold
//				int		m_nDirect;			//对应measure_pos中的Transition:0-'all', 1-'positive', 2-'negative'
//				int		m_nSelect;			//对应measure_pos中的Select:0-'all', 1-'first', 2-'last'
//
//				double	m_dWidth;			//单个检测框的宽度
//				double	m_dHeight;			//单个检测框的高度
//				int		m_nMeasureNum;		//检测框的个数
//				double	m_dStartRow;		//起始点的Row坐标
//				double	m_dStartCol;		//起始点的Col坐标
//				double	m_dEndRow;			//终止点的Row坐标
//				double	m_dEndCol;			//终止点的Col坐标
//				*/
//
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount; i++)
//				{
//					int nLeft, nRight;
//					nLeft = 2 + i * 4;
//					nRight = nLeft + 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat1[i] = nLeft;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat2[i] = nRight;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 1;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 1;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 20;//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向
//
//
//				}
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount; i++)
//				{
//
//
//					int nLeft, nRight;
//					nLeft = i * 4;
//					nRight = nLeft + 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil1[i] = nLeft;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil2[i] = nRight;
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 2;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 80;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向
//
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 2;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 80;//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向
//
//
//
//
//
//				}
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount; i++)
//				{
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic1[i] = 1 + 2 * i;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic2[i] = 1 + 2 * i + 1;
//
//				}
//
//			}
//
//
//			InitCom();
//			m_pParentDlg->m_pDetect->m_CSeperate.SaveParam();
//			m_pParentDlg->m_pDetect->m_CSeperate.SaveLineMatch();
//
//
//
//
//		}
//		catch (HException& e)
//		{
//
//			CString strValue;
//			strValue.Format("自动画线出现问题，错误函数:%s 错误内容:%s", e.ProcName().Text(), e.ErrorMessage().Text());
//			AfxMessageBox(strValue);
//		}
//
//		SetButtonStaus(TRUE);
//		OnBnClickedButtonLineTest();
//	}
//	else
//	{
//		HTuple hv_Height, hv_Width;
//		GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
//		HTuple Row, Column, Phi, Length1, Length2;
//
//		HTuple LTRow, LTColumn, RBRow, RBColumn;
//		HObject hoRegion, hoConnections, hoFillUp, hoCoat;
//		try
//		{
//			//DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &LTRow, &LTColumn, &RBRow, &RBColumn);
//			Threshold(m_hoPretreatImage, &hoRegion, MAX(0, m_EDT_nCoatGrayValue - 10), MIN(m_EDT_nCoatGrayValue + 10, 255));
//			Connection(hoRegion, &hoConnections);
//			FillUp(hoConnections, &hoFillUp);
//			SelectShapeStd(hoFillUp, &hoFillUp, "max_area", 70);
//			OpeningRectangle1(hoFillUp, &hoFillUp, 101, 101);
//			ClosingRectangle1(hoFillUp, &hoCoat, 101, 101);
//			ShapeTrans(hoCoat, &hoCoat, "convex");
//			if (hoCoat.CountObj() == 1)
//			{
//				SmallestRectangle1(hoCoat, &LTRow, &LTColumn, &RBRow, &RBColumn);
//			}
//		}
//		catch (HException & e)
//		{
//			AfxMessageBox("绘制有误");
//		}
//
//		{
//			try
//			{
//				//0涂布左边界；1涂布上边界；2涂布右边界；3涂布下边界；4间歇下边界；5左极耳边；6右极耳边
//				if (m_pParentDlg->m_pDetect->m_bExistJXFlag)//有间歇
//				{
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice = 0;
//				}
//				else
//				{
//					if (m_pParentDlg->m_pDetect->m_bExistJXUpDownFlag)//间歇在底部
//					{
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice = 2;
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice = 3;
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice = 3;
//					}
//					else
//					{
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nChoice = 3;
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nChoice = 2;
//						m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nChoice = 2;
//					}
//				}
//				if (m_pParentDlg->m_pDetect->m_bExistJXUpDownFlag)//间歇在顶部
//				{
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_nDirect = 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_nDirect = 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nDirect = 1;
//				}
//				else
//				{
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[0].m_nDirect = 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[2].m_nDirect = 1;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dStartRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[5].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dEndRow = RBRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[6].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndRow = 0;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[4].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dStartCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndRow = LTRow[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_dEndCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[3].m_nDirect = 2;
//
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dStartCol = LTColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndRow = hv_Height - 1;
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_dEndCol = RBColumn[0];
//					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[1].m_nDirect = 2;
//				}
//				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed; i++)
//				{
//					SetDefaultLineParam(m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[i]);
//				}
//
//				OnBnClickedButtonLineTest();
//
//				/////继续寻找槽位制作模板  
//				if (m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType == 3 || (m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType == 2 && m_pParentDlg->m_pDetect->m_tInit.nGrabberSN == 1) || (m_pParentDlg->m_pDetect->m_tStandardSize.hv_CaoWeiType == 1 && m_pParentDlg->m_pDetect->m_tInit.nGrabberSN == 0))
//				{
//					//生成检测区域
//					HObject hoReducedImage, hoRegions;
//					ErosionRectangle1(hoCoat, &hoCoat, MAX(11, 2 * m_pParentDlg->m_pDetect->m_tParameters.tPretreatParam.dBorderWidth / m_pParentDlg->m_pDetect->m_dColResolution), MAX(11, 2 * m_pParentDlg->m_pDetect->m_tParameters.tPretreatParam.dBorderWidth / m_pParentDlg->m_pDetect->m_dColResolution));
//					ReduceDomain(m_hoPretreatImage, hoCoat, &hoReducedImage);
//					Threshold(hoReducedImage, &hoRegions, 200, 255);
//					Connection(hoRegions, &hoRegions);
//					FillUp(hoRegions, &hoRegions);
//					SelectShape(hoRegions, &hoRegions, "rectangularity", "and", 0.8, 1);
//					SelectShape(hoRegions, &hoRegions, "inner_radius", "and", 20, 99999);
//					SortRegion(hoRegions, &hoRegions, "upper_left", "true", "column");
//					GenContourRegionXld(hoRegions, &hoRegions, "border");
//					HTuple Row, Column, Phi, Length1, Length2, PointOrder;
//					FitRectangle2ContourXld(hoRegions, "regression", -1, 0, 0, 3, 2, &Row, &Column, &Phi, &Length1, &Length2, &PointOrder);
//					GenRectangle2(&hoRegions, Row, Column, Phi, Length1, Length2);
//
//					if (hoRegions.CountObj() == m_pParentDlg->m_pDetect->m_tStandardSize.hvTripNum)
//					{
//						m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions.GenEmptyObj();
//						for (int i = 0; i < hoRegions.CountObj(); i++)
//						{
//							HObject hreg;
//							GenRectangle2(&hreg, Row[i], Column[i], Phi[i], Length1[i] + 100, Length2[i] + 100);
//							disp_message(m_pParentDlg->m_hvWindowHandle, i + 1, "image", Row[i], Column[i], "yellow", "false");
//							ConcatObj(m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions, hreg, &m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions);
//						}
//						SetColor(m_pParentDlg->m_hvWindowHandle, "green");
//						DispObj(m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.hoCaoweiRegions, m_pParentDlg->m_hvWindowHandle);
//					}
//					else
//					{
//						CString  Strmeg;
//						Strmeg.Format("槽位数目和设置的槽位数目不符，请确认,槽位数目%d设置数目%d", hoRegions.CountObj(), m_pParentDlg->m_pDetect->m_tStandardSize.hvTripNum.I());
//						AfxMessageBox(Strmeg);
//					}
//				}
//			}
//			catch (HException& e)
//			{
//
//
//			}
//		}
//
//	}
//}

void CPageLineSet::OnMenuDrawcoat()
{

	if (m_pParentDlg->m_pDetect->m_tInit.nCoatType == M_TwoCam_Continue)
	{
		try
		{
			HTuple LTRow, LTColumn, RBRow, RBColumn;
			HObject hoRectangle;
			HTuple Row1, Column1, Row2, Column2;
			HTuple hv_Height, hv_Width;
			GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);

			SetButtonStaus(FALSE);
			//DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);

			HObject hoRegion, hoConnections, hoFillUp, hoCoat;
			Threshold(m_hoPretreatImage, &hoRegion, MAX(0, m_EDT_nCoatGrayValue - 10), 255);
			Connection(hoRegion, &hoConnections);
			FillUp(hoConnections, &hoFillUp);
			SelectShapeStd(hoFillUp, &hoFillUp, "max_area", 70);
			OpeningRectangle1(hoFillUp, &hoFillUp, 101, 101);
			ClosingRectangle1(hoFillUp, &hoCoat, 101, 101);
			if (hoCoat.CountObj() != 1)
			{
				AfxMessageBox("自动绘制失败，请继续手动绘制");
				DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);

			}
			else
			{
				HTuple LTRow, LTColumn, RBRow, RBColumn;
				SmallestRectangle1(hoCoat, &LTRow, &LTColumn, &RBRow, &RBColumn);

				Column2 = RBColumn + 30 / m_pParentDlg->m_pDetect->m_dColResolution;
				Column1 = LTColumn - 30 / m_pParentDlg->m_pDetect->m_dColResolution;
				Row1 = hv_Height / 2 - 30 / m_pParentDlg->m_pDetect->m_dRowResolution;
				Row2 = hv_Height / 2 + 30 / m_pParentDlg->m_pDetect->m_dRowResolution;
			}


			HObject hoRoI;
			GenRectangle1(&hoRoI, Row1, Column1, Row2, Column2);
			HObject hoFoilRegions, hoCoatRegions;
			GetCoatAndFoilRegions(m_hoPretreatImage, hoRoI, &hoFoilRegions, &hoCoatRegions);
			{
				int nFoilNum, nCoatNum, nAT9Num;
				nFoilNum = hoFoilRegions.CountObj();
				nCoatNum = hoCoatRegions.CountObj();
				nAT9Num = 2 * nCoatNum;
				HTuple hvFLRow, hvFLColumn, hvFBRow, hv_FBColumn;
				SmallestRectangle1(hoFoilRegions, &hvFLRow, &hvFLColumn, &hvFBRow, &hv_FBColumn);
				HTuple hvCLRow, hvCLColumn, hvCBRow, hv_CBColumn;
				SmallestRectangle1(hoCoatRegions, &hvCLRow, &hvCLColumn, &hvCBRow, &hv_CBColumn);
				HTuple hv_StartAllRows, hv_StartAllColumns, hv_EndAllRows, hv_EndAllColumns;
				hv_StartAllRows.Clear();
				hv_StartAllColumns.Clear();
				hv_EndAllRows.Clear();
				hv_EndAllColumns.Clear();
				hv_StartAllRows.Append(hvFLRow).Append(hvFLRow).Append(hvCLRow).Append(hvCLRow);
				hv_StartAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
				hv_EndAllRows.Append(hvFBRow).Append(hvFBRow).Append(hvCBRow).Append(hvCBRow);
				hv_EndAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);


				HTuple hv_Index, hv_LineTuple;
				TupleSortIndex(hv_StartAllColumns, &hv_Index);
				TupleSelect(hv_StartAllRows, hv_Index, &hv_StartAllRows);
				TupleSelect(hv_StartAllColumns, hv_Index, &hv_StartAllColumns);
				TupleSelect(hv_EndAllRows, hv_Index, &hv_EndAllRows);
				TupleSelect(hv_EndAllColumns, hv_Index, &hv_EndAllColumns);
				TupleGenSequence(0, hv_StartAllRows.Length() - 1, 1, &hv_LineTuple);


				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nLineUsed = nFoilNum + nCoatNum + nAT9Num + 1;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount = nCoatNum;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount = nFoilNum;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount = nAT9Num;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nStaggerCount = 0;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatLeftStartIndex = 0;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilLeftStartIndex = 0;
				m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicLeftStartIndex = 0;




				/*
				double	m_dSigma;			//对应measure_pos中的Sigma
				int		m_nThr;				//对应measure_pos中的Threshold
				int		m_nDirect;			//对应measure_pos中的Transition:0-'all', 1-'positive', 2-'negative'
				int		m_nSelect;			//对应measure_pos中的Select:0-'all', 1-'first', 2-'last'

				double	m_dWidth;			//单个检测框的宽度
				double	m_dHeight;			//单个检测框的高度
				int		m_nMeasureNum;		//检测框的个数
				double	m_dStartRow;		//起始点的Row坐标
				double	m_dStartCol;		//起始点的Col坐标
				double	m_dEndRow;			//终止点的Row坐标
				double	m_dEndCol;			//终止点的Col坐标
				*/

				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCoatCount; i++)
				{
					int nLeft, nRight;
					nLeft = 2 + i * 4;
					nRight = nLeft + 1;
					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat1[i] = nLeft;
					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCoat2[i] = nRight;
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 1;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 20;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向

					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 1;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 20;//
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向


				}
				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nFoilCount; i++)
				{


					int nLeft, nRight;
					nLeft = i * 4;
					nRight = nLeft + 1;

					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil1[i] = nLeft;
					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vFoil2[i] = nRight;


					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nDirect = 2;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nThr = 80;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dHeight = 300;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nLeft].m_dSigma = 2;//正向


					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nDirect = 2;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nThr = 80;//
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dHeight = 300;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_nMeasureNum = 20;//正向
					m_pParentDlg->m_pDetect->m_CSeperate.m_sLineTool[nRight].m_dSigma = 2;//正向





				}
				for (int i = 0; i < m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.nCeramicCount; i++)
				{
					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic1[i] = 1 + 2 * i;
					m_pParentDlg->m_pDetect->m_CSeperate.m_tLineMatch.vCeramic2[i] = 1 + 2 * i + 1;

				}

			}


			InitCom();
			m_pParentDlg->m_pDetect->m_CSeperate.SaveParam();
			m_pParentDlg->m_pDetect->m_CSeperate.SaveLineMatch();




		}
		catch (HException& e)
		{

			CString strValue;
			strValue.Format("自动画线出现问题，错误函数:%s 错误内容:%s", e.ProcName().Text(), e.ErrorMessage().Text());
			AfxMessageBox(strValue);
		}

		SetButtonStaus(TRUE);
		OnBnClickedButtonLineTest();
	}
	else
	{
		m_pParentDlg->m_pDetect->m_CSeperate.FirstFindLine();

	}
}

void CPageLineSet::OnBnClickedBtnOnekeydraw()
{
	// TODO: 在此添加控件通知处理程序代码
	OnMenuDrawcoat();
}


void CPageLineSet::OnEnChangeEdtCoatgray()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	m_pParentDlg->m_pDetect->m_CSeperate.m_tMeausreParam.nCoatThres = m_EDT_nCoatGrayValue;
	// TODO:  在此添加控件通知处理程序代码
}


void CPageLineSet::OnBnClickedCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//资源切换
	CDlgCalibrate dlg;
	dlg.m_EDT_dCalSpaceDist = m_pParentDlg->m_pDetect->m_CSeperate.m_dCalSpaceDist;
	dlg.m_hvColPose = m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose;
	dlg.m_hvXPose = m_pParentDlg->m_pDetect->m_CSeperate.m_hvXPose;
	dlg.m_nPoseCount = m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseCount;
	dlg.m_EDT_dCoefficient = m_pParentDlg->m_pDetect->m_CSeperate.m_dCorCoef;
	dlg.m_hvRes = m_pParentDlg->m_pDetect->m_CSeperate.m_hvRes;
	dlg.m_hoImage = m_pParentDlg->m_hoCurrentDispImage;
	if (dlg.DoModal())
	{
		m_pParentDlg->m_pDetect->m_CSeperate.m_dCalSpaceDist = dlg.m_EDT_dCalSpaceDist;
		m_pParentDlg->m_pDetect->m_CSeperate.m_hvColPose = dlg.m_hvColPose;
		m_pParentDlg->m_pDetect->m_CSeperate.m_hvXPose = dlg.m_hvXPose;
		m_pParentDlg->m_pDetect->m_CSeperate.m_nPoseCount = dlg.m_nPoseCount;
		m_pParentDlg->m_pDetect->m_CSeperate.m_dCorCoef = dlg.m_EDT_dCoefficient;
		m_pParentDlg->m_pDetect->m_CSeperate.m_hvRes = dlg.m_hvRes;
	}
}


void CPageLineSet::OnCbnSelchangeComboCalculatChoice2()
{
	// TODO: 在此添加控件通知处理程序代码
	FreshNewParam();
}
