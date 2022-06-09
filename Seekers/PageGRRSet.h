#pragma once
#include "resource.h"
// CPageGRRSet 对话框
class CDlgCreateBaseLine;
class CPageGRRSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPageGRRSet)

public:
	CPageGRRSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageGRRSet();

// 对话框数据
	enum { IDD = IDD_DLG_PAGEGRR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void FlushSpotParam(int& nIndex);
	virtual void PostNcDestroy();
	void ResetSize();
	int m_EDT_nGRRSampleNum;
	BOOL m_CHK_OneKeyGRRData;
	afx_msg void OnBnClickedBtnSetgrrparam();
	CDlgCreateBaseLine* m_pParentDlg;
	afx_msg void OnPaint();
	int m_COM_nCameraIndex;
	int m_EDT_nFLPLeftColumn;
	int m_EDT_nFLPLimistSize;
	int m_EDT_nFLPRightColumn;
	int m_EDT_nYPGroundHighThresh;
	int m_EDT_nYPGroundLowThresh;
	int m_EDT_nYPLeftColumn;
	int m_EDT_nYPRightColumn;
	CString m_EDT_StrFilePath;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnOpenimage();
	afx_msg void OnSelchangeComCameraindex();
	afx_msg void OnBnClickedBtnSaveparam2();
	afx_msg void OnBnClickedBtnTest();

	HTuple m_hvYPArea[2];
	afx_msg void OnBnClickedBtnAutotest();
};
