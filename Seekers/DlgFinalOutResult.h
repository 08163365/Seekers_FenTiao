#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"


// CDlgFinalOutResult �Ի���

class CDlgFinalOutResult : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFinalOutResult)

public:
	CDlgFinalOutResult(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFinalOutResult();

	virtual void OnFinalRelease();

// �Ի�������
	enum { IDD = IDD_DIALOG_SURFACECOUNT };
	CFont m_Listfont;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CReportCtrlExt m_List_UpSide;
	CReportCtrlExt m_List_DownSide;
	int m_nListCtrlSize;
	virtual BOOL OnInitDialog();
public:
	void InitUI();
	void Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
	void ResetSize(int nStyle);
	void InsertColumn();
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnDblclkListUpside(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnNMDblclkListDownside(NMHDR *pNMHDR, LRESULT *pResult);
};
