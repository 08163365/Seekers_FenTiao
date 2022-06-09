#pragma once
#include "Define.h"
#include "FinsTCPExt.h"
#include <queue>
#include "SCILabeller.in.h"

using namespace std;
extern BOOL g_bExitThread;
enum emLightColor
{
	GreenLight,
	YellowLight,
	RedLight,


};
enum LabelIndex
{
	InsideLabel=1,
	OutsideLabel,
};
void ThreadListenLabelLength(PVOID * pParam);//监听距离
class CLabeller : DHInterface
{
public:
	CLabeller(void);
	~CLabeller(void);
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam); // 将调用方需要调用的成员函数声明成纯虚函数
	int      Init(PLABELLERINIT pInit);//初始化
	static   void* _Thread(void *hcthread);//io线程
	void     Loop();
	LABELLERINIT m_tInit;
	BOOL  IsUseLabel(int nLabelIndex);
     CRITICAL_SECTION m_csChange;
public:
	void  WriteExceptLog(HTuple& hvFlag,HTuple& hv_Names);
	void  GetLabelControl(HTuple& hv_Type,HTuple&hv_Data);//获取开关参数
	void  GetLabelParam(HTuple& hv_Type,HTuple&hv_Data);//获取参数
	void SetLabellerStaus(CString& strInfo);
	void  LabelReset();//复位
	void  ReadParam();//读取参数
	void  WriteParam();//保存参数
	void  ClearCount();//换卷计数清零
	void  GetLabelStatus(HTuple*hv_EachFalg,HTuple* hv_WarnMessage);
	void  GetLabelCount(HTuple* hv_Name,HTuple* hv_Count);//计数
	int   GetLabelActionNum(int nLabelSN);//获取贴标数目
	int   GetLabelReieveNum(int nLabelSN);//获取收到信号数目
	void  WriteLabelLog(CString strMessage,BOOL bExceptFlag=TRUE);//写入运行日志
	int   GetLabelNum(int nLabelSN,int nType);//获取计数


	void  XiangJiChuChen();//相机除尘
	void  BaoJingFuWei();//报警复位
	void  ShangQiGangTaiQi();//上带路气缸抬起
	void  ShangQiGangYaXia();//上带路气缸压下
	void  XiaQiGangTaiQi();//下路气缸抬起
	void  XiaQiGangYaXia();//下带路气缸压下
	void  BianMaQiGangTaiQi();//编码器辊子气缸抬起
	void  BianMaQiGangYaXia();//编码器辊气缸压下
	void  SetLightColor(emLightColor LightColor);//设置报警灯的颜色





	CFinsTcpExt m_CFinsTcp;//通讯类
	CString m_strPath;
	LabellerInfo m_tLabellerInfo;//贴标机交互
	CString m_StrLabellerStatus;
	BOOL m_bScreenFlag;
	HTuple m_hvStatusFlag,m_hvWarnNames;
	HTuple m_hvCount,m_hvCountNames;//计数
	HTuple m_hvControl,m_hvControlName;//开关
	HTuple m_hvSystemParam,m_hvSystemParamName;//参数

	BOOL  m_bWarnStaus;
	double m_dRealSpeed;

	HTuple  m_hvLabelRealLocation;//真实的贴标位置
	CString m_strDirectory;

	double m_dStandardLocation; 
	HTuple m_hvLabelLocationOffsetTuple;
	double m_dLabelLocationOffsetDev;

	CString m_strBatch;
};

