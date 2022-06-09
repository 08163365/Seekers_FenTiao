// PageSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Battery.h"
#include "PageSet.h"
#include "afxdialogex.h"
#include "DlgAlgorithm.h"

// CPageSet �Ի���

IMPLEMENT_DYNAMIC(CPageSet, CDialogEx)

CPageSet::CPageSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPageSet::IDD, pParent)
	, m_EDT_dScaleFactor(0)
	, m_EDT_nTapeDynThresh(0)
	, m_EDT_dLTMinArea(0)
	, m_COM_nErrorNameIndex(0)
	, m_EDT_nJDDarkThresh(0)
	, m_EDT_dJDDarkArea(0)
	, m_EDT_nJDLightThresh(0)
	, m_CHK_bUseJDDetect(FALSE)
	, m_EDT_dMinTapeDeviation(0)
	, m_bEnbleUseClassyfy(FALSE)
	, m_EDT_dRightBorderWidth(0)
	, m_EDT_dYaHenPingBiKuanDu(0)
	, m_EDT_nMaxTapeThresh(0)
	, m_EDT_dMinTapeArea(0)
{

	m_EDT_dBorderDarkCoff = 0.0;
	m_EDT_dBorderLightCoff = 0.0;
	m_EDT_dBorderErosionLength = 0.0;
	m_EDT_dBubbleMinCircularity = 0.0;
	m_EDT_dBubbleMinArea = 0.0;
	m_EDT_nBubbleDynThresh = 0;
	m_EDT_nDarkDotDynThresh = 0;
	m_EDT_dDarkDotMinArea = 0.0;
	m_EDT_nDarkLineDynThresh = 0;
	m_EDT_dDarkLineMinRatio = 0.0;
	m_EDT_dDarkLineMinWidth = 0.0;
	m_EDT_nLeakFoilDynThresh = 0;
	m_EDT_dLeakFoilMinArea = 0.0;
	m_EDT_dLeakFoilMinDeviation = 0.0;
	m_EDT_nLeakFoilThresh = 0;
	m_EDT_dLightDotMinArea = 0.0;
	m_EDT_nLightDotDynThresh = 0;
	m_EDT_dMidDarkCoff = 0.0;
	m_EDT_dMidLightCoff = 0.0;
	m_EDT_dFoilDarkMinArea = 0.0;
	m_EDT_nFoilDarkMinNum = 0;
	m_EDT_nFoilDarkThresh = 0;
	m_Font.CreatePointFont(80, "Microsoft YaHei");
	m_EDT_dMLPSongjindu = 0.0;
	m_EDT_dAT9DarkHeight = 0.0;
	m_EDT_nAT9DarkThresh = 0;
	m_EDT_dAT9DarkWidth = 0.0;
	//  m_EDT_nAT9LightThresh = 0;
	m_EDT_dAT9LightHeight = 0.0;
	m_EDT_dAT9LightThresh = 0;
	m_EDT_dAT9LightWidth = 0.0;
	//  m_CHK_nTuoWeiHighThresh = 0;
	//  m_CHK_nTuoWeiLowThresh = 0;
	m_EDT_dTuoWeiMinArea = 0.0;
	m_EDT_nTuoWeiHighThresh = 0;
	m_EDT_nTuoWeiLowThresh = 0;
}

CPageSet::~CPageSet()
{


}

void CPageSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHK_USEBUBBLEDETECT, m_CHK_bUseBubbleDetect);
	DDX_Check(pDX, IDC_CHK_USEDARKDOTDETECT, m_CHK_bUseDarkDotDetect);
	DDX_Check(pDX, IDC_CHK_USEDARKLINEDETECT, m_CHK_bUseDarkLineDetect);
	DDX_Check(pDX, IDC_CHK_USELEAKFOILDETECT, m_CHK_bUseLeakFoilDetect);
	DDX_Check(pDX, IDC_CHK_USELIGHTDOTDETECT, m_CHK_bUseLightDotDetect);
	DDX_Check(pDX, IDC_CHK_USETAPEDETECT, m_CHK_bUseTapeDetect);
	DDX_Text(pDX, IDC_EDT_BORDERDARKCOFF, m_EDT_dBorderDarkCoff);
	DDV_MinMaxDouble(pDX, m_EDT_dBorderDarkCoff, 5, 100);
	DDX_Text(pDX, IDC_EDT_BORDERLIGHTCOFF, m_EDT_dBorderLightCoff);
	DDV_MinMaxDouble(pDX, m_EDT_dBorderLightCoff, 5, 100);
	DDX_Text(pDX, IDC_EDT_BORDEREROSIONLENGTH, m_EDT_dBorderErosionLength);
	DDX_Text(pDX, IDC_EDT_BUBBLEMINCIRCULARITY, m_EDT_dBubbleMinCircularity);
	DDV_MinMaxDouble(pDX, m_EDT_dBubbleMinCircularity, 0, 1);
	DDX_Text(pDX, IDC_EDT_BUBBLEMINDIAM, m_EDT_dBubbleMinArea);
	DDX_Text(pDX, IDC_EDT_BUBBLEDYNTHRESH, m_EDT_nBubbleDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nBubbleDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_DARKDOTMAXTHRESH, m_EDT_nDarkDotDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nDarkDotDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_DARKDOTMINAREA, m_EDT_dDarkDotMinArea);
	DDX_Text(pDX, IDC_EDT_DARKLINEDYNTHRESH, m_EDT_nDarkLineDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nDarkLineDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_DARKLINEMINWHRATIO, m_EDT_dDarkLineMinRatio);
	DDX_Text(pDX, IDC_EDT_DARKLINEMINWIDTH, m_EDT_dDarkLineMinWidth);
	DDX_Text(pDX, IDC_EDT_LEAKFOILDYNTHRESH, m_EDT_nLeakFoilDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nLeakFoilDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_LEAKFOILMINAREA, m_EDT_dLeakFoilMinArea);
	DDX_Text(pDX, IDC_EDT_LEAKFOILMINDEVIATION, m_EDT_dLeakFoilMinDeviation);
	DDX_Text(pDX, IDC_EDT_LEAKFOILTHRESH, m_EDT_nLeakFoilThresh);
	DDV_MinMaxInt(pDX, m_EDT_nLeakFoilThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_LIGHTDOTMINAREA, m_EDT_dLightDotMinArea);
	DDX_Text(pDX, IDC_EDT_LIGHTDOTMINTHRESH, m_EDT_nLightDotDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nLightDotDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_MIDDARKCOFF, m_EDT_dMidDarkCoff);
	DDV_MinMaxDouble(pDX, m_EDT_dMidDarkCoff, 5, 50);
	DDX_Text(pDX, IDC_EDT_MIDLIGHTCOFF, m_EDT_dMidLightCoff);
	DDV_MinMaxDouble(pDX, m_EDT_dMidLightCoff, 5, 50);
	DDX_Text(pDX, IDC_EDT_FOILDARKMINAREA, m_EDT_dFoilDarkMinArea);
	DDX_Text(pDX, IDC_EDT_FOILDARKMINNUM, m_EDT_nFoilDarkMinNum);
	DDX_Text(pDX, IDC_EDT_FOILDARKTHRESH, m_EDT_nFoilDarkThresh);
	DDV_MinMaxInt(pDX, m_EDT_nFoilDarkThresh, 0, 255);
	DDX_Check(pDX, IDC_CHK_USEFOILDETECT, m_CHK_bUseFoilDetect);
	DDX_Control(pDX, IDC_MFCMENUBTN_FEATURE, m_MenuBtn_FeatureSelect);
	DDX_Control(pDX, IDC_LST_TEMPFEATURE, m_LST_Temp);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID_ALGRITHM, m_PropertyGrid_Algrithm);
	DDX_Text(pDX, IDC_EDT_SCALEFACTOR, m_EDT_dScaleFactor);
	DDX_Text(pDX, IDC_EDT_LTHRESH, m_EDT_nTapeDynThresh);
	DDV_MinMaxInt(pDX, m_EDT_nTapeDynThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_MINLIGHTTAPEAREA, m_EDT_dLTMinArea);
	DDX_Control(pDX, IDC_COM_ERRORNAME, m_COM_CErrorName);
	DDX_CBIndex(pDX, IDC_COM_ERRORNAME, m_COM_nErrorNameIndex);
	DDX_Text(pDX, IDC_EDT_JD_DARKTHRESH, m_EDT_nJDDarkThresh);
	DDV_MinMaxInt(pDX, m_EDT_nJDDarkThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_JD_MINDARKAREA, m_EDT_dJDDarkArea);
	DDX_Text(pDX, IDC_EDT_JD_LIGHTTHRESH, m_EDT_nJDLightThresh);
	DDV_MinMaxInt(pDX, m_EDT_nJDLightThresh, 0, 255);
	DDX_Check(pDX, IDC_CHECK1, m_CHK_bUseJDDetect);
	DDX_Text(pDX, IDC_EDT_MLPSONGJINDU, m_EDT_dMLPSongjindu);
	DDV_MinMaxDouble(pDX, m_EDT_dMLPSongjindu, 0.2, 1);
	DDX_Text(pDX, IDC_EDT_TAPEFACTOR, m_EDT_dMinTapeDeviation);
	DDX_Check(pDX, IDC_CHK_USECLASSIFY, m_bEnbleUseClassyfy);
	DDX_Text(pDX, IDC_EDT_AT9DARKHEIGHT, m_EDT_dAT9DarkHeight);
	DDX_Text(pDX, IDC_EDT_AT9DARKTHRESH, m_EDT_nAT9DarkThresh);
	DDV_MinMaxInt(pDX, m_EDT_nAT9DarkThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_AT9DARKWIDTH, m_EDT_dAT9DarkWidth);
	//  DDX_Text(pDX, IDC_EDT_AT9LIGHTHEIGHT, m_EDT_nAT9LightThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nAT9LightThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_AT9LIGHTHEIGHT, m_EDT_dAT9LightHeight);
	DDX_Text(pDX, IDC_EDT_AT9LIGHTTHRESH, m_EDT_dAT9LightThresh);
	DDV_MinMaxInt(pDX, m_EDT_dAT9LightThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_AT9LIGHTWIDTH, m_EDT_dAT9LightWidth);
	DDX_Check(pDX, IDC_CHK_AT9ENABLE, m_CHK_bUseAT9Detect);
	DDX_Check(pDX, IDC_CHK_BUSEDETECTTUOWEI, m_CHK_bUseDetectTuoWei);
	//  DDX_Text(pDX, IDC_EDT_TUOWEIHIGHTHRESH, m_CHK_nTuoWeiHighThresh);
	//  DDV_MinMaxInt(pDX, m_CHK_nTuoWeiHighThresh, 0, 255);
	//  DDX_Text(pDX, IDC_EDT_TUOWEILOWTHRESH, m_CHK_nTuoWeiLowThresh);
	//  DDV_MinMaxInt(pDX, m_CHK_nTuoWeiLowThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_TUOWEIMINAREA, m_EDT_dTuoWeiMinArea);
	DDX_Text(pDX, IDC_EDT_TUOWEIHIGHTHRESH, m_EDT_nTuoWeiHighThresh);
	DDV_MinMaxInt(pDX, m_EDT_nTuoWeiHighThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_TUOWEILOWTHRESH, m_EDT_nTuoWeiLowThresh);
	DDV_MinMaxInt(pDX, m_EDT_nTuoWeiLowThresh, 0, 255);
	DDX_Text(pDX, IDC_EDT_RIGHTBORDERWIDTH, m_EDT_dRightBorderWidth);
	DDX_Text(pDX, IDC_EDT_YAHENPINGBIKUANDU, m_EDT_dYaHenPingBiKuanDu);
	DDX_Text(pDX, IDC_EDT_TAPETHRESH, m_EDT_nMaxTapeThresh);
	DDX_Text(pDX, IDC_EDT_MINTAPETHRESHAREA, m_EDT_dMinTapeArea);
}


BEGIN_MESSAGE_MAP(CPageSet, CDialogEx)
	ON_WM_VSCROLL()

	//	ON_BN_CLICKED(IDC_BUTTON_CREATEMARK, &CPageSet::OnBnClickedButtonCreatemark)
	//	ON_BN_CLICKED(IDC_CHK_USETAPEDETECT, &CPageSet::OnBnClickedChkUsetapedetect)
	//	ON_EN_CHANGE(IDC_EDT_TAPEMAXTHRESH, &CPageSet::OnEnChangeEdtTapemaxthresh)
	//	ON_EN_CHANGE(IDC_EDT_TAPEMINAREA, &CPageSet::OnEnChangeEdtTapeminarea)
	//ON_BN_CLICKED(IDC_BTN_TEST, &CPageSet::OnBnClickedBtnTest)
	//ON_EN_CHANGE(IDC_EDT_TAPEMINAREA, &CPageSet::OnEnChangeEdtTapeminarea)
	//ON_EN_CHANGE(IDC_EDT_TAPEMAXTHRESH, &CPageSet::OnEnChangeEdtTapemaxthresh)
	ON_BN_CLICKED(IDC_CHK_USETAPEDETECT, &CPageSet::OnBnClickedChkUsetapedetect)
	ON_BN_CLICKED(IDC_CHK_USEDARKDOTDETECT, &CPageSet::OnBnClickedChkUsedarkdotdetect)
	ON_EN_CHANGE(IDC_EDT_DARKDOTMAXTHRESH, &CPageSet::OnEnChangeEdtDarkdotmaxthresh)
	//ON_EN_CHANGE(IDC_EDT_DARKDOTMINWIDTH, &CPageSet::OnEnChangeEdtDarkdotminwidth)
	//ON_EN_CHANGE(IDC_EDT_DARKDOTMINHEIGHT, &CPageSet::OnEnChangeEdtDarkdotminheight)
	ON_EN_CHANGE(IDC_EDT_DARKDOTMINAREA, &CPageSet::OnEnChangeEdtDarkdotminarea)
	//ON_EN_CHANGE(IDC_EDT_DARKDOTMAXAREA, &CPageSet::OnEnChangeEdtDarkdotmaxarea)
	ON_BN_CLICKED(IDC_CHK_USELIGHTDOTDETECT, &CPageSet::OnBnClickedChkUselightdotdetect)
	ON_EN_CHANGE(IDC_EDT_LIGHTDOTMINTHRESH, &CPageSet::OnEnChangeEdtLightdotminthresh)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTMINWIDTH, &CPageSet::OnEnChangeEdtLightdotminwidth)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTMINHEIGHT, &CPageSet::OnEnChangeEdtLightdotminheight)
	ON_EN_CHANGE(IDC_EDT_LIGHTDOTMINAREA, &CPageSet::OnEnChangeEdtLightdotminarea)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTMAXAREA, &CPageSet::OnEnChangeEdtLightdotmaxarea)
	ON_BN_CLICKED(IDC_CHK_USEDARKLINEDETECT, &CPageSet::OnBnClickedChkUsedarklinedetect)
	ON_EN_CHANGE(IDC_EDT_DARKLINEDYNTHRESH, &CPageSet::OnEnChangeEdtDarklinedynthresh)
	ON_EN_CHANGE(IDC_EDT_DARKLINEMINWIDTH, &CPageSet::OnEnChangeEdtDarklineminwidth)
	//ON_EN_CHANGE(IDC_EDT_DARKLINEMINHEIGHT, &CPageSet::OnEnChangeEdtDarklineminheight)
	ON_EN_CHANGE(IDC_EDT_DARKLINEMINWHRATIO, &CPageSet::OnEnChangeEdtDarklineminwhratio)
	//ON_BN_CLICKED(IDC_CHK_USELIGHTLINEDETECT, &CPageSet::OnBnClickedChkUselightlinedetect)
	//ON_EN_CHANGE(IDC_EDT_LIGHTLINEDYNTHRESH, &CPageSet::OnEnChangeEdtLightlinedynthresh)
	//ON_EN_CHANGE(IDC_EDT_LIGHTLINEMINWIDTH, &CPageSet::OnEnChangeEdtLightlineminwidth)
	//ON_EN_CHANGE(IDC_EDT_LIGHTLINEMINHEIGHT, &CPageSet::OnEnChangeEdtLightlineminheight)
	//ON_EN_CHANGE(IDC_EDT_LIGHTLINEMINWHRATIO, &CPageSet::OnEnChangeEdtLightlineminwhratio)
	ON_BN_CLICKED(IDC_CHK_USEBUBBLEDETECT, &CPageSet::OnBnClickedChkUsebubbledetect)
	ON_EN_CHANGE(IDC_EDT_BUBBLEDYNTHRESH, &CPageSet::OnEnChangeEdtBubbledynthresh)
	ON_EN_CHANGE(IDC_EDT_BUBBLEMINDIAM, &CPageSet::OnEnChangeEdtBubblemindiam)
	ON_EN_CHANGE(IDC_EDT_BUBBLEMINCIRCULARITY, &CPageSet::OnEnChangeEdtBubblemincircularity)
	//ON_EN_CHANGE(IDC_EDT_BUBBLEINOUTDIFF, &CPageSet::OnEnChangeEdtBubbleinoutdiff)
	ON_BN_CLICKED(IDC_CHK_USELEAKFOILDETECT, &CPageSet::OnBnClickedChkUseleakfoildetect)
	ON_EN_CHANGE(IDC_EDT_LEAKFOILTHRESH, &CPageSet::OnEnChangeEdtLeakfoilthresh)
	ON_EN_CHANGE(IDC_EDT_LEAKFOILMINAREA, &CPageSet::OnEnChangeEdtLeakfoilminarea)
	ON_EN_CHANGE(IDC_EDT_LEAKFOILDYNTHRESH, &CPageSet::OnEnChangeEdtLeakfoildynthresh)
	//ON_EN_CHANGE(IDC_EDT_LEAKFOILMINDIAM, &CPageSet::OnEnChangeEdtLeakfoilmindiam)
	ON_EN_CHANGE(IDC_EDT_LEAKFOILMINDEVIATION, &CPageSet::OnEnChangeEdtLeakfoilmindeviation)
	//ON_EN_CHANGE(IDC_EDT_LEAKFOILMAXDIAM, &CPageSet::OnEnChangeEdtLeakfoilmaxdiam)
	ON_EN_CHANGE(IDC_EDT_BORDEREROSIONLENGTH, &CPageSet::OnEnChangeEdtBordererosionlength)
	ON_EN_CHANGE(IDC_EDT_MIDLIGHTCOFF, &CPageSet::OnEnChangeEdtMidlightcoff)
	ON_EN_CHANGE(IDC_EDT_MIDDARKCOFF, &CPageSet::OnEnChangeEdtMiddarkcoff)
	ON_EN_CHANGE(IDC_EDT_BORDERLIGHTCOFF, &CPageSet::OnEnChangeEdtBorderlightcoff)
	ON_EN_CHANGE(IDC_EDT_BORDERDARKCOFF, &CPageSet::OnEnChangeEdtBorderdarkcoff)
	ON_BN_CLICKED(IDC_CHK_USEFOILDETECT, &CPageSet::OnBnClickedChkUsefoildetect)
	ON_EN_CHANGE(IDC_EDT_FOILDARKTHRESH, &CPageSet::OnEnChangeEdtFoildarkthresh)
	//ON_EN_CHANGE(IDC_EDT_FOILDARKMINWIDTH, &CPageSet::OnEnChangeEdtFoildarkminwidth)
	//ON_EN_CHANGE(IDC_EDT_FOILDARKMINHEIGHT, &CPageSet::OnEnChangeEdtFoildarkminheight)
	ON_EN_CHANGE(IDC_EDT_FOILDARKMINAREA, &CPageSet::OnEnChangeEdtFoildarkminarea)
	ON_EN_CHANGE(IDC_EDT_FOILDARKMINNUM, &CPageSet::OnEnChangeEdtFoildarkminnum)
	//ON_EN_CHANGE(IDC_EDT_FOILLIGHTTHRESH, &CPageSet::OnEnChangeEdtFoillightthresh)
	//ON_EN_CHANGE(IDC_EDT_FOILLIGHTMINAREA, &CPageSet::OnEnChangeEdtFoillightminarea)
	ON_BN_CLICKED(IDC_MFCMENUBTN_FEATURE, &CPageSet::OnBnClickedMfcmenubtnFeature)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CPageSet::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_ADDALGRITHM, &CPageSet::OnBnClickedBtnAddalgrithm)
	ON_BN_CLICKED(IDC_BTN_DELETEALGRITHM, &CPageSet::OnBnClickedBtnDeletealgrithm)
	ON_BN_CLICKED(IDC_BTN_LOADPARAM, &CPageSet::OnBnClickedBtnLoadparam)
	//ON_WM_PAINT()
	//ON_WM_CTLCOLOR()
	//ON_BN_CLICKED(IDC_BTN_AUTOFINDCOFF, &CPageSet::OnBnClickedBtnAutofindcoff)
	ON_EN_CHANGE(IDC_EDT_SCALEFACTOR, &CPageSet::OnEnChangeEdtScalefactor)
	ON_EN_CHANGE(IDC_EDT_LTHRESH, &CPageSet::OnEnChangeEdtLthresh)
	//ON_EN_CHANGE(IDC_EDT_HTHRESH, &CPageSet::OnEnChangeEdtHthresh)
	ON_EN_CHANGE(IDC_EDT_MINLIGHTTAPEAREA, &CPageSet::OnEnChangeEdtMinlighttapearea)
	//ON_EN_CHANGE(IDC_EDT_AT9LOWTHRESH, &CPageSet::OnEnChangeEdtAt9lowthresh)
	//ON_EN_CHANGE(IDC_EDT_AT9MINWIDTH, &CPageSet::OnEnChangeEdtAt9minwidth)
	//ON_EN_CHANGE(IDC_EDT_AT9MINHEIGHT, &CPageSet::OnEnChangeEdtAt9minheight)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTMAXTHRESH, &CPageSet::OnEnChangeEdtLightdotmaxthresh)
	//ON_EN_CHANGE(IDC_EDT_AT9MAXTHRESH, &CPageSet::OnEnChangeEdtAt9maxthresh)
	//ON_EN_CHANGE(IDC_EDT_AT9DARKMINWIDTH, &CPageSet::OnEnChangeEdtAt9darkminwidth)
	//ON_EN_CHANGE(IDC_EDT_AT9MINAREA, &CPageSet::OnEnChangeEdtAt9minarea)
	//ON_EN_CHANGE(IDC_EDT_FOILLIGHTWIDTH, &CPageSet::OnEnChangeEdtFoillightwidth)
	ON_EN_CHANGE(IDC_EDT_JD_DARKTHRESH, &CPageSet::OnEnChangeEdtJdDarkthresh)
	ON_EN_CHANGE(IDC_EDT_JD_MINDARKAREA, &CPageSet::OnEnChangeEdtJdMindarkarea)
	ON_EN_CHANGE(IDC_EDT_JD_LIGHTTHRESH, &CPageSet::OnEnChangeEdtJdLightthresh)
	//ON_EN_CHANGE(IDC_EDT_JD_MINLIGHTAREA, &CPageSet::OnEnChangeEdtJdMinlightarea)
	ON_BN_CLICKED(IDC_CHECK1, &CPageSet::OnBnClickedCheck1)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTDEVIATION, &CPageSet::OnEnChangeEdtLightdotdeviation)
	//ON_EN_CHANGE(IDC_EDT_LIGHTDOTGRAYRANGE, &CPageSet::OnEnChangeEdtLightdotgrayrange)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDT_MLPSONGJINDU, &CPageSet::OnChangeEdtMlpsongjindu)
	ON_EN_CHANGE(IDC_EDT_TAPEFACTOR, &CPageSet::OnEnChangeEdtTapefactor)
	ON_BN_CLICKED(IDC_CHK_USECLASSIFY, &CPageSet::OnClickedChkUseclassify)
	ON_BN_CLICKED(IDC_CHK_AT9ENABLE, &CPageSet::OnBnClickedChkAt9enable)
	ON_EN_CHANGE(IDC_EDT_AT9LIGHTTHRESH, &CPageSet::OnChangeEdtAt9lightthresh)
	ON_EN_CHANGE(IDC_EDT_AT9LIGHTHEIGHT, &CPageSet::OnChangeEdtAt9lightheight)
	ON_EN_CHANGE(IDC_EDT_AT9DARKWIDTH, &CPageSet::OnChangeEdtAt9darkwidth)
	ON_EN_CHANGE(IDC_EDT_AT9DARKTHRESH, &CPageSet::OnChangeEdtAt9darkthresh)
	ON_EN_CHANGE(IDC_EDT_AT9DARKHEIGHT, &CPageSet::OnChangeEdtAt9darkheight)
	ON_EN_CHANGE(IDC_EDT_AT9LIGHTWIDTH, &CPageSet::OnChangeEdtAt9lightwidth)
	ON_BN_CLICKED(IDC_CHK_BUSEDETECTTUOWEI, &CPageSet::OnClickedChkBusedetecttuowei)
	ON_EN_CHANGE(IDC_EDT_TUOWEILOWTHRESH, &CPageSet::OnChangeEdtTuoweilowthresh)
	ON_EN_CHANGE(IDC_EDT_TUOWEIHIGHTHRESH, &CPageSet::OnChangeEdtTuoweihighthresh)
	ON_EN_CHANGE(IDC_EDT_TUOWEIMINAREA, &CPageSet::OnChangeEdtTuoweiminarea)
	ON_EN_CHANGE(IDC_EDT_RIGHTBORDERWIDTH, &CPageSet::OnEnChangeEdtRightborderwidth)
	ON_EN_CHANGE(IDC_EDT_YAHENPINGBIKUANDU, &CPageSet::OnEnChangeEdtYahenpingbikuandu)
	ON_EN_CHANGE(IDC_EDT_TAPETHRESH, &CPageSet::OnEnChangeEdtTapethresh)
	ON_EN_CHANGE(IDC_EDT_MINTAPETHRESHAREA, &CPageSet::OnEnChangeEdtMintapethresharea)
END_MESSAGE_MAP()


// CPageSet ��Ϣ�������



void CPageSet::SetButtonStaus(BOOL bStaus)
{
	//((CDlgAlgorithm*)GetParent())->m_Wnd_Image.SetDrawing(!bStaus);
	//HWND  hwndChild = ::GetWindow(GetSafeHwnd(), GW_CHILD);  //�г����пؼ� 
	//CWnd* pWnd = CWnd::FromHandle(GetSafeHwnd());
	//int woc;
	//CRect Rect;
	//while (hwndChild)
	//{
	//	woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
	//	pWnd->GetDlgItem(woc)->EnableWindow(bStaus);
	//	hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	//}
}


BOOL CPageSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int k = 0; k < ErrorCount; k++)
	{
		m_COM_CErrorName.AddString(g_StrArrayErrorName[k]);
	}
	m_COM_nErrorNameIndex = 0;









	CMenu *pMenu = new CMenu;
	pMenu->LoadMenu(IDR_MENU1);
	pMenu->InsertMenuA(0, MF_BYPOSITION | MF_POPUP | MF_STRING, (int)pMenu->m_hMenu, "�Ӳ˵�");
	for (int i = 0; i < FEATURE_LENGTH; i++)
	{
		pMenu->GetSubMenu(0)->AppendMenuA(MF_STRING, i + 1, NAME_Feature[i]);//����Ҫ��i+1,��Ϊ0������Ϊ��Ӧ

	}
	m_MenuBtn_FeatureSelect.m_hMenu = pMenu->GetSubMenu(0)->GetSafeHmenu();


	CRect rcList;
	m_LST_Temp.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_LST_Temp.GetClientRect(rcList);
	m_LST_Temp.SetFont(&m_Font);
	m_LST_Temp.InsertColumn(0, "����", LVCFMT_CENTER, int(rcList.Width() * 0.3));
	m_LST_Temp.InsertColumn(1, "��Сֵ", LVCFMT_CENTER, int(rcList.Width() * 0.3));
	m_LST_Temp.InsertColumn(2, "���ֵ", LVCFMT_CENTER, int(rcList.Width() * 0.3));
	m_LST_Temp.SetEditable();
	m_LST_Temp.SetGridLines();
	CRect r;
	m_PropertyGrid_Algrithm.GetHeaderCtrl().GetClientRect(&r);
	HDITEM hDitem;
	hDitem.cxy = r.Width() / 3;
	hDitem.mask = HDI_WIDTH;

	m_PropertyGrid_Algrithm.GetHeaderCtrl().SetItem(0, &hDitem);
	hDitem.pszText = _T("����");
	hDitem.mask = HDI_TEXT;
	m_PropertyGrid_Algrithm.GetHeaderCtrl().SetItem(0, &hDitem);
	hDitem.pszText = _T("�趨ֵ");
	hDitem.mask = HDI_TEXT;
	m_PropertyGrid_Algrithm.GetHeaderCtrl().SetItem(1, &hDitem);

	HDITEM item;
	item.cxy = 190;
	item.mask = HDI_WIDTH;
	m_PropertyGrid_Algrithm.GetHeaderCtrl().SetItem(0, new HDITEM(item));
	m_PropertyGrid_Algrithm.SetDescriptionRows(4);

	m_PropertyGrid_Algrithm.SetFont(&m_Font);


	for (int i = 0; i < m_ptParameters->tFeatureSelect.nGroupNum; i++)
	{
		SingleErrorGroup tSingleErrorGroup;
		tSingleErrorGroup = m_ptParameters->tFeatureSelect.Vec_FSelects.at(i);


		CMFCPropertyGridProperty *pGoup0 = new CMFCPropertyGridProperty(tSingleErrorGroup.strErrorName);
		CString strName, strMinValue, strMaxValue;
		for (int k = 0; k < tSingleErrorGroup.nSingleGroupNum; k = k + 3)
		{
			SingleFeature tSingleFeature;
			tSingleFeature = tSingleErrorGroup.Vec_tSingleGroupFeatures.at(k / 3);
			strName = tSingleFeature.StrSingleFeatureName;
			pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("����"), strName, _T("0-1")));
			strMinValue.Format("%.2f", tSingleFeature.dMin);
			pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("MIN"), strMinValue, _T("0-1")));
			strMaxValue.Format("%.2f", tSingleFeature.dMax);
			pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("MAX"), strMaxValue, _T("0-1")));
		}
		m_PropertyGrid_Algrithm.AddProperty(pGoup0);
	}
	CRect rcMine, rcParent;
	GetClientRect(&rcMine);

	SCROLLINFO vinfo;
	vinfo.cbSize = sizeof(vinfo);
	vinfo.fMask = SIF_RANGE | SIF_PAGE;
	//����nPageȡֵ-1ʱ���������᲻���ˡ�
	vinfo.nMax = rcMine.Height() - rcParent.Height();//���������ܹ��������ֵ
	vinfo.nPage = rcMine.Height() - rcParent.Height() - 150;//����������ĳ��̣�ͨ�������¹�ϵ���䳤��/���������ȣ���������ͷ��=nPage/(nMax+2)��

	vinfo.nMin = 0;//���������ܹ�������Сֵ
	vinfo.nTrackPos = 0;
	SetScrollInfo(SB_VERT, &vinfo);//��ʹ��������һ������ʹ�ô������Ҳ������ʾ������

	///Ԥ�������
	m_EDT_dBorderErosionLength = m_ptParameters->tPretreatParam.dBorderWidth;
	m_EDT_dRightBorderWidth = m_ptParameters->tPretreatParam.dRightBorderWidth;
	m_EDT_dScaleFactor = m_ptParameters->tPretreatParam.dScaleFactor;
	m_EDT_dYaHenPingBiKuanDu = m_ptParameters->tPretreatParam.dYaHenPingBiKuanDu;
	m_EDT_dMidLightCoff = m_ptParameters->tPretreatParam.dMidLightCof;
	m_EDT_dMidDarkCoff = m_ptParameters->tPretreatParam.dMidDarkCof;
	m_EDT_dBorderLightCoff = m_ptParameters->tPretreatParam.dBorderLightCof;
	m_EDT_dBorderDarkCoff = m_ptParameters->tPretreatParam.dBorderDarkCof;
	m_EDT_dMLPSongjindu = m_ptParameters->tPretreatParam.dMLPSongjindu;
	m_bEnbleUseClassyfy = m_ptParameters->tPretreatParam.bEnbleUseClassyfy;
	///©������

	m_CHK_bUseLeakFoilDetect = m_ptParameters->tLeakFoilParam.bUseDetect;
	m_EDT_nLeakFoilThresh = m_ptParameters->tLeakFoilParam.nThresh;
	m_EDT_dLeakFoilMinArea = m_ptParameters->tLeakFoilParam.dMinArea;


	////����
	m_CHK_bUseBubbleDetect = m_ptParameters->tBubbleParam.bUseDetect;
	m_EDT_nBubbleDynThresh = m_ptParameters->tBubbleParam.nDynThresh;
	m_EDT_dBubbleMinArea = m_ptParameters->tBubbleParam.dMinArea;
	m_EDT_dBubbleMinCircularity = m_ptParameters->tBubbleParam.dMinCircularity;


	///��

	m_CHK_bUseDarkLineDetect = m_ptParameters->tDarkLineParam.bUseDetect;
	m_EDT_nDarkLineDynThresh = m_ptParameters->tDarkLineParam.nDynThresh;
	m_EDT_dDarkLineMinWidth = m_ptParameters->tDarkLineParam.dMinWidth;
	m_EDT_dDarkLineMinRatio = m_ptParameters->tDarkLineParam.dMinLengthWidthRatio;


	///�װ�

	m_CHK_bUseLightDotDetect = m_ptParameters->tLightDotParam.bUseDetect;
	m_EDT_nLightDotDynThresh = m_ptParameters->tLightDotParam.nLightDotDynThresh;
	m_EDT_dLightDotMinArea = m_ptParameters->tLightDotParam.dMinArea;
	///�ڰ�

	m_CHK_bUseDarkDotDetect = m_ptParameters->tDarkDotParam.bUseDetect;
	m_EDT_nDarkDotDynThresh = m_ptParameters->tDarkDotParam.nDarkDotDynThresh;
	m_EDT_dDarkDotMinArea = m_ptParameters->tDarkDotParam.dMinArea;
	///��̼
	m_CHK_bUseTapeDetect = m_ptParameters->tTapeParam.bUseDetect;

	m_EDT_nTapeDynThresh = m_ptParameters->tTapeParam.nTapeDynThresh;
	m_EDT_dMinTapeDeviation = m_ptParameters->tTapeParam.dDevFactor;
	m_EDT_dLTMinArea = m_ptParameters->tTapeParam.dLTMinArea;

	m_EDT_nMaxTapeThresh = m_ptParameters->tTapeParam.nTapeThresh;
	m_EDT_dMinTapeArea = m_ptParameters->tTapeParam.dTapeThreshMinArea;


	//��������
	m_CHK_bUseJDDetect = m_ptParameters->tJiaoDaiParam.bUseDetect;
	m_EDT_nJDDarkThresh = m_ptParameters->tJiaoDaiParam.nDarkThresh;
	m_EDT_dJDDarkArea = m_ptParameters->tJiaoDaiParam.dMinArea;
	m_EDT_nJDLightThresh = m_ptParameters->tJiaoDaiParam.nLightThresh;


	///����ȱ��

	m_CHK_bUseFoilDetect = m_ptParameters->tFoilParam.bUseDetect;
	m_EDT_nFoilDarkThresh = m_ptParameters->tFoilParam.nMaxDarkThresh;
	m_EDT_dFoilDarkMinArea = m_ptParameters->tFoilParam.dMinDarkArea;


	m_CHK_bUseAT9Detect = m_ptParameters->tAT9Param.bUseDetect;
	m_EDT_dAT9LightThresh = m_ptParameters->tAT9Param.nAT9LMinThresh;
	m_EDT_dAT9LightHeight = m_ptParameters->tAT9Param.dAT9LMinHeight;
	m_EDT_dAT9LightWidth = m_ptParameters->tAT9Param.dAT9LMinWidth;

	m_EDT_nAT9DarkThresh = m_ptParameters->tAT9Param.nAT9DarkMaxThresh;
	m_EDT_dAT9DarkWidth = m_ptParameters->tAT9Param.dAT9DarkMinWidth;
	m_EDT_dAT9DarkHeight = m_ptParameters->tAT9Param.dAT9DarkMinHeight;

	///��β
	m_CHK_bUseDetectTuoWei = m_ptParameters->tTuoWeiParam.bUseDetect;
	m_EDT_nTuoWeiLowThresh = m_ptParameters->tTuoWeiParam.nJXTapeLTMinThresh;
	m_EDT_nTuoWeiHighThresh = m_ptParameters->tTuoWeiParam.nJXTapeLTMaxThresh;
	m_EDT_dTuoWeiMinArea = m_ptParameters->tTuoWeiParam.dJXTapeLTMinArea;


	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPageSet::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO scrollinfo;
	GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);

	switch (nSBCode)
	{
	case SB_BOTTOM: //�����������ײ�
		ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMax) * 10); //������Ļ
		scrollinfo.nPos = scrollinfo.nMax; //�趨������λ��
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL); //���»���λ��
		break;
	case SB_TOP: //������������
		ScrollWindow(0, (scrollinfo.nPos - scrollinfo.nMin) * 10);
		scrollinfo.nPos = scrollinfo.nMin;
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		break;
	case SB_LINEUP: //�����ϼ�ͷ
		scrollinfo.nPos -= 1;
		if (scrollinfo.nPos < scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, 10);
		break;
	case SB_LINEDOWN: //�����¼�ͷ
		scrollinfo.nPos += 1;
		if (scrollinfo.nPos > scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, -10);
		break;
	case SB_PAGEUP: //���������Ϸ��հ�����
		scrollinfo.nPos -= 5;
		if (scrollinfo.nPos < scrollinfo.nMin)
		{
			scrollinfo.nPos = scrollinfo.nMin;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, 10 * 5);
		break;
	case SB_PAGEDOWN: //���������·��հ�����
		scrollinfo.nPos += 5;
		if (scrollinfo.nPos > scrollinfo.nMax)
		{
			scrollinfo.nPos = scrollinfo.nMax;
			break;
		}
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		ScrollWindow(0, -10 * 5);
		break;
	case SB_ENDSCROLL: //����뿪���飬���������϶�
		// MessageBox("SB_ENDSCROLL");
		break;
	case SB_THUMBPOSITION:
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);
		// scrollinfo.nPos = nPos;
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);
		break;
	case SB_THUMBTRACK: //�϶�����
		ScrollWindow(0, (scrollinfo.nPos - nPos) * 10);
		scrollinfo.nPos = nPos;
		SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
		break;
	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CPageSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialogEx::PreTranslateMessage(pMsg);
}



//void CPageSet::OnBnClickedButtonCreatemark()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}

void CPageSet::FreshParam()
{
	UpdateData(TRUE);
	///Ԥ�������
	m_ptParameters->tPretreatParam.dBorderWidth = m_EDT_dBorderErosionLength;
	m_ptParameters->tPretreatParam.dScaleFactor = m_EDT_dScaleFactor;
	m_ptParameters->tPretreatParam.dRightBorderWidth = m_EDT_dRightBorderWidth;
	m_ptParameters->tPretreatParam.dYaHenPingBiKuanDu = m_EDT_dYaHenPingBiKuanDu;
	m_ptParameters->tPretreatParam.dMidLightCof = m_EDT_dMidLightCoff;
	m_ptParameters->tPretreatParam.dMidDarkCof = m_EDT_dMidDarkCoff;
	m_ptParameters->tPretreatParam.dBorderLightCof = m_EDT_dBorderLightCoff;
	m_ptParameters->tPretreatParam.dBorderDarkCof = m_EDT_dBorderDarkCoff;
	m_ptParameters->tPretreatParam.dMLPSongjindu = m_EDT_dMLPSongjindu;
	m_ptParameters->tPretreatParam.bEnbleUseClassyfy = m_bEnbleUseClassyfy;//������
	///©������

	m_ptParameters->tLeakFoilParam.bUseDetect = m_CHK_bUseLeakFoilDetect;
	m_ptParameters->tLeakFoilParam.nThresh = m_EDT_nLeakFoilThresh;
	m_ptParameters->tLeakFoilParam.dMinArea = m_EDT_dLeakFoilMinArea;


	////����
	m_ptParameters->tBubbleParam.bUseDetect = m_CHK_bUseBubbleDetect;
	m_ptParameters->tBubbleParam.nDynThresh = m_EDT_nBubbleDynThresh;
	m_ptParameters->tBubbleParam.dMinArea = m_EDT_dBubbleMinArea;
	m_ptParameters->tBubbleParam.dMinCircularity = m_EDT_dBubbleMinCircularity;


	///����

	m_ptParameters->tDarkLineParam.bUseDetect = m_CHK_bUseDarkLineDetect;
	m_ptParameters->tDarkLineParam.nDynThresh = m_EDT_nDarkLineDynThresh;
	m_ptParameters->tDarkLineParam.dMinWidth = m_EDT_dDarkLineMinWidth;
	m_ptParameters->tDarkLineParam.dMinLengthWidthRatio = m_EDT_dDarkLineMinRatio;


	///�װ�

	m_ptParameters->tLightDotParam.bUseDetect = m_CHK_bUseLightDotDetect;
	m_ptParameters->tLightDotParam.nLightDotDynThresh = m_EDT_nLightDotDynThresh;
	m_ptParameters->tLightDotParam.dMinArea = m_EDT_dLightDotMinArea;

	///�ڰ�

	m_ptParameters->tDarkDotParam.bUseDetect = m_CHK_bUseDarkDotDetect;
	m_ptParameters->tDarkDotParam.nDarkDotDynThresh = m_EDT_nDarkDotDynThresh;
	m_ptParameters->tDarkDotParam.dMinArea = m_EDT_dDarkDotMinArea;


	///��̼
	m_ptParameters->tTapeParam.bUseDetect = m_CHK_bUseTapeDetect;
	m_ptParameters->tTapeParam.nTapeDynThresh = m_EDT_nTapeDynThresh;
	m_ptParameters->tTapeParam.dDevFactor = m_EDT_dMinTapeDeviation;
	m_ptParameters->tTapeParam.dLTMinArea = m_EDT_dLTMinArea;

	m_ptParameters->tTapeParam.nTapeThresh = m_EDT_nMaxTapeThresh;
	m_ptParameters->tTapeParam.dTapeThreshMinArea = m_EDT_dMinTapeArea;

	/////����ȱ��
	m_ptParameters->tFoilParam.bUseDetect = m_CHK_bUseFoilDetect;
	m_ptParameters->tFoilParam.nMaxDarkThresh = m_EDT_nFoilDarkThresh;
	m_ptParameters->tFoilParam.dMinDarkArea = m_EDT_dFoilDarkMinArea;


	/////����������
	m_ptParameters->tJiaoDaiParam.bUseDetect = m_CHK_bUseJDDetect;
	m_ptParameters->tJiaoDaiParam.nDarkThresh = m_EDT_nJDDarkThresh;
	m_ptParameters->tJiaoDaiParam.nLightThresh = m_EDT_nJDLightThresh;
	m_ptParameters->tJiaoDaiParam.dMinArea = m_EDT_dJDDarkArea;

	///AT9ȱ��


	m_ptParameters->tAT9Param.bUseDetect = m_CHK_bUseAT9Detect;
	m_ptParameters->tAT9Param.nAT9LMinThresh = m_EDT_dAT9LightThresh;
	m_ptParameters->tAT9Param.dAT9LMinHeight = m_EDT_dAT9LightHeight;
	m_ptParameters->tAT9Param.dAT9LMinWidth = m_EDT_dAT9LightWidth;

	m_ptParameters->tAT9Param.nAT9DarkMaxThresh = m_EDT_nAT9DarkThresh;
	m_ptParameters->tAT9Param.dAT9DarkMinWidth = m_EDT_dAT9DarkWidth;
	m_ptParameters->tAT9Param.dAT9DarkMinHeight = m_EDT_dAT9DarkHeight;

	///��β

	m_ptParameters->tTuoWeiParam.bUseDetect = m_CHK_bUseDetectTuoWei;
	m_ptParameters->tTuoWeiParam.nJXTapeLTMinThresh = m_EDT_nTuoWeiLowThresh;
	m_ptParameters->tTuoWeiParam.nJXTapeLTMaxThresh = m_EDT_nTuoWeiHighThresh;
	m_ptParameters->tTuoWeiParam.dJXTapeLTMinArea = m_EDT_dTuoWeiMinArea;


	//	m_pParentDlg->Test(1);
}






void CPageSet::OnBnClickedChkUsetapedetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	FreshParam();

}


void CPageSet::OnBnClickedChkUsedarkdotdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtDarkdotmaxthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtDarkdotminwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtDarkdotminheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtDarkdotminarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtDarkdotmaxarea()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnBnClickedChkUselightdotdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtLightdotminthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtLightdotminwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightdotminheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtLightdotminarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtLightdotmaxarea()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnBnClickedChkUsedarklinedetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtDarklinedynthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtDarklineminwidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtDarklineminheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtDarklineminwhratio()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnBnClickedChkUselightlinedetect()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	FreshParam();
//}


//void CPageSet::OnEnChangeEdtLightlinedynthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightlineminwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightlineminheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightlineminwhratio()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnBnClickedChkUsebubbledetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtBubbledynthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtBubblemindiam()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtBubblemincircularity()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


//void CPageSet::OnEnChangeEdtBubbleinoutdiff()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	FreshParam();
//}


void CPageSet::OnBnClickedChkUseleakfoildetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*if (IDOK==MessageBox(_T("ȷ����Ҫ�رո������㷨��"),_T("����!"),MB_ICONINFORMATION|MB_OKCANCEL|MB_SYSTEMMODAL))
	{
	FreshParam();
	}else
	{
	m_CHK_bUseLeakFoilDetect=TRUE;
	UpdateData(TRUE);
	}*/
	FreshParam();
}


void CPageSet::OnEnChangeEdtLeakfoilthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtLeakfoilminarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtLeakfoildynthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


//void CPageSet::OnEnChangeEdtLeakfoilmindiam()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	FreshParam();
//}


void CPageSet::OnEnChangeEdtLeakfoilmindeviation()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


//void CPageSet::OnEnChangeEdtLeakfoilmaxdiam()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	FreshParam();
//}


void CPageSet::OnEnChangeEdtBordererosionlength()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtMidlightcoff()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtMiddarkcoff()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtBorderlightcoff()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtBorderdarkcoff()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnBnClickedChkUsefoildetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnEnChangeEdtFoildarkthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtFoildarkminwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtFoildarkminheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtFoildarkminarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtFoildarkminnum()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtFoillightthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtFoillightminarea()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}

void CPageSet::AddFeature(EM_Feature eFeature)
{
	int nItemCount = m_LST_Temp.GetItemCount();
	m_LST_Temp.InsertItem(nItemCount, NAME_Feature[eFeature]);
	CString strValue;
	strValue.Format("%.2f", 0);
	m_LST_Temp.SetItemText(nItemCount, 1, strValue);
	strValue.Format("%.2f", 999999.00);
	m_LST_Temp.SetItemText(nItemCount, 2, strValue);

}
void CPageSet::OnBnClickedMfcmenubtnFeature()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LST_Temp.EndEdit();
	if (0 != m_MenuBtn_FeatureSelect.m_nMenuResult)
	{
		AddFeature((EM_Feature)(m_MenuBtn_FeatureSelect.m_nMenuResult - 1));
	}
}
void CPageSet::DeleteFeature()
{

	if (0 == m_LST_Temp.GetFirstSelectedItemPosition())//���ûѡ���κ�Ʒ��,����
	{
		return;
	}
	CString strDeleteProName = "";
	while (m_LST_Temp.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) != -1)
	{

		int nItem = m_LST_Temp.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		m_LST_Temp.DeleteItem(nItem);

	}
}

void CPageSet::OnBnClickedBtnDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LST_Temp.EndEdit();
	DeleteFeature();
}


void CPageSet::OnBnClickedBtnAddalgrithm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (0 == m_LST_Temp.GetItemCount())
	{
		return;
	}
	CMFCPropertyGridProperty *pGoup0 = new CMFCPropertyGridProperty(g_StrArrayErrorName[m_COM_nErrorNameIndex]);
	CString strName, strValve;
	for (int k = 0; k < m_LST_Temp.GetItemCount(); k++)
	{
		strName = m_LST_Temp.GetItemText(k, 0);
		pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("����"), strName, _T("0-1")));
		strName = m_LST_Temp.GetItemText(k, 1);
		pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("MIN"), strName, _T("0-1")));
		strName = m_LST_Temp.GetItemText(k, 2);
		pGoup0->AddSubItem(new CMFCPropertyGridProperty(_T("MAX"), strName, _T("0-1")));
	}
	m_PropertyGrid_Algrithm.AddProperty(pGoup0);

}


void CPageSet::OnBnClickedBtnDeletealgrithm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_PropertyGrid_Algrithm.GetPropertyCount() > 0)
	{
		CMFCPropertyGridProperty* pProperty = m_PropertyGrid_Algrithm.GetCurSel();
		if (pProperty->IsGroup())
		{
			m_PropertyGrid_Algrithm.DeleteProperty(pProperty);
		}

	}


}
BOOL CPageSet::IsNumber(CString& strText)
{
	for (int i = 0; i < strText.GetLength(); i++)
	{
		if ((strText.GetAt(i) >57 || strText.GetAt(i) < 48)
			&& strText.GetAt(i) != 46 && strText.GetAt(i) != 45)
		{
			return FALSE;
		}
	}
	return TRUE;
}
//void CPageSet:: WriteParam()
//{
//	
//	CString strValue = "";
//	CString strName = "";
//	CString strDiscription = "";
//	try
//	{
//		FeatureSelect tFeatureSelect;
//		tFeatureSelect.nGroupNum=m_PropertyGrid_Algrithm.GetPropertyCount();
//		for(int i=0;i<m_PropertyGrid_Algrithm.GetPropertyCount();i++)
//		{
//			CMFCPropertyGridProperty* pGroup = m_PropertyGrid_Algrithm.GetProperty(i);
//			strDiscription=pGroup->GetDescription();
//			strValue = (CString)pGroup->GetValue();
//			strName = (CString)pGroup->GetName();
//
//			SingleGroupFeature tSingleFeature;
//			tSingleFeature.nSingleGroupNum=pGroup->GetSubItemsCount();
//			tSingleFeature.hv_SingleFSelects.Append((HTuple)strName);
//			for (int j=0;j<pGroup->GetSubItemsCount();j++)
//			{
//				strName = (CString)pGroup->GetSubItem(j)->GetName();//����
//				strValue = (CString)pGroup->GetSubItem(j)->GetValue();//����ֵ
//				tSingleFeature.hv_SingleFSelects.Append((HTuple)strValue);
//				strDiscription=pGroup->GetSubItem(j)->GetDescription();//�����ļ�
//
//			}
//			tFeatureSelect.Vec_FSelects.push_back(tSingleFeature);
//		
//		}
//		m_ptParameters->tFeatureSelect=tFeatureSelect;
//
//	
//	}catch(HException e)
//	{
//		CString strMessageBox;
//		strMessageBox.Format("%s��ֵ���� :%s",strName,e.ErrorMessage().Text());
//		AfxMessageBox(strMessageBox);
//	}
//
//}


void CPageSet::WriteParam()
{

	CString strValue = "";
	CString strName = "";
	CString strDiscription = "";
	try
	{
		FeatureSelect tFeatureSelect;
		tFeatureSelect.nGroupNum = m_PropertyGrid_Algrithm.GetPropertyCount();
		for (int i = 0; i < m_PropertyGrid_Algrithm.GetPropertyCount(); i++)
		{
			CMFCPropertyGridProperty* pGroup = m_PropertyGrid_Algrithm.GetProperty(i);
			strDiscription = pGroup->GetDescription();
			strValue = (CString)pGroup->GetValue();
			strName = (CString)pGroup->GetName();

			SingleErrorGroup tSingleErrorFeature;
			tSingleErrorFeature.nSingleGroupNum = pGroup->GetSubItemsCount();
			tSingleErrorFeature.strErrorName = (CString)pGroup->GetName();

			for (int j = 0; j < pGroup->GetSubItemsCount(); j = j + 3)
			{
				SingleFeature tSingleFeature;
				tSingleFeature.StrSingleFeatureName = (CString)pGroup->GetSubItem(j)->GetValue();//����ֵ
				tSingleFeature.dMin = atof((CString)pGroup->GetSubItem(j + 1)->GetValue());//����ֵ��СֵMIN
				tSingleFeature.dMax = atof((CString)pGroup->GetSubItem(j + 2)->GetValue());//����ֵ���ֵMAX
				tSingleErrorFeature.Vec_tSingleGroupFeatures.push_back(tSingleFeature);
			}

			tFeatureSelect.Vec_FSelects.push_back(tSingleErrorFeature);
		}
		m_ptParameters->tFeatureSelect = tFeatureSelect;


	}
	catch (HException e)
	{
		CString strMessageBox;
		strMessageBox.Format("%s��ֵ���� :%s", strName, e.ErrorMessage().Text());
		AfxMessageBox(strMessageBox);
	}

}


void CPageSet::OnBnClickedBtnLoadparam()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WriteParam();

}


//void CPageSet::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: �ڴ˴������Ϣ����������
//	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
//	//CRect rect;
//	//GetClientRect(rect);
//	//dc.FillSolidRect(rect,RGB(255,255,255)); //����Ϊ��ɫ����
//	CDialog::OnPaint();
//}


//HBRUSH CPageSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	// TODO:  �ڴ˸��� DC ���κ�����
//	if(nCtlColor==CTLCOLOR_STATIC)
//	{
//		pDC->SetTextColor(RGB(0,0,0));
//		pDC->SetBkColor(RGB(255,255,255));
//		HBRUSH brush=CreateSolidBrush(RGB(255,255,255));
//		return hbr;
//	}else if(nCtlColor==CTLCOLOR_EDIT)
//	{
//		//pDC->SetBkMode(TRANSPARENT);
//		pDC->SetTextColor(RGB(0,0,0));
//		pDC->SetBkColor(RGB(255,255,255));
//		HBRUSH b=CreateSolidBrush(RGB(255,255,255));
//		return hbr;
//	}
//	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
//}

void CPageSet::OnEnChangeEdtScalefactor()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtLthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtHthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtMinlighttapearea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtAt9lowthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtAt9minwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtAt9minheight()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightdotmaxthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtAt9maxthresh()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtAt9darkminwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//		FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtAt9minarea()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtFoillightwidth()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnEnChangeEdtJdDarkthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtJdMindarkarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtJdLightthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


//void CPageSet::OnEnChangeEdtJdMinlightarea()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CPageSet::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();

}


//void CPageSet::OnEnChangeEdtLightdotdeviation()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


//void CPageSet::OnEnChangeEdtLightdotgrayrange()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//	FreshParam();
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}

void CPageSet::OnPaint()
{
	CPaintDC dc(this); // device context for painting TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(200, 200, 200)); //����Ϊ��ɫ����
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


HBRUSH CPageSet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CPageSet::OnChangeEdtMlpsongjindu()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtTapefactor()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnClickedChkUseclassify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnBnClickedChkAt9enable()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnChangeEdtAt9lightthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtAt9lightheight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtAt9darkwidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtAt9darkthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtAt9darkheight()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtAt9lightwidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnClickedChkBusedetecttuowei()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	FreshParam();
}


void CPageSet::OnChangeEdtTuoweilowthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtTuoweihighthresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnChangeEdtTuoweiminarea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtRightborderwidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtYahenpingbikuandu()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtTapethresh()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CPageSet::OnEnChangeEdtMintapethresharea()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	FreshParam();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
