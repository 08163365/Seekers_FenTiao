
#include "stdafx.h"
#include <vector>
#pragma once
// CXmvImageWnd
#define IDR_ZOOM_P      100//ָʾ
#define IDR_ZOOM_IN     101//�Ŵ�
#define IDR_ZOOM_OUT    102//��С
#define IDR_ZOOM_MOVE   103//�϶�
#define IDR_ZOOM_CENTER 104//����
#define IDR_ZOOM_REVERT 105//��ԭ
class CXmvImageWnd : public CStatic
{
	DECLARE_DYNAMIC(CXmvImageWnd)

public:
	CXmvImageWnd();
	virtual ~CXmvImageWnd();
public:
	void DispImage(const HObject& hoImage,BOOL bAdaptive = TRUE/*�����True�Ļ� ֱ������Ӧ��ʾ*/);
	void DispRegion(const HObject& hoRegion,const CString& strColor,const CString& strMode,const int nLineWidth = 1,BOOL bClear = TRUE);
	HTuple GetDispWnd();
	HObject GetDispImage();
	void SetDrawing(BOOL bDrawing = FALSE);
	BOOL GetDrawing();
	void DispRegion();
	void ClearImage();
	void ClearRegion();
private:
	
	void SetPart(const HTuple& Row1, const HTuple& Column1, const HTuple& Row2, const HTuple& Column2);
	HTuple m_hDispWnd,m_hvRegionColor,m_hvRegionMode,m_hvLineWidth;
	double m_dDispImagePartRow0;
	double m_dDispImagePartCol0;
	double m_dDispImagePartRow1;
	double m_dDispImagePartCol1;
	HObject m_hoCurrentDispImage;
	std::vector<HObject> m_vec_hoCurrentDispRegion;
	int    m_nCurrentMode;//��ǰģʽ���������ù����״
	BOOL   m_bLeftButtonDown;
	CPoint m_ptBtnDown;;
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL   m_bDrawing;
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
private:
	CMenu m_Menu;
	afx_msg void OnMenuZoomIn();
	afx_msg void OnMenuZoomOut();
	afx_msg void OnMenuZoomP();
	afx_msg void OnMenuZoomMove();
	afx_msg void OnMenuZoomCenter();
	afx_msg void OnMenuZoomRevert();
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


