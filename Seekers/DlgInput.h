#pragma once
#include "afxwin.h"


// CDlgInput �Ի���

class CDlgInput : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInput)

public:
	CDlgInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInput();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT };

	void InitUI();
	CFont m_ftButton;
	CString m_strTitle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticExt m_cstaticTitle;
	CString m_strInput;
	CButtonExt m_btnOK;
	CButtonExt m_btnCancel;
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEditExt m_EDIT_input;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
};
