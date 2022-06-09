#include "StdAfx.h"
#include "Detect.h"
#include "PageLineSet.h"

#define    MAX_LINEMATCH 6
// 通过导出函数形式向调用方提供指向派生类对象的基类指针



CSeparate::CSeparate(void)
	:m_hvec_GRRpoint(1)
{
	m_bExistJXFlag = FALSE;
	m_bExistOAPFlag = FALSE;
	m_bExistCaoWeiFlag = FALSE;
	m_bReFindLine = FALSE;
}
CSeparate::~CSeparate(void)
{


}

void CSeparate::FlushFlag()
{
	//HTuple hvExistJXType;//间歇存在的类型 0：仅正面间歇  1：仅反面间歇 2：双面间歇
	if (m_tStandardSizes.hvExistJXType == 2 || m_tStandardSizes.hvExistJXType == m_tInit.nGrabberSN)//是否存在间歇
	{

		m_bExistJXFlag = TRUE;

	}
	//HTuple hv_CaoWeiType;//槽位存在的类型 0：不存在槽位 1：仅正面槽位  2：仅反面槽位 3：双面槽位
	if (m_tStandardSizes.hv_CaoWeiType == 3 || m_tStandardSizes.hv_CaoWeiType == m_tInit.nGrabberSN + 1)//是否存在槽位
	{

		m_bExistCaoWeiFlag = TRUE;

	}

	//HTuple hv_OAPType;//OAP存在的类型 0：不存在OAP 1：仅正面OAP  2：仅反面OAP 3：双面OAP
	if (m_tStandardSizes.hv_OAPType == 3 || m_tStandardSizes.hv_OAPType == m_tInit.nGrabberSN + 1)//是否存在OAP
	{

		m_bExistOAPFlag = TRUE;

	}
	m_bExistJXUpDownFlag = (BOOL)m_tStandardSizes.hvQiTouXianChu.I();
}
void CSeparate::SaveParam()
{
	try
	{
		CString strFilePath;
		strFilePath.Format("%s\\Config\\Products\\%s\\SCISeparate\\%d", GetCurrentAppPath(), m_tInit.strProductName,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strFilePath);
		for (int i = 0; i < LINE_COUNT; i++)
		{
			CString strFileName;
			strFileName.Format("%s\\%d.ini", strFilePath, i);
			m_sLineTool[i].WriteParam(strFileName);
		}
		//WritePrivateProfileInt("PARAMETER","PRETREAT",m_tMeausreParam.bPretreat,m_strMyConfig);
		//WritePrivateProfileInt("PARAMETER","USELINEREFER",m_tMeausreParam.bUseLineRefer,m_strMyConfig);
		//WritePrivateProfileInt("PARAMETER","LINEREFERSN",m_tMeausreParam.nLineReferSN,m_strMyConfig);
		//WritePrivateProfileInt("PARAMETER","JUDGECOAT",m_tMeausreParam.bJudgeCoat,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER", "COATTHRES", m_tMeausreParam.nCoatThres, m_strMyConfig);
		WritePrivateProfileInt("CALIBRATIONPARAME", "nCalculateChoose", m_nCalculateChoose, m_strMyConfig);


		SaveLineMatch();
		CString strModelPath;
		strModelPath.Format("%s\\CaoWeiRegions.hobj", strFilePath);
		try
		{
			WriteObject(m_tMeausreParam.hoCaoweiRegions, (HTuple)strModelPath);
		}
		catch (HException& e)
		{

			m_tMeausreParam.hoCaoweiRegions.GenEmptyObj();
		}




		//////////////////////////////////////////////


		CString lpAppName, keyName;
		CString strKeyName;
		lpAppName.Format("GRABBER%d", m_tInit.nGrabberSN);
		m_strCommondh.Format("%s\\Config\\Common.dh", GetCurrentAppPath());
		WritePrivateProfileInt(lpAppName, "POSECOUNT", m_nPoseCount, m_strCommondh);

		for (int i = 0; i < m_nPoseCount; i++)
		{
			strKeyName.Format("PIXELPOSE%d", i);
			WritePrivateProfileDouble(lpAppName, strKeyName, m_hvColPose[i].D(), m_strCommondh);
		}
		for (int i = 0; i < m_nPoseCount; i++)
		{
			strKeyName.Format("XPOSE%d", i);
			WritePrivateProfileDouble(lpAppName, strKeyName, m_hvXPose[i].D(), m_strCommondh);
		}

		for (int i = 0; i < m_hvRes.Length(); i++)
		{

			strKeyName.Format("PartRes%d", i);
			WritePrivateProfileDouble(lpAppName, strKeyName, m_hvRes[i].D(), m_strCommondh);
		}
		lpAppName.Format("CALIBRATION%d", m_tInit.nGrabberSN);
		WritePrivateProfileDouble(lpAppName, "CALSPACEDIST", m_dCalSpaceDist, m_strCommondh);
		WritePrivateProfileDouble(lpAppName, "CORCOEF", m_dCorCoef, m_strCommondh);
		/////////////////////////////////////

	}
	catch (HException e)
	{
		AfxMessageBox("参数保存异常");
	}
}
BOOL IsVertical(HObject Region)
{
	HTuple hv_Phi, hv_Deg;
	OrientationRegion(Region, &hv_Phi);
	TupleDeg(hv_Phi, &hv_Deg);
	if (0 != (HTuple(HTuple(hv_Deg > 45).TupleAnd(hv_Deg<135)).TupleOr(HTuple(hv_Deg>-135).TupleAnd(hv_Deg < -45))))
	{
		return TRUE;
	}
	return FALSE;
}

int CSeparate::Init(SEPARATEINIT &tIn)                  //初始化算法
{
	//SetSystem("int_zooming","false");
	//SetSystem("store_empty_region", "false");
	//SetHcppInterfaceStringEncodingIsUtf8(FALSE);
	//SetSystem("filename_encoding", "locale");
	//SetSystem("read_halcon_files_encoding_fallback", "locale");
	//SetSystem("read_halcon_files_encoding_fallback", "locale");
	//SetSystem("write_halcon_files_encoding", "locale");
	//SetSystem("tsp_tuple_string_operator_mode", "byte");
	//SetSystem("tuple_string_operator_mode", "byte");

	m_tInit = tIn;
	CString strFilePath;
	strFilePath.Format("%s//Config//System.vis", GetCurrentAppPath());
	m_bReFindLine = GetPrivateProfileInt(m_tInit.strProductName, "FindLine", 1, strFilePath);

	ReadParam();//读取检测参数
	ReadLineMatch();
	return TRUE;
}

void CSeparate::ReadLineMatch()//读取线配对
{


	m_strFilePath.Format("%s\\Config\\Products\\%s\\SCISeparate\\%d", GetCurrentAppPath(), m_tInit.strProductName,
		m_tInit.nGrabberSN);
	m_tLineMatch.nLineUsed = GetPrivateProfileInt("LineMatch", "LineUsed", 4, m_strMyConfig);

	if (m_tInit.nCoatType == M_TwoCam_Intermission)
	{
		m_tLineMatch.nLineUsed = 7;
	}
	for (int i = 0; i < m_tLineMatch.nLineUsed; i++)
	{
		CString strFileName;
		strFileName.Format("%s\\%d.ini", m_strFilePath, i);
		m_sLineTool[i].ReadParam(strFileName);
	}

	m_tLineMatch.nCoatCount = GetPrivateProfileInt("LineMatch", "CoatCount", 1, m_strMyConfig);
	m_tLineMatch.nFoilCount = GetPrivateProfileInt("LineMatch", "FoilCount", 2, m_strMyConfig);
	m_tLineMatch.nCeramicCount = GetPrivateProfileInt("LineMatch", "CeramicCount", 0, m_strMyConfig);
	m_tLineMatch.nStaggerCount = GetPrivateProfileInt("LineMatch", "StaggerCount", 0, m_strMyConfig);

	m_tLineMatch.nCoatLeftStartIndex = GetPrivateProfileInt("LineMatch", "nCoatLeftStartIndex", 0, m_strMyConfig);
	m_tLineMatch.nFoilLeftStartIndex = GetPrivateProfileInt("LineMatch", "nFoilLeftStartIndex", 0, m_strMyConfig);
	m_tLineMatch.nCeramicLeftStartIndex = GetPrivateProfileInt("LineMatch", "nCeramicLeftStartIndex", 0, m_strMyConfig);
	m_tLineMatch.vCoat1.clear();
	m_tLineMatch.vCoat2.clear();
	m_tLineMatch.vFoil1.clear();
	m_tLineMatch.vFoil2.clear();
	m_tLineMatch.vCeramic1.clear();
	m_tLineMatch.vCeramic2.clear();
	m_tLineMatch.vStagger1.clear();
	m_tLineMatch.vStagger2.clear();

	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Coat%d", i);
		m_tLineMatch.vCoat1.push_back(GetPrivateProfileInt(lpAppName, "Index1", 1, m_strMyConfig));
		m_tLineMatch.vCoat2.push_back(GetPrivateProfileInt(lpAppName, "Index2", 1, m_strMyConfig));
		m_tLineMatch.vCoatWidthMin.push_back(GetPrivateProfileDouble(lpAppName, "WidthMin", 1, m_strMyConfig));
		m_tLineMatch.vCoatWidthMax.push_back(GetPrivateProfileDouble(lpAppName, "WidthMax", 1, m_strMyConfig));
		m_tLineMatch.vCoatWidthOffset.push_back(GetPrivateProfileDouble(lpAppName, "WidthMaxOffset", 1, m_strMyConfig));
	}
	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Foil%d", i);
		m_tLineMatch.vFoil1.push_back(GetPrivateProfileInt(lpAppName, "Index1", 2, m_strMyConfig));
		m_tLineMatch.vFoil2.push_back(GetPrivateProfileInt(lpAppName, "Index2", 2, m_strMyConfig));
		m_tLineMatch.vFoilWidthMin.push_back(GetPrivateProfileDouble(lpAppName, "WidthMin", 1, m_strMyConfig));
		m_tLineMatch.vFoilWidthMax.push_back(GetPrivateProfileDouble(lpAppName, "WidthMax", 1, m_strMyConfig));
		m_tLineMatch.vFoilWidthOffset.push_back(GetPrivateProfileDouble(lpAppName, "WidthMaxOffset", 1, m_strMyConfig));
	}

	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Ceramic%d", i);
		m_tLineMatch.vCeramic1.push_back(GetPrivateProfileInt(lpAppName, "Index1", 2, m_strMyConfig));
		m_tLineMatch.vCeramic2.push_back(GetPrivateProfileInt(lpAppName, "Index2", 2, m_strMyConfig));
		m_tLineMatch.vCeramicWidthMin.push_back(GetPrivateProfileDouble(lpAppName, "WidthMin", 1, m_strMyConfig));
		m_tLineMatch.vCeramicWidthMax.push_back(GetPrivateProfileDouble(lpAppName, "WidthMax", 1, m_strMyConfig));
		m_tLineMatch.vCeramicWidthOffset.push_back(GetPrivateProfileDouble(lpAppName, "WidthMaxOffset", 1, m_strMyConfig));
	}

	for (int i = 0; i < MAX_LINEMATCH * 3; i++)
	{
		CString lpAppName;
		lpAppName.Format("Stagger%d", i);
		m_tLineMatch.vStagger1.push_back(GetPrivateProfileInt(lpAppName, "Index1", 2, m_strMyConfig));
		m_tLineMatch.vStagger2.push_back(GetPrivateProfileInt(lpAppName, "Index2", 2, m_strMyConfig));
		m_tLineMatch.vStaggerOffset.push_back(GetPrivateProfileDouble(lpAppName, "StaggerOffset", 0, m_strMyConfig));
	}
}
void CSeparate::SaveLineMatch()//保存线配对
{

	if (m_tInit.nCoatType == M_TwoCam_Intermission)
	{
		m_tLineMatch.nLineUsed = 7;
		for (int i = 0; i < m_tLineMatch.nLineUsed; i++)
		{
			CString strFileName;
			strFileName.Format("%s\\%d.ini", m_strFilePath, i);
			m_sLineTool[i].WriteParam(strFileName);
		}

	}



	WritePrivateProfileInt("LineMatch", "LineUsed", m_tLineMatch.nLineUsed, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "CoatCount", m_tLineMatch.nCoatCount, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "FoilCount", m_tLineMatch.nFoilCount, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "CeramicCount", m_tLineMatch.nCeramicCount, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "StaggerCount", m_tLineMatch.nStaggerCount, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "nCoatLeftStartIndex", m_tLineMatch.nCoatLeftStartIndex, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "nFoilLeftStartIndex", m_tLineMatch.nFoilLeftStartIndex, m_strMyConfig);
	WritePrivateProfileInt("LineMatch", "nCeramicLeftStartIndex", m_tLineMatch.nCeramicLeftStartIndex, m_strMyConfig);


	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Coat%d", i);
		WritePrivateProfileInt(lpAppName, "Index1", m_tLineMatch.vCoat1.at(i), m_strMyConfig);
		WritePrivateProfileInt(lpAppName, "Index2", m_tLineMatch.vCoat2.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMin", m_tLineMatch.vCoatWidthMin.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMax", m_tLineMatch.vCoatWidthMax.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMaxOffset", m_tLineMatch.vCoatWidthOffset.at(i), m_strMyConfig);
	}
	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Foil%d", i);
		WritePrivateProfileInt(lpAppName, "Index1", m_tLineMatch.vFoil1.at(i), m_strMyConfig);
		WritePrivateProfileInt(lpAppName, "Index2", m_tLineMatch.vFoil2.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMin", m_tLineMatch.vFoilWidthMin.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMax", m_tLineMatch.vFoilWidthMax.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMaxOffset", m_tLineMatch.vFoilWidthOffset.at(i), m_strMyConfig);
	}

	for (int i = 0; i < MAX_LINEMATCH; i++)
	{
		CString lpAppName;
		lpAppName.Format("Ceramic%d", i);
		WritePrivateProfileInt(lpAppName, "Index1", m_tLineMatch.vCeramic1.at(i), m_strMyConfig);
		WritePrivateProfileInt(lpAppName, "Index2", m_tLineMatch.vCeramic2.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMin", m_tLineMatch.vCeramicWidthMin.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMax", m_tLineMatch.vCeramicWidthMax.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "WidthMaxOffset", m_tLineMatch.vCeramicWidthOffset.at(i), m_strMyConfig);
	}

	for (int i = 0; i < MAX_LINEMATCH * 3; i++)
	{
		CString lpAppName;
		lpAppName.Format("Stagger%d", i);
		WritePrivateProfileInt(lpAppName, "Index1", m_tLineMatch.vStagger1.at(i), m_strMyConfig);
		WritePrivateProfileInt(lpAppName, "Index2", m_tLineMatch.vStagger2.at(i), m_strMyConfig);
		WritePrivateProfileDouble(lpAppName, "StaggerOffset", m_tLineMatch.vStaggerOffset.at(i), m_strMyConfig);
	}
}

//BOOL CSeparate::DeterMinationSize(int nIndex,int nSizeType,double dWidth)//尺寸判定
//{
//	CString strErrorDescription;
//	//if (0 == nSizeType)///判断类型
//	//{
//	//	if (dWidth > m_tLineMatch.vCoatWidthMax[nIndex])
//	//	{
//	//		
//	//		strErrorDescription.Format("第%d条涂布宽度偏大",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Large));
//	//		return FALSE;
//	//	}else if (dWidth < m_tLineMatch.vCoatWidthMin[nIndex])
//	//	{
//	//		strErrorDescription.Format("第%d条涂布宽度偏小",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Small));
//	//		return FALSE;
//	//	}else
//	//	{
//	//		strErrorDescription.Format("第%d条涂布宽度OK",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Normal));
//	//		return TRUE;
//	//	}
//	//}
//	//else if(1 == nSizeType)
//	//{
//	//	if (dWidth > m_tLineMatch.vFoilWidthMax[nIndex])
//	//	{
//	//		strErrorDescription.Format("第%d条极耳宽度偏大",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Large));
//	//		return FALSE;
//	//	}else if (dWidth < m_tLineMatch.vFoilWidthMin[nIndex])
//	//	{
//	//		strErrorDescription.Format("第%d条极耳宽度偏小",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Small));
//	//		return FALSE;
//	//	}else
//	//	{
//	//		strErrorDescription.Format("第%d条极耳宽度OK",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Normal));
//	//		return TRUE;
//	//	}
//	//}
//	//else 
//	//{
//	//	if (dWidth > m_tLineMatch.vCeramicWidthMax[nIndex])
//	//	{
//	//		strErrorDescription.Format("第%d条陶瓷边偏大",nIndex);
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Large));
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		return FALSE;
//	//	}else if (dWidth < m_tLineMatch.vCeramicWidthMin[nIndex])
//	//	{
//	//		strErrorDescription.Format("第%d条陶瓷边偏小",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Small));
//	//		return FALSE;
//	//	}else
//	//	{
//	//		strErrorDescription.Format("第%d条陶瓷边OK",nIndex);
//	//		m_ptOut->hv_ErrorDescription.Append(HTuple(strErrorDescription));
//	//		m_ptOut->hv_ErrorFlag.Append(HTuple(Normal));
//	//		return TRUE;
//	//	}
//	//}
//
//
//	return TRUE;	
//}


//void CSeparate::GetWidth (HObject ho_Image, HObject *ho_RegionTrans, HTuple *hv_Width)
//{
//
//	// Local iconic variables
//	HObject  ho_Rectangle, ho_ImageReduced, ho_ImageOpening;
//	HObject  ho_Region, ho_RegionClosing, ho_ConnectedRegions;
//	HObject  ho_SelectedRegions;
//
//	// Local control variables
//	HTuple  hv_Height, hv_UsedThreshold;
//
//	GetImageSize(ho_Image, &(*hv_Width), &hv_Height);
//	GenRectangle1(&ho_Rectangle, 50, 0, 100, (*hv_Width));
//	ReduceDomain(ho_Image, ho_Rectangle, &ho_ImageReduced);
//	GrayOpeningRect(ho_ImageReduced, &ho_ImageOpening, 2, 15);
//	BinaryThreshold(ho_ImageOpening, &ho_Region, "max_separability", "light", &hv_UsedThreshold);
//	ClosingCircle(ho_Region, &ho_RegionClosing, 3.5);
//	Connection(ho_RegionClosing, &ho_ConnectedRegions);
//	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
//	ShapeTrans(ho_SelectedRegions, &(*ho_RegionTrans), "rectangle1");
//	RegionFeatures((*ho_RegionTrans), "width", &(*hv_Width));
//	return;
//}
void CSeparate::GenFitAT9Region(HObject* hoRegion, HTuple hvLineIndex)//测试
{
	try
	{
		HTuple hv_StartRow, hv_StartColumn, hv_EndRow, hv_EndColumn;
		HTuple hv_ImageWidth, hv_ImageHeight;
		GetImageSize(m_hoPretreatImage, &hv_ImageWidth, &hv_ImageHeight);
		hv_StartRow.Append(0);
		hv_StartColumn.Append(0);
		hv_EndRow.Append(0);
		hv_EndColumn.Append(hv_ImageWidth - 1);
		hv_StartRow.Append(m_hvRowBegin[hvLineIndex[0].I()]);
		hv_StartColumn.Append(m_hvColBegin[hvLineIndex[0].I()]);
		hv_EndRow.Append(m_hvRowEnd[hvLineIndex[0].I()]);
		hv_EndColumn.Append(m_hvColEnd[hvLineIndex[0].I()]);
		hv_StartRow.Append(hv_ImageHeight - 1);
		hv_StartColumn.Append(0);
		hv_EndRow.Append(hv_ImageHeight - 1);
		hv_EndColumn.Append(hv_ImageWidth - 1);
		{
			hv_StartRow.Append(m_hvRowBegin[hvLineIndex[1].I()]);
			hv_StartColumn.Append(m_hvColBegin[hvLineIndex[1].I()]);
			hv_EndRow.Append(m_hvRowEnd[hvLineIndex[1].I()]);
			hv_EndColumn.Append(m_hvColEnd[hvLineIndex[1].I()]);
		}
		HTuple hvIndex;
		int  nLength;
		hvIndex.Append(0).Append(1).Append(2).Append(3);
		nLength = hvIndex.Length();
		HTuple RowBegin, ColBegin, RowEnd, ColEnd;
		for (Hlong i = 0; i < nLength; i++)
		{
			int nIndex, nIndex1, nIndex2;//该序号前一个以及后一个
			nIndex = hvIndex[i];
			if (0 == i)//第一个点
			{
				nIndex1 = hvIndex[nLength - 1];
				nIndex2 = hvIndex[i + 1];
			}
			else if (nLength - 1 == i)//最后一个点
			{
				nIndex1 = hvIndex[i - 1];
				nIndex2 = hvIndex[0];
			}
			else//中间的点
			{
				nIndex1 = hvIndex[i - 1];
				nIndex2 = hvIndex[i + 1];
			}
			HTuple Row1, Row2, Column1, Column2;
			IntersectionLl(hv_StartRow[nIndex], hv_StartColumn[nIndex], hv_EndRow[nIndex], hv_EndColumn[nIndex], hv_StartRow[nIndex1], hv_StartColumn[nIndex1], hv_EndRow[nIndex1], hv_EndColumn[nIndex1], &Row1, &Column1, NULL);
			IntersectionLl(hv_StartRow[nIndex], hv_StartColumn[nIndex], hv_EndRow[nIndex], hv_EndColumn[nIndex], hv_StartRow[nIndex2], hv_StartColumn[nIndex2], hv_EndRow[nIndex2], hv_EndColumn[nIndex2], &Row2, &Column2, NULL);
			HTuple hv_SR, hv_SC, hv_ER, hv_EC;
			hv_SR = MIN(MAX(Row1[0].D(), 0), hv_ImageHeight[0].D() - 1);
			hv_SC = MIN(MAX(Column1[0].D(), 0), hv_ImageWidth[0].D() - 1);
			hv_ER = MIN(MAX(Row2[0].D(), 0), hv_ImageHeight[0].D() - 1);
			hv_EC = MIN(MAX(Column2[0].D(), 0), hv_ImageWidth[0].D() - 1);
			RowBegin.Append(hv_SR);
			ColBegin.Append(hv_SC);
			RowEnd.Append(hv_ER);
			ColEnd.Append(hv_EC);
		}
		HObject RegionLines, RegionUnion, RegionClosing, RegionFillUp;
		GenRegionLine(&RegionLines, RowBegin, ColBegin, RowEnd, ColEnd);
		Union1(RegionLines, &RegionUnion);
		ClosingRectangle1(RegionUnion, &RegionClosing, 50, 50);
		FillUp(RegionClosing, &RegionFillUp);
		ShapeTrans(RegionFillUp, hoRegion, "convex");
	}
	catch (HException&e)
	{
	}
}
void CSeparate::GetSize(int nSourceLineIndex, int nDetLineIndex, double dResolution, HTuple* hv_Value)
{
	HTuple hv_ErrorLineIndex0, hv_ErrorLineIndex1;
	TupleFind(m_hvErrorLineIndex, nSourceLineIndex, &hv_ErrorLineIndex0);
	TupleFind(m_hvErrorLineIndex, nDetLineIndex, &hv_ErrorLineIndex1);
	if (hv_ErrorLineIndex0.Length() > 0 && hv_ErrorLineIndex0 != -1 || hv_ErrorLineIndex1.Length() > 0 && hv_ErrorLineIndex1 != -1)
	{
		*hv_Value = 9999;

	}
	else
	{

		{
			HTuple hv_Distance1, hv_Distance2;

			DistancePl(m_hvFitLineSR[nSourceLineIndex], m_hvFitLineSC[nSourceLineIndex], m_hvFitLineSR[nDetLineIndex], m_hvFitLineSC[nDetLineIndex], m_hvFitLineER[nDetLineIndex], m_hvFitLineEC[nDetLineIndex], &hv_Distance1);
			DistancePl(m_hvFitLineER[nSourceLineIndex], m_hvFitLineEC[nSourceLineIndex], m_hvFitLineSR[nDetLineIndex], m_hvFitLineSC[nDetLineIndex], m_hvFitLineER[nDetLineIndex], m_hvFitLineEC[nDetLineIndex], &hv_Distance2);
			/*		DistancePl(m_hvRowBegin[nSourceLineIndex],m_hvColBegin[nSourceLineIndex],m_hvRowBegin[nDetLineIndex],m_hvColBegin[nDetLineIndex],m_hvRowEnd[nDetLineIndex],m_hvColEnd[nDetLineIndex],&hv_Distance1);
					DistancePl(m_hvRowEnd[nSourceLineIndex],m_hvColEnd[nSourceLineIndex],m_hvRowBegin[nDetLineIndex],m_hvColBegin[nDetLineIndex],m_hvRowEnd[nDetLineIndex],m_hvColEnd[nDetLineIndex],&hv_Distance2);*/
			*hv_Value = (hv_Distance1 + hv_Distance2) / 2 * dResolution;
		}
	}



}





void CSeparate::GetCaoWeiInfo(SEPARATEOUTPUT* ptOut)//获取槽位的信息
{
	try
	{
		//修改
		for (int i = 0; i < m_tMeausreParam.hoCaoweiRegions.CountObj(); i++)
		{

			HObject hoSelectObj, hoReducedImage, hoCropPart, hoCropRegion, hoCropSelectRegions;
			HTuple hvLeftTopRow, hvLeftTopColumn, hvRightBotRow, hvRightBotColumn;
			SelectObj(m_tMeausreParam.hoCaoweiRegions, &hoSelectObj, i + 1);
			SmallestRectangle1(hoSelectObj, &hvLeftTopRow, &hvLeftTopColumn, &hvRightBotRow, &hvRightBotColumn);
			ReduceDomain(m_hoPretreatImage, hoSelectObj, &hoReducedImage);
			CropDomain(hoReducedImage, &hoCropPart);
			Threshold(hoCropPart, &hoCropRegion, 150, 255);
			FillUp(hoCropRegion, &hoCropRegion);
			Connection(hoCropRegion, &hoCropRegion);
			SelectShapeStd(hoCropRegion, &hoCropSelectRegions, "max_area", 70);
			SelectShape(hoCropSelectRegions, &hoCropSelectRegions, HTuple("area").Append("inner_radius"), "and", HTuple(150).Append(10), HTuple(INT_MAX).Append(INT_MAX));

			if (hoCropSelectRegions.CountObj() == 1)
			{
				MoveRegion(hoCropSelectRegions, &hoCropSelectRegions, hvLeftTopRow, hvLeftTopColumn);
				HObject hoRegion, hoRegionContour;
				GenContourRegionXld(hoCropSelectRegions, &hoRegionContour, "border");
				HTuple hvRowCenter, hvColumnCenter, hvPhi, hvLength1, hvLength2, hvPointOrder;
				FitRectangle2ContourXld(hoRegionContour, "regression", -1, 0, 0, 3, 2, &hvRowCenter, &hvColumnCenter, &hvPhi, &hvLength1, &hvLength2, &hvPointOrder);

				//计算清洗槽的
				HTuple LeftX, LeftY;
				HTuple TopX, TopY;
				HTuple RightX, RightY;
				HTuple BottomX, BottomY;
				//左中点
				LeftX = (hvColumnCenter - hvPhi.TupleCos()*hvLength1)*m_dColResolution;
				LeftY = (hvRowCenter + hvPhi.TupleSin()*hvLength1)*m_dRowResolution;
				//右中点
				RightX = (hvColumnCenter + hvPhi.TupleCos()*hvLength1)*m_dColResolution;
				RightY = (hvRowCenter - hvPhi.TupleSin()*hvLength1)*m_dRowResolution;
				//上中点
				TopX = (hvColumnCenter - hvPhi.TupleSin()*hvLength2)*m_dColResolution;
				TopY = (hvRowCenter - hvPhi.TupleCos()*hvLength2)*m_dRowResolution;
				//下中点
				BottomX = (hvColumnCenter + hvPhi.TupleSin()*hvLength2)*m_dColResolution;
				BottomY = (hvRowCenter + hvPhi.TupleCos()*hvLength2)*m_dRowResolution;
				//计算宽度
				HTuple hvWidthValue, hvHeightValue;
				DistancePp(LeftY, LeftX, RightY, RightX, &hvWidthValue);
				DistancePp(TopY, TopX, BottomY, BottomX, &hvHeightValue);
				m_ptOut->hv_TripSNTuple[i] = i;
				m_ptOut->hv_CaoWeiWidthTuple[i] = hvWidthValue;       //计算槽位宽度
				m_ptOut->hv_CaoWeiHeightTuple[i] = hvHeightValue;       //计算槽位宽度
				m_ptOut->hv_CaoWeiXPositionTuple[i] = RightX / m_dColResolution;
				/*m_ptOut->hv_CaoWeiYpositionTuple[i]=hvRowCenter;*/
				m_ptOut->hv_CaoWeiYpositionTuple[i] = BottomY / m_dRowResolution;//3-19根据付工更改标准
				ConcatObj(m_ptOut->ho_ExtractCaoWeiRegions, hoCropSelectRegions, &m_ptOut->ho_ExtractCaoWeiRegions);
			}

		}
	}
	catch (HException& e)
	{

	}
}


void CSeparate::SetLineMatch()//设置区域
{
	CorrectedPoint();//修正点位
	//将涂布区域和极耳区域传出
	HObject Rectangle;
	double dWidth, dWidth1;
	HTuple Row1, Column1, Row2, Column2;
	HTuple hvwidth, hvheight;
	GetImageSize(m_hoGrabImage, &hvwidth, &hvheight);
	if (M_TwoCam_Continue == m_tInit.nCoatType)//如果是连续涂布的话
	{
		int nLineUsed = m_tLineMatch.nCoatCount + m_tLineMatch.nFoilCount + m_tLineMatch.nCeramicCount + 1;
		m_ptOut->hv_LeftEadgePixel = (m_hvColBegin[0] + m_hvColEnd[0]) / 2;
		m_ptOut->hv_RightEadgePixel = (m_hvColBegin[nLineUsed - 1] + m_hvColEnd[nLineUsed - 1]) / 2;
		HObject Domain;
		GetDomain(m_hoPretreatImage, &Domain);//获得感兴趣区域region
		int nIndex1, nIndex2;
		HTuple nIndex, hvWidth, hvErrorFlag, hvWidth1;
		HTuple hvStr;
		for (int i = 0; i < m_tLineMatch.nCoatCount; i++)
		{
			nIndex1 = m_tLineMatch.vCoat1[i];
			nIndex2 = m_tLineMatch.vCoat2[i];
			HTuple Row, Column;
			Row.Append(m_hvRowBegin[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex2]);
			Row.Append(m_hvRowBegin[nIndex2]);
			Column.Append(m_hvColBegin[nIndex1]);
			Column.Append(m_hvColEnd[nIndex1]);
			Column.Append(m_hvColEnd[nIndex2]);
			Column.Append(m_hvColBegin[nIndex2]);
			GenRegionPolygonFilled(&Rectangle, Row, Column);//依据Row，Column里数据顺序依次连接
			Intersection(Rectangle, Domain, &Rectangle);
			HTuple  RowL, RowR, ColumnL, ColumnR;
			IntersectionLl(m_hvRowBegin[nIndex1], m_hvColBegin[nIndex1], m_hvRowEnd[nIndex1], m_hvColEnd[nIndex1], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowL, &ColumnL, NULL);
			IntersectionLl(m_hvRowBegin[nIndex2], m_hvColBegin[nIndex2], m_hvRowEnd[nIndex2], m_hvColEnd[nIndex2], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowR, &ColumnR, NULL);


			if (m_nCalculateChoose == 0)
			{
				dWidth = (ColumnL - ColumnR).TupleAbs()*m_dColResolution;
				dWidth = (dWidth * 1000 + 0.5) / 1000;
			}
			else
			{
				dWidth = GetDistancepl(ColumnL, ColumnR);//采用条纹标定计算
			}

			m_ptOut->hv_CoatWidth[i] = dWidth;

			dWidth1 = std::abs(ColumnR.D() - ColumnL.D());   //计算像素数
			hvWidth1.Append(dWidth1);

			hvWidth.Append(m_ptOut->hv_CoatWidth[i]);
			nIndex.Append(nIndex1);
			CString str;
			str.Format("涂布宽%d", i + 1);
			hvStr.Append(HTuple(str));

			ConcatObj(m_ptOut->ho_Coat, Rectangle, &m_ptOut->ho_Coat);


		}
		for (int i = 0; i < m_tLineMatch.nFoilCount; i++)
		{
			nIndex1 = m_tLineMatch.vFoil1[i];
			nIndex2 = m_tLineMatch.vFoil2[i];
			HTuple Row, Column;
			Row.Append(m_hvRowBegin[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex2]);
			Row.Append(m_hvRowBegin[nIndex2]);
			Column.Append(m_hvColBegin[nIndex1]);
			Column.Append(m_hvColEnd[nIndex1]);
			Column.Append(m_hvColEnd[nIndex2]);
			Column.Append(m_hvColBegin[nIndex2]);
			GenRegionPolygonFilled(&Rectangle, Row, Column);
			Intersection(Rectangle, Domain, &Rectangle);
			HTuple  RowL, RowR, ColumnL, ColumnR;
			IntersectionLl(m_hvRowBegin[nIndex1], m_hvColBegin[nIndex1], m_hvRowEnd[nIndex1], m_hvColEnd[nIndex1], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowL, &ColumnL, NULL);
			IntersectionLl(m_hvRowBegin[nIndex2], m_hvColBegin[nIndex2], m_hvRowEnd[nIndex2], m_hvColEnd[nIndex2], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowR, &ColumnR, NULL);


			if (m_nCalculateChoose == 0)
			{
				dWidth = (ColumnL - ColumnR).TupleAbs()*m_dColResolution;
				dWidth = (dWidth * 10 + 0.5) / 10;
			}
			else
			{
				dWidth = GetDistancepl(ColumnL, ColumnR);//采用条纹标定计算
			}

			m_ptOut->hv_FoilWidth[i] = dWidth;
			hvWidth.Append(m_ptOut->hv_FoilWidth[i]);
			dWidth1 = std::abs(ColumnR.D() - ColumnL.D());
			hvWidth1.Append(dWidth1);


			nIndex.Append(nIndex1);
			CString str;
			str.Format("极耳宽%d", i + 1);
			hvStr.Append(HTuple(str));

			ConcatObj(m_ptOut->ho_Foil, Rectangle, &m_ptOut->ho_Foil);


		}


		HTuple hvColumnTuple;
		hvColumnTuple.Clear();
		for (int i = 0; i < m_tLineMatch.nCeramicCount; i++)
		{
			nIndex1 = m_tLineMatch.vCeramic1[i];
			nIndex2 = m_tLineMatch.vCeramic2[i];
			HTuple Row, Column;
			Row.Append(m_hvRowBegin[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex1]);
			Row.Append(m_hvRowEnd[nIndex2]);
			Row.Append(m_hvRowBegin[nIndex2]);
			Column.Append(m_hvColBegin[nIndex1]);
			Column.Append(m_hvColEnd[nIndex1]);
			Column.Append(m_hvColEnd[nIndex2]);
			Column.Append(m_hvColBegin[nIndex2]);
			GenRegionPolygonFilled(&Rectangle, Row, Column);
			Intersection(Rectangle, Domain, &Rectangle);
			HTuple  RowL, RowR, ColumnL, ColumnR;
			IntersectionLl(m_hvRowBegin[nIndex1], m_hvColBegin[nIndex1], m_hvRowEnd[nIndex1], m_hvColEnd[nIndex1], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowL, &ColumnL, NULL);
			/*HObject ho,ho1,ho2,ho3;
			GenRegionLine(&ho,m_hvRowBegin[nIndex1], m_hvColBegin[nIndex1], m_hvRowEnd[nIndex1], m_hvColEnd[nIndex1]);
			GenRegionLine(&ho1, hvheight / 2, 0, hvheight / 2, hvwidth - 1);*/

			IntersectionLl(m_hvRowBegin[nIndex2], m_hvColBegin[nIndex2], m_hvRowEnd[nIndex2], m_hvColEnd[nIndex2], hvheight / 2, 0, hvheight / 2, hvwidth - 1, &RowR, &ColumnR, NULL);
			/*GenRegionLine(&ho2, m_hvRowBegin[nIndex2], m_hvColBegin[nIndex2], m_hvRowEnd[nIndex2], m_hvColEnd[nIndex2]);
			GenRegionLine(&ho3, hvheight / 2, 0, hvheight / 2, hvwidth - 1);*/


			hvColumnTuple.Append(ColumnL);
			hvColumnTuple.Append(ColumnR);
			if (m_nCalculateChoose == 0)
			{
				dWidth = (ColumnL - ColumnR).TupleAbs()*m_dColResolution;
				dWidth = (dWidth * 10 + 0.5) / 10;
			}
			else
			{
				dWidth = GetDistancepl(ColumnL, ColumnR);//采用条纹标定计算
			}

			m_ptOut->hv_CeramicWidth[i] = dWidth;
			hvWidth.Append(m_ptOut->hv_CeramicWidth[i]);

			dWidth1 = std::abs(ColumnR.D() - ColumnL.D());
			hvWidth1.Append(dWidth1);
			nIndex.Append(nIndex1);
			CString str;
			str.Format("AT9%d", i + 1);//陶瓷宽
			hvStr.Append(HTuple(str));
			ConcatObj(m_ptOut->ho_Ceramic, Rectangle, &m_ptOut->ho_Ceramic);

		}
		///计算AT9距离边缘的位置
		m_ptOut->hv_AT9EdageToProEdage.Clear();
		if (hvColumnTuple.Length() > 0)
		{
			HTuple hv_LeftHalfTuple, hv_RightHalfTuple;
			int nIndex = hvColumnTuple.Length() / 2;
			TupleSelectRange(hvColumnTuple, 0, nIndex - 1, &hv_LeftHalfTuple);
			TupleSelectRange(hvColumnTuple, nIndex, hvColumnTuple.Length() - 1, &hv_RightHalfTuple);
			if (m_nCalculateChoose == 0)
			{
				if (hv_LeftHalfTuple.Length() > 0)
				{

					m_ptOut->hv_AT9EdageToProEdage.Append((hv_LeftHalfTuple - m_ptOut->hv_LeftEadgePixel)*m_dColResolution);
				}
				if (hv_RightHalfTuple.Length() > 0)
				{
					m_ptOut->hv_AT9EdageToProEdage.Append((m_ptOut->hv_RightEadgePixel - hv_RightHalfTuple)*m_dColResolution);
				}
			}
			else
			{
				if (hv_LeftHalfTuple.Length() > 0)
				{
					for (int i = 0; i < hv_LeftHalfTuple.Length(); i++)
					{
						dWidth = GetDistancepl(m_ptOut->hv_LeftEadgePixel, hv_LeftHalfTuple[i]);//采用条纹标定计算
						m_ptOut->hv_AT9EdageToProEdage.Append(dWidth);
					}

				}
				if (hv_RightHalfTuple.Length() > 0)
				{
					for (int i = 0; i < hv_LeftHalfTuple.Length(); i++)
					{
						dWidth = GetDistancepl(hv_RightHalfTuple[i], m_ptOut->hv_RightEadgePixel);//采用条纹标定计算
						m_ptOut->hv_AT9EdageToProEdage.Append(dWidth);
					}

				}
			}

		}
		//产品OK、NG标志
		m_ptOut->hv_Flag = TRUE;
		//按照显示进行排序
		nIndex = nIndex.TupleSortIndex();
		TupleSelect(hvWidth, nIndex, &m_ptOut->hv_SortWidth);
		TupleSelect(hvStr, nIndex, &m_ptOut->hv_Name);

		//计算GRR，每一幅图出20组数据
		int k = 0;
		HTuple hv_GrrEdageToProEdage;
		for (int i = 0; i < m_tStandardSizes.hvTripNum.I() / 2; i++)
		{

			HTuple hvGRRCol1, hvGRRCol2;
			hvGRRCol1 = m_hvec_GRRpoint[i + 2 + k].T();
			hvGRRCol2 = m_hvec_GRRpoint[i + 3 + k].T();
			hv_GrrEdageToProEdage.Append(hvGRRCol1);
			hv_GrrEdageToProEdage.Append(hvGRRCol2);
			k = k + 3;
		}
		if (hv_GrrEdageToProEdage.Length() > 0)
		{
			HTuple hv_LeftHalfTuple, hv_RightHalfTuple;
			int nIndex = hv_GrrEdageToProEdage.Length() / 2;
			TupleSelectRange(hv_GrrEdageToProEdage, 0, nIndex - 1, &hv_LeftHalfTuple);
			TupleSelectRange(hv_GrrEdageToProEdage, nIndex, hv_GrrEdageToProEdage.Length() - 1, &hv_RightHalfTuple);
			if (m_nCalculateChoose == 0)
			{
				if (hv_LeftHalfTuple.Length() > 0)
				{
					m_ptOut->hv_CpkEdageToProEdage.Append((hv_LeftHalfTuple - m_ptOut->hv_LeftEadgePixel)*m_dColResolution);
				}
				if (hv_RightHalfTuple.Length() > 0)
				{
					m_ptOut->hv_CpkEdageToProEdage.Append((m_ptOut->hv_RightEadgePixel - hv_RightHalfTuple)*m_dColResolution);
				}
			}
			else
			{
				if (hv_LeftHalfTuple.Length() > 0)
				{
					for (int i = 0; i < hv_LeftHalfTuple.Length(); i++)
					{
						dWidth = GetDistancepl(m_ptOut->hv_LeftEadgePixel, hv_LeftHalfTuple[i]);//采用条纹标定计算
						m_ptOut->hv_CpkEdageToProEdage.Append(dWidth);
					}

				}
				if (hv_RightHalfTuple.Length() > 0)
				{
					for (int i = 0; i < hv_LeftHalfTuple.Length(); i++)
					{
						dWidth = GetDistancepl(hv_RightHalfTuple[i], m_ptOut->hv_RightEadgePixel);//采用条纹标定计算
						m_ptOut->hv_CpkEdageToProEdage.Append(dWidth);
					}

				}
			}

		}
		m_hvec_GRRpoint.Clear();

	}
	else///间歇料
	{
		try
		{
			/* 6->5->1->4代表涂布 7->3->0->2代表极耳*/
			HTuple hv_ImageWidth, hv_ImageHeight;
			HObject hoCoat, hoFoil;
			HTuple hv_WidthCoat, hv_HeightCoat;
			HTuple hv_WidthFoil, hv_HeightFoil;
			HTuple hv_LocateMiss, hv_BaiBian;
			HTuple hv_Distance1, hv_Distance2;
			//计算涂布的总宽和高度
			GetImageSize(m_hoPretreatImage, &hv_ImageWidth, &hv_ImageHeight);
			hv_HeightCoat = hv_ImageHeight*m_dRowResolution;
			try
			{
				InterSectionLl(HTuple(3).Append(0).Append(1).Append(2));
				GenRegion(&hoCoat, HTuple(3).Append(0).Append(1).Append(2));
			}
			catch (HException& e)
			{
				GenRectangle1(&hoCoat, 0, 0, 200, 200);
			}
			GetSize(0, 2, m_dColResolution, &hv_WidthCoat);
			hv_BaiBian = hv_WidthCoat;
			///计算极耳的宽度和高度
			HTuple hv_InterRowBegin, hv_InterColBegin, hv_InterRowEnd, hv_InterColEnd;
			IntersectionLl(m_hvRowBegin[0], m_hvColBegin[0], m_hvRowEnd[0], m_hvColEnd[0], m_hvRowBegin[3], m_hvColBegin[3], m_hvRowEnd[3], m_hvColEnd[3], &hv_InterRowBegin, &hv_InterColBegin, NULL);
			IntersectionLl(m_hvRowBegin[2], m_hvColBegin[2], m_hvRowEnd[2], m_hvColEnd[2], m_hvRowBegin[3], m_hvColBegin[3], m_hvRowEnd[3], m_hvColEnd[3], &hv_InterRowEnd, &hv_InterColEnd, NULL);
			DistancePp(hv_InterRowBegin, hv_InterColBegin, hv_InterRowEnd, hv_InterColEnd, &hv_Distance2);
			hv_HeightFoil = hv_Distance2*m_dRowResolution;
			GetSize(1, 3, m_dColResolution, &hv_WidthFoil);
			try
			{
				InterSectionLl(HTuple(4).Append(0).Append(3).Append(2));
				GenRegion(&hoFoil, HTuple(4).Append(0).Append(3).Append(2));
			}
			catch (HException& e)
			{
				GenRectangle1(&hoFoil, 0, 0, 200, 200);
			}
			//涂布的四个边的位置
			m_ptOut->hv_RightEadgePixel = (m_hvFitLineSC[6] + m_hvFitLineEC[6]) / 2;
			m_ptOut->hv_LeftEadgePixel = (m_hvFitLineSC[5] + m_hvFitLineEC[5]) / 2;
			m_ptOut->hv_TopEdagePixel = MIN((m_hvFitLineSR[3].D() + m_hvFitLineER[3].D()) / 2, (m_hvFitLineSR[1].D() + m_hvFitLineER[1].D()) / 2);
			m_ptOut->hv_BotEdagePixel = MAX((m_hvFitLineSR[3].D() + m_hvFitLineER[3].D()) / 2, (m_hvFitLineSR[1].D() + m_hvFitLineER[1].D()) / 2);

			if (m_bExistCaoWeiFlag)
			{
				GetCaoWeiInfo(m_ptOut);
			}
			else
			{
				for (int i = 0; i < m_tMeausreParam.hoCaoweiRegions.CountObj(); i++)
				{
					m_ptOut->hv_TripSNTuple.Clear();
					m_ptOut->hv_CaoWeiWidthTuple.Clear();
					m_ptOut->hv_CaoWeiHeightTuple.Clear();//槽位高度
					m_ptOut->hv_CaoWeiXPositionTuple.Clear();//槽位X坐标
					m_ptOut->hv_CaoWeiYpositionTuple.Clear();//槽位Y坐标
				}
			}

			if (m_bExistJXFlag)
			{
				m_ptOut->hv_JXHeight = m_dRowResolution*((m_hvFitLineSR[3] + m_hvFitLineER[3]) / 2 - (m_hvFitLineSR[4] + m_hvFitLineER[4]) / 2).TupleAbs();//间歇高度
				TupleAbs(m_ptOut->hv_JXHeight, &m_ptOut->hv_JXHeight);
				m_ptOut->hv_MPLength = (m_dRowResolution*((m_hvFitLineSR[3] + m_hvFitLineER[3]) / 2 - (m_hvFitLineSR[1] + m_hvFitLineER[1]) / 2)).TupleAbs();//膜长
				m_ptOut->hv_SumLength = m_ptOut->hv_MPLength + m_ptOut->hv_JXHeight;//总长
			}
			else
			{
				m_ptOut->hv_MPLength = hv_ImageHeight*m_dRowResolution;//膜长
				m_ptOut->hv_SumLength = hv_ImageHeight*m_dRowResolution;//总长
				m_ptOut->hv_JXHeight = 0;//间歇高度
			}
			m_ptOut->ho_JXRegion = hoFoil;//间歇区域

			///计算AT9区域
			HTuple hv_LeftAt9WidthValue, hv_RightAt9WidthValue;
			GetSize(0, 5, m_dColResolution, &hv_LeftAt9WidthValue);
			GetSize(2, 6, m_dColResolution, &hv_RightAt9WidthValue);


			m_ptOut->hv_Name.Append("总长");
			m_ptOut->hv_SortWidth.Append(m_ptOut->hv_SumLength);
			m_ptOut->hv_Name.Append("间歇高度");
			m_ptOut->hv_SortWidth.Append(m_ptOut->hv_JXHeight);
			m_ptOut->hv_Name.Append("膜长");
			m_ptOut->hv_SortWidth.Append(m_ptOut->hv_MPLength);
			m_ptOut->hv_Name.Append("左侧AT9宽度");
			m_ptOut->hv_SortWidth.Append(hv_LeftAt9WidthValue);

			m_ptOut->hv_Name.Append("涂膜宽度");
			m_ptOut->hv_SortWidth.Append(hv_WidthCoat);

			m_ptOut->hv_Name.Append("右侧AT9宽度");
			m_ptOut->hv_SortWidth.Append(hv_RightAt9WidthValue);

			HObject hoAt9Region;
			hoAt9Region.GenEmptyObj();

			GenRectangle1(&hoAt9Region, 0, MIN((m_hvFitLineSC[0].D() + m_hvFitLineEC[0].D()) / 2, (m_hvFitLineSC[5].D() + m_hvFitLineEC[5].D()) / 2), hv_ImageHeight - 1, MAX((m_hvFitLineSC[0].D() + m_hvFitLineEC[0].D()) / 2, (m_hvFitLineSC[5].D() + m_hvFitLineEC[5].D()) / 2));
			ConcatObj(m_ptOut->ho_Ceramic, hoAt9Region, &m_ptOut->ho_Ceramic);

			GenRectangle1(&hoAt9Region, 0, MIN((m_hvFitLineSC[2].D() + m_hvFitLineEC[2].D()) / 2, (m_hvFitLineSC[6].D() + m_hvFitLineEC[6].D()) / 2), hv_ImageHeight - 1, MAX((m_hvFitLineSC[2].D() + m_hvFitLineEC[2].D()) / 2, (m_hvFitLineSC[6].D() + m_hvFitLineEC[6].D()) / 2));
			ConcatObj(m_ptOut->ho_Ceramic, hoAt9Region, &m_ptOut->ho_Ceramic);
			/////////////////增加阈值分割测试稳定性
			m_ptOut->hv_Flag = TRUE;
			m_ptOut->hv_CoatWidth = hv_WidthCoat;
			m_ptOut->hv_CeramicWidth.Append(hv_LeftAt9WidthValue).Append(hv_RightAt9WidthValue);
			m_ptOut->hv_FoilWidth = hv_WidthCoat;
			m_ptOut->ho_Coat = hoCoat;
			m_ptOut->ho_Foil = hoFoil;

		}
		catch (HException&e)
		{

		}

	}
}


void CSeparate::PreDetect()
{
	//成员变量初始化
	GenEmptyObj(&m_hoLine);
	m_hvErrorLineIndex.Clear();
	GenEmptyObj(&m_hoCross);
	TupleGenConst(m_tLineMatch.nLineUsed, -1, &m_hvRowBegin);
	TupleGenConst(m_tLineMatch.nLineUsed, -1, &m_hvColBegin);
	TupleGenConst(m_tLineMatch.nLineUsed, -1, &m_hvRowEnd);
	TupleGenConst(m_tLineMatch.nLineUsed, -1, &m_hvColEnd);
	HomMat2dIdentity(&m_ptIn->hvHomMat2D);
	m_ptOut->hv_Name.Clear();
	TupleGenConst(m_tLineMatch.nCoatCount + m_tLineMatch.nFoilCount + m_tLineMatch.nCeramicCount, 0, &m_ptOut->hv_SortWidth);
	TupleGenConst(1, TRUE, &m_ptOut->hv_Flag);
	TupleGenConst(m_tLineMatch.nCoatCount, 0, &m_ptOut->hv_CoatWidth);
	TupleGenConst(m_tLineMatch.nFoilCount, 0, &m_ptOut->hv_FoilWidth);
	TupleGenConst(m_tLineMatch.nCeramicCount, 0, &m_ptOut->hv_CeramicWidth);
	m_ptOut->hv_AT9EdageToProEdage.Clear();
	m_ptOut->hv_CpkEdageToProEdage.Clear();

	///////////槽位的相关尺寸////////////////////////////////
	//输出结构体初始化
	GenEmptyObj(&m_ptOut->ho_ErrorRegion);
	GenEmptyObj(&m_ptOut->ho_Coat);
	GenEmptyObj(&m_ptOut->ho_Foil);
	GenEmptyObj(&m_ptOut->ho_Ceramic);
	GenEmptyObj(&m_ptOut->ho_DispRegion);

	m_ptOut->hv_LeftEadgePixel = 0;
	m_ptOut->hv_RightEadgePixel = 8192;
	m_ptOut->ho_ExtractCaoWeiRegions.GenEmptyObj();//预提取的槽位区域
	TupleGenSequence(0, m_tStandardSizes.hvTripNum - 1, 1, &m_ptOut->hv_TripSNTuple);//槽位隶属于条数
	TupleGenConst(m_tStandardSizes.hvTripNum, 0, &m_ptOut->hv_CaoWeiWidthTuple);//槽位宽度
	TupleGenConst(m_tStandardSizes.hvTripNum, 0, &m_ptOut->hv_CaoWeiHeightTuple);//槽位高度
	TupleGenConst(m_tStandardSizes.hvTripNum, 0, &m_ptOut->hv_CaoWeiXPositionTuple);//槽位X坐标
	TupleGenConst(m_tStandardSizes.hvTripNum, 0, &m_ptOut->hv_CaoWeiYpositionTuple);//槽位Y坐标


	m_ptOut->hv_JXHeight = 0;//间歇高度
	m_ptOut->hv_MPLength = 0;//膜长

	m_ptOut->hv_SumLength = 0;//总长
	m_ptOut->ho_JXRegion.GenEmptyObj();//间歇区域





	HTuple nIndex, hvStr;
	nIndex.Clear();
	hvStr.Clear();
	for (int i = 0; i < m_tLineMatch.nCoatCount; i++)
	{
		int nIndex1 = m_tLineMatch.vCoat1[i];
		nIndex.Append(nIndex1);
		CString strName;
		strName.Format("涂布%d宽", i);
		hvStr.Append(HTuple(strName));
	}
	for (int i = 0; i < m_tLineMatch.nFoilCount; i++)
	{
		int nIndex1 = m_tLineMatch.vFoil1[i];
		nIndex.Append(nIndex1);
		CString strName;
		strName.Format("极耳%d宽", i);
		hvStr.Append(HTuple(strName));
	}
	for (int i = 0; i < m_tLineMatch.nCeramicCount; i++)
	{
		int nIndex1 = m_tLineMatch.vCeramic1[i];
		nIndex.Append(nIndex1);
		CString strName;
		strName.Format("陶瓷%d宽", i);
		hvStr.Append(HTuple(strName));
	}
	nIndex = nIndex.TupleSortIndex();
	for (int i = 0; i < nIndex.Length(); i++)
	{
		m_ptOut->hv_Name.Append(hvStr[nIndex[i].I()]);
	}
}


void CSeparate::InterSectionLl(HTuple hvIndex)
{
	Hlong nLength = hvIndex.Length();
	for (Hlong i = 0; i < nLength; i++)
	{
		int nIndex, nIndex1, nIndex2;//该序号前一个以及后一个
		nIndex = hvIndex[i];
		if (0 == i)//第一个点
		{
			nIndex1 = hvIndex[nLength - 1];
			nIndex2 = hvIndex[i + 1];
		}
		else if (nLength - 1 == i)//最后一个点
		{
			nIndex1 = hvIndex[i - 1];
			nIndex2 = hvIndex[0];
		}
		else//中间的点
		{
			nIndex1 = hvIndex[i - 1];
			nIndex2 = hvIndex[i + 1];
		}
		HTuple Row1, Row2, Column1, Column2;
		IntersectionLl(m_hvRowBegin[nIndex], m_hvColBegin[nIndex], m_hvRowEnd[nIndex], m_hvColEnd[nIndex], m_hvRowBegin[nIndex1], m_hvColBegin[nIndex1], m_hvRowEnd[nIndex1], m_hvColEnd[nIndex1],
			&Row1, &Column1, NULL);
		IntersectionLl(m_hvRowBegin[nIndex], m_hvColBegin[nIndex], m_hvRowEnd[nIndex], m_hvColEnd[nIndex], m_hvRowBegin[nIndex2], m_hvColBegin[nIndex2], m_hvRowEnd[nIndex2], m_hvColEnd[nIndex2],
			&Row2, &Column2, NULL);
		m_hvRowBegin[nIndex] = Row1;
		m_hvColBegin[nIndex] = Column1;
		m_hvRowEnd[nIndex] = Row2;
		m_hvColEnd[nIndex] = Column2;

	}
}
void CSeparate::GenRegion(HObject* Region, HTuple hvIndex)
{
	Hlong nLength = hvIndex.Length();
	HTuple RowBegin, ColBegin, RowEnd, ColEnd;
	for (Hlong i = 0; i < nLength; i++)
	{
		RowBegin.Append(m_hvRowBegin[hvIndex[i].I()]);
		ColBegin.Append(m_hvColBegin[hvIndex[i].I()]);
		RowEnd.Append(m_hvRowEnd[hvIndex[i].I()]);
		ColEnd.Append(m_hvColEnd[hvIndex[i].I()]);
	}
	HObject RegionLines, RegionUnion, RegionClosing, RegionFillUp;
	GenRegionLine(&RegionLines, RowBegin, ColBegin, RowEnd, ColEnd);
	Union1(RegionLines, &RegionUnion);
	ClosingRectangle1(RegionUnion, &RegionClosing, 50, 50);
	FillUp(RegionClosing, &RegionFillUp);
	ShapeTrans(RegionFillUp, Region, "convex");
}



double CSeparate::GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2)
{
	//需要先转化坐标，先把点的坐标转换成物理坐标

	try
	{
		//if (m_tParameters.nHcalibFlag==2)//纵向补偿的条纹标定
		//{
		//	HTuple hvPix, Sorted, Indices, Max, Selected;
		//	hvPix.Append(m_hvColPose);
		//	hvPix.Append(dptCol);
		//	TupleSort(hvPix, &Sorted);
		//	TupleFind(Sorted, dptCol, &Indices);
		//	TupleMax(Indices, &Max);
		//	TupleSelect(m_hvXPose, Max - 1, &Selected);
		//	double dV1 = dptCol - Sorted[Max - 1];

		//	double dpCol = Selected + dV1 * m_hvRes[Max.I() - 1];//这个位置就是左边点的物理坐标
		//	double dpRow = dptRow * m_dRowResolution;//Y坐标

		//	//求解线的物理坐标
		//	hvPix.Clear();
		//	hvPix.Append(m_hvColPose);
		//	hvPix.Append(hvLCol1);
		//	TupleSort(hvPix, &Sorted);
		//	TupleFind(Sorted, hvLCol1, &Indices);
		//	TupleMax(Indices, &Max);
		//	TupleSelect(m_hvXPose, Max - 1, &Selected);
		//	double dV2 = hvLCol1 - Sorted[Max - 1];
		//	double dLCol1 = Selected + dV2 * m_hvRes[Max.I() - 1];//线的起点物理坐标x1
		//	double dLRow1 = hvLRow1 *  m_dRowResolution;//线的起点物理坐标y1

		//	hvPix.Clear();
		//	hvPix.Append(m_hvColPose);
		//	hvPix.Append(hvLCol2);
		//	TupleSort(hvPix, &Sorted);
		//	TupleFind(Sorted, hvLCol2, &Indices);
		//	TupleMax(Indices, &Max);
		//	TupleSelect(m_hvXPose, Max - 1, &Selected);
		//	double dV3 = hvLCol2 - Sorted[Max - 1];
		//	double dLCol2 = Selected + dV3 * m_hvRes[Max.I() - 1];//线的起点物理坐标x2
		//	double dLRow2 = hvLRow2*  m_dRowResolution;//线的起点物理坐标y2

		//	HTuple hvDistance;
		//	DistancePl(dpRow, dpCol, dLRow1, dLCol1, dLRow2, dLCol2, &hvDistance);
		//	return hvDistance.D() * m_dCorCoef;
		//}
		//else
		{
			double dC1 = dptCol;
			double dC2 = (hvLCol1 + hvLCol2) / 2;
			HTuple hvGreater, hvCol1, hvCol2, hv_Indics1, hv_Indics2, hv_DistanceWorld;
			//保证为左为小坐标，右为大的坐标
			hvCol1 = min(dC1, dC2);
			hvCol2 = max(dC1, dC2);
			TupleGreaterEqualElem(m_hvColPose, hvCol1, &hvGreater);
			TupleFindFirst(hvGreater, 1, &hv_Indics1);
			TupleGreaterEqualElem(m_hvColPose, hvCol2, &hvGreater);
			TupleFindFirst(hvGreater, 1, &hv_Indics2);
			hv_DistanceWorld = (m_hvXPose[hv_Indics2].D() - m_hvXPose[hv_Indics1].D())
				+ ((m_hvColPose[hv_Indics1].D() - hvCol1.D())*m_hvRes[hv_Indics1 - 1].D())
				- ((m_hvColPose[hv_Indics2].D() - hvCol2.D())*m_hvRes[hv_Indics2 - 1].D());
			return abs(hv_DistanceWorld.D() *m_dCorCoef);//2021.2.13 增加k值补偿
		}
	}
	catch (HException& e)
	{

	}
	return 0.0;
}


double CSeparate::GetDistancepl(HTuple hvLeftColumn, HTuple hvRightCol)
{
	//需要先转化坐标，先把点的坐标转换成物理坐标

	try
	{
		{
			double dC1 = hvLeftColumn.D();
			double dC2 = hvRightCol.D();
			HTuple hvGreater, hvCol1, hvCol2, hv_Indics1, hv_Indics2, hv_DistanceWorld;
			//保证为左为小坐标，右为大的坐标
			hvCol1 = min(dC1, dC2);
			hvCol2 = max(dC1, dC2);
			TupleGreaterEqualElem(m_hvColPose, hvCol1, &hvGreater);
			TupleFindFirst(hvGreater, 1, &hv_Indics1);
			TupleGreaterEqualElem(m_hvColPose, hvCol2, &hvGreater);
			TupleFindFirst(hvGreater, 1, &hv_Indics2);
			hv_DistanceWorld = (m_hvXPose[hv_Indics2].D() - m_hvXPose[hv_Indics1].D())
				+ ((m_hvColPose[hv_Indics1].D() - hvCol1.D())*m_hvRes[hv_Indics1 - 1].D())
				- ((m_hvColPose[hv_Indics2].D() - hvCol2.D())*m_hvRes[hv_Indics2 - 1].D());
			return abs(hv_DistanceWorld.D() *m_dCorCoef);//2021.2.13 增加k值补偿
		}
	}
	catch (HException& e)
	{

	}
	return 0.0;
}


void CSeparate::CorrectedPoint()//修正点位
{
	HObject RegionLines;
	HTuple RowBegin, ColBegin, RowEnd, ColEnd;

	m_hvFitLineSR = m_hvRowBegin;
	m_hvFitLineSC = m_hvColBegin;
	m_hvFitLineER = m_hvRowEnd;
	m_hvFitLineEC = m_hvColEnd;


	for (int i = 0; i < m_tLineMatch.nLineUsed; i++)
	{
		GenRegionLine(&RegionLines, m_hvRowBegin[i], m_hvColBegin[i], m_hvRowEnd[i], m_hvColBegin[i]);
		if (TRUE == IsVertical(RegionLines))//如果是垂直的线
		{
			IntersectionLl(m_hvRowBegin[i], m_hvColBegin[i], m_hvRowEnd[i], m_hvColEnd[i], 0, 0, 0, m_hvWidth - 1, &RowBegin, &ColBegin, NULL);
			IntersectionLl(m_hvRowBegin[i], m_hvColBegin[i], m_hvRowEnd[i], m_hvColEnd[i], m_hvHeight - 1, 0, m_hvHeight - 1, m_hvWidth - 1, &RowEnd, &ColEnd, NULL);
			m_hvRowBegin[i] = RowBegin;
			m_hvColBegin[i] = ColBegin;
			m_hvRowEnd[i] = RowEnd;
			m_hvColEnd[i] = ColEnd;
		}
		else
		{

			IntersectionLl(m_hvRowBegin[i], m_hvColBegin[i], m_hvRowEnd[i], m_hvColEnd[i], 0, 0, m_hvHeight - 1, 0, &RowBegin, &ColBegin, NULL);
			IntersectionLl(m_hvRowBegin[i], m_hvColBegin[i], m_hvRowEnd[i], m_hvColEnd[i], 0, m_hvWidth - 1, m_hvHeight - 1, m_hvWidth - 1, &RowEnd, &ColEnd, NULL);
			m_hvRowBegin[i] = RowBegin;
			m_hvColBegin[i] = ColBegin;
			m_hvRowEnd[i] = RowEnd;
			m_hvColEnd[i] = ColEnd;
		}
	}
}
BOOL CSeparate::FindPoint()
{
	BOOL bRet = TRUE;
	GetImageSize(m_hoGrabImage, &m_hvWidth, &m_hvHeight);
	m_hoPretreatImage = m_hoGrabImage;

	//如果是程序第一次启动
	if (m_bReFindLine)
	{
		FirstFindLine();
		m_bReFindLine = FALSE;
		CString strFilePath;
		strFilePath.Format("%s//Config//System.vis", GetCurrentAppPath());
		SetFileAttributes(strFilePath, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
		WritePrivateProfileInt(m_tInit.strProductName, "FindLine", m_bReFindLine, strFilePath);
		SetFileAttributes(strFilePath, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
		SaveParam();
	}

	for (int i = 0; i < m_tLineMatch.nLineUsed; i++)
	{
		HObject hoLine;
		double dStartRow(0.0), dStartColumn(0.0), dEndRow(0.0), dEndColumn(0.0);

		if (FALSE == FitLine(m_hoPretreatImage, m_sLineTool[i].AffineTrans(), &hoLine,
			&dStartRow, &dStartColumn, &dEndRow, &dEndColumn, i))
		{
			bRet = FALSE;
			ConcatObj(m_hoLine, hoLine, &m_hoLine);
			m_hvRowBegin[i] = -1;
			m_hvColBegin[i] = -1;
			m_hvRowEnd[i] = -1;
			m_hvColEnd[i] = -1;
			m_hvErrorLineIndex.Append(i);
		}
		else
		{
			ConcatObj(m_hoLine, hoLine, &m_hoLine);
			m_hvRowBegin[i] = dStartRow;
			m_hvColBegin[i] = dStartColumn;
			m_hvRowEnd[i] = dEndRow;
			m_hvColEnd[i] = dEndColumn;

		}
	}

	return bRet;
}

int CSeparate::Detect(SEPARATEINPUT &tIn, SEPARATEOUTPUT *ptOut)
{
	m_ptOut = ptOut;
	m_hoGrabImage = tIn.hoGrabImage;
	m_ptIn = &tIn;
	HTuple hvWidth, hvHeight;
	GetImageSize(m_hoGrabImage, &hvWidth, &hvHeight);
	PreDetect();
	try {
		BOOL bRet = FindPoint();
		if (TRUE == bRet)
		{
			try
			{
				SetLineMatch();
			}
			catch (HException e)
			{
				HTuple  hv_Exception = "区域设置中，程序出现异常,请重新设置区域";
			}
			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("Detect异常_%s", e.ErrorMessage());
		return TRUE;
	}


}
int CSeparate::FitLine(const HObject& Image, CLineToolPara& FitLinePara, HObject* Line,
	double* dStartY, double* dStartX, double* dEndY, double* dEndX, int nLineSelect, int nLineDisplay)
{
	try
	{


#pragma region "先对拟合方式进行判断，如果不是正常拟合直接就返回了"
		HTuple Width, Height;
		GetImageSize(Image, &Width, &Height);
		if (0 != FitLinePara.m_nChoice)
		{
			HObject Domain;
			HTuple Row1, Column1, Row2, Column2;
			GetDomain(Image, &Domain);
			SmallestRectangle1(Domain, &Row1, &Column1, &Row2, &Column2);
			if (1 == FitLinePara.m_nChoice)//固定值
			{
				*dStartX = FitLinePara.m_dStartCol;
				*dStartY = FitLinePara.m_dStartRow;
				*dEndX = FitLinePara.m_dEndCol;
				*dEndY = FitLinePara.m_dEndRow;
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (2 == FitLinePara.m_nChoice)//固定顶端
			{
				*dStartX = FitLinePara.m_dStartCol;
				*dStartY = Row1.I();
				FitLinePara.m_dStartRow = Row1.I();
				*dEndX = FitLinePara.m_dEndCol;
				*dEndY = Row1.I();
				FitLinePara.m_dEndRow = Row1.I();
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (3 == FitLinePara.m_nChoice)//固定底端
			{
				*dStartX = FitLinePara.m_dStartCol;
				*dStartY = Row2.I();
				FitLinePara.m_dStartRow = Row2.I();
				*dEndX = FitLinePara.m_dEndCol;
				*dEndY = Row2.I();
				FitLinePara.m_dEndRow = Row2.I();
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (4 == FitLinePara.m_nChoice)//固定左侧
			{
				*dStartX = Column1.I();
				FitLinePara.m_dEndCol = Column1.I();
				*dStartY = FitLinePara.m_dStartRow;
				*dEndX = Column1.I();
				FitLinePara.m_dStartCol = Column1.I();
				*dEndY = FitLinePara.m_dEndRow;
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (5 == FitLinePara.m_nChoice)//固定右侧
			{
				*dStartX = Column2.I();
				FitLinePara.m_dStartCol = Column2.I();
				*dStartY = FitLinePara.m_dStartRow;
				FitLinePara.m_dEndCol = Column2.I();
				*dEndX = Column2.I();
				*dEndY = FitLinePara.m_dEndRow;
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			return TRUE;
		}

#pragma endregion 

		// Local iconic variables 
		HObject  Rectangle, Contour;
		// Local control variables 
		HTuple  Phi, ResultRow, ResultCol, i, RowC, ColC;
		HTuple  MsrHandle_Measure, transition;
		HTuple  select, RowEdge, ColEdge, Amplitude, Distance, tRow;
		HTuple  tCol, t, RowBegin, ColBegin, RowEnd, ColEnd, Nr;
		HTuple  Nc, Dist;
		Line->GenEmptyObj();
		Phi = ((HTuple(FitLinePara.m_dStartRow) - HTuple(FitLinePara.m_dEndRow)).TupleAtan2(HTuple(FitLinePara.m_dEndCol) - HTuple(FitLinePara.m_dStartCol)))\
			+ (HTuple(90).TupleRad());
		HObject hMeasureRect, hCross;
		ResultRow = HTuple();
		ResultCol = HTuple();
		if (0 != (HTuple(FitLinePara.m_nDirect) == 0))
		{
			transition = "all";
		}
		else if (0 != (HTuple(FitLinePara.m_nDirect) == 1))
		{
			transition = "positive";
		}
		else
		{
			transition = "negative";
		}
		if (0 != (HTuple(FitLinePara.m_nSelect) == 0))
		{
			select = "all";
		}
		else if (0 != (HTuple(FitLinePara.m_nSelect) == 1))
		{
			select = "first";
		}
		else
		{
			select = "last";
		}

		for (i = 0; i < HTuple(FitLinePara.m_nMeasureNum); i += 1)
		{
			RowC = HTuple(FitLinePara.m_dStartRow) + (((HTuple(FitLinePara.m_dEndRow) - HTuple(FitLinePara.m_dStartRow))*i) / (HTuple(FitLinePara.m_nMeasureNum) - 1));
			ColC = HTuple(FitLinePara.m_dStartCol) + (((HTuple(FitLinePara.m_dEndCol) - HTuple(FitLinePara.m_dStartCol))*i) / (HTuple(FitLinePara.m_nMeasureNum) - 1));
			//Convert coordinates to rectangle2 type
			GenRectangle2(&hMeasureRect, RowC, ColC, Phi, HTuple(FitLinePara.m_dHeight), HTuple(FitLinePara.m_dWidth));
			GenRectangle2(&Rectangle, RowC, ColC, Phi, HTuple(FitLinePara.m_dHeight), 1);
			if (0 == nLineDisplay)
			{
				ConcatObj(*Line, Rectangle, Line);
			}
			else
			{
				ConcatObj(*Line, hMeasureRect, Line);
			}

			GenMeasureRectangle2(RowC, ColC, Phi, HTuple(FitLinePara.m_dHeight), HTuple(FitLinePara.m_dWidth),
				Width, Height, "nearest_neighbor", &MsrHandle_Measure);
			try
			{
				MeasurePos(Image, MsrHandle_Measure, HTuple(FitLinePara.m_dSigma), HTuple(FitLinePara.m_nThr),
					transition, select, &RowEdge, &ColEdge, &Amplitude, &Distance);

			}
			catch (HException& e)
			{
				CString str;
				str.Format("%s", e.ErrorMessage().Text());
			}
			CloseMeasure(MsrHandle_Measure);

			tRow = 0;
			tCol = 0;
			t = 0;
			for (int j = 0; j <= (RowEdge.Length()) - 1; j += 1)
			{
				if (0 != (t < HTuple(Amplitude[j]).TupleAbs()))
				{
					tRow = RowEdge[j];
					tCol = ColEdge[j];
					t = HTuple(Amplitude[j]).TupleAbs();
				}
			}
			if (0 != (t > 0))
			{
				ResultRow.Append(tRow);
				ResultCol.Append(tCol);
				GenCrossContourXld(&hCross, tRow[0].D(), tCol[0].D(), 18, 0.78);
				ConcatObj(*Line, hCross, Line);
			}
		}
		//stop ()
		HObject hLine;
		if (ResultRow.Length() > FitLinePara.m_nMeasureNum / 2)
		{
			GenContourPolygonXld(&Contour, ResultRow, ResultCol);//tukey
			FitLineContourXld(Contour, "tukey", -1, 0, 5, 2, &RowBegin,
				&ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
			GenContourPolygonXld(&hLine, RowBegin.TupleConcat(RowEnd), ColBegin.TupleConcat(ColEnd));
			ConcatObj(*Line, hLine, Line);
			*dStartY = RowBegin[0].D();
			*dStartX = ColBegin[0].D();
			*dEndY = RowEnd[0].D();
			*dEndX = ColEnd[0].D();

			m_hvec_GRRpoint[m_hvec_GRRpoint.Length()].T() = ResultCol;
		}
		else//如果没找到线，就是用原来的位置
		{
			GenContourPolygonXld(&hLine, HTuple(FitLinePara.m_dStartRow).TupleConcat(FitLinePara.m_dEndRow),
				HTuple(FitLinePara.m_dStartCol).TupleConcat(FitLinePara.m_dEndCol));
			ConcatObj(*Line, hLine, Line);
			*dStartY = FitLinePara.m_dStartRow;
			*dStartX = FitLinePara.m_dStartCol;
			*dEndY = FitLinePara.m_dEndRow;
			*dEndX = FitLinePara.m_dEndCol;

			m_hvec_GRRpoint[m_hvec_GRRpoint.Length()].T() = FitLinePara.m_dStartCol;
			return TRUE;
		}
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}


}



BOOL CSeparate::ReadParam()
{
	try
	{
		CString strFilePath;
		strFilePath.Format("%s\\Config\\Products\\%s\\SCISeparate\\%d", GetCurrentAppPath(), m_tInit.strProductName,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strFilePath);


		m_strCommondh.Format("%s\\Config\\Common.dh", GetCurrentAppPath());
		CString strSection;
		strSection.Format("GRABBER%d", m_tInit.nGrabberSN);
		m_dRowResolution = GetPrivateProfileDouble(strSection, "ROWRESOLUTION", 0.1, m_strCommondh);
		m_dColResolution = GetPrivateProfileDouble(strSection, "COLUMNRESOLUTION", 0.1, m_strCommondh);


		m_strMyConfig.Format("%s\\Config.ini", strFilePath);
		m_strSystemVis.Format("%s/Config/System.vis", GetCurrentAppPath());
		//m_tMeausreParam.bUseLineRefer = GetPrivateProfileInt("PARAMETER","USELINEREFER",0,m_strMyConfig);
		//m_tMeausreParam.bJudgeCoat = GetPrivateProfileInt("PARAMETER","JUDGECOAT",1,m_strMyConfig);
		m_tMeausreParam.nCoatThres = GetPrivateProfileInt("PARAMETER", "COATTHRES", 150, m_strMyConfig);
		//m_tMeausreParam.nLineReferSN = GetPrivateProfileInt("PARAMETER","LINEREFERSN",0,m_strMyConfig);
		//m_tMeausreParam.bPretreat = GetPrivateProfileInt("PARAMETER","PRETREAT",0,m_strMyConfig);

		m_nCalculateChoose = GetPrivateProfileInt("CALIBRATIONPARAME", "nCalculateChoose", 0, m_strMyConfig);

		CString strModelPath;
		strModelPath.Format("%s\\CaoWeiRegions.hobj", strFilePath);
		try
		{
			ReadObject(&m_tMeausreParam.hoCaoweiRegions, (HTuple)strModelPath);
		}
		catch (HException& e)
		{

			m_tMeausreParam.hoCaoweiRegions.GenEmptyObj();
		}




		CString lpAppName, keyName;
		lpAppName.Format("GRABBER%d", m_tInit.nGrabberSN);

		m_nPoseCount = GetPrivateProfileInt(lpAppName, "POSECOUNT", 1, m_strCommondh);
		for (int i = 0; i < m_nPoseCount; i++)
		{
			double temp;
			keyName.Format("PIXELPOSE%d", i);
			temp = GetPrivateProfileDouble(lpAppName, keyName, 1.0000, m_strCommondh);
			m_hvColPose.Append(HTuple(temp));

			keyName.Format("XPOSE%d", i);
			temp = GetPrivateProfileDouble(lpAppName, keyName, 1.0000, m_strCommondh);
			m_hvXPose.Append(HTuple(temp));

			keyName.Format("PartRes%d", i);
			temp = GetPrivateProfileDouble(lpAppName, keyName, 0.104, m_strCommondh);
			m_hvRes.Append(HTuple(temp));
		}
		lpAppName.Format("CALIBRATION%d", m_tInit.nGrabberSN);
		m_dCalSpaceDist = GetPrivateProfileDouble(lpAppName, "CALSPACEDIST", 1.0000, m_strCommondh);
		m_dCorCoef = GetPrivateProfileDouble(lpAppName, "CORCOEF", 1.0000, m_strCommondh);

		////////////////////////////////////
		return TRUE;
	}
	catch (HException e)
	{
		return FALSE;
	}
}

void CSeparate::FirstFindLine()
{
	// Local iconic variables
	HObject  ho_Image, ho_RegionLines, ho_Cross, ho_RegionLines1;

	// Local control variables
	HTuple  hv_RowBegin, hv_ColumnBegin, hv_RowEnd;
	HTuple  hv_ColumnEnd;
	GetValidRowColumns(m_hoPretreatImage, &ho_RegionLines, &ho_Cross, &hv_RowBegin, &hv_ColumnBegin, &hv_RowEnd, &hv_ColumnEnd);
	if (hv_RowBegin.TupleLength() != 4)
	{
		SYSTEMTIME tSystemTime;
		GetLocalTime(&tSystemTime);
		CString strFilePah, strFileName;
		strFilePah = "D:\\ccd_data\\寻边失败存图\\";
		CreateMultilevelPath(strFilePah);
		strFileName.Format("%s%02d%02d%02d-%02d%02d%02d.bmp",
			strFilePah,tSystemTime.wYear, tSystemTime.wMonth, tSystemTime.wDay, tSystemTime.wHour, tSystemTime.wMinute, tSystemTime.wSecond, tSystemTime.wMilliseconds);
		WriteImage(m_hoPretreatImage, "bmp", 0, (HTuple)strFilePah);
		AfxMessageBox("自动寻边失败！！！请重新配置");
		return;
	}
	//GenRegionLine(&ho_RegionLines1, hv_RowBegin, hv_ColumnBegin, hv_RowEnd, hv_ColumnEnd);
	if (m_tInit.nCoatType == M_TwoCam_Continue)
	{
#pragma region
		//try
		//{
		//	HTuple LTRow, LTColumn, RBRow, RBColumn;
		//	HObject hoRectangle;
		//	HTuple Row1, Column1, Row2, Column2;
		//	HTuple hv_Height, hv_Width;
		//	GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);

		//	HObject hoRegion, hoConnections, hoFillUp, hoCoat;
		//	Threshold(m_hoPretreatImage, &hoRegion, MAX(0, m_EDT_nCoatGrayValue - 10), 255);
		//	Connection(hoRegion, &hoConnections);
		//	FillUp(hoConnections, &hoFillUp);
		//	SelectShapeStd(hoFillUp, &hoFillUp, "max_area", 70);
		//	OpeningRectangle1(hoFillUp, &hoFillUp, 101, 101);
		//	ClosingRectangle1(hoFillUp, &hoCoat, 101, 101);
		//	if (hoCoat.CountObj() != 1)
		//	{
		//		AfxMessageBox("自动绘制失败，请继续手动绘制");
		//		DrawRectangle1(m_pParentDlg->m_hvWindowHandle, &Row1, &Column1, &Row2, &Column2);

		//	}
		//	else
		//	{
		//		HTuple LTRow, LTColumn, RBRow, RBColumn;
		//		SmallestRectangle1(hoCoat, &LTRow, &LTColumn, &RBRow, &RBColumn);

		//		Column2 = RBColumn + 30 / m_dColResolution;
		//		Column1 = LTColumn - 30 / m_dColResolution;
		//		Row1 = hv_Height / 2 - 30 / m_dRowResolution;
		//		Row2 = hv_Height / 2 + 30 / m_dRowResolution;
		//	}


		//	HObject hoRoI;
		//	GenRectangle1(&hoRoI, Row1, Column1, Row2, Column2);
		//	HObject hoFoilRegions, hoCoatRegions;
		//	GetCoatAndFoilRegions(m_hoPretreatImage, hoRoI, &hoFoilRegions, &hoCoatRegions);
		//	{
		//		int nFoilNum, nCoatNum, nAT9Num;
		//		nFoilNum = hoFoilRegions.CountObj();
		//		nCoatNum = hoCoatRegions.CountObj();
		//		nAT9Num = 2 * nCoatNum;
		//		HTuple hvFLRow, hvFLColumn, hvFBRow, hv_FBColumn;
		//		SmallestRectangle1(hoFoilRegions, &hvFLRow, &hvFLColumn, &hvFBRow, &hv_FBColumn);
		//		HTuple hvCLRow, hvCLColumn, hvCBRow, hv_CBColumn;
		//		SmallestRectangle1(hoCoatRegions, &hvCLRow, &hvCLColumn, &hvCBRow, &hv_CBColumn);
		//		HTuple hv_StartAllRows, hv_StartAllColumns, hv_EndAllRows, hv_EndAllColumns;
		//		hv_StartAllRows.Clear();
		//		hv_StartAllColumns.Clear();
		//		hv_EndAllRows.Clear();
		//		hv_EndAllColumns.Clear();
		//		hv_StartAllRows.Append(hvFLRow).Append(hvFLRow).Append(hvCLRow).Append(hvCLRow);
		//		hv_StartAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);
		//		hv_EndAllRows.Append(hvFBRow).Append(hvFBRow).Append(hvCBRow).Append(hvCBRow);
		//		hv_EndAllColumns.Append(hvFLColumn).Append(hv_FBColumn).Append(hvCLColumn).Append(hv_CBColumn);


		//		HTuple hv_Index, hv_LineTuple;
		//		TupleSortIndex(hv_StartAllColumns, &hv_Index);
		//		TupleSelect(hv_StartAllRows, hv_Index, &hv_StartAllRows);
		//		TupleSelect(hv_StartAllColumns, hv_Index, &hv_StartAllColumns);
		//		TupleSelect(hv_EndAllRows, hv_Index, &hv_EndAllRows);
		//		TupleSelect(hv_EndAllColumns, hv_Index, &hv_EndAllColumns);
		//		TupleGenSequence(0, hv_StartAllRows.Length() - 1, 1, &hv_LineTuple);


		//		m_tLineMatch.nLineUsed = nFoilNum + nCoatNum + nAT9Num + 1;
		//		m_tLineMatch.nCoatCount = nCoatNum;
		//		m_tLineMatch.nFoilCount = nFoilNum;
		//		m_tLineMatch.nCeramicCount = nAT9Num;
		//		m_tLineMatch.nStaggerCount = 0;
		//		m_tLineMatch.nCoatLeftStartIndex = 0;
		//		m_tLineMatch.nFoilLeftStartIndex = 0;
		//		m_tLineMatch.nCeramicLeftStartIndex = 0;




		//		/*
		//		double	m_dSigma;			//对应measure_pos中的Sigma
		//		int		m_nThr;				//对应measure_pos中的Threshold
		//		int		m_nDirect;			//对应measure_pos中的Transition:0-'all', 1-'positive', 2-'negative'
		//		int		m_nSelect;			//对应measure_pos中的Select:0-'all', 1-'first', 2-'last'

		//		double	m_dWidth;			//单个检测框的宽度
		//		double	m_dHeight;			//单个检测框的高度
		//		int		m_nMeasureNum;		//检测框的个数
		//		double	m_dStartRow;		//起始点的Row坐标
		//		double	m_dStartCol;		//起始点的Col坐标
		//		double	m_dEndRow;			//终止点的Row坐标
		//		double	m_dEndCol;			//终止点的Col坐标
		//		*/

		//		for (int i = 0; i < m_tLineMatch.nCoatCount; i++)
		//		{
		//			int nLeft, nRight;
		//			nLeft = 2 + i * 4;
		//			nRight = nLeft + 1;
		//			m_tLineMatch.vCoat1[i] = nLeft;
		//			m_tLineMatch.vCoat2[i] = nRight;
		//			m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
		//			m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
		//			m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
		//			m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
		//			m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
		//			m_sLineTool[nLeft].m_nDirect = 1;//正向
		//			m_sLineTool[nLeft].m_nThr = 20;//正向
		//			m_sLineTool[nLeft].m_dHeight = 300;//正向
		//			m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
		//			m_sLineTool[nLeft].m_dSigma = 2;//正向

		//			m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
		//			m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
		//			m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
		//			m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
		//			m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
		//			m_sLineTool[nRight].m_nDirect = 1;//正向
		//			m_sLineTool[nRight].m_nThr = 20;//
		//			m_sLineTool[nRight].m_dHeight = 300;//正向
		//			m_sLineTool[nRight].m_nMeasureNum = 20;//正向
		//			m_sLineTool[nRight].m_dSigma = 2;//正向


		//		}
		//		for (int i = 0; i < m_tLineMatch.nFoilCount; i++)
		//		{


		//			int nLeft, nRight;
		//			nLeft = i * 4;
		//			nRight = nLeft + 1;

		//			m_tLineMatch.vFoil1[i] = nLeft;
		//			m_tLineMatch.vFoil2[i] = nRight;


		//			m_sLineTool[nLeft].m_dStartRow = hv_EndAllRows[nLeft];
		//			m_sLineTool[nLeft].m_dStartCol = hv_EndAllColumns[nLeft];
		//			m_sLineTool[nLeft].m_dEndRow = hv_StartAllRows[nLeft];
		//			m_sLineTool[nLeft].m_dEndCol = hv_StartAllColumns[nLeft];
		//			m_sLineTool[nLeft].m_nChoice = 0;//固定参考标记线
		//			m_sLineTool[nLeft].m_nDirect = 2;//正向
		//			m_sLineTool[nLeft].m_nThr = 80;//正向
		//			m_sLineTool[nLeft].m_dHeight = 300;//正向
		//			m_sLineTool[nLeft].m_nMeasureNum = 20;//正向
		//			m_sLineTool[nLeft].m_dSigma = 2;//正向


		//			m_sLineTool[nRight].m_dStartRow = hv_StartAllRows[nRight];
		//			m_sLineTool[nRight].m_dStartCol = hv_StartAllColumns[nRight];
		//			m_sLineTool[nRight].m_dEndRow = hv_EndAllRows[nRight];
		//			m_sLineTool[nRight].m_dEndCol = hv_EndAllColumns[nRight];
		//			m_sLineTool[nRight].m_nChoice = 0;//固定参考标记线
		//			m_sLineTool[nRight].m_nDirect = 2;//正向
		//			m_sLineTool[nRight].m_nThr = 80;//
		//			m_sLineTool[nRight].m_dHeight = 300;//正向
		//			m_sLineTool[nRight].m_nMeasureNum = 20;//正向
		//			m_sLineTool[nRight].m_dSigma = 2;//正向





		//		}
		//		for (int i = 0; i < m_tLineMatch.nCeramicCount; i++)
		//		{
		//			m_tLineMatch.vCeramic1[i] = 1 + 2 * i;
		//			m_tLineMatch.vCeramic2[i] = 1 + 2 * i + 1;

		//		}

		//	}


		//	InitCom();
		//	SaveParam();
		//	SaveLineMatch();




		//}
		//catch (HException& e)
		//{
		//	CString strValue;
		//	strValue.Format("自动画线出现问题，错误函数:%s 错误内容:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		//	AfxMessageBox(strValue);
		//}
#pragma endregion 
	}
	else
	{
		HTuple hv_Height, hv_Width;
		GetImageSize(m_hoPretreatImage, &hv_Width, &hv_Height);
		HTuple Row, Column, Phi, Length1, Length2;

		//HTuple LTRow, LTColumn, RBRow, RBColumn;
		HObject hoRegion, hoConnections, hoFillUp, hoCoat;
		try
		{
			//需要外部传入参数
			double m_dJXHeight;
			if (m_tInit.nGrabberSN == 0)
			{
				m_dJXHeight = m_tStandardSizes.dDownIntermissionHeight / m_dColResolution;
			}
			else
			{
				m_dJXHeight = m_tStandardSizes.dUpIntermissionHeight / m_dColResolution;
			}
			//0涂布左边界；1涂布上边界；2涂布右边界；3涂布下边界；4间歇下边界；5左极耳边；6右极耳边
			if (m_bExistJXFlag)//有间歇
			{
				m_sLineTool[1].m_nChoice = 0;
				m_sLineTool[3].m_nChoice = 0;
				m_sLineTool[4].m_nChoice = 0;
			}
			else
			{
				if (m_bExistJXUpDownFlag)//间歇在底部
				{
					m_sLineTool[1].m_nChoice = 2;
					m_sLineTool[3].m_nChoice = 3;
					m_sLineTool[4].m_nChoice = 3;
				}
				else
				{
					m_sLineTool[1].m_nChoice = 3;
					m_sLineTool[3].m_nChoice = 2;
					m_sLineTool[4].m_nChoice = 2;
				}
			}
			if (m_bExistJXUpDownFlag)//间歇在底部
			{
				m_sLineTool[0].m_dStartRow = hv_RowBegin[1];
				m_sLineTool[0].m_dStartCol = hv_ColumnBegin[1];
				m_sLineTool[0].m_dEndRow = hv_RowEnd[1];
				m_sLineTool[0].m_dEndCol = hv_ColumnEnd[1];
				m_sLineTool[0].m_nDirect = 2;

				m_sLineTool[2].m_dStartRow = hv_RowBegin[2];
				m_sLineTool[2].m_dStartCol = hv_ColumnBegin[2];
				m_sLineTool[2].m_dEndRow = hv_RowEnd[2];
				m_sLineTool[2].m_dEndCol = hv_ColumnEnd[2];
				m_sLineTool[2].m_nDirect = 1;

				m_sLineTool[5].m_dStartRow = hv_RowBegin[0];
				m_sLineTool[5].m_dStartCol = hv_ColumnBegin[0];
				m_sLineTool[5].m_dEndRow = hv_RowEnd[0];
				m_sLineTool[5].m_dEndCol = hv_ColumnEnd[0];
				m_sLineTool[5].m_nDirect = 1;

				m_sLineTool[6].m_dStartRow = hv_RowBegin[3];
				m_sLineTool[6].m_dStartCol = hv_ColumnBegin[3];
				m_sLineTool[6].m_dEndRow = hv_RowEnd[3];
				m_sLineTool[6].m_dEndCol = hv_ColumnEnd[3];
				m_sLineTool[6].m_nDirect = 2;

				m_sLineTool[1].m_dStartRow = 0;
				m_sLineTool[1].m_dStartCol = hv_ColumnBegin[1];
				m_sLineTool[1].m_dEndRow = 0;
				m_sLineTool[1].m_dEndCol = hv_ColumnBegin[2];
				m_sLineTool[1].m_nDirect = 1;

				m_sLineTool[3].m_dStartRow = hv_Height - m_dJXHeight - 100;
				m_sLineTool[3].m_dStartCol = hv_ColumnEnd[1];
				m_sLineTool[3].m_dEndRow = hv_Height - m_dJXHeight - 100;
				m_sLineTool[3].m_dEndCol = hv_ColumnEnd[2];
				m_sLineTool[3].m_nDirect = 2;
				m_sLineTool[3].m_nSelect = 2;

				m_sLineTool[4].m_dStartRow = hv_Height - 1;
				m_sLineTool[4].m_dStartCol = hv_ColumnEnd[1];
				m_sLineTool[4].m_dEndRow = hv_Height - 1;
				m_sLineTool[4].m_dEndCol = hv_ColumnEnd[2];
				m_sLineTool[4].m_nDirect = 1;
				m_sLineTool[4].m_nSelect = 2;
			}
			else
			{
				m_sLineTool[0].m_dStartRow = hv_RowBegin[1];
				m_sLineTool[0].m_dStartCol = hv_ColumnBegin[1];
				m_sLineTool[0].m_dEndRow = hv_RowEnd[1];
				m_sLineTool[0].m_dEndCol = hv_ColumnEnd[1];
				m_sLineTool[0].m_nDirect = 2;

				m_sLineTool[2].m_dStartRow = hv_RowBegin[2];
				m_sLineTool[2].m_dStartCol = hv_ColumnBegin[2];
				m_sLineTool[2].m_dEndRow = hv_RowEnd[2];
				m_sLineTool[2].m_dEndCol = hv_ColumnEnd[2];
				m_sLineTool[2].m_nDirect = 1;

				m_sLineTool[5].m_dStartRow = hv_RowBegin[0];
				m_sLineTool[5].m_dStartCol = hv_ColumnBegin[0];
				m_sLineTool[5].m_dEndRow = hv_RowEnd[0];
				m_sLineTool[5].m_dEndCol = hv_ColumnEnd[0];
				m_sLineTool[5].m_nDirect = 1;

				m_sLineTool[6].m_dStartRow = hv_RowBegin[3];
				m_sLineTool[6].m_dStartCol = hv_ColumnBegin[3];
				m_sLineTool[6].m_dEndRow = hv_RowEnd[3];
				m_sLineTool[6].m_dEndCol = hv_ColumnEnd[3];
				m_sLineTool[6].m_nDirect = 2;

				m_sLineTool[4].m_dStartRow = 0;
				m_sLineTool[4].m_dStartCol = hv_ColumnBegin[1];
				m_sLineTool[4].m_dEndRow = 0;
				m_sLineTool[4].m_dEndCol = hv_ColumnBegin[2];
				m_sLineTool[4].m_nDirect = 2;
				m_sLineTool[4].m_nSelect = 2;

				m_sLineTool[3].m_dStartRow = m_dJXHeight + 100;
				m_sLineTool[3].m_dStartCol = hv_ColumnBegin[1];
				m_sLineTool[3].m_dEndRow = m_dJXHeight + 100;
				m_sLineTool[3].m_dEndCol = hv_ColumnBegin[2];
				m_sLineTool[3].m_nDirect = 1;
				m_sLineTool[3].m_nSelect = 1;

				m_sLineTool[1].m_dStartRow = hv_Height - 1;
				m_sLineTool[1].m_dStartCol = hv_ColumnEnd[1];
				m_sLineTool[1].m_dEndRow = hv_Height - 1;
				m_sLineTool[1].m_dEndCol = hv_ColumnEnd[2];
				m_sLineTool[1].m_nDirect = 2;
				m_sLineTool[3].m_nSelect = 1;
			}
			for (int i = 0; i < m_tLineMatch.nLineUsed; i++)
			{
				m_sLineTool[i].m_dWidth = 20;
				m_sLineTool[i].m_dHeight = 200;
				m_sLineTool[i].m_nThr = 15;
				if (i == 1 || i == 3 || i == 4)
				{
					m_sLineTool[i].m_dHeight = m_dJXHeight * 2;
					m_sLineTool[i].m_nThr = 50;
				}
				m_sLineTool[i].m_nMeasureNum = 20;
				m_sLineTool[i].m_dSigma = 2;
			}
		}
		catch (HException& e)
		{


		}

	}
}

void CSeparate::GetValidRowColumns(HObject ho_Image, HObject *ho_RegionLines1, HObject *ho_Cross,
	HTuple *hv_RowBegin, HTuple *hv_ColumnBegin, HTuple *hv_RowEnd, HTuple *hv_ColumnEnd)
{

	// Local iconic variables
	HObject  ho_Coat, ho_Contour, ho_Contour1;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_MetrologyHandle;
	HTuple  hv_LineRow1, hv_LineColumn1, hv_LineRow2, hv_LineColumn2;
	HTuple  hv_Tolerance, hv_dLength1, hv_dLength2, hv_Index1;
	HTuple  hv_LineParameter, hv_MRow, hv_MColumn, hv_i, hv_Indices;
	HTupleVector  hvec_ResultVector(1);

	//定位涂布的大致位置
	Threshold(m_hoPretreatImage, &ho_Coat, m_tMeausreParam.nCoatThres, 220);
	OpeningRectangle1(ho_Coat, &ho_Coat, 10, 10);
	Connection(ho_Coat, &ho_Coat);
	SelectShapeStd(ho_Coat, &ho_Coat, "max_area", 70);
	ShapeTrans(ho_Coat, &ho_Coat, "rectangle1");
	HTuple hvRow, hvCol;
	SmallestRectangle2(ho_Coat, &hvRow, &hvCol, NULL, NULL, NULL);
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	//Create the metrology model data structure
	CreateMetrologyModel(&hv_MetrologyHandle);
	SetMetrologyModelImageSize(hv_MetrologyHandle, hv_Width, hv_Height);
	//Define the parameters of the metrology line objects
	hv_LineRow1 = hv_Height / 3;
	hv_LineRow2 = hv_Height / 3 * 2;
	hv_LineColumn1 = hvCol;
	hv_LineColumn2 = hvCol;
	//如果拿取的涂布行坐标与图像中心偏差大于一个值，则判断大致涂布区域找的不对
	HTuple hvDiff;
	TupleSub(hv_Width / 2, hvCol, &hvDiff);
	if (hvDiff.D() > 2000)
	{
		hv_LineColumn1 = hv_Width / 2;
		hv_LineColumn2 = hv_Width / 2;
	}
	hv_Tolerance = 10;
	hv_dLength1 = hv_Width / 2;
	hv_dLength2 = 50;

	//
	GenRegionLine(&(*ho_RegionLines1), hv_LineRow1, hv_LineColumn1, hv_LineRow2, hv_LineColumn2);
	//Create two metrology line objects and set parameters
	AddMetrologyObjectLineMeasure(hv_MetrologyHandle, hv_LineRow1, hv_LineColumn1,
		hv_LineRow2, hv_LineColumn2, hv_dLength1, hv_dLength2, 1, 30, "measure_distance",
		1000, &hv_Index1);
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "num_instances", 10);
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "measure_distance", 300);
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "measure_transition", "all");
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "distance_threshold", 4);
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "measure_threshold", 20);
	SetMetrologyObjectParam(hv_MetrologyHandle, "all", "min_score", 0.6);
	ApplyMetrologyModel(ho_Image, hv_MetrologyHandle);
	//Access results
	GetMetrologyObjectResult(hv_MetrologyHandle, "all", "all", "result_type", "all_param",
		&hv_LineParameter);
	GetMetrologyObjectMeasures(&ho_Contour, hv_MetrologyHandle, "all", "all", &hv_MRow,
		&hv_MColumn);
	GenCrossContourXld(&(*ho_Cross), hv_MRow, hv_MColumn, 60, HTuple(45).TupleRad());
	GetMetrologyObjectResultContour(&ho_Contour1, hv_MetrologyHandle, 0, "all", 1.5);
	if (0 != (int((hv_LineParameter.TupleLength()) > 0)))
	{
		hvec_ResultVector = HTupleVector(hv_LineParameter, 4);
		(*hv_RowBegin) = HTuple();
		(*hv_ColumnBegin) = HTuple();
		(*hv_RowEnd) = HTuple();
		(*hv_ColumnEnd) = HTuple();
		{
			HTuple end_val37 = HTuple(hvec_ResultVector.Length()) - 1;
			HTuple step_val37 = 1;
			for (hv_i = 0; hv_i.Continue(end_val37, step_val37); hv_i += step_val37)
			{
				(*hv_RowBegin) = (*hv_RowBegin).TupleConcat(HTuple(hvec_ResultVector[hv_i].T()[0]));
				(*hv_ColumnBegin) = (*hv_ColumnBegin).TupleConcat(HTuple(hvec_ResultVector[hv_i].T()[1]));
				(*hv_RowEnd) = (*hv_RowEnd).TupleConcat(HTuple(hvec_ResultVector[hv_i].T()[2]));
				(*hv_ColumnEnd) = (*hv_ColumnEnd).TupleConcat(HTuple(hvec_ResultVector[hv_i].T()[3]));
			}
		}
		TupleSortIndex((*hv_ColumnEnd), &hv_Indices);
		TupleSelect((*hv_RowBegin), hv_Indices, &(*hv_RowBegin));
		TupleSelect((*hv_ColumnBegin), hv_Indices, &(*hv_ColumnBegin));
		TupleSelect((*hv_RowEnd), hv_Indices, &(*hv_RowEnd));
		TupleSelect((*hv_ColumnEnd), hv_Indices, &(*hv_ColumnEnd));
		GenRegionLine(&(*ho_RegionLines1), (*hv_RowBegin), (*hv_ColumnBegin), (*hv_RowEnd),
			(*hv_ColumnEnd));
		GetMetrologyObjectMeasures(&ho_Contour, hv_MetrologyHandle, "all", "all", &hv_MRow,
			&hv_MColumn);
		GenCrossContourXld(&(*ho_Cross), hv_MRow, hv_MColumn, 60, HTuple(45).TupleRad());
	}
	ClearMetrologyModel(hv_MetrologyHandle);
	return;
}