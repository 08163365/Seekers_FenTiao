
#include <afxtempl.h>
#include <vector>
#ifndef _DEFINE_GEICHECK
#define _DEFINE_GEICHECK
#define LINE_COUNT 8
#define MAX_LANE  2//����������
#define MAX_LINEMATCH 8

enum e_GrabberType
{
	LINE=0,
	AREA,
};
enum e_ProductType
{
	positive=0,
	negative
};
enum e_CoatType
{
	D_TwoCam_OneTrip,//��Ƭ�� ˫�������
	M_OneCam_OneTrip,//�༫�� ���������
	M_OneCam_TwoTrip,//�༫�� �����˫��
	M_TwoCam_OneTrip,//�༫�� ˫�������
	M_TwoCam_TwoTrip,//�༫�� ˫���˫��
	M_TwoCam_Intermission
};
typedef struct  tagTPARAMETERS
{
	BOOL bPretreat;//�Ƿ����Ԥ����
	int  nSizePretreat;//Ԥ����ߴ��С
	int  nXSizePretreat;//Ԥ����ˮƽ�ߴ��С
	int nHcalibFlag;//0�ر� 1����
	int nFitRegionMode;//��ϵ���������
	e_GrabberType emGrabberType;
	e_CoatType  emCoatType;
	e_ProductType emProductType;
	int nLaneCount;
	int nMaxDiffColumn;//��ʼ�����̵��в�ֵ

	int    nSelectValueMode;//ȡ�㷽ʽ
	double dLocateMissingOffset;//��λ���벹��ֵ
	double dMaxAoBanDeviation;//ʹ����ֵ�ָ�������г��������ƫ��

	BOOL  bUseGRR;//ʹ��GRR
	int   nSelectNum;//ѡȡ����

	//////����������׼
	//double  dStandardWidth;//��¼��ǰ�Ļ�׼
	double  dStandardLeftLocation;//��¼��ǰ������׼
	double  dStandardRightLocation;//��¼��ǰ���Ҳ��׼
	HTuple  hv_Mat2D;
    ///////��ȡ����Ķ�����ֵ//////
	int     bEnaleUseSegAoBan;//
    int     bFindManualOrAuto;//�ֶ������Զ�
	double  dExtractFoilRegionWidth;//������ȡ������
	int     nExtractFoilThresh;//������ȡ��ֵ
	double  dExtractAobanRegionWidth;//����������
	int     nExtractAobanThresh;//������ȡ��ֵ

	///���ʹ��
	BOOL   bEnableVariableWidth;//�����
	double  dRelateOffset;//���ƫ��
}PARAMETERS,*PPARAMETERS;

struct AoBanInfo//������ȡ��Ϣ
{
	HObject hoFoilDetectRegion;//��ȡ��������
	HObject hoExtractFoilRegion;//��ȡ���ļ�������
	HObject hoAoBanRegion;//��������
	HObject hoLineBetAoBanAndFoil;//����ͼ������紦
	HTuple  hvLocateDistance;
	AoBanInfo()
	{
		hoFoilDetectRegion.GenEmptyObj();
		hoExtractFoilRegion.GenEmptyObj();
		hoAoBanRegion.GenEmptyObj();
		hoLineBetAoBanAndFoil.GenEmptyObj();
		hvLocateDistance.Clear();
	}
};

struct Monitor//�ȶ��Լ��
{
	HObject hoLeftXld;
	HObject hoRightXld;
	double dLeftHeight;
	double dLeftWidth;
	double dRightHeight;
	double dRightWidth;
	void ClearAll()
	{
		hoLeftXld.GenEmptyObj();
		hoRightXld.GenEmptyObj();
		dLeftHeight=0;
		dLeftWidth=0;
		dRightHeight=0;
		dRightWidth=0;
	}
};

typedef struct tagTLANESET
{
	double  dAllLength; //�ܲ��ĵĳ���
	double  dLeftStart;       //�����ʼ������һ���ߵľ���
	double  dRightStart;      //�Ҳ���ʼ��������һ���ߵľ���
	int     nLeftCount;    //�����Ϊ��׼��ͨ������
	int     nRightCount;   //���Ҳ�Ϊ��׼��ͨ������
	std::vector<double>  vLeftPos;     //��������Ϊ��׼������ͨ��λ��
	std::vector<double>  vRightPos;    //���Ҳ����Ϊ��׼������ͨ��λ��
}LANESET,*PLANESET;

typedef struct tagTLINEMATCH
{
	int    nLineUsed;//ʹ�õ��߸���
	int    nCoatCount;
	int    nFoilCount;
	std::vector<int> vCoat1;
	std::vector<int> vCoat2;//1��2��һ�� ������Ϳ������
	std::vector<int> vFoil1;
	std::vector<int> vFoil2;//1��2��һ�� �����Ǽ�������

	//����LineMatch����гߴ��ж�
	std::vector<double> vCoatWidthMin;
	std::vector<double> vCoatWidthMax;
	std::vector<double> vCoatWidthOffset;

	std::vector<double> vCoatHeightMin;
	std::vector<double> vCoatHeightMax;
	std::vector<double> vCoatHeightOffset;

	std::vector<double> vFoilWidthMin;
	std::vector<double> vFoilWidthMax;
	std::vector<double> vFoilWidthOffset;

	std::vector<double> vFoilHeightMin;
	std::vector<double> vFoilHeightMax;
	std::vector<double> vFoilHeightOffset;

}LINEMATCH,*PLINEMATCH;



//try�ĺ궨��
#define _TRY \
	try \
{
#define _CATCH_HALCON(str) \
} \
	catch(HException e) \
{ \
	CString strMessageBox; \
	strMessageBox.Format("%s\r\n%s",str,e.ErrorMessage().Text()); \
	AfxMessageBox(strMessageBox); \
}
#define _CATCH_HALCON_NULL \
} \
	catch(HException e) \
{ \
}
#define _CATCH_HALCON_RETURN(str) \
} \
	catch(HException e) \
{ \
	CString strMessageBox; \
	strMessageBox.Format("%s\r\n%s",str,e.ErrorMessage().Text()); \
	AfxMessageBox(strMessageBox); \
	return; \
}
#endif _DEFINE_GEICHECK