////////////////////////////////
////////////网口通信类
////////////////////////////////
#pragma once
#include "StdAfx.h"
#include <vector>
#include "HalconCpp.h"
using namespace HalconCpp;
class CTcpComm
{
public:
#define READ_TIMEOUT  101
public:
	CTcpComm();
	~CTcpComm();
public :
	//打开网口,设置IP，端口号
	bool Open(CString IP, int Port);
	//关闭网口
	bool Close();
	//连接设备
	bool Connect(CString IP, int Port);
	//获取光源亮度（光源控制器）
	bool GetLightBrightness(const int &Channel,int &nBrightness);
	//设置光源亮度（光源控制器），亮度范文0~255
	bool SetLightBrightness(const int &nChannel, int nBrightness);
	//读取数据
	bool CTcpComm::Read(CString  &strReceiveData);
	//写入数据
	bool CTcpComm::Write(const CString &strSendData);

	bool IsOpen() {return m_bOpen;}
	//设置光源常量状态（光源控制器）常量或常灭
	bool CTcpComm::SetLightStatus(bool bOn);
private:
	OVERLAPPED m_tReadovReady, m_tWriteovReady;
	HANDLE m_tcpComm;
	bool m_bOpen;

	HTuple hv_Protocol;
	HTuple hv_Timeout;
	HTuple hv_Socket ;
	HTuple hv_Address;
	HTuple hv_To;
	HTuple hv_Answer;
	HTuple hv_From;
	HTuple hv_Data;
	};

