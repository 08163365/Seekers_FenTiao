#pragma once
#include "XmvImageWnd.h"
#include "afxwin.h"

// CDlgMarkCreate 对话框

class CDlgMarkCreate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMarkCreate)

public:
	CDlgMarkCreate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMarkCreate();

// 对话框数据
	enum { IDD = IDD_DLG_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	DECLARE_MESSAGE_MAP()
public:
	void ComPareChange(double& LastValue,double& CurrentValue,CString strName);//记录日志
	void ComPareChange(int& LastValue,int& CurrentValue,CString strName);//记录日志
	void FlushParam();
	void SetButtonStaus(BOOL bStaus);
	CXmvImageWnd m_Wnd_Image;
	HObject m_hoImage;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButtonExt m_BTN_Return;
	CButtonExt m_BTN_Cancel;
	CButtonExt m_BTN_OK;
	CStaticExt m_STC_Title;
	CFont m_ftButton;
	afx_msg void OnBnClickedBtnReturn();
	afx_msg void OnBnClickedCancel();
	int m_COM_nFitType;
	int m_COM_nReferPoint;
	double m_EDT_dOffSet;
	int m_EDT_nLowThresh;
	int m_EDT_nHighThresh;
	int m_EDT_nExtractFoilThresh;
	int m_EDT_dFoilHeight;
	int m_EDT_dFoilWidth;
	BOOL m_CHK_bUseManualSeg;
	afx_msg void OnBnClickedBtnDetectregion();
	afx_msg void OnBnClickedBtnClearregion();
	BOOL m_CHK_bUseAotoFind;
	CBattery* m_pTempDetect;
	afx_msg void OnBnClickedBtnTest();
	HObject m_hoExistInterMissionRegion;//存在间歇的区域
	HTuple m_hvWindowID;
	CButtonExt m_BTN_CImTest;
	CButtonExt m_BTN_CDrawImRegion;
	CButtonExt m_BTN_CClearIMRegion;
	BOOL m_CHK_bHelpDraw;
	afx_msg void OnBnClickedBtnDispjxdetect();
	double m_EDT_dJXNearCoatHeight;
	BOOL m_CHK_bUseFirstLengthGRR;
	int m_COM_nFirstLengthLocation;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CStaticExt m_STC_CGrayValue;
	BOOL m_CHK_bExistMarkInCoat;
	afx_msg void OnBnClickedBtnDrawmark();
	CButtonExt m_BTN_CDispJXModel;
	CButtonExt m_BTN_CDrawCoatMark;
	BOOL m_CHK_bPreProcess;
	int m_EDT_nLineThresh;
	int m_EDT_nBotLineThresh;
	int m_COM_nFirstOrLastPoint;
	double m_EDT_dNearCoatOffset;
};
