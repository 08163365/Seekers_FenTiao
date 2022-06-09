#include "StdAfx.h"
#include <iostream>  
#include <fstream>  
#include "PDFOperate.h"

#include "Storage.h"
#include "DlgMap.h"
#include "DlgParamSet.h"
#pragma  once
using namespace std;
CString g_strMessage = _T(" ");
extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CStorage();
}
CStorage* g_This = NULL;
//HTupleVector  g_hvec_GroupDecriptInfo(1);  //整组产品的描述信息
//HObjectVector g_hovec_GroupImageInfo(1);//单组产品的信息
HTupleVector  g_hvec_DecriptInfoSN(1);  //整组产品的极耳序号
CEvent g_EventDelete;
CEvent g_EventCreateReport;

CStorage::CStorage() :m_hvecStorageQueues(1)
{
	InitializeCriticalSection(&m_csChange);
	m_dMaxFoilDistance = 100;
	m_hvSingleImageIndexTuple.Clear();
	m_hvSingleImagePathTuple.Clear();
	m_nJXDataIndex = 0;
	TupleGenConst(2, 0, &m_hvLengthSum);
}


CStorage::~CStorage()
{
	DeleteCriticalSection(&m_csChange);
	ReleaseQueues();

}
BOOL CStorage::GetDiskStatus()//获取硬盘的状态
{
	static BOOL bWarn = FALSE;
	BOOL bFull = FALSE;
	DWORD dwTotalDiskSpace, dwFreeDiskSpace, dwUsedDiskSpace;
	ULARGE_INTEGER uiFreeBytesAvailableToCaller;
	ULARGE_INTEGER uiTotalNumberOfBytes;
	ULARGE_INTEGER uiTotalNumberOfFreeBytes;
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	strPath.ReleaseBuffer();
	strPath = m_tWriteDisk.StrImageSavePath;
	int pos = strPath.Find('\\');
	strPath = strPath.Left(pos);
	CString strFreeDiskSpace;
	if (GetDiskFreeSpaceEx(strPath, &uiFreeBytesAvailableToCaller, &uiTotalNumberOfBytes,
		&uiTotalNumberOfFreeBytes))
	{
		dwTotalDiskSpace = (DWORD)(uiTotalNumberOfBytes.QuadPart / 1024 / 1024);
		dwFreeDiskSpace = (DWORD)(uiFreeBytesAvailableToCaller.QuadPart >> 20);
		dwUsedDiskSpace = dwTotalDiskSpace - dwFreeDiskSpace;
		float fds = (float)dwFreeDiskSpace / 1024;
		int nWarnValue = 10;//硬盘报警的状态值 当等于10G时报警
		if (fds < nWarnValue)//如果当前磁盘可用空间小于10 此参数值可设置
		{
			bFull = TRUE;
		}
		else
		{
			bFull = FALSE;
			bWarn = FALSE;
		}
	}
	if (TRUE == bFull)
	{
		if (FALSE == bWarn)
		{

			m_tInit.pLogView->DoSomething("EXCEPTION", (void*)(LPCSTR)("当前磁盘可用空间已经小于10G,系统已经暂停存图,请清理磁盘空间"), NULL);
			bWarn = TRUE;
		}

	}
	return bFull;
}


void CStorage::ExitDo()
{


	WritePrivateProfileDouble("WRITEDISK", "m_hvLengthSum0", m_hvLengthSum[0], m_strConfigFile);//工位一尺寸
	WritePrivateProfileDouble("WRITEDISK", "m_hvLengthSum1", m_hvLengthSum[1], m_strConfigFile);//工位二长度

}
void CreateReport(PVOID* pParam)
{
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_EventCreateReport.m_hObject, INFINITE))
		{
			CPDFOperate  ReportChange;
			CString strFilePath;
			CString strEAResultPath0, strEAResultPath1;
			CString strEANumPath0, strEANumPath1;

			CString strErrorPath0, strErrorPath1;


			Sleep(100);
			ReportChange.Init();

			/*	g_This->m_strReportBatchPath="E:\\2020年项目\\A项目筹备\\一出四\\更改缺陷类型-0827\\Run-x64\\2020年12月\\25日\\5612";
				g_This->m_strReortBatch="5612";*/


			strFilePath.Format("%s\\瑕疵数据\\上带路报表-%s.pdf", g_This->m_strReportBatchPath, g_This->m_strReortBatch);
			strEANumPath0.Format("%s\\瑕疵数据\\EANum.tup", g_This->m_strReportBatchPath);
			strEAResultPath0.Format("%s\\瑕疵数据\\EAResult.tup", g_This->m_strReportBatchPath);
			strErrorPath0.Format("%s\\瑕疵数据\\缺陷信息.tup", g_This->m_strReportBatchPath);



			CString strImageIndexFile, strImagePathFile;
			strImageIndexFile.Format("%s\\瑕疵数据\\ImageIndex.tup", g_This->m_strReportBatchPath);
			strImagePathFile.Format("%s\\瑕疵数据\\ImagePath.tup", g_This->m_strReportBatchPath);

			HTuple hv_Exist0, hv_Exist1, hv_Exist2, hv_Exist3;
			FileExists((HTuple)strEANumPath0, &hv_Exist0);
			FileExists((HTuple)strEANumPath1, &hv_Exist1);
			FileExists((HTuple)strEAResultPath0, &hv_Exist2);
			FileExists((HTuple)strEAResultPath1, &hv_Exist3);
			if (hv_Exist0 == 0 && hv_Exist2 == 0 && hv_Exist1 == 0 && hv_Exist3 == 0)
			{
				continue;
			}

			g_strMessage.Format("开始创建导出PDF文件。。。。。");
			if (TRUE == ReportChange.CreatePDF(strFilePath))
			{
				g_strMessage.Format("开始打开PDF文件。。。。。。");

			}
			else
			{
				g_strMessage.Format("创建PDF文件失败。。。。。。");
				continue;
			}
			try
			{
				int nMaxColumnNum = g_This->m_tStandardSize.hvTripNum.I() + 1;
				{
					CString StrTemp, StrHead;
					CString strValue;
					strValue = g_This->m_strReortBatch;
					//ReportChange.MergeExcel("A1","D1",strValue,50,13,TRUE);
					ReportChange.MergeCells(1, 1, nMaxColumnNum, strValue, TRUE);

					//合并A1-E1单元格，并设置表名"过车信息报表"
					SYSTEMTIME tTime;
					GetLocalTime(&tTime);
					strValue.Format("时间:%d-%d-%d %02d:%02d:%02d", tTime.wYear, tTime.wMonth, tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond);
					ReportChange.MergeCells(2, 1, nMaxColumnNum / 2, strValue);

					strValue = "操作员:admin";
					ReportChange.MergeCells(2, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);
					//合并A1-E1单元格，

					strValue = "膜卷信息";
					//ReportChange.MergeExcel("A3","D3",strValue,20);
					ReportChange.MergeCells(3, 1, nMaxColumnNum, strValue);

					strValue.Format("膜卷号：%s\r\n品种名:%s", g_This->m_strReortBatch, g_This->m_tInit.strProductName);
					//ReportChange.MergeExcel("A4","B5",strValue,20);
					ReportChange.MergeCells(4, 1, nMaxColumnNum / 2, strValue);


					strValue.Format("分条数：%d\r\n条宽:%.2f",g_This->m_tStandardSize.hvTripNum.I(), g_This->m_tStandardSize.dCoatWidth.D());
					//ReportChange.MergeExcel("C4","D5",strValue,20);
					ReportChange.MergeCells(4, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);

					strValue.Format("打标规格");
					ReportChange.MergeCells(5, 1, nMaxColumnNum, strValue);

					CString strTemp = "";
					strValue = "";
					for (int k = 0; k < g_This->m_tStandardSuface.hv_ErrorIndex.Length(); k++)
					{
						strTemp.Format("%s:面积≥%.2fmm^2并且数目＞%d或者面积≥%.2fmm^2并且数目＞%d\r\n", g_This->m_ArrayErrorName.GetAt(g_This->m_tStandardSuface.hv_ErrorIndex[k].I()), g_This->m_tStandardSuface.hv_TotalMaxArea[k].D(), g_This->m_tStandardSuface.hv_TotalMaxNumber[k].I(), g_This->m_tStandardSuface.hv_TotalMaxAreaTwo[k].D(), g_This->m_tStandardSuface.hv_TotalMaxNumberTwo[k].I());
						strValue += strTemp;
					}

					ReportChange.AddMultText(6, 1, nMaxColumnNum, strValue);
					strValue.Format("外观不良统计");
					ReportChange.AddMultText(7, 1, nMaxColumnNum, strValue);

					//strValue.Format(" ");
					//ReportChange.AddMultText(7, 3, 2, strValue);

					HTuple hv_ErrorFileExist0, hv_ErrorFileExist1;
					FileExists((HTuple)strErrorPath0, &hv_ErrorFileExist0);
					FileExists((HTuple)strErrorPath1, &hv_ErrorFileExist1);
					HTuple hv_ErrorSum, hv_TempTuple;
					if (hv_ErrorFileExist0 == 1 || hv_ErrorFileExist1 == 1)
					{
						if (hv_ErrorFileExist0 == 1)//存在
						{
							ReadTuple((HTuple)strErrorPath0, &hv_TempTuple);
							hv_ErrorSum = hv_TempTuple;

						}
						if (hv_ErrorFileExist1 == 1)//存在
						{
							ReadTuple((HTuple)strErrorPath1, &hv_TempTuple);
							if (hv_ErrorSum.Length() > 0)
							{
								TupleAdd(hv_ErrorSum, hv_TempTuple, &hv_ErrorSum);
							}
							else
							{
								hv_ErrorSum = hv_TempTuple;
							}
						}
						strTemp = "";
						strValue = "";
						for (int j = 0; j < hv_ErrorSum.Length() - 2; j++)
						{
							strTemp.Format("%s: %d\r\n", g_This->m_ArrayErrorName[j], hv_ErrorSum[j].I());
							strValue += strTemp;
						}
						ReportChange.AddMultText(8, 1, nMaxColumnNum / 2, strValue);

						int nTotal = hv_ErrorSum[hv_ErrorSum.Length() - 2].I();
						int nErrorTotal = hv_ErrorSum[hv_ErrorSum.Length() - 1].I();
						double dPercent = (nTotal - nErrorTotal)*1.0 / MAX(1, nTotal) * 100;
						strValue.Format("EA总数:%d\r\n贴标总条数:%d\r\n良率：%.2f%s", nTotal, nErrorTotal, dPercent, "%");
						ReportChange.AddMultText(8, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);
					}
					else
					{
						strTemp = "";
						strValue = "";
						for (int j = 0; j < hv_ErrorSum.Length() - 2; j++)
						{
							strValue.Format("%s: %s\r\n", g_This->m_ArrayErrorName[j], "--");
							strValue += strTemp;
						}
						ReportChange.AddMultText(8, 1, nMaxColumnNum / 2, strValue);

						strValue.Format("EA总数:--\r\n贴标总条数:--\r\n良率：--");
						ReportChange.AddMultText(8, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);

					}

					strValue.Format("不良贴标明细");
					ReportChange.MergeCells(9, 1, nMaxColumnNum, strValue);
					g_strMessage.Format("写入表头完成，开始插入贴标详细记录。。。。。。");
					//设置第二行的五个列的列名
					const char*   ListHead[11] = { "EA序号","第一条","第二条","第三条","第四条","第五条","第六条","第七条","第八条","第九条","第十条" };
					////设置第二行的五个列的列名
					for (int i = 0; i < nMaxColumnNum; i++)
					{
						StrTemp = ListHead[i];
						ReportChange.SetCellInfo(10, i + 1, StrTemp);
					}
				}
				long nRows = 11;
				HTuple hv_EAResult, hv_EANum, hv_TempTuple;
				hv_EAResult.Clear();
				hv_EANum.Clear();
				if (hv_Exist0 == 1)
				{
					ReadTuple((HTuple)strEANumPath0, &hv_TempTuple);
					hv_EANum.Append(hv_TempTuple);
				}
				if (hv_Exist1 == 1)
				{
					ReadTuple((HTuple)strEANumPath1, &hv_TempTuple);
					hv_EANum.Append(hv_TempTuple);

				}
				if (hv_Exist2 == 1)
				{

					ReadTuple((HTuple)strEAResultPath0, &hv_TempTuple);
					hv_EAResult.Append(hv_TempTuple);
				}
				if (hv_Exist3 == 1)
				{
					ReadTuple((HTuple)strEAResultPath1, &hv_TempTuple);
					hv_EAResult.Append(hv_TempTuple);

				}

				HTuple hv_Index, hv_Result;
				TupleSortIndex(hv_EANum, &hv_Index);
				TupleSelect(hv_EANum, hv_Index, &hv_EANum);
				TupleSelect(hv_EAResult, hv_Index, &hv_EAResult);
				for (int i = 0; i <= hv_EANum[hv_EANum.Length() - 1]; i++)
				{
					TupleFind(hv_EANum, i, &hv_Index);
					if (hv_Index != -1 && hv_Index.Length() > 0)
					{
						TupleSelect(hv_EAResult, hv_Index, &hv_Result);
						if (hv_Result.Length() > 1)
						{
							CString strValue;
							strValue.Format("%d", hv_EANum[hv_Index.I()].I());
							ReportChange.SetCellInfo(nRows, 1, strValue);
							ReportChange.SetCellInfo(nRows, 2, "NG");
							ReportChange.SetCellInfo(nRows, 3, "NG");
							nRows += 1;
						}
						else if (hv_Result.Length() == 1)
						{
							CString strValue;
							strValue.Format("%d", hv_EANum[hv_Index.I()].I());
							ReportChange.SetCellInfo(nRows, 1, strValue);
							ReportChange.SetCellInfo(nRows, 2, "NG");
							nRows += 1;
						}
					}

				}
				g_strMessage.Format("插入贴标详细记录完成，开始插入小图。。。。。。");


				//插入图像
				{

					HTuple hv_bImagePathExist, hv_ImageIndexExist;
					FileExists((HTuple)strImageIndexFile, &hv_ImageIndexExist);
					FileExists((HTuple)strImagePathFile, &hv_bImagePathExist);


					if (hv_ImageIndexExist == 1)
					{
						HTuple hv_ImageIndex, hv_ImagePath, hv_Index;
						ReadTuple((HTuple)strImageIndexFile, &hv_ImageIndex);
						ReadTuple((HTuple)strImagePathFile, &hv_ImagePath);

						TupleSortIndex(hv_ImageIndex, &hv_Index);
						TupleSelect(hv_ImageIndex, hv_Index, &hv_ImageIndex);
						TupleSelect(hv_ImagePath, hv_Index, &hv_ImagePath);


						CString strCellName, strValue;
						for (int i = 0; i < hv_ImageIndex.Length(); i++)
						{
							g_strMessage.Format("插入小图完成%.2f。。。。。。", i*100.0 / hv_ImageIndex.Length());

							if (i % nMaxColumnNum == 0 && i != 0)
							{
								nRows += 2;
							}
							ReportChange.SetCellImage(nRows, i % nMaxColumnNum + 1, hv_ImagePath[i].S().Text());

							CString strName, strEA, strErrorName, strArea, strX, strY, strGongWeiName;
							CString strALLName = hv_ImagePath[i].S().Text();

							int nImageNameIndex = strALLName.ReverseFind('\\');

							strName = strALLName.Mid(nImageNameIndex + 1, strALLName.GetLength() - nImageNameIndex);


							int nIndex = strName.Find("-");

							strEA = strName.Left(nIndex);
							strEA = strEA.Left(strEA.Find("E"));

							strName = strName.Right(strName.GetLength() - nIndex - 1);
							nIndex = strName.Find("-");
							strErrorName = strName.Left(nIndex);
							strName = strName.Right(strName.GetLength() - nIndex - 1);
							nIndex = strName.Find("-");
							strArea = strName.Left(nIndex);
							strName = strName.Right(strName.GetLength() - nIndex - 1);
							nIndex = strName.Find("-");
							strX = strName.Left(nIndex);
							strName = strName.Right(strName.GetLength() - nIndex - 1);

							nIndex = strName.Find("-");
							strY = strName.Left(nIndex);
							strName = strName.Right(strName.GetLength() - nIndex - 1);

							nIndex = strName.Find("j");
							strGongWeiName = strName.Left(nIndex - 1);
							strValue.Format("\r\nEA序号:%s\r\n缺陷名称:%s\r\n缺陷面积:%s\r\n像素Y:%s\r\n像素X:%s\r\n位置:%s\r\n", strEA, strErrorName, strArea, strY, strX, strGongWeiName);
							ReportChange.AddMultText(nRows + 1, i % nMaxColumnNum + 1, 1, strValue);
						}

					}
				}
				ReportChange.ClosePDF();
				g_strMessage.Format("导出表格完成！！！！！");
				////测试//////////////////////////////////////////////////////////////////////////
				{
					CString strDest, strTemp, strReplace, strBatchPath;
					strBatchPath = g_This->m_strReportBatchPath;
					strDest.Format("ccd");
					int nPosition = strBatchPath.Find(strDest);
					if (nPosition != -1)
					{

						strTemp = strBatchPath.Left(nPosition - strDest.GetLength() + 2);
						//要取代的地方
						//strReplace.Format("D:\\Map",GetCurrentAppPath());
						strReplace = g_This->m_strCopyPath;
						if (1/*DirectoryExist(strReplace)*/)//拷贝路径如果存在则 不存在则报警提示
						{

							strBatchPath.Replace(strBatchPath.Left(nPosition - strDest.GetLength() + 2), strReplace);
							CreateMultilevelPath(strBatchPath);
							CString strSourece, strDestFile;
							strSourece.Format("%s\\瑕疵数据", g_This->m_strReportBatchPath);
							strDestFile.Format("%s\\瑕疵数据", strBatchPath);
							CopyFolder(strSourece, strDestFile);
							strSourece.Format("%s\\瑕疵小图", g_This->m_strReportBatchPath);
							strDestFile.Format("%s\\瑕疵小图", strBatchPath);
							CopyFolder(strSourece, strDestFile);
							g_This->m_CTransfer.SetStauts();

							CString strInfo;
							strInfo.Format("拷贝%s文件到%s", strSourece, strDestFile);
							g_This->m_tInit.pLogView->DoSomething("OPERATERLOG", (LPSTR)(LPCSTR)strInfo, NULL);
						}
						else
						{
							CString strException;
							strException.Format("拷贝目录：%s不存在", strReplace);
							g_This->m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
						}
					}
				}
			}
			catch (HException& e)
			{
				ReportChange.ClosePDF();
				g_strMessage.Format("导出表格失败！！！！！");
			}
		}
	}
}

//void CreateReport(PVOID* pParam)
//{
//	while(1)
//	{
//		if (WAIT_OBJECT_0 ==WaitForSingleObject(g_EventCreateReport.m_hObject,INFINITE))
//		{
//			CReport  ReportChange;
//			CString strFilePath;
//			CString strEAResultPath0,strEAResultPath1;
//		    CString strEANumPath0,strEANumPath1;
//
//			CString strErrorPath0,strErrorPath1;
//
//
//			Sleep(100);
//			if (g_This->m_strGongWei=="UpOffset")//上带路
//			{
//				strFilePath.Format("%s\\瑕疵数据\\上带路报表-%s.xlsx",g_This->m_strReportBatchPath,g_This->m_strReortBatch);
//				strEANumPath0.Format("%s\\瑕疵数据\\工位一-EANum.tup",g_This->m_strReportBatchPath);
//				strEANumPath1.Format("%s\\瑕疵数据\\工位二-EANum.tup",g_This->m_strReportBatchPath);
//				strEAResultPath0.Format("%s\\瑕疵数据\\工位一-EAResult.tup",g_This->m_strReportBatchPath);
//				strEAResultPath1.Format("%s\\瑕疵数据\\工位二-EAResult.tup",g_This->m_strReportBatchPath);
//
//				strErrorPath0.Format("%s\\瑕疵数据\\工位一缺陷信息.tup",g_This->m_strReportBatchPath);
//				strErrorPath1.Format("%s\\瑕疵数据\\工位二缺陷信息.tup",g_This->m_strReportBatchPath);
//
//
//			}else
//			{
//				strFilePath.Format("%s\\瑕疵数据\\下带路报表-%s.xlsx",g_This->m_strReportBatchPath,g_This->m_strReortBatch);
//				strEANumPath0.Format("%s\\瑕疵数据\\工位三-EANum.tup",g_This->m_strReportBatchPath);
//				strEANumPath1.Format("%s\\瑕疵数据\\工位四-EANum.tup",g_This->m_strReportBatchPath);
//				strEAResultPath0.Format("%s\\瑕疵数据\\工位三-EAResult.tup",g_This->m_strReportBatchPath);
//				strEAResultPath1.Format("%s\\瑕疵数据\\工位四-EAResult.tup",g_This->m_strReportBatchPath);
//				strErrorPath0.Format("%s\\瑕疵数据\\工位三缺陷信息.tup",g_This->m_strReportBatchPath);
//				strErrorPath1.Format("%s\\瑕疵数据\\工位四缺陷信息.tup",g_This->m_strReportBatchPath);
//
//
//			}
//			
//		
//			CString strImageIndexFile,strImagePathFile;
//			strImageIndexFile.Format("%s\\瑕疵数据\\%s-ImageIndex.tup",g_This->m_strReportBatchPath,g_This->m_strGongWei);
//			strImagePathFile.Format("%s\\瑕疵数据\\%s-ImagePath.tup",g_This->m_strReportBatchPath,g_This->m_strGongWei);
//
//			HTuple hv_Exist0,hv_Exist1,hv_Exist2,hv_Exist3;
//			FileExists((HTuple)strEANumPath0,&hv_Exist0);
//			FileExists((HTuple)strEANumPath1,&hv_Exist1);
//			FileExists((HTuple)strEAResultPath0,&hv_Exist2);
//			FileExists((HTuple)strEAResultPath1,&hv_Exist3);
//			if (hv_Exist0==0&&hv_Exist2==0 &&  hv_Exist1==0 &&hv_Exist3==0)
//			{
//				continue;
//			}
//
//			{
//
//			}
//			g_strMessage.Format("开始创建导出Excel文件。。。。。");
//			if (TRUE==ReportChange.CreateNewExcel(strFilePath))
//			{
//
//				g_strMessage.Format("开始打开Excel文件。。。。。。");
//
//			}else
//			{
//				g_strMessage.Format("创建Excel文件失败。。。。。。");
//				continue;
//			}
//
//
//			if (TRUE==ReportChange.OpenExcel(strFilePath))
//			{
//				g_strMessage.Format("打开Excel文件完成，开始写入表头。。。。。。");
//			}else
//			{
//				g_strMessage.Format("打开Excel文件失败。。。。。。");
//				continue;
//			}
//
//
//			try
//			{
//
//				//	ReportChange.CreateFixeModel();
//				{
//					CRange0 range;
//
//					CString StrTemp,StrHead;
//
//					CString strValue;
//					strValue=g_This->m_strReortBatch;
//					ReportChange.MergeExcel("A1","D1",strValue,50,13,TRUE);
//					//合并A1-E1单元格，并设置表名"过车信息报表"
//					SYSTEMTIME tTime;
//					GetLocalTime(&tTime);
//
//					strValue.Format("时间:%d-%d-%d %02d:%02d:%02d",tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
//					ReportChange.MergeExcel("A2","B2",strValue,20);
//
//					strValue="操作员:admin";
//					ReportChange.MergeExcel("C2","D2",strValue,20);
//
//					//合并A1-E1单元格，并设置表名"过车信息报表"
//
//					strValue="膜卷信息";
//					ReportChange.MergeExcel("A3","D3",strValue,20);
//
//
//					strValue.Format("膜卷号：%s\r\n品种名:%s",g_This->m_strReortBatch,g_This->m_tInit.strProductName);
//					ReportChange.MergeExcel("A4","B5",strValue,20);
//
//
//					strValue.Format("分条数：2\r\n条宽:123");
//					ReportChange.MergeExcel("C4","D5",strValue,20);
//
//					strValue.Format("打标规格");
//					ReportChange.MergeExcel("A6","D6",strValue,20);
//
//
//					CString strTemp="";
//					strValue="";
//					for (int k=0;k<g_This->m_tStandardSuface.hv_ErrorIndex.Length();k++)
//					{
//						strTemp.Format("%s:面积≥%.2fmm^2并且数目＞%d或者面积≥%.2fmm^2并且数目＞%d\r\n",g_This->m_ArrayErrorName.GetAt(g_This->m_tStandardSuface.hv_ErrorIndex[k].I()),g_This->m_tStandardSuface.hv_TotalMaxArea[k].D(),g_This->m_tStandardSuface.hv_TotalMaxNumber[k].I(),g_This->m_tStandardSuface.hv_TotalMaxAreaTwo[k].D(),g_This->m_tStandardSuface.hv_TotalMaxNumberTwo[k].I());
//						strValue+=strTemp;
//					}
//					ReportChange.MergeExcel("A7","D15",strValue,20);
//					strValue.Format("外观不良统计");
//					ReportChange.MergeExcel("A16","B16",strValue,20);
//					strValue.Format("1");
//					ReportChange.MergeExcel("C16","D16",strValue,20);
//
//					HTuple hv_ErrorFileExist0,hv_ErrorFileExist1;
//					FileExists((HTuple)strErrorPath0,&hv_ErrorFileExist0);
//					FileExists((HTuple)strErrorPath1,&hv_ErrorFileExist1);
//					HTuple hv_ErrorSum,hv_TempTuple;
//					if (hv_ErrorFileExist0==1||hv_ErrorFileExist1==1)
//					{
//						if (hv_ErrorFileExist0==1)//存在
//						{
//							ReadTuple((HTuple)strErrorPath0,&hv_TempTuple);
//							hv_ErrorSum=hv_TempTuple;
//
//						}
//						if (hv_ErrorFileExist1==1)//存在
//						{
//							ReadTuple((HTuple)strErrorPath1,&hv_TempTuple);
//							if (hv_ErrorSum.Length()>0)
//							{
//								TupleAdd(hv_ErrorSum,hv_TempTuple,&hv_ErrorSum);
//							}else
//							{
//								hv_ErrorSum=hv_TempTuple;
//							}
//						}
//						strTemp="";
//						strValue="";
//						for (int j=0;j<hv_ErrorSum.Length()-2;j++)
//						{
//							strTemp.Format("%s: %d\r\n",g_This->m_ArrayErrorName[j],hv_ErrorSum[j].I());
//							strValue+=strTemp;
//						}
//						ReportChange.MergeExcel("A17","B25",strValue,25);
//						int nTotal=hv_ErrorSum[hv_ErrorSum.Length()-2].I();
//						int nErrorTotal=hv_ErrorSum[hv_ErrorSum.Length()-1].I();
//						double dPercent=(nTotal-nErrorTotal)*1.0/MAX(1,nTotal)*100;
//						strValue.Format("EA总数:%d\r\n贴标总条数:%d\r\n良率：%.2f%s",nTotal,nErrorTotal,dPercent,"%");
//						ReportChange.MergeExcel("C17","D25",strValue,25);
//					}else
//					{
//
//						strTemp="";
//						strValue="";
//						for (int j=0;j<hv_ErrorSum.Length()-2;j++)
//						{
//							strValue.Format("%s: %s\r\n",g_This->m_ArrayErrorName[j],"--");
//							strValue+=strTemp;
//						}
//						ReportChange.MergeExcel("A17","B25",strValue,20);
//						strValue.Format("EA总数:--\r\n贴标总条数:--\r\n良率：--");
//						ReportChange.MergeExcel("C17","D25",strValue,20);
//
//					}
//
//
//					strValue.Format("不良贴标明细");
//					ReportChange.MergeExcel("A26","D26",strValue,20);
//					g_strMessage.Format("写入表头完成，开始插入贴标详细记录。。。。。。");
//					//设置第二行的五个列的列名
//					const char*   ListHead[4]={"EA序号","第一条","第二条","备注"};
//					////设置第二行的五个列的列名
//					for (int i=0;i<4;i++)
//					{
//
//						StrTemp=ListHead[i];
//						ReportChange.SetCellText(27,i,StrTemp);
//
//					}
//				}
//				long nRows=ReportChange.GetUsedRows();
//				HTuple hv_EAResult,hv_EANum,hv_TempTuple;
//				hv_EAResult.Clear();
//				hv_EANum.Clear();
//				if (hv_Exist0==1)
//				{
//					ReadTuple((HTuple)strEANumPath0,&hv_TempTuple);
//					hv_EANum.Append(hv_TempTuple);
//				}
//				if (hv_Exist1==1)
//				{
//					ReadTuple((HTuple)strEANumPath1,&hv_TempTuple);
//					hv_EANum.Append(hv_TempTuple);
//
//				}
//				if (hv_Exist2==1)
//				{
//
//					ReadTuple((HTuple)strEAResultPath0,&hv_TempTuple);
//					hv_EAResult.Append(hv_TempTuple);
//				}
//				if (hv_Exist3==1)
//				{
//					ReadTuple((HTuple)strEAResultPath1,&hv_TempTuple);
//					hv_EAResult.Append(hv_TempTuple);
//
//				}
//
//				HTuple hv_Index,hv_Result;
//				TupleSortIndex(hv_EANum,&hv_Index);
//				TupleSelect(hv_EANum,hv_Index,&hv_EANum);
//				TupleSelect(hv_EAResult,hv_Index,&hv_EAResult);
//				for (int i=0;i<=hv_EANum[hv_EANum.Length()-1];i++)
//				{
//					TupleFind(hv_EANum,i,&hv_Index);
//					if (hv_Index!=-1&&hv_Index.Length()>0)
//					{
//						TupleSelect(hv_EAResult,hv_Index,&hv_Result);
//						if (hv_Result.Length()>1)
//						{
//							CString strValue;
//							strValue.Format("%d",hv_EANum[hv_Index.I()].I());
//							ReportChange.SetCellText(nRows+1,0,strValue);
//							ReportChange.SetCellText(nRows+1,1,"NG");
//							ReportChange.SetCellText(nRows+1,2,"NG");
//							nRows=ReportChange.GetUsedRows();
//							//ReportChange.Save();
//						}else if(hv_Result.Length()==1)
//						{
//							CString strValue;
//							strValue.Format("%d",hv_EANum[hv_Index.I()].I());
//							ReportChange.SetCellText(nRows+1,0,strValue);
//							ReportChange.SetCellText(nRows+1,hv_Result[0].I(),"NG");
//							nRows=ReportChange.GetUsedRows();
//							//ReportChange.Save();
//						}
//					}
//
//				}
//
//
//				nRows=ReportChange.GetUsedRows();
//				ReportChange.SetCellText(nRows+1,1," ");
//				ReportChange.SetCellText(nRows+2,1," ");
//
//			    g_strMessage.Format("插入贴标详细记录完成，开始插入小图。。。。。。");
//
//				//插入图像
//				//{
//				//
//				//	HTuple hv_bImagePathExist,hv_ImageIndexExist;
//				//	FileExists((HTuple)strImageIndexFile,&hv_ImageIndexExist);
//				//	FileExists((HTuple)strImagePathFile,&hv_bImagePathExist);
//
//
//				//	if (hv_ImageIndexExist==1)
//				//	{
//				//		HTuple hv_ImageIndex,hv_ImagePath,hv_Index;
//				//		ReadTuple((HTuple)strImageIndexFile,&hv_ImageIndex);
//				//		ReadTuple((HTuple)strImagePathFile,&hv_ImagePath);
//
//				//		TupleSortIndex(hv_ImageIndex,&hv_Index);
//				//		TupleSelect(hv_ImageIndex,hv_Index,&hv_ImageIndex);
//				//		TupleSelect(hv_ImagePath,hv_Index,&hv_ImagePath);
//				//		CString strCellName,strValue;
//				//		for (int i=0;i<hv_ImageIndex.Length();i++)
//				//		{
//
//
//				//			g_strMessage.Format("插入小图完成%.2f。。。。。。",i*100.0/hv_ImageIndex.Length());
//				//		
//				//			if (i%4==0)
//				//			{
//				//				nRows=ReportChange.GetUsedRows();
//				//			}
//				//			strCellName.Format("%c%d",ReportChange.m_Head[i%4],nRows+1);
//				//			ReportChange.InsertPicture(strCellName,20,80,hv_ImagePath[i].S().Text());
//
//
//				//			CString strName,strEA,strErrorName,strArea,strX,strY,strGongWeiName;
//				//			CString strALLName=hv_ImagePath[i].S().Text();
//
//				//			int nImageNameIndex=strALLName.ReverseFind('\\');
//
//				//			strName=strALLName.Mid(nImageNameIndex+1,strALLName.GetLength()-nImageNameIndex);
//
//
//				//			int nIndex=strName.Find("-");
//
//				//			strEA=strName.Left(nIndex);
//				//			strEA=strEA.Left(strEA.Find("E"));
//
//				//			strName=strName.Right(strName.GetLength()-nIndex-1);
//				//			nIndex=strName.Find("-");
//				//			strErrorName=strName.Left(nIndex);
//				//			strName=strName.Right(strName.GetLength()-nIndex-1);
//				//			nIndex=strName.Find("-");
//				//			strArea=strName.Left(nIndex);
//				//			strName=strName.Right(strName.GetLength()-nIndex-1);
//				//			nIndex=strName.Find("-");
//				//			strX=strName.Left(nIndex);
//				//			strName=strName.Right(strName.GetLength()-nIndex-1);
//
//				//			nIndex=strName.Find("-");
//				//			strY=strName.Left(nIndex);
//				//			strName=strName.Right(strName.GetLength()-nIndex-1);
//
//				//			nIndex=strName.Find("j");
//				//			strGongWeiName=strName.Left(nIndex-1);
//				//			strValue.Format("\r\nEA序号:%s\r\n缺陷名称:%s\r\n缺陷面积:%s\r\n像素Y:%s\r\n像素X:%s\r\n位置:%s\r\n",strEA,strErrorName,strArea,strY,strX,strGongWeiName);
//				//			//strValue.Format("EA序号:%d",hv_ImageIndex[i].I());
//				//			ReportChange.SetCellText(nRows+2,i%4,strValue,120);
//				//			//ReportChange.Save();
//
//
//				//		}
//
//				//	}
//				//}
//		    	g_strMessage.Format("导出表格完成！！！！！");
//    		}catch(HException& e)
//			{
//
//			}
//			ReportChange.AddBorderLine();//绘制边框
//			ReportChange.Save();
//			ReportChange.Close();
//
//
//
//
//
//
//		}
//		
//	}
//
//}
void ThreadDelete(PVOID* pParam)
{
	while (1)
	{
		if (WAIT_OBJECT_0 == WaitForSingleObject(g_EventDelete.m_hObject, INFINITE))
		{
			CString  strException;
			strException.Format("开始手动清理缓存图像请等待删除完成。。。。。。。。。。。");
			(g_This->m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

			CString strSavePath;
			strSavePath.Format("%s\\ccd_data", g_This->m_tWriteDisk.StrImageSavePath);
			DeleteOldFileFolder(strSavePath, g_This->m_tWriteDisk.nSaveDays);
			g_EventDelete.ResetEvent();

			strException.Format("缓存清理已完成请继续生产！！");
			(g_This->m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

		}
		Sleep(1000);
	}
}



BOOL CStorage::DirectoryExist(CString& Path)
{
	WIN32_FIND_DATA fd;
	BOOL ret = FALSE;
	HANDLE hFind = FindFirstFile(Path, &fd);
	if (hFind != INVALID_HANDLE_VALUE && (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		ret = TRUE;
	}
	FindClose(hFind);
	return ret;
}

void CStorage::WriteMarkImage(C_ALLOCATORWORKSHOPOUTPUT& Output)
{
	if (m_tWriteDisk.bSaveMarkImage&&Output.bIsFinalResult == TRUE)
	{
		try
		{
			CString strObjectFilePATH, strObjectFileName;
			SYSTEMTIME CurSysTime;
			int nSide = Output.hvPackageType.I();
			GetLocalTime(&CurSysTime);
			strObjectFilePATH.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\瑕疵大图\\Mark图像-%s\\%d工位", m_tWriteDisk.StrImageSavePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, nSide + 1);
			if (FALSE == DirectoryExist(strObjectFilePATH))
				CreateMultilevelPath(strObjectFilePATH);
			HObject hoZoomImage;
			ZoomImageFactor(Output.B[0].A.hoGrabImage, &hoZoomImage, 0.5, 0.5, "nearest_neighbor");
			strObjectFileName.Format("%s\\%d-%.1f-%02d_%02d_%02d_%03d.jpeg", strObjectFilePATH, Output.B[0].A.hvImageSN.I(), Output.B[0].hv_CropRow.D(), CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
			WriteImage(hoZoomImage, "jpg", 0, (HTuple)strObjectFileName);
		}
		catch (HException& e)
		{

		}
	}
}


CString CStorage::GetChildPath(int& nSN)
{
	CString strChildPath;

	switch (nSN)
	{
	case 0:
		strChildPath = "工位一\\正面";
		break;
	case 1:
		strChildPath = "工位二\\正面";
		break;
	case 2:
		strChildPath = "工位一\\反面";
		break;
	case 3:
		strChildPath = "工位二\\反面";
		break;
	default:
		strChildPath = "工位二\\反面";
		break;
	}

	return strChildPath;
}

CString CStorage::GetChildName(int& nSN)
{
	CString strChildPath;

	switch (nSN)
	{
	case 0:
		strChildPath = "工位一正面";
		break;
	case 1:
		strChildPath = "工位二正面";
		break;
	case 2:
		strChildPath = "工位一反面";
		break;
	case 3:
		strChildPath = "工位二反面";
		break;
	default:
		strChildPath = "工位二反面";
		break;
	}

	return strChildPath;
}

void CStorage::WriteImageFile(C_ALLOCATORWORKSHOPOUTPUT& Output, int nCameraSN)
{
	if (FALSE == m_tWriteDisk.bSaveAllGrabberImage)
	{
		return;
	}
	int nSN;
	B_DETECTWORKSHOPOUTPUT* pDetectOutput = NULL;
	pDetectOutput = &Output.B[nCameraSN];
	nSN = pDetectOutput->A.hvGrabberSN.I();


	SYSTEMTIME CurSysTime;
	CString strObjectFilePATH, strObjectFileName, strCommonPath, strChildPath;
	GetLocalTime(&CurSysTime);
	if (m_tWriteDisk.bEnableUpLoadImage)
	{
		strCommonPath.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\蓝标图像上传\\原图-%s\\%s", m_tWriteDisk.StrUpLoadImagePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, GetChildPath(nSN));

	}
	else
	{
		strCommonPath.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\瑕疵大图\\原图-%s\\%s", m_tWriteDisk.StrImageSavePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, GetChildPath(nSN));

	}


	CString strImageSaveName;

	if (pDetectOutput->tDetectOutput.hv_ErrorType.Length() > 0 && m_tWriteDisk.nSaveImageType == 0)
	{
		strObjectFilePATH.Format("%s\\NG", strCommonPath);
		//膜卷-品种-工位-EA号-小时-瑕疵类型-机台号
	/*	strImageSaveName.Format("%s\\%s_%02d时%02d分%02d秒-%03dms-%s_%d帧.jpg",strObjectFilePATH,Output.hvGroupID.S().Text(),CurSysTime.wHour,CurSysTime.wMinute,CurSysTime.wSecond,CurSysTime.wMilliseconds,m_ArrayErrorName[pDetectOutput->tDetectOutput.hv_ErrorType[0].I()],pDetectOutput->A.hvFrameSN.I());
*/
		strImageSaveName.Format("%s\\%s_%s_%s_%d-%02d-%02d %02d-%02d-%02d-%03d-%s_%s_%d帧_%s.jpg", strObjectFilePATH, m_strBatch, m_tInit.strProductName, GetChildName(nSN), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, m_ArrayErrorName[pDetectOutput->tDetectOutput.hv_ErrorType[0].I()], m_tWriteDisk.StrMachineSN, pDetectOutput->A.hvFrameSN.I(), Output.hvGroupID.S().Text());

	}
	else if (m_tWriteDisk.nSaveImageType == 2 && Output.tFinalResult.hv_SurfaceErrorIndex.Length() != 0 && Output.bIsFinalResult == TRUE)
	{


		strObjectFilePATH.Format("%s\\贴标NG", strCommonPath);
		strImageSaveName.Format("%s\\%s_%s_%s_%d-%02d-%02d %02d-%02d-%02d-%03d-%s_%s_%d帧_%s.jpg", strObjectFilePATH, m_strBatch, m_tInit.strProductName, GetChildName(nSN), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, m_ArrayErrorName[Output.tFinalResult.hv_SurfaceErrorIndex[0].I()], m_tWriteDisk.StrMachineSN, pDetectOutput->A.hvFrameSN.I(), Output.hvGroupID.S().Text());
	}
	else
	{
		if (m_tWriteDisk.nSaveImageType == 0 || m_tWriteDisk.nSaveImageType == 2)//0只存储NG图像 OK图像返回
		{
			return;
		}
		strObjectFilePATH.Format("%s\\OK", strCommonPath);
		strImageSaveName.Format("%s\\%s_%02d时%02d分%02d秒-%03dms_%d帧.jpg", strObjectFilePATH, Output.hvGroupID.S().Text(), CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, pDetectOutput->A.hvFrameSN.I());

	}
	HTuple bExists;
	FileExists(HTuple(strObjectFilePATH), &bExists);
	if (0 == bExists)
	{
		CreateMultilevelPath(strObjectFilePATH);
	}


	HObject hoSaveImage;

	if (m_tWriteDisk.strImageFormat == "jpg")
	{
		//WriteImage(,"jpg",0,(HTuple)strImageSaveName);
		hoSaveImage = pDetectOutput->A.hoGrabImage;
	}
	else if (m_tWriteDisk.strImageFormat == "0.5jpg")
	{
		ZoomImageFactor(pDetectOutput->A.hoGrabImage, &hoSaveImage, 0.5, 0.5, "nearest_neighbor");

	}
	else if (m_tWriteDisk.strImageFormat == "0.2jpg")
	{
		ZoomImageFactor(pDetectOutput->A.hoGrabImage, &hoSaveImage, 0.2, 0.2, "nearest_neighbor");

	}
	else
	{
		ZoomImageFactor(pDetectOutput->A.hoGrabImage, &hoSaveImage, 0.1, 0.1, "nearest_neighbor");
	}
	WriteImage(hoSaveImage, "jpg", 0, (HTuple)strImageSaveName);



}

void CStorage::Loop()
{
	HTuple hv_MessageRecv, hv_StopProcInfo, hv_TimeStart, hv_TimeEnd;
	C_ALLOCATORWORKSHOPOUTPUT Output;
	HTuple hvQueue = m_hvQueue;
	CString strBatch;
	while (TRUE)
	{
		try
		{
			DequeueMessage(hvQueue, "timeout", "infinite", &hv_MessageRecv);
			CountSeconds(&hv_TimeStart);//开始计时
			Output.ReadMessage(hv_MessageRecv);
			ClearMessage(hv_MessageRecv);
			++m_nClearFlag[Output.hvPackageType.I()];
			if (GetDiskStatus())//如果磁盘满了，不进行存储
			{
				continue;
			}
			//	CriticalSectionHelper Lock(&m_csChange);
				//先记录正面的	
			strBatch.Format("%s", m_strBatch);
			static int s_nImageSNInfo[2] = { 0 };
			int nSide = Output.hvPackageType.I();

			CString strDataPath, strPicturePath;

			strDataPath.Format("%s\\瑕疵数据", m_strLastBatchPath);
			strPicturePath.Format("%s\\瑕疵小图", m_strLastBatchPath);

			if (DirectoryExist(m_strLastBatchPath))//确认远程路径存在
			{

				if (FALSE == DirectoryExist(strDataPath))
				{
					CreateMultilevelPath(strDataPath);
				}
				if (FALSE == DirectoryExist(strPicturePath))
				{
					CreateMultilevelPath(strPicturePath);
				}

			}
			else
			{
				CString strException;
				strException.Format("存储权限不够或者存储目录:%s 不存在,请确认服务器共享属性开启和服务器无设置密码后再运行设备", m_strLastBatchPath);
				m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
				continue;
			}
			HObject hoSelect, hoImagePart;
			HTuple hv_SingleArea, hv_SingleRow, hv_SingleColumn, hv_SingleWidth, hv_SingleHeight;
			CString strErrorValue, strImageNamePath;
			HTuple hv_ImageWidth, hv_ImageHeight;
			HTuple hv_Value;
			CString strID;
			strID = Output.hvGroupID.S().TextA();
			SYSTEMTIME CurSysTime;
			GetLocalTime(&CurSysTime);

			if (m_tWriteDisk.bSaveProcessInfo)
			{
				for (int nGrabberSN = 0; nGrabberSN < m_nGrabberCount; nGrabberSN++)
				{
					B_DETECTWORKSHOPOUTPUT* pDetectOutput = NULL;
					pDetectOutput = &Output.B[nGrabberSN];
					GetImageSize(Output.B[nGrabberSN].A.hoGrabImage, &hv_ImageWidth, &hv_ImageHeight);

					if (pDetectOutput->tDetectOutput.hv_ErrorType.Length()>0)
					{

						HTuple hv_SortIndex;
						TupleSortIndex(pDetectOutput->tDetectOutput.hv_ErrorType, &hv_SortIndex);
						TupleSelect(pDetectOutput->tDetectOutput.hv_ErrorType, hv_SortIndex, &pDetectOutput->tDetectOutput.hv_ErrorType);
						SelectObj(pDetectOutput->tDetectOutput.ho_ErrorRegion, &pDetectOutput->tDetectOutput.ho_ErrorRegion, hv_SortIndex + 1);
						++s_nImageSNInfo[Output.hvPackageType.I()];

					}

					CString strSide;
					//for (int k = 0; k < MIN(5, pDetectOutput->tDetectOutput.hv_ErrorType.Length()); k++)
					for (int k = 0; k < pDetectOutput->tDetectOutput.hv_ErrorType.Length(); k++)
					{
						SelectObj(pDetectOutput->tDetectOutput.ho_ErrorRegion, &hoSelect, k + 1);
						if (pDetectOutput->tDetectOutput.hv_ErrorType[k] == 11)//AT9Size
						{
							AreaCenter(pDetectOutput->tDetectOutput.ho_Foil, &hv_SingleArea, &hv_SingleRow, &hv_SingleColumn);
							hoSelect = pDetectOutput->tDetectOutput.ho_Foil;
						}
						else
						{
							AreaCenter(hoSelect, &hv_SingleArea, &hv_SingleRow, &hv_SingleColumn);

						}

						if (pDetectOutput->A.hvGrabberSN.I() == 0)
						{
							strSide.Format("正面%d条", pDetectOutput->tDetectOutput.hv_ErrorBelongStrip[k].I() + 1);

						}
						else
						{
							strSide.Format("反面%d条", pDetectOutput->tDetectOutput.hv_ErrorBelongStrip[k].I() + 1);
						}
						strErrorValue.Format("%s-%.2f-(Y_%.1f-X_%.1f)-%s.jpg", m_ArrayErrorName.GetAt(pDetectOutput->tDetectOutput.hv_ErrorType[k].I()), hv_SingleArea.D()*m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()] * m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()], hv_SingleRow.D()* m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()], hv_SingleColumn.D()* m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()], strSide);
						strImageNamePath.Format("%s\\%s-%s", strPicturePath, strID, strErrorValue);//图像名称

						if (m_tWriteDisk.bSmallImage)
						{
							try
							{
								CropRectangle1(pDetectOutput->A.hoGrabImage, &hoImagePart, MAX(1, hv_SingleRow.D() - 100), MAX(1, hv_SingleColumn.D() - 100), MIN(hv_ImageHeight.I() - 1, hv_SingleRow.D() + 100), MIN(hv_ImageWidth.I() - 1, hv_SingleColumn.D() + 100));
								WriteImage(hoImagePart, "jpeg", 0, (HTuple)strImageNamePath);
								m_hvSingleImageIndexTuple.Append(atoi(strID.Left(strID.Find('E'))));
								m_hvSingleImagePathTuple.Append((HTuple)strImageNamePath);

							}
							catch (HException& e)
							{
								CString strException, strFontInfo, strLastInfo;
								HTuple hv_ImgWidth, hv_ImgHeight;
								GetImageSize(Output.B[0].A.hoGrabImage, &hv_ImgWidth, &hv_ImgHeight);
								strFontInfo.Format("前图像号:%d 裁切点%.1f,图像高度:%d", Output.B[0].A.hvImageSN.I(), Output.B[0].hv_CropRow.D(), hv_ImgHeight.I());
								GetImageSize(Output.B[1].A.hoGrabImage, &hv_ImgWidth, &hv_ImgHeight);
								strLastInfo.Format("后图像号:%d 裁切点%.1f,图像高度:%d", Output.B[1].A.hvImageSN.I(), Output.B[1].hv_CropRow.D(), hv_ImgHeight.I());
								strException.Format("扣图异常：函数%s 错误内容:%s,  当前图像高度:%d 坐标Row：%.1f坐标Column：%.1f  %s--%s 路径:%s", e.ProcName().TextA(), e.ErrorMessage().TextA(), hv_ImageHeight.I(), hv_SingleRow.D(), hv_SingleColumn.D(), strFontInfo, strLastInfo, strImageNamePath);
								m_tInit.pLogView->DoSomething("OPERATERLOG", (LPSTR)(LPCSTR)strException, NULL);
							}

						}

						RegionFeatures(hoSelect, (HTuple("width").Append("height")), &hv_Value);
						//描述信息子元组按照下列信息进行存储10个： [图像号，极耳的ID，相机号,错误类型名字，错误信息的行坐标，错误信息的列坐标，错误信息的宽度，错误信息的高度，错误信息的面积，错误信息的最大直径]
						/*HTuple hv_FoilSN,hv_SingleErrorType,hv_SingleRow,hv_SingleColumn,hv_SingleWidth,hv_SingleHeight,hv_SingleArea,hv_SingleMaxDiameter;
						HTuple hv_Value,hv_Diameter;*/
						HTuple hv_SingleInfo;
						hv_SingleInfo.Clear();
						hv_SingleInfo.Append(s_nImageSNInfo[Output.hvPackageType.I()]);
						/*hv_SingleInfo.Append(m_nClearFlag[Output.hvPackageType.I()]);*/
						hv_SingleInfo.Append(pDetectOutput->tDetectOutput.hv_ErrorBelongStrip[k]);
						hv_SingleInfo.Append(nGrabberSN);
						hv_SingleInfo.Append(pDetectOutput->tDetectOutput.hv_ErrorType[k]);
						hv_SingleInfo.Append(hv_SingleRow*m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()]);
						hv_SingleInfo.Append(hv_SingleColumn*m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()]);
						hv_SingleInfo.Append(hv_Value*m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()]);//
						hv_SingleInfo.Append(hv_SingleArea.D()*m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()] * m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()]);
						hv_SingleInfo.Append(1 * m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()]);
						WriteSurfaceRecord(Output, hv_SingleInfo, strDataPath);


					}
				}

			}



			if (Output.bIsFinalResult.I() == TRUE)
			{
				m_nClearFlag[Output.hvPackageType.I()] = 0;
				s_nImageSNInfo[Output.hvPackageType.I()] = 0;
			}
			//存储小图
			if (Output.bIsFinalResult.I() == TRUE)
			{

				if (Output.tFinalResult.hv_SurfaceFlag.I() == FALSE || Output.tFinalResult.hv_SizeFlag.I() == FALSE)
				{
					m_hvImageIndexTuple.Append(m_hvSingleImageIndexTuple);
					m_hvImagePathTuple.Append(m_hvSingleImagePathTuple);
					CString strImageIndexPath, strImagePath;
					strImageIndexPath.Format("%s\\ImageIndex.tup", strDataPath);
					strImagePath.Format("%s\\ImagePath.tup", strDataPath);
					if (m_hvImageIndexTuple.Length() > 0 && m_hvImagePathTuple.Length() > 0)
					{
						try
						{
							WriteTuple(m_hvImageIndexTuple, (HTuple)strImageIndexPath);
							WriteTuple(m_hvImagePathTuple, (HTuple)strImagePath);
						}
						catch (HException& e)
						{

						}

					}
				}
				m_hvSingleImageIndexTuple.Clear();
				m_hvSingleImagePathTuple.Clear();
			}
			////存储尺寸信息
			WriteRecord(Output, strDataPath);//保存数据
			WriteCPKRecord(Output, strDataPath);//保存CPK计算的数据--AT错位值
			WriteGRRRecord(Output, strDataPath);//保存GRR计算的数据--正反面AT错位值（宽度相减）

			//////////////////////存储贴标过程的数据//////////////////////////
			if (Output.bIsFinalResult.I() == TRUE && m_tWriteDisk.bSaveProcessInfo)
			{
				CString strDefectProcessDataPath;
				CString strLabelInfo;
				strDefectProcessDataPath.Format("%s\\瑕疵贴标过程数据-%s.xls", strDataPath, m_strBatch);

				if (!PathFileExists(strDefectProcessDataPath))
				{
					CreateMultilevelPath(strDataPath);

					CString strItem;
					strItem.Format("品种\t产品EA序号\t最终贴标结果\t表面检测结果\t表面报废项目\t表面缺陷数目\t表面缺陷面积\t尺寸检测结果\t尺寸报废项目\t");
					HTuple hv_FileHandle;
					try
					{
						ofstream OutFile;
						OutFile.open(strDefectProcessDataPath, ios::app | ios::out);
						OutFile << strItem << endl;
						OutFile.close();
					}
					catch (HException)
					{

					}

				}
				int nIndex = 1;
				CString   StrSurfaceResult, StrSizeResult;
				CString strSurFaceErrorName, strSurFaceErrorNum, strSurFaceErrorArea;

				if (Output.tFinalResult.hv_SurfaceFlag.I() == TRUE)
				{

					StrSurfaceResult.Format("%s\t%s\t%s\t%s", "OK", "__", "__", "__");//表面的综合结果
				}
				else
				{
					nIndex = 0;
					if (Output.tFinalResult.hv_SurfaceErrorIndex.Length() > 0)
					{
						TupleSort(Output.tFinalResult.hv_SurfaceErrorIndex, &Output.tFinalResult.hv_SurfaceErrorIndex);
						strSurFaceErrorName.Format("%s", m_ArrayErrorName[Output.tFinalResult.hv_SurfaceErrorIndex[0].I()]);//表面报错项
						HTuple hv_Index;
						hv_Index.Clear();

						TupleFind(Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorType, Output.tFinalResult.hv_SurfaceErrorIndex[0], &hv_Index);
						strSurFaceErrorNum.Format("%d", Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index[0].I()].I());//数目
						strSurFaceErrorArea.Format("%.2f", Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index[0].I()].D());//累计面积
						StrSurfaceResult.Format("%s\t%s\t%s\t%s", "NG", strSurFaceErrorName, strSurFaceErrorNum, strSurFaceErrorArea);//表面的综合结果



					}
					else
					{
						StrSurfaceResult.Format("%s\t%s\t%s\t%s", "OK", "__", "__", "__");//表面的综合结果
					}
				}

				CString strSizeErrorName, strSizeErrorNum, strSizeResult;

				if (Output.tFinalResult.hv_SizeFlag.I() == TRUE)
				{
					strSizeResult.Format("%s\t%s", "OK", "__");
				}
				else
				{
					nIndex = 0;
					if (Output.tFinalResult.hv_SizeErrorIndex.Length() > 0)
					{
						TupleSort(Output.tFinalResult.hv_SizeErrorIndex, &Output.tFinalResult.hv_SizeErrorIndex);
						strSurFaceErrorName.Format("%s", m_ArrayErrorName[Output.tFinalResult.hv_SizeErrorIndex[0].I()]);//表面报错项
						strSizeResult.Format("%s\t%s", "NG", strSurFaceErrorName);//尺寸综合结果

					}
					else
					{
						strSizeResult.Format("%s\t%s", "OK", "__");
					}

				}
				ofstream OutFile;
				OutFile.open(strDefectProcessDataPath, ios::app | ios::out);
				strLabelInfo.Format("%s\t%s\t%s\t%s\t%s\t", m_tInit.strProductName, strID, STR_Result[nIndex], StrSurfaceResult, strSizeResult);
				OutFile << strLabelInfo << endl;
				OutFile.close();
			}


			if (Output.bIsFinalResult.I() == TRUE)
			{

				if (Output.tFinalResult.hv_SizeFlag == FALSE || Output.tFinalResult.hv_SurfaceFlag == FALSE)
				{

					CString strValue, strEANumPath, strEAResultPath;
					strEANumPath.Format("%s\\EANum.tup", strDataPath);
					strEAResultPath.Format("%s\\EAResult.tup", strDataPath);
					strValue = strID.Left(strID.Find('E'));
					m_hvEANumTuple[nSide].Append(atoi(strValue));
					m_hvEAResultTuple[nSide].Append(nSide + 1);//0条NG是1 1条NG是2
					WriteTuple(m_hvEANumTuple[nSide], (HTuple)strEANumPath);
					WriteTuple(m_hvEAResultTuple[nSide], (HTuple)strEAResultPath);
				}
			}
		}
		catch (HException& e)
		{

			CString strException;
			strException.Format("存储异常函数:%s 错误内容:%s", e.ProcName().Text(), e.ErrorMessage().Text());
			m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);

		}
	}
}


int CStorage::DoSomething(void* message, void* wParam, void* lParam)
{
	CString strMessage;
	strMessage.Format("%s", (char*)message);
	if ("INITIALIZE" == strMessage)
	{
		return Init((PSTORAGEINIT)wParam);
	}
	else if ("PUSH_MESSAGE" == strMessage)
	{
		return PushMessage(*(HTuple*)wParam);
	}
	else if ("SHOW_MAP" == strMessage)
	{
		return TRUE;
	}
	else if ("GETPARAM" == strMessage)
	{
		CString strParam;
		strParam.Format("%s", wParam);
		if ("MESSAGE_NUM" == strParam)
		{
			HTuple hv_Temp, hv_ConcatTuple;
			hv_ConcatTuple.Clear();
			GetMessageQueueParam(m_hvQueue, "message_num", &hv_Temp);
			hv_ConcatTuple.Append(hv_Temp);
			GetMessageQueueParam(m_hvecStorageQueues[0].T(), "message_num", &hv_Temp);
			hv_ConcatTuple.Append(hv_Temp);
			GetMessageQueueParam(m_hvecStorageQueues[1].T(), "message_num", &hv_Temp);
			hv_ConcatTuple.Append(hv_Temp);
			GetMessageQueueParam(m_hvecStorageQueues[2].T(), "message_num", &hv_Temp);
			hv_ConcatTuple.Append(hv_Temp);
			GetMessageQueueParam(m_hvecStorageQueues[3].T(), "message_num", &hv_Temp);
			hv_ConcatTuple.Append(hv_Temp);
			*(HTuple*)lParam = hv_ConcatTuple;
			return TRUE;
		}
		else if ("SAVEPATH" == strParam)
		{

			_tcscpy((char*)lParam, m_strLastBatchPath);
			return TRUE;
		}
		else if ("GetSetSaveDirectory" == strParam)
		{

			_tcscpy((char*)lParam, m_tWriteDisk.StrImageSavePath);
			return TRUE;
		}
		else if ("CopyStaus" == strParam)
		{

			BOOL bStatus = m_CTransfer.GetStaus();
			*(int*)lParam = bStatus;
			if (bStatus)
			{
				m_CTransfer.SetStauts(FALSE);
			}


		}
		else if ("WRITEFLAG" == strParam)
		{

			*(int*)lParam = m_tWriteDisk.bWriteInLocation;

		}
		else if ("ExportStatus")
		{
			_tcscpy((char*)lParam, g_strMessage);
			return TRUE;
		}
	}
	else if ("SHOW_PARAMSET" == strMessage)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgParamSet dlg;
		dlg.m_pOwner = this;
		dlg.DoModal();
		return TRUE;
	}
	else if ("SETPARAM" == strMessage)
	{

		//CriticalSectionHelper Lock(&m_csChange);
		CString strParam;
		strParam.Format("%s", wParam);
		if ("BATCH" == strParam)//批次
		{


			m_strBatch.Format("%s", lParam);
			////基础文件夹/

			//if (m_strLastBatch!=m_strBatch)
			{
				static BOOL bFirstFlag = FALSE;
				CString strBasePath;
				SYSTEMTIME CurSysTime;
				GetLocalTime(&CurSysTime);
				m_hvEANumTuple[0].Clear();
				m_hvEAResultTuple[0].Clear();
				m_hvEANumTuple[1].Clear();
				m_hvEAResultTuple[1].Clear();
				m_hvImageIndexTuple.Clear();
				m_hvImagePathTuple.Clear();
				TupleGenConst(2, 0, &m_hvLengthSum);
				m_strReportBatchPath = m_strLastBatchPath;
				m_strReortBatch = m_strLastBatch;
				if (m_tWriteDisk.bErrorRegion && bFirstFlag)//启用生成报表
				{
					g_EventCreateReport.SetEvent();
				}
				bFirstFlag = TRUE;
				m_strLastBatch = m_strBatch;
				WritePrivateProfileString("WRITEDISK", "m_strLastBatch", m_strLastBatch, m_strConfigFile);//缓存上次的卷号
				WritePrivateProfileString("WRITEDISK", "m_strLastBatchPath", m_strLastBatchPath, m_strConfigFile);//缓存上次的卷号路径
			}
			return TRUE;
		}
		else if ("GONGWEI" == strParam)
		{

			return TRUE;
		}
		else  if ("SAVEPATH" == strParam)
		{
			m_strLastBatchPath.Format("%s", lParam);
		}
		else if ("SurfaceParam" == strParam)
		{
			m_tStandardSuface = *(STANDARDSURFACE*)lParam;
			return TRUE;
		}
		else if ("SizeParam" == strParam)
		{
			m_tStandardSize = *(STANDARDSIZE*)lParam;
			return TRUE;
		}
		else if ("JXDATAINDEX" == strParam)
		{
			m_nJXDataIndex = 0;
			return TRUE;
		}

		else if ("TestSpeed" == strParam)
		{
			return TRUE;
		}
		else if ("ExitDo" == strParam)
		{
			ExitDo();
			return TRUE;
		}
		else if ("ClearDisk" == strParam)
		{
			g_EventDelete.SetEvent();
			return TRUE;
		}
		else
		{
			CString strMessageBox;
			strMessageBox.Format("SCIStorage模块中 strMessage = %s 有误", (char*)message);
			AfxMessageBox(strMessageBox);
			return FALSE;
		}
	}


}
int CStorage::PushMessage(const HTuple& hvMessageSend)//放置消息进入队列
{
	try
	{
		EnqueueMessage(m_hvQueue, hvMessageSend, HTuple(), HTuple());//将消息投递到剔废车间
		for (int i = 0; i < m_nSaveThreadNum; i++)
		{
			EnqueueMessage(m_hvecStorageQueues[i].T(), hvMessageSend, HTuple(), HTuple());//将消息投递到剔废车间
		}
	}
	catch (HException& e)
	{
		if (H_ERR_MQOVL == e.ErrorCode())
		{
			CString strException;
			strException.Format("存储队列已满,整合线程已退出,请排查原因");
			m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
		}
		else
		{
			throw e;
		}
		return FALSE;
	}
	return TRUE;
}

void* CStorage::_Thread(void *hcthread)//io线程
{
	HDevThread*  hcppthread = (HDevThread*)hcthread;
	try
	{
		g_This->Loop();
		hcppthread->Exit();
		delete hcppthread;
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("SCIStorageDll出现异常\r\n异常信息%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_This->m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
		bool is_direct_call = hcppthread->IsDirectCall();
		hcppthread->Exit();
		delete hcppthread;
		if (is_direct_call)
			throw e;
	}
	return NULL;
}

void CStorage::ReadParam()
{
	m_strSystemVisFile.Format("%s//Config//System.vis", GetCurrentAppPath());
	m_strConfigFile.Format("%s\\Config\\Public\\SCIStorage\\Config.ini", GetCurrentAppPath());
	m_strAGrabberName.RemoveAll();
	m_nGrabberCount = GetPrivateProfileInt("GRABBER", "COUNT", 0, m_strSystemVisFile);
	int nTripNum = GetPrivateProfileInt("System", "nLaneCount", 1, m_strSystemVisFile);//单条还是双条
	for (UINT i = 0; i < m_nGrabberCount*nTripNum; i++)
	{
		CString lpAppName, lpReturnedString;
		lpAppName.Format("GRABBER%d", i);
		GetPrivateProfileString(lpAppName, "NAME", "DEF", lpReturnedString.GetBuffer(MAX_PATH), MAX_PATH, m_strSystemVisFile);
		lpReturnedString.ReleaseBuffer();
		m_strAGrabberName.Add(lpReturnedString);
	}
	//初始化保存设置

	m_tWriteDisk.bWriteInLocation = GetPrivateProfileInt("WRITEDISK", "bWriteInLocation", 0, m_strConfigFile);//读取是否写入原图片信息
	m_tWriteDisk.bSaveProcessInfo = GetPrivateProfileInt("WRITEDISK", "bSaveProcessInfo", 0, m_strConfigFile);//读取是否写入原图片信息
	m_tWriteDisk.bSmallImage = GetPrivateProfileInt("WRITEDISK", "bSmallImage", 0, m_strConfigFile);//读取是否写入小图片信息
	m_tWriteDisk.bErrorRegion = GetPrivateProfileInt("WRITEDISK", "ERRORREGION", 0, m_strConfigFile);//读取是否写入错误Region
	m_tWriteDisk.bSize = GetPrivateProfileInt("WRITEDISK", "SIZE", 0, m_strConfigFile);//读取是否写入尺寸信息
	m_tWriteDisk.nType = GetPrivateProfileInt("WRITEDISK", "TYPE", 1, m_strConfigFile);//读取是否写入尺寸信息
	m_tWriteDisk.bDeleteOK = GetPrivateProfileInt("WRITEDISK", "DELETEOK", 1, m_strConfigFile);//读取是否写入尺寸信息
	m_tWriteDisk.nSaveDays = GetPrivateProfileInt("WRITEDISK", "nSaveDays", 4, m_strConfigFile);//缓存天数

	m_strCopyPath = GetPrivateProfileCString("WRITEDISK", "m_strCopyPath", "\\\\192.168.66.6\\共享", m_strConfigFile);//拷贝路径
	m_strLastBatch = GetPrivateProfileCString("WRITEDISK", "m_strLastBatch", "8888", m_strConfigFile);//缓存上次的卷号
	m_strLastBatchPath = GetPrivateProfileCString("WRITEDISK", "m_strLastBatchPath", " ", m_strConfigFile);//缓存上次的卷号路径

	m_tWriteDisk.bSaveAllGrabberImage = GetPrivateProfileInt("WRITEDISK", "bSaveAllGrabberImage", 1, m_strConfigFile);//读取是否写入尺寸信息
	m_tWriteDisk.strImageFormat = GetPrivateProfileCString("WRITEDISK", "strImageFormat", "bmp", m_strConfigFile);//存图格式
	m_tWriteDisk.bSaveMarkImage = GetPrivateProfileInt("WRITEDISK", "bSaveMarkImage", 0, m_strConfigFile);//是否存储mark图像

	CString strDefaultPath;
	strDefaultPath.Format("%s\\Map", GetCurrentAppPath());
	m_tWriteDisk.StrImageSavePath = GetPrivateProfileCString("WRITEDISK", "StrImageSavePath", strDefaultPath, m_strConfigFile);//存图
	m_tWriteDisk.nSaveImageType = GetPrivateProfileInt("WRITEDISK", "nSaveImageType", 0, m_strConfigFile);//存图

	m_tWriteDisk.StrMachineSN = GetPrivateProfileCString("WRITEDISK", "StrMachineSN", "C1", m_strConfigFile);//存图格式
	m_tWriteDisk.StrUpLoadImagePath = GetPrivateProfileCString("WRITEDISK", "StrUpLoadImagePath", m_tWriteDisk.StrImageSavePath, m_strConfigFile);//上传路径
	m_tWriteDisk.bEnableUpLoadImage = GetPrivateProfileInt("WRITEDISK", "bEnableUpLoadImage", 0, m_strConfigFile);//是否上传蓝标图像



	m_hvLengthSum[0] = GetPrivateProfileDouble("WRITEDISK", "m_hvLengthSum0", 0, m_strConfigFile);//工位一长度
	m_hvLengthSum[1] = GetPrivateProfileDouble("WRITEDISK", "m_hvLengthSum1", 0, m_strConfigFile);//工位二长度

	int nType;
	CString strCommondhPath;
	strCommondhPath.Format("%s//Config//Common.dh", GetCurrentAppPath());
	nType = GetPrivateProfileInt("ERROR", "COUNTTYPE", 0, strCommondhPath);//缺陷种类数目
	m_ArrayErrorName.RemoveAll();
	for (int i = 0; i < nType; i++)
	{
		CString strKey, strValue;
		strKey.Format("ERROR%dNAME", i);
		GetPrivateProfileString("ERRORINFO", strKey, "漏箔", strValue.GetBuffer(MAX_PATH), MAX_PATH, strCommondhPath);
		m_ArrayErrorName.Add(strValue);
	}

	int nGrabberCount;
	nGrabberCount = GetPrivateProfileInt("GRABBER", "COUNT", 2, m_strSystemVisFile);//读取相机的个数


	for (int i = 0; i < nGrabberCount * 2; i++)
	{
		double dwRow, dwColumn;
		CString strSection;
		strSection.Format("GRABBER%d", i);
		dwRow = GetPrivateProfileDouble(strSection, "ROWRESOLUTION", 0.1, strCommondhPath);
		dwColumn = GetPrivateProfileDouble(strSection, "COLUMNRESOLUTION", 0.1, strCommondhPath);
		m_vec_RowResolution.push_back(dwRow);
		m_vec_ColumnResolution.push_back(dwColumn);
	}

	/////读取缓存信息
	CString strFilePath, strEANumPath0, strEANumPath1, strEAResultPath0, strEAResultPath1, strErrorPath0, strErrorPath1;

	strEANumPath0.Format("%s\\瑕疵数据\\EANum.tup", m_strLastBatchPath);
	strEAResultPath0.Format("%s\\瑕疵数据\\EAResult.tup", m_strLastBatchPath);
	strErrorPath0.Format("%s\\瑕疵数据\\缺陷信息.tup", m_strLastBatchPath);
	CString strImageIndexFile, strImagePathFile;
	strImageIndexFile.Format("%s\\瑕疵数据\\ImageIndex.tup", m_strLastBatchPath);
	strImagePathFile.Format("%s\\瑕疵数据\\ImagePath.tup", m_strLastBatchPath);

	m_hvEANumTuple[0].Clear();
	m_hvEAResultTuple[0].Clear();
	m_hvEANumTuple[1].Clear();
	m_hvEAResultTuple[1].Clear();
	m_hvImageIndexTuple.Clear();
	m_hvImagePathTuple.Clear();

	HTuple hv_TempTuple;
	try
	{
		ReadTuple((HTuple)strEANumPath0, &m_hvEANumTuple[0]);

	}
	catch (HException& e)
	{

	}
	try
	{
		ReadTuple((HTuple)strEANumPath1, &m_hvEANumTuple[1]);

	}
	catch (HException& e)
	{

	}

	try
	{
		ReadTuple((HTuple)strEAResultPath0, &m_hvEAResultTuple[0]);
	}
	catch (HException& e)
	{

	}
	try
	{
		ReadTuple((HTuple)strEAResultPath1, &m_hvEAResultTuple[1]);


	}
	catch (HException& e)
	{

	}

	try
	{
		ReadTuple((HTuple)strImageIndexFile, &m_hvImageIndexTuple);
	}
	catch (HException&)
	{

	}
	try
	{
		ReadTuple((HTuple)strImagePathFile, &m_hvImagePathTuple);
	}
	catch (HException)
	{

	}
















}
void CStorage::WriteParam()
{
	//初始化保存设置
	WritePrivateProfileInt("WRITEDISK", "bWriteInLocation", m_tWriteDisk.bWriteInLocation, m_strConfigFile);//读取是否写入原图片信息
	WritePrivateProfileInt("WRITEDISK", "bSaveProcessInfo", m_tWriteDisk.bSaveProcessInfo, m_strConfigFile);//读取是否写入原图片信息
	WritePrivateProfileInt("WRITEDISK", "bSmallImage", m_tWriteDisk.bSmallImage, m_strConfigFile);//读取是否写入小图片信息
	WritePrivateProfileInt("WRITEDISK", "ERRORREGION", m_tWriteDisk.bErrorRegion, m_strConfigFile);//读取是否写入错误Region
	WritePrivateProfileInt("WRITEDISK", "SIZE", m_tWriteDisk.bSize, m_strConfigFile);//读取是否写入尺寸信息
	WritePrivateProfileInt("WRITEDISK", "TYPE", m_tWriteDisk.nType, m_strConfigFile);//读取是否写入尺寸信息
	WritePrivateProfileInt("WRITEDISK", "DELETEOK", m_tWriteDisk.bDeleteOK, m_strConfigFile);//读取是否写入尺寸信息
	WritePrivateProfileInt("WRITEDISK", "nSaveDays", m_tWriteDisk.nSaveDays, m_strConfigFile);//缓存天数
	WritePrivateProfileInt("WRITEDISK", "bSaveMarkImage", m_tWriteDisk.bSaveMarkImage, m_strConfigFile);//是否存储mark图像
	WritePrivateProfileString("WRITEDISK", "StrImageSavePath", m_tWriteDisk.StrImageSavePath, m_strConfigFile);//存图路径
	WritePrivateProfileInt("WRITEDISK", "nSaveImageType", m_tWriteDisk.nSaveImageType, m_strConfigFile);//存图类型


	WritePrivateProfileInt("WRITEDISK", "bSaveAllGrabberImage", m_tWriteDisk.bSaveAllGrabberImage, m_strConfigFile);//读取是否写入尺寸信息
	WritePrivateProfileString("WRITEDISK", "strImageFormat", m_tWriteDisk.strImageFormat, m_strConfigFile);//存图格式

	WritePrivateProfileString("WRITEDISK", "m_strCopyPath", m_strCopyPath, m_strConfigFile);//拷贝路径
	WritePrivateProfileString("WRITEDISK", "StrMachineSN", m_tWriteDisk.StrMachineSN, m_strConfigFile);//机台号

	WritePrivateProfileString("WRITEDISK", "StrUpLoadImagePath", m_tWriteDisk.StrUpLoadImagePath, m_strConfigFile);//上传路径
	WritePrivateProfileInt("WRITEDISK", "bEnableUpLoadImage", m_tWriteDisk.bEnableUpLoadImage, m_strConfigFile);//是否上传蓝标图像



}
void* CStorage::HThreadStorage(void *hcthread)
{
	// +++ define thread context for this procedure
	HDevThread*  hcppthread = (HDevThread*)hcthread;
	try
	{
		// Input parameters
		const HTuple        &cbhv_SaveQueue = hcppthread->GetInputCtrlParamTuple(0);
		const HTuple        &cbhv_ThreadIndex = hcppthread->GetInputCtrlParamTuple(1);
		T_Save(cbhv_SaveQueue, cbhv_ThreadIndex);
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("HThreadStorage is exceptional(HThreadStorage线程异常):%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());

	}
	return NULL;
}

void CStorage::T_Save(HTuple hv_SaveQueue, HTuple hv_ThreadIndex)
{
	HTuple hv_MessageRecv, hv_StopProcInfo, hv_TimeStart, hv_TimeEnd;
	int nThreadInd = hv_ThreadIndex.I();
	int nTrip;
	while (0 != 1)
	{
		DequeueMessage(hv_SaveQueue, "timeout", "infinite", &hv_MessageRecv);
		C_ALLOCATORWORKSHOPOUTPUT tOutput;
		tOutput.ReadMessage(hv_MessageRecv);
		ClearMessage(hv_MessageRecv);
		nTrip = tOutput.hvPackageType.I();
		if (nThreadInd == 0 && nTrip == 0)
		{
			g_This->WriteImageFile(tOutput, 0);//保存图像
		}
		else if (nThreadInd == 1 && nTrip == 1)
		{
			g_This->WriteImageFile(tOutput, 0);//保存图像

		}
		else if (nThreadInd == 2 && nTrip == 0)
		{
			g_This->WriteImageFile(tOutput, 1);//保存图像
		}
		else if (nThreadInd == 3 && nTrip == 1)
		{
			g_This->WriteImageFile(tOutput, 1);//保存图像
		}
		Sleep(1);

	}
}
int CStorage::Init(PSTORAGEINIT pInit)//初始化
{
	SetHcppInterfaceStringEncodingIsUtf8(FALSE);
	SetSystem("filename_encoding", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("write_halcon_files_encoding", "locale");
	SetSystem("tsp_tuple_string_operator_mode", "byte");
	SetSystem("tuple_string_operator_mode", "byte");
	SetSystem("store_empty_region", "false");
	SetSystem("clip_region", "false");



	m_nSaveThreadNum = 4;//开的存储线程

	for (int k = 0; k < 2; k++)
	{
		m_nClearFlag[k] = 0;
	}
	m_tInit.pLogView = pInit->pLogView;
	m_tInit.strProductName.Format("%s", pInit->strProductName);
	m_tInit.nCoatType = pInit->nCoatType;
	g_This = this;
#pragma region "读配置文件"
	ReadParam();
#pragma endregion

#pragma region "创建队列和线程"
	CreateMessageQueue(&m_hvQueue);//创建一个IO队列
	SetMessageQueueParam(m_hvQueue, "max_message_num", 70);
	HDevThreadContext Context;
	HDevThread* pThread = new HDevThread(Context, (void*)_Thread, 0, 0);//开启一个具有2个输入的线程;
	HTuple ProcThreads;
	pThread->ParStart(&ProcThreads);
#pragma endregion

#pragma region "创建图像存储队列和线程"
	InitQueues();
	InitThread();
#pragma endregion
	AfxBeginThread((AFX_THREADPROC)ThreadDelete, NULL);
	AfxBeginThread((AFX_THREADPROC)CreateReport, NULL);//生成报表
	return TRUE;
}


void CStorage::InitQueues()
{
	for (int i = 0; i < m_nSaveThreadNum; i++)
	{
		CreateMessageQueue(&m_hvecStorageQueues[i].T());
		SetMessageQueueParam(m_hvecStorageQueues[i].T(), "max_message_num", 25);
	}
}

void CStorage::ReleaseQueues()
{
	for (int i = 0; i < m_hvecStorageQueues.Length(); i++)
	{
		SetMessageQueueParam(m_hvecStorageQueues[i].T(), "flush_queue", 1);
	}
}

BOOL CStorage::InitThread()
{
	int nStartThread = 0;
	HDevThread*         hcppthread_handle;
	HDevThreadContext   hcppthread_context;
	for (int i = 0; i < m_nSaveThreadNum; i++)//拼图线程
	{
		hcppthread_handle = new HDevThread(hcppthread_context,
			(void*)HThreadStorage, 2, 0);//开启一个具有2个输入的线程
		hcppthread_handle->SetInputCtrlParamTuple(0, m_hvecStorageQueues[i].T());
		hcppthread_handle->SetInputCtrlParamTuple(1, i);//线程编号
		hcppthread_handle->Start();
		nStartThread++;

	}
	return TRUE;
}





void CStorage::WriteSurfaceRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput, HTuple& hv_ErrorInfo, const CString& strPath)
{
	CString strDefectDataPath;

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	//[存储的图像号，极耳的ID，相机号,错误类型名字，错误信息的行坐标，错误信息的列坐标，错误信息的宽度，错误信息的高度，错误信息的面积，错误信息的最大直径]
	strDefectDataPath.Format("%s\\瑕疵过程数据-%s.xls", strPath, m_strBatch);
	if (!PathFileExists(strDefectDataPath))
	{
		CreateMultilevelPath(strPath);

		CString strItem;
		strItem.Format("品种\t产品EA序号\t条数\t相机号\t错误类型\t行坐标\t列坐标\t宽度(mm)\t高度(mm)\t面积(mm^2)\t");
		HTuple hv_FileHandle;
		try
		{
			ofstream OutFile;
			OutFile.open(strDefectDataPath, ios::app | ios::out);
			OutFile << strItem << endl;
			OutFile.close();
		}
		catch (HException)
		{

		}

	}

	HTuple hv_FileHandle;
	CString strSurfaceInfo;
	CString strID, strResult;
	//描述信息子元组按照下列信息进行存储10个： [图像号，极耳的ID，相机号,错误类型名字，错误信息的行坐标，错误信息的列坐标，错误信息的宽度，错误信息的高度，错误信息的面积，错误信息的最大直径,EAID]
	strID = tOutput.hvGroupID.S().TextA();
	try
	{
		ofstream OutFile;
		OutFile.open(strDefectDataPath, ios::app | ios::out);
		strSurfaceInfo.Format("%s\t%s\t%d\t%s\t%s\t%.2f\t%.1f\t%.1f\t%.1f\t%.1f\t", m_tInit.strProductName, strID,
			hv_ErrorInfo[1].I(),
			m_strAGrabberName[hv_ErrorInfo[2].I()],//正面反面
			m_ArrayErrorName.GetAt(hv_ErrorInfo[3].I()),
			hv_ErrorInfo[4].D(),//行坐标
			hv_ErrorInfo[5].D(),//列坐标
			hv_ErrorInfo[6].D(),//宽度
			hv_ErrorInfo[7].D(),//高度
			hv_ErrorInfo[8].D()//面积
			);
		OutFile << strSurfaceInfo << endl;
		OutFile.close();
	}
	catch (HException& e)
	{


	}



}
//void CStorage::WriteSurfaceRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
//{
//	//if (TRUE == m_tWriteDisk.bSize && TRUE == tOutput.bIsFinalResult)
//	{
//		SYSTEMTIME CurSysTime;
//		GetLocalTime(&CurSysTime);
//		CString strSurfaceRecord,strSurfacePath;
//		if (0== tOutput.hvPackageType)//左面涂布
//		{
//			strSurfaceRecord.Format("%s\\Map\\%d年%02d月\\%02d日\\%s\\%s\\第0条表面错误信息.csv",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//		}else
//		{
//			strSurfaceRecord.Format("%s\\Map\\%d年%02d月\\%02d日\\%s\\%s\\第1条表面错误信息.csv",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//		}
//			
//			
//			
//			strSurfacePath.Format("%s\\Map\\%d年%02d月\\%02d日\\%s\\%s",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//			//  [存储的图像号，极耳的ID，相机号,错误类型名字，错误信息的行坐标，错误信息的列坐标，错误信息的宽度，错误信息的高度，错误信息的面积，错误信息的最大直径]
//			if (!PathFileExists(strSurfaceRecord))
//			{
//				CreateMultilevelPath(strSurfacePath);
//				CString strItem;
//				strItem.Format("产品ID,极耳的ID,相机号,错误类型,位置(m),行坐标,列坐标,宽度(mm),高度(mm),面积(mm^2),最大直径(mm),表面结果,");
//				HTuple hv_FileHandle;
//				try
//				{
//					ofstream OutFile;
//					OutFile.open(strSurfaceRecord,ios::app|ios::out);
//					OutFile<<strItem<<endl;
//					OutFile.close();
//					//OpenFile(HTuple(strSurfaceRecord),"append",&hv_FileHandle);
//					/*FwriteString(hv_FileHandle,HTuple(strItem));
//					CloseFile(hv_FileHandle);*/
//				}catch(HException)
//				{
//
//				}
//
//			}
//
//			HTuple hv_FileHandle;
//			CString strSurfaceInfo;
//			CString strID;
//			strID=tOutput.tFinalResult.hv_ProductID.S().TextA();
//
//			try
//			{
//				OpenFile(HTuple(strSurfaceRecord),"append",&hv_FileHandle);
//				HTupleVector  hvec_ResultVector(1);
//				hvec_ResultVector=HTupleVector(g_hvec_GroupDecriptInfo[tOutput.hvPackageType.I()].T(),10);
//				for (int i=0;i<hvec_ResultVector.Length();i++)
//				{
//					
//					strSurfaceInfo.Format("%s\t%d\t%d\t%s\t%.2f\t%.1f\t%.1f\t%.2f\t%.2f\t%.2f\t%.2f\t%s\t\n",
//						strID,
//						(hvec_ResultVector[i].T())[1].I(),
//						(hvec_ResultVector[i].T())[2].I(),//相机序列号
//						 m_ArrayErrorName[(hvec_ResultVector[i].T())[3].I()],
//						 tOutput.B[tOutput.hvPackageType.I()].A.hvEncoder.D()/1000,
//						(hvec_ResultVector[i].T())[4].D(),//行坐标
//						(hvec_ResultVector[i].T())[5].D(),//列坐标
//						(hvec_ResultVector[i].T())[6].D()*m_vec_ColumnResolution[(hvec_ResultVector[i].T())[2].I()],//宽度
//						(hvec_ResultVector[i].T())[7].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()],//高度
//						(hvec_ResultVector[i].T())[8].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()]*m_vec_ColumnResolution[(hvec_ResultVector[i].T())[2].I()],//面积
//						(hvec_ResultVector[i].T())[9].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()],//最大直径
//						STR_Result[tOutput.tFinalResult.hv_SurfaceFlag.I()]);
//					FwriteString(hv_FileHandle,HTuple(strSurfaceInfo));
//				}
//				
//				CloseFile(hv_FileHandle);
//			}catch(HException& e)
//			{
//
//
//
//			}
//		
//	
//
//	}
//}

void CStorage::WriteRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput, const CString& strPath)
{
	if (TRUE == m_tWriteDisk.bSize)
	{
		SYSTEMTIME CurSysTime;
		GetLocalTime(&CurSysTime);
		CString strSizeRecord;
		strSizeRecord.Format("%s\\瑕疵尺寸过程数据-%s.xls", strPath, m_strBatch);
		if (!PathFileExists(strSizeRecord))
		{
			CreateMultilevelPath(strPath);
			CString strItem;
			ofstream outFile;
			outFile.open(strSizeRecord, ios::app | ios::out);
			if (m_tInit.nCoatType == M_TwoCam_Intermission)
			{
				CString strValue(""), strSingleItem("");
				
				for (int i = 0; i < m_nGrabberCount; i++)
				{

						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)
						{
							strSingleItem.Format("相机%d槽位%d高度\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)
						{
							strSingleItem.Format("相机%d槽位%d宽度\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)
						{
							strSingleItem.Format("相机%d槽位%dTD\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple.Length(); k++)
						{
							strSingleItem.Format("相机%d槽位%dMD\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
				}
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)
				{
					strSingleItem.Format("槽位%dTD错位\t", k + 1);
					strValue = strValue + strSingleItem;
				}
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)
				{
					strSingleItem.Format("槽位%dMD错位\t", k + 1);
					strValue = strValue + strSingleItem;
				}
				strItem.Format("品种\t产品ID\t时间\t长度(mm)\t前相机边缘位置\t后相机边缘位置\t速度(m/min)\t总长(mm)\tB面膜长\tA面膜长\tB面间歇\tA面间歇(mm)\t大错位(尾部)(mm)\t小错位(头部)(mm)\t%s", strValue);
			}
			else
			{
				CString strValue(""), strSingleItem("");

				for (int i = 0; i < m_nGrabberCount; i++)
				{
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_Name.Length(); k++)
					{

						strSingleItem.Format("相机%d%s\t", i, tOutput.B[i].tDetectOutput.hv_Name[k].S().Text());

						strValue = strValue + strSingleItem;

					}
				}
				strItem.Format("品种\t产品ID\t时间\t长度(mm)\t前相机边缘位置\t后相机边缘位置\t速度(m/min)\t%s", strValue);
			}
			outFile << strItem << endl;
			outFile.close();
		}
		CString strID, strTime;
		strID = tOutput.hvGroupID.S().TextA();
		strTime.Format("%02d:%02d:%02d %03d", CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
		HTuple hv_FileHandle, hv_SumValue;
		try
		{
			CString strSize;
			ofstream outFile;
			CString strSizeFlag;


			outFile.open(strSizeRecord, ios::app | ios::out);
			if (tOutput.bIsFinalResult == TRUE)
			{
				hv_SumValue = tOutput.tFinalResult.hv_TotalLength;
				strSizeFlag.Format("%d", tOutput.tFinalResult.hv_SizeFlag.I());



			}
			else
			{
				hv_SumValue = 0;
				strSizeFlag = "-";
			}
			HTuple hv_Column0, hv_Column1;
			HTuple hv_FLeftRow, hv_FLeftColumn, hv_FRightRow, hv_FRightColumn, hv_DLeftRow, hv_DLeftColumn, hv_DRightRow, hv_DRightColumn;
			SmallestRectangle1(tOutput.B[0].tDetectOutput.ho_Coat, &hv_FLeftRow, &hv_FLeftColumn, &hv_FRightRow, &hv_FRightColumn);
			SmallestRectangle1(tOutput.B[1].tDetectOutput.ho_Coat, &hv_DLeftRow, &hv_DLeftColumn, &hv_DRightRow, &hv_DRightColumn);
			if (tOutput.hvPackageType.I() == 0)//左侧
			{
				hv_Column0 = hv_FLeftColumn;
				hv_Column1 = hv_DLeftColumn;

			}
			else
			{
				hv_Column0 = hv_FRightColumn;
				hv_Column1 = hv_DRightColumn;
			}
			HTuple hv_ImageHeight;
			GetImageSize(tOutput.B[0].A.hoGrabImage, NULL, &hv_ImageHeight);
			m_hvLengthSum[tOutput.hvPackageType.I()] = m_hvLengthSum[tOutput.hvPackageType.I()] + hv_ImageHeight*m_vec_RowResolution[0];
			if (m_tInit.nCoatType == M_TwoCam_Intermission)
			{

				CString strValue(""), strSingleItem("");

				for (int i = 0; i < m_nGrabberCount; i++)
				{
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)//槽位高
					{

						strSingleItem.Format("%.2f\t", tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple[k].D());
						strValue = strValue + strSingleItem;

					}
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)//槽位宽
					{

						strSingleItem.Format("%.2f\t", tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple[k].D());
						strValue = strValue + strSingleItem;

					}
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)//槽位TD
					{

						strSingleItem.Format("%.2f\t", (tOutput.B[i].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*m_vec_ColumnResolution[i]);
						strValue = strValue + strSingleItem;

					}
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple.Length(); k++)
					{
						HTuple hv_CaoweiMD = MIN(tOutput.B[0].tDetectOutput.hv_BotEdagePixel, tOutput.B[1].tDetectOutput.hv_BotEdagePixel) - tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple[k];
						strSingleItem.Format("%.2f\t", hv_CaoweiMD.D()*m_vec_RowResolution[i]);
						strValue = strValue + strSingleItem;
					}
				}
				//TD错位
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)
				{
					double dForWardTd, dBackTd;
					dForWardTd = (tOutput.B[0].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*m_vec_ColumnResolution[0];
					dBackTd = (tOutput.B[1].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[1].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*m_vec_ColumnResolution[1];
					strSingleItem.Format("%.2f\t", dForWardTd - dBackTd);
					strValue = strValue + strSingleItem;
				}
				//MD错位
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)
				{
					strSingleItem.Format("%.2f\t", (tOutput.B[0].tDetectOutput.hv_CaoWeiYpositionTuple[k].D() - tOutput.B[1].tDetectOutput.hv_CaoWeiYpositionTuple[k].D())*m_vec_ColumnResolution[0]);
					strValue = strValue + strSingleItem;
				}
				strSize.Format("%s\t%s\t%s\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%s",
					m_tInit.strProductName,
					strID,
					strTime,
					m_hvLengthSum[tOutput.hvPackageType.I()].D(),
					hv_Column0.D(),
					hv_Column1.D(),
					tOutput.B[0].hv_PinTuHalfWay.D(),
					hv_SumValue.D(),
					tOutput.B[0].tDetectOutput.hv_MPLength.D(),
					tOutput.B[1].tDetectOutput.hv_MPLength.D(),
					tOutput.B[0].tDetectOutput.hv_JXHeight.D(),
					tOutput.B[1].tDetectOutput.hv_JXHeight.D(),
					tOutput.B[0].tDetectOutput.hv_TailCuoWei.D(),
					tOutput.B[0].tDetectOutput.hv_HeadCuoWei.D(),
					tOutput.hvDislocation.D(),
					strValue);
				outFile << strSize << endl;

			}

			else
			{

				CString strValue(""), strSingleItem("");

				for (int i = 0; i < m_nGrabberCount; i++)
				{
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_SortWidth.Length(); k++)
					{

						strSingleItem.Format("%.2f\t", tOutput.B[i].tDetectOutput.hv_SortWidth[k].D());
						strValue = strValue + strSingleItem;

					}
				}
				strSize.Format("%s\t%s\t%s\t%.3f\t%.3f\t%.3f\t%.3f\t%s",
					m_tInit.strProductName,
					strID,
					strTime,
					m_hvLengthSum[tOutput.hvPackageType.I()].D(),
					hv_Column0.D(),
					hv_Column1.D(),
					tOutput.B[0].hv_PinTuHalfWay.D(),
					strValue);
				outFile << strSize << endl;

			}
			outFile.close();
		}
		catch (HException& e)
		{

			int a = 0;

		}
	}
}

void CStorage::WriteCPKRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput, const CString& strPath)
{

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord;

	strSizeRecord.Format("%s\\计算CPK的数据-%s.xls", strPath, m_strBatch);
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strPath);
		CString strItem;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::app | ios::out);
		if (m_tInit.nCoatType == M_TwoCam_Continue)
		{

			CString strValue(""), strSingleItem("");


			for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length(); k++)
			{
				if (k < tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length() / 2)
				{
					strSingleItem.Format("第%d条涂布错位值\t", k + 1);
				}
				else
				{
					strSingleItem.Format("第%d条AT9错位值\t", k + 1);
				}
				strValue = strValue + strSingleItem;
			}
			strItem.Format("品种\t产品ID\t时间\t%s", strValue);
		}
		outFile << strItem << endl;
		outFile.close();
	}
	CString strID, strTime;
	strID = tOutput.hvGroupID.S().TextA();
	strTime.Format("%02d:%02d:%02d %03d", CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);

	try
	{
		CString strSize;
		ofstream outFile;
		CString strSizeFlag;
		outFile.open(strSizeRecord, ios::app | ios::out);

		CString strValue(""), strSingleItem("");
		//算错位值
		HTuple hvCuoweiTuple;
		TupleSub(tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage, tOutput.B[1].tDetectOutput.hv_AT9EdageToProEdage, &hvCuoweiTuple);
		//对CPK排序，涂布在前
		HTuple hv_CoatCuoWei, hv_CoatCpkIndex, hv_At9CuoWei, hvCPKCuoweiTuple;
		for (int i = 0; i < hvCuoweiTuple.Length() / 4; i++)
		{
			hv_CoatCuoWei.Append(hvCuoweiTuple[i + 1 + i * 3]);
			hv_CoatCuoWei.Append(hvCuoweiTuple[i + 2 + i * 3]);
			//先加入涂布错位
			hvCPKCuoweiTuple.Append(hv_CoatCuoWei);
		}
		TupleFind(hvCuoweiTuple, hv_CoatCuoWei, &hv_CoatCpkIndex);
		TupleRemove(hvCuoweiTuple, hv_CoatCpkIndex, &hv_At9CuoWei);
		//加入AT9错位
		hvCPKCuoweiTuple.Append(hv_At9CuoWei);


		for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length(); k++)
		{
			strSingleItem.Format("%0.6f\t", hvCPKCuoweiTuple[k].D());
			strValue = strValue + strSingleItem;
		}
		strSize.Format("%s\t%s\t%s\t%s",
			m_tInit.strProductName,
			strID,
			strTime,
			strValue);

		outFile << strSize << endl;

	}
	catch (const std::exception&)
	{

	}

}

void CStorage::WriteGRRRecord(C_ALLOCATORWORKSHOPOUTPUT& tOutput, const CString& strPath)
{

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord;
	if (m_tInit.nCoatType == M_TwoCam_Intermission)
	{
		return;
	}
	strSizeRecord.Format("%s\\GRR数据-%s.xls", strPath, m_strBatch);
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strPath);
		//CString strItem;
		//ofstream outFile;
		//outFile.open(strSizeRecord, ios::app | ios::out);
		//if (m_tInit.nCoatType == M_TwoCam_Continue)
		//{

		//	CString strValue(""), strSingleItem("");
		//	for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length()/2; k++)
		//	{

		//		strSingleItem.Format("第%d组AT9错位值\t", k + 1);
		//		/*strItem.Format("品种\t产品ID\t时间\t%s", strSingleItem);*/
		//		outFile << strSingleItem << endl;
		//	}
		//}
		//outFile.close();
	}
	CString strID, strTime;
	strID = tOutput.hvGroupID.S().TextA();
	strTime.Format("%02d:%02d:%02d %03d", CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);

	try
	{
		CString strSize;
		ofstream outFile;
		CString strSizeFlag;
		outFile.open(strSizeRecord, ios::app | ios::out);

		CString strValue(""), strSingleItem("");
		HTuple hv_GrrValue;
		TupleSub(tOutput.B[0].tDetectOutput.hv_CpkEdageToProEdage, tOutput.B[1].tDetectOutput.hv_CpkEdageToProEdage, &hv_GrrValue);

		int  nCount = 0;
		for (int k = 0; k < hv_GrrValue.Length(); k++)
		{
			if (nCount > 7)
			{
				nCount = 0;
			}
			strSingleItem.Format("%0.4f\t", hv_GrrValue[k].D());
			strValue = strValue + strSingleItem;
			if ((k + 1) % (tOutput.B[0].tDetectOutput.hv_CpkEdageToProEdage.Length() / (tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage.Length() / 2)) == 0)
			{
				nCount++;
				strValue = strValue;
				strSize.Format(" %s\t 涂膜错位%d\t %s",
					strTime,
					nCount,
					strValue);
				outFile << strSize << endl;
				strValue = "";
			}
		}
	}
	catch (const std::exception&)
	{

	}

}