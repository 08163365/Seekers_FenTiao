#include "stdafx.h"
#pragma once

extern int       WM_DISP;
extern int       WM_GRABBER;
extern int       WM_DETECT;
extern int       WM_RESTARTPROCESS;
extern int       g_nGrabberCount;
extern int       g_nGrabberType;
extern int       g_nDatabaseType;//���ݿ�����,0 ��ʹ�ã�1 SQL Server���ݿ�
extern PVOID     g_pMainDialog;       //���Ի���;
extern CString   g_strConfigSystemFile;//System.vis
extern CString   g_strConfigCommonFile;//Common.dh
extern CString   g_strProductName;//��ǰ����Ʒ��
extern CString   g_strBatchStartTime;//����ʼʱ��
extern CString   g_strBatch ;//��ǰ���
extern CString   g_strUserName;//��ǰ�û�
extern double    g_dwYield;//��ǰ�ܲ���,�ڻص������׼���




extern s_Count   g_Count;//ͳ����
//�ɼ��ն˸��� 
extern std::vector<DHInterface*>  g_vec_FindMark;//Ѱ��ƴͼ��λ
extern std::vector<DHInterface*>  g_vec_Grabber;    //�ɼ��ն�����
extern std::vector<DHInterface*> g_vec_Algorithm;//�㷨��
extern DHInterface*   g_pLightControl;//��Դ����ģ��



extern std::vector<double>     g_vec_RowResolution;   //�ɼ��ն�����ֱ���
extern std::vector<double>     g_vec_ColumnResolution;//�ɼ��ն˺���ֱ���
extern std::vector<CString>    g_vec_GrabberName;//�ɼ��ն˵�����

extern DHInterface* g_pLog;//��־��
extern DHInterface* g_pStorage;//�洢ģ��
extern DHInterface* g_pCommunicate;//ͨ��ģ��
extern DHInterface* g_pCLabeller;//����PLCģ��

extern HTupleVector  g_hvec_QueueImages;  //�ɼ�����
extern HTupleVector  g_hvec_QueuePintu;  //�ɼ�����
extern HTupleVector  g_hvec_QueueResults; //�������

extern HTupleVector  g_hvec_QueueDisplay; //��ʾ����
extern HTupleVector  g_hvec_VProcThreads;    //�����Ѿ������߳̾��

extern SYSTEMSET      g_tSystemSet;

extern CADOConnect g_AdoDatabase;//���ݿ�

extern CBlendent g_Blendent;//ȫ����ɫ

extern s_ProductParam g_ProductParam;





extern CJudge g_Jugde[2];
extern CFinsTcpExt g_CFinsTcp;//Finsͨ����
extern CWinThread* g_Thread;
extern double g_dwCorrectHeight[2];



extern CRITICAL_SECTION g_CsClearLabelCount;//���ͳ�����������


extern CUpload g_CUpLoad;//
extern bool  g_bExitThread;
extern double g_dScaleFactor;//����ϵ�� Ĭ��0.25


extern CPowerLimit g_CPowerLimit;
extern CString   g_strOperatorPassWord;//����Ա����
extern CString   g_strEngineerPassWord;//����ʦ����
extern CString   g_strFactoryPassWord;//��������

extern double  g_RFirstMean;//�Ҳ��׼��
extern double  g_LFirstMean;//����׼��
extern double  g_LData;//��������·�������
extern double  g_RData;//�Ҳ��·����ݻ�����
extern CRITICAL_SECTION g_CsPopWindowChange;//������
//extern CRITICAL_SECTION g_CsRight;//�Ҳ�������
extern 	CString g_strSaveBasePath;//�洢����·�� �������·�����ŵ�·��
extern CString g_strSaveFullPath;//�洢ȫ·��    ȫ·����覴�����


extern int    g_nGrabberLineDivideNum;//�ɼ���Ƶ��Ŀ
extern int g_Array_nOutNum[2];
extern CEvent g_EventChange;
extern BOOL g_bChangeProduct;//���ͱ�־λ

extern BOOL g_bUseAutoFlag;//�Զ����ͱ�־λ
extern double g_dRealSpeed;//ʵʱ�ٶ�
extern DHInterface* m_pCrash; //������

//extern std::queue<InkjetInfo>    g_queue_LResultData;//��������·�������
//extern std::queue<InkjetInfo>    g_queue_RResultData;//�Ҳ��·����ݻ�����

extern BOOL  g_bType;//0:�ȵ���Ԫ�� 1:��Ŀ��
extern s_TempStoreInfo  g_tStoreInfo;
extern  BOOL  g_bWriteFlag;

extern CreateBaseLineInfo g_tCreateBaseLineInfo;
extern CString g_ArrayStrLastGroupID[2];
extern BOOL   g_bExitLogin;//�Զ��˳���־λ
extern double g_dReceieveSpeed;//�ӷ������յ����ٶ�
extern double g_dTestSpeed[2];//�׼��ٶ�
extern CInkjetProcess g_CInJetProcess[2];
extern CSpotSizeCheck g_CSpotSizeCheck[2];//�����
extern CAutoGRR g_CAutoGRR[2];//�Զ�GRR���
extern HTuple    g_hvVertionInfoTuple;//�汾��Ϣ��
extern CString  g_StrCurrentVersion;//��ǰ�汾��
extern BOOL     g_bUseSeekersWithoutServer;//��ʹ�÷���������
extern HTuple   g_hvPopWindowNum;//��������
extern int g_nTestEANum[2];
extern CString g_strLabelTypeName;//�����������
extern double  g_dCoatThickness;//��ѹ��� ��λmm

extern BOOL g_bNGLabelTwoEA;//�׼��NG������EA

extern HTuple g_hvXTuple;
extern HTuple g_hvYTuple;
extern double g_dEncoderLocation[2];//0
extern BOOL    g_bExceptionSoftStatus;//״̬