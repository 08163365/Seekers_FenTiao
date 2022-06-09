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
		//����Connection����
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
		m_strLastErrorText.Format("Warning: �������ݿⷢ���쳣. ������Ϣ: %s;", e.ErrorMessage());
		return FALSE;
	}
}

_RecordsetPtr& CADOConnect::GetRecordSet(_bstr_t bstrSQL)
{
	///////////////////////////////////////////////////////////
	try
	{
		//�������ݿ⣬���Connection����Ϊ�գ��������������ݿ�
		if (m_pConnection==NULL || m_pRecordset == NULL)
		{
			m_pRecordset=NULL; 
			m_strLastErrorText.Format("Warning: �������ݿⷢ���쳣");
			return m_pRecordset;
		}
		//////////////////////////////////////////////
		if (m_pRecordset->State == adStateOpen)  
		{
			m_pRecordset->Close();
		}
		m_pRecordset->CursorLocation=adUseClient;  //Ϊ����ʹ��Sort����

		//////////////////////////////////////////////
		//ȡ�ñ��еļ�¼        
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenKeyset,
			adLockOptimistic,adCmdText);
		//ʹ�þ�̬�����adOpenKeyset�α귵��ʵ�ʼ��������ڶ�̬�α�ȡ��������Դ����-1��ʵ�ʼ�������
		//���ڻ��ɼ����α���Ϊ�˳�����ʹ��GetRecordsetCount()����
		//���ü������ļ�¼�������������û���������ɾ�������������ڸ���ԭ�м�¼�Ĳ���
		//�ǿɼ���
		//���þ�̬��꣬��Ϊ��¼������һ����̬���ݣ��������û���������ɾ�������²���
		//���ڼ�¼����˵�ǲ��ɼ���
	}
	catch (_com_error e)
	{
		m_pRecordset = NULL;
		m_strLastErrorText.Format("Warning: �򿪼�¼�������쳣. ������Ϣ: %s;", e.ErrorMessage());
	}

	return  m_pRecordset;
}

BOOL CADOConnect::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		//�Ƿ��Ѿ����ӵ����ݿ�
		if (m_pConnection==NULL)
		{
			m_strLastErrorText.Format("Warning: ExecuteSQL ���������쳣");
			return FALSE;
		}
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return TRUE;
	}
	catch (_com_error e)
	{
		m_strLastErrorText.Format("Warning: ExecuteSQL ���������쳣. ������Ϣ: %s;", e.ErrorMessage());
		return FALSE;
	}
}

void CADOConnect::ExitConnect()
{
	try
	{
		//�رռ�¼��������
		if (m_pRecordset!=NULL && m_pRecordset->State==adStateOpen )  //�Ƿ��ڴ�״̬	2007.9.6
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
Name:	ȡ��������Ĵ�����Ϣ.
==========================================================================*/
CString CADOConnect::GetLastErrorText()
{
	return m_strLastErrorText;
}