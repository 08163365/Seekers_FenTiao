#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CSubDlgCount �Ի���

class CSubDlgCount : public CDialogEx
{
	DECLARE_DYNAMIC(CSubDlgCount)

public:
	CSubDlgCount(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSubDlgCount();

// �Ի�������
	enum { IDD = IDD_DIALOG_SUB_COUNT };
	void InsertData();
	void ResetSize();
	int   m_nLaneCount;//ͨ������
protected:
	CFont m_ftButton;
	CFont m_ftList;
	void InitUI();
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	// ����ͳ��
//	CSubDlgHistogram* m_pSubDlgHistogram;
	CListCtrlExt m_listCountClass;
	CListCtrlExt m_listCount;
	afx_msg void OnBnClickedBtnHistogram();
	CButtonExt m_btnHistogram;
	afx_msg void OnBnClickedBtnClearcount();
	CButtonExt m_btnClearCount;
	afx_msg void OnBnClickedBtnHistory();
	CButtonExt m_btnHistory;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
