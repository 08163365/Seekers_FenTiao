#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "SerialPortExt.h"
#include <vector>
#include "LightSerial.h"
#include "Resource.h"
#include "DH_HalconEx.h"
// CDlgSet �Ի���
class CDlgSet : public CDialogEx //CDialogEx
{
	DECLARE_DYNAMIC(CDlgSet)

public:
	CDlgSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSet();
	void InitUI();
    // �Ի�������
	enum { IDD = IDD_DIALOG_SET };

	CLightSerial *m_pOwner;//��ָ��
	BOOL m_bEnable;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	DECLARE_MESSAGE_MAP()
public:
	//�ؼ���������
	CComboBox m_SerialPortList;
	CComboBox m_ChannelList;
	CButton m_Open;
	CButton m_Close;

	//��ȡ��ǰѡ�б���
	CString CurPort;//��ǰѡ�д���
    CString CurChannel;//��ǰѡ��ͨ��
	CString CurLightType;//��ǰ��Դ�ͺ�ѡ��
	afx_msg void OnCbnSelchangeComboSerialList();
	//���ڶ���
	CString m_PortRecored;
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	std::vector<CString> m_PortList; //�����б�	
	afx_msg void OnCbnSelchangeComboChaanelList();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	CComboBox m_LightType;//��Դ�ͺ�
	afx_msg void OnCbnSelchangeComboLightType();
	afx_msg void OnBnClickedButton2();
	int m_Light_Value_Edit;
	CComboBox m_Combo_CommType;//ͨ������
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
