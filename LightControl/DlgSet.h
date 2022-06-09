#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SerialPortExt.h"
#include <vector>
#include "LightSerial.h"
#include "Resource.h"
#include "DH_HalconEx.h"
// CDlgSet 对话框
class CDlgSet : public CDialogEx //CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSet();
	void InitUI();
    // 对话框数据
	enum { IDD = IDD_DIALOG_SET };

	CLightSerial *m_pOwner;//类指针
	BOOL m_bEnable;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	DECLARE_MESSAGE_MAP()
public:
	//控件关联变量
	CComboBox m_SerialPortList;
	CComboBox m_ChannelList;
	CButton m_Open;
	CButton m_Close;

	//获取当前选中变量
	CString CurPort;//当前选中串口
    CString CurChannel;//当前选中通道
	CString CurLightType;//当前光源型号选择
	afx_msg void OnCbnSelchangeComboSerialList();
	//串口对象
	CString m_PortRecored;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	std::vector<CString> m_PortList; //串口列表	
	afx_msg void OnCbnSelchangeComboChaanelList();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CComboBox m_LightType;//光源型号
	afx_msg void OnCbnSelchangeComboLightType();
	afx_msg void OnBnClickedButton2();
	int m_Light_Value_Edit;
	CComboBox m_Combo_CommType;//通信类型
	afx_msg void OnCbnSelchangeComboComtype();
	CEdit m_Edit_Port;
	CEdit m_Edit_Ip;
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonWrite();
	CString m_EDIT_strIp;
	int m_EDIT_nPort;
	afx_msg void OnBnClickedCancel();
	BOOL m_CHK_bUseLightAdjust;
	int m_EDT_nGoalGray;
	int m_EDT_nGrayDev;
	int m_EDT_nSampleFrames;
	int m_EDT_nLightUp;
	int m_EDT_nLightDown;
};
