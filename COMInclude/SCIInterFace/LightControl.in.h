#pragma once
#include "stdafx.h"

typedef struct tagTLIGHTINIT//��ʼ����Դ������
{
	CString strProductName; //�䷽��
	DHInterface* Log;		//д��־�ص�����
	int nProductType;		//��Ʒ����
}LIGHTINIT,*PLIGHTINIT;

enum eLightControlType	//��Դ�������ͺ�
{
	DH_HAPS_48W800_1T = 0,
	DH_HAPS_48W800_2T,
	CST_CPL48320CDP_2TD,
};

enum eLightCommType
{
	
	CT_Tcp = 0,						//����Tcp
	CT_SerialPort ,		//����
};