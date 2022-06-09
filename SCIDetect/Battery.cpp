#include "StdAfx.h"

#include "Battery.h"
#include "DlgAlgorithm.h"
#include <iostream>   
#include <fstream>  
using namespace std;

extern CString   g_strConfigCommonFile;//Common.dh
extern e_GrabberType g_GrabberType;
extern e_ProductType g_ProductType;
extern e_CoatType g_emCoatType;
CStringArray g_StrArrayErrorName;
int g_nLaneCount = 0;
CBlendent g_Blendent;

CBattery::CBattery(void)
{

	//m_pMeasure = NULL;
	m_bExit = FALSE;
	InitializeCriticalSection(&m_csChange);
	Sleep(10);
	m_bPoweFlag = FALSE;
	m_hvReverseTimeCount.Clear();
	m_dMeanValue = 0;
	m_hoTempErrorRegion.GenEmptyObj();
	m_bExistJXFlag = FALSE;
	m_bExistOAPFlag = FALSE;
	m_bExistCaoWeiFlag = FALSE;
	m_bExistJXUpDownFlag = TRUE;
}
CBattery::~CBattery(void)
{
	m_bExit = TRUE;
	DeleteCriticalSection(&m_csChange);
	Sleep(10);
}

CBattery& CBattery::operator= (const CBattery& in)
{
	//CriticalSectionHelper lock(&m_csChange);
	m_tInit = in.m_tInit;
	m_dRowResolution = in.m_dRowResolution;
	m_dColResolution = in.m_dColResolution;
	m_strModelPath = in.m_strModelPath;
	m_tParameters = in.m_tParameters;
	m_tSeparateOut = in.m_tSeparateOut;
	m_strParamFile = in.m_strParamFile;
	m_hvErrorName = in.m_hvErrorName;
	m_hvStrFeature = in.m_hvStrFeature;
	m_tBlackRegion = in.m_tBlackRegion;
	m_tStandardSize = in.m_tStandardSize;
	m_bPoweFlag = in.m_bPoweFlag;
	m_dMeanValue = in.m_dMeanValue;
	m_CSeperate = in.m_CSeperate;
	m_tMLPParam = in.m_tMLPParam;
	m_bExistJXFlag = in.m_bExistJXFlag;
	m_bExistCaoWeiFlag = in.m_bExistCaoWeiFlag;
	m_bExistOAPFlag = in.m_bExistOAPFlag;
	m_bExistJXUpDownFlag = in.m_bExistJXUpDownFlag; ;
	return *this;
}

void CBattery::FlushFlag()
{
	//HTuple hvExistJXType;//间歇存在的类型 0：仅正面间歇  1：仅反面间歇 2：双面间歇
	if (m_tStandardSize.hvExistJXType == 2 || m_tStandardSize.hvExistJXType == m_tInit.nGrabberSN)//是否存在间歇
	{
		m_bExistJXFlag = TRUE;
	}
	//HTuple hv_CaoWeiType;//槽位存在的类型 0：不存在槽位 1：仅正面槽位  2：仅反面槽位 3：双面槽位
	if (m_tStandardSize.hv_CaoWeiType == 3 || m_tStandardSize.hv_CaoWeiType == m_tInit.nGrabberSN + 1)//是否存在槽位
	{
		m_bExistCaoWeiFlag = TRUE;
	}

	//HTuple hv_OAPType;//OAP存在的类型 0：不存在OAP 1：仅正面OAP  2：仅反面OAP 3：双面OAP
	if (m_tStandardSize.hv_OAPType == 3 || m_tStandardSize.hv_OAPType == m_tInit.nGrabberSN + 1)//是否存在OAP
	{
		m_bExistOAPFlag = TRUE;
	}
	if (m_tStandardSize.hvQiTouXianChu == 1)//默认底部
	{
		m_bExistJXUpDownFlag = TRUE;

	}
	else
	{
		m_bExistJXUpDownFlag = FALSE;
	}
}



int CBattery::Init(DETECTINIT &In)                  //初始化算法
{
	SetSystem("int_zooming", "false");
	SetSystem("store_empty_region", "false");
	SetHcppInterfaceStringEncodingIsUtf8(FALSE);
	SetSystem("filename_encoding", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("write_halcon_files_encoding", "locale");
	SetSystem("tsp_tuple_string_operator_mode", "byte");
	SetSystem("tuple_string_operator_mode", "byte");
	SetSystem("tuple_string_operator_mode", "byte");

	m_tInit = In;//111

	CString strSection;
	strSection.Format("GRABBER%d", In.nGrabberSN);
	m_dRowResolution = GetPrivateProfileDouble(strSection, "ROWRESOLUTION", 0.1, g_strConfigCommonFile);
	m_dColResolution = GetPrivateProfileDouble(strSection, "COLUMNRESOLUTION", 0.1, g_strConfigCommonFile);
	int nType = GetPrivateProfileInt("ERROR", "COUNTTYPE", 0, g_strConfigCommonFile);//缺陷种类数目
	g_StrArrayErrorName.RemoveAll();
	for (int i = 0; i < nType; i++)
	{
		CString strKey, strValue;
		strKey.Format("ERROR%dNAME", i);
		GetPrivateProfileString("ERRORINFO", strKey, "漏箔", strValue.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigCommonFile);
		m_hvErrorName.Append((HTuple)strValue);
		g_StrArrayErrorName.Add(strValue);
	}






	CString StrSystemPath;
	StrSystemPath.Format("%s//Config//System.vis", GetCurrentAppPath());
	m_strModelPath.Format("%s\\Config\\Products\\%s\\SCIDetect\\%d", GetCurrentAppPath(), In.strProductName, In.nGrabberSN);
	CreateMultilevelPath(m_strModelPath);
	if (FALSE == ReadParameters())
	{
		CString strMessageBox;
		strMessageBox.Format("%s读取检测参数失败!", In.strProductName);
		AfxMessageBox(strMessageBox);
		return FALSE;
	}

	m_strSystemVis.Format("%s/Config/System.vis", GetCurrentAppPath());
	g_nLaneCount = GetPrivateProfileInt("System", "nLaneCount", 1, m_strSystemVis);//单条还是双条
	////加载分割动态库
	//SEPARATEINIT AlgHoleInit;
	//AlgHoleInit.nGrabberSN=In.nGrabberSN;
	//AlgHoleInit.strProductName=In.strProductName;
	//AlgHoleInit.nProductType=In.nProductType;
	//AlgHoleInit.nCoatType=In.nCoatType;
	//AlgHoleInit.pLogView=In.pLogView;
	//m_pMeasure = LoadDHInterFaceDll("SCISeparate");
	//m_pMeasure->DoSomething("INITIALIZE",&AlgHoleInit,NULL);
	SEPARATEINIT AlgHoleInit;
	AlgHoleInit.nGrabberSN = In.nGrabberSN;
	AlgHoleInit.strProductName = In.strProductName;
	AlgHoleInit.nProductType = In.nProductType;
	AlgHoleInit.pLogView = In.pLogView;
	AlgHoleInit.nCoatType = In.nCoatType;
	m_CSeperate.Init(AlgHoleInit);

	for (int i = 0; i < 18; i++)
	{
		m_hvStrFeature.Append((HTuple)NAME_Feature[i]);
	}

	return TRUE;
}


void  CBattery::InitDetect()
{

#pragma  region "初始化输出结构体"
	m_dMeanValue = 0;
	m_hoAdaptedImage = m_ptDetectIn->hoDetectImage;
	TupleGenConst(MAX_LANE, TRUE, &m_ptDetectOut->hv_Flag);//默认1是OK 0是NG
	m_ptDetectOut->hv_ErrorType.Clear();
	m_ptDetectOut->hv_ErrorLevel.Clear();
	m_ptDetectOut->hv_ErrorBelongStrip.Clear();
	TupleGenConst(1, 0, &m_ptDetectOut->hv_LocateMissing);
	TupleGenConst(MAX_LANE, 0, &m_ptDetectOut->hv_CoatWidth);
	TupleGenConst(MAX_LANE, 0, &m_ptDetectOut->hv_CoatHeight);
	TupleGenConst(MAX_LANE, 0, &m_ptDetectOut->hv_FoilWidth);
	TupleGenConst(MAX_LANE, 0, &m_ptDetectOut->hv_FoilHeight);
	TupleGenConst(1, -1, &m_ptDetectOut->hv_JuanRaoMarkRow);
	TupleGenConst(1, 0, &m_ptDetectOut->hv_BaiBian);

	GenEmptyObj(&m_ptDetectOut->ho_ErrorRegion);
	GenEmptyObj(&m_ptDetectOut->ho_Coat);
	GenEmptyObj(&m_ptDetectOut->ho_Foil);
	m_hoTempErrorRegion.GenEmptyObj();
	GenRectangle1(&m_ptDetectOut->ho_Coat, 0, 0, 200, 200);
	GenRectangle1(&m_ptDetectOut->ho_Foil, 0, 0, 200, 200);
	m_tBlackRegion.InitObject();//初始化区域
#pragma endregion

#pragma region "初始化检测需要的成员变量"
	HomMat2dIdentity(&m_hvHomMat2D);
	GenEmptyObj(&m_tSeparateOut.ho_Coat);
	GenEmptyObj(&m_tSeparateOut.ho_Foil);

	GenEmptyObj(&m_tSeparateOut.ho_DispRegion);

	GenEmptyObj(&m_hoLEnableChoiceRegions);
	GenEmptyObj(&m_hoDEnableChoiceRegions);
	GenEmptyObj(&m_hoCoatMid);
	GenEmptyObj(&m_hoBoderRegion);
	m_hoAt9Region.GenEmptyObj();
	m_hoAdaptedImage = m_ptDetectIn->hoDetectImage;
	m_hoSegRegions.GenEmptyObj();
	m_tRegion.Clear();

	m_hvPeakGrayMid = 0;


	//间歇涂布
	m_ptDetectOut->hv_AT9EdageToProEdage.Clear();
	m_ptDetectOut->hv_CpkEdageToProEdage.Clear();
	m_ptDetectOut->hv_LeftEadgePixel = 0;
	m_ptDetectOut->hv_RightEadgePixel = 8192;
	m_ptDetectOut->ho_ExtractCaoWeiRegions.GenEmptyObj();//预提取的槽位区域
	TupleGenSequence(0, m_tStandardSize.hvTripNum - 1, 1, &m_ptDetectOut->hv_TripSNTuple);//槽位隶属于条数
	TupleGenConst(m_tStandardSize.hvTripNum, 0, &m_ptDetectOut->hv_CaoWeiWidthTuple);//槽位宽度
	TupleGenConst(m_tStandardSize.hvTripNum, 0, &m_ptDetectOut->hv_CaoWeiHeightTuple);//槽位高度
	TupleGenConst(m_tStandardSize.hvTripNum, 0, &m_ptDetectOut->hv_CaoWeiXPositionTuple);//槽位X坐标
	TupleGenConst(m_tStandardSize.hvTripNum, 0, &m_ptDetectOut->hv_CaoWeiYpositionTuple);//槽位Y坐标


	m_ptDetectOut->hv_JXHeight = 0;
	m_ptDetectOut->hv_MPLength = 0;
	m_ptDetectOut->hv_SumLength = 0;
	m_ptDetectOut->ho_JXRegion.GenEmptyObj();

	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve1);//预留字段1 膜长
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve2);//预留字段2 间歇高度
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve3);//预留字段3 首间距
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve4);//预留字段4 极耳数目
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve5);//预留字段5 上圆弧
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve6);//预留字段6 下圆弧
	//TupleGenConst(1,0,&m_ptDetectOut->hv_Reserve7);//预留字段7 凹漏箔







#pragma endregion
}

BOOL CBattery::GetDynRegion(HObject& ho_Image)
{
	try
	{
		// Local iconic variables
		HObject  ho_ImageOut, ho_ROI_0, ho_ObjectSelected;
		HObject  ho_ImageReduced, ho_LightRegion, ho_DarkRegion;
		// Local control variables
		HTuple  hv_WidthValue, hv_HeightValue, hv_Mean;
		HTuple  hv_Deviation;
		ho_ImageOut = ho_Image;
		HTuple hv_AllMean;
		HTuple hv_DiffMean;
		double dMean;
		HObject hoCaoweiDialtions;
		hoCaoweiDialtions.GenEmptyObj();
		DilationRectangle1(m_tSeparateOut.ho_ExtractCaoWeiRegions, &hoCaoweiDialtions, 4 / m_dColResolution, 4 / m_dRowResolution);
		Difference(m_hoCoatMid, hoCaoweiDialtions, &m_hoCoatMid);
		Intensity(m_hoCoatMid, ho_ImageOut, &m_hvPeakGrayMid, NULL);
		Connection(m_hoCoatMid, &m_hoCoatMid);
		m_dMeanValue = m_hvPeakGrayMid.D();
		for (int i = 1; i <= m_hoCoatMid.CountObj(); i++)
		{

			RegionFeatures(m_hoCoatMid.SelectObj(i), "width", &hv_WidthValue);
			RegionFeatures(m_hoCoatMid.SelectObj(i), "height", &hv_HeightValue);
			/*PartitionRectangle(m_hoCoatMid.SelectObj(i), &m_hoSegRegions, hv_WidthValue/10, hv_HeightValue);*/
			PartitionRectangle(m_hoCoatMid.SelectObj(i), &m_hoSegRegions, 50 / m_dColResolution, hv_HeightValue);
			Intensity(m_hoSegRegions, ho_ImageOut, &hv_AllMean, &hv_Deviation);
			Intensity(m_hoCoatMid.SelectObj(i), m_hoAdaptedImage, &m_hvPeakGrayMid, NULL);


			int nLightLowThresh, nDarkHighThresh;
			double dDarkScale, dLightScale;
			for (int hv_j = 1; hv_j <= m_hoSegRegions.CountObj(); hv_j += 1)
			{
				SelectObj(m_hoSegRegions, &ho_ObjectSelected, hv_j);
				ReduceDomain(ho_ImageOut, ho_ObjectSelected, &ho_ImageReduced);
				dMean = hv_AllMean[hv_j - 1].D();
				hv_DiffMean = m_hvPeakGrayMid.D() - dMean;
				if (hv_DiffMean.TupleAbs() > 80)
				{
					dMean = m_hvPeakGrayMid.D();
				}
				if (hv_j == 1 || hv_j == 10)
				{
					dLightScale = m_tParameters.tPretreatParam.dBorderLightCof;
					dDarkScale = m_tParameters.tPretreatParam.dBorderDarkCof;
				}
				else
				{
					dLightScale = m_tParameters.tPretreatParam.dMidLightCof;
					dDarkScale = m_tParameters.tPretreatParam.dMidDarkCof;
				}
				/*	nLightLowThresh=(1+dLightScale)*dMean;
					nDarkHighThresh=(1-dDarkScale)*dMean;*/

				nLightLowThresh = dMean + dLightScale;
				nDarkHighThresh = dMean - dDarkScale;

				Threshold(ho_ImageReduced, &ho_LightRegion, MAX(30, MIN(nLightLowThresh, 255)), 255);
				Threshold(ho_ImageReduced, &ho_DarkRegion, 0, MAX(MIN(255, nDarkHighThresh), 10));
				ConcatObj(m_hoLEnableChoiceRegions, ho_LightRegion, &m_hoLEnableChoiceRegions);
				ConcatObj(m_hoDEnableChoiceRegions, ho_DarkRegion, &m_hoDEnableChoiceRegions);

			}
		}
		Union1(m_hoLEnableChoiceRegions, &m_hoLEnableChoiceRegions);
		ClosingRectangle1(m_hoLEnableChoiceRegions, &m_hoLEnableChoiceRegions, 7, 7);
		FillUp(m_hoLEnableChoiceRegions, &m_hoLEnableChoiceRegions);
		Connection(m_hoLEnableChoiceRegions, &m_hoLEnableChoiceRegions);
		SelectShape(m_hoLEnableChoiceRegions, &m_hoLEnableChoiceRegions, "area", "and", 20, INT_MAX);

		Union1(m_hoDEnableChoiceRegions, &m_hoDEnableChoiceRegions);
		ClosingRectangle1(m_hoDEnableChoiceRegions, &m_hoDEnableChoiceRegions, 7, 7);
		FillUp(m_hoDEnableChoiceRegions, &m_hoDEnableChoiceRegions);
		Connection(m_hoDEnableChoiceRegions, &m_hoDEnableChoiceRegions);
		SelectShape(m_hoDEnableChoiceRegions, &m_hoDEnableChoiceRegions, "area", "and", 20, INT_MAX);

		Union1(m_hoCoatMid, &m_hoCoatMid);


	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("GetDynRegion 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());
		//((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
		return FALSE;
	}
	return TRUE;
}
void CBattery::GetChoiceRegions(HObject& hoLChoiceRegions, HObject& hoDChoiceRegions)
{
	hoLChoiceRegions.GenEmptyObj();
	hoDChoiceRegions.GenEmptyObj();
	HObject hoDiff, hoDiff1;
	hoDiff.GenEmptyObj();
	hoDiff1.GenEmptyObj();
	DilationRectangle1(m_ptDetectOut->ho_ErrorRegion, &hoDiff, 11, 11);
	DilationRectangle1(m_hoTempErrorRegion, &hoDiff1, 11, 11);
	Difference(m_hoLEnableChoiceRegions, hoDiff, &hoLChoiceRegions);//去除已经检测出来的缺陷
	Difference(hoLChoiceRegions, hoDiff1, &hoLChoiceRegions);//去除已经检测出来的缺陷

	Difference(m_hoDEnableChoiceRegions, hoDiff, &hoDChoiceRegions);//去除已经检测出来的缺陷
	Difference(hoDChoiceRegions, hoDiff1, &hoDChoiceRegions);//去除已经检测出来的缺陷




	return;
}



void CBattery::AT9Detect()//AT9检测
{

	//try
	//{

	//	if (m_tParameters.tAT9Param.bUseDetect==FALSE)
	//	{
	//		return;
	//	}
	//	HTuple   hv_AT9LThresh,hv_LMinWidth,hv_LMinHeight;
	//	HTuple   hv_AT9DarkThresh,hv_DarkMinWidth,hv_DarkMinHeight;

	//	double dDilationWidth;
	//	hv_AT9LThresh=m_tParameters.tAT9Param.nAT9LMinThresh;
	//	hv_LMinWidth=m_tParameters.tAT9Param.dAT9LMinWidth/m_dColResolution;
	//	hv_LMinHeight=m_tParameters.tAT9Param.dAT9LMinHeight/m_dRowResolution;

	//	hv_AT9DarkThresh=m_tParameters.tAT9Param.nAT9DarkMaxThresh;
	//	hv_DarkMinWidth=m_tParameters.tAT9Param.dAT9DarkMinWidth/m_dColResolution;
	//	hv_DarkMinHeight=m_tParameters.tAT9Param.dAT9DarkMinHeight/m_dRowResolution;
	//	dDilationWidth=m_tParameters.tPretreatParam.dBorderWidth/m_dColResolution;
	//	/////检测AT9漏金属需要向涂布区域扩充一下
	//	HObject ho_ImageReduced,hoRegion,ho_AT9LeakFoilError,ho_AT9PoSunError,hoConnections,hoFillUp;
	//	ho_AT9LeakFoilError.GenEmptyObj();
	//	ho_AT9PoSunError.GenEmptyObj();
	//	////////检测AT9破损///////
	//	HObject hoBorderRegion;
	//	HObject hoLightRegion;
	//	hoBorderRegion.GenEmptyObj();
	//	if (m_tInit.nProductType==positive)//正极是AT9 负极是边界
	//	{
	//		ConcatObj(m_hoAt9Region,m_hoBoderRegion,&hoLightRegion);
	//		Union1(hoLightRegion,&hoLightRegion);
	//		hoBorderRegion=m_hoAt9Region;
	//	}else
	//	{
	//		hoBorderRegion=m_hoBoderRegion;
	//		hoLightRegion =m_hoBoderRegion;
	//	}


	//	if (hoBorderRegion.CountObj()>0)
	//	{

	//		ReduceDomain(m_ptDetectIn->hoDetectImage, hoLightRegion, &ho_ImageReduced);
	//		Threshold(ho_ImageReduced, &hoRegion, MIN(hv_AT9LThresh, 255), 255);
	//		ClosingRectangle1(hoRegion,&hoRegion,15,15);
	//		Connection(hoRegion, &hoRegion);
	//		SelectShape(hoRegion, &ho_AT9LeakFoilError,HTuple("height").Append("width") , "and", hv_LMinHeight.Append(hv_LMinWidth), HTuple(INT_MAX).Append(INT_MAX));
	//		if (ho_AT9LeakFoilError.CountObj()>0)
	//		{
	//			AddError(ho_AT9LeakFoilError, LouJinShu);
	//		}
 //           //////////////////////检测暗缺陷///////////////////
	//		ErosionRectangle1(hoBorderRegion,&hoBorderRegion,3,1);
	//		ReduceDomain(m_ptDetectIn->hoDetectImage,hoBorderRegion,&ho_ImageReduced);
	//		Threshold(ho_ImageReduced,&hoRegion,0, hv_AT9DarkThresh);
	//		Connection(hoRegion,&hoConnections);
	//		SelectShape(hoConnections,&hoConnections,"area","and",10,INT_MAX);
	//		FillUp(hoConnections,&hoFillUp);
	//		ClosingRectangle1(hoFillUp,&hoFillUp,31,31);
	//		Connection(hoFillUp,&hoConnections);
	//		SelectShape(hoConnections,&ho_AT9PoSunError,HTuple("height").Append("width"),"and",hv_DarkMinHeight.Append(hv_DarkMinWidth*2),HTuple(INT_MAX).Append(INT_MAX));
	//		if (ho_AT9PoSunError.CountObj()>0)
	//		{
	//			AddError(ho_AT9PoSunError, AT9LouJinShu);
	//		}
	//	}

	//}catch(HException& e)
	//{
	//	CString strException;
	//	strException.Format("AT9Detect 函数出现异常:%s_%s",e.ProcName().Text(),e.ErrorMessage().Text());

	//}
	return;

}


void CBattery::TuoWeiDetect()//拖尾检测
{

	/////增加阴极间歇区域检测//////
	if (m_tParameters.tTuoWeiParam.bUseDetect == FALSE)
	{
		return;
	}
	HObject ho_ImageReduced, ho_Region, ho_Connections, ho_FillUp, hoErrorRegion;
	HObject hoJXRegion;
	ErosionRectangle1(m_tBlackRegion.ho_JXRegion, &hoJXRegion, 21, 15);
	HTuple hvJXMinThresh, hvJXMaxThresh, hvJXMinArea, hvJXMaxHeight;
	hvJXMinThresh = m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh;
	hvJXMaxThresh = m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh;
	hvJXMinArea = m_tParameters.tTuoWeiParam.dJXTapeLTMinArea / m_dRowResolution / m_dColResolution;
	hvJXMaxHeight = m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight / m_dRowResolution;
	if (hoJXRegion.CountObj() > 0)
	{
		ReduceDomain(m_hoAdaptedImage, hoJXRegion, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Region, MIN(hvJXMinThresh.I(), hvJXMaxThresh.I()), MAX(hvJXMinThresh.I(), hvJXMaxThresh.I()));
		Connection(ho_Region, &ho_Connections);
		FillUp(ho_Connections, &ho_FillUp);
		ClosingRectangle1(ho_FillUp, &ho_FillUp, 31, 31);
		SelectShape(ho_FillUp, &hoErrorRegion, (HTuple("area").Append("height")), "or", (HTuple(hvJXMinArea).Append(hvJXMaxHeight)), (HTuple(INT_MAX).Append(INT_MAX)));
		if (hoErrorRegion.CountObj() > 0)
		{
			AddError(hoErrorRegion, TuoWei);
			hoErrorRegion.GenEmptyObj();
		}

	}

}


void CBattery::LeakFoilDetect()
{


	try
	{
		// Local iconic variables
		HObject  ho_LeakFoilRegion, ho_ImageReduced, ho_RegionDynThresh;
		HObject  ho_RegionClosing, ho_RegionFillUp, ho_ConnectedRegions;
		HObject  ho_RegionTrans, ho_SelectedRegions, ho_LeakFoilError1;
		HObject  ho_Region, ho_RegionClosing1, ho_ConnectedRegions1;
		HObject  ho_RegionTrans1, ho_LeakFoilError2;

		// Local control variables
		HTuple  hv_bDetect, hv_dResolution, hv_dDynThresh;
		HTuple  hv_dLeakFoilThresh;
		HTuple  hv_dMinDeviation, hv_Concat, hv_Max, hv_Min, hv_LeakFoilMinArea;

		HObject ho_ErrorRegion;
		ho_ErrorRegion.GenEmptyObj();
		if (FALSE == m_tParameters.tLeakFoilParam.bUseDetect)
		{
			return;
		}
		hv_dLeakFoilThresh = m_tParameters.tLeakFoilParam.nThresh;
		hv_LeakFoilMinArea = m_tParameters.tLeakFoilParam.dMinArea / m_dRowResolution / m_dRowResolution;
		//////检测大缺陷
		HObject hoRegion, ho_LeakFoilError0, hoCoat;
		ReduceDomain(m_hoAdaptedImage, m_hoCoatMid, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &hoRegion, MIN(hv_dLeakFoilThresh, 255), 255);
		Difference(hoRegion, m_ptDetectOut->ho_ErrorRegion, &hoRegion);
		Connection(hoRegion, &hoRegion);
		SelectShape(hoRegion, &ho_ErrorRegion, "area", "and", hv_LeakFoilMinArea, INT_MAX);
		if (ho_ErrorRegion.CountObj() > 0)
		{
			AddError(ho_ErrorRegion, LouJinShu);
			ho_ErrorRegion.GenEmptyObj();
		}




		/*	hv_dMinDiameter = m_tParameters.tLeakFoilParam.dMinDiameter/m_dRowResolution;
		hv_dMaxDiameter = m_tParameters.tLeakFoilParam.dMaxDiameter/m_dRowResolution;
		hv_dMinDeviation =  m_tParameters.tLeakFoilParam.dDeviation;*/
		//HObject hoChoiceRegions;
		//HObject hoLChoiceRegions, hoDChoiceRegions;
		//GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
		//hoChoiceRegions = hoLChoiceRegions;
		//if (FALSE == hoChoiceRegions.CountObj())
		//{
		//	
		//	return;
		//}
		//ClosingRectangle1(hoChoiceRegions, &ho_RegionClosing, 3.5,3.5);
		//FillUp(ho_RegionClosing, &ho_RegionFillUp);
		//Connection(ho_RegionFillUp, &ho_ConnectedRegions);
		////*******LeakFoilError1选择白点中心有黑点的漏箔缺陷********************
		//TupleConcat(10, MAX(0,m_hvPeakGrayMid - hv_dDynThresh), &hv_Concat);
		//TupleMax(hv_Concat, &hv_Max);
		//TupleMin(hv_Concat, &hv_Min);
		//SelectGray(ho_ConnectedRegions, m_hoAdaptedImage, &ho_SelectedRegions, HTuple("min").Append("deviation"),"and", hv_Min.TupleConcat(hv_dMinDeviation), hv_Max.TupleConcat(100));
		//SelectShape(ho_SelectedRegions, &ho_LeakFoilError1, "max_diameter", "and", hv_dMinDiameter,hv_dMaxDiameter);
		//ConcatObj(ho_ErrorRegion, ho_LeakFoilError1, &ho_ErrorRegion);
		//if (ho_ErrorRegion.CountObj()>0)
		//{
		//	AddError(ho_ErrorRegion, LouJinShu);
		//}


	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("LeakFoilDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());

	}



	return;
}
void CBattery::BubbleDetect()
{
	try
	{


		// Local iconic variables
		HObject  ho_BubbleRegion, ho_ImageReduced, ho_ImageReduceMean;
		HObject  ho_RegionDynThresh, ho_RegionClosing, ho_ConnectedRegions;
		HObject  ho_SelectedRegions, ho_RegionFillUp, ho_SelectedRegions1;
		HObject  ho_SelectBubbleObject, ho_ObjectSelected, ho_RegionDilation;
		HObject  ho_RegionDifference;

		// Local control variables
		HTuple  hv_bDetect, hv_dResolution, hv_dDynThresh;
		HTuple  hv_dMinArea, hv_dMinCircularity;
		HTuple  hv_dNum, hv_Number, hv_i, hv_MeanIn, hv_Deviation, hv_GrayDiff;
		HTuple  hv_MeanOut, hv_Deviation1;


		HObject ho_ErrorRegion;
		HTupleVector hvec_Param(1);
		ho_ErrorRegion.GenEmptyObj();

		if (FALSE == m_tParameters.tBubbleParam.bUseDetect)
		{
			return;
		}

		hv_dDynThresh = m_tParameters.tBubbleParam.nDynThresh;
		hv_dMinArea = m_tParameters.tBubbleParam.dMinArea / m_dRowResolution;
		hv_dMinCircularity = m_tParameters.tBubbleParam.dMinCircularity;




		HObject hoChoiceRegions;
		HObject hoLChoiceRegions, hoDChoiceRegions;
		HObject ho_RegionDilationRectangle, hoNearRegion;
		HTuple  hv_NearMean;
		GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
		hoChoiceRegions = hoDChoiceRegions;
		if (FALSE == hoChoiceRegions.CountObj())
		{
			return;
		}
		ClosingCircle(hoChoiceRegions, &ho_RegionClosing, 8.5);
		Connection(ho_RegionClosing, &ho_ConnectedRegions);
		FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
		SelectShape(ho_RegionFillUp, &ho_SelectedRegions1, (HTuple("circularity").Append("area")), "and", hv_dMinCircularity.TupleConcat(hv_dMinArea), (HTuple(1).Append(INT_MAX)));
		SelectGray(ho_SelectedRegions1, m_hoAdaptedImage, &ho_ErrorRegion, HTuple("mean"), "and", 0, MAX(0, m_hvPeakGrayMid.D() - hv_dDynThresh));
		if (ho_ErrorRegion.CountObj() > 0)
		{
			AddError(ho_ErrorRegion, QiPao);

		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("BubbleDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}

}
void CBattery::SelectLine(HObject ho_AllLineRegion, HObject *ho_SelectLineRegion, HTuple hv_StandarWidth,
	HTuple hv_StandarLength, HTuple *hv_WidthValue, HTuple *hv_LengthValue)
{

	// Local iconic variables
	HObject  ho_ObjectSelected, ho_RegionMoved, ho_Skeleton;
	HObject  ho_DistanceImage;

	// Local control variables
	HTuple  hv_Number, hv_i, hv_Row1, hv_Column1;
	HTuple  hv_Row2, hv_Column2, hv_null, hv_Diameter, hv_Min;
	HTuple  hv_Max, hv_Range, hv_Area, hv_Row, hv_Column;
	HTuple hv_Radius;
	GenEmptyObj(&(*ho_SelectLineRegion));
	CountObj(ho_AllLineRegion, &hv_Number);
	TupleGenConst(hv_Number, 0, &(*hv_WidthValue));
	TupleGenConst(hv_Number, 0, &(*hv_LengthValue));
	HTuple hvLengthSub, hvWidthSub, hvResult;
	HTuple hvLengthFlag, hvWidthFlag;
	HTuple hv_ErrorIndex;
	if (hv_Number > 0)
	{
		Skeleton(ho_AllLineRegion, &ho_Skeleton);
		AreaCenter(ho_Skeleton, &hv_Area, &hv_Row, &hv_Column);
		InnerCircle(ho_AllLineRegion, NULL, NULL, &hv_Radius);
		TupleSub(hv_Area, hv_StandarLength, &hvLengthSub);
		TupleSub(hv_Radius * 2, hv_StandarWidth, &hvWidthSub);
		TupleGreaterEqualElem(hvLengthSub, 0, &hvLengthFlag);
		TupleGreaterEqualElem(hvWidthSub, 0, &hvWidthFlag);
		TupleAnd(hvLengthFlag, hvWidthFlag, &hvResult);
		TupleFind(hvResult, 1, &hv_ErrorIndex);

		if (hv_ErrorIndex.Length() > 0 && hv_ErrorIndex != -1)
		{

			SelectObj(ho_AllLineRegion, &ho_ObjectSelected, hv_ErrorIndex + 1);
			TupleSelect(hv_Area, hv_ErrorIndex, &(*hv_LengthValue));
			TupleSelect(hv_Radius * 2, hv_ErrorIndex, &(*hv_WidthValue));
			ConcatObj((*ho_SelectLineRegion), ho_ObjectSelected, &(*ho_SelectLineRegion));
		}
	}
	return;
}
void CBattery::LightLineDetect()
{
	//try
	//{
	//	// Local iconic variables
	//	HObject  ho_LightLineRegion, ho_ImageReduced;
	//	HObject  ho_RegionDynThresh, ho_RegionClosing, ho_ConnectedRegions;
	//	HObject  ho_SelectedRegions, ho_RegionFillUp, ho_SelectRegion;

	//	// Local control variables
	//	HTuple  hv_bDetect, hv_dResolution, hv_dDynThresh;
	//	HTuple  hv_dMinWidth, hv_dMinLength, hv_dChangKuanBi, hv_dNum;
	//	HTuple  hv_WidthValue, hv_LengthValue, hv_Number;
	//	HObject ho_ErrorRegion;
	//	GenEmptyObj(&ho_ErrorRegion);
	//	
	//	if (FALSE==m_tParameters.tLightLineParam.bUseDetect)
	//	{
	//		return;
	//	}
	//	hv_dDynThresh = m_tParameters.tLightLineParam.nDynThresh;
	//	hv_dMinWidth = m_tParameters.tLightLineParam.dMinWidth/m_dRowResolution;
	//	hv_dMinLength = m_tParameters.tLightLineParam.dMinHeight/m_dRowResolution;
	//	hv_dChangKuanBi = m_tParameters.tLightLineParam.dMinLengthWidthRatio;
	//	

	//	HObject hoChoiceRegions;
	//	HObject hoLChoiceRegions, hoDChoiceRegions;
	//	GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
	//	hoChoiceRegions = hoLChoiceRegions;
	//	if (FALSE == hoChoiceRegions.CountObj())
	//	{
	//		return;
	//	}
	//	SelectShape(hoChoiceRegions, &hoChoiceRegions, "area", "and", 300, INT_MAX);
	//	Union1(hoChoiceRegions, &ho_RegionClosing);
	//	ClosingRectangle1(ho_RegionClosing, &ho_RegionClosing, 15,55);
	//	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	//	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "anisometry", "and", hv_dChangKuanBi, INT_MAX);
	//	SelectGray(ho_SelectedRegions, m_hoAdaptedImage, &ho_ConnectedRegions, "mean", "and", MAX(0, m_hvPeakGrayMid+hv_dDynThresh),255);
	//	SelectLine(ho_ConnectedRegions, &ho_ErrorRegion, hv_dMinWidth, hv_dMinLength, &hv_WidthValue,&hv_LengthValue);
	//	HTuple hv_Rad0,hv_Rad1;
	//	HObject hoErrorRegion;
	//	hoErrorRegion.GenEmptyObj();
	//	TupleRad(30,&hv_Rad0);
	//	TupleRad(150,&hv_Rad1);
	//	SelectShape(ho_ErrorRegion, &hoErrorRegion, "phi", "and", hv_Rad0, hv_Rad1);
	//	if (0 != hoErrorRegion.CountObj())
	//	{
	//		AddError(hoErrorRegion, TiaoHen);
	//	}
	//	TupleRad(-30,&hv_Rad0);
	//	TupleRad(-150,&hv_Rad1);
	//	SelectShape(ho_ErrorRegion, &hoErrorRegion, "phi", "and", hv_Rad1, hv_Rad0);
	//	if (0 != hoErrorRegion.CountObj())
	//	{
	//		AddError(hoErrorRegion, TiaoHen);
	//	}
	//	
	//}catch(HException& e)
	//{
	//	CString strException;
	//	strException.Format("LightLineDetect 函数出现异常:%s_%s",e.ProcName().Text(),e.ErrorMessage().Text());
	//	//((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);

	//}

	return;
}
void CBattery::BlackLineDetect()
{
	try
	{
		// Local iconic variables
		HObject  ho_BlackLineRegion, ho_ImageReduced;
		HObject  ho_RegionDynThresh, ho_RegionClosing, ho_ConnectedRegions;
		HObject  ho_SelectedRegions, ho_RegionFillUp, ho_SelectedRegions1;
		HObject  ho_SelectedRegions2;

		// Local control variables
		HTuple  hv_bDetect, hv_dResolution, hv_dDynThresh;
		HTuple  hv_dMinWidth, hv_dMinLength, hv_dMinChangKuanBi;
		HTuple  hv_dNum, hv_WidthValue, hv_LengthValue, hv_Number;
		HObject ho_ErrorRegion;
		GenEmptyObj(&ho_ErrorRegion);

		if (FALSE == m_tParameters.tDarkLineParam.bUseDetect)
		{
			return;
		}


		hv_dDynThresh = m_tParameters.tDarkLineParam.nDynThresh;
		hv_dMinWidth = m_tParameters.tDarkLineParam.dMinWidth / m_dRowResolution;
		hv_dMinLength = 0.2;
		hv_dMinChangKuanBi = m_tParameters.tDarkLineParam.dMinLengthWidthRatio;


		HObject hoChoiceRegions;
		HObject hoLChoiceRegions, hoDChoiceRegions;
		GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
		//	hoChoiceRegions = hoDChoiceRegions;
		ConcatObj(hoLChoiceRegions, hoDChoiceRegions, &hoChoiceRegions);
		if (FALSE == hoChoiceRegions.CountObj())
		{
			return;
		}
		SelectShape(hoChoiceRegions, &hoChoiceRegions, "area", "and", 300, INT_MAX);
		ClosingRectangle1(hoChoiceRegions, &ho_RegionClosing, 15, 55);
		Connection(ho_RegionClosing, &ho_ConnectedRegions);
		FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
		SelectShape(ho_RegionFillUp, &ho_SelectedRegions, "anisometry", "and", hv_dMinChangKuanBi, INT_MAX);
		//选择白线和黑线
		SelectGray(ho_SelectedRegions, m_hoAdaptedImage, &ho_ConnectedRegions, HTuple("mean").Append("mean"), "or", HTuple(0).Append(MIN(m_hvPeakGrayMid.D() + hv_dDynThresh.I(), 255)), HTuple(MAX(0, m_hvPeakGrayMid.D() - hv_dDynThresh.I())).Append(255));


		SelectLine(ho_ConnectedRegions, &ho_ErrorRegion, hv_dMinWidth, hv_dMinLength, &hv_WidthValue, &hv_LengthValue);


		if (ho_ErrorRegion.CountObj() > 0)
		{
			AddError(ho_ErrorRegion, TiaoHen);
		}


		//HObject hoErrorRegion;
		//HTuple hv_Rad0,hv_Rad1;
		//GenEmptyObj(&hoErrorRegion);
		//TupleRad(30,&hv_Rad0);
		//TupleRad(150,&hv_Rad1);
		//SelectShape(ho_ErrorRegion, &hoErrorRegion, "phi", "and", hv_Rad0, hv_Rad1);
		//if (hoErrorRegion.CountObj()>0)
		//{
		//	AddError(hoErrorRegion, TiaoHen);
		//}


		//TupleRad(-30,&hv_Rad0);
		//TupleRad(-150,&hv_Rad1);
		//SelectShape(ho_ErrorRegion, &hoErrorRegion, "phi", "and", hv_Rad1, hv_Rad0);
		//if (hoErrorRegion.CountObj()>0)
		//{
		//	AddError(hoErrorRegion, TiaoHen);
		//}


	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("BlackLineDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

	}
	return;
}
void CBattery::BlackDotDetect()
{

	try
	{
		// Local iconic variables
		HObject  ho_BlackDotRegion, ho_ImageReduced, ho_Region;
		HObject  ho_RegionClosing, ho_RegionFillUp, ho_ConnectedRegions;
		HObject  ho_SelectedRegions, ho_RegionTrans;

		// Local control variables
		HTuple  hv_bDetect, hv_DynThresh;
		HTuple  hv_dMinArea;

		HObject ho_ErrorRegion;
		GenEmptyObj(&ho_ErrorRegion);

		if (FALSE == m_tParameters.tDarkDotParam.bUseDetect)
		{
			return;
		}
		hv_DynThresh = m_tParameters.tDarkDotParam.nDarkDotDynThresh;
		hv_dMinArea = m_tParameters.tDarkDotParam.dMinArea / m_dRowResolution / m_dColResolution;

		HObject hoChoiceRegions;
		HObject hoLChoiceRegions, hoDChoiceRegions;
		GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
		hoChoiceRegions = hoDChoiceRegions;
		if (FALSE == hoChoiceRegions.CountObj())
		{
			return;
		}
		//HObject hoImageReduced, hoRegion;
		//ReduceDomain(m_hoAdaptedImage, m_hoCoatMid, &hoImageReduced);
		//Threshold(hoImageReduced, &hoRegion, 0, hv_DynThresh);
		//ClosingRectangle1(hoRegion, &ho_RegionClosing, 15,15);
		Connection(hoChoiceRegions, &ho_RegionClosing);
		SelectGray(ho_RegionClosing, m_hoAdaptedImage, &ho_ConnectedRegions, "mean", "and", 0, MAX(0, m_hvPeakGrayMid.D() - hv_DynThresh));
		SelectShape(ho_ConnectedRegions, &ho_ErrorRegion, "area", "and", hv_dMinArea, INT_MAX);
		if (ho_ErrorRegion.CountObj() > 0)
		{
			AddError(ho_ErrorRegion, HeiDian);
		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("BlackDotDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());

	}
	return;
}

void CBattery::LightDotDetect()
{
	try
	{
		// Local iconic variables
		HObject  ho_LightDotRegion, ho_ImageReduced, ho_Region;
		HObject  ho_RegionClosing, ho_RegionFillUp, ho_ConnectedRegions;
		HObject ho_ErrorRegion;
		GenEmptyObj(&ho_ErrorRegion);

		if (FALSE == m_tParameters.tLightDotParam.bUseDetect)
		{
			return;
		}
		HTuple hv_dDynThresh, hv_dMinArea;
		hv_dDynThresh = m_tParameters.tLightDotParam.nLightDotDynThresh;
		hv_dMinArea = m_tParameters.tLightDotParam.dMinArea / m_dRowResolution / m_dColResolution;
		HObject hoChoiceRegions;
		HObject hoLChoiceRegions, hoDChoiceRegions;
		GetChoiceRegions(hoLChoiceRegions, hoDChoiceRegions);
		hoChoiceRegions = hoLChoiceRegions;
		if (FALSE == hoChoiceRegions.CountObj())
		{
			return;
		}
		ClosingRectangle1(hoChoiceRegions, &ho_RegionClosing, 15, 15);
		SelectGray(ho_RegionClosing, m_hoAdaptedImage, &ho_ConnectedRegions, "mean", "and", MIN(hv_dDynThresh.I() + m_hvPeakGrayMid.D(), 255), 255);
		SelectShape(ho_ConnectedRegions, &ho_ErrorRegion, "area", "and", hv_dMinArea, INT_MAX);
		if (ho_ErrorRegion.CountObj())
		{
			AddError(ho_ErrorRegion, LiangDian);
		}



	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("LightDotDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
	return;

}
void CBattery::TapeDetect()
{
	try
	{


		HObject hoErrorRegion;
		hoErrorRegion.GenEmptyObj();
		if (FALSE == m_tParameters.tTapeParam.bUseDetect)
		{
			return;
		}
		HObject ho_ImageReduced, ho_Region, ho_Connections, ho_FillUp;
		HTuple hvMinDeviation, hv_MinArea, hv_DynThresh;
		hv_DynThresh = m_tParameters.tTapeParam.nTapeDynThresh;
		hv_MinArea = m_tParameters.tTapeParam.dLTMinArea / m_dRowResolution / m_dColResolution;
		hvMinDeviation = m_tParameters.tTapeParam.dDevFactor;
		HObject hoAllRegion, hoLRegion;
		hoAllRegion.GenEmptyObj();
		GetChoiceRegions(hoAllRegion, hoLRegion);
		ConcatObj(hoAllRegion, hoLRegion, &hoAllRegion);
		Union1(hoAllRegion, &hoAllRegion);
		ClosingRectangle1(hoAllRegion, &hoAllRegion, 15, 15);
		Connection(hoAllRegion, &ho_Connections);
		SelectShape(ho_Connections, &ho_Connections, "area", "and", hv_MinArea, INT_MAX);
		FillUp(ho_Connections, &ho_FillUp);
		Connection(ho_FillUp, &ho_Connections);
		SelectGray(ho_Connections, m_hoAdaptedImage, &hoErrorRegion, "deviation", "and", hvMinDeviation, INT_MAX);
		if (hoErrorRegion.CountObj() > 0)
		{
			AddError(hoErrorRegion, TuoTan);
			hoErrorRegion.GenEmptyObj();
		}




		HTuple hvThreshMinArea, hvMaxThresh;
		hvThreshMinArea = m_tParameters.tTapeParam.dTapeThreshMinArea / m_dRowResolution / m_dColResolution;
		hvMaxThresh = m_tParameters.tTapeParam.nTapeThresh;

		ReduceDomain(m_hoAdaptedImage, m_hoCoatMid, &ho_ImageReduced);
		Threshold(ho_ImageReduced, &ho_Region, 0, hvMaxThresh);
		ClosingRectangle1(ho_Region, &ho_Region, 15, 15);
		Connection(ho_Region, &ho_Connections);
		SelectShape(ho_Connections, &hoErrorRegion, "area", "and", hvThreshMinArea, INT_MAX);
		if (hoErrorRegion.CountObj() > 0)
		{
			AddError(hoErrorRegion, TuoTan);
			hoErrorRegion.GenEmptyObj();
		}

	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("TapeDetect 函数出现异常:%s_%s", e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}


}

BOOL CBattery::JiaoDai()
{

	if (m_tParameters.tJiaoDaiParam.bUseDetect == FALSE)
	{
		return TRUE;
	}

	HTuple hv_dDarkThresh, hv_dLightThresh;
	HTuple hv_MinArea;
	hv_dDarkThresh = m_tParameters.tJiaoDaiParam.nDarkThresh;
	hv_dLightThresh = m_tParameters.tJiaoDaiParam.nLightThresh;
	hv_MinArea = m_tParameters.tJiaoDaiParam.dMinArea / m_dRowResolution / m_dRowResolution;

	HObject hoImageReduced, hoRegion, hoConnectionRegions, hoFillUp;
	HObject hoClosinRegion, hoErrorRegions;
	HObject hoLightJiaoDai, hoDarkJiaoDai;
	hoLightJiaoDai.GenEmptyObj();
	hoDarkJiaoDai.GenEmptyObj();
	hoErrorRegions.GenEmptyObj();
	ReduceDomain(m_hoAdaptedImage, m_hoCoatMid, &hoImageReduced);
	Threshold(hoImageReduced, &hoRegion, MIN(hv_dDarkThresh.I(), hv_dLightThresh.I()), MAX(hv_dDarkThresh.I(), hv_dLightThresh.I()));
	Connection(hoRegion, &hoRegion);
	SelectShape(hoRegion, &hoConnectionRegions, "area", "and", 150, INT_MAX);
	Union1(hoConnectionRegions, &hoConnectionRegions);
	FillUp(hoConnectionRegions, &hoFillUp);
	ClosingRectangle1(hoFillUp, &hoClosinRegion, 31, 71);
	Connection(hoClosinRegion, &hoConnectionRegions);
	SelectShape(hoConnectionRegions, &hoDarkJiaoDai, "area", "and", hv_MinArea, INT_MAX);
	ConcatObj(hoErrorRegions, hoDarkJiaoDai, &hoErrorRegions);
	if (hoErrorRegions.CountObj() > 0)
	{
		AddError(hoErrorRegions, JieDai);
		return FALSE;
	}
	return TRUE;




}

void CBattery::FoilDetect()
{
	/*HObject hoErrorRegion;
	hoErrorRegion.GenEmptyObj();
	if (FALSE==m_tParameters.tFoilParam.bUseDetect)
	{
		return;
	}
	HObject ho_ImageReduced,ho_Region,ho_Connections,ho_FillUp;
	HTuple hv_MaxDarkThresh,hv_MinDarkArea;
	hv_MaxDarkThresh=m_tParameters.tFoilParam.nMaxDarkThresh;
	hv_MinDarkArea=m_tParameters.tFoilParam.dMinDarkArea/m_dRowResolution/m_dColResolution;
	HObject hoFoil,hoDilationSmallFoil,hoDilationBigFoil,ho_InFoil,ho_OutFoil;
	ErosionRectangle1(m_tSeparateOut.ho_Foil,&ho_InFoil,5,1);
	Union1(ho_InFoil,&hoFoil);
	ReduceDomain(m_hoAdaptedImage,hoFoil,&ho_ImageReduced);
	Threshold(ho_ImageReduced,&ho_Region,0,hv_MaxDarkThresh);
	ClosingRectangle1(ho_Region,&ho_Region,3,3);
	Connection(ho_Region,&ho_Connections);
	SelectShape(ho_Connections,&hoErrorRegion,"area","and",hv_MinDarkArea,INT_MAX);

	if (hoErrorRegion.CountObj()>0)
	{
		AddError(hoErrorRegion,JiErQueXian,1);
	}*/



}


BOOL CBattery::AddCalssfiyError(HObject ho_ErrorObject, e_ErrorType nErrorCode, int nType)
{
	BOOL bRet = FALSE;
	HObject hoSingleLane, ho_InterErrorRegion;
	Union1(ho_ErrorObject, &ho_ErrorObject);
	HObject hoDetectErrorRegion;
	hoDetectErrorRegion.GenEmptyObj();
	DilationRectangle1(m_ptDetectOut->ho_ErrorRegion, &hoDetectErrorRegion, 11, 11);
	Difference(ho_ErrorObject, hoDetectErrorRegion, &ho_ErrorObject);


	HObject hoDestObject;
	int nStartIndex = 0;
	hoDestObject = m_tSeparateOut.ho_Coat;



	for (int i = 0; i < hoDestObject.CountObj(); i++)
	{
		SelectObj(hoDestObject, &hoSingleLane, i + 1);
		Intersection(hoSingleLane, ho_ErrorObject, &ho_InterErrorRegion);
		Connection(ho_InterErrorRegion, &ho_InterErrorRegion);
		if (ho_InterErrorRegion.CountObj() > 0)
		{
			HTuple hv_Area, hv_SortIndex, hv_ObjectIndex, hv_DescendIndex;
			AreaCenter(ho_InterErrorRegion, &hv_Area, NULL, NULL);
			TupleSortIndex(hv_Area, &hv_SortIndex);
			TupleInverse(hv_SortIndex, &hv_DescendIndex);

			int nChoiceErrorRightIndex = MIN(5, hv_DescendIndex.Length() - 1);
			TupleSelectRange(hv_DescendIndex, 0, nChoiceErrorRightIndex, &hv_ObjectIndex);
			SelectObj(ho_InterErrorRegion, &ho_InterErrorRegion, hv_ObjectIndex + 1);
			int nChoiceErroNum = nChoiceErrorRightIndex + 1;
			HTuple hv_BelongTtip, hv_ErrorIndex;

			//赋值输出结构体
			m_ptDetectOut->hv_Flag[0] = FALSE;

			if (nType == 2)
			{
				TupleGenConst(nChoiceErroNum, MIN(i / 2 + nStartIndex, 3), &hv_BelongTtip);
			}
			else if (nType == 1)
			{
				TupleGenConst(nChoiceErroNum, i + nStartIndex, &hv_BelongTtip);
			}
			else
			{
				TupleGenConst(nChoiceErroNum, MIN(i, 7), &hv_BelongTtip);
			}
			TupleGenConst(nChoiceErroNum, nErrorCode, &hv_ErrorIndex);
			TupleConcat(m_ptDetectOut->hv_ErrorBelongStrip, hv_BelongTtip, &m_ptDetectOut->hv_ErrorBelongStrip);
			TupleConcat(m_ptDetectOut->hv_ErrorType, hv_ErrorIndex, &m_ptDetectOut->hv_ErrorType);
			TupleConcat(m_ptDetectOut->hv_ErrorLevel, hv_ErrorIndex, &m_ptDetectOut->hv_ErrorLevel);
			ConcatObj(m_ptDetectOut->ho_ErrorRegion, ho_InterErrorRegion, &m_ptDetectOut->ho_ErrorRegion);
			bRet = TRUE;
		}
	}





	//{
	//	Connection(ho_ErrorObject,&ho_InterErrorRegion);
	//	if (ho_InterErrorRegion.CountObj()>0)
	//	{
	//		HTuple hv_Area,hv_SortIndex,hv_ObjectIndex,hv_DescendIndex;
	//		AreaCenter(ho_InterErrorRegion,&hv_Area,NULL,NULL);
	//		TupleSortIndex(hv_Area,&hv_SortIndex);
	//		TupleInverse(hv_SortIndex,&hv_DescendIndex);

	//		int nChoiceErrorRightIndex=MIN(5,hv_DescendIndex.Length()-1);
	//		TupleSelectRange(hv_DescendIndex,0,nChoiceErrorRightIndex,&hv_ObjectIndex);
	//		SelectObj(ho_InterErrorRegion,&ho_InterErrorRegion,hv_ObjectIndex+1);
	//		int nChoiceErroNum=nChoiceErrorRightIndex+1;
	//		HTuple hv_BelongTtip,hv_ErrorIndex;
	//		//赋值输出结构体
	//		m_ptDetectOut->hv_Flag[0] = FALSE;
	//		TupleGenConst(nChoiceErroNum,0,&hv_BelongTtip);
	//		TupleGenConst(nChoiceErroNum,nErrorCode,&hv_ErrorIndex);
	//		TupleConcat(m_ptDetectOut->hv_ErrorBelongStrip,hv_BelongTtip,&m_ptDetectOut->hv_ErrorBelongStrip);
	//		TupleConcat(m_ptDetectOut->hv_ErrorLevel,hv_BelongTtip,&m_ptDetectOut->hv_ErrorLevel);
	//		TupleConcat(m_ptDetectOut->hv_ErrorType,hv_ErrorIndex,&m_ptDetectOut->hv_ErrorType);
	//		ConcatObj(m_ptDetectOut->ho_ErrorRegion, ho_InterErrorRegion, &m_ptDetectOut->ho_ErrorRegion);
	//		bRet = TRUE;
	//	}
	//}
	return bRet;
}
BOOL CBattery::AddError(HObject ho_ErrorObject, e_ErrorType nErrorCode, int nType)
{
	BOOL bRet = FALSE;
	HObject hoSingleLane, ho_InterErrorRegion;
	Union1(ho_ErrorObject, &ho_ErrorObject);

	if (m_tInit.nCoatType == M_TwoCam_Intermission)
	{

		if (nErrorCode == TiaoHen || nErrorCode == TuoTan || nErrorCode == HeiDian || nErrorCode == QiPao)
		{
			HObject hoDiffObjet;
			hoDiffObjet.GenEmptyObj();
			DilationRectangle1(m_tSeparateOut.ho_ExtractCaoWeiRegions, &hoDiffObjet, 5 / m_dColResolution, 6 / m_dRowResolution);
			Difference(ho_ErrorObject, m_tBlackRegion.hoYaHenPingBiRegion, &ho_ErrorObject);
			Difference(ho_ErrorObject, hoDiffObjet, &ho_ErrorObject);
		}


	}



	if (m_tParameters.tPretreatParam.bEnbleUseClassyfy)
	{
		if (nErrorCode != LouJinShu && nErrorCode != JieDai&& nErrorCode != TuoWei)
		{
			ConcatObj(m_hoTempErrorRegion, ho_ErrorObject, &m_hoTempErrorRegion);
			return TRUE;
		}

	}




	HObject hoDestObject;
	int nStartIndex = 0;
	hoDestObject = m_tSeparateOut.ho_Coat;
	for (int i = 0; i < hoDestObject.CountObj(); i++)
	{
		SelectObj(hoDestObject, &hoSingleLane, i + 1);
		Intersection(hoSingleLane, ho_ErrorObject, &ho_InterErrorRegion);
		Connection(ho_InterErrorRegion, &ho_InterErrorRegion);
		if (ho_InterErrorRegion.CountObj() > 0)
		{
			HTuple hv_Area, hv_SortIndex, hv_ObjectIndex, hv_DescendIndex;
			AreaCenter(ho_InterErrorRegion, &hv_Area, NULL, NULL);
			TupleSortIndex(hv_Area, &hv_SortIndex);
			TupleInverse(hv_SortIndex, &hv_DescendIndex);

			int nChoiceErrorRightIndex = MIN(5, hv_DescendIndex.Length() - 1);
			TupleSelectRange(hv_DescendIndex, 0, nChoiceErrorRightIndex, &hv_ObjectIndex);
			SelectObj(ho_InterErrorRegion, &ho_InterErrorRegion, hv_ObjectIndex + 1);
			int nChoiceErroNum = nChoiceErrorRightIndex + 1;
			HTuple hv_BelongTtip, hv_ErrorIndex;

			//赋值输出结构体
			m_ptDetectOut->hv_Flag[0] = FALSE;
			TupleGenConst(nChoiceErroNum, i, &hv_BelongTtip);
			TupleGenConst(nChoiceErroNum, nErrorCode, &hv_ErrorIndex);
			TupleConcat(m_ptDetectOut->hv_ErrorBelongStrip, hv_BelongTtip, &m_ptDetectOut->hv_ErrorBelongStrip);
			TupleConcat(m_ptDetectOut->hv_ErrorType, hv_ErrorIndex, &m_ptDetectOut->hv_ErrorType);
			TupleConcat(m_ptDetectOut->hv_ErrorLevel, hv_ErrorIndex, &m_ptDetectOut->hv_ErrorLevel);
			ConcatObj(m_ptDetectOut->ho_ErrorRegion, ho_InterErrorRegion, &m_ptDetectOut->ho_ErrorRegion);
			bRet = TRUE;
		}
	}





	//{
	//	Connection(ho_ErrorObject,&ho_InterErrorRegion);
	//	if (ho_InterErrorRegion.CountObj()>0)
	//	{
	//		HTuple hv_Area,hv_SortIndex,hv_ObjectIndex,hv_DescendIndex;
	//		AreaCenter(ho_InterErrorRegion,&hv_Area,NULL,NULL);
	//		TupleSortIndex(hv_Area,&hv_SortIndex);
	//		TupleInverse(hv_SortIndex,&hv_DescendIndex);

	//		int nChoiceErrorRightIndex=MIN(5,hv_DescendIndex.Length()-1);
	//		TupleSelectRange(hv_DescendIndex,0,nChoiceErrorRightIndex,&hv_ObjectIndex);
	//		SelectObj(ho_InterErrorRegion,&ho_InterErrorRegion,hv_ObjectIndex+1);
	//		int nChoiceErroNum=nChoiceErrorRightIndex+1;
	//		HTuple hv_BelongTtip,hv_ErrorIndex;
	//		//赋值输出结构体
	//		m_ptDetectOut->hv_Flag[0] = FALSE;
	//		TupleGenConst(nChoiceErroNum,0,&hv_BelongTtip);
	//		TupleGenConst(nChoiceErroNum,nErrorCode,&hv_ErrorIndex);
	//		TupleConcat(m_ptDetectOut->hv_ErrorBelongStrip,hv_BelongTtip,&m_ptDetectOut->hv_ErrorBelongStrip);
	//		TupleConcat(m_ptDetectOut->hv_ErrorLevel,hv_BelongTtip,&m_ptDetectOut->hv_ErrorLevel);
	//		TupleConcat(m_ptDetectOut->hv_ErrorType,hv_ErrorIndex,&m_ptDetectOut->hv_ErrorType);
	//		ConcatObj(m_ptDetectOut->ho_ErrorRegion, ho_InterErrorRegion, &m_ptDetectOut->ho_ErrorRegion);
	//		bRet = TRUE;
	//	}
	//}
	return bRet;
}
int  CBattery::Detect(DETECTINPUT &tIn, DETECTOUTPUT *tOut, int nBreakType)
{

	m_ptDetectOut = tOut;
	m_ptDetectIn = &tIn;
	InitDetect();
	CString strTime;
	HTuple hv_TimeStart, hv_TimeEnd;
	CountSeconds(&hv_TimeStart);
	if (FALSE == Separate())//如果分割区域就有问题 直接返回FALSE 尺寸检测也在这里
	{
		return TRUE;
	}


	GetDetetcRegion();
	try
	{
		GetDynRegion(m_hoAdaptedImage);
		//if (FALSE==JiaoDai())//胶带检测
		//{
		//	return TRUE;
		//}
		JiaoDai();
		LeakFoilDetect();
		AT9Detect();
		TuoWeiDetect();
		BubbleDetect();
		BlackLineDetect();
		TapeDetect();
		LightDotDetect();
		BlackDotDetect();
		FoilDetect();
		if (m_tParameters.tPretreatParam.bEnbleUseClassyfy)
		{

			HObject hoObjected;
			HTuple hv_Classes, hv_Confidence;
			Connection(m_hoTempErrorRegion, &m_hoTempErrorRegion);
			for (int i = 0; i < m_hoTempErrorRegion.CountObj(); i++)
			{
				hv_Classes.Clear();
				hv_Confidence.Clear();
				SelectObj(m_hoTempErrorRegion, &hoObjected, i + 1);
				classify(hoObjected, m_hoAdaptedImage, m_tMLPParam.hvMLPHandle, m_tMLPParam.hvGrayFeature, m_tMLPParam.hvRegionFeature, &hv_Classes, &hv_Confidence);
				if (hv_Classes != OK_C && hv_Confidence > m_tParameters.tPretreatParam.dMLPSongjindu)
				{
					BOOL bEnable = FALSE;
					switch (hv_Classes.I())
					{
					case LOUBO_C:
						if (m_tParameters.tLeakFoilParam.bUseDetect)
						{
							bEnable = TRUE;
						}
						break;
					case TUOTAN_C:
						if (m_tParameters.tTapeParam.bUseDetect)
						{
							bEnable = TRUE;
						}
						break;
					case TIAOHEN_C:
						if (m_tParameters.tDarkLineParam.bUseDetect)
						{
							bEnable = TRUE;
						}
						break;
					case QIPAO_C:
						if (m_tParameters.tBubbleParam.bUseDetect)
						{
							bEnable = TRUE;
						}
						break;
					case BAIDIAN_C:
						if (m_tParameters.tLightDotParam.bUseDetect)
						{
							bEnable = TRUE;
						}
						break;
					case HEIDIAN_C:
						if (m_tParameters.tDarkDotParam.bUseDetect)
						{
							bEnable = TRUE;

						}
						break;
					}

					if (bEnable)
					{
						AddCalssfiyError(hoObjected, (e_ErrorType)hv_Classes.I());
					}
				}



			}




		}


	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("相机%d的函数%s 检测异常：%s 异常信息：%s", m_tInit.nGrabberSN, e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

	}



	return TRUE;
}
void CBattery::DIYErrorOut()
{
	try
	{


		CString strAppName, strValue;
		CString strSubKey;
		HTuple hv_SingleFeature;

		for (int i = 0; i < m_tParameters.tFeatureSelect.nGroupNum; i++)
		{

			BOOL bGrayFeature = FALSE;
			BOOL bShapeFearture = FALSE;
			HTuple hv_FeatureName, hv_FeatureMin, hv_FeatureMax;
			HTuple hv_ShapeName, hv_ShapeMin, hv_ShapeMax;
			HTuple hv_Index;
			SingleErrorGroup tSingleErrorGroup;
			tSingleErrorGroup = m_tParameters.tFeatureSelect.Vec_FSelects.at(i);//获取单组的缺陷的筛选项目
			tSingleErrorGroup.strErrorName;//缺陷类型
			TupleFind(m_hvErrorName, (HTuple)tSingleErrorGroup.strErrorName, &hv_Index);
			int n = hv_Index.I();
			if (hv_Index != -1 && hv_Index.Length()>0)
			{
				for (int j = 0; j < tSingleErrorGroup.nSingleGroupNum; j = j + 3)
				{
					SingleFeature tSingleFeature = tSingleErrorGroup.Vec_tSingleGroupFeatures.at(j / 3);

					HTuple hv_FeatureIndex;
					TupleFind(m_hvStrFeature, (HTuple)tSingleFeature.StrSingleFeatureName, &hv_FeatureIndex);
					if (G_min == hv_FeatureIndex || G_max == hv_FeatureIndex || G_mean == hv_FeatureIndex || G_deviation == hv_FeatureIndex)
					{
						bGrayFeature = TRUE;
						hv_FeatureName.Append((HTuple)STR_Feature[hv_FeatureIndex.I()]);
						hv_FeatureMin.Append(tSingleFeature.dMin);
						hv_FeatureMax.Append(tSingleFeature.dMax);

					}
					else
					{
						bShapeFearture = TRUE;
						hv_ShapeName.Append((HTuple)STR_Feature[hv_FeatureIndex.I()]);
						hv_ShapeMin.Append(tSingleFeature.dMin);
						hv_ShapeMax.Append(tSingleFeature.dMax);
					}
				}
				HObject hoLightRegion, hoDarkRegion;
				HObject hoSelectRegion;
				hoSelectRegion.GenEmptyObj();

				GetChoiceRegions(hoLightRegion, hoDarkRegion);
				ConcatObj(hoSelectRegion, hoLightRegion, &hoSelectRegion);
				ConcatObj(hoSelectRegion, hoDarkRegion, &hoSelectRegion);
				if (bGrayFeature == TRUE)
				{
					SelectGray(hoSelectRegion, m_hoAdaptedImage, &hoSelectRegion, hv_FeatureName, "and", hv_FeatureMin, hv_FeatureMax);

				}
				if (bShapeFearture == TRUE)
				{

					SelectShape(hoSelectRegion, &hoSelectRegion, hv_ShapeName, "and", hv_ShapeMin, hv_ShapeMax);

				}
				AddError(hoSelectRegion, (e_ErrorType)hv_Index.I());
			}





		}




	}
	catch (HException& e)
	{

	}


}
int  CBattery::DisplaySetDialog(DETECTINPUT & tIn)
{
	try
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgAlgorithm dlg;
		dlg.m_pRealDetect = this;
		dlg.m_pDetect->DoSomething("COPYOBJECTS", this, NULL);
		dlg.m_DetectInput = tIn;
		dlg.DoModal();
	}
	catch (HException e)
	{
		CString strException;
		strException.Format("出现异常\r\n异常信息%s  [ %s() ]", e.ErrorMessage().Text(), e.ProcName().Text());
		((DHInterface*)m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
	return TRUE;
}

void CBattery::ReadDeFaultParameters()
{

	//m_strParamFile.Format("%s\\缺陷检测参数0.ini",m_strModelPath);

	//m_tParameters.tPretreatParam.dBorderWidth=GetPrivateProfileDouble("tPretreatParam", "dBorderWidth",3, m_strParamFile);
	//m_tParameters.tPretreatParam.dScaleFactor = GetPrivateProfileDouble("tPretreatParam", "dScaleFactor", 1, m_strParamFile);

	//m_tParameters.tPretreatParam.dMidLightCof=GetPrivateProfileDouble("tPretreatParam", "dMidLightCof",0.2, m_strParamFile);
	//m_tParameters.tPretreatParam.dMidDarkCof=GetPrivateProfileDouble("tPretreatParam", "dMidDarkCof",0.2, m_strParamFile);
	//m_tParameters.tPretreatParam.dBorderLightCof=GetPrivateProfileDouble("tPretreatParam", "dBorderLightCof",0.2, m_strParamFile);
	//m_tParameters.tPretreatParam.dBorderDarkCof=GetPrivateProfileDouble("tPretreatParam", "dBorderDarkCof",0.2, m_strParamFile);


	//m_tParameters.tPretreatParam.bEnableUseGrayValue=GetPrivateProfileInt("tPretreatParam", "bEnableUseGrayValue",1, m_strParamFile);
	//m_tParameters.tPretreatParam.nDstGrayValue=GetPrivateProfileInt("tPretreatParam", "nDstGrayValue",80, m_strParamFile);
	//m_tParameters.tPretreatParam.nSourceGrayValue=GetPrivateProfileInt("tPretreatParam", "nSourceGrayValue",50, m_strParamFile);


	//m_tParameters.tLeakFoilParam.bUseDetect = GetPrivateProfileInt("tLeakFoilParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tLeakFoilParam.nThresh = GetPrivateProfileInt("tLeakFoilParam", "nThresh", 150, m_strParamFile);
	//m_tParameters.tLeakFoilParam.dMinArea = GetPrivateProfileDouble("tLeakFoilParam", "dMinArea", 0.8, m_strParamFile);
	//m_tParameters.tLeakFoilParam.nDynThresh = GetPrivateProfileInt("tLeakFoilParam", "nDynThresh", 100, m_strParamFile);
	//m_tParameters.tLeakFoilParam.dMinDiameter = GetPrivateProfileDouble("tLeakFoilParam", "dMinDiameter", 0.5, m_strParamFile);
	//m_tParameters.tLeakFoilParam.dMaxDiameter = GetPrivateProfileDouble("tLeakFoilParam", "dMaxDiameter", 90, m_strParamFile);
	//m_tParameters.tLeakFoilParam.dDeviation = GetPrivateProfileDouble("tLeakFoilParam", "dDeviation", 12, m_strParamFile);


	//m_tParameters.tBubbleParam.bUseDetect = GetPrivateProfileInt("tBubbleParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tBubbleParam.nDynThresh = GetPrivateProfileDouble("tBubbleParam", "nDynThresh", 15, m_strParamFile);
	//m_tParameters.tBubbleParam.dMinDiameter = GetPrivateProfileDouble("tBubbleParam", "dMinDiameter", 1, m_strParamFile);
	//m_tParameters.tBubbleParam.dMinCircularity= GetPrivateProfileDouble("tBubbleParam", "dMinCircularity", 0.7, m_strParamFile);
	//m_tParameters.tBubbleParam.nInOutGrayDiff = GetPrivateProfileDouble("tBubbleParam", "nInOutGrayDiff", 2, m_strParamFile);



	//m_tParameters.tLightLineParam.bUseDetect = GetPrivateProfileInt("tLightLineParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tLightLineParam.nDynThresh = GetPrivateProfileDouble("tLightLineParam", "nDynThresh", 20, m_strParamFile);
	//m_tParameters.tLightLineParam.dMinWidth = GetPrivateProfileDouble("tLightLineParam", "dMinWidth", 0.5, m_strParamFile);
	//m_tParameters.tLightLineParam.dMinHeight = GetPrivateProfileDouble("tLightLineParam", "dMinHeight", 50, m_strParamFile);
	//m_tParameters.tLightLineParam.dMinLengthWidthRatio = GetPrivateProfileDouble("tLightLineParam", "dMinLengthWidthRatio", 8, m_strParamFile);


	//m_tParameters.tDarkLineParam.bUseDetect = GetPrivateProfileInt("tDarkLineParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tDarkLineParam.nDynThresh = GetPrivateProfileDouble("tDarkLineParam", "nDynThresh", 20, m_strParamFile);
	//m_tParameters.tDarkLineParam.dMinWidth = GetPrivateProfileDouble("tDarkLineParam", "dMinWidth", 0.5, m_strParamFile);
	//m_tParameters.tDarkLineParam.dMinHeight = GetPrivateProfileDouble("tDarkLineParam", "dMinHeight", 50, m_strParamFile);
	//m_tParameters.tDarkLineParam.dMinLengthWidthRatio = GetPrivateProfileDouble("tDarkLineParam", "dMinLengthWidthRatio", 8, m_strParamFile);


	//m_tParameters.tLightDotParam.bUseDetect = GetPrivateProfileInt("tLightDotParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tLightDotParam.nMinThresh = GetPrivateProfileInt("tLightDotParam", "nMinThresh", 130, m_strParamFile);
	//m_tParameters.tLightDotParam.nMaxThresh = GetPrivateProfileInt("tLightDotParam", "nMaxThresh", 200, m_strParamFile);
	//m_tParameters.tLightDotParam.dMinWidth = GetPrivateProfileDouble("tLightDotParam", "dMinWidth", 1, m_strParamFile);
	//m_tParameters.tLightDotParam.dMinHeight = GetPrivateProfileDouble("tLightDotParam", "dMinHeight", 1, m_strParamFile);
	//m_tParameters.tLightDotParam.dMinArea = GetPrivateProfileDouble("tLightDotParam", "dMinArea", 1, m_strParamFile);
	//m_tParameters.tLightDotParam.dMaxArea = GetPrivateProfileDouble("tLightDotParam", "dMaxArea", 36000, m_strParamFile);

	//m_tParameters.tLightDotParam.nMinDeviation = GetPrivateProfileInt("tLightDotParam", "nMinDeviation", 20, m_strParamFile);
	//m_tParameters.tLightDotParam.nMinGrayRange = GetPrivateProfileInt("tLightDotParam", "nMinGrayRange", 40, m_strParamFile);



	//m_tParameters.tDarkDotParam.bUseDetect = GetPrivateProfileInt("tDarkDotParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tDarkDotParam.nMaxThresh = GetPrivateProfileInt("tDarkDotParam", "nMaxThresh", 30, m_strParamFile);
	//m_tParameters.tDarkDotParam.dMinWidth = GetPrivateProfileDouble("tDarkDotParam", "dMinWidth", 1, m_strParamFile);
	//m_tParameters.tDarkDotParam.dMinHeight = GetPrivateProfileDouble("tDarkDotParam", "dMinHeight", 1, m_strParamFile);
	//m_tParameters.tDarkDotParam.dMinArea = GetPrivateProfileDouble("tDarkDotParam", "dMinArea", 0.8, m_strParamFile);
	//m_tParameters.tDarkDotParam.dMaxArea = GetPrivateProfileDouble("tDarkDotParam", "dMaxArea", 36000, m_strParamFile);//按照300mm*120mm的视野计算


	//m_tParameters.tTapeParam.bUseDetect = GetPrivateProfileInt("tTapeParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tTapeParam.nMaxThresh = GetPrivateProfileInt("tTapeParam", "nMaxThresh", 12, m_strParamFile);//偏差因子
	//m_tParameters.tTapeParam.dMinArea = GetPrivateProfileDouble("tTapeParam", "dMinArea", 0.8, m_strParamFile);

	//m_tParameters.tTapeParam.nLTMinThresh = GetPrivateProfileInt("tTapeParam", "nLTMinThresh", 0, m_strParamFile);
	//m_tParameters.tTapeParam.nLTMaxThresh= GetPrivateProfileInt("tTapeParam", "nLTMaxThresh", 45, m_strParamFile);
	//m_tParameters.tTapeParam.dLTMinArea = GetPrivateProfileDouble("tTapeParam", "dLTMinArea", 5, m_strParamFile);

	//m_tParameters.tTuoWeiParam.bUseDetect= GetPrivateProfileInt("tTuoWeiParam", "bUseDetect", 0, m_strParamFile);
	//m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh= GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMinThresh", 0, m_strParamFile);
	//m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh = GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMaxThresh", 80, m_strParamFile);
	//m_tParameters.tTuoWeiParam.dJXTapeLTMinArea= GetPrivateProfileDouble("tTuoWeiParam", "dJXTapeLTMinArea", 1, m_strParamFile);
	//m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight= GetPrivateProfileDouble("tTuoWeiParam", "dMaxHeight", 3, m_strParamFile);


	//m_tParameters.tAT9Param.bUseDetect= GetPrivateProfileInt("tAT9Param", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tAT9Param.nAT9LMinThresh= GetPrivateProfileInt("tAT9Param", "nAT9LMinThresh", 220, m_strParamFile);
	//m_tParameters.tAT9Param.dAT9LMinWidth = GetPrivateProfileDouble("tAT9Param", "dAT9LMinWidth", 0.2, m_strParamFile);
	//m_tParameters.tAT9Param.dAT9LMinHeight = GetPrivateProfileDouble("tAT9Param", "dAT9LMinHeight", 0.3, m_strParamFile);
	//m_tParameters.tAT9Param.nAT9DarkMaxThresh= GetPrivateProfileInt("tAT9Param", "nAT9DarkMaxThresh", 60, m_strParamFile);
	//m_tParameters.tAT9Param.dAT9DarkMinWidth = GetPrivateProfileDouble("tAT9Param", "dAT9DarkMinWidth", 0.2, m_strParamFile);
	//m_tParameters.tAT9Param.dAT9DarkMinArea = GetPrivateProfileDouble("tAT9Param", "dAT9DarkMinArea", 1, m_strParamFile);


	//m_tParameters.tFoilParam.bUseDetect = GetPrivateProfileInt("tFoilParam", "bUseDetect", 0, m_strParamFile);
	//m_tParameters.tFoilParam.nLightThresh = GetPrivateProfileInt("tFoilParam", "nLightThresh", 200, m_strParamFile);
	//m_tParameters.tFoilParam.dMinLightArea = GetPrivateProfileDouble("tFoilParam", "dMinLightArea", 20, m_strParamFile);
	//m_tParameters.tFoilParam.dFoilLightMinWidth = GetPrivateProfileDouble("tFoilParam", "dFoilLightMinWidth", 2, m_strParamFile);




	//m_tParameters.tJiaoDaiParam.bUseDetect = GetPrivateProfileInt("tJiaoDaiParam", "bUseDetect", 1, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.nDarkThresh = GetPrivateProfileInt("tJiaoDaiParam", "nDarkThresh", 50, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.dDarkMinArea = GetPrivateProfileDouble("tJiaoDaiParam", "dDarkMinArea", 200, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.nLightThresh = GetPrivateProfileInt("tJiaoDaiParam", "nLightThresh", 180, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.dLightMinArea = GetPrivateProfileDouble("tJiaoDaiParam", "dLightMinArea", 200, m_strParamFile);

	//m_tParameters.tJiaoDaiParam.dDarkMinWidth = GetPrivateProfileDouble("tJiaoDaiParam", "dDarkMinWidth", 10, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.dLightMinWidth = GetPrivateProfileDouble("tJiaoDaiParam", "dLightMinWidth", 10, m_strParamFile);

	//m_tParameters.tJiaoDaiParam.dDarkMinHeight = GetPrivateProfileDouble("tJiaoDaiParam", "dDarkMinHeight", 20, m_strParamFile);
	//m_tParameters.tJiaoDaiParam.dLightMinHeight = GetPrivateProfileDouble("tJiaoDaiParam", "dLightMinHeight", 20, m_strParamFile);

}
BOOL CBattery::ReadParameters()
{


	m_strParamFile.Format("%s\\缺陷检测参数.ini", m_strModelPath);

	//瑕疵检测总开关
	m_tParameters.tUseDefectMasterSwitch = GetPrivateProfileDouble("tUseDefectMasterSwitch", "tUseDefectMasterSwitch", 0, m_strParamFile);

	m_tParameters.tPretreatParam.dYaHenPingBiKuanDu = GetPrivateProfileDouble("tPretreatParam", "dYaHenPingBiKuanDu", 10, m_strParamFile);
	m_tParameters.tPretreatParam.dRightBorderWidth = GetPrivateProfileDouble("tPretreatParam", "dRightBorderWidth", 5, m_strParamFile);
	m_tParameters.tPretreatParam.dBorderWidth = GetPrivateProfileDouble("tPretreatParam", "dBorderWidth", 5, m_strParamFile);
	m_tParameters.tPretreatParam.dScaleFactor = GetPrivateProfileDouble("tPretreatParam", "dScaleFactor", 1, m_strParamFile);

	m_tParameters.tPretreatParam.dMidLightCof = GetPrivateProfileDouble("tPretreatParam", "dMidLightCof", 0.2, m_strParamFile);
	m_tParameters.tPretreatParam.dMidDarkCof = GetPrivateProfileDouble("tPretreatParam", "dMidDarkCof", 0.2, m_strParamFile);
	m_tParameters.tPretreatParam.dBorderLightCof = GetPrivateProfileDouble("tPretreatParam", "dBorderLightCof", 0.2, m_strParamFile);
	m_tParameters.tPretreatParam.dBorderDarkCof = GetPrivateProfileDouble("tPretreatParam", "dBorderDarkCof", 0.2, m_strParamFile);
	m_tParameters.tPretreatParam.dMLPSongjindu = GetPrivateProfileDouble("tPretreatParam", "dMLPSongjindu", 0.7, m_strParamFile);
	m_tParameters.tPretreatParam.bEnbleUseClassyfy = GetPrivateProfileInt("tPretreatParam", "bEnbleUseClassyfy", 0, m_strParamFile);


	m_tParameters.tLeakFoilParam.bUseDetect = GetPrivateProfileInt("tLeakFoilParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tLeakFoilParam.nThresh = GetPrivateProfileInt("tLeakFoilParam", "nThresh", 100, m_strParamFile);
	m_tParameters.tLeakFoilParam.dMinArea = GetPrivateProfileDouble("tLeakFoilParam", "dMinArea", 2, m_strParamFile);


	m_tParameters.tBubbleParam.bUseDetect = GetPrivateProfileInt("tBubbleParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tBubbleParam.nDynThresh = GetPrivateProfileDouble("tBubbleParam", "nDynThresh", 12, m_strParamFile);
	m_tParameters.tBubbleParam.dMinArea = GetPrivateProfileInt("tBubbleParam", "dMinArea", 3, m_strParamFile);
	m_tParameters.tBubbleParam.dMinCircularity = GetPrivateProfileDouble("tBubbleParam", "dMinCircularity", 0.5, m_strParamFile);




	m_tParameters.tDarkLineParam.bUseDetect = GetPrivateProfileInt("tDarkLineParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tDarkLineParam.nDynThresh = GetPrivateProfileDouble("tDarkLineParam", "nDynThresh", 12, m_strParamFile);
	m_tParameters.tDarkLineParam.dMinWidth = GetPrivateProfileDouble("tDarkLineParam", "dMinWidth", 0.5, m_strParamFile);
	m_tParameters.tDarkLineParam.dMinLengthWidthRatio = GetPrivateProfileDouble("tDarkLineParam", "dMinLengthWidthRatio", 5, m_strParamFile);


	m_tParameters.tLightDotParam.bUseDetect = GetPrivateProfileInt("tLightDotParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tLightDotParam.nLightDotDynThresh = GetPrivateProfileInt("tLightDotParam", "nLightDotDynThresh", 40, m_strParamFile);
	m_tParameters.tLightDotParam.dMinArea = GetPrivateProfileDouble("tLightDotParam", "dMinArea", 1, m_strParamFile);


	m_tParameters.tDarkDotParam.bUseDetect = GetPrivateProfileInt("tDarkDotParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tDarkDotParam.nDarkDotDynThresh = GetPrivateProfileInt("tDarkDotParam", "nDarkDotDynThresh", 20, m_strParamFile);
	m_tParameters.tDarkDotParam.dMinArea = GetPrivateProfileDouble("tDarkDotParam", "dMinArea", 2, m_strParamFile);

	m_tParameters.tTapeParam.bUseDetect = GetPrivateProfileInt("tTapeParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tTapeParam.nTapeDynThresh = GetPrivateProfileInt("tTapeParam", "nTapeDynThresh", 20, m_strParamFile);
	m_tParameters.tTapeParam.dDevFactor = GetPrivateProfileDouble("tTapeParam", "dDevFactor", 12, m_strParamFile);
	m_tParameters.tTapeParam.dLTMinArea = GetPrivateProfileDouble("tTapeParam", "dLTMinArea", 5, m_strParamFile);

	m_tParameters.tTapeParam.nTapeThresh = GetPrivateProfileInt("tTapeParam", "nTapeThresh", 50, m_strParamFile);
	m_tParameters.tTapeParam.dTapeThreshMinArea = GetPrivateProfileDouble("tTapeParam", "dTapeThreshMinArea", 5, m_strParamFile);



	m_tParameters.tFoilParam.bUseDetect = GetPrivateProfileInt("tFoilParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tFoilParam.nMaxDarkThresh = GetPrivateProfileInt("tFoilParam", "nMaxDarkThresh", 50, m_strParamFile);
	m_tParameters.tFoilParam.dMinDarkArea = GetPrivateProfileDouble("tFoilParam", "dMinDarkArea", 0.8, m_strParamFile);




	m_tParameters.tJiaoDaiParam.bUseDetect = GetPrivateProfileInt("tJiaoDaiParam", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tJiaoDaiParam.nDarkThresh = GetPrivateProfileInt("tJiaoDaiParam", "nDarkThresh", 50, m_strParamFile);
	m_tParameters.tJiaoDaiParam.dMinArea = GetPrivateProfileDouble("tJiaoDaiParam", "dMinArea", 200, m_strParamFile);
	m_tParameters.tJiaoDaiParam.nLightThresh = GetPrivateProfileInt("tJiaoDaiParam", "nLightThresh", 180, m_strParamFile);




	m_tParameters.tTuoWeiParam.bUseDetect = GetPrivateProfileInt("tTuoWeiParam", "bUseDetect", 0, m_strParamFile);
	if (m_tInit.nProductType == positive)
	{
		m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh = GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMinThresh", 0, m_strParamFile);
		m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh = GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMaxThresh", 80, m_strParamFile);
		m_tParameters.tTuoWeiParam.dJXTapeLTMinArea = GetPrivateProfileDouble("tTuoWeiParam", "dJXTapeLTMinArea", 1, m_strParamFile);
		m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight = GetPrivateProfileDouble("tTuoWeiParam", "dMaxHeight", 3, m_strParamFile);
	}
	else
	{
		m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh = GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMinThresh", 200, m_strParamFile);
		m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh = GetPrivateProfileInt("tTuoWeiParam", "nJXTapeLTMaxThresh", 255, m_strParamFile);
		m_tParameters.tTuoWeiParam.dJXTapeLTMinArea = GetPrivateProfileDouble("tTuoWeiParam", "dJXTapeLTMinArea", 1, m_strParamFile);
		m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight = GetPrivateProfileDouble("tTuoWeiParam", "dMaxHeight", 3, m_strParamFile);
	}






	m_tParameters.tAT9Param.bUseDetect = GetPrivateProfileInt("tAT9Param", "bUseDetect", 1, m_strParamFile);
	m_tParameters.tAT9Param.nAT9LMinThresh = GetPrivateProfileInt("tAT9Param", "nAT9LMinThresh", 220, m_strParamFile);
	m_tParameters.tAT9Param.dAT9LMinWidth = GetPrivateProfileDouble("tAT9Param", "dAT9LMinWidth", 0.2, m_strParamFile);
	m_tParameters.tAT9Param.dAT9LMinHeight = GetPrivateProfileDouble("tAT9Param", "dAT9LMinHeight", 0.2, m_strParamFile);
	m_tParameters.tAT9Param.nAT9DarkMaxThresh = GetPrivateProfileInt("tAT9Param", "nAT9DarkMaxThresh", 60, m_strParamFile);
	m_tParameters.tAT9Param.dAT9DarkMinWidth = GetPrivateProfileDouble("tAT9Param", "dAT9DarkMinWidth", 0.2, m_strParamFile);
	m_tParameters.tAT9Param.dAT9DarkMinHeight = GetPrivateProfileDouble("tAT9Param", "dAT9DarkMinHeight", 0.2, m_strParamFile);


	/////////////////强制开与关//////////////////////
	if (m_tParameters.tUseDefectMasterSwitch == 1)
	{
		m_tParameters.tLeakFoilParam.bUseDetect = 1;
		m_tParameters.tBubbleParam.bUseDetect = 1;
		m_tParameters.tDarkLineParam.bUseDetect = 1;
		m_tParameters.tLightDotParam.bUseDetect = 1;
		m_tParameters.tDarkDotParam.bUseDetect = 1;
		m_tParameters.tTapeParam.bUseDetect = 1;
		m_tParameters.tFoilParam.bUseDetect = 1;
		m_tParameters.tJiaoDaiParam.bUseDetect = 1;
		m_tParameters.tAT9Param.bUseDetect = 1;
		m_tParameters.tTuoWeiParam.bUseDetect = 1;
	}
	/////////////////自定义参数//////////

	CString strPath;
	strPath.Format("%s\\Config.ini", m_strModelPath);
	m_tParameters.tFeatureSelect.nGroupNum = GetPrivateProfileInt("Feature", "FeatureGroupNum", 0, strPath);
	int nGroupNums = m_tParameters.tFeatureSelect.nGroupNum;//一共多少组自定义的缺陷类型
	CString strAppName, strValue;
	CString strSubKey;
	HTuple hv_SingleFeature;
	for (int i = 0; i < nGroupNums; i++)
	{
		strAppName.Format("FeatureGroup%d", i);
		SingleErrorGroup tSingleErrorGroup;
		tSingleErrorGroup.nSingleGroupNum = GetPrivateProfileInt(strAppName, "SingleGroupNum", 0, strPath);
		tSingleErrorGroup.strErrorName = GetPrivateProfileCString(strAppName, "缺陷组名", "面积", strPath);
		for (int j = 0; j < tSingleErrorGroup.nSingleGroupNum; j = j + 3)
		{
			SingleFeature tSingleFeature;
			strSubKey.Format("特征%d", j);
			tSingleFeature.StrSingleFeatureName = GetPrivateProfileCString(strAppName, strSubKey, "面积", strPath);
			strSubKey.Format("MIN%d", j);
			tSingleFeature.dMin = GetPrivateProfileDouble(strAppName, strSubKey, 0, strPath);
			strSubKey.Format("MAX%d", j);
			tSingleFeature.dMax = GetPrivateProfileDouble(strAppName, strSubKey, 9999, strPath);
			tSingleErrorGroup.Vec_tSingleGroupFeatures.push_back(tSingleFeature);
		}
		m_tParameters.tFeatureSelect.Vec_FSelects.push_back(tSingleErrorGroup);
	}

	try
	{
		//CString strModelInfoPath,strTuple;
		//strModelInfoPath.Format("%s\\模板信息",m_strModelPath);
		//CreateMultilevelPath(strModelInfoPath);
		//CString strModelConfig;
		//strModelConfig.Format("%s\\Config.ini",strModelInfoPath);
		///////////
		//m_tParameters.tTempLateInfo.bAotuFindLocate=GetPrivateProfileInt("ModelInfo","bAotuFindLocate",0,strModelConfig);
		//m_tParameters.tTempLateInfo.bUseFirstLengthGRR=GetPrivateProfileInt("ModelInfo","bUseFirstLengthGRR",0,strModelConfig);
		//m_tParameters.tTempLateInfo.nHelpDraw=GetPrivateProfileInt("ModelInfo","nHelpDraw",0,strModelConfig);
		//m_tParameters.tTempLateInfo.nFoilThresh=GetPrivateProfileInt("ModelInfo","nFoilThresh",20,strModelConfig);
		//m_tParameters.tTempLateInfo.dFoilHeight=GetPrivateProfileInt("ModelInfo","dFoilHeight",100,strModelConfig);
		//m_tParameters.tTempLateInfo.dFoilWidth=GetPrivateProfileInt("ModelInfo","dFoilWidth",30,strModelConfig);
		//m_tParameters.tTempLateInfo.dOffset=GetPrivateProfileDouble("ModelInfo","dOffset",1,strModelConfig);
		//m_tParameters.tTempLateInfo.bUseManualSeg=GetPrivateProfileInt("ModelInfo","bUseManualSeg",0,strModelConfig);
		//m_tParameters.tTempLateInfo.bUsePreProcess=GetPrivateProfileInt("ModelInfo","bUsePreProcess",0,strModelConfig);


		//m_tParameters.tTempLateInfo.nHighThresh=GetPrivateProfileInt("ModelInfo","nHighThresh",30,strModelConfig);
		//m_tParameters.tTempLateInfo.nLowThresh=GetPrivateProfileInt("ModelInfo","nLowThresh",0,strModelConfig);
		//m_tParameters.tTempLateInfo.nFitType=GetPrivateProfileInt("ModelInfo","nFitType",0,strModelConfig);
		//m_tParameters.tTempLateInfo.nReferPoint=GetPrivateProfileInt("ModelInfo","nReferPoint",0,strModelConfig);
		//m_tParameters.tTempLateInfo.dJXNearCoatHeight=GetPrivateProfileDouble("ModelInfo","dJXNearCoatHeight",5,strModelConfig);

		//m_tParameters.tTempLateInfo.dNearCoatOffset=GetPrivateProfileDouble("ModelInfo","dNearCoatOffset",0,strModelConfig);


		//m_tParameters.tTempLateInfo.nFirstLengthLocation=GetPrivateProfileInt("ModelInfo","nFirstLengthLocation",1,strModelConfig);
		//m_tParameters.tTempLateInfo.bExistMarkInCoat=GetPrivateProfileInt("ModelInfo","bExistMarkInCoat",0,strModelConfig);

		//m_tParameters.tTempLateInfo.nLineThresh=GetPrivateProfileInt("ModelInfo","nLineThresh",60,strModelConfig);

		//m_tParameters.tTempLateInfo.nBotLineThresh=GetPrivateProfileInt("ModelInfo","nBotLineThresh",20,strModelConfig);
		//m_tParameters.tTempLateInfo.nFirstOrLastPoint=GetPrivateProfileInt("ModelInfo","nFirstOrLastPoint",0,strModelConfig);


		//m_tParameters.tTempLateInfo.dSpotResolution=GetPrivateProfileDouble("ModelInfo","dSpotResolution",0.104,strModelConfig);
		//m_tParameters.tTempLateInfo.nSpotHighThresh=GetPrivateProfileInt("ModelInfo","nSpotHighThresh",100,strModelConfig);
		//m_tParameters.tTempLateInfo.nSpotNum=GetPrivateProfileInt("ModelInfo","nSpotNum",2,strModelConfig);
		//m_tParameters.tTempLateInfo.dSpotSetArea=GetPrivateProfileDouble("ModelInfo","dSpotSetArea",1.5,strModelConfig);

		//try
		//{
		//	strTuple.Format("%s\\检测区域.hobj", strModelInfoPath);
		//	ReadObject(&m_tParameters.tTempLateInfo.hoDetectRegion,(HTuple)strTuple);
		//}catch(HException& e)
		//{
		//	m_tParameters.tTempLateInfo.hoDetectRegion.GenEmptyObj();
		//}






		////MLP分类器
		try
		{
			m_strMLPPath.Format("%s\\Config\\分类器\\%d", GetCurrentAppPath(), m_tInit.nGrabberSN);
			CreateMultilevelPath(m_strMLPPath);

			m_tMLPParam.hvRegionFeature.Clear();
			m_tMLPParam.hvRegionFeature.Append("anisometry");
			m_tMLPParam.hvRegionFeature.Append("width");
			m_tMLPParam.hvRegionFeature.Append("height");
			m_tMLPParam.hvRegionFeature.Append("area");

			m_tMLPParam.hvGrayFeature.Clear();
			m_tMLPParam.hvGrayFeature.Append("mean");
			m_tMLPParam.hvGrayFeature.Append("deviation");
			m_tMLPParam.hvGrayFeature.Append("max");
			m_tMLPParam.hvGrayFeature.Append("min");

			m_tMLPParam.hvFeatureName.Clear();
			m_tMLPParam.hvFeatureName.Append(m_tMLPParam.hvRegionFeature);
			m_tMLPParam.hvFeatureName.Append(m_tMLPParam.hvGrayFeature);
			TupleLength(m_tMLPParam.hvFeatureName, &m_tMLPParam.hvFeatureLength);





			CString strMlpName;
			strMlpName.Format("%s\\DHMLPSurfaceAlg.gmc", m_strMLPPath);
			ReadClassMlp((HTuple)strMlpName, &m_tMLPParam.hvMLPHandle);



			CString  strMLPParam;
			strMLPParam.Format("%s\\MLPConfig.ini", m_strMLPPath);
			m_tMLPParam.nLeftColumn = GetPrivateProfileInt("TestCoordinate", "LeftColumn", 1300, strMLPParam);
			m_tMLPParam.nRightColumn = GetPrivateProfileInt("TestCoordinate", "nRightColumn", 2800, strMLPParam);
		}
		catch (HException& e)
		{
			CreateClassMlp(m_tMLPParam.hvFeatureLength, 10, ClassTotalNum, "softmax", "normalization", 10, 42, &m_tMLPParam.hvMLPHandle);

		}

		try
		{
			CString strDictName;
			strDictName.Format("%s\\DHMLPTrainDict.hdict", m_strMLPPath);
			ReadDict((HTuple)strDictName, HTuple(), HTuple(), &m_tMLPParam.hvTrainDataDict);


		}
		catch (HException& e)
		{
			CreateDict(&m_tMLPParam.hvTrainDataDict);
		}

	}
	catch (HException &)
	{

	}









	return TRUE;



}
BOOL CBattery::WriteParameters()
{
	CriticalSectionHelper lock(&m_csChange);

	m_strParamFile.Format("%s\\缺陷检测参数.ini", m_strModelPath);

	CString strPath;
	strPath.Format("%s//Config//Products//%s//ModelInfo//%d//", GetCurrentAppPath(), m_tInit.strProductName, m_tInit.nGrabberSN);
	CreateMultilevelPath(strPath);

	WritePrivateProfileInt("tUseDefectMasterSwitch", "tUseDefectMasterSwitch", m_tParameters.tUseDefectMasterSwitch, m_strParamFile);//瑕疵总开关

	WritePrivateProfileDouble("tPretreatParam", "dBorderWidth", m_tParameters.tPretreatParam.dBorderWidth, m_strParamFile);
	WritePrivateProfileDouble("tPretreatParam", "dScaleFactor", m_tParameters.tPretreatParam.dScaleFactor, m_strParamFile);

	WritePrivateProfileDouble("tPretreatParam", "dRightBorderWidth", m_tParameters.tPretreatParam.dRightBorderWidth, m_strParamFile);

	WritePrivateProfileDouble("tPretreatParam", "dYaHenPingBiKuanDu", m_tParameters.tPretreatParam.dYaHenPingBiKuanDu, m_strParamFile);


	WritePrivateProfileDouble("tPretreatParam", "dMidLightCof", m_tParameters.tPretreatParam.dMidLightCof, m_strParamFile);
	WritePrivateProfileDouble("tPretreatParam", "dMidDarkCof", m_tParameters.tPretreatParam.dMidDarkCof, m_strParamFile);
	WritePrivateProfileDouble("tPretreatParam", "dBorderLightCof", m_tParameters.tPretreatParam.dBorderLightCof, m_strParamFile);
	WritePrivateProfileDouble("tPretreatParam", "dBorderDarkCof", m_tParameters.tPretreatParam.dBorderDarkCof, m_strParamFile);
	WritePrivateProfileDouble("tPretreatParam", "dMLPSongjindu", m_tParameters.tPretreatParam.dMLPSongjindu, m_strParamFile);

	WritePrivateProfileInt("tPretreatParam", "bEnbleUseClassyfy", m_tParameters.tPretreatParam.bEnbleUseClassyfy, m_strParamFile);



	WritePrivateProfileInt("tLeakFoilParam", "bUseDetect", m_tParameters.tLeakFoilParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tLeakFoilParam", "nThresh", m_tParameters.tLeakFoilParam.nThresh, m_strParamFile);
	WritePrivateProfileDouble("tLeakFoilParam", "dMinArea", m_tParameters.tLeakFoilParam.dMinArea, m_strParamFile);




	WritePrivateProfileInt("tBubbleParam", "bUseDetect", m_tParameters.tBubbleParam.bUseDetect, m_strParamFile);
	WritePrivateProfileDouble("tBubbleParam", "nDynThresh", m_tParameters.tBubbleParam.nDynThresh, m_strParamFile);
	WritePrivateProfileInt("tBubbleParam", "dMinArea", m_tParameters.tBubbleParam.dMinArea, m_strParamFile);
	WritePrivateProfileDouble("tBubbleParam", "dMinCircularity", m_tParameters.tBubbleParam.dMinCircularity, m_strParamFile);




	WritePrivateProfileInt("tDarkLineParam", "bUseDetect", m_tParameters.tDarkLineParam.bUseDetect, m_strParamFile);
	WritePrivateProfileDouble("tDarkLineParam", "nDynThresh", m_tParameters.tDarkLineParam.nDynThresh, m_strParamFile);
	WritePrivateProfileDouble("tDarkLineParam", "dMinWidth", m_tParameters.tDarkLineParam.dMinWidth, m_strParamFile);
	WritePrivateProfileDouble("tDarkLineParam", "dMinLengthWidthRatio", m_tParameters.tDarkLineParam.dMinLengthWidthRatio, m_strParamFile);


	WritePrivateProfileInt("tLightDotParam", "bUseDetect", m_tParameters.tLightDotParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tLightDotParam", "nLightDotDynThresh", m_tParameters.tLightDotParam.nLightDotDynThresh, m_strParamFile);
	WritePrivateProfileDouble("tLightDotParam", "dMinArea", m_tParameters.tLightDotParam.dMinArea, m_strParamFile);


	WritePrivateProfileInt("tDarkDotParam", "bUseDetect", m_tParameters.tDarkDotParam.bUseDetect, m_strParamFile);
	WritePrivateProfileDouble("tDarkDotParam", "dMinArea", m_tParameters.tDarkDotParam.dMinArea, m_strParamFile);
	WritePrivateProfileDouble("tDarkDotParam", "nDarkDotDynThresh", m_tParameters.tDarkDotParam.nDarkDotDynThresh, m_strParamFile);


	WritePrivateProfileInt("tTapeParam", "bUseDetect", m_tParameters.tTapeParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tTapeParam", "nTapeDynThresh", m_tParameters.tTapeParam.nTapeDynThresh, m_strParamFile);
	WritePrivateProfileDouble("tTapeParam", "dLTMinArea", m_tParameters.tTapeParam.dLTMinArea, m_strParamFile);
	WritePrivateProfileDouble("tTapeParam", "dDevFactor", m_tParameters.tTapeParam.dDevFactor, m_strParamFile);

	WritePrivateProfileInt("tTapeParam", "nTapeThresh", m_tParameters.tTapeParam.nTapeThresh, m_strParamFile);
	WritePrivateProfileDouble("tTapeParam", "dTapeThreshMinArea", m_tParameters.tTapeParam.dTapeThreshMinArea, m_strParamFile);




	WritePrivateProfileInt("tFoilParam", "bUseDetect", m_tParameters.tFoilParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tFoilParam", "nMaxDarkThresh", m_tParameters.tFoilParam.nMaxDarkThresh, m_strParamFile);
	WritePrivateProfileDouble("tFoilParam", "dMinDarkArea", m_tParameters.tFoilParam.dMinDarkArea, m_strParamFile);





	WritePrivateProfileInt("tTuoWeiParam", "bUseDetect", m_tParameters.tTuoWeiParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tTuoWeiParam", "nJXTapeLTMinThresh", m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh, m_strParamFile);
	WritePrivateProfileInt("tTuoWeiParam", "nJXTapeLTMaxThresh", m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh, m_strParamFile);
	WritePrivateProfileDouble("tTuoWeiParam", "dJXTapeLTMinArea", m_tParameters.tTuoWeiParam.dJXTapeLTMinArea, m_strParamFile);
	WritePrivateProfileDouble("tTuoWeiParam", "dMaxHeight", m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight, m_strParamFile);







	WritePrivateProfileInt("tAT9Param", "bUseDetect", m_tParameters.tAT9Param.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tAT9Param", "nAT9LMinThresh", m_tParameters.tAT9Param.nAT9LMinThresh, m_strParamFile);
	WritePrivateProfileDouble("tAT9Param", "dAT9LMinWidth", m_tParameters.tAT9Param.dAT9LMinWidth, m_strParamFile);
	WritePrivateProfileDouble("tAT9Param", "dAT9LMinHeight", m_tParameters.tAT9Param.dAT9LMinHeight, m_strParamFile);
	WritePrivateProfileInt("tAT9Param", "nAT9DarkMaxThresh", m_tParameters.tAT9Param.nAT9DarkMaxThresh, m_strParamFile);
	WritePrivateProfileDouble("tAT9Param", "dAT9DarkMinWidth", m_tParameters.tAT9Param.dAT9DarkMinWidth, m_strParamFile);
	WritePrivateProfileDouble("tAT9Param", "dAT9DarkMinHeight", m_tParameters.tAT9Param.dAT9DarkMinHeight, m_strParamFile);



	WritePrivateProfileInt("tJiaoDaiParam", "bUseDetect", m_tParameters.tJiaoDaiParam.bUseDetect, m_strParamFile);
	WritePrivateProfileInt("tJiaoDaiParam", "nDarkThresh", m_tParameters.tJiaoDaiParam.nDarkThresh, m_strParamFile);
	WritePrivateProfileDouble("tJiaoDaiParam", "dMinArea", m_tParameters.tJiaoDaiParam.dMinArea, m_strParamFile);
	WritePrivateProfileInt("tJiaoDaiParam", "nLightThresh", m_tParameters.tJiaoDaiParam.nLightThresh, m_strParamFile);



	try
	{







		////////存储自定义参数

		///////////////////////////////////////////////////
		//strPath.Format("%s\\Config\\Config.ini",GetCurrentAppPath());
		strPath.Format("%s\\Config.ini", m_strModelPath);
		WritePrivateProfileInt("Feature", "FeatureGroupNum", m_tParameters.tFeatureSelect.nGroupNum, strPath);
		CString strAppName;
		CString strSubKey;
		HTuple hv_SingleFeature;
		for (int i = 0; i < m_tParameters.tFeatureSelect.nGroupNum; i++)
		{

			strAppName.Format("FeatureGroup%d", i);
			WritePrivateProfileInt(strAppName, "SingleGroupNum", m_tParameters.tFeatureSelect.Vec_FSelects.at(i).nSingleGroupNum, strPath);

			SingleErrorGroup tSingleErrorGroup;
			tSingleErrorGroup = m_tParameters.tFeatureSelect.Vec_FSelects.at(i);
			WritePrivateProfileInt(strAppName, "SingleGroupNum", tSingleErrorGroup.nSingleGroupNum, strPath);
			WritePrivateProfileString(strAppName, "缺陷组名", tSingleErrorGroup.strErrorName, strPath);
			for (int j = 0; j < tSingleErrorGroup.nSingleGroupNum; j = j + 3)
			{

				SingleFeature tSingleFeature;
				tSingleFeature = tSingleErrorGroup.Vec_tSingleGroupFeatures.at(j / 3);
				strSubKey.Format("特征%d", j);
				WritePrivateProfileString(strAppName, strSubKey, tSingleFeature.StrSingleFeatureName, strPath);
				strSubKey.Format("MIN%d", j);
				WritePrivateProfileDouble(strAppName, strSubKey, tSingleFeature.dMin, strPath);
				strSubKey.Format("MAX%d", j);
				WritePrivateProfileDouble(strAppName, strSubKey, tSingleFeature.dMax, strPath);
			}

		}




	}
	catch (HException e)
	{

	}


	////////////////////////////////////////////////////////////////////////新增加的定位
	try
	{
		//CString strModelInfoPath,strTuple;
		//strModelInfoPath.Format("%s\\模板信息",m_strModelPath);
		//CreateMultilevelPath(strModelInfoPath);
		//CString strModelConfig;
		//strModelConfig.Format("%s\\Config.ini",strModelInfoPath);

		//WritePrivateProfileInt("ModelInfo","bAotuFindLocate",m_tParameters.tTempLateInfo.bAotuFindLocate,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","bUseFirstLengthGRR",m_tParameters.tTempLateInfo.bUseFirstLengthGRR,strModelConfig);


		//WritePrivateProfileInt("ModelInfo","nFoilThresh",m_tParameters.tTempLateInfo.nFoilThresh,strModelConfig);
	 //   WritePrivateProfileInt("ModelInfo","nHelpDraw",m_tParameters.tTempLateInfo.nHelpDraw,strModelConfig);
		//WritePrivateProfileDouble("ModelInfo","dOffset",m_tParameters.tTempLateInfo.dOffset,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","dFoilHeight",m_tParameters.tTempLateInfo.dFoilHeight,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","dFoilWidth",m_tParameters.tTempLateInfo.dFoilWidth,strModelConfig);

		//WritePrivateProfileInt("ModelInfo","bUseManualSeg",m_tParameters.tTempLateInfo.bUseManualSeg,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","bUsePreProcess",m_tParameters.tTempLateInfo.bUsePreProcess,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nLineThresh",m_tParameters.tTempLateInfo.nLineThresh,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nBotLineThresh",m_tParameters.tTempLateInfo.nBotLineThresh,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nFirstOrLastPoint",m_tParameters.tTempLateInfo.nFirstOrLastPoint,strModelConfig);



		//WritePrivateProfileInt("ModelInfo","bExistMarkInCoat",m_tParameters.tTempLateInfo.bExistMarkInCoat,strModelConfig);

		//WritePrivateProfileInt("ModelInfo","nHighThresh",m_tParameters.tTempLateInfo.nHighThresh,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nLowThresh",m_tParameters.tTempLateInfo.nLowThresh,strModelConfig);

		//WritePrivateProfileInt("ModelInfo","nFirstLengthLocation",m_tParameters.tTempLateInfo.nFirstLengthLocation,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nFitType",m_tParameters.tTempLateInfo.nFitType,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nReferPoint",m_tParameters.tTempLateInfo.nReferPoint,strModelConfig);

		//WritePrivateProfileInt("ModelInfo","nSpotHighThresh",m_tParameters.tTempLateInfo.nSpotHighThresh,strModelConfig);
		//WritePrivateProfileInt("ModelInfo","nSpotNum",m_tParameters.tTempLateInfo.nSpotNum,strModelConfig);
		//WritePrivateProfileDouble("ModelInfo","dSpotResolution",m_tParameters.tTempLateInfo.dSpotResolution,strModelConfig);
		//WritePrivateProfileDouble("ModelInfo","dSpotSetArea",m_tParameters.tTempLateInfo.dSpotSetArea,strModelConfig);

		//WritePrivateProfileDouble("ModelInfo","dJXNearCoatHeight",m_tParameters.tTempLateInfo.dJXNearCoatHeight,strModelConfig);
		//WritePrivateProfileDouble("ModelInfo","dNearCoatOffset",m_tParameters.tTempLateInfo.dNearCoatOffset,strModelConfig);

		//try
		//{
		//	strTuple.Format("%s\\检测区域.hobj", strModelInfoPath);
		//	WriteObject(m_tParameters.tTempLateInfo.hoDetectRegion,(HTuple)strTuple);
		//}catch(HException& e)
		//{
		//	m_tParameters.tTempLateInfo.hoDetectRegion.GenEmptyObj();
		//}


		/////////存储点检









	}
	catch (HException &)
	{

	}



	try
	{
		CString strDictName;
		strDictName.Format("%s\\DHMLPTrainDict.hdict", m_strMLPPath);
		WriteDict(m_tMLPParam.hvTrainDataDict, (HTuple)strDictName, HTuple(), HTuple());
	}
	catch (HException& e)
	{

	}

	try
	{
		CString strMlpName;
		strMlpName.Format("%s\\DHMLPSurfaceAlg.gmc", m_strMLPPath);
		WriteClassMlp(m_tMLPParam.hvMLPHandle, (HTuple)strMlpName);

		CString  strMLPParam;
		strMLPParam.Format("%s\\MLPConfig.ini", m_strMLPPath);
		WritePrivateProfileInt("TestCoordinate", "LeftColumn", m_tMLPParam.nLeftColumn, strMLPParam);
		WritePrivateProfileInt("TestCoordinate", "nRightColumn", m_tMLPParam.nRightColumn, strMLPParam);

	}
	catch (HException& e)
	{

	}




	return TRUE;
}

void CBattery::CrudeBreak()
{
	HObject Region, hoCross;
	HTuple  Row, Column;
	GetDomain(m_ptDetectIn->hoDetectImage, &Region);
	AreaCenter(Region, NULL, &Row, &Column);
	GenCrossContourXld(&hoCross, Row, Column, 850, 45);
	ConcatObj(m_ptDetectOut->ho_Foil, hoCross, &m_ptDetectOut->ho_Foil);
	ConcatObj(m_ptDetectOut->ho_Coat, Region, &m_ptDetectOut->ho_Coat);
	ConcatObj(m_ptDetectOut->ho_Foil, Region, &m_ptDetectOut->ho_Foil);
	AddError(Region, TuoTan);
}


void CBattery::GetDetetcRegion()
{
	//获取生成的区域  涂布中心区域 边界区域 AT9区域 

	/////////////////根据涂布宽度生成边界区域////////////////////////////////

	HObject hoErorsionCoat;
	HTuple hv_LTRow, hv_LTColumn, hv_RBRow, hv_RBColumn;
	HTuple hv_BorderWidth = MAX(2, m_tParameters.tPretreatParam.dBorderWidth / m_dColResolution);
	hoErorsionCoat = m_tRegion.ho_CoatUnion;
	HObject hoCoats;
	Connection(m_tRegion.ho_CoatUnion, &hoCoats);
	ErosionRectangle1(hoCoats, &m_hoCoatMid, hv_BorderWidth * 2, 21);
	Difference(hoErorsionCoat, m_hoCoatMid, &m_hoBoderRegion);
	Connection(m_hoBoderRegion, &m_hoBoderRegion);
	if (m_tInit.nCoatType != M_TwoCam_Continue)
	{
		ErosionRectangle1(m_hoCoatMid, &m_hoCoatMid, 1, hv_BorderWidth * 2);
	}

	try
	{
		if (m_tInit.nCoatType == M_TwoCam_Intermission)//间歇涂布存在压痕屏蔽区域
		{

			///如果长膜在上压痕也在上面 确认压痕的位置需要短膜长所在间歇加上偏移100 固定偏移100
			double dOffSetValue = 0;
			int nJXLocation = m_tStandardSize.hvQiTouXianChu;//0 表示间歇在上 1表示间歇在下
			double dYaHenRegionMDWidth = m_tParameters.tPretreatParam.dYaHenPingBiKuanDu / m_dRowResolution;
			int  nConstOffsetValue = 200;//系统拼图保留的上张图的高度
			HTuple hvWidth, hvHeight;
			GetImageSize(m_hoAdaptedImage, &hvWidth, &hvHeight);
			if (m_tStandardSize.hvExistJXType == 0 && m_tInit.nGrabberSN == 1)//仅正面间歇
			{


				if (nJXLocation == 0)//间歇在上
				{
					dOffSetValue = m_tStandardSize.dUpIntermissionHeight / m_dRowResolution + 100;
					GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, dOffSetValue - dYaHenRegionMDWidth / 2, 0, dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);

				}
				else
				{

					dOffSetValue = m_tStandardSize.dUpIntermissionHeight / m_dRowResolution + 10 / m_dRowResolution;
					GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, hvHeight - dOffSetValue - dYaHenRegionMDWidth / 2, 0, hvHeight - dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);

				}


			}
			else if (m_tStandardSize.hvExistJXType == 1 && m_tInit.nGrabberSN == 0)//仅反面间歇
			{

				if (nJXLocation == 0)//间歇在上
				{
					dOffSetValue = m_tStandardSize.dDownIntermissionHeight / m_dRowResolution + 100;
					GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, dOffSetValue - dYaHenRegionMDWidth / 2, 0, dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);

				}
				else
				{

					dOffSetValue = m_tStandardSize.dDownIntermissionHeight / m_dRowResolution + 10 / m_dRowResolution;
					GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, hvHeight - dOffSetValue - dYaHenRegionMDWidth / 2, 0, hvHeight - dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);

				}

			}
			else //双面间歇
			{
				if (nJXLocation == 0)//间歇在上
				{

					dOffSetValue = MAX(m_tStandardSize.dUpIntermissionHeight / m_dRowResolution, m_tStandardSize.dDownIntermissionHeight / m_dRowResolution) + 100;
					if (m_tStandardSize.hvChangMoZaiShang == 0 && m_tInit.nGrabberSN == 1)//后相机生成
					{
						GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, dOffSetValue - dYaHenRegionMDWidth / 2, 0, dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);
					}
					if (m_tStandardSize.hvChangMoZaiShang == 1 && m_tInit.nGrabberSN == 0)//前相机生成
					{
						GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, dOffSetValue - dYaHenRegionMDWidth / 2, 0, dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);
					}


				}
				else
				{

					dOffSetValue = m_tStandardSize.dDownIntermissionHeight / m_dRowResolution + 10 / m_dRowResolution;

					if (m_tStandardSize.hvChangMoZaiShang == 0 && m_tInit.nGrabberSN == 1)//后相机生成
					{
						GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, hvHeight - dOffSetValue - dYaHenRegionMDWidth / 2, 0, hvHeight - dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);
					}
					if (m_tStandardSize.hvChangMoZaiShang == 1 && m_tInit.nGrabberSN == 0)//前相机生成
					{
						GenRectangle1(&m_tBlackRegion.hoYaHenPingBiRegion, hvHeight - dOffSetValue - dYaHenRegionMDWidth / 2, 0, hvHeight - dOffSetValue + dYaHenRegionMDWidth / 2, hvWidth - 1);
					}
				}



			}






		}

	}
	catch (HException& e)
	{

		m_tBlackRegion.hoYaHenPingBiRegion.GenEmptyObj();
	}







}

BOOL CBattery::Separate()
{
	try
	{
		m_tSeparateIn.hoGrabImage = m_ptDetectIn->hoDetectImage;
		m_tSeparateIn.hvHomMat2D = m_hvHomMat2D;
		m_tSeparateIn.nBreakType = 0;
		int nStyle = m_CSeperate.Detect(m_tSeparateIn, &m_tSeparateOut);

		Union1(m_tSeparateOut.ho_Coat, &m_tRegion.ho_CoatUnion);
		Union1(m_tSeparateOut.ho_Foil, &m_tRegion.ho_FoilUnion);
		Union1(m_tSeparateOut.ho_Ceramic, &m_tRegion.ho_CerimicUnion);



		TupleGenConst(1, TRUE, &m_ptDetectOut->hv_Flag);
		m_ptDetectOut->hv_CoatWidth = m_tSeparateOut.hv_CoatWidth;
		m_ptDetectOut->hv_CoatHeight = m_tSeparateOut.hv_CoatWidth;
		m_ptDetectOut->hv_FoilWidth = m_tSeparateOut.hv_FoilWidth;
		m_ptDetectOut->hv_FoilHeight = m_tSeparateOut.hv_FoilWidth;
		m_ptDetectOut->hv_LocateMissing = m_tSeparateOut.hv_CeramicWidth;
		m_ptDetectOut->hv_BaiBian = m_tSeparateOut.hv_CeramicWidth;
		m_ptDetectOut->hv_CeramicWidth = m_tSeparateOut.hv_CeramicWidth;
		m_ptDetectOut->ho_Coat = m_tRegion.ho_CoatUnion;
		m_ptDetectOut->ho_Foil = m_tRegion.ho_FoilUnion;
		m_hoAt9Region = m_tRegion.ho_CerimicUnion;
		m_ptDetectOut->hv_AT9EdageToProEdage = m_tSeparateOut.hv_AT9EdageToProEdage;
		m_ptDetectOut->hv_CpkEdageToProEdage = m_tSeparateOut.hv_CpkEdageToProEdage;
		//间歇涂布
		m_ptDetectOut->hv_LeftEadgePixel = m_tSeparateOut.hv_LeftEadgePixel;
		m_ptDetectOut->hv_RightEadgePixel = m_tSeparateOut.hv_RightEadgePixel;
		m_ptDetectOut->hv_TopEdagePixel = m_tSeparateOut.hv_TopEdagePixel;
		m_ptDetectOut->hv_BotEdagePixel = m_tSeparateOut.hv_BotEdagePixel;
		m_ptDetectOut->hv_SortWidth = m_tSeparateOut.hv_SortWidth;
		m_ptDetectOut->hv_Name = m_tSeparateOut.hv_Name;
		m_ptDetectOut->hv_TripSNTuple = m_tSeparateOut.hv_TripSNTuple;
		m_ptDetectOut->hv_CaoWeiWidthTuple = m_tSeparateOut.hv_CaoWeiWidthTuple;
		m_ptDetectOut->hv_CaoWeiHeightTuple = m_tSeparateOut.hv_CaoWeiHeightTuple;
		m_ptDetectOut->hv_CaoWeiXPositionTuple = m_tSeparateOut.hv_CaoWeiXPositionTuple;
		m_ptDetectOut->hv_CaoWeiYpositionTuple = m_tSeparateOut.hv_CaoWeiYpositionTuple;
		m_ptDetectOut->hv_JXHeight = m_tSeparateOut.hv_JXHeight;
		m_ptDetectOut->hv_MPLength = m_tSeparateOut.hv_MPLength;
		m_ptDetectOut->hv_SumLength = m_tSeparateOut.hv_SumLength;
		m_ptDetectOut->ho_JXRegion = m_tSeparateOut.ho_JXRegion;
		m_ptDetectOut->ho_ExtractCaoWeiRegions = m_tSeparateOut.ho_ExtractCaoWeiRegions;

		m_tBlackRegion.ho_JXRegion = m_tSeparateOut.ho_JXRegion;
		if (m_tParameters.tPretreatParam.dScaleFactor != 1)
		{
			ScaleImage(m_hoAdaptedImage, &m_hoAdaptedImage, m_tParameters.tPretreatParam.dScaleFactor, 0);
		}

		if (m_tStandardSize.hv_CoatType.I() == M_TwoCam_Continue)
		{
			///如果是连续涂布 分条需要按照整体的宽度进行区分

			HTuple hv_SingleWidth, hv_SingleHeight;
			HObject hoSinglePartRectangle, hoConcatObject, hoDestObject, hoConcatRegion, hoUnionRegion;
			hoConcatObject.GenEmptyObj();
			hoDestObject.GenEmptyObj();
			hoConcatRegion.GenEmptyObj();
			hoUnionRegion.GenEmptyObj();
			ConcatObj(hoConcatRegion, m_tSeparateOut.ho_Foil, &hoConcatRegion);
			ConcatObj(hoConcatRegion, m_tSeparateOut.ho_Ceramic, &hoConcatRegion);
			ConcatObj(hoConcatRegion, m_tSeparateOut.ho_Coat, &hoConcatRegion);
			Union1(hoConcatRegion, &hoUnionRegion);
			ShapeTrans(hoUnionRegion, &hoUnionRegion, "rectangle1");

			HTuple hv_LTRow, hvLTColumn, hv_RBRow, hvRBColumn;
			SmallestRectangle1(hoUnionRegion, &hv_LTRow, &hvLTColumn, &hv_RBRow, &hvRBColumn);
			HTuple hv_LeftErosionColumn = hvLTColumn + m_tParameters.tPretreatParam.dBorderWidth / m_dColResolution;
			HTuple hv_RightErosionColumn = hvRBColumn - m_tParameters.tPretreatParam.dRightBorderWidth / m_dColResolution;
			GenRectangle1(&hoUnionRegion, hv_LTRow, MIN(hv_LeftErosionColumn.D(), hv_RightErosionColumn.D()), hv_RBRow, MAX(hv_LeftErosionColumn.D(), hv_RightErosionColumn.D()));






			RegionFeatures(hoUnionRegion, "width", &hv_SingleWidth);
			RegionFeatures(hoUnionRegion, "height", &hv_SingleHeight);





			PartitionRectangle(hoUnionRegion, &hoConcatObject, hv_SingleWidth.D() / m_tStandardSize.hvTripNum.I(), hv_SingleHeight);
			//for (int k=1;k<=m_tSeparateOut.ho_Coat.CountObj();k++)
			//{

			//	RegionFeatures(m_tSeparateOut.ho_Coat.SelectObj(k),"width",&hv_SingleWidth);
			//	RegionFeatures(m_tSeparateOut.ho_Coat.SelectObj(k),"height",&hv_SingleHeight);
			//	PartitionRectangle(m_tSeparateOut.ho_Coat.SelectObj(k),&hoSinglePartRectangle,hv_SingleWidth/2,hv_SingleHeight);
			//	ConcatObj(hoConcatObject,hoSinglePartRectangle,&hoConcatObject);
			//}
			SortRegion(hoConcatObject, &hoDestObject, "upper_left", "true", "column");
			m_ptDetectOut->ho_Coat = m_tSeparateOut.ho_Coat;
			m_tSeparateOut.ho_Coat = hoDestObject;
			m_tBlackRegion.hoTripRegion = hoDestObject;
			m_ptDetectOut->ho_JXRegion = hoDestObject;

		}
		else
		{
			HTuple hv_SingleWidth, hv_SingleHeight;
			HObject hoSinglePartRectangle, hoConcatObject, hoDestObject, hoErosionObject;
			hoConcatObject.GenEmptyObj();
			hoDestObject.GenEmptyObj();
			hoErosionObject.GenEmptyObj();
			HObject hoUnionObject;
			hoUnionObject.GenEmptyObj();

			ConcatObj(hoUnionObject, m_tSeparateOut.ho_Ceramic, &hoUnionObject);
			ConcatObj(hoUnionObject, m_tSeparateOut.ho_Coat, &hoUnionObject);
			Union1(hoUnionObject, &hoUnionObject);
			ShapeTrans(hoUnionObject, &hoUnionObject, "rectangle1");

			HTuple hv_LTRow, hvLTColumn, hv_RBRow, hvRBColumn;
			SmallestRectangle1(hoUnionObject, &hv_LTRow, &hvLTColumn, &hv_RBRow, &hvRBColumn);
			HTuple hv_LeftErosionColumn = hvLTColumn + m_tParameters.tPretreatParam.dBorderWidth / m_dColResolution;
			HTuple hv_RightErosionColumn = hvRBColumn - m_tParameters.tPretreatParam.dRightBorderWidth / m_dColResolution;
			GenRectangle1(&hoUnionObject, hv_LTRow, MIN(hv_LeftErosionColumn.D(), hv_RightErosionColumn.D()), hv_RBRow, MAX(hv_LeftErosionColumn.D(), hv_RightErosionColumn.D()));
			//for (int k=1;k<=hoUnionObject.CountObj();k++)
			//{
			//	//ErosionRectangle1(hoUnionObject.SelectObj(k),&hoErosionObject, MAX(11,m_tParameters.tPretreatParam.dBorderWidth/m_dColResolution*2),MAX(11, m_tParameters.tPretreatParam.dBorderWidth / m_dColResolution * 2));
			//	RegionFeatures(hoUnionObject.SelectObj(k),"width",&hv_SingleWidth);
			//	RegionFeatures(hoUnionObject.SelectObj(k), "height", &hv_SingleHeight);
			//	PartitionRectangle(hoUnionObject.SelectObj(k),&hoSinglePartRectangle,hv_SingleWidth/m_tStandardSize.hvTripNum.I(),hv_SingleHeight);
			//	ConcatObj(hoConcatObject,hoSinglePartRectangle,&hoConcatObject);
			//}
			GetImageSize(m_hoAdaptedImage, &hv_SingleWidth, &hv_SingleHeight);
			if (hv_SingleWidth*m_dColResolution > m_tStandardSize.hvTripNum.I()*m_tStandardSize.dCoatWidth.D())
			{

				for (int k = 1; k <= m_tStandardSize.hvTripNum.I(); k++)
				{

					GenRectangle1(&hoSinglePartRectangle, 0, hv_LeftErosionColumn + (k - 1)*m_tStandardSize.dCoatWidth.D() / m_dColResolution, hv_SingleHeight - 1, hv_LeftErosionColumn + k * m_tStandardSize.dCoatWidth.D() / m_dColResolution);
					ConcatObj(hoConcatObject, hoSinglePartRectangle, &hoConcatObject);
				}
			}
			else
			{
				for (int k = 1; k <= hoUnionObject.CountObj(); k++)
				{

					RegionFeatures(hoUnionObject.SelectObj(k), "width", &hv_SingleWidth);
					RegionFeatures(hoUnionObject.SelectObj(k), "height", &hv_SingleHeight);
					PartitionRectangle(hoUnionObject.SelectObj(k), &hoSinglePartRectangle, hv_SingleWidth / m_tStandardSize.hvTripNum.I(), hv_SingleHeight);
					ConcatObj(hoConcatObject, hoSinglePartRectangle, &hoConcatObject);
				}
			}
			SortRegion(hoConcatObject, &hoDestObject, "upper_left", "true", "column");
			m_ptDetectOut->ho_Coat = m_tSeparateOut.ho_Coat;
			m_tSeparateOut.ho_Coat = hoDestObject;
			m_tBlackRegion.hoTripRegion = hoDestObject;

			m_ptDetectOut->ho_JXRegion = hoDestObject;
		}

	}
	catch (HException e)
	{

		CString StrException;
		StrException.Format("分割模块传值异常:%s:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		((DHInterface*)m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)StrException, NULL);
	}
	return TRUE;
}


BOOL CBattery::MeasureSize()
{

	if (FALSE == Separate())//如果分割区域就有问题 直接返回FALSE 尺寸检测也在这里
	{
		return FALSE;
	}

}
void CBattery::SaveException(HException e, CString strFuncName)
{
	SYSTEMTIME CurrentSystime;
	GetLocalTime(&CurrentSystime);
	//保存图片
	CString strPath;
	CString strImageName;
	strImageName.Format("%d时%d分%d秒.bmp", CurrentSystime.wHour, CurrentSystime.wMinute, CurrentSystime.wSecond);
	CreateDirectory("D:\\SeekersException", NULL);
	strPath.Format("D:\\SeekersException\\%d月%d日\\", CurrentSystime.wMonth, CurrentSystime.wDay);
	CreateDirectory(strPath, NULL);
	WriteImage(m_ptDetectIn->hoDetectImage, "bmp", NULL, HTuple(strPath + strImageName));

	//保存日志
	CString strOut;
	strOut.Format("%s出现异常\r\n异常信息：%s -- %s", strFuncName, e.ErrorMessage().Text(), strImageName);
	HTuple FileHandle;
	OpenFile("D:\\SeekersException\\log.txt", "append", &FileHandle);
	FwriteString(FileHandle, HTuple(strOut));
	FnewLine(FileHandle);
	CloseFile(FileHandle);
}


void CBattery::RectyfyIntermission(HTuple hv_LocationInfo)//根据传进来信息进行调整间歇的高度
{
	//if (m_tInit.nCoatType==M_TwoCam_Intermission)
	//{
	//	int nLeftPosition  =hv_LocationInfo[0].I();
	//	int nRightPosition =hv_LocationInfo[1].I();
	//	int nFontIntermissionHeight=hv_LocationInfo[2].D()/m_dRowResolution;
	//	int nDownIntermissionHeight=hv_LocationInfo[3].D()/m_dRowResolution;

	//	if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==1)
	//	{

	//		if (nFontIntermissionHeight!=0)
	//		{
	//			GenRectangle1(&m_tParameters.tTempLateInfo.hoDetectRegion,0,nLeftPosition-50,nFontIntermissionHeight+50,nRightPosition);
	//		}else
	//		{
	//			m_tParameters.tTempLateInfo.hoDetectRegion.GenEmptyObj();
	//		}

	//	}else
	//	{
	//		if (nDownIntermissionHeight!=0)
	//		{
	//			GenRectangle1(&m_tParameters.tTempLateInfo.hoDetectRegion,0,nLeftPosition-50,nDownIntermissionHeight+50,nRightPosition);
	//		}else
	//		{
	//			m_tParameters.tTempLateInfo.hoDetectRegion.GenEmptyObj();
	//		}

	//	}
	//	WriteParameters();
	//}
}


int CBattery::DoSomething(void* message, void* wParam, void* lParam)
{


	CString  strMessage = (char*)message;
	if (strMessage == "INITIALIZE")
	{
		return Init(*(DETECTINIT*)wParam);

	}
	else if (strMessage == "SHOW")
	{
		return DisplaySetDialog(*(DETECTINPUT*)wParam);
	}
	else if (strMessage == "SETPARAM")
	{
		CString strWparam;
		strWparam.Format("%s", wParam);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if ("tSizeParam" == strWparam)
		{
			CriticalSectionHelper lock(&m_csChange);
			m_tStandardSize = *(STANDARDSIZE*)lParam;
			m_CSeperate.m_tStandardSizes = *(STANDARDSIZE*)lParam;
			if (m_tStandardSize.hv_CoatType == M_TwoCam_Intermission)
			{
				m_CSeperate.m_tLineMatch.nLineUsed = 7;//如果是间歇暂时按照5个线进行规划
				m_CSeperate.m_tInit.nCoatType = M_TwoCam_Intermission;
			}
			else
			{
				m_tInit.nCoatType = M_TwoCam_Continue;
				m_CSeperate.m_tInit.nCoatType = M_TwoCam_Continue;
			}
			FlushFlag();
			m_CSeperate.FlushFlag();
			return TRUE;
		}
		else if ("SetPowerFlag" == strWparam)//权限设置
		{
			CriticalSectionHelper lock(&m_csChange);
			m_bPoweFlag = *(int*)lParam;
			return TRUE;
		}
		else if ("ReLoadParam" == strWparam)//换卷保存
		{
			CriticalSectionHelper lock(&m_csChange);
			ReadDeFaultParameters();
			WriteParameters();
			return TRUE;
		}
		else if ("ReFindLine" == strWparam)
		{
			m_CSeperate.m_bReFindLine = *(BOOL*)lParam;
		}
		else
		{
			AfxMessageBox("wParam有误，在Detect模块");
			return FALSE;
		}
		return TRUE;
	}
	else if ("COPYPARAM" == strMessage)
	{
		CString strWparam;
		strWparam.Format("%s", wParam);
		if ("ALGPARAM" == strWparam)
		{
			//TemplateInfo tTempLateInfo;
			//tTempLateInfo=m_tParameters.tTempLateInfo;
			m_tParameters = ((CBattery*)lParam)->m_tParameters;
			//m_tParameters.tTempLateInfo=tTempLateInfo;
			WriteParameters();
		}
		else if ("LANE" == strWparam)
		{
			//return m_pMeasure->DoSomething(message,wParam,((CBattery*)lParam)->m_pMeasure);
			return FALSE;
		}
		else if ("LINEMATCH" == strWparam)
		{
			//return m_pMeasure->DoSomething(message,wParam,((CBattery*)lParam)->m_pMeasure);
			return FALSE;
		}
		else
		{
			AfxMessageBox("wParam有误，在Detect模块");
			return FALSE;
		}
		return TRUE;
	}
	else if (strMessage == "ACTION")
	{
		int nRes = 0;
		try
		{
			CriticalSectionHelper lock(&m_csChange);
			nRes = Detect(*(DETECTINPUT*)wParam, (DETECTOUTPUT*)lParam, ((DETECTINPUT*)wParam)->nBreakType);
			if (((DETECTOUTPUT*)lParam)->ho_ErrorRegion.CountObj() != ((DETECTOUTPUT*)lParam)->hv_ErrorBelongStrip.Length()
				|| ((DETECTOUTPUT*)lParam)->ho_ErrorRegion.CountObj() != ((DETECTOUTPUT*)lParam)->hv_ErrorLevel.Length()
				|| ((DETECTOUTPUT*)lParam)->ho_ErrorRegion.CountObj() != ((DETECTOUTPUT*)lParam)->hv_ErrorType.Length())
			{
				int a;
				a = ((DETECTOUTPUT*)lParam)->ho_ErrorRegion.CountObj();
				int b = ((DETECTOUTPUT*)lParam)->hv_ErrorBelongStrip.Length();
				int c = ((DETECTOUTPUT*)lParam)->hv_ErrorLevel.Length();
				int d = ((DETECTOUTPUT*)lParam)->hv_ErrorType.Length();


				AfxMessageBox("算法结果有误dddd！");
			}
		}
		catch (HException e)
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

		return nRes;
	}
	else if (strMessage == "COPYFILES")
	{

		CString strSrc, strDst;
		strSrc.Format("%s\\Config\\Products\\%s\\SCIDetect\\%d", GetCurrentAppPath(), wParam,
			m_tInit.nGrabberSN);
		strDst.Format("%s\\Config\\Products\\%s\\SCIDetect\\%d", GetCurrentAppPath(), lParam,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strDst);
		CopyFolder(strSrc, strDst);
		//m_pMeasure->DoSomething(message,wParam,lParam);

		return TRUE;
	}
	else if ("DELETEFILES" == strMessage)//拷贝一个副本 看下
	{
		try
		{
			CString strDst;
			strDst.Format("%s\\Config\\Products\\SCIDetect\\%s\\%d", GetCurrentAppPath(), wParam, m_tInit.nGrabberSN);
			DeleteFolder(strDst);
			HTuple hvFiles;
			strDst.Format("%s\\Config\\Products\\SCIDetect\\%s", GetCurrentAppPath(), wParam);
			ListFiles(HTuple(strDst), "directories", &hvFiles);
			if (0 == hvFiles.Length())//如果里面没文件了，把上一级目录删除掉
			{

				DeleteFolder(strDst);
			}
			//m_pMeasure->DoSomething(message,wParam,lParam);
		}
		catch (HException)
		{

		}
		return TRUE;
	}
	else if ("COPYOBJECTS" == strMessage)
	{
		CriticalSectionHelper lock(&m_csChange);
		*this = *(CBattery*)wParam;


	}
	else if ("GETPARAM" == strMessage)
	{

		CString strWparam;
		strWparam.Format("%s", wParam);
		{
			//return m_pMeasure->DoSomething(message,wParam,lParam);
			return TRUE;
		}



	}
	else
	{
		CString str = "没有" + strMessage + "命令对应的响应函数，请重新确认检测！";
		AfxMessageBox(str);
		return FALSE;
	}


	return FALSE;
}


void CBattery::classify(HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName,
	HTuple hv_RegionFeatureName, HTuple *hv_Classes, HTuple *hv_Confidence)
{

	// Local iconic variables
	HObject  ho_Region;

	// Local control variables
	HTuple  hv_Number, hv_J, hv_Features, hv_Class, hv_Probility;
	HTuple hv_TempConfidence;
	CountObj(ho_Regions, &hv_Number);
	(*hv_Classes) = HTuple();
	(*hv_Confidence) = HTuple();
	{
		HTuple end_val2 = hv_Number;
		HTuple step_val2 = 1;
		for (hv_J = 1; hv_J.Continue(end_val2, step_val2); hv_J += step_val2)
		{
			SelectObj(ho_Regions, &ho_Region, hv_J);
			get_features(ho_Region, ho_Image, hv_GrayFeatureName, hv_RegionFeatureName, &hv_Features);
			EvaluateClassMlp(hv_MLPHandle, hv_Features, &hv_Probility);
			ClassifyClassMlp(hv_MLPHandle, hv_Features, 1, &hv_Class, &hv_TempConfidence);
			(*hv_Classes) = (*hv_Classes).TupleConcat(hv_Class);
			(*hv_Confidence) = (*hv_Confidence).TupleConcat(hv_Probility[hv_Class.I()]);
		}
	}
	return;
}

void CBattery::get_features(HObject ho_Region, HObject ho_Image, HTuple hv_GrayFeaturesName,
	HTuple hv_RegionFeaturesName, HTuple *hv_Features)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_RegionFeatures, hv_GrayFeatures;



	RegionFeatures(ho_Region, hv_RegionFeaturesName, &hv_RegionFeatures);
	GrayFeatures(ho_Region, ho_Image, hv_GrayFeaturesName, &hv_GrayFeatures);
	(*hv_Features).Clear();
	(*hv_Features).Append(hv_GrayFeatures);
	(*hv_Features).Append(hv_RegionFeatures);
	return;
}


