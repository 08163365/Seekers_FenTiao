// PageSpotDetectSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "PageSpotDetectSet.h"
#include "afxdialogex.h"


// CPageSpotDetectSet 对话框

IMPLEMENT_DYNAMIC(CPageSpotDetectSet, CDialogEx)

CPageSpotDetectSet::CPageSpotDetectSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageSpotDetectSet::IDD, pParent)
	, m_CHK_bEnableWriteRow(FALSE)
	, m_EDT_strSpotTime1(_T(""))
	, m_EDT_strSpotTime2(_T(""))
{

	m_COM_nCameraIndex = 0;
	m_EDT_nSpotLowThresh = 0;
	m_EDT_nSpotHighThresh = 0;
	m_EDT_nSpotNum = 0;
	m_EDT_nLeftColumn = 0;
	m_EDT_nRightColumn = 0;
	m_EDT_dSpotStandardArea = 0.0;
	m_EDT_dMinCircleParam = 0.0;
	m_EDT_dFLPHeight = 0.0;
	m_EDT_dFLPHeightDev = 0.0;
	m_EDT_nFLPWarnNGFrame = 0;
	m_EDT_dFLPWidth = 0.0;
	m_EDT_dFLPWidthDev = 0.0;
}

CPageSpotDetectSet::~CPageSpotDetectSet()
{
}

void CPageSpotDetectSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHK_ENABLESPOTCHECK, m_CHK_bEnabeSpotCheck);
	DDX_CBIndex(pDX, IDC_COM_CAMREAINDEX, m_COM_nCameraIndex);
	DDX_Text(pDX, IDC_EDT_LOWTHRESH, m_EDT_nSpotLowThresh);
	DDX_Text(pDX, IDC_EDT_HIGHTHRESH, m_EDT_nSpotHighThresh);
	DDX_Text(pDX, IDC_EDT_SPOTNUM, m_EDT_nSpotNum);
	DDX_Text(pDX, IDC_EDT_LEFTX, m_EDT_nLeftColumn);
	DDX_Text(pDX, IDC_EDT_RIGHTX, m_EDT_nRightColumn);
	DDX_Text(pDX, IDC_EDT_STANDARDAREA, m_EDT_dSpotStandardArea);
	DDX_Check(pDX, IDC_CHK_ENABLEWRITEROW, m_CHK_bEnableWriteRow);
	DDX_Text(pDX, IDC_EDT_MINCIRCLEPARAM, m_EDT_dMinCircleParam);
	DDV_MinMaxDouble(pDX, m_EDT_dMinCircleParam, 0, 1);
	DDX_Text(pDX, IDC_EDT_FLPHEIGHT, m_EDT_dFLPHeight);
	DDX_Text(pDX, IDC_EDT_FLPHEIGHTDEV, m_EDT_dFLPHeightDev);
	DDX_Text(pDX, IDC_EDT_FLPNGWARNFRAME, m_EDT_nFLPWarnNGFrame);
	DDX_Text(pDX, IDC_EDT_FLPWIDTH, m_EDT_dFLPWidth);
	DDX_Text(pDX, IDC_EDT_FLPWIDTHDEV, m_EDT_dFLPWidthDev);
	DDX_Text(pDX, IDC_EDIT_TIME1, m_EDT_strSpotTime1);
	DDX_Text(pDX, IDC_EDIT_TIME2, m_EDT_strSpotTime2);
}


BEGIN_MESSAGE_MAP(CPageSpotDetectSet, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COM_CAMREAINDEX, &CPageSpotDetectSet::OnSelchangeComCamreaindex)
	ON_BN_CLICKED(IDC_BTN_SPOTTEST, &CPageSpotDetectSet::OnBnClickedBtnSpottest)
	ON_BN_CLICKED(IDC_BTN_OPENTESTIMAGE, &CPageSpotDetectSet::OnBnClickedBtnOpentestimage)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPageSpotDetectSet::OnBnClickedBtnSave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_SPOTCALIBRATION, &CPageSpotDetectSet::OnBnClickedBtnSpotcalibration)
END_MESSAGE_MAP()


// CPageSpotDetectSet 消息处理程序


void CPageSpotDetectSet::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}

void CPageSpotDetectSet::ResetSize()
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
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
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


BOOL CPageSpotDetectSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_COM_nCameraIndex = 0;
	m_CHK_bEnabeSpotCheck = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableUseSpot;
	m_EDT_nSpotLowThresh = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLowThresh;
	m_EDT_nSpotHighThresh = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nHighThresh;
	m_EDT_nSpotNum = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nSpotNum;
	m_EDT_dSpotStandardArea = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleStandardArea;
	m_EDT_nLeftColumn = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLeftColumn;
	m_EDT_nRightColumn = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nRightColumn;
	m_CHK_bEnableWriteRow = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableWriteRow;
	m_EDT_dMinCircleParam = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleParam;

	m_EDT_dFLPHeight = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeight;
	m_EDT_dFLPHeightDev = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeightDev;
	m_EDT_dFLPWidth = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidth;
	m_EDT_dFLPWidthDev = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidthDev;
	m_EDT_nFLPWarnNGFrame = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nFLPFrameWarnFrame;

	m_EDT_strSpotTime1 = g_CSpotSizeCheck[m_COM_nCameraIndex].m_strSpotTime[0];
	m_EDT_strSpotTime2 = g_CSpotSizeCheck[m_COM_nCameraIndex].m_strSpotTime[1];
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPageSpotDetectSet::FlushSpotParam(int& nIndex)
{
	UpdateData(TRUE);
	g_CSpotSizeCheck[nIndex].m_tSpotParam.bEnableUseSpot = m_CHK_bEnabeSpotCheck;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nLowThresh = m_EDT_nSpotLowThresh;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nHighThresh = m_EDT_nSpotHighThresh;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nSpotNum = m_EDT_nSpotNum;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.dCircleStandardArea = m_EDT_dSpotStandardArea;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nLeftColumn = m_EDT_nLeftColumn;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nRightColumn = m_EDT_nRightColumn;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.bEnableWriteRow = m_CHK_bEnableWriteRow;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.dCircleParam = m_EDT_dMinCircleParam;

	g_CSpotSizeCheck[nIndex].m_tSpotParam.dFLPHeight = m_EDT_dFLPHeight;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.dFLPHeightDev = m_EDT_dFLPHeightDev;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.dFLPWidth = m_EDT_dFLPWidth;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.dFLPWidthDev = m_EDT_dFLPWidthDev;
	g_CSpotSizeCheck[nIndex].m_tSpotParam.nFLPFrameWarnFrame = m_EDT_nFLPWarnNGFrame;




}
void CPageSpotDetectSet::OnSelchangeComCamreaindex()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	int nIndex = !m_COM_nCameraIndex;
	FlushSpotParam(nIndex);
	m_CHK_bEnabeSpotCheck = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableUseSpot;
	m_EDT_nSpotLowThresh = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLowThresh;
	m_EDT_nSpotHighThresh = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nHighThresh;
	m_EDT_nSpotNum = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nSpotNum;
	m_EDT_dSpotStandardArea = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleStandardArea;
	m_EDT_nLeftColumn = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLeftColumn;
	m_EDT_nRightColumn = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nRightColumn;
	m_CHK_bEnableWriteRow = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableWriteRow;
	m_EDT_dMinCircleParam = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleParam;

	m_EDT_dFLPHeight = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeight;
	m_EDT_dFLPHeightDev = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeightDev;
	m_EDT_dFLPWidth = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidth;
	m_EDT_dFLPWidthDev = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidthDev;
	m_EDT_nFLPWarnNGFrame = g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nFLPFrameWarnFrame;

	UpdateData(FALSE);
}



void CPageSpotDetectSet::OnBnClickedBtnSpottest()
{
	// TODO: 在此添加控件通知处理程序代码
	FlushSpotParam(m_COM_nCameraIndex);
	SpotOutputInfo tSpotOut;
	HTuple hv_WindowID;
	if (m_COM_nCameraIndex == 0)
	{
		hv_WindowID = m_pParentDlg->m_Wnd_FirstImage.GetDispWnd();
		DispObj(m_pParentDlg->m_hoFirstImage, hv_WindowID);
		g_CSpotSizeCheck[m_COM_nCameraIndex].Action(m_pParentDlg->m_hoFirstImage, &tSpotOut);

	}
	else
	{
		hv_WindowID = m_pParentDlg->m_Wnd_SecondImage.GetDispWnd();
		DispObj(m_pParentDlg->m_hoSecondImage, hv_WindowID);
		g_CSpotSizeCheck[m_COM_nCameraIndex].Action(m_pParentDlg->m_hoSecondImage, &tSpotOut);

	}

	SetColor(hv_WindowID, "green");
	DispObj(g_CSpotSizeCheck[m_COM_nCameraIndex].m_hoLeftRegion, hv_WindowID);
	DispObj(g_CSpotSizeCheck[m_COM_nCameraIndex].m_hoRightRegion, hv_WindowID);
	if (tSpotOut.hvSpotWidth.Length() == 2)
	{

		if (tSpotOut.hvFlag == FALSE)
			SetColor(hv_WindowID, "red");
		CString strValue;
		strValue.Format("左侧宽度:%.1fmm 高度:%.1fmm 面积:%.1fmm^2 ", tSpotOut.hvSpotWidth[0].D(), tSpotOut.hvSpotHeight[0].D(), tSpotOut.hvSpotArea[0].D());
		SetTposition(hv_WindowID, 50, 50);
		WriteString(hv_WindowID, (HTuple)strValue);
		strValue.Format("右侧:宽度:%.1fmm 高度:%.1fmm 面积:%.1fmm^2 ", tSpotOut.hvSpotWidth[1].D(), tSpotOut.hvSpotHeight[1].D(), tSpotOut.hvSpotArea[1].D());
		SetTposition(hv_WindowID, 150, 50);
		WriteString(hv_WindowID, (HTuple)strValue);
	}
	SetColor(hv_WindowID, "red");
	DispObj(tSpotOut.hoXldRegion, hv_WindowID);

}



void CPageSpotDetectSet::OnBnClickedBtnOpentestimage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this);
	HTuple Width, Height;
	if (IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName();
		HObject hoImage;
		try
		{

			if (m_COM_nCameraIndex == 0)
			{
				ReadImage(&m_pParentDlg->m_hoFirstImage, (HTuple)strFilename);
				m_pParentDlg->m_Wnd_FirstImage.DispImage(m_pParentDlg->m_hoFirstImage);
				/*GetImageSize(m_pParentDlg->m_hoFirstImage,&Width,&Height);
				SetPart(m_pParentDlg->m_Wnd_FirstImage.GetDispWnd(),0,0,Height-1,Width-1);
				DispObj(m_pParentDlg->m_hoFirstImage,m_pParentDlg->m_Wnd_FirstImage.GetDispWnd());*/
			}
			else
			{
				ReadImage(&m_pParentDlg->m_hoSecondImage, (HTuple)strFilename);
				m_pParentDlg->m_Wnd_SecondImage.DispImage(m_pParentDlg->m_hoSecondImage);
				/*	GetImageSize(m_pParentDlg->m_hoSecondImage,&Width,&Height);
					SetPart(m_pParentDlg->m_Wnd_SecondImage.GetDispWnd(),0,0,Height-1,Width-1);
					DispObj(m_pParentDlg->m_hoSecondImage,m_pParentDlg->m_Wnd_SecondImage.GetDispWnd());*/
			}
		}
		catch (HException &e)
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

	}
}


void CPageSpotDetectSet::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableUseSpot = m_CHK_bEnabeSpotCheck;

	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.bEnableWriteRow = m_CHK_bEnableWriteRow;

	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLowThresh = m_EDT_nSpotLowThresh;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nHighThresh = m_EDT_nSpotHighThresh;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nSpotNum = m_EDT_nSpotNum;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleStandardArea = m_EDT_dSpotStandardArea;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nLeftColumn = m_EDT_nLeftColumn;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nRightColumn = m_EDT_nRightColumn;

	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dCircleParam = m_EDT_dMinCircleParam;

	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeight = m_EDT_dFLPHeight;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPHeightDev = m_EDT_dFLPHeightDev;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidth = m_EDT_dFLPWidth;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.dFLPWidthDev = m_EDT_dFLPWidthDev;
	g_CSpotSizeCheck[m_COM_nCameraIndex].m_tSpotParam.nFLPFrameWarnFrame = m_EDT_nFLPWarnNGFrame;

	g_CSpotSizeCheck[0].m_strSpotTime[0] = m_EDT_strSpotTime1;
	g_CSpotSizeCheck[0].m_strSpotTime[1] = m_EDT_strSpotTime2;
	g_CSpotSizeCheck[1].m_strSpotTime[0] = m_EDT_strSpotTime1;
	g_CSpotSizeCheck[1].m_strSpotTime[1] = m_EDT_strSpotTime2;

	g_CSpotSizeCheck[0].m_tSpotParam.nSpotNum = m_EDT_nSpotNum;
	g_CSpotSizeCheck[1].m_tSpotParam.nSpotNum = m_EDT_nSpotNum;

	for (int k = 0; k < g_nGrabberCount; k++)
	{
		g_CSpotSizeCheck[k].WriteParam();
	}
}


void CPageSpotDetectSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//CRect rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect,RGB(255,255,255)); //设置为绿色背景
}


void CPageSpotDetectSet::OnBnClickedBtnSpotcalibration()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	FlushSpotParam(m_COM_nCameraIndex);
	SpotOutputInfo tSpotOut;
	HTuple hv_WindowID;
	if (m_COM_nCameraIndex == 0)
	{
		hv_WindowID = m_pParentDlg->m_Wnd_FirstImage.GetDispWnd();
		DispObj(m_pParentDlg->m_hoFirstImage, hv_WindowID);
		g_CSpotSizeCheck[m_COM_nCameraIndex].CalibrationAction(m_pParentDlg->m_hoFirstImage, &tSpotOut, m_COM_nCameraIndex);

	}
	else
	{
		hv_WindowID = m_pParentDlg->m_Wnd_SecondImage.GetDispWnd();
		DispObj(m_pParentDlg->m_hoSecondImage, hv_WindowID);
		g_CSpotSizeCheck[m_COM_nCameraIndex].CalibrationAction(m_pParentDlg->m_hoSecondImage, &tSpotOut, m_COM_nCameraIndex);

	}
	SetColor(hv_WindowID, "green");
	DispObj(g_CSpotSizeCheck[m_COM_nCameraIndex].m_hoLeftRegion, hv_WindowID);
	DispObj(g_CSpotSizeCheck[m_COM_nCameraIndex].m_hoRightRegion, hv_WindowID);
	if (tSpotOut.hvSpotWidth.Length() == 2)
	{

		if (tSpotOut.hvFlag == FALSE)
			SetColor(hv_WindowID, "red");
		CString strValue;
		strValue.Format("左(上)侧宽度:%.1fmm 高度:%.1fmm 面积:%.1fmm^2 ", tSpotOut.hvSpotWidth[0].D(), tSpotOut.hvSpotHeight[0].D(), tSpotOut.hvSpotArea[0].D());
		SetTposition(hv_WindowID, 50, 50);
		WriteString(hv_WindowID, (HTuple)strValue);
		strValue.Format("右(下)侧:宽度:%.1fmm 高度:%.1fmm 面积:%.1fmm^2 ", tSpotOut.hvSpotWidth[1].D(), tSpotOut.hvSpotHeight[1].D(), tSpotOut.hvSpotArea[1].D());
		SetTposition(hv_WindowID, 150, 50);
		WriteString(hv_WindowID, (HTuple)strValue);
	}
	SetColor(hv_WindowID, "red");
	DispObj(tSpotOut.hoXldRegion, hv_WindowID);
}
