// DlgMap.cpp : 实现文件
#include "stdafx.h"
#include "DlgMap.h"
#include "afxdialogex.h"
#include "Define.h"

// CDlgMap 对话框

IMPLEMENT_DYNAMIC(CDlgMap, CDialogEx)

	CDlgMap::CDlgMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMap::IDD, pParent)
{
	m_hvMapWindow = NULL;
	m_hvColumnGraduationWindow = NULL;//横向刻度
	m_hvRowGraduationWindow = NULL;//纵向刻度
	m_hvSmallWindow = NULL;
	m_strSelMapPath = "";//当前选中的地图路径
	m_strLoadMapPath = "";//当前加载的地图路径
	m_hvFilesSorted = NULL;
	m_hvSurFaceIndex = 0;
	m_nImageWidth = 0;
	m_nImageHeight = 0;

	CString strCommondh;
	strCommondh.Format("%s//Config//Common.dh",GetCurrentAppPath());
	int nErrorCount;
	nErrorCount = GetPrivateProfileInt("ERROR","COUNTTYPE",10,strCommondh);
	CString lpAppName,lpKeyName,lpRetrunedString;
	for (int i=0;i<nErrorCount;i++)
	{
		lpKeyName.Format("ERROR%dNAME",i);
		GetPrivateProfileString("ERRORINFO",lpKeyName,"null",lpRetrunedString.GetBuffer(MAX_PATH),MAX_PATH,strCommondh);
		lpRetrunedString.ReleaseBuffer();
		m_strAErrorName.Add(lpRetrunedString);
	}
	for (int i=0;i<2;i++)
	{
		lpAppName.Format("GRABBER%d",i);
		m_dwRowResolution[i] = GetPrivateProfileDouble(lpAppName,"ROWRESOLUTION",0.05,strCommondh);
		m_dwColumnResolution[i] = GetPrivateProfileDouble(lpAppName,"COLUMNRESOLUTION",0.05,strCommondh);
	}
}

CDlgMap::~CDlgMap()
{
}

void CDlgMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_cstaticTitle);
	DDX_Control(pDX, IDC_STATIC_DEFECTINFO, m_staticDefectInfo);
	DDX_Control(pDX, IDC_TREE_MAP, m_TreeMap);
	DDX_Control(pDX, IDC_SCROLLBAR_MAP, m_ScrollBarMap);
	DDX_Control(pDX, IDC_STATIC_MAPINFO, m_staticMapInfo);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_DETECT, m_btnDetectFront);
	DDX_Control(pDX, IDC_BTN_OPENFILE, m_btnFileImageOpen);
	DDX_Control(pDX, IDC_BTN_DETECT_BEHIND, m_btnDetectBehind);
	DDX_Control(pDX, IDC_BTN_SAVEFILE, m_btnSaveFile);
}


BEGIN_MESSAGE_MAP(CDlgMap, CDialogEx)
	ON_WM_PAINT()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_MAP, &CDlgMap::OnNMRClickTreeMap)
	ON_COMMAND(ID_MENU_LOADMAP, &CDlgMap::OnMenuLoadmap)
	ON_COMMAND(ID_MENU_DELETEMAP, &CDlgMap::OnMenuDeletemap)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgMap::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CDlgMap::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_DETECT_FRONT, &CDlgMap::OnBnClickedBtnDetectFront)
	ON_BN_CLICKED(IDC_BTN_DETECT_BEHIND, &CDlgMap::OnBnClickedBtnDetectBehind)
	ON_BN_CLICKED(IDC_BTN_SAVEFILE, &CDlgMap::OnBnClickedBtnSavefile)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
//	ON_BN_CLICKED(IDC_BUTTON2, &CDlgMap::OnBnClickedButton2)
//	ON_BN_CLICKED(IDC_BUTTON1, &CDlgMap::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgMap 消息处理程序


BOOL CDlgMap::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	InitStatic();
	InitWindowShow();
	InitGraduation();
	InitTree();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgMap::InitStatic()
{
	COLORREF MainColor = m_Blendent.m_Blendent.Dlg.MainColor;

	m_ftButton.CreatePointFont(100,"Microsoft YaHei");

	m_cstaticTitle.SetTextColor(RGB(255,255,255));
	m_cstaticTitle.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	m_cstaticTitle.SetBkColor(MainColor,0,CStaticExt::Normal);
	//m_cstaticTitle.Format("产品生产地图");

	m_staticDefectInfo.SetTextColor(RGB(0,0,0));
	m_staticDefectInfo.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	//m_staticDefectInfo.SetTransparent(TRUE);
	m_staticDefectInfo.SetBkColor( RGB(255,255,255),0,CStaticExt::Normal);
	m_staticDefectInfo.Format("缺陷信息");
	

	m_staticMapInfo.SetTextColor(RGB(0,0,0));
	m_staticMapInfo.SetFont("Microsoft YaHei", 20, FW_NORMAL);
	//m_staticMapInfo.SetTransparent(TRUE);
	m_staticMapInfo.SetBkColor(RGB(255,255,255),0,CStaticExt::Normal);
	m_staticMapInfo.Format("地图信息");

	short	shBtnColor = m_Blendent.m_Blendent.Button.shOffset;
	COLORREF BKOUT = m_Blendent.m_Blendent.Button.BKColorOut;
	COLORREF BKIN = m_Blendent.m_Blendent.Button.BKColorIn;
	COLORREF BKFOCUS = m_Blendent.m_Blendent.Button.BKColorFocus;
	COLORREF FG = m_Blendent.m_Blendent.Button.FGColorOut;
	//设置背景色 共7个按钮
	m_btnDetectFront.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnDetectFront.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnDetectFront.SetIcon(IDI_ICON_W_DETECT);
	m_btnDetectFront.SetFont(&m_ftButton);

	m_btnDetectBehind.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnDetectBehind.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnDetectBehind.SetIcon(IDI_ICON_W_DETECT);
	m_btnDetectBehind.SetFont(&m_ftButton);

	m_btnCancel.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnCancel.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnCancel.SetIcon(IDI_ICON_W_UNCHECK);
	m_btnCancel.SetFont(&m_ftButton);

	m_btnFileImageOpen.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnFileImageOpen.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnFileImageOpen.SetIcon(IDI_ICON_W_SELECT);
	m_btnFileImageOpen.SetFont(&m_ftButton);

	m_btnSaveFile.OffsetColor(CButtonExt::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_BK_IN, BKIN);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_BK_OUT, BKOUT);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_BK_FOCUS, BKFOCUS);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_FG_IN, FG);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_FG_OUT, FG);
	m_btnSaveFile.SetColor(CButtonExt::BTNST_COLOR_FG_FOCUS, FG);
	m_btnSaveFile.SetFont(&m_ftButton);
	m_btnSaveFile.SetIcon(IDI_ICON_W_SAVE);
}

void CDlgMap::InitWindowShow()
{
	CRect rcMap,rcColumnGraduation,rcRowGraduation,rcAll;
	GetDlgItem(IDC_STATIC_MAP)->GetClientRect(rcAll);
	rcMap = rcAll;
	rcMap.top = rcMap.top + 50;//横向50
	rcMap.left   = rcMap.left + 50;//行程一个50的横条
	rcColumnGraduation = rcAll;
	rcColumnGraduation.bottom = rcColumnGraduation.top + 50;
	rcColumnGraduation.left = rcColumnGraduation.left + 50;
	rcRowGraduation = rcAll;
	rcRowGraduation.right = rcRowGraduation.left + 50;
	rcRowGraduation.top = rcRowGraduation.top + 50;


	CRect rcSmallWindow;
	GetDlgItem(IDC_STATIC_SMALLPICTURE)->GetClientRect(rcSmallWindow);

	SetCheck("~father");
	SetWindowAttr("background_color","white");
	///////两个窗体//
	//SetWindowAttr("background_color","white");

	OpenWindow(rcMap.top,rcMap.left,rcMap.Width(),rcMap.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_MAP)->m_hWnd,"visible","",&m_hvMapWindow);

	//SetWindowAttr("background_color","white");
	OpenWindow(rcColumnGraduation.top,rcColumnGraduation.left,rcColumnGraduation.Width(),rcColumnGraduation.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_MAP)->m_hWnd,"visible","",&m_hvColumnGraduationWindow);
	OpenWindow(rcRowGraduation.top,rcRowGraduation.left,rcRowGraduation.Width(),rcRowGraduation.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_MAP)->m_hWnd,"visible","",&m_hvRowGraduationWindow);

	//SetWindowAttr("background_color","white");
	OpenWindow(rcSmallWindow.top,rcSmallWindow.left,rcSmallWindow.Width(),rcSmallWindow.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_SMALLPICTURE)->m_hWnd,"visible","",&m_hvSmallWindow);

	HTuple hvA;
	OpenWindow(rcAll.top,rcAll.left,rcAll.Width(),rcAll.Height(),
		(Hlong)GetDlgItem(IDC_STATIC_MAP)->m_hWnd,"visible","",&hvA);

	SetCheck("father");
	SetDraw(m_hvRowGraduationWindow, "margin");
	SetDraw(m_hvColumnGraduationWindow, "margin");
	SetDraw(m_hvMapWindow, "margin");
	SetColor(m_hvMapWindow,"red");

}
void CDlgMap::DisplayColumnGraduation()//显示横向刻度表
{
	if (m_hoCurrentObject.IsInitialized())
	{
		SetSystem("flush_graphic","false");
		ClearWindow(m_hvColumnGraduationWindow);
		SetSystem("flush_graphic","true");


		HTuple Row1,Column1,Row2,Column2;
		GetPart(m_hvColumnGraduationWindow,&Row1,&Column1,&Row2,&Column2);
		SetColor(m_hvColumnGraduationWindow,"black");
		DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,0,(Row2+Row1)/2,Column2);

		////////获取产品的高度信息

		double dwFrameLength = m_nImageWidth * m_dwColumnResolution[m_hvSurFaceIndex.I()];//一帧的宽度
		double nStep = Column2.I() / (int)dwFrameLength;
		for(int i=0;i<(int)dwFrameLength;i++)
		{
			double nColumn = i*nStep;
			if (0 == i%10)
			{
				DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,nColumn,Row2,nColumn);
				SetTposition(m_hvColumnGraduationWindow,0,nColumn-55);
				CString strLength;
				strLength.Format("%d",i);
				if(i%30==0)
				{
					WriteString(m_hvColumnGraduationWindow,HTuple(strLength));
				}
				
			}else
			{
				DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,nColumn,(Row2+Row1)/2 + (Row2-(Row2+Row1)/2)/2,nColumn);
			} 
		}
	}
}

void CDlgMap::InitGraduation()//显示刻度表
{
	HObject GraduationImage;
	GenImageConst(&GraduationImage,"byte",4095,4000);

	{
		//横向
		HTuple Width,Height;
		HTuple Row1,Column1,Row2,Column2;
		GetPart(m_hvColumnGraduationWindow,&Row1,&Column1,&Row2,&Column2);
		SetColor(m_hvColumnGraduationWindow,"black");
		DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,0,(Row2+Row1)/2,Column2);
		GetImageSize(GraduationImage,&Width,&Height);
		double dwFrameLength = Width * m_dwColumnResolution[m_hvSurFaceIndex.I()];//一帧的宽度
		double nStep = Column2.I() / (int)dwFrameLength;
		for(int i=0;i<(int)dwFrameLength;i++)
		{
			double nColumn = i*nStep;
			if (0 == i%10)
			{
				DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,nColumn,Row2,nColumn);
				SetTposition(m_hvColumnGraduationWindow,0,nColumn-55);
				CString strLength;
				strLength.Format("%d",i);
				WriteString(m_hvColumnGraduationWindow,HTuple(strLength));
			}else
			{
				DispLine(m_hvColumnGraduationWindow,(Row2+Row1)/2,nColumn,(Row2+Row1)/2 + (Row2-(Row2+Row1)/2)/2,nColumn);
			} 
		}
	}

	{
		/////纵向
		HTuple Width,Height;
		HTuple Row1,Column1,Row2,Column2;
		GetPart(m_hvRowGraduationWindow,&Row1,&Column1,&Row2,&Column2);
		SetColor(m_hvRowGraduationWindow,"black");

		DispLine(m_hvRowGraduationWindow,0,(Column1+Column2)/2,Row2,(Column1+Column2)/2);
		GetImageSize(GraduationImage,&Width,&Height);
		double dwFrameLength = (Height * m_dwRowResolution[m_hvSurFaceIndex.I()])/1000;//一帧的长度
		double dwFrameStart = dwFrameLength * 0;
		double dwFrameEnd   = dwFrameLength * (0+1);
		double nStep = Row2.I() / 100;

		for (int i=0;i<100;i++)//画竖线
		{
			double nRow = i*nStep;
			double dwSetpFrameLength = dwFrameLength / 100 * i;
			if (0 == i%10)
			{
				DispLine(m_hvRowGraduationWindow,nRow,(Column1+Column2)/2,nRow,Column2);
				SetTposition(m_hvRowGraduationWindow,nRow-100,0);
				CString strLength;
				strLength.Format("%.1f",dwFrameStart + dwSetpFrameLength);//转为米数
				WriteString(m_hvRowGraduationWindow,HTuple(strLength));
			}else
			{
				DispLine(m_hvRowGraduationWindow,nRow,(Column1+Column2)/2,nRow,(Column1+Column2)/2+(Column2-(Column1+Column2)/2)/2);
			} 
		}
	}


}
void CDlgMap::DisplayRowGraduation(int nRowIndex)//显示纵向刻度表
{
	SetSystem("flush_graphic","false");
	ClearWindow(m_hvRowGraduationWindow);
	SetSystem("flush_graphic","true");



	HTuple Row1,Column1,Row2,Column2;
	GetPart(m_hvRowGraduationWindow,&Row1,&Column1,&Row2,&Column2);
	SetColor(m_hvRowGraduationWindow,"black");

	DispLine(m_hvRowGraduationWindow,0,(Column1+Column2)/2,Row2,(Column1+Column2)/2);

	////////获取产品的高度信息

	double dwFrameLength = (m_nImageHeight * m_dwRowResolution[m_hvSurFaceIndex.I()])/1000;//一帧的长度
	double dwFrameStart = dwFrameLength * nRowIndex;
	double dwFrameEnd   = dwFrameLength * (nRowIndex+1);
	double nStep = Row2.I() / 100;

	for (int i=0;i<100;i++)//画竖线
	{
		double nRow = i*nStep;
		double dwSetpFrameLength = dwFrameLength / 100 * i;
		if (0 == i%10)
		{
			DispLine(m_hvRowGraduationWindow,nRow,(Column1+Column2)/2,nRow,Column2);
			SetTposition(m_hvRowGraduationWindow,nRow-100,0);
			CString strLength;
			strLength.Format("%.1f",dwFrameStart + dwSetpFrameLength);//转为米数
			WriteString(m_hvRowGraduationWindow,HTuple(strLength));
		}else
		{
			DispLine(m_hvRowGraduationWindow,nRow,(Column1+Column2)/2,nRow,(Column1+Column2)/2+(Column2-(Column1+Column2)/2)/2);
		} 
	}
}

void CDlgMap::DisplayObj()
{
	//如果object的个数为1 代表用户选中了 只保存原图不保存错误区域模式或者错误区域面积为0
	//如果不等于1 可能有两种模式  A 保存原图并且保存错误区域 B 只保存错误区域
	//判断如果第一个为图片 则是1

	SetSystem("flush_graphic","false");
	ClearWindow(m_hvMapWindow);
	SetSystem("flush_graphic","true");

	HObject Image,ErrorRegionUnion;
	HTuple Width,Height;
	if (m_hoCurrentObject.CountObj() != 1)
	{
		SelectObj(m_hoCurrentObject,&Image,1);
		try
		{
			//A情况
			GetImageSize(Image,&Width,&Height);
			SelectObj(m_hoCurrentObject,&ErrorRegionUnion,3);//第二个是Domain
			DispObj(Image,m_hvMapWindow);//显示图片
			DispObj(ErrorRegionUnion,m_hvMapWindow);//显示图片

		}catch(HException )//如果第一张不是图片
		{
			//B情况
			SelectObj(m_hoCurrentObject,&ErrorRegionUnion,2);////第1个是Domain
			DispObj(ErrorRegionUnion,m_hvMapWindow);//显示图片
		}

	}else
	{
		DispObj(m_hoCurrentObject,m_hvMapWindow);//显示图片
	}
}



void CDlgMap::Display(int nRowIndex)//显示刻度表
{
	CString strPathName = m_hvFilesSorted[nRowIndex].S().Text();
	CString strFileName = PathFindFileNameA(strPathName);
	int nRowGraduation = atoi(strFileName);
	DisplayRowGraduation(nRowGraduation-1);//显示刻度
	try
	{
		
		ReadObject(&m_hoCurrentObject,m_hvFilesSorted[nRowIndex]);
		DisplayObj();

		HTuple InfoPath;
		HTuple Length;
		TupleStrlen(m_hvFilesSorted[nRowIndex],&Length);
		TupleStrFirstN(m_hvFilesSorted[nRowIndex],Length-6,&InfoPath);
		ReadTuple(InfoPath,&m_hvCurrentInfo);
	}catch(HException )
	{
		//异常暂时不做操作

	}
	CString strLastPathName = m_hvFilesSorted[m_hvFilesSorted.Length()-1].S().Text();
	CString strLastFileName = PathFindFileNameA(strLastPathName);
	int nRowGraduationCount = atoi(strLastFileName);


	m_staticMapInfo.Format("%d/%d\r\n%d/%d",nRowIndex+1,m_hvFilesSorted.Length(),nRowGraduation,nRowGraduationCount);





}
void CDlgMap::InitTree()
{
	m_TreeMap.DeleteAllItems();


	m_TreeMapImages.Create(16,16,ILC_COLOR32|ILC_MASK,0,0);
	m_TreeMapImages.Add(AfxGetApp()->LoadIconA(IDI_ICON_B_DATABASE));
	m_TreeMapImages.Add(AfxGetApp()->LoadIconA(IDI_ICON_B_FOLDER));
	m_TreeMapImages.Add(AfxGetApp()->LoadIconA(IDI_ICON_B_FLAG1));
	m_TreeMapImages.Add(AfxGetApp()->LoadIconA(IDI_ICON_B_FLAG2));
	m_TreeMap.SetImageList(&m_TreeMapImages, LVSIL_NORMAL);



	HTuple Year,Month,Day;
	GetSystemTime(NULL,NULL,NULL,NULL,&Day,NULL,&Month,&Year);

	CString strMapFolder;
	strMapFolder.Format("%s\\Map\\",GetCurrentAppPath());
	CreateDirectory(strMapFolder,NULL);
	HTuple hvMonths;
	ListFiles(HTuple(strMapFolder),"directories",&hvMonths);//获得月份列表
	for (int i=0;i<hvMonths.Length();i++)
	{
		CString strMonth,strMonthPath;
		strMonthPath.Format("%s",hvMonths[i].S().Text());//为了后面使用
		strMonth.Format("%s",hvMonths[i].S().Text());
		strMonth.Replace(strMapFolder,"");//减去前面路径
		strMonth.Replace("\\","");//减去后面斜杠
		HTREEITEM hRoot = m_TreeMap.InsertItem(strMonth, 0, 0);//插入根节点

		////插入日期
		HTuple hvDays;
		ListFiles(HTuple(strMonthPath),"directories",&hvDays);//获得日期列表
		for (int j=0;j<hvDays.Length();j++)//获取天数列表
		{

			CString strDay,strDayPath;
			strDayPath.Format("%s",hvDays[j].S().Text());//为了后面使用
			strDay.Format("%s",hvDays[j].S().Text());
			strDay.Replace(strMonthPath,"");//减去前面路径
			strDay.Replace("\\","");//减去后面斜杠
			HTREEITEM hRootDay = m_TreeMap.InsertItem(strDay, 1, 1,hRoot);//插入根节点
			HTuple hvBatchs;
			ListFiles(HTuple(strDayPath),"directories",&hvBatchs);//获得卷号列表
			for (int k=0;k<hvBatchs.Length();k++)
			{
				CString strBatch,strBatchPath;
				strBatchPath.Format("%s",hvBatchs[k].S().Text());//为了后面使用
				strBatch.Format("%s",hvBatchs[k].S().Text());
				strBatch.Replace(strDayPath,"");//减去前面路径
				strBatch.Replace("\\","");//减去后面斜杠
				HTREEITEM hRootBatch = m_TreeMap.InsertItem(strBatch, 1, 1,hRootDay);//插入根节点
				HTuple hvProducts;
				ListFiles(HTuple(strBatchPath),"directories",&hvProducts);//获得卷号列表
				for (int m=0;m<hvProducts.Length();m++)//
				{
					CString strProduct,strProductPath;
					strProductPath.Format("%s",hvProducts[m].S().Text());//为了后面使用
					strProduct.Format("%s",hvProducts[m].S().Text());
					strProduct.Replace(strBatchPath,"");//减去前面路径
					strProduct.Replace("\\","");//减去后面斜杠
					HTREEITEM hRootProduct = m_TreeMap.InsertItem(strProduct, 1, 1,hRootBatch);//插入根节点
					m_TreeMap.Expand(hRootProduct,TVE_EXPAND);

					HTuple hvSides;
					ListFiles(HTuple(strProductPath),"directories",&hvSides);//获得正反面列表
					for (int n=0;n<hvSides.Length();n++)//获得产品列表
					{
						CString strSide,strSidePath;
						strSidePath.Format("%s",hvSides[n].S().Text());//为了后面使用
						strSide.Format("%s",hvSides[n].S().Text());
						strSide.Replace(strProductPath,"");//减去前面路径
						strSide.Replace("\\","");//减去后面斜杠
						if (m_strAErrorName[0] == strSide)
						{
							m_TreeMap.InsertItem(strSide, 2, 2,hRootProduct);//插入根节点

						}else if (m_strAErrorName[1] == strSide)
						{
							m_TreeMap.InsertItem(strSide, 3, 3,hRootProduct);//插入根节点
						}else
						{
							m_TreeMap.InsertItem(strSide, 1, 1,hRootProduct);//插入根节点
						}


					}
					if (strProduct == m_strProductName)
					{
						m_TreeMap.Expand(hRootProduct,TVE_EXPAND);
					}
				}
				if (strBatch == m_strBatch)
				{
					m_TreeMap.Expand(hRootBatch,TVE_EXPAND);
				}
			}
			if (atoi(strDay.Mid(0,strDay.GetLength()-2)) == Day.I()
				&& atoi(strMonth.Mid(6,strMonth.GetLength()-8)) == Month.I())///检测是否展开
			{
				m_TreeMap.Expand(hRoot,TVE_EXPAND);
				m_TreeMap.Expand(hRootDay,TVE_EXPAND);
			}
		}
	}
}


void CDlgMap::OnPaint()
{ 
	CPaintDC   dc(this);  
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

	GetDlgItem(IDC_STATIC_MAP)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_STATIC_MAPINFO)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_STATIC_SMALLPICTURE)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_STATIC_DEFECTINFO)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);

	GetDlgItem(IDC_TREE_MAP)->GetWindowRect(rcCtl);
	ScreenToClient(rcCtl);
	dc.MoveTo(rcCtl.left-1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.top-1);
	dc.LineTo(rcCtl.right+1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.bottom+1);
	dc.LineTo(rcCtl.left-1,rcCtl.top-1);
}
void CDlgMap::GetFullPath(HTREEITEM hItem)
{
	if(hItem == m_TreeMap.GetRootItem())// 如果等于根节点
	{
		m_strSelMapPath.Format("%s",m_TreeMap.GetItemText(hItem));
	}
	HTREEITEM parent = hItem;
	CString strPath;  // 保存当前节点路径
	strPath = m_TreeMap.GetItemText(hItem); 
	while((parent = m_TreeMap.GetParentItem(parent)) != NULL)
	{
		CString temp = m_TreeMap.GetItemText(parent);
		strPath = temp + "\\" + strPath;                    
	}
	m_strSelMapPath.Format("%s\\Map\\%s",GetCurrentAppPath(),strPath);
}

void CDlgMap::OnNMRClickTreeMap(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	CPoint  RClickPoint; 
	GetCursorPos(&RClickPoint); //获取当前光标的位置，以便使得菜单可以跟随光标  
	m_TreeMap.ScreenToClient(&RClickPoint);  //m_TREE是CTreeCtrl控件变量。
	HTREEITEM  SelItem = m_TreeMap.HitTest(RClickPoint,NULL); //获取当前鼠标右键单击的位置下的item
	m_TreeMap.SelectItem(SelItem);
	GetFullPath(SelItem);

	m_TreeMap.ClientToScreen(&RClickPoint);
	CMenu menu;//定义下面要用到的cmenu对象  
	menu.LoadMenu(IDR_MENU_RBUTTONMAP);//装载自定义的右键菜单  
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,RClickPoint.x,RClickPoint.y,this);  //在指定位置显示弹出菜单
	*pResult = 0;
}


void CDlgMap::OnMenuLoadmap()
{
	// TODO: 在此添加命令处理程序代码
	if ("正面" == m_strSelMapPath.Mid(m_strSelMapPath.GetLength()-4,4))
	{
		m_hvSurFaceIndex = 0;
	}else if ("反面" == m_strSelMapPath.Mid(m_strSelMapPath.GetLength()-4,4))
	{
		m_hvSurFaceIndex = 1;
	}else
	{
		AfxMessageBox("请选择某一面进行地图生成");
		return;
	}

	//
	FastSortFileNames(HTuple(m_strSelMapPath),&m_hvFilesSorted);//按照名称排序
	int nIndex = MAX(0,m_hvFilesSorted.Length() - 30000);//由于ScrollBa的最大值为32,767，这里取最后的30000个文件
	if (0 == m_hvFilesSorted.Length())
	{
		return;
	}
	TupleLastN(m_hvFilesSorted,nIndex,&m_hvFilesSorted);
	Hlong nLength = m_hvFilesSorted.Length()-1;
	m_ScrollBarMap.SetScrollRange(0,(int)nLength);
	m_ScrollBarMap.SetScrollPos(nLength);

	try
	{
		//显示最后一张图
		ReadObject(&m_hoCurrentObject,m_hvFilesSorted[nLength]);
		HObject Image;
		SelectObj(m_hoCurrentObject,&Image,1);
		HTuple Row1,Row2,Column1,Column2;
		SmallestRectangle1(Image,&Row1,&Column1,&Row2,&Column2);
		m_nImageHeight = Row2.I() - Row1.I();
		m_nImageWidth  = Column2.I() - Column1.I();
		SetPart(m_hvMapWindow,0,0,m_nImageHeight-1,m_nImageWidth-1);

		try
		{
			HTuple InfoPath;
			HTuple Length;
			TupleStrlen(m_hvFilesSorted[nLength],&Length);
			TupleStrFirstN(m_hvFilesSorted[nLength],Length-6,&InfoPath);
			ReadTuple(InfoPath,&m_hvCurrentInfo);
		}catch(HException )
		{
			m_hvCurrentInfo.Clear();
		}
	
	}catch(HException )
	{
		m_hoCurrentObject.Clear();
	}
	if (m_hoCurrentObject.IsInitialized())
	{
		DisplayColumnGraduation();
		Display(nLength);
	}else
	{
		m_staticMapInfo.Format("%s当前没有有效的地图数据",m_strSelMapPath);
		AfxMessageBox("没有有效的地图数据");
	}

}


void CDlgMap::OnMenuDeletemap()
{
	HTREEITEM  hItem = m_TreeMap.GetSelectedItem();
	if(hItem == m_TreeMap.GetRootItem())// 如果等于根节点
	{
		m_strSelMapPath.Format("%s",m_TreeMap.GetItemText(hItem));
	}
	HTREEITEM parent = hItem;
	CString strPath;  // 保存当前节点路径
	strPath = m_TreeMap.GetItemText(hItem); 
	while((parent = m_TreeMap.GetParentItem(parent)) != NULL)
	{
		CString temp = m_TreeMap.GetItemText(parent);
		strPath = temp + "\\" + strPath;                    
	}
	CString strDelFolder;
	strDelFolder.Format("%s\\Map\\%s",GetCurrentAppPath(),strPath);
	DeleteFolder(strDelFolder);
	m_TreeMap.DeleteItem(hItem);
}


void CDlgMap::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDC_SCROLLBAR_MAP == pScrollBar->GetDlgCtrlID())
	{
		int nCurPos = pScrollBar->GetScrollPos();
		int nMax(0),nMin(0);
		m_ScrollBarMap.GetScrollRange(&nMin,&nMax);
		switch(nSBCode)  
		{  
		case SB_LINEDOWN: 
			{
				++nCurPos;
				if (nCurPos < nMin)
				{
					nCurPos = nMin;
				}
				if (nCurPos > nMax)
				{
					nCurPos = nMax;
				}
			}
			break;  
		case SB_LINEUP:
			{
				--nCurPos;
				if (nCurPos < nMin)
				{
					nCurPos = nMin;
				}
				if (nCurPos > nMax)
				{
					nCurPos = nMax;
				}
			}
			break;  
		case SB_PAGEDOWN:
			{
				nCurPos += 10;
				if (nCurPos < nMin)
				{
					nCurPos = nMin;
				}
				if (nCurPos > nMax)
				{
					nCurPos = nMax;
				}
			}
			break;  
		case SB_PAGEUP:
			{
				nCurPos -= 10; 
				if (nCurPos < nMin)
				{
					nCurPos = nMin;
				}
				if (nCurPos > nMax)
				{
					nCurPos = nMax;
				}
			}
			break;
		case SB_TOP:
			{
				nCurPos=nMin;
			}
			break;
		case SB_BOTTOM:
			{
				nCurPos=nMax;
			}
			break;
		case  SB_THUMBTRACK: 
			{
				nCurPos = nPos;
				if (nCurPos < nMin)
				{
					nCurPos = nMin;
				}
				if (nCurPos > nMax)
				{
					nCurPos = nMax;
				}
			}
			break;
		default:
			break;
		}

		pScrollBar->SetScrollPos(nCurPos);

		try
		{
			Display(nCurPos);
		}catch(...)
		{

		}
		

	}
	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CDlgMap::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)//如果是回车
		{
			return TRUE;
		}else if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		return TRUE;
	}else if (pMsg->message == WM_LBUTTONDBLCLK)//如果双击了屏幕
	{
		try
		{
			HTuple Button,Row,Column;
			GetMposition(m_hvMapWindow,&Row,&Column,&Button);
			if (Button == 1)//左键双击
			{
				DisplayObj();
				if (m_hoCurrentObject.CountObj() == 1)
				{
				}else
				{
					HObject Image,ErrorUnion;
					SelectObj(m_hoCurrentObject,&Image,1);
					int nFirstSmallImageIndex = 4;
					try
					{
						GetImageSize(Image,NULL,NULL);//如果第一个是图片，从第三个开始才是Union
						SelectObj(m_hoCurrentObject,&ErrorUnion,nFirstSmallImageIndex -1);
					}catch(HException)//如果第一张不是图片，第一个是Union，第二个正事开始为图片
					{
						nFirstSmallImageIndex = 3;
						SelectObj(m_hoCurrentObject,&ErrorUnion,nFirstSmallImageIndex -1);
					}
					HObject ConnectedRegions,ObjectSelected,ImageSelected;
					HTuple InSide;
					Connection(ErrorUnion,&ConnectedRegions);
					int nErrorIndex = -1;
					for (int i=0;i<ConnectedRegions.CountObj();i++)//先判断有无单击到区域上的
					{
						SelectObj(ConnectedRegions,&ObjectSelected,i+1);
						TestRegionPoint(ObjectSelected,Row,Column,&InSide);
						if (InSide == 1)//如果发现了双击了这个区域
						{
							SelectObj(m_hoCurrentObject,&ImageSelected,nFirstSmallImageIndex + i);
							HTuple Width,Height;
							GetImageSize(ImageSelected,&Width,&Height);
							SetPart(m_hvSmallWindow,0,0,Height-1,Width-1);
							ClearWindow(m_hvSmallWindow);
							DispObj(ImageSelected,m_hvSmallWindow);
							DispCircle(m_hvMapWindow,Row,Column,100);
							nErrorIndex = i;
							break;
						}
					}
					if (InSide == 0)//如果没有点击到区域上的，看下有无单击到该区域周边的
					{
						for (int i=0;i<ConnectedRegions.CountObj();i++)
						{
							SelectObj(ConnectedRegions,&ObjectSelected,i+1);
							HTuple CenterRow,CenterColumn;
							AreaCenter(ObjectSelected,NULL,&CenterRow,&CenterColumn);
							HObject CenterPoint,RegionDilation;
							GenRegionPoints(&CenterPoint,CenterRow,CenterColumn);
							DilationRectangle1(CenterPoint,&RegionDilation,200,200);

							TestRegionPoint(RegionDilation,Row,Column,&InSide);


							if (InSide == 1)//如果发现了双击了这个区域
							{
								SelectObj(m_hoCurrentObject,&ImageSelected,nFirstSmallImageIndex + i);
								HTuple Width,Height;
								GetImageSize(ImageSelected,&Width,&Height);
								SetPart(m_hvSmallWindow,0,0,Height-1,Width-1);
								ClearWindow(m_hvSmallWindow);
								DispObj(ImageSelected,m_hvSmallWindow);
								DispCircle(m_hvMapWindow,CenterRow,CenterColumn,100);
								nErrorIndex = i;
								break;
							}
						}
					}
					if (nErrorIndex!=-1)//显示错误信息
					{
					    int nErrorCount = m_hvCurrentInfo.Length() / 3;//一共有这么多个
						HTuple Type;
						HTuple Level;
						HTuple Flag;
						TupleSelectRange(m_hvCurrentInfo,0,nErrorCount-1,&Type);
						TupleSelectRange(m_hvCurrentInfo,nErrorCount,2*nErrorCount-1,&Level);
						TupleSelectRange(m_hvCurrentInfo,2*nErrorCount,3*nErrorCount-1,&Flag);
						
						if (Type[nErrorIndex].I() < m_strAErrorName.GetCount())
						{
							CString strErrorName = m_strAErrorName[Type[nErrorIndex].I()];
							m_staticDefectInfo.Format("错误类型:%s\r\n错误等级:%d\r\n错误隶属条:%d",
								strErrorName,Level[nErrorIndex].I(),Flag[nErrorIndex].I());

						}
						

					}
					if (InSide == 0)
					{
						ClearWindow(m_hvSmallWindow);
						m_staticDefectInfo.Format("缺陷类型");
					}

				}
				return TRUE;
			}

		}catch(HException)
		{

		}
	}
	else if (pMsg->message == WM_MOUSEWHEEL)
	{
		short zDelta = HIWORD(pMsg->wParam);
		UINT nFlags  = LOWORD(pMsg->wParam);
		OnMouseWheel(nFlags,zDelta,pMsg->pt);
		return TRUE;
	}else if (pMsg->message == WM_MOUSEMOVE && pMsg->wParam & MK_LBUTTON)
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


BOOL CDlgMap::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nVscroll=m_ScrollBarMap.GetScrollPos();
	if (zDelta == -120)  
	{  
		nVscroll += 10;  
		OnVScroll(SB_PAGEDOWN, nVscroll, &m_ScrollBarMap);  
	}  
	else if (zDelta == 120)  
	{  
		nVscroll -= 10;  
		OnVScroll(SB_PAGEUP, nVscroll, &m_ScrollBarMap);  

	}  
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDlgMap::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CDlgMap::Detect()
{


	try
	{
		HTuple Class;
		GetObjClass(m_hoCurrentObject,&Class);
		if (Class[0] == "image")
		{
			DETECTINPUT tDetectInput;
			tDetectInput.hoDetectImage = m_hoCurrentObject.SelectObj(1);;
			tDetectInput.nBreakType    = 0;
			int nGrabber=m_hvSurFaceIndex.I();
			if (m_nLanCount==2)//如果是双条的话，可以用单面前后相机检测左右条来用
			{
				if (m_hvSurFaceIndex.I()==0 )
				{
				if (m_bFront==TRUE)
				{
					nGrabber=0;
					m_bFront=FALSE;
				}else if (m_bBehind==TRUE)
				{
					 nGrabber=1;
					 m_bBehind=FALSE;
				}
			
				
				}
				if (m_hvSurFaceIndex.I()==1)
				{
					if (m_bFront==TRUE)
					{
						nGrabber=2;
						m_bFront=FALSE;
					}else if (m_bBehind==TRUE)
					{
						nGrabber=3;
						m_bBehind=FALSE;
					}


				}
			
			}
			if (TRUE != m_pDetect[nGrabber]->DoSomething("SHOW",&tDetectInput,NULL))
			{
				CString strException;
				strException.Format("算法%d调用显示界面异常,请排查原因",m_hvSurFaceIndex.I());
				AfxMessageBox(strException);
				return;
			}
		}
	}catch(HException e)
	{
	}catch(std::out_of_range )
	{
	}
			
}


void CDlgMap::OnBnClickedBtnOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
   
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
	CFileDialog FileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this); 
	if(IDOK == FileDlg.DoModal())
	{
		CString lpstrFilename = FileDlg.GetPathName(); 
		try
		{
			ReadImage(&m_hoCurrentObject,HTuple(lpstrFilename));
			
		}catch(HException )
		{
			try
			{
				ReadObject(&m_hoCurrentObject,HTuple(lpstrFilename));
				
			}catch(HException )
			{
				AfxMessageBox("打开文件失败");
				return;
			}
			
		}
		HTuple Width,Height;
		GetImageSize(m_hoCurrentObject.SelectObj(1),&Width,&Height);
		SetPart(m_hvMapWindow,0,0,Height-1,Width-1);
		DispObj(m_hoCurrentObject.SelectObj(1),m_hvMapWindow);
	}
}



void CDlgMap::OnBnClickedBtnDetectFront()
{
	m_bFront=TRUE;
	Detect();
}


void CDlgMap::OnBnClickedBtnDetectBehind()
{
	m_bBehind=TRUE;
	Detect();
}


void CDlgMap::OnBnClickedBtnSavefile()
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
		WriteImage(m_hoCurrentObject.SelectObj(1),"bmp",NULL,HTuple(strPath+strImageName));

		CString strMessageBox;
		strMessageBox.Format("图片保存完毕\r\n%s%s\r\n",strPath,strImageName);
		AfxMessageBox(strMessageBox);
	}catch(HException e)
	{
		AfxMessageBox("保存图片失败");
	}

}


HBRUSH CDlgMap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	/*if(nCtlColor==CTLCOLOR_SCROLLBAR)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}*/
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CDlgMap::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CDlgMap::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
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






