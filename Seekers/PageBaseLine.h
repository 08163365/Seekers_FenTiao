#pragma once
#include "resource.h"

// CPageBaseLine 对话框
class CDlgCreateBaseLine;
class CPageBaseLine : public CDialogEx
{
	DECLARE_DYNAMIC(CPageBaseLine)

public:
	CPageBaseLine(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageBaseLine();

// 对话框数据
	enum { IDD = IDD_DLG_PAGEBASEMODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void PostNcDestroy();
	void ResetSize(CRect& SourceRect);
	CDlgCreateBaseLine* m_pParentDlg;
	virtual BOOL OnInitDialog();
	int m_EDT_nFirstCoatLowThresh;
	int m_EDT_nFirstCoatHighThresh;
	int m_EDT_nFirstFoilHighThresh;
	int m_EDT_nFirstFoilLowThresh;
	int m_EDT_nSecondCoatLowThresh;
	int m_EDT_nSecondCoatHighThresh;
	int m_EDT_nSecondFoilLowThresh;
	int m_EDT_nSecondFoilHighThresh;
	afx_msg void OnBnClickedBtnDrawfirstmodel();
	afx_msg void OnBnClickedBtnDrawsecondmodel();
	afx_msg void OnBnClickedBtnOnekey();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnPaint();
};
