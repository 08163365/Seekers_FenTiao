#include "StdAfx.h"
#include "Judge.h"
#include "DlgTabSizeSet.h"
#include "DlgStandardSetting.h"
#include <iostream>
#include <iomanip>
using namespace std;
CJudge::CJudge(void) :m_hvec_CPK(1)
{
	m_nLastImageSN = 1;
	m_dLastCropRow = 0;
	m_dExeptLastCropRow = 0;
	m_nExeptLastSN = 1;
	m_nContinueEANGNum = 0;
	m_bCancelLabelFlag = FALSE;
	m_nContinuAt9NGNum = 0;
}
CJudge::~CJudge(void)
{
}
void CJudge::Init(const INITJUDGE& tInit)
{
	m_tInit = tInit;
	m_strConfigFile.Format("%s\\Config\\Products\\%s\\SCIJudge\\%d\\JudgeParam.ini", GetCurrentAppPath(), m_tInit.strProductName, m_tInit.nSN);
	HTuple hv_Exist;
	FileExists((HTuple)m_strConfigFile, &hv_Exist);
	if (hv_Exist == FALSE)
	{
		CString strPath, strSourceFile, strDestFile;
		strPath.Format("%s\\Config\\Products\\%s\\SCIJudge\\\%d", GetCurrentAppPath(), m_tInit.strProductName, m_tInit.nSN);
		strSourceFile.Format("%s\\Config\\Products\\%s\\SCIJudge\\JudgeParam.ini", GetCurrentAppPath(), m_tInit.strProductName);
		strDestFile.Format("%s\\Config\\Products\\%s\\SCIJudge\\%d\\JudgeParam.ini", GetCurrentAppPath(), m_tInit.strProductName, m_tInit.nSN);
		CreateMultilevelPath(strPath);
		CopyFile((HTuple)strSourceFile, (HTuple)strDestFile);
	}
	ReadParam();
	PreClear();
	WriteParam();
	m_hvComPareTuple = m_tStandardSize.hv_CoatHeight;///比较数组里面的初始值按照标准值赋值
}
BOOL CJudge::ShowParam()
{
	CDlgTabSizeSet dlg;
	dlg.SetOwner(this);
	if (IDOK == dlg.DoModal())
	{
		//WriteParam();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}





void CJudge::JustWarnOrLabel(BOOL bEnable, BOOL bEnableLabel, HTuple hv_StandardSize, HTuple hv_StandardDev, HTuple hv_RealDetectSize, e_ErrorType ErrorType, C_ALLOCATORWORKSHOPOUTPUT& tOutput, BOOL& bRet, int nSN, BOOL bAllNGFlag)//确认是否贴标
{
	HObject hoDomain;
	int nSide = nSN;//前相机还是后相机 前相机取0，后相机取1
	BOOL bSingleFlag = TRUE;
	GetDomain(tOutput.B[nSide].A.hoGrabImage, &hoDomain);
	if (TRUE == bEnable)///
	{
		for (int i = 0; i < hv_RealDetectSize.Length(); i++)
		{
			if (hv_RealDetectSize[i] > hv_StandardSize + hv_StandardDev || hv_RealDetectSize[i] < hv_StandardSize - hv_StandardDev)
			{
				tOutput.B[nSide].tDetectOutput.hv_Flag[0] = FALSE;
				ConcatObj(tOutput.B[nSide].tDetectOutput.ho_ErrorRegion, hoDomain, &tOutput.B[nSide].tDetectOutput.ho_ErrorRegion);
				TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorType, ErrorType, &tOutput.B[nSide].tDetectOutput.hv_ErrorType);
				TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip, i, &tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip);
				bSingleFlag = FALSE;
				if (bEnableLabel && bAllNGFlag == FALSE)//确认打标
				{
					bRet = FALSE;
					m_tFinalResult.hv_SizeErrorIndex.Append(ErrorType);
					m_tFinalResult.hv_SizeErrorBelongTrips.Append(i);
					m_tFinalResult.hv_SizeFlag = FALSE;
				}
			}
		}
		///如果是总长NG 间歇NG 膜长NG 对于间歇料来说 所有条都是NG
		if (bAllNGFlag == TRUE && bSingleFlag == FALSE)
		{
			//for (int i = 0; i < m_tStandardSize.hvTripNum.I(); i++)
			for (int i = 0; i < m_tStandardSize.hvTripNum.I(); i++)
			{
				tOutput.B[nSide].tDetectOutput.hv_Flag[0] = FALSE;
				ConcatObj(tOutput.B[nSide].tDetectOutput.ho_ErrorRegion, hoDomain, &tOutput.B[nSide].tDetectOutput.ho_ErrorRegion);
				TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorType, ErrorType, &tOutput.B[nSide].tDetectOutput.hv_ErrorType);
				TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip, i, &tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip);

				if (bEnableLabel)//确认打标
				{
					bRet = FALSE;
					m_tFinalResult.hv_SizeErrorIndex.Append(ErrorType);
					m_tFinalResult.hv_SizeErrorBelongTrips.Append(i);
					m_tFinalResult.hv_SizeFlag = FALSE;
				}
			}
		}
	}
}



void CJudge::CalCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, CAOWEISIZE& tCaoWeiStandardParam, int nSN, BOOL& bRet)
{



	//HTuple hv_Index;

	//HTuple hvSingleCaoweiHeight,hvSingleCaoweiWidth,hvSingleCaoweiMD,hvSingleCaoweiTD,hvSingleCaoWeiTDCuoWei,hvSingleCaoWeiMDCuoWei;

	//HTuple hvInverseTuple;
	//TupleInverse(tCaoWeiStandardParam.hvCaoWeiTDTuple,&hvInverseTuple);
	//for (int i=0;i<m_tStandardSize.hvTripNum;i++)
	//{

	//	TupleFind(tOutPut.B[nSN].tDetectOutput.hv_TripSNTuple,i,&hv_Index);
	//	if (hv_Index.Length()>0 && hv_Index!=-1)
	//	{
	//		hvSingleCaoweiHeight=tOutPut.B[nSN].tDetectOutput.hv_CaoWeiHeightTuple[i].D();
	//		hvSingleCaoweiWidth=tOutPut.B[nSN].tDetectOutput.hv_CaoWeiWidthTuple[i].D();
	//		hvSingleCaoweiMD =( MIN(tOutPut.B[0].tDetectOutput.hv_BotEdagePixel, tOutPut.B[1].tDetectOutput.hv_BotEdagePixel) - tOutPut.B[nSN].tDetectOutput.hv_CaoWeiYpositionTuple[i]) * g_vec_RowResolution[0];
	//		/*hvSingleCaoweiMD=tOutPut.B[nSN].tDetectOutput.hv_CaoWeiYpositionTuple[i].D()*g_vec_RowResolution[0];*/
	//		hvSingleCaoweiTD=(tOutPut.B[nSN].tDetectOutput.hv_RightEadgePixel.D()-tOutPut.B[nSN].tDetectOutput.hv_CaoWeiXPositionTuple[i].D())*g_vec_ColumnResolution[nSN];

	//		hvSingleCaoWeiMDCuoWei=tOutPut.B[0].tDetectOutput.hv_CaoWeiYpositionTuple[i].D()*g_vec_RowResolution[0]-tOutPut.B[1].tDetectOutput.hv_CaoWeiYpositionTuple[i].D()*g_vec_RowResolution[0];
	//	
	//		hvSingleCaoWeiTDCuoWei = (tOutPut.B[0].tDetectOutput.hv_RightEadgePixel.D() - tOutPut.B[0].tDetectOutput.hv_CaoWeiXPositionTuple[i].D())*g_vec_ColumnResolution[0]
	//			- (tOutPut.B[1].tDetectOutput.hv_RightEadgePixel.D() - tOutPut.B[1].tDetectOutput.hv_CaoWeiXPositionTuple[i].D())*g_vec_ColumnResolution[1];

	//		////高度
	//		ComPareSize(tCaoWeiStandardParam.hvCaoWeiHeight,tCaoWeiStandardParam.hvCaoWeiHeightDev,hvSingleCaoweiHeight,CaoWeiHNG,tOutPut,bRet,nSN,i);
	//		//宽度
	//		ComPareSize(tCaoWeiStandardParam.hvCaoWeiWidth,tCaoWeiStandardParam.hvCaoWeiWidthDev,hvSingleCaoweiWidth,CaoWeiWNG,tOutPut,bRet,nSN,i);
	//		///MD
	//		ComPareSize(tCaoWeiStandardParam.hvCaoWeiMD,tCaoWeiStandardParam.hvCaoWeiMDDev,hvSingleCaoweiMD,CaoWeiMDNG,tOutPut,bRet,nSN,i);
	//		////TD
	//		ComPareSize(hvInverseTuple[i],tCaoWeiStandardParam.hvCaoWeiTDDev,hvSingleCaoweiTD,CaoWeiTDNG,tOutPut,bRet,nSN,i);

	//		///MD错位
	//		ComPareSize(0,tCaoWeiStandardParam.hvCaoWeiMDCuoWeiDev,hvSingleCaoWeiMDCuoWei,CaoWeiMDCuoWeiNG,tOutPut,bRet,nSN,i);
	//		////TD错位

	//		ComPareSize(0,tCaoWeiStandardParam.hvCaoWeiTDCuoWeiDev,hvSingleCaoWeiTDCuoWei,CaoWeiTDCuoWeiNG,tOutPut,bRet,nSN,i);

	//	}


	//}


}

void CJudge::GetCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, BOOL& bRet)
{

	if (m_tStandardSize.hv_CaoWeiType != 0)
	{


		if (m_tStandardSize.hv_CaoWeiType == 1)//正面存在槽位
		{


			CalCaoWeiSizeResult(tOutPut, m_tStandardSize.tUpCaoWeiParam, 0, bRet);



		}
		else if (m_tStandardSize.hv_CaoWeiType == 2)//反面存在槽位
		{

			CalCaoWeiSizeResult(tOutPut, m_tStandardSize.tDownCaoWeiParam, 1, bRet);


		}
		else
		{

			CalCaoWeiSizeResult(tOutPut, m_tStandardSize.tUpCaoWeiParam, 0, bRet);
			CalCaoWeiSizeResult(tOutPut, m_tStandardSize.tDownCaoWeiParam, 1, bRet);

		}







	}

}

void CJudge::CalCaoWeiSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, STANDARDSIZE& tCoatAndFoilStandardParam, int nSN, BOOL& bRet)
{
	//HTuple hv_Index;

	//HTuple hvSingleCoatWidth, hvSingleFoilWidth;

	//HTuple hvInverseTuple;
	//TupleInverse(tCoatAndFoilStandardParam.dCoatWidth, &hvInverseTuple);
	////先判断涂布
	//int k = 0;
	//for (int i = 0; i <  MAX(1, m_tStandardSize.hvTripNum / 2); i++)
	//{
	//	TupleFind(tOutPut.B[nSN].tDetectOutput.hv_TripSNTuple, i, &hv_Index);
	//	if (hv_Index.Length() > 0 && hv_Index != -1)
	//	{
	//		hvSingleCoatWidth = tOutPut.B[nSN].tDetectOutput.hv_CoatWidth[i].D();
	//		hvSingleFoilWidth = tOutPut.B[nSN].tDetectOutput.hv_FoilWidth[i].D();
	//		//涂布宽度
	//		ComPareSize(tCoatAndFoilStandardParam.dCoatWidth, tCoatAndFoilStandardParam.dCoatWidthDev, hvSingleCoatWidth, CoatWidthSize, tOutPut, bRet, nSN, i);
	//		//极耳宽度
	//		ComPareSize(tCoatAndFoilStandardParam.dFoilWidth, tCoatAndFoilStandardParam.dFoilWidthDev, hvSingleFoilWidth, FoilWidthSize, tOutPut, bRet, nSN, i);
	//	}
	//	k++;
	//}
	//if (k== m_tStandardSize.hvTripNum / 2)
	//{
	//	ComPareSize(tCoatAndFoilStandardParam.dFoilWidth, tCoatAndFoilStandardParam.dFoilWidthDev, hvSingleFoilWidth, FoilWidthSize, tOutPut, bRet, nSN, k+1);
	//}
}

void CJudge::GetCoatAndFoiliSizeResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut, BOOL& bRet)
{
	if (m_tStandardSize.hv_CoatType == M_TwoCam_Continue)
	{
		CalCaoWeiSizeResult(tOutPut, m_tStandardSize, 0, bRet);
	}
}

void CJudge::CalculateCPK(int& nTrip, HTuple hv_RealValue, double dStandarValueValue, double dStandarDeviationValue, HTuple* hv_MeanValue, HTuple* hv_DeviationValue, HTuple* hv_CpkValue)
{

	try
	{
		nTrip = 0;
		static HTuple S_hvCPKData[2];
		HTuple hv_TempIndex, hv_EffectiveIndex, hv_EffectiveData;
		HTuple hv_Mean, hv_Deviation, hv_PerVaule, hv_CuoWeiMax, hv_FinalCpk, hv_SelectData, hv_Min, hv_MinIndex, hv_Max, hv_MaxIndex, hv_FinalIndex;
		hv_Min = dStandarValueValue - dStandarDeviationValue;//公差上下限
		hv_Max = dStandarValueValue + dStandarDeviationValue;
		static double  s_dLastCpk = 0;

		m_hvec_CPK[m_hvec_CPK.Length()].T() = hv_RealValue;
		if (m_hvec_CPK.Length() >= 20)
		{
			for (int i = 0; i < m_hvec_CPK[0].T().Length(); i++)
			{
				for (int j = 0; j < m_hvec_CPK.Length(); j++)
				{
					hv_PerVaule.Append(m_hvec_CPK[j].T()[i]);
				}
				TupleGreaterEqualElem(hv_PerVaule, hv_Min, &hv_MinIndex);
				TupleGreaterEqualElem(hv_Max, hv_PerVaule, &hv_MaxIndex);
				TupleAnd(hv_MinIndex, hv_MaxIndex, &hv_FinalIndex);
				TupleFind(hv_FinalIndex, 1, &hv_FinalIndex);
				if (hv_FinalIndex.Length() > 0 && hv_FinalIndex[0] != -1)
				{
					HTuple hv_Value, hv_SelectValue;
					for (int k = 0; k < hv_FinalIndex.Length(); k++)
					{
						hv_Value = hv_FinalIndex[k];
						hv_SelectValue.Append(hv_PerVaule[hv_Value]);
					}
					hv_PerVaule = hv_SelectValue;
				}

				TupleMean(hv_PerVaule, &hv_Mean);
				TupleDeviation(hv_PerVaule, &hv_Deviation);
				{
					double dUp = dStandarValueValue + dStandarDeviationValue;
					double dDown = dStandarValueValue - dStandarDeviationValue;
					HTuple hv_CalCpk = MIN(dUp - hv_Mean.D(), hv_Mean.D() - dDown) / (3 * MAX(0.001, hv_Deviation.D()));
					hv_FinalCpk.Append(hv_CalCpk);
				}
				hv_PerVaule.Clear();
			}
			m_hvCPK.Append(hv_FinalCpk);
			m_hvec_CPK.Clear();
		}

		/*if (hv_RealValue.D()<dStandarValueValue+dStandarDeviationValue && hv_RealValue.D()>dStandarValueValue-dStandarDeviationValue)
		{
			S_hvCPKData[nTrip].Append(hv_RealValue);
		}

		if (S_hvCPKData[nTrip].Length()>20)
		{
			TupleSortIndex(S_hvCPKData[nTrip],&hv_TempIndex);
			TupleSelectRange(hv_TempIndex,2,S_hvCPKData[nTrip].Length()-2,&hv_EffectiveIndex);
			TupleSelect(S_hvCPKData[nTrip],hv_EffectiveIndex,&hv_EffectiveData);
			TupleMean(hv_EffectiveData,&hv_Mean);
			TupleDeviation(hv_EffectiveData,&hv_Deviation);
			TupleMean(hv_EffectiveData,&(*hv_MeanValue));
			double dUp= dStandarValueValue + dStandarDeviationValue;
			double dDown= dStandarValueValue - dStandarDeviationValue;
			*hv_CpkValue=MIN(dUp-hv_Mean.D(),hv_Mean.D()-dDown)/(6*MAX(0.001,hv_Deviation.D()));
			s_dLastCpk = *hv_CpkValue;
			*hv_DeviationValue=hv_Deviation;
			S_hvCPKData[nTrip].Clear();
		}else
		{
			*hv_DeviationValue=0;
			*hv_CpkValue= s_dLastCpk;
			* hv_MeanValue=0;
		}*/
	}
	catch (HException& e)
	{



	}



}





void CJudge::Action(C_ALLOCATORWORKSHOPOUTPUT& tOutPut)
{
	HTuple hv_AllErrorType, hv_AllErrorBelongTrips;
	HObject ho_AllErrorRegion;

	HTuple  hv_UpErrorType, hv_DownErrorType, hv_UpErrorBelongTrips, hv_DownErrorBelongTrips;	//按照裁切位置进行裁切
	HObject ho_UpErrorRegion, ho_DownErrorRegion;
	int nSide = 0;//
	///汇总的缺陷信息
	ho_AllErrorRegion.GenEmptyObj();
	hv_AllErrorType.Clear();
	hv_AllErrorBelongTrips.Clear();
	ConcatObj(tOutPut.B[0].tDetectOutput.ho_ErrorRegion, tOutPut.B[1].tDetectOutput.ho_ErrorRegion, &ho_AllErrorRegion);//前后相机所有的缺陷区域
	TupleConcat(tOutPut.B[0].tDetectOutput.hv_ErrorType, tOutPut.B[1].tDetectOutput.hv_ErrorType, &hv_AllErrorType);//前后相机所有的缺陷
	TupleConcat(tOutPut.B[0].tDetectOutput.hv_ErrorBelongStrip, tOutPut.B[1].tDetectOutput.hv_ErrorBelongStrip, &hv_AllErrorBelongTrips);//前后相机所有缺陷隶属于条数
	g_tStoreInfo.dFirstWidth = tOutPut.B[0].tDetectOutput.hv_CoatWidth;
	g_tStoreInfo.dSecondWidth = tOutPut.B[1].tDetectOutput.hv_CoatWidth;
	g_tStoreInfo.dSumLength = tOutPut.B[0].tDetectOutput.hv_SumLength;
	///预留裁切位置的缺陷信息
	GenEmptyObj(&ho_UpErrorRegion);
	GenEmptyObj(&ho_DownErrorRegion);
	hv_UpErrorType.Clear();
	hv_DownErrorType.Clear();
	hv_UpErrorBelongTrips.Clear();
	hv_DownErrorBelongTrips.Clear();
	try
	{
		BOOL bRet = TRUE;//单张尺寸NG判定
		HObject hoDomain;
		HTuple hv_Width, hv_Height;
		int nSide = 0;//默认判断前相机
		GetImageSize(tOutPut.B[nSide].A.hoGrabImage, &hv_Width, &hv_Height);
		tOutPut.hvRealCPKValue = 0;
		tOutPut.tFinalResult.hv_TotalLength = hv_Height*g_vec_RowResolution[0];
		tOutPut.tFinalResult.hv_SizeFlag = TRUE;
		tOutPut.tFinalResult.hv_SizeErrorIndex.Clear();
		tOutPut.tFinalResult.hv_SizeErrorBelongTrips.Clear();
		tOutPut.tFinalResult.hv_SurfaceFlag = TRUE;
		tOutPut.tFinalResult.hv_SurfaceErrorIndex.Clear();
		tOutPut.tFinalResult.hv_SurfaceErrorBelongTrips.Clear();

		//////尺寸控制贴标
		{
			SegErrorRegion(tOutPut.B[0].A.hoGrabImage, tOutPut.B[0].tDetectOutput.hv_JuanRaoMarkRow, ho_AllErrorRegion, hv_AllErrorType, hv_AllErrorBelongTrips, &ho_UpErrorRegion, &ho_DownErrorRegion, &hv_UpErrorType, &hv_DownErrorType, &hv_UpErrorBelongTrips, &hv_DownErrorBelongTrips);
			ConcatErrorRegion(ho_UpErrorRegion, hv_UpErrorType, hv_UpErrorBelongTrips);
			tOutPut.tFinalResult.hv_SurfaceFlag = GetSurfaceResult(tOutPut);//获取结果
			tOutPut.tFinalResult.hv_SurfaceErrorIndex = m_tFinalResult.hv_SurfaceErrorIndex;///绑定表面错误名称
			tOutPut.tFinalResult.hv_SurfaceErrorBelongTrips = m_tFinalResult.hv_SurfaceErrorBelongTrips;//绑定错误类型


			if (m_tStandardSize.hv_CoatType == M_TwoCam_Continue)///连续涂布尺寸
			{
				HTuple hvCuoweiTuple;
				if (tOutPut.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length() == tOutPut.B[1].tDetectOutput.hv_AT9EdageToProEdage.Length())
				{
					//CPK计算
					TupleSub(tOutPut.B[0].tDetectOutput.hv_AT9EdageToProEdage, tOutPut.B[1].tDetectOutput.hv_AT9EdageToProEdage, &hvCuoweiTuple);
					HTuple hv_Mean, hvDeviation, hvCPK;
					//对CPK排序，涂布在前
					HTuple hv_CoatCuoWei, hv_CoatCpkIndex, hv_At9CuoWei, hvCPKCuoweiTuple;
					for (int i = 0; i < hvCuoweiTuple.Length() / 4; i++)
					{
						hv_CoatCuoWei.Append(hvCuoweiTuple[i + 1 + i * 3]);
						hv_CoatCuoWei.Append(hvCuoweiTuple[i + 2 + i * 3]);
					}
					//先加入涂布错位
					hvCPKCuoweiTuple.Append(hv_CoatCuoWei);
					TupleFind(hvCuoweiTuple, hv_CoatCuoWei, &hv_CoatCpkIndex);
					TupleRemove(hvCuoweiTuple, hv_CoatCpkIndex, &hv_At9CuoWei);
					//加入AT9错位
					hvCPKCuoweiTuple.Append(hv_At9CuoWei);
					m_hvPerATCuoWei = hvCPKCuoweiTuple;
					int nType = 0;
					CalculateCPK(nType, hvCPKCuoweiTuple, 0, m_tStandardSize.dCuoWeiMax, &hv_Mean, &hvDeviation, &hvCPK);
				}
			}
			else//间歇涂布尺寸
			{
				tOutPut.tFinalResult.hv_TotalLength = tOutPut.B[0].tDetectOutput.hv_SumLength;
				//计算大小错位值
				if (m_tStandardSize.hvQiTouXianChu.I())
				{
					//齐头先出，头部错位(小错位)为涂布上边缘的错位，尾部错位(大错位)为涂布下边缘错位
					tOutPut.B[0].tDetectOutput.hv_HeadCuoWei = (tOutPut.B[0].tDetectOutput.hv_TopEdagePixel - tOutPut.B[1].tDetectOutput.hv_TopEdagePixel)*g_vec_RowResolution[0];
					tOutPut.B[0].tDetectOutput.hv_TailCuoWei = (tOutPut.B[0].tDetectOutput.hv_BotEdagePixel - tOutPut.B[1].tDetectOutput.hv_BotEdagePixel).TupleAbs()*g_vec_RowResolution[0];
					tOutPut.B[1].tDetectOutput.hv_HeadCuoWei = tOutPut.B[0].tDetectOutput.hv_HeadCuoWei;
					tOutPut.B[1].tDetectOutput.hv_TailCuoWei = tOutPut.B[0].tDetectOutput.hv_TailCuoWei;
				}
				else
				{
					//不是齐头先出，头部错位(小错位)为涂布下边缘的错位，尾部错位(大错位)为涂布上边缘错位
					tOutPut.B[0].tDetectOutput.hv_HeadCuoWei = (tOutPut.B[0].tDetectOutput.hv_BotEdagePixel - tOutPut.B[1].tDetectOutput.hv_BotEdagePixel)*g_vec_RowResolution[0];
					tOutPut.B[0].tDetectOutput.hv_TailCuoWei = (tOutPut.B[0].tDetectOutput.hv_TopEdagePixel - tOutPut.B[1].tDetectOutput.hv_TopEdagePixel).TupleAbs()*g_vec_RowResolution[0];
					tOutPut.B[1].tDetectOutput.hv_HeadCuoWei = tOutPut.B[0].tDetectOutput.hv_HeadCuoWei;
					tOutPut.B[1].tDetectOutput.hv_TailCuoWei = tOutPut.B[0].tDetectOutput.hv_TailCuoWei;
				}
				JustWarnOrLabel(m_tStandardSize.bEnableUpImHeight, m_tStandardSize.bEnableUpIntermissionLabel, m_tStandardSize.dUpIntermissionHeight, m_tStandardSize.dUpIntermissionHeightDev, tOutPut.B[1].tDetectOutput.hv_JXHeight, UpJXSize, tOutPut, bRet, 1, TRUE);//间歇尺寸
				JustWarnOrLabel(m_tStandardSize.bEnableDownImHeight, m_tStandardSize.bEnableDownIntermissionLabel, m_tStandardSize.dDownIntermissionHeight, m_tStandardSize.dDownIntermissionHeightDev, tOutPut.B[0].tDetectOutput.hv_JXHeight, DownJXSize, tOutPut, bRet, 0, TRUE);//间歇尺寸	

				JustWarnOrLabel(m_tStandardSize.bEnableBHJXTotalLength, m_tStandardSize.bEnableBHJXLabel, m_tStandardSize.dBHJXTotalLength, m_tStandardSize.dBHJXTotalLengthDev, tOutPut.B[1].tDetectOutput.hv_MPLength, BHXJUpSumSize, tOutPut, bRet, 1, TRUE);//膜长
				JustWarnOrLabel(m_tStandardSize.bEnableBHJXLength2, m_tStandardSize.bBHJXLabel2, m_tStandardSize.dBHJXLength2, m_tStandardSize.dBHJXLengthDev2, tOutPut.B[0].tDetectOutput.hv_MPLength, BHXJDownSumSize, tOutPut, bRet, 0, TRUE);//膜长

				JustWarnOrLabel(m_tStandardSize.bEnableTotalWidth, m_tStandardSize.bToatlWidthLabel, m_tStandardSize.dTotalCoatWidth, m_tStandardSize.dTotalCoatWidthDev, tOutPut.B[0].tDetectOutput.hv_CoatWidth, C_WSize, tOutPut, bRet, 0, TRUE);//膜长
				JustWarnOrLabel(m_tStandardSize.bEnableTotalWidth, m_tStandardSize.bToatlWidthLabel, m_tStandardSize.dTotalCoatWidth, m_tStandardSize.dTotalCoatWidthDev, tOutPut.B[1].tDetectOutput.hv_CoatWidth, C_WSize, tOutPut, bRet, 1, TRUE);//膜长
				JustWarnOrLabel(m_tStandardSize.bEnableCoatWidth, m_tStandardSize.bEnableCoatWidthLabel, m_tStandardSize.dCoatWidth, m_tStandardSize.dCoatWidthDev, tOutPut.B[0].tDetectOutput.hv_CoatWidth / MAX(1, m_tStandardSize.hvTripNum.I()), C_SingleWidth, tOutPut, bRet, 0, TRUE);//膜长

				//头部错位,尾部错位
				JustWarnOrLabel(m_tStandardSize.bEnableHeadCuoWei, m_tStandardSize.bHeadCuoWeiLabel, m_tStandardSize.dHeadCuoWei, m_tStandardSize.dHeadCuoWeiDev, tOutPut.B[0].tDetectOutput.hv_HeadCuoWei, HeadCuoWei, tOutPut, bRet, 0, TRUE);
				JustWarnOrLabel(m_tStandardSize.bEnableTailCuoWei, m_tStandardSize.bTailCuoWeiLabel, m_tStandardSize.dTailCuoWei, m_tStandardSize.dTailCuoWeiDev, tOutPut.B[0].tDetectOutput.hv_TailCuoWei, TailCuoWei, tOutPut, bRet, 0, TRUE);
				JustWarnOrLabel(m_tStandardSize.bEnableHeadCuoWei, m_tStandardSize.bHeadCuoWeiLabel, m_tStandardSize.dHeadCuoWei, m_tStandardSize.dHeadCuoWeiDev, tOutPut.B[1].tDetectOutput.hv_HeadCuoWei, HeadCuoWei, tOutPut, bRet, 1, TRUE);
				JustWarnOrLabel(m_tStandardSize.bEnableTailCuoWei, m_tStandardSize.bTailCuoWeiLabel, m_tStandardSize.dTailCuoWei, m_tStandardSize.dTailCuoWeiDev, tOutPut.B[1].tDetectOutput.hv_TailCuoWei, TailCuoWei, tOutPut, bRet, 1, TRUE);
			}
			tOutPut.tFinalResult.hv_SizeFlag = bRet;//获取结果
			tOutPut.tFinalResult.hv_SizeErrorIndex = m_tFinalResult.hv_SizeErrorIndex;///绑定表面错误名称
			tOutPut.tFinalResult.hv_SizeErrorBelongTrips = m_tFinalResult.hv_SizeErrorBelongTrips;//绑定错误隶属条数
			///清空了
			PreClear();
			ConcatErrorRegion(ho_DownErrorRegion, hv_DownErrorType, hv_DownErrorBelongTrips);
			tOutPut.bIsFinalResult = TRUE;
			tOutPut.tFinalResult.hv_FoilNum = 1;
		}
		tOutPut.hvPackageSN = 1;
	}
	catch (HException&e)
	{
		CString strMessage;
		strMessage.Format("%s%s:%s", "双侧极耳CJudge::Action:", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
	}
}




void CJudge::ReadParam()
{
	////新增加自动换型***["XC",模切总宽，总宽公差，定位不齐 定位不齐公差 总长 总长公差 极耳间距0，极耳间距1 极耳间距2,"XC"]//
	m_dMinDist = GetPrivateProfileDouble("SizeParam", "m_dMinDist", 50, m_strConfigFile);
	TupleGenConst(g_Count.nType, 0, &m_hvErrorTypeCount);

	m_tStandardSize.hvTripNum = GetPrivateProfileIntH("SizeParam", "hvTripNum", 2, m_strConfigFile);
	m_tStandardSize.hv_ProductType = GetPrivateProfileIntH("SizeParam", "hv_ProductType", 0, m_strConfigFile);
	m_tStandardSize.hv_CoatType = GetPrivateProfileIntH("SizeParam", "hv_CoatType", 1, m_strConfigFile);
	m_tStandardSize.hv_OAPType = GetPrivateProfileIntH("SizeParam", "hv_OAPType", 0, m_strConfigFile);
	m_tStandardSize.hv_CaoWeiType = GetPrivateProfileIntH("SizeParam", "hv_CaoWeiType", 0, m_strConfigFile);
	m_tStandardSize.hvExistJXType = GetPrivateProfileIntH("SizeParam", "hvExistJXType", 0, m_strConfigFile);
	m_tStandardSize.hvGongYiType = GetPrivateProfileIntH("SizeParam", "hvGongYiType", 1, m_strConfigFile);
	m_tStandardSize.hvQiTouXianChu = GetPrivateProfileIntH("SizeParam", "hvQiTouXianChu", 1, m_strConfigFile);
	m_tStandardSize.hvChangMoZaiShang = GetPrivateProfileIntH("SizeParam", "hvChangMoZaiShang", 1, m_strConfigFile);

	m_tStandardSize.hvZhiDaiGray = GetPrivateProfileIntH("SizeParam", "hvZhiDaiGray", 100, m_strConfigFile);



	//正面的
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiHeight = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiHeight", 5, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiHeightDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiHeightDev", 1, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiWidth = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiWidth", 5, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiWidthDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiWidthDev", 1, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMD = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMD", 5, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMDDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMDDev", 1.5, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiTDDev", 1.5, m_strConfigFile);

	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMDCuoWeiDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMDCuoWeiDev", 1.5, m_strConfigFile);
	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDCuoWeiDev = GetPrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiTDCuoWeiDev", 1.5, m_strConfigFile);

	m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDTuple.Clear();
	for (int i = 0; i < m_tStandardSize.hvTripNum; i++)
	{
		CString strValue;
		strValue.Format("hvCaoWeiTD%d", i);
		m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDTuple.Append(GetPrivateProfileDouble("tUpCaoWeiParam", strValue, 1, m_strConfigFile));

	}



	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiHeight = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiHeight", 5, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiHeightDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiHeightDev", 1, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiWidth = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiWidth", 5, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiWidthDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiWidthDev", 1, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMD = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMD", 5, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMDDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMDDev", 1.5, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiTDDev", 1.5, m_strConfigFile);

	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMDCuoWeiDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMDCuoWeiDev", 1.5, m_strConfigFile);
	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDCuoWeiDev = GetPrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiTDCuoWeiDev", 1.5, m_strConfigFile);

	m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDTuple.Clear();
	for (int i = 0; i < m_tStandardSize.hvTripNum; i++)
	{
		CString strValue;
		strValue.Format("hvCaoWeiTD%d", i);
		m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDTuple.Append(GetPrivateProfileDouble("tDownCaoWeiParam", strValue, 1, m_strConfigFile));

	}





	m_tStandardSize.bEnableTotalLength = GetPrivateProfileIntH("SizeParam", "bEnableTotalLength", 1, m_strConfigFile);
	m_tStandardSize.dTotalLength = GetPrivateProfileDouble("SizeParam", "dTotalLength", 100, m_strConfigFile);
	m_tStandardSize.dTotalLengthDev = GetPrivateProfileDouble("SizeParam", "dTotalLengthDev", 5, m_strConfigFile);
	m_tStandardSize.bEnableTotalLengthLabel = GetPrivateProfileIntH("SizeParam", "bEnableTotalLengthLabel", 1, m_strConfigFile);//1

	m_tStandardSize.bEnableCoatHeight = GetPrivateProfileIntH("SizeParam", "bEnableCoatHeight", 1, m_strConfigFile);
	m_tStandardSize.nCellNumber = GetPrivateProfileIntH("SizeParam", "nCellNumber", 10, m_strConfigFile);
	for (int i = 0; i < m_tStandardSize.nCellNumber; i++)
	{
		CString strAppName;
		strAppName.Format("nCellNumber%d", i);
		m_tStandardSize.hv_CoatHeight.Append(GetPrivateProfileDouble(strAppName, "hv_CoatHeight", 0, m_strConfigFile));
	}
	m_tStandardSize.dCoatHeightDev = GetPrivateProfileDouble("SizeParam", "dCoatHeightDev", 5, m_strConfigFile);
	m_tStandardSize.hvCoatHeightNGNum = GetPrivateProfileIntH("SizeParam", "hvCoatHeightNGNum", 5, m_strConfigFile);

	////宽度
	m_tStandardSize.bEnableCoatWidth = GetPrivateProfileIntH("SizeParam", "bEnableCoatWidth", 1, m_strConfigFile);
	m_tStandardSize.dCoatWidth = GetPrivateProfileDouble("SizeParam", "dCoatWidth", 20, m_strConfigFile);
	m_tStandardSize.dCoatWidthDev = GetPrivateProfileDouble("SizeParam", "dCoatWidthDev", 0.5, m_strConfigFile);
	m_tStandardSize.hvCoatWidthNGNum = GetPrivateProfileIntH("SizeParam", "hvCoatWidthNGNum", 5, m_strConfigFile);
	m_tStandardSize.bEnableCoatWidthLabel = GetPrivateProfileIntH("SizeParam", "bEnableCoatWidthLabel", 1, m_strConfigFile);//2
	m_tStandardSize.dCoatWidthAddValue = GetPrivateProfileDouble("SizeParam", "dCoatWidthAddValue", 0, m_strConfigFile);

	///涂膜上极耳
	m_tStandardSize.bEnableLocateMissing = GetPrivateProfileIntH("SizeParam", "bEnableLocateMissing", 0, m_strConfigFile);
	m_tStandardSize.dLocateMissing = GetPrivateProfileDouble("SizeParam", "dLocateMissing", 1, m_strConfigFile);
	m_tStandardSize.dLocateMissingDev = GetPrivateProfileDouble("SizeParam", "dLocateMissingDev", 0.5, m_strConfigFile);
	m_tStandardSize.hvLocateMissingNGNum = GetPrivateProfileIntH("SizeParam", "hvLocateMissingNGNum", 5, m_strConfigFile);
	m_tStandardSize.bEnableLocateMissingLabel = GetPrivateProfileIntH("SizeParam", "bEnableLocateMissingLabel", 1, m_strConfigFile);//3


	//极耳宽度
	m_tStandardSize.bEnableFoilWidth = GetPrivateProfileIntH("SizeParam", "bEnableFoilWidth", 0, m_strConfigFile);
	m_tStandardSize.dFoilWidth = GetPrivateProfileDouble("SizeParam", "dFoilWidth", 20, m_strConfigFile);
	m_tStandardSize.dFoilWidthDev = GetPrivateProfileDouble("SizeParam", "dFoilWidthDev", 0.5, m_strConfigFile);









	m_tStandardSize.bEnableBHJXTotalLength = GetPrivateProfileIntH("SizeParam", "bEnableBHJXTotalLength", 1, m_strConfigFile);
	m_tStandardSize.bEnableBHJXLabel = GetPrivateProfileIntH("SizeParam", "bEnableBHJXLabel", 0, m_strConfigFile);
	m_tStandardSize.dBHJXTotalLength = GetPrivateProfileDouble("SizeParam", "dBHJXTotalLength", 20, m_strConfigFile);
	m_tStandardSize.dBHJXTotalLengthDev = GetPrivateProfileDouble("SizeParam", "dBHJXTotalLengthDev", 3, m_strConfigFile);





	m_tStandardSize.bEnableUpImHeight = GetPrivateProfileIntH("SizeParam", "bEnableUpImHeight", 1, m_strConfigFile);
	m_tStandardSize.bEnableUpIntermissionLabel = GetPrivateProfileIntH("SizeParam", "bEnableUpIntermissionLabel", 0, m_strConfigFile);
	m_tStandardSize.dUpIntermissionHeight = GetPrivateProfileDouble("SizeParam", "dUpIntermissionHeight", 20, m_strConfigFile);
	m_tStandardSize.dUpIntermissionHeightDev = GetPrivateProfileDouble("SizeParam", "dUpIntermissionHeightDev", 2, m_strConfigFile);


	m_tStandardSize.bEnableDownImHeight = GetPrivateProfileIntH("SizeParam", "bEnableDownImHeight", 1, m_strConfigFile);
	m_tStandardSize.bEnableDownIntermissionLabel = GetPrivateProfileIntH("SizeParam", "bEnableDownIntermissionLabel", 0, m_strConfigFile);
	m_tStandardSize.dDownIntermissionHeight = GetPrivateProfileDouble("SizeParam", "dDownIntermissionHeight", 20, m_strConfigFile);
	m_tStandardSize.dDownIntermissionHeightDev = GetPrivateProfileDouble("SizeParam", "dDownIntermissionHeightDev", 2, m_strConfigFile);




	///总宽////新加
	m_tStandardSize.bEnableTotalWidth = GetPrivateProfileIntH("SizeParam", "bEnableTotalWidth", 1, m_strConfigFile);
	m_tStandardSize.bToatlWidthLabel = GetPrivateProfileIntH("SizeParam", "bToatlWidthLabel", 0, m_strConfigFile);
	m_tStandardSize.dTotalCoatWidth = GetPrivateProfileDouble("SizeParam", "dTotalCoatWidth", 200, m_strConfigFile);
	m_tStandardSize.dTotalCoatWidthDev = GetPrivateProfileDouble("SizeParam", "dTotalCoatWidthDev", 2, m_strConfigFile);

	////后相机总模长
	m_tStandardSize.bEnableBHJXLength2 = GetPrivateProfileIntH("SizeParam", "bEnableBHJXLength2", 1, m_strConfigFile);
	m_tStandardSize.bBHJXLabel2 = GetPrivateProfileIntH("SizeParam", "bBHJXLabel2", 0, m_strConfigFile);
	m_tStandardSize.dBHJXLength2 = GetPrivateProfileDouble("SizeParam", "dBHJXLength2", 200, m_strConfigFile);
	m_tStandardSize.dBHJXLengthDev2 = GetPrivateProfileDouble("SizeParam", "dBHJXLengthDev2", 2, m_strConfigFile);


	///头部错位
	m_tStandardSize.bEnableHeadCuoWei = GetPrivateProfileIntH("SizeParam", "bEnableHeadCuoWei", 1, m_strConfigFile);
	m_tStandardSize.bHeadCuoWeiLabel = GetPrivateProfileIntH("SizeParam", "bHeadCuoWeiLabel", 0, m_strConfigFile);
	m_tStandardSize.dHeadCuoWei = GetPrivateProfileDouble("SizeParam", "dHeadCuoWei", 200, m_strConfigFile);
	m_tStandardSize.dHeadCuoWeiDev = GetPrivateProfileDouble("SizeParam", "dHeadCuoWeiDev", 2, m_strConfigFile);


	///尾部错位
	m_tStandardSize.bEnableTailCuoWei = GetPrivateProfileIntH("SizeParam", "bEnableTailCuoWei", 1, m_strConfigFile);
	m_tStandardSize.bTailCuoWeiLabel = GetPrivateProfileIntH("SizeParam", "bTailCuoWeiLabel", 0, m_strConfigFile);
	m_tStandardSize.dTailCuoWei = GetPrivateProfileDouble("SizeParam", "dTailCuoWei", 200, m_strConfigFile);
	m_tStandardSize.dTailCuoWeiDev = GetPrivateProfileDouble("SizeParam", "dTailCuoWeiDev", 2, m_strConfigFile);


	///////

	////AB错位值
	m_tStandardSize.bEnableABMissValue = GetPrivateProfileIntH("SizeParam", "bEnableABMissValue", 1, m_strConfigFile);
	m_tStandardSize.bEnableABMissValueLabel = GetPrivateProfileIntH("SizeParam", "bEnableABMissValueLabel", 0, m_strConfigFile);
	m_tStandardSize.dABMissValue = GetPrivateProfileDouble("SizeParam", "dABMissValue", 20, m_strConfigFile);
	m_tStandardSize.dABMissValueDev = GetPrivateProfileDouble("SizeParam", "dABMissValueDev", 5, m_strConfigFile);

	m_tStandardSize.nErrorNum = GetPrivateProfileIntH("SizeParam", "nErrorNum", 1, m_strConfigFile);
	m_tStandardSize.dCorrectMin = GetPrivateProfileDouble("SizeParam", "dCorrectMin", 1, m_strConfigFile);
	m_tStandardSize.dCorrectMax = GetPrivateProfileDouble("SizeParam", "dCorrectMax", 5, m_strConfigFile);



	///AT9正反面差值报警参数
	m_tStandardSize.bEnableCuoWei = GetPrivateProfileIntH("SizeParam", "bEnableCuoWei", 0, m_strConfigFile);
	m_tStandardSize.dCuoWeiMax = GetPrivateProfileDouble("SizeParam", "dCuoWeiMax", 0.5, m_strConfigFile);

	///分EA参数
	m_tStandardSize.hvCutPosition = GetPrivateProfileDouble("SizeParam", "hvCutPosition", 10, m_strConfigFile);



	//瑕疵二次判定
	m_tStandardSurface.nErrorCount = GetPrivateProfileIntH("SurfaceParam", "nErrorCount", 0, m_strConfigFile);
	m_tStandardSurface.nSecErrorCount = GetPrivateProfileIntH("SurfaceParam", "nSecErrorCount", 0, m_strConfigFile);
	m_tStandardSurface.hv_ErrorIndex.Clear();
	m_tStandardSurface.hv_TotalMaxNumber.Clear();
	m_tStandardSurface.hv_TotalMaxArea.Clear();
	m_tStandardSurface.hv_TotalMaxNumberTwo.Clear();
	m_tStandardSurface.hv_TotalMaxAreaTwo.Clear();
	m_tStandardSurface.hv_SingleMaxWidth.Clear();
	m_tStandardSurface.hv_EnablePopWindow.Clear();
	for (int i = 0; i < m_tStandardSurface.nSecErrorCount; i++)
	{
		CString strAppName;
		strAppName.Format("nError%d", i);
		m_tStandardSurface.hv_ErrorIndex.Append(GetPrivateProfileIntH(strAppName, "hv_ErrorIndex", 0, m_strConfigFile));
		m_tStandardSurface.hv_TotalMaxNumber.Append(GetPrivateProfileIntH(strAppName, "hv_TotalMaxNumber", 1, m_strConfigFile));
		m_tStandardSurface.hv_TotalMaxArea.Append(GetPrivateProfileDouble(strAppName, "hv_TotalMaxArea", 5.0, m_strConfigFile));
		m_tStandardSurface.hv_TotalMaxNumberTwo.Append(GetPrivateProfileIntH(strAppName, "hv_TotalMaxNumberTwo", 1, m_strConfigFile));
		m_tStandardSurface.hv_TotalMaxAreaTwo.Append(GetPrivateProfileDouble(strAppName, "hv_TotalMaxAreaTwo", 5.0, m_strConfigFile));
		m_tStandardSurface.hv_SingleMaxWidth.Append(GetPrivateProfileDouble(strAppName, "hv_SingleMaxWidth", 0, m_strConfigFile));
		m_tStandardSurface.hv_EnablePopWindow.Append(GetPrivateProfileIntH(strAppName, "hv_EnablePopWindow", 0, m_strConfigFile));
	}

}
void CJudge::WriteParam()
{


	WritePrivateProfileInt("SizeParam", "bEnableTotalLength", m_tStandardSize.bEnableTotalLength, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvTripNum", m_tStandardSize.hvTripNum, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hv_ProductType", m_tStandardSize.hv_ProductType, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hv_CoatType", m_tStandardSize.hv_CoatType, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hv_OAPType", m_tStandardSize.hv_OAPType, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hv_CaoWeiType", m_tStandardSize.hv_CaoWeiType, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvExistJXType", m_tStandardSize.hvExistJXType, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvGongYiType", m_tStandardSize.hvGongYiType, m_strConfigFile);

	WritePrivateProfileInt("SizeParam", "hvQiTouXianChu", m_tStandardSize.hvQiTouXianChu, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvChangMoZaiShang", m_tStandardSize.hvChangMoZaiShang, m_strConfigFile);

	WritePrivateProfileInt("SizeParam", "hvZhiDaiGray", m_tStandardSize.hvZhiDaiGray, m_strConfigFile);



	//正面的
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiHeight", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiHeight, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiHeightDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiHeightDev, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiWidth", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiWidth, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiWidthDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiWidthDev, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMD", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMD, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMDDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMDDev, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiTDDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDDev, m_strConfigFile);


	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiMDCuoWeiDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiMDCuoWeiDev, m_strConfigFile);
	WritePrivateProfileDouble("tUpCaoWeiParam", "hvCaoWeiTDCuoWeiDev", m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDCuoWeiDev, m_strConfigFile);

	for (int i = 0; i < m_tStandardSize.hvTripNum; i++)
	{
		CString strValue;
		strValue.Format("hvCaoWeiTD%d", i);
		WritePrivateProfileDouble("tUpCaoWeiParam", strValue, m_tStandardSize.tUpCaoWeiParam.hvCaoWeiTDTuple[i], m_strConfigFile);

	}

	///反面的槽位
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiHeight", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiHeight, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiHeightDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiHeightDev, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiWidth", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiWidth, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiWidthDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiWidthDev, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMD", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMD, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMDDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMDDev, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiTDDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDDev, m_strConfigFile);

	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiMDCuoWeiDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiMDCuoWeiDev, m_strConfigFile);
	WritePrivateProfileDouble("tDownCaoWeiParam", "hvCaoWeiTDCuoWeiDev", m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDCuoWeiDev, m_strConfigFile);


	for (int i = 0; i < m_tStandardSize.hvTripNum; i++)
	{
		CString strValue;
		strValue.Format("hvCaoWeiTD%d", i);
		WritePrivateProfileDouble("tDownCaoWeiParam", strValue, m_tStandardSize.tDownCaoWeiParam.hvCaoWeiTDTuple[i], m_strConfigFile);

	}






	WritePrivateProfileInt("SizeParam", "bEnableBHJXTotalLength", m_tStandardSize.bEnableBHJXTotalLength, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dBHJXTotalLength", m_tStandardSize.dBHJXTotalLength, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dBHJXTotalLengthDev", m_tStandardSize.dBHJXTotalLengthDev, m_strConfigFile);







	WritePrivateProfileInt("SizeParam", "bEnableUpImHeight", m_tStandardSize.bEnableUpImHeight, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dUpIntermissionHeight", m_tStandardSize.dUpIntermissionHeight, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dUpIntermissionHeightDev", m_tStandardSize.dUpIntermissionHeightDev, m_strConfigFile);


	WritePrivateProfileInt("SizeParam", "bEnableDownImHeight", m_tStandardSize.bEnableDownImHeight, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dDownIntermissionHeight", m_tStandardSize.dDownIntermissionHeight, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dDownIntermissionHeightDev", m_tStandardSize.dDownIntermissionHeightDev, m_strConfigFile);


	////错位值
	WritePrivateProfileInt("SizeParam", "bEnableABMissValue", m_tStandardSize.bEnableABMissValue, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableABMissValueLabel", m_tStandardSize.bEnableABMissValueLabel, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dABMissValue", m_tStandardSize.dABMissValue, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dABMissValueDev", m_tStandardSize.dABMissValueDev, m_strConfigFile);






	WritePrivateProfileInt("SizeParam", "nCellNumber", m_tStandardSize.nCellNumber, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTotalLength", m_tStandardSize.dTotalLength, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTotalLengthDev", m_tStandardSize.dTotalLengthDev, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dCoatWidth", m_tStandardSize.dCoatWidth, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dCoatWidthDev", m_tStandardSize.dCoatWidthDev, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dLocateMissing", m_tStandardSize.dLocateMissing, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dLocateMissingDev", m_tStandardSize.dLocateMissingDev, m_strConfigFile);

	WritePrivateProfileDouble("SizeParam", "dCoatWidthAddValue", m_tStandardSize.dCoatWidthAddValue, m_strConfigFile);

	WritePrivateProfileInt("SizeParam", "bEnableTotalLengthLabel", m_tStandardSize.bEnableTotalLengthLabel, m_strConfigFile);//3
	WritePrivateProfileInt("SizeParam", "bEnableCoatWidthLabel", m_tStandardSize.bEnableCoatWidthLabel, m_strConfigFile);//3
	WritePrivateProfileInt("SizeParam", "bEnableLocateMissingLabel", m_tStandardSize.bEnableLocateMissingLabel, m_strConfigFile);//3


	WritePrivateProfileDouble("SizeParam", "m_dMinDist", m_dMinDist, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "nErrorNum", m_tStandardSize.nErrorNum, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvCoatHeightNGNum", m_tStandardSize.hvCoatHeightNGNum, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvCoatWidthNGNum", m_tStandardSize.hvCoatWidthNGNum, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "hvLocateMissingNGNum", m_tStandardSize.hvLocateMissingNGNum, m_strConfigFile);



	/////////////////////////////////////////////////////////////////////////标使能
	WritePrivateProfileInt("SizeParam", "bEnableBHJXLabel", m_tStandardSize.bEnableBHJXLabel, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableUpIntermissionLabel", m_tStandardSize.bEnableUpIntermissionLabel, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableDownIntermissionLabel", m_tStandardSize.bEnableDownIntermissionLabel, m_strConfigFile);


	WritePrivateProfileDouble("SizeParam", "dCorrectMin", m_tStandardSize.dCorrectMin, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dCorrectMax", m_tStandardSize.dCorrectMax, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dCoatHeightDev", m_tStandardSize.dCoatHeightDev, m_strConfigFile);


	WritePrivateProfileInt("SizeParam", "bEnableCoatHeight", m_tStandardSize.bEnableCoatHeight, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableCoatWidth", m_tStandardSize.bEnableCoatWidth, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableFoilWidth", m_tStandardSize.bEnableFoilWidth, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bEnableLocateMissing", m_tStandardSize.bEnableLocateMissing, m_strConfigFile);

	WritePrivateProfileInt("SizeParam", "bEnableCuoWei", m_tStandardSize.bEnableCuoWei, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dCuoWeiMax", m_tStandardSize.dCuoWeiMax, m_strConfigFile);


	WritePrivateProfileDouble("SizeParam", "hvCutPosition", m_tStandardSize.hvCutPosition, m_strConfigFile);



	///总宽////新加
	WritePrivateProfileInt("SizeParam", "bEnableTotalWidth", m_tStandardSize.bEnableTotalWidth, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bToatlWidthLabel", m_tStandardSize.bToatlWidthLabel, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTotalCoatWidth", m_tStandardSize.dTotalCoatWidth, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTotalCoatWidthDev", m_tStandardSize.dTotalCoatWidthDev, m_strConfigFile);

	////后相机总模长
	WritePrivateProfileInt("SizeParam", "bEnableBHJXLength2", m_tStandardSize.bEnableBHJXLength2, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bBHJXLabel2", m_tStandardSize.bBHJXLabel2, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dBHJXLength2", m_tStandardSize.dBHJXLength2, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dBHJXLengthDev2", m_tStandardSize.dBHJXLengthDev2, m_strConfigFile);


	///头部错位
	WritePrivateProfileInt("SizeParam", "bEnableHeadCuoWei", m_tStandardSize.bEnableHeadCuoWei, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bHeadCuoWeiLabel", m_tStandardSize.bHeadCuoWeiLabel, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dHeadCuoWei", m_tStandardSize.dHeadCuoWei, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dHeadCuoWeiDev", m_tStandardSize.dHeadCuoWeiDev, m_strConfigFile);


	///尾部错位
	WritePrivateProfileInt("SizeParam", "bEnableTailCuoWei", m_tStandardSize.bEnableTailCuoWei, m_strConfigFile);
	WritePrivateProfileInt("SizeParam", "bTailCuoWeiLabel", m_tStandardSize.bTailCuoWeiLabel, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTailCuoWei", m_tStandardSize.dTailCuoWei, m_strConfigFile);
	WritePrivateProfileDouble("SizeParam", "dTailCuoWeiDev", m_tStandardSize.dTailCuoWeiDev, m_strConfigFile);





	int nCellNumber;
	if (m_tStandardSize.nCellNumber == 1)
	{
		nCellNumber = 1;

	}
	else
	{
		nCellNumber = m_tStandardSize.nCellNumber;
	}

	for (int i = 0; i < nCellNumber; i++)
	{
		CString strAppName;
		strAppName.Format("nCellNumber%d", i);
		WritePrivateProfileDouble(strAppName, "hv_CoatHeight", m_tStandardSize.hv_CoatHeight[i].D(), m_strConfigFile);
	}
	WritePrivateProfileInt("SurfaceParam", "nErrorCount", m_tStandardSurface.nErrorCount, m_strConfigFile);
	WritePrivateProfileInt("SurfaceParam", "nSecErrorCount", m_tStandardSurface.nSecErrorCount, m_strConfigFile);
	for (int i = 0; i < m_tStandardSurface.nSecErrorCount; i++)
	{
		CString strAppName;
		strAppName.Format("nError%d", i);
		WritePrivateProfileInt(strAppName, "hv_ErrorIndex", m_tStandardSurface.hv_ErrorIndex[i].I(), m_strConfigFile);
		WritePrivateProfileInt(strAppName, "hv_TotalMaxNumber", m_tStandardSurface.hv_TotalMaxNumber[i].I(), m_strConfigFile);
		WritePrivateProfileDouble(strAppName, "hv_TotalMaxArea", m_tStandardSurface.hv_TotalMaxArea[i].D(), m_strConfigFile);
		WritePrivateProfileInt(strAppName, "hv_TotalMaxNumberTwo", m_tStandardSurface.hv_TotalMaxNumberTwo[i].I(), m_strConfigFile);
		WritePrivateProfileDouble(strAppName, "hv_TotalMaxAreaTwo", m_tStandardSurface.hv_TotalMaxAreaTwo[i].D(), m_strConfigFile);
		WritePrivateProfileDouble(strAppName, "hv_SingleMaxWidth", m_tStandardSurface.hv_SingleMaxWidth[i].D(), m_strConfigFile);
		WritePrivateProfileInt(strAppName, "hv_EnablePopWindow", m_tStandardSurface.hv_EnablePopWindow[i].I(), m_strConfigFile);
	}
}



void CJudge::PreClear()
{

	m_tSurfaceResult.hv_SumErrorType.Clear();
	m_tSurfaceResult.hv_SumErrorBelongTrips.Clear();
	GenEmptyObj(&m_tSurfaceResult.ho_SumErrorRegion);
	m_tFinalResult.hv_SurfaceFlag.Clear();
	m_tFinalResult.hv_SizeFlag.Clear();
	m_tFinalResult.hv_SurfaceErrorIndex.Clear();
	m_tFinalResult.hv_SurfaceErrorBelongTrips.Clear();
	m_tFinalResult.hv_SizeErrorIndex.Clear();
	m_tFinalResult.hv_SizeErrorBelongTrips.Clear();
	m_tFinalResult.hv_FoilNum.Clear();
	m_tFinalResult.hv_TotalLength = 0;
	m_nFoilSN = 0;
}

void CJudge::CorrectSizeResult(HTuple hv_SizeDetect, HTuple hv_SizeStandard, HTuple hv_CorrectThresLow,
	HTuple hv_CorrectThresHigh, HTuple *hv_SizeCorrect)
{

	// Local iconic variables
	// Local control variables
	HTuple  hv_Diff, hv_DiffTemp, hv_Index, hv_Abs;
	HTuple  hv_Sum, hv_Sum1;

	TupleSub(hv_SizeDetect, hv_SizeStandard, &hv_Diff);
	hv_DiffTemp = hv_Diff;
	{
		HTuple end_val2 = (hv_Diff.TupleLength()) - 1;
		HTuple step_val2 = 1;
		for (hv_Index = 0; hv_Index.Continue(end_val2, step_val2); hv_Index += step_val2)
		{
			TupleAbs(HTuple(hv_Diff[hv_Index]), &hv_Abs);
			//***如果满足矫正范围
			if (0 != (HTuple((HTuple(hv_Diff[hv_Index]).TupleAbs()) >= hv_CorrectThresLow).TupleAnd((HTuple(hv_Diff[hv_Index]).TupleAbs()) <= hv_CorrectThresHigh)))
			{
				if (0 != (HTuple(0 == hv_Index).TupleAnd((hv_Index + 1) <= ((hv_Diff.TupleLength()) - 1))))
				{
					//**如果是第0个，则相加后一个就可以了
					TupleSum(HTuple(hv_Diff[hv_Index]).TupleConcat(HTuple(hv_Diff[hv_Index + 1])),
						&hv_Sum);
					//**如果这个值更靠近标准值，那么就使用这个值
					if (0 != ((hv_Sum.TupleAbs()) <= (HTuple(hv_Diff[hv_Index]).TupleAbs())))
					{
						hv_DiffTemp[hv_Index] = hv_Sum;
					}
				}
				else if (0 != (HTuple(((hv_Diff.TupleLength()) - 1) == hv_Index).TupleAnd((hv_Index - 1) >= 0)))
				{
					//**如果是最后一个*****
					TupleSum(HTuple(hv_Diff[hv_Index]).TupleConcat(HTuple(hv_Diff[hv_Index - 1])),
						&hv_Sum);
					//**如果这个值更靠近标准值，那么就使用这个值
					if (0 != ((hv_Sum.TupleAbs()) <= (HTuple(hv_Diff[hv_Index]).TupleAbs())))
					{
						hv_DiffTemp[hv_Index] = hv_Sum;
					}
				}
				else
				{
					//****如果是中间的*************
					TupleSum(HTuple(hv_Diff[hv_Index]).TupleConcat(HTuple(hv_Diff[hv_Index - 1])),
						&hv_Sum);
					TupleSum(HTuple(hv_Diff[hv_Index]).TupleConcat(HTuple(hv_Diff[hv_Index + 1])),
						&hv_Sum1);
					if (0 != (HTuple((hv_Sum.TupleAbs()) <= (hv_Sum1.TupleAbs())).TupleAnd((hv_Sum.TupleAbs()) <= (HTuple(hv_Diff[hv_Index]).TupleAbs()))))
					{
						hv_DiffTemp[hv_Index] = hv_Sum;
					}
					else if (0 != (HTuple((hv_Sum1.TupleAbs()) <= (hv_Sum.TupleAbs())).TupleAnd((hv_Sum1.TupleAbs()) <= (HTuple(hv_Diff[hv_Index]).TupleAbs()))))
					{
						hv_DiffTemp[hv_Index] = hv_Sum1;
					}
				}
			}
		}
	}
	(*hv_SizeCorrect) = hv_SizeStandard + hv_DiffTemp;

	return;
}


void CJudge::SegErrorRegion(HObject hoImage, HTuple hvMarkRow, HObject hoErrorRegion, HTuple hvErrorType, HTuple hvErrorBelongTrips, HObject* ho_UpErrorRegion, HObject* ho_DownErrorRegion, HTuple* hvUpErrorType, HTuple*hvDownErrorType, HTuple* hvUpErrorBelongTrips, HTuple*hvDownErrorBelongTrips)
{
	HTuple hvAreaUp(0), hvAreaDown(0), hv_ErrorType, hv_UpErrorType, hv_DownErrorType(0), hv_TempErrorBelongTrips;
	HTuple hv_Width, hv_Height;
	HObject hoUpRegion, hoDownRegion;
	HObject hoIntersectionUp, hoIntersectionDown;
	HObject ho_UpRegion, ho_DownRegion;
	HObject hoSelectObj;
	hv_UpErrorType.Clear();
	hv_DownErrorType.Clear();

	hv_TempErrorBelongTrips.Clear();


	hoUpRegion.GenEmptyObj();
	hoDownRegion.GenEmptyObj();

	GenEmptyObj(&hoIntersectionUp);
	GenEmptyObj(&hoIntersectionDown);



	GetImageSize(hoImage, &hv_Width, &hv_Height);
	GenRectangle1(&ho_UpRegion, 0, 0, MIN(hvMarkRow, hv_Height - 2), hv_Width - 1);
	HTuple hv_Min;
	TupleMin2(hvMarkRow, hv_Height - 2, &hv_Min);
	GenRectangle1(&ho_DownRegion, hv_Min, 0, hv_Height - 1, hv_Width - 1);


	for (int i = 0; i <= hoErrorRegion.CountObj() - 1; i++)
	{


		SelectObj(hoErrorRegion, &hoSelectObj, i + 1);
		TupleSelect(hvErrorType, i, &hv_ErrorType);
		TupleSelect(hvErrorBelongTrips, i, &hv_TempErrorBelongTrips);
		Intersection(ho_UpRegion, hoSelectObj, &hoIntersectionUp);
		Intersection(ho_DownRegion, hoSelectObj, &hoIntersectionDown);

		AreaCenter(hoIntersectionUp, &hvAreaUp, NULL, NULL);
		AreaCenter(hoIntersectionDown, &hvAreaDown, NULL, NULL);
		if (hvAreaUp > 0)//在上面
		{
			ConcatObj(*ho_UpErrorRegion, hoIntersectionUp, ho_UpErrorRegion);
			(*hvUpErrorType).Append(hv_ErrorType);
			(*hvUpErrorBelongTrips).Append(hv_TempErrorBelongTrips);
		}
		if (hvAreaDown > 0)//在下面
		{
			ConcatObj(*ho_DownErrorRegion, hoIntersectionDown, ho_DownErrorRegion);
			(*hvDownErrorType).Append(hv_ErrorType);
			(*hvDownErrorBelongTrips).Append(hv_TempErrorBelongTrips);
		}

	}
}

void CJudge::ConcatErrorRegion(HObject ho_UpErrorRegion, HTuple hv_UpErrorType, HTuple hv_UpErrorBelongtrips)
{
	HObject hoUnion;
	HTuple hvArea1(0);
	GenEmptyObj(&hoUnion);
	Union1(ho_UpErrorRegion, &hoUnion);
	if (((HRegion)hoUnion).Area() > 0)
	{
		ConcatObj(m_tSurfaceResult.ho_SumErrorRegion, ho_UpErrorRegion, &m_tSurfaceResult.ho_SumErrorRegion);
		m_tSurfaceResult.hv_SumErrorType.Append(hv_UpErrorType);
		m_tSurfaceResult.hv_SumErrorBelongTrips.Append(hv_UpErrorBelongtrips);
	}
}


void CJudge::ComPareSize(HTuple hv_StandardSize, HTuple hv_StandardDev, HTuple hv_RealDetectSize, e_ErrorType ErrorType, C_ALLOCATORWORKSHOPOUTPUT& tOutput, BOOL& bRet, int nSN, int nTrip)
{

	HObject hoDomain;
	int nSide = nSN;
	GetDomain(tOutput.B[nSide].A.hoGrabImage, &hoDomain);

	if (hv_RealDetectSize > hv_StandardSize + hv_StandardDev || hv_RealDetectSize < hv_StandardSize - hv_StandardDev)
	{
		tOutput.B[nSide].tDetectOutput.hv_Flag[0] = FALSE;
		ConcatObj(tOutput.B[nSide].tDetectOutput.ho_ErrorRegion, hoDomain, &tOutput.B[nSide].tDetectOutput.ho_ErrorRegion);
		TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorType, ErrorType, &tOutput.B[nSide].tDetectOutput.hv_ErrorType);
		TupleConcat(tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip, nTrip, &tOutput.B[nSide].tDetectOutput.hv_ErrorBelongStrip);
		m_tFinalResult.hv_SizeErrorIndex.Append(ErrorType);
		m_tFinalResult.hv_SizeErrorBelongTrips.Append(nTrip);
		bRet = FALSE;
	}




}




BOOL CJudge::GetSurfaceResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut)
{

	/////更新服务器下发的缺陷规格

	double dwRowResolution = g_vec_RowResolution[m_tInit.nSN];
	double dwColumnResolution = g_vec_ColumnResolution[m_tInit.nSN];
	HTuple hvIndex, hv_SortTuple;
	HObject hoSingleObj;
	BOOL bRet = TRUE;
	tOutPut.tFinalResult.tSecondSurfaceInfo.clear();
	HObject hoSingleTypeError;
	HTuple hv_ErrorBelongTripsIndex;
	HTuple hv_Diameter, hvSingleTypeSumArea, hv_LessTuple, hv_LessIndex;
	try
	{
		if (m_tSurfaceResult.hv_SumErrorType.Length() > 0)//判断是否存在缺陷
		{
			HTuple hv_SingleTripSurfaceErrorType;
			HObject hv_SingleTripSurfaceErrorRegion;


			for (int k = 0; k < m_tStandardSize.hvTripNum.I(); k++)//遍历每一条
			{
				hv_SingleTripSurfaceErrorType.Clear();
				hv_SingleTripSurfaceErrorRegion.GenEmptyObj();
				{
					BOOL bInit = FALSE;
					if (m_tStandardSurface.hv_ErrorIndex.Length()>0)
					{
						for (int j = 0; j < m_tStandardSurface.hv_ErrorIndex.Length(); j++)
						{
							HTuple hv_ErrorDiameter, hv_SumArea, hv_ErrorNum;
							TupleFind(m_tSurfaceResult.hv_SumErrorBelongTrips, k, &hv_ErrorBelongTripsIndex);///确认该条是否存在缺陷

							if (hv_ErrorBelongTripsIndex.Length() > 0 && hv_ErrorBelongTripsIndex != -1)
							{


								TupleSelect(m_tSurfaceResult.hv_SumErrorType, hv_ErrorBelongTripsIndex, &hv_SingleTripSurfaceErrorType);//提取隶属于当前条的缺陷类型
								SelectObj(m_tSurfaceResult.ho_SumErrorRegion, &hv_SingleTripSurfaceErrorRegion, hv_ErrorBelongTripsIndex + 1);
								TupleFind(hv_SingleTripSurfaceErrorType, m_tStandardSurface.hv_ErrorIndex[j], &hvIndex);//判断当前的缺陷类型是否存在二次判断的缺陷
								if (0 != hvIndex.Length() && -1 != hvIndex)//存在
								{
									bInit = TRUE;
									HTuple hv_HorizonWidth;
									SelectObj(hv_SingleTripSurfaceErrorRegion, &hoSingleTypeError, hvIndex + 1);
									AreaCenter(hoSingleTypeError, &hvSingleTypeSumArea, NULL, NULL);//该类缺陷的全部单个面积
									RegionFeatures(hoSingleTypeError, "width", &hv_HorizonWidth);
									hv_SumArea = hvSingleTypeSumArea.TupleSum()*g_vec_RowResolution[0] * g_vec_ColumnResolution[0];



									HTuple hv_JudageArea, hv_JudageNum, hv_JudageAreaTwo, hv_JudageNumTwo;
									HTuple hv_Result0Index, hv_Result1Index, hv_ResultWidthIndex;
									TupleGreaterEqualElem(hvSingleTypeSumArea*g_vec_RowResolution[0] * g_vec_ColumnResolution[0], m_tStandardSurface.hv_TotalMaxArea[j].D(), &hv_Result0Index);
									TupleGreaterEqualElem(hvSingleTypeSumArea*g_vec_RowResolution[0] * g_vec_ColumnResolution[0], m_tStandardSurface.hv_TotalMaxAreaTwo[j].D(), &hv_Result1Index);
									TupleGreaterEqualElem(hv_HorizonWidth*g_vec_ColumnResolution[0], m_tStandardSurface.hv_SingleMaxWidth[j].D(), &hv_ResultWidthIndex);

									HTuple hv_Result0, hv_Result1;
									TupleFind(hv_Result0Index, 1, &hv_Result0);
									TupleFind(hv_Result1Index, 1, &hv_Result1);

									if ((hv_Result0 != -1 && hv_Result0.Length() >= m_tStandardSurface.hv_TotalMaxNumber[j] || hv_Result1 != -1 && hv_Result1.Length() >= m_tStandardSurface.hv_TotalMaxNumberTwo[j]) && hv_ResultWidthIndex.TupleSum() > 0)
									{




										HTuple hv_LTRow, hvLTColumn, hvRBRow, hv_RBColumn;
										HTuple hv_BelongTrips;
										bRet = FALSE;
										SmallestRectangle1(hoSingleTypeError, &hv_LTRow, &hvLTColumn, &hvRBRow, &hv_RBColumn);
										m_tFinalResult.hv_SurfaceErrorIndex.Append(m_tStandardSurface.hv_ErrorIndex[j]);
										hv_ErrorNum = MAX(hv_Result0.Length(), hv_Result1.Length());
										m_tFinalResult.hv_SurfaceErrorBelongTrips.Append(k);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_ErrorType.Append(m_tStandardSurface.hv_ErrorIndex[j]);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum.Append(hv_ErrorNum);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea.Append(hv_SumArea);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_LeftTopRow.Append(hv_LTRow*g_vec_RowResolution[0]);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_LeftTopColumn.Append(hvLTColumn*g_vec_ColumnResolution[0]);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_RightBottomRow.Append(hvRBRow*g_vec_RowResolution[0]);
										tOutPut.tFinalResult.tSecondSurfaceInfo.hv_RightBottomColumn.Append(hv_RBColumn*g_vec_ColumnResolution[0]);
									}
								}

							}



						}

					}

				}

			}



		}


	}
	catch (HException e)
	{
		CString strException;
		strException.Format("TEST_综合表面结果函数出错:%s", e.ErrorMessage().Text());
		AfxMessageBox(strException);
	}
	return bRet;
}
