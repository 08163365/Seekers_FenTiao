#include "StdAfx.h"
#include "Find.h"
#include "DlgParamSet.h"



CFindMark::CFindMark(void)
{
	InitializeCriticalSection(&m_csFindChange);
}


CFindMark::~CFindMark(void)
{
	ClearMark();
	DeleteCriticalSection(&m_csFindChange);
}
CFindMark& CFindMark::operator= (const CFindMark& in)
{

	m_tInitParam = in.m_tInitParam;
	m_tMarkList = in.m_tMarkList;
	m_nType = in.m_nType;
	m_nMaxThresh = in.m_nMaxThresh;
	m_dIgnoreDist = in.m_dIgnoreDist;
	m_dRowResolution = in.m_dRowResolution;
	m_dModePosition = in.m_dModePosition;
	m_nBackLowThresh = in.m_nBackLowThresh;
	m_nBackHighThresh = in.m_nBackHighThresh;
	m_dFoilSaveHeight = in.m_dFoilSaveHeight;
	m_dJXEADev = in.m_dJXEADev;
	m_nJXGrayType = in.m_nJXGrayType;
	m_dLocateOffset = in.m_dLocateOffset;
	m_nLocateUpOrDown = in.m_nLocateUpOrDown;
	return *this;
}
int CFindMark::ShowDialog(HObject ho_Image)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	try
	{
		CDlgParamSet dlg;
		dlg.SetOwner(this);
		dlg.m_pOwner->DoSomething("COPYOBJECTS", this, NULL);
		dlg.m_hoCurrentDispImage.GenEmptyObj();
		CopyObj(ho_Image, &dlg.m_hoCurrentDispImage, 1, ho_Image.CountObj());
		dlg.DoModal();
		if (dlg.m_bChangeFlag == TRUE)
		{
			return TRUE;

		}
		else
		{
			return FALSE;
		}
	}
	catch (HException)
	{

	}
	return TRUE;
}
int CFindMark::Init(NS_SCIFindMark::INITPARAM& tInitParam)//初始化
{
	SetSystem("border_shape_models", "false");
	m_tInitParam = tInitParam;
	m_strConfigPath.Format("%s\\Config\\Products\\%s\\SCIFindMark\\%d", GetCurrentAppPath(), tInitParam.strProductName, tInitParam.nCameraSN);
	CreateMultilevelPath(m_strConfigPath);
	m_strConfigFile.Format("%s\\Config\\Products\\%s\\SCIFindMark\\%d\\Config.ini", GetCurrentAppPath(), tInitParam.strProductName, tInitParam.nCameraSN);
	m_nType = GetPrivateProfileInt("Mark", "Type", 0, m_strConfigFile);
	m_nMaxThresh = GetPrivateProfileInt("Mark", "m_nMaxThresh", 30, m_strConfigFile);
	m_dIgnoreDist = GetPrivateProfileDouble("Mark", "m_dIgnoreDist", 50, m_strConfigFile);//默认50mm
	m_dModePosition = GetPrivateProfileDouble("Mark", "m_dModePosition", -1, m_strConfigFile);//默认
	m_nBackLowThresh = GetPrivateProfileInt("Mark", "m_nBackLowThresh", 0, m_strConfigFile);
	m_nBackHighThresh = GetPrivateProfileInt("Mark", "m_nBackHighThresh", 30, m_strConfigFile);
	m_dFoilSaveHeight = GetPrivateProfileDouble("Mark", "m_dFoilSaveHeight", 5, m_strConfigFile);//默认
	m_dJXEADev = GetPrivateProfileDouble("Mark", "m_dJXEADev", 4, m_strConfigFile);//默认
	m_nJXGrayType = GetPrivateProfileInt("Mark", "m_nJXGrayType", 1, m_strConfigFile);
	m_dLocateOffset = GetPrivateProfileDouble("Mark", "m_dLocateOffset", 10, m_strConfigFile);//默认
	m_nLocateUpOrDown = GetPrivateProfileInt("Mark", "m_nLocateUpOrDown", 0, m_strConfigFile);//默认

	ReadMark();


	return TRUE;
}


void CFindMark::GetInternalCoatBottom(HObject ho_Image, HObject ho_DetetctRegion, HObject *ho_SelectedRegions1,
	HTuple *hv_LTRow, HTuple *hv_LTColumn, HTuple *hv_BotRow, HTuple *hv_BotColumn)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_RegionFillUp;
	HObject  ho_ConnectedRegions, ho_SelectedRegions;

	// Local control variables
	HTuple  hv_nJianXie, hv_Number;
	HTuple hv_RegionWidth;
	hv_nJianXie = m_dIgnoreDist / m_dRowResolution;
	ReduceDomain(ho_Image, ho_DetetctRegion, &ho_ImageReduced);
	RegionFeatures(ho_DetetctRegion, "width", &hv_RegionWidth);
	if (hv_RegionWidth.Length() == 0)
	{
		hv_RegionWidth = 11;
	}
	if (m_nJXGrayType == 1)//白色
	{
		Threshold(ho_ImageReduced, &ho_Region, m_nMaxThresh, 255);
		FillUp(ho_Region, &ho_RegionFillUp);
		ClosingRectangle1(ho_RegionFillUp, &ho_RegionFillUp, 33, 31);//中间有白条会找不到
		OpeningRectangle1(ho_RegionFillUp, &ho_RegionFillUp, 11, 1);
	}
	else
	{
		HTuple hvMean, hvDev;
		HTuple hv_SortIndex;
		Intensity(ho_DetetctRegion, ho_Image, &hvMean, &hvDev);

		if (hvMean.Length() > 0)
		{
			Threshold(ho_ImageReduced, &ho_Region, 0, MAX(15, hvMean.D() - m_nMaxThresh));
		}
		else
		{
			GenEmptyObj(&ho_Region);
		}
		FillUp(ho_Region, &ho_RegionFillUp);
		ClosingRectangle1(ho_RegionFillUp, &ho_RegionFillUp, hv_RegionWidth[0], 2 / m_dRowResolution);//中间有白条会找不到
		OpeningRectangle1(ho_RegionFillUp, &ho_RegionFillUp, hv_RegionWidth[0] / 3, 1);
	}


	Connection(ho_RegionFillUp, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("convexity").Append("rectangularity")),
		"and", (HTuple(0.85).Append(0.9)), (HTuple(1).Append(1)));
	////
	SelectShape(ho_SelectedRegions, &(*ho_SelectedRegions1), "height", "and", MAX(0, hv_nJianXie.D() - m_dJXEADev / m_dRowResolution), MAX(1, hv_nJianXie.D() + m_dJXEADev / m_dRowResolution));
	//SelectShape(ho_SelectedRegions, &(*ho_SelectedRegions1), "height", "and", hv_nJianXie-40,hv_nJianXie+40);
	CountObj((*ho_SelectedRegions1), &hv_Number);
	if (0 != (int(hv_Number > 0)))
	{
		SmallestRectangle1((*ho_SelectedRegions1), &(*hv_LTRow), &(*hv_LTColumn), &(*hv_BotRow),
			&(*hv_BotColumn));
		if (m_nLocateUpOrDown == 0)//找间歇的顶部)
		{
			*hv_BotRow = *hv_LTRow;
		}
	}
	else
	{
		(*hv_LTRow).Clear();
		(*hv_LTColumn).Clear();
		(*hv_BotRow).Clear();
		(*hv_BotColumn).Clear();
	}
	return;
}


int CFindMark::ThresholdFindIntermission(HObject ho_Image, NS_SCIFindMark::PMARKRESULT ptResult)//采用阈值的方式
{
	ptResult->hv_Angle.Clear();
	ptResult->hv_Column.Clear();
	ptResult->hv_Angle.Clear();
	ptResult->hv_Score.Clear();
	ptResult->hv_AllScore.Clear();
	ptResult->ho_DispObj.GenEmptyObj();

	HTuple hv_Width, hv_Height;
	HObject ho_Search;
	HTuple hv_Row, hv_Column, hv_Angle, hv_Score;
	HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	HTuple  hv_LTopRow, hv_LTopColumn, hv_BottomRow, hv_BotmColumn;
	HObject hoSelectRegion;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	SmallestRectangle1(m_tMarkList.at(0).ho_ModleInfo.SelectObj(3), &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	GenRectangle1(&ho_Search, 0, hv_Column1, hv_Height - 1, hv_Column2);
	GetInternalCoatBottom(ho_Image, ho_Search, &hoSelectRegion, &hv_LTopRow, &hv_LTopColumn, &hv_BottomRow, &hv_BotmColumn);
	ptResult->ho_DispObj = hoSelectRegion;
	if (hv_BottomRow.Length() > 0)
	{
		HTuple hv_Index;


		TupleSortIndex(hv_BottomRow, &hv_Index);
		TupleSelect(hv_BottomRow, hv_Index, &hv_BottomRow);
		TupleSelect(hv_BotmColumn, hv_Index, &hv_BotmColumn);




		double dLimit, dOffset;
		if (m_dLocateOffset < m_dIgnoreDist)//防呆
		{
			dOffset = MAX(-20 / m_dRowResolution, MIN(20 / m_dRowResolution, m_dLocateOffset / m_dRowResolution));
		}
		else
		{
			dOffset = 0;
		}

		for (int i = 0; i < hv_BottomRow.Length(); i++)
		{


			if (m_nLocateUpOrDown == 1)//找间歇的底部
			{
				dLimit = hv_Height - 20;
			}
			else
			{
				dLimit = hv_Height - 20 - m_dIgnoreDist / m_dRowResolution;//找间歇的顶部
			}

			if (hv_BottomRow[i]<dLimit && hv_BottomRow[i]>m_dIgnoreDist / m_dRowResolution)
			{
				if (ptResult->hv_Row.Length() == 0)
				{
					if (hv_BottomRow[i] > m_dIgnoreDist / m_dRowResolution)
					{
						ptResult->hv_Row.Append(MAX(0, hv_BottomRow[i] + dOffset));
						ptResult->hv_Column.Append(hv_BotmColumn[i]);
						ptResult->hv_Angle.Append(0);
						ptResult->hv_Score.Append(1);
						ptResult->hv_AllScore.Append(1);
					}

				}
				else
				{
					if (hv_BottomRow[i] - ptResult->hv_Row[ptResult->hv_Row.Length() - 1] > m_dIgnoreDist / m_dRowResolution)
					{
						ptResult->hv_Row.Append(MAX(0, hv_BottomRow[i] + dOffset));
						ptResult->hv_Column.Append(hv_BotmColumn[i]);
						ptResult->hv_Angle.Append(0);
						ptResult->hv_Score.Append(1);
						ptResult->hv_AllScore.Append(1);
					}

				}


			}





		}

	}
	return TRUE;
}


//
//int CFindMark::Find(const HObject& ho_Image,NS_SCIFindMark::PMARKRESULT ptResult)//匹配
//{
//	ptResult->hv_Angle.Clear();
//	ptResult->hv_Column.Clear();
//	ptResult->hv_Angle.Clear();
//	ptResult->hv_Score.Clear();
//	ptResult->hv_AllScore.Clear();
//	ptResult->ho_DispObj.GenEmptyObj();
//	try
//	{
//		HTuple hv_Width,hv_Height;
//		GetImageSize(ho_Image,&hv_Width,&hv_Height);
//		std::vector<SINGLEMARKINFO>::iterator it;
//		HTuple hv_RowAll, hv_ColumnAll, hv_AngleAll, hv_ScoreAll;
//		HObject ho_Search,ho_ImageReduced;
//		HTuple hv_Row, hv_Column, hv_Angle, hv_Score;
//		for(it=m_tMarkList.begin();it!=m_tMarkList.end();it++)
//		{
//			//先计算出来搜索区域
//			HTuple hv_Row1,hv_Column1,hv_Row2,hv_Column2;
//			SmallestRectangle1(it->ho_ModleInfo.SelectObj(3),&hv_Row1,&hv_Column1,&hv_Row2,&hv_Column2);
//			GenRectangle1(&ho_Search,0,hv_Column1,hv_Height-1,hv_Column2);
//			ReduceDomain(ho_Image,ho_Search,&ho_ImageReduced);
//			if ("形状匹配" == it->hv_ModelParam[1])
//			{
//		
//				FindShapeModel(ho_ImageReduced,it->hv_ModelID,HTuple(-5).TupleRad(), HTuple(10).TupleRad(), 
//					0.5, 1, 0.1, "none", 0, 0.9, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
//			}else if ("灰度匹配" == it->hv_ModelParam[1])
//			{
//			    double dMinScore = 0.8;
//				if (it->hv_ModelParam.Length()>=4)
//				{
//				   dMinScore = it->hv_ModelParam[3];
//				}
//				FindNccModel (ho_ImageReduced, it->hv_ModelID,HTuple(-10).TupleRad(), HTuple(10).TupleRad(), 
//					dMinScore, 1, 0.5, "false", 0, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
//			}
//			if (0 == hv_Score.Length() )
//			{
//				hv_Row    = 0;
//				hv_Column = 0;
//				hv_Angle  = 0;
//				hv_Score  = 0;
//			}
//			hv_RowAll.Append(hv_Row);
//			hv_ColumnAll.Append(hv_Column);
//			hv_AngleAll.Append(hv_Angle);
//			hv_ScoreAll.Append(hv_Score);
//			ptResult->hv_AllScore.Append(hv_Score);
//		}
//		HTuple hv_Indices,hv_Index(0);
//		TupleSortIndex(hv_ScoreAll,&hv_Indices);
//		hv_Index = hv_Indices[hv_Indices.Length()-1];//匹配率最高的
//		if (0 == hv_ScoreAll[hv_Index])//如果匹配率最高的是0，
//		{
//			ptResult->hv_Angle = 0;
//			ptResult->hv_Column = 0;
//			ptResult->hv_Row =0;
//			ptResult->hv_Score = 0;
//			return TRUE;
//		}
//		else
//		{
//			HObject hoSearch = m_tMarkList[hv_Index.I()].ho_ModleInfo.SelectObj(3);
//			HObject hoModel  = m_tMarkList[hv_Index.I()].ho_ModleInfo.SelectObj(2);
//			HObject hoLine   = m_tMarkList[hv_Index.I()].ho_Line;
//			HTuple hv_HomMat2D;
//			HTuple hv_ModelCenterRow,hv_ModelCenterColumn;
//			AreaCenter(m_tMarkList[hv_Index.I()].ho_ModleInfo.SelectObj(2),NULL,&hv_ModelCenterRow,&hv_ModelCenterColumn);
//			VectorAngleToRigid(hv_ModelCenterRow,hv_ModelCenterColumn,0,hv_RowAll[hv_Index],hv_ColumnAll[hv_Index],hv_AngleAll[hv_Index],&hv_HomMat2D);
//			HObject ho_RegionAffineTrans,ho_RegionAffineTrans1;
//			AffineTransRegion(hoLine,&ho_RegionAffineTrans,hv_HomMat2D,"constant");
//			AffineTransRegion(hoModel,&ho_RegionAffineTrans1,hv_HomMat2D,"constant");
//			AreaCenter(ho_RegionAffineTrans,NULL,&ptResult->hv_Row,&ptResult->hv_Column);
//			ptResult->hv_Angle = 0;
//			ptResult->hv_Score = hv_ScoreAll[hv_Index];
//			ConcatObj(ho_RegionAffineTrans1,ho_RegionAffineTrans,&ptResult->ho_DispObj);
//			
//		}
//
//	}catch(HException e)
//	{
//		return FALSE;
//	}
//	return TRUE;
//}

int CFindMark::FindMult(const HObject& ho_Image, NS_SCIFindMark::PMARKRESULT ptResult)//匹配
{
	ptResult->hv_Angle.Clear();
	ptResult->hv_Column.Clear();
	ptResult->hv_Angle.Clear();
	ptResult->hv_Score.Clear();
	ptResult->hv_AllScore.Clear();
	ptResult->ho_DispObj.GenEmptyObj();
	if (m_tMarkList.size() > 0)
	{

		if (m_tMarkList.at(0).hv_ModelParam[1] == "阈值分割匹配")
		{
			try
			{
				ThresholdFind(ho_Image, ptResult);
			}
			catch (HException& e)
			{
				return FALSE;
			}
			return TRUE;
		}
		else if (m_tMarkList.at(0).hv_ModelParam[1] == "间歇分割")
		{
			try
			{
				ThresholdFindIntermission(ho_Image, ptResult);
			}
			catch (HException& e)
			{
				return FALSE;
			}

			return TRUE;
		}
		else
		{
			try
			{
				HTuple hv_Width, hv_Height;
				GetImageSize(ho_Image, &hv_Width, &hv_Height);
				std::vector<SINGLEMARKINFO>::iterator it;
				HTuple hv_RowAll, hv_ColumnAll, hv_AngleAll, hv_ScoreAll;
				HObject ho_Search, ho_ImageReduced;
				HTuple hv_Row, hv_Column, hv_Angle, hv_Score;
				BOOL   bAutoFindLocation = TRUE;
				int k = 0;
				for (it = m_tMarkList.begin(); it != m_tMarkList.end(); it++)
				{
					//先计算出来搜索区域
					HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
					SmallestRectangle1(it->ho_ModleInfo.SelectObj(3), &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
					GenRectangle1(&ho_Search, 0, hv_Column1, hv_Height - 1, hv_Column2);
					ReduceDomain(ho_Image, ho_Search, &ho_ImageReduced);
					HTuple hv_Row0, hv_Column0;
					HObject ho_CropPart;
					CropDomain(ho_ImageReduced, &ho_CropPart);
					SmallestRectangle1(ho_ImageReduced, &hv_Row0, &hv_Column0, NULL, NULL);

					if ("形状匹配" == it->hv_ModelParam[1])
					{
						double dMinScore = it->hv_ModelParam[3].D();
						if (it->hv_ModelParam.Length() >= 4)
						{
							dMinScore = it->hv_ModelParam[3];
						}
						FindShapeModel(ho_CropPart, it->hv_ModelID, HTuple(-2).TupleRad(), HTuple(4).TupleRad(), dMinScore, 10/*最多寻找10个定位*/, 0.5, "least_squares", (HTuple(3).Append(1)), 0.75, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
					}
					else if ("灰度匹配" == it->hv_ModelParam[1])
					{
						double dMinScore = it->hv_ModelParam[3].D();
						if (it->hv_ModelParam.Length() >= 4)
						{
							dMinScore = it->hv_ModelParam[3];
						}
						FindNccModel(ho_CropPart, it->hv_ModelID, HTuple(-5).TupleRad(), HTuple(10).TupleRad(), dMinScore, 0/*寻找所有匹配点*/, 0.5, "true", 0 /*HTuple(4).Append(1)*/, &hv_Row, &hv_Column, &hv_Angle, &hv_Score);
					}
					if (0 == hv_Score.Length())
					{
						hv_Row = 0;
						hv_Column = 0;
						hv_Angle = 0;
						hv_Score = 0;
					}
					if (hv_Score.Length() > 0)
					{
						HTuple hv_ActualRow, hv_ActualColumn;
						hv_ActualRow = hv_Row + hv_Row0;
						hv_ActualColumn = hv_Column + hv_Column0;
						hv_RowAll.Append(hv_ActualRow);
						hv_ColumnAll.Append(hv_ActualColumn);
						hv_AngleAll.Append(hv_Angle);
						hv_ScoreAll.Append(hv_Score);
						ptResult->hv_AllScore.Append(hv_Score);
					}

				}


				HTuple hv_Indices, hv_Index(0);
				TupleSortIndex(hv_ScoreAll, &hv_Indices);
				hv_Index = hv_Indices[hv_Indices.Length() - 1];//匹配率最高的
				if (0 == hv_ScoreAll[hv_Index])//如果匹配率最高的是0，
				{
					ptResult->hv_Angle = 0;
					ptResult->hv_Column = 0;
					ptResult->hv_Row = 0;
					ptResult->hv_Score = 0;
					ptResult->hv_AllScore = 0;
					return TRUE;
				}
				else
				{
					HObject hoSearch = m_tMarkList[0].ho_ModleInfo.SelectObj(3);
					HObject hoModel = m_tMarkList[0].ho_ModleInfo.SelectObj(2);
					HObject hoLine = m_tMarkList[0].ho_Line;
					HTuple hv_HomMat2D;
					HTuple hv_ModelCenterRow, hv_ModelCenterColumn;
					AreaCenter(hoModel, NULL, &hv_ModelCenterRow, &hv_ModelCenterColumn);

					////进行Row排序
					TupleSortIndex(hv_RowAll, &hv_Indices);
					TupleSelect(hv_RowAll, hv_Indices, &hv_RowAll);
					HTuple hv_TempRow, hv_TempIndex;
					hv_TempRow.Clear();
					hv_TempIndex.Clear();
					for (int k = 0; k < hv_RowAll.Length(); k++)
					{
						if (hv_TempRow.Length() == 0)
						{
							if (hv_RowAll[k] > m_dIgnoreDist / m_dRowResolution)//相当于屏蔽了最小间距
							{
								hv_TempRow.Append(hv_RowAll[k]);
								hv_TempIndex.Append(hv_Indices[k]);
							}

						}
						else
						{
							if (hv_RowAll[k] - hv_TempRow[hv_TempRow.Length() - 1] > m_dIgnoreDist / m_dRowResolution)//0515
							{
								hv_TempRow.Append(hv_RowAll[k]);
								hv_TempIndex.Append(hv_Indices[k]);
							}
						}
					}
					hv_RowAll = hv_TempRow;
					TupleSelect(hv_AngleAll, hv_TempIndex, &hv_AngleAll);
					TupleSelect(hv_ColumnAll, hv_TempIndex, &hv_ColumnAll);
					TupleSelect(hv_ScoreAll, hv_TempIndex, &hv_ScoreAll);
					ptResult->hv_Row.Clear();
					ptResult->hv_Column.Clear();
					ptResult->hv_Angle.Clear();
					ptResult->hv_Score.Clear();
					ptResult->hv_AllScore.Clear();

					for (int i = 0; i < hv_RowAll.Length(); i++)
					{
						VectorAngleToRigid(hv_ModelCenterRow, hv_ModelCenterColumn, 0, hv_RowAll[i], hv_ColumnAll[i], hv_AngleAll[i], &hv_HomMat2D);
						HObject ho_RegionAffineTrans, ho_RegionAffineTrans1;
						AffineTransRegion(hoLine, &ho_RegionAffineTrans, hv_HomMat2D, "constant");
						AffineTransRegion(hoModel, &ho_RegionAffineTrans1, hv_HomMat2D, "constant");
						HTuple hv_Row, hv_Column;
						AreaCenter(ho_RegionAffineTrans, NULL, &hv_Row, &hv_Column);


						ptResult->hv_Row.Append(hv_RowAll[i]);
						ptResult->hv_Column.Append(hv_ColumnAll[i]);
						ptResult->hv_Angle.Append(hv_AngleAll[i]);
						ptResult->hv_Score.Append(hv_ScoreAll[i]);
						ptResult->hv_AllScore.Append(hv_ScoreAll[i]);

						HObject ho_Union2;
						Union2(ho_RegionAffineTrans1, ho_RegionAffineTrans, &ho_Union2);
						ConcatObj(ptResult->ho_DispObj, ho_Union2, &ptResult->ho_DispObj);
					}


				}

			}
			catch (HException e)
			{
				return FALSE;
			}
		}

	}



	return TRUE;
}


void  CFindMark::GetTestPositions(HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions,
	HTuple *hv_Row2, HTuple *hv_Column2)
{
	try
	{
		// Local iconic variables
		HObject  ho_Rectangle, ho_ImageReduced, ho_Region;
		HObject  ho_ConnectedRegions1, ho_SelectedRegions1, ho_RegionClosing1;
		HObject  ho_RegionTrans, ho_ImageReduced1, ho_RegionClosing;
		HObject  ho_RegionOpening, ho_RegionFillUp, ho_ConnectedRegions;

		// Local control variables
		HTuple  hv_BackLowThresh, hv_BackHighThresh, hv_FoilThresh;
		HTuple  hv_Width, hv_Height, hv_Row11, hv_Column11, hv_Row21;
		HTuple  hv_Column21, hv_Number, hv_nSN, hv_LefTopRow, hv_LefTopColumn;
		HTuple  hv_RightBottomRow, hv_RightBottomColumn;

		hv_BackLowThresh = m_nBackLowThresh;
		hv_BackHighThresh = m_nBackHighThresh;
		hv_FoilThresh = m_nMaxThresh;
		GenEmptyObj(&(*ho_SelectedRegions));
		GetImageSize(ho_Image, &hv_Width, &hv_Height);
		SmallestRectangle1(ho_ROI_0, &hv_Row11, &hv_Column11, &hv_Row21, &hv_Column21);
		GenRectangle1(&ho_Rectangle, 0, hv_Column11, hv_Height - 1, hv_Column21);
		ReduceDomain(ho_Image, ho_Rectangle, &ho_ImageReduced);
		//****提取背景区域
		Threshold(ho_ImageReduced, &ho_Region, MIN(hv_BackLowThresh.I(), hv_BackHighThresh.I()), MAX(hv_BackLowThresh.I(), hv_BackHighThresh.I()));
		Connection(ho_Region, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		ClosingRectangle1(ho_SelectedRegions1, &ho_RegionClosing1, 15, hv_Height - 1);
		OpeningRectangle1(ho_RegionClosing1, &ho_RegionClosing1, 1, hv_Height / 3);
		Connection(ho_RegionClosing1, &ho_ConnectedRegions1);
		SelectShapeStd(ho_ConnectedRegions1, &ho_SelectedRegions1, "max_area", 70);
		ShapeTrans(ho_SelectedRegions1, &ho_RegionTrans, "rectangle2");
		ErosionRectangle1(ho_RegionTrans, &ho_RegionTrans, 21, 1);
		ReduceDomain(ho_ImageReduced, ho_RegionTrans, &ho_ImageReduced1);
		//***提取极耳
		Threshold(ho_ImageReduced1, &ho_Region, MIN(hv_FoilThresh.I(), 255), 255);
		ClosingRectangle1(ho_Region, &ho_RegionClosing, 15, 15);
		OpeningRectangle1(ho_RegionClosing, &ho_RegionOpening, 11, 1);
		FillUp(ho_RegionOpening, &ho_RegionFillUp);
		Connection(ho_RegionFillUp, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &(*ho_SelectedRegions), (HTuple("width").Append("height")),
			"and", (HTuple(20).Append(20)), (HTuple(1500).Append(2000)));
		ClosingRectangle1((*ho_SelectedRegions), &(*ho_SelectedRegions), 15, 1);

		hv_nSN = m_tInitParam.nCameraSN;
		if (ho_RegionTrans.CountObj() > 0)
		{

			if (hv_nSN == 0 || hv_nSN == 2)
			{
				HTuple hv_Row00, hv_Column00, hv_Row11, hv_Column11;
				SmallestRectangle1(ho_RegionTrans, &hv_Row00, &hv_Column00, &hv_Row11, &hv_Column11);
				GenRectangle1(&ho_RegionTrans, hv_Row00, hv_Column11 - m_dFoilSaveHeight / m_dRowResolution, hv_Row11, hv_Column11);
			}
			else
			{
				HTuple hv_Row00, hv_Column00, hv_Row11, hv_Column11;
				SmallestRectangle1(ho_RegionTrans, &hv_Row00, &hv_Column00, &hv_Row11, &hv_Column11);
				GenRectangle1(&ho_RegionTrans, hv_Row00, hv_Column00, hv_Row11, hv_Column00 + m_dFoilSaveHeight / m_dRowResolution);

			}

			Intersection(ho_RegionTrans, *ho_SelectedRegions, &(*ho_SelectedRegions));

		}

		Connection(*ho_SelectedRegions, &(*ho_SelectedRegions));


		CountObj((*ho_SelectedRegions), &hv_Number);
		//hv_nSN = m_tInitParam.nCameraSN;
		if (0 != (int(hv_Number > 0)))
		{

			SmallestRectangle1((*ho_SelectedRegions), &hv_LefTopRow, &hv_LefTopColumn, &hv_RightBottomRow,
				&hv_RightBottomColumn);
			//****左侧
			if (0 != (int(hv_nSN == 0 || hv_nSN == 2)))
			{
				(*hv_Row2) = hv_LefTopRow;
				(*hv_Column2) = hv_RightBottomColumn;
			}
			else
			{
				//**右侧
				(*hv_Row2) = hv_LefTopRow;
				(*hv_Column2) = hv_LefTopColumn;
			}

			ConcatObj((*ho_SelectedRegions), ho_RegionTrans, &(*ho_SelectedRegions));
		}
		else
		{
			(*hv_Row2) = HTuple();
			(*hv_Column2) = HTuple();
			GenEmptyObj(&(*ho_SelectedRegions));
		}




	}
	catch (HException& e)
	{
		(*hv_Row2) = HTuple();
		(*hv_Column2) = HTuple();
		GenEmptyObj(&(*ho_SelectedRegions));

	}


	return;
}

void CFindMark::GetPositions(HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions,
	HTuple *hv_Row2, HTuple *hv_Column2)
{


	// Local iconic variables
	HObject  ho_Rectangle, ho_ImageReduced, ho_Region;
	HObject  ho_RegionOpening, ho_ConnectedRegions, ho_ErosionRectangle;

	// Local control variables
	HTuple  hv_Width, hv_Height, hv_Row11, hv_Column11;
	HTuple  hv_Row21, hv_Column21, hv_Number, hv_nSN, hv_LTRow;
	HTuple  hv_LTColumn, hv_RBRow, hv_RBColumn, hv_Row1, hv_Column1;

	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	SmallestRectangle1(ho_ROI_0, &hv_Row11, &hv_Column11, &hv_Row21, &hv_Column21);
	GenRectangle1(&ho_Rectangle, 0, hv_Column11, hv_Height - 1, hv_Column21);
	ReduceDomain(ho_Image, ho_Rectangle, &ho_ImageReduced);
	Threshold(ho_ImageReduced, &ho_Region, 0, m_nMaxThresh);
	ClosingRectangle1(ho_Region, &ho_Region, 3, 15);
	HTuple hvRegionWidth;
	RegionFeatures(ho_Region, "width", &hvRegionWidth);

	if (ho_Region.CountObj() > 0)
	{
		OpeningRectangle1(ho_Region, &ho_RegionOpening, hvRegionWidth / 3, 1);
		Connection(ho_RegionOpening, &ho_ConnectedRegions);
		SelectShape(ho_ConnectedRegions, &(*ho_SelectedRegions), (HTuple("area").Append("height")),
			"and", (HTuple(50).Append(30)), (HTuple(9999999999999).Append(9999999999999)));
		CountObj((*ho_SelectedRegions), &hv_Number);
		hv_nSN = m_tInitParam.nCameraSN;
		if (0 != (int(hv_Number > 0)))
		{
			SmallestRectangle1((*ho_SelectedRegions), &hv_LTRow, &hv_LTColumn, &hv_RBRow,
				&hv_RBColumn);
			if (0 != (int(hv_nSN == 0)))
			{
				GenRectangle1(&ho_ErosionRectangle, hv_LTRow, hv_RBColumn - 20, hv_RBRow, hv_RBColumn);
				Intersection(ho_ErosionRectangle, (*ho_SelectedRegions), &(*ho_SelectedRegions));
				SmallestRectangle1((*ho_SelectedRegions), &hv_Row1, &hv_Column1, &(*hv_Row2), &(*hv_Column2));
			}
			else
			{
				GenRectangle1(&ho_ErosionRectangle, hv_LTRow, hv_LTColumn, hv_RBRow, hv_LTColumn + 20);
				Intersection(ho_ErosionRectangle, (*ho_SelectedRegions), &(*ho_SelectedRegions));
				SmallestRectangle1((*ho_SelectedRegions), &hv_Row1, &hv_Column1, &(*hv_Row2), &(*hv_Column2));
			}


		}


	}
	else
	{
		(*hv_Row2).Clear();
		(*hv_Column2).Clear();
		GenEmptyObj(&(*ho_SelectedRegions));
	}





	return;
}

int CFindMark::ThresholdFind(HObject ho_Image, NS_SCIFindMark::PMARKRESULT ptResult)//采用阈值的方式
{
	ptResult->hv_Angle.Clear();
	ptResult->hv_Column.Clear();
	ptResult->hv_Angle.Clear();
	ptResult->hv_Score.Clear();
	ptResult->hv_AllScore.Clear();
	ptResult->ho_DispObj.GenEmptyObj();

	HTuple hv_Width, hv_Height;
	HObject ho_Search;
	HTuple hv_Row, hv_Column, hv_Angle, hv_Score;
	HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
	HTuple hv_BottomRow, hv_BotmColumn;
	HObject hoSelectRegion;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	SmallestRectangle1(m_tMarkList.at(0).ho_ModleInfo.SelectObj(3), &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
	GenRectangle1(&ho_Search, 0, hv_Column1, hv_Height - 1, hv_Column2);
	GetTestPositions(ho_Image, ho_Search, &hoSelectRegion, &hv_BottomRow, &hv_BotmColumn);
	ptResult->ho_DispObj = hoSelectRegion;

	if (hv_BottomRow.Length() > 0)
	{
		HTuple hv_Index;
		TupleSortIndex(hv_BottomRow, &hv_Index);
		TupleSelect(hv_BottomRow, hv_Index, &hv_BottomRow);
		TupleSelect(hv_BotmColumn, hv_Index, &hv_BotmColumn);
		for (int i = 0; i < hv_BottomRow.Length(); i++)
		{
			//if (hv_BottomRow[i]<hv_Height-60)
			{

				if (ptResult->hv_Row.Length() == 0)
				{
					if (hv_BottomRow[i] > m_dIgnoreDist / m_dRowResolution)//新修改
					{

						ptResult->hv_Row.Append(hv_BottomRow[i]);
						ptResult->hv_Column.Append(hv_BotmColumn[i]);
						ptResult->hv_Angle.Append(0);
						ptResult->hv_Score.Append(1);
						ptResult->hv_AllScore.Append(1);
					}
				}
				else
				{
					if (hv_BottomRow[i] - ptResult->hv_Row[ptResult->hv_Row.Length() - 1] > m_dIgnoreDist / m_dRowResolution)
					{
						ptResult->hv_Row.Append(hv_BottomRow[i]);
						ptResult->hv_Column.Append(hv_BotmColumn[i]);
						ptResult->hv_Angle.Append(0);
						ptResult->hv_Score.Append(1);
						ptResult->hv_AllScore.Append(1);
					}

				}


			}
		}

	}
	return TRUE;
}

BOOL CFindMark::SaveJigsawImage(const int& nGrabberSN, const HObject& ho_ImageSave)
{
	try
	{
		CString strObjectFilePATH, strFileName;
		SYSTEMTIME CurSysTime;
		GetLocalTime(&CurSysTime);
		strObjectFilePATH.Format("%s\\Map\\匹配模板\\%d\\%d年%02d月%02d日"
			, GetCurrentAppPath(), nGrabberSN, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay);
		CreateMultilevelPath(strObjectFilePATH);
		HTuple  hv_ImageFiles;
		ListFiles(HTuple(strObjectFilePATH), (HTuple("files").Append("follow_links")), &hv_ImageFiles);
		TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$").Append("ignore_case")),
			&hv_ImageFiles);
		strFileName.Format("%s\\Image%04d", strObjectFilePATH, hv_ImageFiles.Length());
		WriteImage(ho_ImageSave, "bmp", 0, HTuple(strFileName));
	}
	catch (HException e)
	{
		return FALSE;
	}
	return TRUE;

}

void CFindMark::ClearMark()
{
	m_tMarkList.clear();
}

void CFindMark::ReLocate(HTuple & hv_Location)
{
	try
	{
		m_dModePosition = hv_Location[0].I();
		m_dIgnoreDist = hv_Location[1].D();

		if (m_tMarkList.size() > 0)
		{


			HTuple hv_Column0;
			HTuple hv_LeftRow0, hv_LeftColumn0, hv_RightRow0, hv_RightColumn0;
			HObject hoRepalceRegion;
			//AreaCenter(m_tMarkList.at(0).ho_ModleInfo.SelectObj(2),NULL,NULL,&hv_Column0);
			SmallestRectangle1(m_tMarkList.at(0).ho_ModleInfo.SelectObj(3), &hv_LeftRow0, &hv_LeftColumn0, &hv_RightRow0, &hv_RightColumn0);
			double dWidth = hv_RightColumn0 - hv_LeftColumn0;
			if (m_tMarkList.at(0).hv_ModelParam[1] == "阈值分割匹配")
			{
				if (m_tInitParam.nCameraSN == 0 || m_tInitParam.nCameraSN == 2)
				{

					GenRectangle1(&hoRepalceRegion, hv_LeftRow0, m_dModePosition + 150 - dWidth, hv_RightRow0, m_dModePosition + 150);

				}
				else
				{
					GenRectangle1(&hoRepalceRegion, hv_LeftRow0, m_dModePosition - 150, hv_RightRow0, m_dModePosition - 150 + dWidth);

				}
				ReplaceObj(m_tMarkList.at(0).ho_ModleInfo, hoRepalceRegion, &m_tMarkList.at(0).ho_ModleInfo, 2);
				ReplaceObj(m_tMarkList.at(0).ho_ModleInfo, hoRepalceRegion, &m_tMarkList.at(0).ho_ModleInfo, 3);
			}


		}
	}
	catch (HException& e)
	{

	}
	WriteMark();
}

int CFindMark::DoSomething(void* message, void* wParam, void* lParam)//函数入口
{
	CString strMessage;
	strMessage.Format("%s", message);
	if ("INITIALIZE" == strMessage)
	{
		return Init(*(NS_SCIFindMark::PINITPARAM)wParam);
	}
	else if ("SHOW" == strMessage)
	{
		return ShowDialog(*(HObject*)wParam);
	}
	else if ("FIND" == strMessage)
	{
		CriticalSectionHelper lock(&m_csFindChange);
		return FindMult(*(HObject*)wParam, (NS_SCIFindMark::PMARKRESULT)lParam);
	}
	else if ("COPYOBJECTS" == strMessage)
	{
		{
			CriticalSectionHelper lock(&m_csFindChange);
			*this = *(CFindMark*)wParam;
		}
		return TRUE;

	}
	else if ("SETPARAM" == strMessage)
	{
		CString strCommand;
		strCommand.Format("%s", wParam);
		/*if (strCommand=="ModelPosition")
		{
			CriticalSectionHelper lock(&m_csFindChange);
			HTuple hv_PositionInfo;
			hv_PositionInfo=*(HTuple*)lParam;
			ReLocate(hv_PositionInfo);
		}*/
		if (strCommand == "LocatePositionY")
		{

			HTuple hvInfo;

			hvInfo = *(HTuple*)lParam;
			m_nLocateUpOrDown = hvInfo[0].I();
			m_dIgnoreDist = hvInfo[1].D();
			WritePrivateProfileInt("Mark", "m_nLocateUpOrDown", m_nLocateUpOrDown, m_strConfigFile);//默认
			WritePrivateProfileDouble("Mark", "m_dIgnoreDist", m_dIgnoreDist, m_strConfigFile);//默认50mm
		}
		return TRUE;
	}
	else if ("COPYPARAM" == strMessage)//拷贝参数
	{
		CString strCommand;
		strCommand.Format("%s", wParam);
		if (strCommand == "MODELINFO")
		{
			CriticalSectionHelper lock(&m_csFindChange);
			*this = *(CFindMark*)lParam;;
			WriteMark();
		}
		return TRUE;
	}
	else if ("GETPARAM" == strMessage)//拷贝参数
	{
		CString strCommand;
		strCommand.Format("%s", wParam);

		if (strCommand == "LocatePositionY")
		{
			*(int*)lParam = m_nLocateUpOrDown;
		}
		else if (strCommand == "JXHeight")
		{
			*(double*)lParam = m_dIgnoreDist;
		}
		else if (strCommand == "JXDev")
		{
			*(double*)lParam = m_dJXEADev;
		}
		return TRUE;
	}
	return 0;
}


void CFindMark::WriteMark()
{
	CriticalSectionHelper lock(&m_csFindChange);
	DeleteFolder(m_strConfigPath);
	CreateMultilevelPath(m_strConfigPath);
	int nMarkCount = (int)m_tMarkList.size();

	for (int i = 0; i < nMarkCount; i++)
	{
		try
		{
			SINGLEMARKINFO& tSingleMark = m_tMarkList[i];
			CString strModleParam, strModel, strModelInfo, strLine;
			strModleParam.Format("%s\\%d.tup", m_strConfigPath, i);
			strModelInfo.Format("%s\\%d.hobj", m_strConfigPath, i);
			strLine.Format("%s\\%d.line", m_strConfigPath, i);
			WriteTuple(tSingleMark.hv_ModelParam, HTuple(strModleParam));
			WriteObject(tSingleMark.ho_ModleInfo, HTuple(strModelInfo));
			WriteObject(tSingleMark.ho_Line, HTuple(strLine));
			if ("形状匹配" == tSingleMark.hv_ModelParam[1])
			{
				strModel.Format("%s\\%d.shm", m_strConfigPath, i);
				WriteShapeModel(tSingleMark.hv_ModelID, HTuple(strModel));
			}
			else
			{
				strModel.Format("%s\\%d.ncm", m_strConfigPath, i);
				WriteNccModel(tSingleMark.hv_ModelID, HTuple(strModel));
			}
		}
		catch (HException e)
		{
			CString strMessageBox;
			strMessageBox.Format("写入WriteMark%d出错:%s", i, e.ErrorMessage().Text());
			AfxMessageBox(strMessageBox);
		}
	}
	WritePrivateProfileInt("Mark", "Count", nMarkCount, m_strConfigFile);
	WritePrivateProfileInt("Mark", "Type", m_nType, m_strConfigFile);
	WritePrivateProfileInt("Mark", "m_nMaxThresh", m_nMaxThresh, m_strConfigFile);
	WritePrivateProfileDouble("Mark", "m_dIgnoreDist", m_dIgnoreDist, m_strConfigFile);//默认50mm
	WritePrivateProfileDouble("Mark", "m_dModePosition", m_dModePosition, m_strConfigFile);//默认
	WritePrivateProfileInt("Mark", "m_nBackLowThresh", m_nBackLowThresh, m_strConfigFile);
	WritePrivateProfileInt("Mark", "m_nBackHighThresh", m_nBackHighThresh, m_strConfigFile);
	WritePrivateProfileDouble("Mark", "m_dFoilSaveHeight", m_dFoilSaveHeight, m_strConfigFile);//默认
	WritePrivateProfileInt("Mark", "m_nJXGrayType", m_nJXGrayType, m_strConfigFile);
	WritePrivateProfileDouble("Mark", "m_dJXEADev", m_dJXEADev, m_strConfigFile);//默认

	WritePrivateProfileDouble("Mark", "m_dLocateOffset", m_dLocateOffset, m_strConfigFile);//默认
	WritePrivateProfileInt("Mark", "m_nLocateUpOrDown", m_nLocateUpOrDown, m_strConfigFile);//默认

}

int CFindMark::FitLine(const HObject& Image, const CLineToolPara& FitLinePara, HObject* Line,
	double* dStartY, double* dStartX, double* dEndY, double* dEndX, int nLineDisplay)
{
	try
	{
#pragma region "先对拟合方式进行判断，如果不是正常拟合直接就返回了"

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
				*dEndX = FitLinePara.m_dEndCol;
				*dEndY = Row1.I();
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (3 == FitLinePara.m_nChoice)//固定底端
			{
				*dStartX = FitLinePara.m_dStartCol;
				*dStartY = Row2.I();
				*dEndX = FitLinePara.m_dEndCol;
				*dEndY = Row2.I();
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (4 == FitLinePara.m_nChoice)//固定左侧
			{
				*dStartX = Column1.I();
				*dStartY = FitLinePara.m_dStartRow;
				*dEndX = Column1.I();
				*dEndY = FitLinePara.m_dEndRow;
				GenRegionLine(Line, *dStartY, *dStartX, *dEndY, *dEndX);
			}
			else if (5 == FitLinePara.m_nChoice)//固定右侧
			{
				*dStartX = Column2.I();
				*dStartY = FitLinePara.m_dStartRow;
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
		HTuple Width, Height;
		GetImageSize(Image, &Width, &Height);
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
				CloseMeasure(MsrHandle_Measure);
			}
			catch (HException& e)
			{
				CString str;
				str.Format("%s", e.ErrorMessage().Text());
			}


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
			return TRUE;
		}
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}


}

void CFindMark::ReadMark()
{
	m_tMarkList.clear();
	int nMarkCount = GetPrivateProfileInt("Mark", "Count", 0, m_strConfigFile);
	for (int i = 0; i < nMarkCount; i++)
	{
		SINGLEMARKINFO tSingleMark;
		CString strModleParam, strModel, strModelInfo, strLine;
		strModleParam.Format("%s\\%d.tup", m_strConfigPath, i);
		strModelInfo.Format("%s\\%d.hobj", m_strConfigPath, i);
		strLine.Format("%s\\%d.line", m_strConfigPath, i);
		ReadTuple(HTuple(strModleParam), &tSingleMark.hv_ModelParam);
		ReadObject(&tSingleMark.ho_ModleInfo, HTuple(strModelInfo));
		ReadObject(&tSingleMark.ho_Line, HTuple(strLine));
		if ("形状匹配" == tSingleMark.hv_ModelParam[1])
		{
			strModel.Format("%s\\%d.shm", m_strConfigPath, i);
			ReadShapeModel(HTuple(strModel), &tSingleMark.hv_ModelID);
		}
		else
		{
			strModel.Format("%s\\%d.ncm", m_strConfigPath, i);
			ReadNccModel(HTuple(strModel), &tSingleMark.hv_ModelID);
		}

		m_tMarkList.push_back(tSingleMark);
	}
	CString strComoonPath;
	strComoonPath.Format("%s/Config/Common.dh", GetCurrentAppPath());//获取配置文件名称
	CString strSection;
	strSection.Format("GRABBER%d", 0);
	m_dRowResolution = GetPrivateProfileDouble(strSection, "ROWRESOLUTION", 0.1, strComoonPath);


}