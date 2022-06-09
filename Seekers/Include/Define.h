#ifndef P_DEFINE_P
#define P_DEFINE_P
#include "stdafx.h"
#include "HalconCpp.h"
using namespace HalconCpp;

const int ExternalTrigger=0;//�ⴥ��
const int Internal=1;//�ڴ���
const int Intermittent=0;//��Ъʽ
const int Continuous=1;//����ʽ
const int SpeedSection=2;//������Ƭ

#define UM_TIMER_QUEUEINFO WM_USER+1
#define UM_TIMER_STARTGRAB WM_USER+2
#define UM_TIMER_TIME WM_USER+3



#define LOGO_WIDTH 163		//LOGO�Ŀ��
#define MAX_LANE 12


enum e_ProductType
{
	positive=0,//����
	negative//����
};
enum e_CoatType
{
	M_TwoCam_Continue,//����Ϳ��
	M_TwoCam_Intermission,//��ЪͿ��
};

enum e_GongYiNameType
{
	GongYi_ASC1,//˫���Ʒ
	GongYi_M6S,//������Ъ��
	GongYi_MMT,//��Ъ�в�λ
	GongYi_STS1,//A/B����пղ���OAP
	GongYi_STS2,//B����пղ���OAP
};


enum e_FoilInSideType
{
	TwoSideProduct,//˫���Ʒ
	LeftSideProduct,//����Ʒ
	RightSideProduct,//�Ҳ��Ʒ

};
//�������������ӳ���
const CString g_strLetterErrorName[]={
	_T("a"),
	_T("b"),
	_T("c"),
	_T("d"),
	_T("e"),
	_T("f"),
	_T("g"),
	_T("h"),
	_T("i"),
	_T("j"),
	_T("k"),
	_T("l"),
	_T("m"),
	_T("o"),
	_T("p"),
	_T("q"),
	_T("r"),
	_T("s"),
	_T("t"),
	_T("u"),
	_T("v"),
};
typedef struct tagTSYSTEMSET
{
	int nHistoryCount;//��������ʷͼƬ�ĸ���
	int nHistoryType;//����������Ļ���ȫ������,���ǲ�����
	int nRejectPoint;//�޷ϵ�
	
	int  nMergeResults;//�Ƿ��ۺϽ��
	int  nMergeParam;//�������ʹ����ͬ�Ĳ���
	BOOL bMirrorRow;//�Ƿ�ֱ����


	BOOL bTileImage;//�Ƿ�ƴͼ
	//int  nStepLength;
	//double dwMaxFoilDistance;//��󼫶����
	BOOL bSaveTileImage;//�Ƿ񱣴�ƴ֮ǰ��ͼƬ
	int  nDistanceCamera;//���֮��ľ���
	BOOL bAlignedTwoCamera;//�����������,������Ƿ�ʹ��ǰ�����markλ��
	int nLaneCount;//������
	int nOverTimeExit;//��ʱ�˳�����
	BOOL bEnableReloadParam;//���ð���ԭ����
	int  nCameraModel;//�����λѡ��
	//BOOL bEnableLabelCheck;//�������������
	BOOL bEnablePopWindowToWarn;//���õ�������
	int  nMaxPopWindowNum;//��󵯴���Ŀ
	//int  nPushType;//0��������� 1�Ƿ������


	/////
	int  nOverTimeFrameValue;///��ʱ֡��
	int  nOverTimeValue;//��ʱ����
	//��ʱ�Զ��˳�
	int nOverTimeLimitLoginOff;


}SYSTEMSET,*PSYSTEMSET;




struct CreateBaseLineInfo//CreateBaseLineInfo
{

	int nFirstCoatLowThresh;//Ϳ����ȡ����ֵ
	int nFirstCoatHighThresh;//Ϳ����ȡ����ֵh
	int nFirstFoilLowThresh;//Ϳ����ȡ��ֵ
	int nFirstFoilHighThresh;//Ϳ����ȡ��ֵ

	int nSecondCoatLowThresh;//Ϳ����ȡ��ֵ
	int nSecondCoatHighThresh;//Ϳ����ȡ��ֵh
	int nSecondFoilLowThresh;//Ϳ����ȡ��ֵ
	int nSecondFoilHighThresh;//Ϳ����ȡ��ֵ


	int nFirstLeftPosition;//ǰ������Ϳ��λ��
	int nFirstRightPosition;//ǰ����Ҳ�Ϳ��λ��
	int nFirstLeftPositionRow;//ǰ������Ϳ��λ��
	int nFirstRightPositionRow;//ǰ����Ҳ�Ϳ��λ��

	int nSecondLeftPosition;//��������Ϳ��λ��
	int nSecondRightPosition;//������Ҳ�Ϳ��λ��
	int nSecondLeftPositionRow;//��������Ϳ��λ��
	int nSecondRightPositionRow;//������Ҳ�Ϳ��λ��


	/////////////////////////////////////////////////////

};
struct s_ProductParam//��Ʒ������Ϣ
{
	e_CoatType emCoatType;
	e_ProductType emProductType;
};
struct  s_TempStoreInfo//��������
{
	double dFirstWidth;
	double dSecondWidth;
	double dSumLength;
   s_TempStoreInfo()
	{
		dFirstWidth=0;
		dSecondWidth=0;
		dSumLength=0;
	}
};

struct s_SingleCount//������ͳ��
{
	std::vector<int> nClassError;//����,����ĸ���
	int nError;//����Ƭ��
	int nTotal;//��Ƭ��
};

typedef struct tagTRESULTDATA
{
	int nFoilSN;
	int nCoatWidth;
	int nCoatHeight;
	int nFoilWidth;
	int nFoilHeight;
	int nTotalLength;

}RESULTDATA,*PRESULTDATA;

struct s_Count
{
	int nType;//���ִ�������
	std::vector<s_SingleCount> veCount[2];//�ж���������Ӧ���ٸ�
	CStringArray strAClassName;//�������ĳƺ�
}; 


struct s_ErrorClassify
{
	std::vector<int> vecErrorClassify;//����,����ĸ���
};

struct s_LabelCause
{
	std::vector<s_ErrorClassify> vec_LabelCause[2];//��ǩͳ��
};


typedef struct  tagTSUBELEMENT
{
	HObject hoImage;
	HObject hoSubImage;
	int     nGrabberSN;
}SUBELEMENT,*PSUBELEMENT;

//�ߴ��жϽṹ��
typedef struct tagTJUDGECONMENT  ///ÿһ����ж�
{
	bool bIsDecter;      //�����Ƿ���
	bool bIsSecond;      //�����Ƿ��жϸ��������
	int  nMaxNumber;        //����
	double dMaxDiameter;    //���ֱ��
	double dMaxArea;        //������
	double dMaxLength;      //��󳤶�
	double dMaxWidth;		 //�����
}JUDGECONMENT,*PJUDGECONMENT;
typedef struct tagTSIZECONMENT
{
	double dMaxWidth;		 //�������
	double dMinWidth;		 //�������
	double dMaxHeight;       //�߶�����
	double dMinHeight;		 //�߶�����


	double dwHeightStandard;//�߶ȱ�׼ֵ
	double dwWidthStandard;//��ȱ�׼ֵ
	double dwHeightDeviation;//�߶�ƫ��
	double dwWidthDeviation;//���ƫ��
	double dwHeightCorrect;//�߶�����
	double dwWidthCorrect;//�������
	double dwTsizeMax;//T�����ֵ
	double dwBaibianMax;//�ױ����ֵ


	double m_dCorrectMax;
	double m_dCorrectMin;
	double m_dwDiePianMarkWidth;//��ƬU���пڿ������--
	double m_dwDiePianMarkWidthDev;//��ƬU���пڿ������--
	double m_dwDiePianMarkHeight;//��ƬU���пڸ߶�����--
	double m_dwDiePianMarkHeightDev;//��ƬU���пڸ߶�����--
	double m_dwDiePianMarkDiffMax;//��ƬU�οڵĶ�������ֵ--
	double m_dwDistanceBorder;//���߾��׼ֵ
	double m_dwDistanceBorderDev;//���߾�ƫ��
	double m_dwLocateMissing;//��λ�����׼ֵ
	double m_dwLocateMissingDev;//��λ����ƫ��
}SIZECONMENT,*PSIZECONMENT;


struct PowerInfo//Ȩ�޽ṹ��
{
	CString strPowerType;
	CString strJobNum;
	CString strJobPassWord;

};




//enum e_ErrorType
//{
//	C_WSIZE,//Ϳ�����NG
//	C_HSIZE,//Ϳ���߶�NG
//	F_WSIZE,//�������NG
//	F_HSIZE,//�����߶�NG
//	DINGWEIBUQI,//��λ����NG
//	SUMSIZE,//�༫���ܳ�NG
//	BAIBIAN,//AT9�ߴ�NG
//	JIERYUCHICUNBUFU,//������Ŀ��ߴ粻����
//	LOUJINSHU,//©����
//	QIPAO,//����
//	HEIBAN,//�ڰ�
//	BAIBAN,//�װ�
//	HEIXIAN,//����
//	BAIXIAN,//�κ�
//	BIANYUANQUEXIAN,//��Եȱ��
//	BIANJIEQUEXIAN,//�߽�ȱ��
//	AT9QUEXIAN,//AT9ȱ��
//	TUOTANHUOLOUGUN,//��̼��©��
//	JIERDAILIAO,//��������
//	JIEANCECHAOSHI,//��ⳬʱ
//	CHICUNYICHANG,//�ߴ����쳣
//	BIAOMIANYICHANG//�������쳣
//};


#define   VK_0  0x30   
#define   VK_1  0x31   
#define   VK_2  0x32   
#define   VK_3  0x33   
#define   VK_4  0x34   
#define   VK_5  0x35   
#define   VK_6  0x36   
#define   VK_7  0x37   
#define   VK_8  0x38   
#define   VK_9  0x39  
  
//���������ַ�A~Z  
#define   VK_A  0x41   
#define   VK_B  0x42   
#define   VK_C  0x43   
#define   VK_D  0x44   
#define   VK_E  0x45   
#define   VK_F  0x46   
#define   VK_G  0x47   
#define   VK_H  0x48   
#define   VK_I  0x49   
#define   VK_J  0x4A   
#define   VK_K  0x4B   
#define   VK_L  0x4C   
#define   VK_M  0x4D   
#define   VK_N  0x4E   
#define   VK_O  0x4F   
#define   VK_P  0x50   
#define   VK_Q  0x51   
#define   VK_R  0x52   
#define   VK_S  0x53   
#define   VK_T  0x54   
#define   VK_U  0x55   
#define   VK_V  0x56   
#define   VK_W  0x57   
#define   VK_X  0x58   
#define   VK_Y  0x59   
#define   VK_Z  0x5A   
  
//���������ַ�a~z  
#define   VK_a  0x61   
#define   VK_b  0x62   
#define   VK_c  0x63   
#define   VK_d  0x64   
#define   VK_e  0x65   
#define   VK_f  0x66   
#define   VK_g  0x67   
#define   VK_h  0x68   
#define   VK_i  0x69   
#define   VK_j  0x6A   
#define   VK_k  0x6B   
#define   VK_l  0x6C   
#define   VK_m  0x6D   
#define   VK_n  0x6E   
#define   VK_o  0x6F   
#define   VK_p  0x70   
#define   VK_q  0x71   
#define   VK_r  0x72   
#define   VK_s  0x73   
#define   VK_t  0x74   
#define   VK_u  0x75   
#define   VK_v  0x76   
#define   VK_w  0x77   
#define   VK_x  0x78   
#define   VK_y  0x79   
#define   VK_z  0x7A

#endif //P_DEFINE_P