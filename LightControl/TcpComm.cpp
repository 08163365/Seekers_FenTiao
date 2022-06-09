#include "StdAfx.h"
#include "TcpComm.h"

CTcpComm::CTcpComm()
{
	m_bOpen = false;
}

CTcpComm::~CTcpComm()
{
	this->Close();
}


bool CTcpComm::Open(CString IP, int Port) //CString IP, int Port
{
	if (m_bOpen) 
		this->Close();

	hv_Protocol = "TCP4";
	hv_Timeout = 0.1;
    OpenSocketConnect(HTuple(IP), Port, (HTuple("protocol").Append("timeout")), hv_Protocol.TupleConcat(hv_Timeout), &hv_Socket);
	
	
	m_bOpen = true;
	return m_bOpen;
	return false;
}

bool CTcpComm::Close()
{
	if (!m_bOpen) return false;
	
	try 
	{
		CloseSocket(hv_Socket);
		m_bOpen = FALSE;
	}
	catch ( HException&)
	{
		return false;
	}
	return true;
}

bool CTcpComm::Connect(CString IP, int Port)
{
	
	hv_Protocol = "TCP4";
	hv_Timeout = 0.1;

	//Open a socket connection
	try
	{
		OpenSocketConnect(HTuple(IP), Port, (HTuple("protocol").Append("timeout")),
			hv_Protocol.TupleConcat(hv_Timeout), &hv_Socket);
		GetSocketParam(hv_Socket, "address_info", &hv_Address);

		hv_To = HTuple();
		//
		//SendData(hv_Socket, "Z", "SA0001#SB0002#H#", hv_To);
		//ReceiveData(hv_Socket, "z", &hv_Answer, &hv_From);


	}
	catch (HException&)
	{

	}

	m_bOpen = TRUE;
	return TRUE;
}

bool CTcpComm::GetLightBrightness(const int & nChannel, int & nBrightness)
{

	{
		
		if (!m_bOpen) return false;
		if (nChannel < 1) return false;

		CString strCmd;
		CString strRes;
		strCmd.Format("S%c#", 'A' + (nChannel - 1));

		try
		{

			hv_To.Clear();
			hv_From.Clear();
			hv_Answer.Clear();
			SendData(hv_Socket, "z", (HTuple)strCmd, hv_To);
			ReceiveData(hv_Socket, "z", &hv_Answer, &hv_From);//hv_Answer返回一个光源亮度指示符
		}
		catch (HException& e)
		{
			
			return false;
		}
		strRes.Format("%s", hv_Answer.S().Text());//HTuple(.S())->HString(.Text())->char*
		strRes.Delete(0, 1);//b0001, 所以删掉b，再进行转换为整数
		nBrightness = atoi(strRes);

	}

	return true;
}

bool CTcpComm::SetLightBrightness(const int &nChannel, int nBrightness) 
{
	if (!m_bOpen) return false;
	if (nChannel < 1) return false;
	if (nBrightness < 0) nBrightness = 0;
	if (nBrightness > 255) nBrightness = 255;

	CString strCmd, strRes;
	strCmd.Format("S%c%0.4d#", 'A' + (nChannel - 1), nBrightness);
	//发送指令写到try   catch中，可以防止发生错误时直接崩溃
	try
	{
		hv_To.Clear();
		hv_From.Clear();
		hv_Answer.Clear();
		SendData(hv_Socket, "z", (HTuple)strCmd, hv_To);
		ReceiveData(hv_Socket, "z", &hv_Answer, &hv_From);
	}
	catch (HException&)
	{
		
		return false;
	}
	//判断有没有设置成功
	strRes.Format("%s",hv_Answer.S().Text());
	if (strRes.MakeUpper() != _T('A' + (nChannel - 1))) return false;//MakeUpper() 统一转换成大写
	
	return true;
}

bool CTcpComm::Read(CString  &strReceiveData) 
{
	if (!m_bOpen)    return false;
	if (m_tReadovReady.hEvent == NULL)    return false;

	char chRecv[2048];
	if (ReadFile(m_tcpComm, chRecv, 2000, NULL, &m_tReadovReady) == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)    return false;
	}

	DWORD dwRead;
	DWORD dwRes = WaitForSingleObject(m_tReadovReady.hEvent, READ_TIMEOUT);
	strReceiveData.Format("%s", chRecv);
	switch (dwRes)
	{
	case WAIT_OBJECT_0:
		if (!GetOverlappedResult(m_tcpComm, &m_tReadovReady, &dwRead, FALSE))    return false;
		break;

	case WAIT_TIMEOUT:
		break;

	default:
		break;
	}
	return true;
}

bool CTcpComm::Write(const CString &strSendData) 
{
	if (!m_bOpen) 
		return false;

	DWORD    BytesSent;
	DWORD    resD;

	CString strSend = strSendData;
	PurgeComm(m_tcpComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	BytesSent = 0;
	BOOL hr = WriteFile(m_tcpComm,                            // Handle to COMM Port
		(char*)strSend.GetBuffer(),							// Pointer to message buffer in calling finction
		strSend.GetLength(),								// Length of message to send
		&BytesSent,											// Where to store the number of bytes sent
		&m_tWriteovReady);									// Overlapped structure
	strSend.ReleaseBuffer();
	if (!hr)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
		else
		{
			resD = WaitForSingleObject(m_tWriteovReady.hEvent, READ_TIMEOUT);
		}

	}
	return true;
}

bool CTcpComm::SetLightStatus(bool bOn) 
{
	if (!m_bOpen) return false;

	CString strCmd, strRes;
	if (bOn) strCmd.Format("SH#");//常亮
	else strCmd.Format("SL#");//常灭


	try
	{
		hv_To.Clear();
		hv_From.Clear();
		SendData(hv_Socket, "z", (HTuple)strCmd, hv_To);
		ReceiveData(hv_Socket, "z", &hv_Answer, &hv_From);
	}
	catch (HException&)
	{

		return false;
	}


	//if (!Write(strCmd)) return false;
	//Sleep(50);
	//if (!Read(strRes)) return false;









	//判断有没有设置成功
	if (-1 == strCmd.Find(strRes)) return false;

	return true;
}
