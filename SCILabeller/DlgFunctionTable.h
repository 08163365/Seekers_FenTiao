#pragma once
#include "resource.h"
#include "PageCount.h"
#include "PageSystemParam.h"
#include "PageControlParam.h"
#
// CDlgFunctionTable 对话框

class CDlgFunctionTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFunctionTable)

public:
	CDlgFunctionTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFunctionTable();

// 对话框数据
	enum { IDD = IDD_DLG_FUNCTIONTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CPageCount* m_pPageCount;
	CPageSystemParam* m_pPageSystemParam;
	CPageControlParam* m_pPageControlParam;
public:
	CTabCtrlExt m_TAB_CLabelFunction;
	void ResetSize();
	virtual BOOL OnInitDialog();
	std::vector<CDialogEx*> m_vecPages;
	afx_msg void OnTcnSelchangeTabLabelfunction(NMHDR *pNMHDR, LRESULT *pResult);
	void SwitchDispPage(const int & nSN);
};
