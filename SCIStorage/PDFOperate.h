#pragma once
#include "pdflib.hpp"
using namespace std;
using namespace pdflib;
class CPDFOperate
{
public:
	CPDFOperate(void);
	~CPDFOperate(void);
public:
	void Init();
	BOOL CreatePDF(CString strPDFName);
	wstring s2ws( const std::string& s );
	void MergeCells(int nRowIndex,int nColumnIndex,int nMergerColumnNum,CString CellTextInfo,BOOL bHeadFlag=FALSE);
	void SetCellInfo(int nRowIndex,int nColumnIndex,CString CellTextInfo);
	void SetCellImage(int nRowIndex,int nColumnIndex,CString StrImageFile);//º”‘ÿÕºœÒ

	void AddMultText(int nRowIndex,int nColumnIndex,int MergerNum,CString TextInfo);
	void ClosePDF();//πÿ±’PDF
public:
	PDFlib  m_PDF;
    wstring m_searchpath;
	wostringstream  m_optlist;
	int  m_nfont;
	int m_tbl;
};

