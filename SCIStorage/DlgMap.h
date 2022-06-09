#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"
// CDlgMap 对话框

class CDlgMap : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMap)

public:
	CDlgMap(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMap();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAP };

	CBlendent m_Blendent;
protected:
	
	void InitStatic();
	void InitWindowShow();//初始化显示句柄
	void InitGraduation();//显示刻度表
	void InitTree();
	
	void DisplayColumnGraduation();//显示横向刻度表
	void Display(int nRowIndex);//显示图片
	void DisplayObj();
	void DisplayRowGraduation(int nRowIndex);//显示纵向刻度表

	void GetFullPath(HTREEITEM hItem);

public:
	HTuple  m_hvSurFaceIndex;//选中的是哪一面
	DHInterface* m_pDetect[4];//两个检测类
	CString m_strProductName,m_strBatch;
private:
	HTuple m_hvMapWindow;
	HTuple m_hvColumnGraduationWindow;//横向刻度
	HTuple m_hvRowGraduationWindow;//纵向刻度
	HTuple m_hvSmallWindow;
	CString m_strSelMapPath;//当前选中的地图路径
	CString m_strLoadMapPath;//当前加载的地图路径
	HTuple  m_hvFilesSorted;
	HObject m_hoCurrentObject;
	HTuple  m_hvCurrentInfo;
	
	CImageList m_TreeMapImages;
	int m_nImageWidth;
	int m_nImageHeight;
	CFont m_ftButton;
	double m_dwColumnResolution[2];//分辨率
	double m_dwRowResolution[2];//分辨率
	CStringArray m_strAErrorName;
protected:
	void Detect();


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
