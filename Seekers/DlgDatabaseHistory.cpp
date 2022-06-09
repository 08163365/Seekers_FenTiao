// DlgDatabaseHistory.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgDatabaseHistory.h"
#include "afxdialogex.h"

int    WM_READIMAGE_OK= RegisterWindowMessage (_T("WM_READIMAGE_OK"));
int    WM_EXPORT_OK = RegisterWindowMessage(_T("WM_EXPORT_OK"));
// CDlgDatabaseHistory �Ի���

IMPLEMENT_DYNAMIC(CDlgDatabaseHistory, CDialogEx)

CDlgDatabaseHistory::CDlgDatabaseHistory(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDatabaseHistory::IDD, pParent)
	,m_pThreadReadImage(NULL)
{
	m_hoFrontImage.GenEmptyObj();
	m_hoBehindImage.GenEmptyObj();
}

CDlgDatabaseHistory::~CDlgDatabaseHistory()
{
}

void CDlgDatabaseHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATABASE, m_listDataBase);
	DDX_Control(pDX, IDC_LIST_BATCHINFO, m_listBatchInfo);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_TimeCtrl);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_BTN_cancel);
	DDX_Control(pDX, IDC_BUTTON_EXPORT, m_BTN_export);
	DDX_Control(pDX, IDC_BUTTON_GO, m_BTN_go);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgDatabaseHistory, CDialogEx)
	ON_REGISTERED_MESSAGE(WM_READIMAGE_OK,&CDlgDatabaseHistory::OnReadImageOK)
	ON_REGISTERED_MESSAGE(WM_EXPORT_OK,&CDlgDatabaseHistory::OnExportExcelOK)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgDatabaseHistory::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_GO, &CDlgDatabaseHistory::OnBnClickedButtonGo)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DATABASE, &CDlgDatabaseHistory::OnNMDblclkListDatabase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BATCHINFO, &CDlgDatabaseHistory::OnLvnItemchangedListBatchinfo)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CDlgDatabaseHistory::OnBnClickedButtonExport)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgDatabaseHistory::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgDatabaseHistory ��Ϣ�������


BOOL CDlgDatabaseHistory::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitUI();
	InitListCtl();

	SetCheck("~father");
	///////��������//
	SetWindowAttr("background_color","white");
	CRect rcImage;m_hvWindowIDFront,m_hvWindowIDBehind;
	GetDlgItem(IDC_PICTURE_FRONT)->GetClientRect(rcImage);
	OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),
		(Hlong)GetDlgItem(IDC_PICTURE_FRONT)->m_hWnd,"visible","",&m_hvWindowIDFront);
	GetDlgItem(IDC_PICTURE_BEHIND)->GetClientRect(rcImage);
	OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),
		(Hlong)GetDlgItem(IDC_PICTURE_BEHIND)->m_hWnd,"visible","",&m_hvWindowIDBehind);
	SetCheck("father");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgDatabaseHistory::InitUI()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	
	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//���ñ���ɫ ��7����ť
	m_BTN_go.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_go.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	//m_BTN_go.SetIcon(IDI_ICON_W_DETECT);
	m_BTN_go.SetFont(&m_ftButton);

	m_BTN_export.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_export.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	//m_BTN_export.SetIcon(IDI_ICON_W_DETECT);
	m_BTN_export.SetFont(&m_ftButton);

	m_BTN_cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	//m_BTN_cancel.SetIcon(IDI_ICON_W_LEAVE);
	m_BTN_cancel.SetFont(&m_ftButton);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
}

void CDlgDatabaseHistory::InitListCtl()
{
	m_fontList.CreatePointFont(90,"Microsoft YaHei");

	m_listDataBase.SetFont(&m_fontList);
	m_listDataBase.SetColorHeader(TRUE);
	m_listDataBase.SetHeaderHeight(1.5);
	m_listDataBase.SetHeaderBKColor(255,255,255,0);
	m_listDataBase.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS|LVS_EX_SUBITEMIMAGES);

	m_listBatchInfo.SetFont(&m_fontList);
	m_listBatchInfo.SetColorHeader(TRUE);
	m_listBatchInfo.SetHeaderHeight(1.5);
	m_listBatchInfo.SetHeaderBKColor(255,255,255,0);
	m_listBatchInfo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS|LVS_EX_SUBITEMIMAGES);
	
	CString strColumnHeader;
	strColumnHeader.Format("Ʒ��,100;���,100;��λ,80;��ʼʱ��,100;ֹͣʱ��,100;����,100;��Ʒ��,100;������,100;");
	for (int i=0;i<g_Count.nType;i++)
	{
		CString strTemp;
		strTemp.Format("%s;%s,100",strColumnHeader,g_Count.strAClassName.GetAt(i));
		strColumnHeader.Format("%s",strTemp);
	}
	m_listDataBase.SetColumnHeader(strColumnHeader);

	m_listBatchInfo.SetColumnHeader("Ƭ��,50;����,100");
}

void T_ExportExcel(CDlgDatabaseHistory* pDlg)
{
	try
	{
		CString strFile;

		SYSTEMTIME CurrentSystime;
		GetLocalTime(&CurrentSystime);
		//����ͼƬ
		CString strPath;
		CString strFileName; 
		strFileName.Format("%dʱ%d��%d��.xlsx",CurrentSystime.wHour,CurrentSystime.wMinute,CurrentSystime.wSecond);
		CreateDirectory("D:\\SaveXlsx",NULL);
		strPath.Format("D:\\SaveXlsx\\%d��%d��\\",CurrentSystime.wMonth,CurrentSystime.wDay);
		CreateDirectory(strPath,NULL);
		strFile.Format("%s%s",strPath,strFileName);

		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,0);

		COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR); 
		CApplication app;
		CWorkbook book;
		CWorkbooks books;
		CWorksheet sheet;
		CWorksheets sheets;
		CRange range;
		if (!app.CreateDispatch(_T("Excel.Application")))
		{
			AfxMessageBox(_T("Error!Creat Excel Application Server Faile!"));
		}
		books = app.get_Workbooks();
		book = books.Add(covOptional);
		sheets=book.get_Worksheets();
		sheet = sheets.get_Item(COleVariant((short)1));
		range = sheet.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1"))); 
		range.put_ColumnWidth(_variant_t(18.38));
		//��������ڶ��е�ǰʮ����Ԫ��������1��10��ʮ������
		//��ȡ��ͷ
		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,10);
		for (int i = 0;i<pDlg->m_listDataBase.GetColumnCount();i++)
		{
			CString strColumnText = pDlg->m_listDataBase.GetHeaderText(i);
			range.put_Item(_variant_t((long)1),_variant_t((long)i+1),_variant_t(strColumnText));
		}


		
		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,30);

		for (int i = 0;i<pDlg->m_listDataBase.GetItemCount();i++)
		{
			for (int j = 0;j<pDlg->m_listDataBase.GetColumnCount();j++)
			{
				CString strItemText = pDlg->m_listDataBase.GetItemText(i,j);
				range.put_Item(_variant_t((long)2+i),_variant_t((long)j+1),_variant_t(strItemText));
			}
		}
		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,50);

		range = sheet.get_Range(COleVariant("B1"),COleVariant("F1"));
		range.put_ColumnWidth(_variant_t(13.38));

		//����
		book.SaveCopyAs(COleVariant(strFile)); 
		book.put_Saved(true);
		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,70);
		//��β���ͷ�
		range.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();   
		books.ReleaseDispatch();  
		book.Close(covOptional,covOptional,covOptional);//�ر�Workbook����
		books.Close();  //�ر�Workbooks����
		app.ReleaseDispatch();
		app.Quit(); 

		PostMessage(pDlg->m_hWnd,WM_EXPORT_OK,0,100);
		CString strMessageBox;
		strMessageBox.Format("�����ɹ�!\r\n�ļ���:%s\r\n",strFile);
		AfxMessageBox(strMessageBox);


	}catch(HException e)
	{
		AfxMessageBox("����ͼƬʧ��");
	}

}
void T_ReadImage(CDlgDatabaseHistory* pDlg)
{
	POSITION pos = pDlg->m_listBatchInfo.GetFirstSelectedItemPosition();
	int nSelIndex = 0 ;
	if(pos)   
	{   
		nSelIndex = pDlg->m_listBatchInfo.GetNextSelectedItem(pos); 
	}
	CString strIndex = pDlg->m_listBatchInfo.GetItemText(nSelIndex,0);
	CString strImageFile;
	strImageFile.Format("%s\\����\\%s.hobj",pDlg->m_strFilePath,strIndex);
	try
	{
		ReadObject(&pDlg->m_hoFrontImage,HTuple(strImageFile));
	}catch(HException e){}
	strImageFile.Format("%s\\����\\%s.hobj",pDlg->m_strFilePath,strIndex);
	try
	{
		ReadObject(&pDlg->m_hoBehindImage,HTuple(strImageFile));
	}catch(HException e){}
	pDlg->SendMessage(WM_READIMAGE_OK);
	AfxEndThread(1);
	
}
LRESULT CDlgDatabaseHistory::OnExportExcelOK(WPARAM wParam, LPARAM lParam)
{
	ClearWindow(m_hvWindowIDFront);
	switch(lParam)
	{
	case 0:
		{
			disp_message(m_hvWindowIDFront,"1%:��ʼ����Excel����","window",-1,-1,"red","true");
			break;
		}
	case 10:
		{
			disp_message(m_hvWindowIDFront,"10%:��ʼ������ͷ","window",-1,-1,"red","true");
			break;
		}
	case 20:
		{
			disp_message(m_hvWindowIDFront,"30%:��ʼ��������","window",-1,-1,"red","true");
			break;
		}
	case 50:
		{
			disp_message(m_hvWindowIDFront,"50%:��ʼд��Ӳ��","window",-1,-1,"red","true");
			break;
		}
	case 70:
		{
			disp_message(m_hvWindowIDFront,"70%:������ʱ����","window",-1,-1,"red","true");
			break;
		}
	}
	
	return 0;
}
LRESULT CDlgDatabaseHistory::OnReadImageOK(WPARAM wParam, LPARAM lParam)
{
	m_pThreadReadImage = NULL;


	HObject ObjectSelected;
	HTuple Width,Height;
	try
	{
		ClearWindow(m_hvWindowIDFront);
		SelectObj(m_hoFrontImage,&ObjectSelected,1);
		GetImageSize(ObjectSelected,&Width,&Height);
		SetPart(m_hvWindowIDFront,0,0,Height-1,Width-1);
		DispObj(ObjectSelected,m_hvWindowIDFront);
	}catch(HException e){}
	try
	{
		ClearWindow(m_hvWindowIDBehind);
		SelectObj(m_hoBehindImage,&ObjectSelected,1);
		GetImageSize(ObjectSelected,&Width,&Height);
		SetPart(m_hvWindowIDBehind,0,0,Height-1,Width-1);
		DispObj(ObjectSelected,m_hvWindowIDBehind);
	}catch(HException e){}
	return 0;
}
void CDlgDatabaseHistory::OnBnClickedButtonCancel()
{
	CDialogEx::OnCancel();
}


void CDlgDatabaseHistory::OnBnClickedButtonGo()
{
	ReadCount();

	//if (0 == g_nDatabaseType)
	{
		return;
	}
	try
	{
		CString strTime;
		CTime TimeCur;
		m_TimeCtrl.GetTime(TimeCur);
		strTime = TimeCur.Format("%Y-%m-%d");
		CString strSQL;
		CString strTableName("batch");
		strSQL.Format("SELECT * FROM sysobjects WHERE xtype = 'u' AND name='%s'",strTableName);//��ѯ�ߴ���Ƿ������������ھʹ���
		_RecordsetPtr pset = g_AdoDatabase.GetRecordSet(_bstr_t(strSQL));
		if (0 == pset->GetRecordCount())
		{
			//�����ɶ�Ӧ��
			strSQL.Format("CREATE TABLE [dbo].[batch]("
				"[BATCH] [varchar](255) NOT NULL,"
				"[PRODUCTNAME] [varchar](255) NOT NULL,"
				"[FRAMECOUNT] [int] NOT NULL,"
				"[ERRORCOUNT] [int] NOT NULL,"
				"[YIELD] [float] NOT NULL,"
				"[TIME] [date] NOT NULL,");
			for (int i=0;i<g_Count.nType;i++)
			{
				CString strTemp;
				strTemp.Format("%s[EC%d] [int],",strSQL,i+1);
				strSQL.Format("%s",strTemp);
			}
			strSQL +=") ON [PRIMARY]";
			g_AdoDatabase.ExecuteSQL((_bstr_t)strSQL);
		}

		strSQL.Format("SELECT * "
			"FROM [dahengimage].[dbo].[batch] WHERE [TIME] = '%s'",strTime);
		pset = g_AdoDatabase.GetRecordSet(_bstr_t(strSQL));
		m_listDataBase.DeleteAllItems();
		if(!(pset->adoBOF))//����¼��ָ��δָ�����ݿ��е���Ԫ��  
		{  
			pset->MoveFirst();//����ָ����Ԫ��λ�� 
			m_listDataBase.DeleteAllItems();
			while (!pset->adoEOF)//����¼��ָ��δָ�����ݿ��е�βԪ�أ���ѭ����Զ�������  
			{  
				int nItem = 0;
				m_listDataBase.InsertItem(nItem,(char*)(_bstr_t)pset->GetCollect("BATCH"));
				m_listDataBase.SetItemText(nItem,1,(char*)(_bstr_t)pset->GetCollect("PRODUCTNAME"));
				m_listDataBase.SetItemText(nItem,2,(char*)(_bstr_t)pset->GetCollect("FRAMECOUNT"));
				m_listDataBase.SetItemText(nItem,3,(char*)(_bstr_t)pset->GetCollect("ERRORCOUNT"));
				m_listDataBase.SetItemText(nItem,4,(char*)(_bstr_t)pset->GetCollect("YIELD"));
				m_listDataBase.SetItemText(nItem,5,(char*)(_bstr_t)pset->GetCollect("TIME"));
				for (int i=0;i<g_Count.nType;i++)
				{
					CString strCollect;
					strCollect.Format("EC%d",i+1);
					try
					{
						m_listDataBase.SetItemText(nItem,6+i,(char*)(_bstr_t)pset->GetCollect((_bstr_t)strCollect));
					}catch(_com_error e)
					{
						m_listDataBase.SetItemText(nItem,6+i,0);
					}
					
				}
				pset->MoveNext();  
			}

		}

	}
	catch(_com_error e)
	{
		CString strException;
		strException.Format("%s",e.ErrorMessage());
		g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
	}
}
void CDlgDatabaseHistory::ReadBatchInfo()
{
	POSITION pos = m_listDataBase.GetFirstSelectedItemPosition();
	int nSelIndex = 0 ;
	if(pos)   
	{   
		nSelIndex = m_listDataBase.GetNextSelectedItem(pos); 
	}
	CString strBatch = m_listDataBase.GetItemText(nSelIndex,0);
	CString strProduct = m_listDataBase.GetItemText(nSelIndex,1);
	//CString strFrameCount = m_listDataBase.GetItemText(nSelIndex,2);
	int nFrameCount = 0;

	CString strTime;
	CTime TimeCur;
	m_TimeCtrl.GetTime(TimeCur);
	strTime = TimeCur.Format("%Y��%m��\\%d��");
	m_strFilePath.Format("%s\\Map\\%s\\%s\\%s",GetCurrentAppPath(),strTime,strBatch,strProduct);
	HTuple IndexAllError;//���д��ڴ�������
	{//��¼�д��������
		HTuple IndexFront,IndexBehind;//����
		CString strIndex;
		strIndex.Format("%s\\����\\Index",m_strFilePath);
		try
		{
			ReadTuple(HTuple(strIndex),&IndexFront);
		}catch(HException)
		{
			IndexFront.Clear();
		}
		strIndex.Format("%s\\����\\Index",m_strFilePath);
		try
		{
			ReadTuple(HTuple(strIndex),&IndexBehind);
		}catch(HException)
		{
			IndexBehind.Clear();
		}
		nFrameCount = MIN(IndexFront.Length(),IndexBehind.Length());
		TupleConcat(IndexFront,IndexBehind,&IndexAllError);
		TupleExceptRepeat(IndexAllError,&IndexAllError);//�ۺ��˸����������Ժ������
	}
	HTuple Indices;
	m_listBatchInfo.DeleteAllItems();
	for (int i=0;i<MIN(1000,nFrameCount);i++)
	{
		int nItemIndex = m_listBatchInfo.GetItemCount();
		CString strSN;
		strSN.Format("%d",nItemIndex+1);
		m_listBatchInfo.InsertItem(nItemIndex,"");
		m_listBatchInfo.SetItemText(nItemIndex,0,strSN);
		m_listBatchInfo.SetItemText(nItemIndex,1,"");
		TupleFind(IndexAllError,i+1,&Indices);
		if (-1 == Indices || 0 == Indices.Length())//���û����
		{
			m_listBatchInfo.SetItemBkColor(nItemIndex,1,RGB(0,255,0),FALSE);
		}else
		{
			m_listBatchInfo.SetItemBkColor(nItemIndex,1,RGB(255,0,0),FALSE);
		}
	}
}

void CDlgDatabaseHistory::OnNMDblclkListDatabase(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	ReadBatchInfo();
	*pResult = 0;
}
void CDlgDatabaseHistory::OnLvnItemchangedListBatchinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	
	if (pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
	{
		if(NULL == m_pThreadReadImage)
		{
			m_pThreadReadImage = AfxBeginThread((AFX_THREADPROC)T_ReadImage,this);
		}
	}
	
}
void CDlgDatabaseHistory::OnBnClickedButtonExport()
{
	AfxBeginThread((AFX_THREADPROC)T_ExportExcel,this);
}


void CDlgDatabaseHistory::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient,rcCtl;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//���߿���
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);

	GetDlgItem(IDC_LIST_DATABASE)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_LIST_BATCHINFO)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_PICTURE_FRONT)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_PICTURE_BEHIND)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
}


BOOL CDlgDatabaseHistory::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgDatabaseHistory::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgDatabaseHistory::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*90)/100,LWA_ALPHA);
	}else if (WA_CLICKACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}else if (WA_ACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
}


LRESULT CDlgDatabaseHistory::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgDatabaseHistory::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDlgDatabaseHistory::ReadCount()
{
	CTime TimeCur;
	m_TimeCtrl.GetTime(TimeCur);
	m_listDataBase.DeleteAllItems();
	CString strSizeRecord;
	if(g_strSaveFullPath=="")
	{
		strSizeRecord.Format("%s\\%d��%02d��覴�������ܼ�¼.csv",g_strSaveBasePath, TimeCur.GetYear(), TimeCur.GetMonth());
	}else
	{
		int nPosition =g_strSaveFullPath.Find("-");
		strSizeRecord.Format("%s\\%d��%02d��覴�������ܼ�¼.csv",g_strSaveBasePath.Left(nPosition+2),TimeCur.GetYear(), TimeCur.GetMonth());

	}




	if (PathFileExists(strSizeRecord))
	{
		try
		{
			CStdioFile file;
			file.Open(strSizeRecord, CFile::modeRead);
			CString strLine(_T(""));
			//�ҳ��ַ��������С�:���͡�,��֮�������
			int nCount = 0;
			while (file.ReadString(strLine))
			{
				if (!strLine.IsEmpty())
				{
					nCount++;
					if (1 >= nCount)
					{
						continue;
					}
					CString strValue;
					int nPos1, nPos2, nIndex, i;
					nPos1 = strLine.Find(',');
					if (nPos1 != 1)
					{
						nIndex = m_listDataBase.InsertItem(m_listDataBase.GetItemCount(), "");
						i = 0;
					}
					else
					{
						continue;
					}
					m_listDataBase.SetItemText(nIndex, i++, strLine.Left(nPos1));
					nPos2 = nPos1;
					nPos1 = strLine.Find(',', nPos2 + 1);
					while (nPos1 != -1)
					{
						strValue = strLine.Mid(nPos2 + 1, nPos1 - nPos2 - 1);
						m_listDataBase.SetItemText(nIndex, i++, strValue);
						nPos2 = nPos1;
						nPos1 = strLine.Find(',', nPos2 + 1);
					}
				}
			}
		}
		catch (CFileException* e)
		{
			e->ReportError();
			e->Delete();
		}
	}
}

