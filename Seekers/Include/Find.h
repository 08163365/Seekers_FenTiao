#pragma once
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //�������ʱʹ��
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //��̬��Ӧ��ʱʹ��
#endif

#include "HalconCpp.h"
#include <vector>
using namespace HalconCpp;
struct Mark
{
	void Clear()
	{
		strName = "";
		ho_Search.Clear();
		ho_Mark.Clear();
		try
		{
			ClearShapeModel(hv_ID);
		}catch(HException ){}
		try
		{
			ClearNccModel(hv_ID);
		}catch(HException ){}
		hv_ID.Clear();
		nType=-1;
		nSerachType=0;
	}
	CString strName;
	HObject ho_Search;
	HObject ho_Mark;
	HTuple  hv_ID;
	int     nType;
	int     nSerachType;//0:������������1:ȫͼ 2:ȫ�� 3://ȫ��
};
typedef std::vector<Mark> MARKLIST;
struct MarkResult
{
	void Clear()
	{
		 hv_ID.Clear();
		 Row.Clear();
		 Column.Clear();
		 Angle.Clear();
		 Score.Clear();
	}
	HTuple hv_ID;
	HTuple Row;
	HTuple Column;
	HTuple Angle;
	HTuple Score;
};
class CLASS_DECLSPEC CFindMark
{
public:
	CFindMark(void);
	~CFindMark(void);

	BOOL Init(int nID);//��ʼ��
	BOOL Save();//����
	BOOL Find(HObject ho_Image);//ƥ��
	void ShowDialog(HObject ho_Image);//���Ի���


	int  GetMarkCount();
	BOOL GetMark(int nSN,Mark* pMark);
	BOOL GetMark(CString strMarkName,Mark* pMark);

	BOOL GetResult(int nSN,MarkResult* pResult);
	BOOL GetResult(CString strMarkName,MarkResult* pResult);
	BOOL AddMark(const Mark& MarkAdd);
	BOOL DeleteMark(const Mark& MarkDelete);
	CString GetLastErrorMessage();
private:
	BOOL ReadMark();
	BOOL WriteMark();
	void ClearMark();
	CString m_strPath;
	int  m_nID;
	MARKLIST m_MarkList;
	CString m_strLastErrorMessage;
	MarkResult m_Result;



};

