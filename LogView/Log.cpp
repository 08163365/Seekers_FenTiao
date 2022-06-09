#include "stdafx.h"
#include "Log.h"
#include <direct.h>


CLog::CLog(void)
{
	m_bDlgCreated = FALSE;
	m_bWriteTimeLog = FALSE;
	m_bShowException = FALSE;
	Create();
	m_bSoftExceptionStauts=FALSE;
	InitializeCriticalSection(&m_csException);
	InitializeCriticalSection(&m_csTime);
	InitializeCriticalSection(&m_csRecord);
	InitializeCriticalSection(&m_OperateChange);
	m_strGongweiName="上工位";
}


CLog::~CLog(void)
{
	DeleteCriticalSection(&m_csException);
	DeleteCriticalSection(&m_csTime);
	DeleteCriticalSection(&m_csRecord);
	DeleteCriticalSection(&m_OperateChange);
}
void CLog::WriteOperateLog(CString strOperateLog)
{

	EnterCriticalSection(&m_csTime);
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strMessage;
	strMessage.Format(_T("%02d%02d%02d %02d:%02d:%02d.%03d:%s %s\r\n"),CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds,m_strGongweiName,strOperateLog);
	try
	{
		CString strTempLog;
		CreateMultilevelPath(m_strOperatePath);
		strTempLog.Format(_T("%s\\OperateLog-%s.txt"),m_strOperatePath,m_strBatch);
		BOOL bOpenFlag=FALSE;

		if (TRUE==PathFileExists(strTempLog))
		{
			bOpenFlag=m_fileOperateLog.Open(strTempLog,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
		}else
		{
			bOpenFlag=m_fileOperateLog.Open(strTempLog,CFile::modeCreate|CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);

		}
		CString strOperateInfo;
		if (bOpenFlag)
		{
			m_fileOperateLog.SeekToEnd();
			m_fileOperateLog.Write(strMessage.GetBuffer(strMessage.GetLength()),strMessage.GetLength());
			m_fileOperateLog.Close();
			strOperateInfo.Format("%s",strMessage);
		}else
		{
			strOperateInfo.Format("创建文件路径(%s)失败,请确认文件路径是否正常,确认权限是否正常 操作信息:%s ",m_strOperateLog,strMessage);
		}
		{
			CString strPath;
			BOOL bOpenFlag=FALSE;
			if (TRUE==PathFileExists(m_strOperateLog))
			{
				bOpenFlag=m_fileOperateLog.Open(m_strOperateLog,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
			}else
			{
				bOpenFlag=m_fileOperateLog.Open(m_strOperateLog,CFile::modeCreate|CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
			}

			if (bOpenFlag)
			{
				m_fileOperateLog.SeekToEnd();
				m_fileOperateLog.Write(strOperateInfo.GetBuffer(strOperateInfo.GetLength()),strOperateInfo.GetLength());
				m_fileOperateLog.Close();
			}
		}





	
	}catch(CFileException)
	{

	}

	LeaveCriticalSection(&m_csTime);


}

int CLog::DoSomething(void* message,void* wParam,void* lParam)
{
	CString str = (char*)message;

	if ("LOG" == str)
	{
		CString strMessage = (char*)wParam;
		TimeLoging(strMessage);
		return TRUE;

	}else if("TEMPLOG"== str)
	{
	
	CString strMessage = (char*)wParam;
	Loging(strMessage);
	return TRUE;
	}else if ("OPERATERLOG" == str)//操作日志
	{
		CString strMessage = (char*)wParam;
		WriteOperateLog(strMessage);
		return TRUE;

	}else if ("EXCEPTION" == str)
	{
		CString strMessage = (char*)wParam;
		{
			CriticalSectionHelper Lock(&m_OperateChange);
			m_bSoftExceptionStauts=TRUE;
		}
		
		Exceptionloging(strMessage);

		return TRUE;

	}else if ("SETPARAM" == str)
	{
		CString strName = (char*)wParam;

		if ("SHOWEXCEPTION" == strName)
		{
			BOOL bValue = *(BOOL*)lParam;

			if (bValue == NULL)
			{
				return FALSE;
			}
			if (bValue == TRUE)
			{
				m_pDlgException->m_bShow = TRUE;
				m_pDlgException->ShowException(m_strExceptionFileName);

				//m_pDlgException->ShowWindow(SW_NORMAL);

			}else
			{
				m_pDlgException->m_bShow = FALSE;
				m_pDlgException->ShowWindow(SW_HIDE);
			}

			return TRUE;

		}
		else if ("WRITELOG" == strName)
		{
			BOOL bValue = *(BOOL*)lParam;

			m_bWriteTimeLog = bValue;

		}else if ("ISSHOW" == strName)
		{
			BOOL bValue = *(BOOL*)lParam;
			if (TRUE == bValue)
			{
				m_pDlgException->m_bShow = TRUE;
			}else
			{
				m_pDlgException->m_bShow = FALSE;
			}
		}else if ("Batch" == strName)
		{
			CriticalSectionHelper Lock(&m_OperateChange);
			m_strBatch=(char*)lParam;
		}else if ("OperatePath" == strName)
		{
			CriticalSectionHelper Lock(&m_OperateChange);
			m_strOperatePath=(char*)lParam;
		}else if("GONGWEI"==strName)
		{
			CString strInfoName=(char*)lParam;
			if ("UpOffset"==strInfoName)
			{
				m_strGongweiName="上带路";
			}else
			{
				m_strGongweiName="下带路";
			}
			
			return TRUE;
		}else
		{
			return FALSE;
		}
	}else if ("GETPARAM" == str)
	{
		CString strName = (char*)wParam;

		if ("WRITELOG" == strName)
		{
			*(BOOL*)lParam = m_bWriteTimeLog;

			return TRUE;

		}else if ("SoftStatus" == strName)
		{
		    CriticalSectionHelper Lock(&m_OperateChange);
			*(BOOL*)lParam = m_bSoftExceptionStauts;
		}else
		{
			return FALSE;
		}
	}else if ("CREATEEXCEPTION" == str)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState())

		m_pDlgException = new CDlgException;

		m_pDlgException->m_pLog = this;

		m_pDlgException->Create(IDD_DIALOG_EXCEPTION,NULL);
		
		m_pDlgException->CenterWindow();

		m_bDlgCreated = TRUE;

	}else
	{
		return FALSE;
	}
}
void CLog::Loging(CString strTimeLog)
{


	EnterCriticalSection(&m_csTime);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d %s\r\n"),CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour,CurTime.wMinute,CurTime.wSecond,strTimeLog);
	try
	{
		m_fileTime.Open(m_strTimeFileName,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
		m_fileTime.SeekToEnd();
		m_fileTime.Write(strTime.GetBuffer(strTime.GetLength()),strTime.GetLength());
		m_fileTime.Close();
	}catch(...)
	{

	}

	LeaveCriticalSection(&m_csTime);

}
void CLog::TimeLoging(CString strTimeLog)
{
	if (FALSE == m_bWriteTimeLog)
	{
		return;
	}

	EnterCriticalSection(&m_csTime);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strTime;
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d %sms\r\n"),CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour,CurTime.wMinute,CurTime.wSecond,strTimeLog);
	try
	{
		m_fileTime.Open(m_strTimeFileName,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
		m_fileTime.SeekToEnd();
		m_fileTime.Write(strTime.GetBuffer(strTime.GetLength()),strTime.GetLength());
		m_fileTime.Close();
	}catch(...)
	{

	}

	LeaveCriticalSection(&m_csTime);

}
void CLog::Exceptionloging(CString strExceptionlog)
{
	EnterCriticalSection(&m_csException);
	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	CString strException;
	strException.Format(_T("%04d/%02d/%02d %02d:%02d:%02d %s\r\n"),CurTime.wYear,CurTime.wMonth,CurTime.wDay,CurTime.wHour,CurTime.wMinute,CurTime.wSecond,strExceptionlog);
	
	try
	{
		m_fileException.Open(m_strExceptionFileName,CFile::modeReadWrite | CFile::typeBinary|CFile::shareDenyNone);
		m_fileException.SeekToEnd();
		m_fileException.Write(strException.GetBuffer(strException.GetLength()),strException.GetLength());
		m_fileException.Close();
	}catch(...){}

	LeaveCriticalSection(&m_csException);

	if (m_bDlgCreated == TRUE)
	{
		EnterCriticalSection(&m_csException);
		m_pDlgException->ShowException(m_strExceptionFileName);
		LeaveCriticalSection(&m_csException);
		//m_pDlgException->ShowWindow(SW_NORMAL);
	}
}
void CLog::ReadException(CStringArray* strArrayExcepion)
{
	EnterCriticalSection(&m_csException);
	try
	{
		m_fileException.Open(m_strExceptionFileName,CFile::modeRead);
		CString strRead;
		while(m_fileException.ReadString(strRead))
		{
			strArrayExcepion->Add(strRead);
		}
	}catch(...){}
	try
	{
		m_fileException.Close();
	}catch(...){}

	LeaveCriticalSection(&m_csException);
}
void CLog::Create()
{

	SYSTEMTIME tSystemtime;
	GetLocalTime(&tSystemtime);
	CString strDir,strBackup,strFileBackup,strOperateDir;
	strDir.Format(_T("%s\\Log"),GetCurrentAppPath());
	strBackup.Format(_T("%s\\Log\\Backup"),GetCurrentAppPath());
	strOperateDir.Format(_T("%s\\OperateFile\\%04d-%02d-%02d"),strDir,tSystemtime.wYear,tSystemtime.wMonth,tSystemtime.wDay);
	//CreateMultilevelPath(strDir);
	CreateMultilevelPath(strBackup);
   CreateMultilevelPath(strOperateDir);
	try
	{
		m_strExceptionFileName.Format(_T("%s\\ExceptionLog.txt"),strDir);
		strFileBackup.Format(_T("%s\\ExceptionLogHistory.txt"),strBackup);
		m_strOperateLog.Format(_T("%s\\OperateLog.txt"),strOperateDir);
		if (FALSE==PathFileExists(m_strOperateLog))
		{
			m_fileOperateLog.Open(m_strOperateLog,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone);
			m_fileOperateLog.Close();
		}

		if (PathFileExists(m_strExceptionFileName))
		{
			CString strTmp;
			CStdioFile fileBackup,fileException;
			fileBackup.Open(strFileBackup,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone|CFile::modeNoTruncate);
			fileException.Open(m_strExceptionFileName,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone|CFile::modeNoTruncate);
	
			while(fileException.ReadString(strTmp))
			{
				strTmp+= "\r\n";
				fileBackup.SeekToEnd();
				fileBackup.Write(strTmp.GetBuffer(strTmp.GetLength()),strTmp.GetLength());
				strTmp.ReleaseBuffer();

			}



			fileException.Close();
			fileBackup.Close();
			DeleteFile(m_strExceptionFileName);
		}

	
		

		m_fileException.Open(m_strExceptionFileName,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone);
		m_strTimeFileName.Format(_T("%s\\TimeLog.txt"),strDir);
		m_fileTime.Open(m_strTimeFileName,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone);
		m_fileTime.Close();
		m_fileException.Close();
	}catch(...)
	{

	}
}

//void CLog::InitRecordFile(CString strBatch)
//{
//	CString strDir,strDate;
//	CTime time = CTime::GetCurrentTime();
//	strDate.Format(_T("%d年%02d月\\%02d日"),time.GetYear(),time.GetMonth(),time.GetDay());
//	strDir.Format(_T("%s\\History\\%s\\%s"),GetCurrentAppPath(),strDate,strBatch);
//	CreateMultilevelPath(strDir);
//	try
//	{
//		m_strRecordFileName.Format(_T("%s\\DetetionRecord.txt"),strDir);
//		m_fileDataRecord.Open(m_strRecordFileName,CFile::modeReadWrite|CFile::modeCreate | CFile::typeBinary|CFile::shareDenyNone | CFile::modeNoTruncate);
//		m_fileDataRecord.Close();
//	}
//	catch (CFileException)
//	{
//	}
//}

CString CLog::GetCurrentAppPath()
{
	CString path; 
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 
	path.ReleaseBuffer(); 
	int pos = path.ReverseFind('\\'); 
	path = path.Left(pos);
	return path;
}

BOOL CLog::CreateMultilevelPath(CString &pstrFolder)
{
	char *p = pstrFolder.GetBuffer(pstrFolder.GetLength());
	pstrFolder.ReleaseBuffer();
	size_t i,len;
	len=strlen(p);
	for(i=0;i<len;i++)
	{
		if(p[i]=='\\')
		{
			p[i]='\0';
			_mkdir(p);
			p[i]='\\';
		}
	}
	if(len>0)
		_mkdir(p);
	p = NULL;
	return TRUE;
}

//void CLog::WriteRecord()
//{
//	//m_csRecord.Lock();
//	EnterCriticalSection(&m_csRecord);
//	try
//	{
//		CFile file;
//		CString strRecord;
//		file.Open(m_strRecordFileName,CFile::modeReadWrite| CFile::typeBinary|CFile::shareDenyNone|CFile::modeCreate|CFile::modeNoTruncate);
//		file.Seek(0,CFile::end);
//		//m_fileDataRecord.Open(m_strRecordFileName,CFile::modeReadWrite| CFile::typeBinary|CFile::shareDenyNone);
//		//m_fileDataRecord.Seek(0,CFile::end);
//		for(int i=0;i<g_vec_Record.size();i++)
//		{
//			strRecord = g_vec_Record[i];
//			file.Write(strRecord.GetBuffer(strRecord.GetLength()),strRecord.GetLength());
//			//m_fileDataRecord.Write(strRecord.GetBuffer(strRecord.GetLength()),strRecord.GetLength());
//		}
//		file.Close();
//		//m_fileDataRecord.Close();
//	}
//	catch(CFileException)
//	{
//
//	}
//
//	//m_csRecord.Unlock();
//	LeaveCriticalSection(&m_csRecord);
//}

