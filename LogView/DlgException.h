#pragma once
#include "stdafx.h"
#include "Resource.h"
// CDlgException 对话框
class CLog;
class CDlgException : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgException)

public:
	CDlgException(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CDlgException();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXCEPTION };

public:

	CFont m_ftButton;
	CFont m_ftList;

	int m_bShow;
	CLog *m_pLog;

	//CToolTipCtrl m_tipExcepion;

	void ShowException();

	void ShowException(const CString& strExceptionFile);
private:

	void InitUI();

	BOOL ReadException(const CString &strFile,CStringArray* strArrayExcepion);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	virtual void PostNcDestroy();

	afx_msg void OnPaint();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg LRESULT OnNcHitTest(CPoint point);

	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonCancel();

	CxStatic m_cstaticTitle;
	CListBox m_listException;
	CButtonST m_btnCancel;

	afx_msg void OnClose();
	CButtonST m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
};
