#pragma once
enum 
{
	LeftSpotRegion,
	RightSpotRegion,
};
struct SpotOutputInfo
{
	HTuple hvFlag;
	HTuple hvSpotWidth;
	HTuple hvSpotHeight;
	HTuple hvSpotArea;
	HTuple hvXValue;
	HTuple hvYValue;
	double dDistance;
	HObject hoXldRegion;

	void Init()
	{
		 hvFlag=TRUE;
		 hvSpotWidth.Clear();
		 hvSpotHeight.Clear();
		 hvSpotArea.Clear();
		 hvXValue.Clear();
		 hvYValue.Clear();
		 dDistance=0;
		 hoXldRegion.GenEmptyObj();
	}

};
struct SpotParam
{
   BOOL    bEnableUseSpot;//使用
   BOOL    bEnableWriteRow;//使用记录运动方向的坐标
   int     nLowThresh;//低阈值
   int     nHighThresh;//高阈值
   int     nSpotNum;//数目
   double  dCircleStandardArea;//标准面积
   int     nLeftColumn;
   int     nRightColumn;
   double  dCircleParam;
   ////规格//////////////////////////////////////////////////////////////////////////
   double dFLPWidth;
   double dFLPWidthDev;
   double dFLPHeight;
   double dFLPHeightDev;
   int    nFLPFrameWarnFrame;
   double dRowResolution;//校准出来的分辨率
};
///////封装的点检类 作用是输入一个图像，输出一组结果
class CSpotSizeCheck
{
public:
	CSpotSizeCheck(void);
	~CSpotSizeCheck(void);

public:
	SpotParam        m_tSpotParam;
	SpotOutputInfo*  m_ptSpotInfo;
	CString          m_StrSpotCheckPath;
	CString          m_StrSpotCheckDirectory;
	int              m_nSide;//内侧还是外侧
	int              m_nWidth;
	HTuple           m_hvImageWidth;
	HTuple           m_hvImageHeight;
	HObject          m_hoLeftRegion,m_hoRightRegion;
	HObject          m_hoXLDRegion;
	HTuple           m_hvDistanceLocation[2];
	HTuple           m_hvdLastDistance[2];
	BOOL             m_bFirst[2] ;


	 int             m_nLastDistanceCount[2];
	 int             m_ndLastDistance[2];
	 CString         m_strSpotTime[2];//点检时间

public:
	void   ReadParam();
	void   WriteParam();
	void   WriteInfo();
	void GetCurrentLocation (HTuple hv_RowCenter, HTuple *hv_DistanceCountOut,int nLeftOrRight=LeftSpotRegion);
	void GetDestRegionInfo (HObject ho_Image, HObject ho_SpotRegion,HTuple hv_Thresh,int nLeftOrRightIndex=LeftSpotRegion);
	void GetRegion(HObject* hoRegion,int nType);
	void Action(HObject& hoImage,SpotOutputInfo* ptOutInfo);//执行检测
	void GetCalibrationDestRegionInfo(HObject ho_Image, HObject ho_SpotRegion, HTuple hv_Thresh, int nSide, int nLeftOrRightIndex);
	void CalibrationAction(HObject& hoImage, SpotOutputInfo* ptOutInfo, int nSide);
	void WriteRowLocation(HTuple hv_RowCenter, HTuple hv_Height);
	void WriteRow(HTuple hv_Distance);
public:
	int m_nConntinueErrorNum;
	HTuple m_hvRowResolution[2];
};

