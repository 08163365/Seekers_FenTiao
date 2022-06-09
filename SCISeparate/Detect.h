#pragma once
#include "LineToolPara.h"
#include "Judge.in.h"
extern CBlendent g_Blendent;

enum
{
	NOSort,//��ɸѡ
	VerticalSort,//ɸѡ����
	HorizontalSort,//ɸѡ����
	MINCloumns,//ɸѡ����С5��
	MAXCloumns,//ɸѡ�����5��
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
	void ReLocateLine(HTuple hv_Location);//���������ߵ�λ��//���������ߵ�λ��
public:
	LANESET  m_tLane;
	//void ReadLane();//��ȡͨ������
	void GetSize(int nSourceLineIndex,int nDetLineIndex,double dResolution,HTuple* hv_Value);
	void SetLane();//����ͨ������
	void SortPoints(HTuple& hv_Rows, HTuple& hv_Columns, int nSortType);//ȥ���쳣��
	void GetPretreatImage(HObject& hoImage, HTuple& hv_AffineMat2D);
	void WriteGRRData();
	void GetHommat2D(HObject& hoImage, HTuple& hv_Mat2D);
	void FindLocateMissingAgain(HObject hoCoat,HTuple& hv_LocateMiss);
	void GenAT9RegionByPoint(HObject* hoRegion);

	void GetAT9ThreshRegion(HObject ho_Image, HObject* ho_SelectedRegions, HTuple hv_nSide,HTuple hv_dRow, HTuple hv_dColumn);
	void GenFitAT9Region(HObject* hoRegion,HTuple hvLineIndex, FitRegionType nFitRegionType);//����
	void  GetVariableLength (HObject ho_Image, HObject *ho_EdgeCross, HTuple hv_LineRowStart_Measure_02_0, 
		HTuple hv_LineRowEnd_Measure_02_0, HTuple hv_LineColumnStart_Measure_02_0, HTuple hv_LineColumnEnd_Measure_02_0, 
		HTuple *hv_Width_Measure_02_0);//���ɱ��
public:

	LINEMATCH m_tLineMatch;
	void ReadLineMatch();//��ȡ�����
	void SaveLineMatch();//���������
	void SetLineMatch();//���������
	void GenRegion(HObject* Region,HTuple hvIndex);
	void GenRegionHW(HTuple hvIndex,HTuple* hvHeight,HTuple* hvWidth);//��Ӽ����Ⱥ͸߶ȵĺ��� 20170510
	void PreProcessFoil(HObject* Region,HTuple hvIndex);//����Ԥ����
	BOOL DeterMinationSize(int nIndex,int nCoatOrFoil,double dWidth,double dHeight);//�ߴ��ж�
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
	int Init(SEPARATEINIT &tIn);                  //��ʼ���㷨
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
	/*�ú�����Ҫ�Ǽ�����hvInterLine0ID��hvCommoonLineID����0��hvInterLine1ID��hvCommoonLineID����ľ���*/
	void DistanceP_P(HTuple hvInterLine0ID,HTuple hvInterLine1ID,HTuple hvCommoonLineID,double dRowResolution,double dColumnResolution,HTuple* hvDistance);
	/*�ú�����Ҫ�Ǽ�����hvPointLineID�ϵĵ㵽��hvCommoonLineID�ľ���*/
	void DistanceP_L(HTuple hvPointLineID,HTuple hvCommoomLineID,double Resolution,HTuple* hvDistance);
	void GetAoBanRegion (HObject ho_Image, HObject ho_ROI_0, HObject *ho_AoBanRegion);
	double GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2);
public:
	
	HTuple m_hvErrorLineIndex;//����Ѱ������
	
	Monitor   m_tMonitor;
	AoBanInfo   m_tAoBanInfo;//��ȡ�İ�����Ϣ
	SEPARATEOUTPUT* m_ptOut;
	SEPARATEINPUT* m_ptIn;
	SEPARATEINIT m_tInit;
	PARAMETERS m_tParameters;
	HObject m_hoGrabImage;//�ɼ�ԭʼͼƬ
	HObject m_hoPretreatImage;//����Ԥ������ͼƬ
	CLineToolPara m_sLineTool[LINE_COUNT];//�ֱ�Ϊ���������� �ֱ�Ϊ �������� ����
	double m_dRowResolution,m_dColResolution;
	CString m_strSystemVis,m_strMyConfig,m_strCommondh;
	double              m_dCalSpaceDist; //�궨����
	HTuple              m_hvColPose;    //��������
	HTuple              m_hvXPose;      //���������
	int                 m_nPoseCount;   //������Ŀ
	double              m_dCorCoef;     //����ϵ��
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

	HTuple m_hvMean;//ȡ����ֵ
	HObject m_hoReferRegion;//�ο������ֵ
	HObject m_hoEdageCrossRegion;//�ο������ֵ

	STANDARDSIZE m_tStandardSize;

};

