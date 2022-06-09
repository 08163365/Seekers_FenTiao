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
//HTupleVector  g_hvec_GroupDecriptInfo(1);  //�����Ʒ��������Ϣ
//HObjectVector g_hovec_GroupImageInfo(1);//�����Ʒ����Ϣ
HTupleVector  g_hvec_DecriptInfoSN(1);  //�����Ʒ�ļ������
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
BOOL CStorage::GetDiskStatus()//��ȡӲ�̵�״̬
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
		int nWarnValue = 10;//Ӳ�̱�����״ֵ̬ ������10Gʱ����
		if (fds < nWarnValue)//�����ǰ���̿��ÿռ�С��10 �˲���ֵ������
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

			m_tInit.pLogView->DoSomething("EXCEPTION", (void*)(LPCSTR)("��ǰ���̿��ÿռ��Ѿ�С��10G,ϵͳ�Ѿ���ͣ��ͼ,��������̿ռ�"), NULL);
			bWarn = TRUE;
		}

	}
	return bFull;
}


void CStorage::ExitDo()
{


	WritePrivateProfileDouble("WRITEDISK", "m_hvLengthSum0", m_hvLengthSum[0], m_strConfigFile);//��λһ�ߴ�
	WritePrivateProfileDouble("WRITEDISK", "m_hvLengthSum1", m_hvLengthSum[1], m_strConfigFile);//��λ������

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

			/*	g_This->m_strReportBatchPath="E:\\2020����Ŀ\\A��Ŀ�ﱸ\\һ����\\����ȱ������-0827\\Run-x64\\2020��12��\\25��\\5612";
				g_This->m_strReortBatch="5612";*/


			strFilePath.Format("%s\\覴�����\\�ϴ�·����-%s.pdf", g_This->m_strReportBatchPath, g_This->m_strReortBatch);
			strEANumPath0.Format("%s\\覴�����\\EANum.tup", g_This->m_strReportBatchPath);
			strEAResultPath0.Format("%s\\覴�����\\EAResult.tup", g_This->m_strReportBatchPath);
			strErrorPath0.Format("%s\\覴�����\\ȱ����Ϣ.tup", g_This->m_strReportBatchPath);



			CString strImageIndexFile, strImagePathFile;
			strImageIndexFile.Format("%s\\覴�����\\ImageIndex.tup", g_This->m_strReportBatchPath);
			strImagePathFile.Format("%s\\覴�����\\ImagePath.tup", g_This->m_strReportBatchPath);

			HTuple hv_Exist0, hv_Exist1, hv_Exist2, hv_Exist3;
			FileExists((HTuple)strEANumPath0, &hv_Exist0);
			FileExists((HTuple)strEANumPath1, &hv_Exist1);
			FileExists((HTuple)strEAResultPath0, &hv_Exist2);
			FileExists((HTuple)strEAResultPath1, &hv_Exist3);
			if (hv_Exist0 == 0 && hv_Exist2 == 0 && hv_Exist1 == 0 && hv_Exist3 == 0)
			{
				continue;
			}

			g_strMessage.Format("��ʼ��������PDF�ļ�����������");
			if (TRUE == ReportChange.CreatePDF(strFilePath))
			{
				g_strMessage.Format("��ʼ��PDF�ļ�������������");

			}
			else
			{
				g_strMessage.Format("����PDF�ļ�ʧ�ܡ�����������");
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

					//�ϲ�A1-E1��Ԫ�񣬲����ñ���"������Ϣ����"
					SYSTEMTIME tTime;
					GetLocalTime(&tTime);
					strValue.Format("ʱ��:%d-%d-%d %02d:%02d:%02d", tTime.wYear, tTime.wMonth, tTime.wDay, tTime.wHour, tTime.wMinute, tTime.wSecond);
					ReportChange.MergeCells(2, 1, nMaxColumnNum / 2, strValue);

					strValue = "����Ա:admin";
					ReportChange.MergeCells(2, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);
					//�ϲ�A1-E1��Ԫ��

					strValue = "Ĥ����Ϣ";
					//ReportChange.MergeExcel("A3","D3",strValue,20);
					ReportChange.MergeCells(3, 1, nMaxColumnNum, strValue);

					strValue.Format("Ĥ��ţ�%s\r\nƷ����:%s", g_This->m_strReortBatch, g_This->m_tInit.strProductName);
					//ReportChange.MergeExcel("A4","B5",strValue,20);
					ReportChange.MergeCells(4, 1, nMaxColumnNum / 2, strValue);


					strValue.Format("��������%d\r\n����:%.2f",g_This->m_tStandardSize.hvTripNum.I(), g_This->m_tStandardSize.dCoatWidth.D());
					//ReportChange.MergeExcel("C4","D5",strValue,20);
					ReportChange.MergeCells(4, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);

					strValue.Format("�����");
					ReportChange.MergeCells(5, 1, nMaxColumnNum, strValue);

					CString strTemp = "";
					strValue = "";
					for (int k = 0; k < g_This->m_tStandardSuface.hv_ErrorIndex.Length(); k++)
					{
						strTemp.Format("%s:�����%.2fmm^2������Ŀ��%d���������%.2fmm^2������Ŀ��%d\r\n", g_This->m_ArrayErrorName.GetAt(g_This->m_tStandardSuface.hv_ErrorIndex[k].I()), g_This->m_tStandardSuface.hv_TotalMaxArea[k].D(), g_This->m_tStandardSuface.hv_TotalMaxNumber[k].I(), g_This->m_tStandardSuface.hv_TotalMaxAreaTwo[k].D(), g_This->m_tStandardSuface.hv_TotalMaxNumberTwo[k].I());
						strValue += strTemp;
					}

					ReportChange.AddMultText(6, 1, nMaxColumnNum, strValue);
					strValue.Format("��۲���ͳ��");
					ReportChange.AddMultText(7, 1, nMaxColumnNum, strValue);

					//strValue.Format(" ");
					//ReportChange.AddMultText(7, 3, 2, strValue);

					HTuple hv_ErrorFileExist0, hv_ErrorFileExist1;
					FileExists((HTuple)strErrorPath0, &hv_ErrorFileExist0);
					FileExists((HTuple)strErrorPath1, &hv_ErrorFileExist1);
					HTuple hv_ErrorSum, hv_TempTuple;
					if (hv_ErrorFileExist0 == 1 || hv_ErrorFileExist1 == 1)
					{
						if (hv_ErrorFileExist0 == 1)//����
						{
							ReadTuple((HTuple)strErrorPath0, &hv_TempTuple);
							hv_ErrorSum = hv_TempTuple;

						}
						if (hv_ErrorFileExist1 == 1)//����
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
						strValue.Format("EA����:%d\r\n����������:%d\r\n���ʣ�%.2f%s", nTotal, nErrorTotal, dPercent, "%");
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

						strValue.Format("EA����:--\r\n����������:--\r\n���ʣ�--");
						ReportChange.AddMultText(8, nMaxColumnNum / 2 + 1, nMaxColumnNum / 2, strValue);

					}

					strValue.Format("����������ϸ");
					ReportChange.MergeCells(9, 1, nMaxColumnNum, strValue);
					g_strMessage.Format("д���ͷ��ɣ���ʼ����������ϸ��¼������������");
					//���õڶ��е�����е�����
					const char*   ListHead[11] = { "EA���","��һ��","�ڶ���","������","������","������","������","������","�ڰ���","�ھ���","��ʮ��" };
					////���õڶ��е�����е�����
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
				g_strMessage.Format("����������ϸ��¼��ɣ���ʼ����Сͼ������������");


				//����ͼ��
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
							g_strMessage.Format("����Сͼ���%.2f������������", i*100.0 / hv_ImageIndex.Length());

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
							strValue.Format("\r\nEA���:%s\r\nȱ������:%s\r\nȱ�����:%s\r\n����Y:%s\r\n����X:%s\r\nλ��:%s\r\n", strEA, strErrorName, strArea, strY, strX, strGongWeiName);
							ReportChange.AddMultText(nRows + 1, i % nMaxColumnNum + 1, 1, strValue);
						}

					}
				}
				ReportChange.ClosePDF();
				g_strMessage.Format("���������ɣ���������");
				////����//////////////////////////////////////////////////////////////////////////
				{
					CString strDest, strTemp, strReplace, strBatchPath;
					strBatchPath = g_This->m_strReportBatchPath;
					strDest.Format("ccd");
					int nPosition = strBatchPath.Find(strDest);
					if (nPosition != -1)
					{

						strTemp = strBatchPath.Left(nPosition - strDest.GetLength() + 2);
						//Ҫȡ���ĵط�
						//strReplace.Format("D:\\Map",GetCurrentAppPath());
						strReplace = g_This->m_strCopyPath;
						if (1/*DirectoryExist(strReplace)*/)//����·����������� �������򱨾���ʾ
						{

							strBatchPath.Replace(strBatchPath.Left(nPosition - strDest.GetLength() + 2), strReplace);
							CreateMultilevelPath(strBatchPath);
							CString strSourece, strDestFile;
							strSourece.Format("%s\\覴�����", g_This->m_strReportBatchPath);
							strDestFile.Format("%s\\覴�����", strBatchPath);
							CopyFolder(strSourece, strDestFile);
							strSourece.Format("%s\\覴�Сͼ", g_This->m_strReportBatchPath);
							strDestFile.Format("%s\\覴�Сͼ", strBatchPath);
							CopyFolder(strSourece, strDestFile);
							g_This->m_CTransfer.SetStauts();

							CString strInfo;
							strInfo.Format("����%s�ļ���%s", strSourece, strDestFile);
							g_This->m_tInit.pLogView->DoSomething("OPERATERLOG", (LPSTR)(LPCSTR)strInfo, NULL);
						}
						else
						{
							CString strException;
							strException.Format("����Ŀ¼��%s������", strReplace);
							g_This->m_tInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
						}
					}
				}
			}
			catch (HException& e)
			{
				ReportChange.ClosePDF();
				g_strMessage.Format("�������ʧ�ܣ���������");
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
//			if (g_This->m_strGongWei=="UpOffset")//�ϴ�·
//			{
//				strFilePath.Format("%s\\覴�����\\�ϴ�·����-%s.xlsx",g_This->m_strReportBatchPath,g_This->m_strReortBatch);
//				strEANumPath0.Format("%s\\覴�����\\��λһ-EANum.tup",g_This->m_strReportBatchPath);
//				strEANumPath1.Format("%s\\覴�����\\��λ��-EANum.tup",g_This->m_strReportBatchPath);
//				strEAResultPath0.Format("%s\\覴�����\\��λһ-EAResult.tup",g_This->m_strReportBatchPath);
//				strEAResultPath1.Format("%s\\覴�����\\��λ��-EAResult.tup",g_This->m_strReportBatchPath);
//
//				strErrorPath0.Format("%s\\覴�����\\��λһȱ����Ϣ.tup",g_This->m_strReportBatchPath);
//				strErrorPath1.Format("%s\\覴�����\\��λ��ȱ����Ϣ.tup",g_This->m_strReportBatchPath);
//
//
//			}else
//			{
//				strFilePath.Format("%s\\覴�����\\�´�·����-%s.xlsx",g_This->m_strReportBatchPath,g_This->m_strReortBatch);
//				strEANumPath0.Format("%s\\覴�����\\��λ��-EANum.tup",g_This->m_strReportBatchPath);
//				strEANumPath1.Format("%s\\覴�����\\��λ��-EANum.tup",g_This->m_strReportBatchPath);
//				strEAResultPath0.Format("%s\\覴�����\\��λ��-EAResult.tup",g_This->m_strReportBatchPath);
//				strEAResultPath1.Format("%s\\覴�����\\��λ��-EAResult.tup",g_This->m_strReportBatchPath);
//				strErrorPath0.Format("%s\\覴�����\\��λ��ȱ����Ϣ.tup",g_This->m_strReportBatchPath);
//				strErrorPath1.Format("%s\\覴�����\\��λ��ȱ����Ϣ.tup",g_This->m_strReportBatchPath);
//
//
//			}
//			
//		
//			CString strImageIndexFile,strImagePathFile;
//			strImageIndexFile.Format("%s\\覴�����\\%s-ImageIndex.tup",g_This->m_strReportBatchPath,g_This->m_strGongWei);
//			strImagePathFile.Format("%s\\覴�����\\%s-ImagePath.tup",g_This->m_strReportBatchPath,g_This->m_strGongWei);
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
//			g_strMessage.Format("��ʼ��������Excel�ļ�����������");
//			if (TRUE==ReportChange.CreateNewExcel(strFilePath))
//			{
//
//				g_strMessage.Format("��ʼ��Excel�ļ�������������");
//
//			}else
//			{
//				g_strMessage.Format("����Excel�ļ�ʧ�ܡ�����������");
//				continue;
//			}
//
//
//			if (TRUE==ReportChange.OpenExcel(strFilePath))
//			{
//				g_strMessage.Format("��Excel�ļ���ɣ���ʼд���ͷ������������");
//			}else
//			{
//				g_strMessage.Format("��Excel�ļ�ʧ�ܡ�����������");
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
//					//�ϲ�A1-E1��Ԫ�񣬲����ñ���"������Ϣ����"
//					SYSTEMTIME tTime;
//					GetLocalTime(&tTime);
//
//					strValue.Format("ʱ��:%d-%d-%d %02d:%02d:%02d",tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
//					ReportChange.MergeExcel("A2","B2",strValue,20);
//
//					strValue="����Ա:admin";
//					ReportChange.MergeExcel("C2","D2",strValue,20);
//
//					//�ϲ�A1-E1��Ԫ�񣬲����ñ���"������Ϣ����"
//
//					strValue="Ĥ����Ϣ";
//					ReportChange.MergeExcel("A3","D3",strValue,20);
//
//
//					strValue.Format("Ĥ��ţ�%s\r\nƷ����:%s",g_This->m_strReortBatch,g_This->m_tInit.strProductName);
//					ReportChange.MergeExcel("A4","B5",strValue,20);
//
//
//					strValue.Format("��������2\r\n����:123");
//					ReportChange.MergeExcel("C4","D5",strValue,20);
//
//					strValue.Format("�����");
//					ReportChange.MergeExcel("A6","D6",strValue,20);
//
//
//					CString strTemp="";
//					strValue="";
//					for (int k=0;k<g_This->m_tStandardSuface.hv_ErrorIndex.Length();k++)
//					{
//						strTemp.Format("%s:�����%.2fmm^2������Ŀ��%d���������%.2fmm^2������Ŀ��%d\r\n",g_This->m_ArrayErrorName.GetAt(g_This->m_tStandardSuface.hv_ErrorIndex[k].I()),g_This->m_tStandardSuface.hv_TotalMaxArea[k].D(),g_This->m_tStandardSuface.hv_TotalMaxNumber[k].I(),g_This->m_tStandardSuface.hv_TotalMaxAreaTwo[k].D(),g_This->m_tStandardSuface.hv_TotalMaxNumberTwo[k].I());
//						strValue+=strTemp;
//					}
//					ReportChange.MergeExcel("A7","D15",strValue,20);
//					strValue.Format("��۲���ͳ��");
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
//						if (hv_ErrorFileExist0==1)//����
//						{
//							ReadTuple((HTuple)strErrorPath0,&hv_TempTuple);
//							hv_ErrorSum=hv_TempTuple;
//
//						}
//						if (hv_ErrorFileExist1==1)//����
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
//						strValue.Format("EA����:%d\r\n����������:%d\r\n���ʣ�%.2f%s",nTotal,nErrorTotal,dPercent,"%");
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
//						strValue.Format("EA����:--\r\n����������:--\r\n���ʣ�--");
//						ReportChange.MergeExcel("C17","D25",strValue,20);
//
//					}
//
//
//					strValue.Format("����������ϸ");
//					ReportChange.MergeExcel("A26","D26",strValue,20);
//					g_strMessage.Format("д���ͷ��ɣ���ʼ����������ϸ��¼������������");
//					//���õڶ��е�����е�����
//					const char*   ListHead[4]={"EA���","��һ��","�ڶ���","��ע"};
//					////���õڶ��е�����е�����
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
//			    g_strMessage.Format("����������ϸ��¼��ɣ���ʼ����Сͼ������������");
//
//				//����ͼ��
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
//				//			g_strMessage.Format("����Сͼ���%.2f������������",i*100.0/hv_ImageIndex.Length());
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
//				//			strValue.Format("\r\nEA���:%s\r\nȱ������:%s\r\nȱ�����:%s\r\n����Y:%s\r\n����X:%s\r\nλ��:%s\r\n",strEA,strErrorName,strArea,strY,strX,strGongWeiName);
//				//			//strValue.Format("EA���:%d",hv_ImageIndex[i].I());
//				//			ReportChange.SetCellText(nRows+2,i%4,strValue,120);
//				//			//ReportChange.Save();
//
//
//				//		}
//
//				//	}
//				//}
//		    	g_strMessage.Format("���������ɣ���������");
//    		}catch(HException& e)
//			{
//
//			}
//			ReportChange.AddBorderLine();//���Ʊ߿�
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
			strException.Format("��ʼ�ֶ�������ͼ����ȴ�ɾ����ɡ���������������������");
			(g_This->m_tInit.pLogView)->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);

			CString strSavePath;
			strSavePath.Format("%s\\ccd_data", g_This->m_tWriteDisk.StrImageSavePath);
			DeleteOldFileFolder(strSavePath, g_This->m_tWriteDisk.nSaveDays);
			g_EventDelete.ResetEvent();

			strException.Format("��������������������������");
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
			strObjectFilePATH.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\覴ô�ͼ\\Markͼ��-%s\\%d��λ", m_tWriteDisk.StrImageSavePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, nSide + 1);
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
		strChildPath = "��λһ\\����";
		break;
	case 1:
		strChildPath = "��λ��\\����";
		break;
	case 2:
		strChildPath = "��λһ\\����";
		break;
	case 3:
		strChildPath = "��λ��\\����";
		break;
	default:
		strChildPath = "��λ��\\����";
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
		strChildPath = "��λһ����";
		break;
	case 1:
		strChildPath = "��λ������";
		break;
	case 2:
		strChildPath = "��λһ����";
		break;
	case 3:
		strChildPath = "��λ������";
		break;
	default:
		strChildPath = "��λ������";
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
		strCommonPath.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\����ͼ���ϴ�\\ԭͼ-%s\\%s", m_tWriteDisk.StrUpLoadImagePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, GetChildPath(nSN));

	}
	else
	{
		strCommonPath.Format("%s\\ccd_data\\%02d-%d\\%d\\%s-%s\\覴ô�ͼ\\ԭͼ-%s\\%s", m_tWriteDisk.StrImageSavePath, CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_tInit.strProductName, m_strBatch, m_strBatch, GetChildPath(nSN));

	}


	CString strImageSaveName;

	if (pDetectOutput->tDetectOutput.hv_ErrorType.Length() > 0 && m_tWriteDisk.nSaveImageType == 0)
	{
		strObjectFilePATH.Format("%s\\NG", strCommonPath);
		//Ĥ��-Ʒ��-��λ-EA��-Сʱ-覴�����-��̨��
	/*	strImageSaveName.Format("%s\\%s_%02dʱ%02d��%02d��-%03dms-%s_%d֡.jpg",strObjectFilePATH,Output.hvGroupID.S().Text(),CurSysTime.wHour,CurSysTime.wMinute,CurSysTime.wSecond,CurSysTime.wMilliseconds,m_ArrayErrorName[pDetectOutput->tDetectOutput.hv_ErrorType[0].I()],pDetectOutput->A.hvFrameSN.I());
*/
		strImageSaveName.Format("%s\\%s_%s_%s_%d-%02d-%02d %02d-%02d-%02d-%03d-%s_%s_%d֡_%s.jpg", strObjectFilePATH, m_strBatch, m_tInit.strProductName, GetChildName(nSN), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, m_ArrayErrorName[pDetectOutput->tDetectOutput.hv_ErrorType[0].I()], m_tWriteDisk.StrMachineSN, pDetectOutput->A.hvFrameSN.I(), Output.hvGroupID.S().Text());

	}
	else if (m_tWriteDisk.nSaveImageType == 2 && Output.tFinalResult.hv_SurfaceErrorIndex.Length() != 0 && Output.bIsFinalResult == TRUE)
	{


		strObjectFilePATH.Format("%s\\����NG", strCommonPath);
		strImageSaveName.Format("%s\\%s_%s_%s_%d-%02d-%02d %02d-%02d-%02d-%03d-%s_%s_%d֡_%s.jpg", strObjectFilePATH, m_strBatch, m_tInit.strProductName, GetChildName(nSN), CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, m_ArrayErrorName[Output.tFinalResult.hv_SurfaceErrorIndex[0].I()], m_tWriteDisk.StrMachineSN, pDetectOutput->A.hvFrameSN.I(), Output.hvGroupID.S().Text());
	}
	else
	{
		if (m_tWriteDisk.nSaveImageType == 0 || m_tWriteDisk.nSaveImageType == 2)//0ֻ�洢NGͼ�� OKͼ�񷵻�
		{
			return;
		}
		strObjectFilePATH.Format("%s\\OK", strCommonPath);
		strImageSaveName.Format("%s\\%s_%02dʱ%02d��%02d��-%03dms_%d֡.jpg", strObjectFilePATH, Output.hvGroupID.S().Text(), CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds, pDetectOutput->A.hvFrameSN.I());

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
			CountSeconds(&hv_TimeStart);//��ʼ��ʱ
			Output.ReadMessage(hv_MessageRecv);
			ClearMessage(hv_MessageRecv);
			++m_nClearFlag[Output.hvPackageType.I()];
			if (GetDiskStatus())//����������ˣ������д洢
			{
				continue;
			}
			//	CriticalSectionHelper Lock(&m_csChange);
				//�ȼ�¼�����	
			strBatch.Format("%s", m_strBatch);
			static int s_nImageSNInfo[2] = { 0 };
			int nSide = Output.hvPackageType.I();

			CString strDataPath, strPicturePath;

			strDataPath.Format("%s\\覴�����", m_strLastBatchPath);
			strPicturePath.Format("%s\\覴�Сͼ", m_strLastBatchPath);

			if (DirectoryExist(m_strLastBatchPath))//ȷ��Զ��·������
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
				strException.Format("�洢Ȩ�޲������ߴ洢Ŀ¼:%s ������,��ȷ�Ϸ������������Կ����ͷ�����������������������豸", m_strLastBatchPath);
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
							strSide.Format("����%d��", pDetectOutput->tDetectOutput.hv_ErrorBelongStrip[k].I() + 1);

						}
						else
						{
							strSide.Format("����%d��", pDetectOutput->tDetectOutput.hv_ErrorBelongStrip[k].I() + 1);
						}
						strErrorValue.Format("%s-%.2f-(Y_%.1f-X_%.1f)-%s.jpg", m_ArrayErrorName.GetAt(pDetectOutput->tDetectOutput.hv_ErrorType[k].I()), hv_SingleArea.D()*m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()] * m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()], hv_SingleRow.D()* m_vec_RowResolution[pDetectOutput->A.hvGrabberSN.I()], hv_SingleColumn.D()* m_vec_ColumnResolution[pDetectOutput->A.hvGrabberSN.I()], strSide);
						strImageNamePath.Format("%s\\%s-%s", strPicturePath, strID, strErrorValue);//ͼ������

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
								strFontInfo.Format("ǰͼ���:%d ���е�%.1f,ͼ��߶�:%d", Output.B[0].A.hvImageSN.I(), Output.B[0].hv_CropRow.D(), hv_ImgHeight.I());
								GetImageSize(Output.B[1].A.hoGrabImage, &hv_ImgWidth, &hv_ImgHeight);
								strLastInfo.Format("��ͼ���:%d ���е�%.1f,ͼ��߶�:%d", Output.B[1].A.hvImageSN.I(), Output.B[1].hv_CropRow.D(), hv_ImgHeight.I());
								strException.Format("��ͼ�쳣������%s ��������:%s,  ��ǰͼ��߶�:%d ����Row��%.1f����Column��%.1f  %s--%s ·��:%s", e.ProcName().TextA(), e.ErrorMessage().TextA(), hv_ImageHeight.I(), hv_SingleRow.D(), hv_SingleColumn.D(), strFontInfo, strLastInfo, strImageNamePath);
								m_tInit.pLogView->DoSomething("OPERATERLOG", (LPSTR)(LPCSTR)strException, NULL);
							}

						}

						RegionFeatures(hoSelect, (HTuple("width").Append("height")), &hv_Value);
						//������Ϣ��Ԫ�鰴��������Ϣ���д洢10���� [ͼ��ţ�������ID�������,�����������֣�������Ϣ�������꣬������Ϣ�������꣬������Ϣ�Ŀ�ȣ�������Ϣ�ĸ߶ȣ�������Ϣ�������������Ϣ�����ֱ��]
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
			//�洢Сͼ
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
			////�洢�ߴ���Ϣ
			WriteRecord(Output, strDataPath);//��������
			WriteCPKRecord(Output, strDataPath);//����CPK���������--AT��λֵ
			WriteGRRRecord(Output, strDataPath);//����GRR���������--������AT��λֵ����������

			//////////////////////�洢������̵�����//////////////////////////
			if (Output.bIsFinalResult.I() == TRUE && m_tWriteDisk.bSaveProcessInfo)
			{
				CString strDefectProcessDataPath;
				CString strLabelInfo;
				strDefectProcessDataPath.Format("%s\\覴������������-%s.xls", strDataPath, m_strBatch);

				if (!PathFileExists(strDefectProcessDataPath))
				{
					CreateMultilevelPath(strDataPath);

					CString strItem;
					strItem.Format("Ʒ��\t��ƷEA���\t����������\t��������\t���汨����Ŀ\t����ȱ����Ŀ\t����ȱ�����\t�ߴ�����\t�ߴ籨����Ŀ\t");
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

					StrSurfaceResult.Format("%s\t%s\t%s\t%s", "OK", "__", "__", "__");//������ۺϽ��
				}
				else
				{
					nIndex = 0;
					if (Output.tFinalResult.hv_SurfaceErrorIndex.Length() > 0)
					{
						TupleSort(Output.tFinalResult.hv_SurfaceErrorIndex, &Output.tFinalResult.hv_SurfaceErrorIndex);
						strSurFaceErrorName.Format("%s", m_ArrayErrorName[Output.tFinalResult.hv_SurfaceErrorIndex[0].I()]);//���汨����
						HTuple hv_Index;
						hv_Index.Clear();

						TupleFind(Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorType, Output.tFinalResult.hv_SurfaceErrorIndex[0], &hv_Index);
						strSurFaceErrorNum.Format("%d", Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index[0].I()].I());//��Ŀ
						strSurFaceErrorArea.Format("%.2f", Output.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index[0].I()].D());//�ۼ����
						StrSurfaceResult.Format("%s\t%s\t%s\t%s", "NG", strSurFaceErrorName, strSurFaceErrorNum, strSurFaceErrorArea);//������ۺϽ��



					}
					else
					{
						StrSurfaceResult.Format("%s\t%s\t%s\t%s", "OK", "__", "__", "__");//������ۺϽ��
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
						strSurFaceErrorName.Format("%s", m_ArrayErrorName[Output.tFinalResult.hv_SizeErrorIndex[0].I()]);//���汨����
						strSizeResult.Format("%s\t%s", "NG", strSurFaceErrorName);//�ߴ��ۺϽ��

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
					m_hvEAResultTuple[nSide].Append(nSide + 1);//0��NG��1 1��NG��2
					WriteTuple(m_hvEANumTuple[nSide], (HTuple)strEANumPath);
					WriteTuple(m_hvEAResultTuple[nSide], (HTuple)strEAResultPath);
				}
			}
		}
		catch (HException& e)
		{

			CString strException;
			strException.Format("�洢�쳣����:%s ��������:%s", e.ProcName().Text(), e.ErrorMessage().Text());
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
		if ("BATCH" == strParam)//����
		{


			m_strBatch.Format("%s", lParam);
			////�����ļ���/

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
				if (m_tWriteDisk.bErrorRegion && bFirstFlag)//�������ɱ���
				{
					g_EventCreateReport.SetEvent();
				}
				bFirstFlag = TRUE;
				m_strLastBatch = m_strBatch;
				WritePrivateProfileString("WRITEDISK", "m_strLastBatch", m_strLastBatch, m_strConfigFile);//�����ϴεľ��
				WritePrivateProfileString("WRITEDISK", "m_strLastBatchPath", m_strLastBatchPath, m_strConfigFile);//�����ϴεľ��·��
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
			strMessageBox.Format("SCIStorageģ���� strMessage = %s ����", (char*)message);
			AfxMessageBox(strMessageBox);
			return FALSE;
		}
	}


}
int CStorage::PushMessage(const HTuple& hvMessageSend)//������Ϣ�������
{
	try
	{
		EnqueueMessage(m_hvQueue, hvMessageSend, HTuple(), HTuple());//����ϢͶ�ݵ��޷ϳ���
		for (int i = 0; i < m_nSaveThreadNum; i++)
		{
			EnqueueMessage(m_hvecStorageQueues[i].T(), hvMessageSend, HTuple(), HTuple());//����ϢͶ�ݵ��޷ϳ���
		}
	}
	catch (HException& e)
	{
		if (H_ERR_MQOVL == e.ErrorCode())
		{
			CString strException;
			strException.Format("�洢��������,�����߳����˳�,���Ų�ԭ��");
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

void* CStorage::_Thread(void *hcthread)//io�߳�
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
		strException.Format("SCIStorageDll�����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
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
	int nTripNum = GetPrivateProfileInt("System", "nLaneCount", 1, m_strSystemVisFile);//��������˫��
	for (UINT i = 0; i < m_nGrabberCount*nTripNum; i++)
	{
		CString lpAppName, lpReturnedString;
		lpAppName.Format("GRABBER%d", i);
		GetPrivateProfileString(lpAppName, "NAME", "DEF", lpReturnedString.GetBuffer(MAX_PATH), MAX_PATH, m_strSystemVisFile);
		lpReturnedString.ReleaseBuffer();
		m_strAGrabberName.Add(lpReturnedString);
	}
	//��ʼ����������

	m_tWriteDisk.bWriteInLocation = GetPrivateProfileInt("WRITEDISK", "bWriteInLocation", 0, m_strConfigFile);//��ȡ�Ƿ�д��ԭͼƬ��Ϣ
	m_tWriteDisk.bSaveProcessInfo = GetPrivateProfileInt("WRITEDISK", "bSaveProcessInfo", 0, m_strConfigFile);//��ȡ�Ƿ�д��ԭͼƬ��Ϣ
	m_tWriteDisk.bSmallImage = GetPrivateProfileInt("WRITEDISK", "bSmallImage", 0, m_strConfigFile);//��ȡ�Ƿ�д��СͼƬ��Ϣ
	m_tWriteDisk.bErrorRegion = GetPrivateProfileInt("WRITEDISK", "ERRORREGION", 0, m_strConfigFile);//��ȡ�Ƿ�д�����Region
	m_tWriteDisk.bSize = GetPrivateProfileInt("WRITEDISK", "SIZE", 0, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	m_tWriteDisk.nType = GetPrivateProfileInt("WRITEDISK", "TYPE", 1, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	m_tWriteDisk.bDeleteOK = GetPrivateProfileInt("WRITEDISK", "DELETEOK", 1, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	m_tWriteDisk.nSaveDays = GetPrivateProfileInt("WRITEDISK", "nSaveDays", 4, m_strConfigFile);//��������

	m_strCopyPath = GetPrivateProfileCString("WRITEDISK", "m_strCopyPath", "\\\\192.168.66.6\\����", m_strConfigFile);//����·��
	m_strLastBatch = GetPrivateProfileCString("WRITEDISK", "m_strLastBatch", "8888", m_strConfigFile);//�����ϴεľ��
	m_strLastBatchPath = GetPrivateProfileCString("WRITEDISK", "m_strLastBatchPath", " ", m_strConfigFile);//�����ϴεľ��·��

	m_tWriteDisk.bSaveAllGrabberImage = GetPrivateProfileInt("WRITEDISK", "bSaveAllGrabberImage", 1, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	m_tWriteDisk.strImageFormat = GetPrivateProfileCString("WRITEDISK", "strImageFormat", "bmp", m_strConfigFile);//��ͼ��ʽ
	m_tWriteDisk.bSaveMarkImage = GetPrivateProfileInt("WRITEDISK", "bSaveMarkImage", 0, m_strConfigFile);//�Ƿ�洢markͼ��

	CString strDefaultPath;
	strDefaultPath.Format("%s\\Map", GetCurrentAppPath());
	m_tWriteDisk.StrImageSavePath = GetPrivateProfileCString("WRITEDISK", "StrImageSavePath", strDefaultPath, m_strConfigFile);//��ͼ
	m_tWriteDisk.nSaveImageType = GetPrivateProfileInt("WRITEDISK", "nSaveImageType", 0, m_strConfigFile);//��ͼ

	m_tWriteDisk.StrMachineSN = GetPrivateProfileCString("WRITEDISK", "StrMachineSN", "C1", m_strConfigFile);//��ͼ��ʽ
	m_tWriteDisk.StrUpLoadImagePath = GetPrivateProfileCString("WRITEDISK", "StrUpLoadImagePath", m_tWriteDisk.StrImageSavePath, m_strConfigFile);//�ϴ�·��
	m_tWriteDisk.bEnableUpLoadImage = GetPrivateProfileInt("WRITEDISK", "bEnableUpLoadImage", 0, m_strConfigFile);//�Ƿ��ϴ�����ͼ��



	m_hvLengthSum[0] = GetPrivateProfileDouble("WRITEDISK", "m_hvLengthSum0", 0, m_strConfigFile);//��λһ����
	m_hvLengthSum[1] = GetPrivateProfileDouble("WRITEDISK", "m_hvLengthSum1", 0, m_strConfigFile);//��λ������

	int nType;
	CString strCommondhPath;
	strCommondhPath.Format("%s//Config//Common.dh", GetCurrentAppPath());
	nType = GetPrivateProfileInt("ERROR", "COUNTTYPE", 0, strCommondhPath);//ȱ��������Ŀ
	m_ArrayErrorName.RemoveAll();
	for (int i = 0; i < nType; i++)
	{
		CString strKey, strValue;
		strKey.Format("ERROR%dNAME", i);
		GetPrivateProfileString("ERRORINFO", strKey, "©��", strValue.GetBuffer(MAX_PATH), MAX_PATH, strCommondhPath);
		m_ArrayErrorName.Add(strValue);
	}

	int nGrabberCount;
	nGrabberCount = GetPrivateProfileInt("GRABBER", "COUNT", 2, m_strSystemVisFile);//��ȡ����ĸ���


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

	/////��ȡ������Ϣ
	CString strFilePath, strEANumPath0, strEANumPath1, strEAResultPath0, strEAResultPath1, strErrorPath0, strErrorPath1;

	strEANumPath0.Format("%s\\覴�����\\EANum.tup", m_strLastBatchPath);
	strEAResultPath0.Format("%s\\覴�����\\EAResult.tup", m_strLastBatchPath);
	strErrorPath0.Format("%s\\覴�����\\ȱ����Ϣ.tup", m_strLastBatchPath);
	CString strImageIndexFile, strImagePathFile;
	strImageIndexFile.Format("%s\\覴�����\\ImageIndex.tup", m_strLastBatchPath);
	strImagePathFile.Format("%s\\覴�����\\ImagePath.tup", m_strLastBatchPath);

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
	//��ʼ����������
	WritePrivateProfileInt("WRITEDISK", "bWriteInLocation", m_tWriteDisk.bWriteInLocation, m_strConfigFile);//��ȡ�Ƿ�д��ԭͼƬ��Ϣ
	WritePrivateProfileInt("WRITEDISK", "bSaveProcessInfo", m_tWriteDisk.bSaveProcessInfo, m_strConfigFile);//��ȡ�Ƿ�д��ԭͼƬ��Ϣ
	WritePrivateProfileInt("WRITEDISK", "bSmallImage", m_tWriteDisk.bSmallImage, m_strConfigFile);//��ȡ�Ƿ�д��СͼƬ��Ϣ
	WritePrivateProfileInt("WRITEDISK", "ERRORREGION", m_tWriteDisk.bErrorRegion, m_strConfigFile);//��ȡ�Ƿ�д�����Region
	WritePrivateProfileInt("WRITEDISK", "SIZE", m_tWriteDisk.bSize, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	WritePrivateProfileInt("WRITEDISK", "TYPE", m_tWriteDisk.nType, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	WritePrivateProfileInt("WRITEDISK", "DELETEOK", m_tWriteDisk.bDeleteOK, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	WritePrivateProfileInt("WRITEDISK", "nSaveDays", m_tWriteDisk.nSaveDays, m_strConfigFile);//��������
	WritePrivateProfileInt("WRITEDISK", "bSaveMarkImage", m_tWriteDisk.bSaveMarkImage, m_strConfigFile);//�Ƿ�洢markͼ��
	WritePrivateProfileString("WRITEDISK", "StrImageSavePath", m_tWriteDisk.StrImageSavePath, m_strConfigFile);//��ͼ·��
	WritePrivateProfileInt("WRITEDISK", "nSaveImageType", m_tWriteDisk.nSaveImageType, m_strConfigFile);//��ͼ����


	WritePrivateProfileInt("WRITEDISK", "bSaveAllGrabberImage", m_tWriteDisk.bSaveAllGrabberImage, m_strConfigFile);//��ȡ�Ƿ�д��ߴ���Ϣ
	WritePrivateProfileString("WRITEDISK", "strImageFormat", m_tWriteDisk.strImageFormat, m_strConfigFile);//��ͼ��ʽ

	WritePrivateProfileString("WRITEDISK", "m_strCopyPath", m_strCopyPath, m_strConfigFile);//����·��
	WritePrivateProfileString("WRITEDISK", "StrMachineSN", m_tWriteDisk.StrMachineSN, m_strConfigFile);//��̨��

	WritePrivateProfileString("WRITEDISK", "StrUpLoadImagePath", m_tWriteDisk.StrUpLoadImagePath, m_strConfigFile);//�ϴ�·��
	WritePrivateProfileInt("WRITEDISK", "bEnableUpLoadImage", m_tWriteDisk.bEnableUpLoadImage, m_strConfigFile);//�Ƿ��ϴ�����ͼ��



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
		strException.Format("HThreadStorage is exceptional(HThreadStorage�߳��쳣):%s  [ %s() ]", e.ErrorMessage().Text()
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
			g_This->WriteImageFile(tOutput, 0);//����ͼ��
		}
		else if (nThreadInd == 1 && nTrip == 1)
		{
			g_This->WriteImageFile(tOutput, 0);//����ͼ��

		}
		else if (nThreadInd == 2 && nTrip == 0)
		{
			g_This->WriteImageFile(tOutput, 1);//����ͼ��
		}
		else if (nThreadInd == 3 && nTrip == 1)
		{
			g_This->WriteImageFile(tOutput, 1);//����ͼ��
		}
		Sleep(1);

	}
}
int CStorage::Init(PSTORAGEINIT pInit)//��ʼ��
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



	m_nSaveThreadNum = 4;//���Ĵ洢�߳�

	for (int k = 0; k < 2; k++)
	{
		m_nClearFlag[k] = 0;
	}
	m_tInit.pLogView = pInit->pLogView;
	m_tInit.strProductName.Format("%s", pInit->strProductName);
	m_tInit.nCoatType = pInit->nCoatType;
	g_This = this;
#pragma region "�������ļ�"
	ReadParam();
#pragma endregion

#pragma region "�������к��߳�"
	CreateMessageQueue(&m_hvQueue);//����һ��IO����
	SetMessageQueueParam(m_hvQueue, "max_message_num", 70);
	HDevThreadContext Context;
	HDevThread* pThread = new HDevThread(Context, (void*)_Thread, 0, 0);//����һ������2��������߳�;
	HTuple ProcThreads;
	pThread->ParStart(&ProcThreads);
#pragma endregion

#pragma region "����ͼ��洢���к��߳�"
	InitQueues();
	InitThread();
#pragma endregion
	AfxBeginThread((AFX_THREADPROC)ThreadDelete, NULL);
	AfxBeginThread((AFX_THREADPROC)CreateReport, NULL);//���ɱ���
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
	for (int i = 0; i < m_nSaveThreadNum; i++)//ƴͼ�߳�
	{
		hcppthread_handle = new HDevThread(hcppthread_context,
			(void*)HThreadStorage, 2, 0);//����һ������2��������߳�
		hcppthread_handle->SetInputCtrlParamTuple(0, m_hvecStorageQueues[i].T());
		hcppthread_handle->SetInputCtrlParamTuple(1, i);//�̱߳��
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
	//[�洢��ͼ��ţ�������ID�������,�����������֣�������Ϣ�������꣬������Ϣ�������꣬������Ϣ�Ŀ�ȣ�������Ϣ�ĸ߶ȣ�������Ϣ�������������Ϣ�����ֱ��]
	strDefectDataPath.Format("%s\\覴ù�������-%s.xls", strPath, m_strBatch);
	if (!PathFileExists(strDefectDataPath))
	{
		CreateMultilevelPath(strPath);

		CString strItem;
		strItem.Format("Ʒ��\t��ƷEA���\t����\t�����\t��������\t������\t������\t���(mm)\t�߶�(mm)\t���(mm^2)\t");
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
	//������Ϣ��Ԫ�鰴��������Ϣ���д洢10���� [ͼ��ţ�������ID�������,�����������֣�������Ϣ�������꣬������Ϣ�������꣬������Ϣ�Ŀ�ȣ�������Ϣ�ĸ߶ȣ�������Ϣ�������������Ϣ�����ֱ��,EAID]
	strID = tOutput.hvGroupID.S().TextA();
	try
	{
		ofstream OutFile;
		OutFile.open(strDefectDataPath, ios::app | ios::out);
		strSurfaceInfo.Format("%s\t%s\t%d\t%s\t%s\t%.2f\t%.1f\t%.1f\t%.1f\t%.1f\t", m_tInit.strProductName, strID,
			hv_ErrorInfo[1].I(),
			m_strAGrabberName[hv_ErrorInfo[2].I()],//���淴��
			m_ArrayErrorName.GetAt(hv_ErrorInfo[3].I()),
			hv_ErrorInfo[4].D(),//������
			hv_ErrorInfo[5].D(),//������
			hv_ErrorInfo[6].D(),//���
			hv_ErrorInfo[7].D(),//�߶�
			hv_ErrorInfo[8].D()//���
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
//		if (0== tOutput.hvPackageType)//����Ϳ��
//		{
//			strSurfaceRecord.Format("%s\\Map\\%d��%02d��\\%02d��\\%s\\%s\\��0�����������Ϣ.csv",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//		}else
//		{
//			strSurfaceRecord.Format("%s\\Map\\%d��%02d��\\%02d��\\%s\\%s\\��1�����������Ϣ.csv",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//		}
//			
//			
//			
//			strSurfacePath.Format("%s\\Map\\%d��%02d��\\%02d��\\%s\\%s",GetCurrentAppPath(),CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay,
//				m_strBatch,m_tInit.strProductName);
//			//  [�洢��ͼ��ţ�������ID�������,�����������֣�������Ϣ�������꣬������Ϣ�������꣬������Ϣ�Ŀ�ȣ�������Ϣ�ĸ߶ȣ�������Ϣ�������������Ϣ�����ֱ��]
//			if (!PathFileExists(strSurfaceRecord))
//			{
//				CreateMultilevelPath(strSurfacePath);
//				CString strItem;
//				strItem.Format("��ƷID,������ID,�����,��������,λ��(m),������,������,���(mm),�߶�(mm),���(mm^2),���ֱ��(mm),������,");
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
//						(hvec_ResultVector[i].T())[2].I(),//������к�
//						 m_ArrayErrorName[(hvec_ResultVector[i].T())[3].I()],
//						 tOutput.B[tOutput.hvPackageType.I()].A.hvEncoder.D()/1000,
//						(hvec_ResultVector[i].T())[4].D(),//������
//						(hvec_ResultVector[i].T())[5].D(),//������
//						(hvec_ResultVector[i].T())[6].D()*m_vec_ColumnResolution[(hvec_ResultVector[i].T())[2].I()],//���
//						(hvec_ResultVector[i].T())[7].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()],//�߶�
//						(hvec_ResultVector[i].T())[8].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()]*m_vec_ColumnResolution[(hvec_ResultVector[i].T())[2].I()],//���
//						(hvec_ResultVector[i].T())[9].D()*m_vec_RowResolution[(hvec_ResultVector[i].T())[2].I()],//���ֱ��
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
		strSizeRecord.Format("%s\\覴óߴ��������-%s.xls", strPath, m_strBatch);
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
							strSingleItem.Format("���%d��λ%d�߶�\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)
						{
							strSingleItem.Format("���%d��λ%d���\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)
						{
							strSingleItem.Format("���%d��λ%dTD\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
						for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiYpositionTuple.Length(); k++)
						{
							strSingleItem.Format("���%d��λ%dMD\t", i, k + 1);
							strValue = strValue + strSingleItem;
						}
					
				}
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)
				{
					strSingleItem.Format("��λ%dTD��λ\t", k + 1);
					strValue = strValue + strSingleItem;
				}
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)
				{
					strSingleItem.Format("��λ%dMD��λ\t", k + 1);
					strValue = strValue + strSingleItem;
				}
				strItem.Format("Ʒ��\t��ƷID\tʱ��\t����(mm)\tǰ�����Եλ��\t�������Եλ��\t�ٶ�(m/min)\t�ܳ�(mm)\tB��Ĥ��\tA��Ĥ��\tB���Ъ\tA���Ъ(mm)\t���λ(β��)(mm)\tС��λ(ͷ��)(mm)\t%s", strValue);
			}
			else
			{
				CString strValue(""), strSingleItem("");

				for (int i = 0; i < m_nGrabberCount; i++)
				{
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_Name.Length(); k++)
					{

						strSingleItem.Format("���%d%s\t", i, tOutput.B[i].tDetectOutput.hv_Name[k].S().Text());

						strValue = strValue + strSingleItem;

					}
				}
				strItem.Format("Ʒ��\t��ƷID\tʱ��\t����(mm)\tǰ�����Եλ��\t�������Եλ��\t�ٶ�(m/min)\t%s", strValue);
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
			if (tOutput.hvPackageType.I() == 0)//���
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
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple.Length(); k++)//��λ��
					{

						strSingleItem.Format("%.2f\t", tOutput.B[i].tDetectOutput.hv_CaoWeiHeightTuple[k].D());
						strValue = strValue + strSingleItem;

					}
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple.Length(); k++)//��λ��
					{

						strSingleItem.Format("%.2f\t", tOutput.B[i].tDetectOutput.hv_CaoWeiWidthTuple[k].D());
						strValue = strValue + strSingleItem;

					}
					for (int k = 0; k < tOutput.B[i].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)//��λTD
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
				//TD��λ
				for (int k = 0; k < tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple.Length(); k++)
				{
					double dForWardTd, dBackTd;
					dForWardTd = (tOutput.B[0].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[0].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*m_vec_ColumnResolution[0];
					dBackTd = (tOutput.B[1].tDetectOutput.hv_RightEadgePixel.D() - tOutput.B[1].tDetectOutput.hv_CaoWeiXPositionTuple[k].D())*m_vec_ColumnResolution[1];
					strSingleItem.Format("%.2f\t", dForWardTd - dBackTd);
					strValue = strValue + strSingleItem;
				}
				//MD��λ
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

	strSizeRecord.Format("%s\\����CPK������-%s.xls", strPath, m_strBatch);
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
					strSingleItem.Format("��%d��Ϳ����λֵ\t", k + 1);
				}
				else
				{
					strSingleItem.Format("��%d��AT9��λֵ\t", k + 1);
				}
				strValue = strValue + strSingleItem;
			}
			strItem.Format("Ʒ��\t��ƷID\tʱ��\t%s", strValue);
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
		//���λֵ
		HTuple hvCuoweiTuple;
		TupleSub(tOutput.B[0].tDetectOutput.hv_AT9EdageToProEdage, tOutput.B[1].tDetectOutput.hv_AT9EdageToProEdage, &hvCuoweiTuple);
		//��CPK����Ϳ����ǰ
		HTuple hv_CoatCuoWei, hv_CoatCpkIndex, hv_At9CuoWei, hvCPKCuoweiTuple;
		for (int i = 0; i < hvCuoweiTuple.Length() / 4; i++)
		{
			hv_CoatCuoWei.Append(hvCuoweiTuple[i + 1 + i * 3]);
			hv_CoatCuoWei.Append(hvCuoweiTuple[i + 2 + i * 3]);
			//�ȼ���Ϳ����λ
			hvCPKCuoweiTuple.Append(hv_CoatCuoWei);
		}
		TupleFind(hvCuoweiTuple, hv_CoatCuoWei, &hv_CoatCpkIndex);
		TupleRemove(hvCuoweiTuple, hv_CoatCpkIndex, &hv_At9CuoWei);
		//����AT9��λ
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
	strSizeRecord.Format("%s\\GRR����-%s.xls", strPath, m_strBatch);
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

		//		strSingleItem.Format("��%d��AT9��λֵ\t", k + 1);
		//		/*strItem.Format("Ʒ��\t��ƷID\tʱ��\t%s", strSingleItem);*/
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
				strSize.Format(" %s\t ͿĤ��λ%d\t %s",
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