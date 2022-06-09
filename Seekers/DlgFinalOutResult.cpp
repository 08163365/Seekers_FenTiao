// DlgFinalOutResult.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgFinalOutResult.h"
#include "afxdialogex.h"


// CDlgFinalOutResult 对话框

IMPLEMENT_DYNAMIC(CDlgFinalOutResult, CDialogEx)

CDlgFinalOutResult::CDlgFinalOutResult(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFinalOutResult::IDD, pParent)
{

	EnableAutomation();
	m_Listfont.CreatePointFont(90,"Microsoft YaHei");
	m_nListCtrlSize=20;
}

CDlgFinalOutResult::~CDlgFinalOutResult()
{
}

void CDlgFinalOutResult::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void CDlgFinalOutResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UPSIDE, m_List_UpSide);
	DDX_Control(pDX, IDC_LIST_DOWNSIDE, m_List_DownSide);
}


BEGIN_MESSAGE_MAP(CDlgFinalOutResult, CDialogEx)
//	ON_WM_LBUTTONDBLCLK()
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UPSIDE, &CDlgFinalOutResult::OnDblclkListUpside)
//	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOWNSIDE, &CDlgFinalOutResult::OnNMDblclkListDownside)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDlgFinalOutResult, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_IDlgFinalOutResult 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {3F57156F-5F7F-4FD2-8F9B-7E71E659D26F}
static const IID IID_IDlgFinalOutResult =
{ 0x3F57156F, 0x5F7F, 0x4FD2, { 0x8F, 0x9B, 0x7E, 0x71, 0xE6, 0x59, 0xD2, 0x6F } };

BEGIN_INTERFACE_MAP(CDlgFinalOutResult, CDialogEx)
	INTERFACE_PART(CDlgFinalOutResult, IID_IDlgFinalOutResult, Dispatch)
END_INTERFACE_MAP()


// CDlgFinalOutResult 消息处理程序


BOOL CDlgFinalOutResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	InitUI();


	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgFinalOutResult::InitUI()
{
	m_List_UpSide.SetFont(&m_Listfont);
	m_List_DownSide.SetFont(&m_Listfont);
	m_List_UpSide.SetColorHeader(TRUE);
	m_List_UpSide.SetHeaderBKColor(255,255,255,0);
	m_List_DownSide.SetColorHeader(TRUE);
	m_List_DownSide.SetHeaderBKColor(255,255,255,0);
	m_List_UpSide.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_List_DownSide.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	//InsertColumn();
}
void CDlgFinalOutResult::ResetSize(int nStyle)
{
	if (0 == nStyle)
	{
		int nSrcX,nSrcY;
		CRect rtSize,rtList1,rtList2;
		this->GetWindowRect(rtSize);
		ScreenToClient(rtSize);
		nSrcX = rtSize.Width();
		nSrcY = rtSize.Height();

		rtList1.left = rtSize.left;
		rtList1.right = rtSize.right;
		rtList1.top = rtSize.top;
		rtList1.bottom = rtSize.top+rtSize.Height()/2-5;
		GetDlgItem(IDC_LIST_UPSIDE)->MoveWindow(rtList1);

		rtList2.left = rtSize.left;
		rtList2.right = rtSize.right;
		rtList2.top = rtSize.top+rtSize.Height()/2+5;
		rtList2.bottom = rtSize.bottom;
		GetDlgItem(IDC_LIST_DOWNSIDE)->MoveWindow(rtList2);
	}else
	{
		CRect rtSize;
		this->GetWindowRect(rtSize);
		ScreenToClient(rtSize);
		GetDlgItem(IDC_LIST_UPSIDE)->MoveWindow(rtSize);
		GetDlgItem(IDC_LIST_DOWNSIDE)->ShowWindow(SW_HIDE);
	}
	InsertColumn();

}
void CDlgFinalOutResult::InsertColumn()
{
	try
	{
		CReportCtrlExt* pList = NULL;
		for (int i=0;i<1;i++)
		{	

			pList = &m_List_UpSide;
			CRect rcRect;
			pList->GetWindowRect(rcRect);
			CString strName;
			strName.Format("产品EA号%d",i);
			pList->InsertColumn(0,strName,LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(1,"极耳个数",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(2,"极耳个数结果",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(3,"表面综合结果",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(4,"表面报废项",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(5,"数目",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(6,"累计面积",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);

			pList->InsertColumn(7,"尺寸检测结果",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(8,"尺寸报废项",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(9,"输出结果",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);


		}


		for (int i=0;i<1;i++)
		{	
			pList = &m_List_DownSide;
			CRect rcRect;
			pList->GetWindowRect(rcRect);

			CString strName;
			strName.Format("产品EA号%d",i);
			pList->InsertColumn(0,strName,LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			for (int j=0;j<g_tSystemSet.nLaneCount;j++)
			{
				strName.Format("条号%d最终结果",j+1);
				pList->InsertColumn(j+1,strName,LVCFMT_LEFT,rcRect.Width()*1/g_tSystemSet.nLaneCount - 1);

			}
		}



		

	}catch(HException)
	{



	}
}
void CDlgFinalOutResult::Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	//根据结果判断插入到哪个List控件里
	CReportCtrlExt* pList = NULL;
	HWND hwndList;
	//去除非本次循环
	try
	{

		hwndList = ::GetDlgItem(m_hWnd,IDC_LIST_UPSIDE);
		pList = &m_List_UpSide;
		if (TRUE == IsWindowVisible())
		{
			SNDMSG(hwndList,WM_SETREDRAW,FALSE,0);//SetWindowRedraw
			pList->LockWindowUpdate();

		}
		if (pList->GetItemCount() >= m_nListCtrlSize)
		{
			pList->DeleteItem(m_nListCtrlSize-1);
		}

			int nItemCount = 0;
			pList->InsertItem(nItemCount, "");		
			pList->SetItemText(nItemCount, 0, tOutput.tFinalResult.hv_ProductID.S());//第几组产品

			CString strInsert;
			strInsert.Format("%d", tOutput.tFinalResult.hv_FoilNum.I());//当前组极耳的个数
			pList->SetItemText(nItemCount, 1, strInsert);


			{
				strInsert.Format("%s", "OK");//当前组极耳的个数
				pList->SetItemText(nItemCount, 2, strInsert);
			}
			

			if ( tOutput.tFinalResult.hv_SurfaceFlag.I()==1)
			{
				strInsert.Format("%s","OK");//表面的综合结果
				pList->SetItemText(nItemCount, 3, strInsert);
				strInsert.Format("%s","__");
				pList->SetItemText(nItemCount, 4, strInsert);
				pList->SetItemText(nItemCount, 5, strInsert);
				pList->SetItemText(nItemCount, 6, strInsert);
				pList->SetItemText(nItemCount, 7, strInsert);

			}else
			{
				strInsert.Format("%s","NG");//表面的综合结果
				pList->SetItemText(nItemCount, 3, strInsert);
				pList->SetItemBkColor(nItemCount, 3, RGB(255,0,0));

				if (tOutput.tFinalResult.hv_SurfaceErrorIndex.Length()>0)
				{

					HTuple hv_SortErrorIndex;
					TupleSort(tOutput.tFinalResult.hv_SurfaceErrorIndex,&hv_SortErrorIndex);
					HTuple hv_Index;
					hv_Index.Clear();
					TupleFind(hv_SortErrorIndex,JieDai,&hv_Index);///升级要检测接带
					if (hv_Index!=-1 && hv_Index.Length()>0)
					{
						strInsert.Format("%s",g_Count.strAClassName[JieDai] );//表面报错项
						pList->SetItemText(nItemCount, 4, strInsert);
						HTuple hv_Index;
						hv_Index.Clear();
						TupleFind(tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType,JieDai,&hv_Index);
						strInsert.Format("%d",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index].I());//数目
						pList->SetItemText(nItemCount, 5, strInsert);
						strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index].D());//累计面积
						pList->SetItemText(nItemCount, 6, strInsert);

					}else
					{
						strInsert.Format("%s",g_Count.strAClassName[hv_SortErrorIndex[0]] );//表面报错项
						pList->SetItemText(nItemCount, 4, strInsert);
						HTuple hv_Index;
						hv_Index.Clear();
						TupleFind(tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType,tOutput.tFinalResult.hv_SurfaceErrorIndex[0],&hv_Index);
						strInsert.Format("%d",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index].I());//数目
						pList->SetItemText(nItemCount, 5, strInsert);
						strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index].D());//累计面积
						pList->SetItemText(nItemCount, 6, strInsert);
					}
				}
			}
			
			
			if ( tOutput.tFinalResult.hv_SizeFlag.I()==1)
			{
				strInsert.Format("%s","OK");//尺寸的综合结果
				pList->SetItemText(nItemCount, 7, strInsert);
				strInsert.Format("%s","__");
				pList->SetItemText(nItemCount, 8, strInsert);
			}
			else
			{
				strInsert.Format("%s","NG");//尺寸的综合结果
				pList->SetItemText(nItemCount, 7, strInsert);
				pList->SetItemBkColor(nItemCount, 7,RGB(255,0,0));
				if (tOutput.tFinalResult.hv_SizeErrorIndex.Length()>0)
				{
					//HTuple hv_Index;
					//HTuple hv_SortErrorIndex;
					//hv_Index.Clear();
					//TupleSort(tOutput.tFinalResult.hv_SizeErrorIndex,&hv_SortErrorIndex);
					//TupleFind(tOutput.tFinalResult.hv_SizeErrorIndex,SumSize,&hv_Index);///升级要检测总长
					//if (hv_Index!=-1 && hv_Index.Length()>0)
					//{
					//	strInsert.Format("%s",g_Count.strAClassName[SumSize] );//尺寸报错项
					//}else
					//{
					//	strInsert.Format("%s",g_Count.strAClassName[hv_SortErrorIndex[0]] );//表面报错项
					//}
					//pList->SetItemText(nItemCount, 8, strInsert);
					//pList->SetItemBkColor(nItemCount, 8,RGB(255,0,0));

				}

			}
			HTuple hv_TempResult;
			TupleAnd(tOutput.tFinalResult.hv_SizeFlag, tOutput.tFinalResult.hv_SurfaceFlag,&hv_TempResult);
			if (hv_TempResult.I()==0)
			{
				strInsert.Format("%s", "NG");
				pList->SetItemText(nItemCount, 9, strInsert);
				pList->SetItemBkColor(nItemCount, 9, RGB(255,0,0));
			}else
			{
				strInsert.Format("%s","OK");
				pList->SetItemText(nItemCount, 9, strInsert);
			}
			
			
		

		if (TRUE == IsWindowVisible())
		{
			pList->UnlockWindowUpdate();
			SNDMSG(hwndList,WM_SETREDRAW,TRUE,0);
		}

		/////////////////////////////////////////////////////////////////////////插入每条的详细结果

		hwndList = ::GetDlgItem(m_hWnd,IDC_LIST_DOWNSIDE);
		pList = &m_List_DownSide;
		int nItemCount1 = 0;
		pList->InsertItem(nItemCount1, "");		
		pList->SetItemText(nItemCount1, 0, tOutput.tFinalResult.hv_ProductID.S());//第几组产品
	
		HTuple hv_SumErrorBelongTrips;
		hv_SumErrorBelongTrips.Clear();
		hv_SumErrorBelongTrips.Append( tOutput.tFinalResult.hv_SurfaceErrorBelongTrips);
		hv_SumErrorBelongTrips.Append( tOutput.tFinalResult.hv_SizeErrorBelongTrips);
		HTuple hv_Index;
		for (int j=0;j<g_tSystemSet.nLaneCount;j++)
		{

			TupleFind(hv_SumErrorBelongTrips,j,&hv_Index);
			if(hv_Index.Length()>0 && hv_Index!=-1)
			{
				pList->SetItemText(nItemCount1, j+1, "NG");//第几组产品
				pList->SetItemBkColor(nItemCount1, j+1, RGB(255,0,0));

			}else
			{
				pList->SetItemText(nItemCount1, j+1,"OK");//第几组产品

			}


		}



	}catch(HException)
	{

	}
}


//void CDlgFinalOutResult::OnDblclkListUpside(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	int nItem=m_List_UpSide.GetSelectionMark();//获得选 中行的行标
//	if (nItem!=-1)
//	{
//		CString StrProductID;
//		StrProductID=m_List_UpSide.GetItemText(nItem,0);
//		CDlgErrorMonitor dlg;
//		dlg.m_strGroupID=StrProductID;
//		dlg.m_nTripeType=0;
//		if (dlg.DoModal()==IDOK)
//		{
//
//		}
//
//
//
//	}else
//	{
//		CDlgErrorMonitor dlg;
//		dlg.m_strGroupID="";
//		dlg.m_nTripeType=3;
//		if (dlg.DoModal()==IDOK)
//		{
//
//		}
//	}
//	
//	
//	*pResult = 0;
//}


//void CDlgFinalOutResult::OnNMDblclkListDownside(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	int nItem=m_List_DownSide.GetSelectionMark();//获得选 中行的行标
//	if (nItem!=-1)
//	{
//		CString StrProductID;
//		StrProductID=m_List_DownSide.GetItemText(nItem,0);
//		CDlgErrorMonitor dlg;
//		dlg.m_strGroupID=StrProductID;
//		dlg.m_nTripeType=1;
//		if (dlg.DoModal()==IDOK)
//		{
//
//		}
//	}else
//	{
//		CDlgErrorMonitor dlg;
//		dlg.m_strGroupID="";
//		dlg.m_nTripeType=3;
//		if (dlg.DoModal()==IDOK)
//		{
//
//		}
//	}
//	
//	*pResult = 0;
//}
