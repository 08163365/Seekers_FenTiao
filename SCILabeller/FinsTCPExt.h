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
#define RW_Float 2
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

	//��Float32������
	BOOL WriteFloat32(const int& nDMAddr,const float& fWriteVal);
	BOOL ReadFloat32(const int& nDMAddr, float& fReadVal);
	BOOL ReadRegister(const int& nDMAddr, const int& nLenght, std::vector<unsigned char>& vecReadChar);
	BOOL Vec_CharToFloat(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, float& fValue);
	BOOL Vec_CharToInt(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, int& iValue);
	//��int������
	BOOL ReadInt32(const int& nDMAddr,int& nReadVal,CString strArea="DM");
	BOOL ReadInt16(const int& nDMAddr,int& nReadVal);
	
	//дint������
	BOOL WriteInt32(const int& nDMAddr,const int& nWriteVal,CString strArea="DM");
	BOOL WriteInt16(const int& nDMAddr,const int& nWriteVal);
	BOOL WriteRegister(const int& nDMAddr, const std::vector<unsigned char>& vecWriteChar);
	BOOL Vec_CharToShort(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, int& sValue);
	//������ȡint������
	BOOL ReadInt32Batch(const int& nDMAddrStart,const int& nReadNum,std::vector<int>& vecReadVal);
	BOOL ReadFloat32Batch(const int& nDMAddrStart, const int& nReadNum, std::vector<float>& vecReadVal, const int & nSpace = RW_Float);
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
	std::vector<unsigned char> m_vecRead;
	std::vector<unsigned char> m_vecWrite;
	unsigned  char m_cVal4[4];
	CString m_cstrLastError;
};