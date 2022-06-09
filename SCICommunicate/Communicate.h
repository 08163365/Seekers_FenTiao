#pragma once
#include "PIO24BLib.h"
#include "SCICommunicate.in.h"
#include "SerialPortExt.h"
#include <vector>
const int MAX_LANE = 2;
const int DEF_LASTLABLEPOS = -99999;//Ĭ������µı�ǩλ��


const CString STR_LabelWarnInfo[]
=
{
	"OK",
	"ȱ�걨��",
	"�ޱ걨��,�뻻��",
	"�ϴ�����,�ϴ�",
	"�ߴ�����,�ߴ��쳣",
	"©���걨��",
	"����10����",
	"����ͣ������",
	"��������",
	"�����쳣����",
	"��ѹ�ͱ���",
	"��ѹ�߱���",
	"����λ�ñ���",
	"��λ����ʧ�ܱ���",
	"�������쳣����",
	"����ʧ�ܱ���",
	"δ֪����",
};
typedef struct tagDeviceInfo
{
	//�豸�Ѻ�����,���Ѻá���
	CString szDeviceName;
	//�豸��
	CString szDeviceClass;
	//�豸��ʾ��
	CString szDeviceDesc;
	//�豸����
	CString szDriverName;
	//�豸ʵ��
	DWORD dwDevIns;
	//�豸���־
	GUID Guid;
	//����������
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
	BOOL       GetDeviceList(LPGUID lpGuid,BOOL bsFlag);//��ȡ�豸�б�
	BOOL       SetDeviceStatus(DeviceInfo &theDevice,BOOL bStatusFlag);//�����豸״̬
	void       ExceptionRemain(HTuple& hv_DiffValue,CString  strName);//����ʱУ��һ�¾�����������������õ������Ƿ���� ������ʾ
	void       RefreshDelayInfo();//����У��
	void       BaoJingFuWei();//�����������λ
public:
	double m_dMarkerLengthMM;//������ĳ��ȣ�mm��
	int m_nMarkerLength;//������ĳ��ȣ��У�,0 ���0 ,1 ���1
	CRITICAL_SECTION m_csCommunicateChange;//�������������ĵ�ʱ����Ҫ��ס����ֹ����ϵͳ�Ǳ����ڼ�飬�������ȥ���Ĳ���
	CString		m_strProductName;
	CDialogEx* m_pDlg;//������ָ��
	CString strProductName;
	DHInterface* m_pLogView;
	CString				m_strConfigCommonFile;
	CString				m_strConfigSystemFile;
	CString				m_strConfigCommunicateFile;
	CString				m_strConfigCommunVarietyFile;
	CString				m_strConfigVarietyParmaFile;
	CString				m_strConfigSizeParmaFile;
	COMMUICATEPARAM		m_Commuicate;//�߷���ز���
	LabelControlInfo    m_tLabelControlInfo;//������Ϣ����
	ALARMS				m_Alarms;//������ز���
	COMMONPARAM			m_Common;//��������
	CPIO24BLib          m_IOCard[PIO24_COUNT];//���ͼ��IO��
	int                 m_nLabelPos[MAX_LANE];//�����ǩ�Ժ����ͼƬ�е�λ��
	void WriteLabelAction(C_ALLOCATORWORKSHOPOUTPUT& tOutPut);
	void InitThread();
	void ReadParam();
	void WriteInfo();
	void ComPareChange(int LastValue,int CurrentValue,CString strName);
	void ComPareChange(double LastValue,double CurrentValue,CString strName);
	int m_nProductType;
	int m_nGrabberDivideNum;//�ɼ���Ƶ��Ŀ
    HWND m_OutputhWnd;
    CString m_strBatch;
    CString m_strSavePath;
	CString m_StrLocationLog;
	COMMUNICATEINIT m_sInit;
	HTuple m_hvGongWeiName;
	HTuple m_hvDaiLu;//���´�·
	int    m_nMarkCountNum[2];//Mark����
	void  MakeMachineStop();
private:
	HTuple m_hvQueue;//���о��
private:
	static void* _Thread(void *hcthread);//io�߳�
	void Loop();
	
	int Init(PCOMMUNICATEINIT pInit);//��ʼ��
	int PushMessage(const HTuple& hvMessageSend);//������Ϣ�������
	BOOL ReadyOutput(CString & strParam);//Ready�ź�
public:
	double m_dStandardEALength[2];
	int    m_nTripNum;
	HTuple m_hvRemainRow;//�洢ȱ�ݵľ���
//	CSerialPortExt m_Com;       //��������
	
	BOOL  m_bWarnFlag;

	HTuple m_hvRealMonitorLengthTuple;
	HTuple m_hvRealLabelCountTuple;
public:
	//������ص�
	
	BOOL m_bOpen;
	BOOL m_bState;

	CRITICAL_SECTION   m_csSerialCOMUse;

	BOOL m_bCounterFlag;
};

