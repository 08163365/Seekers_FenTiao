#include "stdafx.h"
#pragma once

extern int       WM_DISP;
extern int       WM_GRABBER;
extern int       WM_DETECT;
extern int       WM_RESTARTPROCESS;
extern int       g_nGrabberCount;
extern int       g_nGrabberType;
extern int       g_nDatabaseType;//数据库类型,0 不使用，1 SQL Server数据库
extern PVOID     g_pMainDialog;       //主对话框;
extern CString   g_strConfigSystemFile;//System.vis
extern CString   g_strConfigCommonFile;//Common.dh
extern CString   g_strProductName;//当前加载品种
extern CString   g_strBatchStartTime;//换卷开始时间
extern CString   g_strBatch ;//当前卷号
extern CString   g_strUserName;//当前用户
extern double    g_dwYield;//当前总产量,在回调里以米计数




extern s_Count   g_Count;//统计类
//采集终端个数 
extern std::vector<DHInterface*>  g_vec_FindMark;//寻找拼图定位
extern std::vector<DHInterface*>  g_vec_Grabber;    //采集终端数组
extern std::vector<DHInterface*> g_vec_Algorithm;//算法类
extern DHInterface*   g_pLightControl;//光源控制模块



extern std::vector<double>     g_vec_RowResolution;   //采集终端纵向分辨率
extern std::vector<double>     g_vec_ColumnResolution;//采集终端横向分辨率
extern std::vector<CString>    g_vec_GrabberName;//采集终端的名称

extern DHInterface* g_pLog;//日志类
extern DHInterface* g_pStorage;//存储模块
extern DHInterface* g_pCommunicate;//通信模块
extern DHInterface* g_pCLabeller;//贴标PLC模块

extern HTupleVector  g_hvec_QueueImages;  //采集队列
extern HTupleVector  g_hvec_QueuePintu;  //采集队列
extern HTupleVector  g_hvec_QueueResults; //结果队列

extern HTupleVector  g_hvec_QueueDisplay; //显示队列
extern HTupleVector  g_hvec_VProcThreads;    //所有已经开启线程句柄

extern SYSTEMSET      g_tSystemSet;

extern CADOConnect g_AdoDatabase;//数据库

extern CBlendent g_Blendent;//全局配色

extern s_ProductParam g_ProductParam;





extern CJudge g_Jugde[2];
extern CFinsTcpExt g_CFinsTcp;//Fins通信类
extern CWinThread* g_Thread;
extern double g_dwCorrectHeight[2];



extern CRITICAL_SECTION g_CsClearLabelCount;//清空统计贴标计数锁


extern CUpload g_CUpLoad;//
extern bool  g_bExitThread;
extern double g_dScaleFactor;//缩放系数 默认0.25


extern CPowerLimit g_CPowerLimit;
extern CString   g_strOperatorPassWord;//操作员密码
extern CString   g_strEngineerPassWord;//工程师密码
extern CString   g_strFactoryPassWord;//厂家密码

extern double  g_RFirstMean;//右侧首间距
extern double  g_LFirstMean;//左侧首间距
extern double  g_LData;//左侧数据下发缓存区
extern double  g_RData;//右侧下发数据缓存区
extern CRITICAL_SECTION g_CsPopWindowChange;//数据锁
//extern CRITICAL_SECTION g_CsRight;//右侧数据锁
extern 	CString g_strSaveBasePath;//存储基本路径 服务器下发到卷号的路径
extern CString g_strSaveFullPath;//存储全路径    全路径到瑕疵数据


extern int    g_nGrabberLineDivideNum;//采集分频数目
extern int g_Array_nOutNum[2];
extern CEvent g_EventChange;
extern BOOL g_bChangeProduct;//换型标志位

extern BOOL g_bUseAutoFlag;//自动换型标志位
extern double g_dRealSpeed;//实时速度
extern DHInterface* m_pCrash; //崩溃类

//extern std::queue<InkjetInfo>    g_queue_LResultData;//左侧数据下发缓存区
//extern std::queue<InkjetInfo>    g_queue_RResultData;//右侧下发数据缓存区

extern BOOL  g_bType;//0:先导利元亨 1:海目星
extern s_TempStoreInfo  g_tStoreInfo;
extern  BOOL  g_bWriteFlag;

extern CreateBaseLineInfo g_tCreateBaseLineInfo;
extern CString g_ArrayStrLastGroupID[2];
extern BOOL   g_bExitLogin;//自动退出标志位
extern double g_dReceieveSpeed;//从服务器收到的速度
extern double g_dTestSpeed[2];//首间速度
extern CInkjetProcess g_CInJetProcess[2];
extern CSpotSizeCheck g_CSpotSizeCheck[2];//两侧的
extern CAutoGRR g_CAutoGRR[2];//自动GRR检测
extern HTuple    g_hvVertionInfoTuple;//版本信息表
extern CString  g_StrCurrentVersion;//当前版本号
extern BOOL     g_bUseSeekersWithoutServer;//不使用服务器控制
extern HTuple   g_hvPopWindowNum;//弹窗计数
extern int g_nTestEANum[2];
extern CString g_strLabelTypeName;//贴标规格的名称
extern double  g_dCoatThickness;//冷压厚度 单位mm

extern BOOL g_bNGLabelTwoEA;//首间距NG打连个EA

extern HTuple g_hvXTuple;
extern HTuple g_hvYTuple;
extern double g_dEncoderLocation[2];//0
extern BOOL    g_bExceptionSoftStatus;//状态