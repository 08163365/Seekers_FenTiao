#pragma once
#include "stdafx.h"

typedef struct tagTINITJUDGE//尺寸结果
{
	CString strProductName;
	int     nSN;
}INITJUDGE,*PINITJUDGE;

typedef struct tagTALLDETECTSIZE//尺寸结果
{
	HTuple hv_HeightTuple;//极片长度
	HTuple hv_AllPartsFlag;//整条极片中每张片的尺寸标志位汇总
	HTuple hv_WidthTuple;//极片宽度
	HTuple hv_FoilWidthTuple;//极耳宽度
	HTuple hv_BaiBianTuple;//白边宽度
	HTuple hv_FoilHeightTuple;//极耳的高度 行方向--
	HTuple hv_LocateMissingTuple;

	HTuple hv_UpCWSizePartFlag;   //上面宽度
	HTuple hv_DownCWSizePartFlag;//下面宽度

	HTuple hv_UpLocateSizePartFlag;//正面的定
	HTuple hv_DownLocateSizePartFlag;//反面的标志

	HTuple hv_CHSizePartFlag;//间距

}ALLDETECTSIZE,*PALLDETECTSIZE;


typedef struct tagTCAOWEISIZE//槽位尺寸
{
	HTuple hvCaoWeiHeight;
	HTuple hvCaoWeiHeightDev;
	HTuple hvCaoWeiWidth;
	HTuple hvCaoWeiWidthDev;
	HTuple hvCaoWeiMD;
	HTuple hvCaoWeiMDDev;
	HTuple hvCaoWeiTDDev;
	HTuple hvCaoWeiTDTuple;//水平方向的位置

	HTuple hvCaoWeiMDCuoWeiDev;//运动方向的错位公差
	HTuple hvCaoWeiTDCuoWeiDev;//水平方向的错位公差

}CAOWEISIZE,*PCAOWEISIZE;



typedef struct tagTSTANDARDSIZE//标准尺寸
{
	HTuple nCellNumber;//单条包含片的个数
	HTuple nErrorNum;//单条允许尺寸错误的最大数目
	HTuple dCorrectMax;//修正
	HTuple dCorrectMin;
	///产品规格
	HTuple hv_ProductType;//产品类型0 阴极 1阳极
	HTuple hv_CoatType;//涂布类型 0是连续 1是间歇
	HTuple hv_CaoWeiType;//槽位类型 0 不存在槽位 1正面槽位 2 反面槽位 3 双面存在槽位
	HTuple hv_OAPType;//OAP类型 0 不存在OAP 1正面OAP 2 反面OAP 3 双面存在OAP
	HTuple hvTripNum;//条数
	HTuple hvZhiDaiGray;//纸带的灰度
	/////
    HTuple bEnableTotalLength;//是否检测
	HTuple dTotalLength;//总长
	HTuple dTotalLengthDev;//总长偏差
	HTuple bEnableTotalLengthLabel;//总长贴标


	HTuple hv_CoatHeight;       //单片涂布高度
	HTuple dCoatHeightDev;     //单片涂布偏差
	HTuple bEnableCoatHeight;  //是否检测
	HTuple hvCoatHeightNGNum;//涂布高度NG数目

	HTuple bEnableCoatWidth;  //是否检测


	
	HTuple dCoatWidth;       //单片宽度 列方向
	HTuple dCoatWidthDev;    //单片涂布宽度
	HTuple hvCoatWidthNGNum;//涂布宽度NG数目
	HTuple bEnableCoatWidthLabel;//涂布宽度贴标
	HTuple dCoatWidthAddValue;//加宽值
	


	//极耳宽度检测
	HTuple bEnableFoilWidth;  //是否检测
	HTuple dFoilWidth;    //单片极耳宽度 列方向
	HTuple dFoilWidthDev; //单片极耳宽度偏差
	
	///AT9尺寸检测
	HTuple bEnableLocateMissing;  //是否检测
	HTuple dLocateMissing;//定位不齐标准值
	HTuple dLocateMissingDev;//定位不齐偏差
	HTuple hvLocateMissingNGNum;//定位不齐NG数目
	HTuple bEnableLocateMissingLabel;//前相机定位不齐贴标


	//////不含间歇总长
	HTuple bEnableBHJXTotalLength;  //是否检测不含间歇总长
	HTuple dBHJXTotalLength;        //不含间歇总长
	HTuple dBHJXTotalLengthDev;     //不含间歇总长偏差
	HTuple bEnableBHJXLabel;        //贴标使能



	//////正面间歇高度 
	HTuple bEnableUpImHeight;         //是否检测正面间歇高度
	HTuple dUpIntermissionHeight;     //正面间歇高度标准值
	HTuple dUpIntermissionHeightDev;  //正面间歇高度公差
	HTuple bEnableUpIntermissionLabel;//贴标使能

	//////反面间歇高度（A面） 
	HTuple bEnableDownImHeight;         //是否检测反面间歇高度
	HTuple dDownIntermissionHeight;     //反面间歇高度标准值
	HTuple dDownIntermissionHeightDev;  //反面间歇高度公差
	HTuple bEnableDownIntermissionLabel;//贴标使能

	///总宽////
	HTuple    bEnableTotalWidth;
	HTuple   dTotalCoatWidth;
	HTuple    dTotalCoatWidthDev;
	HTuple    bToatlWidthLabel;

	////反面总模长
	HTuple bEnableBHJXLength2;
	HTuple dBHJXLength2;
	HTuple dBHJXLengthDev2;
	HTuple bBHJXLabel2;


	///头部错位
	HTuple bEnableHeadCuoWei;
	HTuple dHeadCuoWei;
	HTuple dHeadCuoWeiDev;
	HTuple bHeadCuoWeiLabel;

	///尾部错位
	HTuple bEnableTailCuoWei;
	HTuple dTailCuoWei;
	HTuple dTailCuoWeiDev;
	HTuple bTailCuoWeiLabel;
	



	//////错位值/////
	HTuple bEnableABMissValue;  //是否检测AB错位
	HTuple dABMissValue;//AB错位值
	HTuple dABMissValueDev;//AB错位公差
	HTuple bEnableABMissValueLabel;//AB错位贴标使能

	///新加AT9AB面错位报警
	HTuple bEnableCuoWei;//检测错位
	HTuple dCuoWeiMax;//最大值
	HTuple hvCutPosition;//裁切位置


	///定位位置
	//HTuple hvLocateUpAndDown;//定位的位置
	HTuple hvExistJXType;//间歇存在的类型 0：仅正面间歇  1：仅反面间歇 2：双面间歇
	HTuple hvGongYiType;//工艺类型 0：ASC1 1:M6S 2:MMT 3:STS1 4:STS2


	HTuple hvQiTouXianChu;//0 定位在上  1表示齐头先出 定位在下
	HTuple hvChangMoZaiShang;//



	CAOWEISIZE tUpCaoWeiParam;
	CAOWEISIZE tDownCaoWeiParam;




}STANDARDSIZE,*PSTANDARDSIZE;

typedef struct tagTSECONDSURFACEINFO  //二次判断时需要的表面缺陷信息
{
	HTuple hv_ErrorNum;//缺陷的数目
	HTuple hv_ErrorSumArea;//缺陷的总面积
	HTuple hv_ErrorType;//单条所有的错误类型
	HTuple hv_LeftTopRow;//所有缺陷的左上角行数
	HTuple hv_LeftTopColumn;//所有缺陷的左上角列坐标
	HTuple hv_RightBottomRow;//所有缺陷的右下角行坐标
	HTuple hv_RightBottomColumn;//所有缺陷的右下角列坐标

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
typedef struct tagTSTANDARDSURFACE//是否需要特殊判断的表面缺陷参数
{
	HTuple nErrorCount;//一共有多少种缺陷类型
	HTuple nSecErrorCount;//有多少种需要做二次判断
	//需要做二次判断的缺陷索引
	HTuple hv_ErrorIndex;
	//累加判断，如果所有缺陷加起来
	HTuple hv_TotalMaxNumber;      //超过该个数认为是NG
	HTuple hv_TotalMaxArea;        //如果累加面积大于该面积认为NG

	HTuple hv_TotalMaxNumberTwo;      //超过该个数认为是NG
	HTuple hv_TotalMaxAreaTwo;        //如果累加面积大于该面积认为NG

	HTuple hv_SingleMaxWidth;		//最大宽度
	HTuple hv_EnablePopWindow;      //单个缺陷弹窗报警使能开启
	//如果单个缺陷超出限制也认为NG
	//HTuple hv_SingleMaxDiameter;    //最大直径	
	//HTuple hv_SingleMaxLength;     //最大长度
	//HTuple hv_SingleMaxWidth;		//最大宽度
	
}STANDARDSURFACE,*PSTANDARDSURFACE;

typedef struct tagTSIZERESULT//尺寸标志
{

	HTuple hv_SumFlag;//产品总长标志，TURE为OK，FALSE为NG
	HTuple hv_CoatHeightFlag;//尺寸标志 极片长度，TURE为OK，FALSE为NG
	HTuple hv_CoatWidthFlag;//涂布宽度标志 TURE为OK，FALSE为NG
	HTuple hv_FoilWidthFlag;//极耳宽度 TURE为OK，FALSE为NG
	HTuple hv_FoilHeightFlag;//极耳高度 TURE为OK，FALSE为NG
	HTuple hv_TsizeFlag;//错切标志位  TURE为OK，FALSE为NG
	HTuple hv_BaibianFlag;//白边标志位 TURE为OK，FALSE为NG
	HTuple hv_DiePianMarkWidthFlag;//叠片U形切口宽度 TURE为OK，FALSE为NG
	HTuple hv_DiePianMarkHeightFlag;//叠片U形切口高度 TURE为OK，FALSE为NG
	HTuple hv_DiePianMarkDiffFlag;//叠片U形口的对齐度-TURE为OK，FALSE为NG
	HTuple hv_DistanceBorderFlag;//极边距标志位-TURE为OK，FALSE为NG
	HTuple hv_LocateMissingFlag;//定位不齐标志位-TURE为OK，FALSE为NG
}SIZERESULT,*PSIZERESULT;
typedef struct tagTFINALRESULT//最终结果
{
    SECONDSURFACEINFO tSecondSurfaceInfo;//二次判断所需的整条累计信息
	HTuple          hv_SurfaceFlag;//总的表面检测结果,TURE表示没问题，FALSE表示有问题
	HTuple          hv_SurfaceErrorIndex;//表面错误索引
	HTuple          hv_SurfaceErrorBelongTrips;//表面错误索引

	HTuple          hv_SizeFlag;//总尺寸结果
	HTuple          hv_SizeErrorIndex;//尺寸错误索引
	HTuple          hv_SizeErrorBelongTrips;//尺寸归属条数

	HTuple          hv_FoilNum;//极耳个数
	HTuple          hv_TotalLength;
	HTuple          hv_ProductID;//每条产品的ID

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

typedef struct tagTSURFACERESULT  //综合单条所有表面缺陷及类型
{
	 HObject ho_SumErrorRegion;   //单条所有的错误区域
	 HTuple  hv_SumErrorType;     //单条所有的错误类型
	 HTuple  hv_SumErrorBelongTrips;//缺陷隶属于条

}SURFACERESULT,*PSURFACERESULT;


