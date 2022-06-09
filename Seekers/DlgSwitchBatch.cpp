// DlgSwitchBatch.cpp : 实现文件
//

#include "stdafx.h"
//#include <iostream>  
//#include <string>  
//#include <vector>  
//#include <fstream>  
//#include <sstream>  

#include "Seekers.h"
#include "DlgSwitchBatch.h"
#include "afxdialogex.h"
#include "SeekersDlg.h"
#include"tlhelp32.h"
// CDlgSwitchBatch 对话框

IMPLEMENT_DYNAMIC(CDlgSwitchBatch, CDialogEx)

CDlgSwitchBatch::CDlgSwitchBatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSwitchBatch::IDD, pParent)
	, m_strNewBatch(_T(""))
{
}

CDlgSwitchBatch::~CDlgSwitchBatch()
{
}

void CDlgSwitchBatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CURRENT_BATCH, m_cstaticCurrentBatch);
	DDX_Control(pDX, IDC_STATIC_C_NEWBATCH, m_cstaticNewBatch);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Text(pDX, IDC_EDIT_NEWBATCH, m_strNewBatch);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_EDIT_NEWBATCH, m_EDIT_newBatch);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
}


BEGIN_MESSAGE_MAP(CDlgSwitchBatch, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgSwitchBatch::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgSwitchBatch::OnBnClickedButtonCancel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgSwitchBatch::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgSwitchBatch 消息处理程序


BOOL CDlgSwitchBatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitStatic();

	m_strNewBatch.Format("%s",g_strBatch);
	UpdateData(FALSE);
	((CEdit*)GetDlgItem(IDC_EDIT_NEWBATCH))->SetFocus();
	((CEdit*)GetDlgItem(IDC_EDIT_NEWBATCH))->SetSel(0,-1);
	return FALSE;
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSwitchBatch::InitStatic()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_cstaticCurrentBatch.SetTextColor(RGB(0,0,0));
	m_cstaticCurrentBatch.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticCurrentBatch.Format("当前卷号: %s",g_strBatch);
	m_cstaticCurrentBatch.SetTransparent(TRUE);

	m_cstaticNewBatch.SetFont("Microsoft YaHei", 18, FW_NORMAL);
	m_cstaticNewBatch.SetTransparent(TRUE);

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);

	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_btnOK.SetFont(&m_ftButton);
	m_btnCancel.SetFont(&m_ftButton);
	
	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
	m_EDIT_newBatch.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_newBatch.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
}




void CDlgSwitchBatch::CreateNewDirectory()
{

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	g_pLog->DoSomething("SETPARAM","OperatePath",g_strSaveFullPath.GetBuffer(MAX_PATH));
	g_strSaveFullPath.ReleaseBuffer();

	g_pLog->DoSomething("SETPARAM","Batch",g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();

	g_pStorage->DoSomething("SETPARAM","BATCH",g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();

	g_pStorage->DoSomething("SETPARAM","SAVEPATH",g_strSaveBasePath.GetBuffer(MAX_PATH));
	g_strSaveBasePath.ReleaseBuffer();

	g_pCommunicate->DoSomething("SETPARAM","SAVEPATH",g_strSaveFullPath.GetBuffer(MAX_PATH));
	g_strSaveFullPath.ReleaseBuffer();

	g_pCommunicate->DoSomething("SETPARAM","BATCH",g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();

	g_pCLabeller->DoSomething("SETPARAM","BATCH",g_strBatch.GetBuffer(MAX_PATH));
	g_strBatch.ReleaseBuffer();


	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileString("StorgePath", "g_strSaveFullPath", g_strSaveFullPath, g_strConfigSystemFile);
	WritePrivateProfileString("StorgePath", "g_strSaveBasePath", g_strSaveBasePath, g_strConfigSystemFile);
	WritePrivateProfileString("INFO","g_strBatchStartTime",g_strBatchStartTime,g_strConfigSystemFile);
	WritePrivateProfileString("INFO","BATCH",g_strBatch,g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
}


void CDlgSwitchBatch::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	///获取存储路径

	g_pStorage->DoSomething("GETPARAM","WRITEFLAG" ,&g_bWriteFlag);
	UpdateData(TRUE);
	WriteRecordByBatch();
	WriteCount();


	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	g_strBatchStartTime.Format("%02d%02d%02d %02d:%02d:%02d.%03d", CurSysTime.wYear, CurSysTime.wMonth, CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	CString strMeter,strValue;
	g_dwYield = 0.0;
	g_strBatch.Format("%s",m_strNewBatch);

	((CSeekersDlg*)g_pMainDialog)->m_staticBatch.Format("%s",g_strBatch);
	strMeter.Format("%.2fm",g_dwYield/1000);
	g_hvXTuple.Clear();
	g_hvYTuple.Clear();
	((CSeekersDlg*)g_pMainDialog)->m_STC_CMeter.SetWindowTextA(strMeter);
	if (m_strNewBatch.GetLength() == 0)
	{
		AfxMessageBox("新卷号不能为空");
		return;
	}
	for (int j = 0; j < g_tSystemSet.nLaneCount; j++)//每个相机下有几条
	{
		for (int i = 0; i < g_Count.nType; i++)//每一条的错误种类数目
		{
			g_Count.veCount[0][j].nClassError[i] = 0;//每种错误的计数
		}
		g_Count.veCount[0][j].nError = 0;
		g_Count.veCount[0][j].nTotal = 0;
	}
	((CSeekersDlg*)g_pMainDialog)->UpdateLabelCount();
	

	//////写入配置文件
	if(g_bWriteFlag==TRUE)//要求写入本地
	{
		SYSTEMTIME tTime;
		GetLocalTime(&tTime);
		g_strSaveBasePath.Format("D:\\ccd_data\\%02d-%d\\%d\\%s-%s",tTime.wYear,tTime.wMonth,tTime.wDay,g_strProductName,g_strBatch);
		CreateMultilevelPath(g_strSaveBasePath);
		g_strSaveFullPath.Format("%s\\瑕疵数据",g_strSaveBasePath);
		CreateMultilevelPath(g_strSaveFullPath);
	}
	CreateNewDirectory();

	if (TRUE==g_tSystemSet.bEnableReloadParam)
	{
		for(int k=0;k<g_nGrabberCount;k++)
		{
			g_vec_Algorithm[k]->DoSomething("SETPARAM","ReLoadParam",NULL);
		}
	}



	CDialogEx::OnOK();
}

void CDlgSwitchBatch::WriteCount()
{

	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	if(g_strSaveFullPath=="")
	{
		strSizeRecord.Format("%s\\%d年%02d月瑕疵贴标汇总记录.csv",g_strSaveBasePath, CurSysTime.wYear, CurSysTime.wMonth);
		strSizePath.Format("%s\\%d年%02d月", g_strSaveBasePath, CurSysTime.wYear, CurSysTime.wMonth);

	}else
	{
		 CString strDest;
		 strDest.Format("%02d-%d", CurSysTime.wYear, CurSysTime.wMonth);
		int nPosition =g_strSaveBasePath.Find(strDest);
		if (nPosition!=-1)
		{
			strSizePath.Format("%s", g_strSaveBasePath.Left(nPosition+strDest.GetLength()));
			strSizeRecord.Format("%s\\%d年%02d月瑕疵贴标汇总记录.csv",strSizePath, CurSysTime.wYear, CurSysTime.wMonth);
		}else
		{
			strSizePath.Format("%s\\Map\\ccd_data\\%02d-%d", GetCurrentAppPath(), CurSysTime.wYear, CurSysTime.wMonth);
			strSizeRecord.Format("%s\\%d年%02d月瑕疵贴标汇总记录.csv",strSizePath, CurSysTime.wYear, CurSysTime.wMonth);
		}
		

	}
	if (!PathFileExists(strSizeRecord))
	{
		CreateMultilevelPath(strSizePath);
		CString strItem;
		strItem.Format("品种,卷号,工位,开始时间,终止时间,总量,废品量,贴标率,");

		for (int i = 0; i < g_Count.nType; i++)
		{
			CString strName;
			strName.Format("%s,", g_Count.strAClassName[i]);
			strItem += strName;
		}

		HTuple hv_FileHandle;
		ofstream outFile;
		outFile.open(strSizeRecord, ios::out);
		outFile << strItem <<endl;
		outFile.close();
	}

	CString strTime;
	strTime.Format("%02d%02d%02d %02d:%02d:%02d.%03d", CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond, CurSysTime.wMilliseconds);
	HTuple hv_FileHandle;
	try
	{

	    ofstream outFile;
		CString strCount;

		outFile.open(strSizeRecord, ios::ate|ios::out |ios::app);
		for (int nLancount = 0; nLancount < g_tSystemSet.nLaneCount; nLancount++)//双条
		{
		
			CString strGongWeiName;
			int nTotal = 0;
			int nNGTotal = 0;
			int nSum = 0;
			nTotal = nTotal + g_Count.veCount[0][0].nTotal;
			nNGTotal = nNGTotal + g_Count.veCount[0][nLancount].nError;
			///获取工位名称
			strGongWeiName.Format("第%d条", nLancount);
			strCount.Format("%s,%s,%s,%s,%s,%d,%d,%.2f,", g_strProductName,g_strBatch,strGongWeiName,g_strBatchStartTime,strTime, nTotal, nNGTotal,(nNGTotal*1.0)*100/MAX(nTotal,1));
			for (int i = 0; i < g_Count.nType; i++)//所有条的每一类型的错误数目
			{
				nSum =  g_Count.veCount[0].at(nLancount).nClassError[i];
				CString strValue;
				strValue.Format("%d,", nSum);
				strCount += strValue;
			}
			outFile << strCount << endl;
			
		}
	outFile.close();
	}
	catch (HException)
	{

	}
}

void CDlgSwitchBatch::WriteRecordByBatch()//按照卷统计
{
	SYSTEMTIME CurSysTime;
	GetLocalTime(&CurSysTime);
	CString strSizeRecord, strSizePath;
	CString strTime;
	strTime.Format("%02d%02d%02d %02d:%02d:%02d.%03d", CurSysTime.wYear,CurSysTime.wMonth,CurSysTime.wDay, CurSysTime.wHour, CurSysTime.wMinute, CurSysTime.wSecond,CurSysTime.wMilliseconds);
	try
	{
		CString strCount, strGongWeiName;
		HTuple hv_TempTuple;
		TupleGenConst(g_Count.nType+2,0,&hv_TempTuple);
		for (int nLancount = 0; nLancount < g_tSystemSet.nLaneCount; nLancount++)//双条
		{
			strGongWeiName.Format("第%d条", nLancount);

			strSizeRecord.Format("%s\\当前卷贴标记录-%s.xls", g_strSaveFullPath,g_strBatch);
			strSizePath.Format("%s", g_strSaveFullPath);

			if (!PathFileExists(strSizeRecord))
			{
				CreateMultilevelPath(strSizePath);
				CString strItem;
				strItem.Format("品种\t卷号\t工位\t开始时间\t终止时间\t总量\t废品量\t贴标率\t");

				for (int i = 0; i < g_Count.nType; i++)
				{
					CString strName;
					strName.Format("%s\t", g_Count.strAClassName[i]);
					strItem += strName;
				}

				HTuple hv_FileHandle;
				ofstream outFile;
				outFile.open(strSizeRecord, ios::out);
				outFile << strItem <<endl;
				outFile.close();
			}
			ofstream outFile;
			int nTotal = 0;
			int nNGTotal = 0;
			int nSum = 0;
			nTotal = nTotal + g_Count.veCount[0][0].nTotal;
			nNGTotal = nNGTotal + g_Count.veCount[0][nLancount].nError;


			
		
			strCount.Format("%s\t%s\t%s\t%s\t%s\t%d\t%d\t%.2f\t",  g_strProductName,g_strBatch,strGongWeiName,g_strBatchStartTime, strTime, nTotal, nNGTotal,(nNGTotal*1.0)*100/MAX(1,nTotal));
			for (int i = 0; i < g_Count.nType; i++)//所有条的每一类型的错误数目
			{
				hv_TempTuple[i]=hv_TempTuple[i]+g_Count.veCount[0].at(nLancount).nClassError[i];
				nSum =  g_Count.veCount[0].at(nLancount).nClassError[i];
				CString strValue;
				strValue.Format("%d\t", nSum);
				strCount += strValue;
			}
			hv_TempTuple[g_Count.nType]=hv_TempTuple[g_Count.nType]+nTotal;
			hv_TempTuple[g_Count.nType+1]=hv_TempTuple[g_Count.nType+1]+nNGTotal;
			outFile.open(strSizeRecord, ios::ate|ios::out |ios::app);
			outFile << strCount << endl;
			outFile.close();
		}
		try
		{
			CString strErrorTuplePath;
			strErrorTuplePath.Format("%s\\缺陷信息.tup", g_strSaveFullPath);
			WriteTuple(hv_TempTuple,(HTuple)strErrorTuplePath);
		}catch(HException& e)
		{

		}





	}
	catch (HException)
	{

	}



}

void CDlgSwitchBatch::InsertDatabase()
{
	if (0 == g_nDatabaseType)
	{
		return;
	}
	try
	{
		CString strSQL;
		CString strTableName("batch");
		strSQL.Format("SELECT * FROM sysobjects WHERE xtype = 'u' AND name='%s'",strTableName);//查询尺寸表是否存在如果不存在就创建
		_RecordsetPtr pset = g_AdoDatabase.GetRecordSet(_bstr_t(strSQL));
		if (0 == pset->GetRecordCount())
		{
			//先生成对应表
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
		int nErrorCount = 0;
		for (int i=0;i<g_tSystemSet.nLaneCount;i++)
		{
			nErrorCount = nErrorCount + g_Count.veCount[0][i].nError;
		}
		//更改上一次的数据
		strSQL.Format("UPDATE [dahengimage].[dbo].[batch] "
			"SET [BATCH] = '%s' "
			",[PRODUCTNAME] = '%s'"
			",[FRAMECOUNT] = %d "
			",[ERRORCOUNT] = %d "
			",[YIELD] = %.2f ",g_strBatch,g_strProductName,g_Count.veCount[0][0].nTotal +g_Count.veCount[0][1].nTotal,nErrorCount,g_dwYield);
		for (int i=0;i<g_Count.nType;i++)
		{
			CString strTemp;
			strTemp.Format("%s,[EC%d] = %d",strSQL,i+1,g_Count.veCount[0][0].nClassError[i]);
			strSQL.Format("%s",strTemp);
		}
		CString strWhere;
		strWhere.Format("WHERE [BATCH] = '%s'"
			"AND [PRODUCTNAME] = '%s'"
			"AND [FRAMECOUNT] = 0"
			"AND [ERRORCOUNT] = 0",g_strBatch,g_strProductName);
		strSQL +=strWhere;
		if(FALSE == g_AdoDatabase.ExecuteSQL((_bstr_t)strSQL))
		{
			g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)g_AdoDatabase.GetLastErrorText(),NULL);
		}
		//加入本次的数据
		strSQL.Format("INSERT INTO [dahengimage].[dbo].[batch]"
			"([BATCH],[PRODUCTNAME],[FRAMECOUNT],[ERRORCOUNT],[YIELD],[TIME])"
			"VALUES"
			"('%s','%s',%d,%d,%.2f,GETDATE())",m_strNewBatch,g_strProductName,
			0,0,0.00);
		if(FALSE == g_AdoDatabase.ExecuteSQL((_bstr_t)strSQL))
		{
			g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)g_AdoDatabase.GetLastErrorText(),NULL);
		}

	}
	catch(_com_error e)
	{
		CString strException;
		strException.Format("%s",e.ErrorMessage());
		g_pLog->DoSomething("EXCEPTION",(void*)(LPCSTR)strException,NULL);
	}
}
void CDlgSwitchBatch::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgSwitchBatch::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//如果是回车
		{
			OnBnClickedButtonOk();
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSwitchBatch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);
}


BOOL CDlgSwitchBatch::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}


LRESULT CDlgSwitchBatch::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgSwitchBatch::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
