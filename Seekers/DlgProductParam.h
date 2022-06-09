#pragma once
#include "afxwin.h"


// CDlgProductParam �Ի���

class CDlgProductParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProductParam)

public:
	CDlgProductParam(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProductParam();

// �Ի�������
	enum { IDD = IDD_DIALOG_PRODUCTPARAM };
public:
	CFont  m_ftButton;//��ť������
	CFont  m_FontCombo;
	void InitUI();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonOk();
	CButtonExt m_BTN_OK;
	CStaticExt m_STC_Title;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButtonExt m_BTN_close;
	CButtonExt m_BTN_cancel;
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonClose();
	int m_COM_emCoatType;
	int m_COM_emProductType;
	CComboBox m_COM_CCoatType;

};
