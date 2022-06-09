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
   BOOL    bEnableUseSpot;//ʹ��
   BOOL    bEnableWriteRow;//ʹ�ü�¼�˶����������
   int     nLowThresh;//����ֵ
   int     nHighThresh;//����ֵ
   int     nSpotNum;//��Ŀ
   double  dCircleStandardArea;//��׼���
   int     nLeftColumn;
   int     nRightColumn;
   double  dCircleParam;
   ////���//////////////////////////////////////////////////////////////////////////
   double dFLPWidth;
   double dFLPWidthDev;
   double dFLPHeight;
   double dFLPHeightDev;
   int    nFLPFrameWarnFrame;
   double dRowResolution;//У׼�����ķֱ���
};
///////��װ�ĵ���� ����������һ��ͼ�����һ����
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
	int              m_nSide;//�ڲ໹�����
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
	 CString         m_strSpotTime[2];//���ʱ��

public:
	void   ReadParam();
	void   WriteParam();
	void   WriteInfo();
	void GetCurrentLocation (HTuple hv_RowCenter, HTuple *hv_DistanceCountOut,int nLeftOrRight=LeftSpotRegion);
	void GetDestRegionInfo (HObject ho_Image, HObject ho_SpotRegion,HTuple hv_Thresh,int nLeftOrRightIndex=LeftSpotRegion);
	void GetRegion(HObject* hoRegion,int nType);
	void Action(HObject& hoImage,SpotOutputInfo* ptOutInfo);//ִ�м��
	void GetCalibrationDestRegionInfo(HObject ho_Image, HObject ho_SpotRegion, HTuple hv_Thresh, int nSide, int nLeftOrRightIndex);
	void CalibrationAction(HObject& hoImage, SpotOutputInfo* ptOutInfo, int nSide);
	void WriteRowLocation(HTuple hv_RowCenter, HTuple hv_Height);
	void WriteRow(HTuple hv_Distance);
public:
	int m_nConntinueErrorNum;
	HTuple m_hvRowResolution[2];
};

