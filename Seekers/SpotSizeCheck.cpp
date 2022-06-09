#include "StdAfx.h"
#include "SpotSizeCheck.h"


CSpotSizeCheck::CSpotSizeCheck(void)
{
	m_nSide = 0;
	m_nWidth = 200;
	for (int k = 0; k < 2; k++)
	{
		m_hvRowResolution[k] = 0;
		m_hvDistanceLocation[k] = 0;
		m_hvdLastDistance[k] = 0;
		m_bFirst[k] = 1;
		m_nLastDistanceCount[k] = 0;
		m_ndLastDistance[k] = 0;
	}
	m_nConntinueErrorNum = 0;
}


CSpotSizeCheck::~CSpotSizeCheck(void)
{
}

void   CSpotSizeCheck::ReadParam()
{
	m_StrSpotCheckDirectory.Format("%s\\Config\\Public\\点检区域\\%d", GetCurrentAppPath(), m_nSide);
	m_StrSpotCheckPath.Format("%s\\SpotParam.ini", m_StrSpotCheckDirectory);
	CreateMultilevelPath(m_StrSpotCheckDirectory);
	m_tSpotParam.bEnableUseSpot = GetPrivateProfileInt("SpotInfo", "bEnableUseSpot", 0, m_StrSpotCheckPath);
	m_tSpotParam.nLowThresh = GetPrivateProfileInt("SpotInfo", "nLowThresh", 50, m_StrSpotCheckPath);
	m_tSpotParam.nHighThresh = GetPrivateProfileInt("SpotInfo", "nHighThresh", 50, m_StrSpotCheckPath);
	m_tSpotParam.nSpotNum = GetPrivateProfileInt("SpotInfo", "nSpotNum", 1, m_StrSpotCheckPath);
	m_tSpotParam.nLeftColumn = GetPrivateProfileInt("SpotInfo", "nLeftColumn", 200, m_StrSpotCheckPath);
	m_tSpotParam.nRightColumn = GetPrivateProfileInt("SpotInfo", "nRightColumn", 1000, m_StrSpotCheckPath);
	m_tSpotParam.dCircleStandardArea = GetPrivateProfileDouble("SpotInfo", "dCircleStandardArea", 1.5, m_StrSpotCheckPath);
	m_tSpotParam.bEnableWriteRow = GetPrivateProfileInt("SpotInfo", "bEnableWriteRow", 0, m_StrSpotCheckPath);
	m_tSpotParam.dCircleParam = GetPrivateProfileDouble("SpotInfo", "dCircleParam", 0.65, m_StrSpotCheckPath);

	m_tSpotParam.dFLPHeight = GetPrivateProfileDouble("SpotInfo", "dFLPHeight", 5, m_StrSpotCheckPath);
	m_tSpotParam.dFLPHeightDev = GetPrivateProfileDouble("SpotInfo", "dFLPHeightDev", 0.5, m_StrSpotCheckPath);
	m_tSpotParam.dFLPWidth = GetPrivateProfileDouble("SpotInfo", "dFLPWidth", 5, m_StrSpotCheckPath);
	m_tSpotParam.dFLPWidthDev = GetPrivateProfileDouble("SpotInfo", "dFLPWidthDev", 0.5, m_StrSpotCheckPath);
	m_tSpotParam.nFLPFrameWarnFrame = GetPrivateProfileInt("SpotInfo", "nFLPFrameWarnFrame", 3, m_StrSpotCheckPath);
	m_tSpotParam.dRowResolution = GetPrivateProfileDouble("SpotInfo", "dRowResolution", 0, m_StrSpotCheckPath);

	CString strFilePath;
	strFilePath.Format("%s\\Config\\Public\\点检区域\\Paramters.ini", GetCurrentAppPath());
	m_strSpotTime[0]= GetPrivateProfileCString("SpotSet", "SpotTime1", "8:00:00", strFilePath);
	m_strSpotTime[1] = GetPrivateProfileCString("SpotSet", "SpotTime2", "20:00:00", strFilePath);
}

void   CSpotSizeCheck::WriteParam()
{
	WritePrivateProfileInt("SpotInfo", "bEnableUseSpot", m_tSpotParam.bEnableUseSpot, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nLowThresh", m_tSpotParam.nLowThresh, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nHighThresh", m_tSpotParam.nHighThresh, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nSpotNum", m_tSpotParam.nSpotNum, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nLeftColumn", m_tSpotParam.nLeftColumn, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nRightColumn", m_tSpotParam.nRightColumn, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dCircleStandardArea", m_tSpotParam.dCircleStandardArea, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "bEnableWriteRow", m_tSpotParam.bEnableWriteRow, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dCircleParam", m_tSpotParam.dCircleParam, m_StrSpotCheckPath);

	WritePrivateProfileDouble("SpotInfo", "dFLPHeight", m_tSpotParam.dFLPHeight, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dFLPHeightDev", m_tSpotParam.dFLPHeightDev, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dFLPWidth", m_tSpotParam.dFLPWidth, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dFLPWidthDev", m_tSpotParam.dFLPWidthDev, m_StrSpotCheckPath);
	WritePrivateProfileInt("SpotInfo", "nFLPFrameWarnFrame", m_tSpotParam.nFLPFrameWarnFrame, m_StrSpotCheckPath);
	WritePrivateProfileDouble("SpotInfo", "dRowResolution", m_tSpotParam.dRowResolution, m_StrSpotCheckPath);

	CString strFilePath;
	strFilePath.Format("%s\\Config\\Public\\点检区域\\Paramters.ini", GetCurrentAppPath());
	WritePrivateProfileString("SpotSet", "SpotTime1", m_strSpotTime[0], strFilePath);
	WritePrivateProfileString("SpotSet", "SpotTime2", m_strSpotTime[1], strFilePath);
}

void CSpotSizeCheck::GetRegion(HObject*hoRegion, int nType)
{

	if (nType == LeftSpotRegion)
	{

		GenRectangle1(&(*hoRegion), 0, MAX(0, m_tSpotParam.nLeftColumn - m_nWidth / 2), m_hvImageHeight - 1, MAX(0, m_tSpotParam.nLeftColumn - m_nWidth / 2) + m_nWidth);
	}
	else
	{
		GenRectangle1(&(*hoRegion), 0, MIN(m_hvImageWidth, m_tSpotParam.nRightColumn - m_nWidth / 2), m_hvImageHeight - 1, MIN(m_hvImageWidth, m_tSpotParam.nRightColumn - m_nWidth / 2) + m_nWidth);
	}
}

void CSpotSizeCheck::GetCurrentLocation(HTuple hv_RowCenter, HTuple *hv_DistanceCountOut, int nLeftOrRight)
{
	HTuple  hv_Height, hv_RowCenterSorted, hv_i;
	hv_Height = m_hvImageHeight;
	if (0 != (int((hv_RowCenter.TupleLength()) > 0)))
	{
		TupleSort(hv_RowCenter, &hv_RowCenterSorted);
		{
			HTuple end_val6 = (hv_RowCenterSorted.TupleLength()) - 1;
			HTuple step_val6 = 1;
			for (hv_i = 0; hv_i.Continue(end_val6, step_val6); hv_i += step_val6)
			{
				m_bFirst[nLeftOrRight] = 0;
				if (0 != (int(hv_i == 0)))
				{
					m_hvDistanceLocation[nLeftOrRight] = m_hvDistanceLocation[nLeftOrRight] + HTuple(hv_RowCenterSorted[0]) + m_hvdLastDistance[nLeftOrRight];
				}
				else
				{
					m_hvDistanceLocation[nLeftOrRight] = m_hvDistanceLocation[nLeftOrRight] + HTuple(hv_RowCenterSorted[hv_i]) - HTuple(hv_RowCenterSorted[hv_i - 1]);

				}
				*hv_DistanceCountOut = m_hvDistanceLocation[nLeftOrRight];
				if (0 != (int(hv_i == ((hv_RowCenterSorted.TupleLength()) - 1))))
				{
					m_hvdLastDistance[nLeftOrRight] = hv_Height - HTuple(hv_RowCenterSorted[hv_i]);

				}
			}
		}
	}
	else
	{
		if (0 != (int(m_bFirst[nLeftOrRight] == 0)))
		{
			m_hvdLastDistance[nLeftOrRight] = hv_Height + m_hvdLastDistance[nLeftOrRight];
		}
	}
	return;
}


void CSpotSizeCheck::WriteRow(HTuple hv_Distance)
{
	if (hv_Distance.Length() == 0 || m_tSpotParam.bEnableWriteRow == FALSE)
	{
		return;
	}

	CString strFilePath, strFileDirecor, strItem;
	SYSTEMTIME tSystemTime;
	GetLocalTime(&tSystemTime);
	strFileDirecor.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s\\瑕疵稳定性监控", tSystemTime.wYear, tSystemTime.wMonth, tSystemTime.wDay, g_strProductName, g_strBatch);
	strFilePath.Format("%s\\%d相机-%d时MD方向数据.csv", strFileDirecor, m_nSide, tSystemTime.wHour);

	HTuple bExists;
	FileExists(HTuple(strFilePath), &bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strFileDirecor);
		ofstream outFile;
		outFile.open(strFilePath, ios::out | ios::app);
		strItem.Format("%s,%s,%s", "时间", "运动方向绝对坐标(像素)", "运动方向绝对坐标(mm)");
		outFile << strItem << endl;
		outFile.close();
	}
	try
	{
		CString strTime;
		strTime.Format("%02d%02d%02d %02d:%02d:%02d:%03d", tSystemTime.wYear, tSystemTime.wMonth, tSystemTime.wDay, tSystemTime.wHour, tSystemTime.wMinute, tSystemTime.wSecond, tSystemTime.wMilliseconds);
		strItem.Format("%s,%.3f,%.3f", strTime, hv_Distance.D(), hv_Distance.D()*g_vec_RowResolution[0]);
		ofstream outFile;
		CString strLeftInfo, StrRightInfo;
		outFile.open(strFilePath, ios::out | ios::app);
		outFile << strItem << endl;
		outFile.close();
	}
	catch (HException& e)
	{

	}

}

void CSpotSizeCheck::WriteRowLocation(HTuple hv_RowCenter, HTuple hv_Height)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_RowCenterSorted, hv_bFirstEnable, hv_i, hv_DistanceCountOut;

	//static int hv_LastDistanceCount = 0;
	//static int hv_dLastDistance = 0;
	if (0 != (int((hv_RowCenter.TupleLength()) > 0)))
	{
		TupleSort(hv_RowCenter, &hv_RowCenterSorted);
		hv_bFirstEnable = 1;
		{
			HTuple end_val5 = (hv_RowCenterSorted.TupleLength()) - 1;
			HTuple step_val5 = 1;
			for (hv_i = 0; hv_i.Continue(end_val5, step_val5); hv_i += step_val5)
			{
				if (0 != (int(hv_i == ((hv_RowCenterSorted.TupleLength()) - 1))))
				{
					hv_DistanceCountOut = m_nLastDistanceCount[m_nSide] + hv_RowCenterSorted[hv_i].D() + m_ndLastDistance[m_nSide];
					m_ndLastDistance[m_nSide] = hv_Height.I() - hv_RowCenterSorted[hv_i].D();
					m_nLastDistanceCount[m_nSide] = hv_DistanceCountOut.D();
				}
				else
				{
					hv_DistanceCountOut = m_nLastDistanceCount[m_nSide] + hv_RowCenterSorted[hv_i].D() + m_ndLastDistance[m_nSide];
				}



				WriteRow(hv_DistanceCountOut);





			}
		}
	}
	else
	{

		m_ndLastDistance[m_nSide] = hv_Height + m_ndLastDistance[m_nSide];

	}








	return;
}

void CSpotSizeCheck::GetDestRegionInfo(HObject ho_Image, HObject ho_SpotRegion, HTuple hv_Thresh, int nLeftOrRightIndex)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_Border;

	// Local control variables
	HTuple  hv_Number, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Area, hv_PointOrder;
	HObject hoSelectRegion;
	HObject ho_SelectedXLD;
	HTuple hv_RowCenter, hv_ColumnCenter;

	ho_SelectedXLD.GenEmptyObj();
	ReduceDomain(ho_Image, ho_SpotRegion, &ho_ImageReduced);
	ThresholdSubPix(ho_ImageReduced, &ho_Border, hv_Thresh);
	SelectShapeXld(ho_Border, &ho_SelectedXLD, (HTuple("width").Append("height").Append("area").Append("circularity")),
		"and", (HTuple(5).Append(5).Append(80).Append(m_tSpotParam.dCircleParam)), (HTuple(100).Append(100).Append(12000).Append(1)));
	if (ho_SelectedXLD.CountObj() >= 2)
	{
		if (nLeftOrRightIndex == LeftSpotRegion)
		{
			HTuple hv_CurrentRow;
			AreaCenterXld(ho_SelectedXLD, NULL, &hv_CurrentRow, NULL, NULL);
			WriteRowLocation(hv_CurrentRow, m_hvImageHeight);
			SelectObj(ho_SelectedXLD, &ho_SelectedXLD, 1);


			GenRegionContourXld(ho_SelectedXLD, &hoSelectRegion, "margin");
			SelectGray(hoSelectRegion, ho_ImageReduced, &hoSelectRegion, "mean", "and", 0, m_tSpotParam.nHighThresh);

			CountObj(hoSelectRegion, &hv_Number);
			HTuple hv_Distance(0);
			if (hv_Number == 1 && 1 == ho_SelectedXLD.CountObj())
			{
				SmallestRectangle1Xld(ho_SelectedXLD, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
				if (hv_Row1 > 50 && hv_Row2 < m_hvImageHeight - 50)
				{

					AreaCenterXld(ho_SelectedXLD, &hv_Area, &hv_RowCenter, &hv_ColumnCenter, &hv_PointOrder);
					/*m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_vec_ColumnResolution[m_nSide]);
					m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
					m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide] * g_vec_RowResolution[m_nSide]);*/
					if (g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution != 0)
					{
						m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution);
						m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
						HTuple hv_Radius = (hv_Column2 - hv_Column1) / 2 * g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution;

						double dMinStandardArea = 1.5;
						double dMaxStandardArea = 2.0;
						HTuple hv_Area = pow(hv_Radius.D(), 2)*PI;
						HTuple hv_AreaCompensate;
						if (fabs(dMaxStandardArea - hv_Area.D())> fabs(dMinStandardArea - hv_Area.D()))
						{
							hv_AreaCompensate = (dMinStandardArea - hv_Area.D()) / 2;
						}
						else
						{
							hv_AreaCompensate = (dMaxStandardArea - hv_Area.D()) / 2;
						}
						m_ptSpotInfo->hvSpotArea.Append(hv_Area.D() + hv_AreaCompensate);//改为圆面积计算公式，
						m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
						ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
						GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
						m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
					}
					else
					{
						m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_vec_ColumnResolution[m_nSide]);
						m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
						m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide] * g_vec_RowResolution[m_nSide]);
						m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
						ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
						GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
						m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
					}

				}
				else
				{
					hv_RowCenter.Clear();
					GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
				}
			}
			else
			{
				hv_RowCenter.Clear();
				GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
			}
		}
		else if (nLeftOrRightIndex == RightSpotRegion)
		{
			HTuple hv_CurrentRow;
			AreaCenterXld(ho_SelectedXLD, NULL, &hv_CurrentRow, NULL, NULL);
			WriteRowLocation(hv_CurrentRow, m_hvImageHeight);
			SelectObj(ho_SelectedXLD, &ho_SelectedXLD, 2);

			GenRegionContourXld(ho_SelectedXLD, &hoSelectRegion, "margin");
			SelectGray(hoSelectRegion, ho_ImageReduced, &hoSelectRegion, "mean", "and", 0, m_tSpotParam.nHighThresh);

			CountObj(hoSelectRegion, &hv_Number);
			HTuple hv_Distance(0);
			if (hv_Number == 1 && 1 == ho_SelectedXLD.CountObj())
			{
				SmallestRectangle1Xld(ho_SelectedXLD, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
				if (hv_Row1 > 50 && hv_Row2 < m_hvImageHeight - 50)
				{

					AreaCenterXld(ho_SelectedXLD, &hv_Area, &hv_RowCenter, &hv_ColumnCenter, &hv_PointOrder);
					/*m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_vec_ColumnResolution[m_nSide]);
					m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
					m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide] * g_vec_RowResolution[m_nSide]);
					m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
					ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
					GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
					m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);*/
					if (g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution != 0)
					{
						m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution);
						m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
						HTuple hv_Radius = (hv_Column2 - hv_Column1) / 2 * g_CSpotSizeCheck[m_nSide].m_tSpotParam.dRowResolution;

						double dMinStandardArea = 1.5;
						double dMaxStandardArea = 2.0;
						HTuple hv_Area = pow(hv_Radius.D(), 2)*PI;
						HTuple hv_AreaCompensate;
						if (fabs(dMaxStandardArea - hv_Area.D()) > fabs(dMinStandardArea - hv_Area.D()))
						{
							hv_AreaCompensate = (dMinStandardArea - hv_Area.D()) / 2;
						}
						else
						{
							hv_AreaCompensate = (dMaxStandardArea - hv_Area.D()) / 2;
						}
						m_ptSpotInfo->hvSpotArea.Append(hv_Area.D() + hv_AreaCompensate);//改为圆面积计算公式，

						m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
						ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
						GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
						m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
					}
					else
					{
						m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_vec_ColumnResolution[m_nSide]);
						m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
						m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide] * g_vec_RowResolution[m_nSide]);
						m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
						ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
						GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
						m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
					}
				}
				else
				{
					hv_RowCenter.Clear();
					GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
				}
			}
			else
			{
				hv_RowCenter.Clear();
				GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
			}
		}

	}
	/*GenRegionContourXld(ho_SelectedXLD, &hoSelectRegion, "margin");
	SelectGray(hoSelectRegion, ho_ImageReduced, &hoSelectRegion, "mean", "and", 0, m_tSpotParam.nHighThresh);

	CountObj(hoSelectRegion, &hv_Number);
	HTuple hv_Distance(0);
	if (hv_Number==1&&1==ho_SelectedXLD.CountObj())
	{
	SmallestRectangle1Xld(ho_SelectedXLD, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	if (hv_Row1>50 && hv_Row2<m_hvImageHeight-50)
	{

	AreaCenterXld(ho_SelectedXLD, &hv_Area, &hv_RowCenter,&hv_ColumnCenter,&hv_PointOrder);
	m_ptSpotInfo->hvSpotWidth.Append((hv_Column2-hv_Column1)*g_vec_ColumnResolution[m_nSide]);
	m_ptSpotInfo->hvSpotHeight.Append((hv_Row2-hv_Row1)*g_vec_RowResolution[m_nSide]);
	m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide]*g_vec_RowResolution[m_nSide]);
	m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
	ConcatObj(m_ptSpotInfo->hoXldRegion,ho_SelectedXLD,&m_ptSpotInfo->hoXldRegion);
	GetCurrentLocation(hv_RowCenter,&hv_Distance,nLeftOrRightIndex);
	m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
	}else
	{
	hv_RowCenter.Clear();
	GetCurrentLocation(hv_RowCenter,&hv_Distance,nLeftOrRightIndex);
	}
	}
	else
	{
	hv_RowCenter.Clear();
	GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
	}*/
	return;
}

//void CSpotSizeCheck::GetDestRegionInfo(HObject ho_Image, HObject ho_SpotRegion, HTuple hv_Thresh, int nLeftOrRightIndex)
//{
//
//	// Local iconic variables
//	HObject  ho_ImageReduced, ho_Region, ho_Border;
//
//	// Local control variables
//	HTuple  hv_Number, hv_Row1, hv_Column1, hv_Row2;
//	HTuple  hv_Column2, hv_Area, hv_PointOrder;
//	HObject hoSelectRegion;
//	HObject ho_SelectedXLD;
//	HTuple hv_RowCenter, hv_ColumnCenter;
//
//	ho_SelectedXLD.GenEmptyObj();
//	ReduceDomain(ho_Image, ho_SpotRegion, &ho_ImageReduced);
//	ThresholdSubPix(ho_ImageReduced, &ho_Border, hv_Thresh);
//	SelectShapeXld(ho_Border, &ho_SelectedXLD, (HTuple("width").Append("height").Append("area").Append("circularity")),
//		"and", (HTuple(10).Append(10).Append(50).Append(m_tSpotParam.dCircleParam)), (HTuple(100).Append(100).Append(12000).Append(1)));
//	if (ho_SelectedXLD.CountObj() >= 1)
//	{
//		if (nLeftOrRightIndex == LeftSpotRegion)
//		{
//			HTuple hv_CurrentRow;
//			AreaCenterXld(ho_SelectedXLD, NULL, &hv_CurrentRow, NULL, NULL);
//			WriteRowLocation(hv_CurrentRow, m_hvImageHeight);
//		}
//		SelectObj(ho_SelectedXLD, &ho_SelectedXLD, 1);
//	}
//	GenRegionContourXld(ho_SelectedXLD, &hoSelectRegion, "margin");
//	SelectGray(hoSelectRegion, ho_ImageReduced, &hoSelectRegion, "mean", "and", 0, m_tSpotParam.nHighThresh);
//
//	CountObj(hoSelectRegion, &hv_Number);
//	HTuple hv_Distance(0);
//	if (hv_Number == 1 && 1 == ho_SelectedXLD.CountObj())
//	{
//		SmallestRectangle1Xld(ho_SelectedXLD, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
//		if (hv_Row1 > 50 && hv_Row2 < m_hvImageHeight - 50)
//		{
//
//			AreaCenterXld(ho_SelectedXLD, &hv_Area, &hv_RowCenter, &hv_ColumnCenter, &hv_PointOrder);
//			m_ptSpotInfo->hvSpotWidth.Append((hv_Column2 - hv_Column1)*g_vec_ColumnResolution[m_nSide]);
//			m_ptSpotInfo->hvSpotHeight.Append((hv_Row2 - hv_Row1)*g_vec_RowResolution[m_nSide]);
//			m_ptSpotInfo->hvSpotArea.Append(hv_Area*g_vec_ColumnResolution[m_nSide] * g_vec_RowResolution[m_nSide]);
//			m_ptSpotInfo->hvXValue.Append(hv_ColumnCenter*g_vec_ColumnResolution[m_nSide]);
//			ConcatObj(m_ptSpotInfo->hoXldRegion, ho_SelectedXLD, &m_ptSpotInfo->hoXldRegion);
//			GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
//			m_ptSpotInfo->hvYValue.Append(hv_Distance.D()*g_vec_RowResolution[m_nSide]);
//		}
//		else
//		{
//			hv_RowCenter.Clear();
//			GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
//		}
//
//
//
//	}
//	else
//	{
//		hv_RowCenter.Clear();
//		GetCurrentLocation(hv_RowCenter, &hv_Distance, nLeftOrRightIndex);
//	}
//	return;
//}

void   CSpotSizeCheck::WriteInfo()
{

	CString strFilePath, strFileDirecor, strItem;
	SYSTEMTIME tSystemTime;
	GetLocalTime(&tSystemTime);
	//strFileDirecor.Format("%s\\Log\\稳定性监控\\%d-%d-%d",GetCurrentAppPath(),tSystemTime.wYear,tSystemTime.wMonth,tSystemTime.wDay);
	//strFilePath.Format("%s\\%d相机-%d时点检数据.csv",strFileDirecor,m_nSide,tSystemTime.wHour);
	strFileDirecor.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s\\瑕疵稳定性监控", tSystemTime.wYear, tSystemTime.wMonth, tSystemTime.wDay, g_strProductName, g_strBatch);
	strFilePath.Format("%s\\%d相机-%d时点检数据.csv", strFileDirecor, m_nSide, tSystemTime.wHour);
	HTuple bExists;
	FileExists(HTuple(strFilePath), &bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strFileDirecor);
		ofstream outFile;
		outFile.open(strFilePath, ios::out | ios::app);
		strItem.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", "时间", "左侧宽度(mm)", "左侧高度(mm)", "左侧面积(mm^2)", "左侧X坐标(mm)", "左侧Y坐标(mm)", "右侧宽度(mm)", "右侧高度(mm)", "右侧面积(mm^2)", "右侧X坐标(mm)", "右侧Y坐标(mm)", "左右距离(mm)");
		outFile << strItem << endl;
		outFile.close();
	}


	CString strTime;
	strTime.Format("%02d:%02d:%02d %03d", tSystemTime.wHour, tSystemTime.wMinute, tSystemTime.wSecond, tSystemTime.wMilliseconds);

	if (m_ptSpotInfo->hvSpotWidth.Length() == 2)
	{
		ofstream outFile;
		CString strLeftInfo, StrRightInfo;
		outFile.open(strFilePath, ios::out | ios::app);
		strLeftInfo.Format("%.3f,%.3f,%.3f,%.3f,%.3f", m_ptSpotInfo->hvSpotWidth[0].D(), m_ptSpotInfo->hvSpotHeight[0].D(), m_ptSpotInfo->hvSpotArea[0].D(), m_ptSpotInfo->hvXValue[0].D(), m_ptSpotInfo->hvYValue[0].D());
		StrRightInfo.Format("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f", m_ptSpotInfo->hvSpotWidth[1].D(), m_ptSpotInfo->hvSpotHeight[1].D(), m_ptSpotInfo->hvSpotArea[1].D(), m_ptSpotInfo->hvXValue[1].D(), m_ptSpotInfo->hvYValue[1].D(), m_ptSpotInfo->dDistance);
		strItem.Format("%s,%s,%s,", strTime, strLeftInfo, StrRightInfo);
		outFile << strItem << endl;
		outFile.close();
	}
}

void   CSpotSizeCheck::Action(HObject& hoImage, SpotOutputInfo* ptOutInfo)//执行检测
{
	try
	{
		m_hoXLDRegion.GenEmptyObj();
		HTuple hv_Thresh;
		m_ptSpotInfo = ptOutInfo;
		m_ptSpotInfo->Init();
		m_hoLeftRegion.GenEmptyObj();
		m_hoRightRegion.GenEmptyObj();

		if (m_tSpotParam.bEnableUseSpot == FALSE)
		{
			return;
		}
		m_hoLeftRegion.GenEmptyObj();
		m_hoRightRegion.GenEmptyObj();
		hv_Thresh = m_tSpotParam.nLowThresh;
		GetImageSize(hoImage, &m_hvImageWidth, &m_hvImageHeight);
		try
		{
			GetRegion(&m_hoLeftRegion, LeftSpotRegion);
			GetRegion(&m_hoRightRegion, RightSpotRegion);

		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("CSpotSizeCheck::GetRegion 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			m_nConntinueErrorNum++;
		}
		try
		{
			GetDestRegionInfo(hoImage, m_hoLeftRegion, hv_Thresh, LeftSpotRegion);
			GetDestRegionInfo(hoImage, m_hoRightRegion, hv_Thresh, RightSpotRegion);
		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("CSpotSizeCheck::GetDestRegionInfo 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			m_nConntinueErrorNum++;
		}
		if (m_ptSpotInfo->hvXValue.Length() == 2)
		{

			m_hoXLDRegion = m_ptSpotInfo->hoXldRegion;
			m_ptSpotInfo->dDistance = (m_ptSpotInfo->hvXValue[1].D() - m_ptSpotInfo->hvXValue[0].D());
			HTuple hv_SendTuple;
			try
			{
				if (fabs(m_ptSpotInfo->hvSpotArea[0].D() - m_tSpotParam.dFLPHeight) < m_tSpotParam.dFLPHeightDev && fabs(m_ptSpotInfo->hvSpotArea[1].D() - m_tSpotParam.dFLPWidth) < m_tSpotParam.dFLPWidthDev)
				{
					m_ptSpotInfo->hvFlag = FALSE;
					m_nConntinueErrorNum = 0;
				}
				else
				{
					m_nConntinueErrorNum++;
				}

			}
			catch (HException& e)
			{
				m_nConntinueErrorNum++;
			}
		}
		else
		{
			m_nConntinueErrorNum++;
		}
		if (m_nConntinueErrorNum > m_tSpotParam.nFLPFrameWarnFrame)
		{
			CString strException;
			strException.Format("相机%d点检片连续%d帧NG报警，请排查", m_nSide, m_nConntinueErrorNum);
			g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
			m_nConntinueErrorNum = 0;
		}
		WriteInfo();
	}
	catch (HException& e)
	{

		CString strException;
		strException.Format("CSpotSizeCheck::Action 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}

void CSpotSizeCheck::GetCalibrationDestRegionInfo(HObject ho_Image, HObject ho_SpotRegion, HTuple hv_Thresh, int nSide, int nLeftOrRightIndex)
{
	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_Border;

	// Local control variables
	HTuple  hv_Number, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Area, hv_PointOrder;
	HObject hoSelectRegion;
	HObject ho_SelectedXLD;
	HTuple hv_RowCenter, hv_ColumnCenter;
	HTuple hv_CliabrationAre = g_CSpotSizeCheck[m_nSide].m_tSpotParam.dCircleStandardArea;
	HTuple hv_CliabrationRadius = sqrt(g_CSpotSizeCheck[m_nSide].m_tSpotParam.dCircleStandardArea / PI);

	ho_SelectedXLD.GenEmptyObj();
	ReduceDomain(ho_Image, ho_SpotRegion, &ho_ImageReduced);
	ThresholdSubPix(ho_ImageReduced, &ho_Border, hv_Thresh);
	SelectShapeXld(ho_Border, &ho_SelectedXLD, (HTuple("width").Append("height").Append("area").Append("circularity")),
		"and", (HTuple(5).Append(5).Append(80).Append(m_tSpotParam.dCircleParam)), (HTuple(100).Append(100).Append(12000).Append(1)));
	if (ho_SelectedXLD.CountObj() >= 2)
	{
		if (nLeftOrRightIndex == LeftSpotRegion)
		{
			HTuple hv_CurrentRow;
			AreaCenterXld(ho_SelectedXLD, NULL, &hv_CurrentRow, NULL, NULL);
			/*	WriteRowLocation(hv_CurrentRow, m_hvImageHeight);*/
			SelectObj(ho_SelectedXLD, &ho_SelectedXLD, 1);

			GenRegionContourXld(ho_SelectedXLD, &hoSelectRegion, "margin");
			SelectGray(hoSelectRegion, ho_ImageReduced, &hoSelectRegion, "mean", "and", 0, m_tSpotParam.nHighThresh);
			CountObj(hoSelectRegion, &hv_Number);
			HTuple hv_Distance(0);
			if (hv_Number == 1 && 1 == ho_SelectedXLD.CountObj())
			{
				SmallestRectangle1Xld(ho_SelectedXLD, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
				if (hv_Row1 > 50 && hv_Row2 < m_hvImageHeight - 50)
				{
					HTuple hv_TestRadius = (hv_Column2 - hv_Column1) / 2;
					m_hvRowResolution[m_nSide] = sqrt(hv_CliabrationAre.D() / PI) / hv_TestRadius;
					AfxMessageBox("点检校准成功");
				}
			}
			else
			{
				m_hvRowResolution[m_nSide] = 0;
				AfxMessageBox("点检校准失败");
			}
		}

	}
	else
	{
		m_hvRowResolution[m_nSide] = 0;
		AfxMessageBox("点检校准失败");
	}
	return;
}


void CSpotSizeCheck::CalibrationAction(HObject& hoImage, SpotOutputInfo* ptOutInfo, int nSide)
{
	try
	{

		m_hoXLDRegion.GenEmptyObj();
		HTuple hv_Thresh;
		m_ptSpotInfo = ptOutInfo;
		m_ptSpotInfo->Init();
		m_hoLeftRegion.GenEmptyObj();
		m_hoRightRegion.GenEmptyObj();

		m_hoLeftRegion.GenEmptyObj();
		m_hoRightRegion.GenEmptyObj();
		hv_Thresh = m_tSpotParam.nLowThresh;
		GetImageSize(hoImage, &m_hvImageWidth, &m_hvImageHeight);
		try
		{
			GetRegion(&m_hoLeftRegion, LeftSpotRegion);
		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("CSpotSizeCheck::CalibrationAction:GetRegion 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
		}


		try
		{
			GetCalibrationDestRegionInfo(hoImage, m_hoLeftRegion, hv_Thresh, nSide, LeftSpotRegion);
			g_CSpotSizeCheck[nSide].m_tSpotParam.dRowResolution = m_hvRowResolution[nSide].D();
		}
		catch (HException& e)
		{
			CString strException;
			strException.Format("CSpotSizeCheck::GetCalibrationDestRegionInfo 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
				, e.ProcName().Text());
		}

		if (m_ptSpotInfo->hvXValue.Length() == 2)
		{

			m_hoXLDRegion = m_ptSpotInfo->hoXldRegion;
			m_ptSpotInfo->dDistance = (m_ptSpotInfo->hvXValue[1].D() - m_ptSpotInfo->hvXValue[0].D());
			HTuple hv_SendTuple;
		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("CSpotSizeCheck::CalibrationAction 异常信息%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
	}
}