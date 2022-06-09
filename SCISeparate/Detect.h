#pragma once
#include "LineToolPara.h"
#include "Judge.in.h"
extern CBlendent g_Blendent;

enum
{
	NOSort,//不筛选
	VerticalSort,//筛选横线
	HorizontalSort,//筛选竖线
	MINCloumns,//筛选列最小5点
	MAXCloumns,//筛选列最大5点
};

enum FitRegionType
{
	CoatFitRegion,
	AT9FitRegion,
};


class CSeparate : public DHInterface
{
private:
	HTuple m_hvWidth,m_hvHeight;
	HObject m_hoLine,m_hoCross;
	void PreDetect();
	BOOL FindPoint();
	void ReLocateLine(HTuple hv_Location);//重新设置线的位置//重新设置线的位置
public:
	LANESET  m_tLane;
	//void ReadLane();//读取通道设置
	void GetSize(int nSourceLineIndex,int nDetLineIndex,double dResolution,HTuple* hv_Value);
	void SetLane();//设置通道设置
	void SortPoints(HTuple& hv_Rows, HTuple& hv_Columns, int nSortType);//去除异常点
	void GetPretreatImage(HObject& hoImage, HTuple& hv_AffineMat2D);
	void WriteGRRData();
	void GetHommat2D(HObject& hoImage, HTuple& hv_Mat2D);
	void FindLocateMissingAgain(HObject hoCoat,HTuple& hv_LocateMiss);
	void GenAT9RegionByPoint(HObject* hoRegion);

	void GetAT9ThreshRegion(HObject ho_Image, HObject* ho_SelectedRegions, HTuple hv_nSide,HTuple hv_dRow, HTuple hv_dColumn);
	void GenFitAT9Region(HObject* hoRegion,HTuple hvLineIndex, FitRegionType nFitRegionType);//测试
	void  GetVariableLength (HObject ho_Image, HObject *ho_EdgeCross, HTuple hv_LineRowStart_Measure_02_0, 
		HTuple hv_LineRowEnd_Measure_02_0, HTuple hv_LineColumnStart_Measure_02_0, HTuple hv_LineColumnEnd_Measure_02_0, 
		HTuple *hv_Width_Measure_02_0);//生成变宽
public:

	LINEMATCH m_tLineMatch;
	void ReadLineMatch();//读取线配对
	void SaveLineMatch();//保存线配对
	void SetLineMatch();//设置线配对
	void GenRegion(HObject* Region,HTuple hvIndex);
	void GenRegionHW(HTuple hvIndex,HTuple* hvHeight,HTuple* hvWidth);//添加计算宽度和高度的函数 20170510
	void PreProcessFoil(HObject* Region,HTuple hvIndex);//极耳预处理
	BOOL DeterMinationSize(int nIndex,int nCoatOrFoil,double dWidth,double dHeight);//尺寸判定
	void GetDestRegionInfo (HObject ho_Image, HObject ho_SpotRegion,HTuple hv_Thresh, HObject *ho_SelectedXLD, 
		HTuple *hv_RowCenter, HTuple *hv_ColumnCenter, HTuple *hv_RectangleWidth, HTuple *hv_RectangleHeiht);
public:

	double SegWidthCalculate(HTuple ColLeft,HTuple ColRight);
	void GetColSeg(HTuple nCol,HTuple &nIndex);
	BOOL FindLineAgain(const HObject& Image,const CLineToolPara& TempFitLinePara,double* dStartY,double* dStartX,double* dEndY,double* dEndX, HObject* Line,int nLineIndex);
	
	void CorrectedPoint();
	void InterSectionLl(HTuple hvIndex);
	HTuple m_hvRowBegin,m_hvColBegin,m_hvRowEnd,m_hvColEnd;
public:
	int Init(SEPARATEINIT &tIn);                  //初始化算法
	int Detect(SEPARATEINPUT &tIn,SEPARATEOUTPUT *ptOut);                         
	int DisplaySetDialog(SEPARATEINPUT & tIn);
	BOOL ReadParam();
	void SaveParam();
	virtual int DoSomething(void* message,void* wParam,void* lParam);
	void CallException(SEPARATEOUTPUT* ptOut);
	int FitLine (const HObject& Image,const CLineToolPara& FitLinePara, HObject* Line, 
		double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineIndex=0,int nLineDisplay = 0);
	void DistancePp_Ex(const HTuple& Row1,const HTuple& Column1,const HTuple& Row2,const HTuple& Column2,
		const double& dRowResolution,const double& dColResolution,HTuple *Distance);
	/*该函数主要是计算线hvInterLine0ID与hvCommoonLineID交点0和hvInterLine1ID与hvCommoonLineID交点的距离*/
	void DistanceP_P(HTuple hvInterLine0ID,HTuple hvInterLine1ID,HTuple hvCommoonLineID,double dRowResolution,double dColumnResolution,HTuple* hvDistance);
	/*该函数主要是计算线hvPointLineID上的点到线hvCommoonLineID的距离*/
	void DistanceP_L(HTuple hvPointLineID,HTuple hvCommoomLineID,double Resolution,HTuple* hvDistance);
	void GetAoBanRegion (HObject ho_Image, HObject ho_ROI_0, HObject *ho_AoBanRegion);
	double GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2);
public:
	
	HTuple m_hvErrorLineIndex;//错误寻边索引
	
	Monitor   m_tMonitor;
	AoBanInfo   m_tAoBanInfo;//提取的凹版信息
	SEPARATEOUTPUT* m_ptOut;
	SEPARATEINPUT* m_ptIn;
	SEPARATEINIT m_tInit;
	PARAMETERS m_tParameters;
	HObject m_hoGrabImage;//采集原始图片
	HObject m_hoPretreatImage;//经过预处理后的图片
	CLineToolPara m_sLineTool[LINE_COUNT];//分别为检测的四条边 分别为 上下左右 极耳
	double m_dRowResolution,m_dColResolution;
	CString m_strSystemVis,m_strMyConfig,m_strCommondh;
	double              m_dCalSpaceDist; //标定板间距
	HTuple              m_hvColPose;    //像素坐标
	HTuple              m_hvXPose;      //物理坐标点
	int                 m_nPoseCount;   //坐标数目
	double              m_dCorCoef;     //修正系数
	HTuple              m_hvRes;

public:
	CSeparate(void);
	~CSeparate(void);
public:
	HObject m_hoThreshAT9Region;
	HTuple m_hvFitLineSR;
	HTuple m_hvFitLineSC;
	HTuple m_hvFitLineER;
	HTuple m_hvFitLineEC;
	HTuple m_hvTimeCount;
	void GetLastFoil (HObject ho_Image, HObject ho_CoatRegion, HObject *ho_LastFoil);

	HTuple m_hvMean;//取样均值
	HObject m_hoReferRegion;//参考区域均值
	HObject m_hoEdageCrossRegion;//参考区域均值

	STANDARDSIZE m_tStandardSize;

};

