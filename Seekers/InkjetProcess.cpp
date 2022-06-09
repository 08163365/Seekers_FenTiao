#include "StdAfx.h"
#include "InkjetProcess.h"





CInkjetProcess::CInkjetProcess(void)
{
	InitializeCriticalSection(&m_csChange);
	m_nOutCount = 0;
	m_nLastOutCout = 0;
	m_nLastImageSN = 0;
	m_nImageSN = 0;
	m_nVecSize = 0;
}


CInkjetProcess::~CInkjetProcess(void)
{
	DeleteCriticalSection(&m_csChange);
}

int  CInkjetProcess::GetVecSize()
{

	return m_nVecSize;
}

void CInkjetProcess::Action(C_ALLOCATORWORKSHOPOUTPUT& Output)
{

	if (Output.bIsFinalResult == TRUE && (Output.tFinalResult.hv_SurfaceFlag == FALSE || Output.tFinalResult.hv_SizeFlag == FALSE))
	{
		InkjetInfo tInkjetInfo;
		HTuple hv_ErrorIndex;
		int nImageNgSN;
		hv_ErrorIndex.Clear();
		hv_ErrorIndex.Append(Output.tFinalResult.hv_SurfaceErrorIndex).Append(Output.tFinalResult.hv_SizeErrorIndex);
		TupleSort(hv_ErrorIndex, &hv_ErrorIndex);
		tInkjetInfo.hv_FirstStrName = (HTuple)g_Count.strAClassName.GetAt(hv_ErrorIndex[0].I());

		nImageNgSN = Output.B[0].A.hvImageSN.I() & 0xff;//图像号最大255
		tInkjetInfo.hv_NGImageSN = nImageNgSN;
		if (Output.tFinalResult.hv_SurfaceFlag == FALSE)
		{

			tInkjetInfo.hv_EAID = Output.hvGroupID;
			tInkjetInfo.hv_Trip = Output.hvPackageType;
			tInkjetInfo.hv_Speed = g_dReceieveSpeed;
			tInkjetInfo.hv_Yield = g_dwYield;
			for (int k = 0; k < Output.tFinalResult.hv_SurfaceErrorIndex.Length(); k++)
			{
				tInkjetInfo.hv_ErrorName.Append((HTuple)g_Count.strAClassName[Output.tFinalResult.hv_SurfaceErrorIndex[k].I()]);
				tInkjetInfo.hv_ErrorNum.Append(Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[k]);
				tInkjetInfo.hv_TotalLength = Output.tFinalResult.hv_TotalLength.D();
				tInkjetInfo.hv_TripWidth = Output.B[0].tDetectOutput.hv_CoatWidth / 2;
				tInkjetInfo.hv_LTRow.Append(Output.tFinalResult.tSecondSurfaceInfo.hv_LeftTopRow);
				tInkjetInfo.hv_LTColumn.Append(Output.tFinalResult.tSecondSurfaceInfo.hv_LeftTopColumn);
				tInkjetInfo.hv_RBRow.Append(Output.tFinalResult.tSecondSurfaceInfo.hv_RightBottomRow);
				tInkjetInfo.hv_RBColumn.Append(Output.tFinalResult.tSecondSurfaceInfo.hv_RightBottomColumn);
			}
		}
		else if (Output.tFinalResult.hv_SizeFlag == FALSE)
		{

			tInkjetInfo.hv_EAID = Output.hvGroupID;
			tInkjetInfo.hv_Trip = Output.hvPackageType;
			tInkjetInfo.hv_Speed = g_dReceieveSpeed;
			tInkjetInfo.hv_Yield = g_dwYield;

			for (int k = 0; k < Output.tFinalResult.hv_SizeErrorIndex.Length(); k++)
			{
				tInkjetInfo.hv_ErrorName.Append((HTuple)g_Count.strAClassName[Output.tFinalResult.hv_SizeErrorIndex[k].I()]);
				tInkjetInfo.hv_ErrorNum.Append(1);
				tInkjetInfo.hv_TotalLength = Output.tFinalResult.hv_TotalLength.D();
				tInkjetInfo.hv_TripWidth = Output.B[0].tDetectOutput.hv_CoatWidth / 2;
				tInkjetInfo.hv_LTRow.Append(1);
				tInkjetInfo.hv_LTColumn.Append(1);
				tInkjetInfo.hv_RBRow.Append(1000);
				tInkjetInfo.hv_RBColumn.Append(1000);
			}

		}

		AppendQueue(tInkjetInfo);

	}

}




void CInkjetProcess::AppendQueue(InkjetInfo& tInjectInfo)
{
	EnterCriticalSection(&m_csChange);
	m_vectorResultData.push_back(tInjectInfo);
	LeaveCriticalSection(&m_csChange);
}
void CInkjetProcess::SendToInjectInfo(CString& StrQuexinValue, CString& StrPenMaValue, int nSN)
{
	///发送格式 [130,工位号,缺陷信息,喷码信息]//贴标校验
	/*if(g_CUpLoad.m_bComSocket)
	{
	HTuple hv_SendTuple;
	hv_SendTuple.Append(130);
	hv_SendTuple.Append(nSN);
	hv_SendTuple.Append((HTuple)StrQuexinValue);

	}*/

}
void  CInkjetProcess::GetSendInfo(InkjetInfo& tInkjetInfo, int nGongWeiNum, CString& StrSendValue)
{
	CString strValue, strNGData, strTemp, strData, strGongWeiName;
	static int S_nCountNum[4] = { 0 };
	S_nCountNum[nGongWeiNum]++;
	strGongWeiName = g_strCameraName[nGongWeiNum];
	strData.Format("%s#w=%.2f#l=%.2f;time#delay=%d;", strGongWeiName, tInkjetInfo.hv_TripWidth.D(), tInkjetInfo.hv_TotalLength.D(), S_nCountNum[nGongWeiNum]);
	int nIndex = 0;
	for (int k = 0; k < tInkjetInfo.hv_ErrorName.Length(); k++)
	{
		for (int j = 0; j < tInkjetInfo.hv_ErrorNum[k].I(); j++)
		{
			strTemp.Format("%s#x1=%.2f#y1=%.2f#x2=%.2f#y2=%.2f;", tInkjetInfo.hv_ErrorName[k].S().TextA(), tInkjetInfo.hv_LTColumn[nIndex + j].D(), tInkjetInfo.hv_LTRow[nIndex + j].D(), tInkjetInfo.hv_RBColumn[nIndex + j].D(), tInkjetInfo.hv_RBColumn[nIndex + j].D());
			strNGData += strTemp;

		}
		nIndex = tInkjetInfo.hv_ErrorNum[k].I();
	}
	StrSendValue.Format("%s%s", strData, strNGData);
}
void  CInkjetProcess::Init(int nSide)//初始化
{
	m_nSide = nSide;
}
int  CInkjetProcess::GetSN(int nTrip)
{
	if (g_CUpLoad.m_StrGongWeiName == "UpOffset")//如果不是上带路延迟5ms写
	{
		if (nTrip == 0)
		{
			return 1;
		}
		else
		{
			return 2;
		}

	}
	else
	{
		if (nTrip == 0)
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}

}
void CInkjetProcess::PopQueue(int nSide)
{
	EnterCriticalSection(&m_csChange);

	int nImageSN = m_nImageSN;
	int nIoCount = m_nOutCount;
	std::vector<InkjetInfo>::iterator iter;
	BOOL bSendFlag = TRUE;

	HTuple hv_FindEAName, hv_FindErrorName;
	hv_FindEAName.Clear();
	hv_FindErrorName.Clear();

	for (iter = m_vectorResultData.begin(); iter != m_vectorResultData.end();)
	{

		if (iter->hv_NGImageSN.I() == nImageSN)//遍历查找对应的图像号的错误信息
		{
			static int s_NumCount[4] = { 0 };
			s_NumCount[GetSN(nSide)]++;
			CString strPenMaValue = "";//用于发送给喷码机缺陷信息
			CString strQuexianValue = "";//用于统计的缺陷信息
			InkjetInfo tInkjetInfo;
			tInkjetInfo = *iter;
			iter = m_vectorResultData.erase(iter);
			hv_FindEAName.Append(tInkjetInfo.hv_EAID);
			hv_FindErrorName.Append(tInkjetInfo.hv_FirstStrName);

		}
		else
		{
			iter++;
		}


	}
	if (hv_FindEAName.Length() > 0)
	{
		bSendFlag = FALSE;
		CString strQuexianValue, strEAID;
		strEAID = hv_FindEAName[hv_FindEAName.Length() - 1].S().Text();
		strQuexianValue = hv_FindErrorName[hv_FindErrorName.Length() - 1].S().Text();
		SendToInjectInfo(strQuexianValue, strQuexianValue, GetSN(nSide));//发送
		CString strValue;
		strValue.Format("%d,%s,%d", nIoCount, strQuexianValue, nImageSN);
		WriteOutIOInfo(strEAID, strValue);
	}
	m_nVecSize = m_vectorResultData.size();//获取缓存队列的长度
	LeaveCriticalSection(&m_csChange);
}


void  CInkjetProcess::ReadRealOutPortCount()
{
	int nReadCount;
	if (m_nSide == 0)
	{
		nReadCount = 4;//OUT0计数
		g_pCommunicate->DoSomething("READCOUNT", &nReadCount, &m_nOutCount);
	}
	else
	{
		nReadCount = 5;//OUT1计数
		g_pCommunicate->DoSomething("READCOUNT", &nReadCount, &m_nOutCount);
	}
}

int  CInkjetProcess::GetOutCount()
{

	return m_nOutCount;
}

int CInkjetProcess::ReadRealOutImageSN()
{

	int nReadCount;
	if (m_nSide == 0)
	{
		//OUT0出队的图像号
		g_pCommunicate->DoSomething("GETPARAM", "XImageOutSN", &m_nImageSN);
	}
	else
	{
		//OUT1出队的图像号
		g_pCommunicate->DoSomething("GETPARAM", "2ndImageOutSN", &m_nImageSN);
	}
	static int nLastnMageSN[2] = { 0,0 };
	int nCountMaxLine = 0;
	if (nLastnMageSN[m_nSide] != m_nImageSN)
	{
		nLastnMageSN[m_nSide] = m_nImageSN;
		CString strValue;
		strValue.Format("%d,%d,%d", m_nOutCount, m_nImageSN, nCountMaxLine);
		WriteOutIOChangeOnly(strValue);
	}


	return m_nImageSN;

}
BOOL CInkjetProcess::IsFreshImageSN()//判断图像号是否更新成功
{
	if (m_nImageSN != m_nLastImageSN)
	{
		m_nLastImageSN = m_nImageSN;
		return TRUE;

	}
	else
	{
		return FALSE;
	}

}


int CInkjetProcess::GetOutChaneNum()
{

	int nCountChangeNum;
	ReadRealOutPortCount();
	if (m_nOutCount != m_nLastOutCout)
	{
		Sleep(1);//给图像号延迟更新留点空间
		ReadRealOutImageSN();
		nCountChangeNum = MAX(1, m_nOutCount - m_nLastOutCout);
		if (TRUE == IsFreshImageSN())
		{
			///要执行发送给服务器的130 缺陷类型

			PopQueue(m_nSide);
		}
		m_nLastOutCout = m_nOutCount;
		return nCountChangeNum;
	}

	return 0;

}


void  CInkjetProcess::WriteOutIOChangeOnly(CString& strValue)
{
	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_strProductName, g_strBatch);
	strSizeRecord.Format("%s\\工位%d输出端口变化记录-%s.csv.", strSizePath, GetSN(m_nSide), g_strBatch);
	////时间	EAID	编号关联	隶属条	速度	计长	高度	分区1	分区2
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("时间,输出端口计数,图像号,最大行");
		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d.%02d:%02d:%02d:%03d", CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	HTuple hv_FileHandle;
	ofstream outFile;
	CString strCount;
	outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
	strCount.Format("%s,%s", strTime, strValue);
	outFile << strCount << endl;
	outFile.close();
}

void  CInkjetProcess::WriteOutIOInfo(CString& StrEAID, CString& strValue)
{


	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_strProductName, g_strBatch);
	strSizeRecord.Format("%s\\工位%d贴标输出I0口记录-%s.csv.", strSizePath, GetSN(m_nSide), g_strBatch);
	////时间	EAID	编号关联	隶属条	速度	计长	高度	分区1	分区2
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("时间,品种,EAID,速度,输出端口计数,错误类型,出队图像号");
		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d.%02d:%02d:%02d:%03d", CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	HTuple hv_FileHandle;
	ofstream outFile;
	CString strCount;
	outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
	strCount.Format("%s,%s,%s,%.1f,%s", strTime, g_strProductName, StrEAID, g_dRealSpeed, strValue);
	outFile << strCount << endl;
	outFile.close();
}
void  CInkjetProcess::WriteInjeckInfo(InkjetInfo& tInjectInfo, CString& strValue, int& nNumCount)
{
	if (g_CUpLoad.m_tParam.bUseJeket == FALSE)
	{
		return;
	}

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_strProductName, g_strBatch);
	strSizeRecord.Format("%s\\工位%d喷码信息输出记录-%s.csv.", strSizePath, GetSN(m_nSide), g_strBatch);
	////时间	EAID	编号关联	隶属条	速度	计长	高度	分区1	分区2
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("时间,品种,EAID,编号关联,隶属条,速度,计长,喷码信息");
		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d.%02d:%02d:%02d:%03d", CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	HTuple hv_FileHandle;
	ofstream outFile;
	CString strCount;
	outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
	strCount.Format("%s,%s,%s,%d,%d,%.1f,%.1f,%s", strTime, g_strProductName, tInjectInfo.hv_EAID.S().Text(), nNumCount, GetSN(tInjectInfo.hv_Trip.I()), tInjectInfo.hv_Speed.D(), tInjectInfo.hv_Yield.D(), strValue);
	outFile << strCount << endl;
	outFile.close();
}