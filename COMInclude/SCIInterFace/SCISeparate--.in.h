/************************************************************************
("INITIALIZE" == strMessage)//��ʼ��
("ACTION" == strMessage)//���
("SHOW" == strMessage)//���öԻ���
("SETPARAM" == strMessage)//��ȡ����
("GETPARAM" == strMessage)//���ò���
/************************************************************************/
#pragma once
#include "stdafx.h"
///
enum e_ErrorType
{
	JieDai,//����
	LouJinShu,//©����
	TuoTan,//��̼��©��
	TiaoHen,//����
	QiPao,//����
	LiangDian,//����
	HeiDian,//�ڵ�
	JiErQueXian,//����������
	SumSize,//�ܳ�NG
	C_HSize,//�������NG
	C_WSize,//ģ�п��NG
	AT9Size,//ͿĤ�ϼ���(����AT9�ߴ�)NG
	PoSun,//���� W3������
	AT9LouJinShu,//AT9©����
	CircleHeight,//Բ���߶�
	AoLouBoHeight,//��©���߶�
	BHXJSumSize,//������Ъ�ܳ�
	ABSize,//AB��λ�߶�
	UpJXSize,//�����Ъ�߶�
	DownJXSize,//�����Ъ�߶�
	FirstMeanValueSize,//MeanֵNG
	TuoWei,//��β

};
//const CString g_strErrorName[]={
//	_T("����"),
//	_T("©����"),
//	_T("��̼"),
//	_T("����"),
//	_T("����"),
//	_T("����"),
//	_T("�ڵ�"),
//	_T("����������"),
//	_T("��Ĥ��"),
//	_T("�������"),
//	_T("ģ�п��"),
//	_T("ͿĤ�ϼ���"),
//	_T("����"),
//	_T("AT9©����"),
//	_T("Բ���߶�"),
//	_T("��©���߶�"),
//	_T("������Ъ�ܳ�"),
//	_T("AB��λ�߶�"),
//	_T("�����Ъ�߶�"),
//	_T("�����Ъ�߶�"),
//	_T("MeanֵNG"),
//	_T("��β"),
//};







typedef struct  tagTSEPARATEINIT
{
	CString strProductName;//Ʒ������
	int nGrabberSN;//�����ţ�Ĭ��Ϊ0
	PVOID   pLogView;
	int nProductType;
	int nCoatType;//Ϳ������
}SEPARATEINIT,*PSEPARATEINIT;
typedef struct  tagTSEPARATEINPUT
{
	HObject hoGrabImage;
	HTuple  hvHomMat2D;//����任����
	int     nBreakType;//��������
}SEPARATEINPUT,*PSEPARATEINPUT;
typedef struct tagTSEPARATEOUTPUT
{
	HTuple  hvCoatWidth;
	HTuple  hvCoatHeight;
	HTuple  hvFoilWidth;
	HTuple  hvFoilHeight;
	HTuple  hvLocateMiss;//��λ����ֵ
	HTuple  hv_BaiBian;//�ױ߿��
	///��������
	HObject hoCoat;//Ϳ������
	HObject hoFoil;//��������
	HObject hoDispRegion;//�����������м�����
	HObject hoTDispRegion;//AT9����
	HObject hoLane;
	HObject hoErrorRegion;//��������
	HTuple  hvErrorType;//��������
}SEPARATEOUTPUT,*PSEPARATEOUTPUT;



