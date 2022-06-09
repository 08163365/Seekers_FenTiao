#pragma once
#include "afxwin.h"
#include "PageSet.h"
#include "Battery.h"
#include "resource.h"
#include "MLPDlg.h"
#include "PageGRRSet.h"
#include "PageLineSet.h"
#include "afxcmn.h"
// CDlgAlgorithm 对话框


class CDlgAlgorithm : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlgorithm)

public:
	CDlgAlgorithm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAlgorithm();

// 对话框数据
	enum { IDD = IDD_DIALOG_ALGORITHM };
public:
	DETECTINPUT m_DetectInput;
	DETECTOUTPUT m_DetectOut;
	CBattery* m_pDetect;//拷贝检测类,防止直接对传进来的检测类操作，一切先在该类上面进行操作,然后退出时将更改保存值m_pRealDetect
	CBattery* m_pRealDetect;//实际检测类,
	int m_ncount;
	HObject m_hoCurrentDispImage;
	HObject m_hoCurrentDispRegion;//鼠标移动自动检测出来的区域
	HTuple  m_hvWindowHandle;


	double m_dDispImagePartRow0;
	double m_dDispImagePartCol0;
	double m_dDispImagePartRow1;
	double m_dDispImagePartCol1;

	CMenuExt *m_pMenuTools;

	void InitStatic();
	CFont m_ListFont;
	void Test(int nBreakType = 0);
	void SaveOperaterLog();//记录操作痕迹
	void ComPareChange(int& LastValue,int& CurrentValue,CString strName);
	void ComPareChange(double& LastValue,double& CurrentValue,CString strName);
	void Analysisregion(HObject hoRegion);
	void BackFromCreateModel();//从制作模版界面返回
	CFont m_ftButton;
private:
	void InsertList();
	BOOL InsertListSizeError(const int i);
	void InsertSurfaceError(const int i);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	
	CStaticExt m_cstaticTitle;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButtonExt m_btnTest;
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnExit();
	CButtonExt m_btnExit;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMenuAnalysisregion();
	CReportCtrlExt m_listInfo;
	afx_msg void OnMenuAutoAnalysisregion();
	
	afx_msg void OnMenuFindfoil();
	afx_msg void OnMenuDispcoating();
	afx_msg void OnMenuDispfoil();
	afx_msg void OnBnClickedBtnDefectset();
	afx_msg void OnBnClickedBtnSavepara();
	CButtonExt m_btnSavePara;
	afx_msg void OnMenuFirstdetect();
	afx_msg void OnMenuSaveimage();
	afx_msg void OnLvnItemchangedListInfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuCheckset();
	afx_msg void OnBnClickedBtnTestall();
	afx_msg void OnPaint();
	CButtonExt m_btnTestAll;
	CString m_StrUserName;
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	CButtonExt m_btnSaveImage;
	afx_msg void OnBnClickedBtnSaveimage();
	CStaticExt m_STC_GrayValue;
	afx_msg void OnOpenimage();
	afx_msg void OnMenuCoatBorder();
	afx_msg void OnMenuCoatMid();
	CButtonExt m_btnTools;
	afx_msg void OnClickedButtonTools();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnMenuLight();
	afx_msg void OnMenuDark();
	afx_msg void OnMenuSegregion();
	afx_msg void OnMenuAt9region();
	afx_msg void OnBnClickedBtnSavesampleimage();
	afx_msg void OnBnClickedBtnLoadsampleimage();
	CButtonExt m_bnSaveSampleImage;
	CButtonExt m_btn_LoadSampleImage;
public: 
	afx_msg void OnTcnSelchangeTabParamset(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedBtnHelp();
	void SwitchDispPage(const int& nSN);

public:
		HObject m_hoOriginImage;//原图
		CButtonExt m_btn_Help;
		CPageSet* m_PageSurfaceParamSet;//缺陷参数界面
		CMLPDlg*  m_PageMLPParamSet;//分类器界面
		CPageGRRSet* m_PageGRRParamSet; //GRR参数界面
		CPageLineSet * m_PageMeasureParamSet; //分割参数界面
		std::vector<CDialogEx*> m_vecPages;
		CTabCtrl m_TAB_CParamSet;
		
		afx_msg void OnMenuDrawcoat();
		afx_msg void OnMenuJxregion();
		afx_msg void OnMenuTripregion();
		afx_msg void OnMenuYahenpingbiqu();
};
