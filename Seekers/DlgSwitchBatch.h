#pragma once
#include "afxwin.h"


// CDlgSwitchBatch 对话框

class CDlgSwitchBatch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSwitchBatch)

public:
	CDlgSwitchBatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSwitchBatch();

// 对话框数据
	enum { IDD = IDD_DIALOG_SWITCH_BATCH };
	void InsertDatabase();
	void WriteCount();
	void WriteRecordByBatch();//按照卷统计
	void CreateNewDirectory();
	
	//DWORD GetProcessidFromName(CString processName,CString &processPath);
protected:
	void InitStatic();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStaticExt m_cstaticCurrentBatch;
	CStaticExt m_cstaticNewBatch;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	CButtonExt m_btnOK;
	CButtonExt m_btnCancel;
	CFont   m_ftButton;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString m_strNewBatch;
	CString m_strNewBatchPath;
	CStaticExt m_cstaticTitle;
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEditExt m_EDIT_newBatch;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
};
