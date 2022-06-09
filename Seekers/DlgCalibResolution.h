#pragma once
#include "resource.h"
#include "afxwin.h"

// CDlgCalibResolution 对话框

class CDlgCalibResolution : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibResolution)

public:
	CDlgCalibResolution(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibResolution();

// 对话框数据
	enum { IDD = IDD_DLGCALIBRESOLUTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	void SaveRecord();
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	double m_EDT_dFirstMeasureLength;
	double m_EDT_dFirstMeasureWidth;
	double m_EDT_dFirstColumnResolution;
	double m_EDT_dRealMeasureLength;
	double m_EDT_dRealMeasureWidth;
	double m_EDT_dSecondMeasureWidth;
	double m_EDT_dSecondColumnResolution;
	double m_EDT_dVerticalResolution;


	double m_dFirstCalibColumnResolution;
	double m_dSecondCalibColumnResolution;
	double m_dCalibRowResolution;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOnekeycalib();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnReturn();
	afx_msg void OnBnClickedBtnJustcalibbwidth();
	afx_msg void OnBnClickedBtnJustcaliblength();
	CStaticExt m_STC_CTitle;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFont  m_BTN_font;
	CButtonExt m_BTN_CJustCalibWidth;
	CButtonExt m_BTN_CJustCalibLength;
	CButtonExt m_BTN_CCalibALL;
	CButtonExt m_BTN_CWriteCalibResult;
	CButtonExt m_BTN_CReturn;
	double m_EDT_dOmmValue1;
	double m_EDT_dOmmValue2;
	double m_EDT_dOmmValue3;
	double m_EDT_dOmmValue4;
	double m_EDT_dOmmValue5;
	double m_EDT_dOmmValue6;
	double m_EDT_dOmmValue7;
	double m_EDT_dOmmValue8;
	double m_EDT_dCCDMeasureValue1;
	double m_EDT_dCCDMeasureValue2;
	double m_EDT_dCCDMeasureValue3;
	double m_EDT_dCCDMeasureValue4;
	double m_EDT_dCCDMeasureValue5;
	double m_EDT_dCCDMeasureValue6;
	double m_EDT_dCCDMeasureValue7;
	double m_EDT_dCCDMeasureValue8;
	double m_EDT_dCurrent_K;
	double m_EDT_dCurrentThickness;
	double m_EDT_dEncoderDiameter;
	double m_EDT_dStandard_K0;
	double m_EDT_dStandardThickness;
	int m_COM_nEnableThickness;
	afx_msg void OnBnClickedBtnCalculateK0();
	afx_msg void OnBnClickedBtnSaveK0();
//	afx_msg void OnEnChangeEdtCurrentyhickness();
	afx_msg void OnEnKillfocusEdtCurrentyhickness();
};
