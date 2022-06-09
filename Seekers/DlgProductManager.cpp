// DlgProductManager.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgProductManager.h"
#include "afxdialogex.h"
#include "DlgNewProductName.h"
#include "SeekersDlg.h"
#include "DlgInput.h"
// CDlgProductManager 对话框

IMPLEMENT_DYNAMIC(CDlgProductManager, CDialogEx)

CDlgProductManager::CDlgProductManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProductManager::IDD, pParent)
	, m_COM_nProductJiXi(0)
	, m_EDT_nZhiDaiThresh(0)
	, m_EDT_nTrip(0)
	, m_COM_nGongYiLeiXing(0)
	, m_COM_nCoatType(0)
	, m_COM_nCaoWeiChoice(0)
	, m_COM_nOapChoice(0)
	, m_COM_nExistJXType(0)
	, m_CHK_bChangMoZaiShang(FALSE)
	, m_CHK_bQiTouXianChu(FALSE)
	, m_EDT_dTotalCoatWidth(0)
	, m_EDT_dLongCoat(0)
	, m_EDT_dShortCoat(0)
	, m_EDT_dTailCuoWei(0)
	, m_EDT_dHeadCuoWei(0)
{
	m_Listfont.CreatePointFont(110,"Microsoft YaHei");
}

CDlgProductManager::~CDlgProductManager()
{
}

void CDlgProductManager::SetOwner(CJudge* pOwner)
{
	m_pOwner = pOwner;
}

void CDlgProductManager::SetProductParam()
{
	CString strConfigFile;
	strConfigFile.Format("%s\\Config\\Products\\%s\\SCIJudge\\0\\JudgeParam.ini", GetCurrentAppPath(), m_strCurrentProductName);
	m_EDT_nTrip = GetPrivateProfileIntH("SizeParam", "hvTripNum", 2, strConfigFile);
	m_COM_nProductJiXi = GetPrivateProfileIntH("SizeParam", "hv_ProductType", 0, strConfigFile);
	m_COM_nCoatType = GetPrivateProfileIntH("SizeParam", "hv_CoatType", 1, strConfigFile);
	m_COM_nOapChoice = GetPrivateProfileIntH("SizeParam", "hv_OAPType", 0, strConfigFile);
	m_COM_nCaoWeiChoice = GetPrivateProfileIntH("SizeParam", "hv_CaoWeiType", 0, strConfigFile);
	m_COM_nExistJXType = GetPrivateProfileIntH("SizeParam", "hvExistJXType", 0, strConfigFile);
	m_COM_nGongYiLeiXing = GetPrivateProfileIntH("SizeParam", "hvGongYiType", 1, strConfigFile);
	m_CHK_bQiTouXianChu = GetPrivateProfileIntH("SizeParam", "hvQiTouXianChu", 1, strConfigFile);
	m_CHK_bChangMoZaiShang = GetPrivateProfileIntH("SizeParam", "hvChangMoZaiShang", 1, strConfigFile);
	m_EDT_nZhiDaiThresh = GetPrivateProfileIntH("SizeParam", "hvZhiDaiGray", 100, strConfigFile);

	double dDownIntermissionHeight, dUpIntermissionHeight, dBHJXTotalLength, dBHJXLength2;
	dDownIntermissionHeight = GetPrivateProfileDouble("SizeParam", "dDownIntermissionHeight", 20, strConfigFile);
	dUpIntermissionHeight = GetPrivateProfileDouble("SizeParam", "dUpIntermissionHeight", 20, strConfigFile);
	dBHJXTotalLength = GetPrivateProfileDouble("SizeParam", "dBHJXTotalLength", 20, strConfigFile);
	dBHJXLength2 = GetPrivateProfileDouble("SizeParam", "dBHJXLength2", 20, strConfigFile);

	m_EDT_dLongJXHeight = max(dDownIntermissionHeight, dUpIntermissionHeight);
	m_EDT_dShortJXHeight = min(dDownIntermissionHeight, dUpIntermissionHeight);
	m_EDT_dCoatWidth = GetPrivateProfileDouble("SizeParam", "dCoatWidth", 20, strConfigFile);
	m_EDT_dTotalCoatWidth = GetPrivateProfileDouble("SizeParam", "dTotalCoatWidth", 200, strConfigFile);
	m_EDT_dLongCoat = max(dBHJXTotalLength, dBHJXLength2);
	m_EDT_dShortCoat = min(dBHJXTotalLength, dBHJXLength2);
	m_EDT_dHeadCuoWei = GetPrivateProfileDouble("SizeParam", "dHeadCuoWei", 200, strConfigFile);
	m_EDT_dTailCuoWei = GetPrivateProfileDouble("SizeParam", "dTailCuoWei", 200, strConfigFile);

	UpdateData(FALSE);
}

void CDlgProductManager::WriteProductParam(CString strProductName)
{
	UpdateData(TRUE);
	CString strConfigFile;
	//保存系统参数：涂布类型0连续1间歇 产品类型0阴极1阳极
	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileInt(strProductName, "CoatType", m_COM_nCoatType, g_strConfigSystemFile);
	WritePrivateProfileInt(strProductName, "ProductType", m_COM_nProductJiXi, g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式

    //保存大小间歇，长膜短膜，头尾错位参数
	double dLongJXHeight = max(m_EDT_dLongJXHeight, m_EDT_dShortJXHeight);
	double dShortJXHeight = min(m_EDT_dLongJXHeight, m_EDT_dShortJXHeight);
	double dLongCoat = max(m_EDT_dLongCoat, m_EDT_dShortCoat);
	double dShortCoat = min(m_EDT_dLongCoat, m_EDT_dShortCoat);
	//前相机为反面
	double dDownIntermissionHeight, dUpIntermissionHeight, dBHJXTotalLength, dBHJXLength2;
	if (g_tSystemSet.nCameraModel == 0)//前相机为定位相机
	{
		//反面间歇高度
		dDownIntermissionHeight = m_CHK_bChangMoZaiShang == 0 ? dLongJXHeight : dShortJXHeight;
		dBHJXLength2 = m_CHK_bChangMoZaiShang == 0 ? dShortCoat : dLongCoat;
		//正面间歇高度
		dUpIntermissionHeight = m_CHK_bChangMoZaiShang == 0 ? dShortJXHeight : dLongJXHeight;
		dBHJXTotalLength = m_CHK_bChangMoZaiShang == 0 ? dLongCoat : dShortCoat;
	}
	else
	{
		//正面间歇高度
		dUpIntermissionHeight = m_CHK_bChangMoZaiShang == 0 ? dLongJXHeight : dShortJXHeight;
		dBHJXTotalLength = m_CHK_bChangMoZaiShang == 0 ? dShortCoat : dLongCoat;
		//反面间歇高度
		dDownIntermissionHeight = m_CHK_bChangMoZaiShang == 0 ? dShortJXHeight : dLongJXHeight;
		dBHJXLength2 = m_CHK_bChangMoZaiShang == 0 ? dLongCoat : dShortCoat;
	}
	//保存判断参数
	for (int i = 0; i < g_vec_Algorithm.size(); i++)
	{
		strConfigFile.Format("%s\\Config\\Products\\%s\\SCIJudge\\%d\\JudgeParam.ini", GetCurrentAppPath(), strProductName, i);
		WritePrivateProfileInt("SizeParam", "hvTripNum", m_EDT_nTrip, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hv_ProductType", m_COM_nProductJiXi, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hv_CoatType", m_COM_nCoatType, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hv_OAPType", m_COM_nOapChoice, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hv_CaoWeiType", m_COM_nCaoWeiChoice, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hvExistJXType", m_COM_nExistJXType, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hvGongYiType", m_COM_nGongYiLeiXing, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hvQiTouXianChu", m_CHK_bQiTouXianChu, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hvChangMoZaiShang", m_CHK_bChangMoZaiShang, strConfigFile);
		WritePrivateProfileInt("SizeParam", "hvZhiDaiGray", m_EDT_nZhiDaiThresh, strConfigFile);

		WritePrivateProfileDouble("SizeParam", "dDownIntermissionHeight", dDownIntermissionHeight, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dUpIntermissionHeight", dUpIntermissionHeight, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dBHJXTotalLength", dBHJXTotalLength, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dBHJXLength2", dBHJXLength2, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dCoatWidth", m_EDT_dCoatWidth, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dTotalCoatWidth", m_EDT_dTotalCoatWidth, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dHeadCuoWei", m_EDT_dHeadCuoWei, strConfigFile);
		WritePrivateProfileDouble("SizeParam", "dTailCuoWei", m_EDT_dTailCuoWei, strConfigFile);
	}
	//保存间歇查找参数
	for (int i = 0; i < g_vec_FindMark.size(); i++)
	{
		strConfigFile.Format("%s\\Config\\Products\\%s\\SCIFindMark\\%d\\Config.ini", GetCurrentAppPath(), strProductName, i);
		WritePrivateProfileInt("Mark", "m_nLocateUpOrDown", m_CHK_bQiTouXianChu, strConfigFile);//默认
		WritePrivateProfileDouble("Mark", "m_dIgnoreDist", m_CHK_bChangMoZaiShang == 0 ? dLongJXHeight : dShortJXHeight, strConfigFile);//默认50mm
	}
}


void CDlgProductManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_BUTTON_CREATE, m_btnCreate);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_btnCopy);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_btnLoad);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_btnExit);
	DDX_Control(pDX, IDC_LIST_PRODUCT, m_listProduct);
	DDX_Control(pDX, IDC_BUTTON_NOTE, m_btnNote);
	DDX_Control(pDX, IDC_STATIC_CURRENT_PRODUCT, m_STATIC_chCurrentProduct);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	//  DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_clsoe);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_BTN_close);
	DDX_CBIndex(pDX, IDC_COM_JIXING, m_COM_nProductJiXi);
	DDX_Text(pDX, IDC_EDT_ZHIDAITHRESH2, m_EDT_nZhiDaiThresh);
	DDX_Text(pDX, IDC_EDT_TRIPNUM, m_EDT_nTrip);
	DDX_CBIndex(pDX, IDC_COM_GONGYILEIXING, m_COM_nGongYiLeiXing);
	DDX_CBIndex(pDX, IDC_COM_COATTYPE, m_COM_nCoatType);
	DDX_CBIndex(pDX, IDC_COM_CAOWEICHOICE, m_COM_nCaoWeiChoice);
	DDX_CBIndex(pDX, IDC_COM_OAPCOHICE, m_COM_nOapChoice);
	DDX_CBIndex(pDX, IDC_COM_EXISTJXTYPE, m_COM_nExistJXType);
	DDX_Check(pDX, IDC_CHK_CHANGMOZAISHANG, m_CHK_bChangMoZaiShang);
	DDX_Check(pDX, IDC_CHK_QITOUXIANCHU, m_CHK_bQiTouXianChu);
	DDX_Text(pDX, IDC_EDT_LONGJX, m_EDT_dLongJXHeight);
	DDX_Text(pDX, IDC_EDT_SHORTJX, m_EDT_dShortJXHeight);
	DDX_Text(pDX, IDC_EDIT_dCoatWidth, m_EDT_dCoatWidth);


	DDX_Text(pDX, IDC_EDIT_dTotalCoatWidth, m_EDT_dTotalCoatWidth);
	DDX_Text(pDX, IDC_EDT_LONGCOAT, m_EDT_dLongCoat);
	DDX_Text(pDX, IDC_EDT_SHORTCOAT, m_EDT_dShortCoat);
	DDX_Text(pDX, IDC_EDT_TAILCUOWEI, m_EDT_dTailCuoWei);
	DDX_Text(pDX, IDC_EDT_HEADCUOWEI, m_EDT_dHeadCuoWei);
}


BEGIN_MESSAGE_MAP(CDlgProductManager, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CDlgProductManager::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CDlgProductManager::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgProductManager::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CDlgProductManager::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgProductManager::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_NOTE, &CDlgProductManager::OnBnClickedButtonNote)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDlgProductManager::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgProductManager::OnBnClickedButtonSave)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PRODUCT, &CDlgProductManager::OnLvnItemchangedListProduct)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PRODUCT, &CDlgProductManager::OnNMClickListProduct)
END_MESSAGE_MAP()


// CDlgProductManager 消息处理程序


BOOL CDlgProductManager::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitStatic();
	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgProductManager::InitListCtrl()
{
	while(m_listProduct.GetHeaderCtrl()->GetItemCount())
	{
		m_listProduct.DeleteColumn(0);//删除所有列
	}
	m_listProduct.DeleteAllItems();//删除所有行
	m_listProduct.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EDITLABELS|LVS_EX_SUBITEMIMAGES);
	m_listProduct.InsertColumn(0,"序号",LVCFMT_LEFT,55);
	m_listProduct.InsertColumn(1,"配方名",LVCFMT_LEFT,200);
	m_listProduct.InsertColumn(2,"注释",LVCFMT_LEFT,250);

	m_listProduct.SetColorHeader(TRUE);
	m_listProduct.SetHeaderBKColor(255,255,255,0);
	m_listProduct.SetFont(&m_Listfont);
	
	int nCurrentCount = GetPrivateProfileInt("PRODUCT", "COUNT", 0, g_strConfigSystemFile);//获取当前个数
	CString strKey, strValue, strSN;
	CString strNote;
	for (int i = 0; i < nCurrentCount; i++)
	{
		strKey.Format("PRODUCT%d", i);
		GetPrivateProfileString("PRODUCT", strKey, 0, strValue.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
		if (strValue == g_strProductName)
		{
			m_nUseProductSN = i;
			m_nCurrentProductSN = m_nUseProductSN;
			m_strCurrentProductName = g_strProductName;
		}
		strValue.ReleaseBuffer();
		strSN.Format("%d", i + 1);
		int nItemCount = m_listProduct.GetItemCount();
		m_listProduct.InsertItem(nItemCount, 0);
		m_listProduct.SetItemText(nItemCount, 0, strSN);
		m_listProduct.SetItemText(nItemCount, 1, strValue);
		GetPrivateProfileString("PRODUCTNOTE", strValue, "", strNote.GetBuffer(MAX_PATH), MAX_PATH, g_strConfigSystemFile);
		strNote.ReleaseBuffer();
		m_listProduct.SetItemText(nItemCount, 2, strNote);
	}
}


void CDlgProductManager::InitStatic()
{
	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);

	m_STATIC_chCurrentProduct.SetTextColor(RGB(0,0,0));
	m_STATIC_chCurrentProduct.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_STATIC_chCurrentProduct.Format("当前配方:%s",g_strProductName);
	m_STATIC_chCurrentProduct.SetTransparent(TRUE);

	m_COM_nProductJiXi = m_pOwner->m_tStandardSize.hv_ProductType;
	m_EDT_nTrip = m_pOwner->m_tStandardSize.hvTripNum;
	m_COM_nGongYiLeiXing = m_pOwner->m_tStandardSize.hvGongYiType;
	m_COM_nCoatType = m_pOwner->m_tStandardSize.hv_CoatType;
	m_COM_nCaoWeiChoice = m_pOwner->m_tStandardSize.hv_CaoWeiType;
	m_COM_nOapChoice = m_pOwner->m_tStandardSize.hv_OAPType;
	m_COM_nExistJXType = m_pOwner->m_tStandardSize.hvExistJXType;
	m_EDT_nZhiDaiThresh = m_pOwner->m_tStandardSize.hvZhiDaiGray;
	m_CHK_bChangMoZaiShang = m_pOwner->m_tStandardSize.hvChangMoZaiShang;
	m_CHK_bQiTouXianChu = m_pOwner->m_tStandardSize.hvQiTouXianChu;

	m_EDT_dLongJXHeight = max(m_pOwner->m_tStandardSize.dDownIntermissionHeight, m_pOwner->m_tStandardSize.dUpIntermissionHeight);
	m_EDT_dShortJXHeight = min(m_pOwner->m_tStandardSize.dDownIntermissionHeight, m_pOwner->m_tStandardSize.dUpIntermissionHeight);

	m_EDT_dCoatWidth = m_pOwner->m_tStandardSize.dCoatWidth;
	m_EDT_dTotalCoatWidth = m_pOwner->m_tStandardSize.dTotalCoatWidth;
	m_EDT_dLongCoat = max(m_pOwner->m_tStandardSize.dBHJXTotalLength, m_pOwner->m_tStandardSize.dBHJXLength2);
	m_EDT_dShortCoat = min(m_pOwner->m_tStandardSize.dBHJXTotalLength, m_pOwner->m_tStandardSize.dBHJXLength2);
	m_EDT_dHeadCuoWei = m_pOwner->m_tStandardSize.dHeadCuoWei;
	m_EDT_dTailCuoWei = m_pOwner->m_tStandardSize.dTailCuoWei;

	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnCreate.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnDelete.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCopy.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnLoad.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnExit.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnNote.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnSave.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	
	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);

	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);

	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKOUT);

	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);

	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);

	m_btnCreate.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnDelete.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCopy.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnLoad.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnExit.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnNote.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnSave.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);

	m_btnCreate.SetFont(&m_ftButton);
	m_btnDelete.SetFont(&m_ftButton);
	m_btnCopy.SetFont(&m_ftButton);
	m_btnLoad.SetFont(&m_ftButton);
	m_btnExit.SetFont(&m_ftButton);
	m_btnNote.SetFont(&m_ftButton);
	m_btnSave.SetFont(&m_ftButton);

	m_BTN_close.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_BTN_close.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_BTN_close.SetIcon(IDI_ICON_W_UNCHECK);
	m_BTN_close.SetFont(&m_ftButton);
	UpdateData(FALSE);
}

void CDlgProductManager::OnBnClickedButtonExit()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CDlgProductManager::CreateNewProduct(CString strNewProductName)
{
	int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//获取当前个数
	CString strKey;
	strKey.Format("PRODUCT%d",nCurrentCount);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileString("PRODUCT",strKey,strNewProductName,g_strConfigSystemFile);
	nCurrentCount++;
	WritePrivateProfileInt("PRODUCT","COUNT",nCurrentCount,g_strConfigSystemFile);
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
}


void CDlgProductManager::CopyProduct(CString strSrc,CString strDst)
{
	CString strDstFolder,strSrcFolder;
		TCHAR chAllKeysAndValues[1000];
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	GetPrivateProfileSection(strSrc,chAllKeysAndValues,300,g_strConfigSystemFile);
	WritePrivateProfileSection(strDst,chAllKeysAndValues,g_strConfigSystemFile);
	WritePrivateProfileInt(strDst, "FindLine", 1, g_strConfigSystemFile);//是否是首次加载配方
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
	strSrcFolder.Format("%s//Config//Products//%s",GetCurrentAppPath(),strSrc);
	strDstFolder.Format("%s//Config//Products//%s",GetCurrentAppPath(),strDst);
	CopyFolder(strSrcFolder,strDstFolder);

	int nValue;
	double dValue;
	//品种设置参数
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	nValue = GetPrivateProfileInt(strSrc,"CoatType",0,g_strConfigSystemFile);
	WritePrivateProfileInt(strDst,"CoatType",nValue,g_strConfigSystemFile);
	nValue = GetPrivateProfileInt(strSrc,"ProductType",0,g_strConfigSystemFile);
	WritePrivateProfileInt(strDst,"ProductType",nValue,g_strConfigSystemFile);
	nValue = GetPrivateProfileInt(strSrc,"GrabberType",0,g_strConfigSystemFile);
	WritePrivateProfileInt(strDst,"GrabberType",nValue,g_strConfigSystemFile);
	nValue = GetPrivateProfileInt(strSrc,"DetectMisMatch",0,g_strConfigSystemFile);
	WritePrivateProfileInt(strDst,"DetectMisMatch",nValue,g_strConfigSystemFile);
	dValue = GetPrivateProfileDouble(strSrc,"MisMatch",0,g_strConfigSystemFile);
	WritePrivateProfileDouble(strDst,"MisMatch",dValue,g_strConfigSystemFile);

	
}
void CDlgProductManager::DeleteProduct(CString strName)
{
	//写入文件
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
	WritePrivateProfileString("PRODUCTNOTE",strName,NULL,g_strConfigSystemFile);//删除备注
	WritePrivateProfileSection("PRODUCT",0,g_strConfigSystemFile);//清除该键值
	WritePrivateProfileSection(strName,0,g_strConfigSystemFile);
	int nCurrentCount = m_listProduct.GetItemCount();
	WritePrivateProfileInt("PRODUCT","COUNT",nCurrentCount,g_strConfigSystemFile);//获取当前个数
	for(int i=0;i<nCurrentCount;i++)
	{
		CString strProductName = m_listProduct.GetItemText(i,1);
		CString strKey;
		strKey.Format("PRODUCT%d",i);
		WritePrivateProfileString("PRODUCT",strKey,strProductName,g_strConfigSystemFile);
	}
	SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
	CString strSrcFolder;
	strSrcFolder.Format("%s//Config//Products//%s",GetCurrentAppPath(),strName);
	DeleteFolder(strSrcFolder);
}
void CDlgProductManager::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgNewProductName dlg;
	if (IDOK == dlg.DoModal())
	{
		CString strNew;
		strNew.Format("%s",dlg.m_strNewProduct);

		int nCurrentCount = GetPrivateProfileInt("PRODUCT","COUNT",0,g_strConfigSystemFile);//获取当前个数
		CString strKey,strValue,strSN;
		for (int i=0;i<nCurrentCount;i++)
		{
			strKey.Format("PRODUCT%d",i);
			GetPrivateProfileString("PRODUCT",strKey,0,strValue.GetBuffer(MAX_PATH),MAX_PATH,g_strConfigSystemFile);
			strValue.ReleaseBuffer();
			if (strValue==strNew)
			{
				AfxMessageBox("该配方已存在，请重新输入新配方名字");
				return;
			}
		}

		CreateNewProduct(strNew);
		CopyProduct(g_strProductName,strNew);
		InitListCtrl();
	}
}


void CDlgProductManager::OnBnClickedButtonDelete()
{

	if(0 == m_listProduct.GetFirstSelectedItemPosition())//如果没选中任何品种,返回
	{
		return;
	}
	CString strDeleteProName = "";
	if(m_listProduct.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED) != -1)
	{
		int nItem = m_listProduct.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
		strDeleteProName = m_listProduct.GetItemText(nItem,1);
		if (g_strProductName == strDeleteProName)
		{
			AfxMessageBox("选中Modle正在使用，不能删除！");
			return;
		}
		if (IDOK == AfxMessageBox(_T("确认删除Modle:"+ strDeleteProName),MB_ICONINFORMATION | MB_OKCANCEL | MB_SYSTEMMODAL))
		{
			m_listProduct.DeleteItem(nItem);
			DeleteProduct(strDeleteProName);
		}
	}
	InitListCtrl();
}


void CDlgProductManager::OnBnClickedButtonCopy()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//如果没选中任何品种,返回
	{
		return;
	}
	CString strNew;
	m_strCurrentProductName = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos),1);
	strNew = m_strCurrentProductName + "备份";
	CreateNewProduct(strNew);
	CopyProduct(m_strCurrentProductName,strNew);
	InitListCtrl();
}


void CDlgProductManager::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//如果没选中任何品种,返回
	{
		return;
	}
	CString strNewProduct = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos),1);
	CString strMessage;
	if (strNewProduct == g_strProductName)
	{
		strMessage.Format("不能选择当前正在使用的品种%s", strNewProduct);
		AfxMessageBox(strMessage, MB_YESNO);
		return;
	}
	
	strMessage.Format("确定切换品种到:%s  应用程序将会重启",strNewProduct);
	if (IDYES == AfxMessageBox(strMessage,MB_YESNO))
	{
		g_strProductName = strNewProduct;
		SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
		WritePrivateProfileInt(g_strProductName, "FindLine", 1, g_strConfigSystemFile);
		SetFileAttributes(g_strConfigSystemFile, FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
		((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS, 0, 0);
	}
	
}


void CDlgProductManager::OnBnClickedButtonNote()
{
	
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if(0 ==pos)//如果没选中任何品种,返回
	{
		return;
	}
	int nSelItem = m_listProduct.GetNextSelectedItem(pos);
	if (-1!=nSelItem)
	{
		CString strSelProductName;
		m_listProduct.GetItemText(nSelItem,1,strSelProductName.GetBuffer(MAX_PATH),MAX_PATH);
		CDlgInput dlg;
		dlg.m_strTitle.Format("请输入%s的备注",strSelProductName);
		if (IDOK == dlg.DoModal())
		{
			SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_NORMAL);//设置配置文件为可写入模式
			if (""== dlg.m_strInput)
			{
				WritePrivateProfileString("PRODUCTNOTE",strSelProductName,NULL,g_strConfigSystemFile);
			}else
			{
				WritePrivateProfileString("PRODUCTNOTE",strSelProductName,dlg.m_strInput,g_strConfigSystemFile);
			}
			InitListCtrl();
			SetFileAttributes(g_strConfigSystemFile,FILE_ATTRIBUTE_READONLY);//设置配置文件为只读模式
		}
	}
	


}


void CDlgProductManager::OnPaint()
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

	//画边框线
	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(200,200,200));//154,222,106
	dc.SelectObject(pen);
	dc.MoveTo(rcDlg.left,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.top);
	dc.LineTo(rcDlg.right-1,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.bottom-1);
	dc.LineTo(rcDlg.left,rcDlg.top);

	GetDlgItem(IDC_LIST_PRODUCT)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
}


HBRUSH CDlgProductManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgProductManager::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgProductManager::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgProductManager::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
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


LRESULT CDlgProductManager::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcTitle;
	GetDlgItem(IDC_STATIC_TITLE)->GetWindowRect(rcTitle);

	return rcTitle.PtInRect(point)
		? HTCAPTION : CDialogEx::OnNcHitTest(point);
	//return CDialogEx::OnNcHitTest(point);
}


void CDlgProductManager::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CDlgProductManager::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_CPowerLimit.m_bFactoryPower == FALSE && g_CPowerLimit.m_bSuperEngineerPower == FALSE && g_CPowerLimit.m_bEngineerPower == FALSE)
	{
		AfxMessageBox("保存参数需要工程师以上权限");
		return;
	}
	if (m_strCurrentProductName == g_strProductName)//当前选中参数为系统正在使用产品的参数
	{
		UpdateData(TRUE);
		//ComPareChange(m_pOwner->m_tStandardSize.hvTripNum.I(), m_EDT_nTrip, "产品条数");
		m_pOwner->m_tStandardSize.hvTripNum = m_EDT_nTrip;
		//ComPareChange(m_pOwner->m_tStandardSize.hv_ProductType.I(), m_COM_nProductJiXi, "产品极性");
		m_pOwner->m_tStandardSize.hv_ProductType = m_COM_nProductJiXi;
		//ComPareChange(m_pOwner->m_tStandardSize.hv_OAPType.I(), m_COM_nOapChoice, "OAP类型");
		m_pOwner->m_tStandardSize.hv_OAPType = m_COM_nOapChoice;
		//ComPareChange(m_pOwner->m_tStandardSize.hv_CaoWeiType.I(), m_COM_nCaoWeiChoice, "槽位类型");
		m_pOwner->m_tStandardSize.hv_CaoWeiType = m_COM_nCaoWeiChoice;
		//ComPareChange(m_pOwner->m_tStandardSize.hvExistJXType.I(), m_COM_nExistJXType, "间歇类型");
		m_pOwner->m_tStandardSize.hvExistJXType = m_COM_nExistJXType;
		//ComPareChange(m_pOwner->m_tStandardSize.hvGongYiType.I(), m_COM_nGongYiLeiXing, "工艺类型");
		m_pOwner->m_tStandardSize.hvGongYiType = m_COM_nGongYiLeiXing;
		//ComPareChange(m_pOwner->m_tStandardSize.hvChangMoZaiShang.I(), m_CHK_bChangMoZaiShang, "长膜在上");
		m_pOwner->m_tStandardSize.hvChangMoZaiShang = m_CHK_bChangMoZaiShang;
		//ComPareChange(m_pOwner->m_tStandardSize.hvQiTouXianChu.I(), m_CHK_bQiTouXianChu, "齐头先出");
		m_pOwner->m_tStandardSize.hvQiTouXianChu = m_CHK_bQiTouXianChu;
		//ComPareChange(m_pOwner->m_tStandardSize.hvZhiDaiGray.I(), m_EDT_nZhiDaiThresh, "纸带灰度");
		m_pOwner->m_tStandardSize.hvZhiDaiGray = m_EDT_nZhiDaiThresh;
		//ComPareChange(m_pOwner->m_tStandardSize.hv_CoatType.I(), m_COM_nCoatType, "涂布类型");
		m_pOwner->m_tStandardSize.hv_CoatType = m_COM_nCoatType;

		m_pOwner->m_tStandardSize.dCoatWidth = m_EDT_dCoatWidth;
		m_pOwner->m_tStandardSize.dTotalCoatWidth = m_EDT_dTotalCoatWidth;
		m_pOwner->m_tStandardSize.dHeadCuoWei = m_EDT_dHeadCuoWei;
		m_pOwner->m_tStandardSize.dTailCuoWei = m_EDT_dTailCuoWei;
		
		HTuple hvJXSTD;//用于查找间歇的标准长度
		double dLongJXHeight = max(m_EDT_dLongJXHeight, m_EDT_dShortJXHeight);
		double dShortJXHeight = min(m_EDT_dLongJXHeight, m_EDT_dShortJXHeight);
		double dLongCoat = max(m_EDT_dLongCoat, m_EDT_dShortCoat);
		double dShortCoat = min(m_EDT_dLongCoat, m_EDT_dShortCoat);
		//前相机为反面
		if (g_tSystemSet.nCameraModel == 0)//前相机为定位相机
		{
			//反面间歇高度
			m_pOwner->m_tStandardSize.dDownIntermissionHeight = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dLongJXHeight : dShortJXHeight;
			m_pOwner->m_tStandardSize.dBHJXLength2 = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dShortCoat : dLongCoat;
			//正面间歇高度
			m_pOwner->m_tStandardSize.dUpIntermissionHeight = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dShortJXHeight : dLongJXHeight;
			m_pOwner->m_tStandardSize.dBHJXTotalLength = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dLongCoat : dShortCoat;
			hvJXSTD = m_pOwner->m_tStandardSize.dDownIntermissionHeight;
		}
		else
		{
			//正面间歇高度
			m_pOwner->m_tStandardSize.dUpIntermissionHeight = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dLongJXHeight : dShortJXHeight;
			m_pOwner->m_tStandardSize.dBHJXTotalLength = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dShortCoat : dLongCoat;
			//反面间歇高度
			m_pOwner->m_tStandardSize.dDownIntermissionHeight = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dShortJXHeight : dLongJXHeight;
			m_pOwner->m_tStandardSize.dBHJXLength2 = m_pOwner->m_tStandardSize.hvChangMoZaiShang == 0 ? dLongCoat : dShortCoat;
			hvJXSTD = m_pOwner->m_tStandardSize.dUpIntermissionHeight;
		}
		
		int nTripNum = m_pOwner->m_tStandardSize.hvTripNum.I();
		g_pCommunicate->DoSomething("SETPARAM", "TRIPNUM", &nTripNum);
		g_vec_Algorithm[0]->DoSomething("SETPARAM", "tSizeParam", &m_pOwner->m_tStandardSize);
		g_vec_Algorithm[1]->DoSomething("SETPARAM", "tSizeParam", &m_pOwner->m_tStandardSize);
		BOOL bReFindLine = TRUE;
		g_vec_Algorithm[0]->DoSomething("SETPARAM", "ReFindLine", &bReFindLine);
		g_vec_Algorithm[1]->DoSomething("SETPARAM", "ReFindLine", &bReFindLine);

		HTuple hvInfo;//间歇查找设置
		hvInfo.Append(m_pOwner->m_tStandardSize.hvQiTouXianChu).Append(hvJXSTD);
		g_vec_FindMark[0]->DoSomething("SETPARAM", "LocatePositionY", &hvInfo);
		g_vec_FindMark[1]->DoSomething("SETPARAM", "LocatePositionY", &hvInfo);

		g_Jugde[0].WriteParam();
		g_Jugde[1].WriteParam();

		g_ProductParam.emCoatType = (e_CoatType)m_pOwner->m_tStandardSize.hv_CoatType.I();
		g_ProductParam.emProductType = (e_ProductType)m_pOwner->m_tStandardSize.hv_ProductType.I();

		//参数保存至文件
		if (FALSE)
		{
			AfxMessageBox("修改条数后需要重启软件,请保存软件重启");
			((CSeekersDlg*)g_pMainDialog)->PostMessageA(WM_RESTARTPROCESS, 0, 0);
		}
	}
	else
	{
		WriteProductParam(m_strCurrentProductName);
	}
	CString strMessage;
	strMessage.Format("Model:%s 产品参数保存成功！", m_strCurrentProductName);
	AfxMessageBox(strMessage);
}


//void CDlgProductManager::OnLvnItemchangedListProduct(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
//	if (0 == pos)//如果没选中任何品种,返回
//	{
//		return;
//	}
//	m_strCurrentProductName = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos), 1);
//	m_nCurrentProductSN = (int)pos;
//	SetProductParam();
//	*pResult = 0;
//}


void CDlgProductManager::OnNMClickListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_listProduct.GetFirstSelectedItemPosition();
	if (0 == pos)//如果没选中任何品种,返回
	{
		return;
	}
	m_strCurrentProductName = m_listProduct.GetItemText(m_listProduct.GetNextSelectedItem(pos), 1);
	m_nCurrentProductSN = (int)pos;
	CString strInfo;
	strInfo.Format("当前配方:%s", m_strCurrentProductName);
	m_STATIC_chCurrentProduct.SetWindowText(strInfo);
	SetProductParam();
	*pResult = 0;
}
