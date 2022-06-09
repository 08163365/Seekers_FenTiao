/************************************************************************
("INITIALIZE" == strMessage)//��ʼ��
("ACTION" == strMessage)//���
("SHOW" == strMessage)//���öԻ���
("SETPARAM" == strMessage)//��ȡ����
("GETPARAM" == strMessage)//���ò���
/************************************************************************/
#pragma once
#include "stdafx.h"


enum e_ErrorType
{
	JieDai,         //����
	LouJinShu,      //©����
	TuoTan,         //��̼��©��
	TiaoHen,        //����
	QiPao,          //����
	LiangDian,      //����
	HeiDian,        //�ڵ�
	TuoWei,         //����ȱ��

	BHXJDownSumSize,//B��ģ��NG
	DownJXSize,     //B������NG
	BHXJUpSumSize,  //A��ģ��
	UpJXSize,       //A������NG
	C_WSize,        //�ܿ�NG
	C_SingleWidth,  //�������NG
	HeadCuoWei,     //ͷ����λNG
	TailCuoWei,     //β����λNG

	//JiErQueXian,//����������
	//SumSize,//�ܳ�NG
	//C_WSize,//ģ�п��NG
	//AT9Size,//ͿĤ�ϼ���(����AT9�ߴ�)NG��ACS1��
	//PoSun,//���� W3������
	//AT9LouJinShu,//AT9©����
	//CaoWeiHNG,//��λ�߶�NG
	//CaoWeiWNG,//��λ���NG
	//CaoWeiMDNG,//��λMDNG
	//CaoWeiTDNG,//��λTDNG
	//CaoWeiMDCuoWeiNG,//��λMD��λNG
	//CaoWeiTDCuoWeiNG,//��λTD��λNG
	//BHXJSumSize,//������Ъ�ܳ�
	//UpJXSize,//�����Ъ�߶�
	//DownJXSize,//�����Ъ�߶�


	
	ErrorCount,

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







//enum e_ErrorType
//{
//	///覴ô�����Ŀ
//	JieDai,//����
//	LouJinShu,//©����
//	TuoTan,//��̼��©��
//	TiaoHen,//����
//	QiPao,//����
//	LiangDian,
//	HeiDian,
//	JiErQueXian,
//	EXCEPTION,
//	CO_WSIZE,
//	FO_WSIZE,
//	CE_WSIZE,
//	AB_DISLOCATION,//����Ϳ����λ
//
//};
//const CString g_strErrorName[]={
//
//	_T("����"),
//	_T("©����"),
//	_T("��̼"),
//	_T("����"),
//	_T("����"),
//	_T("����"),
//	_T("�ڵ�"),
//	_T("����������"),
//	_T("�쳣"),
//	_T("��Ȳ���"),
//	_T("������Ȳ���"),
//	_T("�մɿ�Ȳ���"),
//	_T("AB���λ����"),
//};

typedef struct  tagTSEPARATEINIT
{
	CString strProductName;//Ʒ������
	int nGrabberSN;//�����ţ�Ĭ��Ϊ0
	PVOID   pLogView;
	int nProductType;
	int nCoatType;
}SEPARATEINIT,*PSEPARATEINIT;
typedef struct  tagTSEPARATEINPUT
{
	HObject hoGrabImage;
	HTuple  hvHomMat2D;//����任����
	int     nBreakType;//��������
}SEPARATEINPUT,*PSEPARATEINPUT;
typedef struct tagTSEPARATEOUTPUT
{
	HTuple  hv_Flag;//TRUE�ǲ�ƷOK,FALSE��ƷΪNG
	//����Ϳ��
	HTuple  hv_CoatWidth;           //����Ĵ����ҵ�Ϳ�����
	HTuple  hv_FoilWidth;           //����Ĵ����ҵļ������
	HTuple  hv_CeramicWidth;        //����Ĵ����ҵ��մɿ��
	HTuple  hv_SortWidth;           //�������ʾ�Ŀ��˳��
	HTuple  hv_Name;                //���ݿ������������
	HObject ho_Coat;                //Ϳ������
	HObject ho_Foil;                //��������
	HObject ho_Ceramic;             //�մ�����
	HObject ho_DispRegion;          //�����������м�����
	HObject ho_ErrorRegion;         //��������
	HTuple  hv_AT9EdageToProEdage;  //��¼AT9�߽�����Ʒ�߽�ľ���
	HTuple hv_CpkEdageToProEdage;	//����Ϳ��CPK����,��¼ÿ��Ϳ�������ڱ�
	//��ЪͿ��
	HTuple  hv_LeftEadgePixel;      //�������
	HTuple  hv_RightEadgePixel;     //�Ҳ�����
	HTuple  hv_TopEdagePixel;       //��������
	HTuple  hv_BotEdagePixel;       //�ײ�����

	HTuple  hv_TripSNTuple;         //��λ����������
	HTuple  hv_CaoWeiWidthTuple;    //��λ���
	HTuple  hv_CaoWeiHeightTuple;   //��λ�߶�
	HTuple  hv_CaoWeiXPositionTuple;//��λX����
	HTuple  hv_CaoWeiYpositionTuple;//��λY����
	HObject ho_ExtractCaoWeiRegions;//��ȡ�Ĳ�λ����

	HTuple  hv_JXHeight;            //��Ъ�߶�
	HTuple  hv_MPLength;            //Ĥ��
	HTuple  hv_SumLength;           //�ܳ�
	HObject ho_JXRegion;            //��Ъ����

}SEPARATEOUTPUT,*PSEPARATEOUTPUT;

