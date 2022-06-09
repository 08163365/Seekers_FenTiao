
// SeekersDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "DlgScatterPlot.h"
#include "ThreadOperations.h"
#include "afxwin.h"
#include "SubDlgSize.h"
#include "DlgException.h"
#include "SubDlgCount.h"
#include "SubDlgControl.h"
#include "DlgStartUp.h"
#include "DlgFinalOutResult.h"
#include "DlgSwitchBatch.h"
#include "DlgPopError.h"

#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"


const int VIEW_PICTURE = 0;
const int VIEW_SIZE    = 1;
//const int VIEW_COUNT   = 2;
const int VIEW_FINAL   = 2;
const int VIEW_LINECHART= 3;
// CSeekersDlg 对话框

static BOOL WINAPI SystemCall(const CString& strMessage, void* wParam, void* lParam);//系统回调
class CSeekersDlg : public CDialogEx
{
// 构造
public:
	CSeekersDlg(CWnd* pParent = NULL);	// 标准构造函数

	~CSeekersDlg();

// 对话框数据
	enum { IDD = IDD_SEEKERS_DIALOG };

	friend class CDlgProductManager;



	BOOL m_bRunning;//是否运行
private:

	int m_nCurrentDisplayView;//当前显示的视图 0为采集图片 1为尺寸栏




private:
	

	void InitStartUp();

	void InitGlobalVariables();//初始化全局变量

	void InitQueues();//初始化队列

	void ClearQueues();//清空队列

	void InitThread();//初始化线程
	
	void InitGrabber();//初始化采集端信息 


	void InitStatusBar();

	void InitUI();

	void InitStatic();

	void InitButton();

	void InitMenu();

	void InitWindowShow();//初始化显示句柄

	void InitSubDlg();

	void ShowButton(int nType);

	void LoadVersionInfoList();//版本信息刷新
public:
	BOOL Login();
	void IntervalTimeSave(int nIntervalMinites=5);
	void GenDispImage (HObject *ho_Image);
	void DispImage(C_ALLOCATORWORKSHOPOUTPUT& tOutput);//历史记录
	void History(C_ALLOCATORWORKSHOPOUTPUT& Output);//历史记录



	void Size(C_ALLOCATORWORKSHOPOUTPUT& Output);//尺寸显示

	void Sub(C_ALLOCATORWORKSHOPOUTPUT& Output);//子窗口

	void Count(C_ALLOCATORWORKSHOPOUTPUT& Output);//计数显示

	void LightAdjust(C_ALLOCATORWORKSHOPOUTPUT& OutPut);//光源自适应

	void  WriteOutHistory(int& nOut0,int &nOut1);
private:
	BOOL DispHistoryAlgorithmDlg(const int &nGrabberSN,const int &nObjectIndex);//选择历史图像队列中的图像，调用算法界面
	BOOL DispSubWinAlgorithmDlg(const int &nSubWindowsHandleSN);//选择错误图像队列中的图像，调用算法界面
	
	CString GetDataBaseConnection();//获得数据库账号密码等连接信息

public:
	void ExitDo();//程序退出时候需要做的事情
	void Reset();
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	    BOOL GetSingleFrameSurfaceResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);//每张都判断一下是否贴标
	CDlgFinalOutResult* m_pFinalOutResult;
	CDlgStartUp* m_pStartUp;
	CSubDlgSize* m_pSubSizeDlg;
	CSubDlgCount* m_pSubCountDlg;
	CDlgScatterPlot* m_pDlgScatterPlot;
	CStatusBarExt m_wndStatusBar;//状态栏
	CMenuExt *m_pMenuTools;//自定义菜单
	CDlgSwitchBatch* m_pDlgBatch;//
	CDlgPopError* m_pDlgPopError;//错误弹窗提示
private:
	void Release();
	void SaveCurrentStatus();
private:
	HObjectVector m_hovec_ResultGrayImage;
	HObjectVector m_hovec_ResultRGBImage;
	HTupleVector m_hvec_ResultSizeHandle;//尺寸结果显示句柄
	HTuple m_hv_ResultSizeHandle;//尺寸结果显示句柄
	HTupleVector m_hvec_MainWindowsHandle;//主窗口显示句柄
	HTupleVector m_hvec_HistroyWindowsHandle;//最近检测过的产品的信息
	HTupleVector m_hvec_SubWindowsHandle;//子窗口显示句柄
	std::list<SUBELEMENT> m_lstSubImage;//ImageList
	int     m_nSubPage;//显示的是第几页的小图片
	HObjectVector m_hovec_Histroy;//Region
	HObjectVector m_hovec_Histroy_Image;//Image
	HObjectVector m_hovec_ResultRegion;//Region
	std::vector <C_ALLOCATORWORKSHOPOUTPUT> m_vec_ResultSize[2];
	
// 实现
protected:

	HICON m_hIcon;
	CBrush m_BrushBlack;//黑色画刷


	// 生成的消息映射函数
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

public:

	CFont  m_ftButton;//按钮的字体
	void Verify();
	void CrashRecord(); 
	
	HTuple m_hvUpContinueErrorFlag;
	HTuple m_hvDownContinueErrorFlag;
	CStaticExt m_staticTitle;
//	CStaticExt m_STC_CountTrip1;
//	CStaticExt m_staticMachineSpeed;
	CStaticExt m_staticBatch;
	CStaticExt m_staticProductName;
//	CStaticExt m_cstaticYield;
//	CStaticExt m_cstaticMachineSpeed;
	CStaticExt m_cstaticBatch;
	CStaticExt m_cstaticProductName;

	CButtonExt m_btnSubBack;
	CButtonExt m_btnSubNext;
	CButtonExt m_btnExitAppication;
	CButtonExt m_btnStart;
	CButtonExt m_btnSwitchBatch;
	CButtonExt m_btnLogin;
//	CButtonExt m_btnMap;
	CButtonExt m_btnProductManager;
	CButtonExt m_btnSwitchViews;
	CButtonExt m_btnTools;
	CButtonExt m_btnCalibResolution;
	CButtonExt m_BTN_DailyUse;

	afx_msg void OnBnClickedButtonStart();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedButtonExitApplication();

	afx_msg void OnBnClickedButtonSwitchViews();

	afx_msg void OnBnClickedButtonSwitchBatch();

//	afx_msg void OnBnClickedButtonMap();

	afx_msg LRESULT OnDisp(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnGrabber(WPARAM wParam, LPARAM lParam);



	afx_msg LRESULT OnDetect(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnRestartProcess(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonProductManager();

	afx_msg void OnBnClickedButtonLogin();

	afx_msg void OnBnClickedButtonSubnext();

	afx_msg void OnBnClickedButtonSubback();

	afx_msg void OnClose();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);

	afx_msg void OnStnClickedStaticBatch();

	afx_msg void OnStnDblclickStaticTitle();

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg void OnBnClickedButtonTools();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnMenuLaneset();
//	afx_msg void OnMenuLinematchset();
	afx_msg void OnMenuProset();
	afx_msg void OnMenuAlgparam();
	afx_msg void OnMenuRestart();
	CButtonExt m_BTN_DrawLeftModel;
//	CButtonExt m_BTN_DrawRightModel;
	afx_msg void OnBnClickedBtnDrawleftmodel();
//	afx_msg void OnBnClickedBtnDrawrightmodel();
	afx_msg void OnMenuTcpconnect();
	void InitLabelCountList();
	void UpdateLabelCount();

	void InitLabelCaoWeiList();
	void UpdateLabelCaoWeiList(C_ALLOCATORWORKSHOPOUTPUT& Output);

	CFont m_ListLabelFont;

	CStaticExt m_STC_CMeter;
	CStaticExt m_STC_CMeterName;
	CListCtrlExt m_LST_LabelCount;
	afx_msg void OnBnClickedBtnClearlabelcount();
	CButtonExt m_BTN_ClearLabeCount;
	afx_msg void OnBnClickedBtnCalibset();
	
	afx_msg void OnBnClickedBtnFindedage(); 
	CButtonExt m_BTN_CFindEdage;
	 HTuple m_hvExitTimeCount;//自动退出计时变量
	 HTuple m_hvExitLightTimeCount;//光源自动退出计时变量

	 afx_msg void OnBnClickedBtnLightonoff();
	 CButtonExt m_BTN_CLightOnOff;
	 afx_msg void OnBnClickedBtnCleardisk();
	 CButtonExt m_btn_ClearDisk;
	 afx_msg void OnMenuGuigeshezhi();
	 afx_msg void OnMenuPlc();
	 afx_msg void OnBnClickedBtnDailyuse();
	 CListCtrlExt m_LST_LableCaoWei;
	 HTupleVector m_hvec_PerAtCuoWei;
	
};
