// ADOConnect.cpp: implementation of the CADOConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADOConnect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CADOConnect::CADOConnect()
{
	m_pRecordset = NULL;
	m_pConnection = NULL;
	m_strLastErrorText.Empty();
}

CADOConnect::~CADOConnect()
{
	try
	{
		if(adStateOpen == m_pConnection->State)
		{
			m_pConnection->Close();
		}
		if(adStateOpen == m_pRecordset->State)
		{
			m_pRecordset->Close();
		}
	}catch (_com_error e)
	{
		
	}
	
}

BOOL CADOConnect::OnInitDBConnect(CString strConnect)
{
	try
	{
		//创建Connection对象
		if(!SUCCEEDED(m_pConnection.CreateInstance(__uuidof(adoConnection))))
		{
			m_pConnection = NULL;
		}
		if(!SUCCEEDED(m_pRecordset.CreateInstance(__uuidof(Recordset))))
		{ 
			m_pRecordset = NULL;
		}
		ASSERT(m_pConnection != NULL);
		ASSERT(m_pRecordset != NULL);
		m_pConnection->Open(_bstr_t(strConnect),"","",adModeUnknown);
		return TRUE;
	}catch (_com_error e)
	{
		m_strLastErrorText.Format("Warning: 连接数据库发生异常. 错误信息: %s;", e.ErrorMessage());
		return FALSE;
	}
}

_RecordsetPtr& CADOConnect::GetRecordSet(_bstr_t bstrSQL)
{
	///////////////////////////////////////////////////////////
	try
	{
		//连接数据库，如果Connection对象为空，则重新连接数据库
		if (m_pConnection==NULL || m_pRecordset == NULL)
		{
			m_pRecordset=NULL; 
			m_strLastErrorText.Format("Warning: 连接数据库发生异常");
			return m_pRecordset;
		}
		//////////////////////////////////////////////
		if (m_pRecordset->State == adStateOpen)  
		{
			m_pRecordset->Close();
		}
		m_pRecordset->CursorLocation=adUseClient;  //为了在使用Sort属性

		//////////////////////////////////////////////
		//取得表中的记录        
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenKeyset,
			adLockOptimistic,adCmdText);
		//使用静态或键集adOpenKeyset游标返回实际计数，对于动态游标取决于数据源返回-1或实际计数。。
		//现在换成键集游标是为了程序中使用GetRecordsetCount()函数
		//采用键集光标的记录集看不到其他用户的新增、删除操作，但对于更新原有记录的操作
		//是可见的
		//采用静态光标，它为记录集产生一个静态备份，但其他用户的新增、删除、更新操作
		//对于记录集来说是不可见的
	}
	catch (_com_error e)
	{
		m_pRecordset = NULL;
		m_strLastErrorText.Format("Warning: 打开记录集发生异常. 错误信息: %s;", e.ErrorMessage());
	}

	return  m_pRecordset;
}

BOOL CADOConnect::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		//是否已经连接到数据库
		if (m_pConnection==NULL)
		{
			m_strLastErrorText.Format("Warning: ExecuteSQL 方法发生异常");
			return FALSE;
		}
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return TRUE;
	}
	catch (_com_error e)
	{
		m_strLastErrorText.Format("Warning: ExecuteSQL 方法发生异常. 错误信息: %s;", e.ErrorMessage());
		return FALSE;
	}
}

void CADOConnect::ExitConnect()
{
	try
	{
		//关闭记录集和连接
		if (m_pRecordset!=NULL && m_pRecordset->State==adStateOpen )  //是否处于打开状态	2007.9.6
		{
			m_pRecordset->Close();
			m_pRecordset.Release();
			m_pRecordset=NULL;
		}
		m_pConnection->Close();
		m_pConnection.Release();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

/*========================================================================
Name:	取得最后发生的错误信息.
==========================================================================*/
CString CADOConnect::GetLastErrorText()
{
	return m_strLastErrorText;
}