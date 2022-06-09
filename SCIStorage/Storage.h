#pragma once
#include "TransferStatus.h"
#include "Define.h"
#include "SCIStorage.in.h"
//extern HTupleVector  g_hvec_GroupDecriptInfo;  //整组产品的描述信息
extern HTupleVector  g_hvec_DecriptInfoSN;  //整组产品的极耳序号
//extern HObjectVector g_hovec_GroupImageInfo;//单组产品的信息
enum e_CoatType
{
	M_TwoCam_Continue,//连续涂布
	M_TwoCam_Intermission,//间歇涂布

};
const CString STR_Result[]={"NG","OK"};
extern CEvent g_EventDelete;
extern CEvent g_EventCreateReport;
extern CString g_strMessage;
void ThreadDelete(PVOID* pParam);
void CreateReport(PVOID* pParam);

class CStorage : DHInterface
{
public:
	CStorage(void);
	~CStorage(void);
public:
	virtual int DoSomething(void* message,void* wParam,void* lParam); // 将调用方需要调用的成员函数声明成纯虚函数
	STORAGEINIT m_tInit;
	//存在对话框设置的变量
	WRITEDISK m_tWriteDisk;

private:
	void ReadParam();
public:


	void ExitDo();
	void AddFileFromFolder(const CString strFolderPath);
	void WriteParam();
	BOOL DirectoryExist(CString& Path);
	//void VistRemoteFile();
private:
	static void* _Thread(void *hcthread);//io线程
	void Loop();
	BOOL GetDiskStatus();//获取硬盘的状态 如果满了
	int Init(PSTORAGEINIT pInit);//初始化
	int PushMessage(const HTuple& hvMessageSend);//放置消息进入队列
	void WriteRecord(C_ALLOCATORWORKSHOPOUTPUT& Output,const CString& strPath);
	void WriteCPKRecord(C_ALLOCATORWORKSHOPOUTPUT& Output, const CString& strPath);
	void WriteGRRRecord(C_ALLOCATORWORKSHOPOUTPUT& Output, const CString& strPath);

	void WriteSurfaceRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput,HTuple& hv_ErrorInfo,const CString& strPath);

	void WriteMarkImage(C_ALLOCATORWORKSHOPOUTPUT& Output);//存储Mark所在的那张图像

	HTuple m_hvQueue;//队列句柄
	int m_nGrabberCount;//相机个数
	int m_nClearFlag[2];
	double m_dMaxFoilDistance;//最大极耳间距
	//从配置文件读的变量	
	CStringArray m_strAGrabberName;
	CString m_strSystemVisFile;
	CString m_strConfigFile;
	//系统要设置的变量，需要加锁
	CRITICAL_SECTION m_csChange;
	CString m_strBatch;//当前卷号
	std::vector<double>     m_vec_RowResolution;   //采集终端纵向分辨率
	std::vector<double>     m_vec_ColumnResolution;//采集终端横向分辨率
public:
	CStringArray m_ArrayErrorName;

	CString m_strLastBatch;//缓存的卷号
	CString m_strLastBatchPath;//上次的卷号
	STANDARDSURFACE m_tStandardSuface;//瑕疵规格
	STANDARDSIZE m_tStandardSize;//尺寸规格
	HTuple m_hvEANumTuple[2],m_hvEAResultTuple[2];
	CString m_strReportBatchPath,m_strReortBatch;
	HTuple m_hvImageIndexTuple,m_hvImagePathTuple;
	HTuple m_hvSingleImageIndexTuple,m_hvSingleImagePathTuple;
public:
	int m_nJXDataIndex;
	CTransferStatus m_CTransfer;
	CString m_strCopyPath;//拷贝路径
	HTuple m_hvLengthSum;//累计长度
public:
	BOOL InitThread();
	static void* HThreadStorage(void *hcthread);
	static void  T_Save(HTuple hv_SaveQueue, HTuple hv_ThreadIndex);//保存图像的线程
	CString GetChildPath(int& nSN);
	void     ReleaseQueues();
	void     InitQueues();
	void     WriteImageFile(C_ALLOCATORWORKSHOPOUTPUT& Output,int nThreadSN);//存储图像

	CString  GetChildName(int& nSN);
	int				m_nSaveThreadNum;				//保存线程数量
	HTupleVector	m_hvecStorageQueues;			//存储队列句柄




};

