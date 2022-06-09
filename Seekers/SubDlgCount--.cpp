// SubDlgCount.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "SubDlgCount.h"
#include "afxdialogex.h"
#include "DlgDatabaseHistory.h"

// CSubDlgCount �Ի���

IMPLEMENT_DYNAMIC(CSubDlgCount, CDialogEx)

	CSubDlgCount::CSubDlgCount(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSubDlgCount::IDD, pParent)
{
	//m_pSubDlgHistogram = NULL;
	m_nLaneCount = MAX_LANE;
	m_Listfont.CreatePointFont(90,"Microsoft YaHei");
	m_nListCtrlSize=10;
}

CSubDlgCount::~CSubDlgCount()
{
}

void CSubDlgCount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COUNTCALSS, m_listCountClass);
	DDX_Control(pDX, IDC_LIST_COUNT, m_listCount);
	DDX_Control(pDX, IDC_BTN_HISTOGRAM, m_btnHistogram);
	DDX_Control(pDX, IDC_BTN_CLEARCOUNT, m_btnClearCount);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistory);
}


BEGIN_MESSAGE_MAP(CSubDlgCount, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_HISTOGRAM, &CSubDlgCount::OnBnClickedBtnHistogram)
	ON_BN_CLICKED(IDC_BTN_CLEARCOUNT, &CSubDlgCount::OnBnClickedBtnClearcount)
	ON_BN_CLICKED(IDC_BTN_HISTORY, &CSubDlgCount::OnBnClickedBtnHistory)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSubDlgCount ��Ϣ�������
void CSubDlgCount::Update(BOOL bBuild/*�Ƿ��ؽ�*/)
{


}
void CSubDlgCount::Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	CReportCtrlExt* pList = NULL;
	
	HWND hwndList;
	//ȥ���Ǳ���ѭ��
	try
	{
		if (0 == tOutput.hvPackageType)//����
		{
			hwndList = ::GetDlgItem(m_hWnd,IDC_LIST_COUNTCALSS);
			pList = &m_listCountClass;
		}else
		{
			hwndList = ::GetDlgItem(m_hWnd,IDC_LIST_COUNT);
			pList = &m_listCount;
		}
		if (TRUE == IsWindowVisible())
		{
			SNDMSG(hwndList,WM_SETREDRAW,FALSE,0);//SetWindowRedraw
			pList->LockWindowUpdate();

		}
	
		/*	pList->InsertColumn(0,"��Ʒ���",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		pList->InsertColumn(1,"��������",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		pList->InsertColumn(2,"�������",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		pList->InsertColumn(3,"�ܴ������(mm^2)",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		pList->InsertColumn(4,"���ֱ��(mm)",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		pList->InsertColumn(5,"�����",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		*/

		for (int k=0;k<tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType.Length();k++)
		{
			if (pList->GetItemCount() >= m_nListCtrlSize)
			{
				pList->DeleteItem(m_nListCtrlSize-1);
			}
			int nItemCount = 0;
			pList->InsertItem(nItemCount, "");
			pList->SetItemText(nItemCount, 0, tOutput.tFinalResult.hv_ProductID.S());//�ڼ����Ʒ
			
			CString strInsert;
			if (tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum!=-1)
			{
				strInsert.Format("%s",g_Count.strAClassName.GetAt(tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorType[k].I()));//��������
				pList->SetItemText(nItemCount, 1, strInsert);
				strInsert.Format("%d",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorNum[k].I());//��������
				pList->SetItemText(nItemCount, 2, strInsert);
				strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorSumArea[k].D());//���������
				pList->SetItemText(nItemCount, 3, strInsert);
				strInsert.Format("%.2f",tOutput.tFinalResult.tSecondSurfaceInfo.hv_ErrorMaxDiameter[k].D());//�������ֱ��
				pList->SetItemText(nItemCount, 4, strInsert);
			}else
			{
				pList->SetItemText(nItemCount, 1, "--");
				pList->SetItemText(nItemCount, 2, "--");
				pList->SetItemText(nItemCount, 3, "--");
				pList->SetItemText(nItemCount, 4, "--");
			}
			if ( tOutput.tFinalResult.hv_SurfaceFlag.I()==1)
			{
				strInsert.Format("%s","OK");//������ۺϽ��
				pList->SetItemText(nItemCount, 5, strInsert);
			
			}else
			{
				strInsert.Format("%s","NG");//������ۺϽ��
				pList->SetItemText(nItemCount, 5, strInsert);
				pList->SetItemBkColor(nItemCount, 5, RGB(255,0,0));
			}

		}
	






	



		if (TRUE == IsWindowVisible())
		{
			pList->UnlockWindowUpdate();
			SNDMSG(hwndList,WM_SETREDRAW,TRUE,0);
		}


	}catch(HException)
	{

	}



}

//void CSubDlgCount::Update(BOOL bBuild/*�Ƿ��ؽ�*/)
//{
//	if (m_pSubDlgHistogram!=NULL)
//	{
//		if (m_pSubDlgHistogram->GetSafeHwnd())
//		{
//			if (m_pSubDlgHistogram->IsWindowVisible())
//			{
//				m_pSubDlgHistogram->Update();
//				return;
//			}
//		}
//	}
//
//	if (bBuild)
//	{
//		while(m_listCountClass.GetHeaderCtrl()->GetItemCount())
//		{
//			m_listCountClass.DeleteColumn(0);
//		}
//		m_listCountClass.DeleteAllItems();
//
//		m_listCountClass.InsertColumn(0, _T("����/���"), LVCFMT_CENTER, 80);
//		for (int i=0;i<g_Count.nType;i++)
//		{
//			m_listCountClass.InsertColumn(i+1,g_Count.strAClassName.GetAt(i),LVCFMT_CENTER,128);//128
//		}
//		int nItemCount = m_listCountClass.GetItemCount();
//		for (int i=0;i<m_nLaneCount;i++)
//		{
//			CString strIndex;
//			strIndex.Format("%d/%d",i,0);
//			m_listCountClass.InsertItem(nItemCount+i,strIndex);
//			CString strCount;
//			for (int j=0;j<g_Count.nType;j++)
//			{
//				strCount.Format("%d",g_Count.veCount[0][i].nClassError[j]);
//				m_listCountClass.SetItemText(nItemCount+i,j+1,strCount);
//			}
//		}
//		
//		nItemCount = m_listCountClass.GetItemCount();
//		m_listCountClass.InsertItem(nItemCount,"�ܼ�0");
//		for (int i=0;i<g_Count.nType;i++)
//		{
//			int nCount=0;
//			for (int j=0;j<m_nLaneCount;j++)
//			{
//				nCount+=g_Count.veCount[0][j].nClassError[i];
//			}
//			CString strCount;
//			strCount.Format("%d",nCount);
//			m_listCountClass.SetItemText(nItemCount,i+1,strCount);
//		}
//		if (2==g_nGrabberCount)
//		{
//			nItemCount = m_listCountClass.GetItemCount();
//			for (int i=0;i<m_nLaneCount;i++)
//			{
//				CString strIndex;
//				strIndex.Format("%d/%d",i,1);
//				m_listCountClass.InsertItem(nItemCount+i,strIndex);
//				CString strCount;
//				for (int j=0;j<g_Count.nType;j++)
//				{
//					strCount.Format("%d",g_Count.veCount[1][i].nClassError[j]);
//					m_listCountClass.SetItemText(i+nItemCount,j+1,strCount);
//				}
//			}
//			nItemCount = m_listCountClass.GetItemCount();
//			m_listCountClass.InsertItem(nItemCount,"�ܼ�1");
//			for (int i=0;i<g_Count.nType;i++)
//			{
//				int nCount=0;
//				for (int j=0;j<m_nLaneCount;j++)
//				{
//					nCount+=g_Count.veCount[1][j].nClassError[i];
//				}
//				CString strCount;
//				strCount.Format("%d",nCount);
//				m_listCountClass.SetItemText(nItemCount,i+1,strCount);
//			}
//
//
//		}
//		
//		m_listCountClass.SetColColor(0,RGB(10,150,20)); //�����б���ɫ
//		m_listCountClass.SetColTextColor(0,RGB(255,255,255));
//		for (int i=0;i<m_listCountClass.GetHeaderCtrl()->GetItemCount();i++)
//		{
//			if (0 == i%2)
//			{
//				m_listCountClass.SetColColor(i,RGB(30,100,90)); //�����б���ɫ
//				m_listCountClass.SetColTextColor(i,RGB(255,255,0));
//			}else
//			{
//				m_listCountClass.SetColColor(i,RGB(50,120,110)); //�����б���ɫ
//				m_listCountClass.SetColTextColor(i,RGB(0,255,255));
//			}
//		}
//		while(m_listCount.GetHeaderCtrl()->GetItemCount())
//		{
//			m_listCount.DeleteColumn(0);
//		}
//		m_listCount.DeleteAllItems();
//		m_listCount.InsertColumn(0, _T("����/���"), LVCFMT_CENTER, 80);
//		m_listCount.InsertColumn(1, _T("����Ƭ��"), LVCFMT_CENTER, 128);
//		m_listCount.InsertColumn(2, _T("����"), LVCFMT_CENTER, 128);
//
//		for (int k=0;k<g_nGrabberCount;k++)
//		{
//			nItemCount = m_listCount.GetItemCount();
//			for (int i=0;i<m_nLaneCount;i++)
//			{
//				CString strIndex;strIndex.Format("%d/%d",i,k);
//				m_listCount.InsertItem(i+nItemCount,strIndex);
//				CString strCount;
//				int nCount;
//				nCount=g_Count.veCount[k][i].nError;
//				strCount.Format("%d",nCount);
//				m_listCount.SetItemText(i+nItemCount,1,strCount);
//				strCount.Format("%d",g_Count.nTotal[i]);
//				m_listCount.SetItemText(i+nItemCount,2,strCount);
//			}
//		}
//		
//
//		m_listCount.SetColColor(0,RGB(10,150,20)); //�����б���ɫ
//		m_listCount.SetColTextColor(0,RGB(255,255,255));
//		for (int i=0;i<m_listCount.GetHeaderCtrl()->GetItemCount();i++)
//		{
//			if (0 == i%2)
//			{
//				m_listCount.SetColColor(i,RGB(30,100,90)); //�����б���ɫ
//				m_listCount.SetColTextColor(i,RGB(255,255,0));
//			}else
//			{
//				m_listCount.SetColColor(i,RGB(50,120,110)); //�����б���ɫ
//				m_listCount.SetColTextColor(i,RGB(0,255,255));
//			}
//		}
//	}
//else//��������ʾ
//	{
////		int nItemCount = m_listCountClass.GetItemCount();
////		/*if (nItemCount!=m_nLaneCount*2+2)
////		{
////			Update(TRUE);
////		}
////*/
//		for (int i=0;i<g_nGrabberCount;i++)
//		{
//			UpdateDate(i);
//		}
//	}
//	//���ÿؼ�����˸
//	//SNDMSG(hwndCountClass,WM_SETREDRAW,TRUE,0);//SetWindowRedraw
//	//SNDMSG(hwndCount,WM_SETREDRAW,TRUE,0);//SetWindowRedraw
//}


BOOL CSubDlgCount::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//AdaptiveSize(m_hWnd);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSubDlgCount::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::DestroyWindow();
}


void CSubDlgCount::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::DestroyWindow();
}


void CSubDlgCount::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


void CSubDlgCount::InitUI()
{

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_ftList.CreatePointFont(130,"���Ĳʺ�");

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

	m_btnHistogram.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_IN,FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_OUT,FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS,FG);
	m_btnHistogram.SetFont(&m_ftButton);
	m_btnHistogram.SetIcon(IDI_ICON_W_123);
	m_btnClearCount.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_IN,FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_OUT,FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS,FG);
	m_btnClearCount.SetFont(&m_ftButton);
	m_btnClearCount.SetIcon(IDI_ICON_W_CLEAR);
	m_btnHistory.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_IN,FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_OUT,FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS,FG);
	m_btnHistory.SetFont(&m_ftButton);
	m_btnHistory.SetIcon(IDI_ICON_W_PAPER);

	Update(TRUE);
	//m_listCountClass.SetHeaderHeight(1.5);          //����ͷ���߶�
	//m_listCountClass.SetHeaderFontHW(15,0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	//m_listCountClass.SetHeaderTextColor(RGB(0,0,0)); //����ͷ��������ɫ 255,200,100
	//m_listCountClass.SetHeaderBKColor(255,255,255,0); //����ͷ������ɫ 100,255,100,8
	//m_listCountClass.SetBkColor(RGB(255,255,255));        //���ñ���ɫ 50,10,10
	//m_listCountClass.SetRowHeigt(30);               //�����и߶�
	//m_listCountClass.SetFontHW(14,0);               //��������߶ȣ��Ϳ��,0��ʾȱʡ���
	//


	//m_listCount.SetHeaderHeight(1.5);          //����ͷ���߶�
	//m_listCount.SetHeaderFontHW(15,0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	//m_listCount.SetHeaderTextColor(RGB(0,0,0)); //����ͷ��������ɫ
	//m_listCount.SetHeaderBKColor(255,255,255,0); //����ͷ������ɫ
	//m_listCount.SetBkColor(RGB(255,255,255));        //���ñ���ɫ
	//m_listCount.SetRowHeigt(30);               //�����и߶�
	//m_listCount.SetFontHW(14,0);               //��������߶ȣ��Ϳ��,0��ʾȱʡ���
	//
	//m_listCountClass.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //������չ���Ϊ����
	//m_listCount.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //������չ���Ϊ����
	

	m_listCount.SetFont(&m_Listfont);
	m_listCount.SetColorHeader(TRUE);
	m_listCount.SetHeaderBKColor(255,255,255,0);
	m_listCount.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	m_listCountClass.SetFont(&m_Listfont);
	m_listCountClass.SetColorHeader(TRUE);
	m_listCountClass.SetHeaderBKColor(255,255,255,0);
	m_listCountClass.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);


}
void CSubDlgCount::InsertColumn()
{
	try
	{
		CReportCtrlExt* pList = NULL;
		for (int i=0;i<2;i++)
		{	
			if (0 == i)//����
			{
				pList = &m_listCountClass;
			}else
			{
				pList = &m_listCount;
			}
			CRect rcRect;
			pList->GetWindowRect(rcRect);
			pList->InsertColumn(0,"��Ʒ���",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
			pList->InsertColumn(1,"��������",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
			pList->InsertColumn(2,"�������",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
			pList->InsertColumn(3,"�ܴ������(mm^2)",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
			pList->InsertColumn(4,"���ֱ��(mm)",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
			pList->InsertColumn(5,"�����",LVCFMT_LEFT,rcRect.Width()*1/6 - 1);
		}
	}catch(HException)
	{

	}
}
void CSubDlgCount::ResetSize()
{
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);

	LONG nSrcX(987),nSrcY(408);
	LONG nDstX = rtSize.Width();  
	LONG nDstY = rtSize.Height();  
	float fsp[2];
	fsp[0]=(float)nDstX/nSrcX;  
	fsp[1]=(float)nDstY/nSrcY;  
	CPoint OldTLPoint,TLPoint; //���Ͻ�  
	CPoint OldBRPoint,BRPoint; //���½�  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ� 
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	int woc; 
	CRect Rect;
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID  
		pWnd->GetDlgItem(woc)->GetWindowRect(Rect);    
		pWnd->ScreenToClient(Rect);    
		OldTLPoint = Rect.TopLeft();    
		TLPoint.x = long(OldTLPoint.x*fsp[0]);    
		TLPoint.y = long(OldTLPoint.y*fsp[1]);    
		OldBRPoint = Rect.BottomRight();    
		BRPoint.x = long(OldBRPoint.x *fsp[0]);    
		BRPoint.y = long(OldBRPoint.y *fsp[1]);    
		Rect.SetRect(TLPoint,BRPoint);    
		pWnd->GetDlgItem(woc)->MoveWindow(Rect,TRUE);  
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);      
	}

	CRect rcWinodw,rtList1,rtList2;
	GetDlgItem(IDC_LIST_COUNTCALSS)->GetWindowRect(rtList1);
	ScreenToClient(rtList1);
	GetDlgItem(IDC_LIST_COUNT)->GetWindowRect(rtList2);
	ScreenToClient(rtList2);
	rcWinodw.left = rtList1.left;
	rcWinodw.top = rtList1.top;
	rcWinodw.bottom = rtList2.bottom;
	rcWinodw.right = rtList2.right;
	InsertColumn();
}

void CSubDlgCount::OnBnClickedBtnHistogram()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//if (m_pSubDlgHistogram->IsWindowVisible())
	//{
	//	
	//	m_btnHistogram.SetWindowTextA("��״ͼ");
	//	
	//	m_btnHistogram.SetIcon(IDI_ICON_W_123);
	//	
	//	GetDlgItem(IDC_LIST_COUNTCALSS)->ShowWindow(SW_NORMAL);
	//	
	//	GetDlgItem(IDC_LIST_COUNT)->ShowWindow(SW_NORMAL);
	//	m_pSubDlgHistogram->ShowWindow(SW_HIDE);
	//	
	//}else
	//{
	//	m_btnHistogram.SetWindowTextA("���");
	//	m_btnHistogram.SetIcon(IDI_ICON_W_LIST);	
	//	GetDlgItem(IDC_LIST_COUNTCALSS)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_LIST_COUNT)->ShowWindow(SW_HIDE);
	//	m_pSubDlgHistogram->ShowWindow(SW_NORMAL);
	//	
	//}
	Update(FALSE);
}


void CSubDlgCount::OnBnClickedBtnClearcount()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(TEXT("�Ƿ����?"),TEXT("��ʾ"),MB_OKCANCEL)!=IDOK)
	{
		return;
	}
	for (int k=0;k<g_nGrabberCount;k++)
	{
	
		for (int i=0;i<g_tSystemSet.nLaneCount;i++)
		{
			g_Count.veCount[k][i].nTotal= 0;
			g_Count.veCount[k][i].nError= 0;

			for (int j=0;j<g_Count.nType;j++)
			{
				g_Count.veCount[k][i].nClassError[j] = 0;
			}
		}

	}


	Update(FALSE);
}


void CSubDlgCount::OnBnClickedBtnHistory()
{
	CDlgDatabaseHistory dlg;
	dlg.DoModal();
}


void CSubDlgCount::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect   rcFullDlg,rcTitle;  
	GetClientRect(rcFullDlg);  
	dc.FillSolidRect(rcFullDlg,RGB(255,255,255));
}


BOOL CSubDlgCount::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
void CSubDlgCount::UpdateDate(int nIndex)
{
	//����������ͳ��
	//for (int i=0;i<m_nLaneCount;i++)
	//{
	//	CString strCount;
	//	for (int j=0;j<g_Count.nType;j++)
	//	{
	//		strCount.Format("%d",g_Count.veCount[nIndex][i].nClassError[j]);
	//		m_listCountClass.SetItemText(i,j+1,strCount);
	//	}
	//}
	//for (int i=0;i<g_Count.nType;i++)
	//{
	//	int nCount=0;
	//	for (int j=0;j<m_nLaneCount;j++)
	//	{
	//		nCount+=g_Count.veCount[nIndex][j].nClassError[i];
	//	}
	//	CString strCount;
	//	strCount.Format("%d",nCount);
	//	m_listCountClass.SetItemText(m_nLaneCount,i+1,strCount);
	//}
	////������ͳ��
	//for (int i=0;i<m_nLaneCount;i++)
	//{
	//	CString strCount;
	//	int nCount;
	//	nCount=g_Count.veCount[nIndex][i].nError;
	//	strCount.Format("%d",nCount);
	//	m_listCount.SetItemText(i,1,strCount);
	//	strCount.Format("%d",g_Count.nTotal[nIndex]);
	//	m_listCount.SetItemText(i,2,strCount);
	//}
}
