#include "stdafx.h"
#include "FinsTCPExt.h"

CFinsTcpExt::CFinsTcpExt()
{
	InitializeCriticalSection(&m_cs);
	m_bConnected = FALSE;
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
			OpenSocketConnect(HTuple(strIP),nPort,"protocol","TCP",&m_hv_Socket);
			SetSocketParam(m_hv_Socket,"timeout",5);
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
//ShakeHand ����
//��������,�磺
//46494e53	0000000c	00000000	00000000	00000000
//����ͷ	�����	������		������		�ڵ�
//��������,�磺 
//46494e53	00000010	00000001	00000000	000000ef	00000063 (ef��PC����ַ��63��PLC��ַ)
//����ͷ	�����	������		������		PC����ַ	PLC��ַ
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
//ReadInt32 ��ȡint����
//��������,�磺
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0002
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	������		DM��ַ		��ȡ����	
//��������,�磺 
//46494e53	0000001a	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000
//����ͷ		�����		������		������		UDP����֡		PC��PLC��ַ	������		�ɹ���ʶ		���ݵ�16λ	���ݸ�16λ
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::ReadInt32(const int& nDMAddr,int& nReadVal)
{
	if (m_hv_PCAddr.Length()==0 || m_hv_PLCAddr.Length()==0 || !m_bConnected) return FALSE;

	CriticalSectionHelper lock(&m_cs);
	nReadVal = 0;

	HTuple hv_DMLower,hv_DMHigher;
	TupleBand(nDMAddr,0xFF,&hv_DMLower);
	TupleRsh(nDMAddr,8,&hv_DMHigher);
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
		if (e.ErrorCode() == 5619)
		{
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteInt32 дint����
//��������,�磺
//46494e53	0000001e	00000002	00000000	80000200	630000ef	00000102	8202ee00	0002		0016		0000
//����ͷ		�����		������		������		UDP����֡		PLC��PC��ַ	д����		DM��ַ		д�볤��		���ݵ�16λ	���ݸ�16λ
//��������,�磺 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//����ͷ		�����		������		������		UDP����֡		PC��PLC��ַ	д����		�ɹ���ʶ
//////////////////////////////////////////////////////////////////////////
BOOL CFinsTcpExt::WriteInt32(const int& nDMAddr,const int& nWriteVal)
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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//ReadInt32Batch ������ȡint������
//��������,�磺
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	������		DM��ַ		��ȡ����	
//��������,�磺 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//����ͷ	�����	������		������		UDP����֡	PC��PLC��ַ	������		�ɹ���ʶ	���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ
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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteInt32Batch ����д��int����
//��������,�磺
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//����ͷ		�����		������		������		UDP����֡		PLC��PC��ַ	д����		DM��ַ		д�볤��		���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ
//��������,�磺 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//����ͷ		�����		������		������		UDP����֡		PC��PLC��ַ	д����		�ɹ���ʶ
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
			//��ʱ��������
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


//////////////////////////////////////////////////////////////////////////
//ReadStringBatch ������ȡstring������
//��������,�磺
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	������		DM��ַ		��ȡ����	
//��������,�磺 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//����ͷ	�����	������		������		UDP����֡	PC��PLC��ַ	������		�ɹ���ʶ	���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ

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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteStringBatch ����д��string����
//��������,�磺
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	д����		DM��ַ		д�볤��		���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ
//��������,�磺 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//����ͷ	�����	������		������		UDP����֡	PC��PLC��ַ	д����		�ɹ���ʶ
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
	if (1==nWriteNum%2)//����˵�����һ���ַ�û����ӣ���λ�����1���ַ�
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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//ReadDMBatch ������ȡDM��������
//��������,�磺
//46494e53	0000001a	00000002	00000000	80000200	630000ef	00000101	8202ee00	0004
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	������		DM��ַ		��ȡ����	
//��������,�磺 
//46494e53	0000001e	00000002	00000000	c0000200	ef000063	00000101	0040		0016		0000		0016		0000
//����ͷ	�����	������		������		UDP����֡	PC��PLC��ַ	������		�ɹ���ʶ	���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ
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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////////
//WriteDMBatch ����д��DM��������
//��������,�磺
//46494e53	00000022	00000002	00000000	80000200	630000ef	00000102	8202ee00	0004		0016		0000		0016		0000
//����ͷ	�����	������		������		UDP����֡	PLC��PC��ַ	д����		DM��ַ		д�볤��	���ݵ�16λ	���ݸ�16λ	���ݵ�16λ	���ݸ�16λ
//��������,�磺 
//46494e53	00000016	00000002	00000000	c0000200	ef000063	00000102	0040
//����ͷ	�����	������		������		UDP����֡	PC��PLC��ַ	д����		�ɹ���ʶ
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
			//��ʱ��������
			ShakeHand();
		}
		return FALSE;
	}
}