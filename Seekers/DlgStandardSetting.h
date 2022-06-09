#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgStandardSetting 对话框

class CDlgStandardSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStandardSetting)

public:
	CDlgStandardSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgStandardSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_STANDARDSETTING };
public:
	CJudge*       m_pOwner;
	CFont       m_FontBig;
	std::vector<CString> m_vec_strErrorName;
	void SetValue();
	void GetValue();
	void ComPareChange(int LastValue,int CurrentValue,CString strName);
	void ComPareChange(double LastValue,double CurrentValue,CString strName);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	void      InitUI();
	virtual BOOL OnInitDialog();
	void GetLSTValue(CAOWEISIZE& tCaoWeiSize,CReportCtrlExt& LstValue,int nSN=0);
	void InsertLSTValue(CAOWEISIZE& tCaoWeiSize,CReportCtrlExt& LstValue,int nSN=0);

	void GetLSTFoilAndCoatValue(STANDARDSIZE& tCoatAndFoilSize, CReportCtrlExt& LstValue, int nSN = 0);
	void InsertLSTFoilAndCoatValue(STANDARDSIZE& tCoatAndFoilSize, CReportCtrlExt& LstValue, int nSN = 0);

	/*void InitLabelSzieStandardList();*/

	double m_EDT_dAllLength;
	double m_EDT_dAllLengthDev;
	double m_EDT_dCoatWidthDev;
	double m_EDT_dBaibianDev;
	double m_EDT_dLocateMissingDev;
	BOOL m_CHECK_bEnableCoatWidth;
	BOOL m_CHECK_bEnableLocateMissing;
	double m_EDT_dCoatWidth;
	double m_EDT_dLocateMissing;
	afx_msg void OnLvnItemchangedListSurface(NMHDR *pNMHDR, LRESULT *pResult);
	CButtonExt m_btn_Cancel;
	CButtonExt m_btn_Delete;
	CButtonExt m_btn_OK;
	CFont   m_FontButton;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnReturn();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	double m_EDT_dMaxHeightDistance;
	BOOL m_CHK_bEnableAllLength;
	BOOL m_CHK_bBHJXLength;
	double m_EDT_dBHJXLength;
	double m_EDT_dBHJXLengthDev;
	double m_EDT_dUpInterMissionHeight;
	double m_EDT_dUpInterMissionHeightDev;
	double m_EDT_dDownInterMissionHeight;
	double m_EDT_dDownInterMissionHeightDev;
	BOOL m_CHK_bEnableDownInterMissionHeight;
	BOOL m_CHK_bEnableUpImHeight;
	BOOL m_CHK_bBHJXSumLabel;
	BOOL m_CHK_bUpJXLabel;
	BOOL m_CHK_bDownJXLabel;
	BOOL m_CHK_bEnableABMiss;
	double m_EDT_dABMissValue;
	double m_EDT_dABMissValueDev;
	BOOL m_CHK_bABMissLabel;
	BOOL m_CHK_bCoatWidthLabel;
	BOOL m_CHK_bUpAt9Label;
	BOOL m_CHK_bTotalLengthLabel;
	double m_EDT_dBHJXMeanLengthDev;
	double m_EDT_dBHJXMeanLength;
	BOOL m_CHK_bEnableBHJXMeanLength;
	BOOL m_CHK_bEnableCuoWei;
	double m_EDT_dCuoWeiMax;
	int m_EDT_nTrip;
	int m_COM_nCaoWeiChoice;
	int m_COM_nCoatType;
	int m_COM_nProductJiXi;
	int m_COM_nOapChoice;
	int m_COM_nExistJXType;
//	int m_EDT_nCaoWeiNum;
	CReportCtrlExt m_LST_CaoWeiParam0;
	CReportCtrlExt m_LST_CaoWeiParam1;
	int m_COM_nGongYiLeiXing;
	afx_msg void OnSelchangeComGongyileixing();
	BOOL m_CHK_bChangMoZaiShang;
	BOOL m_CHK_bQiTouXianChu;
//	afx_msg void OnBnClickedChkChangmozaishang();
	int m_EDT_nZhiDaiThresh;
	///新加的///
	double m_EDT_dTotalCoatWidth;
	double m_EDT_dTotalCoatWidthDev;
	double m_EDT_dBHJXLength2;
	double m_EDT_dBHJXLengthDev2;
	double m_EDT_dHeadCuoWei;
	double m_EDT_dHeadCuoWeiDev;
	double m_EDT_dTailCuoWei;
	double m_EDT_dTailCuoWeiDev;
	BOOL m_CHK_BHJXLabel2;
	BOOL m_CHK_bEnableBHJXLength2;
	BOOL m_CHK_bEnableTotalWidth;
	BOOL m_CHK_bEnableHeadCuoWei;
	BOOL m_CHK_bEnableTailCuoWei;
	BOOL m_CHK_bHeadCuoWeiLabel;
	BOOL m_CHK_bTailCuoWeiLabel;
	BOOL m_CHK_bToatlWidthLabel;
};
