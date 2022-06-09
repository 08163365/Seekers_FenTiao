#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"


// CPageWarn 对话框

class CPageWarn : public CDialogEx
{
	DECLARE_DYNAMIC(CPageWarn)

public:
	CPageWarn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageWarn();

// 对话框数据
	enum { IDD = IDD_DLG_PAGEWARN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ResetSize();
	void UpdateList(HTuple& hv_Name,HTuple& hv_Info);
	CReportCtrlExt m_LST_ErrorInfo;
	CFont m_LST_Font;
};
