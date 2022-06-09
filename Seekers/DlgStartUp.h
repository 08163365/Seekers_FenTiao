#pragma once
#include "afxwin.h"


// CDlgStartUp 对话框

class CDlgStartUp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStartUp)

public:
	CDlgStartUp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStartUp();

// 对话框数据
	enum { IDD = IDD_DIALOG_STARTUP };
	void SetText(CString strText);
protected:
	void InitUI();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void PostNcDestroy();
	virtual void OnCancel();
	CStaticExt m_StaticProcess;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CStaticExt m_STATIC_chEekers;
	CStaticExt m_STATIC_chS;
	CStaticExt m_STATIC_chCompany;
	CStaticExt m_STATIC_chName;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
};
