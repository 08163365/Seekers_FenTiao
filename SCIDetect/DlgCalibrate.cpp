// DlgCalibrate.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCalibrate.h"
#include "afxdialogex.h"


// CDlgCalibrate 对话框

IMPLEMENT_DYNAMIC(CDlgCalibrate, CDialogEx)

CDlgCalibrate::CDlgCalibrate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCalibrate::IDD, pParent)

	, m_EDT_dCCDValue(0)
	, m_EDT_dManualValue(0)
{
	m_pOwner = NULL;
	m_EDT_dCalSpaceDist =5;
	m_EDT_nCalThresh = 30;
	m_EDT_dCoefficient = 1;
	m_hoCross.GenEmptyObj();
}

CDlgCalibrate::~CDlgCalibrate()
{
}

void CDlgCalibrate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_CALIB, m_BTN_Calibration);
	DDX_Control(pDX, IDC_BTN_OPENIMAGE, m_BTN_OpenImage);
	DDX_Text(pDX, IDC_EDT_CALIBSPACE, m_EDT_dCalSpaceDist);
	DDX_Text(pDX, IDC_EDT_CALIBTHRESH, m_EDT_nCalThresh);
	DDX_Text(pDX, IDC_EDT_MODIFYVALUE, m_EDT_dCoefficient);
	DDX_Control(pDX, IDC_STC_IMAGE, m_STC_Image);
	//  DDX_Control(pDX, IDC_LST_COLRESOLUTION, m_m_LC_ColResolution);
	DDX_Control(pDX, IDC_STC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_STC_GRAVALUE, m_STC_GrayValue);
	DDX_Control(pDX, IDC_LST_COLRESOLUTION, m_LC_ColResolution);
	DDX_Control(pDX, IDCANCEL, m_BTN_CACEL);
	DDX_Control(pDX, IDOK, m_BTN_SAVEPARAM);
	DDX_Text(pDX, IDC_EDT_CCDVALUE, m_EDT_dCCDValue);
	DDX_Text(pDX, IDC_EDT_MANUALVALUE, m_EDT_dManualValue);
}


BEGIN_MESSAGE_MAP(CDlgCalibrate, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCalibrate::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCalibrate::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_OPENIMAGE, &CDlgCalibrate::OnBnClickedBtnOpenimage)
	ON_BN_CLICKED(IDC_BTN_CALIB, &CDlgCalibrate::OnBnClickedBtnCalib)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_REFRESHKVALUE, &CDlgCalibrate::OnBnClickedBtnRefreshkvalue)
END_MESSAGE_MAP()



BOOL CDlgCalibrate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	CRect rcImage;
	m_STC_Image.GetClientRect(rcImage);
	SetCheck("~father");
	OpenWindow(rcImage.top,rcImage.left,rcImage.Width(),rcImage.Height(),(Hlong)m_STC_Image.m_hWnd,"visible","",&m_hvWindowID);
	SetCheck("father");
	SetDraw(m_hvWindowID, "margin");
	SetColor(m_hvWindowID, "red");
	SetColored(m_hvWindowID,12);
	HTuple Width,Height;

	GetImageSize(m_hoImage,&Width,&Height);
	SetPart(m_hvWindowID,0,0,Height-1,Width-1);
	DispObj(m_hoImage,m_hvWindowID);

	m_dDispImageStartRow= 0;
	m_dDispImageStartCol = 0;
	m_dDispImageEndRow = Height - 1;
	m_dDispImageEndCol = Width - 1;

	//初始化List
	InitList();
	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgCalibrate::SetButtonColcor(CButtonExt& btn,short	shBtnColor,COLORREF BKOUT,
	COLORREF BKIN,COLORREF BKFOCUS,COLORREF FG)
{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	//btn.SetFont(&font);
}
void CDlgCalibrate::InitUI()
{
	COLORREF MainBKColor = g_Blendent.m_Blendent.Dlg.MainColor;
	m_BtnFont.CreatePointFont(100,"Microsoft YaHei");

	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainBKColor,0,CStaticExt::Normal);

	m_GrayValueFont.CreateFontA(20,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"宋体");
	m_STC_GrayValue.SetFont(&m_GrayValueFont);
	//m_STC_GrayValue.SetBkColor(RGB(255,255,255),0,CStaticExt::Normal);

	//Button控件字体和颜色
	short	 shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

	SetButtonColcor(m_BTN_Calibration,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_OpenImage,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_CACEL,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);
	SetButtonColcor(m_BTN_SAVEPARAM,shBtnColor,BKOUT,BKIN,BKFOCUS,FG);

	m_BTN_Calibration.SetFont(&m_BtnFont);
	m_BTN_OpenImage.SetFont(&m_BtnFont);
	m_BTN_CACEL.SetFont(&m_BtnFont);
	m_BTN_SAVEPARAM.SetFont(&m_BtnFont);
}
void CDlgCalibrate::InitList()
{
	m_LC_ColResolution.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);
	m_LC_ColResolution.InsertColumn(0,"序号",LVCFMT_LEFT, 100);
	m_LC_ColResolution.InsertColumn(1,"像素坐标值",LVCFMT_LEFT, 100);
	m_LC_ColResolution.InsertColumn(2,"像素差值",LVCFMT_LEFT, 100);
	m_LC_ColResolution.InsertColumn(3,"物理坐标差值",LVCFMT_LEFT, 100);

	UpdateData(FALSE);
}
void CDlgCalibrate::WriteParam()
{
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
void CDlgCalibrate::FreshImage()
{
	SetColored(m_hvWindowID,12);
	DispObj(m_hoImage,m_hvWindowID);
	SetColor(m_hvWindowID,"green");
	DispObj(m_hoCross,m_hvWindowID);
}
// CDlgCalibrate 消息处理程序


void CDlgCalibrate::OnBnClickedOk()
{
	WriteParam();
	
}


void CDlgCalibrate::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}

void CDlgCalibrate::OnBnClickedBtnOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
	if(IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_hoImage,HTuple(strFilename));
			HTuple Width,Height;
			GetImageSize(m_hoImage,&Width,&Height);
			SetPart(m_hvWindowID,0,0,Height-1,Width-1);
			DispObj(m_hoImage,m_hvWindowID);
		}
		catch (HException)
		{
			AfxMessageBox("读取图片失败");
		}

	}

}


//void CDlgCalibrate::OnBnClickedBtnCalib()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	try
//	{
//		HTuple  dWidth,dHeight;
//		HTuple  hvCount,hvColPose,hvSegResolution,hvPow;
//		HObject hoRect,hoImage,hoRegionTemp;
//		HTuple  dUnitWidth,hv_Height;
//		hoImage=m_hoImage;
//		GetImageSize(m_hoImage,NULL,&hv_Height);
//		Threshold(hoImage,&hoRegionTemp,0,HTuple(m_EDT_nCalThresh));
//		Connection(hoRegionTemp,&hoRegionTemp);
//		ClosingRectangle1(hoRegionTemp,&hoRegionTemp,1,hv_Height/3);
//		SelectShape(hoRegionTemp,&hoRegionTemp,"area","and",5000,999999);//去除噪点影响
//		SortRegion(hoRegionTemp,&hoRegionTemp,"first_point","true","column");
//		CountObj(hoRegionTemp,&hvCount);
//
//		if (hvCount.I()>10)
//		{
//
//			CopyObj(hoRegionTemp,&hoRegionTemp,2,hvCount-2);
//			RegionFeatures(hoRegionTemp,"column1",&hvColPose);
//			m_hvColPose.Clear();
//			m_hvXPose.Clear();
//			for (int i=0;i<hvColPose.Length();i++)
//			{
//				m_hvXPose.Append(i*2*m_EDT_dCalSpaceDist);
//			}
//			m_hvColPose=hvColPose;
//			m_nPoseCount=hvCount.I()-2;
//			SetColored(m_hvWindowID,12);
//			DispObj(hoRegionTemp,m_hvWindowID);
//			CString strMessage;
//			strMessage.Format("%s:共有%d段","标定成功",m_nPoseCount);
//			disp_message(m_hvWindowID,HTuple(strMessage),"window",12,12,"green","false");
//			FreshParam();
//		}else
//		{
//			disp_message(m_hvWindowID,HTuple("标定图片异常！"),"window",12,12,"red","false");
//		}
//
//	}
//	catch (HException)
//	{
//		disp_message(m_hvWindowID,HTuple("标定异常！"),"window",12,12,"red","false");
//
//	}
//}
//
void CDlgCalibrate::SetButtonStaus(BOOL bStaus)
{
	HWND  hwndChild = ::GetWindow(GetSafeHwnd(), GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		if (IDC_STC_IMAGE!=woc && IDC_LST_COLRESOLUTION!=woc)
		{
			pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
		
		}
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
		
	}
}

void CDlgCalibrate::OnBnClickedBtnCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetButtonStaus(FALSE);
	try
	{
		HTuple  dWidth;
		HTuple  hvCount,hvColPose,hvSegResolution,hvPow;
		HObject hoRect,hoImage,hoRegionTemp;
		HTuple  dUnitWidth,hv_Height;
		hoImage=m_hoImage;
		/////////////////////////////////////////////////////
		DispObj(hoImage, m_hvWindowID);
		HTuple hvHeight,hv_Width;
		HObject Rectangle1,Rectangle2;
		HTuple Row1, Row2, Column1, Column2;
		HTuple Row, Column, Phi, Length1, Length2, MeasureHandle;
		SetColor(m_hvWindowID, "red");
		GetImageSize(m_hoImage,&hv_Width,&hvHeight);
		DrawRectangle1(m_hvWindowID, &Row1, &Column1, &Row2, &Column2);
		GenRectangle1(&Rectangle1, Row1, Column1, Row2, Column2);
		GenRectangle1(&Rectangle2, max(0, (hvHeight - 1) / 2 -200), Column1, min(hvHeight-1,(hvHeight - 1)/2 + 200), Column2);

		AreaCenter(Rectangle1, NULL, &Row, &Column);
		SmallestRectangle2(Rectangle1, &Row, &Column, &Phi, &Length1, &Length2);
		GenMeasureRectangle2(Row, Column, Phi, Length1, Length2, hv_Width, hvHeight, "nearest_neighbor", &MeasureHandle);

		HTuple hvRowEdge,hvColumnEdge,hvAmplitude,hvDistance;
		try
		{
			MeasurePos(m_hoImage, MeasureHandle, 2, m_EDT_nCalThresh, "negative", "all", &hvRowEdge, &hvColumnEdge, &hvAmplitude, &hvDistance);

			HTuple hvRowEdge1, hvColumnEdge1;
			MeasurePos(m_hoImage, MeasureHandle, 2, m_EDT_nCalThresh, "positive", "all", &hvRowEdge1, &hvColumnEdge1, &hvAmplitude, &hvDistance);

			if (hvColumnEdge.Length()==hvColumnEdge1.Length() && hvColumnEdge[0]<hvColumnEdge1[0])
			{
				hvColumnEdge=(hvColumnEdge+hvColumnEdge1)/2;
			}else
			{
				if (hvColumnEdge.Length()>5 && hvColumnEdge1.Length()>5)
				{
					if (hvColumnEdge[0]<hvColumnEdge1[0])
					{
						int nLength=MIN(hvColumnEdge.Length(),hvColumnEdge1.Length());

						TupleSelectRange(hvColumnEdge,0,nLength-1,&hvColumnEdge);
						TupleSelectRange(hvColumnEdge1,0,nLength-1,&hvColumnEdge1);
						TupleSelectRange(hvRowEdge,0,nLength-1,&hvRowEdge);
						hvColumnEdge=(hvColumnEdge+hvColumnEdge1)/2;
					}else
					{
						TupleRemove(hvColumnEdge1,0,&hvColumnEdge1);
						TupleRemove(hvRowEdge1,0,&hvRowEdge1);
						int nLength=MIN(hvColumnEdge.Length(),hvColumnEdge1.Length());
						TupleSelectRange(hvColumnEdge,0,nLength-1,&hvColumnEdge);
						TupleSelectRange(hvColumnEdge1,0,nLength-1,&hvColumnEdge1);
						TupleSelectRange(hvRowEdge,0,nLength-1,&hvRowEdge);
						hvColumnEdge=(hvColumnEdge+hvColumnEdge1)/2;

					}

				}else
				{
					AfxMessageBox("左右边请选择全部黑条");
				}
				
			}
		

			if (hvRowEdge.Length()>5)
			{

				GenCrossContourXld(&m_hoCross, hvRowEdge, hvColumnEdge, 80, 0);
				double dWidth = m_EDT_dCalSpaceDist*2;
				m_hvColPose=hvColumnEdge;;
				double dEnd = dWidth * (m_hvColPose.Length() - 1);
				TupleGenSequence(0, dEnd, dWidth, &m_hvXPose);

				HTuple Selected, Selected1, Selected2, Selected3, Diff, Diff1;
				TupleSelectRange(m_hvColPose, 0, m_hvColPose.Length() - 2, &Selected);
				TupleSelectRange(m_hvColPose, 1, m_hvColPose.Length() - 1, &Selected1);
				TupleSub(Selected1, Selected, &Diff);

				TupleSelectRange(m_hvXPose, 0, m_hvXPose.Length() - 2, &Selected2);
				TupleSelectRange(m_hvXPose, 1, m_hvXPose.Length() - 1, &Selected3);
				TupleSub(Selected3, Selected2, &Diff1);
				m_hvRes = Diff1 / Diff;

				m_nPoseCount=m_hvColPose.Length();
				SetColor(m_hvWindowID,"green");
				DispObj(m_hoCross,m_hvWindowID);
				CString strMessage;
				strMessage.Format("%s:共有%d段 实际位置%d段  分辨率:%d","标定成功",m_nPoseCount,m_hvXPose.Length(),m_hvRes.Length());
				disp_message(m_hvWindowID,HTuple(strMessage),"window",12,12,"green","false");
				FreshParam();
			}else
			{
				disp_message(m_hvWindowID,HTuple("标定图片异常！"),"window",12,12,"red","false");
			}
			CloseMeasure(MeasureHandle);
		}catch(HException& e)
		{
			CloseMeasure(MeasureHandle);

		}

	
	}
	catch (HException)
	{
		disp_message(m_hvWindowID,HTuple("标定异常！"),"window",12,12,"red","false");

	}
		SetButtonStaus(TRUE);
}



// Procedures 
// Local procedures 
void CDlgCalibrate::ConicComp (HTuple hv_Xdata, HTuple hv_Ydata, HTuple hv_nLen, HTuple *hv_na, 
    HTuple *hv_nb, HTuple *hv_nc, HTuple *hv_R2, HTuple *hv_Ycomp)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_atemp, hv_nCoff, hv_dSwit, hv_nI, hv_nJ;
  HTuple  hv_Pow, hv_nK, hv_Pow1, hv_dk, hv_column, hv_mainelement;
  HTuple  hv_Abs, hv_dTmp, hv_bTmp, hv_dMik, hv_dSum, hv_dPow2;
  HTuple  hv_dSubPow, hv_Mean;

  TupleGenConst(5, 0, &hv_atemp);
  TupleGenConst(3, 0, &hv_nCoff);
  TupleGenConst(9, 0, &hv_dSwit);

  {
  HTuple end_val4 = hv_nLen-1;
  HTuple step_val4 = 1;
  for (hv_nI=0; hv_nI.Continue(end_val4, step_val4); hv_nI += step_val4)
  {

    {
    HTuple end_val6 = 3-1;
    HTuple step_val6 = 1;
    for (hv_nJ=0; hv_nJ.Continue(end_val6, step_val6); hv_nJ += step_val6)
    {
      TuplePow(HTuple(hv_Xdata[hv_nI]), hv_nJ, &hv_Pow);
      hv_nCoff[hv_nJ] = HTuple(hv_nCoff[hv_nJ])+(hv_Pow*HTuple(hv_Ydata[hv_nI]));
    }
    }
    {
    HTuple end_val10 = 5-1;
    HTuple step_val10 = 1;
    for (hv_nK=0; hv_nK.Continue(end_val10, step_val10); hv_nK += step_val10)
    {
      TuplePow(HTuple(hv_Xdata[hv_nI]), hv_nK, &hv_Pow1);
      hv_atemp[hv_nK] = HTuple(hv_atemp[hv_nK])+hv_Pow1;
    }
    }
  }
  }
  hv_atemp[0] = hv_nLen;
  {
  HTuple end_val16 = 3-1;
  HTuple step_val16 = 1;
  for (hv_nI=0; hv_nI.Continue(end_val16, step_val16); hv_nI += step_val16)
  {
    ////构建线性方程组系数矩阵，b[]不变
    hv_dk = hv_nI;
    {
    HTuple end_val19 = 3-1;
    HTuple step_val19 = 1;
    for (hv_nJ=0; hv_nJ.Continue(end_val19, step_val19); hv_nJ += step_val19)
    {
      hv_dSwit[(hv_nI*3)+hv_nJ] = HTuple(hv_atemp[hv_dk]);
      hv_dk += 1;
    }
    }
  }
  }
  ////以下为高斯列主元消去法解线性方程组
  {
  HTuple end_val25 = 2-1;
  HTuple step_val25 = 1;
  for (hv_nK=0; hv_nK.Continue(end_val25, step_val25); hv_nK += step_val25)
  {
    hv_column = hv_nK;
    hv_mainelement = HTuple(hv_dSwit[(hv_nK*3)+hv_nK]);
    {
    HTuple end_val28 = 3-1;
    HTuple step_val28 = 1;
    for (hv_nI=hv_nK; hv_nI.Continue(end_val28, step_val28); hv_nI += step_val28)
    {
      TupleFabs(HTuple(hv_dSwit[(hv_nK*3)+hv_nK]), &hv_Abs);
      if (0 != (int(hv_Abs>hv_mainelement)))
      {
        hv_mainelement = hv_Abs;
        hv_column = hv_nI;
      }
    }
    }
    {
    HTuple end_val35 = 3-1;
    HTuple step_val35 = 1;
    for (hv_nJ=hv_nK; hv_nJ.Continue(end_val35, step_val35); hv_nJ += step_val35)
    {
      hv_dTmp = HTuple(hv_dSwit[(hv_nK*3)+hv_nJ]);
      hv_dSwit[(hv_nK*3)+hv_nJ] = HTuple(hv_dSwit[(hv_column*3)+hv_nJ]);
      hv_dSwit[(hv_column*3)+hv_nJ] = hv_dTmp;
    }
    }
    hv_bTmp = HTuple(hv_nCoff[hv_nK]);
    hv_nCoff[hv_nK] = HTuple(hv_nCoff[hv_column]);
    hv_nCoff[hv_column] = hv_bTmp;
    {
    HTuple end_val43 = 3-1;
    HTuple step_val43 = 1;
    for (hv_nI=hv_nK+1; hv_nI.Continue(end_val43, step_val43); hv_nI += step_val43)
    {
      hv_dMik = HTuple(hv_dSwit[(hv_nI*3)+hv_nK])/HTuple(hv_dSwit[(hv_nK*3)+hv_nK]);
      {
      HTuple end_val45 = 3-1;
      HTuple step_val45 = 1;
      for (hv_nJ=hv_nK; hv_nJ.Continue(end_val45, step_val45); hv_nJ += step_val45)
      {
        hv_dSwit[(hv_nI*3)+hv_nJ] = HTuple(hv_dSwit[(hv_nI*3)+hv_nJ])-(hv_dMik*HTuple(hv_dSwit[(hv_nK*3)+hv_nJ]));
      }
      }
      hv_nCoff[hv_nI] = HTuple(hv_nCoff[hv_nI])-(hv_dMik*HTuple(hv_nCoff[hv_nK]));

    }
    }
  }
  }
  hv_nCoff[2] = HTuple(hv_nCoff[2])/HTuple(hv_dSwit[(3*2)+2]);
  for (hv_nI=1; hv_nI>=0; hv_nI+=-1)
  {
    hv_dSum = 0;
    for (hv_nJ=hv_nI+1; hv_nJ<=2; hv_nJ+=1)
    {
      hv_dSum += HTuple(hv_dSwit[(hv_nI*3)+hv_nJ])*HTuple(hv_nCoff[hv_nJ]);
    }
    hv_nCoff[hv_nI] = (HTuple(hv_nCoff[hv_nI])-hv_dSum)/HTuple(hv_dSwit[(hv_nI*3)+hv_nI]);
  }
  (*hv_nc) = ((const HTuple&)hv_nCoff)[0];
  (*hv_nb) = ((const HTuple&)hv_nCoff)[1];
  (*hv_na) = ((const HTuple&)hv_nCoff)[2];

  //***********计算R2***************
  hv_dPow2 = 0;
  hv_dSubPow = 0;
  TupleMean(hv_Ydata, &hv_Mean);
  {
  HTuple end_val68 = hv_nLen-1;
  HTuple step_val68 = 1;
  for (hv_nI=0; hv_nI.Continue(end_val68, step_val68); hv_nI += step_val68)
  {
    (*hv_Ycomp)[hv_nI] = ((((*hv_na)*HTuple(hv_Xdata[hv_nI]))*HTuple(hv_Xdata[hv_nI]))+((*hv_nb)*HTuple(hv_Xdata[hv_nI])))+(*hv_nc);
    hv_dPow2 += (HTuple(hv_Ydata[hv_nI])-HTuple((*hv_Ycomp)[hv_nI]))*(HTuple(hv_Ydata[hv_nI])-HTuple((*hv_Ycomp)[hv_nI]));
    hv_dSubPow += (hv_Mean-HTuple((*hv_Ycomp)[hv_nI]))*(hv_Mean-HTuple((*hv_Ycomp)[hv_nI]));
  }
  }
  (*hv_R2) = 1-(hv_dPow2/hv_dSubPow);
  // stop(...); only in hdevelop
}

void CDlgCalibrate::GetRParam (HTuple hv_Ydata, HTuple *hv_na, HTuple *hv_nb, HTuple *hv_nc, HTuple *hv_R2, 
    HTuple *hv_Ycomp)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Length, hv_Xdata;
  TupleLength(hv_Ydata, &hv_Length);
  TupleGenSequence(1, hv_Length, 1, &hv_Xdata);

  (*hv_na) = 0;
  (*hv_nb) = 0;
  (*hv_nc) = 0;
  (*hv_R2) = 0;
  TupleGenConst(hv_Length, 0, &(*hv_Ycomp));
  ConicComp(hv_Xdata, hv_Ydata, hv_Length, &(*hv_na), &(*hv_nb), &(*hv_nc), &(*hv_R2), 
      &(*hv_Ycomp));
  return;
}
void CDlgCalibrate::FreshParam()
{
	m_LC_ColResolution.DeleteAllItems();	
	m_LC_ColResolution.DeleteAllColumns();	
	m_LC_ColResolution.InsertColumn(0,"序号",LVCFMT_LEFT, 100);
	m_LC_ColResolution.InsertColumn(1,"像素坐标值",LVCFMT_LEFT, 100);
	m_LC_ColResolution.InsertColumn(2,"像素差值",LVCFMT_LEFT, 50);
	m_LC_ColResolution.InsertColumn(3,"物理坐标值(mm)",LVCFMT_LEFT, 50);
	m_LC_ColResolution.InsertColumn(4,"分辨率",LVCFMT_LEFT, 80);
	m_LC_ColResolution.InsertColumn(5,"分辨率千分比",LVCFMT_LEFT, 80);
	CString strProcName,strSinglePixelPose;
	int nItemCount;
	HTuple  hv_PartResolution;

	for (int i=0;i<m_nPoseCount;i++)
	{
		nItemCount=m_LC_ColResolution.GetItemCount();
		strProcName.Format("像素坐标%d",i);
		m_LC_ColResolution.InsertItem(nItemCount,strProcName);
		strSinglePixelPose.Format("%.f",m_hvColPose[i].D());
		m_LC_ColResolution.SetItemText(nItemCount,1,strSinglePixelPose);
		m_LC_ColResolution.SetItemText(nItemCount,2,m_hvColPose[i].D()-m_hvColPose[MAX(i-1,0)].D());
		m_LC_ColResolution.SetItemText(nItemCount,3,m_hvXPose[i].D());
		m_LC_ColResolution.SetItemText(nItemCount,4,(m_hvXPose[i].D()-m_hvXPose[MAX(i-1,0)].D())/MAX(1,m_hvColPose[i].D()-m_hvColPose[MAX(i-1,0)].D()));
		if (i!=0)
		{
			CString strValue1,strValue2,strValue;
			hv_PartResolution.Append((m_hvXPose[i].D()-m_hvXPose[MAX(i-1,0)].D())/MAX(1,m_hvColPose[i].D()-m_hvColPose[MAX(i-1,0)].D()));
			
			if (nItemCount>1)
			{
				strValue2=m_LC_ColResolution.GetItemText(nItemCount-1,4);
				strValue1=m_LC_ColResolution.GetItemText(nItemCount,4);
				strValue.Format("%.2f",fabs((atof(strValue1)-atof(strValue2))/MAX(1,atof(strValue2)))*1000);
				m_LC_ColResolution.SetItemText(nItemCount,5,strValue);
			}else
			{
               m_LC_ColResolution.SetItemText(nItemCount,5,"0");
			}
			
		}else
		{
			m_LC_ColResolution.SetItemText(nItemCount,5,"0");
		}
	}



	if (hv_PartResolution.Length()>0)
	{
		HTuple hv_Na(0),hv_Nb(0),hv_Nc(0),hv_R(0),hv_YComp(0);
		GetRParam(hv_PartResolution,&hv_Na,&hv_Nb,&hv_Nc,&hv_R,&hv_YComp);
		CString strValue;
		strValue.Format("回归系数:%.8f",hv_R.D());
		HTuple hv_Width;
		GetImageSize(m_hoImage,&hv_Width,NULL);
	    SetTposition(m_hvWindowID,200,hv_Width/2);
		WriteString(m_hvWindowID,(HTuple)strValue);
		
		CString str;
		str.Format("%s\\局部分辨率",GetCurrentAppPath());
		HTuple hv_Exist;
		FileExists((HTuple)str,&hv_Exist);
		if (hv_Exist.I()==1)
		{
			DeleteFile((HTuple)str);
		}
		WriteTuple(hv_PartResolution,(HTuple)str);
	}


	//m_LC_ColResolution.SetGridLines(TRUE);
}
BOOL CDlgCalibrate::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		CPoint   pt;
		GetCursorPos(&pt);//获取光标坐标，以屏幕坐标表示
		GetDlgItem(IDC_STC_IMAGE)->ScreenToClient(&pt);//将光标屏幕坐标转换成指定客户区坐标
		CRect WndRect;
		GetDlgItem(IDC_STC_IMAGE)->GetClientRect(&WndRect);
		if (WndRect.PtInRect(pt))
		{
			double	dImagePtX,dImagePtY;
			double	dRow0_1,dCol0_1,dRow1_1,dCol1_1;
			double	dScale=0.1;

			if (zDelta<0)
			{
				dImagePtX=m_dDispImageStartCol+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImageEndCol-m_dDispImageStartCol);
				dImagePtY=m_dDispImageStartRow+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImageEndRow-m_dDispImageStartRow);
				dRow0_1=dImagePtY-1/(1-dScale)*(dImagePtY-m_dDispImageStartRow);
				dRow1_1=dImagePtY-1/(1-dScale)*(dImagePtY-m_dDispImageEndRow);
				dCol0_1=dImagePtX-1/(1-dScale)*(dImagePtX-m_dDispImageStartCol);
				dCol1_1=dImagePtX-1/(1-dScale)*(dImagePtX-m_dDispImageEndCol);
				m_dDispImageStartRow=dRow0_1;
				m_dDispImageStartCol=dCol0_1;
				m_dDispImageEndRow=dRow1_1;
				m_dDispImageEndCol=dCol1_1;
			} 
			else
			{
				dImagePtX=m_dDispImageStartCol+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImageEndCol-m_dDispImageStartCol);
				dImagePtY=m_dDispImageStartRow+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImageEndRow-m_dDispImageStartRow);
				dRow0_1=dImagePtY-1/(1+dScale)*(dImagePtY-m_dDispImageStartRow);
				dRow1_1=dImagePtY-1/(1+dScale)*(dImagePtY-m_dDispImageEndRow);
				dCol0_1=dImagePtX-1/(1+dScale)*(dImagePtX-m_dDispImageStartCol);
				dCol1_1=dImagePtX-1/(1+dScale)*(dImagePtX-m_dDispImageEndCol);
				m_dDispImageStartRow=dRow0_1;
				m_dDispImageStartCol=dCol0_1;
				m_dDispImageEndRow=dRow1_1;
				m_dDispImageEndCol=dCol1_1;
			}
			SetPart(m_hvWindowID, m_dDispImageStartRow, m_dDispImageStartCol, m_dDispImageEndRow, m_dDispImageEndCol);
			SetSystem("flush_graphic","false");
			ClearWindow(m_hvWindowID);
			SetSystem("flush_graphic","true");
			FreshImage();
		}
	}
	catch(HException)
	{

	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CDlgCalibrate::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//屏蔽回车
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)//屏蔽ESC
		{
			return TRUE;
		}else if(pMsg->wParam == VK_SPACE)//屏蔽空格
		{
			return TRUE;
		}
	}
	if (pMsg->message == WM_MBUTTONDOWN)
	{
		CPoint   pt;
		GetCursorPos(&pt);
		GetDlgItem(IDC_STC_IMAGE)->ScreenToClient(&pt);
		CRect WndRect;
		GetDlgItem(IDC_STC_IMAGE)->GetClientRect(&WndRect);

		// 还原显示（不缩放）
		if (WndRect.PtInRect(pt))
		{	
			HTuple tmpWidth,tmpHeight;
			GetImageSize(m_hoImage, &tmpWidth, &tmpHeight);
			m_dDispImageStartRow = 0;
			m_dDispImageStartCol = 0;
			m_dDispImageEndRow = tmpHeight - 1;
			m_dDispImageEndCol = tmpWidth - 1;	
			SetPart(m_hvWindowID, m_dDispImageStartRow, m_dDispImageStartCol, m_dDispImageEndRow, m_dDispImageEndCol);
			ClearWindow(m_hvWindowID);
			FreshImage();
		}
	}
	if (pMsg->message == WM_MOUSEMOVE)
	{
		try
		{
			HTuple Row,Column,Value,Area,Circularity;
			GetMposition(m_hvWindowID,&Row,&Column,NULL);
			GetGrayval(m_hoImage,Row,Column,&Value);
			CString strDisp;
			strDisp.Format("坐标:\r\n \r\nX:%3d Y:%3d\r\n \r\n灰度值: %3d",Column[0].I(),Row[0].I(),Value[0].I());
			m_STC_GrayValue.SetWindowTextA(strDisp);
			return TRUE;
		}catch(HException e)
		{
			CString strDisp;
			strDisp.Format("坐标:\r\n \r\nX:-- Y:--\r\n \r\n灰度值: --");
			m_STC_GrayValue.SetWindowTextA(strDisp);
		}

	}
	if (pMsg->message == WM_MOUSEWHEEL)
	{
		short zDelta = HIWORD(pMsg->wParam);
		UINT nFlags  = LOWORD(pMsg->wParam);
		OnMouseWheel(nFlags,zDelta,pMsg->pt);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgCalibrate::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CDlgCalibrate::OnBnClickedBtnRefreshkvalue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(m_EDT_dCCDValue!=0 && m_EDT_dManualValue!=0)
	{
		m_EDT_dCoefficient=m_EDT_dManualValue/m_EDT_dCCDValue;
	}else
	{
		AfxMessageBox("CCD测量值和人工测量值不可以为0");
	}
	UpdateData(FALSE);
}
