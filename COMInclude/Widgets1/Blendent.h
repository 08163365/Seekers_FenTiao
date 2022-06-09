//////////////////////////////////////////////////////////////////////////
/////blendent.h  �������ý���Ϳؼ�����ɫ               /////////////
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
	COLORREF TextColor;//������ɫ
	COLORREF BKColor;//����ɫ1
	COLORREF BKHigh;//����ɫ2������ɫ1����ɫ�������ڽ���
}BlendentCxStatic,*PBlendentCxStatic;

typedef struct tagTBlendentCReprotList
{
	COLORREF HeaderBKColor;//��ͷ������ɫ
	int nGradient;//����ϵ��
	int nR,nG,nB;//��ͷ������ɫ����������
	COLORREF HeaderTextColor;//��ͷ������ɫ
	COLORREF BKColor;//��������ɫ
}BlendentCReprotList,*PBlendentCReprotList;

typedef struct tagTBlendentCEditEx
{
	COLORREF BKColor;//�༭�򱳾�ɫ
	COLORREF BKColorFocus;//�༭��۽�����ɫ
	COLORREF Normal;//Ĭ��״̬�߿���ɫ
	COLORREF Hover;//��긡��״̬�߿���ɫ
	COLORREF Focus;//�۽�״̬�߿���ɫ
}BlendentCEditEx,*PBlendentCEditEx;

typedef struct tagTBlendentDlg
{
	COLORREF MainColor;//����ɫ,����������ť����ɫ
	int nR,nG,nB;//����ɫ����������;
	COLORREF BKColor;//������ɫ�����ڽ��汳��

}BlendentDlg,*PBlendentDlg;

typedef struct tagTBlendent
{
	tagTBlendentDlg Dlg;//���ڽ��棬������ɫ
	tagTBlendentCButtonST Button;//���ڰ�ť
	tagTBlendentCReprotList List;//�����б�
	tagTBlendentCxStatic Static;//���ھ�̬�ı�
	tagTBlendentCEditEx Edit;//���ڱ༭��
}Blendent,*PBlendent;

class CBlendent
{
public:
	CBlendent(void);
	~CBlendent(void);

	void Init();//���ڳ�ʼ�������������ɫ����
	//void SettingDlg();
public:
	tagTBlendent m_Blendent;//��ɫ
private:
	CString m_strBlendentFile;//�����ļ�·��

private:
	CString GetCurrentAppPath();
	void GetRGB(const CString &strRGB,int &nR,int &nG,int &nB);
};