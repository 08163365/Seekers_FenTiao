#pragma once
#include "XmvImageWnd.h"
#include "afxwin.h"
#include "PageBaseLine.h"
#include "PageGRRSet.h"
#include "PageSpotDetectSet.h"
#include "afxcmn.h"
// CDlgCreateBaseLine �Ի���

class CDlgCreateBaseLine : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCreateBaseLine)

public:
	CDlgCreateBaseLine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCreateBaseLine();

// �Ի�������
	enum { IDD = IDD_DLAGFINDEDAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	CXmvImageWnd m_Wnd_FirstImage;//ǰ�������
	CXmvImageWnd m_Wnd_SecondImage;//���������
	virtual BOOL OnInitDialog();
	void FindCoatRegion (HObject ho_Image, HObject ho_ROI_0, HObject *ho_SelectedRegions, 
		HTuple hv_nLowThresh,HTuple hv_nHighThresh);
public:
	HObject m_hoFirstImage;
	HObject m_hoSecondImage;
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	afx_msg void OnBnClickedBtnCancel();
	CStaticExt m_STC_CTitle;
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFont m_BTN_font;
	CButtonExt m_BTN_CReturn;
	CStaticExt m_STC_CGrayValue;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	
	CPageBaseLine* m_pPageBaseLine;
	CPageGRRSet* m_pPageGRRSet;
	CPageSpotDetectSet* m_pPageSpotDetectSet;
	std::vector<CDialogEx*> m_vecPages;
	CTabCtrl m_Tab_Set;
	afx_msg void OnSelchangeTabSetting(NMHDR *pNMHDR, LRESULT *pResult);
	void SwitchDispPage(const int& nSN);
};
