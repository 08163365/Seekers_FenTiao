#include "StdAfx.h"
#include "DlgSet.h"
#include "LightSerial.h"



extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CLightSerial();
}

CLightSerial* g_This = NULL;
void T_LightConnect()
{
	while(1)
	{

		if (g_This->m_nCurrentStatus==FALSE)
		{
			g_This->Open();
			if (g_This->m_nCurrentStatus)
			{
				g_This->m_strCurStatus="光源连接成功";
			}else
			{
				g_This->m_strCurStatus="光源未连接";
			}
		}
		Sleep(1000);
	}
}

CLightSerial::CLightSerial(void)
{ 
	m_nCurrentStatus = 0;
	g_This = this;
}

CLightSerial::~CLightSerial(void)
{

}

int CLightSerial:: Init(LIGHTINIT &Init)
{
	m_sInit.strProductName = Init.strProductName;//配方
	m_sInit.nProductType = Init.nProductType;
	m_sInit.Log = Init.Log;
	m_strConfigLightSerialFilePath.Format("%s\\Config\\Public\\LightControl", GetCurrentAppPath());
	m_strConfigLightSerialFile.Format("%s\\ComSetting.ini", m_strConfigLightSerialFilePath);
	if (!PathFileExists(m_strConfigLightSerialFilePath))
		CreateMultilevelPath(m_strConfigLightSerialFilePath);

	ReadParams();	

	AfxBeginThread((AFX_THREADPROC)T_LightConnect, NULL);//将监听的端口打开
	
	return TRUE;
}

BOOL CLightSerial::Log(const CString& strLog,int nLevel)
{
	
	if (m_sInit.Log == NULL) return FALSE;
	return m_sInit.Log->DoSomething("OPERATERLOG", (void*)(LPCSTR)strLog, NULL);
	return TRUE/*m_sInit.pLog(strLog, nType, nExcpLevel)*/;
}
BOOL CLightSerial::AdaptLightBright(int nGrayValue, int nChannelIndex)
{
	if (m_tLightParam.bUseLightAdjust== FALSE)
	{
		return FALSE;
	}

	static int s_nContinueNum[2] = { 0,0 };
	m_hvRealGrayValue[nChannelIndex].Append(nGrayValue);
	
	if (m_hvRealGrayValue[nChannelIndex].Length()>=m_tLightParam.nSampleFrames)
	{
		int nMean = m_hvRealGrayValue[nChannelIndex].TupleMean().D();

		int nBrightness = 100;
		HTuple hv_Diff = m_hvRealGrayValue[nChannelIndex].TupleMean() - m_tLightParam.nGoalGray;
		nBrightness = GetBrightnes(nChannelIndex + 1);
		if (hv_Diff>m_tLightParam.nGraySafeDev)//大于
		{
			if (nBrightness>50 && nBrightness<255)
			{
				if (hv_Diff>m_tLightParam.nGraySafeDev + 10)
				{
					m_nCurrentStatus = SetBrightness(nChannelIndex + 1, MAX(m_tLightParam.nLightDown, nBrightness - 10));
				}
				else
				{
					m_nCurrentStatus = SetBrightness(nChannelIndex + 1, MAX(m_tLightParam.nLightDown, nBrightness - 5));
				}
				///测试日志
				{
					CString strException;
					strException.Format("执行光源自适应调暗次数%d,光源亮度%d 目标灰度%d 当前灰度%d ", ++s_nContinueNum[nChannelIndex], nBrightness, m_tLightParam.nGoalGray, nMean);
					Log(strException);
				}
			}
			
		}
		else if (hv_Diff<-m_tLightParam.nGraySafeDev)
		{


			if (nBrightness > 50 && nBrightness < 255)
			{
				if (hv_Diff<-m_tLightParam.nGraySafeDev - 10)
				{
					m_nCurrentStatus = SetBrightness(nChannelIndex + 1, MIN(m_tLightParam.nLightUp, nBrightness + 10));
				}
				else
				{
					m_nCurrentStatus = SetBrightness(nChannelIndex + 1, MIN(m_tLightParam.nLightUp, nBrightness + 5));
				}
				///测试日志
				{
					CString strException;
					strException.Format("执行光源自适应调亮次数%d,光源亮度%d 目标灰度%d 当前灰度%d ", ++s_nContinueNum[nChannelIndex], nBrightness, m_tLightParam.nGoalGray, nMean);
					Log(strException);
				}
			}
		
		}
		else
		{
			///测试日志
			{
				CString strException;
				strException.Format("执行光源自适应调成功次数%d,通道%d,亮度%d", s_nContinueNum[nChannelIndex], nChannelIndex, m_nCurrentStatus);
				Log(strException);
			}
			s_nContinueNum[nChannelIndex] = 0;
		
		}
		m_hvRealGrayValue[nChannelIndex].Clear();
	}


	return TRUE;
}

int CLightSerial::DoSomething(void* message,void* wParam,void* lParam)
{
	CString strMessage;
	strMessage.Format("%s",(char*)message);

	if ("INITIALIZE" == strMessage || "Initialize" == strMessage)
	{
		return Init(*(PLIGHTINIT)wParam);
	}
	else if ("OPEN" == strMessage || "Open" == strMessage)//关闭光源
	{
		
	}
	else if("CLOSE" == strMessage || "Close" == strMessage)//打开光源
	{

	}
	else if("SHOW" == strMessage || "Show" == strMessage)
	{
		return ShowParamDlg(*(BOOL*)lParam);
	}
	else if("SETBRIGHTNESS" == strMessage || "SetBrightness" == strMessage)
	{


	}
	else if ("ADAPTLIGHTBRIGHT" == strMessage || "AdaptLightBright" == strMessage)
	{
		int nCurrentGrayValue = *(int*)(wParam);
		int nLightChannelIndex = *(int*)(lParam);
		AdaptLightBright(nCurrentGrayValue, nLightChannelIndex);
	}
	else if("SETCHANNEL" == strMessage || "SetChannel" == strMessage)
	{

	}else if ("OPENLIGHT" == strMessage || "OpenLight" == strMessage)//打开光源
	{
		OpenLight();
		return TRUE;
	}
	else if ("CLOSELIGHT" == strMessage || "CloseLight" == strMessage)//关闭光源
	{
		 CloseLight();
		 return TRUE;
	}
	else if ("GETCONNECTION" == strMessage || "GetConnection" == strMessage)
	{
		CString strLightStatus;
		strLightStatus=m_strCurStatus;
		_tcscpy((char*)lParam, strLightStatus); 
		return TRUE;
	}
	return TRUE;
}
void CLightSerial::OpenLight()//开启光源
{
	
	if (m_tLightParam.eCommType == CT_SerialPort)
	{
		m_nCurrentStatus = m_SerialPort.SetLightStatus(true);
	}
	else 
	{
		m_nCurrentStatus =  m_TcpComm.SetLightStatus(true);
	}

}
void CLightSerial::CloseLight()//关闭光源
{
	if (m_tLightParam.eCommType == CT_SerialPort)
	{
		m_nCurrentStatus = m_SerialPort.SetLightStatus(false);
	}
	else 
	{
		m_nCurrentStatus = m_TcpComm.SetLightStatus(false);
	}
}
BOOL CLightSerial::ShowParamDlg(BOOL bEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgSet dlg;
	//CDlgIOSetting dlg;
	dlg.m_pOwner = this;
	dlg.m_bEnable = bEnable;
	dlg.DoModal();
	return TRUE;
}

BOOL CLightSerial::SaveParams()//保存参数接口
{
	try
	{
		//获取选中通道
		WritePrivateProfileInt("COM", "CommType", m_tLightParam.eCommType, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "LightType", m_tLightParam.eLightType, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "ComIndex", m_tLightParam.nComIndex, m_strConfigLightSerialFile);
		WritePrivateProfileString("COM", "ComName", m_tLightParam.sComName, m_strConfigLightSerialFile);
		WritePrivateProfileString("COM", "IP", m_tLightParam.sIP, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "Port", m_tLightParam.nTcpPort, m_strConfigLightSerialFile);

		WritePrivateProfileInt("COM", "bUseLightAdjust", m_tLightParam.bUseLightAdjust, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "nSampleFrames", m_tLightParam.nSampleFrames, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "nGoalGray", m_tLightParam.nGoalGray, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "nGraySafeDev", m_tLightParam.nGraySafeDev, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "nLightUp", m_tLightParam.nLightUp, m_strConfigLightSerialFile);
		WritePrivateProfileInt("COM", "nLightDown", m_tLightParam.nLightDown, m_strConfigLightSerialFile);

		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}
}

BOOL CLightSerial::ReadParams()//获取参数接口
{
	try
	{
		m_tLightParam.eCommType = (eLightCommType)GetPrivateProfileInt("COM", "CommType", CT_Tcp, m_strConfigLightSerialFile);
		m_tLightParam.eLightType = (eLightControlType)GetPrivateProfileInt("COM", "LightType", 0, m_strConfigLightSerialFile);
		m_tLightParam.nComIndex = GetPrivateProfileInt("COM", "ComIndex", 0, m_strConfigLightSerialFile);
		GetPrivateProfileString("COM", "ComName", "Com1", m_tLightParam.sComName.GetBuffer(MAX_PATH), MAX_PATH, m_strConfigLightSerialFile);
		m_tLightParam.sComName.ReleaseBuffer();//读取产品名称
		//CST的IP默认是192.168.1.208 
		GetPrivateProfileStringA("COM", "IP", "192.168.1.208", m_tLightParam.sIP.GetBuffer(MAX_PATH), MAX_PATH, m_strConfigLightSerialFile);
		//端口不可更改 6600
		m_tLightParam.nTcpPort = GetPrivateProfileInt("COM", "Port", 6600, m_strConfigLightSerialFile);

		m_tLightParam.bUseLightAdjust = GetPrivateProfileInt("COM", "bUseLightAdjust", 0, m_strConfigLightSerialFile);
		m_tLightParam.nSampleFrames = GetPrivateProfileInt("COM", "nSampleFrames", 3, m_strConfigLightSerialFile);
        m_tLightParam.nGoalGray = GetPrivateProfileInt("COM", "nGoalGray", 90, m_strConfigLightSerialFile);
		m_tLightParam.nGraySafeDev = GetPrivateProfileInt("COM", "nGraySafeDev", 5, m_strConfigLightSerialFile);
		m_tLightParam.nLightUp = GetPrivateProfileInt("COM", "nLightUp", 200, m_strConfigLightSerialFile);
		m_tLightParam.nLightDown = GetPrivateProfileInt("COM", "nLightDown", 100, m_strConfigLightSerialFile);


		return TRUE;

	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}
}

BOOL CLightSerial::Open()
{
	try 
	{
		if (m_tLightParam.eCommType == CT_SerialPort)
		{
			int nBultrate = 19200;
			if (m_tLightParam.eLightType == CST_CPL48320CDP_2TD)
				nBultrate = 38400;
			if (m_SerialPort.Open(m_tLightParam.sComName, nBultrate, 8, 0, 1))
			{
				m_nCurrentStatus = true;
				return true;
			}
			else
			{
				m_nCurrentStatus = false;
				return true;
			}
		}
		else
		{
			//tcp
			if (m_tLightParam.eCommType == CT_Tcp)
			{
				if (m_TcpComm.Open(m_tLightParam.sIP, m_tLightParam.nTcpPort))
				{
					m_nCurrentStatus = true;
					return true;
				}
				else
				{
					m_nCurrentStatus = false;
					return false;
				}
			}
		}
	}
	catch (HException&e)
	{
		CString strLog;
		strLog.Format("%s%s",e.ErrorMessage().Text(),e.ProcName().Text());
		return false;
	}
	
}

BOOL CLightSerial::Close()
{
	if (m_tLightParam.eCommType == CT_SerialPort) 
	{
		if (m_SerialPort.Close())
	{
		m_nCurrentStatus = false;
		return true;
	}
	else
	{
		return false;
	}
	}
	else if (m_tLightParam.eCommType == CT_Tcp)
	{
		if (m_TcpComm.Close())
		{
			m_nCurrentStatus = false;
			return true;
		}
		else
		{
			return false;
		}
	}

}

BOOL CLightSerial::SetBrightness(const int& nChannel, const int& nBrightness)
{
	if (!CheckChannel(nChannel)) return false;
	if (m_tLightParam.eCommType == CT_SerialPort)
	{
		return m_SerialPort.SetLightBrightness(nChannel, nBrightness);
	}
	else 
	{
		return m_TcpComm.SetLightBrightness(nChannel, nBrightness);
	}
	return true;
}

int CLightSerial::GetBrightnes(const int& nChannel)
{
	int nBt = 0;
	if (!CheckChannel(nChannel)) return -1;
	if (m_tLightParam.eCommType == CT_SerialPort)
	{
	if (!m_SerialPort.GetLightBrightness(nChannel, nBt))
	{
		//日志
		CString logMsg;
		logMsg.Format("获取通道%d的亮度值异常:%d", nChannel,nBt);
		//Log(logMsg, ExcpLog);
		return -1;
	}	
	}
	else 
	{
		if (!m_TcpComm.GetLightBrightness(nChannel, nBt))
		{
			//日志
			CString logMsg;
			logMsg.Format("获取通道%d的亮度值异常:%d", nChannel, nBt);
			//Log(logMsg, ExcpLog);
			return -1;
		}
	}
	
	return nBt;
}

BOOL CLightSerial::CheckChannel(const int& nChannel)
{
	switch (m_tLightParam.eLightType)
	{
		case DH_HAPS_48W800_1T:
			return 1 >= nChannel;

		case DH_HAPS_48W800_2T:
			return 2 >= nChannel;

		case CST_CPL48320CDP_2TD:
			return 2 >= nChannel;
         
		default:

			return false;
	}
}