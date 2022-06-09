#pragma once
#include "stdafx.h"
#include <vector>
#define PIO24_PORTS_MAX 32
#define PIO24_COUNT 1
const int  ConstFenpinNum = 2;

//const int  Const_nChannel[]=
//{
//	//��ʼ�����
//1	0,
//2	4,
//3	5,
//4	6,
//5	7,
//6	8,
//7	3,
//8	9,
//9	10,
//10	11,
//11	12,
//12	13,
//};
///////����1-6 ����7-12////////////////
//const int  Const_nChannel[] =
//{
//	//��ʼ�����
//	0,
//	4,
//	5,
//	6,
//	7,
//	8,
//	3,
//	9,
//	10,
//	11,
//	12,
//	13,
//};
/////1-2�������Ӧ��ͨ���������
//const int  Const_nChannelOutCounter[] =
//{
//	///////���������//////
//	45,
//	60,
//	61,
//	62,
//	63,
//	64,
//	46,
//	65,
//	66,
//	67,
//	68,
//	69,
//};
/////�ֶ����ƴ��
//const int Const_TestIndexParam[] =
//{
//	169,170,171,172,173,174,175,176,177,178,179,180
//
//};
/////////////////////////////////////////////

/////����1-5 ����7-11/////ȥ��6ͨ����ʹ��///////////
const int  Const_nChannel[] =
{
	//��ʼ�����
	0,
	4,
	5,
	6,
	7,
	3,
	9,
	10,
	11,
	12,
	8,//ԭ����6
	13,//ԭ����12
};
///1-2�������Ӧ��ͨ���������
const int  Const_nChannelOutCounter[] =
{
	///////���������//////
	45,
	60,
	61,
	62,
	63,
	46,
	65,
	66,
	67,
	68,
	64,
	69,
};
///�ֶ����ƴ��
const int Const_TestIndexParam[] =
{
	169,170,171,172,173,175,176,177,178,179,174,180

};
///////////////////////////////////////////






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

enum e_CoatType
{
	M_TwoCam_Continue,//����Ϳ��
	M_TwoCam_Intermission,//��ЪͿ��

};
#define  MAX_LALBELNUM 12  //���֧���������Ŀ
enum e_RejectType
{
	REJECT_DEFULT,//��������
	REJECT_NONE,//ֹͣ����
};
typedef struct tagTALARMS//����������
{
	BOOL		bEffective;//�Ƿ���Ч
	//int			nMAXContinuousError;//���������
	//int			nCurrentContinuousError;//��ǰ�����������
}ALARMS,*PALARMS;
typedef struct tagTCOMMUICATEPARAM
{
   int			nRejectType;//�޷����� ���Ϊ1�Ǽ���޷� ���Ϊ2Ϊ�����޷� Ĭ��Ϊ0 ���ݽ���û������޷�
   double       dFirstLabelLength;//����������������
   double       dSecondLabelLength;//����������������

   int       nXLengthInPixel;//������X��������
   int       n2ndLengthInPixel;//������2nd��������

   int          nBuChangHangShu;//�������겹������
   int          nTiFeiPulse;//�߷�����
   double       dLabelPosition;//����λ��
   //////////////////////////////��Ʒ�����������ʹ��////////////////////////////////////////////

   HTuple       hvChannelChoice;//ͨ��ѡ��

   double       dVirtualEALength;//����������EA����

   BOOL         bMachineLengthLabel;//�豸�ܳ����
   ////
   BOOL     bUseLabelOffset;//ʹ������ƫ�Ƽ��
   double   dLabelOffsetDev;//����
   double   dUpLabelOffsetCompensate;//�ϴ�·ƫ�Ʋ���
   double   dDownLabelOffsetCompensate;//�´�·ƫ�Ʋ���
   HTuple   hvLabelOffsetCompensateTuple;//����ƫ�Ʋ���

}COMMUICATEPARAM,*PCOMMUICATEPARAM;
typedef struct tagTCOMMONPARAM
{
	e_CoatType  emCoatType;//�豸����
	double		dRowResolution[2];//����ֱ��ʣ�0 ���0��1 ���1
	double		dColumnResolution[2];//����ֱ��ʣ�0 ���0��1 ���1
}COMMONPARAM,*PCOMMONPARAM;


