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
	strInfo.Format("%s退出系统",GetStatus());
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

void  CPowerLimit::ReadParam()//读取权限信息
{
	m_strFilePath.Format("%s\\Config\\Public\\SCIStorage\\DataBase.ini",GetCurrentAppPath());	
	m_vec_EngineerPowerList.clear();
	int nPLcount = GetPrivateProfileInt("POWERLIMITINFO", "COUNT", 1, m_strFilePath);//用户数目
	for (int nPL = 0; nPL < nPLcount; nPL++)
	{
		PowerInfo data;
		CString strKey, strValue;
		strKey.Format("PowerType%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "工程师", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strPowerType=strValue;
		strKey.Format("JOBNUMBER%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "123", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strJobNum=strValue;
		strKey.Format("PASSWORD%d", nPL);
		GetPrivateProfileString("POWERLIMIT", strKey, "123", strValue.GetBuffer(MAX_PATH), MAX_PATH, m_strFilePath);
		data.strJobPassWord=strValue;
		addData(data);//增加
	}


}
void  CPowerLimit::WriteParam()//写入权限
{


	int nPLcount=m_vec_EngineerPowerList.size();
	WritePrivateProfileInt("POWERLIMITINFO", "COUNT", nPLcount, m_strFilePath);//用户数目
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
	if (m_bFactoryPower==FALSE && m_bEngineerPower==FALSE&& m_bOperatorPower==FALSE&& m_bSuperEngineerPower==FALSE)//三者满足一即可切换视图
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
	if (m_bSuperEngineerPower==FALSE&&m_bEngineerPower==FALSE&& m_bFactoryPower==FALSE)//需要厂家权限
	{
		AfxMessageBox("此项操作需要工程师或者厂家权限！");
		//if (FALSE==((CSeekersDlg*)g_pMainDialog)->Login())
		{
		return FALSE;
		}
	}

}

BOOL  CPowerLimit::IsJustFactoryContinue()
{
	if (m_bFactoryPower==FALSE)//需要厂家权限
	{
		AfxMessageBox("此项操作需要厂家权限！");
		//if (FALSE==((CSeekersDlg*)g_pMainDialog)->Login())
		{
			return  FALSE;
		}
	}

}
int  CPowerLimit::GetUserIndex()
{
	int nUserPower=0;
	if (m_bFactoryPower==TRUE )//三者满足一即进入算法界面
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
	 if (strUserName=="操作员")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("操作员密码不对，请重新输入！");
			 return FALSE;
		 }
		 m_bOperatorPower=TRUE;
		 CString strInfo;
		 strCurrentStatus.Format("权限类型:%s 工号:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s登陆系统",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
		 return TRUE;

	 }else if (strUserName=="工程师")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("工程师密码不对，请重新输入！");
			 return FALSE;
		 }
		 CString strInfo;
		 strCurrentStatus.Format("权限类型:%s 工号:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s登陆系统",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);


		 m_bEngineerPower=TRUE;
		 return TRUE;
	 }else if (strUserName=="超级工程师")
	 {

		 PowerInfo  tPowerInfo;
		 tPowerInfo.strPowerType=strUserName;
		 tPowerInfo.strJobNum=strJobNum;
		 tPowerInfo.strJobPassWord=strPassWord;
		 if (MatchPower(tPowerInfo)==FALSE)
		 {
			 AfxMessageBox("超级工程师密码不对，请重新输入！");
			 return FALSE;
		 }
		 CString strInfo;
		 strCurrentStatus.Format("权限类型:%s 工号:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s登陆系统",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);


		 m_bSuperEngineerPower=TRUE;
		 return TRUE;
	 }else if (strUserName=="厂家")
	 {



		 if (strPassWord!=g_strFactoryPassWord)
		 {
			 AfxMessageBox("厂家密码不对，请重新输入！");
			  return FALSE;
		 }

		 m_bFactoryPower=TRUE;

		 CString strInfo;
		 strCurrentStatus.Format("权限类型:%s 工号:%s",strUserName,strJobNum);
		 SetStatus(strCurrentStatus);
		 strInfo.Format("%s登陆系统",GetStatus());
		 g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strInfo,NULL);
		  return TRUE;
	 }
 }