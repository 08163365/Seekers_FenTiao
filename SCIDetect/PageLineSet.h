#pragma once
#include "resource.h"
#include "Detect.h"
#include "afxwin.h"
#include "afxcmn.h"

// CPageLineSet 对话框
class CDlgAlgorithm;
class CPageLineSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPageLineSet)

public:
	CPageLineSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageLineSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAMSET };
public:
	void SetDefaultLineParam(CLineToolPara& LineStruct);
	void SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
		COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG);
	void InitUI();
	void GetCoatAndFoilRegions (HObject ho_Image, HObject ho_ROI_0, HObject *ho_FoilSortedRegions, 
		HObject *ho_CoatSortedRegions);
	void InitCom();
	CFont m_ftButton;
	void GenLineArrow(const CLineToolPara& Line, const HTuple& Size);
	CString m_strInfoDisplay;//显示信息
public:
	CDlgAlgorithm* m_pParentDlg;
	SEPARATEINPUT m_tDetectInput;//检测输入
	HObject m_hoPretreatImage;//预处理后的图片
	HObject m_hoLine;
	HObject m_hoDispRegion;
	HTuple  m_hvWindowID;
public:
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	void GetFoilEdage (HObject ho_Image, HObject ho_Rectangle, HTuple *hv_LTRow, HTuple *hv_LTColumn, 
		HTuple *hv_RBRow, HTuple *hv_RBColumn);
	void SetButtonStaus(BOOL bStaus /* = FALSE */);
	virtual BOOL OnInitDialog();
	double m_EDT_dMeasureLength;
	int    m_EDT_nMeasureLinesNum;
	double m_EDT_dMeasureSigma;
	int     m_EDT_nMeasureThres;
	double  m_EDT_dMeasureWidth;
	int      m_CB_nLineSelect;
	int     m_CB_nSelect;
	int     m_CB_nDirect;

public:

	void FreshNewParam();
	void FreshImage();
	afx_msg void OnBnClickedButtonDrawLine();
	afx_msg void OnBnClickedButtonLineTest();
	afx_msg void OnCbnSelchangeComboLineSelect();
	afx_msg void OnBnClickedButtonTest();
//	afx_msg void OnBnClickedButtonSave();
//	afx_msg void OnBnClickedBntOpenimage();
	afx_msg void OnEnChangeEditMeasurelinesnum();
	afx_msg void OnEnChangeEditMeasurelength();
	afx_msg void OnEnChangeEditMeasurewidth();
	afx_msg void OnEnChangeEditMeasuresigma();
	afx_msg void OnEnChangeEditMeasurethres();
	afx_msg void OnCbnSelchangeComboMeasuretransition();
	afx_msg void OnCbnSelchangeComboMeasureselect();
//	afx_msg void OnBnClickedBntParamset();
	CStaticExt m_STC_Title;
//	CButtonExt m_BTN_ParamSet;
//	CButtonExt m_BTN_OpenImage;
	CButtonExt m_BTN_Test;
//	CButtonExt m_BTN_Save;
	CButtonExt m_BTN_DrawLine;
	CButtonExt m_BTN_LineTest;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int m_CB_nChoice;
	afx_msg void OnCbnSelchangeComboLineChoice();
	CButtonExt m_BTN_ModLine;
	afx_msg void OnBnClickedButtonModLine();
	afx_msg void OnMenuDrawcoat();
	afx_msg void OnBnClickedBtnOnekeydraw();
	int m_EDT_nCoatGrayValue;
	afx_msg void OnEnChangeEdtCoatgray();
	CStatic m_STC_CExampleImage;

	HTuple m_hv_ExampleWindowID;
	afx_msg void OnBnClickedCalib();
	INT m_CB_nCalChoose;
	afx_msg void OnCbnSelchangeComboCalculatChoice2();
};
