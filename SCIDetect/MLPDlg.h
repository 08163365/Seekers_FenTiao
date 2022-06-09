
// MLP��ע����Dlg.h : ͷ�ļ�
//

#pragma once
#include "XmvImageWnd.h"


class CDlgAlgorithm;
enum EmLabelType
{
	Rectangle1,
	Circle,
	Multpoly,
};


class CMLPDlg : public CDialogEx
{
// ����
public:
	CMLPDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MLP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
// ʵ��
public:
	CDlgAlgorithm* m_pParentDlg;
protected:

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnBnClickedBtnOpenimage();
public:
	HObject m_hoCurrentImage;
	CComboBox m_COM_CClassName;
//	int m_COM_nLabelType;
	void InsertSurfaceError(const int i);
public:
	void ReadParam();
	void WriteParam();
public:
	HTuple m_hvDataDict;
	CString m_strDirectPath;
	afx_msg void OnBnClickedBtnLabel();
	HObject m_hoLabelRegion;
	afx_msg void OnBnClickedBtnAddtraindata();
	int m_COM_nClassType;
	int m_Count[ClassTotalNum];
	afx_msg void OnBnClickedBtnTrain();
public:
	void add_samples (HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName, 
		HTuple hv_RegionFeatureName, HTuple hv_Class);
	void get_features (HObject ho_Region, HObject ho_Image, HTuple hv_GrayFeaturesName, 
		HTuple hv_RegionFeaturesName, HTuple *hv_Features);
	void classify (HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName, 
		HTuple hv_RegionFeatureName, HTuple *hv_Classes, HTuple *hv_Confidence);
	void segment (HObject ho_Image, HObject *ho_Regions, HTuple hv_Type);
	void list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
		HTuple *hv_ImageFiles);
public:
	HTuple m_MLPHandle;
	HTuple   m_hvRegionFeature;
	HTuple   m_hvGrayFeature;
	HTuple	 m_hvFeatureName;
	HTuple   m_hvFeatureLength;
	afx_msg void OnBnClickedBtnVerify();
	afx_msg void OnBnClickedBtnSaveall();
//	int m_EDT_nDynThresh;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	MLPParam m_tDHMLPParam;
//	CReportCtrlExt m_LST_CErrorList;
	HObject m_hoErrorRegion;//ȱ������
	HTuple  m_hvErrorType;//ȱ������
	double  m_dRowResolution;//��ֱ�ֱ���
	double  m_dColumnResolution;//ˮƽ�ֱ���
//	afx_msg void OnItemchangedLstErrorlist(NMHDR *pNMHDR, LRESULT *pResult);
//	int m_EDT_nLeftColumn;
//	int m_EDT_nRightColumn;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
