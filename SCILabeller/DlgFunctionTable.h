#pragma once
#include "resource.h"
#include "PageCount.h"
#include "PageSystemParam.h"
#include "PageControlParam.h"
#
// CDlgFunctionTable �Ի���

class CDlgFunctionTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFunctionTable)

public:
	CDlgFunctionTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFunctionTable();

// �Ի�������
	enum { IDD = IDD_DLG_FUNCTIONTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
