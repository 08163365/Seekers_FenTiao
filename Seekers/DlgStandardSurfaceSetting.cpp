// DlgStandardSurfaceSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgStandardSurfaceSetting.h"
#include "afxdialogex.h"
// DlgStandardSetting.cpp : 实现文件
//



// CDlgStandardSurfaceSetting 对话框

IMPLEMENT_DYNAMIC(CDlgStandardSurfaceSetting, CDialogEx)

	CDlgStandardSurfaceSetting::CDlgStandardSurfaceSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStandardSurfaceSetting::IDD, pParent)
	, m_EDT_nTotalMaxNumber(0)
	, m_EDT_dTotalMaxArea(0)
{
	m_pOwner = NULL;
}

CDlgStandardSurfaceSetting::~CDlgStandardSurfaceSetting()
{
}

void CDlgStandardSurfaceSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TotalMaxNumber, m_EDT_nTotalMaxNumber);
	DDX_Text(pDX, IDC_EDIT_dTotalMaxArea, m_EDT_dTotalMaxArea);
	DDX_Control(pDX, IDC_LIST_SURFACE, m_LST_Surface);
	DDX_Control(pDX, IDC_COMBO_ERRORNAME, m_COM_ErrorName);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btn_Add);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btn_Delete);

}


BEGIN_MESSAGE_MAP(CDlgStandardSurfaceSetting, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgStandardSurfaceSetting::OnBnClickedButtonAdd)
//	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgStandardSurfaceSetting::OnBnClickedButtonOk)
//	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgStandardSurfaceSetting::OnBnClickedButtonCancel)

	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgStandardSurfaceSetting::OnBnClickedButtonDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SURFACE, &CDlgStandardSurfaceSetting::OnLvnItemchangedListSurface)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgStandardSurfaceSetting 消息处理程序


void CDlgStandardSurfaceSetting::UpdateSurfaceList()
{
	m_LST_Surface.DeleteAllItems();
	m_LST_Surface.DeleteAllColumns();


	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "缺陷名称", LVCFMT_CENTER, 100);//0
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 100);//1
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "单个面积", LVCFMT_CENTER, 80);//2
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 150);//3
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "累计个数", LVCFMT_CENTER, 80);//4
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 100);//5
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "单个面积", LVCFMT_CENTER, 80);//6
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 150);//7
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "累计个数", LVCFMT_CENTER, 80);//8

	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 150);//7
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "单个宽度(与前面两个面积条件是且的关系)", LVCFMT_CENTER, 180);//8
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "描述", LVCFMT_CENTER, 50);//7
	m_LST_Surface.InsertColumn(m_LST_Surface.GetColumnCount(), "报警弹窗(0关闭 1开启)", LVCFMT_CENTER, 50);//7
	for (int i=0;i<m_pOwner->m_tStandardSurface.hv_ErrorIndex.Length();i++)
	{
		int nItemCount = m_LST_Surface.GetItemCount();
		m_LST_Surface.InsertItem(nItemCount, "");
		CString strInsert;
		strInsert.Format("%s",m_vec_strErrorName[m_pOwner->m_tStandardSurface.hv_ErrorIndex[i].I()]);
		m_LST_Surface.SetItemText(nItemCount,0, strInsert);
		strInsert.Format("%s","面积≥");
		m_LST_Surface.SetItemText(nItemCount, 1,strInsert);
		strInsert.Format("%.2f",m_pOwner->m_tStandardSurface.hv_TotalMaxArea[i].D());
		m_LST_Surface.SetItemText(nItemCount, 2,strInsert);
		strInsert.Format("%s","mm^2且数目≥");
		m_LST_Surface.SetItemText(nItemCount, 3,strInsert);
		strInsert.Format("%d",m_pOwner->m_tStandardSurface.hv_TotalMaxNumber[i].I());
		m_LST_Surface.SetItemText(nItemCount, 4,strInsert);
		strInsert.Format("%s","或者面积≥");
		m_LST_Surface.SetItemText(nItemCount, 5,strInsert);
		strInsert.Format("%.2f",m_pOwner->m_tStandardSurface.hv_TotalMaxAreaTwo[i].D());
		m_LST_Surface.SetItemText(nItemCount, 6,strInsert);
		strInsert.Format("%s","mm^2且数目≥");
		m_LST_Surface.SetItemText(nItemCount, 7,strInsert);
		strInsert.Format("%d",m_pOwner->m_tStandardSurface.hv_TotalMaxNumberTwo[i].I());
		m_LST_Surface.SetItemText(nItemCount, 8,strInsert);


		strInsert.Format("%s","且宽度(与前面两个面积条件均是且关系)≥");
		m_LST_Surface.SetItemText(nItemCount, 9,strInsert);
		strInsert.Format("%.2f",m_pOwner->m_tStandardSurface.hv_SingleMaxWidth[i].D());
		m_LST_Surface.SetItemText(nItemCount, 10,strInsert);
		strInsert.Format("%s","mm");
		m_LST_Surface.SetItemText(nItemCount, 11,strInsert);


		strInsert.Format("%d",m_pOwner->m_tStandardSurface.hv_EnablePopWindow[i].I());
		m_LST_Surface.SetItemText(nItemCount, 12,strInsert);
	}

}
void CDlgStandardSurfaceSetting::ComPareChange(int LastValue,int  CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("%d侧打标规格检测参数%s由%d更改为%d",m_pOwner->m_tInit.nSN,strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}
void CDlgStandardSurfaceSetting::ComPareChange(double LastValue,double CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("%d侧打标规格检测参数%s由%.2f更改为%.2f",m_pOwner->m_tInit.nSN,strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}


void CDlgStandardSurfaceSetting::GetSurfaceList()
{

	STANDARDSURFACE tStandardSurface;
	tStandardSurface=m_pOwner->m_tStandardSurface;
	m_pOwner->m_tStandardSurface.hv_ErrorIndex.Clear();
	m_pOwner->m_tStandardSurface.hv_TotalMaxNumber.Clear();
	m_pOwner->m_tStandardSurface.hv_TotalMaxArea.Clear();
	m_pOwner->m_tStandardSurface.hv_TotalMaxNumberTwo.Clear();
	m_pOwner->m_tStandardSurface.hv_TotalMaxAreaTwo.Clear();
	m_pOwner->m_tStandardSurface.hv_SingleMaxWidth.Clear();
	m_pOwner->m_tStandardSurface.hv_EnablePopWindow.Clear();
	int nItemCount = m_LST_Surface.GetItemCount();
	if(tStandardSurface.hv_ErrorIndex.Length()!=nItemCount)
	{

		ComPareChange(tStandardSurface.hv_ErrorIndex.Length(),nItemCount,"瑕疵判断种类");
	}
	m_pOwner->m_tStandardSurface.nSecErrorCount = nItemCount;
	m_pOwner->m_tStandardSurface.nErrorCount    = m_COM_ErrorName.GetCount();
	for (int i=0;i<nItemCount;i++)
	{
		CString strErrorName = m_LST_Surface.GetItemText(i,0);
		int nErrorIndex;
		for (int j=0;j<m_vec_strErrorName.size();j++)
		{
			if (strErrorName == m_vec_strErrorName[j])
			{
				nErrorIndex = j;
				break;
			}
		}
		m_pOwner->m_tStandardSurface.hv_ErrorIndex.Append(nErrorIndex);
		CString strText ;
		strText = m_LST_Surface.GetItemText(i,2);
		m_pOwner->m_tStandardSurface.hv_TotalMaxArea.Append(atof(strText));

		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s面积1",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_TotalMaxArea[i].D(),atof(strText),strMessage);
		}


		strText = m_LST_Surface.GetItemText(i,4);
		m_pOwner->m_tStandardSurface.hv_TotalMaxNumber.Append(atoi(strText));

		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s数目1",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_TotalMaxNumber[i].I(),atoi(strText),strMessage);
		}


		strText = m_LST_Surface.GetItemText(i,6);
		m_pOwner->m_tStandardSurface.hv_TotalMaxAreaTwo.Append(atof(strText));

		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s面积2",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_TotalMaxAreaTwo[i].D(),atof(strText),strMessage);
		}

		strText = m_LST_Surface.GetItemText(i,8);
		m_pOwner->m_tStandardSurface.hv_TotalMaxNumberTwo.Append(atoi(strText));
		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s数目2",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_TotalMaxNumberTwo[i].I(),atoi(strText),strMessage);
		}


		strText = m_LST_Surface.GetItemText(i,10);
		m_pOwner->m_tStandardSurface.hv_SingleMaxWidth.Append(atof(strText));

		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s宽度",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_SingleMaxWidth[i].D(),atof(strText),strMessage);
		}


		strText = m_LST_Surface.GetItemText(i,12);
		m_pOwner->m_tStandardSurface.hv_EnablePopWindow.Append(atoi(strText));
		if(tStandardSurface.hv_ErrorIndex.Length()==nItemCount)
		{
			CString strMessage;
			strMessage.Format("%s弹窗报警使能",m_vec_strErrorName[nErrorIndex]);
			ComPareChange(tStandardSurface.hv_EnablePopWindow[i].I(),atoi(strText),strMessage);
		}

	}
}

void CDlgStandardSurfaceSetting::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_FontButton);
}

void  CDlgStandardSurfaceSetting::InitUI()
{
	m_FontButton.CreatePointFont(100,"Microsoft YaHei");

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	SetButtonColors(m_btn_Delete,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_btn_Add,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);






}
BOOL CDlgStandardSurfaceSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_FontBig.CreatePointFont(100, "微软雅黑");

	m_LST_Surface.SetFont(&m_FontBig);
	m_LST_Surface.SetColorHeader(TRUE);
	m_LST_Surface.SetHeaderBKColor(255, 255, 255, 1);
	m_LST_Surface.SetHeaderFontHW(14, 0);
	m_LST_Surface.SetEditable();
	m_LST_Surface.SetGridLines();

	CString strFileName;
	strFileName.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
	int nErrorCount = GetPrivateProfileIntH("ERROR","COUNTTYPE",1,strFileName);
	for (int i=0;i<nErrorCount;i++)
	{
		CString strErrorName,lpKeyName;
		lpKeyName.Format("ERROR%dNAME",i);
		strErrorName = GetPrivateProfileCString("ERRORINFO",lpKeyName,"NONE",strFileName);
		m_vec_strErrorName.push_back(strErrorName);
		m_COM_ErrorName.AddString(strErrorName);
		m_COM_ErrorName.SetCurSel(0);
	}


	UpdateSurfaceList();
	m_LST_Surface.SetFocus();




	UpdateData(FALSE);
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgStandardSurfaceSetting::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_LST_Surface.GetItemCount();i++)
	{
		CString strName = m_LST_Surface.GetItemText(i,0);
		if (strName == m_vec_strErrorName[m_COM_ErrorName.GetCurSel()])
		{
			return;
		}
	}
	if (TRUE == UpdateData(TRUE))
	{
		int nItemCount = m_LST_Surface.GetItemCount();
		m_LST_Surface.InsertItem(nItemCount,"");
		m_LST_Surface.SetItemText(nItemCount,0,m_vec_strErrorName[m_COM_ErrorName.GetCurSel()]);
		CString strInsert;

		strInsert.Format("%s","面积大于");
		m_LST_Surface.SetItemText(nItemCount, 1,strInsert);
		strInsert.Format("%.1f",m_EDT_dTotalMaxArea);
		m_LST_Surface.SetItemText(nItemCount,2,strInsert);
		strInsert.Format("%s","mm^2且数目大于");
		m_LST_Surface.SetItemText(nItemCount, 3,strInsert);
		strInsert.Format("%d",m_EDT_nTotalMaxNumber);
		m_LST_Surface.SetItemText(nItemCount,4,strInsert);
		strInsert.Format("%s","或者面积大于");
		m_LST_Surface.SetItemText(nItemCount, 5,strInsert);
		strInsert.Format("%.1f",m_EDT_dTotalMaxArea);
		m_LST_Surface.SetItemText(nItemCount,6,strInsert);
		strInsert.Format("%s","mm^2且数目大于");
		m_LST_Surface.SetItemText(nItemCount, 7,strInsert);
		strInsert.Format("%d",m_EDT_nTotalMaxNumber);
		m_LST_Surface.SetItemText(nItemCount,8,strInsert);

		strInsert.Format("%s","且宽度(与前面两个面积条件均是且关系)≥");
		m_LST_Surface.SetItemText(nItemCount, 9,strInsert);
		strInsert.Format("%.2f",0);
		m_LST_Surface.SetItemText(nItemCount, 10,strInsert);
		strInsert.Format("%s","mm");
		m_LST_Surface.SetItemText(nItemCount, 11,strInsert);
		strInsert.Format("%d",0);
		m_LST_Surface.SetItemText(nItemCount, 12,strInsert);

	}
}



void CDlgStandardSurfaceSetting::OnBnClickedButtonDelete()
{
	POSITION tPos = m_LST_Surface.GetFirstSelectedItemPosition();
	int nItem = m_LST_Surface.GetNextSelectedItem(tPos);
	if (nItem >= 0)
	{
		m_LST_Surface.DeleteItem(nItem);
	}

}


void CDlgStandardSurfaceSetting::OnLvnItemchangedListSurface(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgStandardSurfaceSetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(255,255,255));


}





HBRUSH CDlgStandardSurfaceSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}










