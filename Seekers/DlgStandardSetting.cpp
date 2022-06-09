// DlgStandardSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Seekers.h"
#include "DlgStandardSetting.h"
#include "afxdialogex.h"


// CDlgStandardSetting 对话框

IMPLEMENT_DYNAMIC(CDlgStandardSetting, CDialogEx)

CDlgStandardSetting::CDlgStandardSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStandardSetting::IDD, pParent)
	, m_EDT_dAllLength(0)
	, m_EDT_dAllLengthDev(0)
	, m_EDT_dCoatWidthDev(0)
	, m_EDT_dBaibianDev(0)
	, m_EDT_dLocateMissingDev(0)
	, m_CHECK_bEnableCoatWidth(FALSE)
	, m_CHECK_bEnableLocateMissing(FALSE)
	, m_EDT_dCoatWidth(0)
	, m_EDT_dLocateMissing(0)
	, m_CHK_bBHJXSumLabel(FALSE)
	, m_CHK_bUpJXLabel(FALSE)
	, m_CHK_bDownJXLabel(FALSE)
	, m_CHK_bEnableABMiss(FALSE)
	, m_EDT_dABMissValue(0)
	, m_EDT_dABMissValueDev(0)
	, m_CHK_bABMissLabel(FALSE)
	, m_EDT_dBHJXMeanLengthDev(0)
	, m_EDT_dBHJXMeanLength(0)
	, m_CHK_bEnableBHJXMeanLength(FALSE)
	, m_CHK_bEnableCuoWei(FALSE)
	, m_EDT_dCuoWeiMax(0)
	, m_EDT_nTrip(0)
	, m_COM_nExistJXType(0)
	, m_COM_nGongYiLeiXing(0)
	, m_CHK_bChangMoZaiShang(FALSE)
	, m_CHK_bQiTouXianChu(FALSE)
	, m_EDT_nZhiDaiThresh(0)
	, m_EDT_dTotalCoatWidth(0)
	, m_EDT_dTotalCoatWidthDev(0)
	, m_EDT_dBHJXLength2(0)
	, m_EDT_dBHJXLengthDev2(0)
	, m_EDT_dHeadCuoWei(0)
	, m_EDT_dHeadCuoWeiDev(0)
	, m_EDT_dTailCuoWei(0)
	, m_EDT_dTailCuoWeiDev(0)
	, m_CHK_BHJXLabel2(FALSE)
	, m_CHK_bEnableBHJXLength2(FALSE)
	, m_CHK_bEnableTotalWidth(FALSE)
	, m_CHK_bEnableHeadCuoWei(FALSE)
	, m_CHK_bEnableTailCuoWei(FALSE)
	, m_CHK_bHeadCuoWeiLabel(FALSE)
	, m_CHK_bTailCuoWeiLabel(FALSE)
	, m_CHK_bToatlWidthLabel(FALSE)
{
	m_pOwner = NULL;
	m_EDT_dMaxHeightDistance = 0.0;
	m_EDT_dBHJXLength = 0.0;
	m_EDT_dBHJXLengthDev = 0.0;
	m_EDT_dUpInterMissionHeight = 0.0;
	m_EDT_dUpInterMissionHeightDev = 0.0;
	m_EDT_dDownInterMissionHeight = 0.0;
	m_EDT_dDownInterMissionHeightDev = 0.0;
	m_EDT_dCuoWeiMax = 0.0;
	m_COM_nCaoWeiChoice = 0;
	m_COM_nCoatType = 0;
	m_COM_nProductJiXi = 0;
	m_COM_nOapChoice = 0;
	//  m_EDT_dCaoWeiHeightDev = 0.0;
	//  m_EDT_dCaoWeiHeight = 0.0;
	//  m_EDT_dCaoWeiHeightDev = 0.0;
	//  m_EDT_dCaoWeiMDDev = 0.0;
	//  m_EDT_dCaoWeiTDDev = 0.0;
	//  m_EDT_dCaoWeiWidth = 0.0;
	//  m_EDT_dCaoWeiWidthDev = 0.0;
}

CDlgStandardSetting::~CDlgStandardSetting()
{


}

void CDlgStandardSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ALLLENGTH, m_EDT_dAllLength);
	DDX_Text(pDX, IDC_EDIT_ALLLENGTHDEV, m_EDT_dAllLengthDev);
	DDX_Text(pDX, IDC_EDIT_dCoatWidthDev, m_EDT_dCoatWidthDev);
	DDX_Text(pDX, IDC_EDIT_dLocateMissingDev, m_EDT_dLocateMissingDev);
	DDX_Check(pDX, IDC_CHECK_bEnableCoatWidth, m_CHECK_bEnableCoatWidth);
	DDX_Check(pDX, IDC_CHECK_bEnableLocateMissing, m_CHECK_bEnableLocateMissing);
	DDX_Text(pDX, IDC_EDIT_dCoatWidth, m_EDT_dCoatWidth);
	DDX_Text(pDX, IDC_EDIT_dLocateMissing, m_EDT_dLocateMissing);


	DDX_Check(pDX, IDC_CHK_BENABLEALLLENGTH, m_CHK_bEnableAllLength);


	DDX_Check(pDX, IDC_CHK_BENABLEBHJX, m_CHK_bBHJXLength);
	DDX_Text(pDX, IDC_EDT_BHJXTOTALLENGTH, m_EDT_dBHJXLength);
	DDX_Text(pDX, IDC_EDT_BHJXTOTALLENGTHDEV, m_EDT_dBHJXLengthDev);

	DDX_Check(pDX, IDC_CHK_BENABLEUPIMHEIGHT, m_CHK_bEnableUpImHeight);
	DDX_Text(pDX, IDC_EDT_UPINTERMISSIONHEIGHT, m_EDT_dUpInterMissionHeight);
	DDX_Text(pDX, IDC_EDT_UPINTERMISSIONHEIGHTDEV, m_EDT_dUpInterMissionHeightDev);

	DDX_Text(pDX, IDC_EDT_DOWNINTEMISSIONHEIGHT, m_EDT_dDownInterMissionHeight);
	DDX_Text(pDX, IDC_EDT_DOWNINTERMISSIONHEIGHTDEV, m_EDT_dDownInterMissionHeightDev);
	DDX_Check(pDX, IDC_CHK_BENABLEDOWNIMHEIGHT, m_CHK_bEnableDownInterMissionHeight);

	DDX_Check(pDX, IDC_CHK_BHJXLABEL, m_CHK_bBHJXSumLabel);
	DDX_Check(pDX, IDC_CHK_UPJXLABEL, m_CHK_bUpJXLabel);
	DDX_Check(pDX, IDC_CHK_DOWNJXLABEL, m_CHK_bDownJXLabel);
	DDX_Check(pDX, IDC_CHK_BENABLEABMISS, m_CHK_bEnableABMiss);
	DDX_Text(pDX, IDC_EDT_ABMISSVALUE, m_EDT_dABMissValue);
	DDX_Text(pDX, IDC_EDT_ABMISSVALUEDEV, m_EDT_dABMissValueDev);
	DDX_Check(pDX, IDC_CHK_ABLABEL, m_CHK_bABMissLabel);
	DDX_Check(pDX, IDC_CHK_WIDTH, m_CHK_bCoatWidthLabel);
	DDX_Check(pDX, IDC_CHK_UPAT9LABEL, m_CHK_bUpAt9Label);
	DDX_Check(pDX, IDC_CHK_TOTALLENGTHLABEL, m_CHK_bTotalLengthLabel);
	DDX_Text(pDX, IDC_EDT_BHJXMEANLENGTHDDEV, m_EDT_dBHJXMeanLengthDev);
	DDX_Text(pDX, IDC_EDT_BHJXMEANLENGTH, m_EDT_dBHJXMeanLength);
	DDX_Check(pDX, IDC_CHK_BHJXMEANLENGTH, m_CHK_bEnableBHJXMeanLength);
	DDX_Check(pDX, IDC_CHK_ENABLECUOWEI, m_CHK_bEnableCuoWei);
	DDX_Text(pDX, IDC_EDT_CUOWEIMAX, m_EDT_dCuoWeiMax);
	//  DDX_Check(pDX, IDC_CHK_OVEREALENGTHLABEL, m_CHK_bOverEALenthLabel);
	DDX_Text(pDX, IDC_EDT_TRIPNUM, m_EDT_nTrip);
	DDV_MinMaxInt(pDX, m_EDT_nTrip, 1, 12);
	DDX_CBIndex(pDX, IDC_COM_CAOWEICHOICE, m_COM_nCaoWeiChoice);
	DDX_CBIndex(pDX, IDC_COM_COATTYPE, m_COM_nCoatType);
	DDX_CBIndex(pDX, IDC_COM_JIXING, m_COM_nProductJiXi);
	DDX_CBIndex(pDX, IDC_COM_OAPCOHICE, m_COM_nOapChoice);
	//  DDX_Control(pDX, IDC_LST_CAOWEI, m_LST_CCaoWei);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIHEIGHT, m_EDT_dCaoWeiHeightDev);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIHEIGHT, m_EDT_dCaoWeiHeight);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIHEIGHTDEV, m_EDT_dCaoWeiHeightDev);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIMDDEV, m_EDT_dCaoWeiMDDev);
	//  DDX_Text(pDX, IDC_EDT_CAOWEITDDEV, m_EDT_dCaoWeiTDDev);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIWIDTH, m_EDT_dCaoWeiWidth);
	//  DDX_Text(pDX, IDC_EDT_CAOWEIWIDTHDEV, m_EDT_dCaoWeiWidthDev);
	DDX_CBIndex(pDX, IDC_COM_EXISTJXTYPE, m_COM_nExistJXType);
	//  DDX_Text(pDX, IDC_EDT_CAOWEINUM, m_EDT_nCaoWeiNum);
	DDX_Control(pDX, IDC_LST_CAOWEIPARAM0, m_LST_CaoWeiParam0);
	DDX_Control(pDX, IDC_LST_CAOWEIPARAM1, m_LST_CaoWeiParam1);
	DDX_CBIndex(pDX, IDC_COM_GONGYILEIXING, m_COM_nGongYiLeiXing);
	DDX_Check(pDX, IDC_CHK_CHANGMOZAISHANG, m_CHK_bChangMoZaiShang);
	DDX_Check(pDX, IDC_CHK_QITOUXIANCHU, m_CHK_bQiTouXianChu);
	DDX_Text(pDX, IDC_EDT_ZHIDAITHRESH, m_EDT_nZhiDaiThresh);
	DDV_MinMaxInt(pDX, m_EDT_nZhiDaiThresh, 50, 255);
	DDX_Text(pDX, IDC_EDIT_dTotalCoatWidth, m_EDT_dTotalCoatWidth);
	DDX_Text(pDX, IDC_EDIT_dTotalCoatWidthDev, m_EDT_dTotalCoatWidthDev);
	DDX_Text(pDX, IDC_EDT_BHJXTOTALLENGTH2, m_EDT_dBHJXLength2);
	DDX_Text(pDX, IDC_EDT_BHJXTOTALLENGTHDEV2, m_EDT_dBHJXLengthDev2);
	DDX_Text(pDX, IDC_EDT_HEADCUOWEI, m_EDT_dHeadCuoWei);
	DDX_Text(pDX, IDC_EDT_HEADCUOWEIDEV, m_EDT_dHeadCuoWeiDev);
	DDX_Text(pDX, IDC_EDT_TAILCUOWEI, m_EDT_dTailCuoWei);
	DDX_Text(pDX, IDC_EDT_TAILCUOWEIDEV, m_EDT_dTailCuoWeiDev);
	DDX_Check(pDX, IDC_CHK_BHJXLABEL2, m_CHK_BHJXLabel2);
	DDX_Check(pDX, IDC_CHK_BENABLEBHJX2, m_CHK_bEnableBHJXLength2);
	DDX_Check(pDX, IDC_CHECK_bEnableTotalCoatWidth, m_CHK_bEnableTotalWidth);
	DDX_Check(pDX, IDC_CHK_BENABLEHEADCUOWEI, m_CHK_bEnableHeadCuoWei);
	DDX_Check(pDX, IDC_CHK_BENABLETAILCUOWEI, m_CHK_bEnableTailCuoWei);
	DDX_Check(pDX, IDC_CHK_HEADCUOWEILABEL, m_CHK_bHeadCuoWeiLabel);
	DDX_Check(pDX, IDC_CHK_TAILCUOWEILABEL, m_CHK_bTailCuoWeiLabel);
	DDX_Check(pDX, IDC_CHK_TOTALWIDTHLABEL, m_CHK_bToatlWidthLabel);
}


BEGIN_MESSAGE_MAP(CDlgStandardSetting, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_RETURN, &CDlgStandardSetting::OnBnClickedBtnReturn)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COM_GONGYILEIXING, &CDlgStandardSetting::OnSelchangeComGongyileixing)
//	ON_BN_CLICKED(IDC_CHK_CHANGMOZAISHANG, &CDlgStandardSetting::OnBnClickedChkChangmozaishang)
END_MESSAGE_MAP()


// CDlgStandardSetting 消息处理程序
void CDlgStandardSetting::GetLSTValue(CAOWEISIZE& tCaoWeiSize,CReportCtrlExt& LstValue,int nSN)
{



	int k=0;
	CString strValue;
	CString strName;
	if (nSN==0)
	{
		strName="正面";
	}else
	{
		strName="反面";
	}
	for (int i=0;i<LstValue.GetItemCount();i++)
	{
		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiHeight.D(),atof(strValue),strName+"槽位高度");
		tCaoWeiSize.hvCaoWeiHeight=atof(strValue);
	    

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiHeightDev.D(),atof(strValue),strName+"槽位高度公差");
		tCaoWeiSize.hvCaoWeiHeightDev=atof(strValue);
	

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiWidth.D(),atof(strValue),strName+"槽位宽度");
		tCaoWeiSize.hvCaoWeiWidth=atof(strValue);

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiWidthDev.D(),atof(strValue),strName+"槽位宽度公差");
		tCaoWeiSize.hvCaoWeiWidthDev=atof(strValue);

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiMD.D(),atof(strValue),strName+"槽位MD");
		tCaoWeiSize.hvCaoWeiMD=atof(strValue);

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiMDDev.D(),atof(strValue),strName+"槽位MD公差");
		tCaoWeiSize.hvCaoWeiMDDev=atof(strValue);

		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiTDDev.D(),atof(strValue),strName+"槽位TD公差");
		tCaoWeiSize.hvCaoWeiTDDev=atof(strValue);


		strValue=LstValue.GetItemText(i,++k);
		ComPareChange(tCaoWeiSize.hvCaoWeiMDCuoWeiDev.D(),atof(strValue),strName+"槽位MD错位公差");
		tCaoWeiSize.hvCaoWeiMDCuoWeiDev=atof(strValue);


		strValue=LstValue.GetItemText(i,++k);
		tCaoWeiSize.hvCaoWeiTDCuoWeiDev=atof(strValue);

		HTuple hvTempCaoWeiTDTuple;
		hvTempCaoWeiTDTuple=tCaoWeiSize.hvCaoWeiTDTuple;
		tCaoWeiSize.hvCaoWeiTDTuple.Clear();
		CString strTDName;
		double dLastValue=0;
		for (int j=0;j<m_EDT_nTrip;j++)
		{
			strTDName.Format("%sTD%d标准值",strName,j+1);
			int nColumnCount=++k;
			int nn=LstValue.GetColumnCount();
			if (nColumnCount<LstValue.GetColumnCount())
			{
				strValue=LstValue.GetItemText(i,nColumnCount);
				dLastValue=hvTempCaoWeiTDTuple[j].D();
			}else
			{
				dLastValue=0;
				strValue="0";
			}

			ComPareChange(dLastValue,atof(strValue),strTDName);


			tCaoWeiSize.hvCaoWeiTDTuple.Append(atof(strValue));
		}



	}
}

void CDlgStandardSetting::GetLSTFoilAndCoatValue(STANDARDSIZE& tCoatAndFoilSize, CReportCtrlExt& LstValue, int nSN)
{
	int k = 0;
	CString strValue;
	CString strName;
	if (nSN == 0)
	{
		strName = "正面尺寸规格(mm)";
	}
	else
	{
		strName = "反面尺寸规格(mm)";
	}
	for (int i = 0; i < LstValue.GetItemCount(); i++)
	{
		HTuple hvCoatWidthTuple, hvCoatWidthTupleDev,hvFoilWidthTuple, hvFoilWidthTupleDev;
		hvCoatWidthTuple = tCoatAndFoilSize.dCoatWidth;
		hvCoatWidthTupleDev= tCoatAndFoilSize.dCoatWidthDev;
		hvFoilWidthTuple = tCoatAndFoilSize.dFoilWidth;
		hvFoilWidthTupleDev = tCoatAndFoilSize.dFoilWidthDev;

		tCoatAndFoilSize.dCoatWidth.Clear();
		tCoatAndFoilSize.dCoatWidthDev.Clear();
		tCoatAndFoilSize.dFoilWidth.Clear();
		tCoatAndFoilSize.dFoilWidthDev.Clear();

		CString strTDName;
		double dLastValue = 0;
		for (int j = 0; j < MAX(1,m_EDT_nTrip/2); j++)
		{
			strTDName.Format("涂膜%d标准值", j);
			
			if (k < LstValue.GetColumnCount()&&j< hvCoatWidthTuple.Length())
			{
				k++;
				strValue = LstValue.GetItemText(i, k);
				dLastValue = hvCoatWidthTuple[j].D();
			}
			else
			{
				dLastValue = 0;
				strValue = "0";
			}
			ComPareChange(dLastValue, atof(strValue), strTDName);
			tCoatAndFoilSize.dCoatWidth.Append(atof(strValue));
			//涂布公差
			strTDName.Format("涂膜%d公差", j);
			if (k < LstValue.GetColumnCount() && j < hvCoatWidthTupleDev.Length())
			{
				k++;
				strValue = LstValue.GetItemText(i, k);
				dLastValue = hvCoatWidthTupleDev[j].D();
			}
			else
			{
				dLastValue = 0;
				strValue = "0";
			}
			ComPareChange(dLastValue, atof(strValue), strTDName);
			tCoatAndFoilSize.dCoatWidthDev.Append(atof(strValue));
		}

		for (int j = 0; j < MAX(2,(m_EDT_nTrip / 2)+1); j++)
		{
			strTDName.Format("极耳%d标准值", j);
			if (k < LstValue.GetColumnCount()&&j< hvFoilWidthTuple.Length())
			{
				k++;
				strValue = LstValue.GetItemText(i, k);
				dLastValue = hvFoilWidthTuple[j].D();
			}
			else
			{
				dLastValue = 0;
				strValue = "0";
			}
			ComPareChange(dLastValue, atof(strValue), strTDName);
			tCoatAndFoilSize.dFoilWidth.Append(atof(strValue));
			//极耳公差
			strTDName.Format("极耳%d公差", j);
			if (k < LstValue.GetColumnCount()&& j < hvFoilWidthTupleDev.Length())
			{
				k++;
				strValue = LstValue.GetItemText(i, k);
				dLastValue = hvFoilWidthTupleDev[j].D();
			}
			else
			{
				dLastValue = 0;
				strValue = "0";
			}
			ComPareChange(dLastValue, atof(strValue), strTDName);
			tCoatAndFoilSize.dFoilWidthDev.Append(atof(strValue));
		}
	}
}


void CDlgStandardSetting::InsertLSTValue(CAOWEISIZE& tCaoWeiSize,CReportCtrlExt& LstValue,int nSN)
{
	LstValue.DeleteAllItems();
	LstValue.DeleteAllColumns();
	
	CRect rcRect;
	LstValue.SetEditable();
	LstValue.SetGridLines();
	LstValue.GetWindowRect(rcRect);
	int k=0;
	int nColumnNum= m_EDT_nTrip +10;
	int nWidth=rcRect.Width()*1/nColumnNum - 3;
	CString strName;
	if (nSN==0)
	{
		LstValue.InsertColumn(k,"正面排数(mm)",LVCFMT_LEFT,nWidth);
	}else
	{
		LstValue.InsertColumn(k,"反面排数(mm)",LVCFMT_LEFT,nWidth);
	}

	LstValue.InsertColumn(++k,"高度(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"高度公差(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"宽度(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"宽度公差(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"MD(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"MD公差(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"TD公差(mm)",LVCFMT_LEFT,nWidth);

	LstValue.InsertColumn(++k,"MD错位公差(mm)",LVCFMT_LEFT,nWidth);
	LstValue.InsertColumn(++k,"TD错位公差(mm)",LVCFMT_LEFT,nWidth);

	CString strTD;
	for (int i=0;i<m_EDT_nTrip;i++)
	{
		strTD.Format("TD%d(mm)",i+1);
		LstValue.InsertColumn(++k,strTD,LVCFMT_LEFT,nWidth);
	}


	k=0;
	CString strValue;
	LstValue.InsertItem(0,"第一排");
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiHeight.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiHeightDev.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiWidth.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiWidthDev.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiMD.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiMDDev.D());
	LstValue.SetItemText(0,++k,strValue);
	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiTDDev.D());
	LstValue.SetItemText(0,++k,strValue);

	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiMDCuoWeiDev.D());
	LstValue.SetItemText(0,++k,strValue);


	strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiTDCuoWeiDev.D());
	LstValue.SetItemText(0,++k,strValue);

	for (int i=0;i<m_EDT_nTrip;i++)
	{

		if (i<tCaoWeiSize.hvCaoWeiTDTuple.Length())
		{
			strValue.Format("%.2f",tCaoWeiSize.hvCaoWeiTDTuple[i].D());
		}else
		{
			strValue.Format("%.2f",0);
		}

		LstValue.SetItemText(0,++k,strValue);
	}






}

void CDlgStandardSetting::InsertLSTFoilAndCoatValue(STANDARDSIZE& tCoatAndFoilSize, CReportCtrlExt& LstValue, int nSN)
{
	LstValue.DeleteAllItems();
	LstValue.DeleteAllColumns();
	CRect rcRect;
	LstValue.SetEditable();
	LstValue.SetGridLines();
	LstValue.GetWindowRect(rcRect);
	int k = 0;
	int nColumnNum = (m_EDT_nTrip + 1)*2;
	int nWidth = rcRect.Width() * 1 / nColumnNum - 3;
	CString strName;
	if (nSN == 0)
	{
		LstValue.InsertColumn(k, "正面尺寸规格(mm)", LVCFMT_LEFT, nWidth);
	}
	else
	{
		LstValue.InsertColumn(k, "反面尺寸规格(mm)", LVCFMT_LEFT, nWidth);
	}

	CString strTD;
	for (int i = 0; i < MAX(1,m_EDT_nTrip/2); i++)
	{
		strTD.Format("涂布%d宽度(mm)", i+1);
		LstValue.InsertColumn(++k, strTD, LVCFMT_LEFT, nWidth);
		strTD.Format("涂布%d宽度公差(mm)", i+1);
		LstValue.InsertColumn(++k, strTD, LVCFMT_LEFT, nWidth);
	}

	for (int i = 0; i <MAX(2,( m_EDT_nTrip / 2)+1); i++)
	{
		strTD.Format("极耳%d宽度(mm)", i+1);
		LstValue.InsertColumn(++k, strTD, LVCFMT_LEFT, nWidth);
		strTD.Format("极耳%d宽度公差(mm)", i+1);
		LstValue.InsertColumn(++k, strTD, LVCFMT_LEFT, nWidth);
	}

	k = 0;
	CString strValue;
	LstValue.InsertItem(0, "规格参数");

	for (int i = 0; i < MAX(1,m_EDT_nTrip / 2); i++)
	{
		strValue.Format("%.2f", tCoatAndFoilSize.dCoatWidth[i].D());
		LstValue.SetItemText(0, ++k, strValue);
		strValue.Format("%.2f", tCoatAndFoilSize.dCoatWidthDev[i].D());
		LstValue.SetItemText(0, ++k, strValue);
	}

	for (int i = 0; i < MAX(2,(m_EDT_nTrip / 2) + 1); i++)
	{
		strValue.Format("%.2f", tCoatAndFoilSize.dFoilWidth[i].D());
		LstValue.SetItemText(0, ++k, strValue);
		strValue.Format("%.2f", tCoatAndFoilSize.dFoilWidthDev[i].D());
		LstValue.SetItemText(0, ++k, strValue);
	}
}


void CDlgStandardSetting::SetValue()
{
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
    m_CHK_bEnableAllLength= m_pOwner->m_tStandardSize.bEnableTotalLength;
	m_EDT_dAllLength = m_pOwner->m_tStandardSize.dTotalLength;
	m_EDT_dAllLengthDev = m_pOwner->m_tStandardSize.dTotalLengthDev;


	m_CHECK_bEnableLocateMissing = m_pOwner->m_tStandardSize.bEnableLocateMissing;
    m_EDT_dLocateMissing = m_pOwner->m_tStandardSize.dLocateMissing;
	m_EDT_dLocateMissingDev = m_pOwner->m_tStandardSize.dLocateMissingDev;

	m_CHECK_bEnableCoatWidth = m_pOwner->m_tStandardSize.bEnableCoatWidth;
	m_EDT_dCoatWidth = m_pOwner->m_tStandardSize.dCoatWidth;
	m_EDT_dCoatWidthDev = m_pOwner->m_tStandardSize.dCoatWidthDev;


	m_CHK_bTotalLengthLabel=m_pOwner->m_tStandardSize.bEnableTotalLengthLabel;//1
	m_CHK_bCoatWidthLabel=m_pOwner->m_tStandardSize.bEnableCoatWidthLabel;//1
	m_CHK_bUpAt9Label=m_pOwner->m_tStandardSize.bEnableLocateMissingLabel;//1


	///间歇////

	m_CHK_bBHJXLength=m_pOwner->m_tStandardSize.bEnableBHJXTotalLength;
	m_EDT_dBHJXLength=m_pOwner->m_tStandardSize.dBHJXTotalLength;
	m_EDT_dBHJXLengthDev=m_pOwner->m_tStandardSize.dBHJXTotalLengthDev;
	m_CHK_bBHJXSumLabel=m_pOwner->m_tStandardSize.bEnableBHJXLabel;



	m_CHK_bEnableUpImHeight=m_pOwner->m_tStandardSize.bEnableUpImHeight;
	m_EDT_dUpInterMissionHeight=m_pOwner->m_tStandardSize.dUpIntermissionHeight;
	m_EDT_dUpInterMissionHeightDev=m_pOwner->m_tStandardSize.dUpIntermissionHeightDev;
	m_CHK_bUpJXLabel=m_pOwner->m_tStandardSize.bEnableUpIntermissionLabel;



	m_CHK_bEnableDownInterMissionHeight=m_pOwner->m_tStandardSize.bEnableDownImHeight;
	m_EDT_dDownInterMissionHeight=m_pOwner->m_tStandardSize.dDownIntermissionHeight;
	m_EDT_dDownInterMissionHeightDev=m_pOwner->m_tStandardSize.dDownIntermissionHeightDev;
	m_CHK_bDownJXLabel=m_pOwner->m_tStandardSize.bEnableDownIntermissionLabel;


	///新加的

	 m_CHK_bEnableTotalWidth = m_pOwner->m_tStandardSize.bEnableTotalWidth;
	 m_CHK_bToatlWidthLabel = m_pOwner->m_tStandardSize.bToatlWidthLabel;
	
	 m_EDT_dTotalCoatWidth = m_pOwner->m_tStandardSize.dTotalCoatWidth;
	 m_EDT_dTotalCoatWidthDev = m_pOwner->m_tStandardSize.dTotalCoatWidthDev;


	 m_CHK_BHJXLabel2 = m_pOwner->m_tStandardSize.bBHJXLabel2;
	 m_CHK_bEnableBHJXLength2 = m_pOwner->m_tStandardSize.bEnableBHJXLength2;
	 m_EDT_dBHJXLength2 = m_pOwner->m_tStandardSize.dBHJXLength2;
	 m_EDT_dBHJXLengthDev2 = m_pOwner->m_tStandardSize.dBHJXLengthDev2;

	 m_CHK_bEnableHeadCuoWei = m_pOwner->m_tStandardSize.bEnableHeadCuoWei;
	 m_CHK_bHeadCuoWeiLabel = m_pOwner->m_tStandardSize.bHeadCuoWeiLabel;
	 m_EDT_dHeadCuoWei = m_pOwner->m_tStandardSize.dHeadCuoWei;
	 m_EDT_dHeadCuoWeiDev = m_pOwner->m_tStandardSize.dHeadCuoWeiDev;

	


	 m_CHK_bTailCuoWeiLabel = m_pOwner->m_tStandardSize.bTailCuoWeiLabel;
	 m_CHK_bEnableTailCuoWei = m_pOwner->m_tStandardSize.bEnableTailCuoWei;
	 m_EDT_dTailCuoWei = m_pOwner->m_tStandardSize.dTailCuoWei;
	 m_EDT_dTailCuoWeiDev = m_pOwner->m_tStandardSize.dTailCuoWeiDev;




	
	














	m_CHK_bEnableABMiss=m_pOwner->m_tStandardSize.bEnableABMissValue;
	m_EDT_dABMissValue=m_pOwner->m_tStandardSize.dABMissValue;
	m_EDT_dABMissValueDev=m_pOwner->m_tStandardSize.dABMissValueDev;
	m_CHK_bABMissLabel=m_pOwner->m_tStandardSize.bEnableABMissValueLabel;

	


	m_CHK_bEnableCuoWei=m_pOwner->m_tStandardSize.bEnableCuoWei;
	m_EDT_dCuoWeiMax=m_pOwner->m_tStandardSize.dCuoWeiMax;

	//InsertLSTValue(m_pOwner->m_tStandardSize.tUpCaoWeiParam,m_LST_CaoWeiParam0);
	//InsertLSTValue(m_pOwner->m_tStandardSize.tDownCaoWeiParam,m_LST_CaoWeiParam1,1);

	UpdateData(FALSE);
}

void CDlgStandardSetting::ComPareChange(int LastValue,int  CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("%d侧打标规格检测参数%s由%d更改为%d",m_pOwner->m_tInit.nSN,strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}
void CDlgStandardSetting::ComPareChange(double LastValue,double CurrentValue,CString strName)
{
	CString strMessage;
	if (LastValue!=CurrentValue)
	{
		strMessage.Format("%d侧打标规格检测参数%s由%.2f更改为%.2f",m_pOwner->m_tInit.nSN,strName,LastValue,CurrentValue);
		g_pLog->DoSomething("OPERATERLOG",(void*)(LPCSTR)strMessage,NULL);
	}

}



void CDlgStandardSetting::GetValue()
{
	m_LST_CaoWeiParam0.EndEdit(TRUE);
	m_LST_CaoWeiParam1.EndEdit(TRUE);
	UpdateData(TRUE);


	ComPareChange(m_pOwner->m_tStandardSize.bEnableTotalLength.I(),m_CHK_bEnableAllLength,"总长开关");
	m_pOwner->m_tStandardSize.bEnableTotalLength = m_CHK_bEnableAllLength;

	ComPareChange(m_pOwner->m_tStandardSize.dTotalLength.D(),m_EDT_dAllLength,"总长");
	m_pOwner->m_tStandardSize.dTotalLength = m_EDT_dAllLength;

	ComPareChange(m_pOwner->m_tStandardSize.dTotalLengthDev.D(),m_EDT_dAllLengthDev,"总长公差");
	m_pOwner->m_tStandardSize.dTotalLengthDev = m_EDT_dAllLengthDev;
	ComPareChange(m_pOwner->m_tStandardSize.dCoatWidthDev.D(),m_EDT_dCoatWidthDev,"涂布宽度公差");
	m_pOwner->m_tStandardSize.dCoatWidthDev = m_EDT_dCoatWidthDev;

	ComPareChange(m_pOwner->m_tStandardSize.dLocateMissingDev.D(),m_EDT_dLocateMissingDev,"上料高度公差");
	m_pOwner->m_tStandardSize.dLocateMissingDev = m_EDT_dLocateMissingDev;

	ComPareChange(m_pOwner->m_tStandardSize.bEnableCoatWidth.I(),m_CHECK_bEnableCoatWidth,"涂布宽度开关");
	m_pOwner->m_tStandardSize.bEnableCoatWidth = m_CHECK_bEnableCoatWidth;
	ComPareChange(m_pOwner->m_tStandardSize.bEnableLocateMissing.I(),m_CHECK_bEnableLocateMissing,"上料高度开关");
	m_pOwner->m_tStandardSize.bEnableLocateMissing = m_CHECK_bEnableLocateMissing;
	ComPareChange(m_pOwner->m_tStandardSize.dCoatWidth.D(),m_EDT_dCoatWidth,"涂布宽度标准值");
	m_pOwner->m_tStandardSize.dCoatWidth  = m_EDT_dCoatWidth;


	ComPareChange(m_pOwner->m_tStandardSize.hvTripNum.I(),m_EDT_nTrip,"产品条数");
    m_pOwner->m_tStandardSize.hvTripNum=m_EDT_nTrip;

	ComPareChange(m_pOwner->m_tStandardSize.hv_ProductType.I(),m_COM_nProductJiXi,"产品极性");
	m_pOwner->m_tStandardSize.hv_ProductType=m_COM_nProductJiXi;



	ComPareChange(m_pOwner->m_tStandardSize.hv_OAPType.I(),m_COM_nOapChoice,"OAP类型");
	m_pOwner->m_tStandardSize.hv_OAPType=m_COM_nOapChoice;

	ComPareChange(m_pOwner->m_tStandardSize.hv_CaoWeiType.I(),m_COM_nCaoWeiChoice,"槽位类型");
	m_pOwner->m_tStandardSize.hv_CaoWeiType=m_COM_nCaoWeiChoice;


	ComPareChange(m_pOwner->m_tStandardSize.hvExistJXType.I(),m_COM_nExistJXType,"间歇类型");
	m_pOwner->m_tStandardSize.hvExistJXType=m_COM_nExistJXType;


	ComPareChange(m_pOwner->m_tStandardSize.hvGongYiType.I(), m_COM_nGongYiLeiXing, "工艺类型");
	m_pOwner->m_tStandardSize.hvGongYiType = m_COM_nGongYiLeiXing;
	
	ComPareChange(m_pOwner->m_tStandardSize.hvChangMoZaiShang.I(), m_CHK_bChangMoZaiShang, "长膜在上");
	m_pOwner->m_tStandardSize.hvChangMoZaiShang = m_CHK_bChangMoZaiShang;

	ComPareChange(m_pOwner->m_tStandardSize.hvQiTouXianChu.I(), m_CHK_bQiTouXianChu, "齐头先出");
	m_pOwner->m_tStandardSize.hvQiTouXianChu = m_CHK_bQiTouXianChu;


	ComPareChange(m_pOwner->m_tStandardSize.hvZhiDaiGray.I(), m_EDT_nZhiDaiThresh, "纸带灰度");
	m_pOwner->m_tStandardSize.hvZhiDaiGray=m_EDT_nZhiDaiThresh;

	ComPareChange(m_pOwner->m_tStandardSize.hv_CoatType.I(),m_COM_nCoatType,"涂布类型");
	m_pOwner->m_tStandardSize.hv_CoatType=m_COM_nCoatType;

	ComPareChange(m_pOwner->m_tStandardSize.dLocateMissing.D(),m_EDT_dLocateMissing,"上料高度标准值");
	m_pOwner->m_tStandardSize.dLocateMissing = m_EDT_dLocateMissing;

	ComPareChange(m_pOwner->m_tStandardSize.bEnableBHJXTotalLength.I(),m_CHK_bBHJXLength,"总长(不含间歇)开关");
	m_pOwner->m_tStandardSize.bEnableBHJXTotalLength=m_CHK_bBHJXLength;

	ComPareChange(m_pOwner->m_tStandardSize.dBHJXTotalLength.D(),m_EDT_dBHJXLength,"总长(不含间歇)");
	m_pOwner->m_tStandardSize.dBHJXTotalLength=m_EDT_dBHJXLength;
	ComPareChange(m_pOwner->m_tStandardSize.dBHJXTotalLengthDev.D(),m_EDT_dBHJXLengthDev,"总长(不含间歇)公差");
	m_pOwner->m_tStandardSize.dBHJXTotalLengthDev=m_EDT_dBHJXLengthDev;


	ComPareChange(m_pOwner->m_tStandardSize.bEnableUpImHeight.I(),m_CHK_bEnableUpImHeight,"正面间歇开关");
	m_pOwner->m_tStandardSize.bEnableUpImHeight=m_CHK_bEnableUpImHeight;

	ComPareChange(m_pOwner->m_tStandardSize.dUpIntermissionHeight.D(),m_EDT_dUpInterMissionHeight,"正面间歇标准值");
	m_pOwner->m_tStandardSize.dUpIntermissionHeight=m_EDT_dUpInterMissionHeight;

	ComPareChange(m_pOwner->m_tStandardSize.dUpIntermissionHeightDev.D(),m_EDT_dUpInterMissionHeightDev,"正面间歇标准公差");
	m_pOwner->m_tStandardSize.dUpIntermissionHeightDev=m_EDT_dUpInterMissionHeightDev;

	ComPareChange(m_pOwner->m_tStandardSize.bEnableDownImHeight.I(),m_CHK_bEnableDownInterMissionHeight,"反面间歇开关");
	m_pOwner->m_tStandardSize.bEnableDownImHeight=m_CHK_bEnableDownInterMissionHeight;
	ComPareChange(m_pOwner->m_tStandardSize.dDownIntermissionHeight.D(),m_EDT_dDownInterMissionHeight,"反面间歇标准值");
	m_pOwner->m_tStandardSize.dDownIntermissionHeight=m_EDT_dDownInterMissionHeight;
	ComPareChange(m_pOwner->m_tStandardSize.dDownIntermissionHeightDev.D(),m_EDT_dDownInterMissionHeightDev,"反面间歇标准公差");
	m_pOwner->m_tStandardSize.dDownIntermissionHeightDev=m_EDT_dDownInterMissionHeightDev;

	ComPareChange(m_pOwner->m_tStandardSize.bEnableBHJXLabel.I(),m_CHK_bBHJXSumLabel,"不含间歇贴标开关");
	m_pOwner->m_tStandardSize.bEnableBHJXLabel=m_CHK_bBHJXSumLabel;
	
	ComPareChange(m_pOwner->m_tStandardSize.bEnableUpIntermissionLabel.I(),m_CHK_bUpJXLabel,"正面间歇贴标开关");
	m_pOwner->m_tStandardSize.bEnableUpIntermissionLabel=m_CHK_bUpJXLabel;
	ComPareChange(m_pOwner->m_tStandardSize.bEnableDownIntermissionLabel.I(),m_CHK_bDownJXLabel,"反面间歇贴标开关");
	m_pOwner->m_tStandardSize.bEnableDownIntermissionLabel=m_CHK_bDownJXLabel;
	


	ComPareChange(m_pOwner->m_tStandardSize.bEnableABMissValue.I(),m_CHK_bEnableABMiss,"AB错位检测开关");
	m_pOwner->m_tStandardSize.bEnableABMissValue=m_CHK_bEnableABMiss;
	ComPareChange(m_pOwner->m_tStandardSize.dABMissValue.D(),m_EDT_dABMissValue,"AB错位标准值");
	m_pOwner->m_tStandardSize.dABMissValue=m_EDT_dABMissValue;
	ComPareChange(m_pOwner->m_tStandardSize.dABMissValueDev.D(),m_EDT_dABMissValueDev,"AB错位公差");
	m_pOwner->m_tStandardSize.dABMissValueDev=m_EDT_dABMissValueDev;
	ComPareChange(m_pOwner->m_tStandardSize.bEnableABMissValueLabel.I(),m_CHK_bABMissLabel,"AB错位贴标开关");
	m_pOwner->m_tStandardSize.bEnableABMissValueLabel=m_CHK_bABMissLabel;


	ComPareChange(m_pOwner->m_tStandardSize.bEnableTotalLengthLabel.I(),m_CHK_bTotalLengthLabel,"总长贴标开关");
	m_pOwner->m_tStandardSize.bEnableTotalLengthLabel=m_CHK_bTotalLengthLabel;//1
	ComPareChange(m_pOwner->m_tStandardSize.bEnableCoatWidthLabel.I(),m_CHK_bCoatWidthLabel,"涂布宽度贴标开关");
	m_CHK_bCoatWidthLabel=m_pOwner->m_tStandardSize.bEnableCoatWidthLabel=m_CHK_bCoatWidthLabel;//1
	ComPareChange(m_pOwner->m_tStandardSize.bEnableUpImHeight.I(),m_CHK_bUpAt9Label,"前相机AT9贴标开关");
	m_pOwner->m_tStandardSize.bEnableLocateMissingLabel=m_CHK_bUpAt9Label;//1

	ComPareChange(m_pOwner->m_tStandardSize.bEnableCuoWei.I(),m_CHK_bEnableCuoWei,"AT9错位检测开关");
	m_pOwner->m_tStandardSize.bEnableCuoWei=m_CHK_bEnableCuoWei;
	ComPareChange(m_pOwner->m_tStandardSize.dCuoWeiMax.D(),m_EDT_dCuoWeiMax,"AT9错位报警值");
	m_pOwner->m_tStandardSize.dCuoWeiMax=m_EDT_dCuoWeiMax;


	///新加的



	ComPareChange(m_pOwner->m_tStandardSize.bEnableTotalWidth.I(), m_CHK_bEnableTotalWidth, "总宽检测使能");
	  m_pOwner->m_tStandardSize.bEnableTotalWidth= m_CHK_bEnableTotalWidth;

	  ComPareChange(m_pOwner->m_tStandardSize.bToatlWidthLabel.I(), m_CHK_bToatlWidthLabel, "总宽打标使能");
	 m_pOwner->m_tStandardSize.bToatlWidthLabel= m_CHK_bToatlWidthLabel;

	 ComPareChange(m_pOwner->m_tStandardSize.dTotalCoatWidth.D(), m_EDT_dTotalCoatWidth, "总宽度");
	  m_pOwner->m_tStandardSize.dTotalCoatWidth= m_EDT_dTotalCoatWidth;

	  ComPareChange(m_pOwner->m_tStandardSize.dTotalCoatWidthDev.D(), m_EDT_dTotalCoatWidthDev, "总宽度公差");
	  m_pOwner->m_tStandardSize.dTotalCoatWidthDev= m_EDT_dTotalCoatWidthDev;



	  ComPareChange(m_pOwner->m_tStandardSize.bBHJXLabel2.I(), m_CHK_BHJXLabel2, "A面模长打标使能");
	  m_pOwner->m_tStandardSize.bBHJXLabel2= m_CHK_BHJXLabel2;
	  ComPareChange(m_pOwner->m_tStandardSize.bEnableBHJXLength2.I(), m_CHK_bEnableBHJXLength2, "A面模长检测使能");
	  m_pOwner->m_tStandardSize.bEnableBHJXLength2= m_CHK_bEnableBHJXLength2;
	  ComPareChange(m_pOwner->m_tStandardSize.dBHJXLength2.D(), m_EDT_dBHJXLength2, "A面模长值");
	  m_pOwner->m_tStandardSize.dBHJXLength2= m_EDT_dBHJXLength2;
	  ComPareChange(m_pOwner->m_tStandardSize.dBHJXLengthDev2.D(), m_EDT_dBHJXLengthDev2, "A面模长公差");
	  m_pOwner->m_tStandardSize.dBHJXLengthDev2= m_EDT_dBHJXLengthDev2;




	  ComPareChange(m_pOwner->m_tStandardSize.bEnableHeadCuoWei.I(), m_CHK_bEnableHeadCuoWei, "头部错位检测使能");
	  m_pOwner->m_tStandardSize.bEnableHeadCuoWei= m_CHK_bEnableHeadCuoWei;
	  ComPareChange(m_pOwner->m_tStandardSize.bHeadCuoWeiLabel.I(), m_CHK_bHeadCuoWeiLabel, "头部错位打标使能");
	  m_pOwner->m_tStandardSize.bHeadCuoWeiLabel= m_CHK_bHeadCuoWeiLabel;
	  ComPareChange(m_pOwner->m_tStandardSize.dHeadCuoWei.D(), m_EDT_dHeadCuoWei, "头部错位");
	  m_pOwner->m_tStandardSize.dHeadCuoWei= m_EDT_dHeadCuoWei;
	  ComPareChange(m_pOwner->m_tStandardSize.dHeadCuoWeiDev.D(), m_EDT_dHeadCuoWeiDev, "头部错位公差");
	 m_pOwner->m_tStandardSize.dHeadCuoWeiDev= m_EDT_dHeadCuoWeiDev;



	 ComPareChange(m_pOwner->m_tStandardSize.bTailCuoWeiLabel.I(), m_CHK_bTailCuoWeiLabel, "尾部错位检测使能");
	 m_pOwner->m_tStandardSize.bTailCuoWeiLabel= m_CHK_bTailCuoWeiLabel;
	 ComPareChange(m_pOwner->m_tStandardSize.bEnableTailCuoWei.I(), m_CHK_bEnableTailCuoWei, "尾部错位检测使能");
	 m_pOwner->m_tStandardSize.bEnableTailCuoWei= m_CHK_bEnableTailCuoWei;
	 ComPareChange(m_pOwner->m_tStandardSize.dTailCuoWei.D(), m_EDT_dTailCuoWei, "尾部错位公差");
	 m_pOwner->m_tStandardSize.dTailCuoWei= m_EDT_dTailCuoWei;

	 ComPareChange(m_pOwner->m_tStandardSize.dTailCuoWeiDev.D(), m_EDT_dTailCuoWeiDev, "尾部错位公差");
	 m_pOwner->m_tStandardSize.dTailCuoWeiDev= m_EDT_dTailCuoWeiDev;













	//GetLSTValue(m_pOwner->m_tStandardSize.tUpCaoWeiParam,m_LST_CaoWeiParam0,0);
	//GetLSTValue(m_pOwner->m_tStandardSize.tDownCaoWeiParam,m_LST_CaoWeiParam1,1);

	UpdateData(FALSE);
}
void CDlgStandardSetting::SetButtonColors(CButtonExt & btn,short btnoffset,COLORREF bkin,COLORREF bkout,COLORREF bkfoucus,COLORREF btfin,COLORREF btfout,COLORREF btffocus)

{
	btn.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, btnoffset);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_IN, bkin);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, bkout);
	btn.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, bkfoucus);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_IN, btfin);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, btfout);
	btn.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, btffocus);
	btn.SetFont(&m_FontButton);
}

void  CDlgStandardSetting::InitUI()
{
	m_FontButton.CreatePointFont(100,"Microsoft YaHei");

	COLORREF MainColor = g_Blendent.m_Blendent.Dlg.MainColor;
	short	shBtnColor = g_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = g_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = g_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = g_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = g_Blendent.m_Blendent.Button.FGColorOut;


}
BOOL CDlgStandardSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	SetValue();
	UpdateData(FALSE);
	InitUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgStandardSetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(255,255,255));


}


void CDlgStandardSetting::OnBnClickedBtnReturn()
{
	// TODO: 在此添加控件通知处理程序代码
		CDialogEx::OnCancel();
}


HBRUSH CDlgStandardSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}













void CDlgStandardSetting::OnSelchangeComGongyileixing()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_COM_nGongYiLeiXing== GongYi_ASC1)//连涂料
	{
		m_COM_nOapChoice = 0;
		m_COM_nCaoWeiChoice = 0;
		m_COM_nCoatType =0;
		m_COM_nExistJXType = 0;
	}else if(m_COM_nGongYiLeiXing == GongYi_M6S)
	{
		m_COM_nOapChoice = 0;
		m_COM_nCaoWeiChoice = 0;
		m_COM_nCoatType = 1;
		m_COM_nExistJXType = 2;
	}
	else if (m_COM_nGongYiLeiXing == GongYi_MMT)
	{
		m_COM_nOapChoice = 0;
		m_COM_nCaoWeiChoice = 2;
		m_COM_nCoatType = 1;
		m_COM_nExistJXType = 2;
	}
	else if (m_COM_nGongYiLeiXing == GongYi_STS1)
	{
		m_COM_nOapChoice = 1;
		m_COM_nCaoWeiChoice = 0;
		m_COM_nCoatType = 1;
		m_COM_nExistJXType = 2;
	}
	else if (m_COM_nGongYiLeiXing == GongYi_STS2)
	{
		m_COM_nOapChoice = 2;
		m_COM_nCaoWeiChoice = 0;
		m_COM_nCoatType = 1;
		m_COM_nExistJXType = 2;
	}


	UpdateData(FALSE);











}


//void CDlgStandardSetting::OnBnClickedChkChangmozaishang()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}
