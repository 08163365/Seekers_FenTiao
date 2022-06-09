#pragma once

const CString g_strCameraName[]=
{
	_T("cam_a"),
	_T("cam_a"),
	_T("cam_b"),
	_T("cam_c"),
	_T("cam_d"),
};
///ʱ��	��ƬID	��Ź���	���	�ٶ�	�Ƴ�	�߶�	����1	����2	����3	����4	����5	����6	����7	����8

struct InkjetInfo//��ī����Ϣ
{
	HTuple hv_EAID;
	HTuple hv_Trip;
	HTuple hv_Speed;
	HTuple hv_Yield;
	HTuple hv_NGImageSN;//д�������ͼ���
	HTuple hv_FirstStrName;//��һ���ȼ�������
	HTuple hv_ErrorName;//��������
	HTuple hv_ErrorNum;//������Ŀ
	HTuple hv_TripWidth;//�������
	HTuple hv_TotalLength;//�ܳ�
	HTuple hv_LTRow;
	HTuple hv_LTColumn;
	HTuple hv_RBRow;
	HTuple hv_RBColumn;

};
////������Ϊ���ϴ���Ϣ������������õ� 
class CInkjetProcess
{
public:
	CInkjetProcess(void);
	~CInkjetProcess(void);

public:
	void Init(int nSide);//��ʼ��
	void Action(C_ALLOCATORWORKSHOPOUTPUT& OutPut);//ִ�л�ȡ������Ϣ
	void AppendQueue(InkjetInfo& tInjectInfo);//��������Ϣ���
	void PopQueue(int nSide);//������Ϣ����
	void WriteInjeckInfo(InkjetInfo& tInjectInfo,CString& strValue,int& nNumCount);//��¼��־
	void GetSendInfo(InkjetInfo& tInkjetInfo,int nGongWeiNum,CString& StrSendValue);//��ȡ���͸���������ַ���
	void SendToInjectInfo(CString& StrQuexinValue,CString& StrPenMaValue,int nSN);//���ַ������͸�������
	int  GetSN(int nTrip);//��ȡ��λ��

	void  ReadRealOutPortCount();//��ȡ����ڼ���
	void  WriteOutIOInfo(CString& StrEAID,CString& strValue);//��¼IO�����Ϣ
public:
	int  GetOutCount();//��ȡ��ǰOUT����
	int  GetOutChaneNum();//��ȡOUT�仯����Ŀ
	int  ReadRealOutImageSN();//��ȡ���ӵ�ͼ���
	BOOL IsFreshImageSN();//�ж�ͼ����Ƿ���³ɹ�
	int  GetVecSize();//��ȡ���г���
	void  WriteOutIOChangeOnly(CString& strValue);//����Ͽ��仯
private:
	int m_nOutCount;//�������
	int m_nLastOutCout;
	int m_nImageSN;
	int m_nLastImageSN;
	int m_nVecSize;//

public:
	int m_nSide;//����
	CRITICAL_SECTION m_csChange;//������
	std::vector<InkjetInfo>    m_vectorResultData;//���ݻ������

};

