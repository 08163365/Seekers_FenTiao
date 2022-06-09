// DlgSystemSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgSystemSet.h"
#include "afxdialogex.h"


// CDlgSystemSet �Ի���

IMPLEMENT_DYNAMIC(CDlgSystemSet, CDialogEx)

CDlgSystemSet::CDlgSystemSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSystemSet::IDD, pParent)
	, m_nHistoryType(0)
	, m_nHistoryCount(0)
	, m_CHECK_bTileImage(FALSE)
	, m_CHECK_bSaveTitleImage(FALSE)
	, m_EDT_nDistanceCamera(0)
	, m_EDT_nOverTimeExit(0)
	, m_COM_nCameraModel(0)
	, m_CHK_bEnableReLoadParam(FALSE)
	, m_CHK_bPopWindowToWarn(FALSE)
	, m_EDT_nMaxPopWindowNum(0)
	, m_EDT_nOverTimeFrames(0)
	, m_EDT_nTimeLimitValue(0)
{


	m_EDT_nOverTimeLimitLoginOff = 0;
}

CDlgSystemSet::~CDlgSystemSet()
{
}

void CDlgSystemSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLUMNRESOLUTION0, m_cstaticColumnResolution0);
	DDX_Control(pDX, IDC_STATIC_COLUMNRESOLUTION1, m_cstaticColumnResolution1);
	DDX_Control(pDX, IDC_STATIC_ROWRESOLUTION0, m_cstaticRowResolution0);
	DDX_Control(pDX, IDC_STATIC_ROWRESOLUTION1, m_cstaticRowResolution1);
	DDX_CBIndex(pDX, IDC_COMBO_HISTORY_TYPE, m_nHistoryType);
	DDX_Text(pDX, IDC_EDIT_HISTORY_COUNT, m_nHistoryCount);
	DDV_MinMaxInt(pDX, m_nHistoryCount, 1, 30);
	DDX_Control(pDX, IDC_EDIT_HISTORY_COUNT, m_EDIT_historyCount);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_STC_Title);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_BTN_cancel);
	DDX_Control(pDX, IDC_BUTTON_OK, m_BTN_ok);
	DDX_Check(pDX, IDC_CHECK_TILEIMAGE, m_CHECK_bTileImage);
	DDX_Check(pDX, IDC_CHECK_SAVETILEIMAGE_BEFORE, m_CHECK_bSaveTitleImage);
	DDX_Text(pDX, IDC_EDIT_DISTANCECAMERS, m_EDT_nDistanceCamera);
	DDX_Control(pDX, IDC_EDIT_DISTANCECAMERS, m_EDT_DistanceCamera);
	DDX_Text(pDX, IDC_EDT_OVERTIMEEXIT, m_EDT_nOverTimeExit);
	DDX_Control(pDX, IDC_EDT_OVERTIMEEXIT, m_EDT_COverTimeExit);
	DDX_CBIndex(pDX, IDC_COM_MODELCAMERA, m_COM_nCameraModel);
	DDX_Check(pDX, IDC_CHK_RELOADPARAMBYBATCH, m_CHK_bEnableReLoadParam);
	DDX_Check(pDX, IDC_CHK_POPWINDOWTOWARN, m_CHK_bPopWindowToWarn);
	DDX_Text(pDX, IDC_EDT_MAXNUMLIMIT, m_EDT_nMaxPopWindowNum);
	DDX_Check(pDX, IDC_CHECK_MERGEPARAM, m_CHK_bEnableMergeParam);
	DDX_Check(pDX, IDC_CHECK_MERGERESULT, m_CHK_bEnableMergeResult);
	DDX_Text(pDX, IDC_EDT_OVERTIMEFRAMES, m_EDT_nOverTimeFrames);
	DDX_Text(pDX, IDC_EDT_OVERTIMEVALUE, m_EDT_nTimeLimitValue);
	DDX_Text(pDX, IDC_EDT_LOGIOFFTIMELIMIT, m_EDT_nOverTimeLimitLoginOff);
}


BEGIN_MESSAGE_MAP(CDlgSystemSet, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgSystemSet::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgSystemSet::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CDlgSystemSet::OnBnClickedButtonCancel)
END_MESSAGE_MAP()


// CDlgSystemSet ��Ϣ�������

BOOL CDlgSystemSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_nHistoryCount = g_tSystemSet.nHistoryCount;
	m_nHistoryType = g_tSystemSet.nHistoryType;

	m_CHECK_bTileImage = g_tSystemSet.bTileImage;
	m_CHECK_bSaveTitleImage = g_tSystemSet.bSaveTileImage;
	m_EDT_nDistanceCamera = g_tSystemSet.nDistanceCamera;
	
	m_EDT_nOverTimeExit=g_tSystemSet.nOverTimeExit;
	m_COM_nCameraModel=g_tSystemSet.nCameraModel;
    m_CHK_bEnableReLoadParam=g_tSystemSet.bEnableReloadParam;

	m_CHK_bPopWindowToWarn=g_tSystemSet.bEnablePopWindowToWarn;
	m_EDT_nMaxPopWindowNum=g_tSystemSet.nMaxPopWindowNum;


	m_CHK_bEnableMergeResult=g_tSystemSet.nMergeResults;
	m_CHK_bEnableMergeParam=g_tSystemSet.nMergeParam ;
	
	m_EDT_nOverTimeFrames=g_tSystemSet.nOverTimeFrameValue;
	m_EDT_nTimeLimitValue=g_tSystemSet.nOverTimeValue;

	m_EDT_nOverTimeLimitLoginOff=g_tSystemSet.nOverTimeLimitLoginOff;

	InitStatic();
	//((CButton*)GetDlgItem(IDC_CHECK_MERGERESULT))->SetCheck(g_tSystemSet.nMergeResults);
	//((CButton*)GetDlgItem(IDC_CHECK_MERGEPARAM))->SetCheck(g_tSystemSet.nMergeParam);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}





void CDlgSystemSet::InitStatic()
{
	m_FontButton.CreatePointFont(100,"Microsoft YaHei");

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;


	m_STC_Title.SetTextColor(RGB(255,255,255));
	m_STC_Title.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STC_Title.SetBkColor(MainColor,0,CStaticExt::Normal);


	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);

	m_BTN_ok.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_ok.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	
	m_BTN_cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);


	



	m_BTN_ok.SetFont(&m_FontButton);
	m_BTN_cancel.SetFont(&m_FontButton);

	m_cstaticColumnResolution0.SetTextColor(RGB(0,255,0));
	m_cstaticColumnResolution1.SetTextColor(RGB(0,255,0));
	m_cstaticRowResolution0.SetTextColor(RGB(0,255,0));
	m_cstaticRowResolution1.SetTextColor(RGB(0,255,0));

	m_cstaticColumnResolution0.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticColumnResolution1.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticRowResolution0.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);
	m_cstaticRowResolution1.SetBkColor( RGB(0,0,0),RGB(0,0,0),CStaticExt::Normal);

	
	m_FontEdit.CreateFontA(20,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"����ϸ��");
	GetDlgItem(IDC_EDIT_HISTORY_COUNT)->SetFont(&m_FontEdit);

	m_FontCombo.CreateFontA(15,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS
		,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,"����ϸ��");
	GetDlgItem(IDC_COMBO_HISTORY_TYPE)->SetFont(&m_FontCombo);


	try
	{

		{
			m_cstaticColumnResolution0.Format("%.5f",g_vec_ColumnResolution[0]);
			m_cstaticColumnResolution1.Format("%.5f",g_vec_ColumnResolution[1]);
			m_cstaticRowResolution0.Format("%.5f",g_vec_RowResolution[0]);
			m_cstaticRowResolution1.Format("%.5f",g_vec_RowResolution[1]);
		}


	}catch(std::out_of_range)
	{

	}
	m_EDIT_historyCount.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_historyCount.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);



	m_EDT_DistanceCamera.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_DistanceCamera.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);

	
	


	m_EDT_COverTimeExit.SetBkGndColor(g_Blendent.m_Blendent.Edit.BKColor,g_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_COverTimeExit.SetBorderColor(g_Blendent.m_Blendent.Edit.Normal,
		g_Blendent.m_Blendent.Edit.Hover,g_Blendent.m_Blendent.Edit.Focus);
	

}

void CDlgSystemSet::ComPareChange(int LastValue,int  CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("ϵͳ���ò���%s��%d����Ϊ%d",strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}
BOOL CDlgSystemSet::Save()
{
	if (UpdateData(TRUE))
	{
		ComPareChange(g_tSystemSet.nHistoryCount,m_nHistoryCount,"����ͼ����Ŀ");
		ComPareChange(g_tSystemSet.nHistoryType,m_nHistoryType,"����ͼ������");
		ComPareChange(g_tSystemSet.bTileImage,m_CHECK_bTileImage,"ƴͼʹ��");
		ComPareChange(g_tSystemSet.bSaveTileImage,m_CHECK_bSaveTitleImage,"����ƴͼǰͼ��ʹ��");
		ComPareChange(g_tSystemSet.nDistanceCamera,m_EDT_nDistanceCamera,"ǰ��������ؾ���");
		ComPareChange(g_tSystemSet.nCameraModel,m_COM_nCameraModel,"��λ���");
		ComPareChange(g_tSystemSet.bEnableReloadParam,m_CHK_bEnableReLoadParam,"����ԭʹ��");
		
		ComPareChange(g_tSystemSet.bEnablePopWindowToWarn,m_CHK_bPopWindowToWarn,"����ʹ��");
		ComPareChange(g_tSystemSet.nMaxPopWindowNum,m_EDT_nMaxPopWindowNum,"��󵯴�����");
		ComPareChange(g_tSystemSet.nMergeResults,m_CHK_bEnableMergeResult,"������ʹ����ͬ����ʹ��");
		ComPareChange(g_tSystemSet.nMergeParam,m_CHK_bEnableMergeParam,"�ߴ���ʹ����ͬ����ʹ��");
		ComPareChange(g_tSystemSet.nOverTimeExit,m_EDT_nOverTimeExit,"��ʱ��Դ�ر�����");

		ComPareChange(g_tSystemSet.nOverTimeFrameValue,m_EDT_nOverTimeFrames,"������ʱ֡��");
		ComPareChange(g_tSystemSet.nOverTimeValue,m_EDT_nTimeLimitValue,"��ʱ����");

		ComPareChange(g_tSystemSet.nOverTimeLimitLoginOff,m_EDT_nOverTimeLimitLoginOff,"��½��ʱ�˳�����");

	
		


		g_tSystemSet.nHistoryCount   = m_nHistoryCount;
		g_tSystemSet.nHistoryType    = m_nHistoryType;
		
		g_tSystemSet.bTileImage      = m_CHECK_bTileImage;
		g_tSystemSet.bSaveTileImage  = m_CHECK_bSaveTitleImage;
		g_tSystemSet.nDistanceCamera = m_EDT_nDistanceCamera;
	
		g_tSystemSet.nOverTimeExit=m_EDT_nOverTimeExit;
		g_tSystemSet.nCameraModel=m_COM_nCameraModel;
		g_tSystemSet.bEnableReloadParam=m_CHK_bEnableReLoadParam;
	
		g_tSystemSet.bEnablePopWindowToWarn=m_CHK_bPopWindowToWarn;
		g_tSystemSet.nMaxPopWindowNum=m_EDT_nMaxPopWindowNum;
		g_tSystemSet.nMergeResults = m_CHK_bEnableMergeResult;
		g_tSystemSet.nMergeParam = m_CHK_bEnableMergeParam;

		g_tSystemSet.nOverTimeFrameValue=m_EDT_nOverTimeFrames;
		g_tSystemSet.nOverTimeValue=m_EDT_nTimeLimitValue;

		g_tSystemSet.nOverTimeLimitLoginOff=m_EDT_nOverTimeLimitLoginOff;


		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//���������ļ�Ϊ��д��ģʽ
		WritePrivateProfileInt("System","MergeResults",g_tSystemSet.nMergeResults,g_strConfigSystemFile);//
		WritePrivateProfileInt("System","MergeParam",g_tSystemSet.nMergeParam,g_strConfigSystemFile);//д����ʷ�ؿ�����
		WritePrivateProfileInt("System","HistoryCount",g_tSystemSet.nHistoryCount,g_strConfigSystemFile);//д����ʷ�ؿ�����
		WritePrivateProfileInt("System","HistoryType",g_tSystemSet.nHistoryType,g_strConfigSystemFile);//д����ʷ�ؿ�����
		WritePrivateProfileInt("System","bEnableReloadParam",g_tSystemSet.bEnableReloadParam ,g_strConfigSystemFile);
		WritePrivateProfileInt("System","nOverTimeLimitLoginOff",g_tSystemSet.nOverTimeLimitLoginOff ,g_strConfigSystemFile);


		WritePrivateProfileInt("System","bEnablePopWindowToWarn",g_tSystemSet.bEnablePopWindowToWarn ,g_strConfigSystemFile);


		WritePrivateProfileInt("System","nMaxPopWindowNum",g_tSystemSet.nMaxPopWindowNum ,g_strConfigSystemFile);

		
		WritePrivateProfileInt("System","nOverTimeExit",g_tSystemSet.nOverTimeExit,g_strConfigSystemFile);//��ʱ�˳�

		WritePrivateProfileInt("System","TileImage",g_tSystemSet.bTileImage,g_strConfigSystemFile);//д����ʷ�ؿ�����
		WritePrivateProfileInt("System","DistanceCamera",g_tSystemSet.nDistanceCamera,g_strConfigSystemFile);//д����ʷ�ؿ�����
		WritePrivateProfileInt("System","nCameraModel",g_tSystemSet.nCameraModel,g_strConfigSystemFile);//д����ʷ�ؿ�����

		WritePrivateProfileInt("System","nOverTimeFrameValue",g_tSystemSet.nOverTimeFrameValue ,g_strConfigSystemFile);
		WritePrivateProfileInt("System","nOverTimeValue",g_tSystemSet.nOverTimeValue ,g_strConfigSystemFile);


		SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//���������ļ�Ϊֻ��ģʽ
		
		
		CDialogEx::OnOK();
		return TRUE;
	}else
	{
		return FALSE;
	}


}


void CDlgSystemSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg,rcTitle,rcClient;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	//���߿���
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);
}


HBRUSH CDlgSystemSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CDlgSystemSet::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CMFCPropertyPage::OnEraseBkgnd(pDC);
}


void CDlgSystemSet::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (WA_INACTIVE == nState)  
	{  
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
		this->SetLayeredWindowAttributes(0,(255*85)/100,LWA_ALPHA);
	}else if (WA_CLICKACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}else if (WA_ACTIVE == nState)
	{
		::SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE
			,::GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
		this->RedrawWindow();
	}
}     


LRESULT CDlgSystemSet::OnNcHitTest(CPoint point)
{
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);

	//return CDialogEx::OnNcHitTest(point);
}


BOOL CDlgSystemSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//���λس�
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)//����ESC
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgSystemSet::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CDlgSystemSet::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Save();
	CDialogEx::OnOK();
}


void CDlgSystemSet::OnBnClickedButtonCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

