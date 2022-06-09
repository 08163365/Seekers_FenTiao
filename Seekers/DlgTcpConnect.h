#pragma once
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"
#include "c:\program files (x86)\microsoft visual studio 10.0\vc\atlmfc\include\afxwin.h"


// CDlgTcpConnect 对话框

class CDlgTcpConnect : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTcpConnect)

public:
	CDlgTcpConnect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTcpConnect();

// 对话框数据
	enum { IDD = IDD_DIALOG_TCPCONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	CStaticExt m_STC_CTitle;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButtonExt m_BTN_Save;
	CButtonExt m_BTN_Cancel;
	CButtonExt m_BTN_Return;
	afx_msg void OnBnClickedBtnReturn();
	CString m_EDT_StrComIP;
	int m_EDT_nComPort;
	CString m_COM_StrGongWeiName;
	int m_EDT_nStopNum;

};
