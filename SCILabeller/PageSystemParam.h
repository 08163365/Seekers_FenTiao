#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"


// CPageSystemParam �Ի���

class CPageSystemParam : public CDialogEx
{
	DECLARE_DYNAMIC(CPageSystemParam)

public:
	CPageSystemParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageSystemParam();

// �Ի�������
	enum { IDD = IDD_DLG_PAGESYSTEMPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void UpdateList(HTuple& hv_Name,HTuple& hv_Info);
	void ResetSize();
	CFont m_LST_Font;
	CReportCtrlExt m_LST_ParamList;
};
