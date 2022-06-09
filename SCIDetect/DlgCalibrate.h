#pragma once
#include "Detect.h"
#include "PageLineSet.h"
// CDlgCalibrate 对话框

class CDlgCalibrate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCalibrate)

public:
	CDlgCalibrate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCalibrate();

// 对话框数据
	enum { IDD = IDD_DIALOG_CALIBRATE };
public:

	CSeparate* m_pOwner;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
	
	CFont               m_BtnFont;//Button控件显示的字体
	CFont               m_GrayValueFont;//灰度坐标显示的字体
	HTuple              m_hvWindowID;//图片显示窗口ID
	double              m_dDispImageStartRow;
	double              m_dDispImageStartCol;
	double              m_dDispImageEndRow;
	double              m_dDispImageEndCol;
	HTuple              m_hvCurrentPixelPose;
	CString             m_strCommonConfig;//相机配置文件路径
	virtual BOOL OnInitDialog();
	void SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
		COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG);
	void InitUI(); //初始化界面控件
	void InitList();//初始化列表
	void FreshImage();//刷新窗口显示的图片
	void WriteParam();//写分辨率到文件
	void FreshParam();

	CReportCtrlExt m_LC_ColResolution;
	HTuple              m_hvColPose;        //像素坐标
	HTuple              m_hvXPose;          //物理坐标点
	int                 m_nPoseCount;       //坐标数目
	HTuple              m_hvRes;///分辨率

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
