#pragma once
#include "afxcmn.h"
#include "DlgStandardSetting.h"
#include "DlgStandardSurfaceSetting.h"
// CDlgTabSizeSet 对话框

class CDlgTabSizeSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTabSizeSet)

public:
	CDlgTabSizeSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTabSizeSet();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	void SetOwner(CJudge* pOwner) { m_pOwner = pOwner; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	void SwitchDispPage(const int& nSN);
	
	CDlgStandardSetting* m_pPageStandardSize;
	CDlgStandardSurfaceSetting* m_pPageStandardSurface;

public:
	CJudge*       m_pOwner;
	CTabCtrl m_Tab_Common;
	CFont    m_TabFont;
	virtual BOOL OnInitDialog();
	std::vector<CDialogEx*> m_vecPages;
	afx_msg void OnSelchangeTabSet(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_nTripNum;//条数
};
