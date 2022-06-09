// DlgFinalOutResult.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgFinalOutResult.h"
#include "afxdialogex.h"


// CDlgFinalOutResult �Ի���

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
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

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

// ע��: ������� IID_IDlgFinalOutResult ֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {3F57156F-5F7F-4FD2-8F9B-7E71E659D26F}
static const IID IID_IDlgFinalOutResult =
{ 0x3F57156F, 0x5F7F, 0x4FD2, { 0x8F, 0x9B, 0x7E, 0x71, 0xE6, 0x59, 0xD2, 0x6F } };

BEGIN_INTERFACE_MAP(CDlgFinalOutResult, CDialogEx)
	INTERFACE_PART(CDlgFinalOutResult, IID_IDlgFinalOutResult, Dispatch)
END_INTERFACE_MAP()


// CDlgFinalOutResult ��Ϣ�������


BOOL CDlgFinalOutResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	InitUI();


	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
			strName.Format("��ƷEA��%d",i);
			pList->InsertColumn(0,strName,LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(1,"��������",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(2,"�����������",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(3,"�����ۺϽ��",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(4,"���汨����",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(5,"��Ŀ",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(6,"�ۼ����",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);

			pList->InsertColumn(7,"�ߴ�����",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(8,"�ߴ籨����",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			pList->InsertColumn(9,"������",LVCFMT_LEFT,rcRect.Width()*1/11 - 1);


		}


		for (int i=0;i<1;i++)
		{	
			pList = &m_List_DownSide;
			CRect rcRect;
			pList->GetWindowRect(rcRect);

			CString strName;
			strName.Format("��ƷEA��%d",i);
			pList->InsertColumn(0,strName,LVCFMT_LEFT,rcRect.Width()*1/11 - 1);
			for (int j=0;j<g_tSystemSet.nLaneCount;j++)
			{
				strName.Format("����%d���ս��",j+1);
				pList->InsertColumn(j+1,strName,LVCFMT_LEFT,rcRect.Width()*1/g_tSystemSet.nLaneCount - 1);

			}
		}



		

	}catch(HException)
	{



	}
}
void CDlgFinalOutResult::Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	//���ݽ���жϲ��뵽�ĸ�List�ؼ���
	CReportCtrlExt* pList = NULL;
	HWND hwndList;
	//ȥ���Ǳ���ѭ��
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
			pList->SetItemText(nItemCount, 0, tOutput.tFinalResult.hv_ProductID.S());//�ڼ����Ʒ

			CString strInsert;
			strInsert.Format("%d", tOutput.tFinalResult.hv_FoilNum.I());//��ǰ�鼫���ĸ���
			pList->SetItemText(nItemCount, 1, strInsert);


			{
				strInsert.Format("%s", "OK");//��ǰ�鼫���ĸ���
				pList->SetItemText(nItemCount, 2, strInsert);
			}
			

			if ( tOutput.tFinalResult.hv_SurfaceFlag.I()==1)
			{
				strInsert.Format("%s","OK");//������ۺϽ��
				pList->SetItemText(nItemCount, 3, strInsert);
				strInsert.Format("%s","__");
				pList->SetItemText(nItemCount, 4, strInsert);
				pList->SetItemText(nItemCount, 5, strInsert);
				pList->SetItemText(nItemCount, 6, strInsert);
				pList->SetItemText(nItemCount, 7, strInsert);

			}else
			{
				strInsert.Format("%s","NG");//������ۺϽ��
				pList->SetItemText(nItemCount, 3, strInsert);
				pList->SetItemBkColor(nItemCount, 3, RGB(255,0,0));

				if (tOutput.tFinalResult.hv_SurfaceErrorIndex.Length()>0)
				{

					HTuple hv_SortErrorIndex;
					TupleSort(tOutput.tFinalResult.hv_SurfaceErrorIndex,&hv_SortErrorIndex);
					HTuple hv_Index;
					hv_Index.Clear();
					TupleFind(hv_SortErrorIndex,JieDai,&hv_Index);///����Ҫ���Ӵ�
					if (hv_Index!=-1 && hv_Index.Length()>0)
					{
						strInsert.Format("%s",g_Count.strAClassName[JieDai] );//���汨����
						pList->SetItemText(nItemCount, 4, strInsert);
						HTuple hv_Index;
						hv_Index.Clear();
						TupleFind(tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType,JieDai,&hv_Index);
						strInsert.Format("%d",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index].I());//��Ŀ
						pList->SetItemText(nItemCount, 5, strInsert);
						strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index].D());//�ۼ����
						pList->SetItemText(nItemCount, 6, strInsert);

					}else
					{
						strInsert.Format("%s",g_Count.strAClassName[hv_SortErrorIndex[0]] );//���汨����
						pList->SetItemText(nItemCount, 4, strInsert);
						HTuple hv_Index;
						hv_Index.Clear();
						TupleFind(tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType,tOutput.tFinalResult.hv_SurfaceErrorIndex[0],&hv_Index);
						strInsert.Format("%d",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[hv_Index].I());//��Ŀ
						pList->SetItemText(nItemCount, 5, strInsert);
						strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[hv_Index].D());//�ۼ����
						pList->SetItemText(nItemCount, 6, strInsert);
					}
				}
			}
			
			
			if ( tOutput.tFinalResult.hv_SizeFlag.I()==1)
			{
				strInsert.Format("%s","OK");//�ߴ���ۺϽ��
				pList->SetItemText(nItemCount, 7, strInsert);
				strInsert.Format("%s","__");
				pList->SetItemText(nItemCount, 8, strInsert);
			}
			else
			{
				strInsert.Format("%s","NG");//�ߴ���ۺϽ��
				pList->SetItemText(nItemCount, 7, strInsert);
				pList->SetItemBkColor(nItemCount, 7,RGB(255,0,0));
				if (tOutput.tFinalResult.hv_SizeErrorIndex.Length()>0)
				{
					//HTuple hv_Index;
					//HTuple hv_SortErrorIndex;
					//hv_Index.Clear();
					//TupleSort(tOutput.tFinalResult.hv_SizeErrorIndex,&hv_SortErrorIndex);
					//TupleFind(tOutput.tFinalResult.hv_SizeErrorIndex,SumSize,&hv_Index);///����Ҫ����ܳ�
					//if (hv_Index!=-1 && hv_Index.Length()>0)
					//{
					//	strInsert.Format("%s",g_Count.strAClassName[SumSize] );//�ߴ籨����
					//}else
					//{
					//	strInsert.Format("%s",g_Count.strAClassName[hv_SortErrorIndex[0]] );//���汨����
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

		/////////////////////////////////////////////////////////////////////////����ÿ������ϸ���

		hwndList = ::GetDlgItem(m_hWnd,IDC_LIST_DOWNSIDE);
		pList = &m_List_DownSide;
		int nItemCount1 = 0;
		pList->InsertItem(nItemCount1, "");		
		pList->SetItemText(nItemCount1, 0, tOutput.tFinalResult.hv_ProductID.S());//�ڼ����Ʒ
	
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
				pList->SetItemText(nItemCount1, j+1, "NG");//�ڼ����Ʒ
				pList->SetItemBkColor(nItemCount1, j+1, RGB(255,0,0));

			}else
			{
				pList->SetItemText(nItemCount1, j+1,"OK");//�ڼ����Ʒ

			}


		}



	}catch(HException)
	{

	}
}


//void CDlgFinalOutResult::OnDblclkListUpside(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	int nItem=m_List_UpSide.GetSelectionMark();//���ѡ ���е��б�
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	int nItem=m_List_DownSide.GetSelectionMark();//���ѡ ���е��б�
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
