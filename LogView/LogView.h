// LogView.h : LogView DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLogViewApp
// �йش���ʵ�ֵ���Ϣ������� LogView.cpp
//

class CLogViewApp : public CWinApp
{
public:
	CLogViewApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
