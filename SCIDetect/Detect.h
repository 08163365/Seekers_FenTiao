#pragma once
#include "LineToolPara.h"

extern CBlendent g_Blendent;
class CSeparate
{
private:
	HTuple m_hvWidth,m_hvHeight;
	HObject m_hoLine,m_hoCross;
	void PreDetect();
	BOOL FindPoint();//输出m_tLineMatch.nLineUsed条拟合线的开始点和结束点图像坐标给m_hvRowBegin,m_hvColBegin,m_hvRowEnd,m_hvColEnd;
public:
	void ReadLineMatch();//读取线配对
	void SaveLineMatch();//保存线配对
	void SetLineMatch();//设置线配对
	void GenRegion(HObject* Region,HTuple hvIndex);

public:
	void GenFitAT9Region(HObject* hoRegion,HTuple hvLineIndex);//测试
	void CorrectedPoint();
	void InterSectionLl(HTuple hvIndex);
	HTuple m_hvRowBegin,m_hvColBegin,m_hvRowEnd,m_hvColEnd;
public:
	int Init(SEPARATEINIT &tIn);                  //初始化算法
	int Detect(SEPARATEINPUT &tIn,SEPARATEOUTPUT *ptOut);
	double GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2);
	double GetDistancepl(HTuple hvLeftColumn, HTuple hvRightCol);
	
	BOOL ReadParam();
	void FirstFindLine();
	void GetValidRowColumns(HObject ho_Image, HObject *ho_RegionLines1, HObject *ho_Cross, HTuple *hv_RowBegin, HTuple *hv_ColumnBegin, HTuple *hv_RowEnd, HTuple *hv_ColumnEnd);
	void SaveParam();
	void GetCaoWeiInfo(SEPARATEOUTPUT* ptOut);//获取槽位的信息
	int FitLine (const HObject& Image, CLineToolPara& FitLinePara, HObject* Line, 
		double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineSelect,int nLineDisplay = 0);
public:
	
	STANDARDSIZE   m_tStandardSizes;
	LINEMATCH m_tLineMatch;
	SEPARATEOUTPUT*  m_ptOut;
	SEPARATEINPUT*   m_ptIn;
	SEPARATEINIT     m_tInit;
	MEASUREPARAM     m_tMeausreParam;
	HObject          m_hoGrabImage;//采集原始图片
	HObject          m_hoPretreatImage;//经过预处理后的图片
	CLineToolPara    m_sLineTool[LINE_COUNT];//分别为检测的四条边 分别为 上下左右 极耳
	CString          m_strSystemVis,m_strMyConfig,m_strCommondh;

public:
	void GetSize(int nSourceLineIndex,int nDetLineIndex,double dResolution,HTuple* hv_Value);
	CString m_strHalconCalibInfoPath;
	
	BOOL m_bTitleImage;
public:
	CSeparate(void);
	~CSeparate(void);
	void FlushFlag();
public:
	double m_dRowResolution,m_dColResolution;
	HTuple m_hvFitLineSR;
	HTuple m_hvFitLineSC;
	HTuple m_hvFitLineER;
	HTuple m_hvFitLineEC;
	HTuple m_hvErrorLineIndex;
	CString  m_strFilePath;

	BOOL    m_bExistJXFlag;
	BOOL    m_bExistOAPFlag;
	BOOL    m_bExistCaoWeiFlag;
	BOOL    m_bExistJXUpDownFlag;
	HTupleVector m_hvec_GRRpoint;//存连涂所有线的列坐标
	////

	double              m_dCalSpaceDist; //标定板间距
	HTuple              m_hvColPose;    //像素坐标
	HTuple              m_hvXPose;      //物理坐标点
	int                 m_nPoseCount;   //坐标数目
	double              m_dCorCoef;     //修正系数
	HTuple              m_hvRes;

	int                       m_nCalculateChoose;  //0是分辨率   1是条纹标定

	BOOL m_bReFindLine;//是否需要重新找边
};

