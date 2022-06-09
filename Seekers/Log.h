#pragma once
#include "stdafx.h"
class CLog
{
public:
	CLog(void);
	~CLog(void);
public:
	void TimeLoging(CString strTimeLog);
	void Exceptionloging(CString strExceptionlog);
	void ReadException(CStringArray* strArrayExcepion);
public:
	BOOL m_bWriteTimeLog;
private:
	void Create();
	CCriticalSection m_csTime;
	CCriticalSection m_csException;

	HTuple m_hvExceptionFile;
	HTuple m_hvTimeFile;

	CString m_strExceptionFileName;
	CString m_strTimeFileName;

};

