#include "StdAfx.h"
#include <iostream>  
#include <fstream>  
#include "Labeller.h"
#include "DlgLabelInfoDisp.h"


#pragma  once
using namespace std;

extern "C" __declspec(dllexport) DHInterface* Export(void)
{
	return (DHInterface*)new CLabeller();
}
CLabeller* g_This = NULL;

BOOL g_bExitThread=FALSE;

 const int  Const_MaxLableNum = 10;


void ThreadListenLabelLength(PVOID * pParam)
{

	while(g_bExitThread==FALSE)
	{
		//double m_dStandardLocation; 
		//HTuple m_hvLabelLocationOffsetTuple;
		//double m_dLabelLocationOffsetDev;
		if (g_This->m_CFinsTcp.m_bConnected)
		{

		  int nDmLengthAddress = 1352;
		  int nDmLengthFlag = 1356;
		  float dLengthReadFlag = 0;
		  float dLength = 0;
		  float dResetValue = 0;
		  g_This->m_CFinsTcp.ReadFloat32(nDmLengthFlag, dLengthReadFlag);
		  /////��¼PLC������λ��ƫ��
		  if (dLengthReadFlag==1)
		  {
			  g_This->m_CFinsTcp.ReadFloat32(nDmLengthAddress, dLength);
			  g_This->m_CFinsTcp.WriteFloat32(nDmLengthFlag, dResetValue);

			  SYSTEMTIME CurSysTime;
			  GetLocalTime(&CurSysTime);
			  CString strSizeRecord, strSizePath;
			  strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_This->m_tInit.strProductName, g_This->m_strBatch);
			  strSizeRecord.Format("%s\\PLC�յ���׼�źż�¼-%s.csv.", strSizePath, g_This->m_strBatch);

			  if (!PathFileExists(strSizeRecord))
			  {
				  CreateMultilevelPath(strSizePath);
				  CString strItem;
				  strItem.Format("ʱ��,PLC�յ���׼�źż��(1352)");
				  HTuple hv_FileHandle;
				  ofstream outFile;
				  outFile.open(strSizeRecord, ios::out);
				  outFile << strItem << endl;
				  outFile.close();
			  }
			  ofstream outFile;
			  CString strTime,strItem;
			  strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);

			  outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
			  strItem.Format("%s,%.1f", strTime, dLength);
			  outFile << strItem << endl;
			  outFile.close();


		  }


		  /////��¼������ƫ��///////
		  int UpDMFlag = 10008;//�ϴ�·1-6��־λ��ַ 1��ʱ���ȡ����λ�� 0��ʱ�򲻶�ȡ
		  int UpDMStart = 10010;//�ϴ�·1-6�������ʼ��ַ �����1����λ�õĴ�ŵ�ַ10010
		  float  UpReadFlag = 0;
		  int DownDMFlag = 10078;//�´�·7-12��־λ��ַ 1��ʱ���ȡ����λ�� 0��ʱ�򲻶�ȡ
		  int DownDMStart = 10080;//�ϴ�·7-12�������ʼ��ַ �����1����λ�õĴ�ŵ�ַ10010
		  float  DownReadFlag = 0;///�´�·PLCƫ�ƶ�ȡ��־
		//  g_This->m_CFinsTcp.WriteFloat32(UpDMFlag, UpReadFlag);
		  g_This->m_CFinsTcp.ReadFloat32(UpDMFlag, UpReadFlag);
		  g_This->m_CFinsTcp.ReadFloat32(DownDMFlag, DownReadFlag);

		  std::vector<float> UpTempVector;
		  std::vector<float> DownTempVector;
		  UpTempVector.clear();
		  DownTempVector.clear();
		 CString strUpLocationTotalName, strUpSingleLocationName;
		 CString strDownLocationTotalName, strDownSingleLocationName;
		 BOOL bWriteFlag = FALSE;
		 double dRealLoction;
		if (UpReadFlag ==1)
		   {
			CString strException;
			strException.Format("������ϴ�·����10008��ַ==1");
			((DHInterface*)(g_This->m_tInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strException, NULL);
			//

		    	bWriteFlag = TRUE;
                g_This->m_CFinsTcp.ReadFloat32Batch(UpDMStart, Const_MaxLableNum/2, UpTempVector);///�ϴ�·ȡ5��
				
				//��λ
				
				
				for (int i=0;i<UpTempVector.size();i++)
				{
					   dRealLoction = UpTempVector[i] + g_This->m_hvLabelLocationOffsetTuple[i];
					double dLoateValue= dRealLoction - g_This->m_dStandardLocation;;
					g_This->m_hvLabelRealLocation[i]= UpTempVector[i];
					if (fabs(dLoateValue)>g_This->m_dLabelLocationOffsetDev && dRealLoction != g_This->m_hvLabelLocationOffsetTuple[i])
					{
						CString strException;
						strException.Format("�ϴ�·�����%d ԭʼ����ƫ��%.1f ����:%.1f ��������λ��:%.1f ��������λ��:%.1f,��������ֵ%.1fmm,��ȷ��",i+1, UpTempVector[i],g_This->m_hvLabelLocationOffsetTuple[i].D(), dRealLoction, g_This->m_dStandardLocation,g_This->m_dLabelLocationOffsetDev);
						((DHInterface*)(g_This->m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
					}
				}

					for (int i = 0; i <UpTempVector.size(); i++)
					{
						double dLoateValue = UpTempVector[i] + g_This->m_hvLabelLocationOffsetTuple[i];
						strUpSingleLocationName.Format("%.1f,", dLoateValue);
						strUpLocationTotalName += strUpSingleLocationName;
					}




		}
		else
		{
			for (int i = 0; i <5; i++)
			{

				strUpSingleLocationName.Format("--,");
				strUpLocationTotalName += strUpSingleLocationName;
			}


		}
		if (DownReadFlag==1)
		{
			bWriteFlag = TRUE;
			g_This->m_CFinsTcp.ReadFloat32Batch(DownDMStart, Const_MaxLableNum/2, DownTempVector);
			CString strException;
			strException.Format("������´�·����10078��ַ==1");
			((DHInterface*)(g_This->m_tInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strException, NULL);

			for (int i = 0; i<DownTempVector.size(); i++)
			{
				int nIndex = i + Const_MaxLableNum/2;//Ҫ���´�·�ĵ�һ����ֵ����6��
				dRealLoction = DownTempVector[i] + g_This->m_hvLabelLocationOffsetTuple[nIndex];
				double dLoateValue = dRealLoction - g_This->m_dStandardLocation;
				g_This->m_hvLabelRealLocation[nIndex] = DownTempVector[i];
				if (fabs(dLoateValue)>g_This->m_dLabelLocationOffsetDev && dRealLoction != g_This->m_hvLabelLocationOffsetTuple[nIndex])
				{
					CString strException;
					strException.Format("�´�·�����%d ԭʼ����ƫ��%.1f ����:%.1f ��������λ��:%.1f  ��������λ��:%.1f,��������ֵ%.1fmm,��ȷ��", nIndex + 1, DownTempVector[i], g_This->m_hvLabelLocationOffsetTuple[nIndex].D(), dRealLoction, g_This->m_dStandardLocation, g_This->m_dLabelLocationOffsetDev);
					((DHInterface*)(g_This->m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
				}
			}

			for (int i = 0; i <DownTempVector.size(); i++)
			{
				int nIndex = i + Const_MaxLableNum/2;//Ҫ���´�·�ĵ�һ����ֵ����6��
				
				double dLoateValue = DownTempVector[i] + g_This->m_hvLabelLocationOffsetTuple[nIndex];
				strDownSingleLocationName.Format("%.1f,", dLoateValue);
				strDownLocationTotalName += strDownSingleLocationName;
			}
		}
		else
		{
			for (int i = 0; i <Const_MaxLableNum/2; i++)
			{
		
				strDownSingleLocationName.Format("--,");
				strDownLocationTotalName += strDownSingleLocationName;
			}


		}

		/////��¼ƫ��λ��/////////////////////////////////////


		if (bWriteFlag)
		{
			float nUpDMFlagLog=0;
			g_This->m_CFinsTcp.ReadFloat32(UpDMFlag, nUpDMFlagLog);
			CString strException;
			strException.Format("����д��¼����10008��ַ==%.1f", nUpDMFlagLog);
			((DHInterface*)(g_This->m_tInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strException, NULL);

			int nFlag = 1;
			g_This->m_CFinsTcp.WriteFloat32(UpDMFlag, nFlag);
			Sleep(50);
			g_This->m_CFinsTcp.WriteFloat32(UpDMFlag, dResetValue);
			SYSTEMTIME CurSysTime;
			GetLocalTime(&CurSysTime);
			CString strSizeRecord, strSizePath;
			strSizePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, g_This->m_tInit.strProductName, g_This->m_strBatch);
			strSizeRecord.Format("%s\\ʵʱ��������¼-%s.csv.", strSizePath, g_This->m_strBatch);
			if (!PathFileExists(strSizeRecord))
			{
				CreateMultilevelPath(strSizePath);
				CString strItem;
				CString strLocationTotalName, strSingleLocationName;
				for (int i = 0; i <Const_MaxLableNum; i++)
				{
					strSingleLocationName.Format("��%d�����λ��,", i + 1);
					strLocationTotalName += strSingleLocationName;

				}
				strItem.Format("ʱ��,%s,�ٶ�(m/min)", strLocationTotalName);
				HTuple hv_FileHandle;
				ofstream outFile;
				outFile.open(strSizeRecord, ios::out);
				outFile << strItem << endl;
				outFile.close();
			}


			CString strTime, strItem;
			strTime.Format("%02d%02d%02d% 02d:%02d:%02d %03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
			strItem.Format("%s,%s%s,%.1f", strTime, strUpLocationTotalName, strDownLocationTotalName, g_This->m_dRealSpeed);
			HTuple hv_FileHandle;
			ofstream outFile;
			outFile.open(strSizeRecord, ios::ate | ios::out | ios::app);
			outFile << strItem << endl;
			outFile.close();

		}
		}







		Sleep(50);






	}




}



CLabeller::CLabeller(void)
{
	InitializeCriticalSection(&m_csChange);
	m_StrLabellerStatus="PLC�ѶϿ�";

	m_hvStatusFlag.Clear();
	m_hvWarnNames.Clear();
	m_hvCount.Clear();
	m_hvCountNames.Clear();

	m_hvControl.Clear();
	m_hvControlName.Clear();

	m_hvSystemParam.Clear();
	m_hvSystemParamName.Clear();

	m_bWarnStaus=FALSE;
	m_dRealSpeed = 0;
	TupleGenConst(12,0,&m_hvLabelRealLocation);
}


CLabeller::~CLabeller(void)
{
	DeleteCriticalSection(&m_csChange);
}


void   CLabeller::XiangJiChuChen()//�������
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nDM=132;
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDM,dValue);

	}
}
void   CLabeller::BaoJingFuWei()//������λ
{

	LabelReset();
	m_bWarnStaus=FALSE;
}
void   CLabeller::ShangQiGangTaiQi()//�ϴ�·����̧��
{
	if (m_CFinsTcp.m_bConnected)
	{

		int nDMSQGTQ=120;//
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDMSQGTQ,dValue);


	}
}
void   CLabeller::ShangQiGangYaXia()//�ϴ�·����ѹ��
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nDMSQGYX=122;//
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDMSQGYX,dValue);

	}
}
void   CLabeller::XiaQiGangTaiQi()//��·����̧��
{
	if (m_CFinsTcp.m_bConnected)
	{

		int nDMXQGYX=124;//
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDMXQGYX,dValue);


	}
}
void   CLabeller::XiaQiGangYaXia()//�´�·����ѹ��
{
	if (m_CFinsTcp.m_bConnected)
	{

		int nDMXQGYX=126;//
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDMXQGYX,dValue);


	}
}
void   CLabeller::BianMaQiGangTaiQi()//��������������̧��
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nDM=128;
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDM,dValue);
	
	}
}
void   CLabeller::BianMaQiGangYaXia()//������������ѹ��
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nDM=130;
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDM,dValue);

	}
}





void CLabeller::SetLabellerStaus(CString& strInfo)
{
	{
		CriticalSectionHelper  TempLock(&m_csChange);
		m_StrLabellerStatus=strInfo;
	}
}
BOOL  CLabeller::IsUseLabel(int nLabelIndex)
{

	if (nLabelIndex==1)
	{
		if (m_tLabellerInfo.nEnableLabelName==0 ||m_tLabellerInfo.nEnableLabelName==1)
			return TRUE;
		else 
			return FALSE;
	}else if (nLabelIndex==2)
	{
		if (m_tLabellerInfo.nEnableLabelName==0 ||m_tLabellerInfo.nEnableLabelName==2)
			return TRUE;
		else 
			return FALSE;
	}



}



void CLabeller::Loop()
{

	CString strException;
	while (g_bExitThread==FALSE)
	{

		    static int s_nCount=0;
			if (m_CFinsTcp.m_bConnected==FALSE)
			{
				
				if (TRUE==m_CFinsTcp.Connect(m_tLabellerInfo.strLabelIpAddress,m_tLabellerInfo.nLabelPort,CT_CLIENT))
				{
					
					
						strException.Format("PLC������");
						WriteLabelLog(strException,FALSE);
						SetLabellerStaus(strException);
					
				}else
				{
					if (s_nCount++>10)
					{
						s_nCount=0;
						strException.Format("PLC�ѶϿ�");
						WriteLabelLog(strException);
						SetLabellerStaus(strException);
					}
				}
			}else
			{
				s_nCount=0;
				strException.Format("�����������",FALSE);
			}
			////д����
			{
				int nDMHeart=100;
				double dValue=1;
				m_CFinsTcp.WriteFloat32(nDMHeart,dValue);

			}
			HTuple hvStatusFlag,hvWarnNames;
            GetLabelStatus(&hvStatusFlag,&hvWarnNames);
			WriteExceptLog(hvStatusFlag,hvWarnNames);
			if (m_hvStatusFlag.Length()==0 && m_hvWarnNames.Length()==0)
			{
				m_hvStatusFlag=hvStatusFlag;
				m_hvWarnNames=hvWarnNames;
			}

			////��ȡϵͳ����
			HTuple hvName,hvValue;
			GetLabelParam(hvName,hvValue);
			if (m_hvSystemParam.Length()==0 && m_hvSystemParamName.Length()==0)
			{
				m_hvSystemParam=hvValue;
				m_hvSystemParamName=hvName;
			}

			
			/////�ж��ٶ�
			if (m_dRealSpeed == 0)
			{
				///�ٶ�С��0 �Ƶ�
				SetLightColor(YellowLight);
			}
			else
			{
				///�ٶȴ���0 �̵�
				SetLightColor(GreenLight);
			}

			
		
		



			Sleep(500);
	}
}
void CLabeller::WriteExceptLog(HTuple& hvFlag,HTuple& hv_Names)
{

	for (int k=0;k<hv_Names.Length();k++)
	{

		if (hvFlag[k]==1)
		{

			WriteLabelLog(hv_Names[k].S().Text());
		}

	}




}

void CLabeller::SetLightColor(emLightColor LightColor)
{
	int nDM = 108;
	float dValue = 0;

	if (m_CFinsTcp.m_bConnected)
	{
	switch (LightColor)
	{
	case GreenLight:
		/////�Ƶ���
		 nDM = 108;
		 dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//�����
		nDM = 110;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//��ɫ����
		nDM = 106;
		dValue = 1;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//��������
		nDM = 112;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		break;


	case YellowLight:
		///�Ƶ���
		 nDM = 108;
		 dValue = 1;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//�����
		nDM = 110;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//��ɫ����
		nDM = 106;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//��������
		nDM = 112;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		break;

	case RedLight:
		///�Ƶ���
		 nDM = 108;
		 dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//�����
		nDM = 110;
		dValue = 1;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		////��ɫ����
		nDM = 106;
		dValue = 0;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		//��������
		nDM = 112;
		dValue = 1;
		m_CFinsTcp.WriteFloat32(nDM, dValue);

		break;



	}

	
		

	}




}

int CLabeller::DoSomething(void* message,void* wParam,void* lParam)
{
	CString strMessage;
	strMessage.Format("%s",(char*)message);
	if ("INITIALIZE" == strMessage)
	{
		return Init((PLABELLERINIT)wParam);
	}else if ("GETPARAM" == strMessage)
	{
		CString strParam;
		strParam.Format("%s",(char*)wParam);
		if ("StrLabellerStatus" == strParam)
		{
			_tcscpy((char*)lParam, m_StrLabellerStatus);
			return TRUE;
		}
		else if ("PLCWarnStatus" == strParam)
		{
			*(BOOL*)lParam = m_bWarnStaus;
			return TRUE;
		}else if ("LabelRealLocation"==strParam)
		{
			*(HTuple*)lParam = m_hvLabelRealLocation;
			return TRUE;

		}
	}else if ("SHOWLABELLERDLG" == strMessage)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CDlgLabelInfoDisp dlg;
		dlg.m_pOwner = this;
		dlg.DoModal();
		return TRUE;
	}else if ("SETPARAM" == strMessage)
	{
		CString strParam;
		strParam.Format("%s",(char*)wParam);
       if ("YELLOW_LIGHT"==strParam)//�Ƶ�
		{
		   SetLightColor(YellowLight);

		}
		else if ("RED_LIGHT"==strParam)//���
		{
			SetLightColor(RedLight);
		}
		else if ("GREEN_LIGHT"==strParam)//�̵�
		{
			SetLightColor(GreenLight);
		}
		else if ("FUZZY_LIGHT"==strParam)//������
		{
			if (m_CFinsTcp.m_bConnected)
			{
				int nDM=112;
				float dValue=1;
				m_CFinsTcp.WriteFloat32(nDM,dValue);

			}
		}
		else if ("SDLQGYX"==strParam)//�ϴ�·����ѹ��
		{
			ShangQiGangYaXia();
		}else if ("SDLQGTQ"==strParam)//�ϴ�·����̧��
		{
			ShangQiGangTaiQi();
		}else if ("XDLQGYX"==strParam)//�´�·����ѹ��
		{
			XiaQiGangYaXia();
		}else if ("XDLQGTQ"==strParam)//�´�·����̧��
		{
			XiaQiGangTaiQi();
		}else if ("BMGQGYX"==strParam)//������������ѹ��
		{
			BianMaQiGangYaXia();
		}else if ("BMGQGTTQ"==strParam)//������������̧��
		{
			BianMaQiGangTaiQi();
		}else if ("BJFW"==strParam)//������λ
		{
			BaoJingFuWei();
		}else if ("XJCC"==strParam)//�������
		{
			XiangJiChuChen();
		}
		else if ("RealSpeed" == strParam)//�������
		{
			m_dRealSpeed=*(double*)lParam;

		}
		else if ("ExitThread" == strParam)//�������
		{
			g_bExitThread =TRUE;

		}else if ("StandardLabelLocation" == strParam)//�������
		{
			m_dStandardLocation=*(double*)lParam;

		}else if ("LabelLocationOffset" == strParam)//�������
		{
			m_hvLabelLocationOffsetTuple=*(HTuple*)lParam;

		}else if ("LabelOffsetDev" == strParam)//�������
		{
			m_dLabelLocationOffsetDev=*(double*)lParam;

		}else if (strParam=="BATCH")
		{
			m_strBatch.Format("%s",lParam);

		}
		return TRUE;
	}
	
	else if ("UpdateResolution" == strMessage)
	{
	
		return TRUE;
	}
	else
	{
		CString strMessageBox;
		strMessageBox.Format("SCILabellerģ���� strMessage = %s ����", (char*)message);
		AfxMessageBox(strMessageBox);
		return FALSE;
	}
}

void* CLabeller::_Thread(void *hcthread)//io�߳�
{
	HDevThread*  hcppthread = (HDevThread*) hcthread;
	try
	{
		g_This->Loop();
		hcppthread->Exit();
		delete hcppthread;
	}
	catch(HException& e)
	{
		CString strException;
		strException.Format("CLabeller�����쳣\r\n�쳣��Ϣ%s  [ %s() ]",e.ErrorMessage().Text()
			,e.ProcName().Text());
		/*g_This->m_tInit.pLogView(strException, ExcpLog,NoExcp);*/
		bool is_direct_call = hcppthread->IsDirectCall();
		hcppthread->Exit();
		delete hcppthread;
		if (is_direct_call)
			throw e;
	}
	return NULL;
}


int CLabeller::Init(PLABELLERINIT pInit)//��ʼ��
{
	SetHcppInterfaceStringEncodingIsUtf8(FALSE);
	SetSystem("filename_encoding","locale");
	SetSystem("read_halcon_files_encoding_fallback","locale");
	SetSystem("read_halcon_files_encoding_fallback","locale");
	SetSystem("write_halcon_files_encoding","locale");
	SetSystem("tsp_tuple_string_operator_mode","byte");
	SetSystem("tuple_string_operator_mode","byte");
	SetSystem("store_empty_region", "false");
	SetSystem("clip_region", "false");
	m_tInit.pSysCall = pInit->pSysCall;
	m_tInit.pLogView = pInit->pLogView;
	m_tInit.strProductName.Format("%s",pInit->strProductName);
	g_This = this;
#pragma region "�������ļ�"
	ReadParam();
	m_tInit.pSysCall("GETPARAM","StandardLabelLocation",&m_dStandardLocation);
	m_tInit.pSysCall("GETPARAM","LabelLocationOffset",&m_hvLabelLocationOffsetTuple);
	m_tInit.pSysCall("GETPARAM","LabelOffsetDev",&m_dLabelLocationOffsetDev);
#pragma endregion

#pragma region "�������к��߳�"
	//HDevThreadContext Context;
	//HDevThread* pThread = new HDevThread(Context,(void*)_Thread,0,0);//����һ������2��������߳�;
	//HTuple ProcThreads;
	//pThread->ParStart(&ProcThreads);

	//AfxBeginThread((AFX_THREADPROC)ThreadListenLabelLength, NULL);


#pragma endregion






	return TRUE;
}

void  CLabeller::GetLabelControl(HTuple& hv_Type,HTuple&hv_Data)//��ȡ����
{
	hv_Type.Clear();
	hv_Data.Clear();
	int nValue=0;


	CString strName;
	m_CFinsTcp.ReadInt16(m_tLabellerInfo.nLianJiJianCeEnableAddr,nValue);
	strName.Format("�����������(%d)",m_tLabellerInfo.nLianJiJianCeEnableAddr);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);


	int nDM=4212;
	m_CFinsTcp.ReadInt16(nDM,nValue);
	strName.Format("��������(%d)",nDM);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);


	m_CFinsTcp.ReadInt16(m_tLabellerInfo.nShouDongChuBiaoMoshi,nValue);
	strName.Format("�ֶ�����ģʽ(%d)",m_tLabellerInfo.nShouDongChuBiaoMoshi);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);



	m_CFinsTcp.ReadInt16(m_tLabellerInfo.nLeiJiaDaBiaoBaoJingEnableAddr,nValue);
	strName.Format("�ۼƴ�걨��(%d)",m_tLabellerInfo.nLeiJiaDaBiaoBaoJingEnableAddr);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);

	m_CFinsTcp.ReadInt16(m_tLabellerInfo.nLeiXuBaoJingEnableAddr,nValue);
	strName.Format("������걨��(%d)",m_tLabellerInfo.nLeiXuBaoJingEnableAddr);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);



	m_CFinsTcp.ReadInt16(m_tLabellerInfo.nSiLianBiaoEnableAddr,nValue);
	strName.Format("�����깦��(%d)",m_tLabellerInfo.nSiLianBiaoEnableAddr);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);

	nDM=4760;
	m_CFinsTcp.ReadInt16(nDM,nValue);
	strName.Format("Mark��־����(%d)",nDM);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);

	nDM=4769;
	m_CFinsTcp.ReadInt16(nDM,nValue);
	strName.Format("�ϴ���������(%d)",nDM);
	hv_Type.Append((HTuple)strName);
	hv_Data.Append(nValue);

	

}
void  CLabeller::GetLabelParam(HTuple& hv_Type,HTuple&hv_Data)//��ȡ����
{
	hv_Type.Clear();
	hv_Data.Clear();
	float  dValue=0;

	///////
	//��������ת�ٶȣ�MM/S��  20
	//	�ϴ�·�����������λ 50
	//	�ϴ�·��������½�λ 52
	//	�´�·�����������λ 54
	//	�´�·��������½�λ 56
	//	������ѹ����������λ 58
	//	������ѹ�������½�λ 60
	///Param
	int nDM=20;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("��������ת�ٶȣ�MM/S��(20)");
	hv_Data.Append(dValue);

	nDM=50;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("�ϴ�·�����������λ(50)");
	hv_Data.Append(dValue);

	nDM=52;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("�ϴ�·��������½�λ(52)");
	hv_Data.Append(dValue);

	nDM=54;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("�´�·�����������λ(54)");
	hv_Data.Append(dValue);

	nDM=56;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("�´�·��������½�λ(56)");
	hv_Data.Append(dValue);

	nDM=58;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("������ѹ����������λ(58)");
	hv_Data.Append(dValue);


	nDM=60;
	m_CFinsTcp.ReadFloat32(nDM,dValue);
	hv_Type.Append("������ѹ�������½�λ(60)");
	hv_Data.Append(dValue);


	int nControlDM = 10024;//���ƶ�ȡ�ϴ�
	std::vector<float> ControlTempVector;
	m_CFinsTcp.ReadFloat32Batch(nControlDM, 12, ControlTempVector);///���Ʊ�־λ12��
	CString strMessage;
	for (int i = 0; i < ControlTempVector.size(); i++)
	{

		strMessage.Format("�����%d�ϴ�����������(%d)", i + 1, nControlDM + i * 2);
		hv_Type.Append((HTuple)strMessage);
		hv_Data.Append(ControlTempVector[i]);


	}


}


int  CLabeller::GetLabelNum(int nLabelSN,int nType)//��ȡ����
{
	int nValue=0;
	switch(nType)
	{
	case LabelReceiveNumAddress:
		
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nLabelReceiveNumAddress,nValue);
		return nValue;
		break;

	case LabelActionNumAddress:
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nLabelActionNumAddress,nValue);
		return nValue;
		break;
	case SensorNumAddress:
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nSensorNumAddress,nValue);
		return nValue;
		break;

	case SiLianBiaoNumAddress:
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nSiLianBiaoNumAddress,nValue);
		return nValue;
		break;
	case SiLianBiaoDaBiaoNumAddress:
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nSiLianBiaoDaBiaoNumAddress,nValue);
		return nValue;
		break;
	case PingBiNumAddress:
		m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nPingBiNumAddress,nValue);
		return nValue;
		break;
	}
}

int  CLabeller::GetLabelActionNum(int nLabelSN)//��ȡ������Ŀ
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nValue=0;
        m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nLabelActionNumAddress,nValue);
		return nValue;

	}else
	{
		return 0;
	}

}

void  CLabeller::LabelReset()//��λ
{

	if (m_CFinsTcp.m_bConnected)
	{
		WriteLabelLog("�����������λ",FALSE);
		m_bScreenFlag=TRUE;
		int nDM=104;
		float dValue=1;
		m_CFinsTcp.WriteFloat32(nDM,dValue);
		SetLightColor(YellowLight);

	}

}




int  CLabeller::GetLabelReieveNum(int nLabelSN)//��ȡ�յ��ź���Ŀ
{
	if (m_CFinsTcp.m_bConnected)
	{
		int nValue=0;
	    m_CFinsTcp.ReadInt16(m_tLabellerInfo.tLabelWarnAddress[nLabelSN-1].nLabelReceiveNumAddress,nValue);
		return nValue;

	}else
	{
		return 0;
	}
}


void CLabeller::ReadParam()//��ȡ����
{

	m_strDirectory.Format("%s\\Config\\Public\\SCILabeller",GetCurrentAppPath());
	CreateMultilevelPath(m_strDirectory);
	m_strPath.Format("%s\\SCILabeller.ini",m_strDirectory);
	m_tLabellerInfo.nLabelPort=GetPrivateProfileInt("MarkInfo","nLablePort",9600,m_strPath);
	m_tLabellerInfo.strLabelIpAddress=GetPrivateProfileCString("MarkInfo","strLabelIpAddress","192.168.250.1",m_strPath);
	m_tLabellerInfo.nHeartbeatAddress=GetPrivateProfileInt("MarkInfo","nHeartbeatAddress",4200,m_strPath);
	m_tLabellerInfo.nStartBatchAddress=GetPrivateProfileInt("MarkInfo","nStartBatchAddress",4204,m_strPath);
	m_tLabellerInfo.nResetAlarmAddress=GetPrivateProfileInt("MarkInfo","nResetAlarmAddress",4208,m_strPath);
	m_tLabellerInfo.nLabelMotorAlarmAddress=GetPrivateProfileInt("MarkInfo","nLabelMotorAlarmAddress",4332,m_strPath);
	m_tLabellerInfo.nLabelSumAlarmAddress=GetPrivateProfileInt("MarkInfo","nLabelSumAlarmAddress",4336,m_strPath);
	m_tLabellerInfo.nEnableLabelName=GetPrivateProfileInt("MarkInfo","nEnableLabelName",1,m_strPath);
	m_tLabellerInfo.nManual1_ZD_SD_Address=GetPrivateProfileInt("MarkInfo","nManual1_ZD_SD_Address",502,m_strPath);
	m_tLabellerInfo.nFourLabelEanbleFlagAddress=GetPrivateProfileInt("MarkInfo","nFourLabelEanbleFlagAddress",4762,m_strPath);

	m_tLabellerInfo.nQiGangYaXiaYiChangAddr=GetPrivateProfileInt("MarkInfo","nQiGangYaXiaYiChangAddr",4630,m_strPath);
	m_tLabellerInfo.nQiGangTaiQiYiChangAddr=GetPrivateProfileInt("MarkInfo","nQiGangTaiQiYiChangAddr",4632,m_strPath);


	//Param
	m_tLabellerInfo.nDuoLunZhiJingAddress=GetPrivateProfileInt("MarkInfo","nDuoLunZhiJingAddress",100,m_strPath);

	m_tLabellerInfo.nBianMaQiMaiChongLiangAddr=GetPrivateProfileInt("MarkInfo","nBianMaQiMaiChongLiangAddr",106,m_strPath);

	m_tLabellerInfo.nJiPianEALengthAddr=GetPrivateProfileInt("MarkInfo","nJiPianEALengthAddr",504,m_strPath);

	m_tLabellerInfo.nBiaoQianLengthAddr=GetPrivateProfileInt("MarkInfo","nBiaoQianLengthAddr",320,m_strPath);

	m_tLabellerInfo.nPingBiXinHaoLengthAddr=GetPrivateProfileInt("MarkInfo","nPingBiXinHaoLengthAddr",330,m_strPath);

	m_tLabellerInfo.nDangeLeijiAddr=GetPrivateProfileInt("MarkInfo","nDangeLeijiAddr",160,m_strPath);

	m_tLabellerInfo.nZongDabiaoLeijiAddr=GetPrivateProfileInt("MarkInfo","nZongDabiaoLeijiAddr",168,m_strPath);

	m_tLabellerInfo.nLianXuDabiaoBaoJingAddr=GetPrivateProfileInt("MarkInfo","nLianXuDabiaoBaoJingAddr",145,m_strPath);

	m_tLabellerInfo.nSiLianBiaoWeiZhiAddr=GetPrivateProfileInt("MarkInfo","nSiLianBiaoWeiZhiAddr",4802,m_strPath);

	m_tLabellerInfo.nJiGuang1JuLiAddr=GetPrivateProfileInt("MarkInfo","nJiGuang1JuLiAddr",4804,m_strPath);

	m_tLabellerInfo.nJiGuang2JuLiAddr=GetPrivateProfileInt("MarkInfo","nJiGuang2JuLiAddr",4806,m_strPath);

	m_tLabellerInfo.nSiLianBiaoJianGeAddr=GetPrivateProfileInt("MarkInfo","nSiLianBiaoJianGeAddr",4550,m_strPath);

	m_tLabellerInfo.nBiaoQianDuanDaiShuAddr=GetPrivateProfileInt("MarkInfo","nBiaoQianDuanDaiShuAddr",166,m_strPath);

	//Control


	m_tLabellerInfo.nLianJiJianCeEnableAddr=GetPrivateProfileInt("MarkInfo","nLianJiJianCeEnableAddr",4750,m_strPath);
	m_tLabellerInfo.nLeiJiaDaBiaoBaoJingEnableAddr=GetPrivateProfileInt("MarkInfo","nLeiJiaDaBiaoBaoJingEnableAddr",4754,m_strPath);
	m_tLabellerInfo.nLeiXuBaoJingEnableAddr=GetPrivateProfileInt("MarkInfo","nLeiXuBaoJingEnableAddr",4758,m_strPath);

	m_tLabellerInfo.nMarkBiaoZhiEnableAddr=GetPrivateProfileInt("MarkInfo","nMarkBiaoZhiEnableAddr",4760,m_strPath);
	m_tLabellerInfo.nSiLianBiaoEnableAddr=GetPrivateProfileInt("MarkInfo","nSiLianBiaoEnableAddr",4762,m_strPath);
	m_tLabellerInfo.nShouDongChuBiaoMoshi=GetPrivateProfileInt("MarkInfo","nShouDongChuBiaoMoshi",4734,m_strPath);

	

	CString strKey;
	for (int k=0;k<LabelNum;k++)
	{
		strKey.Format("nLabelEnableLabelAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelEnableLabelAddress=GetPrivateProfileInt("MarkInfo",strKey,4212,m_strPath);
		strKey.Format("nLabelContinueAlarmAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelContinueAlarmAddress=GetPrivateProfileInt("MarkInfo",strKey,4392,m_strPath);
		strKey.Format("nLabelMissAlaramAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelMissAlaramAddress=GetPrivateProfileInt("MarkInfo",strKey,4300,m_strPath);
		strKey.Format("nLabelSettingFailedAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelSettingFailedAddress=GetPrivateProfileInt("MarkInfo",strKey,4316,m_strPath);
		strKey.Format("nLocateRightAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLocateRightAddress=GetPrivateProfileInt("MarkInfo",strKey,4306,m_strPath);
		strKey.Format("nLabelBreakAlarmAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelBreakAlarmAddress=GetPrivateProfileInt("MarkInfo",strKey,4372,m_strPath);

		strKey.Format("nLabelSumAlarmAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelSumAlarmAddress=GetPrivateProfileInt("MarkInfo",strKey,4340,m_strPath);

		strKey.Format("nFourLabelReceieveAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nFourLabelReceieveAddress=GetPrivateProfileInt("MarkInfo",strKey,4432,m_strPath);




		strKey.Format("nLabelCuoWeiAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelCuoWeiAddress=GetPrivateProfileInt("MarkInfo",strKey,4302,m_strPath);

		strKey.Format("nLabelEmptyAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelEmptyAddress=GetPrivateProfileInt("MarkInfo",strKey,4408,m_strPath);

		strKey.Format(" nLabelNOUseAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k]. nLabelNOUseAddress=GetPrivateProfileInt("MarkInfo",strKey,4356,m_strPath);







		///count
		strKey.Format("nLabelReceiveNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelReceiveNumAddress=GetPrivateProfileInt("MarkInfo",strKey,260,m_strPath);
		strKey.Format("nLabelActionNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nLabelActionNumAddress=GetPrivateProfileInt("MarkInfo",strKey,20,m_strPath);

		strKey.Format("nSensorNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nSensorNumAddress=GetPrivateProfileInt("MarkInfo",strKey,610,m_strPath);

		strKey.Format("nSiLianBiaoNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoNumAddress=GetPrivateProfileInt("MarkInfo",strKey,152,m_strPath);

		strKey.Format("nSiLianBiaoDaBiaoNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoDaBiaoNumAddress=GetPrivateProfileInt("MarkInfo",strKey,156,m_strPath);


		strKey.Format("nPingBiNumAddress_%d",k);
		m_tLabellerInfo.tLabelWarnAddress[k].nPingBiNumAddress=GetPrivateProfileInt("MarkInfo",strKey,52,m_strPath);

	}


	try
	{
		CString strPath;
		strPath.Format("%s\\RealLocation.tuple",m_strDirectory);
		ReadTuple((HTuple)strPath,&m_hvLabelRealLocation);

	}catch(HException& e)
	{
		TupleGenConst(12,0,&m_hvLabelRealLocation);

	}
	
	
	


}

void  CLabeller::WriteParam()//�������
{
	WritePrivateProfileInt("MarkInfo","nLablePort",m_tLabellerInfo.nLabelPort,m_strPath);
	WritePrivateProfileString("MarkInfo","strLabelIpAddress",m_tLabellerInfo.strLabelIpAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nHeartbeatAddress",m_tLabellerInfo.nHeartbeatAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nStartBatchAddress",m_tLabellerInfo.nStartBatchAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nResetAlarmAddress",m_tLabellerInfo.nResetAlarmAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nLabelMotorAlarmAddress",m_tLabellerInfo.nLabelMotorAlarmAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nLabelSumAlarmAddress",m_tLabellerInfo.nLabelSumAlarmAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nEnableLabelName",m_tLabellerInfo.nEnableLabelName,m_strPath);
	WritePrivateProfileInt("MarkInfo","nFourLabelEanbleFlagAddress",m_tLabellerInfo.nFourLabelEanbleFlagAddress,m_strPath);
	WritePrivateProfileInt("MarkInfo","nQiGangYaXiaYiChangAddr",m_tLabellerInfo.nQiGangYaXiaYiChangAddr,m_strPath);
	WritePrivateProfileInt("MarkInfo","nQiGangTaiQiYiChangAddr",m_tLabellerInfo.nQiGangTaiQiYiChangAddr,m_strPath);
	WritePrivateProfileInt("MarkInfo","nShouDongChuBiaoMoshi",m_tLabellerInfo.nShouDongChuBiaoMoshi,m_strPath);

	//Param
	WritePrivateProfileInt("MarkInfo","nDuoLunZhiJingAddress",m_tLabellerInfo.nDuoLunZhiJingAddress,m_strPath);

	WritePrivateProfileInt("MarkInfo","nBianMaQiMaiChongLiangAddr",m_tLabellerInfo.nBianMaQiMaiChongLiangAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nJiPianEALengthAddr",m_tLabellerInfo.nJiPianEALengthAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nBiaoQianLengthAddr",m_tLabellerInfo.nBiaoQianLengthAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nPingBiXinHaoLengthAddr",m_tLabellerInfo.nPingBiXinHaoLengthAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nDangeLeijiAddr",m_tLabellerInfo.nDangeLeijiAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nZongDabiaoLeijiAddr",m_tLabellerInfo.nZongDabiaoLeijiAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nLianXuDabiaoBaoJingAddr",m_tLabellerInfo.nLianXuDabiaoBaoJingAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nSiLianBiaoWeiZhiAddr",m_tLabellerInfo.nSiLianBiaoWeiZhiAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nJiGuang1JuLiAddr",m_tLabellerInfo.nJiGuang1JuLiAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nJiGuang2JuLiAddr",m_tLabellerInfo.nJiGuang2JuLiAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nSiLianBiaoJianGeAddr",m_tLabellerInfo.nSiLianBiaoJianGeAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nBiaoQianDuanDaiShuAddr",m_tLabellerInfo.nBiaoQianDuanDaiShuAddr,m_strPath);

	//Control


	WritePrivateProfileInt("MarkInfo","nLianJiJianCeEnableAddr",m_tLabellerInfo.nLianJiJianCeEnableAddr,m_strPath);
	WritePrivateProfileInt("MarkInfo","nLeiJiaDaBiaoBaoJingEnableAddr",m_tLabellerInfo.nLeiJiaDaBiaoBaoJingEnableAddr,m_strPath);
	WritePrivateProfileInt("MarkInfo","nLeiXuBaoJingEnableAddr",m_tLabellerInfo.nLeiXuBaoJingEnableAddr,m_strPath);

	WritePrivateProfileInt("MarkInfo","nMarkBiaoZhiEnableAddr",m_tLabellerInfo.nMarkBiaoZhiEnableAddr,m_strPath);
	WritePrivateProfileInt("MarkInfo","nSiLianBiaoEnableAddr",m_tLabellerInfo.nSiLianBiaoEnableAddr,m_strPath);


	CString strKey;
	for (int k=0;k<LabelNum;k++)
	{
		strKey.Format("nLabelEnableLabelAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelEnableLabelAddress,m_strPath);
		
		strKey.Format("nLabelContinueAlarmAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelContinueAlarmAddress,m_strPath);
		strKey.Format("nLabelMissAlaramAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelMissAlaramAddress,m_strPath);
		strKey.Format("nLabelSettingFailedAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelSettingFailedAddress,m_strPath);
		strKey.Format("nLocateRightAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLocateRightAddress,m_strPath);
		strKey.Format("nLabelBreakAlarmAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelBreakAlarmAddress,m_strPath);
		strKey.Format("nLabelSumAlarmAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelSumAlarmAddress,m_strPath);
		strKey.Format("nFourLabelReceieveAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nFourLabelReceieveAddress,m_strPath);
		strKey.Format("nLabelReceiveNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelReceiveNumAddress,m_strPath);
		strKey.Format("nLabelActionNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelActionNumAddress,m_strPath);
		strKey.Format("nSensorNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nSensorNumAddress,m_strPath);
		strKey.Format("nSiLianBiaoNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoNumAddress,m_strPath);
		strKey.Format("nSiLianBiaoDaBiaoNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoDaBiaoNumAddress,m_strPath);
		strKey.Format("nPingBiNumAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nPingBiNumAddress,m_strPath);


		strKey.Format("nLabelCuoWeiAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelCuoWeiAddress,m_strPath);


		strKey.Format("nLabelEmptyAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelEmptyAddress,m_strPath);

		strKey.Format(" nLabelNOUseAddress_%d",k);
		WritePrivateProfileInt("MarkInfo",strKey,m_tLabellerInfo.tLabelWarnAddress[k].nLabelNOUseAddress,m_strPath);

	}
}

void  CLabeller::WriteLabelLog(CString strMessage,BOOL bExceptFlag)//д��������־
{
	
	if (bExceptFlag)
	{
		if (m_bWarnStaus==FALSE)
		{
			//m_bWarnStaus=TRUE;
			//((DHInterface*)(m_tInit.pLogView))->DoSomething("EXCEPTION", (void*)(LPCSTR)strMessage, NULL);
		}
	
	}else
	{
		((DHInterface*)(m_tInit.pLogView))->DoSomething("OPERATERLOG", (void*)(LPCSTR)strMessage, NULL);
	}
  

}

void  CLabeller::GetLabelCount(HTuple* hv_Name,HTuple* hv_Count)//����
{

	
	(*hv_Name).Clear();
	(*hv_Count).Clear();
	int nItemCount;
	CString strName,strValue;
	for (int k=0;k<LabelNum;k++)
	{

		int nValue=0;
		//�յ�����ź�	��������ȡ	D36
		int nDM=36;
		strName.Format("�����%d�յ�����ź�(%d)",k,nDM);
		m_CFinsTcp.ReadInt16(nDM,nValue);
		strValue.Format("%d",nValue);
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);


	//	ִ�е������ź�	��������ȡ	D260

		strName.Format("�����%d�յ��ź���(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nLabelReceiveNumAddress);
		strValue.Format("%d",GetLabelNum(k+1,LabelReceiveNumAddress));
		
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);
	
		//���θ���	��������ȡ	D52
		strName.Format("�����%d������(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nPingBiNumAddress);
		strValue.Format("%d",GetLabelNum(k+1,PingBiNumAddress));

		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);


		//��������	��������ȡ	D20

		strName.Format("�����%d�����(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nLabelActionNumAddress);
		strValue.Format("%d",GetLabelNum(k+1,LabelActionNumAddress));


		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);

	

		//��⵽�������	��������ȡ	D610
		strName.Format("�����%d��Ӧ��(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nSensorNumAddress);
		strValue.Format("%d",GetLabelNum(k+1,SensorNumAddress));
		

		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);



		//	��������ȡ	D161

		 nDM=161;
		strName.Format("�����%d�ۼƸ���(%d)",k,nDM);
		m_CFinsTcp.ReadInt16(nDM,nValue);
		strValue.Format("%d",nValue);
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);


		//	�ֶ���������	��������ȡ	D76

		nDM=76;
		strName.Format("�����%d�ֶ���������(%d)",k,nDM);
		m_CFinsTcp.ReadInt16(nDM,nValue);
		strValue.Format("%d",nValue);
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);


		//	©�����	��������ȡ	D22

		nDM=22;
		strName.Format("�����%d©�����(%d)",k,nDM);
		m_CFinsTcp.ReadInt16(nDM,nValue);
		strValue.Format("%d",nValue);
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);

		//������������	��������ȡ	D140

		nDM=140;
		strName.Format("�����%d������������(%d)",k,nDM);
		m_CFinsTcp.ReadInt16(nDM,nValue);
		strValue.Format("%d",nValue);
		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);

		//�յ�4�����ź�	��������ȡ	D152

		strName.Format("�����%d�յ���������(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoNumAddress);
		strValue.Format("%d",GetLabelNum(k+1,SiLianBiaoNumAddress));
		

		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);

		//4����������	��������ȡ	D156
		strName.Format("�����%d����������(%d)",k,m_tLabellerInfo.tLabelWarnAddress[k].nSiLianBiaoDaBiaoNumAddress);
	
		strValue.Format("%d",GetLabelNum(k+1,SiLianBiaoDaBiaoNumAddress));

		(*hv_Name).Append((HTuple)strName);
		(*hv_Count).Append((HTuple)strValue);


	}

}


void  CLabeller::GetLabelStatus(HTuple* hv_EachFalg,HTuple* hv_WarnMessage)//��ȡÿ���˿ڵ�״̬
{
	(*hv_EachFalg).Clear();
	(*hv_WarnMessage).Clear();
	CString strException;
	{
		float dValue=1;
		CString strName;
		int nStartDM=10052;//�ϴ���Ӧ��
		int nTempDM;
		int nControlDM = 10024;//���ƶ�ȡ�ϴ�
		//std::vector<float> ControlTempVector;
		//std::vector<float> StatusTempVector;
		//m_CFinsTcp.ReadFloat32Batch(nStartDM,12, StatusTempVector);///״̬��ַ12��
		//m_CFinsTcp.ReadFloat32Batch(nControlDM, 12, ControlTempVector);///���Ʊ�־λ12��

		//if (StatusTempVector.size()== ControlTempVector.size() && ControlTempVector.size()>0)
		//{


		//	for (int i=0;i<ControlTempVector.size();i++)
		//	{
		//		if (ControlTempVector[i]==1)
		//		{
		//			strName.Format("%d��������ϴ���ⱨ��(%d)", i + 1, nStartDM + 2 * i);
		//			(*hv_WarnMessage).Append((HTuple)strName);
		//			(*hv_EachFalg).Append(StatusTempVector[i]);


		//		}
		//		else
		//		{
		//			strName.Format("%d��������ϴ���ⱨ��(%d)", i + 1, nStartDM + 2 * i);
		//			(*hv_WarnMessage).Append((HTuple)strName);
		//			(*hv_EachFalg).Append(0);

		//		}
		//		
		//	}


		//}

		/*����λ��ͨѶ�쳣	D200
		�ϴ�·������������쳣	D202
		�ϴ�·��������½��쳣	D204
		�´�·������������쳣	D206
		�´�·��������½��쳣	D208
		������ѹ�����������쳣	D210
		������ѹ�������½��쳣	D212*/


		nStartDM=200;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("����λ��ͨѶ�쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);



		nStartDM=202;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("�ϴ�·������������쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);



		nStartDM=204;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("�ϴ�·��������½��쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);


		nStartDM=206;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("�´�·������������쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);


		nStartDM=208;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("�´�·��������½��쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);

		nStartDM=210;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("������ѹ�����������쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);


		nStartDM=212;
		m_CFinsTcp.ReadFloat32(nStartDM,dValue);
		strName.Format("������ѹ�������½��쳣(%d)",nStartDM);
		(*hv_WarnMessage).Append((HTuple)strName);
		(*hv_EachFalg).Append(dValue);


		////�ٶ�����������׻���̧��״̬Ҫ����/////
		//if (m_dRealSpeed>0)
		{

			nStartDM = 58;//����������̧��
			m_CFinsTcp.ReadFloat32(nStartDM, dValue);
			strName.Format("������ѹ����������ʱδ��ѹ(58)");
			(*hv_WarnMessage).Append((HTuple)strName);
			(*hv_EachFalg).Append(dValue);

			nStartDM = 50;//�ϴ�·���������̧��
			m_CFinsTcp.ReadFloat32(nStartDM, dValue);
			strName.Format("�ϴ�·�������������ʱδ��ѹ(50)");
			(*hv_WarnMessage).Append((HTuple)strName);
			(*hv_EachFalg).Append(dValue);


			nStartDM = 54;//�´�·���������̧��
			m_CFinsTcp.ReadFloat32(nStartDM, dValue);
			strName.Format("�´�·�������������ʱδ��ѹ(54)");
			(*hv_WarnMessage).Append((HTuple)strName);
			(*hv_EachFalg).Append(dValue);

		}







	}






}
void  CLabeller::ClearCount()//�����������
{

	if (m_CFinsTcp.m_bConnected)
	{
		int nValue = 0;
		m_CFinsTcp.WriteInt16(m_tLabellerInfo.nStartBatchAddress, nValue);
		Sleep(10);
		nValue = 1;
		m_CFinsTcp.WriteInt16(m_tLabellerInfo.nStartBatchAddress, nValue);
		CString strInfo;
		strInfo = "�������������";
		WriteLabelLog(strInfo, FALSE);
	}
}

