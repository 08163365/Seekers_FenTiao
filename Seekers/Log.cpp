#include "StdAfx.h"
#include "Log.h"
#include "SeekersDlg.h"

CLog::CLog(void)
{
	m_bWriteTimeLog = FALSE;
	Create();
}


CLog::~CLog(void)
{
	try
	{
		CloseFile(m_hvTimeFile);
		CloseFile(m_hvExceptionFile);
	}catch(HException)
	{

	}
}



void CLog::TimeLoging(CString strTimeLog)
{
	if (FALSE == m_bWriteTimeLog)
	{
		return;
	}
	m_csTime.Lock();

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strTime;
	strTime.Format("%02d:%02d:%02d ::%sms\n",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,strTimeLog);
	try
	{
		FwriteString(m_hvTimeFile,HTuple(strTime));
	}catch(HException)
	{

	}

	m_csTime.Unlock();

}
void CLog::Exceptionloging(CString strExceptionlog)
{
	m_csException.Lock();

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strException;
	strException.Format("%02d:%02d:%02d ::%s\n",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,strExceptionlog);

	try
	{
		FwriteString(m_hvExceptionFile,HTuple(strException));
	}catch(HException)
	{

	}

	m_csException.Unlock();


	((CSeekersDlg*)g_pMainDialog)->m_pException->Show();
}
void CLog::ReadException(CStringArray* strArrayExcepion)
{
	HTuple Line,IsEOF;
	HTuple FileHandle;
	try
	{
		OpenFile(HTuple(m_strExceptionFileName),"input",&FileHandle);
		CString strRead;
		do
		{
			FreadLine(FileHandle,&Line,&IsEOF);
			strRead.Format("%s",Line.S().Text());
			if (strRead!="")
			{
				strArrayExcepion->Add(strRead);
			}
			
		}while(IsEOF==0);
	}catch(HException){}
	try
	{
		CloseFile(FileHandle);
	}catch(HException){}
}
void CLog::Create()
{
	CString strDir;
	strDir.Format("%s\\log",GetCurrentAppPath());
	CreateMultilevelPath(strDir);
	try
	{
		m_strExceptionFileName.Format("%s\\ExceptionLog.txt",strDir);
		OpenFile(HTuple(m_strExceptionFileName),"output",&m_hvExceptionFile);
		m_strTimeFileName.Format("%s\\TimeLog.txt",strDir);
		OpenFile(HTuple(m_strTimeFileName),"output",&m_hvTimeFile);
	}catch(HException)
	{

	}

	

}