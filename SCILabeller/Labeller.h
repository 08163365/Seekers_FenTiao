#pragma once
#include "Define.h"
#include "FinsTCPExt.h"
#include <queue>
#include "SCILabeller.in.h"

using namespace std;
extern BOOL g_bExitThread;
enum emLightColor
{
	GreenLight,
	YellowLight,
	RedLight,


};
enum LabelIndex
{
	InsideLabel=1,
	OutsideLabel,
};
void ThreadListenLabelLength(PVOID * pParam);//��������
class CLabeller : DHInterface
{
public:
	CLabeller(void);
	~CLabeller(void);
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam); // �����÷���Ҫ���õĳ�Ա���������ɴ��麯��
	int      Init(PLABELLERINIT pInit);//��ʼ��
	static   void* _Thread(void *hcthread);//io�߳�
	void     Loop();
	LABELLERINIT m_tInit;
	BOOL  IsUseLabel(int nLabelIndex);
     CRITICAL_SECTION m_csChange;
public:
	void  WriteExceptLog(HTuple& hvFlag,HTuple& hv_Names);
	void  GetLabelControl(HTuple& hv_Type,HTuple&hv_Data);//��ȡ���ز���
	void  GetLabelParam(HTuple& hv_Type,HTuple&hv_Data);//��ȡ����
	void SetLabellerStaus(CString& strInfo);
	void  LabelReset();//��λ
	void  ReadParam();//��ȡ����
	void  WriteParam();//�������
	void  ClearCount();//�����������
	void  GetLabelStatus(HTuple*hv_EachFalg,HTuple* hv_WarnMessage);
	void  GetLabelCount(HTuple* hv_Name,HTuple* hv_Count);//����
	int   GetLabelActionNum(int nLabelSN);//��ȡ������Ŀ
	int   GetLabelReieveNum(int nLabelSN);//��ȡ�յ��ź���Ŀ
	void  WriteLabelLog(CString strMessage,BOOL bExceptFlag=TRUE);//д��������־
	int   GetLabelNum(int nLabelSN,int nType);//��ȡ����


	void  XiangJiChuChen();//�������
	void  BaoJingFuWei();//������λ
	void  ShangQiGangTaiQi();//�ϴ�·����̧��
	void  ShangQiGangYaXia();//�ϴ�·����ѹ��
	void  XiaQiGangTaiQi();//��·����̧��
	void  XiaQiGangYaXia();//�´�·����ѹ��
	void  BianMaQiGangTaiQi();//��������������̧��
	void  BianMaQiGangYaXia();//������������ѹ��
	void  SetLightColor(emLightColor LightColor);//���ñ����Ƶ���ɫ





	CFinsTcpExt m_CFinsTcp;//ͨѶ��
	CString m_strPath;
	LabellerInfo m_tLabellerInfo;//���������
	CString m_StrLabellerStatus;
	BOOL m_bScreenFlag;
	HTuple m_hvStatusFlag,m_hvWarnNames;
	HTuple m_hvCount,m_hvCountNames;//����
	HTuple m_hvControl,m_hvControlName;//����
	HTuple m_hvSystemParam,m_hvSystemParamName;//����

	BOOL  m_bWarnStaus;
	double m_dRealSpeed;

	HTuple  m_hvLabelRealLocation;//��ʵ������λ��
	CString m_strDirectory;

	double m_dStandardLocation; 
	HTuple m_hvLabelLocationOffsetTuple;
	double m_dLabelLocationOffsetDev;

	CString m_strBatch;
};

