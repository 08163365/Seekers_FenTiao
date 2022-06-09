#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgHistory 对话框

class CDlgHistory : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHistory)

public:
	CDlgHistory(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHistory();

// 对话框数据
	enum { IDD = IDD_DIALOg_tSystemSet };
public:
	CFont m_fontList;
	CFont m_ftButton;
	void InitUI();
	void InitListCtrl();

	
	HObject m_hoImage;//图片列表
	HObject m_hoRegion;//Region列表
	HTuple  m_hvWindowID;

	HObject m_hoDispImage;
	int     m_nGrabberSN;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDetect();
	afx_msg void OnBnClickedBtnExit();
	CReportCtrlExt m_LC_Image;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickListImage(NMHDR *pNMHDR, LRESULT *pResult);
	CButtonExt m_BTN_Detect;
	CButtonExt m_BTN_Exit;
	afx_msg void OnPaint();
	CStaticExt m_STC_Title;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
};
