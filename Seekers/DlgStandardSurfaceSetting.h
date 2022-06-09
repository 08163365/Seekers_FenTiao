#pragma once


// CDlgStandardSurfaceSetting 对话框IDD_DIALOG_STANDARDSURFACE

#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"


class CDlgStandardSurfaceSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStandardSurfaceSetting)

public:
	CDlgStandardSurfaceSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStandardSurfaceSetting();

	// 对话框数据
	enum { IDD = IDD_DIALOG_STANDARDSURFACE };
public:
	CJudge*       m_pOwner;
	CFont       m_FontBig;
	std::vector<CString> m_vec_strErrorName;
	
	void FlushParam();
	void UpdateSurfaceList();
	void GetSurfaceList();
	void ComPareChange(int LastValue,int CurrentValue,CString strName);
	void ComPareChange(double LastValue,double CurrentValue,CString strName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	void      InitUI();
	virtual BOOL OnInitDialog();
	int    m_EDT_nTotalMaxNumber;
	double m_EDT_dTotalMaxArea;
	afx_msg void OnBnClickedButtonAdd();
//	afx_msg void OnBnClickedButtonOk();
//	afx_msg void OnBnClickedButtonCancel();
	CReportCtrlExt m_LST_Surface;
	afx_msg void OnEnChangeEditCellcount();
	CComboBox m_COM_ErrorName;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLvnItemchangedListSurface(NMHDR *pNMHDR, LRESULT *pResult);

	CButtonExt m_btn_Add;
	CButtonExt m_btn_Cancel;
	CButtonExt m_btn_Delete;
	CButtonExt m_btn_OK;
	CFont   m_FontButton;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
