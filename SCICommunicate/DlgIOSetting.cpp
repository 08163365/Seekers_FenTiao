// DlgIOSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "SCICommunicate.h"
#include "DlgIOSetting.h"
#include "resource.h"

// CDlgIOSetting 对话框

IMPLEMENT_DYNAMIC(CDlgIOSetting, CDialogEx)

CDlgIOSetting::CDlgIOSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgIOSetting::IDD, pParent)
	, m_nIOCardSN(0)
	, m_nRejectType(0)
	, m_EDT_DabiaoDelay(0)
	, m_EDT_dLabelActionLocation(0)
	, m_EDT_dFirstLabelLength(0)
	, m_EDT_dSecondLabelLength(0)
	, m_EDT_nFreshImageSNDelay(0)
	, m_EDT_nTBFenPinNum(0)
	, m_EDT_dVirtualEALength(0)
	, m_COM_nTestLabelIndex(0)
	, m_CHK_bMachineLengthLabel(FALSE)
	, m_dLabelOffsetDev(0)
	, m_EDT_dDownLabelOffsetCompensate(0)
	, m_EDT_dUpLabelOffsetCompensate(0)
	, m_CHK_bUseLabelOffset(FALSE)
{
	m_ftButton.CreatePointFont(100,"Microsoft YaHei");
	m_ftEdit.CreatePointFont(90,"Microsoft YaHei");
	m_nRejectDelay = 0;
	m_nRejectDelay1 = 0;
	m_EDT_nTiFeiPulse = 80;
	m_EDT_nBuChangHangShu = 60;
	m_COM_nChannel0 = 0;
	m_COM_nChannel1 = 0;
	m_COM_nChannel10 = 0;
	m_COM_nChannel11 = 0;
	m_COM_nChannel2 = 0;
	m_COM_nChannel3 = 0;
	m_COM_nChannel4 = 0;
	m_COM_nChannel5 = 0;
	m_COM_nChannel6 = 0;
	m_COM_nChannel7 = 0;
	m_COM_nChannel8 = 0;
	m_COM_nChannel9 = 0;
}

CDlgIOSetting::~CDlgIOSetting()
{
	m_ftButton.DeleteObject();
	m_ftEdit.DeleteObject();
}

void CDlgIOSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_IOCARDSN, m_nIOCardSN);
	DDV_MinMaxInt(pDX, m_nIOCardSN, 0, 4);
	DDX_Control(pDX, IDC_BUTTON_CALLIOCARDPARAMSET, m_btnCallIOCardParamset);
	DDX_Radio(pDX, IDC_RADIO_REJECT, m_nRejectType);
	DDX_Control(pDX, IDC_EDIT_IOCARDSN, m_IOCardSN);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDCANCEL, m_BTN_Cancel);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_Close);
	DDX_Control(pDX, IDC_EDIT_DELAY_REJECT, m_EDIT_rejectDelay);
	DDX_Text(pDX, IDC_EDIT_DELAY_REJECT, m_nRejectDelay);
	DDX_Text(pDX, IDC_EDT_DABIAODELAY, m_EDT_DabiaoDelay);
	DDX_Control(pDX, IDC_EDT_DABIAODELAY, m_EDT_CDABIAODELAY);
	DDX_Control(pDX, IDC_EDIT_DELAY_REJECT1, m_EDIT_rejectDelay1);
	DDX_Text(pDX, IDC_EDIT_DELAY_REJECT1, m_nRejectDelay1);
	DDX_Text(pDX, IDC_EDT_TIFEIPULSE, m_EDT_nTiFeiPulse);
	DDX_Control(pDX, IDC_EDT_TIFEIPULSE, m_EDT_CTiFeiPulse);
	DDX_Text(pDX, IDC_EDT_BUCHANGHANGSHU, m_EDT_nBuChangHangShu);
	DDX_Control(pDX, IDC_EDT_BUCHANGHANGSHU, m_EDT_CBuChangHangShu);
	DDX_Text(pDX, IDC_EDT_LABELACTIONOFFSET, m_EDT_dLabelActionLocation);
	DDX_Text(pDX, IDC_EDT_FIRSTLABELLENGTH, m_EDT_dFirstLabelLength);
	DDX_Text(pDX, IDC_EDT_SECONDLABELLENGTH, m_EDT_dSecondLabelLength);
	DDX_Control(pDX, IDC_EDT_LABELACTIONOFFSET, m_EDT_CLabelActionOffset);
	DDX_Control(pDX, IDC_EDT_SECONDLABELLENGTH, m_EDT_CSecondLabelLength);
	DDX_Control(pDX, IDC_EDT_FIRSTLABELLENGTH, m_EDT_CFirstLabelLength);
	DDX_Text(pDX, IDC_EDT_FRESHIMAGESNDELAY, m_EDT_nFreshImageSNDelay);
	DDX_Text(pDX, IDC_EDT_FENPIN, m_EDT_nTBFenPinNum);
	DDX_Control(pDX, IDC_EDT_FENPIN, m_EDT_CTBFenPin);
	DDX_Control(pDX, IDC_EDT_FRESHIMAGESNDELAY, m_EDT_CFreshImageSN);
	DDX_Check(pDX, IDC_CHK_ENABLESETLABEL, m_CHK_bEnableLabelSet);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL0, m_COM_nChannel0);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL1, m_COM_nChannel1);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL10, m_COM_nChannel10);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL11, m_COM_nChannel11);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL2, m_COM_nChannel2);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL3, m_COM_nChannel3);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL4, m_COM_nChannel4);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL5, m_COM_nChannel5);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL6, m_COM_nChannel6);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL7, m_COM_nChannel7);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL8, m_COM_nChannel8);
	DDX_CBIndex(pDX, IDC_COM_CHANNEL9, m_COM_nChannel9);
	DDX_Text(pDX, IDC_EDT_VIRTUALEALENGTH, m_EDT_dVirtualEALength);
	DDX_CBIndex(pDX, IDC_COM_LABELTEST, m_COM_nTestLabelIndex);
	DDX_Check(pDX, IDC_CHK_MACHINELABEL, m_CHK_bMachineLengthLabel);
	DDX_Text(pDX, IDC_EDT_DOFFSETDEV, m_dLabelOffsetDev);
	DDX_Text(pDX, IDC_EDT_DOWNCOMPENSATE2, m_EDT_dDownLabelOffsetCompensate);
	DDX_Text(pDX, IDC_EDT_UPCOMPENSATE, m_EDT_dUpLabelOffsetCompensate);
	DDX_Check(pDX, IDC_CHK_ENABLEOFFSET, m_CHK_bUseLabelOffset);
	DDX_Control(pDX, IDC_LST_OFFSETINFO, m_LST_CLabelOffsetInfo);
}


BEGIN_MESSAGE_MAP(CDlgIOSetting, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_CALLIOCARDPARAMSET, &CDlgIOSetting::OnBnClickedButtonCalliocardparamset)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CDlgIOSetting::OnBnClickedButtonOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &CDlgIOSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgIOSetting::OnBnClickedButtonClose)
ON_BN_CLICKED(IDC_BTN_CALDISTINMM, &CDlgIOSetting::OnBnClickedBtnCaldistinmm)
ON_BN_CLICKED(IDC_BTN_RERESHDELAYLINE, &CDlgIOSetting::OnBnClickedBtnRereshdelayline)
ON_BN_CLICKED(IDC_CHK_ENABLESETLABEL, &CDlgIOSetting::OnBnClickedChkEnablesetlabel)
ON_BN_CLICKED(IDC_BTN_TESTLABELOUPUT, &CDlgIOSetting::OnBnClickedBtnTestlabelouput)
ON_BN_CLICKED(IDC_BTN_LABELRESET, &CDlgIOSetting::OnBnClickedBtnLabelreset)
ON_BN_CLICKED(IDC_BTN_ONEKEYCALIB, &CDlgIOSetting::OnBnClickedBtnOnekeycalib)
//ON_BN_CLICKED(IDC_BTN_READSENSOR, &CDlgIOSetting::OnBnClickedBtnReadsensor)
END_MESSAGE_MAP()


// CDlgIOSetting 消息处理程序


BOOL CDlgIOSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	InitValue();
	m_cstaticTitle.SetFocus();
	return FALSE;
	  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgIOSetting::InitUI()
{
	COLORREF MainColor = m_Blendent.m_Blendent.Dlg.MainColor;

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	short	shBtnColor = m_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = m_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = m_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = m_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = m_Blendent.m_Blendent.Button.FGColorOut;

	m_btnCallIOCardParamset.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCallIOCardParamset.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCallIOCardParamset.SetFont(&m_ftButton);
	m_btnCallIOCardParamset.SetIcon(IDI_ICON_W_SET);

	m_btnOK.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnOK.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnOK.SetFont(&m_ftButton);


	m_BTN_Cancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Cancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_Cancel.SetFont(&m_ftButton);

	m_BTN_Close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_Close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_Close.SetFont(&m_ftButton);
	m_BTN_Close.SetIcon(IDI_ICON_W_UNCHECK);


	m_EDT_CLabelActionOffset.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CLabelActionOffset.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);

	m_EDT_CLabelActionOffset.SetFont(&m_ftEdit);
	m_EDT_CSecondLabelLength.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CSecondLabelLength.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);

	m_EDT_CFirstLabelLength.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CFirstLabelLength.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);



	m_EDIT_rejectDelay.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_rejectDelay.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);
	m_EDIT_rejectDelay1.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDIT_rejectDelay1.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);



	
	m_EDT_CDABIAODELAY.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor, m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CDABIAODELAY.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover, m_Blendent.m_Blendent.Edit.Focus);

	m_IOCardSN.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_IOCardSN.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);



	
	m_EDT_CBuChangHangShu.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CBuChangHangShu.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);

	m_EDT_CTiFeiPulse.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CTiFeiPulse.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);
	
	m_IOCardSN.SetFont(&m_ftEdit);
	m_EDT_CDABIAODELAY.SetFont(&m_ftEdit);
	m_EDT_CBuChangHangShu.SetFont(&m_ftEdit);
	m_EDT_CTiFeiPulse.SetFont(&m_ftEdit);




	m_EDT_CTBFenPin.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CTBFenPin.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);

	m_EDT_CFreshImageSN.SetBkGndColor(m_Blendent.m_Blendent.Edit.BKColor,m_Blendent.m_Blendent.Edit.BKColorFocus);
	m_EDT_CFreshImageSN.SetBorderColor(m_Blendent.m_Blendent.Edit.Normal,
		m_Blendent.m_Blendent.Edit.Hover,m_Blendent.m_Blendent.Edit.Focus);








	m_EDT_CTBFenPin.SetFont(&m_ftEdit);
	m_EDT_CFreshImageSN.SetFont(&m_ftEdit);



	m_LST_CLabelOffsetInfo.SetColorHeader(TRUE);
	m_LST_CLabelOffsetInfo.SetHeaderBKColor(255, 255, 255, 0);
	m_LST_CLabelOffsetInfo.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER);

	CRect rcRect;
	m_LST_CLabelOffsetInfo.GetWindowRect(rcRect);
	m_LST_CLabelOffsetInfo.SetEditable(TRUE);

	m_LST_CLabelOffsetInfo.InsertColumn(0, "贴标机序号", LVCFMT_LEFT, rcRect.Width()/ 3 - 1);
	m_LST_CLabelOffsetInfo.InsertColumn(1, "偏移补偿(mm)", LVCFMT_LEFT, rcRect.Width()/ 3 - 1);
	m_LST_CLabelOffsetInfo.InsertColumn(2, "实测偏移(mm)", LVCFMT_LEFT, rcRect.Width() / 3 - 1);


	
}

BOOL CDlgIOSetting::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
	{
		CRect rcTitle;
		GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);
		if (rcTitle.PtInRect(pMsg->pt))
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION,  pMsg->lParam);
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgIOSetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   rcDlg,rcTitle,rcClient,rcCtl;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg);
	GetDlgItem(IDC_STATIC_TITLE)->GetClientRect(rcTitle); 
	rcClient = rcDlg;
	rcClient.top = rcTitle.bottom;
	dc.FillSolidRect(rcClient,RGB(255,255,255));

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);

	//CImage Image;
	//CString strBmp;
	//strBmp.Format("%s//res//DelayModel.bmp",GetCurrentAppPath());
	//if (!PathFileExistsA(strBmp))
	//{
	//	return;
	//}
	//Image.Load(strBmp);
	//if (Image.IsNull())
	//{
	//	return;
	//}
	//CRect rcBmp;
	//GetDlgItem(IDC_STATIC_DELAYMODEL)->GetWindowRect(rcBmp); 
	//ScreenToClient(rcBmp);

	//::SetStretchBltMode(dc.GetSafeHdc(), HALFTONE);  
	//::SetBrushOrgEx(dc.GetSafeHdc(), 0, 0, NULL);  
	//Image.Draw(dc.GetSafeHdc(),rcBmp);
}


BOOL CDlgIOSetting::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CDlgIOSetting::InitValue()
{
	m_nRejectType =  m_pCommuicate->m_Commuicate.nRejectType;
	m_EDT_dFirstLabelLength =  m_pCommuicate->m_Commuicate.dFirstLabelLength;
	m_EDT_dSecondLabelLength =  m_pCommuicate->m_Commuicate.dSecondLabelLength;
	m_nRejectDelay =  m_pCommuicate->m_Commuicate.nXLengthInPixel;
	m_nRejectDelay1 =  m_pCommuicate->m_Commuicate.n2ndLengthInPixel;
	m_EDT_dLabelActionLocation=  m_pCommuicate->m_Commuicate.dLabelPosition;
	m_EDT_nTiFeiPulse= m_pCommuicate->m_Commuicate.nTiFeiPulse;
	m_EDT_nBuChangHangShu=m_pCommuicate->m_Commuicate.nBuChangHangShu;
	m_CHK_bMachineLengthLabel=m_pCommuicate->m_Commuicate.bMachineLengthLabel;
    m_EDT_dVirtualEALength=m_pCommuicate->m_Commuicate.dVirtualEALength;

	m_CHK_bUseLabelOffset= m_pCommuicate->m_Commuicate.bUseLabelOffset;
	m_dLabelOffsetDev= m_pCommuicate->m_Commuicate.dLabelOffsetDev;

	m_EDT_dUpLabelOffsetCompensate = m_pCommuicate->m_Commuicate.dUpLabelOffsetCompensate;
	m_EDT_dDownLabelOffsetCompensate = m_pCommuicate->m_Commuicate.dDownLabelOffsetCompensate;




	if (m_pCommuicate->m_IOCard[0].hasCard)
	{

		m_nRejectDelay = m_pCommuicate->m_IOCard[0].readParam(49);//打标X 设置行数
        m_nRejectDelay1=m_pCommuicate->m_IOCard[0].readParam(76);//2nd 设置行数
		m_EDT_DabiaoDelay = m_pCommuicate->m_IOCard[0].readParam(161);//贴标机反应时间
		m_EDT_nBuChangHangShu=m_pCommuicate->m_IOCard[0].readParam(162);//总的补偿行数
		m_EDT_nTiFeiPulse=m_pCommuicate->m_IOCard[0].readParam(92);//踢废队列输出脉冲
		m_pCommuicate->m_Commuicate.nBuChangHangShu=m_EDT_nBuChangHangShu;
		m_EDT_nFreshImageSNDelay=m_pCommuicate->m_IOCard[0].readParam(63);//显示图像号延迟更新
			//	int a=(4348&0x00f0)>>4;
		m_EDT_nTBFenPinNum=(m_pCommuicate->m_IOCard[0].readParam(28)&0xf0)>>4;//显示分频数

	}

	//GetDlgItem(IDC_EDT_FIRSTLABELLENGTH)->EnableWindow(0);
	//GetDlgItem(IDC_EDT_SECONDLABELLENGTH)->EnableWindow(0);

	GetDlgItem(IDC_EDT_FRESHIMAGESNDELAY)->EnableWindow(0);
	GetDlgItem(IDC_EDT_FENPIN)->EnableWindow(0);

	GetDlgItem(IDC_EDT_BUCHANGHANGSHU)->EnableWindow(0);
	GetDlgItem(IDC_EDT_DABIAODELAY)->EnableWindow(0);



	for (int i=0;i<m_pCommuicate->m_Commuicate.hvChannelChoice.Length();i++)///通道选择
	{
		switch (i)
		{
		case 0:
			m_COM_nChannel0=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL0)->EnableWindow(0);
			break;
		case 1:
			m_COM_nChannel1=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL1)->EnableWindow(0);
			break;
		case 2:
			m_COM_nChannel2=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL2)->EnableWindow(0);
			break;
		case 3:
			m_COM_nChannel3=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL3)->EnableWindow(0);
			break;
		case 4:
			m_COM_nChannel4=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL4)->EnableWindow(0);
			break;
		case 5:
			m_COM_nChannel5=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL5)->EnableWindow(0);
			break;
		case 6:
			m_COM_nChannel6=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL6)->EnableWindow(0);
			break;
		case 7:
			m_COM_nChannel7=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL7)->EnableWindow(0);
			break;
		case 8:
			m_COM_nChannel8=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL8)->EnableWindow(0);
			break;
		case 9:
			m_COM_nChannel9=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL9)->EnableWindow(0);
			break;
		case 10:
			m_COM_nChannel10=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL10)->EnableWindow(0);
			break;
		case 11:
			m_COM_nChannel11=m_pCommuicate->m_Commuicate.hvChannelChoice[i];
			GetDlgItem(IDC_COM_CHANNEL11)->EnableWindow(0);
			break;

		}

			

		
	}


	

		CString strSingleName, strValue;
		int nItemCount;
		HTuple hvRealMonitorLengthTuple;
		m_pCommuicate->m_sInit.pSysCall("GETPARAM","LabelRealLocation",&m_pCommuicate->m_hvRealMonitorLengthTuple);
	    for (int i = 0; i<MAX_LALBELNUM; i++)
	    {
		nItemCount = m_LST_CLabelOffsetInfo.GetItemCount();
		strSingleName.Format("%d", i+1);
		m_LST_CLabelOffsetInfo.InsertItem(nItemCount, strSingleName);
		strValue.Format("%.1f", m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple[i].D());
		m_LST_CLabelOffsetInfo.SetItemText(nItemCount, 1, strValue);
		strValue.Format("%.1f", m_pCommuicate->m_hvRealMonitorLengthTuple[i].D());
		m_LST_CLabelOffsetInfo.SetItemText(nItemCount, 2, strValue);
	   }

	UpdateData(FALSE);
}

void CDlgIOSetting::Save()
{



	UpdateData(TRUE);
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.nRejectType,m_nRejectType,"贴标控制开关");
	m_pCommuicate->m_Commuicate.nRejectType = m_nRejectType;
	if (m_pCommuicate->m_IOCard[0].hasCard)
	{

		m_pCommuicate->m_IOCard[0].writeParam(49,m_nRejectDelay);//打标队列x
		m_pCommuicate->m_IOCard[0].writeParam(76,m_nRejectDelay1);//第二路打标延时队列
		m_pCommuicate->m_IOCard[0].writeParam(161, m_EDT_DabiaoDelay);//打标延时
		m_pCommuicate->m_IOCard[0].writeParam(162, m_EDT_nBuChangHangShu);//总的补偿行数
		m_pCommuicate->m_IOCard[0].writeParam(92, m_EDT_nTiFeiPulse);//踢废队列输出脉冲


		CString strFile = GetCurrentAppPath() + "\\PIO24B_reg_init.txt";
		CString strApp = "PIO24B";
		CString strValue;
		strValue.Format("0x%x", m_nRejectDelay);
		WritePrivateProfileString(strApp, "49", strValue, strFile);
		strValue.Format("0x%x", m_nRejectDelay1);
		WritePrivateProfileString(strApp, "76", strValue, strFile);
		strValue.Format("0x%x", m_EDT_DabiaoDelay);
		WritePrivateProfileString(strApp, "161", strValue, strFile);

		strValue.Format("0x%x", m_EDT_nTiFeiPulse);
		WritePrivateProfileString(strApp, "92", strValue, strFile);
		strValue.Format("0x%x", m_EDT_nBuChangHangShu);
		WritePrivateProfileString(strApp, "162", strValue, strFile);


	}

	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.nBuChangHangShu,m_EDT_nBuChangHangShu,"变速补偿行数");
	m_pCommuicate->m_Commuicate.nBuChangHangShu=m_EDT_nBuChangHangShu;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dFirstLabelLength,m_EDT_dFirstLabelLength,"内侧贴标机物理距离(mm)");
	m_pCommuicate->m_Commuicate.dFirstLabelLength=m_EDT_dFirstLabelLength;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dSecondLabelLength,m_EDT_dSecondLabelLength,"外侧贴标机物理距离(mm)");
	m_pCommuicate->m_Commuicate.dSecondLabelLength=m_EDT_dSecondLabelLength;
	m_pCommuicate->ComPareChange((int)m_pCommuicate->m_Commuicate.nXLengthInPixel,m_nRejectDelay,"打标X延迟行数");
	m_pCommuicate->m_Commuicate.nXLengthInPixel=m_nRejectDelay;

	m_pCommuicate->ComPareChange((int)m_pCommuicate->m_Commuicate.n2ndLengthInPixel,m_nRejectDelay1,"打标2nd延迟行数");
	m_pCommuicate->m_Commuicate.n2ndLengthInPixel=m_nRejectDelay1;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dLabelPosition,m_EDT_dLabelActionLocation,"打标位置");
	m_pCommuicate->m_Commuicate.dLabelPosition=m_EDT_dLabelActionLocation;

	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.bMachineLengthLabel,m_CHK_bMachineLengthLabel,"设备测长打标");
	m_pCommuicate->m_Commuicate.bMachineLengthLabel=m_CHK_bMachineLengthLabel;


	m_pCommuicate->m_Commuicate.nTiFeiPulse=m_EDT_nTiFeiPulse;

	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dVirtualEALength,m_EDT_dVirtualEALength,"虚拟EA长度");
	m_pCommuicate->m_Commuicate.dVirtualEALength=m_EDT_dVirtualEALength;
	


	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.bUseLabelOffset, m_CHK_bUseLabelOffset, "贴标偏移监控开关");
	m_pCommuicate->m_Commuicate.bUseLabelOffset= m_CHK_bUseLabelOffset;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dLabelOffsetDev, m_dLabelOffsetDev, "贴标偏移公差");
	m_pCommuicate->m_Commuicate.dLabelOffsetDev= m_dLabelOffsetDev;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dUpLabelOffsetCompensate, m_EDT_dUpLabelOffsetCompensate, "上带路贴标偏移补偿");
	m_pCommuicate->m_Commuicate.dUpLabelOffsetCompensate= m_EDT_dUpLabelOffsetCompensate;
	m_pCommuicate->ComPareChange(m_pCommuicate->m_Commuicate.dDownLabelOffsetCompensate, m_EDT_dDownLabelOffsetCompensate, "下带路贴标偏移补偿");
   m_pCommuicate->m_Commuicate.dDownLabelOffsetCompensate= m_EDT_dDownLabelOffsetCompensate;


   ////





   CString strValue,strName;
   HTuple hvLastTuple;
   hvLastTuple = m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple;
   m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple.Clear();
   for (int i = 0; i<MAX_LALBELNUM; i++)
   {
	   strValue= m_LST_CLabelOffsetInfo.GetItemText(i, 1);
	   strName.Format("第%d个贴标机偏移补偿", i + 1);
	   m_pCommuicate->ComPareChange(hvLastTuple[i].D(), atof(strValue), strName);
	   m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple.Append(atof(strValue));
   }








	//Mark距离贴标机的行数

	CString strKeyName;
	for (int i=0;i<MAX_LALBELNUM;i++)///通道选择
	{
		strKeyName.Format("TripLabel%d",i);
		switch (i)
		{
		case 0:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel0;
			break;
		case 1:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel1;
		
			break;
		case 2:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel2;

			break;
		case 3:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel3;
			
			break;
		case 4:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel4;

			break;
		case 5:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel5;
	
			break;
		case 6:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel6;

			break;
		case 7:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel7;

			break;
		case 8:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel8;

			break;
		case 9:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel9;

			break;
		case 10:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel10;

			break;
		case 11:
			m_pCommuicate->m_Commuicate.hvChannelChoice[i]=m_COM_nChannel11;

			break;
		}
		WritePrivateProfileInt("LabelChannelSet",strKeyName,m_pCommuicate->m_Commuicate.hvChannelChoice[i].I(),m_pCommuicate->m_strConfigCommunicateFile);
	}




	WritePrivateProfileInt("IO","REJECTTYPE",m_pCommuicate->m_Commuicate.nRejectType,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","dXLengthInPixel",m_pCommuicate->m_Commuicate.nXLengthInPixel,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","d2ndLengthInPixel",m_pCommuicate->m_Commuicate.n2ndLengthInPixel,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","dLabelPosition",m_pCommuicate->m_Commuicate.dLabelPosition,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","nTiFeiPulse",m_pCommuicate->m_Commuicate.nTiFeiPulse,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","nBuChangHangShu",m_pCommuicate->m_Commuicate.nBuChangHangShu,m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileInt("IO","bMachineLengthLabel",m_pCommuicate->m_Commuicate.bMachineLengthLabel,m_pCommuicate->m_strConfigCommunicateFile);


	WritePrivateProfileInt("IO", "bUseLabelOffset", m_pCommuicate->m_Commuicate.bUseLabelOffset, m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dLabelOffsetDev", m_pCommuicate->m_Commuicate.dLabelOffsetDev, m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dUpLabelOffsetCompensate", m_pCommuicate->m_Commuicate.dUpLabelOffsetCompensate, m_pCommuicate->m_strConfigCommunicateFile);
	WritePrivateProfileDouble("IO", "dDownLabelOffsetCompensate", m_pCommuicate->m_Commuicate.dDownLabelOffsetCompensate, m_pCommuicate->m_strConfigCommunicateFile);

	CString strSingleName;
	for (int i = 0; i<m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple.Length(); i++)
	{
		strSingleName.Format("dSingleTripOffset%d", i);
		WritePrivateProfileDouble("LabelOffsetTuple", strSingleName, m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple[i].D(), m_pCommuicate->m_strConfigCommunicateFile);
	}
	//公共参数 不会跟着品种变化
	WritePrivateProfileDouble("IO","dFirstLabelLength",m_pCommuicate->m_Commuicate.dFirstLabelLength,m_pCommuicate->m_strConfigCommonFile);
	WritePrivateProfileDouble("IO","dSecondLabelLength",m_pCommuicate->m_Commuicate.dSecondLabelLength,m_pCommuicate->m_strConfigCommonFile);






}

//void CDlgIOSetting::OnBnClickedButtonTestReady()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (UpdateData(TRUE))
//	{
//		static BOOL bTest=TRUE;
//		if(bTest)
//		{
//			m_btnTestReady.SetIcon(IDI_ICON_W_PAUSE);
//		}else
//		{
//			m_btnTestReady.SetIcon(IDI_ICON_W_LIVE);
//		}
//
//		if (m_pCommuicate->m_IOCard[0].hasCard)//IO卡
//		{
//			m_pCommuicate->m_IOCard[0].setCardOutput(m_nReadyIO,bTest);
//		}
//		bTest = !bTest;
//		GetDlgItem(IDC_EDIT_READY)->EnableWindow(bTest);
//	}
//}


//void CDlgIOSetting::OnBnClickedButtonTestContinuouserroralarms()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (UpdateData(TRUE))
//	{
//		static BOOL bTest=TRUE;
//		if(bTest)
//		{
//			m_btnTestContinuousErrorAlarms.SetIcon(IDI_ICON_W_PAUSE);
//		}else
//		{
//			m_btnTestContinuousErrorAlarms.SetIcon(IDI_ICON_W_LIVE);
//		}
//
//		if (m_pCommuicate->m_IOCard[0].hasCard)//IO卡
//		{
//			m_pCommuicate->m_IOCard[0].setCardOutput(m_nContinuousErrorAlarmsIO,bTest);
//		}
//		bTest = !bTest;
//		GetDlgItem(IDC_EDIT_CONTINUOUSERRORALARMS)->EnableWindow(bTest);
//		m_pCommuicate->m_Alarms.nCurrentContinuousError=0;
//	}
//}


//void CDlgIOSetting::OnBnClickedButtonContinuouserrorset()
//{
//
//	GetDlgItem(IDC_EDIT_CONTINUOUSERRORALARMS)->EnableWindow(m_btnContinuousErrorSet.GetCheck());
//	GetDlgItem(IDC_EDIT_CONTINUOUSERRORCOUNTMAX)->EnableWindow(m_btnContinuousErrorSet.GetCheck());
//}


void CDlgIOSetting::OnBnClickedButtonCalliocardparamset()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox("卡内参数十分重要!\r\n错误的更改会引起软件异常运行!\r\n您已经和厂家联系过要更改此参数了吗?\r\n",MB_YESNO) == IDYES)
	{

		if (UpdateData(TRUE))
		{
			if (m_pCommuicate->m_IOCard[m_nIOCardSN].hasCard)
			{
				{
					CString strLog;
					strLog.Format("进入接口卡调试界面");
					m_pCommuicate->m_sInit.pLogView->DoSomething("OPERATERLOG",(LPSTR)(LPCSTR)strLog,NULL);
				}

				m_pCommuicate->m_IOCard[m_nIOCardSN].Show_PIO24B_DebugDialog(NULL);
			}else
			{
				AfxMessageBox("无法打开，没有该接口卡，请检测");
			}
		}
	}
}

void CDlgIOSetting::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LST_CLabelOffsetInfo.EndEdit(TRUE); 
	if (IDOK==MessageBox(_T("确认需要修改写入打标参数吗，修改完毕需要重启！"),_T("警告!"),MB_ICONINFORMATION|MB_OKCANCEL|MB_SYSTEMMODAL))
	{
			Save();
			m_pCommuicate->m_sInit.pSysCall("SETPARAM","StandardLabelLocation",&m_pCommuicate->m_Commuicate.dLabelPosition);
			m_pCommuicate->m_sInit.pSysCall("SETPARAM","LabelLocationOffset",&m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple);
			m_pCommuicate->m_sInit.pSysCall("SETPARAM","LabelOffsetDev",&m_pCommuicate->m_Commuicate.dLabelOffsetDev);
	}

	CDialogEx::OnOK();
}

HBRUSH CDlgIOSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	return hbr;
}


void CDlgIOSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgIOSetting::OnBnClickedButtonClose()
{
	CDialogEx::OnCancel();
}






//void CDlgIOSetting::OnCbnSelchangeComTiebiaomoshi()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	if (m_COM_nTieBiaoType==ZhengChangTiebiao)
//	{
//
//		GetDlgItem(IDC_EDT_OFFSET)->EnableWindow(0);
//
//	}else
//	{
//		GetDlgItem(IDC_EDT_OFFSET)->EnableWindow(1);
//	}
//}





//void CDlgIOSetting::OnEnChangeEdtLabelactionoffset()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//
//	UpdateData(TRUE);
//	if (m_EDT_dFirstLabelLength!=0)
//	{
//		m_nRejectDelay=(m_EDT_dFirstLabelLength-m_EDT_dLabelActionLocation)*m_pCommuicate->m_nGrabberDivideNum/(m_pCommuicate->m_Common.dRowResolution[0]*16)-m_pCommuicate->m_Commuicate.nBuChangHangShu;
//
//	}
//	if (m_EDT_dSecondLabelLength!=0)
//	{
//
//		m_nRejectDelay1=(m_EDT_dSecondLabelLength-m_EDT_dLabelActionLocation)*m_pCommuicate->m_nGrabberDivideNum/(m_pCommuicate->m_Common.dRowResolution[0]*16)-m_pCommuicate->m_Commuicate.nBuChangHangShu;
//
//
//	}
//
//	UpdateData(FALSE);
//
//
//	// TODO:  在此添加控件通知处理程序代码
//}


void CDlgIOSetting::OnBnClickedBtnCaldistinmm()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	{
		m_EDT_dFirstLabelLength=(m_nRejectDelay+m_pCommuicate->m_Commuicate.nBuChangHangShu)*(m_pCommuicate->m_Common.dRowResolution[0]* ConstFenpinNum)/m_pCommuicate->m_nGrabberDivideNum+m_EDT_dLabelActionLocation;
		m_EDT_dSecondLabelLength=(m_nRejectDelay1+m_pCommuicate->m_Commuicate.nBuChangHangShu)*(m_pCommuicate->m_Common.dRowResolution[0]* ConstFenpinNum)/m_pCommuicate->m_nGrabberDivideNum+m_EDT_dLabelActionLocation;
	}
	
	UpdateData(FALSE);



}


void CDlgIOSetting::OnBnClickedBtnRereshdelayline()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nLineDelay,nLineDelay1;
	nLineDelay=m_nRejectDelay;
	nLineDelay1=m_nRejectDelay1;

	if (m_EDT_dFirstLabelLength!=0)
	{

		{
			m_nRejectDelay=(m_EDT_dFirstLabelLength-m_EDT_dLabelActionLocation)*m_pCommuicate->m_nGrabberDivideNum/(m_pCommuicate->m_Common.dRowResolution[0]* ConstFenpinNum)-m_pCommuicate->m_Commuicate.nBuChangHangShu;


	/*		CString strInfo;
			strInfo.Format("m_EDT_dFirstLabelLength:%.1f m_EDT_dLabelActionLocation:%.1f m_nGrabberDivideNum:%d m_Common.dRowResolution[0]:%.5f ConstFenpinNum:%d  nBuChangHangShu:%d", m_EDT_dFirstLabelLength,
				m_EDT_dLabelActionLocation, m_pCommuicate->m_nGrabberDivideNum, m_pCommuicate->m_Common.dRowResolution[0], ConstFenpinNum, m_pCommuicate->m_Commuicate.nBuChangHangShu);
			m_pCommuicate->m_pLogView->DoSomething("EXCEPTION", (void*)(LPCSTR)strInfo, NULL);*/
		}
		
	}
	if (m_EDT_dSecondLabelLength!=0)
	{

		{
			m_nRejectDelay1=(m_EDT_dSecondLabelLength-m_EDT_dLabelActionLocation)*m_pCommuicate->m_nGrabberDivideNum/(m_pCommuicate->m_Common.dRowResolution[0]* ConstFenpinNum)-m_pCommuicate->m_Commuicate.nBuChangHangShu;

		}
		

	}
	CString strInfo;
	strInfo.Format("打标X延迟行由%d更新为%d \r\n 2nd延迟行由%d更新为%d ",nLineDelay,m_nRejectDelay,nLineDelay1,m_nRejectDelay1);
	AfxMessageBox(strInfo);
	UpdateData(FALSE);
}







void CDlgIOSetting::OnBnClickedChkEnablesetlabel()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if(m_CHK_bEnableLabelSet)
	{
		int nEnable=1;
		for (int i=0;i<MAX_LALBELNUM;i++)///通道选择
		{
			switch (i)
			{
			case 0:
				GetDlgItem(IDC_COM_CHANNEL0)->EnableWindow(nEnable);
				break;
			case 1:
				GetDlgItem(IDC_COM_CHANNEL1)->EnableWindow(nEnable);
				break;
			case 2:
				GetDlgItem(IDC_COM_CHANNEL2)->EnableWindow(nEnable);
				break;
			case 3:
				GetDlgItem(IDC_COM_CHANNEL3)->EnableWindow(nEnable);
				break;
			case 4:
				GetDlgItem(IDC_COM_CHANNEL4)->EnableWindow(nEnable);
				break;
			case 5:
				GetDlgItem(IDC_COM_CHANNEL5)->EnableWindow(nEnable);
				break;
			case 6:
				GetDlgItem(IDC_COM_CHANNEL6)->EnableWindow(nEnable);
				break;
			case 7:
				GetDlgItem(IDC_COM_CHANNEL7)->EnableWindow(nEnable);
				break;
			case 8:
				GetDlgItem(IDC_COM_CHANNEL8)->EnableWindow(nEnable);
				break;
			case 9:
				GetDlgItem(IDC_COM_CHANNEL9)->EnableWindow(nEnable);
				break;
			case 10:
				GetDlgItem(IDC_COM_CHANNEL10)->EnableWindow(nEnable);
				break;
			case 11:
				GetDlgItem(IDC_COM_CHANNEL11)->EnableWindow(nEnable);
				break;
			}

		}




	}else
	{

		int nEnable=0;
		for (int i=0;i<MAX_LALBELNUM;i++)///通道选择
		{
			switch (i)
			{
			case 0:
				GetDlgItem(IDC_COM_CHANNEL0)->EnableWindow(nEnable);
				break;
			case 1:
				GetDlgItem(IDC_COM_CHANNEL1)->EnableWindow(nEnable);
				break;
			case 2:
				GetDlgItem(IDC_COM_CHANNEL2)->EnableWindow(nEnable);
				break;
			case 3:
				GetDlgItem(IDC_COM_CHANNEL3)->EnableWindow(nEnable);
				break;
			case 4:
				GetDlgItem(IDC_COM_CHANNEL4)->EnableWindow(nEnable);
				break;
			case 5:
				GetDlgItem(IDC_COM_CHANNEL5)->EnableWindow(nEnable);
				break;
			case 6:
				GetDlgItem(IDC_COM_CHANNEL6)->EnableWindow(nEnable);
				break;
			case 7:
				GetDlgItem(IDC_COM_CHANNEL7)->EnableWindow(nEnable);
				break;
			case 8:
				GetDlgItem(IDC_COM_CHANNEL8)->EnableWindow(nEnable);
				break;
			case 9:
				GetDlgItem(IDC_COM_CHANNEL9)->EnableWindow(nEnable);
				break;
			case 10:
				GetDlgItem(IDC_COM_CHANNEL10)->EnableWindow(nEnable);
				break;
			case 11:
				GetDlgItem(IDC_COM_CHANNEL11)->EnableWindow(nEnable);
				break;
			}

		}



	}




}


void CDlgIOSetting::OnBnClickedBtnTestlabelouput()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(TRUE);
	////const int TestIndexParam[12]=
	////{
	////	169,170,171,172,173,174,175,176,177,178,179,180

	////};
	//if (m_pCommuicate->m_IOCard[0].hasCard)
	//{
	//	m_pCommuicate->m_IOCard[0].writeParam(Const_TestIndexParam[m_COM_nTestLabelIndex],1);
	//	Sleep(30);
	//	m_pCommuicate->m_IOCard[0].writeParam(Const_TestIndexParam[m_COM_nTestLabelIndex],0);
	//}



}


void CDlgIOSetting::OnBnClickedBtnLabelreset()
{
	// TODO: 在此添加控件通知处理程序代码


}


void CDlgIOSetting::OnBnClickedBtnOnekeycalib()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strValue;
	for (int i = 0; i<MAX_LALBELNUM; i++)
	{
		m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple[i]= m_pCommuicate->m_Commuicate.dLabelPosition-m_pCommuicate->m_hvRealMonitorLengthTuple[i];
		strValue.Format("%.1f", m_pCommuicate->m_Commuicate.hvLabelOffsetCompensateTuple[i].D());
		m_LST_CLabelOffsetInfo.SetItemText(i, 1, strValue);
	}
}


//void CDlgIOSetting::OnBnClickedBtnReadsensor()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//
//	int nNum = 0;
//	{
//
//		m_pCommuicate->ReadDabiaoLocationFromLabel(m_COM_nTestLabelIndex + 1, nNum);
//	}
//
//	{
//
//
//		CString str;
//		str.Format("贴标机%d贴标位置%.1fmm", m_COM_nTestLabelIndex + 1, nNum*0.1);
//		AfxMessageBox(str);
//
//
//	
//
//
//	}
//
//
//
//
//
//}
