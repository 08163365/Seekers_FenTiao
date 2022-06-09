//////////////////////////////////////////////////////////////////////////
////SerialPortExt.h 串口通信类 SW 2020.10.11
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
	//打开串口 nBaudRate波特率 nBitCount数据位 nParity校验位 nStopBits停止位
	bool Open(CString strPortName, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);
	bool Open(int nComSN, int nBaudRate = 9600, int nBitCount = 8, int nParity = 0, int nStopBits = 0);

	//关闭串口
	bool Close();

	//读取串口信息
	bool Read(CString &strReceiveData, DWORD dwReceiveLength = 0);
	bool Read(char* buf, DWORD dwReceiveLength);
	//写入串口信息
	bool Write(const CString &strSendData);
	bool Write(char* buf,int len);

	//获取设备串口列表
	bool GetComList(std::vector<CString> &vecComList);

	//获取光源亮度(光源控制器)
	bool GetLightBrightness(const int &nChannel, int &nBrightness);

	//设置光源亮度(光源控制器),亮度范围0~255
	bool SetLightBrightness(const int &nChannel, int nBrightness);

	//设置光源常亮状态(光源控制器) 常亮或常灭
	bool SetLightStatus(bool bOn = true);
	BOOL ReadDabiaoNum(unsigned char id, int& nNum);//读取出标数目
	bool IsOpen() { return m_bOpen; }
public:
	//打标机
	//计算CRC16
	uint16_t crc16tablefast(uint8_t *ptr, uint16_t len);
	//1、报警复位
	BOOL ResetDabiao(unsigned char  id);
	//查看打标机是否已报警
	int CheckDbState(unsigned char  id,CString& StrWarnInfo);
private:
	OVERLAPPED m_tReadovReady, m_tWriteovReady;
	HANDLE m_hComm;
public:
	bool m_bOpen;
public:
	//新增报警
	BOOL DbState;
	int NgCount;
	DHInterface* pLogView;
};

