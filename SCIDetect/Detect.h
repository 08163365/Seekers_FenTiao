#pragma once
#include "LineToolPara.h"

extern CBlendent g_Blendent;
class CSeparate
{
private:
	HTuple m_hvWidth,m_hvHeight;
	HObject m_hoLine,m_hoCross;
	void PreDetect();
	BOOL FindPoint();//���m_tLineMatch.nLineUsed������ߵĿ�ʼ��ͽ�����ͼ�������m_hvRowBegin,m_hvColBegin,m_hvRowEnd,m_hvColEnd;
public:
	void ReadLineMatch();//��ȡ�����
	void SaveLineMatch();//���������
	void SetLineMatch();//���������
	void GenRegion(HObject* Region,HTuple hvIndex);

public:
	void GenFitAT9Region(HObject* hoRegion,HTuple hvLineIndex);//����
	void CorrectedPoint();
	void InterSectionLl(HTuple hvIndex);
	HTuple m_hvRowBegin,m_hvColBegin,m_hvRowEnd,m_hvColEnd;
public:
	int Init(SEPARATEINIT &tIn);                  //��ʼ���㷨
	int Detect(SEPARATEINPUT &tIn,SEPARATEOUTPUT *ptOut);
	double GetDistancepl(double dptRow, double dptCol, HTuple hvLRow1, HTuple hvLCol1, HTuple hvLRow2, HTuple hvLCol2);
	double GetDistancepl(HTuple hvLeftColumn, HTuple hvRightCol);
	
	BOOL ReadParam();
	void FirstFindLine();
	void GetValidRowColumns(HObject ho_Image, HObject *ho_RegionLines1, HObject *ho_Cross, HTuple *hv_RowBegin, HTuple *hv_ColumnBegin, HTuple *hv_RowEnd, HTuple *hv_ColumnEnd);
	void SaveParam();
	void GetCaoWeiInfo(SEPARATEOUTPUT* ptOut);//��ȡ��λ����Ϣ
	int FitLine (const HObject& Image, CLineToolPara& FitLinePara, HObject* Line, 
		double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineSelect,int nLineDisplay = 0);
public:
	
	STANDARDSIZE   m_tStandardSizes;
	LINEMATCH m_tLineMatch;
	SEPARATEOUTPUT*  m_ptOut;
	SEPARATEINPUT*   m_ptIn;
	SEPARATEINIT     m_tInit;
	MEASUREPARAM     m_tMeausreParam;
	HObject          m_hoGrabImage;//�ɼ�ԭʼͼƬ
	HObject          m_hoPretreatImage;//����Ԥ������ͼƬ
	CLineToolPara    m_sLineTool[LINE_COUNT];//�ֱ�Ϊ���������� �ֱ�Ϊ �������� ����
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
	HTupleVector m_hvec_GRRpoint;//����Ϳ�����ߵ�������
	////

	double              m_dCalSpaceDist; //�궨����
	HTuple              m_hvColPose;    //��������
	HTuple              m_hvXPose;      //���������
	int                 m_nPoseCount;   //������Ŀ
	double              m_dCorCoef;     //����ϵ��
	HTuple              m_hvRes;

	int                       m_nCalculateChoose;  //0�Ƿֱ���   1�����Ʊ궨

	BOOL m_bReFindLine;//�Ƿ���Ҫ�����ұ�
};

