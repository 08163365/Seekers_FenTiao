#include "StdAfx.h"
#include "Communicate.h"
#include "DlgIOSetting.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <setupapi.h>
#include "rpcdce.h"
#include <string>
#include <fstream>
using namespace std;
extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CCommunicate();
}

CCommunicate* g_This = NULL;
BOOL g_bExitThread = FALSE;


CCommunicate::CCommunicate(void)
{
	m_strProductName = "";
	m_strConfigCommonFile = "";
	m_strConfigCommunicateFile = "";
	m_strConfigSystemFile = "";
	m_nMarkerLength = 0;
	m_dMarkerLengthMM = 0;
	m_strConfigCommunVarietyFile = "";
	m_strConfigVarietyParmaFile = "";
	m_strConfigSizeParmaFile = "";
	m_dStandardEALength[0] = 1000;
	m_dStandardEALength[1] = 1000;
	m_nMarkCountNum[0] = 0;
	m_nMarkCountNum[1] = 0;
	InitializeCriticalSection(&m_csCommunicateChange);
	InitializeCriticalSection(&m_csSerialCOMUse);
	m_nTripNum = 12;
	TupleGenConst(MAX_LALBELNUM, 500, &m_hvRemainRow);
	TupleGenConst(MAX_LALBELNUM, 0, &m_hvRealMonitorLengthTuple);

	TupleGenConst(MAX_LALBELNUM, 0, &m_hvRealLabelCountTuple);
	m_bWarnFlag = FALSE;
	m_bOpen = FALSE;
	m_bState = FALSE;
	m_bCounterFlag = FALSE;

}


CCommunicate::~CCommunicate(void)
{
	DeleteCriticalSection(&m_csCommunicateChange);
	DeleteCriticalSection(&m_csSerialCOMUse);

}


void  CCommunicate::WriteLabelAction(C_ALLOCATORWORKSHOPOUTPUT& tOutPut)
{

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_sInit.strProductName, m_strBatch);
	if (m_sInit.strGongWei == "UpOffset")
	{
		strSizeRecord.Format("%s\\�ϴ�·覴�������Ϣд��ӿڿ���¼-%s.csv.", strSizePath, m_strBatch);
	}
	else
	{
		strSizeRecord.Format("%s\\�´�·覴�������Ϣд��ӿڿ���¼-%s.csv.", strSizePath, m_strBatch);
	}

	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("ʱ��,��λ,�ӳ��в���,����λ�ò���,EA���,����ͼ���,�������λ��,����ڼ���");
		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem << endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	HTuple hv_FileHandle;
	ofstream outFile;
	CString strCount, strValue;

	int nTieBiaoRow = MAX(0, MIN(5000 * m_nGrabberDivideNum / ConstFenpinNum, (int)(tOutPut.B[0].hv_CropRow[0].D()*m_nGrabberDivideNum / ConstFenpinNum)));//
	if (tOutPut.hvPackageType.I() == 0)//0��
	{
		strValue.Format("%d,%d,%.1f,%s,%d,%d,%d", tOutPut.hvPackageType.I() + 1, m_Commuicate.nXLengthInPixel, m_Commuicate.dLabelPosition, tOutPut.hvGroupID.S().Text(), tOutPut.B[0].A.hvImageSN.I() & 0xff, nTieBiaoRow, m_IOCard[0].readCounter(4));
	}
	else
	{

		strValue.Format("%d,%d,%.1f,%s,%d,%d,%d", tOutPut.hvPackageType.I() + 1, m_Commuicate.n2ndLengthInPixel, m_Commuicate.dLabelPosition, tOutPut.hvGroupID.S().Text(), tOutPut.B[0].A.hvImageSN.I() & 0xff, nTieBiaoRow, m_IOCard[0].readCounter(5));
	}
	outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
	strCount.Format("%s,%s", strTime, strValue);
	outFile << strCount << endl;
	outFile.close();
}




void CCommunicate::Loop()
{

	HTuple hv_MessageRecv, hv_StopProcInfo, hv_TimeStart, hv_TimeEnd, hv_ErrorFlag;
	C_ALLOCATORWORKSHOPOUTPUT tOutput;
	HTuple hvQueue = m_hvQueue;
	while (TRUE)
	{
		DequeueMessage(hvQueue, "timeout", "infinite", &hv_MessageRecv);
		GetMessageParam(hv_MessageRecv, "key_exists", "stop", &hv_StopProcInfo);
		if (0 != (HTuple(hv_StopProcInfo[0])))
		{
			break;
		}

		CountSeconds(&hv_TimeStart);//��ʼ��ʱ
		tOutput.ReadMessage(hv_MessageRecv);
		///����
		{


			CriticalSectionHelper Lock(&m_csCommunicateChange);
			////�����־ 2019.5.24��
			if (m_IOCard[0].hasCard)
			{
				unsigned short bySignEnable(1);
				int nsTripSN;
				int nTieBiaoRow = MAX(0, MIN(5000 * m_nGrabberDivideNum / ConstFenpinNum, (int)(tOutput.B[0].hv_CropRow[0].D()*m_nGrabberDivideNum / ConstFenpinNum)));//
				int nImageSN = tOutput.B[0].A.hvImageSN[0].I() & 0xff;
				if (REJECT_DEFULT == m_Commuicate.nRejectType)
				{

					if (M_TwoCam_Intermission == m_sInit.nCoatType)//��Ъ�� ���ղ���λ�ý�������
					{

						if (FALSE == tOutput.tFinalResult.hv_SurfaceFlag || FALSE == tOutput.tFinalResult.hv_SizeFlag)
						{

							HTuple hv_BelongTrip, hv_LabelIndex;
							hv_BelongTrip.Clear();
							hv_BelongTrip.Append(tOutput.tFinalResult.hv_SurfaceErrorBelongTrips);
							hv_BelongTrip.Append(tOutput.tFinalResult.hv_SizeErrorBelongTrips);
							for (int k = 0; k < m_nTripNum; k++)
							{
								TupleFind(hv_BelongTrip, k, &hv_LabelIndex);
								if (hv_LabelIndex.Length() > 0 && hv_LabelIndex != -1)
								{
									m_IOCard[0].sendChckRslt(bySignEnable, nImageSN, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], nTieBiaoRow);

									{

										SYSTEMTIME CurSysTime;
										GetLocalTime(&CurSysTime);
										CString strSizeRecord, strSizePath;
										strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_sInit.strProductName, m_strBatch);

										strSizeRecord.Format("%s\\覴�������Ϣд��ӿڿ���¼-%s.csv.", strSizePath, m_strBatch);
										if (!PathFileExists(strSizeRecord))
										{
											CreateMultilevelPath(strSizePath);
											CString strItem;
											strItem.Format("ʱ��,����,��������,ͨ��,�ӳ��в���,����λ�ò���,EA���,����ͼ���,�������λ��,ͼ���е���λ��");
											HTuple hv_FileHandle;
											ofstream outFile;
											outFile.open(strSizeRecord, ios::out);
											outFile << strItem << endl;
											outFile.close();
										}

										CString strTime;
										strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
										HTuple hv_FileHandle;
										ofstream outFile;
										CString strCount, strValue;


										strValue.Format("%d,%d,%d,%d,%.1f,%s,%d,%d,%.1f", k + 1, m_Commuicate.hvChannelChoice[k].I() + 1, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], m_Commuicate.nXLengthInPixel, m_Commuicate.dLabelPosition, tOutput.hvGroupID.S().Text(), tOutput.B[0].A.hvImageSN.I() & 0xff, nTieBiaoRow, tOutput.B[0].hv_CropRow[0].D());

										outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
										strCount.Format("%s,%s", strTime, strValue);
										outFile << strCount << endl;
										outFile.close();


									}


								}
								Sleep(1);

							}

						}
					}
					else//����Ϳ��
					{
						HTuple hv_Height;
						GetImageSize(tOutput.B[0].A.hoGrabImage, NULL, &hv_Height);

						if (FALSE == tOutput.tFinalResult.hv_SurfaceFlag || FALSE == tOutput.tFinalResult.hv_SizeFlag)
						{

							HTuple hv_BelongTrip, hv_LabelIndex;
							hv_BelongTrip.Clear();
							hv_BelongTrip.Append(tOutput.tFinalResult.hv_SurfaceErrorBelongTrips);
							hv_BelongTrip.Append(tOutput.tFinalResult.hv_SizeErrorBelongTrips);
							HObject hoUnionErrorObject;
							HTuple  hvUnionErrorTrip;
							hoUnionErrorObject.GenEmptyObj();
							hvUnionErrorTrip.Clear();
							hvUnionErrorTrip.Append(tOutput.B[0].tDetectOutput.hv_ErrorBelongStrip).Append(tOutput.B[1].tDetectOutput.hv_ErrorBelongStrip);
							ConcatObj(tOutput.B[0].tDetectOutput.ho_ErrorRegion, tOutput.B[1].tDetectOutput.ho_ErrorRegion, &hoUnionErrorObject);

							for (int k = 0; k < m_nTripNum; k++)
							{
								TupleFind(hv_BelongTrip, k, &hv_LabelIndex);
								if (hv_LabelIndex.Length() > 0 && hv_LabelIndex != -1)//���ڸ���ȱ��
								{
									HTuple hv_ErrorTripIndex;
									HObject hoSelectObj, hoSelectUnionErrorRegion;

									TupleFind(hvUnionErrorTrip, k, &hv_ErrorTripIndex);
									SelectObj(hoUnionErrorObject, &hoSelectObj, hv_ErrorTripIndex + 1);
									Union1(hoSelectObj, &hoSelectUnionErrorRegion);
									HTuple hv_LTRow, hv_LTColumn, hv_RBRow, hvRBColumn;
									SmallestRectangle1(hoSelectUnionErrorRegion, &hv_LTRow, &hv_LTColumn, &hv_RBRow, &hvRBColumn);

									double dVirtualEAHeight = m_Commuicate.dVirtualEALength / m_Common.dRowResolution[0];//��Ʒ�߶�3000��


									if (hv_RBRow + m_hvRemainRow[k] > dVirtualEAHeight)
									{
										int nPointRow = 0;
										int nImageSN = tOutput.B[0].A.hvImageSN[0].I();
										if (hv_RBRow - hv_LTRow < dVirtualEAHeight)//С�ڵ��Ų�Ʒ�߶� ��һ���ź�
										{

											nPointRow = MAX(0, MIN(5000 * m_nGrabberDivideNum / ConstFenpinNum, (int)(hv_LTRow.D()*m_nGrabberDivideNum / ConstFenpinNum)));
											m_IOCard[0].sendChckRslt(1, nImageSN, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], nPointRow);


											{

												SYSTEMTIME CurSysTime;
												GetLocalTime(&CurSysTime);
												CString strSizeRecord, strSizePath;
												strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_sInit.strProductName, m_strBatch);

												strSizeRecord.Format("%s\\覴�������Ϣд��ӿڿ���¼-%s.csv.", strSizePath, m_strBatch);
												if (!PathFileExists(strSizeRecord))
												{
													CreateMultilevelPath(strSizePath);
													CString strItem;
													strItem.Format("ʱ��,����,��������,ͨ��,�ӳ��в���,����λ�ò���,EA���,����ͼ���,�������λ��,ͼ���е���λ��");
													HTuple hv_FileHandle;
													ofstream outFile;
													outFile.open(strSizeRecord, ios::out);
													outFile << strItem << endl;
													outFile.close();
												}

												CString strTime;
												strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
												HTuple hv_FileHandle;
												ofstream outFile;
												CString strCount, strValue;


												strValue.Format("%d,%d,%d,%d,%.1f,%s,%d,%d,%.1f", k + 1, m_Commuicate.hvChannelChoice[k].I() + 1, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], m_Commuicate.nXLengthInPixel, m_Commuicate.dLabelPosition, tOutput.hvGroupID.S().Text(), tOutput.B[0].A.hvImageSN.I() & 0xff, nPointRow, hv_LTRow.D());

												outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
												strCount.Format("%s,%s", strTime, strValue);
												outFile << strCount << endl;
												outFile.close();


											}





										}
										else
										{
											//�ж�һ��

											int nSendSignalNum = ceil((hv_RBRow.D() - hv_LTRow.D()) / dVirtualEAHeight);
											for (int i = 0; i < nSendSignalNum; i++)
											{
												nPointRow = MAX(0, MIN(5000 * m_nGrabberDivideNum / ConstFenpinNum, (int)((hv_LTRow.D() + i*dVirtualEAHeight)*m_nGrabberDivideNum / ConstFenpinNum)));
												m_IOCard[0].sendChckRslt(1, nImageSN, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], nPointRow);


												{

													SYSTEMTIME CurSysTime;
													GetLocalTime(&CurSysTime);
													CString strSizeRecord, strSizePath;
													strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, m_sInit.strProductName, m_strBatch);

													strSizeRecord.Format("%s\\覴�������Ϣд��ӿڿ���¼-%s.csv.", strSizePath, m_strBatch);
													if (!PathFileExists(strSizeRecord))
													{
														CreateMultilevelPath(strSizePath);
														CString strItem;
														strItem.Format("ʱ��,����,��������,ͨ��,�ӳ��в���,����λ�ò���,EA���,����ͼ���,�������λ��,ͼ���е���λ��");
														HTuple hv_FileHandle;
														ofstream outFile;
														outFile.open(strSizeRecord, ios::out);
														outFile << strItem << endl;
														outFile.close();
													}

													CString strTime;
													strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
													HTuple hv_FileHandle;
													ofstream outFile;
													CString strCount, strValue;


													strValue.Format("%d,%d,%d,%d,%.1f,%s,%d,%d,%.1f", k + 1, m_Commuicate.hvChannelChoice[k].I() + 1, Const_nChannel[m_Commuicate.hvChannelChoice[k].I()], m_Commuicate.nXLengthInPixel, m_Commuicate.dLabelPosition, tOutput.hvGroupID.S().Text(), tOutput.B[0].A.hvImageSN.I() & 0xff, nPointRow, (hv_LTRow.D() + i*dVirtualEAHeight));

													outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
													strCount.Format("%s,%s", strTime, strValue);
													outFile << strCount << endl;
													outFile.close();


												}





											}

										}








										m_hvRemainRow[k] = hv_Height - hv_LTRow;
									}
									else
									{
										m_hvRemainRow[k] = m_hvRemainRow[k] + hv_Height;
									}








									/*m_IOCard[0].sendChckRslt(bySignEnable,nImageSN,Const_nChannel[m_Commuicate.hvChannelChoice[k].I()],nTieBiaoRow);*/
								}
								else
								{

									m_hvRemainRow[k] = m_hvRemainRow[k] + hv_Height;

								}

							}

						}
						else
						{
							for (int k = 0; k < m_nTripNum; k++)
							{

								m_hvRemainRow[k] = m_hvRemainRow[k] + hv_Height;


							}



						}











					}


				}
				///////д������λ����Ϣ

			}
			CountSeconds(&hv_TimeEnd);//������ʱ

		}
		ClearMessage(hv_MessageRecv);
	}
}


void CCommunicate::WriteInfo()
{



	ComPareChange(m_Commuicate.dFirstLabelLength, m_tLabelControlInfo.dFirstLabelLength, "�ڲ�������ܾ���(mm)");
	m_Commuicate.dFirstLabelLength = m_tLabelControlInfo.dFirstLabelLength;
	ComPareChange(m_Commuicate.dSecondLabelLength, m_tLabelControlInfo.dSecondLabelLength, "���������ܾ���(mm)");
	m_Commuicate.dSecondLabelLength = m_tLabelControlInfo.dSecondLabelLength;
	ComPareChange(m_Commuicate.dLabelPosition, m_tLabelControlInfo.dLabelActionPosition, "����λ��(mm)");
	m_Commuicate.dLabelPosition = m_tLabelControlInfo.dLabelActionPosition;

	if (m_tLabelControlInfo.bEnableFirstLabel && m_tLabelControlInfo.bEnableSecondLabel)
	{
		ComPareChange(m_Commuicate.nRejectType, REJECT_DEFULT, "����λ��(mm)");
		m_Commuicate.nRejectType = REJECT_DEFULT;
	}
	else
	{
		ComPareChange(m_Commuicate.nRejectType, REJECT_NONE, "����λ��(mm)");
		m_Commuicate.nRejectType = REJECT_NONE;
	}


	if (m_IOCard[0].hasCard)
	{
		m_Commuicate.nBuChangHangShu = m_IOCard[0].readParam(162);//ԭ142 ����ƫ��20
	}


	{
		int   nRejectDelay, nRejectDelay1;

#pragma  region   "����������·��Ĵ��X���ӳ�����"
		if (m_Commuicate.dFirstLabelLength != 0 && m_IOCard[0].hasCard)
		{

			{
				nRejectDelay = (m_Commuicate.dFirstLabelLength - m_Commuicate.dLabelPosition)*m_nGrabberDivideNum / (m_Common.dRowResolution[0] * ConstFenpinNum) - m_Commuicate.nBuChangHangShu;

			}
			m_IOCard[0].writeParam(49, nRejectDelay);//������x
			CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
			CString strApp = "PIO24B";
			CString strValue;
			strValue.Format("0x%x", nRejectDelay);
			WritePrivateProfileString(strApp, "49", strValue, strFile);
			ComPareChange(m_Commuicate.nXLengthInPixel, nRejectDelay, "���X�ӳ�����");
			m_Commuicate.nXLengthInPixel = nRejectDelay;
		}
#pragma  endregion 


#pragma  region   "����������·���2nd���ӳ�����"

		if (m_Commuicate.dSecondLabelLength != 0 && m_IOCard[0].hasCard)
		{

			{
				nRejectDelay1 = (m_Commuicate.dSecondLabelLength - m_Commuicate.dLabelPosition)*m_nGrabberDivideNum / (m_Common.dRowResolution[0] * ConstFenpinNum) - m_Commuicate.nBuChangHangShu;
			}

			m_IOCard[0].writeParam(76, nRejectDelay1);//�ڶ�·�����ʱ����
			CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
			CString strApp = "PIO24B";
			CString strValue;
			strValue.Format("0x%x", nRejectDelay1);
			WritePrivateProfileString(strApp, "76", strValue, strFile);
			ComPareChange(m_Commuicate.n2ndLengthInPixel, nRejectDelay1, "���2nd�ӳ�����");
			m_Commuicate.n2ndLengthInPixel = nRejectDelay1;
		}
#pragma  endregion 

	}
	////�洢���������Ϣ
	WritePrivateProfileInt("IO", "REJECTTYPE", m_Commuicate.nRejectType, m_strConfigCommunicateFile);


	WritePrivateProfileInt("IO", "dXLengthInPixel", m_Commuicate.nXLengthInPixel, m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO", "d2ndLengthInPixel", m_Commuicate.n2ndLengthInPixel, m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO", "dLabelPosition", m_Commuicate.dLabelPosition, m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO", "nBuChangHangShu", m_Commuicate.nBuChangHangShu, m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO", "bMachineLengthLabel", m_Commuicate.bMachineLengthLabel, m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO", "bUseLabelOffset", m_Commuicate.bUseLabelOffset, m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dLabelOffsetDev", m_Commuicate.dLabelOffsetDev, m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dUpLabelOffsetCompensate", m_Commuicate.dUpLabelOffsetCompensate, m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dDownLabelOffsetCompensate", m_Commuicate.dDownLabelOffsetCompensate, m_strConfigCommunicateFile);





	WritePrivateProfileDouble("IO", "dVirtualEALength", m_Commuicate.dVirtualEALength, m_strConfigCommunicateFile);
	//�������� �������Ʒ�ֱ仯
	WritePrivateProfileDouble("IO", "dFirstLabelLength", m_Commuicate.dFirstLabelLength, m_strConfigCommonFile);
	WritePrivateProfileDouble("IO", "dSecondLabelLength", m_Commuicate.dSecondLabelLength, m_strConfigCommonFile);

}
void CCommunicate::MakeMachineStop()
{
	if (m_IOCard[0].hasCard)
	{
		m_IOCard[0].setCardOutput(0, 1);//����ͣ��
		Sleep(20);
		m_IOCard[0].setCardOutput(0, 0);//����ͣ��
	}
}
int CCommunicate::DoSomething(void* message, void* wParam, void* lParam)
{
	CString strMessage;
	strMessage.Format("%s", (char*)message);
	if ("INITIALIZE" == strMessage)
	{
		return Init((PCOMMUNICATEINIT)wParam);
	}
	else if ("PUSH_MESSAGE" == strMessage)
	{
		return PushMessage(*(HTuple*)wParam);
	}
	else if ("SHOW" == strMessage)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgIOSetting dlg;
		dlg.m_pCommuicate = this;
		dlg.DoModal();
	}
	else if ("SHOWPARAM" == strMessage)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgIOSetting dlg;
		dlg.m_pCommuicate = this;
		dlg.DoModal();
		m_OutputhWnd = dlg.m_hWnd;
	}
	else if ("READY" == strMessage)
	{
		CString strParam;
		strParam.Format("%s", wParam);
		return ReadyOutput(strParam);
	}
	else if ("READCOUNT" == strMessage)
	{
		CString strCountSN;
		strCountSN.Format("%d", *(int*)wParam);
		if ("28" == strCountSN)
		{
			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(28);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("29" == strCountSN)
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(29);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("16" == strCountSN)
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(16);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("4" == strCountSN)//��ȡ���0�ļ���
		{

			if (m_IOCard[0].hasCard)
			{


				*(int*)lParam = m_IOCard[0].readCounter(4);//�������ߵ�ʱ���

			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("5" == strCountSN)//��ȡ���1�ļ���
		{

			if (m_IOCard[0].hasCard)
			{

				*(int*)lParam = m_IOCard[0].readCounter(5);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("6" == strCountSN)//��ȡ���2�ļ���
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(6);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("18" == strCountSN)//��ȡ���2�ļ���
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(18);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("32" == strCountSN)
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readParam(32);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("49" == strCountSN)
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readParam(49);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("57" == strCountSN)//��ʱ����0
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readParam(57);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("166" == strCountSN)//�ɼ���Ƶ����������20
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readParam(166);
			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("XOutPortNum" == strCountSN)//��ȡ���Xͼ����ӳٵ�λ�������
		{

			if (m_IOCard[0].hasCard)
			{

				*(int*)lParam = m_IOCard[0].readCounter(4);//�������߶�ȡ4

			}
			else
			{
				*(int*)lParam = 0;
			}
		}
		else if ("2ndOutPortNum" == strCountSN)//��ȡ2ndͼ����ӳٵ�λ���������
		{

			if (m_IOCard[0].hasCard)
			{
				*(int*)lParam = m_IOCard[0].readCounter(5);//�������߶�ȡ5
			}
			else
			{
				*(int*)lParam = 0;
			}
		}

	}
	else if ("SETPARAM" == strMessage)
	{
		CriticalSectionHelper Lock(&m_csCommunicateChange);
		CString strParam;
		strParam.Format("%s", wParam);
		if (strParam == "LabelControlInfo")
		{
			m_tLabelControlInfo = *(LabelControlInfo*)lParam;
			WriteInfo();
			return TRUE;
		}
		else if (strParam == "BATCH")
		{
			m_strBatch.Format("%s", lParam);

		}
		else if ("SAVEPATH" == strParam)
		{
			m_strSavePath.Format("%s", lParam);
		}
		else if ("SandardEALength0" == strParam)//����
		{
			m_dStandardEALength[0] = *(double*)lParam;

		}
		else if ("SandardEALength1" == strParam)//����
		{
			m_dStandardEALength[1] = *(double*)lParam;
		}
		else if ("TRIPNUM" == strParam)//����
		{
			m_nTripNum = *(int*)lParam;
		}
		else if ("SetParam63" == strParam)
		{
			int nFenPin, nImageHeight, nLastFreshParam;
			nImageHeight = *(int*)lParam;
			if (m_IOCard[0].hasCard)
			{

				nLastFreshParam = m_IOCard[0].readParam(63);//��ʾͼ����ӳٸ���
				//	int a=(4348&0x00f0)>>4;
				nFenPin = (m_IOCard[0].readParam(28) & 0xf0) >> 4;//��ʾ��Ƶ��
				int nFreshParam;
				nFreshParam = nImageHeight*m_nGrabberDivideNum / ((nFenPin + 1) * 2);
				m_IOCard[0].writeParam(63, nFreshParam);
				CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
				CString strApp = "PIO24B";
				CString strValue;
				strValue.Format("0x%x", nFreshParam);
				WritePrivateProfileString(strApp, "63", strValue, strFile);
				ComPareChange(nLastFreshParam, nFreshParam, "ͼ����ӳٸ���63");
			}



		}
		else if ("BJFW" == strParam)//����
		{
			BaoJingFuWei();
		}
		else if ("EXIT" == strParam)
		{
			g_bExitThread = TRUE;
		}
		else if ("BJTJ" == strParam)//����ͣ��
		{
			MakeMachineStop();
		}



	}
	else if ("GETPARAM" == strMessage)
	{
		CString strParam;
		strParam.Format("%s", wParam);
		if ("MESSAGE_NUM" == strParam)
		{
			GetMessageQueueParam(m_hvQueue, "message_num", (HTuple*)lParam);
			return TRUE;
		}
		else if ("GRABBERDIVIDENUM" == strParam)
		{
			*(int*)lParam = m_nGrabberDivideNum;
			return TRUE;
		}
		else if (strParam == "LabelControlInfo")
		{
			LabelControlInfo tLabelControlInfo;

			if (m_Commuicate.nRejectType == REJECT_DEFULT)
			{
				tLabelControlInfo.bEnableFirstLabel = 1;
				tLabelControlInfo.bEnableSecondLabel = 1;

			}
			else
			{
				tLabelControlInfo.bEnableFirstLabel = 0;
				tLabelControlInfo.bEnableSecondLabel = 0;
			}
			tLabelControlInfo.dFirstLabelLength = m_Commuicate.dFirstLabelLength;
			tLabelControlInfo.dSecondLabelLength = m_Commuicate.dSecondLabelLength;
			tLabelControlInfo.dLabelActionPosition = m_Commuicate.dLabelPosition;
			*(LabelControlInfo*)lParam = tLabelControlInfo;
		}
		else if (strParam == "ToFirstLabelLength")
		{
			*(double*)lParam = m_Commuicate.dFirstLabelLength;
			return TRUE;
		}
		else if (strParam == "ToSecondLabelLength")
		{
			*(double*)lParam = m_Commuicate.dSecondLabelLength;
			return TRUE;
		}
		else if (strParam == "LabelStatus")
		{
			*(int*)lParam = m_bOpen;
			return TRUE;
		}
		else if (strParam == "LabelWarnStaus")
		{
			*(BOOL*)lParam = m_bWarnFlag;
			return TRUE;
		}
		else if ("XImageOutSN" == strParam)//����ͼ���
		{
			if (m_IOCard[0].hasCard)
			{



				*(int*)lParam = m_IOCard[0].readCounter(53);//T2rbf��ȡ���x����ͼ���


			}
			else
			{
				*(int*)lParam = 0;
			}
			return TRUE;
		}
		else if ("2ndImageOutSN" == strParam)//����ͼ���
		{
			if (m_IOCard[0].hasCard)
			{


				*(int*)lParam = m_IOCard[0].readCounter(60);//��markΪ��׼���������ȡ����ͼ���

			}
			else
			{
				*(int*)lParam = 0;
			}
			return TRUE;
		}

		else if (strParam == "StandardLabelLocation")
		{
			*(double*)lParam = m_Commuicate.dLabelPosition;
			return TRUE;
		}
		else if (strParam == "LabelLocationDev")
		{
			*(double*)lParam = m_Commuicate.dLabelOffsetDev;
			return TRUE;
		}
		else if (strParam == "LabelLocationOffset")
		{
			*(HTuple*)lParam = m_Commuicate.hvLabelOffsetCompensateTuple;
			return TRUE;
		}
		else if ("LabelCounterChange" == strParam)
		{

			*(HTuple*)lParam = m_hvRealLabelCountTuple;
			return TRUE;
		}
		else
		{
			AfxMessageBox("����ͨ�Ų�������!");
			return TRUE;
		}
	}
	else if ("ENABLE" == strMessage)
	{
		for (int i = 0; i < PIO24_COUNT; i++)
		{
			if (m_IOCard[i].hasCard)
			{
				m_IOCard[i].softReset();
				m_IOCard[i].enable(true);
				m_IOCard[i].softReset();//need reset twice
			}
		}
	}
	else if ("OUTPUT" == strMessage)
	{
		int nPort = *(int*)wParam;
		CString strParam;
		strParam.Format("%s", lParam);
		if (0 <= nPort && nPort <= 7)
		{
			if (m_IOCard[0].hasCard)
			{
				if ("ON" == strParam)
				{
					m_IOCard[0].setCardOutput(nPort, 1);
				}
				else if ("OFF" == strParam)
				{
					m_IOCard[0].setCardOutput(nPort, 0);
				}
			}
		}
	}
	else if ("COPYFILES" == strMessage)
	{
		CString strwParam, strlParam;
		strwParam.Format("%s", wParam);
		strlParam.Format("%s", lParam);
		strwParam = m_strConfigCommunVarietyFile + strwParam;
		strlParam = m_strConfigCommunVarietyFile + strlParam;
		CopyFolder(strwParam, strlParam);
	}
	else if ("DELETEFILES" == strMessage)
	{
		CString strwParam;
		strwParam.Format("%s", wParam);
		strwParam = m_strConfigCommunVarietyFile + strwParam;
		DeleteFolder(strwParam);
	}
	else if ("ENCODER_COUNT" == strMessage)
	{
		if (m_IOCard[0].hasCard)
		{
			int nCounter47, nCounter49, nCounter50, nCounter51;
			nCounter47 = m_IOCard[0].readCounter(47);
			nCounter49 = m_IOCard[0].readCounter(49);
			nCounter50 = m_IOCard[0].readCounter(50);
			nCounter51 = m_IOCard[0].readCounter(51);
			*(LONG*)lParam = (long)nCounter47 + (long)nCounter49 * 65536 + (long)nCounter50 * 65536 * 65536
				+ (long)nCounter51 * 65536 * 65536 * 65536;
			return TRUE;
		}
	}
	else
	{
		CString strMessageBox;
		strMessageBox.Format("Communicate����û����Ӧ��DoSomething:%s", strMessage);
		AfxMessageBox(strMessageBox);
	}
	return TRUE;
}
int CCommunicate::PushMessage(const HTuple& hvMessageSend)//������Ϣ�������
{
	try
	{
		EnqueueMessage(m_hvQueue, hvMessageSend, HTuple(), HTuple());//����ϢͶ�ݵ��޷ϳ���
	}
	catch (HException& e)
	{
		if (H_ERR_MQOVL == e.ErrorCode())
		{
			CString strException;
			strException.Format("IO��������,�����߳����˳�,���Ų�ԭ��");
			m_sInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
		}
		else
		{
			throw e;
		}
		return FALSE;
	}
	return TRUE;
}

void* CCommunicate::_Thread(void *hcthread)//io�߳�
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
		strException.Format("io_thread�����쳣\r\n�쳣��Ϣ%s  [ %s() ]", e.ErrorMessage().Text()
			, e.ProcName().Text());
		g_This->m_sInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
		bool is_direct_call = hcppthread->IsDirectCall();
		hcppthread->Exit();
		delete hcppthread;
		if (is_direct_call)
			throw e;
	}
	return NULL;
}
void CCommunicate::ComPareChange(int LastValue, int  CurrentValue, CString strName)
{
	CString strMessage;
	if (LastValue != CurrentValue)
	{
		strMessage.Format("������Ʋ���%s��%d����Ϊ%d", strName, LastValue, CurrentValue);
		((DHInterface*)(m_sInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strMessage, NULL);
	}

}
void CCommunicate::ComPareChange(double LastValue, double CurrentValue, CString strName)
{
	CString strMessage;
	if (LastValue != CurrentValue)
	{
		strMessage.Format("������Ʋ���%s��%.2f����Ϊ%.2f", strName, LastValue, CurrentValue);
		((DHInterface*)(m_sInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strMessage, NULL);
	}

}
BOOL CCommunicate::GetDeviceList(LPGUID lpGuid, BOOL bsFlag)
{
	BOOL bFlag = TRUE;

	do
	{
		HDEVINFO hDevInfo;
		SP_DEVINFO_DATA DeviceInfoData;
		DWORD i;
		// �õ������豸 HDEVINFO      
		hDevInfo = SetupDiGetClassDevs(lpGuid, 0, 0, DIGCF_PRESENT/* | DIGCF_ALLCLASSES */);
		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			bFlag = FALSE;
			break;
		}
		DeviceInfo theItem;
		// ѭ���о�     
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
		{
			TCHAR szClassBuf[MAX_PATH] = { 0 };
			TCHAR szDescBuf[MAX_PATH] = { 0 };
			TCHAR szDriver[MAX_PATH] = { 0 };
			TCHAR szFriName[MAX_PATH] = { 0 };

			// ��ȡ����  
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_CLASS, NULL, (PBYTE)szClassBuf, MAX_PATH - 1, NULL))
			{
				///*continue*/;

			}

			theItem.szDeviceClass = szClassBuf;

			//��ȡ�豸������Ϣ
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)szDescBuf, MAX_PATH - 1, NULL))
			{
				///*continue*/;

			}

			theItem.szDeviceDesc = szDescBuf;

			//��ȡ�豸������
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_DRIVER, NULL, (PBYTE)szDriver, MAX_PATH - 1, NULL))
			{
				///*continue*/;

			}

			theItem.szDriverName = szDriver;

			//��ȡ�豸�Ѻ���
			if (!SetupDiGetDeviceRegistryProperty(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szFriName, MAX_PATH - 1, NULL))
			{
				///*continue*/;

			}
			//ERROR_INSUFFICIENT_BUFFER

			theItem.szDeviceName = szFriName;
			theItem.dwDevIns = DeviceInfoData.DevInst;
			theItem.Guid = DeviceInfoData.ClassGuid;

		}

		//  �ͷ�     
		SetupDiDestroyDeviceInfoList(hDevInfo);
		SetDeviceStatus(theItem, bsFlag);



	} while (FALSE);

	return bFlag;
}
BOOL CCommunicate::SetDeviceStatus(DeviceInfo &theDevice, BOOL bStatusFlag)
{
	BOOL bFlag = TRUE;
	do
	{
		SP_DEVINFO_DATA DeviceInfoData;
		DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		HDEVINFO hDevInfo;
		// �õ��豸 HDEVINFO      
		hDevInfo = SetupDiGetClassDevs(&theDevice.Guid, 0, 0, DIGCF_PRESENT /*| DIGCF_ALLCLASSES */);
		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			bFlag = FALSE;
			break;
		}

		//�ж��Ƿ�������豸
		bFlag = FALSE;
		int index = 0;
		while (SetupDiEnumDeviceInfo(hDevInfo, index++, &DeviceInfoData))
		{
			if (DeviceInfoData.DevInst == theDevice.dwDevIns)
			{
				bFlag = TRUE;
				break;
			}
		}
		if (!bFlag)
		{

		}
		else
		{
			//��ʼ������
			SP_PROPCHANGE_PARAMS propChange;
			propChange.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
			propChange.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
			propChange.Scope = DICS_FLAG_GLOBAL;
			/*propChange.StateChange = bStatusFlag ? DICS_START: DICS_STOP;*/
			if (1 == bStatusFlag)
			{
				propChange.StateChange = DICS_ENABLE;//���ã�DICS_ENABLE  �����ã�DICS_DISABLE
			}
			else
			{
				propChange.StateChange = DICS_DISABLE;//���ã�DICS_ENABLE  �����ã�DICS_DISABLE
			}



			propChange.HwProfile = 0;

			if (SetupDiSetClassInstallParams(hDevInfo, &DeviceInfoData, (SP_CLASSINSTALL_HEADER*)&propChange, sizeof(propChange)))
			{
				if (!SetupDiChangeState(hDevInfo, &DeviceInfoData))
				{

					bFlag = FALSE;
				}
			}
			else
			{

				bFlag = FALSE;
			}
		}

		//  �ͷ�     
		SetupDiDestroyDeviceInfoList(hDevInfo);


	} while (FALSE);

	return TRUE;
}
void CCommunicate::BaoJingFuWei()//�����������λ
{


}

int CCommunicate::Init(PCOMMUNICATEINIT pInit)//��ʼ��
{

	///ǿ�����ýӿڿ�
	CString szGUID = _T("e7a1d5b1-64f7-4414-8dd7-e1479749ccbd");//�ӿڿ����guid��
	GUID guid;                  //ȫ��Ψһ��ʶ��(GUID��GloballyUnique Identifier)
	UuidFromString((unsigned char*)((LPCTSTR)(szGUID.Left(36))), &guid);//���ֽ�
	GetDeviceList(&guid, 1);



	SetHcppInterfaceStringEncodingIsUtf8(FALSE);
	SetSystem("filename_encoding", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("read_halcon_files_encoding_fallback", "locale");
	SetSystem("write_halcon_files_encoding", "locale");
	SetSystem("tsp_tuple_string_operator_mode", "byte");
	SetSystem("tuple_string_operator_mode", "byte");
	SetSystem("store_empty_region", "false");
	SetSystem("clip_region", "false");
#pragma region "��ȡ�����ļ�"
	CString strValue;
	m_strProductName = pInit->strProductName;
	m_pLogView = pInit->pLogView;
	m_pDlg = pInit->pDlg;
	m_nProductType = pInit->nCoatType;
	m_strConfigSystemFile.Format("%s\\Config\\System.vis", GetCurrentAppPath());
	m_strConfigCommonFile.Format("%s\\Config\\Common.dh", GetCurrentAppPath());
	m_strConfigCommunicateFile.Format("%s\\Config\\Products\\%s\\SCICommunicate\\Communicate.ini", GetCurrentAppPath(), m_strProductName);
	m_strConfigCommunVarietyFile.Format("%s\\Config\\Products\\%s\\SCICommunicate\\", GetCurrentAppPath(), m_strProductName);
#pragma endregion
	m_sInit.pLogView = pInit->pLogView;
	m_sInit.nCoatType = pInit->nCoatType;
	m_sInit.strGongWei = pInit->strGongWei;
	m_sInit.pSysCall = pInit->pSysCall;
	m_sInit.strProductName.Format("%s", pInit->strProductName);
	g_This = this;
	if (m_sInit.strGongWei == "UpOffset")
	{

		m_hvGongWeiName.Append("1");
		m_hvGongWeiName.Append("2");
		m_hvDaiLu.Append("�ϴ�·");
	}
	else
	{
		m_hvGongWeiName.Append("3");
		m_hvGongWeiName.Append("4");
		m_hvDaiLu.Append("�´�·");
	}
#pragma region "�����ӿڿ�"
	for (int i = 0; i < PIO24_COUNT; i++)
	{
		CString strConfigFile;
		strConfigFile.Format("PIO24B_reg_init.txt");
		if (0 != i)
		{
			strConfigFile.Format("PIO24B_reg_init%d.txt", i);
		}
		if (m_IOCard[i].openCard(i))
		{
			m_IOCard[i].hardReset();
			if (!m_IOCard[i].initRegisters(strConfigFile, "PIO24B"))
			{
				CString strMessageBox;
				strMessageBox.Format("%sע��ʧ��", strConfigFile);
				AfxMessageBox(strMessageBox);
			}
		}
		else
		{
			CString strException;
			strException.Format("δ���ֽӿڿ�%d", i);
			if (i == 0)
			{
				m_sInit.pLogView->DoSomething("EXCEPTION", (LPSTR)(LPCSTR)strException, NULL);
			}

		}
	}

	if (m_IOCard[0].hasCard)
	{
		m_nGrabberDivideNum = m_IOCard[0].readParam(166);
		if (m_nGrabberDivideNum == 0 || m_nGrabberDivideNum == 1)
		{
			m_nGrabberDivideNum = 1;
		}
	}
	else
	{
		m_nGrabberDivideNum = 1;
	}



	if (m_IOCard[0].hasCard)
	{
		for (int i = 0; i < 12; i++)
		{
			m_IOCard[0].writeParam(Const_TestIndexParam[i], 0);
		}

	}


	ReadParam();
#pragma endregion
	InitThread();
	return TRUE;
}





void SetDataTranslate(CString&strFilePath, double& dData, BOOL bWriteFlag)
{

	if (bWriteFlag)
	{
		CString strValue;
		ofstream outFile;
		strValue.Format("%.2f", dData);
		outFile.open(strFilePath, ios::out);
		outFile << strValue << endl;
		outFile.close();

	}
	else
	{

		char CharValue[100];
		CString strValue;
		ifstream infile;;
		infile.open(strFilePath, ios::in);
		infile >> CharValue;
		infile.close();
		strValue.Format("%s", CharValue);
		dData = atof(strValue);
	}



}


BOOL CCommunicate::ReadyOutput(CString &strParam)
{
	if (m_IOCard[0].hasCard)
	{
		if ("ON" == strParam)
		{
			m_IOCard[0].setCardOutput(1, 1);

		}
		else if ("OFF" == strParam)
		{
			m_IOCard[0].setCardOutput(1, 0);
		}
	}

	return TRUE;
}

void CCommunicate::InitThread()
{
#pragma region "�������к��߳�"
	CreateMessageQueue(&m_hvQueue);//����һ��IO����
	SetMessageQueueParam(m_hvQueue, "max_message_num", 80);
	HDevThreadContext Context;
	HDevThread* pThread = new HDevThread(Context,
		(void*)_Thread, 0, 0);//����һ������2��������߳�;
	HTuple ProcThreads;
	pThread->ParStart(&ProcThreads);


#pragma endregion

}





void CCommunicate::ExceptionRemain(HTuple& hv_DiffValue, CString strName)
{

	if (hv_DiffValue.TupleAbs() > 30)
	{
		CString strInfo;
		strInfo.Format("�������ʱ:%s�仯��:%.1f", strName, hv_DiffValue.D());
		m_pLogView->DoSomething("EXCEPTION", (void*)(LPCSTR)strInfo, NULL);
	}

}



void CCommunicate::RefreshDelayInfo()
{

	if (m_IOCard[0].hasCard)
	{

		int nBuChangHangShu = m_IOCard[0].readParam(162);
		ComPareChange(m_Commuicate.nBuChangHangShu, nBuChangHangShu, "����ʱ���ٲ�������");
		if (nBuChangHangShu != m_Commuicate.nBuChangHangShu)
		{

			CString strInfo;
			strInfo.Format("�ӿڿ��ı��ٲ�������162���ֱ仯����ȷ��");
			m_pLogView->DoSomething("EXCEPTION", (void*)(LPCSTR)strInfo, NULL);
		}
		m_Commuicate.nBuChangHangShu = nBuChangHangShu;

	}


	{
		int   nRejectDelay, nRejectDelay1;

#pragma  region   "��������ʱ�Ĵ��X���ӳ�����"
		if (m_Commuicate.dFirstLabelLength != 0 && m_IOCard[0].hasCard)
		{


			nRejectDelay = (m_Commuicate.dFirstLabelLength - m_Commuicate.dLabelPosition)*m_nGrabberDivideNum / (m_Common.dRowResolution[0] * ConstFenpinNum) - m_Commuicate.nBuChangHangShu;
			m_IOCard[0].writeParam(49, nRejectDelay);//������x
			CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
			CString strApp = "PIO24B";
			CString strValue;
			strValue.Format("0x%x", nRejectDelay);
			WritePrivateProfileString(strApp, "49", strValue, strFile);
			HTuple hvDiff = m_Commuicate.nXLengthInPixel - nRejectDelay;
			ExceptionRemain(hvDiff, "����ʱ���X�ӳ�����");
			ComPareChange(m_Commuicate.nXLengthInPixel, nRejectDelay, "����ʱ���X�ӳ�����");
			m_Commuicate.nXLengthInPixel = nRejectDelay;
		}
#pragma  endregion 


#pragma  region   "��������ʱ2nd���ӳ�����"

		if (m_Commuicate.dSecondLabelLength != 0 && m_IOCard[0].hasCard)
		{
			{
				nRejectDelay1 = (m_Commuicate.dSecondLabelLength - m_Commuicate.dLabelPosition)*m_nGrabberDivideNum / (m_Common.dRowResolution[0] * ConstFenpinNum) - m_Commuicate.nBuChangHangShu;
			}



			m_IOCard[0].writeParam(76, nRejectDelay1);//�ڶ�·�����ʱ����
			CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
			CString strApp = "PIO24B";
			CString strValue;
			strValue.Format("0x%x", nRejectDelay1);
			WritePrivateProfileString(strApp, "76", strValue, strFile);


			HTuple hvDiff = m_Commuicate.n2ndLengthInPixel - nRejectDelay1;
			ExceptionRemain(hvDiff, "����ʱ���2nd�ӳ�����");

			ComPareChange(m_Commuicate.n2ndLengthInPixel, nRejectDelay1, "����ʱ���2nd�ӳ�����");
			m_Commuicate.n2ndLengthInPixel = nRejectDelay1;
		}
#pragma  endregion 

	}

}



void CCommunicate::ReadParam()
{

	///һЩ���е�����Ӧ�ò�Ҫ���Ų�Ʒ��


	m_Common.emCoatType = (e_CoatType)GetPrivateProfileInt(m_strProductName, "CoatType", 0, m_strConfigSystemFile);
	m_Common.dRowResolution[0] = GetPrivateProfileDouble("GRABBER0", "ROWRESOLUTION", 0.056600, m_strConfigCommonFile);
	m_Common.dRowResolution[1] = GetPrivateProfileDouble("GRABBER1", "ROWRESOLUTION", 0.056600, m_strConfigCommonFile);
	m_Common.dColumnResolution[0] = GetPrivateProfileDouble("GRABBER0", "COLUMNRESOLUTION", 0.055103, m_strConfigCommonFile);
	m_Common.dColumnResolution[1] = GetPrivateProfileDouble("GRABBER1", "COLUMNRESOLUTION", 0.055103, m_strConfigCommonFile);

	m_Commuicate.nRejectType = GetPrivateProfileInt("IO", "REJECTTYPE", 0, m_strConfigCommunicateFile);
	//���������������Ʒ�ֱ仯
	m_Commuicate.dFirstLabelLength = GetPrivateProfileDouble("IO", "dFirstLabelLength", 6429, m_strConfigCommonFile);
	m_Commuicate.dSecondLabelLength = GetPrivateProfileDouble("IO", "dSecondLabelLength", 6327, m_strConfigCommonFile);

	m_Commuicate.nXLengthInPixel = GetPrivateProfileInt("IO", "dXLengthInPixel", 0, m_strConfigCommunicateFile);
	m_Commuicate.n2ndLengthInPixel = GetPrivateProfileInt("IO", "d2ndLengthInPixel", 0, m_strConfigCommunicateFile);
	m_Commuicate.dLabelPosition = GetPrivateProfileInt("IO", "dLabelPosition", 500, m_strConfigCommunicateFile);


	m_Commuicate.bMachineLengthLabel = GetPrivateProfileInt("IO", "bMachineLengthLabel", 0, m_strConfigCommunicateFile);

	m_Commuicate.bUseLabelOffset = GetPrivateProfileInt("IO", "bUseLabelOffset", 0, m_strConfigCommunicateFile);
	m_Commuicate.dLabelOffsetDev = GetPrivateProfileDouble("IO", "dLabelOffsetDev", 20, m_strConfigCommunicateFile);
	m_Commuicate.dUpLabelOffsetCompensate = GetPrivateProfileDouble("IO", "dUpLabelOffsetCompensate", 0, m_strConfigCommunicateFile);
	m_Commuicate.dDownLabelOffsetCompensate = GetPrivateProfileDouble("IO", "dDownLabelOffsetCompensate", 0, m_strConfigCommunicateFile);



	m_Commuicate.nTiFeiPulse = GetPrivateProfileInt("IO", "nTiFeiPulse", 20, m_strConfigCommunicateFile);
	m_Commuicate.nBuChangHangShu = GetPrivateProfileInt("IO", "nBuChangHangShu", 1162, m_strConfigCommunicateFile);
	m_Commuicate.dVirtualEALength = GetPrivateProfileDouble("IO", "dVirtualEALength", 200, m_strConfigCommunicateFile);

	CString strSingleName;
	m_Commuicate.hvLabelOffsetCompensateTuple.Clear();
	for (int i = 0; i < MAX_LALBELNUM; i++)
	{
		strSingleName.Format("dSingleTripOffset%d", i);
		m_Commuicate.hvLabelOffsetCompensateTuple.Append(GetPrivateProfileDouble("LabelOffsetTuple", strSingleName, 0, m_strConfigCommunicateFile));
	}

	TupleGenConst(MAX_LALBELNUM, 0, &m_hvRealMonitorLengthTuple);
	TupleGenConst(m_nTripNum, 0, &m_hvRealLabelCountTuple);



	if (m_Commuicate.nRejectType == REJECT_DEFULT)
	{
		m_tLabelControlInfo.bEnableFirstLabel = 1;
		m_tLabelControlInfo.bEnableSecondLabel = 1;

	}
	else
	{
		m_tLabelControlInfo.bEnableFirstLabel = 0;
		m_tLabelControlInfo.bEnableSecondLabel = 0;
	}


	CString strKeyName;
	int nChannelValue;
	m_Commuicate.hvChannelChoice.Clear();
	for (int i = 0; i < MAX_LALBELNUM; i++)///ͨ��ѡ��
	{
		strKeyName.Format("TripLabel%d", i);
		nChannelValue = GetPrivateProfileInt("LabelChannelSet", strKeyName, i, m_strConfigCommunicateFile);
		m_Commuicate.hvChannelChoice.Append(nChannelValue);

	}



	m_tLabelControlInfo.dFirstLabelLength = m_Commuicate.dFirstLabelLength;
	m_tLabelControlInfo.dSecondLabelLength = m_Commuicate.dSecondLabelLength;
	m_tLabelControlInfo.dLabelActionPosition = m_Commuicate.dLabelPosition;
	RefreshDelayInfo();///ÿ���������ʱ����Ҫ���¼���һ�½��з���
	/////////////////////////////////////////
	if (m_IOCard[0].hasCard)
	{
		int nUseMarkMode, nXDelay, n2ndDelay, nInsideMarkDelay, nOutSideMarkDelay, nInSideMarkToLabelDist, nOutSideMarkToLabelDist;

		nXDelay = m_IOCard[0].readParam(49);//������x
		n2ndDelay = m_IOCard[0].readParam(76);//�ڶ�·�����ʱ��

		ComPareChange(nXDelay, m_Commuicate.nXLengthInPixel, "����nXLengthInPixelΪ��׼");
		ComPareChange(n2ndDelay, m_Commuicate.n2ndLengthInPixel, "����n2ndLengthInPixelΪ��׼");


		//////ÿ��������Ҫд��ӿڿ���ǰ�����ļ��е�����
		m_IOCard[0].writeParam(49, m_Commuicate.nXLengthInPixel);//������x
		m_IOCard[0].writeParam(76, m_Commuicate.n2ndLengthInPixel);//�ڶ�·�����ʱ����

		m_IOCard[0].writeParam(172, 0);//�ر�Mark����ʹ��


		CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
		CString strApp = "PIO24B";
		CString strValue;
		strValue.Format("0x%x", m_Commuicate.nXLengthInPixel);
		WritePrivateProfileString(strApp, "49", strValue, strFile);
		strValue.Format("0x%x", m_Commuicate.n2ndLengthInPixel);
		WritePrivateProfileString(strApp, "76", strValue, strFile);

		WritePrivateProfileString(strApp, "176", strValue, strFile);


	}

}





