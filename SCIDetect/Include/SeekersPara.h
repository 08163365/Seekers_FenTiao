
#include <afxtempl.h>
#ifndef _DEFINE_GEICHECK
#define _DEFINE_GEICHECK
#include <vector>
#include "LineToolPara.h"

#define LINE_COUNT  22

const int MAX_LANE  = 1; //������16��
#define M_PI       3.14159265358979323846


// ��������ö��
enum CLASSTYPE
{
	JIAODAI_C,
	LOUBO_C,
	TUOTAN_C,
	TIAOHEN_C,
	QIPAO_C,
	BAIDIAN_C,
	HEIDIAN_C,
	OK_C,
	ClassTotalNum,
};
const CString g_strClassName[] =
{
	"����",
	"©��",
	"��̼",
	"����",
	"����",
	"�׵�",
	"�ڵ�",
	"OK",
};
struct MLPParam
{
	HTuple hvTrainDataDict;//ѵ�����ݼ���
	HTuple hvMLPHandle;//����������
	HTuple hvRegionFeature;//��������
	HTuple hvGrayFeature;//�Ҷ�����
	HTuple hvFeatureName;//��������
	HTuple hvFeatureLength;//��������
	int    nLeftColumn;
	int    nRightColumn;
};



struct ExmaraParam
{
	double dAlpha;
	int nLowThresh;
	int nHighThresh;
	double dMinArea;
};
typedef struct tagTBlackRegion
{
	HObject ho_JXRegion;
	HObject hoTripRegion;//����ʾ��ͼ
	HObject hoYaHenPingBiRegion;//���Ъ�����ѹ��
	void InitObject()
	{
		GenEmptyObj(&hoTripRegion);
		GenEmptyObj(&ho_JXRegion);
		GenEmptyObj(&hoYaHenPingBiRegion);
	}
}BlackRegion,*PBlackRegion;

	typedef struct tagTMYREGION
	{
		HObject ho_CoatUnion;
		HObject ho_CerimicUnion;
		HObject ho_FoilUnion;

		void Clear()
		{
			GenEmptyObj(&ho_CoatUnion);
			GenEmptyObj(&ho_CerimicUnion);
			GenEmptyObj(&ho_FoilUnion);
		}
	}MYREGION,*PMYREGION;
typedef struct tagTSINGLEMODEL
{
	HObject hoModelInfo;//0 ԭͼ 1 ģ������ 2 ��������
	HTuple  hvModelID;

}SINGLEMODEL,*PSINGLEMODEL;
typedef std::vector<SINGLEMODEL> MARKVECTOR;
typedef struct tagTLINEMATCH
{
	int    nLineUsed;//��������
	int    nCoatCount;//Ϳ�����������Ҫ�ߵĸ���
	int    nFoilCount;//�������������Ҫ�ߵĸ���
	int    nCeramicCount;//�մ����������Ҫ�ߵĸ���
	int    nStaggerCount;//��λ���������Ҫ�ߵĸ���

	int     nCoatLeftStartIndex;
	int     nFoilLeftStartIndex;
	int     nCeramicLeftStartIndex;


	std::vector<int> vCoat1;
	std::vector<int> vCoat2;//1��2��һ�� ������Ϳ������
	std::vector<int> vFoil1;
	std::vector<int> vFoil2;//1��2��һ�� �����Ǽ�������
	std::vector<int> vCeramic1;
	std::vector<int> vCeramic2;//1��2��һ�� �������մ�����


	std::vector<int> vStagger1;
	std::vector<int> vStagger2;//1��2��һ�� �����Ǽ���λ����

	//����LineMatch����гߴ��ж�
	std::vector<double> vCoatWidthMin;
	std::vector<double> vCoatWidthMax;
	std::vector<double> vCoatWidthOffset;

	std::vector<double> vFoilWidthMin;
	std::vector<double> vFoilWidthMax;
	std::vector<double> vFoilWidthOffset;

	std::vector<double> vCeramicWidthMin;
	std::vector<double> vCeramicWidthMax;
	std::vector<double> vCeramicWidthOffset;

	std::vector<double> vStaggerOffset;//��λ����



}LINEMATCH,*PLINEMATCH;


typedef struct  tagTMEASUREPARAM
{
	//BOOL bPretreat;//�Ƿ����Ԥ����
	//int  nSizePretreat;//Ԥ����ߴ��С
	//BOOL bUseLineRefer;//ʹ�òο��ߣ������ڵ�������Ĳ�ȫͿ�������
	//int  nLineReferSN;//����������Ϊ�ο���
	//BOOL bJudgeCoat;//�ж�Ϳ���Ƿ�Ϳ��
	int  nCoatThres;//�ж�Ϳ����ֵ
	//int nCameraSize;//��K�����
	//HTuple hv_ColResolution;//���еķֱ���
	//int nCalibType;
	HObject hoCaoweiRegions;//��λ��ȡ����

}MEASUREPARAM,*PMEASUREPARAM;


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


#endif _DEFINE_GEICHECK



struct TuoWeiParam//��̼����
{

	///��Ъ�����̼
	BOOL   bUseDetect;//����
	int    nJXTapeLTMinThresh;//��Ъ������̼����С��ֵ
	int    nJXTapeLTMaxThresh;//��Ъ������̼�������ֵ
	double dJXTapeLTMinArea;//��Ъ������̼����С���
	double  dJXTuoWeiMaxHeight;//��β���߶�
};
struct AT9Param//AT9����
{

	BOOL   bUseDetect;//����
	////©��////
	int    nAT9LMinThresh;//AT9��С��ֵ
	double dAT9LMinWidth;//AT9��С���
	double dAT9LMinHeight;//AT9��С�߶�
	/////AT9����ȱ��
	int    nAT9DarkMaxThresh;//��С�Ҷ�
	double dAT9DarkMinWidth;//AT9��С��ȱ�ݻҶ�
	double dAT9DarkMinHeight;//��С�߶�
};


struct PretreatParam//Ԥ����
{
	double dBorderWidth;//�߽���mm
	double dRightBorderWidth;//�Ҳ�߽���mm
	double dScaleFactor;//ͼ������ϵ�� 
	double  dMidLightCof;//������������ȡϵ��ȡֵ 0-1
	double dMidDarkCof;//���İ�������ȡϵ�� 0-1
	double dBorderLightCof;//������������ȡϵ��0-1
	double dBorderDarkCof;//���İ�������ȡϵ��0-1
	double dYaHenPingBiKuanDu;//ѹ�����ο��
	BOOL   bEnbleUseClassyfy;//���÷�����
	double dMLPSongjindu;//MLP�ɽ���
};

struct JiaoDaiParam
{
	BOOL   bUseDetect;//����
	int    nDarkThresh;//����ȡ��ֵ
	int    nLightThresh;//����ֵ
	double dMinArea;//��С���
};
struct LeakFoilParam//©������
{
	BOOL bUseDetect;//����
	///��·1
	int nThresh;
	double dMinArea;



};
struct BubbleParam//���ݲ���
{
	BOOL   bUseDetect;//����
	int    nDynThresh;//�Աȶ�
	double dMinArea;//��С���
	double dMinCircularity;//��СԲ��
};


struct DarkLineParam//���߲���
{
	BOOL   bUseDetect;//����
	int    nDynThresh;//�Աȶ�
	double dMinWidth;//��С���
	double dMinLengthWidthRatio;//��С�����
};
struct LightDotParam//�װ߲���
{
	BOOL   bUseDetect;//����
	int    nLightDotDynThresh;//��ֵ����
	double dMinArea;//��С���
};

struct DarkDotParam//�ڰ߲���
{
	BOOL   bUseDetect;//����
	int    nDarkDotDynThresh;//��ֵ����
	double dMinArea;//��С���
};

struct TapeParam//��������
{
	BOOL   bUseDetect;//����
	int    nTapeDynThresh;//��̼�Աȶ�
	double  dDevFactor;//ƫ������
	double dLTMinArea;//����������С���

	int    nTapeThresh;//��ֵ�ָ�
	double dTapeThreshMinArea;//��С���
};

struct FoilParam//����������
{
	BOOL   bUseDetect;//����
	int    nMaxDarkThresh;//��ֵ����
	double dMinDarkArea;//��С���
};


struct  SingleFeature//�Զ���ȱ����
{
	CString StrSingleFeatureName;
	double dMin;
	double dMax;

};

struct  SingleErrorGroup//�Զ���ȱ����
{
	CString strErrorName;//������Ŀ����
	int    nSingleGroupNum;//��ǰ��Ŀ���ж�����ɸѡ������
	std::vector<SingleFeature>Vec_tSingleGroupFeatures;

};
struct  FeatureSelect//�Զ���ȱ����
{
	std::vector<SingleErrorGroup> Vec_FSelects;//�Զ���ȱ����������
	int nGroupNum;//�Զ���ȱ����Ŀ
};
typedef struct  tagTPARAMETERS
{
	BOOL tUseDefectMasterSwitch;
	PretreatParam tPretreatParam;
	LeakFoilParam tLeakFoilParam;
	BubbleParam   tBubbleParam;
	DarkLineParam tDarkLineParam;
	LightDotParam tLightDotParam;
	DarkDotParam  tDarkDotParam;
	TapeParam     tTapeParam;
	JiaoDaiParam  tJiaoDaiParam;
	FoilParam     tFoilParam;
	TuoWeiParam   tTuoWeiParam;
	AT9Param      tAT9Param;
	FeatureSelect tFeatureSelect;
}PARAMETERS, *PPARAMETERS;

