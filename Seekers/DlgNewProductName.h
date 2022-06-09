#pragma once
#include "afxwin.h"


// CDlgNewProductName �Ի���

class CDlgNewProductName : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNewProductName)

public:
	CDlgNewProductName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewProductName();


	void InitStatic();


// �Ի�������
	enum { IDD = IDD_DIALOG_NEWPRODUCTNAME };

protected:



	CFont m_ftButton;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStaticExt m_cstaticTitle;
	CButtonExt m_btnOK;
	CButtonExt m_btnCancel;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	CString m_strNewProduct;
	CStaticExt m_STATIC_chNewProduct;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEditExt m_EDIT_newProductName;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonClose();
	CButtonExt m_BTN_close;
};
