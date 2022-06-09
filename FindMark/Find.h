#pragma once
#include "stdafx.h"

enum e_ProductType
{
	positive=0,//正极
	negative//负极
};
enum e_CoatType
{
	LianXu,//连续料
	JIN,//正常的间歇料

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
	HObject ho_ModleInfo;//0代表图片 1 代表模板区域 2 代表搜索区域
	HObject ho_Line;//转换的线的位置关系
	HTuple  hv_ModelID;
	HTuple  hv_ModelParam;//名字，模板类型，膨胀参数,最小匹配率

}SINGLEMARKINFO,*PSINGLEMARKINFO;
typedef std::vector<SINGLEMARKINFO> MARKLIST;


class  CFindMark : public DHInterface
{
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam);//函数入口

public:
	CFindMark(void);
	~CFindMark(void);
	CFindMark& operator= (const CFindMark& in);

	int Init(NS_SCIFindMark::INITPARAM& tInitParam);//初始化
	//int Find(const HObject& ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//匹配
	
	int ThresholdFind(HObject ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//阈值分割寻找极耳R角
	void GetPositions (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
		HTuple *hv_Row2, HTuple *hv_Column2);//寻找极耳R角
	void GetTestPositions (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
		HTuple *hv_Row2, HTuple *hv_Column2);

public:
	int ThresholdFindIntermission(HObject ho_Image,NS_SCIFindMark::PMARKRESULT ptResult);//采用阈值的方式寻找间歇
	void GetInternalCoatBottom (HObject ho_Image, HObject ho_DetetctRegion, HObject *ho_SelectedRegions1, 
		HTuple *hv_LTRow, HTuple *hv_LTColumn, HTuple *hv_BotRow, HTuple *hv_BotColumn);//寻找间歇区域
	int ShowDialog(HObject ho_Image);//调对话框
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
	double m_dFoilSaveHeight;//极耳保留高度
	int m_nMaxThresh;//分割阈值
	int m_nJXGrayType;//间歇灰度 0黑色 1白色
	int m_nBackLowThresh;//背景低阈值
	int m_nBackHighThresh;//背景高阈值
	double m_dModePosition;//寻找靠近涂布的位置
	double m_dIgnoreDist;//屏蔽距离
	double m_dJXEADev;//间歇公差

	double m_nLocateUpOrDown;//定位间歇顶部或者尾步
	double m_dLocateOffset;//间歇定位偏移

	int FitLine (const HObject& Image,const CLineToolPara& FitLinePara, HObject* Line, 
		double* dStartY,double* dStartX,double* dEndY,double* dEndX,int nLineDisplay = 0);
	CRITICAL_SECTION m_csFindChange;
	double m_dRowResolution;
};



