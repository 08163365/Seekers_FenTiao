#include "stdafx.h"
typedef BOOL(WINAPI *SystemCallBack)(const CString& strMessage, void* wParam, void* lParam);//ϵͳ�ص�����
typedef struct tagTLABELLERINIT//��ʼ���洢��
{
	CString strProductName;
	PVOID   pLogView; 
	SystemCallBack	pSysCall;  //�ص�,ģ���������ͨѶ����ͨ����������������
	
}LABELLERINIT,*PLABELLERINIT;
