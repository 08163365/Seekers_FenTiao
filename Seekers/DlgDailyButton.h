#pragma once


// CDlgDailyButton �Ի���

class CDlgDailyButton : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDailyButton)

public:
	CDlgDailyButton(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDailyButton();

// �Ի�������
	enum { IDD = IDD_DLG_OPERATORTOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnXiangjichuchen();
	afx_msg void OnBnClickedBtnBaojingfuwei();
	afx_msg void OnBnClickedBtnBianmagunyaxia();
	afx_msg void OnBnClickedBtnBianmaguntaiqi();
	afx_msg void OnBnClickedBtnSdlqgyx();
	afx_msg void OnBnClickedBtnSdlqgtq();
	afx_msg void OnBnClickedBtnXdlqgyx();
	afx_msg void OnBnClickedBtnXdlqgtq();
	double m_EDT_dLabelLoaction;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
