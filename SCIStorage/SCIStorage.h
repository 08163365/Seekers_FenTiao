// SCIStorage.h : SCIStorage DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSCIStorageApp
// �йش���ʵ�ֵ���Ϣ������� SCIStorage.cpp
//

class CSCIStorageApp : public CWinApp
{
public:
	CSCIStorageApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
