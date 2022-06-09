#pragma once
#include "Detect.h"
#include "PageLineSet.h"
// CDlgCalibrate �Ի���

class CDlgCalibrate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibrate)

public:
	CDlgCalibrate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCalibrate();

// �Ի�������
	enum { IDD = IDD_DIALOG_CALIBRATE };
public:

	CSeparate* m_pOwner;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	//CPageLineSet*       m_DlgParamSet;
	CButtonExt m_BTN_Calibration;
	CButtonExt m_BTN_OpenImage;
	CButtonExt m_BTN_CACEL;
	CButtonExt m_BTN_SAVEPARAM;
	double m_EDT_dCalSpaceDist;
	int m_EDT_nCalThresh;
	double m_EDT_dCoefficient;
	void ConicComp (HTuple hv_Xdata, HTuple hv_Ydata, HTuple hv_nLen, HTuple *hv_na, 
		HTuple *hv_nb, HTuple *hv_nc, HTuple *hv_R2, HTuple *hv_Ycomp);
	void GetRParam (HTuple hv_Ydata, HTuple *hv_na, HTuple *hv_nb, HTuple *hv_nc, HTuple *hv_R2, 
		HTuple *hv_Ycomp);
public:
	CStaticExt m_STC_Image;
	CStaticExt m_STC_Title;
	CStaticExt m_STC_GrayValue;
	HObject      m_hoImage;
	
	CFont               m_BtnFont;//Button�ؼ���ʾ������
	CFont               m_GrayValueFont;//�Ҷ�������ʾ������
	HTuple              m_hvWindowID;//ͼƬ��ʾ����ID
	double              m_dDispImageStartRow;
	double              m_dDispImageStartCol;
	double              m_dDispImageEndRow;
	double              m_dDispImageEndCol;
	HTuple              m_hvCurrentPixelPose;
	CString             m_strCommonConfig;//��������ļ�·��
	virtual BOOL OnInitDialog();
	void SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
		COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG);
	void InitUI(); //��ʼ������ؼ�
	void InitList();//��ʼ���б�
	void FreshImage();//ˢ�´�����ʾ��ͼƬ
	void WriteParam();//д�ֱ��ʵ��ļ�
	void FreshParam();

	CReportCtrlExt m_LC_ColResolution;
	HTuple              m_hvColPose;        //��������
	HTuple              m_hvXPose;          //���������
	int                 m_nPoseCount;       //������Ŀ
	HTuple              m_hvRes;///�ֱ���

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedBtnOpenimage();
	afx_msg void OnBnClickedBtnCalib();

	void SetButtonStaus(BOOL bStaus);
	afx_msg void OnPaint();
	double m_EDT_dCCDValue;
	double m_EDT_dManualValue;
	afx_msg void OnBnClickedBtnRefreshkvalue();
	HObject m_hoCross;
};
