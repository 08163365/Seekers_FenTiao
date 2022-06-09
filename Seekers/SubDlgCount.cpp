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

void CSubDlgCount::InsertData()
{

	const int nColumCount = 16;
	for (int nGrabberSN = 0; nGrabberSN < g_nGrabberCount; nGrabberSN++)
	{
		CListCtrlExt *pList;
		HWND hwnd;
		if (nGrabberSN == 0)
		{
			pList = &m_listCountClass;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNTCALSS);
		}
		else
		{
			pList = &m_listCount;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNT);
		}
		SNDMSG(hwnd, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

		while (pList->GetHeaderCtrl()->GetItemCount())
		{
			pList->DeleteColumn(0);
		}

		pList->DeleteAllItems();
		pList->InsertColumn(0, _T("����/���"), LVCFMT_CENTER, 50);//50
		for (int i = 0; i < MIN(nColumCount, g_Count.strAClassName.GetSize()); i++)//�����������Ŀ
		{
			pList->InsertColumn(i + 1, g_Count.strAClassName.GetAt(i), LVCFMT_CENTER, 80);//128
		}
		int nItemCount = pList->GetItemCount();
		for (int i = 0; i < m_nLaneCount; i++)//����ͳ����Ŀ
		{
			CString strIndex;
			strIndex.Format("%d/%d", i, nGrabberSN);
			pList->InsertItem(nItemCount + i, strIndex);
			CString strCount;
			for (int j = 0; j < MIN(nColumCount, g_Count.strAClassName.GetSize()); j++)
			{
				strCount.Format("%d", g_Count.veCount[nGrabberSN][i].nClassError[j]);
				pList->SetItemText(nItemCount + i, j + 1, strCount);
			}
		}


		pList->SetColColor(0, RGB(10, 150, 20)); //�����б���ɫ
		pList->SetColTextColor(0, RGB(255, 255, 255));
		for (int i = 0; i < pList->GetHeaderCtrl()->GetItemCount(); i++)
		{
			if (0 == i % 2)
			{
				pList->SetColColor(i, RGB(30, 100, 90)); //�����б���ɫ
				pList->SetColTextColor(i, RGB(255, 255, 0));
			}
			else
			{
				pList->SetColColor(i, RGB(50, 120, 110)); //�����б���ɫ
				pList->SetColTextColor(i, RGB(0, 255, 255));
			}
		}
		//���ÿؼ�����˸
		SNDMSG(hwnd, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
	}
	for (int nGrabberSN = 0; nGrabberSN < g_nGrabberCount; nGrabberSN++)
	{
		CListCtrlExt *pList;
		HWND hwnd;
		if (nGrabberSN == 0)
		{
			pList = &m_listCountClass;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNTCALSS);
		}
		else
		{
			pList = &m_listCount;
			hwnd = ::GetDlgItem(m_hWnd, IDC_LIST_COUNT);
		}
		SNDMSG(hwnd, WM_SETREDRAW, FALSE, 0);//SetWindowRedraw

		int nItemCount = pList->GetItemCount();
		pList->InsertItem(nItemCount, "����/���");
		pList->SetItemColor(0, nItemCount, RGB(255, 255, 255));
		pList->SetItemTextColor(0, nItemCount, RGB(0, 0, 0));
		for (int i = 0; i < MAX(0, g_Count.strAClassName.GetSize() - (nColumCount + 1)); i++)//�����������Ŀ
		{

			pList->SetItemColor(i + 1, nItemCount, RGB(255, 255, 255));
			pList->SetItemTextColor(i + 1, nItemCount, RGB(0, 0, 0));
			pList->SetItemText(nItemCount, i + 1, g_Count.strAClassName.GetAt(nColumCount + 1 + i));//128

		}
		nItemCount = pList->GetItemCount();
		for (int i = 0; i < m_nLaneCount; i++)//����ͳ����Ŀ
		{
			CString strIndex;
			strIndex.Format("%d/%d", i, nGrabberSN);
			pList->InsertItem(nItemCount + i, strIndex);
			CString strCount;
			for (int j = 0; j < MAX(0, g_Count.strAClassName.GetSize() - (nColumCount + 1)); j++)
			{
				strCount.Format("%d", g_Count.veCount[nGrabberSN][i].nClassError[nColumCount + 1 + j]);
				pList->SetItemText(nItemCount + i, j + 1, strCount);
			}
		}
		//���ÿؼ�����˸
		SNDMSG(hwnd, WM_SETREDRAW, TRUE, 0);//SetWindowRedraw
	}

}


BOOL CSubDlgCount::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//AdaptiveSize(m_hWnd);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_nLaneCount = g_tSystemSet.nLaneCount;//����Ϊ����
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

	m_ftButton.CreatePointFont(100, "Microsoft YaHei");
	m_ftList.CreatePointFont(80, "Microsoft YaHei");

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

	m_btnHistogram.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnHistogram.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnHistogram.SetFont(&m_ftButton);
	m_btnHistogram.SetIcon(IDI_ICON_W_123);
	m_btnClearCount.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnClearCount.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnClearCount.SetFont(&m_ftButton);
	m_btnClearCount.SetIcon(IDI_ICON_W_CLEAR);
	m_btnHistory.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnHistory.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnHistory.SetFont(&m_ftButton);
	m_btnHistory.SetIcon(IDI_ICON_W_PAPER);


	m_listCountClass.SetHeaderHeight(1.5);          //����ͷ���߶�
	m_listCountClass.SetHeaderFontHW(13, 0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	m_listCountClass.SetHeaderTextColor(RGB(0, 0, 0)); //����ͷ��������ɫ 255,200,100
	m_listCountClass.SetHeaderBKColor(255, 255, 255, 0); //����ͷ������ɫ 100,255,100,8
	m_listCountClass.SetBkColor(RGB(255, 255, 255));        //���ñ���ɫ 50,10,10
	m_listCountClass.SetRowHeigt(30);               //�����и߶�
	m_listCountClass.SetFontHW(12, 0);              //��������߶ȣ��Ϳ��,0��ʾȱʡ���

	m_listCount.SetHeaderHeight(1.5);          //����ͷ���߶�
	m_listCount.SetHeaderFontHW(13, 0);         //����ͷ������߶�,�Ϳ��,0��ʾȱʡ������Ӧ 
	m_listCount.SetHeaderTextColor(RGB(0, 0, 0)); //����ͷ��������ɫ
	m_listCount.SetHeaderBKColor(255, 255, 255, 0); //����ͷ������ɫ
	m_listCount.SetBkColor(RGB(255, 255, 255));        //���ñ���ɫ
	m_listCount.SetRowHeigt(30);               //�����и߶�
	m_listCount.SetFontHW(12, 0);               //��������߶ȣ��Ϳ��,0��ʾȱʡ���

	m_listCountClass.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //������չ���Ϊ����
	m_listCount.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);  //������չ���Ϊ����
	InsertData();
}

void CSubDlgCount::ResetSize()
{
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);

	LONG nSrcX(987), nSrcY(408);
	LONG nDstX = rtSize.Width();
	LONG nDstY = rtSize.Height();
	float fsp[2];
	fsp[0] = (float)nDstX / nSrcX;
	fsp[1] = (float)nDstY / nSrcY;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ� 
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
		pWnd->GetDlgItem(woc)->GetWindowRect(Rect);
		pWnd->ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		pWnd->GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}

}

void CSubDlgCount::OnBnClickedBtnHistogram()
{

}


void CSubDlgCount::OnBnClickedBtnClearcount()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (MessageBox(TEXT("�Ƿ����?"), TEXT("��ʾ"), MB_OKCANCEL) != IDOK)
	{
		return;
	}
	for (int k = 0; k < g_nGrabberCount; k++)//���ٸ����
	{

		for (int j = 0; j < m_nLaneCount; j++)//ÿ��������м���
		{
			for (int i = 0; i < g_Count.nType; i++)//ÿһ���Ĵ���������Ŀ
			{
				g_Count.veCount[k][j].nClassError[i] = 0;//ÿ�ִ���ļ���
			}
			g_Count.veCount[k][j].nError = 0;
			g_Count.veCount[k][j].nTotal = 0;
		}
	}


	InsertData();
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
	CRect   rcFullDlg, rcTitle;
	GetClientRect(rcFullDlg);
	dc.FillSolidRect(rcFullDlg, RGB(255, 255, 255));
}


BOOL CSubDlgCount::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
