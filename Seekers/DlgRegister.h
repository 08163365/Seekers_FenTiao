#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgRegister 对话框

class CDlgRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegister)

public:
	CDlgRegister(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRegister();

// 对话框数据
	enum { IDD = IDD_DIALOG_REGISTER };
	void InitUI();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strRegisterCode;
	CString m_strUserName;
	CString m_strPassword;
	CStaticExt m_cstaticTitle;
	CFont m_ftButton;
	virtual BOOL OnInitDialog();
	CButtonExt m_btnOK;
	CButtonExt m_btnCancel;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	CStaticExt m_STATIC_chName;
	CStaticExt m_STATIC_chPassword;
	CStaticExt m_STATIC_chRegister;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEditExt m_EDIT_password;
	CEditExt m_EDIT_registerCode;
	CEditExt m_EDIT_userName;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnStnClickedStaticTitle();
	afx_msg void OnBnClickedButtonClose();
	CReportCtrlExt m_LST_CPowerInfo;
	afx_msg void OnBnClickedBtnAdd();
	void  SetList();
	afx_msg void OnBnClickedBtnDelete();
	CString m_COM_StrPowerType;
	afx_msg void OnBnClickedBtnLook();
};
