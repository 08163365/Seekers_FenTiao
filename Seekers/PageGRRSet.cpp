// PageGRRSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "PageGRRSet.h"
#include "afxdialogex.h"

// CPageGRRSet 对话框

IMPLEMENT_DYNAMIC(CPageGRRSet, CDialogEx)

CPageGRRSet::CPageGRRSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageGRRSet::IDD, pParent)
	, m_EDT_StrFilePath(_T(""))
{

	m_EDT_nGRRSampleNum = 0;
	m_COM_nCameraIndex = 0;
	m_EDT_nFLPLeftColumn = 0;
	m_EDT_nFLPLimistSize = 0;
	m_EDT_nFLPRightColumn = 0;
	m_EDT_nYPGroundHighThresh = 0;
	m_EDT_nYPGroundLowThresh = 0;
	m_EDT_nYPLeftColumn = 0;
	m_EDT_nYPRightColumn = 0;
	m_hvYPArea[0].Clear();
	m_hvYPArea[1].Clear();
}

CPageGRRSet::~CPageGRRSet()
{
}

void CPageGRRSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_GRRSAMPLENUM, m_EDT_nGRRSampleNum);
	DDX_Check(pDX, IDC_CHK_ONEKEYCLOSE, m_CHK_OneKeyGRRData);
	DDX_CBIndex(pDX, IDC_COM_CAMERAINDEX, m_COM_nCameraIndex);
	DDX_Text(pDX, IDC_EDT_FLPLEFTCOLUMN, m_EDT_nFLPLeftColumn);
	DDX_Text(pDX, IDC_EDT_FLPLIMITSIZE, m_EDT_nFLPLimistSize);
	DDX_Text(pDX, IDC_EDT_FLPRIGHTCOLUMN, m_EDT_nFLPRightColumn);
	DDX_Text(pDX, IDC_EDT_YPGROUNDHIGHTHRESH, m_EDT_nYPGroundHighThresh);
	DDV_MinMaxInt(pDX, m_EDT_nYPGroundHighThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_YPGROUNDLOWTHRESH, m_EDT_nYPGroundLowThresh);
	DDV_MinMaxInt(pDX, m_EDT_nYPGroundLowThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_YPLEFTCOLUMN, m_EDT_nYPLeftColumn);
	DDX_Text(pDX, IDC_EDT_YPRIGHTCOLUMN, m_EDT_nYPRightColumn);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_SAMPLEFILE, m_EDT_StrFilePath);
}


BEGIN_MESSAGE_MAP(CPageGRRSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SETGRRPARAM, &CPageGRRSet::OnBnClickedBtnSetgrrparam)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_OPENIMAGE, &CPageGRRSet::OnBnClickedBtnOpenimage)
	ON_CBN_SELCHANGE(IDC_COM_CAMERAINDEX, &CPageGRRSet::OnSelchangeComCameraindex)
	ON_BN_CLICKED(IDC_BTN_SAVEPARAM2, &CPageGRRSet::OnBnClickedBtnSaveparam2)
	ON_BN_CLICKED(IDC_BTN_TEST, &CPageGRRSet::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_AUTOTEST, &CPageGRRSet::OnBnClickedBtnAutotest)
END_MESSAGE_MAP()


// CPageGRRSet 消息处理程序


void CPageGRRSet::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CDialogEx::PostNcDestroy();
}
void CPageGRRSet::ResetSize()
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


void CPageGRRSet::OnBnClickedBtnSetgrrparam()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_EDT_nGRRSampleNum==0)
	{
		AfxMessageBox("取样点数不得为0，请重新设置");
		return;
	}
	for (int i=0;i<g_nGrabberCount;i++)
	{
		int nOffControl=m_CHK_OneKeyGRRData;
		HTuple hv_SetTuple;
		hv_SetTuple.Clear();
		hv_SetTuple.Append(m_CHK_OneKeyGRRData);
		hv_SetTuple.Append(m_EDT_nGRRSampleNum);
		g_vec_Algorithm[i]->DoSomething("SETPARAM","GRRDataOff",&hv_SetTuple);
	}
}


void CPageGRRSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//CRect rect;
	//GetClientRect(rect);
	//dc.FillSolidRect(rect,RGB(255,255,255)); //设置为绿色背景
}


BOOL CPageGRRSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_COM_nCameraIndex=0;
	m_EDT_nFLPLeftColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPLeftColumn;
	m_EDT_nFLPRightColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPRightColumn;
	m_EDT_nFLPLimistSize=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPSizeLimit;
	m_EDT_nYPLeftColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLeftColumn;
	m_EDT_nYPRightColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPRightColumn;
	m_EDT_nYPGroundLowThresh=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLowThresh;
	m_EDT_nYPGroundHighThresh=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPHighThresh;
	m_EDT_StrFilePath=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.strGRRImagePath;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CPageGRRSet::FlushSpotParam(int& nIndex)
{
	UpdateData(TRUE);
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nFLPLeftColumn=m_EDT_nFLPLeftColumn;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nFLPRightColumn=m_EDT_nFLPRightColumn;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nFLPSizeLimit=m_EDT_nFLPLimistSize ;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nYPLeftColumn=m_EDT_nYPLeftColumn;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nYPRightColumn=m_EDT_nYPRightColumn;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nYPLowThresh=m_EDT_nYPGroundLowThresh;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.nYPHighThresh=m_EDT_nYPGroundHighThresh;
	g_CAutoGRR[nIndex].m_tGRRParamInfo.strGRRImagePath=m_EDT_StrFilePath;

}

void CPageGRRSet::OnBnClickedBtnOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
	HTuple Width,Height;
	if(IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName(); 
		try
		{

			if (m_COM_nCameraIndex==0)
			{
				ReadImage(&m_pParentDlg->m_hoFirstImage,(HTuple)strFilename);
				m_pParentDlg->m_Wnd_FirstImage.DispImage(m_pParentDlg->m_hoFirstImage);
			
			}else
			{
				ReadImage(&m_pParentDlg->m_hoSecondImage,(HTuple)strFilename);
				m_pParentDlg->m_Wnd_SecondImage.DispImage(m_pParentDlg->m_hoSecondImage);
			}	
		}catch(HException &e )
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

	}
}





void CPageGRRSet::OnSelchangeComCameraindex()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	int nIndex=!m_COM_nCameraIndex;
	FlushSpotParam(nIndex);



	m_EDT_nFLPLeftColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPLeftColumn;
	m_EDT_nFLPRightColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPRightColumn;
	m_EDT_nFLPLimistSize=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPSizeLimit;
	m_EDT_nYPLeftColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLeftColumn;
	m_EDT_nYPRightColumn=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPRightColumn;
	m_EDT_nYPGroundLowThresh=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLowThresh;
	m_EDT_nYPGroundHighThresh=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPHighThresh;
	m_EDT_StrFilePath=g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.strGRRImagePath;

	UpdateData(FALSE);






}


void CPageGRRSet::OnBnClickedBtnSaveparam2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPLeftColumn=m_EDT_nFLPLeftColumn;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPRightColumn=m_EDT_nFLPRightColumn;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nFLPSizeLimit=m_EDT_nFLPLimistSize ;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLeftColumn=m_EDT_nYPLeftColumn;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPRightColumn=m_EDT_nYPRightColumn;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPLowThresh=m_EDT_nYPGroundLowThresh;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nYPHighThresh=m_EDT_nYPGroundHighThresh;
	g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.strGRRImagePath=m_EDT_StrFilePath;


	for (int k=0;k<g_nGrabberCount;k++)
	{
		g_CAutoGRR[k].WriteParam();
	}
}


void CPageGRRSet::OnBnClickedBtnTest()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		UpdateData(TRUE);
		FlushSpotParam(m_COM_nCameraIndex);

	
		HObject ho_Image;
		HTuple hv_Width, hv_Height;
		HTuple hvPointNum,hv_RepeatArea;
		HObject hoFLPRectangle,hoFLPObj;
		HObject ho_YPRegions;
		HTuple hv_WindowID;
		hoFLPObj.GenEmptyObj();
		ho_YPRegions.GenEmptyObj();
		if (m_COM_nCameraIndex==0)
		{
			ho_Image=m_pParentDlg->m_hoFirstImage;
			hv_WindowID=m_pParentDlg->m_Wnd_FirstImage.GetDispWnd();
		}else
		{
			ho_Image=m_pParentDlg->m_hoSecondImage;
			hv_WindowID=m_pParentDlg->m_Wnd_SecondImage.GetDispWnd();
		}
		GetImageSize(ho_Image, &hv_Width, &hv_Height);
		g_CAutoGRR[m_COM_nCameraIndex].GetFLPDetectInfo(ho_Image,&hoFLPRectangle,&hoFLPObj,hv_Height,&hvPointNum);
		g_CAutoGRR[m_COM_nCameraIndex].GetYPInfo(ho_Image, &ho_YPRegions, hv_Height, &hv_RepeatArea);
		m_hvYPArea[m_COM_nCameraIndex].Append(hv_RepeatArea);
		HObject hoConCatRegions,hoUnionRegion;
		hoConCatRegions.GenEmptyObj();
		
		ConcatObj(hoFLPObj,ho_YPRegions,&hoConCatRegions);
		Union1(hoConCatRegions,&hoUnionRegion);
		if (hvPointNum.Length()==3 && (hvPointNum[0]+hvPointNum[2]==11))
		g_CAutoGRR[m_COM_nCameraIndex].WriteFLPImage(hoUnionRegion,ho_Image);
		else
		g_CAutoGRR[m_COM_nCameraIndex].WriteFLPImage(hoUnionRegion,ho_Image,0);

		DispObj(ho_Image,hv_WindowID);
		if (hoFLPRectangle.CountObj()>0)
		{
			SetColor(hv_WindowID,"green");
			DispObj(hoFLPRectangle,hv_WindowID);

		}
		if (hoFLPObj.CountObj()>0)
		{
			SetColor(hv_WindowID,"red");
			DispObj(hoFLPObj,hv_WindowID);

		}
		if (ho_YPRegions.CountObj()>0)
		{
			SetColor(hv_WindowID,"blue");
			DispObj(ho_YPRegions,hv_WindowID);
			HTuple hvTempRow,hv_TempColumn,hv_TempArea;
			AreaCenter(ho_YPRegions,&hv_TempArea,&hvTempRow,&hv_TempColumn);
			HTuple hv_CurrentTuple,hv_BaseTuple;
			TupleLastN(m_hvYPArea[m_COM_nCameraIndex], m_hvYPArea[m_COM_nCameraIndex].Length()-10,&hv_CurrentTuple);
			TupleFirstN(m_hvYPArea[m_COM_nCameraIndex],9,&hv_BaseTuple);
			CString strValue;
			if(hv_BaseTuple.Length()==hv_CurrentTuple.Length())
			{
				for (int i=0;i<hvTempRow.Length();i++)
				{
					SetTposition(hv_WindowID,hvTempRow[i]+100,hv_TempColumn[i]-200);
					strValue.Format("%.3fmm^2",hv_BaseTuple[i].D()+(hv_CurrentTuple[i].D()-hv_BaseTuple[i].D())/ MAX(g_CAutoGRR[m_COM_nCameraIndex].m_tGRRParamInfo.nScaleFactor,1));
					WriteString(hv_WindowID,(HTuple)strValue);

				}
			}


		}

	}catch(HException& e)
	{

	}


}


void CPageGRRSet::OnBnClickedBtnAutotest()
{
	// TODO: 在此添加控件通知处理程序代码
	if(g_dRealSpeed>0)
		return;
	UpdateData(TRUE);
	FlushSpotParam(m_COM_nCameraIndex);
	g_CAutoGRR[m_COM_nCameraIndex].Action();
}
