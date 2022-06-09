#pragma once
#include "DlgException.h"

class CriticalSectionHelper
{
public:
	CriticalSectionHelper(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}

	~CriticalSectionHelper()
	{
		LeaveCriticalSection(m_pcs);
		m_pcs = NULL;
	}

private:
	CRITICAL_SECTION *m_pcs;
};
class CLog : public DHInterface
{
public:
	CLog(void);

	~CLog(void);

	virtual int DoSomething(void* message,void* wParam,void* lParam);

	//void InitRecordFile(CString strBatch);
	void Loging(CString strTimeLog);
	void TimeLoging(CString strTimeLog);
	void Exceptionloging(CString strExceptionlog);
	void WriteOperateLog(CString strOperateLog);
	void ReadException(CStringArray* strArrayExcepion);

	//void WriteRecord();
public:

	BOOL m_bSoftExceptionStauts;
	BOOL m_bWriteTimeLog;
	BOOL m_bShowException;
	BOOL m_bDlgCreated;
	CString m_strBatch;
	CString m_strOperatePath;
	CRITICAL_SECTION  m_OperateChange;
	CString m_strGongweiName; 
private:
	
	void Create();

	CString GetCurrentAppPath();

	BOOL CreateMultilevelPath(CString &pstrFolder);

	CDlgException *m_pDlgException;

	CRITICAL_SECTION m_csTime;
	CRITICAL_SECTION m_csException;
	CRITICAL_SECTION m_csRecord;



	CStdioFile m_fileException;
	CStdioFile m_fileTime;
	CStdioFile m_fileDataRecord;

	CString m_strExceptionFileName;
	CString m_strTimeFileName;
	CString m_strRecordFileName;

	CStdioFile m_fileOperateLog;
	CString m_strOperateLog;
};

