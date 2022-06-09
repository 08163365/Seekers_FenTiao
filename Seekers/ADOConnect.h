// ADOConnect.h: interface for the CADOConnect class.
//
//////////////////////////////////////////////////////////////////////2007.8.12
/////////////////////////////////////////////////////////////////////////////////////
/*strSQL.Format("INSERT INTO [dahengimage].[dbo].[size]"
	"([BATCH],[PRODUCTNAME],[FRAMESN],[TIME],[SIZE1],[SIZE2],[SIZE3],[SIZE4],[SIZE5],[SIZE6],[SIZE7])"
	"VALUES"
	"('12345',12345,12345,GETDATE(),'12345',12345,12345,12345,12345,12345,12345)");
*/
/*strSQL.Format("CREATE TABLE [dbo].[size]("
"[BATCH] [varchar](255) NOT NULL,"
"[PRODUCTNAME] [varchar](255) NOT NULL,"
"[FRAMESN] [int] NOT NULL,"
"[TIME] [date] NOT NULL,"
"[SIZE1] [float] NOT NULL,"
"[SIZE2] [float] NOT NULL,"
"[SIZE3] [float] NOT NULL,"
"[SIZE4] [float] NOT NULL,"
"[SIZE5] [float] NOT NULL,"
"[SIZE6] [float] NOT NULL,"
"[SIZE7] [float] NOT NULL"
") ON [PRIMARY]");*/
#if !defined(AFX_ADOCONNECT_H__B91835B8_2FD1_49AB_AEDB_FEDEF3B34A6D__INCLUDED_)
#define AFX_ADOCONNECT_H__B91835B8_2FD1_49AB_AEDB_FEDEF3B34A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF") rename("Connection","adoConnection")

class CADOConnect  
{
public:
	//���һ��ָ��Connection�����ָ��
	_ConnectionPtr  m_pConnection;
	//���һ��ָ��Recordset�����ָ��
	_RecordsetPtr   m_pRecordset;

	//��ʼ�����������ݿ�
	BOOL OnInitDBConnect(CString strConnect);
	//ִ�в�ѯ
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	//ִ��SQL���
	BOOL  ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();

	// �쳣��Ϣ ----------------------------------
	CString GetLastErrorText();


public:
	CADOConnect();
	virtual ~CADOConnect();
private:
	CString m_strLastErrorText;
};

#endif // !defined(AFX_ADOCONNECT_H__B91835B8_2FD1_49AB_AEDB_FEDEF3B34A6D__INCLUDED_)
