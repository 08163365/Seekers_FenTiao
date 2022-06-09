#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"

class CDlgDatabaseHistory;
// CDlgDatabaseHistory 对话框
void T_ReadImage(CDlgDatabaseHistory* pDlg);
void T_ExportExcel(CDlgDatabaseHistory* pDlg);
class CDlgDatabaseHistory : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDatabaseHistory)

public:
	CDlgDatabaseHistory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDatabaseHistory();

// 对话框数据
	enum { IDD = IDD_DIALOG_DATABASE_HISTORY };

	CFont m_fontList;
	CFont m_ftButton;

	void InitUI();
	void InitListCtl();
	void ReadBatchInfo();
	CString m_strFilePath;
	HObject m_hoFrontImage,m_hoBehindImage;
	CWinThread* m_pThreadReadImage;
	HTuple m_hvWindowIDFront,m_hvWindowIDBehind;
private:
	void ReadCount();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnReadImageOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExportExcelOK(WPARAM wParam, LPARAM lParam);
	
	virtual BOOL OnInitDialog();
	CReportCtrlExt m_listDataBase;
	afx_msg void OnBnClickedButtonCancel();
	CReportCtrlExt m_listBatchInfo;
	afx_msg void OnBnClickedButtonGo();
	CDateTimeCtrl m_TimeCtrl;
	afx_msg void OnNMDblclkListDatabase(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListBatchinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonExport();
	CStaticExt m_cstaticTitle;
	CButtonExt m_BTN_cancel;
	CButtonExt m_BTN_export;
	CButtonExt m_BTN_go;
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
};
