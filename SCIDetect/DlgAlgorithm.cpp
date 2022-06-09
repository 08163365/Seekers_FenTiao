// DlgAlgorithm.cpp : 实现文件
//

#include "stdafx.h"
#include "SCIDetect.h"
#include "DlgAlgorithm.h"
#include "afxdialogex.h"
#include "DHInterFace.h"
#include "PageSet.h"
#include "DlgHelp.h"
extern CString   g_strConfigCommonFile;//Common.dh
extern e_CoatType g_emCoatType;
extern e_ProductType g_ProductType;
// CDlgAlgorithm 对话框
IMPLEMENT_DYNAMIC(CDlgAlgorithm, CDialogEx)

	CDlgAlgorithm::CDlgAlgorithm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlgorithm::IDD, pParent)
	, m_StrUserName(_T(""))
{

	m_dDispImagePartRow0 = 0;
	m_dDispImagePartCol0 = 0;
	m_dDispImagePartRow1 = 0;
	m_dDispImagePartCol1 = 0;
	m_pDetect = new CBattery;
	m_pRealDetect = NULL;
	m_pMenuTools = NULL;
	m_ListFont.CreatePointFont(120,"Microsoft YaHei");
	m_hoOriginImage.GenEmptyObj();
}

CDlgAlgorithm::~CDlgAlgorithm()
{
		

	if (m_pDetect!=NULL)
	{
		delete m_pDetect;

	}
	if (m_pMenuTools!=NULL)
	{
		delete m_pMenuTools;
	}
	m_ListFont.DeleteObject();

}

void CDlgAlgorithm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BTN_TEST, m_btnTest);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
	DDX_Control(pDX, IDC_BTN_SAVEPARA, m_btnSavePara);
	DDX_Control(pDX, IDC_BTN_TESTALL, m_btnTestAll);
	DDX_Control(pDX, IDC_BTN_SAVEIMAGE, m_btnSaveImage);
	DDX_Control(pDX, IDC_STATIC_GRAY, m_STC_GrayValue);
	DDX_Control(pDX, IDC_BUTTON_TOOLS, m_btnTools);
	DDX_Control(pDX, IDC_BTN_SAVESAMPLEIMAGE, m_bnSaveSampleImage);
	DDX_Control(pDX, IDC_BTN_LOADSAMPLEIMAGE, m_btn_LoadSampleImage);
	DDX_Control(pDX, IDC_BTN_HELP, m_btn_Help);
	DDX_Control(pDX, IDC_TAB_PARAMSET, m_TAB_CParamSet);
}


BEGIN_MESSAGE_MAP(CDlgAlgorithm, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgAlgorithm::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CDlgAlgorithm::OnBnClickedBtnExit)
	ON_WM_MOUSEWHEEL()
//	ON_BN_CLICKED(IDC_BTN_PARAMSET, &CDlgAlgorithm::OnBnClickedBtnParamset)
	ON_COMMAND(ID_MENU_ANALYSISREGION, &CDlgAlgorithm::OnMenuAnalysisregion)
	ON_COMMAND(ID_MENU_AUTO_ANALYSISREGION, &CDlgAlgorithm::OnMenuAutoAnalysisregion)

	ON_COMMAND(ID_MENU_FINDFOIL, &CDlgAlgorithm::OnMenuFindfoil)
	ON_COMMAND(ID_MENU_DISPCOATING, &CDlgAlgorithm::OnMenuDispcoating)
	ON_COMMAND(ID_MENU_DISPFOIL, &CDlgAlgorithm::OnMenuDispfoil)
	ON_BN_CLICKED(IDC_BTN_SAVEPARA, &CDlgAlgorithm::OnBnClickedBtnSavepara)
	ON_COMMAND(ID_MENU_FIRSTDETECT, &CDlgAlgorithm::OnMenuFirstdetect)
	ON_COMMAND(ID_MENU_SAVEIMAGE, &CDlgAlgorithm::OnMenuSaveimage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_INFO, &CDlgAlgorithm::OnLvnItemchangedListInfo)
	ON_COMMAND(ID_MENU_CHECKSET, &CDlgAlgorithm::OnMenuCheckset)
	ON_BN_CLICKED(IDC_BTN_TESTALL, &CDlgAlgorithm::OnBnClickedBtnTestall)
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_BTN_MEASURE, &CDlgAlgorithm::OnBnClickedBtnMeasure)
//	ON_BN_CLICKED(IDC_BTN_DRAWMODEL, &CDlgAlgorithm::OnBnClickedBtnDrawmodel)
//	ON_BN_CLICKED(IDC_BTN_DEFECTCONTROL, &CDlgAlgorithm::OnBnClickedBtnDefectcontrol)
	ON_BN_CLICKED(IDC_BTN_SAVEIMAGE, &CDlgAlgorithm::OnBnClickedBtnSaveimage)
	ON_COMMAND(ID_OPENIMAGE, &CDlgAlgorithm::OnOpenimage)
	ON_COMMAND(ID_MENU_COAT_BORDER, &CDlgAlgorithm::OnMenuCoatBorder)
	ON_COMMAND(ID_MENU_COAT_MID, &CDlgAlgorithm::OnMenuCoatMid)
	ON_BN_CLICKED(IDC_BUTTON_TOOLS, &CDlgAlgorithm::OnClickedButtonTools)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_COMMAND(ID_MENU_LIGHT, &CDlgAlgorithm::OnMenuLight)
	ON_COMMAND(ID_MENU_DARK, &CDlgAlgorithm::OnMenuDark)
	ON_COMMAND(ID_MENU_SEGREGION, &CDlgAlgorithm::OnMenuSegregion)
	ON_COMMAND(ID_MENU_AT9REGION, &CDlgAlgorithm::OnMenuAt9region)
	ON_BN_CLICKED(IDC_BTN_SAVESAMPLEIMAGE, &CDlgAlgorithm::OnBnClickedBtnSavesampleimage)
	ON_BN_CLICKED(IDC_BTN_LOADSAMPLEIMAGE, &CDlgAlgorithm::OnBnClickedBtnLoadsampleimage)
	ON_BN_CLICKED(IDC_BTN_HELP, &CDlgAlgorithm::OnBnClickedBtnHelp)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PARAMSET, &CDlgAlgorithm::OnTcnSelchangeTabParamset)
	ON_COMMAND(ID_MENU_DRAWCOAT, &CDlgAlgorithm::OnMenuDrawcoat)
	ON_COMMAND(ID_MENU_JXREGION, &CDlgAlgorithm::OnMenuJxregion)
	ON_COMMAND(ID_MENU_TRIPREGION, &CDlgAlgorithm::OnMenuTripregion)
	ON_COMMAND(ID_MENU_YAHENPINGBIQU, &CDlgAlgorithm::OnMenuYahenpingbiqu)
END_MESSAGE_MAP()


// CDlgAlgorithm 消息处理程序
void CDlgAlgorithm::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)
{
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN,btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	return;
}

void CDlgAlgorithm::InitStatic()
{


	COLORREF DlgMainColor=g_Blendent.m_Blendent.Dlg.MainColor;
	COLORREF DlgBkColor=g_Blendent.m_Blendent.Dlg.BKColor;
	COLORREF BKColor=g_Blendent.m_Blendent.Static.BKColor;
	COLORREF BKHigh=g_Blendent.m_Blendent.Static.BKHigh;
	COLORREF TitleTxtColor=g_Blendent.m_Blendent.Static.TextColor;
	m_ftButton.CreateFontA(17,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"华文细黑");

	m_STC_GrayValue.SetTransparent(FALSE);
	CString strDisp;
	strDisp.Format("坐标:X:-- Y:--Offset：--GrayValue: --");
	m_STC_GrayValue.SetWindowTextA(strDisp);
	m_STC_GrayValue.SetTextColor(RGB(0,0,0));
	m_STC_GrayValue.SetFont("华文细黑", 20, FW_BOLD);
	m_STC_GrayValue.SetBkColor(RGB(255,255,255));
	short shOffset=g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKColorIn=g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKColorOut=g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKColorFocus=g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FGColorIn=g_Blendent.m_Blendent.Button.FGColorIn;
	COLORREF FGColorOut=g_Blendent.m_Blendent.Button.FGColorOut;
	COLORREF FGColorFocus=g_Blendent.m_Blendent.Button.FGColorFocus;
	SetButtonColors(m_btnTest,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);
	SetButtonColors(m_btnTestAll,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);
	SetButtonColors(m_btnSavePara,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);
	SetButtonColors(m_btnExit,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);

	SetButtonColors(m_btnSaveImage,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);

	SetButtonColors(m_btnTools,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);

	SetButtonColors(m_btn_Help,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);

	m_btn_Help.SetFont(&m_ftButton);
	
	SetButtonColors(m_btn_LoadSampleImage,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);
	SetButtonColors(m_bnSaveSampleImage,shOffset,BKColorIn,BKColorOut,BKColorFocus,FGColorIn,FGColorOut,FGColorFocus);
	m_btn_LoadSampleImage.SetFont(&m_ftButton);
	m_bnSaveSampleImage.SetFont(&m_ftButton);

	m_btnTest.SetFont(&m_ftButton);
	m_btnTest.SetIcon(IDI_ICON_TEST);
	m_btnExit.SetFont(&m_ftButton);
	m_btnExit.SetIcon(IDI_ICON_CANCEL);
	
	m_btnSavePara.SetFont(&m_ftButton);
	m_btnSavePara.SetIcon(IDI_ICON_SAVE);
	m_btnTestAll.SetFont(&m_ftButton);
	m_btnTestAll.SetIcon(IDI_ICON_DEFECT);
	m_btnSaveImage.SetFont(&m_ftButton);
	m_btnSaveImage.SetIcon(IDI_ICON_SAVEIMAGE);

	m_btnTools.SetFont(&m_ftButton);
	m_btnTools.SetIcon(IDI_ICON_W_LAYER);
	m_listInfo.SetGridLines(TRUE); // SHow grid lines
	m_listInfo.SetColumnHeader(_T("序号, 90; 属性, 100; 值,1300;"));
	m_listInfo.DeleteAllItems();
	m_listInfo.SetFont(&m_ListFont);

	m_pMenuTools = new CMenuExt;
	m_pMenuTools->SetAttributes(95,"Microsoft YaHei",40,35,
		g_Blendent.m_Blendent.Menu.BKColorNormal,
		g_Blendent.m_Blendent.Menu.BKColorFocus,
		g_Blendent.m_Blendent.Menu.BDColorFocus,
		g_Blendent.m_Blendent.Menu.TextColorNormal,
		g_Blendent.m_Blendent.Menu.TextColorFocus,
		g_Blendent.m_Blendent.Menu.IconBKColor,
		g_Blendent.m_Blendent.Menu.SPColor);
	m_pMenuTools->LoadMenu(IDR_MENU_RBUTTONUP);
	m_pMenuTools->ChangeMenuItem(m_pMenuTools);
}

BOOL CDlgAlgorithm::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	m_TAB_CParamSet.InsertItem(0, "瑕疵检测参数");
	m_TAB_CParamSet.InsertItem(1, "分类器参数");
	m_TAB_CParamSet.InsertItem(2, "区域分割参数");
	m_TAB_CParamSet.InsertItem(3, "GRR参数");
 	CRect rcTabItem;
	m_TAB_CParamSet.GetClientRect(&rcTabItem);
	//调整子对话框在父窗口中的位置
	rcTabItem.top += 20;
	rcTabItem.bottom -= 1;
	rcTabItem.left += 1;
	rcTabItem.right -= 2;







	InitStatic();

	
	/////////////////////////////////
	try
	{
		if (m_DetectInput.hoDetectImage.CountObj())
		{
			GenEmptyObj(&m_hoCurrentDispImage);
			m_hoCurrentDispImage = m_DetectInput.hoDetectImage;
			
		}
		if(m_hoCurrentDispImage.IsInitialized())
		{
			CRect rcImage;
			GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(rcImage);
			SetCheck("~father");
			OpenWindow(rcImage.top, rcImage.left, rcImage.Width(), rcImage.Height(),
				(Hlong)GetDlgItem(IDC_STATIC_IMAGE)->m_hWnd, "visible", "", &m_hvWindowHandle);
			SetDraw(m_hvWindowHandle, "margin");
			HTuple Width, Height;
			GetImageSize(m_hoCurrentDispImage, &Width, &Height);
			SetPart(m_hvWindowHandle, 0, 0, Height - 1, Width - 1);
			SetCheck("father");
			DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
			
		}
		m_hoOriginImage=m_DetectInput.hoDetectImage;
		HTuple Width,Height;
		GetImageSize(m_hoCurrentDispImage,&Width,&Height);
		m_dDispImagePartRow0 = 0;
		m_dDispImagePartCol0 = 0;
		m_dDispImagePartRow1 = Height-1;
		m_dDispImagePartCol1 = Width-1;



		m_PageSurfaceParamSet = new CPageSet;
		m_PageSurfaceParamSet->m_ptParameters =  &m_pDetect->m_tParameters;
		m_PageSurfaceParamSet->m_pParentDlg = this;
		m_PageSurfaceParamSet->Create(CPageSet::IDD, &m_TAB_CParamSet);
		m_PageSurfaceParamSet->MoveWindow(rcTabItem);
		m_PageSurfaceParamSet->ShowWindow(SW_SHOW);
		m_vecPages.push_back(m_PageSurfaceParamSet);




		m_PageMLPParamSet = new CMLPDlg;
		m_PageMLPParamSet->m_pParentDlg=this;
		m_PageMLPParamSet->m_tDHMLPParam=m_pDetect->m_tMLPParam;
		m_PageMLPParamSet->Create(CMLPDlg::IDD, &m_TAB_CParamSet);
		m_PageMLPParamSet->MoveWindow(rcTabItem);
		m_PageMLPParamSet->ShowWindow(SW_HIDE);
		m_vecPages.push_back(m_PageMLPParamSet);



		m_PageMeasureParamSet = new CPageLineSet;
		m_PageMeasureParamSet->m_pParentDlg = this;
		m_PageMeasureParamSet->Create(CPageLineSet::IDD, &m_TAB_CParamSet);
		m_PageMeasureParamSet->MoveWindow(rcTabItem);
		m_PageMeasureParamSet->ShowWindow(SW_HIDE);
		m_vecPages.push_back(m_PageMeasureParamSet);



		m_PageGRRParamSet = new CPageGRRSet;
		m_PageGRRParamSet->m_pParentDlg=this;
		m_PageGRRParamSet->Create(CPageGRRSet::IDD, &m_TAB_CParamSet);
		m_PageGRRParamSet->MoveWindow(rcTabItem);
		m_PageGRRParamSet->ShowWindow(SW_HIDE);
		m_vecPages.push_back(m_PageGRRParamSet);







		Test();
	}catch(HException e)
	{
		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgAlgorithm::PreTranslateMessage(MSG* pMsg)
{
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
				CDialogEx::OnCancel();
				return TRUE;
			}else if (pMsg->wParam == VK_F4  && HIBYTE(::GetKeyState(VK_MENU)))//屏蔽ALT+F4
			{
				
				return TRUE;
			}
			else if (pMsg->wParam == VK_LEFT)
			{
				try
				{
					if (m_PageGRRParamSet->m_hvGrayTuple.Length()>0)
					{
						CString str;
						str.Format("%s\\Config\\Public\\%d参数.tup", GetCurrentAppPath(),m_pDetect->m_tInit.nGrabberSN);
						WriteTuple(m_PageGRRParamSet->m_hvGrayTuple, (HTuple)str);
					}

				}
				catch (HException& e)
				{

				}

				m_PageGRRParamSet->m_bFlag = FALSE;
				m_PageGRRParamSet->m_hvGrayTuple.Clear();
				m_PageGRRParamSet->m_hvLEakFoilTuple.Clear();

				return TRUE;
			}
			else if (pMsg->wParam == VK_RIGHT)
			{
				m_PageGRRParamSet->m_bFlag = TRUE;
				m_PageGRRParamSet->m_hvGrayTuple.Clear();
				m_PageGRRParamSet->m_hvLEakFoilTuple.Clear();
				try
				{
					CString str;
					str.Format("%s\\Config\\Public\\%d参数.tup", GetCurrentAppPath(), m_pDetect->m_tInit.nGrabberSN);
					ReadTuple((HTuple)str, &m_PageGRRParamSet->m_hvGrayTuple);
				}
				catch (HException& e)
				{
					m_PageGRRParamSet->m_hvGrayTuple.Append(0.2).Append(0.7).Append(1).Append(1.5).Append(2);
				}

			}


		}else if (pMsg->message == WM_MOUSEMOVE/* && GetAsyncKeyState(VK_LCONTROL)*/)
		{
			HTuple Row(0),Column(0),Value(0),PeakGray(0);
			CString strDisp;
			try
			{

				if (m_hoCurrentDispImage.CountObj())
				{
					SetSystem("flush_graphic","false");
					DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
					SetColor(m_hvWindowHandle,"red");
					SetSystem("flush_graphic","true");
					GetMposition(m_hvWindowHandle,&Row,&Column,NULL);
					GetGrayval(m_hoCurrentDispImage,Row,Column,&Value);
					strDisp.Format("X:%3d  Y:%3d 灰度值:%3d",Column[0].I(),Row[0].I(),Value.I());
					m_STC_GrayValue.SetWindowTextA(strDisp);

				}
			}catch(HException e)
			{
				strDisp.Format("X:%3d  Y:%3d  灰度值:---",Column[0].I(),Row[0].I());
				m_STC_GrayValue.SetWindowTextA(strDisp);
			}
		}else if (pMsg->message == WM_MOUSEWHEEL)
		{
			short zDelta = HIWORD(pMsg->wParam);
			UINT nFlags  = LOWORD(pMsg->wParam);
			OnMouseWheel(nFlags,zDelta,pMsg->pt);
			return TRUE;
		}else if (pMsg->message == WM_MBUTTONDOWN)
		{
			if (NULL != m_hvWindowHandle)
			{
				CPoint   pt;
				GetCursorPos(&pt);
				GetDlgItem(IDC_STATIC_IMAGE)->ScreenToClient(&pt);
				CRect WndRect;
				GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&WndRect);

				// 还原显示（不缩放）
				if (WndRect.PtInRect(pt))
				{	
					HTuple tmpWidth;
					HTuple tmpHeight;

					GetImageSize(m_hoCurrentDispImage, &tmpWidth, &tmpHeight);

					m_dDispImagePartRow0 = 0;
					m_dDispImagePartCol0 = 0;
					m_dDispImagePartRow1 = tmpHeight - 1;
					m_dDispImagePartCol1 = tmpWidth - 1;	
					SetPart(m_hvWindowHandle, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);

					ClearWindow(m_hvWindowHandle);
					DispObj(m_hoCurrentDispImage, m_hvWindowHandle);	
				}
			}
			return TRUE;
		}
		else if (pMsg->message == WM_RBUTTONUP)
		{
			if (NULL != m_hvWindowHandle)
			{
				CPoint   pt;
				GetCursorPos(&pt);
				GetDlgItem(IDC_STATIC_IMAGE)->ScreenToClient(&pt);
				CRect WndRect;
				GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&WndRect);

				// 还原显示（不缩放）
				if (WndRect.PtInRect(pt))
				{	
					GetDlgItem(IDC_STATIC_IMAGE)->ClientToScreen(&pt);
					CMenu menu;
					menu.LoadMenu(IDR_MENU_RBUTTONUP);
					CMenu *pPopup=menu.GetSubMenu(0);
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
						this);
				}
			}
			return TRUE;
		}

	}catch(HException e)
	{
		CString strException;
		strException.Format("allocator_thread出现异常\r\n异常信息%s  [ %s() ]",e.ErrorMessage().Text()
			,e.ProcName().Text());
		AfxMessageBox(strException);
	}



	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CDlgAlgorithm::InsertListSizeError(const int i)
{
	CString strInsert,strErrorName;

	if(0!=m_DetectOut.hv_ErrorType.Length() &&m_DetectOut.hv_ErrorType[i]<11)
	{
		int nItemCount = m_listInfo.GetItemCount();
		/*strErrorName.Format("%d条%s",m_DetectOut.hv_ErrorBelongStrip[i].I(),g_strErrorName[m_DetectOut.hv_ErrorType[i].I()]);*/
		strErrorName.Format("%d条%s",m_DetectOut.hv_ErrorBelongStrip[i].I(),g_StrArrayErrorName[m_DetectOut.hv_ErrorType[i].I()]);
		strInsert.Format("%d",i);
		m_listInfo.InsertItem(nItemCount,strInsert);
		m_listInfo.SetItemText(nItemCount,1,strErrorName);
		return FALSE;
	}







	return TRUE;
}

void CDlgAlgorithm::InsertSurfaceError(const int i)
{
	
		CString strInsert,strErrorName;
		HObject ObjectSelected,ho_Dialtion,ho_Dialtion1,ho_Difference;
		HTuple hv_Area,hv_AniSomeValue,hv_Convexity,hv_MaxDiameter;
		HTuple  hv_Width(0),hv_Length(0),hv_InDeviation,hv_OutDeviation,hv_OutMean,hv_OutPeak,hv_InMean,hv_InPeak,hv_Ra,hv_Rb,hv_DialRadius,hv_Height;
		SelectObj(m_DetectOut.ho_ErrorRegion,&ObjectSelected,i+1);
		//区域的面积
		AreaCenter(ObjectSelected,&hv_Area,NULL,NULL);
		///区域的宽度和长度
		HTuple hv_JieMianWidth;
		InnerCircle(ObjectSelected, NULL, NULL, &hv_JieMianWidth);
		if (m_DetectOut.hv_ErrorType[i].I()!=TiaoHen)
		{
			RegionFeatures(ObjectSelected,"width",&hv_Width);
			RegionFeatures(ObjectSelected,"height",&hv_Height);
		}else
		{
			HObject ho_Skeleton;
			HTuple hv_Row,hv_Column,hv_Radius,hv_Length;

			Skeleton(ObjectSelected, &ho_Skeleton);
			AreaCenter(ObjectSelected, &hv_Area, &hv_Row, &hv_Column);
			AreaCenter(ho_Skeleton, &hv_Length, NULL, NULL);
			InnerCircle(ObjectSelected, NULL, NULL, &hv_Radius);
			hv_Width=hv_Radius*2;
			hv_Height=hv_Length;
		}
		



		//////////////////////////////////////////////////////////////////////////
		HTuple hv_CircularValue;
		Circularity(ObjectSelected,&hv_CircularValue);
		//区域是均值和偏差
		Intensity(ObjectSelected,m_hoCurrentDispImage,&hv_InMean,&hv_InDeviation);
		//最大直径
		DiameterRegion(ObjectSelected,NULL,NULL,NULL,NULL,&hv_MaxDiameter);
   
		int nPeakGray=m_pDetect->m_hvPeakGrayMid[0].D();
		int nItemCount = m_listInfo.GetItemCount();
		/*strErrorName.Format("%d条%s",m_DetectOut.hv_ErrorBelongStrip[i].I(),g_strErrorName[m_DetectOut.hv_ErrorType[i].I()]);*/
		strErrorName.Format("%d条%s",m_DetectOut.hv_ErrorBelongStrip[i].I()+1,g_StrArrayErrorName[m_DetectOut.hv_ErrorType[i].I()]);
		strInsert.Format("%d",i);
		m_listInfo.InsertItem(nItemCount,strInsert);
		m_listInfo.SetItemText(nItemCount,1,strErrorName);
		strInsert.Format("面积:%.2f,最大直径:%.2f,宽度：%.2fmm,截面宽度:%.2fmm,高度(长度):%.2fmm,均值:%.2f,偏差:%.2f,圆度:%.2f,图像PeakGray:%d",
			hv_Area[0].D()*m_pDetect->m_dColResolution*m_pDetect->m_dRowResolution,
			hv_MaxDiameter[0].D()*m_pDetect->m_dRowResolution,
			hv_Width[0].D()*m_pDetect->m_dColResolution,
			hv_JieMianWidth[0].D()*2*m_pDetect->m_dRowResolution,
			hv_Height[0].D()*m_pDetect->m_dRowResolution,
			hv_InMean[0].D(),
			hv_InDeviation[0].D(),
			hv_CircularValue.D(),
			nPeakGray);
		m_listInfo.SetItemText(nItemCount,2,strInsert);

}
void CDlgAlgorithm::InsertList()
{

	for (int i=0;i<m_DetectOut.ho_ErrorRegion.CountObj();i++)
	{
		//if(TRUE == InsertListSizeError(i))
		{
			InsertSurfaceError(i);
		}

	}

}
void CDlgAlgorithm::Test(int nBreakType)
{
	try
	{

		m_listInfo.DeleteAllItems();
		while(m_listInfo.GetColumnCount()>0) m_listInfo.DeleteColumn(0);
       m_listInfo.SetColumnHeader(_T("序号, 90; 属性, 100; 值,1300;"));

		HTuple dwDetectStart,dwDetectEnd;
		CountSeconds(&dwDetectStart);
		m_pDetect->Detect(m_DetectInput,&m_DetectOut,nBreakType);
		CountSeconds(&dwDetectEnd);
		ClearWindow(m_hvWindowHandle);
		m_hoCurrentDispImage = m_pDetect->m_hoAdaptedImage;
		DispObj(m_pDetect->m_hoAdaptedImage,m_hvWindowHandle);
		SetColor(m_hvWindowHandle,"green");
		DispObj(m_DetectOut.ho_Coat,m_hvWindowHandle);
		SetColor(m_hvWindowHandle,"yellow");
		DispObj(m_DetectOut.ho_Foil,m_hvWindowHandle);
		SetColor(m_hvWindowHandle,"red");
		DispObj(m_DetectOut.ho_ErrorRegion,m_hvWindowHandle);
		//检测时间
	
		InsertList();
		CString strInsert;
		int nItemCount = m_listInfo.GetItemCount();
		m_listInfo.InsertItem(nItemCount,"花费时间");
		strInsert.Format("%.2fms",(dwDetectEnd.D() - dwDetectStart.D())*1000);
		m_listInfo.SetItemText(nItemCount,1,strInsert);
		nItemCount = m_listInfo.GetItemCount();
		m_listInfo.InsertItem(nItemCount, "原图均值");
		strInsert.Format("%.1f", m_pDetect->m_dMeanValue);
		m_listInfo.SetItemText(nItemCount, 1, strInsert);

	

	}catch(HException &e)
	{
		AfxMessageBox(e.ErrorMessage().Text());
	}
}

void CDlgAlgorithm::OnBnClickedBtnTest()
{
	Test();
}
void CDlgAlgorithm::OnBnClickedBtnTestall()
{
	Test(1);
}

void CDlgAlgorithm::OnBnClickedBtnExit()
{

	CDialogEx::OnCancel();
}


BOOL CDlgAlgorithm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	try
	{
		if(m_hvWindowHandle!=NULL && m_hoCurrentDispImage.CountObj())
		{
			CPoint   pt;
			GetCursorPos(&pt);
			GetDlgItem(IDC_STATIC_IMAGE)->ScreenToClient(&pt);
			CRect WndRect;
			GetDlgItem(IDC_STATIC_IMAGE)->GetClientRect(&WndRect);
			if (WndRect.PtInRect(pt))
			{			
				double	ImagePtX,ImagePtY;
				double	Row0_1,Col0_1,Row1_1,Col1_1;
				double	Scale=0.1;

				if (zDelta>0)
				{
					ImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
					ImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
					Row0_1=ImagePtY-1/(1-Scale)*(ImagePtY-m_dDispImagePartRow0);
					Row1_1=ImagePtY-1/(1-Scale)*(ImagePtY-m_dDispImagePartRow1);
					Col0_1=ImagePtX-1/(1-Scale)*(ImagePtX-m_dDispImagePartCol0);
					Col1_1=ImagePtX-1/(1-Scale)*(ImagePtX-m_dDispImagePartCol1);
					m_dDispImagePartRow0=Row0_1;
					m_dDispImagePartCol0=Col0_1;
					m_dDispImagePartRow1=Row1_1;
					m_dDispImagePartCol1=Col1_1;
				}
				else
				{
					ImagePtX=m_dDispImagePartCol0+(pt.x-WndRect.left)/(WndRect.Width()-1.0)*(m_dDispImagePartCol1-m_dDispImagePartCol0);
					ImagePtY=m_dDispImagePartRow0+(pt.y-WndRect.top)/(WndRect.Height()-1.0)*(m_dDispImagePartRow1-m_dDispImagePartRow0);
					Row0_1=ImagePtY-1/(1+Scale)*(ImagePtY-m_dDispImagePartRow0);
					Row1_1=ImagePtY-1/(1+Scale)*(ImagePtY-m_dDispImagePartRow1);
					Col0_1=ImagePtX-1/(1+Scale)*(ImagePtX-m_dDispImagePartCol0);
					Col1_1=ImagePtX-1/(1+Scale)*(ImagePtX-m_dDispImagePartCol1);
					m_dDispImagePartRow0=Row0_1;
					m_dDispImagePartCol0=Col0_1;
					m_dDispImagePartRow1=Row1_1;
					m_dDispImagePartCol1=Col1_1;
				}


				SetPart(m_hvWindowHandle, m_dDispImagePartRow0, m_dDispImagePartCol0, m_dDispImagePartRow1, m_dDispImagePartCol1);
				SetSystem("flush_graphic","false");
				ClearWindow(m_hvWindowHandle);
				SetSystem("flush_graphic","true");
				DispObj(m_hoCurrentDispImage, m_hvWindowHandle);	
			}
		}
	}catch(HException )
	{

	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


//void CDlgAlgorithm::OnBnClickedBtnParamset()
//{
//	
//
//}

void CDlgAlgorithm::Analysisregion(HObject hoRegion)
{

	//"面积","直径","长短轴比","圆似度","矩似度","紧凑度","角度","孔洞数目","最小灰度","最大灰度","平均灰度",
	//"灰度差值","长轴","短轴","行坐标","列坐标","宽","高",
	CString strInsert,strErrorName;
	HObject ObjectSelected,ho_Dialtion,ho_Dialtion1,ho_Difference;
	HTuple hv_Area,hv_AniSomeValue,hv_Convexity,hv_MaxDiameter,hvCircularity;
	HTuple hv_Min,hv_Max,hv_Rectangularity,hvNumHoles;
	HTuple hvRa,hv_Rb,hv_Phi,hvRatio,hvCompactness;
	HTuple  hv_Width(0),hv_Length(0),hv_OutPeak,hv_InMean,hv_InPeak,hv_DialRadius,hv_Height;
	HTuple hvRow,hvColumn,hvRange;
	SelectObj(hoRegion,&ObjectSelected,1);
	//区域的面积
	AreaCenter(ObjectSelected,&hv_Area,&hvRow,&hvColumn);
	///区域的宽度和长度
	//RegionFeatures(ObjectSelected,"width",&hv_Width);
	//RegionFeatures(ObjectSelected,"height",&hv_Height);
	//区域是均值和偏差
	//最大直径
	DiameterRegion(ObjectSelected,NULL,NULL,NULL,NULL,&hv_MaxDiameter);
	Circularity(ObjectSelected,&hvCircularity);
	EllipticAxis(ObjectSelected,&hvRa,&hv_Rb,&hv_Phi);
	HeightWidthRatio(ObjectSelected,&hv_Height,&hv_Width,&hvRatio);
	Rectangularity(ObjectSelected,&hv_Rectangularity);
	Compactness(ObjectSelected,&hvCompactness);
	ConnectAndHoles(ObjectSelected,NULL,&hvNumHoles);
	MinMaxGray(ObjectSelected,m_hoCurrentDispImage,0,&hv_Min,&hv_Max,&hvRange);
	HTuple hvMean,hvDeviation;
	Intensity(ObjectSelected,m_hoCurrentDispImage,&hvMean,&hvDeviation);
	int nPeakGray=m_pDetect->m_hvPeakGrayMid[0].D();
	m_listInfo.DeleteAllItems();
	int nItemCount = m_listInfo.GetItemCount();
	strErrorName.Format("%s","当前区域");
	strInsert.Format("特征信息%d",0);
	m_listInfo.InsertItem(nItemCount,strInsert);
	m_listInfo.SetItemText(nItemCount,1,strErrorName);

	strInsert.Format("面积(像素):%.2f,直径(像素):%.2f,圆似度:%.1f,矩似度:%.1f,紧凑度：%.1f,角度:%.2f,孔洞数目:%d,图像PeakGray:%d",
		hv_Area[0].D(),
		hv_MaxDiameter[0].D(),
		hvCircularity.D(),
		hv_Rectangularity.D(),
		hvCompactness.D(),
		hv_Phi.D(),
		hvNumHoles.I(),
		nPeakGray);
	m_listInfo.SetItemText(nItemCount,2,strInsert);

	strInsert.Format("特征信息%d",1);
	m_listInfo.InsertItem(nItemCount,strInsert);
	m_listInfo.SetItemText(nItemCount,1,strErrorName);

	strInsert.Format("最小灰度:%.1f,最大灰度%.1f,平均灰度%.1f,灰度偏差因子%.1f,长轴:%.1f,\
					 短轴:%.1f,宽度:%.1f,高度:%.1f,行坐标:%.1f,列坐标:%.1f,图像PeakGray:%d",
					 hv_Min.D(),
					 hv_Max.D(),
					 hvMean.D(),
					 hvDeviation.D(),
					 hvRa.D(),
					 hv_Rb.D(),
					 hv_Width.D(),
					 hv_Height.D(),
					 hvRow.D(),
					 hvColumn.D(),
					 nPeakGray);
	m_listInfo.SetItemText(nItemCount,2,strInsert);



}
void CDlgAlgorithm::OnMenuAnalysisregion()
{
	// TODO: 在此添加命令处理程序代码
	try
	{
		DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
		disp_message(m_hvWindowHandle,"请绘制您要分析的区域,以右键结束绘制","window",-1,-1,"yellow","false");
		DrawRegion(&m_hoCurrentDispRegion,m_hvWindowHandle);

		Analysisregion(m_hoCurrentDispRegion);

		SetColor(m_hvWindowHandle,"red");
		DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
		disp_message(m_hvWindowHandle,"绘制完成,区域信息已显示在下边列表","window",-1,-1,"yellow","false");
		DispObj(m_hoCurrentDispRegion,m_hvWindowHandle);
	}catch(HException e)
	{

	}
}


void CDlgAlgorithm::OnMenuAutoAnalysisregion()
{
	try
	{
		Analysisregion(m_hoCurrentDispRegion);

		SetColor(m_hvWindowHandle,"red");
		DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
		disp_message(m_hvWindowHandle,"区域信息已显示在下边列表","window",-1,-1,"yellow","false");
		DispObj(m_hoCurrentDispRegion,m_hvWindowHandle);

	}catch(HException e)
	{

	}

}





void CDlgAlgorithm::OnMenuFindfoil()
{
	// TODO: 在此添加命令处理程序代码



}

void CDlgAlgorithm::OnMenuDispcoating()
{
	// TODO: 在此添加命令处理程序代码


}


void CDlgAlgorithm::OnMenuDispfoil()
{


}


void CDlgAlgorithm::BackFromCreateModel()//从制作模版界面返回
{
	GetDlgItem(IDC_STATIC_IMAGE)->ShowWindow(TRUE);
	GetDlgItem(IDC_LIST_INFO)->ShowWindow(TRUE);
}


void CDlgAlgorithm::OnBnClickedBtnDefectset()
{


}

void CDlgAlgorithm::ComPareChange(double& LastValue,double& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("相机%d的检测参数%s由%.2f更改为%.2f",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgAlgorithm::ComPareChange(int& LastValue,int& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("相机%d的检测参数%s由%d更改为%d",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgAlgorithm::SaveOperaterLog()
{
	///预处理
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dBorderWidth,m_pDetect->m_tParameters.tPretreatParam.dBorderWidth,"tPretreatParam.dBorderWidth");

	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dRightBorderWidth,m_pDetect->m_tParameters.tPretreatParam.dRightBorderWidth,"tPretreatParam.dRightBorderWidth");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dYaHenPingBiKuanDu,m_pDetect->m_tParameters.tPretreatParam.dYaHenPingBiKuanDu,"tPretreatParam.dYaHenPingBiKuanDu");

	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dScaleFactor,m_pDetect->m_tParameters.tPretreatParam.dScaleFactor,"tPretreatParam.dScaleFactor");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dMidDarkCof,m_pDetect->m_tParameters.tPretreatParam.dMidDarkCof,"tPretreatParam.dMidDarkCof");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dMidLightCof,m_pDetect->m_tParameters.tPretreatParam.dMidLightCof,"tPretreatParam.dMidLightCof");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dBorderDarkCof,m_pDetect->m_tParameters.tPretreatParam.dBorderDarkCof,"tPretreatParam.dBorderDarkCof");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dBorderLightCof,m_pDetect->m_tParameters.tPretreatParam.dBorderLightCof,"tPretreatParam.dBorderLightCof");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.dMLPSongjindu,m_pDetect->m_tParameters.tPretreatParam.dMLPSongjindu,"tPretreatParam.dMLPSongjindu");
	ComPareChange(m_pRealDetect->m_tParameters.tPretreatParam.bEnbleUseClassyfy,m_pDetect->m_tParameters.tPretreatParam.bEnbleUseClassyfy,"tPretreatParam.bEnbleUseClassyfy");



	///漏箔参数
	ComPareChange(m_pRealDetect->m_tParameters.tLeakFoilParam.bUseDetect,m_pDetect->m_tParameters.tLeakFoilParam.bUseDetect,"tLeakFoilParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tLeakFoilParam.nThresh,m_pDetect->m_tParameters.tLeakFoilParam.nThresh,"tLeakFoilParam.nThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tLeakFoilParam.dMinArea,m_pDetect->m_tParameters.tLeakFoilParam.dMinArea,"tLeakFoilParam.dMinArea");


	///气泡
	ComPareChange(m_pRealDetect->m_tParameters.tBubbleParam.bUseDetect,m_pDetect->m_tParameters.tBubbleParam.bUseDetect,"tBubbleParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tBubbleParam.nDynThresh,m_pDetect->m_tParameters.tBubbleParam.nDynThresh,"tBubbleParam.nDynThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tBubbleParam.dMinArea,m_pDetect->m_tParameters.tBubbleParam.dMinArea,"tBubbleParam.dMinArea");
	ComPareChange(m_pRealDetect->m_tParameters.tBubbleParam.dMinCircularity,m_pDetect->m_tParameters.tBubbleParam.dMinCircularity,"tBubbleParam.dMinCircularity");



	//线
	ComPareChange(m_pRealDetect->m_tParameters.tDarkLineParam.bUseDetect,m_pDetect->m_tParameters.tDarkLineParam.bUseDetect,"tDarkLineParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tDarkLineParam.nDynThresh,m_pDetect->m_tParameters.tDarkLineParam.nDynThresh,"tDarkLineParam.nDynThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tDarkLineParam.dMinWidth,m_pDetect->m_tParameters.tDarkLineParam.dMinWidth,"tDarkLineParam.dMinWidth");
	ComPareChange(m_pRealDetect->m_tParameters.tDarkLineParam.dMinLengthWidthRatio,m_pDetect->m_tParameters.tDarkLineParam.dMinLengthWidthRatio,"tDarkLineParam.dMinLengthWidthRatio");


	///亮点

	ComPareChange(m_pRealDetect->m_tParameters.tLightDotParam.bUseDetect,m_pDetect->m_tParameters.tLightDotParam.bUseDetect,"tLightDotParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tLightDotParam.nLightDotDynThresh,m_pDetect->m_tParameters.tLightDotParam.nLightDotDynThresh,"tLightDotParam.nLightDotDynThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tLightDotParam.dMinArea,m_pDetect->m_tParameters.tLightDotParam.dMinArea,"tLightDotParam.dMinArea");


	///黑点 

	ComPareChange(m_pRealDetect->m_tParameters.tDarkDotParam.bUseDetect,m_pDetect->m_tParameters.tDarkDotParam.bUseDetect,"tDarkDotParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tDarkDotParam.nDarkDotDynThresh,m_pDetect->m_tParameters.tDarkDotParam.nDarkDotDynThresh,"tDarkDotParam.nDarkDotDynThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tDarkDotParam.dMinArea,m_pDetect->m_tParameters.tDarkDotParam.dMinArea,"tDarkDotParam.dMinArea");

	///胶带


	ComPareChange(m_pRealDetect->m_tParameters.tJiaoDaiParam.bUseDetect,m_pDetect->m_tParameters.tJiaoDaiParam.bUseDetect,"tJiaoDaiParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tJiaoDaiParam.nDarkThresh,m_pDetect->m_tParameters.tJiaoDaiParam.nDarkThresh,"tJiaoDaiParam.nDarkThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tJiaoDaiParam.nLightThresh,m_pDetect->m_tParameters.tJiaoDaiParam.nLightThresh,"tJiaoDaiParam.nLightThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tJiaoDaiParam.dMinArea,m_pDetect->m_tParameters.tJiaoDaiParam.dMinArea,"tJiaoDaiParam.dMinArea");
	////脱碳

	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.bUseDetect,m_pDetect->m_tParameters.tTapeParam.bUseDetect,"tTapeParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.nTapeDynThresh,m_pDetect->m_tParameters.tTapeParam.nTapeDynThresh,"tTapeParam.nMaxThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.dDevFactor,m_pDetect->m_tParameters.tTapeParam.dDevFactor,"tTapeParam.dDevFactor");
	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.dLTMinArea,m_pDetect->m_tParameters.tTapeParam.dLTMinArea,"tTapeParam.dLTMinArea");


	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.nTapeThresh, m_pDetect->m_tParameters.tTapeParam.nTapeThresh, "tTapeParam.nTapeThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tTapeParam.dTapeThreshMinArea, m_pDetect->m_tParameters.tTapeParam.dTapeThreshMinArea, "tTapeParam.dTapeThreshMinArea");





	///拖尾
	ComPareChange(m_pRealDetect->m_tParameters.tTuoWeiParam.bUseDetect,m_pDetect->m_tParameters.tTuoWeiParam.bUseDetect,"tTuoWeiParam.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh,m_pDetect->m_tParameters.tTuoWeiParam.nJXTapeLTMinThresh,"tTuoWeiParam.nJXTapeLTMinThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh,m_pDetect->m_tParameters.tTuoWeiParam.nJXTapeLTMaxThresh,"tTuoWeiParam.nJXTapeLTMaxThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tTuoWeiParam.dJXTapeLTMinArea,m_pDetect->m_tParameters.tTuoWeiParam.dJXTapeLTMinArea,"tTuoWeiParam.dJXTapeLTMinArea");
	ComPareChange(m_pRealDetect->m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight,m_pDetect->m_tParameters.tTuoWeiParam.dJXTuoWeiMaxHeight,"tTuoWeiParam.dJXTuoWeiMaxHeight");


	


	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.bUseDetect,m_pDetect->m_tParameters.tAT9Param.bUseDetect,"tAT9Param.bUseDetect");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.nAT9LMinThresh,m_pDetect->m_tParameters.tAT9Param.nAT9LMinThresh,"tAT9Param.nAT9LMinThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.dAT9LMinWidth,m_pDetect->m_tParameters.tAT9Param.dAT9LMinWidth,"tAT9Param.dAT9LMinWidth");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.dAT9LMinHeight,m_pDetect->m_tParameters.tAT9Param.dAT9LMinHeight,"tAT9Param.dAT9LMinHeight");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.nAT9DarkMaxThresh,m_pDetect->m_tParameters.tAT9Param.nAT9DarkMaxThresh,"tAT9Param.nAT9DarkMaxThresh");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.dAT9DarkMinWidth,m_pDetect->m_tParameters.tAT9Param.dAT9DarkMinWidth,"tAT9Param.dAT9DarkMinWidth");
	ComPareChange(m_pRealDetect->m_tParameters.tAT9Param.dAT9DarkMinHeight,m_pDetect->m_tParameters.tAT9Param.dAT9DarkMinHeight,"tAT9Param.dAT9DarkMinHeight");


}


void CDlgAlgorithm::OnBnClickedBtnSavepara()
{  
	if (m_pDetect->m_bPoweFlag==TRUE)
	{
		SaveOperaterLog();
		m_pRealDetect->DoSomething("COPYOBJECTS",m_pDetect,NULL);
		m_pDetect->m_CSeperate.SaveParam();
		m_pRealDetect->WriteParameters();	
		CDialogEx::OnCancel();
	}else
	{
		AfxMessageBox("修改保存参数需要超级工程师权限或者厂家权限，当前权限无法保存");
		CDialogEx::OnCancel();
	}
	
}


void CDlgAlgorithm::OnMenuFirstdetect()
{



}


void CDlgAlgorithm::OnMenuSaveimage()
{
	try
	{
		SYSTEMTIME CurrentSystime;
		GetLocalTime(&CurrentSystime);
		//保存图片
		CString strPath;
		CString strImageName; 
		strImageName.Format("%d时%d分%d秒.bmp",CurrentSystime.wHour,CurrentSystime.wMinute,CurrentSystime.wSecond);
		CreateDirectory("D:\\SaveImage",NULL);
		strPath.Format("D:\\SaveImage\\%d月%d日\\",CurrentSystime.wMonth,CurrentSystime.wDay);
		CreateDirectory(strPath,NULL);
		/*	WriteImage(m_hoCurrentDispImage,"bmp",NULL,HTuple(strPath+strImageName));
		*/
		WriteImage(m_DetectInput.hoDetectImage,"bmp",NULL,HTuple(strPath+strImageName));

		CString strMessageBox;
		strMessageBox.Format("图片保存完毕\r\n%s%s\r\n",strPath,strImageName);
		AfxMessageBox(strMessageBox);
	}catch(HException e)
	{
		AfxMessageBox("保存图片失败");
	}

}


void CDlgAlgorithm::OnLvnItemchangedListInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if (pNMLV->uNewState== (LVIS_SELECTED | LVIS_FOCUSED))
	{
		try
		{
			DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
			SetColor(m_hvWindowHandle,"red");
			DispRegion(m_DetectOut.ho_ErrorRegion.SelectObj(pNMLV->iItem+1),m_hvWindowHandle);
			m_PageMLPParamSet->m_hoLabelRegion=m_DetectOut.ho_ErrorRegion.SelectObj(pNMLV->iItem+1);


		}catch(HException e)
		{

		}

	}


}


void CDlgAlgorithm::OnMenuCheckset()
{
	
}




void CDlgAlgorithm::OnPaint()
{


	m_cstaticTitle.SetTransparent(TRUE);//将静态文本框设置为透明状态
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	CRect   rcFullDlg,rcTitle,rcCtl;  
	CPaintDC   dc(this);  
	GetClientRect(rcFullDlg);  
	dc.FillSolidRect(rcFullDlg,RGB(255,255,255));
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	dc.FillSolidRect(rcTitle,MainColor);

	int nSubR,nSubG,nSubB;
	nSubR = max(g_Blendent.m_Blendent.Dlg.nR-30,0);
	nSubG = max(g_Blendent.m_Blendent.Dlg.nG-30,0);
	nSubB = max(g_Blendent.m_Blendent.Dlg.nB-30,0);
	COLORREF SubColor = RGB(nSubR,nSubG,nSubB);
	//画标题下边的线
	CPen pen,pen1,pen2,pen3;
	pen.CreatePen(PS_SOLID,1,SubColor);//109,177,61
	dc.SelectObject(pen);
	dc.MoveTo(rcTitle.left,rcTitle.bottom);
	dc.LineTo(rcTitle.right,rcTitle.bottom);
	pen1.CreatePen(PS_SOLID,1,RGB(200,200,200));//94,162,46
	dc.SelectObject(pen1);
	dc.MoveTo(rcTitle.left,rcTitle.bottom+1);
	dc.LineTo(rcTitle.right,rcTitle.bottom+1);
	pen2.CreatePen(PS_SOLID,1,RGB(220,220,220));//154,222,106
	dc.SelectObject(pen2);
	dc.MoveTo(rcTitle.left,rcTitle.bottom+2);
	dc.LineTo(rcTitle.right,rcTitle.bottom+2);
	CDialogEx::OnPaint();
	return;
	pen3.CreatePen(PS_SOLID,1,RGB(200,200,200));
	dc.SelectObject(pen3);
	CString strLogo;
	strLogo.Format("%s\\res\\logo.png",GetCurrentAppPath());
	CRect rcLogo;
	rcLogo.left = 0;
	rcLogo.top = 0;
	rcLogo.bottom = rcTitle.bottom;//标题高39
	rcLogo.right = rcLogo.left + 163;//logo宽163
	CImage Pic;
	Pic.Load(strLogo);
	if (Pic.IsNull())
	{
		return;
	}
	if ( Pic.GetBPP() == 32 ) 
	{ 
		for(int i = 0; i < Pic.GetWidth(); i++) 
		{ 
			for(int j = 0; j < Pic.GetHeight(); j++) 
			{ 
				unsigned char* pucColor = reinterpret_cast<unsigned char *>(Pic.GetPixelAddress(i , j)); 
				pucColor[0] = pucColor[0] * pucColor[3] / 255; 
				pucColor[1] = pucColor[1] * pucColor[3] / 255; 
				pucColor[2] = pucColor[2] * pucColor[3] / 255; 
			} 
		} 
	} 
	::SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);  
	::SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);  
	Pic.Draw(dc.GetSafeHdc(),rcLogo);


	CDialogEx::OnPaint();

}


//void CDlgAlgorithm::OnBnClickedBtnMeasure()
//{
//	//SEPARATEINPUT  SEPARATEINPUT;
//
//	//SEPARATEINPUT.hoGrabImage=m_pDetect->m_ptDetectIn->hoDetectImage;
//	//SEPARATEINPUT.hvHomMat2D=m_pDetect->m_hvHomMat2D;
//	//SEPARATEINPUT.nBreakType=0;
//	//if(IDOK == m_pDetect->m_pMeasure->DoSomething("SHOW",&SEPARATEINPUT,NULL))//如果说已经更改
//	//{
//	//	CriticalSectionHelper lock(&m_pRealDetect->m_csChange);//更改参数是加锁 作用域结束，析构解锁
//	//	m_pRealDetect->m_pMeasure->DoSomething("COPYOBJECTS",m_pDetect->m_pMeasure,NULL);
//	//}
//}


//void CDlgAlgorithm::OnBnClickedBtnDrawmodel()
//{
//
//	CPageGRRSet dlg;
//	dlg.DoModal();
//}


//void CDlgAlgorithm::OnBnClickedBtnDefectcontrol()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	
//
//
//}


void CDlgAlgorithm::OnBnClickedBtnSaveimage()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		SYSTEMTIME CurrentSystime;
		GetLocalTime(&CurrentSystime);
		//保存图片
		CString strPath;
		CString strImageName; 
		strImageName.Format("%d时%d分%d秒.bmp",CurrentSystime.wHour,CurrentSystime.wMinute,CurrentSystime.wSecond);
		CreateDirectory("D:\\SaveImage",NULL);
		strPath.Format("D:\\SaveImage\\%d月%d日\\",CurrentSystime.wMonth,CurrentSystime.wDay);
		CreateDirectory(strPath,NULL);
		//WriteImage(m_hoCurrentDispImage,"bmp",NULL,HTuple(strPath+strImageName));
		WriteImage(m_DetectInput.hoDetectImage,"bmp",NULL,HTuple(strPath+strImageName));
		CString strMessageBox;
		strMessageBox.Format("图片保存完毕\r\n%s%s\r\n",strPath,strImageName);
		AfxMessageBox(strMessageBox);
	}catch(HException e)
	{
		AfxMessageBox("保存图片失败");
	}

}





void CDlgAlgorithm::OnOpenimage()
{
	// TODO: 在此添加命令处理程序代码

	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
	HTuple Width,Height;
	if(IDOK == FileDlg.DoModal())
	{
		CString strFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_hoCurrentDispImage,(HTuple)strFilename);
			m_hoOriginImage=m_hoCurrentDispImage;
			GetImageSize(m_hoCurrentDispImage,&Width,&Height);
			SetPart(m_hvWindowHandle,0,0,Height-1,Width-1);
			DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
			m_DetectInput.hoDetectImage=m_hoCurrentDispImage;
			m_PageMLPParamSet->m_hoCurrentImage=m_hoCurrentDispImage;
			m_PageMeasureParamSet->m_hoPretreatImage=m_hoCurrentDispImage;
			DETECTOUTPUT Out;
			m_pDetect->Detect(m_DetectInput,&Out,0);
		}catch(HException &e )
		{
			AfxMessageBox(e.ErrorMessage().Text());
		}

	}



}











void CDlgAlgorithm::OnMenuCoatBorder()
{
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"red");
	DispObj(m_DetectOut.ho_ErrorRegion,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"green");
	DispObj(m_pDetect->m_hoBoderRegion,m_hvWindowHandle);
}


void CDlgAlgorithm::OnMenuCoatMid()
{
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"red");
	DispObj(m_DetectOut.ho_ErrorRegion,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"green");
	DispObj(m_pDetect->m_hoCoatMid,m_hvWindowHandle);
}


void CDlgAlgorithm::OnClickedButtonTools()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rct;
	GetDlgItem(IDC_BUTTON_TOOLS)->GetWindowRect(&rct);
	CPoint point(rct.left,rct.bottom);
	CMenu* m_tempmenu = m_pMenuTools->GetSubMenu(0);
	m_tempmenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN,point.x,point.y,this);
}


void CDlgAlgorithm::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == 0) 
	{
		m_pMenuTools->DrawItem(lpDrawItemStruct);
	}
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void CDlgAlgorithm::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == 0) 
	{
		m_pMenuTools->MeasureItem(lpMeasureItemStruct);
	}
	CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//void CDlgAlgorithm::OnBnClickedBtnSegment()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//
//}


void CDlgAlgorithm::OnMenuLight()
{
	// TODO: 在此添加命令处理程序代码
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"red");
	DispObj(m_pDetect->m_hoLEnableChoiceRegions,m_hvWindowHandle);
}


void CDlgAlgorithm::OnMenuDark()
{
	// TODO: 在此添加命令处理程序代码
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"red");
	DispObj(m_pDetect->m_hoDEnableChoiceRegions,m_hvWindowHandle);
}


void CDlgAlgorithm::OnMenuSegregion()
{
	// TODO: 在此添加命令处理程序代码
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"green");
	DispObj(m_pDetect->m_hoSegRegions,m_hvWindowHandle);

}


void CDlgAlgorithm::OnMenuAt9region()
{
	// TODO: 在此添加命令处理程序代码

	DispObj(m_DetectInput.hoDetectImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle,"green");
	HObject hoAT9Region;
	hoAT9Region.GenEmptyObj();
    ErosionRectangle1(m_pDetect->m_hoAt9Region,&hoAT9Region,5,1);
	if (hoAT9Region.CountObj()>0)
	{
		DispObj(m_pDetect->m_hoAt9Region,m_hvWindowHandle);
	}
}




void CDlgAlgorithm::OnBnClickedBtnSavesampleimage()
{
	// TODO: 在此添加控件通知处理程序代码

	try
	{
		SYSTEMTIME CurrentSystime;
		GetLocalTime(&CurrentSystime);
		//保存图片
		CString strPath;
		CString strImageName; 
		strImageName.Format("%d时%d分%d秒.bmp",CurrentSystime.wHour,CurrentSystime.wMinute,CurrentSystime.wSecond);
		strPath.Format("D:\\点检图像集\\相机%d\\",m_pDetect->m_tInit.nGrabberSN);
		CreateMultilevelPath(strPath);
	
		WriteImage(m_DetectInput.hoDetectImage,"bmp",NULL,HTuple(strPath+strImageName));
		CString strMessageBox;
		strMessageBox.Format("点检图片保存完毕\r\n%s%s\r\n",strPath,strImageName);
		AfxMessageBox(strMessageBox);
	}catch(HException e)
	{
		AfxMessageBox("保存点检图片失败");
	}
}


void CDlgAlgorithm::OnBnClickedBtnLoadsampleimage()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		static int s_nCount=0;
		HObject  ho_Image;
		HTuple  hv_ImageFiles, hv_Index;
		HTuple hv_IsExist;
		CString strFilieParh;
		strFilieParh.Format("D:\\点检图像集\\相机%d",m_pDetect->m_tInit.nGrabberSN);
		FileExists((HTuple)strFilieParh,&hv_IsExist);
		if (hv_IsExist==0)
		{
			CString strException;
			strException.Format("未发现%s文件夹",strFilieParh);
			AfxMessageBox(strException);
		}

		ListFiles((HTuple)strFilieParh, (HTuple("files").Append("follow_links")),&hv_ImageFiles);
		TupleRegexpSelect(hv_ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima|hobj)$").Append("ignore_case")), 
			&hv_ImageFiles);
		if(hv_ImageFiles.TupleLength()==0)
		{

			CString strException;
			strException.Format("%s文件夹内无点检图像",strFilieParh);
			AfxMessageBox(strException);
		}else if(s_nCount<hv_ImageFiles.TupleLength())
		{
			HTuple Width,Height;
			ReadImage(&m_hoCurrentDispImage,HTuple(hv_ImageFiles[s_nCount]));
			m_hoOriginImage=m_hoCurrentDispImage;
			GetImageSize(m_hoCurrentDispImage,&Width,&Height);
			SetPart(m_hvWindowHandle,0,0,Height-1,Width-1);
			DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
			m_DetectInput.hoDetectImage=m_hoCurrentDispImage;
			Test();
			s_nCount++;
		}else
		{
			CString strException;
			strException.Format("%s文件夹内点检图像点检完毕！！",strFilieParh);
			AfxMessageBox(strException);
			s_nCount=0;
		}
	}catch(HException& e)
	{

	}



}


void CDlgAlgorithm::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码

	CDlgHelp dlg;
	dlg.DoModal();

	//CString strHelpFile;
	//strHelpFile.Format("%s\\参数说明.pdf",GetCurrentAppPath());
	//if (!PathFileExists(strHelpFile))
	//{
	//	
	//	return;
	//}
	//CStdioFile	 helpFile;
	//helpFile.Open(_T(strHelpFile), CFile::modeWrite/*|CFile::typeText*/,NULL);

	//if (helpFile.m_hFile == CFile::hFileNull)
	//{
	//	return;
	//}
	//helpFile.Close();
	//HINSTANCE hInstance = ShellExecute(NULL,_T("open"),_T(strHelpFile),NULL,NULL,SW_SHOW);
	//ASSERT(hInstance != NULL);
	//return;
}




void CDlgAlgorithm::SwitchDispPage(const int & nSN)
{
	if (nSN >= m_vecPages.size())return;

	for (int i = 0; i<m_vecPages.size(); i++)
	{
		if (nSN == i)
			m_vecPages[i]->ShowWindow(SW_NORMAL);
		else
			m_vecPages[i]->ShowWindow(SW_HIDE);
	}
}
void CDlgAlgorithm::OnTcnSelchangeTabParamset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	SwitchDispPage(m_TAB_CParamSet.GetCurSel());
}


void CDlgAlgorithm::OnMenuDrawcoat()
{
	// TODO: 在此添加命令处理程序代码
	m_PageMeasureParamSet->OnMenuDrawcoat();



}


void CDlgAlgorithm::OnMenuJxregion()
{
	// TODO: 在此添加命令处理程序代码
	SetColor(m_hvWindowHandle, "green");
	DispObj(m_pDetect->m_tBlackRegion.ho_JXRegion, m_hvWindowHandle);
}


void CDlgAlgorithm::OnMenuTripregion()
{
	// TODO: 在此添加命令处理程序代码
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle, "blue");
	DispObj(m_pDetect->m_tBlackRegion.hoTripRegion, m_hvWindowHandle);
}


void CDlgAlgorithm::OnMenuYahenpingbiqu()
{
	// TODO: 在此添加命令处理程序代码
	DispObj(m_hoCurrentDispImage,m_hvWindowHandle);
	SetColor(m_hvWindowHandle, "yellow");
	DispObj(m_pDetect->m_tBlackRegion.hoYaHenPingBiRegion, m_hvWindowHandle);
}
