#include "stdafx.h"
#include "Battery.h"
#include "SCIDetect.h"

#include "afxdialogex.h"
#pragma once
// CPageGRRSet 对话框

class CDlgAlgorithm;

class CPageGRRSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPageGRRSet)

public:
	CPageGRRSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageGRRSet();
// 对话框数据
	enum { IDD = IDD_DIALOG_CREATEMODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetButtonStaus(BOOL bStaus);
	void SetButtonStyle(CButtonExt& Button,COLORREF& btnBKIN,COLORREF& btnBKOUT,COLORREF btnBKFOCUS,COLORREF& btnFG,CFont& btnFont);
    MARKVECTOR m_ModelInfoVector; 
public:
	ExmaraParam m_tExmaraParam;//提取参数
	HObject     m_hoModelInfo;
	HTuple  m_hvModelId;
	HTuple m_hvModeParam;

	HObject m_hoMatchObject;
	void InitStatic();
	CFont m_ftButton;
	CButtonExt m_BTN_Return;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDlgAlgorithm* m_pParentDlg;
public:
	void GetCalbHW (HObject ho_ImageReduced, HObject *ho_Edges, HTuple *hv_H0, HTuple *hv_W0);
	double m_dRowResolution;
	double m_EDT_dCalibArea;
	afx_msg void OnBnClickedBtnCalib();
	afx_msg void OnBnClickedBtnDarwcalibregion();
	int m_EDT_nLowThresh;
	int m_EDT_nHighThresh;
	double m_EDT_dAlpha;
	double m_EDT_dMinArea;

	HTuple m_hvGrayTuple;
	HTuple m_hvLEakFoilTuple;
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnOpenimage();
	afx_msg void OnBnClickedBtnTest2();

	BOOL m_bFlag;
	CReportCtrlExt m_LST_Area;
	int m_COM_nExtractColor;
	int m_COM_bUseMode;
	int m_EDT_nPointNum;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
