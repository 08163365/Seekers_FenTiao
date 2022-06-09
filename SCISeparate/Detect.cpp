#include "StdAfx.h"
#include "Detect.h"
#include "DlgParamSet.h"
#include "SCISeparate.h"
#include <iostream>   
#include <fstream>  
using namespace std;
// 通过导出函数形式向调用方提供指向派生类对象的基类指针
extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CSeparate();
}



CBlendent g_Blendent;
CSeparate::CSeparate(void)
{
	m_hvTimeCount.Clear();
	m_hoThreshAT9Region.GenEmptyObj();
	m_hoReferRegion.GenEmptyObj();
	m_hoEdageCrossRegion.GenEmptyObj();
}
CSeparate::~CSeparate(void)
{
}
void CSeparate::SaveParam()
{
	try
	{
		CString strFilePath;
		strFilePath.Format("%s\\Config\\%s\\SCISeparate\\%d",GetCurrentAppPath(),m_tInit.strProductName,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strFilePath);
		for (int i=0;i<LINE_COUNT;i++)
		{
			CString strFileName;
			strFileName.Format("%s\\%d.ini",strFilePath,i);
			m_sLineTool[i].WriteParam(strFileName);
		}
		WritePrivateProfileInt("PARAMETER","PRETREAT",m_tParameters.bPretreat,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","SIZEPRETREAT",m_tParameters.nSizePretreat,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nXSizePretreat",m_tParameters.nXSizePretreat,m_strMyConfig);

		WritePrivateProfileInt("PARAMETER","bUseGRR",m_tParameters.bUseGRR,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nSelectNum",m_tParameters.nSelectNum,m_strMyConfig);
	
		WritePrivateProfileInt("PARAMETER","nExtractFoilThresh",m_tParameters.nExtractFoilThresh,m_strMyConfig);
		
		WritePrivateProfileInt("PARAMETER","NHCALIBFLAG0",m_tParameters.nHcalibFlag ,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nFitRegionMode",m_tParameters.nFitRegionMode ,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nMaxDiffColumn",m_tParameters.nMaxDiffColumn ,m_strMyConfig);

		WritePrivateProfileDouble("PARAMETER","dLocateMissingOffset",m_tParameters.dLocateMissingOffset ,m_strMyConfig);
		WritePrivateProfileDouble("PARAMETER","dMaxAoBanDeviation",m_tParameters.dMaxAoBanDeviation,m_strMyConfig);

		WritePrivateProfileInt("PARAMETER","bEnaleUseSegAoBan",m_tParameters.bEnaleUseSegAoBan,m_strMyConfig);


		WritePrivateProfileInt("PARAMETER","bEnableVariableWidth",m_tParameters.bEnableVariableWidth,m_strMyConfig);
		WritePrivateProfileDouble("PARAMETER","dRelateOffset",m_tParameters.dRelateOffset,m_strMyConfig);

		WritePrivateProfileInt("PARAMETER","bFindManualOrAuto",m_tParameters.bFindManualOrAuto,m_strMyConfig);
		WritePrivateProfileDouble("PARAMETER","dExtractFoilRegionWidth",m_tParameters.dExtractFoilRegionWidth,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nExtractFoilThresh",m_tParameters.nExtractFoilThresh,m_strMyConfig);
		WritePrivateProfileDouble("PARAMETER","dExtractAobanRegionWidth",m_tParameters.dExtractAobanRegionWidth,m_strMyConfig);
		WritePrivateProfileInt("PARAMETER","nExtractAobanThresh",m_tParameters.nExtractAobanThresh,m_strMyConfig);

		WritePrivateProfileDouble("System","dStandardLeftLocation",m_tParameters.dStandardLeftLocation,m_strMyConfig);//左侧基准
		WritePrivateProfileDouble("System","dStandardRightLocation",m_tParameters.dStandardRightLocation,m_strMyConfig);//右侧基准
		WritePrivateProfileInt("PARAMETER","nSelectValueMode",m_tParameters.nSelectValueMode,m_strMyConfig);


		if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)
		{

			CString lpAppName,keyName;
			CString strKeyName;
			lpAppName.Format("GRABBER%d",m_tInit.nGrabberSN);
			m_strCommondh.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
			WritePrivateProfileInt(lpAppName,"POSECOUNT",m_nPoseCount,m_strCommondh);
		
			for (int i=0;i<m_nPoseCount;i++)
			{
				strKeyName.Format("PIXELPOSE%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvColPose[i].D(),m_strCommondh);
			}
			for (int i=0;i<m_nPoseCount;i++)
			{
				strKeyName.Format("XPOSE%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvXPose[i].D(),m_strCommondh);
			}

			for (int i=0;i<m_hvRes.Length();i++)
			{

				strKeyName.Format("PartRes%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvRes[i].D(),m_strCommondh);
			}
			lpAppName.Format("CALIBRATION%d",m_tInit.nGrabberSN);
			WritePrivateProfileDouble(lpAppName,"CALSPACEDIST",m_dCalSpaceDist,m_strCommondh);
			WritePrivateProfileDouble(lpAppName,"CORCOEF",m_dCorCoef,m_strCommondh);

			/////同个相机不同工位///////

			lpAppName.Format("GRABBER%d",m_tInit.nGrabberSN+1);
			WritePrivateProfileInt(lpAppName,"POSECOUNT",m_nPoseCount,m_strCommondh);

			for (int i=0;i<m_nPoseCount;i++)
			{
				strKeyName.Format("PIXELPOSE%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvColPose[i].D(),m_strCommondh);
			}
			for (int i=0;i<m_nPoseCount;i++)
			{
				strKeyName.Format("XPOSE%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvXPose[i].D(),m_strCommondh);
			}

			for (int i=0;i<m_hvRes.Length();i++)
			{

				strKeyName.Format("PartRes%d",i);
				WritePrivateProfileDouble(lpAppName,strKeyName,m_hvRes[i].D(),m_strCommondh);
			}
			lpAppName.Format("CALIBRATION%d",m_tInit.nGrabberSN+1);
			WritePrivateProfileDouble(lpAppName,"CALSPACEDIST",m_dCalSpaceDist,m_strCommondh);
			WritePrivateProfileDouble(lpAppName,"CORCOEF",m_dCorCoef,m_strCommondh);


		}
	
	}catch(HException e)
	{

	}
}
BOOL IsVertical (HObject Region)
{
	HTuple hv_Phi,hv_Deg;
	OrientationRegion(Region,&hv_Phi);
	TupleDeg(hv_Phi,&hv_Deg);
	if (0 != (HTuple(HTuple(hv_Deg>45).TupleAnd(hv_Deg<135)).TupleOr(HTuple(hv_Deg>-135).TupleAnd(hv_Deg<-45))))
	{
		return TRUE;
	}
	return FALSE;
}

void CSeparate::DistancePp_Ex(const HTuple& Row1,const HTuple& Column1,const HTuple& Row2,const HTuple& Column2,
	const double& dRowResolution,const double& dColResolution,HTuple *Distance)
{
	HTuple RowDistance = HTuple(Row2 - Row1).TupleAbs() * dRowResolution;
	HTuple ColDistance = HTuple(Column2 - Column1).TupleAbs() * dColResolution;
	TupleSqrt(RowDistance*RowDistance+ColDistance*ColDistance,Distance);
}
int CSeparate::Init(SEPARATEINIT &tIn)                  //初始化算法
{
	SetHcppInterfaceStringEncodingIsUtf8(FALSE);

	SetSystem("filename_encoding","locale");
	SetSystem("read_halcon_files_encoding_fallback","locale");
	SetSystem("read_halcon_files_encoding_fallback","locale");
	SetSystem("write_halcon_files_encoding","locale");
	SetSystem("tsp_tuple_string_operator_mode","byte");
	SetSystem("tuple_string_operator_mode","byte");
	SetSystem("store_empty_region", "false");
	SetSystem("clip_region", "false");
	m_tInit = tIn;
	ReadParam();//读取检测参数
	//ReadLane();//读取通道参数
	ReadLineMatch();
	return 0 ;
}

void CSeparate::GetAoBanRegion (HObject ho_Image, HObject ho_ROI_0, HObject *ho_AoBanRegion)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_RegionClosing;
	HObject  ho_ConnectedRegions, ho_SelectedRegions;
	// Local control variables
	HTuple  hv_Number, hv_HeightValue;
	if (ho_ROI_0.CountObj()>0)
	{
		Union1(ho_ROI_0, &ho_ROI_0);

	}
	ReduceDomain(ho_Image, ho_ROI_0, &ho_ImageReduced);
	GrayOpeningRect(ho_ImageReduced, &ho_ImageReduced, 51, 5);

	
	/*Threshold(ho_ImageReduced, &ho_Region, 0, m_tParameters.nExtractAobanThresh);*/
	Threshold(ho_ImageReduced, &ho_Region, 0, MAX(20,m_hvMean.D()-m_tParameters.nExtractAobanThresh));


	ClosingRectangle1(ho_Region, &ho_RegionClosing, 5, 5);
	Connection(ho_RegionClosing, &ho_RegionClosing);
	if (ho_RegionClosing.CountObj()>0)
	{HTuple hv_RegionHeight;
	RegionFeatures(ho_RegionClosing,"height",&hv_RegionHeight);
	OpeningRectangle1(ho_RegionClosing,&ho_RegionClosing,1,MAX(15,hv_RegionHeight[0].D()/2));
	}
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area","and",10,999999);
//	SelectShapeStd(ho_ConnectedRegions, &ho_SelectedRegions, "max_area", 70);
	CountObj(ho_SelectedRegions, &hv_Number);
	if (0 != (int(hv_Number>0)))
	{
		ShapeTrans(ho_SelectedRegions, &(*ho_AoBanRegion),"rectangle1");
	}
	else
	{
		GenEmptyObj(&(*ho_AoBanRegion));
	}
	return;
}

void CSeparate::CallException(SEPARATEOUTPUT* ptOut)
{
	//给输出结构体赋值
	HObject hoDomain;
	GetDomain(m_hoGrabImage,&hoDomain);
	GetDomain(m_hoGrabImage,&ptOut->hoCoat);
	GetDomain(m_hoGrabImage,&ptOut->hoFoil);
	GetDomain(m_hoGrabImage,&ptOut->hoErrorRegion);
	TupleGenConst(1,TuoTan,&ptOut->hvErrorType);

	TupleGenConst(m_tLineMatch.nCoatCount,0,&ptOut->hvCoatWidth);
	TupleGenConst(m_tLineMatch.nCoatCount,0,&ptOut->hvCoatHeight);
	TupleGenConst(m_tLineMatch.nCoatCount,0,&ptOut->hvFoilWidth);
	TupleGenConst(m_tLineMatch.nCoatCount,0,&ptOut->hvFoilHeight);
	TupleGenConst(2,0,&m_ptOut->hv_BaiBian);//每一片要有两个白边
	TupleGenConst(2,0,&m_ptOut->hvLocateMiss);//两个敷料高度
	HTuple Row,Column;
	AreaCenter(m_hoGrabImage,NULL,&Row,&Column);
	GenCrossContourXld(&ptOut->hoDispRegion,Row,Column,1000,0.785398);

}

void  CSeparate::ReLocateLine(HTuple hv_Location)//重新设置线的位置
{
	int nLeftPosition=hv_Location[0].I();
	int nRightPosition=hv_Location[1].I();

	m_tParameters.dStandardLeftLocation=nLeftPosition;
	m_tParameters.dStandardRightLocation=nRightPosition;
	int nColumnCenter,nLeftOffset,nRightOffset;
	{
		if (positive==m_tInit.nProductType)
		{
			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)//正极左侧
			{

				nLeftOffset=  nLeftPosition-(m_sLineTool[3].m_dStartCol+m_sLineTool[3].m_dEndCol)/2;
			    nRightOffset= nRightPosition-(m_sLineTool[6].m_dStartCol+m_sLineTool[6].m_dEndCol)/2;
				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
				{
					if (i==5 || i==6)
					{
						m_sLineTool[i].m_dStartCol=nRightPosition;
						m_sLineTool[i].m_dEndCol=nRightPosition;
					}else if (i==3 || i==4)
					{
						m_sLineTool[i].m_dStartCol=nLeftPosition;
						m_sLineTool[i].m_dEndCol=nLeftPosition;

					}else 
					{
						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
					}
				}

			}
			else if (m_tInit.nGrabberSN==1 || m_tInit.nGrabberSN==3)//正极右侧
			{

				nLeftOffset=  nLeftPosition-(m_sLineTool[6].m_dStartCol+m_sLineTool[6].m_dEndCol)/2;
				nRightOffset=nRightPosition-(m_sLineTool[3].m_dStartCol+m_sLineTool[3].m_dEndCol)/2;

				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
				{

					if (i==5 || i==6 )
					{
						m_sLineTool[i].m_dStartCol=nLeftPosition;
						m_sLineTool[i].m_dEndCol=nLeftPosition;


					}else if (i==3 || i==4 )
					{
						m_sLineTool[i].m_dStartCol=nRightPosition;
						m_sLineTool[i].m_dEndCol  =nRightPosition;


					}else
					{
						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;

					}
				}
	

			}
		}else
		{

			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)//负极左侧
			{
				nLeftOffset=  nLeftPosition-(m_sLineTool[4].m_dStartCol+m_sLineTool[4].m_dEndCol)/2;
				nRightOffset=nRightPosition-(m_sLineTool[5].m_dStartCol+m_sLineTool[5].m_dEndCol)/2;
				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
				{

					if (i==5)
					{
						m_sLineTool[i].m_dStartCol=nRightPosition;
						m_sLineTool[i].m_dEndCol=nRightPosition;

					}else if (i==4)
					{
						m_sLineTool[i].m_dStartCol=nLeftPosition;
						m_sLineTool[i].m_dEndCol=nLeftPosition;
					}
					else
					{
						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
					}
				}


			}else//负极右侧
			{
				nLeftOffset=  nLeftPosition-(m_sLineTool[5].m_dStartCol+m_sLineTool[5].m_dEndCol)/2;
				nRightOffset=nRightPosition-(m_sLineTool[4].m_dStartCol+m_sLineTool[4].m_dEndCol)/2;

				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
				{

					if (i==5)
					{
						m_sLineTool[i].m_dStartCol=nLeftPosition;
						m_sLineTool[i].m_dEndCol=nLeftPosition;
						m_tParameters.dStandardLeftLocation=(m_sLineTool[i].m_dStartCol+m_sLineTool[i].m_dEndCol)/2;
					}else if (i==4)
					{
						m_sLineTool[i].m_dStartCol=nRightPosition;
						m_sLineTool[i].m_dEndCol=nRightPosition;
					}
					else
					{
						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;
					}
				}
			
			}
		}
		SaveParam();
	}
}


//void  CSeparate::ReLocateLine(HTuple hv_Location)//重新设置线的位置
//{
//	int nLeftPosition=hv_Location[0].I();
//	int nRightPosition=hv_Location[1].I();
//
//	m_tParameters.dStandardLeftLocation=nLeftPosition;
//	m_tParameters.dStandardRightLocation=nRightPosition;
//	int nColumnCenter,nLeftOffset,nRightOffset;
//	{
//
//		if (positive==m_tInit.nProductType)
//		{
//			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)//正极左侧
//			{
//				nLeftOffset=  nLeftPosition-(m_sLineTool[3].m_dStartCol+m_sLineTool[3].m_dEndCol)/2;
//				nRightOffset=nRightPosition-(m_sLineTool[6].m_dStartCol+m_sLineTool[6].m_dEndCol)/2;
//
//				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
//				{
//
//					if (i==5 || i==6 )
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;
//					}else
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
//
//					}
//
//
//
//				}
//
//
//
//				if (m_tParameters.hoModelRegion.CountObj()>0)
//				{
//
//					HTuple hv_Column0;
//					AreaCenter(m_tParameters.hoModelRegion,NULL,NULL,&hv_Column0);
//					MoveRegion(m_tParameters.hoModelRegion,&m_tParameters.hoModelRegion,0,nLeftPosition-hv_Column0);
//
//				}
//			}
//			else if (m_tInit.nGrabberSN==1 || m_tInit.nGrabberSN==3)//正极右侧
//			{
//
//				nLeftOffset=  nLeftPosition-(m_sLineTool[6].m_dStartCol+m_sLineTool[6].m_dEndCol)/2;
//				nRightOffset=nRightPosition-(m_sLineTool[3].m_dStartCol+m_sLineTool[3].m_dEndCol)/2;
//
//				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
//				{
//
//					if (i==5 || i==6 )
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
//
//
//					}else
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;
//
//					}
//
//
//
//				}
//				if (m_tParameters.hoModelRegion.CountObj()>0)//如果存在移动模板
//				{
//					HTuple hv_Column0;
//					AreaCenter(m_tParameters.hoModelRegion,NULL,NULL,&hv_Column0);
//					MoveRegion(m_tParameters.hoModelRegion,&m_tParameters.hoModelRegion,0,nRightPosition-hv_Column0);
//				}
//
//			}
//		}else
//		{
//
//			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)//负极左侧
//			{
//				nLeftOffset=  nLeftPosition-(m_sLineTool[4].m_dStartCol+m_sLineTool[4].m_dEndCol)/2;
//				nRightOffset=nRightPosition-(m_sLineTool[5].m_dStartCol+m_sLineTool[5].m_dEndCol)/2;
//				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
//				{
//
//					if (i==5)
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;
//
//					}else
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
//					}
//
//
//
//				}
//				if (m_tParameters.hoModelRegion.CountObj()>0)//如果存在移动模板
//				{
//					HTuple hv_Column0;
//					AreaCenter(m_tParameters.hoModelRegion,NULL,NULL,&hv_Column0);
//					MoveRegion(m_tParameters.hoModelRegion,&m_tParameters.hoModelRegion,0,nLeftPosition-hv_Column0);
//				}
//
//
//			}else//负极右侧
//			{
//				nLeftOffset=  nLeftPosition-(m_sLineTool[5].m_dStartCol+m_sLineTool[5].m_dEndCol)/2;
//				nRightOffset=nRightPosition-(m_sLineTool[4].m_dStartCol+m_sLineTool[4].m_dEndCol)/2;
//
//				for (int i=0;i<m_tLineMatch.nLineUsed;i++)
//				{
//
//					if (i==5)
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nLeftOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nLeftOffset;
//						m_tParameters.dStandardLeftLocation=(m_sLineTool[i].m_dStartCol+m_sLineTool[i].m_dEndCol)/2;
//					}else
//					{
//						m_sLineTool[i].m_dStartCol=m_sLineTool[i].m_dStartCol+nRightOffset;
//						m_sLineTool[i].m_dEndCol=m_sLineTool[i].m_dEndCol+nRightOffset;
//					}
//				}
//				if (m_tParameters.hoModelRegion.CountObj()>0)//如果存在移动模板
//				{
//					HTuple hv_Column0;
//					AreaCenter(m_tParameters.hoModelRegion,NULL,NULL,&hv_Column0);
//					MoveRegion(m_tParameters.hoModelRegion,&m_tParameters.hoModelRegion,0,nRightPosition-hv_Column0);
//				}
//			}
//		}
//		SaveParam();
//	}
//}
//



int CSeparate::DoSomething(void* message,void* wParam,void* lParam)
{
	CString strMessage;
	strMessage.Format("%s",(char*)message);
	if ("INITIALIZE" == strMessage)//初始化
	{
		return Init(*(PSEPARATEINIT)wParam);
	}else if ("ACTION" == strMessage)
	{
		
		int nReturn = Detect(*(PSEPARATEINPUT)wParam,(PSEPARATEOUTPUT)lParam);
		return nReturn;
	}else if ("SHOW" == strMessage)
	{
		return DisplaySetDialog(*(PSEPARATEINPUT)wParam);
	}else if("SETPARAM" == strMessage)
	{
		CString strWparam;
		strWparam.Format("%s",wParam);
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if("LINEMATCH" == strWparam)
		{
			return TRUE;
		}else if("ProductLocation" == strWparam)///产品位置
		{
			HTuple  hv_Location=*(HTuple*)lParam;
			ReLocateLine(hv_Location);

		}else if ("GRRDataOff"==strWparam)
		{
			HTuple hv_InfoTuple;
			hv_InfoTuple=*(HTuple*)lParam;
			if (hv_InfoTuple.Length()==2)
			{
				m_tParameters.bUseGRR=hv_InfoTuple[0].I();
				m_tParameters.nSelectNum=hv_InfoTuple[1].I();
			}
		}
		else if ("tSizeParam" == strWparam)
		{
		
			m_tStandardSize = *(STANDARDSIZE*)lParam;
			return TRUE;
		}
	   else
		{
			AfxMessageBox("wParam有误，在Separate模块");
			return FALSE;
		}
		return TRUE;
	}
	else if("GETPARAM" == strMessage)
	{
		CString strWparam;
		strWparam.Format("%s",wParam);
		if ("LANECOUNT" == strWparam)
		{
			*(INT*)lParam = m_tLane.nLeftCount+m_tLane.nRightCount;
		}else  if ("RightPosition" == strWparam)
		{
			*(double*)lParam =m_tParameters.dStandardRightLocation;
		}else if ("LeftPosition" == strWparam)
		{
			*(double*)lParam =m_tParameters.dStandardLeftLocation;
		}

	}
	else if("COPYPARAM" == strMessage)
	{
		CString strWparam;
		strWparam.Format("%s",wParam);
		if("LANE" == strWparam)
		{
			m_tLane = ((CSeparate*)lParam)->m_tLane;
			//SaveLane();
		}else if("LINEMATCH" == strWparam)
		{
			m_tLineMatch = ((CSeparate*)lParam)->m_tLineMatch;
			SaveLineMatch();
		}else
		{
			AfxMessageBox("wParam有误，在Detect模块");
			return FALSE;
		}
		return TRUE;
	}
	else if ("GETPARAM" == strMessage)
	{
		return TRUE;
	}else if ("COPYOBJECTS" == strMessage)//拷贝一个副本
	{
		*this = *(CSeparate*)wParam;
		return TRUE;
	}else if ("COPYFILES" == strMessage)//拷贝一个副本
	{
		CString strSrc,strDst;
		strSrc.Format("%s\\Config\\%s\\SCISeparate\\%d",GetCurrentAppPath(),wParam,
			m_tInit.nGrabberSN);
		strDst.Format("%s\\Config\\%s\\SCISeparate\\%d",GetCurrentAppPath(),lParam,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strDst);
		CopyFolder(strSrc,strDst);
		return TRUE;
	}else if ("DELETEFILES" == strMessage)//拷贝一个副本
	{
		CString strDst;
		strDst.Format("%s\\Config\\%s\\SCISeparate\\%d",GetCurrentAppPath(),wParam,
			m_tInit.nGrabberSN);
		DeleteFolder(strDst);
		HTuple hvFiles;
		strDst.Format("%s\\Config\\%s\\SCISeparate",GetCurrentAppPath(),wParam);

		try
		{
			ListFiles(HTuple(strDst),"directories",&hvFiles);
			if (0 == hvFiles.Length())//如果里面没文件了，把上一级目录删除掉
			{
				DeleteFolder(strDst);
			}
		}catch(HException e)
		{

		}




		return TRUE;
	}else
	{
		CString 
			AfxMessageBox("CSeparate Message 异常");
	}
	return FALSE;
}

void CSeparate::ReadLineMatch()//读取线配对
{
	m_tLineMatch.nLineUsed = GetPrivateProfileInt("LineMatch","LineUsed",7,m_strMyConfig);

	if (m_tInit.nProductType==positive)
	{
		m_tLineMatch.nLineUsed=7;//正极检测AT9所以需要7条线
	}else
	{

		
		m_tLineMatch.nLineUsed=6;//负极无AT9需要6条线 //需要做变宽
		
		
	}


	m_tLineMatch.nCoatCount = GetPrivateProfileInt("LineMatch","CoatCount",1,m_strMyConfig);
	m_tLineMatch.nFoilCount = GetPrivateProfileInt("LineMatch","FoilCount",2,m_strMyConfig);
	m_tLineMatch.vCoat1.clear();
	m_tLineMatch.vCoat2.clear();
	m_tLineMatch.vFoil1.clear();
	m_tLineMatch.vFoil2.clear();

	for(int i=0;i<MAX_LINEMATCH;i++)
	{
		CString lpAppName;
		lpAppName.Format("Coat%d",i);
		m_tLineMatch.vCoat1.push_back(GetPrivateProfileInt(lpAppName,"Index1",1,m_strMyConfig));
		m_tLineMatch.vCoat2.push_back(GetPrivateProfileInt(lpAppName,"Index2",1,m_strMyConfig));
		m_tLineMatch.vCoatWidthMin.push_back(GetPrivateProfileDouble(lpAppName,"WidthMin",1,m_strMyConfig));
		m_tLineMatch.vCoatWidthMax.push_back(GetPrivateProfileDouble(lpAppName,"WidthMax",1,m_strMyConfig));
		m_tLineMatch.vCoatWidthOffset.push_back(GetPrivateProfileDouble(lpAppName,"WidthMaxOffset",0,m_strMyConfig));

		m_tLineMatch.vCoatHeightMin.push_back(GetPrivateProfileDouble(lpAppName,"HeightMin",1,m_strMyConfig));
		m_tLineMatch.vCoatHeightMax.push_back(GetPrivateProfileDouble(lpAppName,"HeightMax",1,m_strMyConfig));
		m_tLineMatch.vCoatHeightOffset.push_back(GetPrivateProfileDouble(lpAppName,"HeightMaxOffset",0,m_strMyConfig));
	}
	for(int i=0;i<MAX_LINEMATCH;i++)
	{
		CString lpAppName;
		lpAppName.Format("Foil%d",i);
		m_tLineMatch.vFoil1.push_back(GetPrivateProfileInt(lpAppName,"Index1",2,m_strMyConfig));
		m_tLineMatch.vFoil2.push_back(GetPrivateProfileInt(lpAppName,"Index2",2,m_strMyConfig));

		m_tLineMatch.vFoilWidthMin.push_back(GetPrivateProfileDouble(lpAppName,"WidthMin",1,m_strMyConfig));
		m_tLineMatch.vFoilWidthMax.push_back(GetPrivateProfileDouble(lpAppName,"WidthMax",1,m_strMyConfig));
		m_tLineMatch.vFoilWidthOffset.push_back(GetPrivateProfileDouble(lpAppName,"WidthMaxOffset",0,m_strMyConfig));

		m_tLineMatch.vFoilHeightMin.push_back(GetPrivateProfileDouble(lpAppName,"HeightMin",1,m_strMyConfig));
		m_tLineMatch.vFoilHeightMax.push_back(GetPrivateProfileDouble(lpAppName,"HeightMax",1,m_strMyConfig));
		m_tLineMatch.vFoilHeightOffset.push_back(GetPrivateProfileDouble(lpAppName,"HeightMaxOffset",0,m_strMyConfig));
	}
}
void CSeparate::SaveLineMatch()//保存线配对
{
	WritePrivateProfileInt("LineMatch","LineUsed",m_tLineMatch.nLineUsed,m_strMyConfig);
	WritePrivateProfileInt("LineMatch","CoatCount",m_tLineMatch.nCoatCount,m_strMyConfig);
	WritePrivateProfileInt("LineMatch","FoilCount",m_tLineMatch.nFoilCount,m_strMyConfig);
	for(int i=0;i<MAX_LINEMATCH;i++)
	{
		CString lpAppName;
		lpAppName.Format("Coat%d",i);
		WritePrivateProfileInt(lpAppName,"Index1",m_tLineMatch.vCoat1.at(i),m_strMyConfig);
		WritePrivateProfileInt(lpAppName,"Index2",m_tLineMatch.vCoat2.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMin",m_tLineMatch.vCoatWidthMin.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMax",m_tLineMatch.vCoatWidthMax.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMaxOffset",m_tLineMatch.vCoatWidthOffset.at(i),m_strMyConfig);

		WritePrivateProfileDouble(lpAppName,"HeightMin",m_tLineMatch.vCoatHeightMin.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"HeightMax",m_tLineMatch.vCoatHeightMax.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"HeightMaxOffset",m_tLineMatch.vCoatHeightOffset.at(i),m_strMyConfig);
	}
	for(int i=0;i<MAX_LINEMATCH;i++)
	{
		CString lpAppName;
		lpAppName.Format("Foil%d",i);
		WritePrivateProfileInt(lpAppName,"Index1",m_tLineMatch.vFoil1.at(i),m_strMyConfig);
		WritePrivateProfileInt(lpAppName,"Index2",m_tLineMatch.vFoil2.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMin",m_tLineMatch.vFoilWidthMin.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMax",m_tLineMatch.vFoilWidthMax.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"WidthMaxOffset",m_tLineMatch.vFoilWidthOffset.at(i),m_strMyConfig);

		WritePrivateProfileDouble(lpAppName,"HeightMin",m_tLineMatch.vFoilHeightMin.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"HeightMax",m_tLineMatch.vFoilHeightMax.at(i),m_strMyConfig);
		WritePrivateProfileDouble(lpAppName,"HeightMaxOffset",m_tLineMatch.vFoilHeightOffset.at(i),m_strMyConfig);
	}
}

BOOL CSeparate::DeterMinationSize(int nIndex,int nCoatOrFoil,double dWidth,double dHeight)//尺寸判定
{
	//int i = nIndex;
	//if (0 == nCoatOrFoil)
	//{
	//	if (dWidth > m_tLineMatch.vCoatWidthMax[i])
	//	{
	//		m_ptOut->hvErrorType.Append(C_WSIZE);
	//		return FALSE;
	//	}
	//	if (dWidth < m_tLineMatch.vCoatWidthMin[i])
	//	{
	//		m_ptOut->hvErrorType.Append(C_WSIZE);
	//		return FALSE;
	//	}
	//	if (dHeight > m_tLineMatch.vCoatHeightMax[i])
	//	{
	//		m_ptOut->hvErrorType.Append(C_HSIZE);
	//		return FALSE;
	//	}
	//	if (dHeight < m_tLineMatch.vCoatHeightMin[i])
	//	{
	//		m_ptOut->hvErrorType.Append(C_HSIZE);
	//		return FALSE;
	//	}
	//}
	//else
	//{
	//	if (dWidth > m_tLineMatch.vFoilWidthMax[i])
	//	{
	//		m_ptOut->hvErrorType.Append(F_WSIZE);
	//		return FALSE;
	//	}
	//	if (dWidth < m_tLineMatch.vFoilWidthMin[i])
	//	{
	//		m_ptOut->hvErrorType.Append(F_WSIZE);
	//		return FALSE;
	//	}
	//	if (dHeight > m_tLineMatch.vFoilHeightMax[i])
	//	{
	//		m_ptOut->hvErrorType.Append(F_HSIZE);
	//		return FALSE;
	//	}
	//	if (dHeight < m_tLineMatch.vFoilHeightMin[i])
	//	{
	//		m_ptOut->hvErrorType.Append(F_HSIZE);
	//		return FALSE;
	//	}
	//}
	return TRUE;	
}


double CSeparate::GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2)
{
	//需要先转化坐标，先把点的坐标转换成物理坐标

	try
	{
		if (m_tParameters.nHcalibFlag==2)//纵向补偿的条纹标定
		{
			HTuple hvPix, Sorted, Indices, Max, Selected;
			hvPix.Append(m_hvColPose);
			hvPix.Append(dptCol);
			TupleSort(hvPix, &Sorted);
			TupleFind(Sorted, dptCol, &Indices);
			TupleMax(Indices, &Max);
			TupleSelect(m_hvXPose, Max - 1, &Selected);
			double dV1 = dptCol - Sorted[Max - 1];

			double dpCol = Selected + dV1 * m_hvRes[Max.I() - 1];//这个位置就是左边点的物理坐标
			double dpRow = dptRow * m_dRowResolution;//Y坐标

			//求解线的物理坐标
			hvPix.Clear();
			hvPix.Append(m_hvColPose);
			hvPix.Append(hvLCol1);
			TupleSort(hvPix, &Sorted);
			TupleFind(Sorted, hvLCol1, &Indices);
			TupleMax(Indices, &Max);
			TupleSelect(m_hvXPose, Max - 1, &Selected);
			double dV2 = hvLCol1 - Sorted[Max - 1];
			double dLCol1 = Selected + dV2 * m_hvRes[Max.I() - 1];//线的起点物理坐标x1
			double dLRow1 = hvLRow1 *  m_dRowResolution;//线的起点物理坐标y1

			hvPix.Clear();
			hvPix.Append(m_hvColPose);
			hvPix.Append(hvLCol2);
			TupleSort(hvPix, &Sorted);
			TupleFind(Sorted, hvLCol2, &Indices);
			TupleMax(Indices, &Max);
			TupleSelect(m_hvXPose, Max - 1, &Selected);
			double dV3 = hvLCol2 - Sorted[Max - 1];
			double dLCol2 = Selected + dV3 * m_hvRes[Max.I() - 1];//线的起点物理坐标x2
			double dLRow2 = hvLRow2*  m_dRowResolution;//线的起点物理坐标y2

			HTuple hvDistance;
			DistancePl(dpRow, dpCol, dLRow1, dLCol1, dLRow2, dLCol2, &hvDistance);
			return hvDistance.D() * m_dCorCoef;
		}
		else
		{
			double dC1 = dptCol;
			double dC2 = (hvLCol1 + hvLCol2 ) /2;
			HTuple hvGreater, hvCol1, hvCol2,hv_Indics1,hv_Indics2, hv_DistanceWorld;
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





void CSeparate::GetSize(int nSourceLineIndex,int nDetLineIndex,double dResolution,HTuple* hv_Value)
{
	HTuple hv_ErrorLineIndex0,hv_ErrorLineIndex1;
	TupleFind(m_hvErrorLineIndex,nSourceLineIndex,&hv_ErrorLineIndex0);
	TupleFind(m_hvErrorLineIndex,nDetLineIndex,&hv_ErrorLineIndex1);
	if (hv_ErrorLineIndex0.Length()>0 && hv_ErrorLineIndex0!=-1||hv_ErrorLineIndex1.Length()>0 && hv_ErrorLineIndex1!=-1)
	{
		*hv_Value=9999;
		
	}else
	{
		if (m_tParameters.nHcalibFlag!=0)///采用标定板标定
		{
			 
			/**hv_Value=GetDistancepl((m_hvRowBegin[nSourceLineIndex].D()+m_hvRowEnd[nSourceLineIndex]).D() /2,(m_hvColEnd[nSourceLineIndex].D()+m_hvColBegin[nSourceLineIndex].D()) /2,m_hvRowBegin[nDetLineIndex].D(),m_hvColBegin[nDetLineIndex].D(),m_hvRowEnd[nDetLineIndex].D(),m_hvColEnd[nDetLineIndex].D());*/
			*hv_Value=GetDistancepl((m_hvFitLineSR[nSourceLineIndex].D()+m_hvFitLineER[nSourceLineIndex]).D() /2,(m_hvFitLineSC[nSourceLineIndex].D()+m_hvFitLineEC[nSourceLineIndex].D()) /2,m_hvFitLineSR[nDetLineIndex].D(),m_hvFitLineSC[nDetLineIndex].D(),m_hvFitLineER[nDetLineIndex].D(),m_hvFitLineEC[nDetLineIndex].D());

		}else
		{
			HTuple hv_Distance1,hv_Distance2;

			DistancePl(m_hvFitLineSR[nSourceLineIndex], m_hvFitLineSC[nSourceLineIndex], m_hvFitLineSR[nDetLineIndex], m_hvFitLineSC[nDetLineIndex], m_hvFitLineER[nDetLineIndex], m_hvFitLineEC[nDetLineIndex], &hv_Distance1);
			DistancePl(m_hvFitLineER[nSourceLineIndex], m_hvFitLineEC[nSourceLineIndex], m_hvFitLineSR[nDetLineIndex], m_hvFitLineSC[nDetLineIndex], m_hvFitLineER[nDetLineIndex], m_hvFitLineEC[nDetLineIndex], &hv_Distance2);
	/*		DistancePl(m_hvRowBegin[nSourceLineIndex],m_hvColBegin[nSourceLineIndex],m_hvRowBegin[nDetLineIndex],m_hvColBegin[nDetLineIndex],m_hvRowEnd[nDetLineIndex],m_hvColEnd[nDetLineIndex],&hv_Distance1);
			DistancePl(m_hvRowEnd[nSourceLineIndex],m_hvColEnd[nSourceLineIndex],m_hvRowBegin[nDetLineIndex],m_hvColBegin[nDetLineIndex],m_hvRowEnd[nDetLineIndex],m_hvColEnd[nDetLineIndex],&hv_Distance2);*/
			*hv_Value=(hv_Distance1+hv_Distance2)/2*dResolution;
		}
	}

	

}

void CSeparate::WriteGRRData()
{

	/////20分钟自动关闭GRR嵌套存储数据的功能
	if (m_tParameters.bUseGRR)
	{
		if (m_hvTimeCount.Length()==0)
		{
			CountSeconds(&m_hvTimeCount);
		}else
		{
			HTuple hv_CurrentTime;
			CountSeconds(&hv_CurrentTime);
			if ((hv_CurrentTime-m_hvTimeCount)>1200)
			{
				m_tParameters.bUseGRR=FALSE;
				m_hvTimeCount.Clear();
			}
		}

	}

	/////////////////////////////
	if (m_tParameters.bUseGRR)//测量
	{
		HTuple hv_Distance1,hv_Distance2;
		double dValue;
		CString strFilePath,strFile;
		CString strIndex,strItem;
		int nIndex1,nIndex2;
		if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)
		{
			if (m_tInit.nProductType==positive)
			{
				nIndex1=3;
				nIndex2=6;
			}
			else
			{
				nIndex1=4;
				nIndex2=5;
			}
			if (m_tInit.nGrabberSN==0)
			{
				strIndex="正面相机嵌套总宽";
			}else if (m_tInit.nGrabberSN==2)
			{
				strIndex="反面相机嵌套总宽";
			}
			strFilePath.Format("%s\\%s.csv",GetCurrentAppPath(),strIndex);
			ofstream outFile;
			outFile.open(strFilePath,  ios::out |ios::app);
			double dLast=0;
			CString strOutInfo="";
			for (int i=1;i<1+m_tParameters.nSelectNum;i++)
			{


				HTuple hv_TempRow,hv_TempColumn;
				hv_TempRow=m_hvRowBegin[nIndex1]+(m_hvRowEnd[nIndex1]-m_hvRowBegin[nIndex1])/m_tParameters.nSelectNum*i;
				hv_TempColumn=m_hvColBegin[nIndex1]+(m_hvColEnd[nIndex1]-m_hvColBegin[nIndex1])/m_tParameters.nSelectNum*i;
				DistancePl(hv_TempRow,hv_TempColumn,m_hvRowBegin[nIndex2],m_hvColBegin[nIndex2],m_hvRowEnd[nIndex2],m_hvColEnd[nIndex2],&hv_Distance1);
			
				dValue=hv_Distance1*m_dColResolution;
				if (i==1)
				{
					dLast=dValue;
				}else
				{
					if (m_tInit.nProductType==positive)
					{

						dValue=dLast+(dValue-dLast)/2;
					}else
					{
						dValue=dLast+(dValue-dLast)/8;
					}
				
				}
				strItem.Format("%.5f,",dValue);
				strOutInfo+=strItem;
				
			}
			outFile << strOutInfo <<endl;
			outFile.close();
		}
		///////////////存储AT9嵌套数据///////////////////	
		CString strAT9OutInfo="";
		{
			if (m_tInit.nGrabberSN==0)
			{
				strIndex="正面相机左侧嵌套AT9或者切入深度宽";
			}else if (m_tInit.nGrabberSN==1)
			{
				strIndex="正面相机右侧嵌套AT9或者切入深度宽";
			}else if (m_tInit.nGrabberSN==2)
			{
				strIndex="反面相机左侧嵌套AT9或者切入深度宽";
			}else if (m_tInit.nGrabberSN==3)
			{
				strIndex="反面相机右侧嵌套AT9或者切入深度宽";
			}

			nIndex1=3;
			nIndex2=4;
			strFilePath.Format("%s\\%s.csv",GetCurrentAppPath(),strIndex);
			ofstream outFile;
			outFile.open(strFilePath,  ios::out |ios::app);
			double dLastAT9=0;
			for (int i=1;i<1+m_tParameters.nSelectNum;i++)
			{
				HTuple hv_TempRow,hv_TempColumn;
				hv_TempRow=m_hvRowBegin[nIndex1]+(m_hvRowEnd[nIndex1]-m_hvRowBegin[nIndex1])/m_tParameters.nSelectNum*i;
				hv_TempColumn=m_hvColBegin[nIndex1]+(m_hvColEnd[nIndex1]-m_hvColBegin[nIndex1])/m_tParameters.nSelectNum*i;
				DistancePl(hv_TempRow,hv_TempColumn,m_hvRowBegin[nIndex2],m_hvColBegin[nIndex2],m_hvRowEnd[nIndex2],m_hvColEnd[nIndex2],&hv_Distance1);
				dValue=hv_Distance1*m_dColResolution;
				if (i==1)
				{
					dLastAT9=dValue;
				}else
				{
					if (m_tInit.nProductType==positive)
					{
						dValue=dLastAT9+(dValue-dLastAT9)/2;
					}else
					{
						dValue=dLastAT9+(dValue-dLastAT9)/5;
					}
					
				}
				strItem.Format("%.5f,",dValue);
				strAT9OutInfo+=strItem;
				
			}
			outFile << strAT9OutInfo <<endl;
			outFile.close();
		}

	}






}


BOOL CSeparate::FindLineAgain(const HObject& Image,const CLineToolPara& TempFitLinePara,double* dStartY,double* dStartX,double* dEndY,double* dEndX, HObject* Line,int nLineIndex)
{

	HObject  Rectangle, Contour;
	// Local control variables 
	HTuple  Phi, ResultRow, ResultCol, i, RowC, ColC;
	HTuple  MsrHandle_Measure, transition;
	HTuple  select, RowEdge, ColEdge, Amplitude, Distance, tRow;
	HTuple  tCol, t, RowBegin, ColBegin, RowEnd, ColEnd, Nr;
	HTuple  Nc, Dist;
	HObject hMeasureRect,hCross;
	HTuple Width, Height;
	GetImageSize(Image,&Width,&Height);
	if (nLineIndex==3)
	{
		Phi = ((HTuple(TempFitLinePara.m_dStartRow) - HTuple(TempFitLinePara.m_dEndRow)).TupleAtan2(HTuple(TempFitLinePara.m_dEndCol) - HTuple(TempFitLinePara.m_dStartCol)))\
			+ (HTuple(90).TupleRad());

		for (int i=0; i<HTuple(TempFitLinePara.m_nMeasureNum); i+=1)
		{
			RowC = HTuple(TempFitLinePara.m_dStartRow)+(((HTuple(TempFitLinePara.m_dEndRow)-HTuple(TempFitLinePara.m_dStartRow))*i)/(HTuple(TempFitLinePara.m_nMeasureNum)-1));
			ColC = HTuple(TempFitLinePara.m_dStartCol)+(((HTuple(TempFitLinePara.m_dEndCol)-HTuple(TempFitLinePara.m_dStartCol))*i)/(HTuple(TempFitLinePara.m_nMeasureNum)-1));
			//Convert coordinates to rectangle2 type
			GenRectangle2(&hMeasureRect,RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), HTuple(TempFitLinePara.m_dWidth));
			GenRectangle2(&Rectangle,RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), 1);
			ConcatObj(*Line,Rectangle,Line);
			try
			{
			 	transition = "positive";
				if (0 != (HTuple(TempFitLinePara.m_nSelect) == 0))
				{
					select = "all";
				}
				else if (0 != (HTuple(TempFitLinePara.m_nSelect) == 1))
				{
					select = "first";
				}
				else
				{
					select = "last";
				}
				GenMeasureRectangle2(RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), HTuple(TempFitLinePara.m_dWidth),
					Width, Height, "nearest_neighbor", &MsrHandle_Measure);
				MeasurePos(Image, MsrHandle_Measure, HTuple(TempFitLinePara.m_dSigma), HTuple(MIN(255,TempFitLinePara.m_nThr+30)), 
					transition, select, &RowEdge, &ColEdge, &Amplitude, &Distance);
			}
			catch(HException& e)
			{

			}
			CloseMeasure(MsrHandle_Measure);

			tRow = 0;
			tCol = 0;
			t = 0;
			for (int j=0; j<=(RowEdge.Length())-1; j+=1)
			{
				if (0 != (t<HTuple(Amplitude[j]).TupleAbs()))
				{
					tRow = RowEdge[j];
					tCol = ColEdge[j];
					t = HTuple(Amplitude[j]).TupleAbs();
				}
			}
			if (0 != (t>0))
			{
				ResultRow.Append(tRow);
				ResultCol.Append(tCol);
				GenCrossContourXld(&hCross,tRow[0].D(),tCol[0].D(),18,0.78);
				ConcatObj(*Line,hCross,Line);
			}
		}
		//stop ()
		HObject hLine;
		if (ResultRow.Length()>TempFitLinePara.m_nMeasureNum/3)
		{
			SortPoints(ResultRow, ResultCol, TempFitLinePara.m_nSortType);
			GenContourPolygonXld(&Contour, ResultRow, ResultCol);//tukey
			FitLineContourXld(Contour, "tukey", -1, 0, 5, 2, &RowBegin, 
				&ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
			GenContourPolygonXld(&hLine, RowBegin.TupleConcat(RowEnd), ColBegin.TupleConcat(ColEnd));
			ConcatObj(*Line,hLine,Line);
			*dStartY = RowBegin[0].D();
			*dStartX = ColBegin[0].D();
			*dEndY = RowEnd[0].D();
			*dEndX = ColEnd[0].D();
			return TRUE;
		}
	}

	return FALSE;


}




void CSeparate::GetLastFoil (HObject ho_Image, HObject ho_CoatRegion, HObject *ho_LastFoil)
{


	// Local iconic variables
	HObject  ho_FoilBottomROI, ho_FoilBottomImageReduced;
	HObject  ho_FoilRegion, ho_RegionClosing, ho_ConnectedRegions;

	// Local control variables
	HTuple  hv_nCameraSN, hv_offsetColumn, hv_dFoilWidth;
	HTuple  hv_dFoilHeight, hv_nFoilBottomThresh, hv_Width;
	HTuple  hv_Height, hv_LeftTopRow, hv_LeftTopColumn, hv_RightBottomRow;
	HTuple  hv_RightBottomColumn, hv_Number;

	hv_nCameraSN = m_tInit.nGrabberSN;
	hv_offsetColumn = 10;
	hv_dFoilWidth = 31;//25
	hv_dFoilHeight =m_tParameters.dExtractFoilRegionWidth/m_dColResolution;
	hv_nFoilBottomThresh = m_tParameters.nExtractFoilThresh;
	GetImageSize(ho_Image, &hv_Width, &hv_Height);
	SmallestRectangle1(ho_CoatRegion, &hv_LeftTopRow, &hv_LeftTopColumn, &hv_RightBottomRow, 
		&hv_RightBottomColumn);
	if (0 != (HTuple(int(hv_nCameraSN==0)).TupleOr(int(hv_nCameraSN==2))))
	{
		GenRectangle1(&ho_FoilBottomROI, 0, (hv_LeftTopColumn-hv_offsetColumn)-(hv_dFoilHeight), 
			hv_Height-1, hv_LeftTopColumn-hv_offsetColumn);
	}
	else
	{

		GenRectangle1(&ho_FoilBottomROI, 0, hv_RightBottomColumn+hv_offsetColumn, hv_Height-1, 
			(hv_RightBottomColumn+hv_offsetColumn)+(hv_dFoilHeight));
	}
	m_tAoBanInfo.hoFoilDetectRegion=ho_FoilBottomROI;

	ReduceDomain(ho_Image, ho_FoilBottomROI, &ho_FoilBottomImageReduced);
	Threshold(ho_FoilBottomImageReduced, &ho_FoilRegion, hv_nFoilBottomThresh, 255);
	ClosingRectangle1(ho_FoilRegion, &ho_RegionClosing, 10, hv_dFoilWidth);
	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_ConnectedRegions, (HTuple("height").Append("width")), 
		"and", hv_dFoilWidth.TupleConcat(20), (HTuple(99999).Append(999)));
	OpeningRectangle1(ho_ConnectedRegions, &ho_ConnectedRegions, 1, 15);
	SortRegion(ho_ConnectedRegions, &(*ho_LastFoil), "first_point", "true", "row");
	m_tAoBanInfo.hoExtractFoilRegion=*ho_LastFoil;
	/*CountObj((*ho_LastFoil), &hv_Number);
	if (0 != (int(hv_Number>0)))
	{
	SelectObj((*ho_LastFoil), &(*ho_LastFoil), hv_Number);
	}
	else
	{
	GenEmptyObj(&(*ho_LastFoil));

	}*/
	return;
}

void CSeparate::FindLocateMissingAgain(HObject hoCoat,HTuple& hv_LocateMiss)
{

	if (m_tInit.nProductType==negative&& m_tParameters.bEnaleUseSegAoBan==TRUE)//如果是负极引入阈值分割
	{
		HTuple hvNum;
		HObject hoAobanDetectRegion,hoAoBanRegion;
		HObject ho_LastFoilRegion;
		GetLastFoil (m_hoPretreatImage,hoCoat,&ho_LastFoilRegion);
		HObject hoLine3Again;
		double dWidthRegion=MAX(10,m_tParameters.dExtractAobanRegionWidth/m_dColResolution);
		hoLine3Again.GenEmptyObj();
		if (ho_LastFoilRegion.CountObj()>0)
		{
			HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
			SmallestRectangle1(ho_LastFoilRegion,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
			double dStartRow, dStartColumn, dEndRow, dEndColumn;

			dStartRow = MIN(hv_LTRow.I(), hv_RBRow.I());
			dEndRow= MAX(hv_LTRow.I(), hv_RBRow.I());
			if(m_tInit.nGrabberSN==0 ||  m_tInit.nGrabberSN==2)
			{
				GenRectangle1(&hoAobanDetectRegion, MIN(hv_LTRow, hv_RBRow), hv_RBColumn, MAX(hv_LTRow, hv_RBRow), hv_RBColumn + dWidthRegion);


				dStartColumn = hv_RBColumn;
				dEndColumn = hv_RBColumn;
			}
			else
			{
				GenRectangle1(&hoAobanDetectRegion, MIN(hv_LTRow, hv_RBRow), hv_LTColumn - dWidthRegion, MAX(hv_LTRow, hv_RBRow), hv_LTColumn);
				/*TempFitLinePara.m_dStartCol = hv_LTColumn;
				TempFitLinePara.m_dEndCol = hv_LTColumn;*/
				dStartColumn = hv_LTColumn;
				dEndColumn = hv_LTColumn;
			}
			
			//if (FALSE == FindLineAgain(m_hoPretreatImage, TempFitLinePara, &dStartRow, &dStartColumn, &dEndRow, &dEndColumn, &hoLine3Again, 3))
			//{
			//	dStartRow=TempFitLinePara.m_dStartRow;
			//	dStartColumn=TempFitLinePara.m_dStartCol;
			//	dEndRow=TempFitLinePara.m_dEndRow;
			//	dEndColumn=TempFitLinePara.m_dEndCol;
			//}

			m_hvRowBegin[3]=dStartRow;
			m_hvColBegin[3]=dStartColumn;
			m_hvRowEnd[3]=dEndRow;
			m_hvColEnd[3]=dEndColumn;

			GenRegionLine(&hoLine3Again, MIN(dStartRow,dEndRow)-100, dStartColumn, MAX(dStartRow, dEndRow) +100, dEndColumn);
			
			HTuple hv_Lin3Index;
			TupleFind(m_hvErrorLineIndex, 3, &hv_Lin3Index);
			if (hv_Lin3Index.Length() > 0 && hv_Lin3Index != -1)
				TupleRemove(m_hvErrorLineIndex, hv_Lin3Index, &m_hvErrorLineIndex);
			/*GetSize(3,4,m_dColResolution,&hv_LocateMiss);
			m_ptOut->hv_BaiBian = hv_LocateMiss;*/
		}else
		{
			GenRectangle1(&hoAobanDetectRegion, MIN(m_hvRowBegin[3].D(), m_hvRowEnd[3].D()), MIN(m_hvColBegin[3].D(), m_hvColEnd[3].D()) - m_sLineTool[3].m_dHeight / 2, MAX(m_hvRowBegin[3].D(), m_hvRowEnd[3].D()), MAX(m_hvColBegin[3].D(), m_hvColEnd[3].D()) + m_sLineTool[3].m_dHeight / 2);
		}

	
		ConcatObj(hoLine3Again,hoAobanDetectRegion,&hoLine3Again);
		GetAoBanRegion(m_hoPretreatImage,hoAobanDetectRegion,&hoAoBanRegion);
		CountObj(hoAoBanRegion,&hvNum);
		if (hvNum>0)
		{
			HTuple hv_LeftRow0,hv_LeftColumn0,hv_RightRow1,hv_RightColumn1;
			int nDetLineIndex=4;
			HTuple hv_Lenggth0;
			HTuple hvStartRow,hvStartColumn,hvEndRow,hvEndColumn;
			SmallestRectangle1(hoAoBanRegion,&hv_LeftRow0,&hv_LeftColumn0,&hv_RightRow1,&hv_RightColumn1);
			TupleLength(hv_LeftRow0,&hv_Lenggth0);
			TupleGenConst(hv_Lenggth0,m_hvRowBegin[nDetLineIndex],&hvStartRow);
			TupleGenConst(hv_Lenggth0,m_hvColBegin[nDetLineIndex],&hvStartColumn);
			TupleGenConst(hv_Lenggth0,m_hvRowEnd[nDetLineIndex],&hvEndRow);
			TupleGenConst(hv_Lenggth0,m_hvColEnd[nDetLineIndex],&hvEndColumn);


			/*TupleGenConst(hv_Lenggth0, m_hvFitLineSR[nDetLineIndex], &hvStartRow);
			TupleGenConst(hv_Lenggth0, m_hvFitLineSC[nDetLineIndex], &hvStartColumn);
			TupleGenConst(hv_Lenggth0, m_hvFitLineER[nDetLineIndex], &hvEndRow);
			TupleGenConst(hv_Lenggth0, m_hvFitLineEC[nDetLineIndex], &hvEndColumn);*/



			HTuple hv_UpRow,hv_UpColumn,hv_DownRow,hv_DownColumn;
			HTuple hv_RightColumnCorrect;
			TupleMax2(hv_LeftColumn0 + 1, hv_RightColumn1, &hv_RightColumnCorrect);

			IntersectionLl(hv_LeftRow0,hv_LeftColumn0,hv_LeftRow0, hv_RightColumnCorrect,hvStartRow,hvStartColumn,hvEndRow,hvEndColumn,&hv_UpRow,&hv_UpColumn,NULL);
			IntersectionLl(hv_RightRow1,hv_LeftColumn0,hv_RightRow1, hv_RightColumnCorrect,hvStartRow,hvStartColumn,hvEndRow,hvEndColumn,&hv_DownRow,&hv_DownColumn,NULL);


			HTuple hv_Value,hv_Select;
			if (m_tInit.nGrabberSN==1||m_tInit.nGrabberSN==3)
			{
				hv_Value=(hv_LeftColumn0-(hv_UpColumn+hv_DownColumn)/2)*m_dColResolution+m_tParameters.dLocateMissingOffset;
			}else
			{
				hv_Value=((hv_UpColumn+hv_DownColumn)/2-hv_RightColumn1)*m_dColResolution+m_tParameters.dLocateMissingOffset;
			}


		    m_tAoBanInfo.hvLocateDistance=hv_Value;




			//排序计算最小6个均值

			if (m_tStandardSize.dLocateMissing.Length()==0 || m_tParameters.nSelectValueMode==0)//为了兼容之前的
			{
				TupleSort(hv_Value,&hv_Value);
				TupleFirstN(hv_Value,MIN(hv_Value.Length()-1,5),&hv_Select);
				TupleMean(hv_Select,&hv_Value);
			}
			else
			{
				///计算和标准值最接近的
				HTuple hv_SubLocate, hv_LocateAbs, hv_LocateIndex;
				TupleSub(hv_Value, m_tStandardSize.dLocateMissing, &hv_SubLocate);
				TupleAbs(hv_SubLocate, &hv_LocateAbs);
				TupleSortIndex(hv_LocateAbs, &hv_LocateIndex);
				TupleFirstN(hv_LocateIndex, MIN(hv_LocateIndex.Length() - 1, 5), &hv_LocateIndex);
				TupleSelect(hv_Value, hv_LocateIndex, &hv_Value);
				TupleMean(hv_Value, &hv_Value);
			}
			
			
			
		





			if (hv_Value.TupleAbs()<m_tParameters.dMaxAoBanDeviation)
			{
				hv_LocateMiss=hv_Value;
			}
			
			m_tAoBanInfo.hoAoBanRegion=hoAoBanRegion;
			m_tAoBanInfo.hoLineBetAoBanAndFoil=hoLine3Again;
		}else
		{
			m_tAoBanInfo.hoAoBanRegion.GenEmptyObj();
			m_tAoBanInfo.hoLineBetAoBanAndFoil=hoLine3Again;

		}
	  
	}
else if (m_tInit.nProductType==positive&& m_tParameters.bEnaleUseSegAoBan==TRUE)
	{
		HTuple hv_MiddleColumn,hv_MiddleRow;
		hv_MiddleRow=(m_hvRowBegin[3]+m_hvRowEnd[3])/2;
		hv_MiddleColumn=(m_hvColBegin[3]+m_hvColEnd[3])/2;
		GetAT9ThreshRegion(m_hoPretreatImage,&m_hoThreshAT9Region,m_tInit.nGrabberSN,hv_MiddleRow,hv_MiddleColumn);
		if (m_hoThreshAT9Region.CountObj()==1)
		{
			HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
			SmallestRectangle1(m_hoThreshAT9Region,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN==2)
			{
				m_hvFitLineSR[4]=hv_RBRow;
				m_hvFitLineSC[4]=hv_RBColumn;
				m_hvFitLineER[4]=hv_RBRow;
				m_hvFitLineEC[4]=hv_RBColumn;

	


			}else
			{
				m_hvFitLineSR[4]=hv_LTRow;
				m_hvFitLineSC[4]=hv_LTColumn;
				m_hvFitLineER[4]=hv_LTRow;
				m_hvFitLineEC[4]=hv_LTColumn;
			}
			HTuple hv_ErrorLineIndex1;
			TupleFind(m_hvErrorLineIndex,4,&hv_ErrorLineIndex1);
			if (hv_ErrorLineIndex1!=-1 && hv_ErrorLineIndex1.Length()>0)
			{
				TupleRemove(m_hvErrorLineIndex,hv_ErrorLineIndex1,&m_hvErrorLineIndex);
				
		     }
			GetSize(4, 3, m_dColResolution, &hv_LocateMiss);
			m_ptOut->hvLocateMiss = hv_LocateMiss;
	}





}

}
void  CSeparate::GetAT9ThreshRegion(HObject ho_Image, HObject* ho_SelectedRegions, HTuple hv_nSide,HTuple hv_dRow, HTuple hv_dColumn)
{

	try
	{
		HObject  ho_AT9ROI_0, ho_ImageReduced, ho_ImagePart;
		HObject  ho_ImageScaled, ho_ImageClosing, ho_Region, ho_RegionClosing;
		HObject  ho_ConnectedRegions;
		HTuple  hv_Width, hv_Height, hv_Row1, hv_Column1;
		HTuple  hv_Row2, hv_Column2, hv_HorProjection, hv_VertProjection;
		HTuple  hv_VertProjectionMin;
		GetImageSize(ho_Image, &hv_Width, &hv_Height);
		if (0 != (HTuple(int(hv_nSide == 0)).TupleOr(int(hv_nSide == 2))))
		{
			GenRectangle1(&ho_AT9ROI_0, hv_Height - 200, hv_dColumn, hv_Height - 100, hv_dColumn + 150);
		}
		else
		{
			GenRectangle1(&ho_AT9ROI_0, hv_Height - 200, hv_dColumn - 150, hv_Height - 100, hv_dColumn);
		}
		ReduceDomain(ho_Image, ho_AT9ROI_0, &ho_ImageReduced);
		SmallestRectangle1(ho_AT9ROI_0, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
		CropDomain(ho_ImageReduced, &ho_ImagePart);
		ScaleImage(ho_ImagePart, &ho_ImageScaled, 3, 0);
		GrayClosingShape(ho_ImageScaled, &ho_ImageClosing, 17, 7, "rectangle");
		GrayProjections(ho_ImageClosing, ho_ImageClosing, "simple", &hv_HorProjection,
			&hv_VertProjection);
		TupleMin(hv_VertProjection, &hv_VertProjectionMin);
		if (0 != (int((HTuple(hv_VertProjectionMin[0]) + 50) < 255)))
		{
			Threshold(ho_ImageClosing, &ho_Region, hv_VertProjectionMin + 50, 255);
			ClosingRectangle1(ho_Region, &ho_RegionClosing, 11, 11);
			OpeningRectangle1(ho_RegionClosing, &ho_RegionClosing, 1, 15);
			Connection(ho_RegionClosing, &ho_ConnectedRegions);
			SelectShapeStd(ho_ConnectedRegions, &(*ho_SelectedRegions), "max_area", 70);
			MoveRegion((*ho_SelectedRegions), &(*ho_SelectedRegions), hv_Row1, hv_Column1);
			ShapeTrans((*ho_SelectedRegions), &(*ho_SelectedRegions), "rectangle1");
		}
		else
		{
			GenEmptyObj(&(*ho_SelectedRegions));
		}


	}catch(HException& e)
	{

		GenEmptyObj(&(*ho_SelectedRegions));
	}
	return;
}
void CSeparate::GenAT9RegionByPoint(HObject* hoRegion)
{

	HTuple hv_LeftTopRow,hv_LeftTopColumn;
	HTuple hv_LeftBottomRow,hv_LeftBottomColumn;
	HTuple hv_RightTopRow,hv_RightTopColumn;
	HTuple hv_RightBottopRow,hv_RightBottomColumn;
	int nLine1;
	HTuple hv_ImageWidth,hv_ImageHeight;
	HTuple hv_Rows,hv_Columns;
	GetImageSize(m_hoPretreatImage,&hv_ImageWidth,&hv_ImageHeight);
	{
		try
		{
			nLine1=3;
			IntersectionLl(m_hvRowBegin[nLine1],m_hvColBegin[nLine1],m_hvRowEnd[nLine1],m_hvColEnd[nLine1],0,0,0,hv_ImageWidth-1,&hv_RightTopRow,&hv_RightTopColumn,NULL);
			double  a=hv_RightTopRow.D();
			double  b=hv_RightTopColumn.D();
			nLine1=4;
			IntersectionLl(m_hvRowBegin[nLine1],m_hvColBegin[nLine1],m_hvRowEnd[nLine1],m_hvColEnd[nLine1],0,0,0,hv_ImageWidth-1,&hv_LeftTopRow,&hv_LeftTopColumn,NULL);
			nLine1=3;
			IntersectionLl(m_hvRowBegin[nLine1],m_hvColBegin[nLine1],m_hvRowEnd[nLine1],m_hvColEnd[nLine1],hv_ImageHeight-1,0,hv_ImageHeight-1,hv_ImageWidth-1,&hv_RightBottopRow,&hv_RightBottomColumn,NULL);
			nLine1=4;
			IntersectionLl(m_hvRowBegin[nLine1],m_hvColBegin[nLine1],m_hvRowEnd[nLine1],m_hvColEnd[nLine1],hv_ImageHeight-1,0,hv_ImageHeight-1,hv_ImageWidth-1,&hv_LeftBottomRow,&hv_LeftBottomColumn,NULL);
			hv_Rows.Append(hv_RightTopRow).Append(hv_RightBottopRow).Append(hv_LeftBottomRow).Append(hv_LeftTopRow).Append(hv_RightTopRow);
			hv_Columns.Append(hv_RightTopColumn).Append(hv_RightBottomColumn).Append(hv_LeftBottomColumn).Append(hv_LeftTopColumn).Append(hv_RightTopColumn);
			GenRegionPolygon(&(*hoRegion),hv_Rows,hv_Columns);	
			FillUp(*hoRegion,&(*hoRegion));
		}catch(HException& e)
		{
			CString strException;
			strException.Format("相机%d CSeparate::GenAT9RegionByPoint出现异:函数%s:错误内容:%s",m_tInit.nGrabberSN,e.ProcName().Text(),e.ErrorMessage().Text());
			((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
		}

	}
}
void CSeparate::GenFitAT9Region(HObject* hoRegion,HTuple hvLineIndex, FitRegionType nFitRegionType)//测试
{
	try
	{
	HTuple hv_StartRow,hv_StartColumn,hv_EndRow,hv_EndColumn;
	HTuple hv_ImageWidth,hv_ImageHeight;
	GetImageSize(m_hoPretreatImage,&hv_ImageWidth,&hv_ImageHeight);
	hv_StartRow.Append(0);
	hv_StartColumn.Append(0);
	hv_EndRow.Append(0);
    hv_EndColumn.Append(hv_ImageWidth-1);
	hv_StartRow.Append(m_hvRowBegin[hvLineIndex[0].I()]);
	hv_StartColumn.Append(m_hvColBegin[hvLineIndex[0].I()]);
	hv_EndRow.Append(m_hvRowEnd[hvLineIndex[0].I()]);
	hv_EndColumn.Append(m_hvColEnd[hvLineIndex[0].I()]);
	hv_StartRow.Append(hv_ImageHeight-1);
	hv_StartColumn.Append(0);
	hv_EndRow.Append(hv_ImageHeight-1);
	hv_EndColumn.Append(hv_ImageWidth-1);
	//if (nFitRegionType== CoatFitRegion)
	//{
	//	hv_StartRow.Append(m_hvRowBegin[hvLineIndex[1].I()]);
	//	hv_StartColumn.Append((m_hvColBegin[hvLineIndex[0].I()]+ m_hvColBegin[hvLineIndex[1].I()])/2);
	//	hv_EndRow.Append(m_hvRowEnd[hvLineIndex[1].I()]);
	//	hv_EndColumn.Append((m_hvColEnd[hvLineIndex[0].I()]+ m_hvColEnd[hvLineIndex[1].I()])/2);
	//}
	//else
	{
		hv_StartRow.Append(m_hvRowBegin[hvLineIndex[1].I()]);
		hv_StartColumn.Append(m_hvColBegin[hvLineIndex[1].I()]);
		hv_EndRow.Append(m_hvRowEnd[hvLineIndex[1].I()]);
		hv_EndColumn.Append(m_hvColEnd[hvLineIndex[1].I()]);
	}
	HTuple hvIndex;
	int  nLength;
	hvIndex.Append(0).Append(1).Append(2).Append(3);
	nLength=hvIndex.Length();
	HTuple RowBegin,ColBegin,RowEnd,ColEnd;
	for (Hlong i=0;i<nLength;i++)
	{
		int nIndex,nIndex1,nIndex2;//该序号前一个以及后一个
		nIndex = hvIndex[i];
		if (0 == i)//第一个点
		{
			nIndex1 = hvIndex[nLength-1];
			nIndex2 = hvIndex[i+1];
		}else if (nLength-1 == i)//最后一个点
		{
			nIndex1 = hvIndex[i-1];
			nIndex2 = hvIndex[0];
		}else//中间的点
		{
			nIndex1 = hvIndex[i-1];
			nIndex2 = hvIndex[i+1];
		}
		HTuple Row1,Row2,Column1,Column2;
		IntersectionLl(hv_StartRow[nIndex],hv_StartColumn[nIndex],hv_EndRow[nIndex],hv_EndColumn[nIndex],hv_StartRow[nIndex1],hv_StartColumn[nIndex1],hv_EndRow[nIndex1],hv_EndColumn[nIndex1],&Row1,&Column1,NULL);
		IntersectionLl(hv_StartRow[nIndex],hv_StartColumn[nIndex],hv_EndRow[nIndex],hv_EndColumn[nIndex],hv_StartRow[nIndex2],hv_StartColumn[nIndex2],hv_EndRow[nIndex2],hv_EndColumn[nIndex2],&Row2,&Column2,NULL);
		HTuple hv_SR, hv_SC, hv_ER, hv_EC;
		hv_SR = MIN(MAX(Row1[0].D(),0),hv_ImageHeight[0].D()-1);
		hv_SC = MIN(MAX(Column1[0].D(),0),hv_ImageWidth[0].D()-1);
		hv_ER = MIN(MAX(Row2[0].D(),0),hv_ImageHeight[0].D()-1);
		hv_EC = MIN(MAX(Column2[0].D(),0),hv_ImageWidth[0].D()-1);
		RowBegin.Append(hv_SR);
		ColBegin.Append(hv_SC);
		RowEnd.Append(hv_ER);
		ColEnd.Append(hv_EC);
	}
	HObject RegionLines,RegionUnion,RegionClosing,RegionFillUp;
	GenRegionLine(&RegionLines,RowBegin,ColBegin,RowEnd,ColEnd);
	Union1(RegionLines,&RegionUnion);
	ClosingRectangle1(RegionUnion,&RegionClosing,50,50);
	FillUp(RegionClosing,&RegionFillUp);
	ShapeTrans(RegionFillUp,hoRegion,"convex");
	}
	catch (HException&e)
	{
	}
}

void CSeparate::GetDestRegionInfo (HObject ho_Image, HObject ho_SpotRegion,HTuple hv_Thresh, HObject *ho_SelectedXLD, 
	HTuple *hv_RowCenter, HTuple *hv_ColumnCenter, HTuple *hv_RectangleWidth, HTuple *hv_RectangleHeiht)
{

	// Local iconic variables
	HObject  ho_ImageReduced, ho_Region, ho_Border;

	// Local control variables
	HTuple  hv_Number, hv_Row1, hv_Column1, hv_Row2;
	HTuple  hv_Column2, hv_Area, hv_PointOrder;

	ReduceDomain(ho_Image, ho_SpotRegion, &ho_ImageReduced);
	//if (m_tParameters.nExtractColor==0)//白色
	//{
	//		Threshold(ho_ImageReduced, &ho_Region, hv_Thresh, 255);
	//}else
	//{
	//		Threshold(ho_ImageReduced, &ho_Region, 0, hv_Thresh);
	//}
	

	ThresholdSubPix(ho_ImageReduced, &ho_Border, hv_Thresh);
	SelectShapeXld(ho_Border, &(*ho_SelectedXLD), (HTuple("width").Append("height").Append("area")), 
		"and", (HTuple(20).Append(20).Append(1000)), (HTuple(150).Append(100).Append(8000)));
	CountObj((*ho_SelectedXLD), &hv_Number);
	if (0 != (int(hv_Number==1)))
	{
		SmallestRectangle1Xld((*ho_SelectedXLD), &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
		//    if(Row1>30 and Row2<Height-30)
		AreaCenterXld((*ho_SelectedXLD), &hv_Area, &(*hv_RowCenter), &(*hv_ColumnCenter), 
			&hv_PointOrder);
		(*hv_RectangleWidth) = hv_Column2-hv_Column1;
		(*hv_RectangleHeiht) = hv_Row2-hv_Row1;
		//    endif
	}
	return;
}



void CSeparate::SetLineMatch()//设置区域
{
	CorrectedPoint();//修正点位
	if (2==m_tParameters.nLaneCount)
	{
		try
		{
#pragma  region "双侧极耳尺寸分割测量"
		
			/* 6->5->1->4代表涂布 7->3->0->2代表极耳*/
			HTuple hv_ImageWidth,hv_ImageHeight;
		    HObject hoCoat,hoFoil;
			HTuple hv_WidthCoat,hv_HeightCoat;
			HTuple hv_WidthFoil,hv_HeightFoil;
			HTuple hv_LocateMiss,hv_BaiBian;
			HTuple hv_Distance1,hv_Distance2;
			double dMidColumn;
			{
				dMidColumn=(m_hvColBegin[5].D() +m_hvColEnd[5].D())/2;//参考点
			}
		
			//计算涂布的总宽和高度
			GetImageSize(m_hoPretreatImage,&hv_ImageWidth,&hv_ImageHeight);
	
			hv_HeightCoat=hv_ImageHeight*m_dRowResolution;
			if (m_tParameters.nFitRegionMode)
			{
				GenFitAT9Region(&hoCoat, HTuple(4).Append(5), CoatFitRegion);
			}else
			{
			  GenRectangle1(&hoCoat,0,MIN((m_hvColBegin[4].D() +m_hvColEnd[4].D())/2,dMidColumn),hv_ImageHeight-1,MAX((m_hvColBegin[4].D() +m_hvColEnd[4].D())/2,dMidColumn));
			}
			
			if (m_tInit.nProductType==positive)
			{
				GetSize(3,6,m_dColResolution,&hv_WidthCoat);
				GetSize(4,5,m_dColResolution,&hv_BaiBian);

			}else
			{
				GetSize(4,5,m_dColResolution,&hv_WidthCoat);
				hv_BaiBian=hv_WidthCoat;
			
				if (m_tInit.nCoatType==M_TwoCam_Intermission && m_tParameters.bEnableVariableWidth)
				{
					try
					{
						HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
						SmallestRectangle1(hoCoat,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
						HTuple hv_UpRow,hv_SColumn,hv_EColumn,hv_DownRow;

						hv_LTColumn=MIN((m_hvColBegin[4].D() +m_hvColEnd[4].D())/2,(m_hvColBegin[5].D() +m_hvColEnd[5].D())/2);
						hv_RBColumn=MAX((m_hvColBegin[4].D() +m_hvColEnd[4].D())/2,(m_hvColBegin[5].D() +m_hvColEnd[5].D())/2);
						hv_UpRow=5/m_dRowResolution;
						hv_DownRow=hv_RBRow.D()-MAX(m_tStandardSize.dUpIntermissionHeight.D()/m_dRowResolution,m_tStandardSize.dDownIntermissionHeight.D()/m_dRowResolution)-5/m_dRowResolution-m_tParameters.dRelateOffset/m_dRowResolution;
						
						hv_SColumn=hv_LTColumn-5/m_dColResolution;
						hv_EColumn=hv_RBColumn+5/m_dColResolution;
						HObject hoUpEdgeCross,hoDownEdgeCross;
						HTuple hv_UpWidth,hv_DownWidth;
						hoUpEdgeCross.GenEmptyObj();
						hoDownEdgeCross.GenEmptyObj();
						hv_BaiBian.Clear();
						GetVariableLength(m_hoPretreatImage,&hoUpEdgeCross,hv_UpRow, hv_UpRow,hv_SColumn,hv_EColumn,&hv_UpWidth);
						hv_BaiBian.Append(hv_UpWidth*m_dColResolution);
						GetVariableLength(m_hoPretreatImage,&hoDownEdgeCross,hv_DownRow, hv_DownRow,hv_SColumn,hv_EColumn,&hv_DownWidth);
						hv_BaiBian.Append(hv_DownWidth*m_dColResolution);

						ConcatObj(m_hoEdageCrossRegion,hoUpEdgeCross,&m_hoEdageCrossRegion);
						ConcatObj(m_hoEdageCrossRegion,hoDownEdgeCross,&m_hoEdageCrossRegion);
					}catch(HException& e)
					{

						hv_BaiBian.Clear();
						hv_BaiBian.Append(9999);
						hv_BaiBian.Append(9999);

					}


				}
		
			}







			///计算极耳的宽度和高度
			HTuple hv_InterRowBegin,hv_InterColBegin,hv_InterRowEnd,hv_InterColEnd;
			IntersectionLl(m_hvRowBegin[0],m_hvColBegin[0],m_hvRowEnd[0],m_hvColEnd[0],m_hvRowBegin[3],m_hvColBegin[3],m_hvRowEnd[3],m_hvColEnd[3],&hv_InterRowBegin,&hv_InterColBegin,NULL);
			IntersectionLl(m_hvRowBegin[2],m_hvColBegin[2],m_hvRowEnd[2],m_hvColEnd[2],m_hvRowBegin[3],m_hvColBegin[3],m_hvRowEnd[3],m_hvColEnd[3],&hv_InterRowEnd,&hv_InterColEnd,NULL);
			DistancePp(hv_InterRowBegin,hv_InterColBegin,hv_InterRowEnd,hv_InterColEnd,&hv_Distance2);
			HObject hoAT9Region;
			hoAT9Region.GenEmptyObj();
			if (m_tParameters.nFitRegionMode)
			{
				GenFitAT9Region(&hoAT9Region,HTuple(3).Append(4),AT9FitRegion);
			    //GenAT9RegionByPoint(&hoAT9Region);
			}else
			{
				GenRectangle1(&hoAT9Region, 0, MIN((m_hvColBegin[3] + m_hvColEnd[3]) / 2, (m_hvColBegin[4] + m_hvColEnd[4]) / 2), hv_ImageHeight - 1, MAX((m_hvColBegin[3] + m_hvColEnd[3]) / 2, (m_hvColBegin[4] + m_hvColEnd[4]) / 2));

			}

			
			hv_HeightFoil=hv_Distance2*m_dRowResolution;	
			GetSize(1,3,m_dColResolution,&hv_WidthFoil);
		    GenRegion(&hoFoil,HTuple(2).Append(3).Append(0).Append(1));
			////计算AT9宽度
			if (m_tInit.nGrabberSN==0 || m_tInit.nGrabberSN == 2)//左侧定位不齐和右侧定位不齐
			{
				if (m_tInit.nProductType == positive)
				{
					GetSize(4, 3, m_dColResolution, &hv_LocateMiss);
				}
				else
				{
					GetSize(3, 4, m_dColResolution, &hv_LocateMiss);
				}
				hv_LocateMiss=hv_LocateMiss+m_tParameters.dLocateMissingOffset;
				//hv_BaiBian=hv_LocateMiss;
				//获取左侧边缘的灰度值
				try
				{
					HTuple hv_ImageHeight,hv_ImageWidth;
					HObject hoRectangle;
					GetImageSize(m_hoPretreatImage,&hv_ImageWidth,&hv_ImageHeight);
					GenRectangle1(&m_hoReferRegion,hv_ImageHeight/2,m_hvFitLineSC[4].D()+10,hv_ImageHeight/2+100,m_hvFitLineSC[4].D()+100);
					Intensity(m_hoReferRegion,m_hoPretreatImage,&m_hvMean,NULL);
				}catch(HException& e)
				{
					m_hvMean=-1;
					m_hoReferRegion.GenEmptyObj();

				}
				
			}
			else
			{
				
				if (m_tInit.nProductType == positive)
				{
					GetSize(4, 3, m_dColResolution, &hv_LocateMiss);
				}
				else
				{
					GetSize(3, 4, m_dColResolution, &hv_LocateMiss);
				}
				
				hv_LocateMiss=hv_LocateMiss+m_tParameters.dLocateMissingOffset;
		       // hv_BaiBian=hv_LocateMiss;
				//获取右侧边缘的灰度值
				try
				{
					HTuple hv_ImageHeight,hv_ImageWidth;
					HObject hoRectangle;
					GetImageSize(m_hoPretreatImage,&hv_ImageWidth,&hv_ImageHeight);
					GenRectangle1(&m_hoReferRegion,hv_ImageHeight/2,MAX(10,m_hvFitLineSC[4].D()-100),hv_ImageHeight/2+100,MAX(100,m_hvFitLineSC[4].D()-10));
					Intensity(m_hoReferRegion,m_hoPretreatImage,&m_hvMean,NULL);
				}catch(HException& e)
				{
					m_hvMean=-1;
					m_hoReferRegion.GenEmptyObj();
				}
		
			}
			




			/////////////////增加阈值分割测试稳定性
		
	        FindLocateMissingAgain(hoCoat,hv_LocateMiss);
			//////////////////////
			WriteGRRData();
			m_ptOut->hv_BaiBian = hv_BaiBian;//
			m_ptOut->hoCoat=hoCoat;
			m_ptOut->hoFoil=hoFoil;
			m_ptOut->hvFoilWidth= hv_WidthFoil;
			m_ptOut->hvFoilHeight =hv_HeightFoil;
			m_ptOut->hvCoatWidth  = hv_WidthCoat;
			m_ptOut->hvCoatHeight = hv_HeightCoat;
			m_ptOut->hoTDispRegion= hoAT9Region;//AT9区域,待添加
			m_ptOut->hvLocateMiss    =hv_LocateMiss;//辅料高度差
          
#pragma endregion
		}catch(HException&e)
		{
			 CString strException;
			 strException.Format("相机%d CSeparate::SetLineMatch出现异:函数%s:错误内容:%s",m_tInit.nGrabberSN,e.ProcName().Text(),e.ErrorMessage().Text());
            ((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);




		}
#pragma  endregion 
	}

}
/*void CSeparate::SaveLane()
{
WritePrivateProfileDouble("Lane","LeftStart",m_tLane.dLeftStart,m_strMyConfig);
WritePrivateProfileDouble("Lane","RightStart",m_tLane.dRightStart,m_strMyConfig);
WritePrivateProfileDouble("Lane","AllLength",m_tLane.dAllLength,m_strMyConfig);
WritePrivateProfileInt("Lane","LeftCount",m_tLane.nLeftCount,m_strMyConfig);
WritePrivateProfileInt("Lane","RightCount",m_tLane.nRightCount,m_strMyConfig);
for(int i=0;i<MAX_LANE;i++)
{
CString lpKeyName;
lpKeyName.Format("%d",i);
WritePrivateProfileDouble("LeftLane",lpKeyName,m_tLane.vLeftPos[i],m_strMyConfig);
}
for(int i=0;i<MAX_LANE;i++)
{
CString lpKeyName;
lpKeyName.Format("%d",i);
WritePrivateProfileDouble("RightLane",lpKeyName,m_tLane.vRightPos[i],m_strMyConfig);
}
}
//void CSeparate::ReadLane()//读取通道设置
//{
//	m_tLane.dLeftStart     = GetPrivateProfileDouble("Lane","LeftStart",0.0,m_strMyConfig);
//	m_tLane.dRightStart    = GetPrivateProfileDouble("Lane","RightStart",0.0,m_strMyConfig);
//	m_tLane.dAllLength     = GetPrivateProfileDouble("Lane","AllLength",380.0,m_strMyConfig);
//	m_tLane.nLeftCount     = GetPrivateProfileInt("Lane","LeftCount",1,m_strMyConfig);
//	m_tLane.nRightCount    = GetPrivateProfileInt("Lane","RightCount",1,m_strMyConfig);
//	m_tLane.vLeftPos.clear();
//	m_tLane.vRightPos.clear();
//	for(int i=0;i<MAX_LANE;i++)
//	{
//		CString lpKeyName;
//		lpKeyName.Format("%d",i);
//		m_tLane.vLeftPos.push_back(GetPrivateProfileDouble("LeftLane",lpKeyName,1.0,m_strMyConfig));
//	}
//	for(int i=0;i<MAX_LANE;i++)
//	{
//		CString lpKeyName;
//		lpKeyName.Format("%d",i);
//		m_tLane.vRightPos.push_back(GetPrivateProfileDouble("RightLane",lpKeyName,1.0,m_strMyConfig));
//	}
//}*/
void CSeparate::SetLane()//设置通道设置
{
	//单片直接就是整个Region了
	GetDomain(m_hoGrabImage,&m_ptOut->hoLane);
}
void CSeparate::PreDetect()
{
	m_tMonitor.ClearAll();
	m_hoLine.GenEmptyObj();
	m_hoCross.GenEmptyObj();
	HomMat2dIdentity(&m_tParameters.hv_Mat2D);
	GenEmptyObj(&m_ptOut->hoCoat);
	GenEmptyObj(&m_ptOut->hoFoil);
	GenEmptyObj(&m_ptOut->hoDispRegion);
	GenEmptyObj(&m_ptOut->hoTDispRegion);
	GenEmptyObj(&m_ptOut->hoErrorRegion);
	m_ptOut->hvErrorType.Clear();
	TupleGenConst(1,0,&m_ptOut->hvCoatHeight);
	TupleGenConst(1,0,&m_ptOut->hvCoatWidth);
	TupleGenConst(1,0,&m_ptOut->hvFoilHeight);
	TupleGenConst(1,0,&m_ptOut->hvFoilWidth);
	TupleGenConst(1,0,&m_ptOut->hv_BaiBian);//每一片要有两个白边
	TupleGenConst(1,0,&m_ptOut->hvLocateMiss);//两个敷料高度

	TupleGenConst(m_tLineMatch.nLineUsed,-1,&m_hvRowBegin);
	TupleGenConst(m_tLineMatch.nLineUsed,-1,&m_hvColBegin);
	TupleGenConst(m_tLineMatch.nLineUsed,-1,&m_hvRowEnd);
	TupleGenConst(m_tLineMatch.nLineUsed,-1,&m_hvColEnd);
	m_hvErrorLineIndex.Clear();
	m_hoReferRegion.GenEmptyObj();
	m_hoEdageCrossRegion.GenEmptyObj();
	m_hoThreshAT9Region.GenEmptyObj();
	m_hvMean=0;
}
void CSeparate::PreProcessFoil(HObject* Region,HTuple hvIndex)
{
	Hlong nLength = hvIndex.Length();
	HTuple RowBegin,ColBegin,RowEnd,ColEnd;
	for (Hlong i=0;i<nLength;i++)
	{
		RowBegin.Append(m_sLineTool[hvIndex[i]].m_dStartRow);
		ColBegin.Append(m_sLineTool[hvIndex[i].I()].m_dStartCol);
		RowEnd.Append(m_sLineTool[hvIndex[i].I()].m_dEndRow);
		ColEnd.Append(m_sLineTool[hvIndex[i].I()].m_dEndCol);
	}
	HObject RegionLines,RegionUnion,RegionClosing,RegionFillUp;
	GenRegionLine(&RegionLines,RowBegin,ColBegin,RowEnd,ColEnd);
	Union1(RegionLines,&RegionUnion);
	ClosingRectangle1(RegionUnion,&RegionClosing,80,80);
	FillUp(RegionClosing,&RegionFillUp);
	ShapeTrans(RegionFillUp,Region,"rectangle1");
	DilationRectangle1(*Region,Region,300,700);
}

void CSeparate::InterSectionLl(HTuple hvIndex)
{
	Hlong nLength = hvIndex.Length();
	HTuple hv_Height,hv_Width;
	GetImageSize(m_hoPretreatImage,&hv_Width,&hv_Height);
	for (Hlong i=0;i<nLength;i++)
	{
		int nIndex,nIndex1,nIndex2;//该序号前一个以及后一个
		nIndex = hvIndex[i];
		if (0 == i)//第一个点
		{
			nIndex1 = hvIndex[nLength-1];
			nIndex2 = hvIndex[i+1];
		}else if (nLength-1 == i)//最后一个点
		{
			nIndex1 = hvIndex[i-1];
			nIndex2 = hvIndex[0];
		}else//中间的点
		{
			nIndex1 = hvIndex[i-1];
			nIndex2 = hvIndex[i+1];
		}
		HTuple Row1,Row2,Column1,Column2;
		IntersectionLl(m_hvRowBegin[nIndex],m_hvColBegin[nIndex],m_hvRowEnd[nIndex],m_hvColEnd[nIndex],m_hvRowBegin[nIndex1],m_hvColBegin[nIndex1],m_hvRowEnd[nIndex1],m_hvColEnd[nIndex1],
			&Row1,&Column1,NULL);
		IntersectionLl(m_hvRowBegin[nIndex],m_hvColBegin[nIndex],m_hvRowEnd[nIndex],m_hvColEnd[nIndex],m_hvRowBegin[nIndex2],m_hvColBegin[nIndex2],m_hvRowEnd[nIndex2],m_hvColEnd[nIndex2],
			&Row2,&Column2,NULL);
		m_hvRowBegin[nIndex] = MIN(MAX(Row1[0].D(),0),hv_Height[0].D()-1);
		m_hvColBegin[nIndex] = MIN(MAX(Column1[0].D(),0),hv_Width[0].D()-1);
		m_hvRowEnd[nIndex] = MIN(MAX(Row2[0].D(),0),hv_Height[0].D()-1);
		m_hvColEnd[nIndex] = MIN(MAX(Column2[0].D(),0),hv_Width[0].D()-1);

	}
}
void CSeparate::GenRegionHW(HTuple hvIndex,HTuple* hvHeight,HTuple* hvWidth)
{  // 1->5->6->4 都是从左边顺时针
	//7->3->0->2
	HTuple hvLeftTopRow,hvLeftTopColumn,hvRightTopRow,hvRightTopColumn,hvLeftBottomRow,hvLeftBottomColumn,hvRightBottomRow,hvRightBottomColumn;
	HTuple hvDistance1(0),hvDistance2(0);
	IntersectionLl(m_hvRowBegin[hvIndex[2].I()],m_hvColBegin[hvIndex[2].I()],m_hvRowEnd[hvIndex[2].I()],m_hvColEnd[hvIndex[2].I()],m_hvRowBegin[hvIndex[1].I()],m_hvColBegin[hvIndex[1].I()],m_hvRowEnd[hvIndex[1].I()],m_hvColEnd[hvIndex[1].I()],&hvLeftTopRow,&hvLeftTopColumn,NULL);
	IntersectionLl(m_hvRowBegin[hvIndex[0].I()],m_hvColBegin[hvIndex[0].I()],m_hvRowEnd[hvIndex[0].I()],m_hvColEnd[hvIndex[0].I()],m_hvRowBegin[hvIndex[1].I()],m_hvColBegin[hvIndex[1].I()],m_hvRowEnd[hvIndex[1].I()],m_hvColEnd[hvIndex[1].I()],&hvLeftBottomRow,&hvLeftBottomColumn,NULL);
	IntersectionLl(m_hvRowBegin[hvIndex[2].I()],m_hvColBegin[hvIndex[2].I()],m_hvRowEnd[hvIndex[2].I()],m_hvColEnd[hvIndex[2].I()],m_hvRowBegin[hvIndex[3].I()],m_hvColBegin[hvIndex[3].I()],m_hvRowEnd[hvIndex[3].I()],m_hvColEnd[hvIndex[3].I()],&hvRightTopRow,&hvRightTopColumn,NULL);
	IntersectionLl(m_hvRowBegin[hvIndex[0].I()],m_hvColBegin[hvIndex[0].I()],m_hvRowEnd[hvIndex[0].I()],m_hvColEnd[hvIndex[0].I()],m_hvRowBegin[hvIndex[3].I()],m_hvColBegin[hvIndex[3].I()],m_hvRowEnd[hvIndex[3].I()],m_hvColEnd[hvIndex[3].I()],&hvRightBottomRow,&hvRightBottomColumn,NULL);

	HTuple hvDifferRow(0),hvDifferColumn(0);
	double  dwDistance(0);
	hvDifferRow=(hvLeftTopRow-hvLeftBottomRow)*m_dRowResolution;
	hvDifferColumn=(hvLeftTopColumn-hvLeftBottomColumn)*m_dColResolution;
	dwDistance=(hvDifferRow*hvDifferRow+hvDifferColumn*hvDifferColumn)[0].D();
	hvDistance1=sqrt(dwDistance);
	hvDifferRow=(hvRightTopRow-hvRightBottomRow)*m_dRowResolution;
	hvDifferColumn=(hvRightTopColumn-hvRightBottomColumn)*m_dColResolution;
	dwDistance=hvDifferRow*hvDifferRow+hvDifferColumn*hvDifferColumn;
	hvDistance2=sqrt(dwDistance);
	*hvHeight=(hvDistance1+hvDistance2)/2;

	hvDifferRow=(hvLeftTopRow-hvRightTopRow)*m_dRowResolution;
	hvDifferColumn=(hvLeftTopColumn-hvRightTopColumn)*m_dColResolution;
	dwDistance=hvDifferRow*hvDifferRow+hvDifferColumn*hvDifferColumn;
	hvDistance1=sqrt(dwDistance);
	hvDifferRow=(hvLeftBottomRow-hvRightBottomRow)*m_dRowResolution;
	hvDifferColumn=(hvLeftBottomColumn-hvRightBottomColumn)*m_dColResolution;
	dwDistance=hvDifferRow*hvDifferRow+hvDifferColumn*hvDifferColumn;
	hvDistance2=sqrt(dwDistance);
	*hvWidth=(hvDistance1+hvDistance2)/2;

}
void CSeparate::GenRegion(HObject* Region,HTuple hvIndex)
{
	Hlong nLength = hvIndex.Length();
	HTuple RowBegin,ColBegin,RowEnd,ColEnd;
	for (Hlong i=0;i<nLength;i++)
	{
		RowBegin.Append(m_hvRowBegin[hvIndex[i].I()]);
		ColBegin.Append(m_hvColBegin[hvIndex[i].I()]);
		RowEnd.Append(m_hvRowEnd[hvIndex[i].I()]);
		ColEnd.Append(m_hvColEnd[hvIndex[i].I()]);
	}
	HObject RegionLines,RegionUnion,RegionClosing,RegionFillUp;
	GenRegionLine(&RegionLines,RowBegin,ColBegin,RowEnd,ColEnd);
	Union1(RegionLines,&RegionUnion);
	ClosingRectangle1(RegionUnion,&RegionClosing,50,50);
	FillUp(RegionClosing,&RegionFillUp);
	ShapeTrans(RegionFillUp,Region,"convex");
}
double CSeparate::SegWidthCalculate(HTuple ColLeft,HTuple ColRight)
{
	HTuple nIndexLeft,nIndexRight;
	double dWidthTemp=0,dWidth;
	double dWidthLeft,dWidthRight;
	GetColSeg(ColLeft,nIndexLeft);
	GetColSeg(ColRight,nIndexRight);
	if (nIndexLeft==nIndexRight)
	{
		dWidth=(m_hvXPose[nIndexRight+1].D()-m_hvXPose[nIndexLeft].D())-
			((m_hvXPose[nIndexRight+1].D()-m_hvXPose[nIndexRight].D())/(m_hvColPose[nIndexRight+1].D()-m_hvColPose[nIndexRight].D()))*
			(m_hvColPose[nIndexRight+1].D()-ColRight.I()+ColLeft.I()-m_hvColPose[nIndexLeft].D());
	}
	else
	{
		//判断左侧边线位于区间中间线左侧或右侧
		if (ColLeft.I()-m_hvColPose[nIndexLeft].D()<m_hvColPose[nIndexLeft+1].D()-ColLeft.I())
		{
			dWidthLeft=(m_hvXPose[nIndexLeft+1].D()-m_hvXPose[nIndexLeft])-
				((m_hvXPose[nIndexLeft+1].D()-m_hvXPose[nIndexLeft])/(m_hvColPose[nIndexLeft+1].D()-m_hvColPose[nIndexLeft].D()))*
				(ColLeft.I()-m_hvColPose[nIndexLeft].D());
		}else
		{
			dWidthLeft=((m_hvXPose[nIndexLeft+1].D()-m_hvXPose[nIndexLeft])/(m_hvColPose[nIndexLeft+1].D()-m_hvColPose[nIndexLeft].D()))*
				(m_hvColPose[nIndexLeft+1].D()-ColLeft.I());
		}
		//判断右侧边线位于区间左侧或右侧
		if (m_hvColPose[nIndexRight+1].D()-ColRight.I()<ColRight.I()-m_hvColPose[nIndexRight].D())
		{
			dWidthRight=(m_hvXPose[nIndexRight+1].D()-m_hvXPose[nIndexRight].D())-
				(m_hvXPose[nIndexRight+1].D()-m_hvXPose[nIndexRight].D())/(m_hvColPose[nIndexRight+1].D()-m_hvColPose[nIndexRight].D())*
				(m_hvColPose[nIndexRight+1].D()-ColRight.I());
		}else
		{
			dWidthRight=(m_hvXPose[nIndexRight+1].D()-m_hvXPose[nIndexRight].D())/(m_hvColPose[nIndexRight+1].D()-m_hvColPose[nIndexRight].D())*
				(ColRight.I()-m_hvColPose[nIndexRight].D());
		}

		dWidth=dWidthLeft+dWidthRight+m_hvXPose[nIndexRight].D()-m_hvXPose[nIndexLeft+1].D();

	}
	return dWidth*m_dCorCoef;
}
void CSeparate::GetColSeg(HTuple nCol,HTuple &nIndex)
{
	HTuple hvIndex(0);
	try
	{	
		HTuple hv_SubTuple(0),hv_SelectTuple(0),hv_Max(0);
		TupleSub(m_hvColPose,nCol,&hv_SubTuple);
		TupleLessEqualElem(hv_SubTuple,0,&hv_SelectTuple);
		TupleFind(hv_SelectTuple,0,&hvIndex);
		nIndex=hvIndex[0]-1;
	}

	catch (HException& e)
	{
		nIndex=0;
	}

}
void  CSeparate::SortPoints(HTuple& hv_Rows, HTuple& hv_Columns, int nSortType)
{
	HTuple hv_Indexs, hv_Selects;
	if (NOSort == nSortType)
	{
		return;
	}
	else if (VerticalSort == nSortType)
	{
		////////////////////////聚类算法
		HTuple hv_SubTuple, hv_LessTuple, hv_Sum;
		int nDiff = m_tParameters.nMaxDiffColumn;//偏离7个像素
		for (int i = 0; i < hv_Rows.Length(); i++)
		{

			TupleSub(hv_Rows, hv_Rows[i], &hv_SubTuple);
			TupleAbs(hv_SubTuple, &hv_SubTuple);
			TupleLessElem(hv_SubTuple, nDiff, &hv_LessTuple);
			TupleSum(hv_LessTuple, &hv_Sum);
			if (hv_Sum * 2 > hv_Rows.Length())
			{
				TupleFind(hv_LessTuple, 1, &hv_Indexs);
				TupleSelect(hv_Rows, hv_Indexs, &hv_Rows);
				TupleSelect(hv_Columns, hv_Indexs, &hv_Columns);
				HTuple hv_SortIndex;
				TupleSortIndex(hv_Columns, &hv_SortIndex);
				TupleSelect(hv_Rows, hv_SortIndex, &hv_Rows);
				TupleSelect(hv_Columns, hv_SortIndex, &hv_Columns);
				break;
			}


		}

	}
	else if (HorizontalSort == nSortType)
	{
		HTuple hv_SubTuple, hv_LessTuple, hv_Sum;
		int nDiff = m_tParameters.nMaxDiffColumn;
		for (int i = 0; i < hv_Columns.Length(); i++)
		{

			TupleSub(hv_Columns, hv_Columns[i], &hv_SubTuple);
			TupleAbs(hv_SubTuple, &hv_SubTuple);
			TupleLessElem(hv_SubTuple, nDiff, &hv_LessTuple);
			TupleSum(hv_LessTuple, &hv_Sum);
			if (hv_Sum * 2 > hv_Columns.Length())
			{
				TupleFind(hv_LessTuple, 1, &hv_Indexs);
				TupleSelect(hv_Rows, hv_Indexs, &hv_Rows);
				TupleSelect(hv_Columns, hv_Indexs, &hv_Columns);
				HTuple hv_SortIndex;

				TupleSortIndex(hv_Rows, &hv_SortIndex);
				TupleSelect(hv_Rows, hv_SortIndex, &hv_Rows);
				TupleSelect(hv_Columns, hv_SortIndex, &hv_Columns);
				break;
			}
		}
	}
	else if (MINCloumns == nSortType)
	{
		HTuple hv_SubTuple, hv_LessTuple, hv_Sum;
	    HTuple hv_SortIndex,hv_SelectIndex;
		TupleSortIndex(hv_Columns,&hv_SortIndex);
		TupleFirstN(hv_SortIndex,MIN(5,hv_SortIndex.Length()-1),&hv_SelectIndex);
		if (hv_SelectIndex.Length()>0)
		{
			TupleSelect(hv_Columns,hv_SelectIndex,&hv_Columns);
			TupleSelect(hv_Rows,hv_SelectIndex,&hv_Rows);
		}
	}else if (MAXCloumns == nSortType)
	{
		HTuple hv_SubTuple, hv_LessTuple, hv_Sum;
		HTuple hv_SortIndex,hv_SelectIndex;
		TupleSortIndex(hv_Columns,&hv_SortIndex);
		TupleInverse(hv_SortIndex,&hv_SortIndex);
		TupleFirstN(hv_SortIndex,MIN(5,hv_SortIndex.Length()-1),&hv_SelectIndex);
		if (hv_SelectIndex.Length()>0)
		{
			TupleSelect(hv_Columns,hv_SelectIndex,&hv_Columns);
			TupleSelect(hv_Rows,hv_SelectIndex,&hv_Rows);
		}
	}
}

void CSeparate::CorrectedPoint()//修正点位
{
	/* 7->3->0->2代表极耳
	6->5->1->4代表涂布*/
	m_hvFitLineSR=m_hvRowBegin;
	m_hvFitLineSC=m_hvColBegin;
	m_hvFitLineER=m_hvRowEnd;
	m_hvFitLineEC=m_hvColEnd;
	InterSectionLl(HTuple(2).Append(3).Append(0).Append(1));//求出对应的交点

}

void CSeparate::GetHommat2D(HObject& hoImage, HTuple& hv_Mat2D)
{
	HomMat2dIdentity(&hv_Mat2D);
	
}

BOOL CSeparate::FindPoint()
{
	if (1==m_tParameters.nLaneCount && m_tLineMatch.nLineUsed<8)//单侧极耳需要8条线
	{
		return FALSE;
	}
	BOOL bRet = TRUE;
	m_hoPretreatImage = m_hoGrabImage;


	GetHommat2D(m_hoPretreatImage,m_tParameters.hv_Mat2D);

	if (m_tParameters.bPretreat)
	{

		GetPretreatImage(m_hoPretreatImage, m_tParameters.hv_Mat2D);//预处理
	}

	for (int i=0;i<m_tLineMatch.nLineUsed;i++)
	{
		HObject hoLine;
		double dStartRow(0.0),dStartColumn(0.0),dEndRow(0.0),dEndColumn(0.0);
		if(FALSE == FitLine(m_hoPretreatImage,m_sLineTool[i].AffineTrans(m_tParameters.hv_Mat2D,i),&hoLine,&dStartRow,&dStartColumn,&dEndRow,&dEndColumn,i))
		{

			ConcatObj(m_hoLine,hoLine,&m_hoLine);
			m_hvRowBegin[i] = dStartRow;
			m_hvColBegin[i] = dStartColumn;
			m_hvRowEnd[i]   = dEndRow;
			m_hvColEnd[i]   = dEndColumn;
			m_hvErrorLineIndex.Append(i);

		}else
		{

			if (fabs(dStartColumn-dEndColumn)>m_tParameters.nMaxDiffColumn &&(4==i||5==i||3==i||6==i))//如果是起始终止列小于若干列则认为寻边失败
			{
				m_hvErrorLineIndex.Append(i);
				ConcatObj(m_hoLine,hoLine,&m_hoLine);
				m_hvRowBegin[i] = m_sLineTool[i].m_dStartRow;
				m_hvColBegin[i] = m_sLineTool[i].m_dStartCol;
				m_hvRowEnd[i]   = m_sLineTool[i].m_dEndRow;
				m_hvColEnd[i]   = m_sLineTool[i].m_dEndCol;
			}else
			{
				ConcatObj(m_hoLine,hoLine,&m_hoLine);
				m_hvRowBegin[i] = dStartRow;
				m_hvColBegin[i] = dStartColumn;
				m_hvRowEnd[i]   = dEndRow;
				m_hvColEnd[i]   = dEndColumn;
			}
		}
	}
	return bRet;
}

void CSeparate::DistanceP_L(HTuple hvPointLineID,HTuple hvCommoomLineID,double Resolution,HTuple* hvDistance)
{
	HTuple hv_Distance1,hv_Distance2;
	DistancePl(m_hvRowBegin[hvPointLineID],m_hvColBegin[hvPointLineID],m_hvRowBegin[hvCommoomLineID],m_hvColBegin[hvCommoomLineID],m_hvRowEnd[hvCommoomLineID],m_hvColEnd[hvCommoomLineID],&hv_Distance1);
	DistancePl(m_hvRowEnd[hvPointLineID],m_hvColEnd[hvPointLineID],m_hvRowBegin[hvCommoomLineID],m_hvColBegin[hvCommoomLineID],m_hvRowEnd[hvCommoomLineID],m_hvColEnd[hvCommoomLineID],&hv_Distance2);
	*hvDistance=(hv_Distance1+hv_Distance2)/2*Resolution;
}
void CSeparate::DistanceP_P(HTuple hvInterLine0ID,HTuple hvInterLine1ID,HTuple hvCommoonLineID,double dRowResolution,double dColumnResolution,HTuple* hvDistance)
{
	HTuple hv_InterRowBegin,hv_InterColBegin,hv_InterRowEnd,hv_InterColEnd;
	IntersectionLl(m_hvRowBegin[hvInterLine0ID],m_hvColBegin[hvInterLine0ID],m_hvRowEnd[hvInterLine0ID],m_hvColEnd[hvInterLine0ID],m_hvRowBegin[hvCommoonLineID],m_hvColBegin[hvCommoonLineID],m_hvRowEnd[hvCommoonLineID],m_hvColEnd[hvCommoonLineID],&hv_InterRowBegin,&hv_InterColBegin,NULL);
	IntersectionLl(m_hvRowBegin[hvInterLine1ID],m_hvColBegin[hvInterLine1ID],m_hvRowEnd[hvInterLine1ID],m_hvColEnd[hvInterLine1ID],m_hvRowBegin[hvCommoonLineID],m_hvColBegin[hvCommoonLineID],m_hvRowEnd[hvCommoonLineID],m_hvColEnd[hvCommoonLineID],&hv_InterRowEnd,&hv_InterColEnd,NULL);
	HTuple RowDistance = HTuple(hv_InterRowBegin - hv_InterRowEnd).TupleAbs() * dRowResolution;
	HTuple ColDistance = HTuple(hv_InterColBegin - hv_InterColEnd).TupleAbs() * dColumnResolution;
	TupleSqrt(RowDistance*RowDistance+ColDistance*ColDistance,&(*hvDistance));
}



void CSeparate::GetPretreatImage(HObject& hoImage, HTuple& hv_AffineMat2D)
{
	try
	{
		HObject hoLine,hoRectangle,hoImageReduced;
		CLineToolPara tLineToolParam;
		int nLineIndex=3;
		tLineToolParam=m_sLineTool[3].AffineTrans(hv_AffineMat2D,nLineIndex);
		GenRegionLine(&hoLine,tLineToolParam.m_dStartRow,tLineToolParam.m_dStartCol,tLineToolParam.m_dEndRow,tLineToolParam.m_dEndCol);
		DilationRectangle1(hoLine,&hoRectangle,150,150);
		ReduceDomain(hoImage,hoRectangle,&hoImageReduced);
		GrayOpeningRect(hoImageReduced, &hoImageReduced, m_tParameters.nSizePretreat, m_tParameters.nXSizePretreat);
		PaintGray(hoImageReduced,hoImage,&hoImage);

	}catch(HException& e)
	{
		hoImage=m_hoPretreatImage;
	}

}
void  CSeparate::GetVariableLength (HObject ho_Image, HObject *ho_EdgeCross, HTuple hv_LineRowStart_Measure_02_0, 
	HTuple hv_LineRowEnd_Measure_02_0, HTuple hv_LineColumnStart_Measure_02_0, HTuple hv_LineColumnEnd_Measure_02_0, 
	HTuple *hv_Width_Measure_02_0)
{

	// Local iconic variables
	HObject  ho_EdgeCross1;

	// Local control variables
	HTuple  hv_AmplitudeThreshold, hv_RoiWidthLen2;
	HTuple  hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Dr;
	HTuple  hv_TmpCtrl_Dc, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1;
	HTuple  hv_TmpCtrl_Len2, hv_MsrHandle_Measure_02_0, hv_Row1_Measure_02_0;
	HTuple  hv_Column1_Measure_02_0, hv_Amplitude1_Measure_02_0;
	HTuple  hv_Row2_Measure_02_0, hv_Column2_Measure_02_0, hv_Amplitude2_Measure_02_0;
	HTuple  hv_Distance_Measure_02_0;

	//Measure 02: Convert coordinates to rectangle2 type
	hv_AmplitudeThreshold = 32;
	hv_RoiWidthLen2 = 15;
	hv_TmpCtrl_Row = 0.5*(hv_LineRowStart_Measure_02_0+hv_LineRowEnd_Measure_02_0);
	hv_TmpCtrl_Column = 0.5*(hv_LineColumnStart_Measure_02_0+hv_LineColumnEnd_Measure_02_0);
	hv_TmpCtrl_Dr = hv_LineRowStart_Measure_02_0-hv_LineRowEnd_Measure_02_0;
	hv_TmpCtrl_Dc = hv_LineColumnEnd_Measure_02_0-hv_LineColumnStart_Measure_02_0;
	hv_TmpCtrl_Phi = hv_TmpCtrl_Dr.TupleAtan2(hv_TmpCtrl_Dc);
	hv_TmpCtrl_Len1 = 0.5*(((hv_TmpCtrl_Dr*hv_TmpCtrl_Dr)+(hv_TmpCtrl_Dc*hv_TmpCtrl_Dc)).TupleSqrt());
	hv_TmpCtrl_Len2 = hv_RoiWidthLen2;
	//Measure 02: Create measure for line Measure 02 [0]
	//Measure 02: Attention: This assumes all images have the same size!
	HTuple hv_Width,hv_Height;
	GetImageSize(ho_Image,&hv_Width,&hv_Height);
	GenMeasureRectangle2(hv_TmpCtrl_Row, hv_TmpCtrl_Column, hv_TmpCtrl_Phi, hv_TmpCtrl_Len1, 
		hv_TmpCtrl_Len2, hv_Width,hv_Height, "nearest_neighbor", &hv_MsrHandle_Measure_02_0);
	//Measure 02: Execute measurements
	MeasurePairs(ho_Image, hv_MsrHandle_Measure_02_0, 2, hv_AmplitudeThreshold, "all", 
		"all", &hv_Row1_Measure_02_0, &hv_Column1_Measure_02_0, &hv_Amplitude1_Measure_02_0, 
		&hv_Row2_Measure_02_0, &hv_Column2_Measure_02_0, &hv_Amplitude2_Measure_02_0, 
		&(*hv_Width_Measure_02_0), &hv_Distance_Measure_02_0);
	//Measure 02: Do something with the results
	if (0 != (int(((*hv_Width_Measure_02_0).TupleLength())==0)))
	{
		(*hv_Width_Measure_02_0) = 0;
		GenRegionLine(&(*ho_EdgeCross), hv_LineRowStart_Measure_02_0, hv_LineColumnStart_Measure_02_0, hv_LineRowEnd_Measure_02_0, hv_LineColumnEnd_Measure_02_0);
	}
	else
	{
		if (0 != (HTuple(int((hv_Row1_Measure_02_0.TupleLength())>0)).TupleAnd(int((hv_Row2_Measure_02_0.TupleLength())>0))))
		{
			GenCrossContourXld(&(*ho_EdgeCross), hv_Row1_Measure_02_0, hv_Column1_Measure_02_0, 
				100, 0);
			GenCrossContourXld(&ho_EdgeCross1, hv_Row2_Measure_02_0, hv_Column2_Measure_02_0, 
				100, 0);
			ConcatObj((*ho_EdgeCross), ho_EdgeCross1, &(*ho_EdgeCross));
		}
	}
	return;
}
int CSeparate::Detect(SEPARATEINPUT &tIn,SEPARATEOUTPUT *ptOut)
{
	m_ptOut = ptOut;
	m_hoGrabImage = tIn.hoGrabImage;
	m_ptIn = &tIn;
	PreDetect();

	try
	{
		BOOL bRet = FindPoint();
		if (TRUE == bRet)
		{	

			SetLineMatch();
			return TRUE;
		}
		else
		{
			CallException(ptOut);	
			return TRUE;
		}   
	}catch(HException e)
	{
		CallException(ptOut);
		return TRUE;
	}


}     

int CSeparate::FitLine (const HObject& Image, const CLineToolPara& FitLinePara, HObject* Line, 
	double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineIndex,int nLineDisplay)
{
	try
	{
#pragma region "先对拟合方式进行判断，如果不是正常拟合直接就返回了"
		HTuple Width, Height;
		GetImageSize(Image,&Width,&Height);
		CLineToolPara TempFitLinePara;
		TempFitLinePara=FitLinePara;

		TempFitLinePara.m_dStartRow=MAX(0,MIN(TempFitLinePara.m_dStartRow,Height.I()-2));
		TempFitLinePara.m_dStartCol=MAX(0,MIN(TempFitLinePara.m_dStartCol,Width.I()-2));
		TempFitLinePara.m_dEndRow=MAX(1,MIN(TempFitLinePara.m_dEndRow,Height.I()-1));
		TempFitLinePara.m_dEndCol=MAX(1,MIN(TempFitLinePara.m_dEndCol,Width.I()-1));
		if (0!=TempFitLinePara.m_nChoice)
		{
			HObject Domain;
			HTuple Row1,Column1,Row2,Column2;
			GetDomain(Image,&Domain);
			SmallestRectangle1(Domain,&Row1,&Column1,&Row2,&Column2);
			if (1 == TempFitLinePara.m_nChoice)//固定值
			{
				*dStartX = TempFitLinePara.m_dStartCol;
				*dStartY = TempFitLinePara.m_dStartRow;
				*dEndX   = TempFitLinePara.m_dEndCol;
				*dEndY   = TempFitLinePara.m_dEndRow;
				GenRegionLine(Line,*dStartY,*dStartX,*dEndY,*dEndX);
			}else if (2 == TempFitLinePara.m_nChoice)//固定顶端
			{
				*dStartX = TempFitLinePara.m_dStartCol;
				*dStartY = Row1.I();
				*dEndX   = TempFitLinePara.m_dEndCol;
				*dEndY   = Row1.I();
				GenRegionLine(Line,*dStartY,*dStartX,*dEndY,*dEndX);
			}else if (3 == TempFitLinePara.m_nChoice)//固定底端
			{
				*dStartX = TempFitLinePara.m_dStartCol;
				*dStartY = Row2.I();
				*dEndX   = TempFitLinePara.m_dEndCol;
				*dEndY   = Row2.I();
				GenRegionLine(Line,*dStartY,*dStartX,*dEndY,*dEndX);
			}else if (4 == TempFitLinePara.m_nChoice)//固定左侧
			{
				*dStartX = Column1.I();
				*dStartY = TempFitLinePara.m_dStartRow;
				*dEndX   = Column1.I();
				*dEndY   = TempFitLinePara.m_dEndRow;
				GenRegionLine(Line,*dStartY,*dStartX,*dEndY,*dEndX);
			}else if (5 == TempFitLinePara.m_nChoice)//固定右侧
			{
				*dStartX = Column2.I();
				*dStartY = TempFitLinePara.m_dStartRow;
				*dEndX   = Column2.I();
				*dEndY   = TempFitLinePara.m_dEndRow;
				GenRegionLine(Line,*dStartY,*dStartX,*dEndY,*dEndX);
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
		Phi = ((HTuple(TempFitLinePara.m_dStartRow)-HTuple(TempFitLinePara.m_dEndRow)).TupleAtan2(HTuple(TempFitLinePara.m_dEndCol)-HTuple(TempFitLinePara.m_dStartCol)))\
			+(HTuple(90).TupleRad());
		HObject hMeasureRect,hCross;
		ResultRow = HTuple();
		ResultCol = HTuple();
		if (0 != (HTuple(TempFitLinePara.m_nDirect)==0))
		{
			transition = "all";
		}
		else if (0 != (HTuple(TempFitLinePara.m_nDirect)==1))
		{
			transition = "positive";
		}
		else
		{
			transition = "negative";
		}
		if (0 != (HTuple(TempFitLinePara.m_nSelect)==0))
		{
			select = "all";
		}
		else if (0 != (HTuple(TempFitLinePara.m_nSelect)==1))
		{
			select = "first";
		}
		else
		{
			select = "last";
		}

		for (i=0; i<HTuple(TempFitLinePara.m_nMeasureNum); i+=1)
		{
			RowC = HTuple(TempFitLinePara.m_dStartRow)+(((HTuple(TempFitLinePara.m_dEndRow)-HTuple(TempFitLinePara.m_dStartRow))*i)/(HTuple(TempFitLinePara.m_nMeasureNum)-1));
			ColC = HTuple(TempFitLinePara.m_dStartCol)+(((HTuple(TempFitLinePara.m_dEndCol)-HTuple(TempFitLinePara.m_dStartCol))*i)/(HTuple(TempFitLinePara.m_nMeasureNum)-1));
			//Convert coordinates to rectangle2 type
			GenRectangle2(&hMeasureRect,RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), HTuple(TempFitLinePara.m_dWidth));
			GenRectangle2(&Rectangle,RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), 1);
			if (0 == nLineDisplay)
			{
				ConcatObj(*Line,Rectangle,Line);
			}else
			{
				ConcatObj(*Line,hMeasureRect,Line);
			}
			try
			{

				GenMeasureRectangle2(RowC, ColC, Phi, HTuple(TempFitLinePara.m_dHeight), HTuple(TempFitLinePara.m_dWidth), 
					Width, Height, "nearest_neighbor", &MsrHandle_Measure);
				MeasurePos(Image, MsrHandle_Measure, HTuple(TempFitLinePara.m_dSigma), HTuple(TempFitLinePara.m_nThr), 
					transition, select, &RowEdge, &ColEdge, &Amplitude, &Distance);
				
			}
			catch(HException& e)
			{
				
			}
			CloseMeasure(MsrHandle_Measure);

			tRow = 0;
			tCol = 0;
			t = 0;
			for (int j=0; j<=(RowEdge.Length())-1; j+=1)
			{
				if (0 != (t<HTuple(Amplitude[j]).TupleAbs()))
				{
					tRow = RowEdge[j];
					tCol = ColEdge[j];
					t = HTuple(Amplitude[j]).TupleAbs();
				}
			}
			if (0 != (t>0))
			{
				ResultRow.Append(tRow);
				ResultCol.Append(tCol);
				GenCrossContourXld(&hCross,tRow[0].D(),tCol[0].D(),18,0.78);
				ConcatObj(*Line,hCross,Line);
			}
		}
		//stop ()
		HObject hLine;
		if (ResultRow.Length()>FitLinePara.m_nMeasureNum/2)
		{

			SortPoints(ResultRow, ResultCol, TempFitLinePara.m_nSortType);
			GenContourPolygonXld(&Contour, ResultRow, ResultCol);//tukey
	
			FitLineContourXld(Contour, "tukey", -1, 0, 5, 2, &RowBegin, 
				&ColBegin, &RowEnd, &ColEnd, &Nr, &Nc, &Dist);
			GenContourPolygonXld(&hLine, RowBegin.TupleConcat(RowEnd), ColBegin.TupleConcat(ColEnd));
			ConcatObj(*Line,hLine,Line);
			*dStartY = RowBegin[0].D();
			*dStartX = ColBegin[0].D();
			*dEndY = RowEnd[0].D();
			*dEndX = ColEnd[0].D();


		}
		else//如果没找到线，就是用原来的位置
		{

			GenContourPolygonXld(&hLine, HTuple(TempFitLinePara.m_dStartRow).TupleConcat(TempFitLinePara.m_dEndRow), 
				HTuple(TempFitLinePara.m_dStartCol).TupleConcat(TempFitLinePara.m_dEndCol));
			ConcatObj(*Line,hLine,Line);
			*dStartY = TempFitLinePara.m_dStartRow;
			*dStartX = TempFitLinePara.m_dStartCol;
			*dEndY = TempFitLinePara.m_dEndRow;
			*dEndX = TempFitLinePara.m_dEndCol;
			return FALSE;
		}
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}


}

int CSeparate::DisplaySetDialog(SEPARATEINPUT & tIn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgParamSet dlg;
	dlg.m_pRealDetect = this;
	*dlg.m_pTempDetect = *this;
	dlg.m_pDetectInput = &tIn;
	return (int)dlg.DoModal() ;
}

BOOL CSeparate::ReadParam()
{
	try
	{
		CString strFilePath;
		strFilePath.Format("%s\\Config\\%s\\SCISeparate\\%d",GetCurrentAppPath(),m_tInit.strProductName,
			m_tInit.nGrabberSN);
		CreateMultilevelPath(strFilePath);
		for (int i=0;i<LINE_COUNT;i++)
		{
			CString strFileName;
			strFileName.Format("%s\\%d.ini",strFilePath,i);
			m_sLineTool[i].ReadParam(strFileName);
		}
		m_strMyConfig.Format("%s\\Config.ini",strFilePath);
		m_strSystemVis.Format("%s/Config/System.vis",GetCurrentAppPath());
		m_tParameters.bPretreat = GetPrivateProfileInt("PARAMETER","PRETREAT",0,m_strMyConfig);
		m_tParameters.nSizePretreat = GetPrivateProfileInt("PARAMETER","SIZEPRETREAT",15,m_strMyConfig);
		m_tParameters.nXSizePretreat = GetPrivateProfileInt("PARAMETER","nXSizePretreat",15,m_strMyConfig);
		m_tParameters.bUseGRR = GetPrivateProfileInt("PARAMETER","bUseGRR",0,m_strMyConfig);
		m_tParameters.nSelectNum = GetPrivateProfileInt("PARAMETER","nSelectNum",1,m_strMyConfig);
		
		m_tParameters.nExtractFoilThresh = GetPrivateProfileInt("PARAMETER","nExtractFoilThresh",70,m_strMyConfig);

		m_tParameters.nHcalibFlag = GetPrivateProfileInt("PARAMETER","NHCALIBFLAG0",0,m_strMyConfig);
		m_tParameters.nFitRegionMode = GetPrivateProfileInt("PARAMETER","nFitRegionMode",0,m_strMyConfig);
		m_tParameters.nMaxDiffColumn = GetPrivateProfileInt("PARAMETER","nMaxDiffColumn",20,m_strMyConfig);
		m_tParameters.dLocateMissingOffset=GetPrivateProfileDouble("PARAMETER","dLocateMissingOffset",0,m_strMyConfig);
		m_tParameters.dMaxAoBanDeviation=GetPrivateProfileDouble("PARAMETER","dMaxAoBanDeviation",3,m_strMyConfig);

		m_tParameters.nSelectValueMode=GetPrivateProfileInt("PARAMETER","nSelectValueMode",0,m_strMyConfig);

		m_tParameters.bEnaleUseSegAoBan=GetPrivateProfileInt("PARAMETER","bEnaleUseSegAoBan",0,m_strMyConfig);
	 	m_tParameters.bFindManualOrAuto=GetPrivateProfileInt("PARAMETER","bFindManualOrAuto",1,m_strMyConfig);
	    m_tParameters.dExtractFoilRegionWidth=GetPrivateProfileDouble("PARAMETER","dExtractFoilRegionWidth",15,m_strMyConfig);
		m_tParameters.nExtractFoilThresh = GetPrivateProfileInt("PARAMETER","nExtractFoilThresh",60,m_strMyConfig);
		m_tParameters.dExtractAobanRegionWidth=GetPrivateProfileDouble("PARAMETER","dExtractAobanRegionWidth",5,m_strMyConfig);
		m_tParameters.nExtractAobanThresh=GetPrivateProfileInt("PARAMETER","nExtractAobanThresh",30,m_strMyConfig);
		m_tParameters.bEnableVariableWidth=GetPrivateProfileInt("PARAMETER","bEnableVariableWidth",0,m_strMyConfig);

		m_tParameters.dRelateOffset=GetPrivateProfileDouble("PARAMETER","dRelateOffset",0,m_strMyConfig);


		m_tParameters.emCoatType = (e_CoatType)GetPrivateProfileInt(m_tInit.strProductName,"CoatType",0,m_strSystemVis);//左余料
		m_tParameters.emProductType = (e_ProductType)GetPrivateProfileInt(m_tInit.strProductName,"ProductType",0,m_strSystemVis);//右余料
		m_tParameters.emGrabberType = (e_GrabberType)GetPrivateProfileInt(m_tInit.strProductName,"GrabberType",0,m_strSystemVis);;//分条数
		m_tParameters.nLaneCount = GetPrivateProfileInt("System","nLaneCount",1,m_strSystemVis);//单条还是双条



		//m_tParameters.dStandardWidth= GetPrivateProfileDouble("System","dStandardWidth",-1,m_strMyConfig);//中心基准
		m_tParameters.dStandardLeftLocation= GetPrivateProfileDouble("System","dStandardLeftLocation",-1,m_strMyConfig);//左侧基准
		m_tParameters.dStandardRightLocation= GetPrivateProfileDouble("System","dStandardRightLocation",-1,m_strMyConfig);//右侧基准



		CString lpAppName,keyName;
		lpAppName.Format("GRABBER%d",m_tInit.nGrabberSN);
		m_strCommondh.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
		m_dColResolution = GetPrivateProfileDouble(lpAppName,"COLUMNRESOLUTION",1.0000,m_strCommondh);
		m_dRowResolution = GetPrivateProfileDouble(lpAppName,"ROWRESOLUTION",1.0000,m_strCommondh);
		m_nPoseCount=GetPrivateProfileInt(lpAppName,"POSECOUNT",1,m_strCommondh);
		for (int i=0;i<m_nPoseCount;i++)
		{
			double temp;
			keyName.Format("PIXELPOSE%d",i);
			temp=GetPrivateProfileDouble(lpAppName,keyName,1.0000,m_strCommondh);
			m_hvColPose.Append(HTuple(temp));

			keyName.Format("XPOSE%d",i);
			temp=GetPrivateProfileDouble(lpAppName,keyName,1.0000,m_strCommondh);
			m_hvXPose.Append(HTuple(temp));

			keyName.Format("PartRes%d",i);
			temp=GetPrivateProfileDouble(lpAppName,keyName,0.104,m_strCommondh);
			m_hvRes.Append(HTuple(temp));
		}
		lpAppName.Format("CALIBRATION%d",m_tInit.nGrabberSN);
		m_dCalSpaceDist=GetPrivateProfileDouble(lpAppName,"CALSPACEDIST",1.0000,m_strCommondh);
		m_dCorCoef=GetPrivateProfileDouble(lpAppName,"CORCOEF",1.0000,m_strCommondh);
		return TRUE;
	}catch(HException e)
	{
		return FALSE;
	}
}