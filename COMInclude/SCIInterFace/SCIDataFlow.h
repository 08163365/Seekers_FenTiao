#include "stdafx.h"
#include "DH_HalconEx.h"

typedef struct tagTA_GRABWORKSHOPOUTPUT//采集车间产出
{
	HObject hoGrabImage;//采集到的图像
	HTuple  hvGrabberSN;//相机序号
	HTuple  hvFrameSN; //第几帧
	HTuple  hvEncoder;//编码器行数
	HTuple  hvImageSN;//图像号
	HTuple  hv_PinTuNormal;//拼图成功的标志 0：未拼图初始值 1是正常成功 2是强制出图
	HTuple  hvTimeStamp;//时间戳
	inline int ReadMessage(const HTuple& hvMessage,const int& nGrabberSN)//读取带序号的消息
	{	
		 CString strKey;
		strKey.Format("A%d_ENCODER", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hvEncoder);//
		strKey.Format("A%d_FRAMESN", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hvFrameSN);//
		strKey.Format("A%d_IMAGESN", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hvImageSN);//
		strKey.Format("A%d_GRABBERSN", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hvGrabberSN);//
		strKey.Format("A%d_PinTuNormal", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hv_PinTuNormal);//

		strKey.Format("A%d_TIMESTAMP", nGrabberSN);
		GetMessageTuple(hvMessage, HTuple(strKey), &hvTimeStamp);//


		strKey.Format("A%d_IMAGE", nGrabberSN);
		GetMessageObj(&hoGrabImage, hvMessage, HTuple(strKey));//
		return TRUE;
	}
	inline int WriteMessage(const HTuple& hvMessage,const int& nGrabberSN)
	{
		CString strKey;
		strKey.Format("A%d_ENCODER",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvEncoder);//
		strKey.Format("A%d_FRAMESN",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvFrameSN);//
		strKey.Format("A%d_IMAGESN",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvImageSN);//
		strKey.Format("A%d_GRABBERSN",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvGrabberSN);//

		strKey.Format("A%d_PinTuNormal", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hv_PinTuNormal);//
		strKey.Format("A%d_TIMESTAMP",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvTimeStamp);//
		strKey.Format("A%d_IMAGE",nGrabberSN);
		SetMessageObj(hoGrabImage, hvMessage, HTuple(strKey));//
		return TRUE;
	}
}
A_GRABWORKSHOPOUTPUT,*PA_GRABWORKSHOPOUTPUT;
typedef struct tagTB_DETECTWORKSHOPOUTPUT//检测车间产出
{
	A_GRABWORKSHOPOUTPUT A;
	DETECTOUTPUT         tDetectOutput;
	HTuple  hvTimeStamp;//时间戳
	HTuple  hv_CropRow;//拼图时候和Mark点偏出来的误差值
	HTuple  hv_PinTuHalfWay;//切到一半重新切的时候

	inline int ReadMessage(const HTuple& hvMessage,const int& nGrabberSN)
	{	
		A.ReadMessage(hvMessage, nGrabberSN);
		CString strKey;
		    strKey.Format("A%d_hv_PinTuHalfWay", nGrabberSN);
		    GetMessageTuple(hvMessage, HTuple(strKey), &hv_PinTuHalfWay);//
			strKey.Format("B%d_FLAG", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Flag);
			strKey.Format("B%d_ERRORTYPE", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_ErrorType);
			strKey.Format("B%d_ERRORLEVEL", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_ErrorLevel);
			strKey.Format("B%d_ERRORBELONGSTRIP", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_ErrorBelongStrip);
			strKey.Format("B%d_LOCATEMISSING", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_LocateMissing);
			strKey.Format("B%d_COATWIDTH", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CoatWidth);
			strKey.Format("B%d_COATHEIGHT", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CoatHeight);
			strKey.Format("B%d_FOILWIDTH", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_FoilWidth);
			strKey.Format("B%d_FOILHEIGHT", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_FoilHeight);
			strKey.Format("B%d_MARKROW", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_JuanRaoMarkRow);
			strKey.Format("B%d_Baibian", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_BaiBian);

	/*		strKey.Format("B%d_hv_Reserve1", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve1);
			strKey.Format("B%d_hv_Reserve2", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve2);
			strKey.Format("B%d_hv_Reserve3", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve3);
			strKey.Format("B%d_hv_Reserve4", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve4);
			strKey.Format("B%d_hv_Reserve5", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve5);
			strKey.Format("B%d_hv_Reserve6", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve6);
			strKey.Format("B%d_hv_Reserve7", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Reserve7);*/


			///新增加的间歇和连续涂布
			strKey.Format("B%d_hv_CeramicWidth", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CeramicWidth);
			strKey.Format("B%d_hv_SortWidth", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_SortWidth);
			strKey.Format("B%d_hv_Name", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_Name);
			strKey.Format("B%d_hv_AT9EdageToProEdage", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_AT9EdageToProEdage);
			strKey.Format("B%d_hv_CpkEdageToProEdage", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CpkEdageToProEdage);
			//间歇涂布
			strKey.Format("B%d_hv_LeftEadgePixel", nGrabberSN);//左侧像素
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_LeftEadgePixel);
			strKey.Format("B%d_hv_RightEadgePixel", nGrabberSN);//右侧像素
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_RightEadgePixel);
			strKey.Format("B%d_hv_TopEdagePixel", nGrabberSN);//顶部像素
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_TopEdagePixel);
			strKey.Format("B%d_hv_BotEdagePixel", nGrabberSN);//底部像素
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_BotEdagePixel);
			strKey.Format("B%d_hv_HeadCuoWei", nGrabberSN);//头部错位
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_HeadCuoWei);
			strKey.Format("B%d_hv_TailCuoWei", nGrabberSN);//尾部错位
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_TailCuoWei);

			strKey.Format("B%d_hv_TripSNTuple", nGrabberSN);//槽位隶属于条数
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_TripSNTuple);
			strKey.Format("B%d_hv_CaoWeiWidthTuple", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CaoWeiWidthTuple);
			strKey.Format("B%d_hv_CaoWeiHeightTuple", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CaoWeiHeightTuple);
			strKey.Format("B%d_hv_CaoWeiXPositionTuple", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CaoWeiXPositionTuple);
			strKey.Format("B%d_hv_CaoWeiYpositionTuple", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_CaoWeiYpositionTuple);
			strKey.Format("B%d_hv_JXHeight", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_JXHeight);
			strKey.Format("B%d_hv_MPLength", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_MPLength);
			strKey.Format("B%d_hv_SumLength", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_SumLength);
			strKey.Format("B%d_ho_JXRegion", nGrabberSN);
			GetMessageObj(&tDetectOutput.ho_JXRegion, hvMessage, HTuple(strKey));
			strKey.Format("B%d_ho_ExtractCaoWeiRegions", nGrabberSN);
			GetMessageObj(&tDetectOutput.ho_ExtractCaoWeiRegions, hvMessage, HTuple(strKey));

			strKey.Format("B%d_TIMESTAMP", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &hvTimeStamp);
			strKey.Format("B%d_CROPROW", nGrabberSN);
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_CropRow);

			strKey.Format("B%d_ERRORREGION", nGrabberSN);
			GetMessageObj(&tDetectOutput.ho_ErrorRegion, hvMessage, HTuple(strKey));
			strKey.Format("B%d_COAT", nGrabberSN);
			GetMessageObj(&tDetectOutput.ho_Coat, hvMessage, HTuple(strKey));
			strKey.Format("B%d_FOIL", nGrabberSN);
			GetMessageObj(&tDetectOutput.ho_Foil, hvMessage, HTuple(strKey));
		return TRUE;
	}
	inline int WriteMessage(const HTuple& hvMessage,const int& nGrabberSN)
	{
		A.WriteMessage(hvMessage,nGrabberSN);
		CString strKey;
		strKey.Format("A%d_hv_PinTuHalfWay", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hv_PinTuHalfWay);//


		strKey.Format("B%d_FLAG",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Flag);
		strKey.Format("B%d_ERRORTYPE",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_ErrorType);
		strKey.Format("B%d_ERRORLEVEL",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_ErrorLevel);
		strKey.Format("B%d_ERRORBELONGSTRIP",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_ErrorBelongStrip);
		strKey.Format("B%d_LOCATEMISSING",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_LocateMissing);
		strKey.Format("B%d_COATWIDTH",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CoatWidth);//涂布尺寸
		strKey.Format("B%d_COATHEIGHT",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CoatHeight);
		strKey.Format("B%d_FOILWIDTH",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_FoilWidth);//极耳宽度尺寸
		strKey.Format("B%d_FOILHEIGHT",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_FoilHeight);
		strKey.Format("B%d_MARKROW",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_JuanRaoMarkRow);
		strKey.Format("B%d_Baibian",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_BaiBian);
		//////////////////////////新添加//////////////////////////////////////////////
		///*		strKey.Format("B%d_hv_Reserve1", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve1);
		//strKey.Format("B%d_hv_Reserve2", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve2);
		//strKey.Format("B%d_hv_Reserve3", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve3);
		//strKey.Format("B%d_hv_Reserve4", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve4);
		//strKey.Format("B%d_hv_Reserve5", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve5);
		//strKey.Format("B%d_hv_Reserve6", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve6);
		//strKey.Format("B%d_hv_Reserve7", nGrabberSN);
		//SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Reserve7);

		///////////////////////////////////////////////////////////////////////

		///新增加的间歇和连续涂布
		strKey.Format("B%d_hv_CeramicWidth", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CeramicWidth);//AT9尺寸

		strKey.Format("B%d_hv_SortWidth", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_SortWidth);
		strKey.Format("B%d_hv_Name", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Name);
		strKey.Format("B%d_hv_AT9EdageToProEdage", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_AT9EdageToProEdage);
		strKey.Format("B%d_hv_CpkEdageToProEdage", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CpkEdageToProEdage);
		//间歇涂布
		strKey.Format("B%d_hv_LeftEadgePixel", nGrabberSN);//左侧像素
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_LeftEadgePixel);
		strKey.Format("B%d_hv_RightEadgePixel", nGrabberSN);//右侧像素
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_RightEadgePixel);
		strKey.Format("B%d_hv_TopEdagePixel", nGrabberSN);//顶部像素
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_TopEdagePixel);
		strKey.Format("B%d_hv_BotEdagePixel", nGrabberSN);//底部像素
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_BotEdagePixel);
		strKey.Format("B%d_hv_HeadCuoWei", nGrabberSN);//头部错位
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_HeadCuoWei);
		strKey.Format("B%d_hv_TailCuoWei", nGrabberSN);//尾部错位
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_TailCuoWei);

		strKey.Format("B%d_hv_TripSNTuple", nGrabberSN);//槽位隶属于条数
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_TripSNTuple);
		strKey.Format("B%d_hv_CaoWeiWidthTuple", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CaoWeiWidthTuple);
		strKey.Format("B%d_hv_CaoWeiHeightTuple", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CaoWeiHeightTuple);
		strKey.Format("B%d_hv_CaoWeiXPositionTuple", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CaoWeiXPositionTuple);
		strKey.Format("B%d_hv_CaoWeiYpositionTuple", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CaoWeiYpositionTuple);
		strKey.Format("B%d_hv_JXHeight", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_JXHeight);
		strKey.Format("B%d_hv_MPLength", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_MPLength);
		strKey.Format("B%d_hv_SumLength", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_SumLength);


		if (FALSE == tDetectOutput.ho_JXRegion.IsInitialized())
		{
			GenEmptyObj(&tDetectOutput.ho_JXRegion);
		}

		strKey.Format("B%d_ho_JXRegion", nGrabberSN);
		SetMessageObj(tDetectOutput.ho_JXRegion, hvMessage, HTuple(strKey));

		if (FALSE == tDetectOutput.ho_ExtractCaoWeiRegions.IsInitialized())
		{
			GenEmptyObj(&tDetectOutput.ho_ExtractCaoWeiRegions);
		}
		strKey.Format("B%d_ho_ExtractCaoWeiRegions", nGrabberSN);
		SetMessageObj(tDetectOutput.ho_ExtractCaoWeiRegions, hvMessage, HTuple(strKey));

		///////////////////////////////////////////////////////////////////////
		strKey.Format("B%d_TIMESTAMP",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hvTimeStamp);
		strKey.Format("B%d_CROPROW",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), hv_CropRow);
		strKey.Format("B%d_ERRORREGION",nGrabberSN);
		if (FALSE == tDetectOutput.ho_ErrorRegion.IsInitialized())
		{
			GenEmptyObj(&tDetectOutput.ho_ErrorRegion);
		}



		SetMessageObj(tDetectOutput.ho_ErrorRegion,hvMessage, HTuple(strKey));
		strKey.Format("B%d_COAT",nGrabberSN);
		if (FALSE == tDetectOutput.ho_Coat.IsInitialized())
		{
			GenEmptyObj(&tDetectOutput.ho_Coat);
		}
		SetMessageObj(tDetectOutput.ho_Coat,hvMessage, HTuple(strKey));

		strKey.Format("B%d_FOIL",nGrabberSN);
		if (FALSE == tDetectOutput.ho_Foil.IsInitialized())
		{
			GenEmptyObj(&tDetectOutput.ho_Foil);
		}
		SetMessageObj(tDetectOutput.ho_Foil,hvMessage, HTuple(strKey));
		return TRUE;
	}
}
B_DETECTWORKSHOPOUTPUT,*PB_DETECTWORKSHOPOUTPUT;
typedef struct tagTC_ALLOCATORWORKSHOPOUTPUT
{
	B_DETECTWORKSHOPOUTPUT B[2];//2个检测车间产出
	HTuple hvPackageSN;//当前作为极耳的序列号使用
	HTuple hvPackageType;//条数, 0为左侧,1为右侧
	HTuple hvDislocation;//当前未使用该参数
	HTuple hvRealCPKValue;//实时计算CPK的值
	HTuple hvGroupID;//组号
    HTuple bIsFinalResult;//是否是最终结果,TRUE是最终结果，FALSE不是最终结果
	HTuple hvTimeStamp;//时间戳
	FINALRESULT tFinalResult;
	inline int ReadMessage(const HTuple& hvMessage)
	{
		CString strKey;
		strKey.Format("hvPackageSN");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvPackageSN);
		strKey.Format("hvPackageType");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvPackageType);
		strKey.Format("hvDislocation");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvDislocation);
		strKey.Format("C_hvTimeStamp");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvTimeStamp);
		strKey.Format("bIsFinalResult");
		GetMessageTuple(hvMessage, HTuple(strKey), &bIsFinalResult);

		strKey.Format("hvRealCPKValue");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvRealCPKValue);

		strKey.Format("hvGroupID");
		GetMessageTuple(hvMessage, HTuple(strKey), &hvGroupID);
	if (TRUE == bIsFinalResult)
		{
			tFinalResult.ReadMessage(hvMessage);
		}

		B[0].ReadMessage(hvMessage,0);
		B[1].ReadMessage(hvMessage,1);
		
		return TRUE;
	}
	inline int WriteMessage(const HTuple& hvMessage)
	{

		CString strKey;
		strKey.Format("hvPackageSN");
		SetMessageTuple(hvMessage, HTuple(strKey), hvPackageSN);
		strKey.Format("hvPackageType");
		SetMessageTuple(hvMessage, HTuple(strKey), hvPackageType);
		strKey.Format("hvDislocation");
		SetMessageTuple(hvMessage, HTuple(strKey), hvDislocation);
		strKey.Format("C_hvTimeStamp");
		SetMessageTuple(hvMessage, HTuple(strKey), hvTimeStamp);

		strKey.Format("hvGroupID");
		SetMessageTuple(hvMessage, HTuple(strKey), hvGroupID);

		strKey.Format("bIsFinalResult");
		SetMessageTuple(hvMessage, HTuple(strKey), bIsFinalResult);

		strKey.Format("hvRealCPKValue");
		SetMessageTuple(hvMessage, HTuple(strKey), hvRealCPKValue);



		if (TRUE == bIsFinalResult)
		{
			tFinalResult.WriteMessage(hvMessage);
		}
		B[0].WriteMessage(hvMessage,0);
		B[1].WriteMessage(hvMessage,1);

		
		return TRUE;
	}
}
C_ALLOCATORWORKSHOPOUTPUT,*PC_ALLOCATORWORKSHOPOUTPUT;