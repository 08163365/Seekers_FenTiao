//////////////////////////////////////////////////////////////////////////
/////blendent.h  用于设置界面和控件的颜色               /////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"

typedef struct tagTBlendentCButtonST
{
	short shOffset;//offset
	COLORREF BKColorIn;//BTNST_COLOR_BK_IN
	COLORREF BKColorOut;//BTNST_COLOR_BK_IN
	COLORREF BKColorFocus;//BTNST_COLOR_BK_FOCUS
	COLORREF FGColorIn;//BTNST_COLOR_FG_IN
	COLORREF FGColorOut;//BTNST_COLOR_FG_OUT
	COLORREF FGColorFocus;//BTNST_COLOR_FG_FOCUS
}BlendentCButtonST,*PBlendentCButtonST;

typedef struct tagTBlendentCxStatic
{
	COLORREF TextColor;//字体颜色
	COLORREF BKColor;//背景色1
	COLORREF BKHigh;//背景色2（背景色1是主色），用于渐变
}BlendentCxStatic,*PBlendentCxStatic;

typedef struct tagTBlendentCReprotList
{
	COLORREF HeaderBKColor;//表头背景颜色
	int nGradient;//渐变系数
	int nR,nG,nB;//表头背景颜色，三个分量
	COLORREF HeaderTextColor;//表头字体颜色
	COLORREF BKColor;//表身背景颜色
}BlendentCReprotList,*PBlendentCReprotList;

typedef struct tagTBlendentCEditEx
{
	COLORREF BKColor;//编辑框背景色
	COLORREF BKColorFocus;//编辑框聚焦背景色
	COLORREF Normal;//默认状态边框颜色
	COLORREF Hover;//鼠标浮动状态边框颜色
	COLORREF Focus;//聚焦状态边框颜色
}BlendentCEditEx,*PBlendentCEditEx;

typedef struct tagTBlendentDlg
{
	COLORREF MainColor;//主配色,标题栏、按钮的颜色
	int nR,nG,nB;//主配色的三个分量;
	COLORREF BKColor;//背景配色，用于界面背景

}BlendentDlg,*PBlendentDlg;

typedef struct tagTBlendent
{
	tagTBlendentDlg Dlg;//用于界面，含主配色
	tagTBlendentCButtonST Button;//用于按钮
	tagTBlendentCReprotList List;//用于列表
	tagTBlendentCxStatic Static;//用于静态文本
	tagTBlendentCEditEx Edit;//用于编辑框
}Blendent,*PBlendent;

class CBlendent
{
public:
	CBlendent(void);
	~CBlendent(void);

	void Init();//用于初始化所有字体和颜色变量
	//void SettingDlg();
public:
	tagTBlendent m_Blendent;//配色
private:
	CString m_strBlendentFile;//配置文件路径

private:
	CString GetCurrentAppPath();
	void GetRGB(const CString &strRGB,int &nR,int &nG,int &nB);
};