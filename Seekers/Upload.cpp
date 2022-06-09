#include "StdAfx.h"
#include <regex>
#include "Upload.h"
#include "SeekersDlg.h"
#include "DlgProductManager.h"

#pragma once
CUpload* g_This = NULL;

const CString STR_Control[] =
{
	"关闭",
	"开启",
};

CUpload::CUpload()
{

	InitializeCriticalSection(&m_csChange);
}


CUpload::~CUpload()
{
	DeleteCriticalSection(&m_csChange);
}




void T_Connect()
{
	HTuple hvError, hvOpenStatus, ParamValue;
	while (FALSE == g_bExitThread)
	{
		WaitSeconds(1);
	}
}

void CUpload::Upload(C_ALLOCATORWORKSHOPOUTPUT& C)
{
	HTuple hvMessageSend;
	try
	{
		static int nCount = 0;
		//判断是否需要点检，需要点检则投递消息
		if (!IsInSpotTime() && nCount == 0)return;
		nCount++;
		if (nCount > g_CSpotSizeCheck[0].m_tSpotParam.nSpotNum)
			nCount = 0;
		CreateMessage(&hvMessageSend);
		C.WriteMessage(hvMessageSend);
		EnqueueMessage(m_hvQueue, hvMessageSend, HTuple(), HTuple());
	}
	catch (HException& e)
	{
	}
	ClearMessage(hvMessageSend);//消息投递完成,清空消息
}




BOOL CUpload::ReadParam()
{

	m_strConfigPath.Format("%s\\Config\\Public\\SCIPLC.ini", GetCurrentAppPath());
	m_tParam.strAddressCom = GetPrivateProfileCString("Upload", "strAddressCom", "127.0.0.1", m_strConfigPath);
	m_tParam.strAddressData = GetPrivateProfileCString("Upload", "strAddressData", "127.0.0.1", m_strConfigPath);
	m_tParam.nPortCom = GetPrivateProfileInt("Upload", "nPortCom", 4000, m_strConfigPath);
	m_tParam.strOffsetServerIP = GetPrivateProfileCString("Upload", "strOffsetServerIP", "127.0.0.1", m_strConfigPath);
	m_tParam.nOffsetServerPort = GetPrivateProfileInt("Upload", "nOffsetServerPort", 4000, m_strConfigPath);
	m_tParam.nBackDataSourceIndex = GetPrivateProfileInt("Upload", "nBackDataSourceIndex", 0, m_strConfigPath);
	m_tParam.dMaxDataLimit = GetPrivateProfileDouble("Upload", "dMaxDataLimit", 2, m_strConfigPath);
	m_tParam.nNGNumToStopMachine = GetPrivateProfileInt("Upload", "nNGNumToStopMachine", 5, m_strConfigPath);

	m_tParam.bUseJeket = GetPrivateProfileInt("Upload", "bUseJeket", 0, m_strConfigPath);
	m_tParam.bEnableDYOffset = GetPrivateProfileInt("Upload", "bEnableDYOffset", 1, m_strConfigPath);
	m_StrGongWeiName = GetPrivateProfileCString("Upload", "工位", "UpOffset", m_strConfigPath);
	for (int i = 0; i < CAMERA_COUNT; i++)
	{
		CString strPort;
		strPort.Format("nPortData%d", i);
		m_tParam.nPortData[i] = GetPrivateProfileInt("Upload", strPort, 4001, m_strConfigPath);
		TupleGenConst(g_Count.nType, 0, &m_hvErrorCount[i]);//生成错误类型长度的统计数
	}
	return TRUE;
}

BOOL CUpload::WriteParam()
{
	WritePrivateProfileString("Upload", "strAddressCom", m_tParam.strAddressCom, m_strConfigPath);
	WritePrivateProfileString("Upload", "strAddressData", m_tParam.strAddressData, m_strConfigPath);
	WritePrivateProfileString("Upload", "工位", m_StrGongWeiName, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nPortCom", m_tParam.nPortCom, m_strConfigPath);
	WritePrivateProfileString("Upload", "strOffsetServerIP", m_tParam.strOffsetServerIP, m_strConfigPath);

	WritePrivateProfileInt("Upload", "bUseJeket", m_tParam.bUseJeket, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nBackDataSourceIndex", m_tParam.nBackDataSourceIndex, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nOffsetServerPort", m_tParam.nOffsetServerPort, m_strConfigPath);
	WritePrivateProfileDouble("Upload", "dMaxDataLimit", m_tParam.dMaxDataLimit, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nNGNumToStopMachine", m_tParam.nNGNumToStopMachine, m_strConfigPath);
	WritePrivateProfileInt("Upload", "bEnableDYOffset", m_tParam.bEnableDYOffset, m_strConfigPath);
	for (int i = 0; i < CAMERA_COUNT; i++)
	{
		CString strPort;
		strPort.Format("nPortData%d", i);
		WritePrivateProfileInt("Upload", strPort, m_tParam.nPortData[i], m_strConfigPath);
	}
	return TRUE;
}

BOOL CUpload::Init()
{
	g_This = this;
	CreateMessageQueue(&m_hvQueue);//创建一个IO队列
	SetMessageQueueParam(m_hvQueue, "max_message_num", 10);
	HDevThreadContext Context;
	HDevThread* pThread = new HDevThread(Context,
		(void*)HThreadUpload, 1, 0);//开启一个具有1个输入的线程;
	pThread->SetInputCtrlParamTuple(0, m_hvQueue);
	pThread->Start();
	ReadParam();
	AfxBeginThread((AFX_THREADPROC)T_Connect, NULL);//将监听的端口打开


	return TRUE;
}




CString CUpload::GetFileVersion(CString strFileName)
{
#pragma comment(lib, "Version.lib ")
	//获取当前文件的版本信息
	DWORD dwLen = GetFileVersionInfoSize(strFileName, NULL);
	char *pszAppVersion = new char[dwLen + 1];
	CString strVersion;
	if (pszAppVersion)
	{
		memset(pszAppVersion, 0, sizeof(char)*(dwLen + 1));
		GetFileVersionInfo(strFileName, NULL, dwLen, pszAppVersion);

		UINT nLen(0);
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		VerQueryValue(pszAppVersion, "\\", (LPVOID*)&pFileInfo, &nLen);
		if (pFileInfo)
		{
			//获取版本号
			strVersion.Format("%d.%d.%d.%d", HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strVersion;
}

void* CUpload::HThreadUpload(void *hcthread)//io线程
{
	HDevThread*  hcppthread = (HDevThread*)hcthread;
	try
	{
		const HTuple        &cbhv_Queue = hcppthread->GetInputCtrlParamTuple(0);
		g_This->T_Upload(cbhv_Queue);
	}
	catch (HException& e)
	{
	}
	return NULL;
}



BOOL IsValidFileName(const std::string& strFileName)
{
	// Windows下文件名中不能包含\/:*?"<>|这些字符
	std::regex regExpress("[\\/:*?\"<>|]");
	BOOL bValid = !std::regex_search(strFileName, regExpress);
	return bValid;
}



void T_ComSocket()
{
	HTuple hvTimeStart(0), hvTimeEnd(0);
	HTuple hv_TimeNoDataStart(0), hv_TimeNoDataEnd(0);
	CountSeconds(&hv_TimeNoDataStart);
	CountSeconds(&hv_TimeNoDataEnd);
	Sleep(5000);

}

void  CUpload::GetGongWeiName(const C_ALLOCATORWORKSHOPOUTPUT &tOutput, CString& strComputerName)
{
	if (m_StrGongWeiName == "DownOffset")
	{
		if (tOutput.hvPackageType.I() == 0)
		{
			strComputerName = "工位3";
		}
		else
		{
			strComputerName = "工位4";

		}
	}
	else
	{
		if (tOutput.hvPackageType.I() == 0)
		{
			strComputerName = "工位1";
		}
		else
		{
			strComputerName = "工位2";

		}

	}


}


void CUpload::CountContinueErrorNum(C_ALLOCATORWORKSHOPOUTPUT &Output)
{

	try
	{

		//int nSide=Output.hvPackageType.I();
		//HTuple hv_SizeIndex,hv_SurfaceIndex;
		//if (Output.tFinalResult.hv_SizeFlag==FALSE || Output.tFinalResult.hv_SurfaceFlag==FALSE)
		//{
		//	for (int i=0;i<g_Count.nType;i++)
		//	{
		//		TupleFind(Output.tFinalResult.hv_SizeErrorIndex,i,&hv_SizeIndex);
		//		if (hv_SizeIndex!=-1 && hv_SizeIndex.Length()!=0)
		//		{
		//			m_hvErrorCount[nSide][i]=m_hvErrorCount[nSide][i]+1;
		//		}
		//		TupleFind(Output.tFinalResult.hv_SurfaceErrorIndex,i,&hv_SurfaceIndex);
		//		if (hv_SurfaceIndex!=-1 && hv_SurfaceIndex.Length()!=0)
		//		{
		//			m_hvErrorCount[nSide][i]=m_hvErrorCount[nSide][i]+1;
		//		}

		//	}
		//	HTuple hv_GreatIndex;
		//	TupleGreaterEqualElem(m_hvErrorCount[nSide],m_tParam.nNGNumToStopMachine,&hv_GreatIndex);
		//	TupleFind(hv_GreatIndex,1,&hv_GreatIndex);
		//	if (hv_GreatIndex.Length()>0 && hv_GreatIndex!=-1)
		//	{
		//		HTuple hv_MinIndex;

		//		TupleMin(hv_GreatIndex,&hv_MinIndex);
		//		m_hvErrorCount[nSide][hv_GreatIndex]=0;
		//		if (g_CUpLoad.m_bComSocket)
		//		{
		//			HTuple hv_SendTuple;
		//			CString strValue;
		//			CString strGongWeiName;
		//			int nCameraSN=0;
		//			int nErrorIndex=0;
		//			hv_SendTuple.Append(120);
		//			{

		//				if (nSide==0)//0条 前相机
		//				{
		//					nCameraSN=2;
		//					strGongWeiName="工位一";
		//					nErrorIndex=hv_MinIndex[0].I()*2;
		//				}else//1条后相机
		//				{
		//					nCameraSN=3;
		//					strGongWeiName="工位二";
		//					nErrorIndex=hv_MinIndex[0].I()*2+1;
		//				}
		//				hv_SendTuple.Append(nCameraSN);
		//				strValue.Format("上带路%s瑕疵连续%d个EA产品相同NG报警_%s_错误索引 %d",strGongWeiName,m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(hv_MinIndex[0].I()),nErrorIndex);
		//				hv_SendTuple.Append((HTuple)strValue);
		//				hv_SendTuple.Append(nErrorIndex);
		//			}
		//		}

		//	}

		//}else
		//{
		//	TupleGenConst(g_Count.nType,0,&m_hvErrorCount[nSide]);

		//}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("CountContinueErrorNum提示：%s:%s", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}


}
void CUpload::SetWarnToServer()
{
	try
	{
		/////输出口的计数
		/////////////////////////////////////////////
		BOOL  bExceptionSoftStatus = FALSE;
		g_pLog->DoSomething("GETPARAM", "SoftStatus", &bExceptionSoftStatus);
		if (TRUE == bExceptionSoftStatus)
		{
			CString strValue;
			strValue.Format("给设备发送停机信号");
			g_pLog->DoSomething("OPERATERLOG", (void*)(LPCSTR)strValue, NULL);
			g_pCommunicate->DoSomething("SETPARAM", "BJTJ", NULL);
			g_pCLabeller->DoSomething("SETPARAM", "RED_LIGHT", NULL);
		}

	}
	catch (HException& e)
	{

	}
}

void CUpload::CalculateCPK(int& nTrip, double& dRealValue, double dStandarValueValue, double dStandarDeviationValue, HTuple* hv_MeanValue, HTuple* hv_DeviationValue, HTuple* hv_CpkValue)
{

	try
	{
		nTrip = 0;
		static HTuple S_hvCPKData[2];
		HTuple hv_TempIndex, hv_EffectiveIndex, hv_EffectiveData;
		HTuple hv_Mean, hv_Deviation;
		static double  s_dLastCpk = 0;
		if (dRealValue<dStandarValueValue + dStandarDeviationValue && dRealValue>dStandarValueValue - dStandarDeviationValue)
		{
			S_hvCPKData[nTrip].Append(dRealValue);
		}

		if (S_hvCPKData[nTrip].Length() > 20)
		{
			TupleSortIndex(S_hvCPKData[nTrip], &hv_TempIndex);
			TupleSelectRange(hv_TempIndex, 2, S_hvCPKData[nTrip].Length() - 2, &hv_EffectiveIndex);
			TupleSelect(S_hvCPKData[nTrip], hv_EffectiveIndex, &hv_EffectiveData);
			TupleMean(hv_EffectiveData, &hv_Mean);
			TupleDeviation(hv_EffectiveData, &hv_Deviation);
			TupleMean(hv_EffectiveData, &(*hv_MeanValue));
			double dUp = dStandarValueValue + dStandarDeviationValue;
			double dDown = dStandarValueValue - dStandarDeviationValue;
			*hv_CpkValue = MIN(dUp - hv_Mean.D(), hv_Mean.D() - dDown) / (6 * MAX(0.001, hv_Deviation.D()));
			s_dLastCpk = *hv_CpkValue;
			*hv_DeviationValue = hv_Deviation;
			S_hvCPKData[nTrip].Clear();

		}
		else
		{
			*hv_DeviationValue = 0;
			*hv_CpkValue = s_dLastCpk;
			*hv_MeanValue = 0;
		}

	}
	catch (HException& e)
	{



	}



}

void CUpload::SendClosedData(C_ALLOCATORWORKSHOPOUTPUT &Output)//发送闭环数据
{
	//int nSide;
	//nSide=Output.hvPackageType.I();

	//static double SdDeviation[2]={0};
	//static double SdMean[2]={0};
	//static double SdCPK[2]={0};
	//g_LFirstMean=Output.hvDislocation.D();
	//g_LData=Output.B[m_tParam.nBackDataSourceIndex].tDetectOutput.hv_LocateMissing.D();
	//if (g_Jugde[nSide].m_nABCuoWeiType==MeanDataBack)
	//{
	//	g_LData=(Output.B[0].tDetectOutput.hv_LocateMissing.D()+Output.B[0].tDetectOutput.hv_LocateMissing.D())/2;
	//}
	//HTuple hvMean,hv_Deviation,hvCpk;
	//CalculateCPK(nSide,g_LData,g_Jugde[nSide].m_tStandardSize.dLocateMissing.D(),g_Jugde[nSide].m_tStandardSize.dLocateMissingDev.D(),&hvMean,&hv_Deviation,&hvCpk);
	//SdMean[nSide]=hvMean;
	//SdDeviation[nSide]=hv_Deviation;
	//SdCPK[nSide]=hvCpk;

}



void CUpload::SendExceptionWarn(int& nGrabberSN, CString strException)
{
	///停机报警
	g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
}


void CUpload::T_Upload(HTuple hvQueue)
{
	HTuple hv_MessageRecv, hvMessageNum, hv_StopProcInfo, hv_TimeStart, hv_TimeEnd, hv_ErrorFlag;
	while (!FALSE)
	{
		DequeueMessage(hvQueue, "timeout", "infinite", &hv_MessageRecv);
		C_ALLOCATORWORKSHOPOUTPUT tOutput;

		tOutput.ReadMessage(hv_MessageRecv);
		ClearMessage(hv_MessageRecv);
		int nSide = tOutput.hvPackageType.I();
		try
		{
			///点检
			SpotOutputInfo tSpotOut;
			g_CSpotSizeCheck[0].Action(tOutput.B[0].A.hoGrabImage, &tSpotOut);
			g_CSpotSizeCheck[1].Action(tOutput.B[1].A.hoGrabImage, &tSpotOut);
		}
		catch (HException& e)
		{


		}
	}
}

BOOL CUpload::IsInSpotTime()
{
	int nHour1, nMinute1, nHour2, nMinute2;
	sscanf(g_CSpotSizeCheck[0].m_strSpotTime[0], "%d:%d", &nHour1, &nMinute1);
	sscanf(g_CSpotSizeCheck[0].m_strSpotTime[1], "%d:%d", &nHour2, &nMinute2);

	//定时点检
	static CTime time;
	time = CTime::GetCurrentTime();
	static BOOL s_bSpotFirst = FALSE, s_bSpotSecond = FALSE;
	BOOL bInSpotTime = FALSE;
	///点检时间1
	if (time.GetHour() == nHour1 && time.GetMinute() >= nMinute1 &&
		time.GetMinute() < (nMinute1 + 5) && s_bSpotFirst == FALSE)
	{
		bInSpotTime = TRUE;//开始点检
		s_bSpotFirst = TRUE;//是否点检的标志位置TRUE
	}
	///点检时间2
	if (time.GetHour() == nHour2 && time.GetMinute() >= nMinute2 &&
		time.GetMinute() < (nMinute2 + 5) && s_bSpotSecond == FALSE)
	{
		bInSpotTime = TRUE;//开始点检
		s_bSpotSecond = TRUE;//是否点检的标志位置TRUE
	}
	return bInSpotTime;
}
