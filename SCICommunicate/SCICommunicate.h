// SCICommunicate.h : SCICommunicate DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSCICommunicateApp
// �йش���ʵ�ֵ���Ϣ������� SCICommunicate.cpp
//

class CSCICommunicateApp : public CWinApp
{
public:
	CSCICommunicateApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
