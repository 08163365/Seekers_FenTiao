#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "Find.h"
#include "afxwin.h"
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
	enum { IDD = IDD_DIALOG_PARAM };

	CImageList m_TreeImages;


	BOOL CreatThreshModel(HObject& ho_ImageReduced);
	void DispImage(const HObject& hoImage);
	void DispMark(const SINGLEMARKINFO&  MarkDisp);
	void SetOwner(CFindMark* pOwner);
	BOOL Create_ShapeModel(HObject& ho_ImageReduced);
	BOOL Create_NCCModel(HObject& ho_ImageReduced);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
//	virtual void PostNcDestroy();
public:
	CTreeCtrl m_MarkTree;
	virtual BOOL OnInitDialog();
	BOOL IsDrawing();
	void Draw(CString strDrawType,HObject* Region);
	BOOL Create_ThreshIntermisssModel(HObject& ho_ImageReduced);
private:
	// 滚轮缩放
	CFont  m_ftButton;
	HTuple m_hvDispImageWidth;		// 显示图像宽
	HTuple m_hvDispImageHeight;		// 显示图像高
	double m_dDispImagePartRow0;
	double m_dDispImagePartCol0;
	double m_dDispImagePartRow1;
	double m_dDispImagePartCol1;
private:
	

	CBlendent m_tBlendent;
	void InitUI();
	void SetButton(CButtonExt& tButton);
	void InitTree();
	BOOL m_bDrawing;
	//
public:
	CFindMark* m_pRealDetect;
	CFindMark* m_pOwner;
	HObject m_hoDispRegion;
	void SetButtonStaus(BOOL bStaus);
public:
	HObject m_hoImage;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void MyMouseWheel(short zDelta);
	void MyMouseMove();
	void MyFitImage();
	HObject  m_hoCurrentDispImage;
	HTuple   m_hv_WindowID;
	afx_msg void OnBnClickedButtonAdd();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	int m_nSearchCoefficient_Column;
	CString m_strMarkType;
	CString m_strDrawType;
	CString m_strMarkName;
	afx_msg void OnBnClickedButtonOpenimage();
	afx_msg void OnNMDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuRename();
	CButtonExt m_BTN_OpenImage;
	CButtonExt m_BTN_AddMark;
	afx_msg void OnBnClickedButtonCancel();
	CButtonExt m_BTN_Cancel;
	CStaticExt m_cstaticTitle;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAddline();
	afx_msg void OnBnClickedButtonTest();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnEnChangeEdtIgnoredist();
	afx_msg void OnEnChangeEdtMaxthresh();
	afx_msg void OnBnClickedBtnSaveparam();
	CButtonExt m_BTN_CSaveParam;
	int m_EDT_nMaxThresh;
	double m_EDT_dIgnoreDist;
	double m_EDT_dMiniScore;
	CEditExt m_EDT_MinScore;
	CEditExt m_EDT_MarkName;
	CEditExt m_EDT_DilaColumn;
	CButtonExt m_BTN_ModifyLine;
	CButtonExt m_BTN_Test;
	int m_COM_nType;
	afx_msg void OnBnClickedBtnGetintermissionheight();
	int m_EDT_nBackLowThresh;
	int m_EDT_nBackHighThresh;
	afx_msg void OnChangeEdtBalcklowthresh();
	afx_msg void OnChangeEdtBackhighthresh();
	CStaticExt m_STC_CImageGrayValue;
	afx_msg void OnBnClickedBtnGetmean();
	double m_EDT_dFoilSaveHeight;
	afx_msg void OnChangeEdtSavefoilheight();
	double m_EDT_dJXEADev;
	afx_msg void OnEnChangeEdtJxeadev();
	BOOL m_bChangeFlag;//修改标志
	int m_COM_nLocateJXUpOrDown;
	double m_EDT_dLocateOffset;
	afx_msg void OnEnChangeEdtLocateoffset();
	afx_msg void OnCbnSelchangeComLocateupordown();
	int m_COM_nJXGray;
	afx_msg void OnCbnSelchangeComJxgray();
};
