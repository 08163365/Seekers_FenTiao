#include "stdafx.h"
#include "FinsTCPExt.h"

CFinsTcpExt::CFinsTcpExt()
{
	InitializeCriticalSection(&m_cs);
	m_bConnected = FALSE;
	m_vecWrite.clear();
}

CFinsTcpExt::~CFinsTcpExt()
{

}

BOOL CFinsTcpExt::Connect(const CString& strIP,const int& nPort,CommType emType)
{
	m_emType = emType;
	if (emType == CT_SERVER)
	{
		try
		{
			OpenSocketAccept(nPort,HTuple("protocol").Append("timeout"),HTuple("TCP4").Append(3.0),&m_hv_AcceptingSocket);
			SocketAcceptConnect (m_hv_AcceptingSocket, "auto", &m_hv_Socket);


			return ShakeHand();
		}catch(HException& e)
		{
			m_hv_Socket.Clear();
			return FALSE;
		}
	}
	else
	{
		try
		{
			OpenSocketConnect(HTuple(strIP),nPort,HTuple("protocol").Append("timeout"),HTuple("TCP").Append(3),&m_hv_Socket);
			SetSocketParam(m_hv_Socket,"timeout",3);

			
			return ShakeHand();
		}catch(HException& e)
		{
			m_hv_Socket.Clear();
			return FALSE;
		}
	}
}

BOOL CFinsTcpExt::Disconnect()
{
	if (m_emType == CT_SERVER)
	{
		try
		{
			CloseSocket(m_hv_Socket);
			CloseSocket(m_hv_AcceptingSocket);
			m_hv_Socket.Clear();
			m_hv_AcceptingSocket.Clear();
			m_bConnected = FALSE;
			return TRUE;
		}catch(HException& e)
		{
			m_bConnected = FALSE;
			return FALSE;
		}
	}
	else
	{
		try
		{
			CloseSocket(m_hv_Socket);
			m_hv_Socket.Clear();
			m_bConnected = FALSE;
			return TRUE;
		}catch(HException& e)
		{
			m_bConnected = FALSE;
			return FALSE;
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
//ShakeHand 握手
//发送数据,如：
//46494e53	0000000c	00000000	00000000	00000000
//命令头	命令长度	命令码		错误码		节点
//返回数据,如： 
//46494e53	00000010	00000001	00000000	000000ef	00000063 (ef是PC机地址，63是PLC地址)
//命令头	命令长度	命令码		错误码		PC机地址	PLC地址
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ShakeHand()
{
	if (m_emType == CT_SERVER)
	{
		int nSends[24] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		HTuple hv_SendData(nSends,24);
		HTuple hv_RecvData,hv_From;
		try
		{
			ReceiveData(m_hv_Socket,"C20",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=20/* || hv_RecvData[15]!=0x00*/) return FALSE;
			//m_hv_PLCAddr = hv_RecvData[hv_RecvData.Length()-1];
			//m_hv_PCAddr = hv_RecvData[hv_RecvData.Length()-5];
			SendData(m_hv_Socket,"C24",hv_SendData,HTuple());
			m_bConnected = TRUE;
			return TRUE;
		}catch(HException& e)
		{
			m_bConnected = FALSE;
			return FALSE;
		}
	}
	else
	{
		int nSends[20] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		HTuple hv_SendData(nSends,20);
		HTuple hv_RecvData,hv_From;
		try
		{
			SendData(m_hv_Socket,"C20",hv_SendData,HTuple());
			ReceiveData(m_hv_Socket,"C24",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=24 || hv_RecvData[15]!=0x00) return FALSE;
			m_hv_PLCAddr = hv_RecvData[hv_RecvData.Length()-1];
			m_hv_PCAddr = hv_RecvData[hv_RecvData.Length()-5];
			m_bConnected = TRUE;
			return TRUE;
		}catch(HException& e)
		{
			m_bConnected = FALSE;
			return FALSE;
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////
//ReadInt32 读取int数据
//发送数据,如：
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0002
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	读命令		DM地址		读取长度	
//返回数据,如： 
//46494e53	0000001a	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000
//命令头		命令长度		命令码		错误码		UDP接收帧		PC和PLC地址	读命令		成功标识		数据低16位	数据高16位
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ReadInt32(const int& nDMAddr,int& nReadVal,CString strArea)
{
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	nReadVal = 0;

	HTuple hv_DMLower,hv_DMHigher;
	TupleBand(nDMAddr,0xFF,&hv_DMLower);
	TupleRsh(nDMAddr,8,&hv_DMHigher);
	
	

	if(strArea=="DM")
	{
		int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
			m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,0x00,0x02};
		HTuple hv_SendData(nSends,34);
		HTuple hv_RecvData,hv_From;
		try
		{
			SendData(m_hv_Socket,"C34",hv_SendData,HTuple());
			ReceiveData(m_hv_Socket,"C34",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=34) return FALSE;
			nReadVal = (hv_RecvData[hv_RecvData.Length()-2].I() << 24) 
				+ (hv_RecvData[hv_RecvData.Length()-1].I() << 16) 
				+ (hv_RecvData[hv_RecvData.Length()-4].I() << 8) 
				+ (hv_RecvData[hv_RecvData.Length()-3].I());
			return TRUE;
		}catch(HException& e)
		{
			m_bConnected=FALSE;
			//if (e.ErrorCode() == 5619)
			//{
			//	//超时重新握手
			//	ShakeHand();
			//}
			return FALSE;
		}
	}
	else
	{
		int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
			m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0xB2,hv_DMHigher.I(),hv_DMLower.I(),0x00,0x00,0x02};
		HTuple hv_SendData(nSends,34);
		HTuple hv_RecvData,hv_From;
		try
		{
			SendData(m_hv_Socket,"C34",hv_SendData,HTuple());
			ReceiveData(m_hv_Socket,"C34",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=34) return FALSE;
			nReadVal = (hv_RecvData[hv_RecvData.Length()-2].I() << 24) 
				+ (hv_RecvData[hv_RecvData.Length()-1].I() << 16) 
				+ (hv_RecvData[hv_RecvData.Length()-4].I() << 8) 
				+ (hv_RecvData[hv_RecvData.Length()-3].I());
			return TRUE;
		}catch(HException& e)
		{
			if (e.ErrorCode() == 5619)
			{
				//超时重新握手
				ShakeHand();
			}
			return FALSE;
		}
	}


	
}

//////////////////////////////////////////////////////////////////////////
//WriteInt32 写int数据
//发送数据,如：
//46494e53	0000001e	00000002	00000000	80000200	630000ef	00000102	8202ee00	0002		0016		0000
//命令头		命令长度		命令码		错误码		UDP发送帧		PLC和PC地址	写命令		DM地址		写入长度		数据低16位	数据高16位
//返回数据,如： 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//命令头		命令长度		命令码		错误码		UDP接收帧		PC和PLC地址	写命令		成功标识
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteInt32(const int& nDMAddr,const int& nWriteVal,CString strArea)
{
	if (!m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);

	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0) return FALSE;

	HTuple hv_DMLower,hv_DMHigher;
	TupleBand(nDMAddr,0xFF,&hv_DMLower);
	TupleRsh(nDMAddr,8,&hv_DMHigher);
	int nVal1 = nWriteVal & 0xff;
	int nVal2 = (nWriteVal >> 8) & 0xff;
	int nVal3 = (nWriteVal >> 16) & 0xff;
	int nVal4 = (nWriteVal >> 24) & 0xff;


	if(strArea=="DM")
	{
		int nSends[38] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
			m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,0x00,0x02,nVal2,nVal1,nVal4,nVal3};
		HTuple hv_SendData(nSends,38);
		HTuple hv_RecvData,hv_From;
		try
		{
			SendData(m_hv_Socket,"C38",hv_SendData,HTuple());
			ReceiveData(m_hv_Socket,"C30",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=30) return FALSE;
			if (hv_RecvData[hv_RecvData.Length()-1].I() != 0x40 
				&& hv_RecvData[hv_RecvData.Length()-1].I() != 0x00) 
				return FALSE;

			return TRUE;
		}catch(HException& e)
		{
			if (e.ErrorCode() == 5619)
			{
				//超时重新握手
				ShakeHand();
			}
			return FALSE;
		}
	}
	else
	{
		int nSends[38] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
			m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0xB2,hv_DMHigher.I(),hv_DMLower.I(),0x00,0x00,0x02,nVal2,nVal1,nVal4,nVal3};
		HTuple hv_SendData(nSends,38);
		HTuple hv_RecvData,hv_From;
		try
		{
			SendData(m_hv_Socket,"C38",hv_SendData,HTuple());
			ReceiveData(m_hv_Socket,"C30",&hv_RecvData,&hv_From);
			if (hv_RecvData.Length()!=30) return FALSE;
			if (hv_RecvData[hv_RecvData.Length()-1].I() != 0x40 
				&& hv_RecvData[hv_RecvData.Length()-1].I() != 0x00) 
				return FALSE;

			return TRUE;
		}catch(HException& e)
		{
			if (e.ErrorCode() == 5619)
			{
				//超时重新握手
				ShakeHand();
			}
			return FALSE;
		}
	}

}



//WriteInt32Batch 批量写入int数据
//发送数据,如：
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//命令头		命令长度		命令码		错误码		UDP发送帧		PLC和PC地址	写命令		DM地址		写入长度		数据低16位	数据高16位	数据低16位	数据高16位
//返回数据,如： 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//命令头		命令长度		命令码		错误码		UDP接收帧		PC和PLC地址	写命令		成功标识
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteRegister(const int& nDMAddr, const std::vector<unsigned char>& vecWriteChar)
{
	if (nDMAddr == 0)
	{
		m_cstrLastError.Format("写入地址为0");
		return FALSE;
	}
	if (vecWriteChar.size() == 0 || vecWriteChar.size() % 2 != 0)
	{
		m_cstrLastError.Format("写入数据为0,或为奇数");
		return FALSE;
	}
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0)
	{
		m_cstrLastError.Format("本机地址或目标地址异常");
		return FALSE;
	}

	int nSends[34] = { 0x46,0x49,0x4e,0x53,0x00,0x00,((26 + vecWriteChar.size()) >> 8) & 0xff,(26 + vecWriteChar.size()) & 0xff,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0x82,(nDMAddr >> 8) & 0xff,nDMAddr & 0xff,0x00,((vecWriteChar.size() / 2) >> 8) & 0xff ,(vecWriteChar.size() / 2) & 0xff };
	HTuple hv_SendData(nSends, 34);
	for (std::vector<unsigned char>::const_iterator cItr = vecWriteChar.cbegin();cItr!= vecWriteChar.cend();cItr++)
	{
		hv_SendData.Append(*cItr);
	}

	if (hv_SendData.Length() != 34 + vecWriteChar.size())
	{
		m_cstrLastError.Format("数据包长度不符");
		return FALSE;
	}
	HTuple hv_RecvData, hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d", hv_SendData.Length());
		SendData(m_hv_Socket, HTuple(strFormat), hv_SendData, HTuple());
		ReceiveData(m_hv_Socket, "C30", &hv_RecvData, &hv_From);
		if (hv_RecvData.Length() != 30)
		{
			m_cstrLastError.Format("接收数据包长度不符");
			return FALSE;
		}
		if (hv_RecvData[hv_RecvData.Length() - 1].I() != 0x40
			&& hv_RecvData[hv_RecvData.Length() - 1].I() != 0x00)
		{
			m_cstrLastError.Format("接收数据包异常");
			return FALSE;
		}
		return TRUE;
	}
	catch (HException& e)
	{
		m_cstrLastError.Format("FUNC：%s,	ErrorCode:%d,	ErrorMSG:%s", e.ProcName().Text(), e.ErrorCode(), e.ErrorMessage().Text());
		return FALSE;
	}
}
BOOL CFinsTcpExt::WriteInt16(const int& nDMAddr,const int& nWriteVal)
{
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	m_vecWrite.clear();
	m_vecWrite.push_back((nWriteVal >> 8) & 0xff);
	m_vecWrite.push_back(nWriteVal & 0xff);
	return WriteRegister(nDMAddr, m_vecWrite);



}



//////////////////////////////////////////////////////////////////////////
//ReadInt32Batch 批量读取int型数据
//发送数据,如：
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	读命令		DM地址		读取长度	
//返回数据,如： 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//命令头	命令长度	命令码		错误码		UDP接收帧	PC和PLC地址	读命令		成功标识	数据低16位	数据高16位	数据低16位	数据高16位
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ReadInt32Batch(const int& nDMAddrStart,const int& nReadNum,std::vector<int>& vecReadVal)
{
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	vecReadVal.clear();

	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(nReadNum*4,0xFF,&hv_LengthLower);
	TupleRsh(nReadNum*4,8,&hv_LengthHigher);


	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",30+nReadNum*8);
		SendData(m_hv_Socket,"C34",hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,HTuple(strFormat),&hv_RecvData,&hv_From);
		if (hv_RecvData.Length()!=30+nReadNum*8) return FALSE;
		int nLength = hv_RecvData.Length();
		for (int i=30;i<hv_RecvData.Length();i=i+8)
		{
			/*CString strTmp;
			strTmp.Format("%02x%02x %02x%02x %02x%02x %02x%02x ",hv_RecvData[i].I(),hv_RecvData[i+1].I(),
			hv_RecvData[i+2].I(),hv_RecvData[i+3].I(),hv_RecvData[i+4].I(),hv_RecvData[i+5].I(),
			hv_RecvData[i+6].I(),hv_RecvData[i+7].I());*/
			//strRead+=strTmp;
			int nReadVal = (hv_RecvData[i+2].I() << 24) 
				+ (hv_RecvData[i+3].I() << 16) 
				+ (hv_RecvData[i].I() << 8) 
				+ (hv_RecvData[i+1].I());
			vecReadVal.push_back(nReadVal);
		}
		//theGolbal.tLog.TimeLoging(strRead);
		if (vecReadVal.size()!=nReadNum)
		{
			return FALSE;
		}
		
		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteInt32Batch 批量写入int数据
//发送数据,如：
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//命令头		命令长度		命令码		错误码		UDP发送帧		PLC和PC地址	写命令		DM地址		写入长度		数据低16位	数据高16位	数据低16位	数据高16位
//返回数据,如： 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//命令头		命令长度		命令码		错误码		UDP接收帧		PC和PLC地址	写命令		成功标识
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteInt32Batch(const int& nDMAddrStart,const std::vector<int>& vecWriteVal)
{
	if (!m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	int nWriteNum = vecWriteVal.size();
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || nWriteNum==0) return FALSE;
	
	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher,hv_CmdLengthLower,hv_CmdLengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(nWriteNum*4,0xFF,&hv_LengthLower);
	TupleRsh(nWriteNum*4,8,&hv_LengthHigher);
	TupleBand(26+nWriteNum*8,0xFF,&hv_CmdLengthLower);
	TupleRsh(26+nWriteNum*8,8,&hv_CmdLengthHigher);

	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,hv_CmdLengthHigher.I(),hv_CmdLengthLower.I(),0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	//CString strRead="write 8508: ";
	for (int i=0;i<nWriteNum;i++)
	{
		int nWriteVal = vecWriteVal[i];
		int nVal1 = nWriteVal & 0xff;
		int nVal2 = (nWriteVal >> 8) & 0xff;
		int nVal3 = (nWriteVal >> 16) & 0xff;
		int nVal4 = (nWriteVal >> 24) & 0xff;

		/*CString strTmp;
			strTmp.Format("%02x%02x %02x%02x %02x%02x %02x%02x ",nVal2,nVal1,
			nVal4,nVal3,0,0,0,0);
			strRead+=strTmp;*/

		hv_SendData.Append(nVal2);
		hv_SendData.Append(nVal1);
		hv_SendData.Append(nVal4);
		hv_SendData.Append(nVal3);
		hv_SendData.Append(0);
		hv_SendData.Append(0);
		hv_SendData.Append(0);
		hv_SendData.Append(0);
	}
	//theGolbal.tLog.TimeLoging(strRead);
	if (hv_SendData.Length()!=34+nWriteNum*8)
	{
		return FALSE;
	}
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",hv_SendData.Length());
		SendData(m_hv_Socket,HTuple(strFormat),hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,"C30",&hv_RecvData,&hv_From);
		if (hv_RecvData.Length()!=30) return FALSE;
		if (hv_RecvData[hv_RecvData.Length()-1].I() != 0x40 
			&& hv_RecvData[hv_RecvData.Length()-1].I() != 0x00) 
			return FALSE;

		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}



void  CFinsTcpExt::Read_hex_to_float(const float& val, HTuple hvdata)
{
	char hex[4];
	char* p = (char*)(&val);
	HTuple hv_aa;
	TupleSelect(hvdata, hvdata.Length() - 4, &hv_aa);
	hex[1] = hv_aa[0].I();
	TupleSelect(hvdata, hvdata.Length() - 3, &hv_aa);
	hex[0] = hv_aa[0].I();

	TupleSelect(hvdata, hvdata.Length() - 2, &hv_aa);
	hex[3] = hv_aa[0].I();
	TupleSelect(hvdata, hvdata.Length()-1, &hv_aa);
	hex[2] = hv_aa[0].I();

	float f = *(float *)hex;

	*p = hex[0];
	*(p + 1) = hex[1];
	*(p + 2) = hex[2];
	*(p + 3) = hex[3];
}


BOOL CFinsTcpExt::ReadRegister(const int& nDMAddr, const int& nLenght, std::vector<unsigned char>& vecReadChar)
{
	if (nDMAddr == 0)
	{
		return FALSE;
	}
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0)
	{
		return FALSE;
	}
	int nSends[34] = { 0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0x82,(nDMAddr >> 8) & 0xff,nDMAddr & 0xff,0x00,(nLenght >> 8) & 0xff,(nLenght) & 0xff };
	HTuple hv_SendData(nSends, 34);
	HTuple hv_RecvData, hv_From;
	try
	{
		CString cstrFormat;
		cstrFormat.Format("C%d", 30 + nLenght * 2);
		SendData(m_hv_Socket, "C34", hv_SendData, HTuple());
		ReceiveData(m_hv_Socket, HTuple(cstrFormat), &hv_RecvData, &hv_From);
		if (hv_RecvData.Length() != 30 + nLenght * 2)
		{
			return FALSE;
		}

		for (int i = 30; i <hv_RecvData.Length(); i++)
		{
			vecReadChar.push_back(hv_RecvData[i].I());
		}
		return TRUE;
	}
	catch (HException& e)
	{
		m_bConnected=FALSE;
		return FALSE;
	}
}

BOOL CFinsTcpExt::Vec_CharToInt(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, int& iValue)
{
	iValue = 0;
	if (itrEnd - itrBegin < 4)
	{
		return FALSE;
	}
	iValue = int(*itrBegin << 8) + int(*(itrBegin + 1)) + int(*(itrBegin + 2) << 24) + int(*(itrBegin + 3) << 16);
	return TRUE;
}
BOOL CFinsTcpExt::Vec_CharToShort(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, int& sValue)
{
	sValue = 0;
	if (itrEnd - itrBegin < 2)
	{
		return FALSE;
	}
	sValue = int(*itrBegin << 8) + int(*(itrBegin + 1));
	return TRUE;
}
BOOL CFinsTcpExt::Vec_CharToFloat(std::vector<unsigned char>::const_iterator &itrBegin, std::vector<unsigned char>::const_iterator &itrEnd, float& fValue)
{
	fValue = 0;
	if (itrEnd - itrBegin < 4)
	{
		return FALSE;
	}
	m_cVal4[0] = *(itrBegin + 1);
	m_cVal4[1] = *(itrBegin);
	m_cVal4[2] = *(itrBegin + 3);
	m_cVal4[3] = *(itrBegin + 2);
	fValue = *(float*)m_cVal4;
	return TRUE;
}


BOOL CFinsTcpExt::ReadInt16(const int& nDMAddr,int& nReadVal)
{
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	nReadVal = 0;
	m_vecRead.clear();
	int nLength = 1;//int16
	if (ReadRegister(nDMAddr, nLength , m_vecRead))
	{
		if (m_vecRead.size() < 2)
		{
			return FALSE;
		}

		return Vec_CharToShort(m_vecRead.cbegin(), m_vecRead.cbegin() + 2, nReadVal);
	}
	else
	{
		return FALSE;
	}

}




BOOL CFinsTcpExt::WriteFloat32(const int& nDMAddr,const float& fWriteVal)
{
	CriticalSectionHelper lock(&m_cs);
	if (!m_bConnected)
	{
		return FALSE;
	}
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0) return FALSE;
	m_vecWrite.clear();
	memset(m_cVal4, 0, 4);
	memcpy(m_cVal4, &fWriteVal, 4);
	m_vecWrite.push_back(m_cVal4[1]);
	m_vecWrite.push_back(m_cVal4[0]);
	m_vecWrite.push_back(m_cVal4[3]);
	m_vecWrite.push_back(m_cVal4[2]);
	return WriteRegister(nDMAddr, m_vecWrite);


}
BOOL CFinsTcpExt::ReadFloat32Batch(const int& nDMAddrStart, const int& nReadNum, std::vector<float>& vecReadVal, const int & nSpace)
{
	CriticalSectionHelper lock(&m_cs);
	m_vecRead.clear();
	vecReadVal.clear();
	if (ReadRegister(nDMAddrStart, nSpace * nReadNum, m_vecRead))
	{
		if (m_vecRead.size() < nSpace * nReadNum * 2)
		{
			return FALSE;
		}
		for (int i = 0; i < nReadNum; i++)
		{
			float iTem(0);
			Vec_CharToFloat(m_vecRead.cbegin() + i * 2 * nSpace, m_vecRead.cbegin() + (i + 1) * 2 * nSpace, iTem);
			vecReadVal.push_back(iTem);
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CFinsTcpExt::ReadFloat32(const int& nDMAddr, float& fReadVal)
{
	if (m_hv_PCAddr.Length() == 0 || m_hv_PLCAddr.Length() == 0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	fReadVal = 0;
	m_vecRead.clear();
	int nLength = 2;//Float32
	if (ReadRegister(nDMAddr, nLength, m_vecRead))
	{
		if (m_vecRead.size() < 4)
		{
			return FALSE;
		}

		return Vec_CharToFloat(m_vecRead.cbegin(), m_vecRead.cbegin() + 4, fReadVal);
	}
	else
	{
		return FALSE;
	}

}






//////////////////////////////////////////////////////////////////////////
//ReadStringBatch 批量读取string型数据
//发送数据,如：
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	读命令		DM地址		读取长度	
//返回数据,如： 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//命令头	命令长度	命令码		错误码		UDP接收帧	PC和PLC地址	读命令		成功标识	数据低16位	数据高16位	数据低16位	数据高16位

//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ReadStringBatch(const int& nDMAddrStart,const int& nReadNum,CString& strReadVal)
{
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	strReadVal="";

	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(int(nReadNum*0.5+0.5),0xFF,&hv_LengthLower);
	TupleRsh(int(nReadNum*0.5+0.5),8,&hv_LengthHigher);
	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",30+int(nReadNum+0.5));
		SendData(m_hv_Socket,"C34",hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,HTuple(strFormat),&hv_RecvData,&hv_From);
		/*float a;
		Read_hex_to_float( a, hv_RecvData);*/

		if (hv_RecvData.Length()!=30+int(nReadNum+0.5)) return FALSE;
		int nLength = hv_RecvData.Length();
		for (int i=30;i<hv_RecvData.Length();i=i+2)
		{
			CString strTmp;
			strTmp.Format("%c%c",hv_RecvData[i+1].I(),hv_RecvData[i].I());
			strReadVal += strTmp;
			if(hv_RecvData[i+1].I()==0 || hv_RecvData[i].I()==0)
			{
				break;
			}
		}
		//theGolbal.tLog.TimeLoging(strRead);
		/*if (vecReadVal.size()!=nReadNum)
		{
		return FALSE;
		}*/
		
		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteStringBatch 批量写入string数据
//发送数据,如：
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	写命令		DM地址		写入长度		数据低16位	数据高16位	数据低16位	数据高16位
//返回数据,如： 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//命令头	命令长度	命令码		错误码		UDP接收帧	PC和PLC地址	写命令		成功标识
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteStringBatch(const int& nDMAddrStart,const CString& strWriteVal)
{
	if (!m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	int nWriteNum = strWriteVal.GetLength();
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || nWriteNum==0) return FALSE;
	
	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher,hv_CmdLengthLower,hv_CmdLengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(int(nWriteNum*0.5+0.5),0xFF,&hv_LengthLower);
	TupleRsh(int(nWriteNum*0.5+0.5),8,&hv_LengthHigher);
	TupleBand(26+int(nWriteNum*0.5+0.5),0xFF,&hv_CmdLengthLower);
	TupleRsh(26+int(nWriteNum*0.5+0.5),8,&hv_CmdLengthHigher);

	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,hv_CmdLengthHigher.I(),hv_CmdLengthLower.I(),0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	//CString strRead="write 8508: ";
	for (int i=0;i<nWriteNum;i=i+2)
	{
		int nStr0 = strWriteVal[i+1];
		int nStr1 = strWriteVal[i];

		hv_SendData.Append(nStr0);
		hv_SendData.Append(nStr1);
	}
	if (1==nWriteNum%2)//奇数说明最后一个字符没有添加，高位需填充1个字符
	{
		int nStr0 = 0;
		int nStr1 = strWriteVal[strWriteVal.GetLength()-1];

		hv_SendData.Append(nStr0);
		hv_SendData.Append(nStr1);
	}
	//theGolbal.tLog.TimeLoging(strRead);
	/*if (hv_SendData.Length()!=34+nWriteNum*8)
	{
		return FALSE;
	}*/
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",hv_SendData.Length());
		SendData(m_hv_Socket,HTuple(strFormat),hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,"C30",&hv_RecvData,&hv_From);
		if (hv_RecvData.Length()!=30) return FALSE;
		if (hv_RecvData[hv_RecvData.Length()-1].I() != 0x40 
			&& hv_RecvData[hv_RecvData.Length()-1].I() != 0x00) 
			return FALSE;

		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//ReadDMBatch 批量读取DM区域数据
//发送数据,如：
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	读命令		DM地址		读取长度	
//返回数据,如： 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//命令头	命令长度	命令码		错误码		UDP接收帧	PC和PLC地址	读命令		成功标识	数据低16位	数据高16位	数据低16位	数据高16位
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ReadDMBatch(const int& nDMAddrStart,const int& nLength,HTuple& hvRead)
{
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	hvRead.Clear();

	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(nLength,0xFF,&hv_LengthLower);
	TupleRsh(nLength,8,&hv_LengthHigher);
	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x01,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",30+nLength*2);
		SendData(m_hv_Socket,"C34",hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,HTuple(strFormat),&hv_RecvData,&hv_From);
		if (hv_RecvData.Length()!=30+nLength*2) return FALSE;
		int nLength = hv_RecvData.Length();
		hvRead = hv_RecvData.TupleLastN(30);
		
		if (hvRead.Length()!=nLength*2)
		{
			return FALSE;
		}

		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteDMBatch 批量写入DM区域数据
//发送数据,如：
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//命令头	命令长度	命令码		错误码		UDP发送帧	PLC和PC地址	写命令		DM地址		写入长度	数据低16位	数据高16位	数据低16位	数据高16位
//返回数据,如： 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//命令头	命令长度	命令码		错误码		UDP接收帧	PC和PLC地址	写命令		成功标识
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteDMBatch(const int& nDMAddrStart,const HTuple& hvWrite)
{
	if (!m_bConnected) return FALSE;
	HTuple hvWriteTmp = hvWrite;
	CriticalSectionHelper lock(&m_cs);
	int nWriteNum = hvWriteTmp.Length();
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || nWriteNum==0) return FALSE;
	if (nWriteNum%2==1)
	{
		hvWriteTmp = hvWriteTmp.TupleInsert(hvWriteTmp.Length()-2,0);
		nWriteNum = hvWriteTmp.Length();
	}
	
	HTuple hv_DMLower,hv_DMHigher,hv_LengthLower,hv_LengthHigher,hv_CmdLengthLower,hv_CmdLengthHigher;
	TupleBand(nDMAddrStart,0xFF,&hv_DMLower);
	TupleRsh(nDMAddrStart,8,&hv_DMHigher);
	TupleBand(int(nWriteNum*0.5+0.5),0xFF,&hv_LengthLower);
	TupleRsh(int(nWriteNum*0.5+0.5),8,&hv_LengthHigher);
	TupleBand(26+nWriteNum,0xFF,&hv_CmdLengthLower);
	TupleRsh(26+nWriteNum,8,&hv_CmdLengthHigher);

	int nSends[34] = {0x46,0x49,0x4e,0x53,0x00,0x00,hv_CmdLengthHigher.I(),hv_CmdLengthLower.I(),0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x80,0x00,0x02,0x00,
		m_hv_PLCAddr.I(),0x00,0x00,m_hv_PCAddr.I(),0x00,0x00,0x01,0x02,0x82,hv_DMHigher.I(),hv_DMLower.I(),0x00,hv_LengthHigher.I(),hv_LengthLower.I()};
	HTuple hv_SendData(nSends,34);
	hv_SendData.Append(hvWriteTmp);
	//theGolbal.tLog.TimeLoging(strRead);
	if (hv_SendData.Length()!=34+nWriteNum)
	{
		return FALSE;
	}
	HTuple hv_RecvData,hv_From;
	try
	{
		CString strFormat;
		strFormat.Format("C%d",hv_SendData.Length());
		SendData(m_hv_Socket,HTuple(strFormat),hv_SendData,HTuple());
		ReceiveData(m_hv_Socket,"C30",&hv_RecvData,&hv_From);
		if (hv_RecvData.Length()!=30) return FALSE;
		if (hv_RecvData[hv_RecvData.Length()-1].I() != 0x40 
			&& hv_RecvData[hv_RecvData.Length()-1].I() != 0x00) 
			return FALSE;

		return TRUE;
	}catch(HException& e)
	{
		if (e.ErrorCode() == 5619)
		{
			//超时重新握手
			ShakeHand();
		}
		return FALSE;
	}
}