#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgHelp �Ի���

class CDlgHelp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHelp();

// �Ի�������
	enum { IDD = IDD_DIALOG_HELP };
private:
	CFont m_ftButton;
	CString GetFileVersion(CString strFileName);
	void InitUI();
	CString GetDesignatedDiskFreeSpace();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticExt m_cstaticTitle;
	CStaticExt m_staticVersion;
	afx_msg void OnBnClickedBtnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
	CReportCtrlExt m_Lst_CVersionInfo;
	afx_msg void OnBnClickedBtnRefreshversion();
};
