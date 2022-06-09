#pragma once
#include "TransferStatus.h"
#include "Define.h"
#include "SCIStorage.in.h"
//extern HTupleVector  g_hvec_GroupDecriptInfo;  //�����Ʒ��������Ϣ
extern HTupleVector  g_hvec_DecriptInfoSN;  //�����Ʒ�ļ������
//extern HObjectVector g_hovec_GroupImageInfo;//�����Ʒ����Ϣ
enum e_CoatType
{
	M_TwoCam_Continue,//����Ϳ��
	M_TwoCam_Intermission,//��ЪͿ��

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
	virtual int DoSomething(void* message,void* wParam,void* lParam); // �����÷���Ҫ���õĳ�Ա���������ɴ��麯��
	STORAGEINIT m_tInit;
	//���ڶԻ������õı���
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
	static void* _Thread(void *hcthread);//io�߳�
	void Loop();
	BOOL GetDiskStatus();//��ȡӲ�̵�״̬ �������
	int Init(PSTORAGEINIT pInit);//��ʼ��
	int PushMessage(const HTuple& hvMessageSend);//������Ϣ�������
	void WriteRecord(C_ALLOCATORWORKSHOPOUTPUT& Output,const CString& strPath);
	void WriteCPKRecord(C_ALLOCATORWORKSHOPOUTPUT& Output, const CString& strPath);
	void WriteGRRRecord(C_ALLOCATORWORKSHOPOUTPUT& Output, const CString& strPath);

	void WriteSurfaceRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput,HTuple& hv_ErrorInfo,const CString& strPath);

	void WriteMarkImage(C_ALLOCATORWORKSHOPOUTPUT& Output);//�洢Mark���ڵ�����ͼ��

	HTuple m_hvQueue;//���о��
	int m_nGrabberCount;//�������
	int m_nClearFlag[2];
	double m_dMaxFoilDistance;//��󼫶����
	//�������ļ����ı���	
	CStringArray m_strAGrabberName;
	CString m_strSystemVisFile;
	CString m_strConfigFile;
	//ϵͳҪ���õı�������Ҫ����
	CRITICAL_SECTION m_csChange;
	CString m_strBatch;//��ǰ���
	std::vector<double>     m_vec_RowResolution;   //�ɼ��ն�����ֱ���
	std::vector<double>     m_vec_ColumnResolution;//�ɼ��ն˺���ֱ���
public:
	CStringArray m_ArrayErrorName;

	CString m_strLastBatch;//����ľ��
	CString m_strLastBatchPath;//�ϴεľ��
	STANDARDSURFACE m_tStandardSuface;//覴ù��
	STANDARDSIZE m_tStandardSize;//�ߴ���
	HTuple m_hvEANumTuple[2],m_hvEAResultTuple[2];
	CString m_strReportBatchPath,m_strReortBatch;
	HTuple m_hvImageIndexTuple,m_hvImagePathTuple;
	HTuple m_hvSingleImageIndexTuple,m_hvSingleImagePathTuple;
public:
	int m_nJXDataIndex;
	CTransferStatus m_CTransfer;
	CString m_strCopyPath;//����·��
	HTuple m_hvLengthSum;//�ۼƳ���
public:
	BOOL InitThread();
	static void* HThreadStorage(void *hcthread);
	static void  T_Save(HTuple hv_SaveQueue, HTuple hv_ThreadIndex);//����ͼ����߳�
	CString GetChildPath(int& nSN);
	void     ReleaseQueues();
	void     InitQueues();
	void     WriteImageFile(C_ALLOCATORWORKSHOPOUTPUT& Output,int nThreadSN);//�洢ͼ��

	CString  GetChildName(int& nSN);
	int				m_nSaveThreadNum;				//�����߳�����
	HTupleVector	m_hvecStorageQueues;			//�洢���о��




};

