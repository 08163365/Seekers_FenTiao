#pragma once

#include "XmvImageWnd.h"
// CDlgHelp �Ի���

class CDlgHelp : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHelp();

// �Ի�������
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CXmvImageWnd m_Wnd_Image;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
