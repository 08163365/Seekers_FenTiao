#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "resource.h"
// CDlgPopError 对话框
struct PopErrorInfo
{
	    int nSide;
		CString StrErrorInfo; 
		HTuple   hvGrabberSN;
		HObject  hoImage;
		HObject  hoErrorRegion;

};


class CDlgPopError : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPopError)

public:
	CDlgPopError(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPopError();

// 对话框数据
	enum { IDD = IDD_DLG_POPERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	void Disp();
	PopErrorInfo m_tPopErrorInfo;
	virtual BOOL OnInitDialog();
	CString m_STC_StrErrorName;
	CFont  m_Font;
	afx_msg void OnBnClickedCancel();
	HTuple  m_hvWindowHandle;
//	afx_msg void OnBnClickedButton();
	CStaticExt m_STC_CErrorName;
	afx_msg void OnBnClickedBtnManuallabel();
	CButton m_btnCancel;
	CButton m_btn_ManulLabelControl;
	CButton m_btn_Analysis;
	afx_msg void OnBnClickedBtnAnalysis();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	void  AddPopWindowNumm(int& nTrip);//弹窗加1
	void  SubPopWindowNum(int& nTrip);//弹窗减1
	CRITICAL_SECTION m_csAddSubChange;
};
