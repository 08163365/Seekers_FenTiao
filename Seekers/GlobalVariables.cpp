#include "stdafx.h"


int       WM_DISP = RegisterWindowMessage (_T("MYDISP"));
int       WM_GRABBER = RegisterWindowMessage (_T("MYGRABBER"));
int       WM_DETECT = RegisterWindowMessage (_T("MYDETECT"));
int       WM_RESTARTPROCESS = RegisterWindowMessage (_T("WM_RESTARTPROCESS"));


CString   g_strConfigSystemFile = NULL;
CString   g_strConfigCommonFile = NULL;
CString   g_strProductName = NULL;//当前加载品种
CString   g_strBatchStartTime="00";//换卷开始时间
CString   g_strBatch = NULL;//当前卷号
CString   g_strUserName = NULL;//当前用户
double    g_dwYield = 0;//当前总产量,在回调里以米计数


int       g_nDatabaseType = 0;
int       g_nGrabberCount = 1;        //采集终端个数 
int       g_nGrabberType = 0;

//InitGlobalVariables中赋值

std::vector<DHInterface*>  g_vec_FindMark;//寻找拼图定位
std::vector<DHInterface*>  g_vec_Grabber;    //采集终端数组
std::vector<DHInterface*> g_vec_Algorithm;//算法类


 DHInterface*   g_pLightControl= NULL;//光源控制模块
DHInterface* g_pStorage = NULL;//存储模块
DHInterface* g_pCommunicate = NULL;//通信模块
DHInterface* g_pCLabeller= NULL;//贴标PLC模块
std::vector<double>     g_vec_RowResolution;   //采集终端纵向分辨率
std::vector<double>     g_vec_ColumnResolution;//采集终端横向分辨率
std::vector<CString>    g_vec_GrabberName;//采集终端的名称



DHInterface* g_pLog = NULL;

//数据流程
//回调-->采集队列           对应回调线程的个数
//采集队列-->处理-->结果队列
//结果队列-->剔废队列
//结果队列-->显示队列        //主线程中收到显示队列的消息,会负责显示工作
//结果队列-->保存队列        //

//InitQueues中赋值
HTupleVector  g_hvec_QueueImages(1);  //采集队列
HTupleVector  g_hvec_QueueResults(1); //结果队列
HTupleVector  g_hvec_QueuePintu(1);  //采集队列
HTupleVector  g_hvec_QueueDisplay(1); //显示队列
HTupleVector  g_hvec_VProcThreads(1);    //所有已经开启线程句柄

CADOConnect  g_AdoDatabase;//数据库
SYSTEMSET    g_tSystemSet;
PVOID        g_pMainDialog = NULL;       //主对话框;
s_Count      g_Count;//统计类
//s_LabelCause g_LabelCauseCount;
//贴标原因统计
CBlendent g_Blendent;
s_ProductParam g_ProductParam;
//std::vector<CString> g_vec_strDetecName;				//NG项目名字
//std::vector<CString> g_vec_strJudegName;				//判断项目名字
CJudge g_Jugde[2];
CFinsTcpExt g_CFinsTcp;
CWinThread* g_Thread=NULL;
double g_dwCorrectHeight[2];
CRITICAL_SECTION g_CsClearLabelCount;//清空统计贴标计数锁

CUpload g_CUpLoad;//
bool  g_bExitThread=false;
double g_dScaleFactor = 0.02;
CPowerLimit g_CPowerLimit;//权限设置
CString   g_strOperatorPassWord;//操作员密码
CString   g_strEngineerPassWord;//工程师密码
CString   g_strFactoryPassWord;//厂家密码

double  g_RFirstMean=0;//右侧首间距
double  g_LFirstMean=0;//左侧首间距
double  g_LData=0;//左侧数据下发缓存区
double  g_RData=0;//右侧下发数据缓存区
CRITICAL_SECTION g_CsPopWindowChange;//数据锁
//CRITICAL_SECTION g_CsRight;//右侧数据锁

CString g_strSaveFullPath;//存储全路径
CString g_strSaveBasePath;//存储基本路径

int    g_nGrabberLineDivideNum;//采集分频数目
int g_Array_nOutNum[2]={0};
CEvent g_EventChange;
BOOL g_bChangeProduct=FALSE;//换型标志位
BOOL g_bUseAutoFlag=FALSE;//自动换型标志位
double g_dRealSpeed=0;//实时速度
DHInterface* m_pCrash; //崩溃类
BOOL g_bType=TRUE;//类型
s_TempStoreInfo  g_tStoreInfo;
BOOL  g_bWriteFlag=TRUE;
CreateBaseLineInfo g_tCreateBaseLineInfo;
CString g_ArrayStrLastGroupID[2]={_T("000EA"),_T("000EA")};//初始化EA序号
BOOL   g_bExitLogin=FALSE;
double g_dReceieveSpeed=0;//从服务器收到的速度
double g_dTestSpeed[2]={-1,-1};//首间速度
CInkjetProcess g_CInJetProcess[2];
CSpotSizeCheck g_CSpotSizeCheck[2];//两侧的
CAutoGRR       g_CAutoGRR[2];//自动GRR检测
HTuple  g_hvVertionInfoTuple;//版本信息表
CString  g_StrCurrentVersion="1.0.0.0";//当前版本号
BOOL     g_bUseSeekersWithoutServer;//不使用服务器控制
HTuple   g_hvPopWindowNum;//弹窗计数
int g_nTestEANum[2]={0,0};
CString g_strLabelTypeName="";//贴标规格的名称
double  g_dCoatThickness=-1;//冷压厚度 单位mm
BOOL g_bNGLabelTwoEA=FALSE;//首间距NG打连个EA

HTuple g_hvXTuple;
HTuple g_hvYTuple;
double g_dEncoderLocation[2];//0
BOOL   g_bExceptionSoftStatus=FALSE;//状态