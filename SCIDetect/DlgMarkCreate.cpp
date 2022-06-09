// DlgMarkCreate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SCIDetect.h"
#include "DlgMarkCreate.h"
#include "afxdialogex.h"


// CDlgMarkCreate �Ի���

IMPLEMENT_DYNAMIC(CDlgMarkCreate, CDialogEx)

CDlgMarkCreate::CDlgMarkCreate(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMarkCreate::IDD, pParent)
	, m_CHK_bHelpDraw(FALSE)
	, m_EDT_dJXNearCoatHeight(0)
	, m_CHK_bUseFirstLengthGRR(FALSE)
	, m_COM_nFirstLengthLocation(0)
	, m_CHK_bExistMarkInCoat(FALSE)
	, m_CHK_bPreProcess(FALSE)
	, m_EDT_nLineThresh(0)
	, m_EDT_nBotLineThresh(0)
	, m_COM_nFirstOrLastPoint(0)
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_COM_nFitType = 0;
	m_COM_nReferPoint = 0;
	m_EDT_dOffSet = 0.0;
	m_EDT_nLowThresh = 0;
	m_EDT_nHighThresh = 0;
	m_EDT_nExtractFoilThresh = 0;
	m_EDT_dFoilHeight = 0;
	m_EDT_dFoilWidth = 0;
	m_hoExistInterMissionRegion.GenEmptyObj();
	//  m_EDT_nSpotHighThresh = 0;
	//  m_EDT_nSpotNum = 0;
	//  m_EDT_dSpotArea = 0.0;
	m_EDT_dNearCoatOffset = 0.0;
}

CDlgMarkCreate::~CDlgMarkCreate()
{
	m_ftButton.DeleteObject();
}
void CDlgMarkCreate::SetButtonStaus(BOOL bStaus)
{
	HWND  hwndChild = ::GetWindow(GetSafeHwnd(), GW_CHILD);  //�г����пؼ� 
	CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	int woc;
	CRect Rect;
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
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
void CDlgMarkCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BTN_DRAWMODEL, m_BTN_DRAWMODEL);
	//  DDX_Control(pDX, IDC_BTN_DRAWMODEL, m_BTN_DrawModel);
	DDX_Control(pDX, IDC_BTN_RETURN, m_BTN_Return);
	//  DDX_Control(pDX, IDC_BTN_TEST, m_BTN_Test);
	DDX_Control(pDX, IDCANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDOK, m_BTN_OK);
	DDX_Control(pDX, IDC_STC_TITLE, m_STC_Title);
	DDX_CBIndex(pDX, IDC_COM_CALDISTANCCE, m_COM_nFitType);
	DDX_CBIndex(pDX, IDC_COM_REFERPOINTTYPE, m_COM_nReferPoint);
	DDX_Text(pDX, IDC_EDT_OFFSET, m_EDT_dOffSet);
	DDX_Text(pDX, IDC_EDT_LOWTHRESH, m_EDT_nLowThresh);
	DDX_Text(pDX, IDC_EDT_HIGHTHRESH, m_EDT_nHighThresh);
	DDX_Text(pDX, IDC_EDT_EXTRACTFOILTHRESH, m_EDT_nExtractFoilThresh);
	DDX_Text(pDX, IDC_EDT_FOILHEIGHT, m_EDT_dFoilHeight);
	DDX_Text(pDX, IDC_EDT_FOILWIDTH, m_EDT_dFoilWidth);
	DDX_Check(pDX, IDC_CHK_MANUALTHRESH, m_CHK_bUseManualSeg);
	DDX_Check(pDX, IDC_CHK_AUTOFIND, m_CHK_bUseAotoFind);
	DDX_Control(pDX, IDC_BTN_TEST, m_BTN_CImTest);
	DDX_Control(pDX, IDC_BTN_DETECTREGION, m_BTN_CDrawImRegion);
	DDX_Control(pDX, IDC_BTN_CLEARREGION, m_BTN_CClearIMRegion);
	DDX_Check(pDX, IDC_CHK_HELPDRAW, m_CHK_bHelpDraw);
	//  DDX_Text(pDX, IDC_EDT_SPOTHIGHTHRESH, m_EDT_nSpotHighThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nSpotHighThresh, 0, 255);
	//  DDX_Text(pDX, IDC_EDT_SPOTNUM, m_EDT_nSpotNum);
	//  DDV_MinMaxInt(pDX, m_EDT_nSpotNum, 1, 10);
	//  DDX_Text(pDX, IDC_EDT_SETASPOTREA, m_EDT_dSpotArea);
	//  DDV_MinMaxDouble(pDX, m_EDT_dSpotArea, 0, 10);
	DDX_Text(pDX, IDC_EDT_JXNEARCOATHEIGHT, m_EDT_dJXNearCoatHeight);
	DDV_MinMaxDouble(pDX, m_EDT_dJXNearCoatHeight, 1, 100);
	DDX_Check(pDX, IDC_CHK_USEFIRSTLENGTHGRR, m_CHK_bUseFirstLengthGRR);
	DDX_CBIndex(pDX, IDC_COM_FIRSTLENGTHLOCATION, m_COM_nFirstLengthLocation);
	DDX_Control(pDX, IDC_STC_GRAYVALUE, m_STC_CGrayValue);
	DDX_Check(pDX, IDC_CHK_BEXISTMARK, m_CHK_bExistMarkInCoat);
	DDX_Control(pDX, IDC_BTN_DISPJXDETECT, m_BTN_CDispJXModel);
	DDX_Control(pDX, IDC_BTN_DRAWMARK, m_BTN_CDrawCoatMark);
	//  DDX_Control(pDX, IDC_BTN_CALIBSPOT, m_BTN_CSetCalibObject);
	//  DDX_Control(pDX, IDC_BTN_TESTCALIB, m_BTN_CTestCalibObject);
	DDX_Check(pDX, IDC_CHK_BUSEPREDETECT, m_CHK_bPreProcess);
	DDX_Text(pDX, IDC_EDT_LINETHRESH, m_EDT_nLineThresh);
	DDX_Text(pDX, IDC_EDT_BOTLINETHRESH, m_EDT_nBotLineThresh);
	DDX_CBIndex(pDX, IDC_COM_LASTORFIRSTPOINT, m_COM_nFirstOrLastPoint);
	DDX_Text(pDX, IDC_EDT_JXNEARCOATOFFSET, m_EDT_dNearCoatOffset);
}


BEGIN_MESSAGE_MAP(CDlgMarkCreate, CDialogEx)
//	ON_BN_CLICKED(IDC_BTN_DRAWMODEL, &CDlgMarkCreate::OnBnClickedBtnDrawmodel)
//	ON_BN_CLICKED(IDC_BTN_TEST, &CDlgMarkCreate::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDOK, &CDlgMarkCreate::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_RETURN, &CDlgMarkCreate::OnBnClickedBtnReturn)
	ON_BN_CLICKED(IDCANCEL, &CDlgMarkCreate::OnBnClickedCancel)
//	ON_BN_CLICKED(IDC_BTN_DETECTREGION, &CDlgMarkCreate::OnBnClickedBtnDetectregion)
//	ON_BN_CLICKED(IDC_CHK_AUTOFIND, &CDlgMarkCreate::OnBnClickedChkAutofind)
//	ON_BN_CLICKED(IDC_CHK_USEMODELMATCH, &CDlgMarkCreate::OnBnClickedChkUsemodelmatch)
//	ON_BN_CLICKED(IDC_BTN_ADDSPOTCHECKREGION, &CDlgMarkCreate::OnBnClickedBtnAddspotcheckregion)
//	ON_BN_CLICKED(1036, &CDlgMarkCreate::OnBnClicked1036)
//ON_BN_CLICKED(IDC_BTN_TESTSPOTCHECK, &CDlgMarkCreate::OnBnClickedBtnTestspotcheck)
//ON_BN_CLICKED(IDC_BTN_CLEARREGION, &CDlgMarkCreate::OnBnClickedBtnClearregion)
ON_BN_CLICKED(IDC_BTN_DETECTREGION, &CDlgMarkCreate::OnBnClickedBtnDetectregion)
ON_BN_CLICKED(IDC_BTN_CLEARREGION, &CDlgMarkCreate::OnBnClickedBtnClearregion)
ON_BN_CLICKED(IDC_BTN_TEST, &CDlgMarkCreate::OnBnClickedBtnTest)
//ON_BN_CLICKED(IDC_BTN_CALIBSPOT, &CDlgMarkCreate::OnBnClickedBtnCalibspot)
ON_BN_CLICKED(IDC_BTN_DISPJXDETECT, &CDlgMarkCreate::OnBnClickedBtnDispjxdetect)
//ON_BN_CLICKED(IDC_BTN_TESTCALIB, &CDlgMarkCreate::OnBnClickedBtnTestcalib)
ON_BN_CLICKED(IDC_BTN_DRAWMARK, &CDlgMarkCreate::OnBnClickedBtnDrawmark)
END_MESSAGE_MAP()


// CDlgMarkCreate ��Ϣ�������

void CDlgMarkCreate::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_ftButton);
}
BOOL CDlgMarkCreate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rcClient;
	GetDlgItem(IDC_STC_IMAGE)->GetWindowRect(rcClient);
	ScreenToClient(rcClient);
	m_Wnd_Image.Create("", GetDlgItem(IDC_STC_IMAGE)->GetStyle(), rcClient, this,IDC_STC_IMAGE);   
	m_Wnd_Image.DispImage(m_hoImage);
	m_hvWindowID=m_Wnd_Image.GetDispWnd();
	SetDraw(m_hvWindowID,"margin");

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//���ñ���ɫ ��7����ť
	CStaticExt ;
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_CGrayValue.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);
	SetButtonColors(m_BTN_Return,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_Cancel,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_OK,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);

	SetButtonColors(m_BTN_CImTest,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CDrawImRegion,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CClearIMRegion,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);


	SetButtonColors(m_BTN_CDispJXModel,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);
	SetButtonColors(m_BTN_CDrawCoatMark,0,BKIN,BKOUT,BKFOCUS,FG,FG,FG);






	m_BTN_Return.SetIcon(IDI_ICON_W_UNCHECK);

	m_CHK_bUseAotoFind=m_pTempDetect->m_tParameters.tTempLateInfo.bAotuFindLocate;
	m_CHK_bHelpDraw=m_pTempDetect->m_tParameters.tTempLateInfo.nHelpDraw;
	m_EDT_dFoilHeight=m_pTempDetect->m_tParameters.tTempLateInfo.dFoilHeight;
	m_EDT_dFoilWidth=m_pTempDetect->m_tParameters.tTempLateInfo.dFoilWidth;
	m_EDT_nExtractFoilThresh=m_pTempDetect->m_tParameters.tTempLateInfo.nFoilThresh;
	m_EDT_nLowThresh=m_pTempDetect->m_tParameters.tTempLateInfo.nLowThresh;
	m_EDT_nHighThresh=m_pTempDetect->m_tParameters.tTempLateInfo.nHighThresh;
	m_CHK_bUseManualSeg=m_pTempDetect->m_tParameters.tTempLateInfo.bUseManualSeg;
	m_CHK_bPreProcess=m_pTempDetect->m_tParameters.tTempLateInfo.bUsePreProcess;
	m_EDT_dOffSet=m_pTempDetect->m_tParameters.tTempLateInfo.dOffset;
	m_COM_nFitType=m_pTempDetect->m_tParameters.tTempLateInfo.nFitType;
	m_COM_nFirstLengthLocation=m_pTempDetect->m_tParameters.tTempLateInfo.nFirstLengthLocation;
	m_COM_nReferPoint=m_pTempDetect->m_tParameters.tTempLateInfo.nReferPoint;
	m_hoExistInterMissionRegion=m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion;


	m_CHK_bUseFirstLengthGRR=m_pTempDetect->m_tParameters.tTempLateInfo.bUseFirstLengthGRR;
	m_EDT_dJXNearCoatHeight=m_pTempDetect->m_tParameters.tTempLateInfo.dJXNearCoatHeight;
	m_EDT_dNearCoatOffset=m_pTempDetect->m_tParameters.tTempLateInfo.dNearCoatOffset;

	m_CHK_bExistMarkInCoat=m_pTempDetect->m_tParameters.tTempLateInfo.bExistMarkInCoat;
	m_EDT_nLineThresh=m_pTempDetect->m_tParameters.tTempLateInfo.nLineThresh;
	m_EDT_nBotLineThresh=m_pTempDetect->m_tParameters.tTempLateInfo.nBotLineThresh;
	m_COM_nFirstOrLastPoint=m_pTempDetect->m_tParameters.tTempLateInfo.nFirstOrLastPoint;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//void CDlgMarkCreate::OnBnClickedBtnDrawmodel()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	try
//	{
//		CString strMessage;
//		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
//		HObject hoSearch,ho_ImageReduced,hoMarkRegion;
//		SetButtonStaus(FALSE);
//
//		m_Wnd_Image.m_bDrawing=TRUE;
//		strMessage.Format("�밴ס�������Mark����,�Ҽ�����");
//		SetTposition(m_Wnd_Image.GetDispWnd(),12,12);
//		WriteString(m_Wnd_Image.GetDispWnd(),(HTuple)strMessage);
//
//		DrawRectangle1(m_Wnd_Image.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//		GenRectangle1(&hoMarkRegion, hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//		ReduceDomain(m_hoImage,hoMarkRegion,&ho_ImageReduced);
//
//		CreateNccModel(ho_ImageReduced, "auto",HTuple(-15).TupleRad(), HTuple(15),"auto", "use_polarity",  &m_tTemplateInfo.hv_ModelID);
//		m_tTemplateInfo.hoModelRegion=hoMarkRegion;
//		m_Wnd_Image.DispImage(m_hoImage);
//		m_Wnd_Image.DispRegion(hoMarkRegion,"yellow","margin",2,TRUE);
//		m_Wnd_Image.m_bDrawing=FALSE;
//		SetButtonStaus(TRUE);
//		AfxMessageBox("���Ƴɹ�!");
//
//	}catch(HException& e)
//	{
//		SetButtonStaus(TRUE);
//	}
//
//
//}


//void CDlgMarkCreate::OnBnClickedBtnTest()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//
//	HObject ho_SearchRegion,hoSearchImage;
//	HObject ho_CropPart,hoModelRegion;
//	HTuple hv_LeftTopRow,hv_LeftColumn,hv_RightColumn;
//	HTuple hv_Width,hv_Height;
//	HTuple hv_Row0, hv_Column0, hv_Angle0, hv_Score0;
//	HTuple hv_TimeStart,hv_TimeEnd;
//	m_Wnd_Image.DispImage(m_hoImage);
//	if (m_tTemplateInfo.hoModelRegion.CountObj()==0)
//	{
//		AfxMessageBox("��ǰ������ģ��");
//		return;
//	}
//	CountSeconds(&hv_TimeStart);
//	DilationRectangle1(m_tTemplateInfo.hoModelRegion, &ho_SearchRegion, 300, 1);
//	SmallestRectangle1(ho_SearchRegion,&hv_LeftTopRow,&hv_LeftColumn,NULL,&hv_RightColumn);
//	GetImageSize(m_hoImage,&hv_Width,&hv_Height);
//	GenRectangle1(&ho_SearchRegion,0,hv_LeftColumn,hv_Height-1,hv_RightColumn);
//	ReduceDomain(m_hoImage,ho_SearchRegion,&hoSearchImage);
//	CropDomain(hoSearchImage,&ho_CropPart);
//	FindNccModel(ho_CropPart, m_tTemplateInfo.hv_ModelID,HTuple(-15).TupleRad(), HTuple(15).TupleRad(), m_tTemplateInfo.dMinScore, 1, 0.5, "true", 0, &hv_Row0, &hv_Column0, &hv_Angle0, &hv_Score0);
//	if (hv_Score0.Length()!=0)
//	{
//
//
//	
//		if (m_tTemplateInfo.hoDetectRegion.CountObj()>0)
//		{
//			HTuple hv_Row,hv_Column;
//			HTuple HomMat2D;
//			HObject hoAffinesRegion;
//			HomMat2dIdentity(&HomMat2D);
//			AreaCenter(m_tTemplateInfo.hoModelRegion,NULL,&hv_Row,&hv_Column);
//			VectorAngleToRigid(hv_Row,hv_Column,0,hv_Row0, hv_LeftColumn+hv_Column0, 0,&HomMat2D);
//			AffineTransRegion(m_tTemplateInfo.hoDetectRegion,&hoAffinesRegion, HomMat2D,"nearest_neighbor");
//			AffineTransRegion(m_tTemplateInfo.hoModelRegion, &hoModelRegion, HomMat2D, "nearest_neighbor");
//
//
//			m_Wnd_Image.DispRegion(hoAffinesRegion,"yellow","margin",2,TRUE);
//			m_Wnd_Image.DispRegion(hoModelRegion, "green", "margin", 2, TRUE);
//		}
//		SetTposition(m_Wnd_Image.GetDispWnd(),20,20);
//		CString strValue;
//		CountSeconds(&hv_TimeEnd);
//		strValue.Format("ƥ����:%.2f  ��ʱ��%.2fms",hv_Score0.D(),(hv_TimeEnd.D()-hv_TimeStart.D())*1000);
//		WriteString(m_Wnd_Image.GetDispWnd(),(HTuple)strValue);
//	}else
//	{
//		AfxMessageBox("δ�ҵ���λ");
//	}
//
//
//
//
//}


void CDlgMarkCreate::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FlushParam();
	//UpdateData(TRUE);
//	m_pTempDetect->m_tParameters.tTempLateInfo.bUseSpotCheck=m_CHK_bUseSpotCheck;
	CDialogEx::OnOK();
}


void CDlgMarkCreate::OnBnClickedBtnReturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDlgMarkCreate::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDlgMarkCreate::OnBnClickedBtnDetectregion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	try
	{
		UpdateData(TRUE);
		SetButtonStaus(FALSE);
		CString strMessage;
		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
		HObject hoDetectRegion;
		m_Wnd_Image.m_bDrawing=TRUE;
		strMessage.Format("����Ƽ�Ъ������򣬻��Ƶļ�Ъ����Ҫ������Ъ,�Ҽ�����");

		if (m_CHK_bHelpDraw)
		{
			SmallestRectangle1(m_pTempDetect->m_tSeparateOut.ho_Coat,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
			double dUpInterMissiobHeight=m_pTempDetect->m_tStandardSize.dUpIntermissionHeight.D()/m_pTempDetect->m_dRowResolution;
			double dDownInterMissiobHeight=m_pTempDetect->m_tStandardSize.dDownIntermissionHeight/m_pTempDetect->m_dRowResolution;
			if (m_pTempDetect->m_tInit.nGrabberSN==0 || m_pTempDetect->m_tInit.nGrabberSN==1)
			{
				GenRectangle1(&hoDetectRegion, hv_Row1-dUpInterMissiobHeight-7/m_pTempDetect->m_dRowResolution,hv_Column0-50,hv_Row1,hv_Column1+50);
			}else
			{
				GenRectangle1(&hoDetectRegion, hv_Row1-dDownInterMissiobHeight-7/m_pTempDetect->m_dRowResolution,hv_Column0-50,hv_Row1,hv_Column1+50);
			}

			
		}else
		{
			HTuple hv_Column00,hv_Column11;
			DrawRectangle1(m_Wnd_Image.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
			GenRectangle1(&hoDetectRegion, hv_Row0,hv_Column0,hv_Row1,hv_Column1);
		}
		
		CString strRectangleMessage;
		int nLast=0;
		int nCurrent=1;
		strRectangleMessage.Format("���Ƶļ�Ъ�������Ͻ�:(%.1f,%.1f),���½�:(%.1f,%.1f)",hv_Row0.D(),hv_Column0.D(),hv_Row1.D(),hv_Column1.D());
		ComPareChange(nLast,nCurrent,strRectangleMessage);
		m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion=hoDetectRegion;
		m_Wnd_Image.DispRegion(hoDetectRegion,"green","margin",2,TRUE);
		m_Wnd_Image.m_bDrawing=FALSE;
		SetButtonStaus(TRUE);
		AfxMessageBox("���Ƽ�Ъ������ɣ�");
	}catch(HException& e)
	{

		SetButtonStaus(TRUE);
		AfxMessageBox("���Ƽ�Ъ����ʧ��,�����»��ƣ�");

	}



}


void CDlgMarkCreate::OnBnClickedBtnClearregion()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	  m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion.GenEmptyObj();
}


void CDlgMarkCreate::ComPareChange(double& LastValue,double& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pTempDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("���%d�ļ�����%s��%.2f����Ϊ%.2f",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pTempDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgMarkCreate::ComPareChange(int& LastValue,int& CurrentValue,CString strName)
{
	CString strMessage;
	int nGrabberSN=m_pTempDetect->m_tInit.nGrabberSN;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("���%d�ļ�����%s��%d����Ϊ%d",nGrabberSN,strName,LastValue,CurrentValue);
		((DHInterface*)m_pTempDetect->m_tInit.pLogView)->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}

void CDlgMarkCreate::FlushParam()
{

	UpdateData(TRUE);

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.bAotuFindLocate,m_CHK_bUseAotoFind,"CDlgMarkCreate�Ƿ���ڼ�Ъ");
	m_pTempDetect->m_tParameters.tTempLateInfo.bAotuFindLocate=m_CHK_bUseAotoFind;
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.bUseFirstLengthGRR,m_CHK_bUseFirstLengthGRR,"CDlgMarkCreate����GRRģʽ");
	m_pTempDetect->m_tParameters.tTempLateInfo.bUseFirstLengthGRR=m_CHK_bUseFirstLengthGRR;
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nHelpDraw,m_CHK_bHelpDraw,"CDlgMarkCreate�Ƿ������Ƽ�Ъ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nHelpDraw=m_CHK_bHelpDraw;


	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.bExistMarkInCoat,m_CHK_bExistMarkInCoat,"CDlgMarkCreateͿ�������Ƿ����Mark");
	m_pTempDetect->m_tParameters.tTempLateInfo.bExistMarkInCoat=m_CHK_bExistMarkInCoat;



	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.dFoilHeight,m_EDT_dFoilHeight,"CDlgMarkCreate�����߶�");
	m_pTempDetect->m_tParameters.tTempLateInfo.dFoilHeight=m_EDT_dFoilHeight;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.dFoilWidth,m_EDT_dFoilWidth,"CDlgMarkCreate�������");
	m_pTempDetect->m_tParameters.tTempLateInfo.dFoilWidth=m_EDT_dFoilWidth;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nFoilThresh,m_EDT_nExtractFoilThresh,"CDlgMarkCreate����������ȡ��ֵ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nFoilThresh=m_EDT_nExtractFoilThresh;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nLowThresh,m_EDT_nLowThresh,"CDlgMarkCreate�ֶ���ȡ����ֵ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nLowThresh=m_EDT_nLowThresh;
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nHighThresh,m_EDT_nHighThresh,"CDlgMarkCreate�ֶ���ȡ����ֵ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nHighThresh=m_EDT_nHighThresh;
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.bUseManualSeg,m_CHK_bUseManualSeg,"CDlgMarkCreate�Ƿ������ֶ��ָ�");
	m_pTempDetect->m_tParameters.tTempLateInfo.bUseManualSeg=m_CHK_bUseManualSeg;


	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.bUsePreProcess,m_CHK_bPreProcess,"CDlgMarkCreate�Ƿ�����Ԥ����");
	m_pTempDetect->m_tParameters.tTempLateInfo.bUsePreProcess=m_CHK_bPreProcess;


	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.dOffset,m_EDT_dOffSet,"CDlgMarkCreateƫ��");
	m_pTempDetect->m_tParameters.tTempLateInfo.dOffset=m_EDT_dOffSet;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nFitType,m_COM_nFitType,"CDlgMarkCreate�����׼������");
	m_pTempDetect->m_tParameters.tTempLateInfo.nFitType=m_COM_nFitType;
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nFirstLengthLocation,m_COM_nFirstLengthLocation,"CDlgMarkCreate�׼�������λ��");
	m_pTempDetect->m_tParameters.tTempLateInfo.nFirstLengthLocation=m_COM_nFirstLengthLocation;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nReferPoint,m_COM_nReferPoint,"CDlgMarkCreate�ο���");
	m_pTempDetect->m_tParameters.tTempLateInfo.nReferPoint=m_COM_nReferPoint;



	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.dJXNearCoatHeight,m_EDT_dJXNearCoatHeight,"��ЪͿ��������");
	m_pTempDetect->m_tParameters.tTempLateInfo.dJXNearCoatHeight=m_EDT_dJXNearCoatHeight;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.dNearCoatOffset,m_EDT_dNearCoatOffset,"��ЪͿ����������ƫ��");
	m_pTempDetect->m_tParameters.tTempLateInfo.dNearCoatOffset=m_EDT_dNearCoatOffset;

	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nLineThresh,m_EDT_nLineThresh,"��Ъ����Ѱ����ֵ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nLineThresh=m_EDT_nLineThresh;
	
	
	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nBotLineThresh,m_EDT_nBotLineThresh,"��Ъ�ײ�Ѱ����ֵ");
	m_pTempDetect->m_tParameters.tTempLateInfo.nBotLineThresh=m_EDT_nBotLineThresh;


	ComPareChange(m_pTempDetect->m_tParameters.tTempLateInfo.nFirstOrLastPoint,m_COM_nFirstOrLastPoint,"��ЪѰ�ߵ�ѡ��");
	m_pTempDetect->m_tParameters.tTempLateInfo.nFirstOrLastPoint=m_COM_nFirstOrLastPoint;

}


void CDlgMarkCreate::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FlushParam();
	DETECTINPUT tDetectInput;
	DETECTOUTPUT tDetectOut;
	tDetectInput.hoDetectImage=m_hoImage;
	tDetectInput.nBreakType=0;
	m_pTempDetect->Detect(tDetectInput,&tDetectOut,0);
	SetColor(m_hvWindowID,"green");
	DispObj(m_pTempDetect->m_tSeparateOut.ho_Coat,m_Wnd_Image.GetDispWnd());
	//��ʾ��ǰͼ�����ֵ
	if (m_pTempDetect->m_tInit.nCoatType==M_TwoCam_Intermission)
	{
		CString strValue;
		SetColor(m_hvWindowID,"green");
		strValue.Format("Ĥ����%.3fmm ��Ъ�߶�:%.3fmm �׼��:%.3fmm ������Ŀ:%d ��Բ���߶ȣ�%.3f ��Բ����:%.3f ��©����:%.3f",tDetectOut.hv_Reserve1.D(),tDetectOut.hv_Reserve2.D(),tDetectOut.hv_Reserve3.D(),tDetectOut.hv_Reserve4.I(),tDetectOut.hv_Reserve5.D(),tDetectOut.hv_Reserve6.D(),tDetectOut.hv_Reserve7.D());
		SetTposition(m_hvWindowID,12,12);
		WriteString(m_hvWindowID,(HTuple)strValue);
		SetColor(m_hvWindowID,"yellow");
		DispObj(m_pTempDetect->m_tBlackRegion.ho_FoilSelectRegions,m_hvWindowID);
		SetColor(m_hvWindowID,"pink");
		DispObj(m_pTempDetect->m_tBlackRegion.ho_FoilBottomRegion,m_hvWindowID);
		SetColor(m_hvWindowID,"green");
		DispObj(m_pTempDetect->m_tBlackRegion.hoIgnoreRegion,m_hvWindowID);
		SetColor(m_hvWindowID,"red");
		DispObj(m_pTempDetect->m_tBlackRegion.hoLineContour,m_hvWindowID);
		SetColor(m_hvWindowID,"blue");
		DispObj(m_pTempDetect->m_tBlackRegion.ho_BlackRegion,m_hvWindowID);
		SetColor(m_hvWindowID,"yellow");
		DispObj(m_pTempDetect->m_tBlackRegion.ho_DestRegion,m_hvWindowID);
	}


}


void CDlgMarkCreate::OnBnClickedBtnDispjxdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	try
	{
		if (m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion.CountObj()>0)
		{
			DispObj(m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion,m_hvWindowID);
		}
	}catch(HException& e)
	{

	}

	
}


//void CDlgMarkCreate::OnBnClickedBtnTestcalib()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//
//	FlushParam();
//	SetButtonStaus(FALSE);
//
//	try
//	{
//		CString strMessage;
//		HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1, hv_UsedThreshold;
//		HObject hoSearch,ho_ImageReduced,hoRegion;
//		HObject  hoRectangle;
//		SetButtonStaus(FALSE);
//		m_Wnd_Image.m_bDrawing=TRUE;
//		strMessage.Format("����Ƶ������,�Ҽ���������");
//		SetTposition(m_Wnd_Image.GetDispWnd(),12,12);
//		WriteString(m_Wnd_Image.GetDispWnd(),(HTuple)strMessage);
//		DrawRectangle1(m_Wnd_Image.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
//		GenRectangle1(&hoRectangle, hv_Row0,hv_Column0,hv_Row1,hv_Column1);
//		m_pTempDetect->GetSpotContour(m_hoImage,hoRectangle,&hoRegion);
//		HTuple hv_WidthPixel;
//		if (hoRegion.CountObj()==1)
//		{
//			HTuple hv_H0;
//			HTuple hv_LTRow,hv_LTColumn,hv_RBRow,hv_RBColumn;
//			SmallestRectangle1Xld(hoRegion,&hv_LTRow,&hv_LTColumn,&hv_RBRow,&hv_RBColumn);
//			hv_H0=hv_RBRow-hv_LTRow;
//			HTuple hv_Row0,hv_Column0;
//			CString strInfo;
//			double dRadius=m_pTempDetect->m_tParameters.tTempLateInfo.dSpotResolution*hv_H0/2;
//			strInfo.Format("���:%.3fmm",dRadius*dRadius*M_PI);
//			AreaCenterXld(hoRegion,NULL,&hv_Row0,&hv_Column0,NULL);
//			SetTposition(m_hvWindowID,hv_Row0,hv_Column0);
//			WriteString(m_hvWindowID,(HTuple)strInfo);
//			DispObj(hoRegion,m_hvWindowID);
//		}else
//		{
//			AfxMessageBox("δ��ȡ����Ч�ĵ��Ŀ��,����ڵ����ֵ�ٲ���");
//		}
//
//
//
//		SetButtonStaus(TRUE);
//		m_Wnd_Image.m_bDrawing=FALSE;
//	}catch(HException& e)
//	{
//		SetButtonStaus(TRUE);
//	}
//}


BOOL CDlgMarkCreate::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_MOUSEMOVE/* && GetAsyncKeyState(VK_LCONTROL)*/)
	{
		HTuple Row(0),Column(0),Value(0),PeakGray(0);
		CString strDisp;
		try
		{

			if (m_hoImage.CountObj())
			{
				SetSystem("flush_graphic","false");
				DispObj(m_hoImage,m_hvWindowID);
				SetColor(m_hvWindowID,"red");
				SetSystem("flush_graphic","true");
				GetMposition(m_hvWindowID,&Row,&Column,NULL);
				GetGrayval(m_hoImage,Row,Column,&Value);
				strDisp.Format("�����ͣ����Ϣ:Row:%d  Column:%d �Ҷ�ֵ:%d",Row[0].I(),Column[0].I(),Value.I());
				m_STC_CGrayValue.SetWindowTextA(strDisp);

			}
		}catch(HException e)
		{
			strDisp.Format("�����ͣ����Ϣ:Row:%d  Column:%d �Ҷ�ֵ:---",Row[0].I(),Column[0].I());
			m_STC_CGrayValue.SetWindowTextA(strDisp);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgMarkCreate::OnBnClickedBtnDrawmark()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (m_pTempDetect->m_tInit.nCoatType!=M_TwoCam_Intermission)
	{


		try
		{
			UpdateData(TRUE);
			SetButtonStaus(FALSE);
			CString strMessage;
			HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
			HObject hoDetectRegion;
			m_Wnd_Image.m_bDrawing=TRUE;
			strMessage.Format("�����Mark���μ�������Ҽ���ɼ��");

			{
				DrawRectangle1(m_Wnd_Image.GetDispWnd(),&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
				GenRectangle1(&hoDetectRegion, hv_Row0,hv_Column0,hv_Row1,hv_Column1);
			}

			CString strRectangleMessage;
			int nLast=0;
			int nCurrent=1;
			strRectangleMessage.Format("Mark���μ���������Ͻ�:(%.1f,%.1f),���½�:(%.1f,%.1f)",hv_Row0.D(),hv_Column0.D(),hv_Row1.D(),hv_Column1.D());
			ComPareChange(nLast,nCurrent,strRectangleMessage);
			m_pTempDetect->m_tParameters.tTempLateInfo.hoDetectRegion=hoDetectRegion;
			m_Wnd_Image.DispRegion(hoDetectRegion,"green","margin",2,TRUE);
			m_Wnd_Image.m_bDrawing=FALSE;
			SetButtonStaus(TRUE);
			AfxMessageBox("����Mark������ɣ�");
		}catch(HException& e)
		{

			SetButtonStaus(TRUE);
			AfxMessageBox("����Markʧ��,�����»��ƣ�");

		}
	}

}
