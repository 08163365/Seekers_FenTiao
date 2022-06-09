#pragma once
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
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
	int     nSerachType;//0:正常膨胀区域，1:全图 2:全列 3://全行
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

	BOOL Init(int nID);//初始化
	BOOL Save();//保存
	BOOL Find(HObject ho_Image);//匹配
	void ShowDialog(HObject ho_Image);//调对话框


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

