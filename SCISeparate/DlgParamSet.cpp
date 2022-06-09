// DlgParamSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SCISeparate.h"
#include "DlgParamSet.h"
#include "afxdialogex.h"
#include "DlgCalibrate.h"
#include "DlgSet.h"

// CDlgParamSet 对话框
void gen_arrow_contour_xld (HObject *ho_Arrow, HTuple hv_Row1, HTuple hv_Column1, 
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
	if (0 != (hv_ZeroLengthIndices!=-1))
	{
		hv_Length[hv_ZeroLengthIndices] = -1;
	}
	//
	//Calculate auxiliary variables.
	hv_DR = (1.0*(hv_Row2-hv_Row1))/hv_Length;
	hv_DC = (1.0*(hv_Column2-hv_Column1))/hv_Length;
	hv_HalfHeadWidth = hv_HeadWidth/2.0;
	//
	//Calculate end points of the arrow head.
	hv_RowP1 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))+(hv_HalfHeadWidth*hv_DC);
	hv_ColP1 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))-(hv_HalfHeadWidth*hv_DR);
	hv_RowP2 = (hv_Row1+((hv_Length-hv_HeadLength)*hv_DR))-(hv_HalfHeadWidth*hv_DC);
	hv_ColP2 = (hv_Column1+((hv_Length-hv_HeadLength)*hv_DC))+(hv_HalfHeadWidth*hv_DR);
	//
	//Finally create output XLD contour for each input point pair
	{
		HTuple end_val45 = (hv_Length.TupleLength())-1;
		HTuple step_val45 = 1;
		for (hv_Index=0; hv_Index.Continue(end_val45, step_val45); hv_Index += step_val45)
		{
			if (0 != (HTuple(hv_Length[hv_Index])==-1))
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
void GetEachStartAndEndPoint (HTuple hv_Row, HTuple hv_Column, HTuple hv_Phi, HTuple hv_Length1, 
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
  for (hv_i=1; hv_i.Continue(end_val12, step_val12); hv_i += step_val12)
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
  SelectObj(ho_RegionLines1, &ho_ObjectSelected1, HTuple(hv_Indices[0])+1);
  SelectObj(ho_RegionLines1, &ho_ObjectSelected2, HTuple(hv_Indices1[0])+1);
  SelectObj(ho_RegionLines1, &ho_ObjectSelected3, HTuple(hv_Indices[hv_Number1-1])+1);
  SelectObj(ho_RegionLines1, &ho_ObjectSelected4, HTuple(hv_Indices1[hv_Number2-1])+1);
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
  /* TupleConcat((*hv_StartRow), hv_line1StartRow, &(*hv_StartRow));
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
  */
  //**按照由外向内的搜索方向确定，各个边的起始坐标和终止坐标
  TupleConcat((*hv_EndRow), hv_line1StartRow, &(*hv_EndRow));
  TupleConcat((*hv_EndRow), hv_line2StartRow, &(*hv_EndRow));
  TupleConcat((*hv_EndRow), hv_line1EndRow, &(*hv_EndRow));
  TupleConcat((*hv_EndRow), hv_line2EndRow, &(*hv_EndRow));

  TupleConcat((*hv_EndColumn), hv_line1StartColumn, &(*hv_EndColumn));
  TupleConcat((*hv_EndColumn), hv_line2StartColumn, &(*hv_EndColumn));
  TupleConcat((*hv_EndColumn), hv_line1EndColumn, &(*hv_EndColumn));
  TupleConcat((*hv_EndColumn), hv_line2EndColumn, &(*hv_EndColumn));

  TupleConcat((*hv_StartRow), hv_line1EndRow, &(*hv_StartRow));
  TupleConcat((*hv_StartRow), hv_line2EndRow, &(*hv_StartRow));
  TupleConcat((*hv_StartRow), hv_line2StartRow, &(*hv_StartRow));
  TupleConcat((*hv_StartRow), hv_line1StartRow, &(*hv_StartRow));

  TupleConcat((*hv_StartColumn), hv_line1EndColumn, &(*hv_StartColumn));
  TupleConcat((*hv_StartColumn), hv_line2EndColumn, &(*hv_StartColumn));
  TupleConcat((*hv_StartColumn), hv_line2StartColumn, &(*hv_StartColumn));
  TupleConcat((*hv_StartColumn), hv_line1StartColumn, &(*hv_StartColumn));

  return;
}
IMPLEMENT_DYNAMIC(CDlgParamSet, CDialogEx)

	CDlgParamSet::CDlgParamSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParamSet::IDD, pParent)
	, m_CB_nChoice(0)
	, m_CHK_bEnableVariableWidth(FALSE)
	, m_EDT_dRelateOffset(0)
{
	m_pTempDetect = new CSeparate;
	m_EDT_dMeasureLength = 0.0;
	m_EDT_nMeasureLinesNum = 0;
	m_EDT_dMeasureSigma = 0;
	m_EDT_nMeasureThres = 0;
	m_EDT_dMeasureWidth = 0;
	m_CB_nLineSelect = 0;
	m_CB_nSelect = 0;
	m_CB_nDirect = 0;
	m_pMenuTools = NULL;
	//  m_CB_nPosition = 0;
	m_List_Font.CreatePointFont(100,"Microsoft YaHei");
	m_COM_nSortType = 0;
	//  m_EDT_dCurrentWidth = 0.0;
	//  m_EDT_dRealWidth = 0.0;
	//  m_EDT_dColumnResolution = 0.0;
	//  m_EDT_dMinScore = 0.0;
	//  m_EDT_nMaxMatchNum = 0.0;
	//  m_COM_bUseAoBanSeg = 0;
	m_COM_bFindAobanManualOrAuto = 0;
	m_EDT_nAoBanThresh = 0;
	m_EDT_dAoBanDetectRegionWidth = 0.0;
	m_EDT_dFoilDetectRegionWidth = 0.0;
	m_EDT_nFoilDetectThresh = 0;
}

CDlgParamSet::~CDlgParamSet()
{
	delete m_pTempDetect;
	if (m_pMenuTools)
	{
		delete m_pMenuTools;
	}
}

void CDlgParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STC_IMAGE, m_STC_Image);
	DDX_Control(pDX, IDC_STATIC_GRAYVALUE, m_STC_GrayValue);
	DDX_Control(pDX, IDC_LC_CHECKINFO, m_LC_CheckInfo);
	DDX_Text(pDX, IDC_EDIT_MEASURELENGTH, m_EDT_dMeasureLength);
	DDX_Text(pDX, IDC_EDIT_MEASURELINESNUM, m_EDT_nMeasureLinesNum);
	DDX_Text(pDX, IDC_EDIT_MEASURESIGMA, m_EDT_dMeasureSigma);
	DDX_Text(pDX, IDC_EDIT_MEASURETHRES, m_EDT_nMeasureThres);
	DDX_Text(pDX, IDC_EDIT_MEASUREWIDTH, m_EDT_dMeasureWidth);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_SELECT, m_CB_nLineSelect);
	DDX_CBIndex(pDX, IDC_COMBO_MEASURESELECT, m_CB_nSelect);
	DDX_CBIndex(pDX, IDC_COMBO_MEASURETRANSITION, m_CB_nDirect);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_BNT_PARAMSET, m_BTN_ParamSet);
	DDX_Control(pDX, IDC_BNT_OPENIMAGE, m_BTN_OpenImage);
	DDX_Control(pDX, IDC_BNT_SAVEIMAGE, m_BTN_SaveImage);
	DDX_Control(pDX, IDC_BNT_TEST, m_BTN_Test);
	DDX_Control(pDX, IDC_BNT_SAVE, m_BTN_Save);
	DDX_Control(pDX, IDC_BNT_EXIT, m_BTN_Exit);
	DDX_Control(pDX, IDC_BUTTON_DRAW_LINE, m_BTN_DrawLine);
	DDX_Control(pDX, IDC_BUTTON_LINE_TEST, m_BTN_LineTest);
	DDX_CBIndex(pDX, IDC_COMBO_LINE_CHOICE, m_CB_nChoice);
	DDX_Control(pDX, IDC_BNT_CANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDC_BUTTON_MOD_LINE, m_BTN_ModLine);
	DDX_Control(pDX, IDC_BTN_CALIB, m_BTN_CalibSet);
	DDX_Control(pDX, IDC_STC_EXAMPLE, m_STC_Example);
	DDX_CBIndex(pDX, IDC_COM_SORT, m_COM_nSortType);
	//  DDX_CBIndex(pDX, IDC_COM_AOBANREGIONMODE, m_COM_bUseAoBanSeg);
	DDX_Check(pDX, IDC_CHK_USEAOBANSEG, m_CHK_bUseAoBanSegMode);
	DDX_CBIndex(pDX, IDC_COM_AOBANREGIONMODE, m_COM_bFindAobanManualOrAuto);
	DDX_Text(pDX, IDC_EDT_AOBANDETECTTHRESH, m_EDT_nAoBanThresh);
	DDX_Text(pDX, IDC_EDT_AOBANETECTREGION, m_EDT_dAoBanDetectRegionWidth);
	DDX_Text(pDX, IDC_EDT_FOILDETECTREGION, m_EDT_dFoilDetectRegionWidth);
	DDX_Text(pDX, IDC_EDT_FOILDETECTTHRESH, m_EDT_nFoilDetectThresh);
	DDX_Check(pDX, IDC_CHK_USEVARIABWIDTHDETECT, m_CHK_bEnableVariableWidth);
	DDX_Text(pDX, IDC_EDT_RELATEOFFSET, m_EDT_dRelateOffset);
}


BEGIN_MESSAGE_MAP(CDlgParamSet, CDialogEx)
	ON_BN_CLICKED(IDC_BNT_TEST, &CDlgParamSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BNT_SAVE, &CDlgParamSet::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BNT_EXIT, &CDlgParamSet::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BNT_OPENIMAGE, &CDlgParamSet::OnBnClickedBntOpenimage)
	ON_BN_CLICKED(IDC_BNT_SAVEIMAGE, &CDlgParamSet::OnBnClickedBntSaveimage)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_DRAW_LINE, &CDlgParamSet::OnBnClickedButtonDrawLine)
	ON_BN_CLICKED(IDC_BUTTON_LINE_TEST, &CDlgParamSet::OnBnClickedButtonLineTest)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_SELECT, &CDlgParamSet::OnCbnSelchangeComboLineSelect)
	ON_EN_CHANGE(IDC_EDIT_MEASURELINESNUM, &CDlgParamSet::OnEnChangeEditMeasurelinesnum)
	ON_EN_CHANGE(IDC_EDIT_MEASURELENGTH, &CDlgParamSet::OnEnChangeEditMeasurelength)
	ON_EN_CHANGE(IDC_EDIT_MEASUREWIDTH, &CDlgParamSet::OnEnChangeEditMeasurewidth)
	ON_EN_CHANGE(IDC_EDIT_MEASURESIGMA, &CDlgParamSet::OnEnChangeEditMeasuresigma)
	ON_EN_CHANGE(IDC_EDIT_MEASURETHRES, &CDlgParamSet::OnEnChangeEditMeasurethres)
	ON_CBN_SELCHANGE(IDC_COMBO_MEASURETRANSITION, &CDlgParamSet::OnCbnSelchangeComboMeasuretransition)
	ON_CBN_SELCHANGE(IDC_COMBO_MEASURESELECT, &CDlgParamSet::OnCbnSelchangeComboMeasureselect)
	ON_BN_CLICKED(IDC_BNT_PARAMSET, &CDlgParamSet::OnBnClickedBntParamset)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_ACTIVATE()
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_CHOICE, &CDlgParamSet::OnCbnSelchangeComboLineChoice)
	ON_BN_CLICKED(IDC_BNT_CANCEL, &CDlgParamSet::OnBnClickedBntCancel)
	ON_BN_CLICKED(IDC_BUTTON_MOD_LINE, &CDlgParamSet::OnBnClickedButtonModLine)
	ON_COMMAND(ID_MENU_DRAWCOAT, &CDlgParamSet::OnMenuDrawcoat)
	ON_COMMAND(ID_MENU_DRAWFOIL, &CDlgParamSet::OnMenuDrawfoil)
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BTN_CALIB, &CDlgParamSet::OnBnClickedBtnCalib)

ON_CBN_SELCHANGE(IDC_COM_SORT, &CDlgParamSet::OnSelchangeComSort)
ON_EN_CHANGE(IDC_EDT_FOILDETECTREGION, &CDlgParamSet::OnEnChangeEdtFoildetectregion)
ON_BN_CLICKED(IDC_CHK_USEAOBANSEG, &CDlgParamSet::OnBnClickedChkUseaobanseg)
ON_CBN_SELCHANGE(IDC_COM_AOBANREGIONMODE, &CDlgParamSet::OnCbnSelchangeComAobanregionmode)
ON_EN_CHANGE(IDC_EDT_FOILDETECTTHRESH, &CDlgParamSet::OnEnChangeEdtFoildetectthresh)
ON_EN_CHANGE(IDC_EDT_AOBANETECTREGION, &CDlgParamSet::OnEnChangeEdtAobanetectregion)
ON_EN_CHANGE(IDC_EDT_AOBANDETECTTHRESH, &CDlgParamSet::OnEnChangeEdtAobandetectthresh)
//ON_BN_CLICKED(IDC_BTN_DRAWRECTANGLE, &CDlgParamSet::OnBnClickedBtnDrawrectangle)
//ON_BN_CLICKED(IDC_BTN_DRAWRIGHTRECTANGLE, &CDlgParamSet::OnBnClickedBtnDrawrightrectangle)
ON_BN_CLICKED(IDC_CHK_USEVARIABWIDTHDETECT, &CDlgParamSet::OnBnClickedChkUsevariabwidthdetect)
ON_EN_CHANGE(IDC_EDT_RELATEOFFSET, &CDlgParamSet::OnEnChangeEdtRelateoffset)
END_MESSAGE_MAP()



// CDlgParamSet 消息处理程序




BOOL CDlgParamSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	InitUI();
	try
	{
		if (FALSE == m_pDetectInput->hoGrabImage.IsInitialized())
		{
			GenImageConst(&m_pDetectInput->hoGrabImage,"byte",4096,6000);
		}
		if (0 == m_pDetectInput->hvHomMat2D.Length())
		{
			HomMat2dIdentity(&m_pDetectInput->hvHomMat2D);
		}
		if (m_pTempDetect->m_tParameters.bPretreat)
		{
			m_hoPretreatImage=m_pTempDetect->m_hoPretreatImage;		
		}else
		{
			m_hoPretreatImage = m_pDetectInput->hoGrabImage;
		}


		

		CRect rcImage;
		m_STC_Image.GetClientRect(rcImage);
		SetCheck("~father");
		///////两个窗体//
		OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),(Hlong)m_STC_Image.m_hWnd,"visible","",&m_hvWindowID);

		m_STC_Example.GetClientRect(rcImage);
		OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),(Hlong)m_STC_Example.m_hWnd,"visible","",&m_hv_ExampleWindowID);

		SetCheck("father");
		SetDraw(m_hvWindowID, "margin");
		SetColor(m_hvWindowID, "red");
		SetColored(m_hvWindowID,12);
		HTuple Width,Height;
		GetImageSize(m_hoPretreatImage,&Width,&Height);
		SetPart(m_hvWindowID,0,0,Height-1,Width-1);
		DispObj(m_hoPretreatImage,m_hvWindowID);


		m_dDispImagePartRow0 = 0;
		m_dDispImagePartCol0 = 0;
		m_dDispImagePartRow1 = Height - 1;
		m_dDispImagePartCol1 = Width - 1;
		m_LC_CheckInfo.DeleteAllItems();
		m_LC_CheckInfo.SetFont(&m_List_Font);
		m_LC_CheckInfo.ModifyStyle(NULL,LVS_REPORT);
		m_LC_CheckInfo.SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
		m_LC_CheckInfo.InsertColumn(0, "属性", LVCFMT_LEFT,140);
		m_LC_CheckInfo.InsertColumn(1, "值", LVCFMT_LEFT,140);

		//////////////////////////////////////////////////////////////////////////
		CString strDisp;
		strDisp.Format("坐标:  X:-- Y:--\r\n \r\n灰度值: --");
		m_STC_GrayValue.SetWindowTextA(strDisp);
		int n = m_CB_nLineSelect;
		m_EDT_nMeasureLinesNum = m_pTempDetect->m_sLineTool[n].m_nMeasureNum;
		m_EDT_dMeasureSigma    = m_pTempDetect->m_sLineTool[n].m_dSigma;
		m_EDT_dMeasureLength   = m_pTempDetect->m_sLineTool[n].m_dHeight;
		m_EDT_nMeasureThres    = m_pTempDetect->m_sLineTool[n].m_nThr;
		m_EDT_dMeasureWidth    = m_pTempDetect->m_sLineTool[n].m_dWidth;
		m_CB_nSelect           = m_pTempDetect->m_sLineTool[n].m_nSelect;
		m_CB_nDirect           = m_pTempDetect->m_sLineTool[n].m_nDirect;
		m_CB_nChoice           = m_pTempDetect->m_sLineTool[n].m_nChoice;
		m_COM_nSortType        = m_pTempDetect->m_sLineTool[n].m_nSortType;

		/////////////////////////////////////////
		m_CHK_bUseAoBanSegMode=m_pTempDetect->m_tParameters.bEnaleUseSegAoBan;
		m_COM_bFindAobanManualOrAuto=m_pTempDetect->m_tParameters.bFindManualOrAuto;
		m_EDT_dFoilDetectRegionWidth=m_pTempDetect->m_tParameters.dExtractFoilRegionWidth;
		m_EDT_nFoilDetectThresh=m_pTempDetect->m_tParameters.nExtractFoilThresh;
		m_EDT_dAoBanDetectRegionWidth=m_pTempDetect->m_tParameters.dExtractAobanRegionWidth;
		m_EDT_nAoBanThresh=m_pTempDetect->m_tParameters.nExtractAobanThresh;
		m_CHK_bEnableVariableWidth=m_pTempDetect->m_tParameters.bEnableVariableWidth;

		m_EDT_dRelateOffset=m_pTempDetect->m_tParameters.dRelateOffset;
		





		InitCom();
		CString strRightFilepath,strLeftFilepath;

		if (m_pTempDetect->m_tInit.nProductType==positive)
		{
			strLeftFilepath.Format("%s\\res\\ZJLeft.png",GetCurrentAppPath());
			strRightFilepath.Format("%s\\res\\ZJRight.png",GetCurrentAppPath());

		}else
		{
			strLeftFilepath.Format("%s\\res\\FJLeft.png",GetCurrentAppPath());
			strRightFilepath.Format("%s\\res\\FJRight.png",GetCurrentAppPath());
		}
		HObject hoPicture;
		HTuple hvWidth,hvHeight,hvNum;
		if (0==m_pTempDetect->m_tInit.nGrabberSN || 2==m_pTempDetect->m_tInit.nGrabberSN)
		{
			ReadImage(&hoPicture,(HTuple)strLeftFilepath);
		}else
		{
			ReadImage(&hoPicture,(HTuple)strRightFilepath);
		}
		
		CountObj(hoPicture,&hvNum);
		if (hvNum == 0)
		{
			AfxMessageBox("缺少划线示意图");
		}
		GetImageSize(hoPicture,&hvWidth,&hvHeight);
		if (1 ==m_pTempDetect->m_tParameters.nLaneCount)
		{
			SetPart(m_hv_ExampleWindowID,0,0,hvHeight,hvWidth);
			DispObj(hoPicture,m_hv_ExampleWindowID);
		}else if (2 == m_pTempDetect->m_tParameters.nLaneCount)
		{
			SetPart(m_hv_ExampleWindowID,0,0,hvHeight,hvWidth);
			DispObj(hoPicture,m_hv_ExampleWindowID);
		}
	}catch(HException e)
	{
		AfxMessageBox(e.ErrorMessage().Text());
	}

	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgParamSet::ComPareChange(double& LastValue,double& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pTempDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("相机%d的尺寸分割检测参数%s由%.2f更改为%.2f",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pTempDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgParamSet::ComPareChange(int& LastValue,int& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pTempDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("相机%d的尺寸分割检测参数%s由%d更改为%d",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pTempDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgParamSet::SaveOperaterLog()//
{
	///预处理
	ComPareChange(m_pRealDetect->m_tParameters.nSizePretreat,m_pTempDetect->m_tParameters.nSizePretreat,"预处理");
	ComPareChange(m_pRealDetect->m_tParameters.nXSizePretreat,m_pTempDetect->m_tParameters.nXSizePretreat,"预处理尺寸");
	ComPareChange(m_pRealDetect->m_tParameters.bUseGRR,m_pTempDetect->m_tParameters.bUseGRR,"GRR开关");
	ComPareChange(m_pRealDetect->m_tParameters.nHcalibFlag,m_pTempDetect->m_tParameters.nHcalibFlag,"标定模式");
	ComPareChange(m_pRealDetect->m_tParameters.nSelectNum,m_pTempDetect->m_tParameters.nSelectNum,"GRR选取点的数目");
	ComPareChange(m_pRealDetect->m_tParameters.nExtractFoilThresh,m_pTempDetect->m_tParameters.nExtractFoilThresh,"极耳的提取阈值");
	ComPareChange(m_pRealDetect->m_tParameters.nFitRegionMode,m_pTempDetect->m_tParameters.nFitRegionMode,"区域拟合方式");
	ComPareChange(m_pRealDetect->m_tParameters.dLocateMissingOffset,m_pTempDetect->m_tParameters.dLocateMissingOffset,"定位不齐系数");
	ComPareChange(m_pRealDetect->m_tParameters.nMaxDiffColumn,m_pTempDetect->m_tParameters.nMaxDiffColumn,"起始列坐标偏差");
	ComPareChange(m_pRealDetect->m_tParameters.dMaxAoBanDeviation,m_pTempDetect->m_tParameters.dMaxAoBanDeviation,"凹板最大偏差");
	ComPareChange(m_pRealDetect->m_tParameters.bEnaleUseSegAoBan,m_pTempDetect->m_tParameters.bEnaleUseSegAoBan,"启用凹版分割");
	ComPareChange(m_pRealDetect->m_tParameters.dExtractFoilRegionWidth,m_pTempDetect->m_tParameters.dExtractFoilRegionWidth,"提取极耳宽度限制");
	ComPareChange(m_pRealDetect->m_tParameters.nExtractFoilThresh,m_pTempDetect->m_tParameters.nExtractFoilThresh,"提取极耳阈值");
	ComPareChange(m_pRealDetect->m_tParameters.dExtractAobanRegionWidth,m_pTempDetect->m_tParameters.dExtractAobanRegionWidth,"凹版搜索宽度");
	ComPareChange(m_pRealDetect->m_tParameters.nExtractAobanThresh,m_pTempDetect->m_tParameters.nExtractAobanThresh,"凹版对比度");
	ComPareChange(m_pRealDetect->m_tParameters.nSelectValueMode,m_pTempDetect->m_tParameters.nSelectValueMode,"取点方式");
	ComPareChange(m_pRealDetect->m_tParameters.bEnableVariableWidth,m_pTempDetect->m_tParameters.bEnableVariableWidth,"启用变宽检测");
	ComPareChange(m_pRealDetect->m_tParameters.dRelateOffset,m_pTempDetect->m_tParameters.dRelateOffset,"启用变宽相对偏移检测");
	
	
}


void CDlgParamSet::OnBnClickedButtonTest()
{
	try
	{
		if(TRUE == UpdateData(TRUE))
		{
			HTuple hvTimestart,hvTimeend;
			CountSeconds(&hvTimestart);
			SEPARATEOUTPUT Output;
			m_pTempDetect->Detect(*m_pDetectInput,&Output);
			CountSeconds(&hvTimeend);
			SetColored(m_hvWindowID,12);
			DispObj(m_hoPretreatImage,m_hvWindowID);
			DispObj(Output.hoDispRegion,m_hvWindowID);
			SetColor(m_hvWindowID,"red");
			DispObj(Output.hoCoat,m_hvWindowID);
			SetColor(m_hvWindowID,"blue");
			DispObj(Output.hoFoil,m_hvWindowID);

			SetColor(m_hvWindowID,"yellow");
			DispObj(Output.hoTDispRegion,m_hvWindowID);

			SetColor(m_hvWindowID, "medium blue");
			DispObj(m_pTempDetect->m_tAoBanInfo.hoFoilDetectRegion,m_hvWindowID);
			SetColor(m_hvWindowID, "green");
			DispObj(m_pTempDetect->m_tAoBanInfo.hoExtractFoilRegion,m_hvWindowID);

			SetColor(m_hvWindowID, "medium slate blue");
			DispObj(m_pTempDetect->m_tAoBanInfo.hoLineBetAoBanAndFoil,m_hvWindowID);


			SetColor(m_hvWindowID, "white");
			DispObj(m_pTempDetect->m_hoThreshAT9Region, m_hvWindowID);

			SetColor(m_hvWindowID, "white");
			DispObj(m_pTempDetect->m_hoReferRegion, m_hvWindowID);

			



			SetColor(m_hvWindowID, "pink");
			/*DispObj(m_pTempDetect->m_tAoBanInfo.hoAoBanRegion,m_hvWindowID);*/
			HTuple hv_CenerRow,hv_CenterColumn;
			CString strLocateValue;
			for (int k=0;k<m_pTempDetect->m_tAoBanInfo.hoAoBanRegion.CountObj();k++)
			{
				AreaCenter(m_pTempDetect->m_tAoBanInfo.hoAoBanRegion.SelectObj(k+1),NULL,&hv_CenerRow,&hv_CenterColumn);
				DispObj(m_pTempDetect->m_tAoBanInfo.hoAoBanRegion.SelectObj(k+1),m_hvWindowID);
				strLocateValue.Format("料高%d:%.2f",k+1,m_pTempDetect->m_tAoBanInfo.hvLocateDistance[k].D());
				if (m_pTempDetect->m_tInit.nGrabberSN==0 ||m_pTempDetect->m_tInit.nGrabberSN==2)
				{
					SetTposition(m_hvWindowID,(k+1)*(Output.hvCoatHeight.D()/m_pTempDetect->m_dRowResolution/m_pTempDetect->m_tAoBanInfo.hoAoBanRegion.CountObj()),hv_CenterColumn+100);

				}else
				{
					SetTposition(m_hvWindowID,(k+1)*(Output.hvCoatHeight.D()/m_pTempDetect->m_dRowResolution/m_pTempDetect->m_tAoBanInfo.hoAoBanRegion.CountObj()),MAX(0,hv_CenterColumn-500));
				}
				WriteString(m_hvWindowID,(HTuple)strLocateValue);

			}



			m_hoDispRegion = Output.hoDispRegion.ConcatObj(Output.hoCoat.ConcatObj(Output.hoFoil));
			m_LC_CheckInfo.DeleteAllItems();
			CString strAttribute,strVal,strValTemp;
			strAttribute.Format("属性");
			int nItemCount = 0;

			CString strTemp;
			for (int i=0;i<Output.hvCoatWidth.Length();i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"涂布宽");
				strTemp.Format("%.2fmm  ",Output.hvCoatWidth[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
			}

			for (int i=0;i<Output.hvCoatHeight.Length();i++)
			{

				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"涂布高");
				strTemp.Format("%.2fmm  ",Output.hvCoatHeight[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
			}				
			for (int i=0;i<Output.hvFoilWidth.Length();i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"极耳宽");
				strTemp.Format("%.2fmm  ",Output.hvFoilWidth[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
			}
			for (int i=0;i<Output.hvFoilHeight.Length();i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"极耳高");
				strTemp.Format("%.2fmm  ",Output.hvFoilHeight[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
			}


			for (int i=0;i<Output.hvLocateMiss.Length();i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"定位不齐0");
				strTemp.Format("%.2fmm  ",Output.hvLocateMiss[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
			}

			SetColor(m_hvWindowID, "yellow");
			DispObj(m_pTempDetect->m_hoEdageCrossRegion, m_hvWindowID);

			for (int i = 0; i<Output.hv_BaiBian.Length(); i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();

				if (m_pTempDetect->m_tParameters.bEnableVariableWidth)
				{

					m_LC_CheckInfo.InsertItem(nItemCount, "加宽膜宽");
				}else
				{
					m_LC_CheckInfo.InsertItem(nItemCount, "膜宽");
				}
				strTemp.Format("%.2fmm  ", Output.hv_BaiBian[i].D());
				m_LC_CheckInfo.SetItemText(nItemCount, 1, strTemp);
			}
			for (int i=0;i<m_pTempDetect->m_hvErrorLineIndex.Length();i++)
			{
				nItemCount = m_LC_CheckInfo.GetItemCount();
				m_LC_CheckInfo.InsertItem(nItemCount,"拟合线出错");
				strTemp.Format("线序号%d  ", m_pTempDetect->m_hvErrorLineIndex[i].I());
				m_LC_CheckInfo.SetItemText(nItemCount,1,strTemp);
				m_LC_CheckInfo.SetItemBkColor(nItemCount, 1, RGB(255, 0, 0));
			}

			nItemCount = m_LC_CheckInfo.GetItemCount();
			m_LC_CheckInfo.InsertItem(nItemCount,"涂布灰度均值");
			strVal.Format("%.1f",m_pTempDetect->m_hvMean.D());
			m_LC_CheckInfo.SetItemText(nItemCount,1,strVal);

			nItemCount = m_LC_CheckInfo.GetItemCount();
			m_LC_CheckInfo.InsertItem(nItemCount,"检测时间");
			strVal.Format("%.2f ms",HTuple(hvTimeend-hvTimestart).D()*1000);
			m_LC_CheckInfo.SetItemText(nItemCount,1,strVal);

		}
	}catch(HException e)
	{

	}

	

}
void CDlgParamSet::OnBnClickedButtonSave()
{
	if(TRUE == UpdateData(TRUE))
	{

		SaveOperaterLog();
		*m_pRealDetect = *m_pTempDetect;
		m_pRealDetect->SaveParam();
		//m_pRealDetect->SaveLane();
		m_pRealDetect->SaveLineMatch();
		AfxMessageBox("保存完毕");
	}
}
void CDlgParamSet::OnBnClickedButtonExit()
{
	CDialogEx::OnCancel();

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
		}
	}
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		short zDelta = HIWORD(pMsg->wParam);
		UINT nFlags  = LOWORD(pMsg->wParam);
		OnMouseWheel(nFlags,zDelta,pMsg->pt);
		return TRUE;
	}
	if (pMsg->message == WM_MBUTTONDOWN)
	{
		CPoint   pt;
		GetCursorPos(&pt);
		GetDlgItem(IDC_STC_IMAGE)->ScreenToClient(&pt);
		CRect WndRect;
		GetDlgItem(IDC_STC_IMAGE)->GetClientRect(&WndRect);

		// 还原显示（不缩放）
		if (WndRect.PtInRect(pt))
		{	
			HTuple tmpWidth,tmpHeight;
			GetImageSize(m_hoPretreatImage, &tmpWidth, &tmpHeight);
			m_dDispImagePartRow0 = 0;
			m_dDispImagePartCol0 = 0;
			m_dDispImagePartRow1 = tmpHeight - 1;
			m_dDispImagePartCol1 = tmpWidth - 1;	
			SetPart(m_hvWindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
			ClearWindow(m_hvWindowID);
			FreshImage();
		}
	}
	if (pMsg->message == WM_MOUSEMOVE)
	{
		try
		{
			HTuple Row,Column,Value,Area,Circularity;
			GetMposition(m_hvWindowID,&Row,&Column,NULL);
			GetGrayval(m_hoPretreatImage,Row,Column,&Value);
			CString strDisp;
			strDisp.Format("坐标:  X:%3d Y:%3d\r\n \r\n灰度值: %3d",Column[0].I(),Row[0].I(),Value[0].I());
			m_STC_GrayValue.SetWindowTextA(strDisp);
			return TRUE;
		}catch(HException e)
		{
			CString strDisp;
			strDisp.Format("坐标:  X:-- Y:--\r\n \r\n灰度值: --");
			m_STC_GrayValue.SetWindowTextA(strDisp);
		}

	}
	if (pMsg->message == WM_RBUTTONUP)
	{
		
			CPoint   pt;
			GetCursorPos(&pt);
			GetDlgItem(IDC_STC_IMAGE)->ScreenToClient(&pt);
			CRect WndRect;
			GetDlgItem(IDC_STC_IMAGE)->GetClientRect(&WndRect);

			// 还原显示（不缩放）
			if (WndRect.PtInRect(pt))
			{	
				GetDlgItem(IDC_STC_IMAGE)->ClientToScreen(&pt);
				CMenu* m_tempmenu = m_pMenuTools->GetSubMenu(0);
				m_tempmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
			}
		return TRUE;
	}
	/*if (pMsg->wParam == VK_F1)
	{
	CDlgHelp dlg;
	dlg.DoModal();
	return TRUE;
	}*/
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgParamSet::OnBnClickedBntOpenimage()
{

	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
	if(IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_pDetectInput->hoGrabImage,(HTuple)strFilename);
			if (m_pTempDetect->m_tParameters.bPretreat)
			{
				m_hoPretreatImage=m_pDetectInput->hoGrabImage;
				HTuple hv_Mat2D;
				m_pTempDetect->GetHommat2D(m_hoPretreatImage, hv_Mat2D);
				m_pTempDetect->GetPretreatImage(m_hoPretreatImage, hv_Mat2D);
			}else
			{
				m_hoPretreatImage = m_pDetectInput->hoGrabImage;
			}
	         

			HTuple Width,Height;
			GetImageSize(m_hoPretreatImage,&Width,&Height);
			SetPart(m_hvWindowID,0,0,Height-1,Width-1);
			DispObj(m_hoPretreatImage,m_hvWindowID);
			m_dDispImagePartRow0 = 0;
			m_dDispImagePartCol0 = 0;
			m_dDispImagePartRow1 = Height - 1;
			m_dDispImagePartCol1 = Width - 1;

		}catch(HException )
		{
			AfxMessageBox("读取图片失败");
		}

	}
}


void CDlgParamSet::OnBnClickedBntSaveimage()
{
	try
	{
		SYSTEMTIME CurrentSystime;
		GetLocalTime(&CurrentSystime);
		//保存图片
		CString strPath;
		CString strImageName; 
		strImageName.Format("%d时%d分%d秒.bmp",CurrentSystime.wHour,CurrentSystime.wMinute,CurrentSystime.wSecond);
		CreateDirectory("D:\\SaveImage",NULL);
		strPath.Format("D:\\SaveImage\\%d月%d日\\",CurrentSystime.wMonth,CurrentSystime.wDay);
		CreateDirectory(strPath,NULL);
		WriteImage(m_pDetectInput->hoGrabImage,"bmp",NULL,HTuple(strPath+strImageName));

		CString strMessageBox;
		strMessageBox.Format("图片保存完毕\r\n%s%s\r\n",strPath,strImageName);
		AfxMessageBox(strMessageBox);
	}catch(HException e)
	{
		AfxMessageBox("保存图片失败");
	}
}


BOOL CDlgParamSet::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		CPoint   pt;
		GetCursorPos(&pt);
		GetDlgItem(IDC_STC_IMAGE)->ScreenToClient(&pt);
		CRect WndRect;
		GetDlgItem(IDC_STC_IMAGE)->GetClientRect(&WndRect);
		if (WndRect.PtInRect(pt))
		{			
			double	dImagePtX,dImagePtY;
			double	dRow0_1,dCol0_1,dRow1_1,dCol1_1;
			double	dScale=0.1;

			if (zDelta<0)
			{
				dImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
				dImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
				dRow0_1=dImagePtY-1/(1-dScale)*(dImagePtY-m_dDispImagePartRow0);
				dRow1_1=dImagePtY-1/(1-dScale)*(dImagePtY-m_dDispImagePartRow1);
				dCol0_1=dImagePtX-1/(1-dScale)*(dImagePtX-m_dDispImagePartCol0);
				dCol1_1=dImagePtX-1/(1-dScale)*(dImagePtX-m_dDispImagePartCol1);
				m_dDispImagePartRow0=dRow0_1;
				m_dDispImagePartCol0=dCol0_1;
				m_dDispImagePartRow1=dRow1_1;
				m_dDispImagePartCol1=dCol1_1;
			}
			else
			{
				dImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
				dImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
				dRow0_1=dImagePtY-1/(1+dScale)*(dImagePtY-m_dDispImagePartRow0);
				dRow1_1=dImagePtY-1/(1+dScale)*(dImagePtY-m_dDispImagePartRow1);
				dCol0_1=dImagePtX-1/(1+dScale)*(dImagePtX-m_dDispImagePartCol0);
				dCol1_1=dImagePtX-1/(1+dScale)*(dImagePtX-m_dDispImagePartCol1);
				m_dDispImagePartRow0=dRow0_1;
				m_dDispImagePartCol0=dCol0_1;
				m_dDispImagePartRow1=dRow1_1;
				m_dDispImagePartCol1=dCol1_1;
			}
			SetPart(m_hvWindowID, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
			SetSystem("flush_graphic","false");
			ClearWindow(m_hvWindowID);
			SetSystem("flush_graphic","true");
			FreshImage();
		}
	}catch(HException )
	{

	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
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
		case IDC_STC_IMAGE:
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		case IDC_STC_EXAMPLE:
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;


		}
		pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);  
			
	}
}
void CDlgParamSet::OnBnClickedButtonDrawLine()
{
	
	SetButtonStaus(FALSE);

	HTuple	tRow0,tCol0,tRow1,tCol1,Phi,Length1,Length2;
	HTuple hv_Height,hv_Width;
	if (FALSE == m_hoPretreatImage.IsInitialized())
	{
		SetButtonStaus(TRUE);
		return;
	}
	GetImageSize(m_hoPretreatImage,&hv_Width,&hv_Height);
	m_hoDispRegion.GenEmptyObj();
	DrawLine(m_hvWindowID,&tRow0,&tCol0,&tRow1,&tCol1);



	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartRow=tRow0[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartCol=tCol0[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndRow=tRow1[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndCol=tCol1[0].D();


	double dStartY,dStartX,dEndY,dEndX;
	m_pTempDetect->FitLine(m_hoPretreatImage,m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),
		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,m_CB_nLineSelect,1);
	GenLineArrow(m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),80);
	FreshImage();
	SetButtonStaus(TRUE);
}
void CDlgParamSet::FreshNewParam()
{
	UpdateData(TRUE);
	int n = m_CB_nLineSelect;
	m_pTempDetect->m_sLineTool[n].m_nMeasureNum = m_EDT_nMeasureLinesNum;
	m_pTempDetect->m_sLineTool[n].m_dSigma      = m_EDT_dMeasureSigma;
	m_pTempDetect->m_sLineTool[n].m_dHeight     = m_EDT_dMeasureLength;
	m_pTempDetect->m_sLineTool[n].m_nThr        = m_EDT_nMeasureThres;
	m_pTempDetect->m_sLineTool[n].m_dWidth      = m_EDT_dMeasureWidth;
	m_pTempDetect->m_sLineTool[n].m_nSelect     = m_CB_nSelect;
	m_pTempDetect->m_sLineTool[n].m_nDirect     = m_CB_nDirect;
	m_pTempDetect->m_sLineTool[n].m_nChoice       = m_CB_nChoice;
	m_pTempDetect->m_sLineTool[n].m_nSortType     = m_COM_nSortType;

	m_pTempDetect->m_tParameters.bEnaleUseSegAoBan=m_CHK_bUseAoBanSegMode;
	m_pTempDetect->m_tParameters.bFindManualOrAuto=m_COM_bFindAobanManualOrAuto;
	m_pTempDetect->m_tParameters.dExtractFoilRegionWidth=m_EDT_dFoilDetectRegionWidth;
	m_pTempDetect->m_tParameters.nExtractFoilThresh=m_EDT_nFoilDetectThresh;
	m_pTempDetect->m_tParameters.dExtractAobanRegionWidth=m_EDT_dAoBanDetectRegionWidth;
	m_pTempDetect->m_tParameters.nExtractAobanThresh=m_EDT_nAoBanThresh;

    m_pTempDetect->m_tParameters.bEnableVariableWidth=m_CHK_bEnableVariableWidth;
	m_pTempDetect->m_tParameters.dRelateOffset=m_EDT_dRelateOffset;
	double dStartY,dStartX,dEndY,dEndX;
	m_pTempDetect->FitLine(m_hoPretreatImage,m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),
		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,m_CB_nLineSelect,1);

	FreshImage();
}
void CDlgParamSet::FreshImage()
{
	SetColored(m_hvWindowID,12);
	DispObj(m_hoPretreatImage,m_hvWindowID);

	if (m_hoDispRegion.IsInitialized())
	{
		DispObj(m_hoDispRegion,m_hvWindowID);
	}

}
void CDlgParamSet::OnBnClickedButtonLineTest()
{
	UpdateData(TRUE);
	m_hoDispRegion.GenEmptyObj();
	for (int i=0;i<m_pTempDetect->m_tLineMatch.nLineUsed;i++)
	{
		HObject hoDispRegion;
		double dStartY,dStartX,dEndY,dEndX;
		m_pTempDetect->FitLine(m_hoPretreatImage,m_pTempDetect->m_sLineTool[i].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),
			&hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,m_CB_nLineSelect);
		ConcatObj(m_hoDispRegion,hoDispRegion,&m_hoDispRegion);
		GenLineArrow(m_pTempDetect->m_sLineTool[i].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),80);
	}
	FreshImage();
}


void CDlgParamSet::OnCbnSelchangeComboLineSelect()
{
	UpdateData(TRUE);
	int n = m_CB_nLineSelect;
	m_EDT_nMeasureLinesNum = m_pTempDetect->m_sLineTool[n].m_nMeasureNum;
	m_EDT_dMeasureSigma    = m_pTempDetect->m_sLineTool[n].m_dSigma;
	m_EDT_dMeasureLength   = m_pTempDetect->m_sLineTool[n].m_dHeight;
	m_EDT_nMeasureThres    = m_pTempDetect->m_sLineTool[n].m_nThr;
	m_EDT_dMeasureWidth    = m_pTempDetect->m_sLineTool[n].m_dWidth;
	m_CB_nSelect           = m_pTempDetect->m_sLineTool[n].m_nSelect;
	m_CB_nDirect           = m_pTempDetect->m_sLineTool[n].m_nDirect;
	m_CB_nChoice           = m_pTempDetect->m_sLineTool[n].m_nChoice;
	m_COM_nSortType = m_pTempDetect->m_sLineTool[n].m_nSortType;
	double dStartY,dStartX,dEndY,dEndX;
	m_pTempDetect->FitLine(m_hoPretreatImage,m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),
		&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,m_CB_nLineSelect,1);
	GenLineArrow(m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),80);
	FreshImage();
	UpdateData(FALSE);
}


void CDlgParamSet::OnEnChangeEditMeasurelinesnum()
{
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEditMeasurelength()
{
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEditMeasurewidth()
{
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEditMeasuresigma()
{
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEditMeasurethres()
{
	FreshNewParam();
}


void CDlgParamSet::OnCbnSelchangeComboMeasuretransition()
{
	FreshNewParam();
}


void CDlgParamSet::OnCbnSelchangeComboMeasureselect()
{
	FreshNewParam();
}
void CDlgParamSet::SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
	COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG)
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
void CDlgParamSet::InitCom()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_LINE_SELECT))->ResetContent();
	CString strInsert;
	for (int i=0;i<m_pTempDetect->m_tLineMatch.nLineUsed;i++)
	{
	   strInsert.Format("%d",i);
	   ((CComboBox*)GetDlgItem(IDC_COMBO_LINE_SELECT))->AddString(strInsert);
	}
}


void CDlgParamSet::GenLineArrow(const CLineToolPara& Line, const HTuple& Size)
{
	try
	{
		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2;
		HTuple  hv_Rad, hv_RowCenter, hv_ColCenter, hv_Length, hv_Phi1;
		HTuple  hv_DiffRow,hv_DiffColumn, hv_DiffRowabs,hv_DiffColumnabs, hv_RowBegain, hv_ColumnBegain;
		HTuple  hv_RowEnd, hv_ColumnEnd;
		HTuple hv_Width,hv_Height;
		GetImageSize(m_hoPretreatImage,&hv_Width,&hv_Height);

		hv_RowBegain=Line.m_dStartRow;
		hv_ColumnBegain=Line.m_dStartCol; 
		hv_RowEnd=Line.m_dEndRow;
		hv_ColumnEnd=Line.m_dEndCol;
		

		hv_DiffRow=hv_RowEnd-hv_RowBegain;
		hv_DiffColumn=hv_ColumnEnd-hv_ColumnBegain;
		TupleAbs(hv_DiffRow,&hv_DiffRowabs);
		TupleAbs(hv_DiffColumn,&hv_DiffColumnabs);
		LinePosition(hv_RowBegain, hv_ColumnBegain, hv_RowEnd, hv_ColumnEnd, &hv_RowCenter, &hv_ColCenter, 
			NULL,NULL);
		HObject hoArrowXLD;
		if (hv_DiffColumnabs>=hv_DiffRowabs)
		{
			if (hv_DiffColumn>0)
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter+300, hv_ColCenter, hv_RowCenter-300, hv_ColCenter, Size, Size);
			}
			else
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter-300, hv_ColCenter, hv_RowCenter+300, hv_ColCenter, Size, Size);
			}

		}
		else
		{
			if (hv_DiffRow>0)
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter, hv_ColCenter-300, hv_RowCenter, hv_ColCenter+300, Size, Size);
			}
			else
			{
				gen_arrow_contour_xld(&hoArrowXLD, hv_RowCenter, hv_ColCenter+300, hv_RowCenter, hv_ColCenter-300, Size, Size);
			}
		}
		SetColor(m_hvWindowID,"red");

		if (m_hoDispRegion.IsInitialized())
		{
			ConcatObj(m_hoDispRegion,hoArrowXLD,&m_hoDispRegion);
		}else
		{
			GenEmptyObj(&m_hoDispRegion);
			ConcatObj(m_hoDispRegion,hoArrowXLD,&m_hoDispRegion);
		}
	}catch(HException e)
	{
		CString strMessageBox;
		strMessageBox.Format("生成方向时候出现异常!\r\n异常信息：%s",e.ErrorMessage().Text());
		AfxMessageBox(strMessageBox);
	}

}
void CDlgParamSet::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_ftGrayValue.CreateFontA(20,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"宋体");
	m_STC_GrayValue.SetFont(&m_ftGrayValue);
	m_STC_GrayValue.SetBkColor(RGB(255,255,255),0,CStaticExt::Normal);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	SetButtonColcor(m_BTN_ParamSet,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_OpenImage,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_SaveImage,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_Test,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_Save,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_Exit,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_DrawLine,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_LineTest,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_Cancel,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_ModLine,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_CalibSet,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);

	m_BTN_ParamSet.SetFont(&m_ftButton);
    m_BTN_OpenImage.SetFont(&m_ftButton);
	m_BTN_SaveImage.SetFont(&m_ftButton);
	m_BTN_Test.SetFont(&m_ftButton);
	m_BTN_Save.SetFont(&m_ftButton);
	m_BTN_Exit.SetFont(&m_ftButton);
	m_BTN_DrawLine.SetFont(&m_ftButton);
	m_BTN_LineTest.SetFont(&m_ftButton);
	m_BTN_Cancel.SetFont(&m_ftButton);
	m_BTN_ModLine.SetFont(&m_ftButton);
	m_BTN_Exit.SetIcon(IDI_ICON_W_UNCHECK);


	m_pMenuTools = new CMenuExt;
	m_pMenuTools->SetAttributes(95,"Microsoft YaHei",35,40,
		g_Blendent.m_Blendent.Menu.BKColorNormal,
		g_Blendent.m_Blendent.Menu.BKColorFocus,
		g_Blendent.m_Blendent.Menu.BDColorFocus,
		g_Blendent.m_Blendent.Menu.TextColorNormal,
		g_Blendent.m_Blendent.Menu.TextColorFocus,
		g_Blendent.m_Blendent.Menu.IconBKColor,
		g_Blendent.m_Blendent.Menu.SPColor);
	m_pMenuTools->LoadMenu(IDR_MENU_RBUTTONUP);
	m_pMenuTools->ChangeMenuItem(m_pMenuTools);

	if (m_pTempDetect->m_tInit.nGrabberSN==1 ||m_pTempDetect->m_tInit.nGrabberSN==3 )
	{
		GetDlgItem(IDC_BTN_CALIB)->ShowWindow(SW_HIDE);
	}

}

//void CDlgParamSet::OnBnClickedBntCalib()
//{
//	CDlgCalibrate dlg;
//	_TRY
//	dlg.m_hvRow.Append(m_pTempDetect->m_hvInterRow[4]);
//	dlg.m_hvRow.Append(m_pTempDetect->m_hvInterRow[2]);
//	dlg.m_hvRow.Append(m_pTempDetect->m_hvInterRow[4]);
//	dlg.m_hvRow.Append(m_pTempDetect->m_hvInterRow[5]);
//	dlg.m_hvColumn.Append(m_pTempDetect->m_hvInterCol[4]);
//	dlg.m_hvColumn.Append(m_pTempDetect->m_hvInterCol[2]);
//	dlg.m_hvColumn.Append(m_pTempDetect->m_hvInterCol[4]);
//	dlg.m_hvColumn.Append(m_pTempDetect->m_hvInterCol[5]);
//	dlg.m_pOwner = m_pTempDetect;
//	_CATCH_HALCON_RETURN("没有找到足够多的点")
//	dlg.DoModal();
//	
//}


void CDlgParamSet::OnBnClickedBntParamset()
{
	CDlgSet dlg;
	dlg.m_pOwner = m_pTempDetect;
	if (IDOK == dlg.DoModal())
	{
		m_hoPretreatImage = m_pDetectInput->hoGrabImage;
		if (m_pTempDetect->m_tParameters.bPretreat)
		{
			HTuple hv_Mat2D;
			m_pTempDetect->GetHommat2D(m_hoPretreatImage, hv_Mat2D);
		    m_pTempDetect->GetPretreatImage(m_hoPretreatImage, hv_Mat2D);
		}
	}
}


void CDlgParamSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg,rcTitle,rcCtl;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	//GetClientRect(rcDlg);  
	dc.FillSolidRect(rcDlg,RGB(255,255,255));

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);


	GetDlgItem(IDC_LC_CHECKINFO)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_STC_IMAGE)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
	



}


HBRUSH CDlgParamSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL CDlgParamSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


LRESULT CDlgParamSet::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
	if (rcTitle.PtInRect(point))
	{
		return HTCAPTION;
	}
	return CDialogEx::OnNcHitTest(point);
}



void CDlgParamSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

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


void CDlgParamSet::OnCbnSelchangeComboLineChoice()
{
	FreshNewParam();
}


void CDlgParamSet::OnBnClickedBntCancel()
{
	CDialogEx::OnCancel();
}


//void CDlgParamSet::OnBnClickedButtonLinematch()
//{
//	CDlgLineMatch dlg;
//	dlg.m_pOwner  = m_pTempDetect;
//	dlg.DoModal();
//}


void CDlgParamSet::OnBnClickedButtonModLine()
{

	SetButtonStaus(FALSE);
	HTuple	tRow0,tCol0,tRow1,tCol1,Phi,Length1,Length2;
	HTuple hv_Width,hv_Height;
	if (FALSE == m_hoPretreatImage.IsInitialized())
	{
       SetButtonStaus(TRUE);
		return;
	}
	GetImageSize(m_hoPretreatImage,&hv_Width,&hv_Height);

	m_hoDispRegion.GenEmptyObj();


	DrawLineMod(m_hvWindowID,m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartRow,
		m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartCol,
		m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndRow,
		m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndCol,
		&tRow0,&tCol0,&tRow1,&tCol1);
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartRow=tRow0[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dStartCol=tCol0[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndRow=tRow1[0].D();
	m_pTempDetect->m_sLineTool[m_CB_nLineSelect].m_dEndCol=tCol1[0].D();



	double dStartY,dStartX,dEndY,dEndX;
	try
	{
		m_pTempDetect->FitLine(m_hoPretreatImage,m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),
			&m_hoDispRegion,&dStartY,&dStartX,&dEndY,&dEndX,m_CB_nLineSelect,1);

	}catch(HException& e)
	{
		CString strExceptMessage;
		strExceptMessage.Format("%s:%s",e.ProcName().Text(),e.ErrorMessage().Text());
		AfxMessageBox(strExceptMessage);

	}
	
	GenLineArrow(m_pTempDetect->m_sLineTool[m_CB_nLineSelect].AffineTrans(m_pTempDetect->m_tParameters.hv_Mat2D,m_CB_nLineSelect),80);
	FreshImage();
	SetButtonStaus(TRUE);
}

void CDlgParamSet::OnMenuDrawcoat()
{
	HTuple Row,Column,Phi,Length1,Length2;
	SetButtonStaus(FALSE);
	HTuple LTRow,LTColumn,RBRow,RBColumn;
	try
	{
		DrawRectangle1(m_hvWindowID,&LTRow,&LTColumn,&RBRow,&RBColumn);
	}catch(HException & e)
	{
		SetButtonStaus(FALSE);
		AfxMessageBox("绘制有误");
	}
	
	
	if (m_pTempDetect->m_tInit.nProductType==negative)
	{
		if (m_pTempDetect->m_tInit.nGrabberSN==0 ||m_pTempDetect->m_tInit.nGrabberSN==2)//左侧
		{

			m_pTempDetect->m_sLineTool[4].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[4].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[4].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[4].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[4].m_nDirect=2;

			m_pTempDetect->m_sLineTool[5].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[5].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[5].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[5].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[5].m_nDirect=2;


			m_pTempDetect->m_sLineTool[6].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[6].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[6].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[6].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[6].m_nDirect=2;
		}
		else
		{

			m_pTempDetect->m_sLineTool[6].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[6].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[6].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[6].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[6].m_nDirect=2;

			m_pTempDetect->m_sLineTool[5].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[5].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[5].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[5].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[5].m_nDirect=2;

			m_pTempDetect->m_sLineTool[4].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[4].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[4].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[4].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[4].m_nDirect=2;

		}
	}else
	{
		if (m_pTempDetect->m_tInit.nGrabberSN==0 ||m_pTempDetect->m_tInit.nGrabberSN==2)//左侧
		{


			m_pTempDetect->m_sLineTool[3].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[3].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[3].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[3].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[3].m_nDirect=2;

			m_pTempDetect->m_sLineTool[4].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[4].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[4].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[4].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[4].m_nDirect=1;

			m_pTempDetect->m_sLineTool[5].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[5].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[5].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[5].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[5].m_nDirect=1;


			m_pTempDetect->m_sLineTool[6].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[6].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[6].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[6].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[6].m_nDirect=2;
		}
		else
		{

			m_pTempDetect->m_sLineTool[6].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[6].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[6].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[6].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[6].m_nDirect=2;

			m_pTempDetect->m_sLineTool[5].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[5].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[5].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[5].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[5].m_nDirect=1;

			m_pTempDetect->m_sLineTool[4].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[4].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[4].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[4].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[4].m_nDirect=1;
			
			m_pTempDetect->m_sLineTool[3].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[3].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[3].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[3].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[3].m_nDirect=2;



		}
	}
	SetButtonStaus(TRUE);
	OnBnClickedButtonLineTest();
}

void CDlgParamSet::OnMenuDrawfoil()
{
	HTuple Row,Column,Phi,Length1,Length2;
	SetButtonStaus(FALSE);
	HTuple LTRow,LTColumn,RBRow,RBColumn;
	try
	{
		DrawRectangle1(m_hvWindowID,&LTRow,&LTColumn,&RBRow,&RBColumn);
	}catch(HException & e)
	{
		SetButtonStaus(FALSE);
		AfxMessageBox("绘制有误");
	}

	/* 从左到右 7->3->0->2代表极耳  6->5->1->4代表涂布*/


	if (m_pTempDetect->m_tInit.nProductType==positive)
	{
		if (m_pTempDetect->m_tInit.nGrabberSN==0 ||m_pTempDetect->m_tInit.nGrabberSN==2)//左侧
		{

			int nLineSelect;
			nLineSelect=0;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
			nLineSelect=1;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=2;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
		}
		else
		{

			int nLineSelect;
			nLineSelect=0;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
			nLineSelect=1;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=2;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
		}
	}else
	{
		if (m_pTempDetect->m_tInit.nGrabberSN==0 ||m_pTempDetect->m_tInit.nGrabberSN==2)//左侧
		{

			int nLineSelect;
			nLineSelect=0;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
			nLineSelect=1;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=2;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=3;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
		}
		else
		{

			int nLineSelect;
			nLineSelect=0;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;
			nLineSelect=1;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=2;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   = RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = RBColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

			nLineSelect=3;
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartRow =LTRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dStartCol = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndRow   =RBRow[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_dEndCol   = LTColumn[0];
			m_pTempDetect->m_sLineTool[nLineSelect].m_nDirect=1;

		}
	}
	SetButtonStaus(TRUE);
	//OnBnClickedButtonLineTest();
}


void CDlgParamSet::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == 0) 
	{
		m_pMenuTools->MeasureItem(lpMeasureItemStruct);
	}
	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void CDlgParamSet::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == 0) 
	{
		m_pMenuTools->DrawItem(lpDrawItemStruct);
	}
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CDlgParamSet::OnBnClickedBtnCalib()
{
	// TODO: 在此添加控件通知处理程序代码

	AFX_MANAGE_STATE(AfxGetStaticModuleState());//资源切换
	CDlgCalibrate dlg;
	dlg.m_DlgParamSet=this;
	dlg.DoModal();

}


//void CDlgParamSet::OnCbnSelchangeCombPosition()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	m_pTempDetect->m_tParameters.bFoilLocation=m_CB_nPosition;
//	OnBnClickedButtonTest();
//}







void CDlgParamSet::OnSelchangeComSort()
{
	// TODO: 在此添加控件通知处理程序代码
	FreshNewParam();
}


//void CDlgParamSet::OnBnClickedBtnStartcalibresolution()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	if (m_EDT_dCurrentWidth!=0 && m_EDT_dRealWidth!=0)
//	{
//
//		m_EDT_dColumnResolution=m_EDT_dRealWidth/m_EDT_dCurrentWidth*m_pTempDetect->m_dColResolution;
//
//	}else
//	{
//		AfxMessageBox("实测宽度值和当前宽度值均不可为0，请重新输入");
//	}
//	UpdateData(FALSE);
//
//
//}


//void CDlgParamSet::OnBnClickedBtnSaveresolution()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	CString strSection;
//	if (m_pTempDetect->m_tInit.nGrabberSN==0  || m_pTempDetect->m_tInit.nGrabberSN==1)
//	{
//		strSection.Format("GRABBER%d",0);
//		WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dColumnResolution,m_pTempDetect->m_strCommondh);
//		strSection.Format("GRABBER%d",1);
//		WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dColumnResolution,m_pTempDetect->m_strCommondh);
//	}else
//	{
//		strSection.Format("GRABBER%d",2);
//		WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dColumnResolution,m_pTempDetect->m_strCommondh);
//		strSection.Format("GRABBER%d",3);
//		WritePrivateProfileDouble(strSection,"COLUMNRESOLUTION",m_EDT_dColumnResolution,m_pTempDetect->m_strCommondh);
//	}
//
//	AfxMessageBox("保存成功");
//
//
//
//
//}


//void CDlgParamSet::OnBnClickedChkUsemodelmatch()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	FreshNewParam();
//}


//void CDlgParamSet::OnEnChangeEdtMinscore()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//	FreshNewParam();
//	// TODO:  在此添加控件通知处理程序代码
//}


//void CDlgParamSet::OnEnChangeEdtMaxmatchnumber()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//	FreshNewParam();
//	// TODO:  在此添加控件通知处理程序代码
//}


//void CDlgParamSet::OnBnClickedBtnDrawmodel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	try
//	{
//
//		if (m_CHK_bUseModel)
//		{
//			HObject hoModelRectangle;
//			HObject hoImageReduced;
//			HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
//			HTuple hv_ModelID;
//			HObject hoRegion;
//			SetButtonStaus(FALSE);
//			SetTposition(m_hvWindowID,12,12);
//			WriteString(m_hvWindowID,"按住左键绘制模板，点击右键绘制完成");
//			DrawRectangle1(m_hvWindowID,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//			GenRectangle1(&hoModelRectangle,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//			ReduceDomain(m_hoPretreatImage,hoModelRectangle,&hoImageReduced);
//			CreateNccModel(hoImageReduced, "auto",HTuple(-10).TupleRad(), HTuple(10).TupleRad(),"auto", "use_polarity",  &hv_ModelID);
//			m_pTempDetect->m_tParameters.hoModelRegion=hoModelRectangle;
//			m_pTempDetect->m_tParameters.hv_ModelID=hv_ModelID;
//			SetButtonStaus(TRUE);
//			AfxMessageBox("绘制成功！");
//		}
//	}catch(HException& e)
//	{
//		SetButtonStaus(TRUE);
//	}
//}


//void CDlgParamSet::OnBnClickedBtnMatchtest()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (m_pTempDetect->m_tParameters.bUseModel&&m_pTempDetect->m_tParameters.hoModelRegion.CountObj()>0)
//	{
//		HTuple hv_ImageHeight;
//		HObject hoSearchRegion,hoImageReduced;
//		HTuple  hv_Column1,hv_Column2,hv_HomMat2D;
//		HTuple hv_Row0, hv_Column0, hv_Angle0, hv_Score0;
//		HObject hoCross;
//		GetImageSize(m_hoPretreatImage,NULL,&hv_ImageHeight);
//		DilationRectangle1(m_pTempDetect->m_tParameters.hoModelRegion,&hoSearchRegion,200,1);
//		SmallestRectangle1(hoSearchRegion,NULL,&hv_Column1,NULL,&hv_Column2);
//		GenRectangle1(&hoSearchRegion,0,hv_Column1,hv_ImageHeight-1,hv_Column2);
//		ReduceDomain(m_hoPretreatImage,hoSearchRegion,&hoImageReduced);
//		HObject hoImagePart;
//		HTuple hv_OriginalRow,hv_OriginalColumn;
//		SmallestRectangle1(hoImageReduced,&hv_OriginalRow,&hv_OriginalColumn,NULL,NULL);
//		CropDomain(hoImageReduced,&hoImagePart);
//		FindNccModel(hoImagePart, m_pTempDetect->m_tParameters.hv_ModelID,HTuple(-10).TupleRad(), HTuple(10).TupleRad(), m_pTempDetect->m_tParameters.dMinScore, 1, 0.5, "true", 0, &hv_Row0, &hv_Column0, &hv_Angle0, &hv_Score0);
//		HTuple hv_ModelRow0,hv_ModelColumn0;
//		
//		AreaCenter(m_pTempDetect->m_tParameters.hoModelRegion,NULL,&hv_ModelRow0,&hv_ModelColumn0);
//		DispObj(hoSearchRegion,m_hvWindowID);
//		if (hv_Row0.Length() > 0)
//		{
//
//			hv_Row0=hv_Row0+hv_OriginalRow;
//			hv_Column0=hv_Column0+hv_OriginalColumn;
//
//
//
//			HTuple hv_Index;
//			TupleSortIndex(hv_Row0, &hv_Index);
//			TupleSelect(hv_Row0, hv_Index, &hv_Row0);
//			TupleSelect(hv_Column0, hv_Index, &hv_Column0);
//
//
//		}
//		for (int i=0;i<hv_Row0.Length();i++)
//		{
//		
//			if (i!=0)
//			{
//				if ((hv_Row0[i]-hv_Row0[i-1]).TupleAbs()>50)
//				{
//					GenCrossContourXld(&hoCross,hv_Row0[i], hv_Column0[i], 100,hv_Angle0);
//					DispObj(hoCross,m_hvWindowID);
//					CString strValue;
//					strValue.Format("匹配率：%.2f",hv_Score0[i].D());
//					SetTposition(m_hvWindowID,50*(1+i),12);
//					WriteString(m_hvWindowID,(HTuple)strValue);
//				}else
//				{
//					continue;
//				}
//			}else
//			{
//				GenCrossContourXld(&hoCross,hv_Row0[i], hv_Column0[i], 100,hv_Angle0);
//				DispObj(hoCross,m_hvWindowID);
//				CString strValue;
//				strValue.Format("匹配率：%.2f",hv_Score0[i].D());
//				SetTposition(m_hvWindowID,50*(1+i),12);
//				WriteString(m_hvWindowID,(HTuple)strValue);
//			}
//			
//			
//			
//		}
//		if (hv_Score0.Length()==0)
//		{
//			CString strValue;
//			strValue.Format("未发现模板");
//			SetTposition(m_hvWindowID,12,12);
//			WriteString(m_hvWindowID,(HTuple)strValue);
//		}
//	}
//
//
//}



//void CDlgParamSet::OnBnClickedBtnClearmodel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	
//	try
//	{
//		m_pTempDetect->m_tParameters.hoModelRegion.GenEmptyObj();
//		ClearNccModel(m_pTempDetect->m_tParameters.hv_ModelID);
//	}catch(HException& e)
//	{
//
//	}
//
//
//
//
//}


void CDlgParamSet::OnEnChangeEdtFoildetectregion()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	FreshNewParam();
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnBnClickedChkUseaobanseg()
{
	// TODO: 在此添加控件通知处理程序代码
	FreshNewParam();
}


void CDlgParamSet::OnCbnSelchangeComAobanregionmode()
{
	// TODO: 在此添加控件通知处理程序代码
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEdtFoildetectthresh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	FreshNewParam();
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnEnChangeEdtAobanetectregion()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	FreshNewParam();
	// TODO:  在此添加控件通知处理程序代码
}


void CDlgParamSet::OnEnChangeEdtAobandetectthresh()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	FreshNewParam();
	// TODO:  在此添加控件通知处理程序代码
}


//void CDlgParamSet::OnBnClickedBtnDrawrectangle()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	SetButtonStaus(FALSE);
//	try
//	{
//		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
//		DrawRectangle1(m_hvWindowID,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//		GenRectangle1(&m_pTempDetect->m_tParameters.hoLeftMonitorRegion,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//		SetButtonStaus(TRUE);
//	}catch(HException& e)
//	{
//		SetButtonStaus(TRUE);
//	}
//
//
//
//}


//void CDlgParamSet::OnBnClickedBtnDrawrightrectangle()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	SetButtonStaus(FALSE);
//	try
//	{
//		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
//		DrawRectangle1(m_hvWindowID,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//		GenRectangle1(&m_pTempDetect->m_tParameters.hoRightMonitorRegion,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//		SetButtonStaus(TRUE);
//	}catch(HException& e)
//	{
//		SetButtonStaus(TRUE);
//	}
//}


void CDlgParamSet::OnBnClickedChkUsevariabwidthdetect()
{
	// TODO: 在此添加控件通知处理程序代码
	FreshNewParam();
}


void CDlgParamSet::OnEnChangeEdtRelateoffset()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	FreshNewParam();
	// TODO:  在此添加控件通知处理程序代码
}
