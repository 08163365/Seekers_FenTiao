#pragma once
#include "Storage.h"
#include "afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"

// CDlgParamSet 对话框

class CDlgParamSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParamSet)

public:
	CDlgParamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgParamSet();
// 对话框数据
	enum { IDD = IDD_DIALOG_PARAMSET };

	void InitCheck();
	CBlendent m_tBlendent;

	CStorage* m_pOwner;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButtonExt m_checkWriteTime;
//	CButtonExt m_checkDiskImage;
	CButtonExt m_checkDiskErrorRegion;
	CButtonExt m_checkDiskSize;
	// 换卷时候删除好片图片
	CButtonExt m_checkDiskDeleteOK;
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CStaticExt m_STC_Title;
	CButtonExt m_BTN_OK;
	CButtonExt m_BTN_Cancel;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	int m_EDT_nDiskRemainVolume;
	int m_EDT_nCacheDays;
//	CString m_EDT_StrSavePath;
	CButtonExt m_checkDiskProcessInfo;
	CButtonExt m_checkDiskSmallImage;
	CString m_COM_StrImageFormat;
	int m_CHK_bSaveAllImage;
	CButtonExt m_CHK_SaveAllImage;
	BOOL m_CHK_bSaveInLocation;
	CButtonExt m_CHK_CSaveLocation;
	BOOL m_CHK_bSaveMarkImage;
	CButtonExt m_CHK_CSaveMarkImage;
	int m_COM_nImageType;
	CString m_MFCBROWSE_StrImageSavePath;
//	CButtonExt m_checDefinedPath;
//	afx_msg void OnBnClickedCheckChoicepath();
	CString m_EDT_StrMachineSN;
	BOOL m_CHK_bEnableUpLoadImage;
	CString m_EDT_StrUpLoadImagePath;
};
