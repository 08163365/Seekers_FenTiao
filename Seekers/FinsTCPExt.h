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
	BOOL m_bConnected;//连接状态
	//连接
	BOOL Connect(const CString& strIP,const int& nPort,CommType emType);

	//断开连接
	BOOL Disconnect();

	//握手
	BOOL ShakeHand();

	//读int型数据
	BOOL ReadInt32(const int& nDMAddr,int& nReadVal);

	//写int型数据
	BOOL WriteInt32(const int& nDMAddr,const int& nWriteVal);

	//批量读取int型数据
	BOOL ReadInt32Batch(const int& nDMAddrStart,const int& nReadNum,std::vector<int>& vecReadVal);

	//批量写入int型数据
	BOOL WriteInt32Batch(const int& nDMAddrStart,const std::vector<int>& vecWriteVal);

	//批量读取string型数据
	BOOL ReadStringBatch(const int& nDMAddrStart,const int& nReadNum,CString& strReadVal);

	//批量写入string型数据
	BOOL WriteStringBatch(const int& nDMAddrStart,const CString& strWriteVal);
private:
	//批量读取DM区域数据
	BOOL ReadDMBatch(const int& nDMAddrStart,const int& nLength,HTuple& hvRead);

	//批量写入DM区域数据
	BOOL WriteDMBatch(const int& nDMAddrStart,const HTuple& hvWrite);

	///16位转浮点
	void  Read_hex_to_float(const float& val, HTuple hvdata);

private:
	CRITICAL_SECTION m_cs;//临界区，防止线程冲突
	CommType m_emType;
	HTuple m_hv_AcceptingSocket;

	
	
	//PLC目标地址，从握手数据中读取
	HTuple m_hv_PLCAddr;
	//PC源地址，从握手数据中读取
	HTuple m_hv_PCAddr;
public:
	HTuple m_hv_Socket;

};