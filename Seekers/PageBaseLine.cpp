// PageBaseLine.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "PageBaseLine.h"
#include "afxdialogex.h"


// CPageBaseLine 对话框

IMPLEMENT_DYNAMIC(CPageBaseLine, CDialogEx)

CPageBaseLine::CPageBaseLine(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageBaseLine::IDD, pParent)
{

	m_EDT_nFirstCoatLowThresh = 0;
	m_EDT_nFirstCoatHighThresh = 0;
	m_EDT_nFirstFoilHighThresh = 0;
	m_EDT_nFirstFoilLowThresh = 0;
	m_EDT_nSecondCoatLowThresh = 0;
	m_EDT_nSecondCoatHighThresh = 0;
	m_EDT_nSecondFoilLowThresh = 0;
	m_EDT_nSecondFoilHighThresh = 0;
}

CPageBaseLine::~CPageBaseLine()
{
}

void CPageBaseLine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_FIRSTCOATLOWTHRESH, m_EDT_nFirstCoatLowThresh);
	DDX_Text(pDX, IDC_EDT_FIRSTCOATHIGHTHRESH, m_EDT_nFirstCoatHighThresh);
	DDX_Text(pDX, IDC_EDT_FIRSTFOILHIGHTHRESH, m_EDT_nFirstFoilHighThresh);
	DDX_Text(pDX, IDC_EDT_FIRSTFOILLOWTHRESH, m_EDT_nFirstFoilLowThresh);
	DDX_Text(pDX, IDC_EDT_SECONDCOATLOWTHRESH, m_EDT_nSecondCoatLowThresh);
	DDX_Text(pDX, IDC_EDT_SECONDCOATHIGHTHRESH, m_EDT_nSecondCoatHighThresh);
	DDX_Text(pDX, IDC_EDT_SECONDFOILLOWTHRESH, m_EDT_nSecondFoilLowThresh);
	DDX_Text(pDX, IDC_EDT_SECONDFOILHIGHTHRESH, m_EDT_nSecondFoilHighThresh);
}


BEGIN_MESSAGE_MAP(CPageBaseLine, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_DRAWFIRSTMODEL, &CPageBaseLine::OnBnClickedBtnDrawfirstmodel)
	ON_BN_CLICKED(IDC_BTN_DRAWSECONDMODEL, &CPageBaseLine::OnBnClickedBtnDrawsecondmodel)
	ON_BN_CLICKED(IDC_BTN_ONEKEY, &CPageBaseLine::OnBnClickedBtnOnekey)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPageBaseLine::OnBnClickedBtnSave)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPageBaseLine 消息处理程序


void CPageBaseLine::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}
void CPageBaseLine::ResetSize(CRect& SourceRect)
{
	CRect rtSize;
	this->GetWindowRect(rtSize);
	ScreenToClient(rtSize);

	
	LONG nSrcX(1456),nSrcY(1497);

	nSrcX=SourceRect.Width();
	nSrcY=SourceRect.Height();
	LONG nDstX = rtSize.Width();  
	LONG nDstY = rtSize.Height();  
	float fsp[2];
	fsp[0]=(float)nDstX/nSrcX;  
	fsp[1]=(float)nDstY/nSrcY;  
	CPoint OldTLPoint,TLPoint; //左上角  
	CPoint OldBRPoint,BRPoint; //右下角  
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	int woc; 
	CRect Rect;
	while(hwndChild)      
	{      
		woc=::GetDlgCtrlID(hwndChild);//取得ID  
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

}


BOOL CPageBaseLine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_EDT_nFirstCoatLowThresh=g_tCreateBaseLineInfo.nFirstCoatLowThresh;
	m_EDT_nFirstCoatHighThresh=g_tCreateBaseLineInfo.nFirstCoatHighThresh;
	m_EDT_nFirstFoilLowThresh=g_tCreateBaseLineInfo.nFirstFoilLowThresh;
	m_EDT_nFirstFoilHighThresh=g_tCreateBaseLineInfo.nFirstFoilHighThresh;

	m_EDT_nSecondCoatLowThresh=g_tCreateBaseLineInfo.nSecondCoatLowThresh;
	m_EDT_nSecondCoatHighThresh=g_tCreateBaseLineInfo.nSecondCoatHighThresh;
	m_EDT_nSecondFoilLowThresh=g_tCreateBaseLineInfo.nSecondFoilLowThresh;
	m_EDT_nSecondFoilHighThresh=g_tCreateBaseLineInfo.nSecondFoilHighThresh;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPageBaseLine::OnBnClickedBtnDrawfirstmodel()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		UpdateData(TRUE);
		m_pParentDlg->m_Wnd_FirstImage.m_bDrawing=TRUE;
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		HObject hoROI,hoSelectRegion;
		m_pParentDlg->m_Wnd_FirstImage.DispImage(m_pParentDlg->m_hoFirstImage);
		DrawRectangle1(m_pParentDlg->m_Wnd_FirstImage.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
		GenRectangle1(&hoROI,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
		m_pParentDlg->FindCoatRegion(m_pParentDlg->m_hoFirstImage,hoROI,&hoSelectRegion,m_EDT_nFirstCoatLowThresh,m_EDT_nFirstCoatHighThresh);
		if (hoSelectRegion.CountObj()>0)
		{
			m_pParentDlg->m_Wnd_FirstImage.DispImage(m_pParentDlg->m_hoFirstImage);
			m_pParentDlg->m_Wnd_FirstImage.DispRegion(hoSelectRegion,"yellow","margin",2,TRUE);
			HTuple hv_LeftColumn,hv_RightColumn,hv_LeftRow,hv_RightRow;
			SmallestRectangle1(hoSelectRegion,&hv_LeftRow,&hv_LeftColumn,&hv_RightRow,&hv_RightColumn);
			SetTposition(m_pParentDlg->m_Wnd_FirstImage.GetDispWnd(),50,50);
			WriteString(m_pParentDlg->m_Wnd_FirstImage.GetDispWnd(),"寻边成功");
			g_tCreateBaseLineInfo.nFirstLeftPosition=hv_LeftColumn;
			g_tCreateBaseLineInfo.nFirstRightPosition=hv_RightColumn;
			g_tCreateBaseLineInfo.nFirstLeftPositionRow=hv_LeftRow;
			g_tCreateBaseLineInfo.nFirstRightPositionRow=hv_RightRow;
		}

		m_pParentDlg->m_Wnd_FirstImage.m_bDrawing=FALSE;
	}catch(HException& e)
	{

	}








}


void CPageBaseLine::OnBnClickedBtnDrawsecondmodel()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		UpdateData(TRUE);
		m_pParentDlg->m_Wnd_SecondImage.m_bDrawing=TRUE;
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		HObject hoROI,hoSelectRegion;
		m_pParentDlg->m_Wnd_SecondImage.DispImage(m_pParentDlg->m_hoSecondImage);
		DrawRectangle1(m_pParentDlg->m_Wnd_SecondImage.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
		GenRectangle1(&hoROI,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
		m_pParentDlg->FindCoatRegion(m_pParentDlg->m_hoSecondImage,hoROI,&hoSelectRegion,m_EDT_nSecondCoatLowThresh,m_EDT_nSecondCoatHighThresh);
		if (hoSelectRegion.CountObj()>0)
		{
			m_pParentDlg->m_Wnd_SecondImage.DispImage(m_pParentDlg->m_hoSecondImage);
			m_pParentDlg->m_Wnd_SecondImage.DispRegion(hoSelectRegion,"yellow","margin",2,TRUE);
			HTuple hv_LeftColumn,hv_RightColumn,hv_LeftRow,hv_RightRow;
			SmallestRectangle1(hoSelectRegion,&hv_LeftRow,&hv_LeftColumn,&hv_RightRow,&hv_RightColumn);
			SetTposition(m_pParentDlg->m_Wnd_SecondImage.GetDispWnd(),50,50);
			WriteString(m_pParentDlg->m_Wnd_SecondImage.GetDispWnd(),"寻边成功");
			g_tCreateBaseLineInfo.nSecondLeftPosition=hv_LeftColumn;
			g_tCreateBaseLineInfo.nSecondRightPosition=hv_RightColumn;

			g_tCreateBaseLineInfo.nSecondLeftPositionRow=hv_LeftRow;
			g_tCreateBaseLineInfo.nSecondRightPositionRow=hv_RightRow;

		}
		m_pParentDlg->m_Wnd_SecondImage.m_bDrawing=FALSE;
	}catch(HException& e)
	{

	}





}


void CPageBaseLine::OnBnClickedBtnOnekey()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (IDOK==MessageBox(_T("一键发送基准后每个相机的线的位置会发生变化，确认需要一键发送基准吗！"),_T("警告!"),MB_ICONINFORMATION|MB_OKCANCEL|MB_SYSTEMMODAL))
	{

		HTuple hv_FistPosition,hv_SecondPosition;
		//依次放入了左侧边界 右侧边界 正面间歇高度 反面间歇高度四个值 左右边界是int 间歇是double
		hv_FistPosition.Append(g_tCreateBaseLineInfo.nFirstLeftPosition).Append(g_tCreateBaseLineInfo.nFirstRightPosition).Append(g_Jugde[0].m_tStandardSize.dUpIntermissionHeight).Append(g_Jugde[0].m_tStandardSize.dDownIntermissionHeight);
		hv_SecondPosition.Append(g_tCreateBaseLineInfo.nSecondLeftPosition).Append(g_tCreateBaseLineInfo.nSecondRightPosition).Append(g_Jugde[0].m_tStandardSize.dUpIntermissionHeight).Append(g_Jugde[0].m_tStandardSize.dDownIntermissionHeight);

		for (int i=0;i<g_nGrabberCount;i++)
		{
			if (i==0)
			{
				g_vec_Algorithm[i]->DoSomething("SETPARAM","ProductLocation",&hv_FistPosition);
			}else
			{
				g_vec_Algorithm[i]->DoSomething("SETPARAM","ProductLocation",&hv_SecondPosition);
			}
		}
		///分给定位新的间歇功能
			HTuple hv_PosiontionInfo;
			int nMiddleCenterPosition=(g_tCreateBaseLineInfo.nFirstLeftPosition+g_tCreateBaseLineInfo.nFirstRightPosition)/2;
			hv_PosiontionInfo.Append(nMiddleCenterPosition).Append(g_Jugde[0].m_tStandardSize.dUpIntermissionHeight);//位置 间歇高度 
			g_vec_FindMark[0]->DoSomething("SETPARAM","ModelPosition",&hv_PosiontionInfo);
			g_vec_FindMark[1]->DoSomething("SETPARAM","ModelPosition",&hv_PosiontionInfo);



	}
}





void CPageBaseLine::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	g_tCreateBaseLineInfo.nFirstCoatLowThresh=m_EDT_nFirstCoatLowThresh;
	g_tCreateBaseLineInfo.nFirstCoatHighThresh=m_EDT_nFirstCoatHighThresh;
	g_tCreateBaseLineInfo.nFirstFoilLowThresh=m_EDT_nFirstFoilLowThresh;
	g_tCreateBaseLineInfo.nFirstFoilHighThresh=m_EDT_nFirstFoilHighThresh;

	g_tCreateBaseLineInfo.nSecondCoatLowThresh=m_EDT_nSecondCoatLowThresh;
	g_tCreateBaseLineInfo.nSecondCoatHighThresh=m_EDT_nSecondCoatHighThresh;
	g_tCreateBaseLineInfo.nSecondFoilLowThresh=m_EDT_nSecondFoilLowThresh;
	g_tCreateBaseLineInfo.nSecondFoilHighThresh=m_EDT_nSecondFoilHighThresh;

	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileInt("CreateBaseLineInfo","nFirstCoatLowThresh",g_tCreateBaseLineInfo.nFirstCoatLowThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nFirstCoatHighThresh",g_tCreateBaseLineInfo.nFirstCoatHighThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nFirstFoilLowThresh",g_tCreateBaseLineInfo.nFirstFoilLowThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nFirstFoilHighThresh",g_tCreateBaseLineInfo.nFirstFoilHighThresh,g_strConfigSystemFile);

	WritePrivateProfileInt("CreateBaseLineInfo","nSecondCoatLowThresh",g_tCreateBaseLineInfo.nSecondCoatLowThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nSecondCoatHighThresh",g_tCreateBaseLineInfo.nSecondCoatHighThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nSecondFoilLowThresh",g_tCreateBaseLineInfo.nSecondFoilLowThresh,g_strConfigSystemFile);
	WritePrivateProfileInt("CreateBaseLineInfo","nSecondFoilHighThresh",g_tCreateBaseLineInfo.nSecondFoilHighThresh,g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式

}


void CPageBaseLine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//CRect rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect,RGB(255,255,255)); //设置为绿色背景
}
