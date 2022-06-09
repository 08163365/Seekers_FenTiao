#pragma once
#include "afxwin.h"


// CSubDlgControl 对话框

class CSubDlgControl : public CDialogEx
{
	DECLARE_DYNAMIC(CSubDlgControl)

public:
	CSubDlgControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubDlgControl();

// 对话框数据
	enum { IDD = IDD_DIALOG_SUB_CONTROL };

	CFont m_ftButton;

	void InitUI();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonExt m_btnReset;
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnPaint();
	CButtonExt m_btnReStartProcess;
	afx_msg void OnBnClickedButtonRestartprocess();
	CButtonExt m_btnShutDown;
	afx_msg void OnBnClickedButtonShutdown();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
