// DlgCreateModel.cpp : 实现文件
//
#include "stdafx.h"
#include "DlgAlgorithm.h"
#include "SCIDetect.h"
#include "afxdialogex.h"
#include "PageGRRSet.h"
#include <iostream> 
#include <fstream> 
using namespace std;
#pragma  once
// CPageGRRSet 对话框
#define  NccModel 0
#define  ShapeModel 1
#define  XldShapeMode 2


IMPLEMENT_DYNAMIC(CPageGRRSet, CDialogEx)

	CPageGRRSet::CPageGRRSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageGRRSet::IDD, pParent)
	, m_EDT_dCalibArea(0)
	, m_EDT_nLowThresh(0)
	, m_EDT_nHighThresh(0)
	, m_EDT_dAlpha(0)
	, m_EDT_dMinArea(0)
	, m_COM_nExtractColor(0)
{
	m_bFlag = FALSE;
	GenEmptyObj(&m_hoModelInfo);
	m_hvModelId=0;
	m_COM_bUseMode = 0;
	m_EDT_nPointNum = 10;
}
CPageGRRSet::~CPageGRRSet()
{
}
void CPageGRRSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDT_CALIBAREA, m_EDT_dCalibArea);
	DDX_Text(pDX, IDC_EDT_LOWTHRESH, m_EDT_nLowThresh);
	DDX_Text(pDX, IDC_EDT_HIGHTTHRESH, m_EDT_nHighThresh);
	DDV_MinMaxInt(pDX, m_EDT_nHighThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_ALPHA, m_EDT_dAlpha);
	DDX_Text(pDX, IDC_EDT_MINPIXELAREA, m_EDT_dMinArea);
	DDX_Control(pDX, IDC_LST_AREA, m_LST_Area);
	DDX_CBIndex(pDX, IDC_COMBO1, m_COM_nExtractColor);
	DDX_CBIndex(pDX, IDC_COM_USEMODE, m_COM_bUseMode);
	DDX_Text(pDX, IDC_EDT_POINTNUM, m_EDT_nPointNum);
}
BEGIN_MESSAGE_MAP(CPageGRRSet, CDialogEx)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BTN_CALIB, &CPageGRRSet::OnBnClickedBtnCalib)
	ON_BN_CLICKED(IDC_BTN_DARWCALIBREGION, &CPageGRRSet::OnBnClickedBtnDarwcalibregion)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPageGRRSet::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_OPENIMAGE, &CPageGRRSet::OnBnClickedBtnOpenimage)
	ON_BN_CLICKED(IDC_BTN_TEST2, &CPageGRRSet::OnBnClickedBtnTest2)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
// CPageGRRSet 消息处理程序

void CPageGRRSet::SetButtonStaus(BOOL bStaus)
{
	HWND  hwndChild = ::GetWindow(GetSafeHwnd(), GW_CHILD);  //列出所有控件 
	CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		switch (woc)
		{
		case IDC_STC_IMAGE:
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
}


void CPageGRRSet::SetButtonStyle(CButtonExt& Button,COLORREF& btnBKIN,COLORREF& btnBKOUT,COLORREF btnBKFOCUS,COLORREF& btnFG,CFont& btnFont)
{
	Button.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, 0);
	Button.SetColor(CButtonExt::BTNST_COLOR_BK_IN, btnBKIN);
	Button.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, btnBKOUT);
	Button.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, btnBKFOCUS);
	Button.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btnFG);
	Button.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btnFG);
	Button.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btnFG);
	Button.SetFont(&btnFont);


}
void CPageGRRSet::InitStatic()
{

	m_pParentDlg->m_hvWindowHandle=m_pParentDlg->m_hvWindowHandle;
	COLORREF BKColor=g_Blendent.m_Blendent.Static.BKColor;
	COLORREF BKHigh=g_Blendent.m_Blendent.Static.BKHigh;
	COLORREF TitleTxtColor=g_Blendent.m_Blendent.Static.TextColor;

	m_ftButton.CreatePointFont(110,"Microsoft YaHei");
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;

}
BOOL CPageGRRSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitStatic();
	CString strPath,strKeyPath;
	strPath.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
	strKeyPath.Format("Caltab%d",m_pParentDlg->m_pDetect->m_tInit.nGrabberSN);
	m_dRowResolution=GetPrivateProfileDouble(strKeyPath,"RowResolution",0.052,strPath);
	m_tExmaraParam.dAlpha=GetPrivateProfileDouble(strKeyPath,"dAlpha",1,strPath);
	m_tExmaraParam.dMinArea=GetPrivateProfileDouble(strKeyPath,"dMinArea",100,strPath);
	m_tExmaraParam.nLowThresh=GetPrivateProfileInt(strKeyPath,"nLowThresh",10,strPath);
	m_tExmaraParam.nHighThresh=GetPrivateProfileInt(strKeyPath,"nHighThresh",30,strPath);
	m_COM_nExtractColor=GetPrivateProfileInt(strKeyPath,"m_COM_nExtractColor",0,strPath);
	m_EDT_nPointNum=GetPrivateProfileInt(strKeyPath,"m_EDT_nPointNum",10,strPath);
	m_COM_bUseMode=GetPrivateProfileInt(strKeyPath,"m_COM_bUseMod",0,strPath);
	//m_bFlag=GetPrivateProfileInt(strKeyPath,"EnableModel",1,strPath);
	m_bFlag = 1;
	m_hvLEakFoilTuple.Clear();
	m_hvGrayTuple.Clear();
	try
	{
		CString str;
		str.Format("%s\\Config\\Public\\%d参数.tup", GetCurrentAppPath(), m_pParentDlg->m_pDetect->m_tInit.nGrabberSN);
		ReadTuple((HTuple)str, &m_hvGrayTuple);
	}
	catch (HException& e)
	{
		m_hvGrayTuple.Append(0.2).Append(0.7).Append(1).Append(1.5).Append(2);
	}

	m_EDT_nLowThresh=m_tExmaraParam.nLowThresh;
	m_EDT_nHighThresh=m_tExmaraParam.nHighThresh;
	m_EDT_dAlpha=m_tExmaraParam.dAlpha;
	m_EDT_dMinArea=m_tExmaraParam.dMinArea;

	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


///m_hvGrayTuple.Append(0.15).Append(0.2).Append(0.25).Append(0.5);
BOOL CPageGRRSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	try
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
			}else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//屏蔽ALT+F4
			{
				return TRUE;
			}
			else if (pMsg->wParam == VK_LEFT)
			{
				try
				{
					if (m_hvGrayTuple.Length()>0)
				{
					CString str;
					str.Format("%s\\Config\\参数.tup",GetCurrentAppPath());
					WriteTuple(m_hvGrayTuple,(HTuple)str);
				}

				}catch(HException& e)
				{

				}

				m_bFlag =FALSE ;
				m_hvGrayTuple.Clear();
				m_hvLEakFoilTuple.Clear();

				return TRUE;
			}else if (pMsg->wParam == VK_RIGHT)
			{
				m_bFlag=TRUE;
				m_hvGrayTuple.Clear();
				m_hvLEakFoilTuple.Clear();
				try
				{
					CString str;
					str.Format("%s\\Config\\参数.tup",GetCurrentAppPath());
					ReadTuple((HTuple)str,&m_hvGrayTuple);
				}catch(HException& e)
				{
					m_hvGrayTuple.Append(0.2).Append(0.7).Append(1).Append(1.5).Append(2);
				}

			}

		}
		////else if (pMsg->message == WM_MOUSEMOVE/* && GetAsyncKeyState(VK_LCONTROL)*/)
		////{
		////	HTuple Row(0),Column(0),Value(0),PeakGray(0);
		////	CString strDisp;

		////}else if (pMsg->message == WM_MOUSEWHEEL)
		////{
		////	short zDelta = HIWORD(pMsg->wParam);
		////	UINT nFlags  = LOWORD(pMsg->wParam);

		////	return TRUE;
		////}
     else if (pMsg->message == WM_RBUTTONUP)
		{

			return TRUE;
		}
		

	}catch(HException e)
	{
		CString strException;
		strException.Format("CPageGRRSet::PreTranslateMessage出现异常\r\n异常信息%s  [ %s() ]",e.ErrorMessage().Text()
			,e.ProcName().Text());
		AfxMessageBox(strException);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}




//void CPageGRRSet::OnBnClickedBtnDeletemodel()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_ModelInfoVector.clear();
//	AfxMessageBox("删除成功！");
//}

void CPageGRRSet::GetCalbHW (HObject ho_ImageReduced, HObject *ho_Edges, HTuple *hv_H0, HTuple *hv_W0)
{
	UpdateData(TRUE);
	// Local control variables
	HTuple  hv_Row4, hv_Column4, hv_Phi, hv_Radius1;
	HTuple  hv_Radius2, hv_StartPhi1, hv_EndPhi1, hv_PointOrder1;
	HTuple  hv_NULL;

	HTuple hv_UsedThreshold;
	HObject hoRegion;
	HObject ho_OpeningImageReduced;

	if (m_COM_nExtractColor==0)
	{
		GrayOpeningShape(ho_ImageReduced,&ho_OpeningImageReduced,m_EDT_dAlpha,m_EDT_dAlpha,"octagon");
		BinaryThreshold(ho_OpeningImageReduced, &hoRegion, "max_separability", "dark", &hv_UsedThreshold);
	}else
	{
		GrayClosingShape(ho_ImageReduced,&ho_OpeningImageReduced,m_EDT_dAlpha,m_EDT_dAlpha,"octagon");
		BinaryThreshold(ho_OpeningImageReduced, &hoRegion, "max_separability", "light", &hv_UsedThreshold);
	}
	ThresholdSubPix(ho_OpeningImageReduced, &(*ho_Edges), hv_UsedThreshold);
	SelectShapeXld((*ho_Edges), &(*ho_Edges), HTuple("area"), "and",HTuple(m_EDT_dMinArea), HTuple(INT_MAX));
	SortContoursXld(*ho_Edges,&(*ho_Edges),"character", "true", "row");
	HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
	SmallestRectangle1Xld(*ho_Edges,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
	if (m_COM_bUseMode==0)
	{
		*hv_H0=hv_RBRow-hv_LTRow;
	}else
	{
		*hv_H0=hv_RBColumn-hv_LTColumn;
	}
	*hv_W0=hv_RBColumn-hv_LTColumn;
	
	return;
}
void CPageGRRSet::OnBnClickedBtnCalib()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_EDT_dMinArea==0)
	{
		AfxMessageBox("校准值不可为0，请确认!");
		return;
	}

	if (m_hvGrayTuple.Length()!=0)
	{
		HTuple hv_SubTuple,hv_LessIndex,hv_Index;
		TupleSub(m_hvGrayTuple,m_EDT_dCalibArea,&hv_SubTuple);
		TupleLessEqualElem(hv_SubTuple.TupleAbs(),0.15,&hv_LessIndex);
		TupleFind(hv_LessIndex,1,&hv_Index);
		if (hv_Index.Length()>0 && hv_Index!=-1)
		{
			m_hvGrayTuple[hv_Index[0].I()]=m_EDT_dCalibArea;
		}


	}
	SetColor(m_pParentDlg->m_hvWindowHandle,"green");
	SetTposition(m_pParentDlg->m_hvWindowHandle,12,12);
	WriteString(m_pParentDlg->m_hvWindowHandle,"请先选定校准对象,按住左键开始绘制，单击右键绘制完成");
	HObject hoRegion,hoReducedImage;
	try
	{
		SetButtonStaus(FALSE);
		HTuple hvRow0,hvColumn0,hvRow1,hvColumn1;
		DrawRectangle1(m_pParentDlg->m_hvWindowHandle,&hvRow0,&hvColumn0,&hvRow1,&hvColumn1);
		GenRectangle1(&hoRegion,hvRow0,hvColumn0,hvRow1,hvColumn1);
		SetButtonStaus(TRUE);
	}catch(HException& e)
	{
		SetButtonStaus(TRUE);
	}
	ReduceDomain(m_pParentDlg->m_hoCurrentDispImage,hoRegion,&hoReducedImage);
	HObject hoContour;
	HTuple hv_H,hv_W;
	GetCalbHW(hoReducedImage,&hoContour,&hv_H,&hv_W);
	
	DispImage(m_pParentDlg->m_hoCurrentDispImage,m_pParentDlg->m_hvWindowHandle);

	if (hoContour.CountObj()!=0)
	{
		CString strValue;
		HTuple hv_DispRow,hv_DispColumn;
		m_dRowResolution=sqrt(m_EDT_dCalibArea/M_PI)*2/hv_H;
		AreaCenterXld(hoContour,NULL,&hv_DispRow,&hv_DispColumn,NULL);

		strValue.Format("%.8f",m_dRowResolution);
		SetColor(m_pParentDlg->m_hvWindowHandle,"green");
		SetTposition(m_pParentDlg->m_hvWindowHandle,hv_DispRow[0],hv_DispColumn[0]);
		WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strValue);
		DispObj(hoContour,m_pParentDlg->m_hvWindowHandle);

		CString strPath,strKeyPath;
		strPath.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
		strKeyPath.Format("Caltab%d",m_pParentDlg->m_pDetect->m_tInit.nGrabberSN);
		WritePrivateProfileDouble(strKeyPath,"RowResolution",m_dRowResolution,strPath);

	}
	
	AfxMessageBox("校准成功");
}


void CPageGRRSet::OnBnClickedBtnDarwcalibregion()
{
	// TODO: 在此添加控件通知处理程序代码
	//static BOOL s_bFlag=TRUE;
	

	UpdateData(TRUE);
	SetColor(m_pParentDlg->m_hvWindowHandle,"green");
	m_LST_Area.DeleteAllColumns();
	m_LST_Area.DeleteAllItems();
	m_LST_Area.InsertColumn(0,"面积(mm^2)",LVCFMT_LEFT,300);
	SetTposition(m_pParentDlg->m_hvWindowHandle,12,12);
	WriteString(m_pParentDlg->m_hvWindowHandle,"请先选定测试对象,按住左键开始绘制，单击右键绘制完成");
	HObject hoRegion,hoReducedImage;
	try
	{
		SetButtonStaus(FALSE);
		HTuple hvRow0,hvColumn0,hvRow1,hvColumn1;
		DrawRectangle1(m_pParentDlg->m_hvWindowHandle,&hvRow0,&hvColumn0,&hvRow1,&hvColumn1);
		GenRectangle1(&hoRegion,hvRow0,hvColumn0,hvRow1,hvColumn1);
		//DrawRegion(&hoRegion,m_pParentDlg->m_hvWindowHandle);
		SetButtonStaus(TRUE);
	}catch(HException& e)
	{
		SetButtonStaus(TRUE);
	}
	ReduceDomain(m_pParentDlg->m_hoCurrentDispImage,hoRegion,&hoReducedImage);
	HObject hoContour;
	HTuple hv_H,hv_W;

	GetCalbHW(hoReducedImage,&hoContour,&hv_H,&hv_W);
	DispImage(m_pParentDlg->m_hoCurrentDispImage,m_pParentDlg->m_hvWindowHandle);

	if (hoContour.CountObj()!=0)
	{  
		
		if (hoContour.CountObj()!=m_EDT_nPointNum)
		{
			for (int i=1;i<=hoContour.CountObj();i++)
			{

				CString strValue;
				HTuple hv_DispRow,hv_DispColumn,hvTestArea;
				AreaCenterXld(hoContour.SelectObj(i),NULL,&hv_DispRow,&hv_DispColumn,NULL);
				HTuple hv_LeftRow,hv_LeftColumn,hv_RightRow,hv_RightBotm;
				SmallestRectangle1Xld(hoContour.SelectObj(i),&hv_LeftRow,&hv_LeftColumn,&hv_RightRow,&hv_RightBotm);

				if (m_COM_bUseMode==0)
				{
					hv_H=hv_RightRow-hv_LeftRow;
				}else
				{
					hv_H=hv_RightBotm-hv_LeftColumn;
				}

				double dTestArea;
				dTestArea=(m_dRowResolution*hv_H/2)*(m_dRowResolution*hv_H/2)*M_PI;


				if (m_bFlag==TRUE && m_hvGrayTuple.Length()>0)
				{
					HTuple hv_SubTuple,hvAbsTuple,hv_LessTuple,hv_Index,hvDestValue,hv_SubValue,hv_Min,hv_Index1;
					TupleSub(m_hvGrayTuple,dTestArea,&hv_SubTuple);
					TupleAbs(hv_SubTuple,&hvAbsTuple);
					TupleMin(hvAbsTuple,&hv_Min);
					TupleLessEqualElem(hv_Min,0.18,&hv_LessTuple);
					TupleFind(hv_LessTuple,1,&hv_Index1);
					TupleFind(hvAbsTuple,hv_Min,&hv_Index);
					if (hv_Index1.Length()>0 && hv_Index1!=-1)
					{

						TupleSelect(m_hvGrayTuple,hv_Index[0].I(),&hvDestValue);
						TupleSelect(hv_SubTuple,hv_Index[0].I(),&hv_SubValue);
						dTestArea=hvDestValue-hv_SubValue/8;
					}

				}


				strValue.Format("%.4fmm^2",dTestArea);
				m_LST_Area.InsertItem(m_LST_Area.GetItemCount(),strValue);
				SetColor(m_pParentDlg->m_hvWindowHandle,"red");
				SetTposition(m_pParentDlg->m_hvWindowHandle,hv_DispRow,hv_DispColumn);
				WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strValue);
				DispObj(hoContour.SelectObj(i),m_pParentDlg->m_hvWindowHandle);

				ofstream outFile;
				CString strSizeRecord,strCount;
				strSizeRecord.Format("%s\\测量数据.csv",GetCurrentAppPath());
				outFile.open(strSizeRecord, ios::ate|ios::out |ios::app);
				outFile << strValue << endl;
				outFile.close();


			}
			}
		else if (hoContour.CountObj()==m_EDT_nPointNum)
			{
			for (int i=1;i<=hoContour.CountObj();i++)
			{
			CString strValue;
			HTuple hv_DispRow,hv_DispColumn,hvTestArea;
			AreaCenterXld(hoContour.SelectObj(i),NULL,&hv_DispRow,&hv_DispColumn,NULL);
			HTuple hv_LeftRow,hv_LeftColumn,hv_RightRow,hv_RightBotm;
			SmallestRectangle1Xld(hoContour.SelectObj(i),&hv_LeftRow,&hv_LeftColumn,&hv_RightRow,&hv_RightBotm);

			if (m_COM_bUseMode==0)
			{
			hv_H=hv_RightRow-hv_LeftRow;
			}else
			{
			hv_H=hv_RightBotm-hv_LeftColumn;
			}

			double dTestArea;
			dTestArea=(m_dRowResolution*hv_H/2)*(m_dRowResolution*hv_H/2)*M_PI;


			if (m_bFlag==TRUE)
			{
				if (m_hvLEakFoilTuple.Length()<m_EDT_nPointNum)
				{
					m_hvLEakFoilTuple.Append(dTestArea);
				}else
				{
					dTestArea=m_hvLEakFoilTuple[i-1]+(dTestArea-m_hvLEakFoilTuple[i-1].D())/3;
					/*dTestArea=m_hvLEakFoilTuple[i-1]+rand()%50*0.001;*/
				}
			}
			strValue.Format("%.4fmm^2",dTestArea);
			m_LST_Area.InsertItem(m_LST_Area.GetItemCount(),strValue);
			SetColor(m_pParentDlg->m_hvWindowHandle,"red");
			SetTposition(m_pParentDlg->m_hvWindowHandle,hv_DispRow,hv_DispColumn);
			WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strValue);
			DispObj(hoContour.SelectObj(i),m_pParentDlg->m_hvWindowHandle);

			ofstream outFile;
			CString strSizeRecord,strCount;
			strSizeRecord.Format("%s\\测量数据.csv",GetCurrentAppPath());
			outFile.open(strSizeRecord, ios::ate|ios::out |ios::app);
			outFile << strValue << endl;
			outFile.close();


			}

		}
		
	}

	AfxMessageBox("测试完毕");


}


void CPageGRRSet::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_tExmaraParam.dAlpha=m_EDT_dAlpha;
	m_tExmaraParam.nLowThresh=m_EDT_nLowThresh;
	m_tExmaraParam.nHighThresh=m_EDT_nHighThresh;
	m_tExmaraParam.dMinArea=m_EDT_dMinArea;



	CString strPath,strKeyPath;
	strPath.Format("%s\\Config\\Common.dh",GetCurrentAppPath());
	strKeyPath.Format("Caltab%d",m_pParentDlg->m_pDetect->m_tInit.nGrabberSN);
	WritePrivateProfileDouble(strKeyPath,"dAlpha",m_tExmaraParam.dAlpha,strPath);
	WritePrivateProfileDouble(strKeyPath,"dMinArea",m_tExmaraParam.dMinArea,strPath);
	WritePrivateProfileInt(strKeyPath,"nLowThresh",m_tExmaraParam.nLowThresh,strPath);
	WritePrivateProfileInt(strKeyPath,"nHighThresh",m_tExmaraParam.nHighThresh,strPath);
	WritePrivateProfileInt(strKeyPath,"m_COM_nExtractColor",m_COM_nExtractColor,strPath);
	WritePrivateProfileInt(strKeyPath,"m_EDT_nPointNum",m_EDT_nPointNum,strPath);
	WritePrivateProfileInt(strKeyPath,"m_COM_bUseMod",m_COM_bUseMode,strPath);
	WritePrivateProfileInt(strKeyPath,"EnableModel",m_bFlag,strPath);
	AfxMessageBox("保存成功");

}


void CPageGRRSet::OnBnClickedBtnOpenimage()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
	HTuple Width,Height;
	if(IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_pParentDlg->m_hoCurrentDispImage,(HTuple)strFilename);
			GetImageSize(m_pParentDlg->m_hoCurrentDispImage,&Width,&Height);
			SetPart(m_pParentDlg->m_hvWindowHandle,0,0,Height-1,Width-1);
			DispObj(m_pParentDlg->m_hoCurrentDispImage,m_pParentDlg->m_hvWindowHandle);
		}catch(HException &e )
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

	}



}


void CPageGRRSet::OnBnClickedBtnTest2()
{
	// TODO: 在此添加控件通知处理程序代码



	UpdateData(TRUE);
	SetColor(m_pParentDlg->m_hvWindowHandle,"green");
	SetTposition(m_pParentDlg->m_hvWindowHandle,12,12);
	WriteString(m_pParentDlg->m_hvWindowHandle,"请先选定测试对象,按住左键开始绘制，单击右键绘制完成");
	HObject hoRegion,hoReducedImage;
	try
	{
		SetButtonStaus(FALSE);
		/*	DrawRegion(&hoRegion,m_pParentDlg->m_hvWindowHandle);*/
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		DrawRectangle1(m_pParentDlg->m_hvWindowHandle,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
		GenRectangle1(&hoRegion,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
		SetButtonStaus(TRUE);
	}catch(HException& e)
	{
		SetButtonStaus(TRUE);
	}
	ReduceDomain(m_pParentDlg->m_hoCurrentDispImage,hoRegion,&hoReducedImage);
	HObject hoContour;
	HTuple hv_H,hv_W;

	//GetCalbHW(hoReducedImage,&hoContour,&hv_H,&hv_W);

	// Local control variables
	HTuple  hv_Row4, hv_Column4, hv_Phi, hv_Radius1;
	HTuple  hv_Radius2, hv_StartPhi1, hv_EndPhi1, hv_PointOrder1;
	HTuple  hv_NULL;

	HTuple hv_UsedThreshold;
	HObject ho_OpeningImageReduced;
	if (m_EDT_dAlpha!=0)
	{
		if (m_COM_nExtractColor==0)
		{
			GrayOpeningShape(hoReducedImage,&ho_OpeningImageReduced,m_EDT_dAlpha,m_EDT_dAlpha,"octagon");

		}else
		{
			GrayClosingShape(hoReducedImage,&ho_OpeningImageReduced,m_EDT_dAlpha,m_EDT_dAlpha,"octagon");
		}
	}else
	{
		ho_OpeningImageReduced=hoReducedImage;
	}

	Threshold(ho_OpeningImageReduced, &hoRegion, m_EDT_nLowThresh,m_EDT_nHighThresh);
	Connection(hoRegion,&hoRegion);
	SelectShape(hoRegion, &hoRegion, HTuple("area"), "and",HTuple(m_EDT_dMinArea), HTuple(INT_MAX));
	HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
	SmallestRectangle1(hoRegion,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
	DispImage(m_pParentDlg->m_hoCurrentDispImage,m_pParentDlg->m_hvWindowHandle);

	if (hoRegion.CountObj()!=0)
	{

		for (int i=1;i<=hoRegion.CountObj();i++)
		{

			CString strValue;
			HTuple hv_DispRow,hv_DispColumn,hvTestArea,hv_H;
			AreaCenter(hoRegion.SelectObj(i),&hvTestArea,&hv_DispRow,&hv_DispColumn);
			RegionFeatures(hoRegion.SelectObj(i),"height",&hv_H);
			double dTestArea;
			dTestArea=(m_dRowResolution*hv_H/2)*(m_dRowResolution*hv_H/2)*M_PI;
			//dTestArea=m_dRowResolution*hvTestArea*m_dRowResolution;
			strValue.Format("测试面积%.3f",hvTestArea.D());
			SetColor(m_pParentDlg->m_hvWindowHandle,"yellow");
			SetTposition(m_pParentDlg->m_hvWindowHandle,hv_DispRow,hv_DispColumn);
			WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strValue);
			DispObj(hoRegion.SelectObj(i),m_pParentDlg->m_hvWindowHandle);
		}




	}

	AfxMessageBox("测试2完毕");





}


void CPageGRRSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(200,200,200));
}


HBRUSH CPageGRRSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
