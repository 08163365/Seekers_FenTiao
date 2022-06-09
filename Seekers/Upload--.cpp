#include "StdAfx.h"
#include <regex>
#include "Upload.h"
#include "SeekersDlg.h"
#include "DlgProductManager.h"

#pragma once
CUpload* g_This = NULL;
CUpload::CUpload()
{
	m_bComSocket = FALSE;
	for (int i=0;i<CAMERA_COUNT;i++)
	{
		m_bSocket[i] = FALSE;
	}
	
}


CUpload::~CUpload()
{
	//WriteParam();
}

void T_Connect()  
{
	HTuple hvError, hvOpenStatus, ParamValue;
	while (FALSE == g_bExitThread)
	{


		try
		{
			for (int i=0;i<CAMERA_COUNT;i++)
			{
				if (FALSE == g_CUpLoad.m_bSocket[i])
				{
					try
					{

						HALCON_TRY(CloseSocket(g_CUpLoad.m_Socket[i]));
						OpenSocketConnect(HTuple(g_CUpLoad.m_tParam.strAddressData),g_CUpLoad.m_tParam.nPortData[i], HTuple(), HTuple(), &g_CUpLoad.m_Socket[i]);
						SetSocketParam(g_CUpLoad.m_Socket[i], "timeout", 3);
					    g_CUpLoad.m_bSocket[i] = TRUE;
						//	OpenSocketConnect(HTuple(g_CUpLoad.m_tParam.strAddressData),g_CUpLoad.m_tParam.nPortData[i], "timeout", 1, &g_CUpLoad.m_Socket[i]);
						CString strLog;
						strLog.Format("���ݷ�����%s:%d���ӳɹ�", g_CUpLoad.m_tParam.strAddressData, g_CUpLoad.m_tParam.nPortData[i]);
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
						
					}
					catch (HException e)
					{

						CString strLog;
						strLog.Format("���ݷ�����%s:%d����ʧ��1", g_CUpLoad.m_tParam.strAddressData, g_CUpLoad.m_tParam.nPortData[i]);
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
						g_CUpLoad.m_bSocket[i] = FALSE;
						
					}
				}
				if (FALSE == g_CUpLoad.m_bComSocket)
				{
					try
					{
						HALCON_TRY(CloseSocket(g_CUpLoad.m_ComSocket));
						OpenSocketConnect(HTuple(g_CUpLoad.m_tParam.strAddressCom), g_CUpLoad.m_tParam.nPortCom, HTuple(), HTuple(), &g_CUpLoad.m_ComSocket);
						SetSocketParam(g_CUpLoad.m_ComSocket, "timeout", 3);
						CString strLog;
						strLog.Format("ͨ�÷�����%s:%d���ӳɹ�", g_CUpLoad.m_tParam.strAddressCom, g_CUpLoad.m_tParam.nPortCom);
						g_CUpLoad.m_bComSocket = TRUE;
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
					}
					catch (HException e)
					{
						CString strLog;
						strLog.Format("ͨ�÷�����%s:%d����ʧ��", g_CUpLoad.m_tParam.strAddressCom, g_CUpLoad.m_tParam.nPortCom);
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
						g_CUpLoad.m_bComSocket = FALSE;
					}

				}else
				{
					if (((CSeekersDlg*)g_pMainDialog)->m_bRunning==FALSE)
					{
						
						int nSN=0;
						CString strExcepValue;
						strExcepValue.Format("%s","覴ü�����ֹͣ�����飡");
						g_CUpLoad.SendExceptionWarn(nSN,strExcepValue);
				        WaitSeconds(5);
					}
				}
			}
		}
		catch (HalconCpp::HException e)
		{
		}
		WaitSeconds(0.5);


	}
}

void CUpload::Upload(C_ALLOCATORWORKSHOPOUTPUT& C)
{
	try
	{
		HTuple hvMessageSend;
		CreateMessage(&hvMessageSend);
		C.WriteMessage(hvMessageSend);
		EnqueueMessage(m_hvQueue, hvMessageSend, HTuple(), HTuple());
		ClearMessage(hvMessageSend);//��ϢͶ�����,�����Ϣ
	}
	catch (HException& e)
	{
	}
}



void T_ListenOffsetSocket()
{

	try
	{
		OpenSocketConnect(HTuple(g_CUpLoad.m_tParam.strOffsetServerIP), g_CUpLoad.m_tParam.nOffsetServerPort, HTuple(), HTuple(), &g_CUpLoad.m_hvOffsetSocket);
		SetSocketParam(g_CUpLoad.m_hvOffsetSocket, "timeout", 5);
		g_CUpLoad.m_bOffsetServerConnect = TRUE;
	}
	catch (HException e)
	{
		CString strLog;
		strLog.Format("�ջ�IP:%s:%d ���ӽ���ʧ�� ʧ��ԭ��%s ", g_CUpLoad.m_tParam.strOffsetServerIP, g_CUpLoad.m_tParam.nOffsetServerPort,e.ErrorMessage().Text());
		g_CUpLoad.m_bOffsetServerConnect = FALSE;
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
	}
	HTuple hvError, hvOpenStatus, ParamValue;
	while (FALSE == g_bExitThread)
	{
		if (FALSE == g_CUpLoad.m_bOffsetServerConnect)//�����������ʧ�ܣ���ô1������һ��
		{
			try
			{
				OpenSocketConnect(HTuple(g_CUpLoad.m_tParam.strOffsetServerIP), g_CUpLoad.m_tParam.nOffsetServerPort, HTuple(), HTuple(), &g_CUpLoad.m_hvOffsetSocket);
				CString strLog;
				strLog.Format("�ջ�IP:%s:%d �������ӽ����ɹ�", g_CUpLoad.m_tParam.strOffsetServerIP, g_CUpLoad.m_tParam.nOffsetServerPort);
				g_CUpLoad.m_bOffsetServerConnect = TRUE;
				g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
			}
			catch (HException e)
			{
				CString strLog;
				strLog.Format("�ջ�IP:%s:%d �������ӽ���ʧ��", g_CUpLoad.m_tParam.strOffsetServerIP, g_CUpLoad.m_tParam.nOffsetServerPort);
				g_CUpLoad.m_bOffsetServerConnect = FALSE;
				if (g_nGrabberType==1)
				{
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
					g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)strLog,NULL);
				}
			
			}
		}
		WaitSeconds(3);

		
	}
}

BOOL CUpload::ReadParam()
{
	
	m_strConfigPath.Format("%s\\Config\\SCIPLC.ini", GetCurrentAppPath());
	m_tParam.strAddressCom = GetPrivateProfileCString("Upload", "strAddressCom", "127.0.0.1", m_strConfigPath);
	m_tParam.strAddressData = GetPrivateProfileCString("Upload", "strAddressData", "127.0.0.1", m_strConfigPath);
	m_tParam.nPortCom = GetPrivateProfileInt("Upload", "nPortCom", 4000, m_strConfigPath);
	m_tParam.strOffsetServerIP = GetPrivateProfileCString("Upload", "strOffsetServerIP", "127.0.0.1", m_strConfigPath);
	m_tParam.nOffsetServerPort = GetPrivateProfileInt("Upload", "nOffsetServerPort", 4000, m_strConfigPath);
	m_tParam.nBackDataSourceIndex = GetPrivateProfileInt("Upload", "nBackDataSourceIndex", 0, m_strConfigPath);
	m_tParam.dMaxDataLimit = GetPrivateProfileDouble("Upload", "dMaxDataLimit", 2, m_strConfigPath);
	m_tParam.nNGNumToStopMachine = GetPrivateProfileInt("Upload", "nNGNumToStopMachine", 5, m_strConfigPath);

	m_tParam.bUseJeket= GetPrivateProfileInt("Upload", "bUseJeket", 0, m_strConfigPath);
	
	m_StrGongWeiName= GetPrivateProfileCString("Upload", "��λ", "UpOffset", m_strConfigPath);
	for (int i=0;i<CAMERA_COUNT;i++)
	{
		CString strPort;
		strPort.Format("nPortData%d", i);
		m_tParam.nPortData[i] = GetPrivateProfileInt("Upload", strPort, 4001, m_strConfigPath);
		TupleGenConst(g_Count.nType,0,&m_hvErrorCount[i]);//���ɴ������ͳ��ȵ�ͳ����
	}
	return TRUE;
}

BOOL CUpload::WriteParam()
{
	WritePrivateProfileString("Upload", "strAddressCom", m_tParam.strAddressCom, m_strConfigPath);
	WritePrivateProfileString("Upload", "strAddressData", m_tParam.strAddressData, m_strConfigPath);
	WritePrivateProfileString("Upload", "��λ", m_StrGongWeiName, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nPortCom", m_tParam.nPortCom, m_strConfigPath);
    WritePrivateProfileString("Upload", "strOffsetServerIP", m_tParam.strOffsetServerIP, m_strConfigPath);

    WritePrivateProfileInt("Upload", "bUseJeket", m_tParam.bUseJeket, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nBackDataSourceIndex", m_tParam.nBackDataSourceIndex, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nOffsetServerPort", m_tParam.nOffsetServerPort, m_strConfigPath);
	WritePrivateProfileDouble("Upload", "dMaxDataLimit", m_tParam.dMaxDataLimit, m_strConfigPath);
	WritePrivateProfileInt("Upload", "nNGNumToStopMachine", m_tParam.nNGNumToStopMachine , m_strConfigPath);
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
	CreateMessageQueue(&m_hvQueue);//����һ��IO����
	SetMessageQueueParam(m_hvQueue, "max_message_num", 10);
	HDevThreadContext Context;
	HDevThread* pThread = new HDevThread(Context,
		(void*)HThreadUpload, 1, 0);//����һ������1��������߳�;
	pThread->SetInputCtrlParamTuple(0, m_hvQueue);
	pThread->Start();
	ReadParam();
	AfxBeginThread((AFX_THREADPROC)T_Connect, NULL);//�������Ķ˿ڴ�
	AfxBeginThread((AFX_THREADPROC)T_ComSocket, NULL);//�������Ķ˿ڴ�
	AfxBeginThread((AFX_THREADPROC)T_ListenOffsetSocket, NULL);//���������۵Ķ˿ڴ�
	return TRUE;
}
CString CUpload::GetFileVersion(CString strFileName)
{
#pragma comment(lib, "Version.lib ")
	//��ȡ��ǰ�ļ��İ汾��Ϣ
	DWORD dwLen = GetFileVersionInfoSize(strFileName,NULL); 
	char *pszAppVersion = new char[dwLen+1];
	CString strVersion;
	if(pszAppVersion)
	{
		memset(pszAppVersion,0,sizeof(char)*(dwLen+1));
		GetFileVersionInfo(strFileName,NULL,dwLen,pszAppVersion);

		UINT nLen(0);
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		VerQueryValue(pszAppVersion,"\\",(LPVOID*)&pFileInfo,&nLen);
		if(pFileInfo)
		{
			//��ȡ�汾��
			strVersion.Format("%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strVersion;
}

void* CUpload::HThreadUpload(void *hcthread)//io�߳�
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
	// Windows���ļ����в��ܰ���\/:*?"<>|��Щ�ַ�
	std::regex regExpress("[\\/:*?\"<>|]");
	BOOL bValid = !std::regex_search(strFileName, regExpress);
	return bValid;
}
void T_ComSocket()
{
	HTuple hvTimeStart(0), hvTimeEnd(0);
	HTuple hv_TimeNoDataStart(0),hv_TimeNoDataEnd(0);
	Sleep(5000);
	while (FALSE == g_bExitThread)
	{
		HTuple hvDataType;
		try
		{
			GetNextSocketDataType(g_This->m_ComSocket, &hvDataType);
		}
		catch (HException e)
		{
			CountSeconds(&hvTimeEnd);
			if (hvTimeEnd - hvTimeStart > 5 && TRUE ==g_This->m_bComSocket && hvTimeStart!=0 && hvTimeEnd!=0)//���5����û���յ��κ��ź��ˣ�֤���Ͽ��ˣ���������
			{
				g_This->m_bComSocket = FALSE;

				CString strReceieveInfo;
				strReceieveInfo.Format("%s","GetNextSocketDataType�����쳣�������ͨѶ�Ͽ�");
				g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);

			}
			continue;
		}
		CountSeconds(&hvTimeStart);
		CString strRecvData;
		strRecvData.Format("%s", hvDataType.S().Text());
		if ("tuple" == hvDataType)//�����������ָ��
		{

			try
			{
				CountSeconds(&hv_TimeNoDataStart);
				HTuple hvData;
				ReceiveTuple(g_This->m_ComSocket, &hvData);
				if (hvData.Length()>2 && "HX" == hvData[0])//���Ͳ�����HX
				{
					//if (hvData[1].I()==1)
					{
						if (g_bUseAutoFlag==TRUE)
						{
							static BOOL s_bWriteFlag[2]={FALSE,FALSE};
							CString strNewProductName=hvData[2].S().Text();

							CString strReceieveInfo;
							strReceieveInfo.Format("�������·�����ָ��:�ͺ�����:%s",strNewProductName);
							g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
							BOOL bExist=FALSE;
							int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//��ȡ��ǰ����
							CString strKey,strValue;
							for (int i=0;i<nCurrentCount;i++)
							{
								strKey.Format("PRODUCT%d",i);
								GetPrivateProfileString("PRODUCT",strKey,0,strValue.GetBuffer(MAX_PATH),MAX_PATH,g_strConfigSystemFile);
								strValue.ReleaseBuffer();
								if (strValue==strNewProductName)
								{
									g_strProductName = strValue;
									bExist=TRUE;
								}
							}
							///����Ʒ���ͺ� �������ֱ�ӵ��� ����Ҫ����д�����
							if (bExist)
							{
								CString strName;
								int nTrip=(hvData[1].I()+1)%2;
								if (g_tSystemSet.nMergeParam==1)
								{
									nTrip=0;
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									nTrip=1;
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									s_bWriteFlag[0]=TRUE;
									s_bWriteFlag[1]=TRUE;
								}
								else
								{
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									s_bWriteFlag[nTrip]=TRUE;
								}
								if (s_bWriteFlag[0]==TRUE && s_bWriteFlag[1]==TRUE)
								{
									s_bWriteFlag[0]=FALSE;
									s_bWriteFlag[1]=FALSE;
									g_bChangeProduct=TRUE;
									((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
								}
							}else
							{
								CString strName;
								int nTrip=(hvData[1].I()+1)%2;
								if (g_tSystemSet.nMergeParam==1)
								{
									nTrip=0;
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									nTrip=1;
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									s_bWriteFlag[0]=TRUE;
									s_bWriteFlag[1]=TRUE;
								}else
								{
									strName.Format("%s\\�ͺŲ���%d.tup",GetCurrentAppPath(),nTrip);
									WriteTuple(hvData,(HTuple)strName);
									s_bWriteFlag[nTrip]=TRUE;
								}
								if (s_bWriteFlag[0]==TRUE && s_bWriteFlag[1]==TRUE)
								{
									s_bWriteFlag[0]=FALSE;
									s_bWriteFlag[1]=FALSE;
									CDlgProductManager dlg;
									dlg.CreateNewProduct(strNewProductName);
									dlg.CopyProduct(g_strProductName,strNewProductName);
									Sleep(3000);
									g_strProductName = strNewProductName;
									HTuple hvIndex;
									TupleFind(hvData,"JX",&hvIndex);
									if (hvIndex.Length()==2)
									{

										/*
										[HX����·��ţ�Ĥ���ͺţ�DY����ȣ���ȹ��DY��CK����ȣ���ȹ��CK��
										JE��AT9��ȣ�AT9��ȹ���ܳ����ܳ������ȣ���ȹ��������ȣ�������ȹ������������������������߹���������������๫�JEH�������ߣ�JEH��JEW��������JEW��JED��������࣬JED��JE��XH��ѭ��״̬(0/1),XH��
										JX, ������(0)/��Ъ��(1)��ǰ�����Ъ�߶ȣ�ǰ�����Ъ�߶ȹ���������Ъ�߶ȣ��������Ъ�߶ȹ���׼�����࣬�׼�����๫�JX��HX]
										*/
										HTuple hv_Min,hv_JXFlag;
										TupleMin(hvIndex,&hv_Min);
										hv_JXFlag=hv_Min[0].I()+1;
										if (hvData[hv_JXFlag.I()]==1)
										{
											g_ProductParam.emCoatType=M_TwoCam_Intermission;
										}else
										{
											g_ProductParam.emCoatType=M_TwoCam_TwoTrip;
										}

										SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
										WritePrivateProfileInt(g_strProductName,"CoatType",g_ProductParam.emCoatType,g_strConfigSystemFile);
										SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ
									}

									g_bChangeProduct=TRUE;
									((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
								}
							}
							/*	///����Ʒ���ͺ�
							CString strName;
							strName.Format("%s\\�ͺŲ���0.tup",GetCurrentAppPath());
							WriteTuple(hvData,(HTuple)strName);
							strName.Format("%s\\�ͺŲ���1.tup",GetCurrentAppPath());
							WriteTuple(hvData,(HTuple)strName);*/
						}

					}

				}
                else if("Close"== hvData[0])//�ر����
				{
					CString strReceieveInfo;
					strReceieveInfo.Format("%s","�������·��ر����ָ��");
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
					g_bChangeProduct=TRUE;
					((CSeekersDlg*)g_pMainDialog)->OnBnClickedButtonExitApplication();

				}
				else if (hvData.Length()>2 && 4== hvData[0])//�Զ�����  [4 ·�� ���]
				{
					{
						CString strBatch,strBatchPath;
						strBatch=hvData[2].S().Text();
						strBatchPath=hvData[1].S().TextA();//�������·��ĸþ�Ĵ洢·�� 

						g_Jugde[0].m_nABCuoWeiType=NormalDataBack;
						g_Jugde[1].m_nABCuoWeiType=NormalDataBack;


						CString strDest,strTemp,strReplace;
						strDest.Format("ccd");
						int nPosition =strBatchPath.Find(strDest);
						if (nPosition!=-1)
						{
							/*strReplace.Format("%s\\Map",GetCurrentAppPath());*/
							strReplace="D:\\";
							strBatchPath.Replace(strBatchPath.Left(nPosition-strDest.GetLength()+2),strReplace);
							CreateMultilevelPath(strBatchPath);

						}else
						{
							g_bWriteFlag=TRUE;
						}



						if (strBatch==g_strBatch && strBatch!=" ")
						{
							continue;

						}else if (FALSE==IsValidFileName(strBatch.GetBuffer()))
						{



							CString strReceieveInfo;
							strReceieveInfo.Format("�������·��Ƿ����:%s �·��洢·��:%s ��ִ�л������ ���ش洢״̬:%d",strBatch,strBatchPath,g_bWriteFlag);
							g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
							continue;
						}

						SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//�洢��ű����쳣��ʧ
						WritePrivateProfileString("INFO","BATCH",g_strBatch,g_strConfigSystemFile);
						
						CString strLastBasePath,StrSendMergePath;
						g_pStorage->DoSomething("GETPARAM","SaveLastBacthPath",strLastBasePath.GetBuffer(500));
						strLastBasePath.ReleaseBuffer();
						StrSendMergePath.Format("%s\\覴�����",strLastBasePath);
						g_pStorage->DoSomething("GETPARAM","WRITEFLAG" ,&g_bWriteFlag);
						//��ȡ�ϴεĴ洢·��
						CString strReceieveInfo;
						strReceieveInfo.Format("�������·����:%s ���ش洢״̬:%d",strBatchPath,g_bWriteFlag);
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);

						CDlgSwitchBatch dlg;
						dlg.m_strNewBatch=strBatch;
						dlg.m_strNewBatchPath=strBatchPath;
						dlg.AutoChangeBatch();

						if(g_bWriteFlag==TRUE)//Ҫ��д�뱾��
						{
							SYSTEMTIME tTime;
							GetLocalTime(&tTime);
							g_strSaveBasePath.Format("%s\\Map\\ccd_data\\%02d-%d\\%d\\%s-%s",GetCurrentAppPath(),tTime.wYear,tTime.wMonth,tTime.wDay,g_strProductName,strBatch);
							g_strSaveFullPath.Format("%s\\覴�����",g_strSaveBasePath);
						}
						Sleep(500);//��ֹ�����¼û��д��
						HTuple hv_FilePath;
						hv_FilePath.Append(7);
						hv_FilePath.Append((HTuple)StrSendMergePath);
						SendTuple(g_This->m_ComSocket, hv_FilePath);//�ظ��ϲ�·��


						if (TRUE==g_tSystemSet.bEnableReloadParam)//����ԭ����
						{
							for(int k=0;k<4;k++)
							{
								g_vec_Algorithm[k]->DoSomething("SETPARAM","ReLoadParam",NULL);
							}
						}
						


					}

				}
				else if (hvData=="Version")//��ȡϵͳ�İ汾��
				{
					CString strSeekers,strVersion;
					HTuple hv_FileVersion;
					hv_FileVersion.Append(6);
					strSeekers.Format("%s\\Seekers.exe",GetCurrentAppPath());
					strVersion.Format("Ver:%s",g_StrCurrentVersion);
					/*strVersion.Format("Ver:%s",g_CUpLoad.GetFileVersion(strSeekers));*/
					hv_FileVersion.Append((HTuple)strVersion);
					hv_FileVersion.Append(g_hvVertionInfoTuple);
					SendTuple(g_This->m_ComSocket, hv_FileVersion);//�ظ��汾��Ϣ

				}
				else if (hvData.Length()>0 && hvData[0]=="GET_TBKZ")//��������ȡ������Ʋ���
				{

					CString strReceieveInfo;
					strReceieveInfo.Format("%s","��������ȡ�������ȿ��Ʋ���");
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);

					LabelControlInfo tLabelControlInfo;
					HTuple hv_SendTuple;
					hv_SendTuple.Clear();
					g_pCommunicate->DoSomething("GETPARAM","LabelControlInfo",&tLabelControlInfo);
					hv_SendTuple.Append("GET_TBKZ");
					hv_SendTuple.Append(tLabelControlInfo.dFirstLabelLength);
					hv_SendTuple.Append(tLabelControlInfo.bEnableFirstLabel);
					hv_SendTuple.Append(tLabelControlInfo.dSecondLabelLength);
					hv_SendTuple.Append(tLabelControlInfo.bEnableSecondLabel);
					hv_SendTuple.Append(tLabelControlInfo.dLabelActionPosition);
					hv_SendTuple.Append(g_Jugde[0].m_tStandardSize.hvCutPosition);
					SendTuple(g_This->m_ComSocket, hv_SendTuple);//�ظ����������Ϣ

				}
				else if (hvData.Length()>0 && hvData[0]=="SET_TBKZ")//��������ȡ������Ʋ���
				{
					CString strReceieveInfo;
					strReceieveInfo.Format("%s","�����������������ȿ��Ʋ���");
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
					LabelControlInfo tLabelControlInfo;
					tLabelControlInfo.dFirstLabelLength=hvData[1].D();
					tLabelControlInfo.bEnableFirstLabel=hvData[2].D();
					tLabelControlInfo.dSecondLabelLength=hvData[3].D();
					tLabelControlInfo.bEnableSecondLabel=hvData[4].D();
					tLabelControlInfo.dLabelActionPosition=hvData[5].D();

					if (hvData.Length()>6)//���Ӳ���λ��
					{
						g_Jugde[0].m_tStandardSize.hvCutPosition=hvData[6].D();
						g_Jugde[1].m_tStandardSize.hvCutPosition=hvData[6].D();
					}

					g_pCommunicate->DoSomething("SETPARAM","LabelControlInfo",&tLabelControlInfo);
					HTuple hv_SendTuple;
					hv_SendTuple.Clear();
					hv_SendTuple.Append("SET_TBKZ");
					SendTuple(g_This->m_ComSocket, hv_SendTuple);//�ظ����������Ϣ
				}
				else if (hvData.Length()>0 && hvData[0]=="TG")//����������覴�������
				{


					CString strReceieveInfo;
					strReceieveInfo.Format("%s ����:%d ��ǰ�ٶ�:%.1fm/min","�������������������",hvData.Length(),g_dRealSpeed);
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);

					try
					{
						HTuple hv_Info;
						HTupleVector  hvec_ResultVector(1);
						TupleLastN(hvData,1,&hv_Info);
						if (hv_Info.Length()%5==0 && hv_Info.Length()>0)
						{

							hvec_ResultVector = HTupleVector(hv_Info, 5);
							for (int k=0;k<2;k++)
							{
								STANDARDSURFACE tStandarSurface;
								tStandarSurface.hv_ErrorIndex.Clear();
								tStandarSurface.hv_TotalMaxArea.Clear();
								tStandarSurface.hv_TotalMaxNumber.Clear();
								tStandarSurface.hv_TotalMaxAreaTwo.Clear();
								tStandarSurface.hv_TotalMaxNumberTwo.Clear();
								tStandarSurface.hv_SingleMaxWidth.Clear();
								tStandarSurface.hv_EnablePopWindow.Clear();
								for (int i=0;i<hvec_ResultVector.Length();i++)
								{
									if (((const HTuple&)hvec_ResultVector[i].T())[1] == 0 || hvec_ResultVector[i].T()[0]>g_Count.nType)
									{
										continue;
									}
									tStandarSurface.hv_ErrorIndex.Append( ((const HTuple&)hvec_ResultVector[i].T())[0]);
									tStandarSurface.hv_TotalMaxArea.Append( ((const HTuple&)hvec_ResultVector[i].T())[1]);
									tStandarSurface.hv_TotalMaxNumber.Append( ((const HTuple&)hvec_ResultVector[i].T())[2]);
									tStandarSurface.hv_TotalMaxAreaTwo.Append( ((const HTuple&)hvec_ResultVector[i].T())[3]);
									tStandarSurface.hv_TotalMaxNumberTwo.Append( ((const HTuple&)hvec_ResultVector[i].T())[4]);
									tStandarSurface.hv_SingleMaxWidth.Append(0);
									tStandarSurface.hv_EnablePopWindow.Append(0);
								}
								tStandarSurface.nErrorCount=g_Count.nType;
								tStandarSurface.nSecErrorCount =tStandarSurface.hv_ErrorIndex.Length();


								WritePrivateProfileInt("SurfaceParam","nErrorCount",tStandarSurface.nErrorCount,g_Jugde[k].m_strConfigFile);
								WritePrivateProfileInt("SurfaceParam","nSecErrorCount",tStandarSurface.nSecErrorCount,g_Jugde[k].m_strConfigFile);
								for (int i=0;i<tStandarSurface.nSecErrorCount;i++)
								{
									CString strAppName;
									strAppName.Format("nError%d",i);
									WritePrivateProfileInt(strAppName,"hv_ErrorIndex",tStandarSurface.hv_ErrorIndex[i].I(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileInt(strAppName,"hv_TotalMaxNumber",tStandarSurface.hv_TotalMaxNumber[i].I(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileDouble(strAppName,"hv_TotalMaxArea",tStandarSurface.hv_TotalMaxArea[i].D(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileInt(strAppName,"hv_TotalMaxNumberTwo",tStandarSurface.hv_TotalMaxNumberTwo[i].I(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileDouble(strAppName,"hv_TotalMaxAreaTwo",tStandarSurface.hv_TotalMaxAreaTwo[i].D(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileDouble(strAppName,"hv_SingleMaxWidth",tStandarSurface.hv_SingleMaxWidth[i].D(),g_Jugde[k].m_strConfigFile);
									WritePrivateProfileInt(strAppName,"hv_EnablePopWindow",tStandarSurface.hv_EnablePopWindow[i].I(),g_Jugde[k].m_strConfigFile);
								}
								g_Jugde[k].m_tStandardSurface=tStandarSurface;
							
							}
						}
					}catch(HException& e)
					{
						strReceieveInfo.Format("��������������������쳣����:%s �������ݣ�%s",e.ProcName().Text(),e.ErrorMessage().Text());
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
					}
					
				}
				else if (5 == hvData[0] && hvData.Length()==2 )//�����������滻�ͺ�
				{


					CString strNewProductName=hvData[1].S().Text();

					CString strReceieveInfo;
					strReceieveInfo.Format("�������������·�����ָ��:�ͺ�����:%s",strNewProductName);
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
					BOOL bExist=FALSE;
					int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//��ȡ��ǰ����
					CString strKey,strValue;
					for (int i=0;i<nCurrentCount;i++)
					{
						strKey.Format("PRODUCT%d",i);
						GetPrivateProfileString("PRODUCT",strKey,0,strValue.GetBuffer(MAX_PATH),MAX_PATH,g_strConfigSystemFile);
						strValue.ReleaseBuffer();
						if (strValue==strNewProductName)
						{

							bExist=TRUE;
						}
					}

					HTuple hv_RebackData;
					hv_RebackData.Clear();
					if (bExist)
					{
						if (strNewProductName!=g_strProductName)
						{
							g_strProductName=strNewProductName;
							g_bChangeProduct=TRUE;
							((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
						}
						hv_RebackData.Append(5);
						SendTuple(g_This->m_ComSocket, hv_RebackData);//����5 ��ǰ�ͺŴ���

					}else
					{
						hv_RebackData.Append(6);//�������в�Ʒ ���ͺŲ�Ʒ������ �򷵻�6
						SendTuple(g_This->m_ComSocket, hv_RebackData);//�����Ƿ����
					}
				}
				else if (hvData.Length()>0 && "Side" == hvData[0])//[Side,��0/��1/˫2,Side]
				{

					//˫ �� �� 0 1 2
					if (hvData[1]==0)//��
					{
						g_ProductParam.nFoilInSide=1;

					}else if (hvData[1]==1)//��
					{

						g_ProductParam.nFoilInSide=2;

					}else if (hvData[1]==2)//˫
					{
						g_ProductParam.nFoilInSide=0;
					}
					SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
					WritePrivateProfileInt(g_strProductName,"nFoilInSide",g_ProductParam.nFoilInSide,g_strConfigSystemFile);
					CString strReceieveInfo;
					strReceieveInfo.Format("%s","�������·���˫����ָ��");
					g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
					g_bChangeProduct=TRUE;
					((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS,0,0);
					/*((CSeekersDlg*)g_pMainDialog)->OnBnClickedButtonExitApplication();*/
				}
				else if (hvData.Length()>0&&hvData[0]=="PassWord")//Ȩ�޲���
				{
					//0ע�� 1����Ա 2����ʦ 3���� 4IPQC--- �˺�---����
					CString strStatus;
					CString str,strBtnText;
					CString strInfo;
					if (hvData[1]==0)//ע��
					{
					
						g_bExitLogin=FALSE;
						str="�û���¼";
						g_CPowerLimit.ClearFlag();
						strInfo="�������·�ע��";
					}else 
					{
					
						if (hvData[1] == 1)//����Ա
						{
							g_CPowerLimit.m_bOperatorPower = TRUE;
							strStatus = "����Ա";

						}else if(hvData[1] == 2)//����ʦ
						{
							g_CPowerLimit.m_bSuperEngineerPower=TRUE;
							strStatus="����ʦ";

						}else if (hvData[1]==3)//����
						{
							g_CPowerLimit.m_bOperatorPower=TRUE;
							strStatus="����";

						}else //IPQC
						{
							g_CPowerLimit.m_bEngineerPower=TRUE;
							strStatus="IPQC";
							
						}
						strInfo.Format("Ȩ������:%s ����:%s",strStatus,hvData[2].S().Text());
						g_bExitLogin=TRUE;
						
						
						if (g_CPowerLimit.m_bSuperEngineerPower ||g_CPowerLimit.m_bFactoryPower)
						{
							for (int i=0;i<4;i++)
							{
								int nPowerFlag=1;
								g_vec_Algorithm[i]->DoSomething("SETPARAM","SetPowerFlag",&nPowerFlag);
							}
						}
						str.Format("�û�:%s",strStatus);
					}
					
					

					g_CPowerLimit.SetStatus(str);

					{
						///Log
						CString strLog;
						strLog.Format("%s��½ϵͳ",strInfo);
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
					}
				}
				else if (hvData.Length()>0 && "AT9alarm" == hvData[0])//[1,�ٶ�]
				{
					if (hvData[2]==1)//����1��ʱ���Ǽ�����Ϊmeanֵ�ջ�
					{
						g_Jugde[0].m_nABCuoWeiType=MeanDataBack;
						g_Jugde[1].m_nABCuoWeiType=MeanDataBack;
					}
					
				}
				else if (hvData.Length()>0 && 1 == hvData[0])//[1,�ٶ�]
				{
					SendTuple(g_This->m_ComSocket, 1);//�յ��������ź�
					g_dReceieveSpeed=hvData[1].D();

				}
			}
			catch (HException )
			{
				g_This->m_bComSocket = FALSE;
				CString strReceieveInfo;
				strReceieveInfo.Format("%s","tuple�����쳣�������ͨѶ�Ͽ�");
				g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
			}

		}
		else
		{
			if (hv_TimeNoDataEnd - hv_TimeNoDataStart > 10 && TRUE ==g_This->m_bComSocket&&hv_TimeNoDataStart!=0 &&hv_TimeNoDataEnd!=0)//���5����û���յ��κ��ź��ˣ�֤���Ͽ��ˣ���������
			{

				CString strReceieveInfo;
				strReceieveInfo.Format("%s","����10�յ���tuple�������� �������ͨ�ÿ��ƶ˿ڶϿ�");
				g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strReceieveInfo,NULL);
				g_This->m_bComSocket = FALSE;
			}else
			{
				CountSeconds(&hv_TimeNoDataEnd);
			}
		}
		
		Sleep(0.5);
	}
}

void  CUpload::GetGongWeiName(const C_ALLOCATORWORKSHOPOUTPUT &tOutput,CString& strComputerName)
{
	if (m_StrGongWeiName=="DownOffset")
	{
		if (tOutput.hvPackageType.I()==0)
		{
			strComputerName="��λ3";
		}else
		{
			strComputerName="��λ4";

		}
	}
	else
	{
		if (tOutput.hvPackageType.I()==0)
		{
			strComputerName="��λ1";
		}else
		{
			strComputerName="��λ2";

		}

	}


}

void CUpload::SendOriginalImage(const C_ALLOCATORWORKSHOPOUTPUT &tOutput,int& nGrabberSN)
{
	if (FALSE == m_bSocket[nGrabberSN])
	{
		return;
	}
	HObject ImageZoom;
	static int nCount[CAMERA_COUNT]={0,0};
	try
	{
		if (++nCount[nGrabberSN]%10==0)
		{
			ZoomImageFactor(tOutput.B[nGrabberSN].A.hoGrabImage, &ImageZoom, g_dScaleFactor, g_dScaleFactor, "nearest_neighbor");
			SendImage(ImageZoom, m_Socket[nGrabberSN]);
			nCount[nGrabberSN]=1;
		}

	}catch(HException& e)
	{
		m_bSocket[nGrabberSN]=FALSE;
		CString strLog;
		strLog.Format("SendImageʧ�� ����%s:%s",e.ProcName().Text(),e.ErrorMessage().Text());
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
	}

}

void CUpload::CountContinueErrorNum(C_ALLOCATORWORKSHOPOUTPUT &Output)
{

	try
	{
		if (g_ProductParam.emCoatType!=M_TwoCam_Intermission)
		{
			return;
		}
		int nSide=Output.hvPackageType.I();
		HTuple hv_SizeIndex,hv_SurfaceIndex;
		if (Output.tFinalResult.hv_SizeFlag==FALSE || Output.tFinalResult.hv_SurfaceFlag==FALSE)
		{
			for (int i=0;i<g_Count.nType;i++)
			{
				TupleFind(Output.tFinalResult.hv_SizeErrorIndex,i,&hv_SizeIndex);
				if (hv_SizeIndex!=-1 && hv_SizeIndex.Length()!=0)
				{
					m_hvErrorCount[nSide][i]=m_hvErrorCount[nSide][i]+1;
				}
				TupleFind(Output.tFinalResult.hv_SurfaceErrorIndex,i,&hv_SurfaceIndex);
				if (hv_SurfaceIndex!=-1 && hv_SurfaceIndex.Length()!=0)
				{
					m_hvErrorCount[nSide][i]=m_hvErrorCount[nSide][i]+1;
				}

			}
			HTuple hv_GreatIndex;
			TupleGreaterEqualElem(m_hvErrorCount[nSide],m_tParam.nNGNumToStopMachine,&hv_GreatIndex);
			TupleFind(hv_GreatIndex,1,&hv_GreatIndex);
			if (hv_GreatIndex.Length()>0 && hv_GreatIndex!=-1)
			{
				HTuple hv_MinIndex;

				TupleMin(hv_GreatIndex,&hv_MinIndex);
				m_hvErrorCount[nSide][hv_GreatIndex]=0;
				if (g_CUpLoad.m_bComSocket)
				{
					HTuple hv_SendTuple;
					CString strValue;
					CString strGongWeiName;
					int nCameraSN=0;
					int nErrorIndex=0;
					hv_SendTuple.Append(120);
					if (g_CUpLoad.m_StrGongWeiName=="DownOffset")//�´�·
					{

						if (nSide==0)//0�� ǰ���
						{
							nCameraSN=4;
							strGongWeiName="��λ��";
							nErrorIndex=hv_MinIndex[0].I()*2;
						}else//1�������
						{
							nCameraSN=5;
							strGongWeiName="��λ��";
							nErrorIndex=hv_MinIndex[0].I()*2+1;
						}
						strValue.Format("�´�·%s覴�����%d��EA��Ʒ��ͬNG����_%s_�������� %d",strGongWeiName,m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(hv_MinIndex[0].I()),nErrorIndex);
						hv_SendTuple.Append(nCameraSN);
						hv_SendTuple.Append((HTuple)strValue);
						hv_SendTuple.Append(nErrorIndex);

					}
					else//�ϴ�·
					{

						if (nSide==0)//0�� ǰ���
						{
							nCameraSN=2;
							strGongWeiName="��λһ";
							nErrorIndex=hv_MinIndex[0].I()*2;
						}else//1�������
						{
							nCameraSN=3;
							strGongWeiName="��λ��";
							nErrorIndex=hv_MinIndex[0].I()*2+1;
						}
						hv_SendTuple.Append(nCameraSN);
						strValue.Format("�ϴ�·%s覴�����%d��EA��Ʒ��ͬNG����_%s_�������� %d",strGongWeiName,m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(hv_MinIndex[0].I()),nErrorIndex);
						hv_SendTuple.Append((HTuple)strValue);
						hv_SendTuple.Append(nErrorIndex);
					}
					try
					{
						SendTuple(g_CUpLoad.m_ComSocket,hv_SendTuple);
					}catch(HException& e)
					{
						g_CUpLoad.m_bComSocket=FALSE;
					}

				}

			}

		}else
		{
			TupleGenConst(g_Count.nType,0,&m_hvErrorCount[nSide]);

		}
	}catch(HException& e)
	{
		CString strException;
		strException.Format("CountContinueErrorNum��ʾ��%s:%s",e.ProcName().Text(),e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
	}


}
void CUpload::SetWarnToServer(C_ALLOCATORWORKSHOPOUTPUT &Output)
{
	try
	{
		/////����ڵļ���
	
		/////////////////////////////////////////////
		BOOL  bExceptionSoftStatus=FALSE;
		g_pLog->DoSomething("GETPARAM","SoftStatus",&bExceptionSoftStatus);
		if (TRUE==bExceptionSoftStatus)
		{
			if (g_CUpLoad.m_bComSocket)
			{

				HTuple hv_SendTuple;
				int nCameraSN=0;
				hv_SendTuple.Append(120);
				if (g_CUpLoad.m_StrGongWeiName=="DownOffset")//�´�·
				{
					CString strMessage;
					hv_SendTuple.Append(4);
					strMessage.Format("�´�·覴ó����쳣,�����´�����Ų�ԭ��");
					hv_SendTuple.Append((HTuple)strMessage);
				}else
				{
					CString strMessage;
					hv_SendTuple.Append(2);
					strMessage.Format("�ϴ�·覴ó����쳣,�����´�����Ų�ԭ��");
					hv_SendTuple.Append((HTuple)strMessage);
				}
				try
				{
					SendTuple(g_CUpLoad.m_ComSocket,hv_SendTuple);
				}catch(HException& e)
				{
					g_CUpLoad.m_bComSocket=FALSE;
				}
			}
		}
		//////////////////////////////////////
		CountContinueErrorNum(Output);
		static int S_nContinueNum[2]={0,0};
		if(Output.bIsFinalResult==TRUE)
		{
			if (Output.tFinalResult.hv_SizeFlag==FALSE || Output.tFinalResult.hv_SurfaceFlag==FALSE)
			{
				S_nContinueNum[Output.hvPackageType.I()]++;

				if (S_nContinueNum[Output.hvPackageType.I()]>m_tParam.nNGNumToStopMachine)//����5��NG��ʱ���ͱ�����Ϣ
				{

					if (g_CUpLoad.m_bComSocket)
					{

						HTuple hv_SendTuple;
						int nCameraSN=0;
						hv_SendTuple.Append(120);
						if (g_CUpLoad.m_StrGongWeiName=="DownOffset")//�´�·
						{
							if (Output.hvPackageType.I()==0)//0�� ǰ���
							{
								nCameraSN=4;

							}else//1�������
							{
								nCameraSN=5;
							}
							hv_SendTuple.Append(nCameraSN);
							CString strName;
							if (Output.tFinalResult.hv_SizeErrorIndex.Length()>0)
							{
								strName.Format("�´�·覴�����%d��EA��ƷNG����_%s",m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(Output.tFinalResult.hv_SizeErrorIndex[0].I()));

							}else if (Output.tFinalResult.hv_SurfaceErrorIndex.Length()>0)
							{
								strName.Format("�´�·覴�����%d��EA��ƷNG����_%s",m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(Output.tFinalResult.hv_SurfaceErrorIndex[0].I()));
							}else
							{
								strName.Format("�´�·覴�����%d��EA��ƷNG����",m_tParam.nNGNumToStopMachine);
							}

							hv_SendTuple.Append((HTuple)strName);
							try
							{
								SendTuple(g_CUpLoad.m_ComSocket,hv_SendTuple);
							}catch(HException& e)
							{
								g_CUpLoad.m_bComSocket=FALSE;
							}
						}
						else//�ϴ�·
						{
							if (Output.hvPackageType.I()==0)//0�� ǰ���
							{
								nCameraSN=2;
							}else//1�������
							{
								nCameraSN=3;
							}
							hv_SendTuple.Append(nCameraSN);


							CString strName;
							if (Output.tFinalResult.hv_SizeErrorIndex.Length()>0)
							{
								strName.Format("�ϴ�·覴�����%d��EA��ƷNG����_%s",m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(Output.tFinalResult.hv_SizeErrorIndex[0].I()));

							}else if (Output.tFinalResult.hv_SurfaceErrorIndex.Length()>0)
							{
								strName.Format("�ϴ�·覴�����%d��EA��ƷNG����_%s",m_tParam.nNGNumToStopMachine,g_Count.strAClassName.GetAt(Output.tFinalResult.hv_SurfaceErrorIndex[0].I()));
							}else
							{
								strName.Format("�ϴ�·覴�����%d��EA��ƷNG����",m_tParam.nNGNumToStopMachine);
							}

							hv_SendTuple.Append((HTuple)strName);

							try
							{
								SendTuple(g_CUpLoad.m_ComSocket,hv_SendTuple);
							}catch(HException& e)
							{
								g_CUpLoad.m_bComSocket=FALSE;
							}
						}
					}

				





					S_nContinueNum[Output.hvPackageType.I()]=0;
				}
			}else
			{
				S_nContinueNum[Output.hvPackageType.I()]=0;
			}


		}
	}catch(HException& e)
	{

	}
}

void CUpload::CalculateCPK(int& nTrip,double& dRealValue,double dStandarValueValue,double dStandarDeviationValue,HTuple* hv_MeanValue,HTuple* hv_DeviationValue,HTuple* hv_CpkValue)
{

	try
	{
		static HTuple S_hvCPKData[2];
		HTuple hv_TempIndex,hv_EffectiveIndex,hv_EffectiveData;
		HTuple hv_Mean,hv_Deviation;
		if (dRealValue<dStandarValueValue+dStandarDeviationValue && dRealValue>dStandarValueValue-dStandarDeviationValue)
		{
			S_hvCPKData[nTrip].Append(dRealValue);
		}

		if (S_hvCPKData[nTrip].Length()>100)
		{
			TupleSortIndex(S_hvCPKData[nTrip],&hv_TempIndex);
			TupleSelectRange(hv_TempIndex,2,S_hvCPKData[nTrip].Length()-2,&hv_EffectiveIndex);
			TupleSelect(S_hvCPKData[nTrip],hv_EffectiveIndex,&hv_EffectiveData);
			TupleMean(hv_EffectiveData,&hv_Mean);
			TupleDeviation(hv_EffectiveData,&hv_Deviation);
			TupleMean(hv_EffectiveData,&(*hv_MeanValue));
			double dUp=g_Jugde[nTrip].m_tStandardSize.dLocateMissing+g_Jugde[nTrip].m_tStandardSize.dLocateMissingDev;
			double dDown=g_Jugde[nTrip].m_tStandardSize.dLocateMissing-g_Jugde[nTrip].m_tStandardSize.dLocateMissingDev;
			*hv_CpkValue=MIN(dUp-hv_Mean.D(),hv_Mean.D()-dDown)/(6*MAX(0.001,hv_Deviation.D()));
			TupleLastN(S_hvCPKData[nTrip],1,&S_hvCPKData[nTrip]);
			*hv_DeviationValue=hv_Deviation;

		}else
		{
			*hv_DeviationValue=0;
			*hv_CpkValue=0;
			* hv_MeanValue=0;
		}

	}catch(HException& e)
	{



	}



}

void CUpload::SendClosedData(C_ALLOCATORWORKSHOPOUTPUT &Output)//���ͱջ�����
{
	int nSide;
	nSide=Output.hvPackageType.I();

	static double SdDeviation[2]={0};
	static double SdMean[2]={0};
	static double SdCPK[2]={0};


	if (nSide==0)
	{
	    g_LFirstMean=Output.hvDislocation.D();
		g_LData=Output.B[m_tParam.nBackDataSourceIndex].tDetectOutput.hv_LocateMissing.D();
		if (g_Jugde[nSide].m_nABCuoWeiType==MeanDataBack)
		{
			g_LData=(Output.B[0].tDetectOutput.hv_LocateMissing.D()+Output.B[0].tDetectOutput.hv_LocateMissing.D())/2;
		}
		HTuple hvMean,hv_Deviation,hvCpk;
		CalculateCPK(nSide,g_LData,g_Jugde[nSide].m_tStandardSize.dLocateMissing.D(),g_Jugde[nSide].m_tStandardSize.dLocateMissingDev.D(),&hvMean,&hv_Deviation,&hvCpk);
		SdMean[nSide]=hvMean;
		SdDeviation[nSide]=hv_Deviation;
		SdCPK[nSide]=hvCpk;

	}else
	{
	    g_RFirstMean=Output.hvDislocation.D();
		g_RData=Output.B[m_tParam.nBackDataSourceIndex].tDetectOutput.hv_LocateMissing.D();
		if (g_Jugde[nSide].m_nABCuoWeiType==MeanDataBack)
		{
			g_RData=(Output.B[0].tDetectOutput.hv_LocateMissing.D()+Output.B[1].tDetectOutput.hv_LocateMissing.D())/2;
		}
		HTuple hvMean,hv_Deviation,hvCpk;
		CalculateCPK(nSide,g_RData,g_Jugde[nSide].m_tStandardSize.dLocateMissing.D(),g_Jugde[nSide].m_tStandardSize.dLocateMissingDev.D(),&hvMean,&hv_Deviation,&hvCpk);
		SdMean[nSide]=hvMean;
		SdDeviation[nSide]=hv_Deviation;
		SdCPK[nSide]=hvCpk;
	}
	

	////֧�ֵ���ջ�
	if (((nSide == 0)||(nSide == 1 &&g_ProductParam.nFoilInSide==RightSideProduct)) &&g_CUpLoad.m_bOffsetServerConnect)
	{
		////ͼ�������ȥ�Ҳ� Ҳ���ǿ�������һ���ȥ�Ǵ���
		if (fabs(g_LData)<m_tParam.dMaxDataLimit && fabs(g_RData)<m_tParam.dMaxDataLimit)
		{

			HTuple  hv_Width,hv_Height;
			GetImageSize(Output.B[0].A.hoGrabImage,&hv_Width,&hv_Height);
			HTuple hv_SendDataInfo;//����"UpOffset"  ����г���� �Ҳ��г���� �ܿ��  ��������� �Ҳ�������
			HTuple hv_LeftColumn, hv_RightColumn, hv_MidColumn;
			hv_SendDataInfo.Clear();
			SmallestRectangle1(Output.B[0].tDetectOutput.ho_Coat, NULL, &hv_LeftColumn, NULL, &hv_MidColumn);
			hv_RightColumn = hv_MidColumn * 2 - hv_LeftColumn;
			//13+2����Meanֵ
			hv_SendDataInfo.Append((HTuple)g_CUpLoad.m_StrGongWeiName);//�Ϲ�λ��UpOffset �¹�λ��DownOffset
	    	hv_SendDataInfo.Append(g_LData);
			hv_SendDataInfo.Append(g_RData);
			hv_SendDataInfo.Append(Output.B[0].tDetectOutput.hv_CoatWidth);
			hv_SendDataInfo.Append(hv_LeftColumn*g_dScaleFactor);
			hv_SendDataInfo.Append(hv_RightColumn*g_dScaleFactor);
			hv_SendDataInfo.Append(SdMean[0]);//0Mean
			hv_SendDataInfo.Append(SdDeviation[0]);//0Deviation
			hv_SendDataInfo.Append(SdCPK[0]);//0CPK
			hv_SendDataInfo.Append(SdMean[1]);//1Mean
			hv_SendDataInfo.Append(SdDeviation[1]);//1Deviation
			hv_SendDataInfo.Append(SdCPK[1]);//1CPK
			hv_SendDataInfo.Append(hv_Height*g_vec_RowResolution[0]);//����ͼ��߶�mm
			hv_SendDataInfo.Append(g_LFirstMean);//��������׼��Meanֵ
			hv_SendDataInfo.Append(g_RFirstMean);//�����Ҳ��׼��Meanֵ


		    
			try
			{
			  SendTuple(g_CUpLoad.m_hvOffsetSocket, hv_SendDataInfo);
			}catch(HException& e)
			{
				g_CUpLoad.m_bOffsetServerConnect=FALSE;
			}

		}
	}
}

void CUpload::GetMarkWarn()
{

	CString StrWarnMessage;
	HTuple hv_MarkDistanceFlag,hv_LableDistFlag;
	g_pCommunicate->DoSomething("GETPARAM","MarkDistanceStatus",&hv_MarkDistanceFlag);
	int nSide=0;
	if (hv_MarkDistanceFlag.Length()==4 && (hv_MarkDistanceFlag[0]==0 || hv_MarkDistanceFlag[1]==0))//����NG����
	{

		if (hv_MarkDistanceFlag[0]==0)
		{
			nSide=0;
			StrWarnMessage.Format("��λһ������Mark�źż�౨��ͣ��,Mark���%.1fmm",hv_MarkDistanceFlag[2].D());
			SendExceptionWarn(nSide,StrWarnMessage);

		}
		if (hv_MarkDistanceFlag[1]==0)
		{
			nSide=1;
			StrWarnMessage.Format("��λ��������Mark�źż�౨��ͣ��,Mark���%.1fmm",hv_MarkDistanceFlag[3].D());
			SendExceptionWarn(nSide,StrWarnMessage);
		}
		g_pCommunicate->DoSomething("SETPARAM","ResetMarkDistanceFlags",NULL);

	}


	g_pCommunicate->DoSomething("GETPARAM","LabelDistanceStatus",&hv_LableDistFlag);
	if (hv_LableDistFlag.Length()==4 && (hv_LableDistFlag[0]==0 || hv_LableDistFlag[1]==0))//����NG����
	{

		if (hv_LableDistFlag[0]==0)
		{
			nSide=0;
			StrWarnMessage.Format("��λһʵʱ��ص�����λ�ñ���ͣ��,����λ��%.1fmm",hv_LableDistFlag[2].D());
			SendExceptionWarn(nSide,StrWarnMessage);
		}
		if (hv_LableDistFlag[1]==0)
		{
			nSide=1;
			StrWarnMessage.Format("��λ��ʵʱ��ص�����λ�ñ���ͣ��,����λ��%.1fmm",hv_LableDistFlag[3].D());
			SendExceptionWarn(nSide,StrWarnMessage);
		}

		g_pCommunicate->DoSomething("SETPARAM","ResetLabelDistanceFlags",NULL);
	}

}

void CUpload::SendAT9CuoWeiWarn(CString& strException)
{
	if (g_CUpLoad.m_bComSocket)
	{
		HTuple hv_SendTuple;
		hv_SendTuple.Append("AT9alarm");
		if (g_CUpLoad.m_StrGongWeiName=="DownOffset")//�´�·
		{
			hv_SendTuple.Append(1);
		}else
		{
			hv_SendTuple.Append(0);
		}
		hv_SendTuple.Append((HTuple)strException);
	}

}


void CUpload::SendExceptionWarn(int& nGrabberSN,CString strException)
{
	if (g_CUpLoad.m_bComSocket)
	{

		HTuple hv_SendTuple;
		int nCameraSN=0;
		hv_SendTuple.Append(120);
		if (g_CUpLoad.m_StrGongWeiName=="DownOffset")//�´�·
		{
			if (nGrabberSN==0)//0�� ǰ���
			{
				nCameraSN=4;

			}else//1�������
			{
				nCameraSN=5;
			}
			hv_SendTuple.Append(nCameraSN);

			CString strExceptinValue;
			strExceptinValue.Format("�´�·%s",strException);
			hv_SendTuple.Append((HTuple)strExceptinValue);

		}else//�ϴ�·
		{
			if (nGrabberSN==0)//0�� ǰ���
			{
				nCameraSN=2;
			}else//1�������
			{
				nCameraSN=3;
			}
			hv_SendTuple.Append(nCameraSN);
			CString strExceptinValue;
			strExceptinValue.Format("�ϴ�·%s",strException);
			hv_SendTuple.Append((HTuple)strExceptinValue);
		}
		try
		{
			SendTuple(g_CUpLoad.m_ComSocket,hv_SendTuple);
		}catch(HException& e)
		{
			g_CUpLoad.m_bComSocket=FALSE;
		}
	}
}




void CUpload::SendTransferStaus(C_ALLOCATORWORKSHOPOUTPUT& Output)//����������Ϳ�����ɱ�־
{
	BOOL bFlag=FALSE;
	g_pStorage->DoSomething("GETPARAM","CopyStaus",&bFlag);
	if (bFlag&&m_bComSocket)
	{

		try
		{
			CString strInfo;
			HTuple hv_TupleSend;
			hv_TupleSend=4;
			SendTuple(m_ComSocket,hv_TupleSend);
			strInfo.Format("����������Ϳ�����ɱ�־");
			g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
		}catch(HException& e)
		{

		}
	}





}
void CUpload::T_Upload(HTuple hvQueue)
{
	HTuple hv_MessageRecv, hvMessageNum, hv_StopProcInfo,hv_TimeStart, hv_TimeEnd,hv_ErrorFlag;
	while (!FALSE)
	{
		DequeueMessage(hvQueue, "timeout", "infinite", &hv_MessageRecv);
		C_ALLOCATORWORKSHOPOUTPUT tOutput;

		tOutput.ReadMessage(hv_MessageRecv);
		ClearMessage(hv_MessageRecv);
	    CountSeconds(&hv_TimeStart);

		GetMarkWarn();//��ȡ����λ�ú�Mark�źű�����Ϣ
		SetWarnToServer(tOutput);//����NG��Ʒ����
		SendClosedData(tOutput);///���ͱջ�����
		SendTransferStaus(tOutput);
		/***************************�ϴ�����********************************/
			
		HTuple  hv_MQSendInfo;
		CString strComputerName;
		GetGongWeiName(tOutput,strComputerName);//��ȡ��λ��
		if (tOutput.B[0].tDetectOutput.hv_ErrorType.Length()>0 || tOutput.B[1].tDetectOutput.hv_ErrorType.Length()>0 )
		{
			hv_MQSendInfo.Append("MQ");
		}
		
			
		for (int i = 0; i < CAMERA_COUNT; i++)
		{
				if (FALSE == m_bSocket[i])
				{
					continue;
				}
				////���Ͳɼ���ԭͼ
				
				SendOriginalImage(tOutput,i);
				
				////����Сͼ��������Ϣ
				HTuple  hv_width,hv_height;
				HObject hoImagePart,hoSelect;
				HTuple  hv_SingleRow,hv_SingleColumn,hv_SingleArea;
				HTuple bExists;
				HTuple hv_SendTuple;
				CString strErrorValue;
				CString strFileName, strImageName;
				SYSTEMTIME CurSysTime;
			
				GetLocalTime(&CurSysTime);
				GetImageSize(tOutput.B[i].A.hoGrabImage,&hv_width,&hv_height);
				if (m_StrGongWeiName=="DownOffset")
				{
					if (i==0)
					{
						strFileName.Format("//%s/����/��ͼ/�´�·/����/%02d��%02d��%02d��/%s_%s", m_tParam.strAddressCom,CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,g_strProductName, g_strBatch);//�洢�ļ�Ŀ¼
					}else
					{
						strFileName.Format("//%s/����/��ͼ/�´�·/����/%02d��%02d��%02d��/%s_%s", m_tParam.strAddressCom,CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,g_strProductName, g_strBatch);//�洢�ļ�Ŀ¼
					}

				}
				else 
				{
					if (i==0)
					{
						strFileName.Format("//%s/����/��ͼ/�ϴ�·/����/%02d��%02d��%02d��/%s_%s",m_tParam.strAddressCom,CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,g_strProductName, g_strBatch);//�洢�ļ�Ŀ¼
					}else
					{
						strFileName.Format("//%s/����/��ͼ/�ϴ�·/����/%02d��%02d��%02d��/%s_%s",m_tParam.strAddressCom,CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay,g_strProductName, g_strBatch);//�洢�ļ�Ŀ¼
					}

				}
			
				FileExists(HTuple(strFileName), &bExists);
				if (0 == bExists)
				{
				CreateMultilevelPath(strFileName);
				}
				
			
			for (int k=0;k<MIN(1,tOutput.B[i].tDetectOutput.hv_ErrorType.Length());k++)
				{

					
					hv_SendTuple.Clear();//����ÿ��Сͼ�Ĵ洢λ��
					SelectObj(tOutput.B[i].tDetectOutput.ho_ErrorRegion,&hoSelect,k+1);
					AreaCenter(hoSelect,&hv_SingleArea,&hv_SingleRow,&hv_SingleColumn);
					strErrorValue.Format("%s-%.2f-%.2f-%.2f", g_Count.strAClassName.GetAt(tOutput.B[i].tDetectOutput.hv_ErrorType[k].I()),hv_SingleArea.D()*g_vec_RowResolution[0] * g_vec_RowResolution[0], hv_SingleRow.D(), hv_SingleColumn.D());
					strImageName.Format("%s/%02dʱ-%02d��-%02d��-%02d����-%s", strFileName, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds,strErrorValue );//ͼ������
					
					try
					{
						CropRectangle1(tOutput.B[i].A.hoGrabImage,&hoImagePart,MAX(1,hv_SingleRow.D()-100),MAX(1,hv_SingleColumn.D()-100),MIN(hv_height.I()-1,hv_SingleRow.D()+100),MIN(hv_width.I()-1,hv_SingleColumn.D()+100));
						WriteImage(hoImagePart, "jpeg", 0, (HTuple)strImageName);
					}catch(HException& e)
					{

					}
					/////����Сͼ�Ĵ洢λ��
					hv_SendTuple.Append(99);//99�ǽ�����־
					hv_SendTuple.Append((HTuple)strImageName);
					try
					{
						SendTuple(m_Socket[i],hv_SendTuple);

					}
					catch (HException& e)
					{
						CString strLog;
						strLog.Format("����СͼSendTupleʧ�� ����%s:%s",e.ProcName().Text(),e.ErrorMessage().Text());
						g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
						m_bSocket[i]=FALSE;
						
					}
					
					////���ʹ�����Ϣ
					CString strTime,strValue;
					strTime.Format("%02dʱ-%02d��-%02d��-%02d����",CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
					hv_MQSendInfo.Append(tOutput.hvPackageType.I());//EAλ�ñ�־ 0����� 1���Ҳ�
					hv_MQSendInfo.Append((HTuple)strComputerName);//��λ��
					hv_MQSendInfo.Append(0);//����λ�� ��ʱ��Ϊ0
					hv_MQSendInfo.Append(tOutput.hvGroupID);//EA���
					hv_MQSendInfo.Append((HTuple)g_Count.strAClassName.GetAt(tOutput.B[i].tDetectOutput.hv_ErrorType[k].I()));//ȱ������
					strValue.Format("%.2f",hv_SingleArea.D()*g_vec_RowResolution[0] * g_vec_RowResolution[0]);
					hv_MQSendInfo.Append(atof(strValue));//ȱ�����
					strValue.Format("%.2f",hv_SingleColumn.D());
					hv_MQSendInfo.Append(atof(strValue));//ȱ�ݺ�����
					strValue.Format("%.2f",hv_SingleRow.D());
					hv_MQSendInfo.Append(atof(strValue));//ȱ��������
					hv_MQSendInfo.Append((HTuple)strTime);//ʱ��
					hv_MQSendInfo.Append((HTuple)strImageName);
					hv_MQSendInfo.Append("MQ");
				}
		}
		////ÿ��EA������ʱ����ͳ����Ϣ
		HTuple hv_CountSendInfo,hv_ErrorTypeSendInfo,hv_ErrorNumSendInfo;
		if (tOutput.bIsFinalResult==TRUE)
		{
			hv_CountSendInfo.Append("ResultInfo");
			hv_CountSendInfo.Append(tOutput.hvPackageType.I());//λ�ñ�־ 0���ڲ� 1�����
			hv_CountSendInfo.Append(tOutput.tFinalResult.hv_ProductID);//EA���
			hv_CountSendInfo.Append((HTuple)strComputerName);//��λ����
			hv_CountSendInfo.Append(tOutput.tFinalResult.hv_SizeFlag);//�ߴ��־
			hv_CountSendInfo.Append(tOutput.tFinalResult.hv_SurfaceFlag);//ȱ�ݱ�־
			hv_CountSendInfo.Append(g_Count.veCount[0][tOutput.hvPackageType.I()].nTotal);//��ǰ��λ������
			hv_CountSendInfo.Append(g_Count.veCount[0][tOutput.hvPackageType.I()].nError);//��ǰ��λNG������
			hv_CountSendInfo.Append("ResultInfo");
			hv_ErrorTypeSendInfo.Append("ErrorType");
			for (int k=0;k<g_Count.nType;k++)
			{
				hv_ErrorTypeSendInfo.Append((HTuple)g_Count.strAClassName.GetAt(k));
			}
			hv_ErrorTypeSendInfo.Append("ErrorType");
			hv_ErrorNumSendInfo.Append("ErrorNum");
			for (int j=0;j<g_Count.nType;j++)
			{
				hv_ErrorNumSendInfo.Append(g_Count.veCount[0].at(tOutput.hvPackageType.I()).nClassError[j]);
			}
			hv_ErrorNumSendInfo.Append("ErrorNum");

		}
		hv_MQSendInfo.Append(hv_CountSendInfo);
		hv_MQSendInfo.Append(hv_ErrorTypeSendInfo);
		hv_MQSendInfo.Append(hv_ErrorNumSendInfo);
		try
		{



			CountSeconds(&hv_TimeEnd);//������ʱ
	      if (m_bSocket[0] && hv_MQSendInfo.Length()>0)
			{
				SendTuple(m_Socket[0],hv_MQSendInfo);//���õͶ˿ڷ��� A���� 4002  B����4004
				/*HTuple FileHandle;
				SYSTEMTIME CurSysTime;
				GetLocalTime(&CurSysTime);
				CString strFilePath, strFileDirector;
				HTuple bExists;
				strFileDirector.Format("%s\\����������\\", GetCurrentAppPath());
				FileExists(HTuple(strFileDirector), &bExists);
				if (0 == bExists)
				{
					CreateMultilevelPath(strFileDirector);
				}
				strFilePath.Format("%s\\��Ϣ��¼.txt", strFileDirector);
				OpenFile((HTuple)strFilePath, "append", &FileHandle);
				FnewLine(FileHandle);
				FwriteString(FileHandle, hv_MQSendInfo);
				CloseFile(FileHandle);
*/



			}
		}
		catch (HException& e)
		{

			CString strLog;
			strLog.Format("SendTupleʧ�� ����%s:%s",e.ProcName().Text(),e.ErrorMessage().Text());
			g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strLog,NULL);
			m_bSocket[0]=FALSE;
			
			
		}
		


	}
}
