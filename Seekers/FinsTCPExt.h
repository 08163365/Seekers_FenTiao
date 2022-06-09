#pragma once
//#include "stdafx.h"
//#include "HalconCpp.h"
//using namespace HalconCpp;
class CriticalSectionHelper
{
public:
	CriticalSectionHelper(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}

	~CriticalSectionHelper()
	{
		LeaveCriticalSection(m_pcs);
		m_pcs = NULL;
	}

private:
	CRITICAL_SECTION *m_pcs;
};
enum CommType
{
	CT_SERVER=0,
	CT_CLIENT,
};

class CFinsTcpExt
{
public:
	CFinsTcpExt();
	~CFinsTcpExt();

public:
	BOOL m_bConnected;//����״̬
	//����
	BOOL Connect(const CString& strIP,const int& nPort,CommType emType);

	//�Ͽ�����
	BOOL Disconnect();

	//����
	BOOL ShakeHand();

	//��int������
	BOOL ReadInt32(const int& nDMAddr,int& nReadVal);

	//дint������
	BOOL WriteInt32(const int& nDMAddr,const int& nWriteVal);

	//������ȡint������
	BOOL ReadInt32Batch(const int& nDMAddrStart,const int& nReadNum,std::vector<int>& vecReadVal);

	//����д��int������
	BOOL WriteInt32Batch(const int& nDMAddrStart,const std::vector<int>& vecWriteVal);

	//������ȡstring������
	BOOL ReadStringBatch(const int& nDMAddrStart,const int& nReadNum,CString& strReadVal);

	//����д��string������
	BOOL WriteStringBatch(const int& nDMAddrStart,const CString& strWriteVal);
private:
	//������ȡDM��������
	BOOL ReadDMBatch(const int& nDMAddrStart,const int& nLength,HTuple& hvRead);

	//����д��DM��������
	BOOL WriteDMBatch(const int& nDMAddrStart,const HTuple& hvWrite);

	///16λת����
	void  Read_hex_to_float(const float& val, HTuple hvdata);

private:
	CRITICAL_SECTION m_cs;//�ٽ�������ֹ�̳߳�ͻ
	CommType m_emType;
	HTuple m_hv_AcceptingSocket;

	
	
	//PLCĿ���ַ�������������ж�ȡ
	HTuple m_hv_PLCAddr;
	//PCԴ��ַ�������������ж�ȡ
	HTuple m_hv_PCAddr;
public:
	HTuple m_hv_Socket;

};