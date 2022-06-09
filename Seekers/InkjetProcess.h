#pragma once

const CString g_strCameraName[]=
{
	_T("cam_a"),
	_T("cam_a"),
	_T("cam_b"),
	_T("cam_c"),
	_T("cam_d"),
};
///时间	切片ID	编号关联	相机	速度	计长	高度	分区1	分区2	分区3	分区4	分区5	分区6	分区7	分区8

struct InkjetInfo//喷墨机信息
{
	HTuple hv_EAID;
	HTuple hv_Trip;
	HTuple hv_Speed;
	HTuple hv_Yield;
	HTuple hv_NGImageSN;//写入贴标的图像号
	HTuple hv_FirstStrName;//第一优先级的名称
	HTuple hv_ErrorName;//错误名称
	HTuple hv_ErrorNum;//错误数目
	HTuple hv_TripWidth;//分条宽度
	HTuple hv_TotalLength;//总长
	HTuple hv_LTRow;
	HTuple hv_LTColumn;
	HTuple hv_RBRow;
	HTuple hv_RBColumn;

};
////该类是为了上传信息给喷码机贴标用的 
class CInkjetProcess
{
public:
	CInkjetProcess(void);
	~CInkjetProcess(void);

public:
	void Init(int nSide);//初始化
	void Action(C_ALLOCATORWORKSHOPOUTPUT& OutPut);//执行获取喷码信息
	void AppendQueue(InkjetInfo& tInjectInfo);//将喷码信息入队
	void PopQueue(int nSide);//喷码信息出队
	void WriteInjeckInfo(InkjetInfo& tInjectInfo,CString& strValue,int& nNumCount);//记录日志
	void GetSendInfo(InkjetInfo& tInkjetInfo,int nGongWeiNum,CString& StrSendValue);//获取发送给喷码机的字符串
	void SendToInjectInfo(CString& StrQuexinValue,CString& StrPenMaValue,int nSN);//将字符串发送给服务器
	int  GetSN(int nTrip);//获取工位号

	void  ReadRealOutPortCount();//读取输出口计数
	void  WriteOutIOInfo(CString& StrEAID,CString& strValue);//记录IO输出信息
public:
	int  GetOutCount();//获取当前OUT计数
	int  GetOutChaneNum();//获取OUT变化的数目
	int  ReadRealOutImageSN();//获取出队的图像号
	BOOL IsFreshImageSN();//判断图像号是否更新成功
	int  GetVecSize();//获取队列长度
	void  WriteOutIOChangeOnly(CString& strValue);//输出断开变化
private:
	int m_nOutCount;//输出计数
	int m_nLastOutCout;
	int m_nImageSN;
	int m_nLastImageSN;
	int m_nVecSize;//

public:
	int m_nSide;//索引
	CRITICAL_SECTION m_csChange;//数据锁
	std::vector<InkjetInfo>    m_vectorResultData;//数据缓存队列

};

