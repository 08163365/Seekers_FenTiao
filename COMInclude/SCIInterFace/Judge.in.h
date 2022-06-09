#pragma once
#include "stdafx.h"

typedef struct tagTINITJUDGE//�ߴ���
{
	CString strProductName;
	int     nSN;
}INITJUDGE,*PINITJUDGE;

typedef struct tagTALLDETECTSIZE//�ߴ���
{
	HTuple hv_HeightTuple;//��Ƭ����
	HTuple hv_AllPartsFlag;//������Ƭ��ÿ��Ƭ�ĳߴ��־λ����
	HTuple hv_WidthTuple;//��Ƭ���
	HTuple hv_FoilWidthTuple;//�������
	HTuple hv_BaiBianTuple;//�ױ߿��
	HTuple hv_FoilHeightTuple;//�����ĸ߶� �з���--
	HTuple hv_LocateMissingTuple;

	HTuple hv_UpCWSizePartFlag;   //������
	HTuple hv_DownCWSizePartFlag;//������

	HTuple hv_UpLocateSizePartFlag;//����Ķ�
	HTuple hv_DownLocateSizePartFlag;//����ı�־

	HTuple hv_CHSizePartFlag;//���

}ALLDETECTSIZE,*PALLDETECTSIZE;


typedef struct tagTCAOWEISIZE//��λ�ߴ�
{
	HTuple hvCaoWeiHeight;
	HTuple hvCaoWeiHeightDev;
	HTuple hvCaoWeiWidth;
	HTuple hvCaoWeiWidthDev;
	HTuple hvCaoWeiMD;
	HTuple hvCaoWeiMDDev;
	HTuple hvCaoWeiTDDev;
	HTuple hvCaoWeiTDTuple;//ˮƽ�����λ��

	HTuple hvCaoWeiMDCuoWeiDev;//�˶�����Ĵ�λ����
	HTuple hvCaoWeiTDCuoWeiDev;//ˮƽ����Ĵ�λ����

}CAOWEISIZE,*PCAOWEISIZE;



typedef struct tagTSTANDARDSIZE//��׼�ߴ�
{
	HTuple nCellNumber;//��������Ƭ�ĸ���
	HTuple nErrorNum;//��������ߴ����������Ŀ
	HTuple dCorrectMax;//����
	HTuple dCorrectMin;
	///��Ʒ���
	HTuple hv_ProductType;//��Ʒ����0 ���� 1����
	HTuple hv_CoatType;//Ϳ������ 0������ 1�Ǽ�Ъ
	HTuple hv_CaoWeiType;//��λ���� 0 �����ڲ�λ 1�����λ 2 �����λ 3 ˫����ڲ�λ
	HTuple hv_OAPType;//OAP���� 0 ������OAP 1����OAP 2 ����OAP 3 ˫�����OAP
	HTuple hvTripNum;//����
	HTuple hvZhiDaiGray;//ֽ���ĻҶ�
	/////
    HTuple bEnableTotalLength;//�Ƿ���
	HTuple dTotalLength;//�ܳ�
	HTuple dTotalLengthDev;//�ܳ�ƫ��
	HTuple bEnableTotalLengthLabel;//�ܳ�����


	HTuple hv_CoatHeight;       //��ƬͿ���߶�
	HTuple dCoatHeightDev;     //��ƬͿ��ƫ��
	HTuple bEnableCoatHeight;  //�Ƿ���
	HTuple hvCoatHeightNGNum;//Ϳ���߶�NG��Ŀ

	HTuple bEnableCoatWidth;  //�Ƿ���


	
	HTuple dCoatWidth;       //��Ƭ��� �з���
	HTuple dCoatWidthDev;    //��ƬͿ�����
	HTuple hvCoatWidthNGNum;//Ϳ�����NG��Ŀ
	HTuple bEnableCoatWidthLabel;//Ϳ���������
	HTuple dCoatWidthAddValue;//�ӿ�ֵ
	


	//������ȼ��
	HTuple bEnableFoilWidth;  //�Ƿ���
	HTuple dFoilWidth;    //��Ƭ������� �з���
	HTuple dFoilWidthDev; //��Ƭ�������ƫ��
	
	///AT9�ߴ���
	HTuple bEnableLocateMissing;  //�Ƿ���
	HTuple dLocateMissing;//��λ�����׼ֵ
	HTuple dLocateMissingDev;//��λ����ƫ��
	HTuple hvLocateMissingNGNum;//��λ����NG��Ŀ
	HTuple bEnableLocateMissingLabel;//ǰ�����λ��������


	//////������Ъ�ܳ�
	HTuple bEnableBHJXTotalLength;  //�Ƿ��ⲻ����Ъ�ܳ�
	HTuple dBHJXTotalLength;        //������Ъ�ܳ�
	HTuple dBHJXTotalLengthDev;     //������Ъ�ܳ�ƫ��
	HTuple bEnableBHJXLabel;        //����ʹ��



	//////�����Ъ�߶� 
	HTuple bEnableUpImHeight;         //�Ƿ��������Ъ�߶�
	HTuple dUpIntermissionHeight;     //�����Ъ�߶ȱ�׼ֵ
	HTuple dUpIntermissionHeightDev;  //�����Ъ�߶ȹ���
	HTuple bEnableUpIntermissionLabel;//����ʹ��

	//////�����Ъ�߶ȣ�A�棩 
	HTuple bEnableDownImHeight;         //�Ƿ��ⷴ���Ъ�߶�
	HTuple dDownIntermissionHeight;     //�����Ъ�߶ȱ�׼ֵ
	HTuple dDownIntermissionHeightDev;  //�����Ъ�߶ȹ���
	HTuple bEnableDownIntermissionLabel;//����ʹ��

	///�ܿ�////
	HTuple    bEnableTotalWidth;
	HTuple   dTotalCoatWidth;
	HTuple    dTotalCoatWidthDev;
	HTuple    bToatlWidthLabel;

	////������ģ��
	HTuple bEnableBHJXLength2;
	HTuple dBHJXLength2;
	HTuple dBHJXLengthDev2;
	HTuple bBHJXLabel2;


	///ͷ����λ
	HTuple bEnableHeadCuoWei;
	HTuple dHeadCuoWei;
	HTuple dHeadCuoWeiDev;
	HTuple bHeadCuoWeiLabel;

	///β����λ
	HTuple bEnableTailCuoWei;
	HTuple dTailCuoWei;
	HTuple dTailCuoWeiDev;
	HTuple bTailCuoWeiLabel;
	



	//////��λֵ/////
	HTuple bEnableABMissValue;  //�Ƿ���AB��λ
	HTuple dABMissValue;//AB��λֵ
	HTuple dABMissValueDev;//AB��λ����
	HTuple bEnableABMissValueLabel;//AB��λ����ʹ��

	///�¼�AT9AB���λ����
	HTuple bEnableCuoWei;//����λ
	HTuple dCuoWeiMax;//���ֵ
	HTuple hvCutPosition;//����λ��


	///��λλ��
	//HTuple hvLocateUpAndDown;//��λ��λ��
	HTuple hvExistJXType;//��Ъ���ڵ����� 0���������Ъ  1���������Ъ 2��˫���Ъ
	HTuple hvGongYiType;//�������� 0��ASC1 1:M6S 2:MMT 3:STS1 4:STS2


	HTuple hvQiTouXianChu;//0 ��λ����  1��ʾ��ͷ�ȳ� ��λ����
	HTuple hvChangMoZaiShang;//



	CAOWEISIZE tUpCaoWeiParam;
	CAOWEISIZE tDownCaoWeiParam;




}STANDARDSIZE,*PSTANDARDSIZE;

typedef struct tagTSECONDSURFACEINFO  //�����ж�ʱ��Ҫ�ı���ȱ����Ϣ
{
	HTuple hv_ErrorNum;//ȱ�ݵ���Ŀ
	HTuple hv_ErrorSumArea;//ȱ�ݵ������
	HTuple hv_ErrorType;//�������еĴ�������
	HTuple hv_LeftTopRow;//����ȱ�ݵ����Ͻ�����
	HTuple hv_LeftTopColumn;//����ȱ�ݵ����Ͻ�������
	HTuple hv_RightBottomRow;//����ȱ�ݵ����½�������
	HTuple hv_RightBottomColumn;//����ȱ�ݵ����½�������

	void clear()
	{
		hv_ErrorNum.Clear();
		hv_ErrorSumArea.Clear();
		hv_ErrorType.Clear();
		hv_LeftTopRow.Clear();
		hv_LeftTopColumn.Clear();
		hv_RightBottomRow.Clear();
		hv_RightBottomColumn.Clear();

	}
}SECONDSURFACEINFO ,*PSECONDSURFACEINFO;
typedef struct tagTSTANDARDSURFACE//�Ƿ���Ҫ�����жϵı���ȱ�ݲ���
{
	HTuple nErrorCount;//һ���ж�����ȱ������
	HTuple nSecErrorCount;//�ж�������Ҫ�������ж�
	//��Ҫ�������жϵ�ȱ������
	HTuple hv_ErrorIndex;
	//�ۼ��жϣ��������ȱ�ݼ�����
	HTuple hv_TotalMaxNumber;      //�����ø�����Ϊ��NG
	HTuple hv_TotalMaxArea;        //����ۼ�������ڸ������ΪNG

	HTuple hv_TotalMaxNumberTwo;      //�����ø�����Ϊ��NG
	HTuple hv_TotalMaxAreaTwo;        //����ۼ�������ڸ������ΪNG

	HTuple hv_SingleMaxWidth;		//�����
	HTuple hv_EnablePopWindow;      //����ȱ�ݵ�������ʹ�ܿ���
	//�������ȱ�ݳ�������Ҳ��ΪNG
	//HTuple hv_SingleMaxDiameter;    //���ֱ��	
	//HTuple hv_SingleMaxLength;     //��󳤶�
	//HTuple hv_SingleMaxWidth;		//�����
	
}STANDARDSURFACE,*PSTANDARDSURFACE;

typedef struct tagTSIZERESULT//�ߴ��־
{

	HTuple hv_SumFlag;//��Ʒ�ܳ���־��TUREΪOK��FALSEΪNG
	HTuple hv_CoatHeightFlag;//�ߴ��־ ��Ƭ���ȣ�TUREΪOK��FALSEΪNG
	HTuple hv_CoatWidthFlag;//Ϳ����ȱ�־ TUREΪOK��FALSEΪNG
	HTuple hv_FoilWidthFlag;//������� TUREΪOK��FALSEΪNG
	HTuple hv_FoilHeightFlag;//�����߶� TUREΪOK��FALSEΪNG
	HTuple hv_TsizeFlag;//���б�־λ  TUREΪOK��FALSEΪNG
	HTuple hv_BaibianFlag;//�ױ߱�־λ TUREΪOK��FALSEΪNG
	HTuple hv_DiePianMarkWidthFlag;//��ƬU���пڿ�� TUREΪOK��FALSEΪNG
	HTuple hv_DiePianMarkHeightFlag;//��ƬU���пڸ߶� TUREΪOK��FALSEΪNG
	HTuple hv_DiePianMarkDiffFlag;//��ƬU�οڵĶ����-TUREΪOK��FALSEΪNG
	HTuple hv_DistanceBorderFlag;//���߾��־λ-TUREΪOK��FALSEΪNG
	HTuple hv_LocateMissingFlag;//��λ�����־λ-TUREΪOK��FALSEΪNG
}SIZERESULT,*PSIZERESULT;
typedef struct tagTFINALRESULT//���ս��
{
    SECONDSURFACEINFO tSecondSurfaceInfo;//�����ж�����������ۼ���Ϣ
	HTuple          hv_SurfaceFlag;//�ܵı�������,TURE��ʾû���⣬FALSE��ʾ������
	HTuple          hv_SurfaceErrorIndex;//�����������
	HTuple          hv_SurfaceErrorBelongTrips;//�����������

	HTuple          hv_SizeFlag;//�ܳߴ���
	HTuple          hv_SizeErrorIndex;//�ߴ��������
	HTuple          hv_SizeErrorBelongTrips;//�ߴ��������

	HTuple          hv_FoilNum;//��������
	HTuple          hv_TotalLength;
	HTuple          hv_ProductID;//ÿ����Ʒ��ID

	inline int ReadMessage(const HTuple& hvMessage)
	{
		CString strKey;
		{
			strKey.Format("hv_SurfaceFlag");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SurfaceFlag);
			strKey.Format("hv_SurfaceErrorIndex");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SurfaceErrorIndex);

			strKey.Format("hv_SurfaceErrorBelongTrips");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SurfaceErrorBelongTrips);


			strKey.Format("hv_SizeFlag");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SizeFlag);
			strKey.Format("hv_SizeErrorIndex");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SizeErrorIndex);
			strKey.Format("hv_SizeErrorBelongTrips");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_SizeErrorBelongTrips);






			strKey.Format("hv_FoilNum");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_FoilNum);


			strKey.Format("hv_TotalLength");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_TotalLength);
			strKey.Format("hv_ProductID");
			GetMessageTuple(hvMessage, HTuple(strKey), &hv_ProductID);

			strKey.Format("tSecondSurfaceInfo_hv_ErrorType");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_ErrorType);
			strKey.Format("tSecondSurfaceInfo_hv_ErrorNum");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_ErrorNum);
			strKey.Format("tSecondSurfaceInfo_hv_ErrorSumArea");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_ErrorSumArea);

			strKey.Format("tSecondSurfaceInfo_hv_LeftTopRow");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_LeftTopRow);

			strKey.Format("tSecondSurfaceInfo_hv_LeftTopColumn");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_LeftTopColumn);

			strKey.Format("tSecondSurfaceInfo_hv_RightBottomRow");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_RightBottomRow);

			strKey.Format("tSecondSurfaceInfo_hv_RightBottomColumn");
			GetMessageTuple(hvMessage, HTuple(strKey), &tSecondSurfaceInfo.hv_RightBottomColumn);
		}
		return TRUE;
	}
	inline int WriteMessage(const HTuple& hvMessage)
	{
		CString strKey;

		////tAllDetectSize///////
		{
			strKey.Format("hv_SurfaceFlag");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SurfaceFlag);

			strKey.Format("hv_SurfaceErrorIndex");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SurfaceErrorIndex);

			strKey.Format("hv_SurfaceErrorBelongTrips");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SurfaceErrorBelongTrips);


			strKey.Format("hv_SizeFlag");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SizeFlag);

			strKey.Format("hv_SizeErrorIndex");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SizeErrorIndex);

			strKey.Format("hv_SizeErrorBelongTrips");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_SizeErrorBelongTrips);
			



			strKey.Format("hv_FoilNum");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_FoilNum);

			strKey.Format("hv_TotalLength");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_TotalLength);

			strKey.Format("hv_ProductID");
			SetMessageTuple(hvMessage, HTuple(strKey), hv_ProductID);

	



			strKey.Format("tSecondSurfaceInfo_hv_ErrorType");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_ErrorType);
			strKey.Format("tSecondSurfaceInfo_hv_ErrorNum");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_ErrorNum);
			strKey.Format("tSecondSurfaceInfo_hv_ErrorSumArea");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_ErrorSumArea);

			strKey.Format("tSecondSurfaceInfo_hv_LeftTopRow");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_LeftTopRow);

			strKey.Format("tSecondSurfaceInfo_hv_LeftTopColumn");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_LeftTopColumn);

			strKey.Format("tSecondSurfaceInfo_hv_RightBottomRow");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_RightBottomRow);

			strKey.Format("tSecondSurfaceInfo_hv_RightBottomColumn");
			SetMessageTuple(hvMessage, HTuple(strKey), tSecondSurfaceInfo.hv_RightBottomColumn);


		}
		return TRUE;
	}
}FINALRESULT,*PFINALRESULT;

typedef struct tagTSURFACERESULT  //�ۺϵ������б���ȱ�ݼ�����
{
	 HObject ho_SumErrorRegion;   //�������еĴ�������
	 HTuple  hv_SumErrorType;     //�������еĴ�������
	 HTuple  hv_SumErrorBelongTrips;//ȱ����������

}SURFACERESULT,*PSURFACERESULT;


