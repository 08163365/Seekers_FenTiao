#pragma once
#include "afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"


// CDlgSystemSet 对话框

class CDlgSystemSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSystemSet)

public:
	CDlgSystemSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSystemSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_SYSTEMSET };

	void InitStatic();
	BOOL Save();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStaticExt m_cstaticColumnResolution0;
	CStaticExt m_cstaticColumnResolution1;
	CStaticExt m_cstaticRowResolution0;
	CStaticExt m_cstaticRowResolution1;
	CFont     m_FontEdit;
	CFont     m_FontCombo;
	CFont	  m_FontButton;
	afx_msg void OnBnClickedCheckConsecutiveerrorAlarm();
	int m_nHistoryType;
	int m_nHistoryCount;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CEditExt m_EDIT_historyCount;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	CStaticExt m_STC_Title;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonClose();
	CButtonExt m_BTN_close;
	CButtonExt m_BTN_cancel;
	CButtonExt m_BTN_ok;
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonCancel();
	void ComPareChange(int LastValue,int  CurrentValue,CString strName);
	// 是否拼接图片
	BOOL m_CHECK_bTileImage;
//	int m_EDT_nStepLength;
//	CEditExt m_EDT_StepLength;
	BOOL m_CHECK_bSaveTitleImage;
	int m_EDT_nDistanceCamera;
	CEditExt m_EDT_DistanceCamera;

//	double m_EDT_dwMaxFoilDistance;
//	CEditExt m_EDTC_MaxFoilDistance;
//	double m_EDT_dEADistDeviation;
//	CEditExt m_EDT_CEADistDeviation;
	int m_EDT_nOverTimeExit;
	CEditExt m_EDT_COverTimeExit;
	int m_COM_nCameraModel;
	BOOL m_CHK_bEnableReLoadParam;
	BOOL m_CHK_bPopWindowToWarn;
	int m_EDT_nMaxPopWindowNum;
	BOOL m_CHK_bEnableMergeParam;
	BOOL m_CHK_bEnableMergeResult;
	int m_EDT_nOverTimeFrames;
	int m_EDT_nTimeLimitValue;
	int m_EDT_nOverTimeLimitLoginOff;
};
