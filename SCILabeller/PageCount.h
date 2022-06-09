#pragma once
#include "resource.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxcmn.h"

// CPageCount 对话框

class CPageCount : public CDialogEx
{
	DECLARE_DYNAMIC(CPageCount)

public:
	CPageCount(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageCount();

// 对话框数据
	enum { IDD = IDD_DLG_PAGECOUNT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	void ResetSize();
	void UpdateList(HTuple& hv_Name,HTuple& hv_Info);
	CReportCtrlExt m_LST_LabelCountList;
	CFont m_LST_Font;

};
