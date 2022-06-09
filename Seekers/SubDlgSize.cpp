// SubDlgSize.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "SubDlgSize.h"
#include "afxdialogex.h"

// CSubDlgSize 对话框

IMPLEMENT_DYNAMIC(CSubDlgSize, CDialogEx)

CSubDlgSize::CSubDlgSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSubDlgSize::IDD, pParent)
	, m_nListCtrlSize(20)
{
	m_Listfont.CreatePointFont(90, "Microsoft YaHei");
}

CSubDlgSize::~CSubDlgSize()
{
	m_Listfont.DeleteObject();
}

void CSubDlgSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SIZE, m_listSize);
	DDX_Control(pDX, IDC_LIST_SIZE2, m_listSize2);
}


BEGIN_MESSAGE_MAP(CSubDlgSize, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSubDlgSize 消息处理程序


BOOL CSubDlgSize::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//AdaptiveSize(m_hWnd);
	// TODO:  在此添加额外的初始化
	InitUI();
	//ResetSize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSubDlgSize::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}

void CSubDlgSize::InitUI()
{
	m_listSize.SetFont(&m_Listfont);
	m_listSize2.SetFont(&m_Listfont);
	m_listSize.SetColorHeader(TRUE);
	m_listSize.SetHeaderBKColor(255, 255, 255, 0);
	m_listSize2.SetColorHeader(TRUE);
	m_listSize2.SetHeaderBKColor(255, 255, 255, 0);
	m_listSize.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_listSize2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);


	//InsertColumn();
}

void CSubDlgSize::Update(CReportCtrlExt* pList, B_DETECTWORKSHOPOUTPUT* pDetectOutput)
{
}
void CSubDlgSize::InsertColumn()
{
	try
	{
		CReportCtrlExt* pList = NULL;
		CString strGongWeiName;
		for (int i = 0; i < g_nGrabberCount; i++)
		{
			if (0 == i)//左侧的
			{
				pList = &m_listSize;

			}
			else
			{
				pList = &m_listSize2;
			}

			strGongWeiName = GetGongWeiName(g_CUpLoad.m_StrGongWeiName, i);
			CRect rcRect;
			pList->GetWindowRect(rcRect);
			int k = 0;
			int nColumnNum = 10;
			int nWidth = rcRect.Width() * 1 / nColumnNum - 3;
			CString strName;

			if (g_Jugde[0].m_tStandardSize.hv_CoatType == M_TwoCam_Continue)
			{



			}
			else
			{
				strName.Format("%sEA号", strGongWeiName);
				pList->InsertColumn(k, strName, LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "总宽度", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "总长度", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, i ? "A面膜长(前)" : "B面膜长(前)", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, i ? "A面留白(前)" : "B面留白(前)", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "小错位(头部)", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "大错位(尾部)", LVCFMT_LEFT, nWidth);

			}
		}


	}
	catch (HException)
	{
	}
}

BOOL  CSubDlgSize::IsError(HTuple& hv_ErroType, e_ErrorType nErrorCode)
{
	HTuple hv_Index;
	hv_Index.Clear();
	TupleFind(hv_ErroType, nErrorCode, &hv_Index);
	if (hv_Index != -1 && hv_Index.Length() > 0)
	{
		return TRUE;
	}
	return FALSE;
}
void CSubDlgSize::Insert(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	//根据结果判断插入到哪个List控件里

	if (g_Jugde[0].m_tStandardSize.hv_CoatType == M_TwoCam_Continue)
	{
		InsertContinue(tOutput);
	}
	else
	{
		InsertIntermission(tOutput);
	}

}

void CSubDlgSize::InsertIntermission(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	//根据结果判断插入到哪个List控件里

	CReportCtrlExt* pList = NULL;
	HWND hwndList;

	//去除非本次循环
	try
	{
		int nSN;
		CString strInsert;
		for (int i = 0; i < g_nGrabberCount; i++)
		{
			nSN = i;
			if (0 == nSN)//前相机
			{
				hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_SIZE);
				pList = &m_listSize;
			}
			else//后相机
			{
				hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_SIZE2);
				pList = &m_listSize2;
			}
			if (pList->GetItemCount() >= m_nListCtrlSize)
			{
				pList->DeleteItem(m_nListCtrlSize - 1);
			}

			SNDMSG(hwndList, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw
			pList->LockWindowUpdate();

			{
				/*	pList->InsertColumn(++k,"总宽度",LVCFMT_LEFT,nWidth);
				pList->InsertColumn(++k,"总长度",LVCFMT_LEFT,nWidth);
				pList->InsertColumn(++k,"B面膜长(前)",LVCFMT_LEFT,nWidth);
				pList->InsertColumn(++k,"B面留白(前)",LVCFMT_LEFT,nWidth);
				pList->InsertColumn(++k, "A面膜长(前)", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "A面留白(前)", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "头部错位", LVCFMT_LEFT, nWidth);
				pList->InsertColumn(++k, "尾部错位", LVCFMT_LEFT, nWidth);
				*/
				//////
				int nItemCount = 0;
				int k = 0;
				pList->InsertItem(nItemCount, "");
				pList->SetItemText(nItemCount, k, tOutput.hvGroupID.S().Text());

				strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_CoatWidth.D());
				pList->SetItemText(nItemCount, ++k, strInsert);
				if (TRUE == IsError(tOutput.B[0].tDetectOutput.hv_ErrorType, C_WSize))
				{
					pList->SetItemBkColor(nItemCount, k, RGB(255, 0, 0));
				}

				////总长
				if (tOutput.bIsFinalResult == TRUE)
				{
					strInsert.Format("%.3f", tOutput.tFinalResult.hv_TotalLength.D());

				}
				else
				{
					strInsert.Format("--");
				}
				pList->SetItemText(nItemCount, ++k, strInsert);


				////总膜长
				strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_MPLength.D());
				pList->SetItemText(nItemCount, ++k, strInsert);


				if (TRUE == IsError(tOutput.B[nSN].tDetectOutput.hv_ErrorType, nSN == 0 ? BHXJDownSumSize : BHXJUpSumSize))
				{
					pList->SetItemBkColor(nItemCount, k, RGB(255, 0, 0));
				}


				////间歇高度
				strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_JXHeight.D());
				pList->SetItemText(nItemCount, ++k, strInsert);
				if (TRUE == IsError(tOutput.B[nSN].tDetectOutput.hv_ErrorType, nSN == 0 ? DownJXSize : UpJXSize))
				{
					pList->SetItemBkColor(nItemCount, k, RGB(255, 0, 0));
				}

				////头部错位
				strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_HeadCuoWei.D());
				pList->SetItemText(nItemCount, ++k, strInsert);
				if (TRUE == IsError(tOutput.B[nSN].tDetectOutput.hv_ErrorType, HeadCuoWei))
				{
					pList->SetItemBkColor(nItemCount, k, RGB(255, 0, 0));
				}
				////尾部错位
				strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_TailCuoWei.D());
				pList->SetItemText(nItemCount, ++k, strInsert);
				if (TRUE == IsError(tOutput.B[nSN].tDetectOutput.hv_ErrorType, TailCuoWei))
				{
					pList->SetItemBkColor(nItemCount, k, RGB(255, 0, 0));
				}

			}
			pList->UnlockWindowUpdate();
			SNDMSG(hwndList, WM_SETREDRAW, TRUE, 0);


		}



	}
	catch (HException& e)
	{
		CString strErrorInfo;
		strErrorInfo.Format("函数Size：%s %s", e.ProcName().Text(), e.ErrorMessage().Text());
		CString strException;
		strException.Format("Location: %d EANum:%s Info：%s", tOutput.hvPackageType.I(), tOutput.hvGroupID.S().Text(), strErrorInfo);
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}
void CSubDlgSize::InsertContinue(C_ALLOCATORWORKSHOPOUTPUT& tOutput)
{
	//根据结果判断插入到哪个List控件里
	CReportCtrlExt* pList = NULL;
	HWND hwndList;
	//去除非本次循环
	try
	{
		int nSN;
		CString strInsert;
		for (int i = 0; i < g_nGrabberCount; i++)
		{
			nSN = i;
			if (0 == nSN)//前相机
			{
				hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_SIZE);
				pList = &m_listSize;
			}
			else//后相机
			{
				hwndList = ::GetDlgItem(m_hWnd, IDC_LIST_SIZE2);
				pList = &m_listSize2;
			}
			int nn = pList->GetColumnCount();
			if (pList->GetColumnCount() != tOutput.B[nSN].tDetectOutput.hv_SortWidth.Length() + 1)
			{
				CRect rcRect;
				pList->GetWindowRect(rcRect);
				int k = 0;
				int nColumnNum = tOutput.B[nSN].tDetectOutput.hv_SortWidth.Length() + 1;
				int nWidth = rcRect.Width() * 1 / nColumnNum - 3;
				CString strName, strGongWeiName;
				strGongWeiName = GetGongWeiName(g_CUpLoad.m_StrGongWeiName, i);
				strName.Format("%sEA号", strGongWeiName);
				pList->InsertColumn(0, strName, LVCFMT_LEFT, nWidth);
				for (int k = 0; k < tOutput.B[nSN].tDetectOutput.hv_Name.Length(); k++)
				{
					pList->InsertColumn(k + 1, tOutput.B[nSN].tDetectOutput.hv_Name[k].S().Text(), LVCFMT_LEFT, nWidth);
				}

			}
			if (pList->GetItemCount() >= m_nListCtrlSize)
			{
				pList->DeleteItem(m_nListCtrlSize - 1);
			}

			SNDMSG(hwndList, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw
			pList->LockWindowUpdate();

			{
				int nItemCount = 0;
				pList->InsertItem(nItemCount, "");
				pList->SetItemText(nItemCount, 0, tOutput.hvGroupID.S().Text());
				for (int j = 0; j < tOutput.B[nSN].tDetectOutput.hv_SortWidth.Length(); j++)
				{
					strInsert.Format("%.3f", tOutput.B[nSN].tDetectOutput.hv_SortWidth[j].D());
					pList->SetItemText(nItemCount, j + 1, strInsert);
				}
			}
			pList->UnlockWindowUpdate();
			SNDMSG(hwndList, WM_SETREDRAW, TRUE, 0);
		}
	}
	catch (HException& e)
	{
		CString strException;
		strException.Format("InsertContinue函数Size：%s %s", e.ProcName().Text(), e.ErrorMessage().Text());
		g_pLog->DoSomething("EXCEPTION", (void*)(LPCSTR)strException, NULL);
	}
}

void CSubDlgSize::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


void CSubDlgSize::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::DestroyWindow();
}


BOOL CSubDlgSize::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//屏蔽回车
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)//屏蔽ESC
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//屏蔽ALT+F4
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSubDlgSize::ResetSize(int nStyle)
{
	if (0 == nStyle)
	{
		int nSrcX, nSrcY;
		CRect rtSize, rtList1, rtList2;
		this->GetWindowRect(rtSize);
		ScreenToClient(rtSize);
		nSrcX = rtSize.Width();
		nSrcY = rtSize.Height();

		rtList1.left = rtSize.left;
		rtList1.right = rtSize.right;
		rtList1.top = rtSize.top;
		rtList1.bottom = rtSize.top + rtSize.Height() / 2;
		GetDlgItem(IDC_LIST_SIZE)->MoveWindow(rtList1);

		rtList2.left = rtSize.left;
		rtList2.right = rtSize.right;
		rtList2.top = rtSize.top + rtSize.Height() / 2 + 5;
		rtList2.bottom = rtSize.bottom;
		GetDlgItem(IDC_LIST_SIZE2)->MoveWindow(rtList2);
	}
	else
	{
		CRect rtSize;
		this->GetWindowRect(rtSize);
		ScreenToClient(rtSize);
		GetDlgItem(IDC_LIST_SIZE)->MoveWindow(rtSize);

		GetDlgItem(IDC_LIST_SIZE2)->ShowWindow(SW_HIDE);
	}
	InsertColumn();

}

void CSubDlgSize::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	//ResetSize();
}


void CSubDlgSize::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//CRect   rcFullDlg;   
	//GetClientRect(rcFullDlg);  
	//dc.FillSolidRect(rcFullDlg,RGB(255,255,255));   //设置为绿色背景255,240,150
	CDialogEx::OnPaint();
}


BOOL CSubDlgSize::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
