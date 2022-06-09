#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"
#include "afxmenubutton.h"
#include "afxcmn.h"
#include "afxpropertygridctrl.h"
// CPageSet 对话框

const int Tolerance = 50;
const CString NAME_Feature[]=
{
	"面积","直径","长短轴比","圆似度","矩似度","紧凑度","角度","孔洞数目","最小灰度","最大灰度","平均灰度",
	"灰度差值","长轴","短轴","行坐标","列坐标","宽","高",
};
const CString STR_Feature[]=
{
	"area","max_diameter","anisometry","circularity","rectangularity","compactness","phi","holes_num",
	"min","max","mean","deviation","ra","rb","row","column","width","height",
};
enum EM_Feature
{
	area,max_diameter,anisometry,circularity,rectangularity,compactness,phi,holes_num,G_min,G_max,G_mean,
	G_deviation,ra,rb,row,column,width,height,
};
const int FEATURE_LENGTH = height+1;//数组长度

class CDlgAlgorithm;
class CPageSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPageSet)

public:
	CPageSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETPARAM };

	PARAMETERS* m_ptParameters;

	void SetButtonStaus(BOOL bStaus);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void FreshParam();//更新参数
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnBnClickedButtonCreatemark();
	BOOL m_CHK_bUseBubbleDetect;
	BOOL m_CHK_bUseDarkDotDetect;
	BOOL m_CHK_bUseDarkLineDetect;
	BOOL m_CHK_bUseLeakFoilDetect;
	BOOL m_CHK_bUseLightDotDetect;
//	BOOL m_CHK_bUseLightLineDetect;
	BOOL m_CHK_bUseTapeDetect;
	double m_EDT_dBorderDarkCoff;
	double m_EDT_dBorderLightCoff;
	double m_EDT_dBorderErosionLength;
//	int m_EDT_dBubbleDynThresh;
//	int m_EDT_dBubbleInOutDiff;
	double m_EDT_dBubbleMinCircularity;
	double m_EDT_dBubbleMinArea;
	int m_EDT_nBubbleDynThresh;
//	int m_EDT_nBubbleInOutDiff;
//	double m_EDT_dDarkDotMaxArea;
	int m_EDT_nDarkDotDynThresh;
	double m_EDT_dDarkDotMinArea;
//	double m_EDT_dDarkDotMinHeight;
//	double m_EDT_dDarkDotMinWidth;
	int m_EDT_nDarkLineDynThresh;
//	double m_EDT_dDarkLineMinHeight;
	double m_EDT_dDarkLineMinRatio;
	double m_EDT_dDarkLineMinWidth;
	int m_EDT_nLeakFoilDynThresh;
//	double m_EDT_dLeakFoilMaxDiameter;
	double m_EDT_dLeakFoilMinArea;
	double m_EDT_dLeakFoilMinDeviation;
//	double m_EDT_dLeakFoilMinDiameter;
	int m_EDT_nLeakFoilThresh;

	double m_EDT_dLightDotMinArea;

	int m_EDT_nLightDotDynThresh;

	double m_EDT_dMidDarkCoff;
	double m_EDT_dMidLightCoff;
	afx_msg void OnBnClickedChkUsetapedetect();
	afx_msg void OnBnClickedChkUsedarkdotdetect();
	afx_msg void OnEnChangeEdtDarkdotmaxthresh();
	afx_msg void OnEnChangeEdtDarkdotminarea();
	afx_msg void OnBnClickedChkUselightdotdetect();
	afx_msg void OnEnChangeEdtLightdotminthresh();
	afx_msg void OnEnChangeEdtLightdotminarea();
	afx_msg void OnBnClickedChkUsedarklinedetect();
	afx_msg void OnEnChangeEdtDarklinedynthresh();
	afx_msg void OnEnChangeEdtDarklineminwidth();
	afx_msg void OnEnChangeEdtDarklineminwhratio();
	afx_msg void OnBnClickedChkUsebubbledetect();
	afx_msg void OnEnChangeEdtBubbledynthresh();
	afx_msg void OnEnChangeEdtBubblemindiam();
	afx_msg void OnEnChangeEdtBubblemincircularity();
	afx_msg void OnBnClickedChkUseleakfoildetect();
	afx_msg void OnEnChangeEdtLeakfoilthresh();
	afx_msg void OnEnChangeEdtLeakfoilminarea();
	afx_msg void OnEnChangeEdtLeakfoildynthresh();
	afx_msg void OnEnChangeEdtLeakfoilmindeviation();
	afx_msg void OnEnChangeEdtBordererosionlength();
	afx_msg void OnEnChangeEdtMidlightcoff();
	afx_msg void OnEnChangeEdtMiddarkcoff();
	afx_msg void OnEnChangeEdtBorderlightcoff();
	afx_msg void OnEnChangeEdtBorderdarkcoff();
	double m_EDT_dFoilDarkMinArea;
//	double m_EDT_dFoilDarkMinHeight;
	int m_EDT_nFoilDarkMinNum;
//	double m_EDT_dFoilDarkMinWidth;
	int m_EDT_nFoilDarkThresh;
//	double m_EDT_dFoilLightMinArea;
//	int m_EDT_nFoilLightThresh;
	BOOL m_CHK_bUseFoilDetect;
	afx_msg void OnBnClickedChkUsefoildetect();
	afx_msg void OnEnChangeEdtFoildarkthresh();
//	afx_msg void OnEnChangeEdtFoildarkminwidth();
//	afx_msg void OnEnChangeEdtFoildarkminheight();
	afx_msg void OnEnChangeEdtFoildarkminarea();
	afx_msg void OnEnChangeEdtFoildarkminnum();
//	afx_msg void OnEnChangeEdtFoillightthresh();
//	afx_msg void OnEnChangeEdtFoillightminarea();
	CMFCMenuButton m_MenuBtn_FeatureSelect;
	afx_msg void OnBnClickedMfcmenubtnFeature();
	void AddFeature(EM_Feature eFeature);
	void DeleteFeature();
	CReportCtrlExt m_LST_Temp;
	CMFCPropertyGridCtrl m_PropertyGrid_Algrithm;
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnBnClickedBtnAddalgrithm();
	afx_msg void OnBnClickedBtnDeletealgrithm();
	void  WriteParam();
	BOOL IsNumber(CString& strText);

	afx_msg void OnBnClickedBtnLoadparam();

	double m_EDT_dScaleFactor;
	afx_msg void OnEnChangeEdtScalefactor();
	int m_EDT_nTapeDynThresh;
//	int m_EDT_nLTMaxThresh;
	double m_EDT_dLTMinArea;
	afx_msg void OnEnChangeEdtLthresh();
//	afx_msg void OnEnChangeEdtHthresh();
	afx_msg void OnEnChangeEdtMinlighttapearea();
//	int m_EDT_nAT9MinThresh;
//	double m_EDT_dAT9MinWidth;
//	double m_EDT_dAT9MinHeight;
//	afx_msg void OnEnChangeEdtAt9lowthresh();
//	afx_msg void OnEnChangeEdtAt9minwidth();
//	afx_msg void OnEnChangeEdtAt9minheight();
//	afx_msg void OnEnChangeEdtLightdotmaxthresh();
//	int m_EDT_nAT9MaxThresh;
//	double m_EDT_dAT9DarkMinWidth;
//	double m_EDT_dAT9DarkMinArea;
//	afx_msg void OnEnChangeEdtAt9maxthresh();
//	afx_msg void OnEnChangeEdtAt9darkminwidth();
//	afx_msg void OnEnChangeEdtAt9minarea();
//	double m_EDT_dFoilLightMinWidth;
//	afx_msg void OnEnChangeEdtFoillightwidth();
	CComboBox m_COM_CErrorName;
	int m_COM_nErrorNameIndex;
	CFont m_Font;
	int m_EDT_nJDDarkThresh;
	double m_EDT_dJDDarkArea;
	int m_EDT_nJDLightThresh;
	BOOL m_CHK_bUseJDDetect;
	afx_msg void OnEnChangeEdtJdDarkthresh();
	afx_msg void OnEnChangeEdtJdMindarkarea();
	afx_msg void OnEnChangeEdtJdLightthresh();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	double m_EDT_dMLPSongjindu;
	afx_msg void OnChangeEdtMlpsongjindu();
	double m_EDT_dMinTapeDeviation;
	afx_msg void OnEnChangeEdtTapefactor();
	BOOL m_bEnbleUseClassyfy;
	afx_msg void OnClickedChkUseclassify();
	public:
		CDlgAlgorithm* m_pParentDlg;
		double m_EDT_dAT9DarkHeight;
		int m_EDT_nAT9DarkThresh;
		double m_EDT_dAT9DarkWidth;
		double m_EDT_dAT9LightHeight;
		int m_EDT_dAT9LightThresh;
		double m_EDT_dAT9LightWidth;
		BOOL m_CHK_bUseAT9Detect;
		afx_msg void OnBnClickedChkAt9enable();
		afx_msg void OnChangeEdtAt9lightthresh();
		afx_msg void OnChangeEdtAt9lightheight();
		afx_msg void OnChangeEdtAt9darkwidth();
		afx_msg void OnChangeEdtAt9darkthresh();
		afx_msg void OnChangeEdtAt9darkheight();
		afx_msg void OnChangeEdtAt9lightwidth();
		BOOL m_CHK_bUseDetectTuoWei;
//		int m_CHK_nTuoWeiHighThresh;
//		int m_CHK_nTuoWeiLowThresh;
		double m_EDT_dTuoWeiMinArea;
		int m_EDT_nTuoWeiHighThresh;
		int m_EDT_nTuoWeiLowThresh;
		afx_msg void OnClickedChkBusedetecttuowei();
		afx_msg void OnChangeEdtTuoweilowthresh();
		afx_msg void OnChangeEdtTuoweihighthresh();
		afx_msg void OnChangeEdtTuoweiminarea();
		double m_EDT_dRightBorderWidth;
		afx_msg void OnEnChangeEdtRightborderwidth();
		double m_EDT_dYaHenPingBiKuanDu;
		afx_msg void OnEnChangeEdtYahenpingbikuandu();
		int m_EDT_nMaxTapeThresh;
		double m_EDT_dMinTapeArea;
		afx_msg void OnEnChangeEdtTapethresh();
		afx_msg void OnEnChangeEdtMintapethresharea();
};
