#pragma once
#include "resource.h"
#include "Detect.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"


// CDlgParamSet 对话框
class CDlgParamSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParamSet)

public:
	CDlgParamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgParamSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_PARAMSET };
public:
	void SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
		COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG);
	void InitUI();

	void InitCom();
	CFont m_ftButton;

	void GenLineArrow(const CLineToolPara& Line, const HTuple& Size);
	CString m_strInfoDisplay;//显示信息
public:

	CSeparate* m_pTempDetect;//模拟测试类
	CSeparate* m_pRealDetect;//实际检测类
	PSEPARATEINPUT m_pDetectInput;//检测输入
	HObject m_hoPretreatImage;//预处理后的图片
protected:
	HObject m_hoDispRegion;

	HTuple  m_hvWindowID;
	HTuple  m_hv_ExampleWindowID;
	CFont   m_ftGrayValue;
public:
	double m_dDispImagePartRow0;
	double m_dDispImagePartCol0;
	double m_dDispImagePartRow1;
	double m_dDispImagePartCol1;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	void SaveOperaterLog();//记录操作痕迹
	void ComPareChange(int& LastValue,int& CurrentValue,CString strName);
	void ComPareChange(double& LastValue,double& CurrentValue,CString strName);
	CMenuExt *m_pMenuTools;
	void SetButtonStaus(BOOL bStaus /* = FALSE */);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonExit();
	CStatic m_STC_Image;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStaticExt m_STC_GrayValue;
	afx_msg void OnBnClickedBntOpenimage();
	afx_msg void OnBnClickedBntSaveimage();
	CReportCtrlExt m_LC_CheckInfo;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	double m_EDT_dMeasureLength;
	int m_EDT_nMeasureLinesNum;
	double m_EDT_dMeasureSigma;
	int m_EDT_nMeasureThres;
	double m_EDT_dMeasureWidth;
	int m_CB_nLineSelect;
	int m_CB_nSelect;
	int m_CB_nDirect;
	afx_msg void OnBnClickedButtonDrawLine();
	afx_msg void OnBnClickedButtonLineTest();
	afx_msg void OnCbnSelchangeComboLineSelect();
public:

	void FreshNewParam();
	void FreshImage();
	afx_msg void OnEnChangeEditMeasurelinesnum();
	afx_msg void OnEnChangeEditMeasurelength();
	afx_msg void OnEnChangeEditMeasurewidth();
	afx_msg void OnEnChangeEditMeasuresigma();
	afx_msg void OnEnChangeEditMeasurethres();
	afx_msg void OnCbnSelchangeComboMeasuretransition();
	afx_msg void OnCbnSelchangeComboMeasureselect();
	afx_msg void OnBnClickedBntParamset();
	CStaticExt m_STC_Title;
	CButtonExt m_BTN_ParamSet;
	CButtonExt m_BTN_OpenImage;
	CButtonExt m_BTN_SaveImage;
	CButtonExt m_BTN_Test;
	CButtonExt m_BTN_Save;
	CButtonExt m_BTN_Exit;
	CButtonExt m_BTN_DrawLine;
	CButtonExt m_BTN_LineTest;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	int m_CB_nChoice;
	afx_msg void OnCbnSelchangeComboLineChoice();
	CButtonExt m_BTN_Cancel;
	afx_msg void OnBnClickedBntCancel();
	CButtonExt m_BTN_ModLine;
	afx_msg void OnBnClickedButtonModLine();
	afx_msg void OnMenuDrawcoat();
	afx_msg void OnMenuDrawfoil();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedBtnCalib();
	CButtonExt m_BTN_CalibSet;
//	int m_CB_nPosition;
//	afx_msg void OnCbnSelchangeCombPosition();
	CStaticExt m_STC_Example;
	CFont m_List_Font;
	int m_COM_nSortType;
	afx_msg void OnSelchangeComSort();
//	double m_EDT_dCurrentWidth;
//	double m_EDT_dRealWidth;
//	double m_EDT_dColumnResolution;
//	afx_msg void OnBnClickedBtnStartcalibresolution();
//	afx_msg void OnBnClickedBtnSaveresolution();
//	double m_EDT_dMinScore;
//	double m_EDT_nMaxMatchNum;
//	BOOL m_CHK_bUseModel;
//	afx_msg void OnBnClickedChkUsemodelmatch();
//	afx_msg void OnEnChangeEdtMinscore();
//	afx_msg void OnEnChangeEdtMaxmatchnumber();
//	afx_msg void OnBnClickedBtnDrawmodel();
//	afx_msg void OnBnClickedBtnMatchtest();
//	CButtonExt m_BTN_CDrawmodel;
//	CButtonExt m_BTN_CMacthTest;
//	afx_msg void OnBnClickedBtnCreatereference();
//	afx_msg void OnBnClickedBtnCreatereference();
//	afx_msg void OnBnClickedBtnClearmodel();
//	int m_COM_bUseAoBanSeg;
	BOOL m_CHK_bUseAoBanSegMode;
	int m_COM_bFindAobanManualOrAuto;
	int m_EDT_nAoBanThresh;
	double m_EDT_dAoBanDetectRegionWidth;
	double m_EDT_dFoilDetectRegionWidth;
	int m_EDT_nFoilDetectThresh;
	afx_msg void OnEnChangeEdtFoildetectregion();
	afx_msg void OnBnClickedChkUseaobanseg();
	afx_msg void OnCbnSelchangeComAobanregionmode();
	afx_msg void OnEnChangeEdtFoildetectthresh();
	afx_msg void OnEnChangeEdtAobanetectregion();
	afx_msg void OnEnChangeEdtAobandetectthresh();
//	afx_msg void OnBnClickedBtnDrawrectangle();
//	afx_msg void OnBnClickedBtnDrawrightrectangle();
	BOOL m_CHK_bEnableVariableWidth;
	afx_msg void OnBnClickedChkUsevariabwidthdetect();
	double m_EDT_dRelateOffset;
	afx_msg void OnEnChangeEdtRelateoffset();
};
