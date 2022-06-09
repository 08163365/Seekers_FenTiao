#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgProductManager 对话框

class CDlgProductManager : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProductManager)

public:
	CDlgProductManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProductManager();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRODUCT_MANAGER };

protected:
	CFont m_ftButton;
	CFont m_Listfont;

	void CopyProduct(CString strSrc,CString strDst);
	void DeleteProduct(CString strName);
	void CreateNewProduct(CString strNewProductName);
	void InitStatic();
	void InitListCtrl();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CxStatic m_cstaticTitle;
	CButtonST m_btnCreate;
	CButtonST m_btnDelete;
	CButtonST m_btnCopy;
	CButtonST m_btnLoad;
	virtual BOOL OnInitDialog();
	CButtonST m_btnExit;
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonLoad();
	CReportCtrl m_listProduct;
	afx_msg void OnBnClickedButtonNote();
	CButtonST m_btnNote;
	CxStatic m_STATIC_chCurrentProduct;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
