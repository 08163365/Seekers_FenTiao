// SCIDetect.h : SCIDetect DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSCIDetectApp
// �йش���ʵ�ֵ���Ϣ������� SCIDetect.cpp
//
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
	M_TwoCam_Continue,//����Ϳ��
	M_TwoCam_Intermission,//��ЪͿ��
};
class CSCIDetectApp : public CWinApp
{
public:
	CSCIDetectApp();

// ��д
public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
