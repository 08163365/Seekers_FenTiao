#pragma once
#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"
#include "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\atlmfc\include\afxcmn.h"

// CDlgIOSetting 对话框
enum eTieBiaoMoShi
{
    ZhengChangTiebiao,//正常模式贴标
	PianYiTiebiao,//按照偏移距离贴标
};


class CDlgIOSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgIOSetting)

public:

	CDlgIOSetting(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CDlgIOSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_IOSETTING };

public:

	CFont  m_ftButton;//按钮的字体
	CFont  m_ftEdit;
	CBlendent m_Blendent;

	CCommunicate *m_pCommuicate;//类指针

private:

	void InitUI();

	void InitValue();

	void Save();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
//	int m_nContinuousErrorAlarmsIO;
//	int m_nReadyIO;

	int m_nIOCardSN;
	int m_nRejectType;
//	int m_nFrontDelay;
//	int m_nBehindDelay;
	int m_nRejectDelay;
//	int m_nPulseWidth;
//	int m_nContinuousErrorCountMax;

//	double m_dMarkerLengh;

//	CButtonExt m_btnTestContinuousErrorAlarms;
//	CButtonExt m_btnTestReady;
	CButtonExt m_btnCallIOCardParamset;
	CButtonExt m_btnOK;
//	CButtonExt m_btnContinuousErrorSet;

//	CReportCtrlExt m_listIO;

	CStaticExt m_cstaticTitle;

//	CEditExt m_EDIT_continuousErrorAlarmsIO;
//	CEditExt m_EDIT_behindDelay;
//	CEditExt m_FrontDelay;
	CEditExt m_EDIT_rejectDelay;
	CEditExt m_IOCardSN;

//	CEditExt m_EDIT_pulseWidth;
//	CEditExt m_EDIT_ready;
//	CEditExt m_EDIT_markerLengh;
//	CEditExt m_EDIT_continuousErrorCountMax;
	
	virtual BOOL OnInitDialog();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnPaint();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

//	afx_msg void OnBnClickedButtonTestReady();

//	afx_msg void OnBnClickedButtonTestContinuouserroralarms();

//	afx_msg void OnBnClickedButtonContinuouserrorset();

	afx_msg void OnBnClickedButtonCalliocardparamset();

	afx_msg void OnBnClickedButtonOk();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonClose();

	CButtonExt m_BTN_Cancel;
	CButtonExt m_BTN_Close;
	int m_EDT_DabiaoDelay;
	CEditExt m_EDT_CDABIAODELAY;
	CEditExt m_EDIT_rejectDelay1;
	int m_nRejectDelay1;//第二路打标队列延迟行数
	int m_EDT_nTiFeiPulse;
	CEditExt m_EDT_CTiFeiPulse;
	int m_EDT_nBuChangHangShu;
	CEditExt m_EDT_CBuChangHangShu;
//	CComboBox m_COM_CTieBiaoType;
//	double m_EDT_dLableOffSet;
//	CEditExt m_EDT_CLableOffSet;
//	int m_COM_nTieBiaoType;
//	afx_msg void OnCbnSelchangeComTiebiaomoshi();
	double m_EDT_dLabelActionLocation;
	double m_EDT_dFirstLabelLength;
	double m_EDT_dSecondLabelLength;
//	afx_msg void OnEnChangeEdtLabelactionoffset();
//	CEdit m_EDT_CFIRSTLABELLENGTH;
//	CEdit m_EDT_CSECONDLABELLENGTH;
	CEditExt m_EDT_CLabelActionOffset;
	CEditExt m_EDT_CSecondLabelLength;
	CEditExt m_EDT_CFirstLabelLength;
//	afx_msg void OnEnChangeEdtLabelactionoffset();
	afx_msg void OnBnClickedBtnCaldistinmm();
	afx_msg void OnBnClickedBtnRereshdelayline();

	int m_EDT_nFreshImageSNDelay;
	int m_EDT_nTBFenPinNum;
	CEditExt m_EDT_CTBFenPin;
	CEditExt m_EDT_CFreshImageSN;

	BOOL m_CHK_bEnableLabelSet;
	int m_COM_nChannel0;
	int m_COM_nChannel1;
	int m_COM_nChannel10;
	int m_COM_nChannel11;
	int m_COM_nChannel2;
	int m_COM_nChannel3;
	int m_COM_nChannel4;
	int m_COM_nChannel5;
	int m_COM_nChannel6;
	int m_COM_nChannel7;
	int m_COM_nChannel8;
	int m_COM_nChannel9;
	afx_msg void OnBnClickedChkEnablesetlabel();
	double m_EDT_dVirtualEALength;
	int m_COM_nTestLabelIndex;
	afx_msg void OnBnClickedBtnTestlabelouput();
	afx_msg void OnBnClickedBtnLabelreset();
	BOOL m_CHK_bMachineLengthLabel;
	double m_dLabelOffsetDev;
	double m_EDT_dDownLabelOffsetCompensate;
	double m_EDT_dUpLabelOffsetCompensate;
	BOOL m_CHK_bUseLabelOffset;
	CReportCtrlExt m_LST_CLabelOffsetInfo;
	afx_msg void OnBnClickedBtnOnekeycalib();
//	afx_msg void OnBnClickedBtnReadsensor();
};
