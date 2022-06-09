#include "stdafx.h"
#include "DH_HalconEx.h"

typedef struct tagTA_GRABWORKSHOPOUTPUT//�ɼ��������
{
	HObject hoGrabImage;//�ɼ�����ͼ��
	HTuple  hvGrabberSN;//������
	HTuple  hvFrameSN; //�ڼ�֡
	HTuple  hvEncoder;//����������
	HTuple  hvImageSN;//ͼ���
	HTuple  hv_PinTuNormal;//ƴͼ�ɹ��ı�־ 0��δƴͼ��ʼֵ 1�������ɹ� 2��ǿ�Ƴ�ͼ
	HTuple  hvTimeStamp;//ʱ���
	inline int ReadMessage(const HTuple& hvMessage,const int& nGrabberSN)//��ȡ����ŵ���Ϣ
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
typedef struct tagTB_DETECTWORKSHOPOUTPUT//��⳵�����
{
	A_GRABWORKSHOPOUTPUT A;
	DETECTOUTPUT         tDetectOutput;
	HTuple  hvTimeStamp;//ʱ���
	HTuple  hv_CropRow;//ƴͼʱ���Mark��ƫ���������ֵ
	HTuple  hv_PinTuHalfWay;//�е�һ�������е�ʱ��

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


			///�����ӵļ�Ъ������Ϳ��
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
			//��ЪͿ��
			strKey.Format("B%d_hv_LeftEadgePixel", nGrabberSN);//�������
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_LeftEadgePixel);
			strKey.Format("B%d_hv_RightEadgePixel", nGrabberSN);//�Ҳ�����
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_RightEadgePixel);
			strKey.Format("B%d_hv_TopEdagePixel", nGrabberSN);//��������
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_TopEdagePixel);
			strKey.Format("B%d_hv_BotEdagePixel", nGrabberSN);//�ײ�����
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_BotEdagePixel);
			strKey.Format("B%d_hv_HeadCuoWei", nGrabberSN);//ͷ����λ
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_HeadCuoWei);
			strKey.Format("B%d_hv_TailCuoWei", nGrabberSN);//β����λ
			GetMessageTuple(hvMessage, HTuple(strKey), &tDetectOutput.hv_TailCuoWei);

			strKey.Format("B%d_hv_TripSNTuple", nGrabberSN);//��λ����������
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
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CoatWidth);//Ϳ���ߴ�
		strKey.Format("B%d_COATHEIGHT",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CoatHeight);
		strKey.Format("B%d_FOILWIDTH",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_FoilWidth);//������ȳߴ�
		strKey.Format("B%d_FOILHEIGHT",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_FoilHeight);
		strKey.Format("B%d_MARKROW",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_JuanRaoMarkRow);
		strKey.Format("B%d_Baibian",nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_BaiBian);
		//////////////////////////�����//////////////////////////////////////////////
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

		///�����ӵļ�Ъ������Ϳ��
		strKey.Format("B%d_hv_CeramicWidth", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CeramicWidth);//AT9�ߴ�

		strKey.Format("B%d_hv_SortWidth", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_SortWidth);
		strKey.Format("B%d_hv_Name", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_Name);
		strKey.Format("B%d_hv_AT9EdageToProEdage", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_AT9EdageToProEdage);
		strKey.Format("B%d_hv_CpkEdageToProEdage", nGrabberSN);
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_CpkEdageToProEdage);
		//��ЪͿ��
		strKey.Format("B%d_hv_LeftEadgePixel", nGrabberSN);//�������
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_LeftEadgePixel);
		strKey.Format("B%d_hv_RightEadgePixel", nGrabberSN);//�Ҳ�����
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_RightEadgePixel);
		strKey.Format("B%d_hv_TopEdagePixel", nGrabberSN);//��������
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_TopEdagePixel);
		strKey.Format("B%d_hv_BotEdagePixel", nGrabberSN);//�ײ�����
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_BotEdagePixel);
		strKey.Format("B%d_hv_HeadCuoWei", nGrabberSN);//ͷ����λ
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_HeadCuoWei);
		strKey.Format("B%d_hv_TailCuoWei", nGrabberSN);//β����λ
		SetMessageTuple(hvMessage, HTuple(strKey), tDetectOutput.hv_TailCuoWei);

		strKey.Format("B%d_hv_TripSNTuple", nGrabberSN);//��λ����������
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
	B_DETECTWORKSHOPOUTPUT B[2];//2����⳵�����
	HTuple hvPackageSN;//��ǰ��Ϊ���������к�ʹ��
	HTuple hvPackageType;//����, 0Ϊ���,1Ϊ�Ҳ�
	HTuple hvDislocation;//��ǰδʹ�øò���
	HTuple hvRealCPKValue;//ʵʱ����CPK��ֵ
	HTuple hvGroupID;//���
    HTuple bIsFinalResult;//�Ƿ������ս��,TRUE�����ս����FALSE�������ս��
	HTuple hvTimeStamp;//ʱ���
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