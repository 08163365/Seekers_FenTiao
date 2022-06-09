#pragma once
#include "afxwin.h"


// CDlgException 对话框

class CDlgException : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgException)

public:
	CDlgException(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgException();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXCEPTION };
	void InitUI();
	void Show();
	BOOL m_bShow;//是否显示
protected:
	CFont m_ftButton;
	CFont m_ftList;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStaticExt m_cstaticTitle;
	virtual BOOL OnInitDialog();
	CListBox m_listException;
	CButtonExt m_btnCancel;
	afx_msg void OnBnClickedButtonCancel();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
