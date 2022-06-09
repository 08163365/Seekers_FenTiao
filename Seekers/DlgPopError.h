#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "resource.h"
// CDlgPopError �Ի���
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
	CDlgPopError(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPopError();

// �Ի�������
	enum { IDD = IDD_DLG_POPERROR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	void  AddPopWindowNumm(int& nTrip);//������1
	void  SubPopWindowNum(int& nTrip);//������1
	CRITICAL_SECTION m_csAddSubChange;
};
