#include "StdAfx.h"
#include "FinsTCPExt.h"
#include "PowerLimit.h"
#include "SeekersDlg.h"

//class CriticalSectionHelper
//{
//public:
//	CriticalSectionHelper(CRITICAL_SECTION *pcs)
//	{
//		m_pcs = pcs;
//		EnterCriticalSection(m_pcs);
//	}
//
//	~CriticalSectionHelper()
//	{
//		LeaveCriticalSection(m_pcs);
//		m_pcs = NULL;
//	}
//
//private:
//	CRITICAL_SECTION *m_pcs;
//};




CPowerLimit::CPowerLimit(void)
{
	m_bFactoryPower=FALSE;
	m_bEngineerPower=FALSE;
	m_bOperatorPower=FALSE;
	m_bSuperEngineerPower=FALSE;
	m_strCurrentStatus=_T(" ");
	InitializeCriticalSection(&m_csPowChange);
}





CPowerLimit::~CPowerLimit(void)
{
	DeleteCriticalSection(&m_csPowChange);
}
void  CPowerLimit::SetStatus(CString strInfo)
{
	{
		CriticalSectionHelper PLock(&m_csPowChange);
		m_strCurrentStatus=strInfo;
	}
	
}
CString  CPowerLimit::GetStatus()
{

	{
	CriticalSectionHelper PLock(&m_csPowChange);
	return m_strCurrentStatus;
	}
}
void  CPowerLimit::ClearFlag()
{
	CString strInfo;
	strInfo.Format("%s�˳�ϵͳ",GetStatus());
	g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
	SetStatus(" ");
	m_bFactoryPower=FALSE;
	m_bEngineerPower=FALSE;
	m_bOperatorPower=FALSE;
	m_bSuperEngineerPower=FALSE;
	for (int i=0;i<g_nGrabberCount;i++)
	{
		int nPowerFlag=0;
		g_vec_Algorithm[i]->DoSomething("SETPARAM","SetPowerFlag",&nPowerFlag);
	}
}


BOOL CPowerLimit::addData(PowerInfo data)
{
	std::vector<PowerInfo>::iterator iter;
	for (iter = m_vec_EngineerPowerList.begin(); iter != m_vec_EngineerPowerList.end(); iter++)
	{
		if (data.strJobNum == iter->strJobNum/* &&data.strJobPassWord == iter->strJobPassWord*/)
		{
			return FALSE;
		}
	}
	m_vec_EngineerPowerList.push_back(data);
	return TRUE;
}

void  CPowerLimit::ReadParam()//��ȡȨ����Ϣ
{
	m_strFilePath.Format("%s\\Config\\Public\\SCIStorage\\DataBase.ini",GetCurrentAppPath());	
	m_vec_EngineerPowerList.clear();
	int nPLcount = GetPrivateProfileInt("POWERLIMITINFO", "COUNT", 1, m_strFilePath);//�û���Ŀ
	for (int nPL = 0; nPL < nPLcount; nPL++)
	{
		PowerInfo data;
		CString strKey, strValue;
		strKey.Format("PowerType%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "����ʦ", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strPowerType=strValue;
		strKey.Format("JOBNUMBER%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "123", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strJobNum=strValue;
		strKey.Format("PASSWORD%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "123", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strJobPassWord=strValue;
		addData(data);//����
	}


}
void  CPowerLimit::WriteParam()//д��Ȩ��
{


	int nPLcount=m_vec_EngineerPowerList.size();
	WritePrivateProfileInt("POWERLIMITINFO", "COUNT", nPLcount, m_strFilePath);//�û���Ŀ
	for (int nPL = 0; nPL < nPLcount; nPL++)
	{
		PowerInfo  tPowerInfo;
		tPowerInfo=m_vec_EngineerPowerList[nPL];
		CString strKey, strValue;

		strKey.Format("PowerType%d", nPL);
		WritePrivateProfileString("POWERLIMIT", strKey, tPowerInfo.strPowerType, m_strFilePath);
		strKey.Format("JOBNUMBER%d", nPL);
		WritePrivateProfileString("POWERLIMIT", strKey, tPowerInfo.strJobNum, m_strFilePath);
		strKey.Format("PASSWORD%d", nPL);
		WritePrivateProfileString("POWERLIMIT", strKey, tPowerInfo.strJobPassWord,m_strFilePath);
	}




}

BOOL  CPowerLimit::IsAllContinue()
{
	if (m_bFactoryPower==FALSE && m_bEngineerPower==FALSE&& m_bOperatorPower==FALSE&& m_bSuperEngineerPower==FALSE)//��������һ�����л���ͼ
	{

		//if (FALSE==((CSeekersDlg*)g_pMainDialog)->Login())
		{
			return FALSE;
		}

	}else
	{
		return TRUE;
	}

}
BOOL  CPowerLimit::IsEngineAndFactoryContinue()
{
	if (m_bSuperEngineerPower==FALSE&&m_bEngineerPower==FALSE&& m_bFactoryPower==FALSE)//��Ҫ����Ȩ��
	{
		AfxMessageBox("���������Ҫ����ʦ���߳���Ȩ�ޣ�");
		//if (FALSE==((CSeekersDlg*)g_pMainDialog)->Login())
		{
		return FALSE;
		}
	}

}

BOOL  CPowerLimit::IsJustFactoryContinue()
{
	if (m_bFactoryPower==FALSE)//��Ҫ����Ȩ��
	{
		AfxMessageBox("���������Ҫ����Ȩ�ޣ�");
		//if (FALSE==((CSeekersDlg*)g_pMainDialog)->Login())
		{
			return  FALSE;
		}
	}

}
int  CPowerLimit::GetUserIndex()
{
	int nUserPower=0;
	if (m_bFactoryPower==TRUE )//��������һ�������㷨����
	{
		nUserPower=3;
	}else if ( m_bSuperEngineerPower==TRUE )
	{
		nUserPower=2;
	}else if ( m_bEngineerPower==TRUE )
	{
		nUserPower=1;
	}else if (m_bOperatorPower==TRUE)
	{
		nUserPower=0;
	}
	return nUserPower;
}

BOOL CPowerLimit::MatchPower(const PowerInfo & TempPowerInfo)
{

	std::vector<PowerInfo>::iterator iter;
	for (iter = m_vec_EngineerPowerList.begin(); iter != m_vec_EngineerPowerList.end(); iter++)
	{
		if (TempPowerInfo.strPowerType == iter->strPowerType &&TempPowerInfo.strJobNum == iter->strJobNum &&TempPowerInfo.strJobPassWord == iter->strJobPassWord)
		{
			return TRUE;
		}
	}
	return FALSE;
	
}

 BOOL  CPowerLimit::GetUserPower(CString strUserName,CString strJobNum,CString strPassWord)
 {
	 CString strCurrentStatus;
	 ClearFlag();
	 if (strUserName=="����Ա")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("����Ա���벻�ԣ����������룡");
			 return FALSE;
		 }
		 m_bOperatorPower=TRUE;
		 CString strInfo;
		 strCurrentStatus.Format("Ȩ������:%s ����:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s��½ϵͳ",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
		 return TRUE;

	 }else if (strUserName=="����ʦ")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("����ʦ���벻�ԣ����������룡");
			 return FALSE;
		 }
		 CString strInfo;
		 strCurrentStatus.Format("Ȩ������:%s ����:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s��½ϵͳ",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);


		 m_bEngineerPower=TRUE;
		 return TRUE;
	 }else if (strUserName=="��������ʦ")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("��������ʦ���벻�ԣ����������룡");
			 return FALSE;
		 }
		 CString strInfo;
		 strCurrentStatus.Format("Ȩ������:%s ����:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s��½ϵͳ",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);


		 m_bSuperEngineerPower=TRUE;
		 return TRUE;
	 }else if (strUserName=="����")
	 {



		 if (strPassWord!=g_strFactoryPassWord)
		 {
			 AfxMessageBox("�������벻�ԣ����������룡");
			  return FALSE;
		 }

		 m_bFactoryPower=TRUE;

		 CString strInfo;
		 strCurrentStatus.Format("Ȩ������:%s ����:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s��½ϵͳ",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
		  return TRUE;
	 }
 }