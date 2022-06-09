
// SeekersDlg.h : ͷ�ļ�
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
// CSeekersDlg �Ի���

static BOOL WINAPI SystemCall(const CString& strMessage, void* wParam, void* lParam);//ϵͳ�ص�
class CSeekersDlg : public CDialogEx
{
// ����
public:
	CSeekersDlg(CWnd* pParent = NULL);	// ��׼���캯��

	~CSeekersDlg();

// �Ի�������
	enum { IDD = IDD_SEEKERS_DIALOG };

	friend class CDlgProductManager;



	BOOL m_bRunning;//�Ƿ�����
private:

	int m_nCurrentDisplayView;//��ǰ��ʾ����ͼ 0Ϊ�ɼ�ͼƬ 1Ϊ�ߴ���




private:
	

	void InitStartUp();

	void InitGlobalVariables();//��ʼ��ȫ�ֱ���

	void InitQueues();//��ʼ������

	void ClearQueues();//��ն���

	void InitThread();//��ʼ���߳�
	
	void InitGrabber();//��ʼ���ɼ�����Ϣ 


	void InitStatusBar();

	void InitUI();

	void InitStatic();

	void InitButton();

	void InitMenu();

	void InitWindowShow();//��ʼ����ʾ���

	void InitSubDlg();

	void ShowButton(int nType);

	void LoadVersionInfoList();//�汾��Ϣˢ��
public:
	BOOL Login();
	void IntervalTimeSave(int nIntervalMinites=5);
	void GenDispImage (HObject *ho_Image);
	void DispImage(C_ALLOCATORWORKSHOPOUTPUT& tOutput);//��ʷ��¼
	void History(C_ALLOCATORWORKSHOPOUTPUT& Output);//��ʷ��¼



	void Size(C_ALLOCATORWORKSHOPOUTPUT& Output);//�ߴ���ʾ

	void Sub(C_ALLOCATORWORKSHOPOUTPUT& Output);//�Ӵ���

	void Count(C_ALLOCATORWORKSHOPOUTPUT& Output);//������ʾ

	void LightAdjust(C_ALLOCATORWORKSHOPOUTPUT& OutPut);//��Դ����Ӧ

	void  WriteOutHistory(int& nOut0,int &nOut1);
private:
	BOOL DispHistoryAlgorithmDlg(const int &nGrabberSN,const int &nObjectIndex);//ѡ����ʷͼ������е�ͼ�񣬵����㷨����
	BOOL DispSubWinAlgorithmDlg(const int &nSubWindowsHandleSN);//ѡ�����ͼ������е�ͼ�񣬵����㷨����
	
	CString GetDataBaseConnection();//������ݿ��˺������������Ϣ

public:
	void ExitDo();//�����˳�ʱ����Ҫ��������
	void Reset();
	void SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus);
	    BOOL GetSingleFrameSurfaceResult(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);//ÿ�Ŷ��ж�һ���Ƿ�����
	CDlgFinalOutResult* m_pFinalOutResult;
	CDlgStartUp* m_pStartUp;
	CSubDlgSize* m_pSubSizeDlg;
	CSubDlgCount* m_pSubCountDlg;
	CDlgScatterPlot* m_pDlgScatterPlot;
	CStatusBarExt m_wndStatusBar;//״̬��
	CMenuExt *m_pMenuTools;//�Զ���˵�
	CDlgSwitchBatch* m_pDlgBatch;//
	CDlgPopError* m_pDlgPopError;//���󵯴���ʾ
private:
	void Release();
	void SaveCurrentStatus();
private:
	HObjectVector m_hovec_ResultGrayImage;
	HObjectVector m_hovec_ResultRGBImage;
	HTupleVector m_hvec_ResultSizeHandle;//�ߴ�����ʾ���
	HTuple m_hv_ResultSizeHandle;//�ߴ�����ʾ���
	HTupleVector m_hvec_MainWindowsHandle;//��������ʾ���
	HTupleVector m_hvec_HistroyWindowsHandle;//��������Ĳ�Ʒ����Ϣ
	HTupleVector m_hvec_SubWindowsHandle;//�Ӵ�����ʾ���
	std::list<SUBELEMENT> m_lstSubImage;//ImageList
	int     m_nSubPage;//��ʾ���ǵڼ�ҳ��СͼƬ
	HObjectVector m_hovec_Histroy;//Region
	HObjectVector m_hovec_Histroy_Image;//Image
	HObjectVector m_hovec_ResultRegion;//Region
	std::vector <C_ALLOCATORWORKSHOPOUTPUT> m_vec_ResultSize[2];
	
// ʵ��
protected:

	HICON m_hIcon;
	CBrush m_BrushBlack;//��ɫ��ˢ


	// ���ɵ���Ϣӳ�亯��
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();

	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()

public:

	CFont  m_ftButton;//��ť������
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
	 HTuple m_hvExitTimeCount;//�Զ��˳���ʱ����
	 HTuple m_hvExitLightTimeCount;//��Դ�Զ��˳���ʱ����

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
