#pragma once

// CPageSpotDetectSet 对话框
class CDlgCreateBaseLine;
class CPageSpotDetectSet : public CDialogEx
{
	DECLARE_DYNAMIC(CPageSpotDetectSet)

public:
	CPageSpotDetectSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPageSpotDetectSet();

// 对话框数据
	enum { IDD = IDD_DLG_PAGESPOTDETECT };
	CDlgCreateBaseLine* m_pParentDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	public:
	virtual void PostNcDestroy();
	void ResetSize();
	
	BOOL m_CHK_bEnabeSpotCheck;
	int m_COM_nCameraIndex;
	int m_EDT_nSpotLowThresh;
	int m_EDT_nSpotHighThresh;
	int m_EDT_nSpotNum;
	int m_EDT_nLeftColumn;
	int m_EDT_nRightColumn;
	double m_EDT_dSpotStandardArea;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComCamreaindex();
	void        FlushSpotParam(int& nIndex);
	afx_msg void OnBnClickedBtnSpottest();
	afx_msg void OnBnClickedBtnOpentestimage();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnPaint();
	BOOL m_CHK_bEnableWriteRow;
	double m_EDT_dMinCircleParam;
	double m_EDT_dFLPHeight;
	double m_EDT_dFLPHeightDev;
	int m_EDT_nFLPWarnNGFrame;
	double m_EDT_dFLPWidth;
	double m_EDT_dFLPWidthDev;
	CString m_EDT_strSpotTime1;
	CString m_EDT_strSpotTime2;
	afx_msg void OnBnClickedBtnSpotcalibration();
};
