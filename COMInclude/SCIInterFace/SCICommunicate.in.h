#pragma once
#include "stdafx.h"
typedef BOOL(WINAPI *SystemCallBack)(const CString& strMessage, void* wParam, void* lParam);//ϵͳ�ص�����
typedef struct tagTCOMMUNICATEINIT//��ʼ��ͨ����
{
	CDialogEx* pDlg;//������ָ��
	CString strProductName;
	DHInterface* pLogView;
	CString strGongWei;
	SystemCallBack	pSysCall;  //�ص�,ģ���������ͨѶ����ͨ����������������
	int nCoatType;//0�ǵ�Ƭ 4����
}COMMUNICATEINIT,*PCOMMUNICATEINIT;
typedef struct tagTLabelControl//��ʼ��ͨ����
{
	///GET_TBCZ ��λһ���� ��һʹ��  ��λ������ ��λ��ʹ�� ����λ��

	double  dFirstLabelLength;
	BOOL    bEnableFirstLabel;

	double  dSecondLabelLength;
	BOOL    bEnableSecondLabel;

	double dLabelActionPosition;//����λ��


}LabelControlInfo,*pLabelControlInfo;


