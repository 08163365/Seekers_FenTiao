#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"

#include "Labeller.h"
// CPageControlParam 对话框

class CPageControlParam : public CDialogEx
{
	DECLARE_DYNAMIC(CPageControlParam)

public:
	CPageControlParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageControlParam();

// 对话框数据
	enum { IDD = IDD_DLG_PAGECONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CLabeller* m_pChildOwner;
	void ResetSize();
	void UpdateList(HTuple& hv_Name,HTuple& hv_Info);
	CReportCtrlExt m_LST_ControlList;
	CFont m_LST_Font;
	afx_msg void OnBnClickedBtnEnableconnect();
	afx_msg void OnBnClickedBtnEnablemannual();
	afx_msg void OnBnClickedBtnEnablesumwarn();
	afx_msg void OnBnClickedBtnEnablefourlabel();
	afx_msg void OnBnClickedBtnEnableconlabelwarn();
	afx_msg void OnBnClickedBtnEnablelabel();
	afx_msg void OnBnClickedBtnEnablemarkuse();
	afx_msg void OnBnClickedBtnEnableduandai();
};
