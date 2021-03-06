#pragma once

#include "XmvImageWnd.h"
// CDlgHelp 对话框

class CDlgHelp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHelp();

// 对话框数据
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CXmvImageWnd m_Wnd_Image;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
