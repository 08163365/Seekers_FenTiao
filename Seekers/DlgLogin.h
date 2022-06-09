#pragma once
#include "afxwin.h"


// CDlgLogin �Ի���

class CDlgLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLogin)

public:
	CDlgLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogin();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOGIN };


	void InitUI();
protected:

	BOOL Checkin();//У���û��������Ƿ���ȷ
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFont   m_ftButton;
	CFont   m_ftComBox;
	CStaticExt m_cstaticTitle;
	virtual BOOL OnInitDialog();
	CButtonExt m_btnLogin;
	CButtonExt m_btnCancel;
	CButtonExt m_btnRegister;
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonRegister();
	CString m_strUserName;
	CString m_strPassword;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	CStaticExt m_STATIC_chName;
	CStaticExt m_STATIC_chPassword;
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CEditExt m_EDIT_password;
//	CEditExt m_EDIT_userName;
//	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
	CComboBox m_COM_CUserName;
	CString m_COM_StrUserName;
	CString m_EDT_StrJobNum;
	CEditExt m_EDT_CJobNum;
	CStaticExt m_STC_CJobNum;
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	CStaticExt m_STC_CCapLockStatus;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
