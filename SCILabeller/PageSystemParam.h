#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"


// CPageSystemParam 对话框

class CPageSystemParam : public CDialogEx
{
	DECLARE_DYNAMIC(CPageSystemParam)

public:
	CPageSystemParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageSystemParam();

// 对话框数据
	enum { IDD = IDD_DLG_PAGESYSTEMPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void UpdateList(HTuple& hv_Name,HTuple& hv_Info);
	void ResetSize();
	CFont m_LST_Font;
	CReportCtrlExt m_LST_ParamList;
};
