#pragma once
#include "afxcmn.h"

#include "afxwin.h"

// CSubDlgCount 对话框

class CSubDlgCount : public CDialogEx
{
	DECLARE_DYNAMIC(CSubDlgCount)

public:
	CSubDlgCount(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubDlgCount();

// 对话框数据
	enum { IDD = IDD_DIALOG_SUB_COUNT };
	void Update(BOOL bBuild/*是否重建*/);
	void UpdateDate(int nIndex);
	void ResetSize();
	int   m_nLaneCount;//通道个数
	void Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
protected:
	CFont m_ftButton;
	CFont m_ftList;
	void InitUI();
	int m_nListCtrlSize;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	// 分类统计
	void InsertColumn();
	CReportCtrlExt m_listCountClass;
	CReportCtrlExt m_listCount;
	CFont m_Listfont;
	afx_msg void OnBnClickedBtnHistogram();
	CButtonExt m_btnHistogram;
	afx_msg void OnBnClickedBtnClearcount();
	CButtonExt m_btnClearCount;
	afx_msg void OnBnClickedBtnHistory();
	CButtonExt m_btnHistory;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
