#include "stdafx.h"
#include "SerialPortExt.h"

//const CString STR_LabelWarnInfo[]
//=
//{
//	"OK",
//	"ȱ�걨��",
//	"�ޱ걨��,�뻻��",
//	"�ϴ�����,�ϴ�",
//	"�ߴ�����,�ߴ��쳣",
//	"©���걨��",
//	"����10����",
//	"����ͣ������",
//	"��������",
//	"�����쳣����",
//	"��ѹ�ͱ���",
//	"��ѹ�߱���",
//	"����λ�ñ���",
//	"��λ����ʧ�ܱ���",
//	"�������쳣����",
//	"����ʧ�ܱ���",
//	"δ֪����",
//};




CSerialPortExt::CSerialPortExt()
{
	m_bOpen = false;
	DbState = FALSE;
}

CSerialPortExt::CSerialPortExt(CString strPortName, int nBaudRate, int nBitCount, int nParity, int nStopBits)
{
	m_bOpen = false;
	this->Open(strPortName, nBaudRate, nBitCount, nParity, nStopBits);
}

CSerialPortExt::CSerialPortExt(int nComSN, int nBaudRate, int nBitCount, int nParity, int nStopBits)
{
	m_bOpen = false;
	this->Open(nComSN, nBaudRate, nBitCount, nParity, nStopBits);
}

CSerialPortExt::~CSerialPortExt()
{
	this->Close();
}

bool CSerialPortExt::Open(int nComSN, int nBaudRate, int nBitCount, int nParity, int nStopBits)
{
	CString strCom;
	strCom.Format("COM%d", nComSN);
	return Open(strCom, nBaudRate, nBitCount, nParity, nStopBits);
}

bool CSerialPortExt::Open(CString strPortName, int nBaudRate, int nBitCount, int nParity, int nStopBits)
{
	if (m_bOpen) this->Close();
	
	//CString strPort = "\\\\.\\" + strPortName;
	CString strPort = strPortName + ":";
	m_hComm = CreateFile(_T(strPort), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
	int error = GetLastError();

	if (m_hComm == INVALID_HANDLE_VALUE)        return false;
	if (!SetupComm(m_hComm, 1024, 512))        return false;

	COMMTIMEOUTS commtimeouts;
	commtimeouts.ReadIntervalTimeout = MAXDWORD;
	commtimeouts.ReadTotalTimeoutConstant = 0;
	commtimeouts.ReadTotalTimeoutMultiplier = 0;
	commtimeouts.WriteTotalTimeoutConstant = 0;
	commtimeouts.WriteTotalTimeoutMultiplier = 0;

	if (!SetCommTimeouts(m_hComm, &commtimeouts))        return false;

	memset(&m_tReadovReady, 0, sizeof(OVERLAPPED));
	memset(&m_tWriteovReady, 0, sizeof(OVERLAPPED));
	m_tReadovReady.hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
	m_tWriteovReady.hEvent = CreateEventA(NULL, TRUE, FALSE, NULL);

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	DCB dcb;
	GetCommState(m_hComm, &dcb);
	//dcb.fBinary = TRUE;
	//dcb.fParity = TRUE;
	// ������  ����λ  ��־λ �����Լ����豸�ڴ����޸� 
	dcb.BaudRate = nBaudRate;        // baud rate
	dcb.ByteSize = nBitCount;
	dcb.Parity = nParity;
	dcb.StopBits = nStopBits - 1;//bytesizeΪ8ʱ������ֹͣλΪ0���궨�岻ͬ

	if (!SetCommState(m_hComm, &dcb))        return false;

	m_bOpen = true;
	return m_bOpen;
}

bool CSerialPortExt::Write(const CString &strSendData)
{
	if (!m_bOpen)    return false;

	DWORD    BytesSent;
	DWORD    resD;

	CString strSend = strSendData;
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	BytesSent = 0;
	BOOL hr = WriteFile(m_hComm,                            // Handle to COMM Port
		(char*)strSend.GetBuffer(),							// Pointer to message buffer in calling finction
		strSend.GetLength(),								// Length of message to send
		&BytesSent,											// Where to store the number of bytes sent
		&m_tWriteovReady);									// Overlapped structure
	strSend.ReleaseBuffer();
	if (!hr)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			pLogView->DoSomething("EXCEPTION", _T("CSerialPortExt::Write��������ERROR_IO_PENDING"), NULL);
			return false;
		}
		else
		{
			resD = WaitForSingleObject(m_tWriteovReady.hEvent, READ_TIMEOUT);
		}
		switch (resD)
		{
		case WAIT_OBJECT_0:
		{
			if (!GetOverlappedResult(m_hComm, &m_tWriteovReady, &BytesSent, false))
			{
				pLogView->DoSomething("EXCEPTION", _T("CSerialPortExt::Write����GetOverlappedResult����0"), NULL);
				return false;
			}
			else
				return true;

		}
		default:
			pLogView->DoSomething("EXCEPTION", _T("CSerialPortExt::Write����default����0"), NULL);
			return false;
			break;
		}
	}
	return true;
}

bool CSerialPortExt::Write(char * buf, int len)
{
	unsigned char bbit = buf[7];
	if (!m_bOpen)    return false;

	DWORD    BytesSent;
	DWORD    resD;


	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	BytesSent = 0;
	BOOL hr = WriteFile(m_hComm,                            // Handle to COMM Port
		buf,							// Pointer to message buffer in calling finction
		len,								// Length of message to send
		&BytesSent,											// Where to store the number of bytes sent
		&m_tWriteovReady);									// Overlapped structure
	if (!hr)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Write����GetLastError"), NULL);
			return false;
		}
		else
		{
			resD = WaitForSingleObject(m_tWriteovReady.hEvent, READ_TIMEOUT);
		}
		switch (resD)
		{
		case WAIT_OBJECT_0:
		{
			if (!GetOverlappedResult(m_hComm, &m_tWriteovReady, &BytesSent, false))
			{
				pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Write����GetOverlappedResult"), NULL);
				return false;
			}
				
			else
				return true;

		}
		default:
			pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Write����default"), NULL);
			return false;
			break;
		}
	}
	return true;
}

bool CSerialPortExt::Close()
{
	if (!m_bOpen) return false;	

	CloseHandle(m_tReadovReady.hEvent);
	CloseHandle(m_tWriteovReady.hEvent);
	m_tReadovReady.hEvent = NULL;
	m_tWriteovReady.hEvent = NULL;

	if (CloseHandle(m_hComm))
	{
		m_hComm = NULL;
		m_bOpen = false;
		return true;
	}
	else
	{
		m_hComm = NULL;
		return false;
	}
}

bool CSerialPortExt::Read(CString &strReceiveData, DWORD dwReceiveLength)
{
	if (!m_bOpen)    return false;
	if (m_tReadovReady.hEvent == NULL)    return false;

	char chRecv[2048];
	if (ReadFile(m_hComm, chRecv, 2000, &dwReceiveLength, &m_tReadovReady) == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)    return false;
	}
	if (dwReceiveLength == 0)    return false;
	chRecv[dwReceiveLength] = 0;

	DWORD dwRead;
	DWORD dwRes = WaitForSingleObject(m_tReadovReady.hEvent, READ_TIMEOUT);
	strReceiveData.Format("%s", chRecv);
	switch (dwRes)
	{
	case WAIT_OBJECT_0:
		if (!GetOverlappedResult(m_hComm, &m_tReadovReady, &dwRead, FALSE))    return false;
		break;

	case WAIT_TIMEOUT:
		break;

	default:
		break;
	}
	return true;
}

bool CSerialPortExt::Read(char * buf, DWORD dwReceiveLength)
{
	if (!m_bOpen)    return false;
	if (m_tReadovReady.hEvent == NULL)
	{
		/*pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Read����m_tReadovReady.hEvent"), NULL);*/
		return false;
	}

	char chRecv[2048];
	if (ReadFile(m_hComm, chRecv, 2000, &dwReceiveLength, &m_tReadovReady) == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
		//	pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Read����ERROR_IO_PENDING"), NULL);
			return false;
		}
			
	}
	if (dwReceiveLength == 0)
	{
		//pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Read����dwReceiveLength==0"), NULL);
		return false;
	}
	
	chRecv[dwReceiveLength] = 0;

	DWORD dwRead;
	DWORD dwRes = WaitForSingleObject(m_tReadovReady.hEvent, READ_TIMEOUT);
	for (int i = 0; i < dwReceiveLength; i++)
	{
		buf[i] = chRecv[i];
	}
	//strReceiveData.Format("%s", chRecv);
	switch (dwRes)
	{
	case WAIT_OBJECT_0:
		if (!GetOverlappedResult(m_hComm, &m_tReadovReady, &dwRead, FALSE))
		{
			//pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Read����WAIT_OBJECT_0"), NULL);
			return false;
		}
		break;

	case WAIT_TIMEOUT:
	{
		//pLogView->DoSomething("EXCEPTION", _T(" CSerialPortExt::Read����WAIT_TIMEOUT"), NULL);
	}
	break;

	default:
		break;
	}
	return true;
}

bool CSerialPortExt::GetComList(std::vector<CString> &vecList)
{
	HKEY hKey;
	vecList.clear();

	LONG nRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"Hardware\\DeviceMap\\SerialComm", NULL,
		KEY_READ, &hKey);
	if (nRetVal == ERROR_SUCCESS)
	{
		int i = 0;
		char portName[256], commName[256];// commName����������
		DWORD dwLong, dwSize;
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			nRetVal = RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize);
			if (nRetVal == ERROR_NO_MORE_ITEMS) // ö�ٴ���
				break;

			CString strCommName;
			strCommName.Format("%s", commName);
			vecList.push_back(strCommName);
			i++;
		}
		RegCloseKey(hKey);
	}

	if (vecList.size() == 0) return false;
	
	return true;
}

bool CSerialPortExt::GetLightBrightness(const int& nChannel, int &nBrightness)
{
	if (!m_bOpen) return false;
	if (nChannel < 1) return false;

	CString strCmd,strRes;
	strCmd.Format("S%c#", 'A' + (nChannel - 1));

	if (!Write(strCmd)) return false;
	Sleep(50);//��Ҫ����
	if (!Read(strRes)) return false;

	strRes.Delete(0,1);//b0001, ����ɾ��b���ٽ���ת��Ϊ����
	nBrightness = atoi(strRes);
	return true;
}

bool CSerialPortExt::SetLightBrightness(const int &nChannel, int nBrightness)
{
	if (!m_bOpen) return false;
	if (nChannel < 1) return false;
	if (nBrightness < 0) nBrightness = 0;
	if (nBrightness > 255) nBrightness = 255;

	CString strCmd, strRes;
	strCmd.Format("S%c%0.4d#", 'A' + (nChannel - 1), nBrightness);

	if (!Write(strCmd)) return false;
	Sleep(50);
	if (!Read(strRes)) return false;

	//�ж���û�����óɹ�
	if(strRes != _T('A' + (nChannel - 1))) return false;

	return true;
}

bool CSerialPortExt::SetLightStatus(bool bOn)
{
	if (!m_bOpen) return false;

	CString strCmd, strRes;
	if(bOn) strCmd.Format("SH#");//����
	else strCmd.Format("SL#");//����

	if (!Write(strCmd)) return false;
	Sleep(50);
	if (!Read(strRes)) return false;

	//�ж���û�����óɹ�
	if (-1 == strCmd.Find(strRes)) return false;

	return true;
}

uint16_t CSerialPortExt::crc16tablefast(uint8_t * ptr, uint16_t len)
{
	uint16_t crc = 0xffff;
	uint16_t i;
	uint8_t ch;

	for (i = 0; i < len; i++) {
		ch = *ptr++;
		crc = crctalbeabs[(ch ^ crc) & 15] ^ (crc >> 4);
		crc = crctalbeabs[((ch >> 4) ^ crc) & 15] ^ (crc >> 4);
	}
	////���
	//uint8_t hcrc = (crc << 8) >> 8;
	//uint8_t lcrc = crc >> 8;
	//printf(" H value : 0x%x\n", hcrc);
	//printf(" L value : 0x%x\n", lcrc);
	return crc;
}


BOOL CSerialPortExt::ReadDabiaoNum(unsigned char id,int& nNum)
{


	unsigned char data[32];
	data[0] = id;
	data[1] = 0x04;
	data[2] = 0x00;
	data[3] = 0x0E;
	data[4] = 0x00;
	data[5] = 0x01;
	//

	//1 50 09
	//	2 50 3A
	//3 51 EB
	//4 50 5C
	//5 51 8D
	//6 51 BE
	//7 50 6F
	//8 50 90
	//	9 51 41
	//	10 51 72
	//	11 50 A3
	//12 51 14

	switch (id)
	{
	case 1:
		data[6] = 0x50;
		data[7] = 0x09;
		break;
	case 2:
		data[6] = 0x50;
		data[7] = 0x3A;
		break;
	case 3:
		data[6] = 0x51;
		data[7] = 0xEB;
		break;
	case 4:
		data[6] = 0x50;
		data[7] = 0x5C;
		break;
	case 5:
		data[6] = 0x51;
		data[7] = 0x8D;
		break;
	case 6:
		data[6] = 0x51;
		data[7] = 0xBE;
		break;
	case 7:
		data[6] = 0x50;
		data[7] = 0x6F;
		break;
	case 8:
		data[6] = 0x50;
		data[7] = 0x90;
		break;
	case 9:
		data[6] = 0x51;
		data[7] = 0x41;
		break;
	case 10:
		data[6] = 0x51;
		data[7] = 0x72;
		break;
	case 11:
		data[6] = 0x50;
		data[7] = 0xA3;
		break;
	default:
		data[6] = 0x51;
		data[7] = 0x14;
		break;
;
	}

	/*uint16_t crc = crc16tablefast(data, 6);
	data[6] = (crc << 8) >> 8;
	data[7] = crc >> 8;*/

	//CString sendm;
	//sendm.Format(_T("��־%d����%x %x %x %x %x %x %x %x"),
	//	m_bOpen,data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)sendm, NULL);
	////AfxMessageBox(sendm);

	//if (!Write((char*)data, 8)) return 0;
	//Sleep(50);
	//if (!Read((char*)data, 7)) return 0;

	//sendm.Format(_T("��־%d����%x %x %x %x %x %x %x %x"),
	//	m_bOpen, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//AfxMessageBox(sendm);
	//pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)sendm, NULL);

	//nNum = data[3]*256+ data[4];
	return TRUE;


}

BOOL CSerialPortExt::ResetDabiao(unsigned char id)
{
	//if (!m_bOpen) return false;
	//0x01 0x05
	unsigned char data[32];
	data[0] = id;
	data[1] = 0x05;
	data[2] = 0x00;
	data[3] = 0x02;
	data[4] = 0xFF;
	data[5] = 0x00;
	uint16_t crc = crc16tablefast(data, 6);
	data[6] = (crc << 8) >> 8;
	data[7] = crc >> 8;
	//CString sendm;
	//sendm.Format(_T("����%x %x %x %x %x %x %x %x"),
	//	data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//AfxMessageBox(sendm);
	if (!Write((char*)data,8)) return false;
	Sleep(30);
	//data[7] = 0x00;
	if (!Read((char*)data,8)) return false;

	//sendm.Format(_T("����%x %x %x %x %x %x %x %x"),
	//	data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//AfxMessageBox(sendm);
	//�ж���û�����óɹ�
	//
	return true;
}

int CSerialPortExt::CheckDbState(unsigned char id,CString& StrWarnInfo)
{
	//if (!m_bOpen) return 0;
	unsigned char data[32];
	data[0] = id;
	data[1] = 0x04;
	data[2] = 0x00;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x01;
	uint16_t crc = crc16tablefast(data, 6);
	data[6] = (crc << 8) >> 8;
	data[7] = crc >> 8;
	//CString sendm;
	//sendm.Format(_T("����%x %x %x %x %x %x %x %x"),
	//	data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//pLogView->DoSomething("LOG", (LPSTR)(LPCSTR)sendm, NULL);
	if (!Write((char*)data,8)) return 0;
	Sleep(50);
	if (!Read((char*)data, 7)) return 0;
	//sendm.Format(_T("����%x %x %x %x %x %x %x %x"),
	//	data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
	//pLogView->DoSomething("LOG", (LPSTR)(LPCSTR)sendm, NULL);
	//AfxMessageBox(sendm);
	//�ж���û�б���״̬
	if (0x00 != data[4]) 
	{
		int nErrorIndex=data[4];
		StrWarnInfo=STR_LabelWarnInfo[MIN(16,nErrorIndex)];
		return -1;
	}

	return 1;
}

