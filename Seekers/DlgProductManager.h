#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgProductManager 对话框

class CDlgProductManager : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProductManager)

public:
	CDlgProductManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgProductManager();

// 对话框数据
	enum { IDD = IDD_DIALOG_PRODUCT_MANAGER };


	CJudge*      m_pOwner;

	void SetOwner(CJudge* pOwner);
	void SetProductParam();
	void WriteProductParam(CString strProductName);

private:
	int m_nCurrentProductSN;//当前选择的配方序号
	int m_nUseProductSN;//系统使用的配方序号

	CString m_strCurrentProductName;

protected:
	CFont m_ftButton;
	CFont m_Listfont;

	
	void DeleteProduct(CString strName);
	void InitStatic();
	void InitListCtrl();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void CreateNewProduct(CString strNewProductName);
	void CopyProduct(CString strSrc,CString strDst);
	CStaticExt m_cstaticTitle;
	CButtonExt m_btnCreate;
	CButtonExt m_btnDelete;
	CButtonExt m_btnCopy;
	CButtonExt m_btnLoad;
	virtual BOOL OnInitDialog();
	CButtonExt m_btnExit;
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonLoad();
	CReportCtrlExt m_listProduct;
	afx_msg void OnBnClickedButtonNote();
	CButtonExt m_btnNote;
	CStaticExt m_STATIC_chCurrentProduct;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CButtonExt m_BTN_close;
	afx_msg void OnBnClickedButtonClose();
	int m_COM_nProductJiXi;
	int m_EDT_nZhiDaiThresh;
	int m_EDT_nTrip;
	int m_COM_nGongYiLeiXing;
	int m_COM_nCoatType;
	int m_COM_nCaoWeiChoice;
	int m_COM_nOapChoice;
	int m_COM_nExistJXType;
	afx_msg void OnBnClickedButtonSave();
	BOOL m_CHK_bChangMoZaiShang;
	BOOL m_CHK_bQiTouXianChu;
//	afx_msg void OnLvnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	double m_EDT_dLongJXHeight;
	double m_EDT_dShortJXHeight;
	CButtonExt m_btnSave;
	afx_msg void OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult);
	double m_EDT_dCoatWidth;
	double m_EDT_dTotalCoatWidth;
	double m_EDT_dLongCoat;
	double m_EDT_dShortCoat;
	double m_EDT_dTailCuoWei;
	double m_EDT_dHeadCuoWei;
};
