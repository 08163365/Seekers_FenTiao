#include "stdafx.h"


int       WM_DISP = RegisterWindowMessage (_T("MYDISP"));
int       WM_GRABBER = RegisterWindowMessage (_T("MYGRABBER"));
int       WM_DETECT = RegisterWindowMessage (_T("MYDETECT"));
int       WM_RESTARTPROCESS = RegisterWindowMessage (_T("WM_RESTARTPROCESS"));


CString   g_strConfigSystemFile = NULL;
CString   g_strConfigCommonFile = NULL;
CString   g_strProductName = NULL;//��ǰ����Ʒ��
CString   g_strBatchStartTime="00";//����ʼʱ��
CString   g_strBatch = NULL;//��ǰ���
CString   g_strUserName = NULL;//��ǰ�û�
double    g_dwYield = 0;//��ǰ�ܲ���,�ڻص������׼���


int       g_nDatabaseType = 0;
int       g_nGrabberCount = 1;        //�ɼ��ն˸��� 
int       g_nGrabberType = 0;

//InitGlobalVariables�и�ֵ

std::vector<DHInterface*>  g_vec_FindMark;//Ѱ��ƴͼ��λ
std::vector<DHInterface*>  g_vec_Grabber;    //�ɼ��ն�����
std::vector<DHInterface*> g_vec_Algorithm;//�㷨��


 DHInterface*   g_pLightControl= NULL;//��Դ����ģ��
DHInterface* g_pStorage = NULL;//�洢ģ��
DHInterface* g_pCommunicate = NULL;//ͨ��ģ��
DHInterface* g_pCLabeller= NULL;//����PLCģ��
std::vector<double>     g_vec_RowResolution;   //�ɼ��ն�����ֱ���
std::vector<double>     g_vec_ColumnResolution;//�ɼ��ն˺���ֱ���
std::vector<CString>    g_vec_GrabberName;//�ɼ��ն˵�����



DHInterface* g_pLog = NULL;

//��������
//�ص�-->�ɼ�����           ��Ӧ�ص��̵߳ĸ���
//�ɼ�����-->����-->�������
//�������-->�޷϶���
//�������-->��ʾ����        //���߳����յ���ʾ���е���Ϣ,�Ḻ����ʾ����
//�������-->�������        //

//InitQueues�и�ֵ
HTupleVector  g_hvec_QueueImages(1);  //�ɼ�����
HTupleVector  g_hvec_QueueResults(1); //�������
HTupleVector  g_hvec_QueuePintu(1);  //�ɼ�����
HTupleVector  g_hvec_QueueDisplay(1); //��ʾ����
HTupleVector  g_hvec_VProcThreads(1);    //�����Ѿ������߳̾��

CADOConnect  g_AdoDatabase;//���ݿ�
SYSTEMSET    g_tSystemSet;
PVOID        g_pMainDialog = NULL;       //���Ի���;
s_Count      g_Count;//ͳ����
//s_LabelCause g_LabelCauseCount;
//����ԭ��ͳ��
CBlendent g_Blendent;
s_ProductParam g_ProductParam;
//std::vector<CString> g_vec_strDetecName;				//NG��Ŀ����
//std::vector<CString> g_vec_strJudegName;				//�ж���Ŀ����
CJudge g_Jugde[2];
CFinsTcpExt g_CFinsTcp;
CWinThread* g_Thread=NULL;
double g_dwCorrectHeight[2];
CRITICAL_SECTION g_CsClearLabelCount;//���ͳ�����������

CUpload g_CUpLoad;//
bool  g_bExitThread=false;
double g_dScaleFactor = 0.02;
CPowerLimit g_CPowerLimit;//Ȩ������
CString   g_strOperatorPassWord;//����Ա����
CString   g_strEngineerPassWord;//����ʦ����
CString   g_strFactoryPassWord;//��������

double  g_RFirstMean=0;//�Ҳ��׼��
double  g_LFirstMean=0;//����׼��
double  g_LData=0;//��������·�������
double  g_RData=0;//�Ҳ��·����ݻ�����
CRITICAL_SECTION g_CsPopWindowChange;//������
//CRITICAL_SECTION g_CsRight;//�Ҳ�������

CString g_strSaveFullPath;//�洢ȫ·��
CString g_strSaveBasePath;//�洢����·��

int    g_nGrabberLineDivideNum;//�ɼ���Ƶ��Ŀ
int g_Array_nOutNum[2]={0};
CEvent g_EventChange;
BOOL g_bChangeProduct=FALSE;//���ͱ�־λ
BOOL g_bUseAutoFlag=FALSE;//�Զ����ͱ�־λ
double g_dRealSpeed=0;//ʵʱ�ٶ�
DHInterface* m_pCrash; //������
BOOL g_bType=TRUE;//����
s_TempStoreInfo  g_tStoreInfo;
BOOL  g_bWriteFlag=TRUE;
CreateBaseLineInfo g_tCreateBaseLineInfo;
CString g_ArrayStrLastGroupID[2]={_T("000EA"),_T("000EA")};//��ʼ��EA���
BOOL   g_bExitLogin=FALSE;
double g_dReceieveSpeed=0;//�ӷ������յ����ٶ�
double g_dTestSpeed[2]={-1,-1};//�׼��ٶ�
CInkjetProcess g_CInJetProcess[2];
CSpotSizeCheck g_CSpotSizeCheck[2];//�����
CAutoGRR       g_CAutoGRR[2];//�Զ�GRR���
HTuple  g_hvVertionInfoTuple;//�汾��Ϣ��
CString  g_StrCurrentVersion="1.0.0.0";//��ǰ�汾��
BOOL     g_bUseSeekersWithoutServer;//��ʹ�÷���������
HTuple   g_hvPopWindowNum;//��������
int g_nTestEANum[2]={0,0};
CString g_strLabelTypeName="";//�����������
double  g_dCoatThickness=-1;//��ѹ��� ��λmm
BOOL g_bNGLabelTwoEA=FALSE;//�׼��NG������EA

HTuple g_hvXTuple;
HTuple g_hvYTuple;
double g_dEncoderLocation[2];//0
BOOL   g_bExceptionSoftStatus=FALSE;//״̬