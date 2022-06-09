#pragma once

#include "PageCount.h"
#include "PageSystemParam.h"
#include "PageControlParam.h"
#include "PageWarn.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"
#include "Labeller.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"


// CDlgLabelInfoDisp 对话框
#define UM_TIMER_FLUSH WM_USER+11
#define WM_DISPLISTMESSGE (WM_USER+1000)   
extern CBlendent g_Blendent;
//extern int       WM_DISPLISTMESSGE ;
class CDlgLabelInfoDisp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLabelInfoDisp)

public:
	CDlgLabelInfoDisp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLabelInfoDisp();

// 对话框数据
	enum { IDD = IDD_DLG_LABELINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void SetLightColor(CButtonExt& btn,int nFlag=0);//设置指示灯的颜色
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	DECLARE_MESSAGE_MAP()
public:
	void 	FlushList();
	void InsertMessage(CString strWarnMessage, int nLightColor=0);
	virtual BOOL OnInitDialog();
	CFont m_ftButton;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CStaticExt m_STC_strIp;
	
	afx_msg void OnBnClickedBtnClearcount();
	afx_msg void OnBnClickedButtononemark();
	afx_msg void OnBnClickedButtontwomark();
	afx_msg void OnBnClickedButtononezd();
	afx_msg void OnBnClickedButtontwozd();
	BOOL m_bConnected;
	CLabeller* m_pOwner;
	int m_COM_nLabelName;
	int m_EDT_nTestAddress;
	float m_EDT_dTestValue;
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnBnClickedBtnHelp();
//	CReportCtrlExt m_LST_ParamList;
//	CReportCtrlExt m_LST_LabelCountList;
//	CReportCtrlExt m_LST_ControlList;
//	CReportCtrlExt m_LST_ErrorInfo;
	afx_msg void OnPaint();
//	afx_msg void OnBnClickedBtnReadspeed();
public:
	CPageCount* m_pPageCount;
	CPageSystemParam* m_pPageSystemParam;
	CPageControlParam* m_pPageControlParam;
	CPageWarn* m_pPageWarn;
	CTabCtrlExt m_TAB_CLabelFunction;

public:
	void ResetSize();
	std::vector<CDialogEx*> m_vecPages;
	void SwitchDispPage(const int & nSN);
	afx_msg void OnSelchangeTabLabelfunction(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnQgyx();
	afx_msg void OnBnClickedBtnTqqg();
protected:
	afx_msg LRESULT OnDisplistmessge(WPARAM wParam, LPARAM lParam);
public:
//	CStaticExt m_STC_CSeBiao;
//	CStaticExt m_STC_CDuiShe;
	afx_msg void OnBnClickedBtnReadspeed();
	afx_msg void OnBnClickedBtnXdlqgyx();
	afx_msg void OnBnClickedBtnXdltqqg();
	CStaticExt m_STC_COAPSebiao1;
//	CStatic m_STC_CSeBiao2;
	CStaticExt m_STC_COAPSebiao2;
};
