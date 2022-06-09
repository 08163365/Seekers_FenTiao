#pragma once
#include "PIO24BLib.h"
#include "SCICommunicate.in.h"
#include "SerialPortExt.h"
#include <vector>
const int MAX_LANE = 2;
const int DEF_LASTLABLEPOS = -99999;//默认情况下的标签位置


const CString STR_LabelWarnInfo[]
=
{
	"OK",
	"缺标报警",
	"无标报警,请换卷",
	"断带报警,断带",
	"走带报警,走带异常",
	"漏贴标报警",
	"余量10报警",
	"余量停机报警",
	"贴标误差报警",
	"输入异常报警",
	"电压低报警",
	"电压高报警",
	"贴标位置报警",
	"定位光纤失败报警",
	"编码器异常报警",
	"补标失败报警",
	"未知报警",
};
typedef struct tagDeviceInfo
{
	//设备友好名称,很友好……
	CString szDeviceName;
	//设备类
	CString szDeviceClass;
	//设备显示名
	CString szDeviceDesc;
	//设备驱动
	CString szDriverName;
	//设备实例
	DWORD dwDevIns;
	//设备类标志
	GUID Guid;
	//按类名排序
	bool operator < (const tagDeviceInfo &tmp) const
	{
		if (tmp.szDeviceClass != szDeviceClass)
		{
			return tmp.szDeviceClass.CompareNoCase (szDeviceClass) > 0;
		}
		else
		{
			return tmp.szDeviceDesc.CompareNoCase (szDeviceDesc) > 0;
		}
	}
}DeviceInfo;
void  ThreadIOListen(PVOID * pParam);

void ThreadListenLabelCounter(PVOID * pParam);
void SetDataTranslate(CString&strFilePath, double& dData, BOOL bWriteFlag);
extern BOOL g_bExitThread;
class CCommunicate : DHInterface
{
public:
	CCommunicate(void);
	virtual ~CCommunicate(void);
	virtual int DoSomething(void* message,void* wParam,void* lParam);
	BOOL       GetDeviceList(LPGUID lpGuid,BOOL bsFlag);//获取设备列表
	BOOL       SetDeviceStatus(DeviceInfo &theDevice,BOOL bStatusFlag);//设置设备状态
	void       ExceptionRemain(HTuple& hv_DiffValue,CString  strName);//启动时校验一下距离算出的行数和设置的行数是否相等 报警提示
	void       RefreshDelayInfo();//防呆校验
	void       BaoJingFuWei();//贴标机报警复位
public:
	double m_dMarkerLengthMM;//打标机标的长度（mm）
	int m_nMarkerLength;//打标机标的长度（行）,0 相机0 ,1 相机1
	CRITICAL_SECTION m_csCommunicateChange;//当参数发生更改的时候，需要锁住，防止出现系统那边正在检查，这边正好去更改参数
	CString		m_strProductName;
	CDialogEx* m_pDlg;//主界面指针
	CString strProductName;
	DHInterface* m_pLogView;
	CString				m_strConfigCommonFile;
	CString				m_strConfigSystemFile;
	CString				m_strConfigCommunicateFile;
	CString				m_strConfigCommunVarietyFile;
	CString				m_strConfigVarietyParmaFile;
	CString				m_strConfigSizeParmaFile;
	COMMUICATEPARAM		m_Commuicate;//踢废相关参数
	LabelControlInfo    m_tLabelControlInfo;//贴标信息控制
	ALARMS				m_Alarms;//报警相关参数
	COMMONPARAM			m_Common;//公共参数
	CPIO24BLib          m_IOCard[PIO24_COUNT];//大恒图像IO卡
	int                 m_nLabelPos[MAX_LANE];//贴完标签以后的在图片中的位置
	void WriteLabelAction(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);
	void InitThread();
	void ReadParam();
	void WriteInfo();
	void ComPareChange(int LastValue,int CurrentValue,CString strName);
	void ComPareChange(double LastValue,double CurrentValue,CString strName);
	int m_nProductType;
	int m_nGrabberDivideNum;//采集分频数目
    HWND m_OutputhWnd;
    CString m_strBatch;
    CString m_strSavePath;
	CString m_StrLocationLog;
	COMMUNICATEINIT m_sInit;
	HTuple m_hvGongWeiName;
	HTuple m_hvDaiLu;//上下带路
	int    m_nMarkCountNum[2];//Mark计数
	void  MakeMachineStop();
private:
	HTuple m_hvQueue;//队列句柄
private:
	static void* _Thread(void *hcthread);//io线程
	void Loop();
	
	int Init(PCOMMUNICATEINIT pInit);//初始化
	int PushMessage(const HTuple& hvMessageSend);//放置消息进入队列
	BOOL ReadyOutput(CString & strParam);//Ready信号
public:
	double m_dStandardEALength[2];
	int    m_nTripNum;
	HTuple m_hvRemainRow;//存储缺陷的距离
//	CSerialPortExt m_Com;       //打标机串口
	
	BOOL  m_bWarnFlag;

	HTuple m_hvRealMonitorLengthTuple;
	HTuple m_hvRealLabelCountTuple;
public:
	//串口相关的
	
	BOOL m_bOpen;
	BOOL m_bState;

	CRITICAL_SECTION   m_csSerialCOMUse;

	BOOL m_bCounterFlag;
};

