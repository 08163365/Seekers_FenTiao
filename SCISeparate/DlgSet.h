#pragma once
#include "Detect.h"
#include "afxwin.h"

// CDlgSet 对话框

class CDlgSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAM };
public:
	CSeparate* m_pOwner;
	CFont m_ftButton;

	void SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
		COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void InitUI();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	int m_COM_bPretreat;
//	int m_EDT_nSizePretreat;
//	CString m_COM_strInfoDisplay;
	afx_msg void OnPaint();
	CButtonExt m_BTN_OK;
	CButtonExt m_BTN_Cancel;
	CStaticExt m_STC_Title;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	int m_COM_nHcalib;
	CStaticExt m_STC_Hcalib;
	int m_EDT_nDiffColumn;
	CStaticExt m_STC_DiffColumn;
//	double m_EDT_dJiErGaoBuChang;
//	double m_EDT_dJiErKuanBuChang;
	int m_EDT_nLineNum;
	double m_EDT_dLocateMissingOffset;
//	double m_nMaxAobanDevition;
//	BOOL m_CHK_bUseThreshAoBan;
	double m_EDT_dAoBanMaxDeviation;
	int m_EDT_nTreatSize;
	int m_EDT_nXTreatSize;
	int m_EDT_nSelectNum;
	BOOL m_CHK_bUseGRR;
//	int m_EDT_nAoBanThresh;
//	int m_COM_BEnableFindLastFoil;
//	int m_EDT_nFoilThresh;
//	double m_EDT_dRegionWidthSet;
	int m_COM_nFitRegionMode;
//	int m_COM_bUseMonitor;
//	int m_COM_bExtractColor;
//	int m_EDT_nLeftExtractThresh;
//	double m_EDT_dLeftWidthOffset;
//	double m_EDT_dLeftHeightOffset;
//	double m_EDT_dRightHeightOffset;
//	double m_EDT_dRightWidthOffset;
//	int m_EDT_nRightExtractThresh;
	int m_COM_nSelectValueMode;
};
