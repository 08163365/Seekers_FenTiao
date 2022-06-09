#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSubDlgSize 对话框

class CSubDlgSize : public CDialogEx
{
	DECLARE_DYNAMIC(CSubDlgSize)

public:
	CSubDlgSize(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSubDlgSize();
	CFont m_Listfont;
// 对话框数据
	enum { IDD = IDD_DIALOG_SUB_SIZE };
public:
	void Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
	void InsertIntermission(C_ALLOCATORWORKSHOPOUTPUT& tOutput);
	void InsertContinue(C_ALLOCATORWORKSHOPOUTPUT& tOutput);

	void Update(CReportCtrlExt* pList,B_DETECTWORKSHOPOUTPUT* pDetectOutput);
	void ResetSize(int nStyle);
	void InsertColumn();
protected:
	int m_nListCtrlSize;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void InitUI();	
	DECLARE_MESSAGE_MAP()
public:
	BOOL  IsError(HTuple& hv_ErroType,e_ErrorType nErrorCode);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	CReportCtrlExt m_listSize;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CReportCtrlExt m_listSize2;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
