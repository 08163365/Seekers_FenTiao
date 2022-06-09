#include "StdAfx.h"
#include "PDFOperate.h"


CPDFOperate::CPDFOperate(void)
{
}


CPDFOperate::~CPDFOperate(void)
{
}

wstring CPDFOperate::s2ws( const std::string& s )
{

	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();

	size_t _Dsize = s.size() + 1;

	wchar_t *_Dest = new wchar_t[_Dsize];

	wmemset(_Dest, 0, _Dsize);

	mbstowcs(_Dest,_Source,_Dsize);

	std::wstring result = _Dest;

	delete []_Dest;

	setlocale(LC_ALL, "C");

	return result;

}



void CPDFOperate::Init()
{
	CString strPath;
	strPath.Format("%s\\Configdata",GetCurrentAppPath());
	m_searchpath = s2ws(strPath.GetBuffer());
	//m_searchpath = L"../data";
	m_tbl=-1;
	
	
}
BOOL CPDFOperate::CreatePDF(CString strPDFName)
{
	m_optlist.str(L"");
	m_optlist << L"searchpath={{" << m_searchpath << L"}}";
	m_PDF.set_option(m_optlist.str());
	if (m_PDF.begin_document(s2ws(strPDFName.GetBuffer()), L"") == -1)
	{
		
		return FALSE ;
	}
	m_PDF.set_info(L"Creator", L"PDFlib starter sample");
	m_PDF.set_info(L"Title", L"starter_table");
	m_nfont = m_PDF.load_font(L"simfang", L"winansi", L"");
	return TRUE;
}
void CPDFOperate::MergeCells(int nRowIndex,int nColumnIndex,int nMergerColumnNum,CString CellTextInfo, BOOL bHeadFlag)
{
	
	m_optlist.str(L"");
	if (bHeadFlag)//БъЬт
	{
		m_optlist << L"fittextline={position=center font=" << m_nfont
			<< L" fontsize=14} colspan=" << nMergerColumnNum;
	}else
	{
		m_optlist << L"fittextline={position={left center} font=" << m_nfont
			<< L" fontsize=14} colspan=" << nMergerColumnNum;
	}

	wstring strInfo=s2ws(CellTextInfo.GetBuffer());



	m_tbl = m_PDF.add_table_cell(m_tbl, nColumnIndex, nRowIndex,strInfo, m_optlist.str());

}
void CPDFOperate::AddMultText(int nRowIndex,int nColumnIndex,int MergerNum,CString TextInfo)
{
	int tf=-1;

	tf = m_PDF.add_textflow(tf, s2ws(TextInfo.GetBuffer()),L"charref fontname=simfang encoding=winansi fontsize=14");
	if (tf == -1) 
	{

	}
	m_optlist.str(L"");
	m_optlist << L"fittextline={position=center font=" << m_nfont
		<< L" fontsize=14} colspan=" << MergerNum<<" ";

	m_optlist << L"marginleft=1 marginright=1 margintop=1 marginbottom=1 "L"textflow=" << tf;
	m_tbl = m_PDF.add_table_cell(m_tbl, nColumnIndex, nRowIndex, L"", m_optlist.str());
	if (m_tbl == -1) 
	{

	}
}


void CPDFOperate::SetCellInfo(int nRowIndex,int nColumnIndex,CString CellTextInfo)
{
	m_optlist.str(L"");			
	m_optlist << L"colwidth=20% fittextline={font=" << m_nfont<<  L" fontsize=14}";
	m_tbl = m_PDF.add_table_cell(m_tbl, nColumnIndex, nRowIndex, s2ws(CellTextInfo.GetBuffer()), m_optlist.str());
}



void CPDFOperate::SetCellImage(int nRowIndex,int nColumnIndex,CString StrImageFile)
{



	int nimage = m_PDF.load_image(L"auto",  s2ws(StrImageFile.GetBuffer()), L"");
	//if (nimage == -1) 
	//{

	//}

	m_optlist.str(L"");
	m_optlist<<L"image=" << nimage << L" rowheight=100 fitimage={orientate=north }";
	m_tbl = m_PDF.add_table_cell(m_tbl, nColumnIndex, nRowIndex, L"", m_optlist.str());
}





void CPDFOperate::ClosePDF()
{
	wstring nresult;
	do 
	{
		double llx= 10, lly=10, urx=550, ury=800;
		m_PDF.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
	   nresult = m_PDF.fit_table(m_tbl, llx, lly, urx, ury, 
			L"header=1 rowheightdefault=auto "
			L"fill={{area=rowodd fillcolor={gray 0.9}}} "
			L"stroke={{line=other}}");
		
		m_PDF.end_page_ext(L"");
	} while (nresult == L"_boxfull");
	m_PDF.delete_table(m_tbl, L"");
	m_PDF.end_document(L"");
}