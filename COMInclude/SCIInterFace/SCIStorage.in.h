#include "stdafx.h"
typedef struct tagTSTORAGE//��ʼ���洢��
{
	CString strProductName;
	DHInterface* pLogView;
	int nCoatType;
	int nProductType;
}STORAGEINIT,*PSTORAGEINIT;
typedef struct tagTAlgRithm
{
	DHInterface* p_Algorithm0;
	DHInterface* p_Algorithm1;
	DHInterface* p_Algorithm2;
	DHInterface* p_Algorithm3;
}AlgRithm,*PAlgRithm;
