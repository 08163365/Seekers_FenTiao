#pragma once
#include "DHInterFace.h"
#include "SerialPortExt.h"
#include "LightControl.in.h"
#include "TcpComm.h"
void T_LightConnect();
class CLightSerial : DHInterface
{
public:
	typedef struct tagTParam
	{
		eLightCommType  eCommType;   //通讯类型
		eLightControlType	eLightType; //光源控制器类型
		CString				sComName;   //串口号
		int					nComIndex;  //串口下拉下标
		CString sIP;						//IP号
		int nTcpPort;                    //网络端口

		BOOL bUseLightAdjust;//是否启用光源自适应
		int  nSampleFrames;//闭环取样帧数
		int  nGoalGray;//目标灰度
		int  nGraySafeDev;//可以不进行光源自适应的范围
		int  nLightUp;//光源亮度上限
		int  nLightDown;//光源亮度下限
	};
public:
	CLightSerial(void);
	~CLightSerial(void);
	virtual int DoSomething(void* message,void* wParam,void* lParam);

	int Init(LIGHTINIT &Init);

	BOOL IsOpen() { return m_SerialPort.IsOpen(); }
	BOOL Open(); //打开光源控制器串口
	BOOL Close(); //关闭光源控制器串口
	void OpenLight();//开启光源
	void CloseLight();//关闭光源
	BOOL AdaptLightBright(int nGrayValue, int nChannelIndex);
	BOOL SetBrightness(const int& nChannel, const int& nBrightness);//设置光源控制器亮度
	int  GetBrightnes(const int& nChannel);//获取光源控制器的亮度
	BOOL Log(const CString& strLog,  int nLevel = 0);
	BOOL SaveParams();//保存参数接口
	BOOL ReadParams();//获取参数接口

	CLightSerial::tagTParam& GetLightParam() {return m_tLightParam; }
	
private:
	BOOL CheckChannel(const int& nChannel);
private:
	BOOL ShowParamDlg(BOOL bEnable);
	LIGHTINIT m_sInit;
	CString m_strConfigLightSerialFile;
	CString m_strConfigLightSerialFilePath;
	tagTParam m_tLightParam;
	CSerialPortExt m_SerialPort;	
	CTcpComm m_TcpComm;

public:
	int	 m_nCurrentStatus;//当前状态
	CString CurLightType;
	CString m_strCurStatus;//当前状态
	HTuple m_hvRealGrayValue[2];//图像实时灰度值
};

extern CLightSerial* g_This;

