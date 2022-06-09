#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
// CDlgMap �Ի���

class CDlgMap : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMap)

public:
	CDlgMap(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMap();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAP };

	CBlendent m_Blendent;
protected:
	
	void InitStatic();
	void InitWindowShow();//��ʼ����ʾ���
	void InitGraduation();//��ʾ�̶ȱ�
	void InitTree();
	
	void DisplayColumnGraduation();//��ʾ����̶ȱ�
	void Display(int nRowIndex);//��ʾͼƬ
	void DisplayObj();
	void DisplayRowGraduation(int nRowIndex);//��ʾ����̶ȱ�

	void GetFullPath(HTREEITEM hItem);

public:
	HTuple  m_hvSurFaceIndex;//ѡ�е�����һ��
	DHInterface* m_pDetect[4];//���������
	CString m_strProductName,m_strBatch;
private:
	HTuple m_hvMapWindow;
	HTuple m_hvColumnGraduationWindow;//����̶�
	HTuple m_hvRowGraduationWindow;//����̶�
	HTuple m_hvSmallWindow;
	CString m_strSelMapPath;//��ǰѡ�еĵ�ͼ·��
	CString m_strLoadMapPath;//��ǰ���صĵ�ͼ·��
	HTuple  m_hvFilesSorted;
	HObject m_hoCurrentObject;
	HTuple  m_hvCurrentInfo;
	
	CImageList m_TreeMapImages;
	int m_nImageWidth;
	int m_nImageHeight;
	CFont m_ftButton;
	double m_dwColumnResolution[2];//�ֱ���
	double m_dwRowResolution[2];//�ֱ���
	CStringArray m_strAErrorName;
protected:
	void Detect();


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStaticExt m_cstaticTitle;
	CStaticExt m_staticDefectInfo;
	CTreeCtrl m_TreeMap;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnNMRClickTreeMap(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuLoadmap();
	afx_msg void OnMenuDeletemap();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_ScrollBarMap;
	CStaticExt m_staticMapInfo;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CButtonExt m_btnCancel;
	CButtonExt m_btnDetectFront;
	afx_msg void OnBnClickedBtnCancel();
	
	afx_msg void OnBnClickedBtnOpenfile();
	CButtonExt m_btnFileImageOpen;
	afx_msg void OnBnClickedBtnDetectFront();
	afx_msg void OnBnClickedBtnDetectBehind();
	CButtonExt m_btnDetectBehind;
	CButtonExt m_btnSaveFile;
	afx_msg void OnBnClickedBtnSavefile();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
   int m_nLanCount;
   BOOL m_bFront,m_bBehind;
};
