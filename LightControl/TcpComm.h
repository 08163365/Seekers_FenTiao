////////////////////////////////
////////////����ͨ����
////////////////////////////////
#pragma once
#include "StdAfx.h"
#include <vector>
#include "HalconCpp.h"
using namespace HalconCpp;
class CTcpComm
{
public:
#define READ_TIMEOUT  101
public:
	CTcpComm();
	~CTcpComm();
public :
	//������,����IP���˿ں�
	bool Open(CString IP, int Port);
	//�ر�����
	bool Close();
	//�����豸
	bool Connect(CString IP, int Port);
	//��ȡ��Դ���ȣ���Դ��������
	bool GetLightBrightness(const int &Channel,int &nBrightness);
	//���ù�Դ���ȣ���Դ�������������ȷ���0~255
	bool SetLightBrightness(const int &nChannel, int nBrightness);
	//��ȡ����
	bool CTcpComm::Read(CString  &strReceiveData);
	//д������
	bool CTcpComm::Write(const CString &strSendData);

	bool IsOpen() {return m_bOpen;}
	//���ù�Դ����״̬����Դ����������������
	bool CTcpComm::SetLightStatus(bool bOn);
private:
	OVERLAPPED m_tReadovReady, m_tWriteovReady;
	HANDLE m_tcpComm;
	bool m_bOpen;

	HTuple hv_Protocol;
	HTuple hv_Timeout;
	HTuple hv_Socket ;
	HTuple hv_Address;
	HTuple hv_To;
	HTuple hv_Answer;
	HTuple hv_From;
	HTuple hv_Data;
	};

