#pragma once
#include "stdafx.h"

enum e_ProductType
{
	positive=0,//����
	negative//����
};
enum e_CoatType
{
	LianXu,//������
	JIN,//�����ļ�Ъ��

};
class CriticalSectionHelper
{
public:
	CriticalSectionHelper(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}

	~CriticalSectionHelper()
	{
		LeaveCriticalSection(m_pcs);
		m_pcs = NULL;
	}

private:
	CRITICAL_SECTION *m_pcs;
};
typedef struct tagTSINGLEMARKINFO
{
	HObject ho_ModleInfo;//0����ͼƬ 1 ����ģ������ 2 ������������
	HObject ho_Line;//ת�����ߵ�λ�ù�ϵ
	HTuple  hv_ModelID;
	HTuple  hv_ModelParam;//���֣�ģ�����ͣ����Ͳ���,��Сƥ����

}SINGLEMARKINFO,*PSINGLEMARKINFO;
typedef std::vector<SINGLEMARKINFO> MARKLIST;


class  CFindMark : public DHInterface
{
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam);//�������

public:
	CFindMark(void);
	~CFindMark(void);
	CFindMark& operator= (const CFindMark& in);

	int Init(NS_SCIFindMark::INITPARAM& tInitParam);//��ʼ��
	//int Find(const HObject& ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//ƥ��
	
	int ThresholdFind(HObject ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//��ֵ�ָ�Ѱ�Ҽ���R��
	void GetPositions (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
		HTuple *hv_Row2, HTuple *hv_Column2);//Ѱ�Ҽ���R��
	void GetTestPositions (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
		HTuple *hv_Row2, HTuple *hv_Column2);

public:
	int ThresholdFindIntermission(HObject ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//������ֵ�ķ�ʽѰ�Ҽ�Ъ
	void GetInternalCoatBottom (HObject ho_Image, HObject ho_DetetctRegion, HObject *ho_SelectedRegions1, 
		HTuple *hv_LTRow, HTuple *hv_LTColumn, HTuple *hv_BotRow, HTuple *hv_BotColumn);//Ѱ�Ҽ�Ъ����
	int ShowDialog(HObject ho_Image);//���Ի���
public:
	int FindMult(const HObject& ho_Image, NS_SCIFindMark::PMARKRESULT ptResult);
	void ReadMark();
	void WriteMark();
	void ClearMark();
	BOOL SaveJigsawImage(const int& nGrabberSN,const HObject& ho_ImageSave);
	void ReLocate(HTuple & hv_Location);
	CString m_strConfigPath;
	CString m_strConfigFile;

	MARKLIST   m_tMarkList;
	CString m_strLastErrorMessage;

	NS_SCIFindMark::INITPARAM  m_tInitParam;
	int m_nType;
	double m_dFoilSaveHeight;//���������߶�
	int m_nMaxThresh;//�ָ���ֵ
	int m_nJXGrayType;//��Ъ�Ҷ� 0��ɫ 1��ɫ
	int m_nBackLowThresh;//��������ֵ
	int m_nBackHighThresh;//��������ֵ
	double m_dModePosition;//Ѱ�ҿ���Ϳ����λ��
	double m_dIgnoreDist;//���ξ���
	double m_dJXEADev;//��Ъ����

	double m_nLocateUpOrDown;//��λ��Ъ��������β��
	double m_dLocateOffset;//��Ъ��λƫ��

	int FitLine (const HObject& Image,const CLineToolPara& FitLinePara, HObject* Line, 
		double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineDisplay = 0);
	CRITICAL_SECTION m_csFindChange;
	double m_dRowResolution;
};



