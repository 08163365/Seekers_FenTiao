// SCILabeller.h : SCILabeller DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSCILabellerApp
// �йش���ʵ�ֵ���Ϣ������� SCILabeller.cpp
//

class CSCILabellerApp : public CWinApp
{
public:
	CSCILabellerApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
