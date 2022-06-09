#include "StdAfx.h"
#include "HcModbusTcpClient.h"

HcModbusTcpClient::HcModbusTcpClient(void)
{
	m_nPort = g_nPLCPort;
	m_strIpAddress = _T(g_strPLCIP);
	CStringA str;
	str = m_strIpAddress;
	DWORD IpAddress = inet_addr(g_strPLCIP);
	m_dIpAddress   = IpAddress;//inet_addr("192.168.1.2");
	m_bConnected   = FALSE;
	m_bFirstFailed = TRUE;
	m_bExiting     = FALSE;
}


HcModbusTcpClient::~HcModbusTcpClient(void)
{
	if(m_bConnected)
	{
		m_fnClose(1);
	}
	FreeLibrary(hInst);
}
BOOL HcModbusTcpClient::ServerConnectIsEnable(CString strIp,int nPort,int nTimeOut)
{
	WSADATA wsaData;
	if(::WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf ("Failed to WSAStartup\r\n");
		return FALSE;
	}
	SOCKET fd;
	if ((fd = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		int nCode = ::WSAGetLastError();
		printf ("Failed to create socket\r\n");
		//AfxMessageBox(_T("Failed to create socket\r\n"));
		//PRINTF(_T("Client Failed to create socket."));
		//MYBOX.LogWrite(_T("Client Failed to create socket."));
		return FALSE;
	}
	char tmpstr[TEMP_STR_LEN];
	memset(tmpstr, 0, TEMP_STR_LEN);
	WideCharToMultiByte( CP_ACP, 0, (LPCWSTR)strIp.GetBuffer(), -1, tmpstr, TEMP_STR_LEN, NULL, NULL ); 
	strIp.ReleaseBuffer();
	struct sockaddr_in video_server;
	memset (&video_server, 0, sizeof (video_server));
	video_server.sin_family = AF_INET;
	video_server.sin_addr.s_addr = inet_addr (g_strPLCIP);
	video_server.sin_port = htons (g_nPLCPort);

	int error=-1, len;
	len = sizeof(int);
	timeval tm;
	fd_set set;
	unsigned long ul = 1;
	//ioctlsocket(fd, FIONBIO, &ul); //设置为非阻塞模式

	bool ret = false;
	if(connect(fd, (struct sockaddr *)&video_server, sizeof(video_server)) == -1)
	{
		int t = WSAGetLastError();
		tm.tv_sec  = nTimeOut;
		tm.tv_usec = 0;
		FD_ZERO(&set);
		FD_SET(fd, &set);
		if( select(fd+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&error, /*(socklen_t *)*/&len);
			if(error == 0) 
				ret = TRUE;
			else 
				ret = false;
		} 
		else 
			ret = false;
	}
	else 
		ret = TRUE;
	ul = 0;
	ioctlsocket(fd, FIONBIO, &ul); //设置为阻塞模式
	if(!ret) 
	{
		closesocket(fd);
		fprintf(stderr , "Cannot Connect the server!/n");
		CString strMsg;
		strMsg.Format(_T("Client Cannot Connect the server %s:%d."),strIp,nPort);
		//PRINTF(strMsg);

		return FALSE;
	}
	closesocket(fd);
	fprintf( stderr , "Connected!/n");
	return TRUE;
}
BOOL HcModbusTcpClient::Connect()
{
//	//PRINTF(_T("HcModbusTcpClient::Connect() start..."));
	if(m_bExiting)
			return FALSE;
	BOOL bIsEnable = FALSE;
//	CPing ping;
	CStringA strIp;
	strIp = m_strIpAddress;
	for(int i =0; i < 3; i++)
	{
		if(ServerConnectIsEnable(m_strIpAddress,m_nPort))//ServerConnectIsEnable(_T("192.168.1.2"),502)
		{
			bIsEnable = TRUE;
			break;
		}
		if(m_bExiting)
			return FALSE;
		strIp.ReleaseBuffer();
		Sleep(1);
	}
	strIp.ReleaseBuffer();
	if(!bIsEnable)
	{
		////PRINTF(_T("HcModbusTcpClient::Connect() failed1."));
		CString strMsg;
		strMsg.Format(_T("连接控制器失败:%s"),m_strIpAddress);
	//	//PRINTF(strMsg);
		if(m_bFirstFailed)
		{
			//AfxMessageBox(strMsg);
			m_bFirstFailed = FALSE;
		}
		m_bConnected = FALSE;
		m_dwLastConnectTime = GetTickCount();
		return FALSE;
	}
	Sleep(1);
	 m_fnSetTimeOut(1,500);
	if(m_bExiting)
			return FALSE;
	 int nRes = m_fnConnect(1,m_nPort,m_dIpAddress);
	 m_bConnected = nRes;
	 if(!m_bConnected)
	 {
		 ////PRINTF(_T("HcModbusTcpClient::Connect() failed2."));
		 CString strMsg;
		 strMsg.Format(_T("连接%s,nPort:%d,IpPort:%d 失败."),m_strIpAddress,1,m_nPort);
		// //PRINTF(strMsg);
		 if(m_bFirstFailed)
		 {
			//AfxMessageBox(strMsg);
			 //BLMessageBox(strMsg);
			m_bFirstFailed = FALSE;
		 }
		 m_bConnected = FALSE;
	 }
	 else
	 {
		m_bFirstFailed = TRUE;
	 }
	 //PRINTF(_T("HcModbusTcpClient::Connect() finished."));
	 m_bConnected = TRUE;
	 m_dwLastConnectTime = GetTickCount();
	 return nRes;
}
BOOL HcModbusTcpClient::Init()
{
	
	hInst = LoadLibraryA("HCTcp.dll");//
	if(NULL == hInst)
	{
		AfxMessageBox(_T("加载动态库HCTcp.dll 失败."));
		return FALSE;
	}
	m_fnConnect = (MBTCPCONNECT)GetProcAddress(hInst,"mbtcpConnect");
	if(!m_fnConnect)
	{
		//AfxMessageBox(_T("获取函数mbtcpConnect 失败."));
		return FALSE;
	}
	m_fnClose = (MBTCPCLOSE)GetProcAddress(hInst,"mbtcpClose");
	if(!m_fnClose)
	{
		//AfxMessageBox(_T("获取函数mbtcpClose 失败."));
		return FALSE;
	}
	m_fnSetTimeOut = (MBTCPSETTIMEOUT)GetProcAddress(hInst,"mbtcpSetTimeout");
	if(!m_fnSetTimeOut)
	{
		//AfxMessageBox(_T("获取函数mbtcpSetTimeout 失败."));
		return FALSE;
	}
	m_fnSetDelay = (MBTCPSETDELAY)GetProcAddress(hInst,"mbtcpSetDelay");
	if(!m_fnSetDelay)
	{
		//AfxMessageBox(_T("获取函数mbtcpSetDelay 失败."));
		return FALSE;
	}
	m_fnMbt01 = (MBTCPFCN01)GetProcAddress(hInst,"mbtcpfcn01");
	if(!m_fnMbt01)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn01 失败."));
		return FALSE;
	}
	m_fnMbt02 = (MBTCPFCN01)GetProcAddress(hInst,"mbtcpfcn02");
	if(!m_fnMbt02)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn02 失败."));
		return FALSE;
	}
	m_fnMbt03 = (MBTCPFCN03)GetProcAddress(hInst,"mbtcpfcn03");
	if(!m_fnMbt03)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn03 失败."));
		return FALSE;
	}
	m_fnMbt04 = (MBTCPFCN04)GetProcAddress(hInst,"mbtcpfcn04");
	if(!m_fnMbt04)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn04 失败."));
		return FALSE;
	}
	m_fnMbt05 = (MBTCPFCN05)GetProcAddress(hInst,"mbtcpfcn05");
	if(!m_fnMbt05)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn05 失败."));
		return FALSE;
	}
	m_fnMbt06 = (MBTCPFCN06)GetProcAddress(hInst,"mbtcpfcn06");
	if(!m_fnMbt06)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn06 失败."));
		return FALSE;
	}
	m_fnMbt15 = (MBTCPFCN15)GetProcAddress(hInst,"mbtcpfcn15");
	if(!m_fnMbt15)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn15 失败."));
		return FALSE;
	}
	m_fnMbt16 = (MBTCPFCN16)GetProcAddress(hInst,"mbtcpfcn16");
	if(!m_fnMbt06)
	{
		//AfxMessageBox(_T("获取函数mbtcpfcn16 失败."));
		return FALSE;
	}
	return TRUE;
}
BOOL HcModbusTcpClient::GetMutiWordData(int nAddress,int* ReadData,int nCount)
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	
	BYTE RxdBuffer[200];
	int len = 0;
	int nReadCount = nCount;
	m_fnMbt03(1,255,nAddress,nReadCount,(int*)RxdBuffer,&len);
	WORD* pWord = new WORD[nReadCount];
	int nDataLen = 2*nReadCount;
	if(len > nDataLen)
	{
		for(int i = len-nDataLen; i < len; i+=2)
		{
			pWord[(i-(len-nDataLen))/2] = MAKEWORD(RxdBuffer[i+1],RxdBuffer[i]);
			ReadData[(i-(len-nDataLen))/2] = pWord[(i-(len-nDataLen))/2];
		}
	}
	else
	{
		delete []pWord;
		m_fnClose(1);
		//Connect();
		//PRINTF(_T("GetMutiWordData m_fnMbt03 error,nAddress%d,nCount%d,len%d."),nAddress,nCount,len);
		m_bConnected = FALSE;
		m_readLock.Unlock();
		return FALSE;
	}
	//delete []RxdBuffer;
	//DWORD data = *(DWORD*)pWord;
	delete []pWord;
	m_readLock.Unlock();
	return TRUE;
}
DWORD HcModbusTcpClient::GetDWordData(int nAddress)//32 QW DWORD
{
	///////////////获取汇川PLC数据////////////
	//BYTE* RxdBuffer = new BYTE[20+nCount*2];
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	
	BYTE RxdBuffer[20];
	int len = 0;
	int nReadCount = 2;
	m_fnMbt03(1,255,nAddress*2,nReadCount,(int*)RxdBuffer,&len);
	WORD* pWord = new WORD[nReadCount];
	int nDataLen = 2*nReadCount;
	if(len > nDataLen)
	{
		for(int i = len-nDataLen; i < len; i+=2)
		{
			pWord[(i-(len-nDataLen))/2] = MAKEWORD(RxdBuffer[i+1],RxdBuffer[i]);
		}
	}
	else
	{
		delete []pWord;
		m_fnClose(1);
		//Connect();
		//PRINTF(_T("GetDWordData m_fnMbt03 error,nAddress%d,len%d."),nAddress,len);
		m_bConnected = FALSE;
		m_readLock.Unlock();
		return FALSE;
	}
	//delete []RxdBuffer;
	DWORD data = *(DWORD*)pWord;
	delete []pWord;
	m_readLock.Unlock();
	///////////////获取汇川PLC数据结束////////////
	return data;
}

WORD HcModbusTcpClient::GetWordData(int nAddress) //M 
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	///////////////获取汇川PLC数据////////////
	//BYTE* RxdBuffer = new BYTE[20+nCount*2];
	BYTE RxdBuffer[20];
	int len = 0;
	int nReadCount = 1;
	m_fnMbt03(1,255,nAddress,nReadCount,(int*)RxdBuffer,&len);
	WORD* pWord = new WORD[nReadCount];
	int nDataLen = 2*nReadCount;
	if(len > nDataLen)
	{
		for(int i = len-nDataLen; i < len; i+=2)
		{
			pWord[(i-(len-nDataLen))/2] = MAKEWORD(RxdBuffer[i+1],RxdBuffer[i]);
		}
	}
	else
	{
		delete []pWord;
		m_fnClose(1);
		m_bConnected = FALSE;
		//PRINTF(_T("GetWordData m_fnMbt03 error,nAddress%d,len%d."),nAddress,len);
		m_readLock.Unlock();
		return FALSE;
	}
	//delete []RxdBuffer;
	DWORD data = *(DWORD*)pWord;
	delete []pWord;
	m_readLock.Unlock();
	///////////////获取汇川PLC数据结束////////////
	return data;
}
BOOL HcModbusTcpClient::SetDWordData(int nAddress,int nValue)
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	BYTE RxdBuffer[20];
	WORD TxdBuffer[20];
	TxdBuffer[0] = LOWORD(nValue);
	TxdBuffer[1] = HIWORD(nValue);
	int len = 0;
	int nReadCount = 1;
	//mbtcpfcn16(int nport, int node, int address, int Count,WORD* TxdBuffer,int* RxdBuffer,int* RxdLength);
	int nRes = m_fnMbt16(1,255,nAddress*2,2,TxdBuffer,(int*)RxdBuffer,&len);
	if(nRes < 1)
	{
		m_fnClose(1);
		//Connect();
		m_bConnected = FALSE;
		//PRINTF(_T("SetDWordData m_fnMbt16 error,nAddress%d,nValue%d,len%d."),nAddress,nValue,len);
		m_readLock.Unlock();
		return FALSE;
	}
	m_readLock.Unlock();
	return nRes;
}

BOOL HcModbusTcpClient::SetWordData(int nAddress,int nValue)
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	BYTE RxdBuffer[20];
	WORD TxdBuffer[20];
	TxdBuffer[0] = nValue;
	TxdBuffer[1] = 0;
	int len = 0;
	int nReadCount = 1;
	//mbtcpfcn16(int nport, int node, int address, int Count,WORD* TxdBuffer,int* RxdBuffer,int* RxdLength);
	int nRes = m_fnMbt16(1,255,nAddress,1,TxdBuffer,(int*)RxdBuffer,&len);
	if(nRes < 1)
	{
		m_fnClose(1);
		//Connect();
		m_bConnected = FALSE;
		//PRINTF(_T("SetWordData m_fnMbt16 error,nAddress%d,nValue%d,len%d."),nAddress,nValue,len);
		m_readLock.Unlock();
		return FALSE;
	}
	m_readLock.Unlock();
	return nRes;
}

BOOL HcModbusTcpClient::ReadBit(int nAddress)
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	BYTE RxdBuffer[20];
	int len = 0;
	m_fnMbt02(1,255,nAddress,1,(int*)RxdBuffer,&len);
	if(len > 0)
	{
		m_readLock.Unlock();
		return RxdBuffer[len-1];
	}
	m_fnClose(1);
	//Connect();
	m_bConnected = FALSE;
	//PRINTF(_T("ReadBit m_fnMbt02 error,nAddress%d,len%d."),nAddress,len);
	m_readLock.Unlock();
	return FALSE;
}
BOOL HcModbusTcpClient::ReadWBit(int nAddress)
{
	WORD nRes = GetWordData(nAddress/16);
	int nPoint = nAddress%16;

	return nRes & 0x01 << nPoint;
}
BOOL HcModbusTcpClient::WriteWBit(int nAddress,BOOL bOn)
{
	WORD nRes = GetWordData(nAddress/16);//
	int nPoint = nAddress%16;
	if(bOn)
	{
		nRes |= 0x0001 << nPoint;
	}
	else
	{
		nRes &= ~(0x0001 << nPoint);
	}
	
	return SetWordData(nAddress/16,nRes);
}
BOOL HcModbusTcpClient::WriteBit(int nAddress,BOOL bOn)
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	BYTE RxdBuffer[20];
	int len = 0;
	m_fnMbt05(1,255,nAddress,1,bOn,(int*)RxdBuffer,&len);
	if(len > 0)
	{
		m_readLock.Unlock();
		return TRUE;
	}
	m_fnClose(1);
	//Connect();
	m_bConnected = FALSE;
	//PRINTF(_T("WriteBit m_fnMbt05 error,nAddress%d,bOn%d,len%d."),nAddress,bOn,len);
	m_readLock.Unlock();
	return FALSE;
}
int HcModbusTcpClient::ReadInChannel(int nAddressStart,int nCount) //Q bit
{
	if(!m_bConnected)
		return 0;
	m_readLock.Lock();
	if(nCount > 32)
		nCount = 32;
	BYTE RxdBuffer[20];
	int len = 0;
	m_fnMbt02(1,255,nAddressStart,nCount,(int*)RxdBuffer,&len);
	if(len > 0 && len < 19)
	{
		m_readLock.Unlock();
		int res = RxdBuffer[len-1];
		if(nCount < 9)
			res = RxdBuffer[len-1];
		else
		{
			res = RxdBuffer[len-2];
			res |= RxdBuffer[len-1] << 8;
		}
		return res;
	}
	m_fnClose(1);
	//Connect();
	m_bConnected = FALSE;
	//PRINTF(_T("ReadInChannel m_fnMbt02 error,AddressStart%d,count%d,len%d."),nAddressStart,nCount,len);
	m_readLock.Unlock();
	return 0;
}

//BOOL HcModbusTcpClient::GetWordData(int nAddress,int* ReadData,int nCount)
//{
//	if(nCount < 1 || ReadData == NULL)
//		return FALSE;
//	///////////////获取汇川PLC数据////////////
//	BYTE* RxdBuffer = new BYTE[20+nCount*2];
//	int len = 0;
//	int nReadCount = nCount;
//	m_fnMbt03(1,255,nAddress,nReadCount,(int*)RxdBuffer,&len);
//	WORD* pWord = new WORD[nReadCount];
//	int nDataLen = 2*nReadCount;
//	if(len > nDataLen)
//	{
//		for(int i = len-nDataLen; i < len; i+=2)
//		{
//			pWord[(i-(len-nDataLen))/2] = MAKEWORD(RxdBuffer[i],RxdBuffer[i+1]);
//		}
//	}
//	else
//	{
//		return FALSE;
//	}
//	delete []RxdBuffer;
//	*ReadData = *(DWORD*)pWord;
//	delete []pWord;
//
//	///////////////获取汇川PLC数据结束////////////
//	return TRUE;
//}