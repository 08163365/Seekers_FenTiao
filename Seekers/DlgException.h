#pragma once
#include "afxwin.h"


// CDlgException �Ի���

class CDlgException : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgException)

public:
	CDlgException(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgException();

// �Ի�������
	enum { IDD = IDD_DIALOG_EXCEPTION };
	void InitUI();
	void Show();
	BOOL m_bShow;//�Ƿ���ʾ
protected:
	CFont m_ftButton;
	CFont m_ftList;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
