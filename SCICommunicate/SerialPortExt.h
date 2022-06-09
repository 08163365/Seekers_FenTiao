//////////////////////////////////////////////////////////////////////////
////SerialPortExt.h ����ͨ���� SW 2020.10.11
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <vector>
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
const uint16_t crctalbeabs[] = {
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
};
class CSerialPortExt
{
public:
#define READ_TIMEOUT  100

public:
	CSerialPortExt();
	CSerialPortExt(CString strPortName, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);
	CSerialPortExt(int nComSN, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);

	~CSerialPortExt();

public:
	//�򿪴��� nBaudRate������ nBitCount����λ nParityУ��λ nStopBitsֹͣλ
	bool Open(CString strPortName, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);
	bool Open(int nComSN, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);

	//�رմ���
	bool Close();

	//��ȡ������Ϣ
	bool Read(CString &strReceiveData, DWORD dwReceiveLength = 0);
	bool Read(char* buf, DWORD dwReceiveLength);
	//д�봮����Ϣ
	bool Write(const CString &strSendData);
	bool Write(char* buf,int len);

	//��ȡ�豸�����б�
	bool GetComList(std::vector<CString> &vecComList);

	//��ȡ��Դ����(��Դ������)
	bool GetLightBrightness(const int &nChannel, int &nBrightness);

	//���ù�Դ����(��Դ������),���ȷ�Χ0~255
	bool SetLightBrightness(const int &nChannel, int nBrightness);

	//���ù�Դ����״̬(��Դ������) ��������
	bool SetLightStatus(bool bOn = true);
	BOOL ReadDabiaoNum(unsigned char id, int& nNum);//��ȡ������Ŀ
	bool IsOpen() { return m_bOpen; }
public:
	//����
	//����CRC16
	uint16_t crc16tablefast(uint8_t *ptr, uint16_t len);
	//1��������λ
	BOOL ResetDabiao(unsigned char  id);
	//�鿴�����Ƿ��ѱ���
	int CheckDbState(unsigned char  id,CString& StrWarnInfo);
private:
	OVERLAPPED m_tReadovReady, m_tWriteovReady;
	HANDLE m_hComm;
public:
	bool m_bOpen;
public:
	//��������
	BOOL DbState;
	int NgCount;
	DHInterface* pLogView;
};

