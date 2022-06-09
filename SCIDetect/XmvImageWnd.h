
#include "stdafx.h"
#include <vector>
#pragma once
// CXmvImageWnd
#define IDR_ZOOM_P      100//指示
#define IDR_ZOOM_IN     101//放大
#define IDR_ZOOM_OUT    102//缩小
#define IDR_ZOOM_MOVE   103//拖动
#define IDR_ZOOM_CENTER 104//居中
#define IDR_ZOOM_REVERT 105//还原
class CXmvImageWnd : public CStatic
{
	DECLARE_DYNAMIC(CXmvImageWnd)

public:
	CXmvImageWnd();
	virtual ~CXmvImageWnd();
public:
	void DispImage(const HObject& hoImage,BOOL bAdaptive = TRUE/*如果是True的话 直接自适应显示*/);
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
	int    m_nCurrentMode;//当前模式，用来设置光标形状
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


