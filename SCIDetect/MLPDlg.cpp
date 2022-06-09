
// MLP标注工具Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MLPDlg.h"
#include "DlgAlgorithm.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框




CMLPDlg::CMLPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMLPDlg::IDD, pParent)
	, m_COM_nClassType(0)
{

	m_hoCurrentImage.GenEmptyObj();
	m_hoErrorRegion.GenEmptyObj();
	m_hvErrorType.Clear();
}

void CMLPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COM_ERRORNAME, m_COM_CClassName);
	//  DDX_CBIndex(pDX, IDC_COM_LABELTYPE, m_COM_nLabelType);
	DDX_CBIndex(pDX, IDC_COM_ERRORNAME, m_COM_nClassType);
	//  DDX_Text(pDX, IDC_EDT_DYNTHRESH, m_EDT_nDynThresh);
	//  DDV_MinMaxInt(pDX, m_EDT_nDynThresh, 0, 100);
	//  DDX_Control(pDX, IDC_LST_ERRORLIST, m_LST_CErrorList);
	//  DDX_Text(pDX, IDC_EDT_LEFTCOLUMN, m_EDT_nLeftColumn);
	//  DDX_Text(pDX, IDC_EDT_RIGHTCOLUMN, m_EDT_nRightColumn);
}

BEGIN_MESSAGE_MAP(CMLPDlg, CDialogEx)
//	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BTN_OPENIMAGE, &CMLPDlg::OnBnClickedBtnOpenimage)
	ON_BN_CLICKED(IDC_BTN_LABEL, &CMLPDlg::OnBnClickedBtnLabel)
	ON_BN_CLICKED(IDC_BTN_ADDTRAINDATA, &CMLPDlg::OnBnClickedBtnAddtraindata)
	ON_BN_CLICKED(IDC_BTN_TRAIN, &CMLPDlg::OnBnClickedBtnTrain)
	ON_BN_CLICKED(IDC_BTN_VERIFY, &CMLPDlg::OnBnClickedBtnVerify)
	ON_BN_CLICKED(IDC_BTN_SAVEALL, &CMLPDlg::OnBnClickedBtnSaveall)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_ERRORLIST, &CMLPDlg::OnItemchangedLstErrorlist)
ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMLPDlg 消息处理程序

BOOL CMLPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hoLabelRegion.GenEmptyObj();
	// 将“关于...”菜单项添加到系统菜单中。
	m_hoCurrentImage=m_pParentDlg->m_hoCurrentDispImage;
	// TODO: 在此添加额外的初始化代码
	ReadParam();
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//void CMLPDlg::OnSysCommand(UINT nID, LPARAM lParam)
//{
//
//		CDialogEx::OnSysCommand(nID, lParam);
//	
//}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMLPDlg::OnPaint()
{
	//CDialogEx::OnPaint();
	CPaintDC dc(this); // device context for painting
	CRect   rcDlg;  
	GetWindowRect(rcDlg);
	ScreenToClient(rcDlg); 
	dc.FillSolidRect(rcDlg,RGB(200,200,200));
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
//HCURSOR CMLPDlg::OnQueryDragIcon()
//{
//	return static_cast<HCURSOR>(m_hIcon);
//
//
//}

void CMLPDlg::ReadParam()
{

	for (int i=0;i<ClassTotalNum;i++)
	{
		m_COM_CClassName.Clear();
		m_COM_CClassName.InsertString(i,g_strClassName[i]);
	}
	m_MLPHandle=m_tDHMLPParam.hvMLPHandle;
	m_hvDataDict=m_tDHMLPParam.hvTrainDataDict;
	m_hvRegionFeature=m_tDHMLPParam.hvRegionFeature;
	m_hvGrayFeature=m_tDHMLPParam.hvGrayFeature;
	m_hvFeatureName=m_tDHMLPParam.hvFeatureName;
	m_hvFeatureLength=m_tDHMLPParam.hvFeatureLength;
	
}
void CMLPDlg::WriteParam()
{
	UpdateData(TRUE);
	m_tDHMLPParam.hvMLPHandle=m_MLPHandle;
	m_tDHMLPParam.hvTrainDataDict=m_hvDataDict;
	m_tDHMLPParam.hvRegionFeature=m_hvRegionFeature;
	m_tDHMLPParam.hvGrayFeature=m_hvGrayFeature;
	m_tDHMLPParam.hvFeatureName=m_hvFeatureName;
	m_tDHMLPParam.hvFeatureLength=m_hvFeatureLength;
	m_pParentDlg->m_pDetect->m_tMLPParam=m_tDHMLPParam;

}

//void CMLPDlg::OnBnClickedBtnOpenimage()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	UpdateData(TRUE);
//	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||"); 
//	CFileDialog FileDlg(TRUE, _T("*.*"), NULL, 0, szFilter, this); 
//	HTuple Width,Height;
//	if(IDOK == FileDlg.DoModal())
//	{
//		CString strFilename = FileDlg.GetPathName(); 
//		try
//		{
//			HTuple hv_Path;
//			hv_Path.Append((HTuple)strFilename);
//			ReadImage(&m_hoCurrentImage,hv_Path);
//			m_pParentDlg->m_hoCurrentDispImage=m_hoCurrentImage;
//			DispObj(m_hoCurrentImage,m_pParentDlg->m_hvWindowHandle);
//		}catch(HException &e )
//		{
//			AfxMessageBox(e.ErrorMessage().Text());
//		}
//
//	}
//
//}


void CMLPDlg::OnBnClickedBtnLabel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_hoLabelRegion.GenEmptyObj();
	HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1,hv_Radius;

	
	try
	{

		
		DrawRegion(&m_hoLabelRegion,m_pParentDlg->m_hvWindowHandle);

		
	}catch(HException& e)
	{

	}
	DispObj(m_hoCurrentImage,m_pParentDlg->m_hvWindowHandle);
	SetColor(m_pParentDlg->m_hvWindowHandle,"green");
	DispObj(m_hoLabelRegion,m_pParentDlg->m_hvWindowHandle);

}


void CMLPDlg::OnBnClickedBtnAddtraindata()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if(m_hoLabelRegion.CountObj()>0)
	{
		HTuple hv_Num;
		CString strKey;

		try
		{
			strKey.Format("%s_Total_tuple",g_strClassName[m_COM_nClassType]);
			GetDictTuple(m_hvDataDict,(HTuple)strKey,&hv_Num);
		}catch(HException& e)
		{
			hv_Num=0;
		}
		////////将LabelRegion扣一下小图 避免用大图耗时间 
		
			HTuple hv_Row0,hv_Column0,hv_Row1,hv_Column1;
			HObject hoRectangle,hoDilationRectangle;
			SmallestRectangle1(m_hoLabelRegion,&hv_Row0,&hv_Column0,&hv_Row1,&hv_Column1);
			GenRectangle1(&hoRectangle,hv_Row0,hv_Column0,hv_Row1,hv_Column1);
			DilationRectangle1(hoRectangle,&hoDilationRectangle,101,101);

			HTuple hv_Row2,hv_Column2,hv_Row3,hv_Column3;
			SmallestRectangle1(hoDilationRectangle,&hv_Row2,&hv_Column2,&hv_Row3,&hv_Column3);

			HObject hoCropPartImage,hoMoveRegion;
			HTuple hv_Height, hv_Width;
			GetImageSize(m_hoCurrentImage, &hv_Width, &hv_Height);
			CropRectangle1(m_hoCurrentImage, &hoCropPartImage, MAX(0,hv_Row2.D()), MAX(0,hv_Column2.D()), MIN(hv_Row3, hv_Height.I()-1), MIN(hv_Width.I()-1,hv_Column3));
			MoveRegion(m_hoLabelRegion,&hoMoveRegion,-hv_Row2,-hv_Column2);

		strKey.Format("%s_%d_tuple",g_strClassName[m_COM_nClassType],hv_Num.I());
		SetDictTuple(m_hvDataDict,(HTuple)strKey,m_COM_nClassType);
		strKey.Format("%s_%d_region",g_strClassName[m_COM_nClassType],hv_Num.I());
		SetDictObject(hoMoveRegion,m_hvDataDict,(HTuple)strKey);
		strKey.Format("%s_%d_Image",g_strClassName[m_COM_nClassType],hv_Num.I());
		SetDictObject(hoCropPartImage,m_hvDataDict,(HTuple)strKey);
		strKey.Format("%s_Total_tuple",g_strClassName[m_COM_nClassType]);
		hv_Num=hv_Num+1;
		SetDictTuple(m_hvDataDict,(HTuple)strKey,hv_Num);
	}
}
void CMLPDlg::list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
	HTuple *hv_ImageFiles)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_ImageDirectoryIndex, hv_ImageFilesTmp;
	HTuple  hv_CurrentImageDirectory, hv_HalconImages, hv_OS;
	HTuple  hv_Directories, hv_Index, hv_Length, hv_NetworkDrive;
	HTuple  hv_Substring, hv_FileExists, hv_AllFiles, hv_i;
	HTuple  hv_Selection;

	//This procedure returns all files in a given directory
	//with one of the suffixes specified in Extensions.
	//
	//Input parameters:
	//ImageDirectory: Directory or a tuple of directories with images.
	//   If a directory is not found locally, the respective directory
	//   is searched under %HALCONIMAGES%/ImageDirectory.
	//   See the Installation Guide for further information
	//   in case %HALCONIMAGES% is not set.
	//Extensions: A string tuple containing the extensions to be found
	//   e.g. ['png','tif',jpg'] or others
	//If Extensions is set to 'default' or the empty string '',
	//   all image suffixes supported by HALCON are used.
	//Options: as in the operator list_files, except that the 'files'
	//   option is always used. Note that the 'directories' option
	//   has no effect but increases runtime, because only files are
	//   returned.
	//
	//Output parameter:
	//ImageFiles: A tuple of all found image file names
	//
	if (0 != (HTuple(HTuple(int(hv_Extensions==HTuple())).TupleOr(int(hv_Extensions==HTuple("")))).TupleOr(int(hv_Extensions==HTuple("default")))))
	{
		hv_Extensions.Clear();
		hv_Extensions[0] = "ima";
		hv_Extensions[1] = "tif";
		hv_Extensions[2] = "tiff";
		hv_Extensions[3] = "gif";
		hv_Extensions[4] = "bmp";
		hv_Extensions[5] = "jpg";
		hv_Extensions[6] = "jpeg";
		hv_Extensions[7] = "jp2";
		hv_Extensions[8] = "jxr";
		hv_Extensions[9] = "png";
		hv_Extensions[10] = "pcx";
		hv_Extensions[11] = "ras";
		hv_Extensions[12] = "xwd";
		hv_Extensions[13] = "pbm";
		hv_Extensions[14] = "pnm";
		hv_Extensions[15] = "pgm";
		hv_Extensions[16] = "ppm";
		//
	}
	(*hv_ImageFiles) = HTuple();
	//Loop through all given image directories.
	{
		HTuple end_val27 = (hv_ImageDirectory.TupleLength())-1;
		HTuple step_val27 = 1;
		for (hv_ImageDirectoryIndex=0; hv_ImageDirectoryIndex.Continue(end_val27, step_val27); hv_ImageDirectoryIndex += step_val27)
		{
			hv_ImageFilesTmp = HTuple();
			hv_CurrentImageDirectory = HTuple(hv_ImageDirectory[hv_ImageDirectoryIndex]);
			if (0 != (int(hv_CurrentImageDirectory==HTuple(""))))
			{
				hv_CurrentImageDirectory = ".";
			}
			GetSystem("image_dir", &hv_HalconImages);
			GetSystem("operating_system", &hv_OS);
			if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
			{
				hv_HalconImages = hv_HalconImages.TupleSplit(";");
			}
			else
			{
				hv_HalconImages = hv_HalconImages.TupleSplit(":");
			}
			hv_Directories = hv_CurrentImageDirectory;
			{
				HTuple end_val41 = (hv_HalconImages.TupleLength())-1;
				HTuple step_val41 = 1;
				for (hv_Index=0; hv_Index.Continue(end_val41, step_val41); hv_Index += step_val41)
				{
					hv_Directories = hv_Directories.TupleConcat((HTuple(hv_HalconImages[hv_Index])+"/")+hv_CurrentImageDirectory);
				}
			}
			TupleStrlen(hv_Directories, &hv_Length);
			TupleGenConst(hv_Length.TupleLength(), 0, &hv_NetworkDrive);
			if (0 != (int((hv_OS.TupleSubstr(0,2))==HTuple("Win"))))
			{
				{
					HTuple end_val47 = (hv_Length.TupleLength())-1;
					HTuple step_val47 = 1;
					for (hv_Index=0; hv_Index.Continue(end_val47, step_val47); hv_Index += step_val47)
					{
						if (0 != (int((HTuple(hv_Directories[hv_Index]).TupleStrlen())>1)))
						{
							TupleStrFirstN(HTuple(hv_Directories[hv_Index]), 1, &hv_Substring);
							if (0 != (HTuple(int(hv_Substring==HTuple("//"))).TupleOr(int(hv_Substring==HTuple("\\\\")))))
							{
								hv_NetworkDrive[hv_Index] = 1;
							}
						}
					}
				}
			}
			hv_ImageFilesTmp = HTuple();
			{
				HTuple end_val57 = (hv_Directories.TupleLength())-1;
				HTuple step_val57 = 1;
				for (hv_Index=0; hv_Index.Continue(end_val57, step_val57); hv_Index += step_val57)
				{
					FileExists(HTuple(hv_Directories[hv_Index]), &hv_FileExists);
					if (0 != hv_FileExists)
					{
						ListFiles(HTuple(hv_Directories[hv_Index]), HTuple("files").TupleConcat(hv_Options), 
							&hv_AllFiles);
						hv_ImageFilesTmp = HTuple();
						{
							HTuple end_val62 = (hv_Extensions.TupleLength())-1;
							HTuple step_val62 = 1;
							for (hv_i=0; hv_i.Continue(end_val62, step_val62); hv_i += step_val62)
							{
								TupleRegexpSelect(hv_AllFiles, ((".*"+HTuple(hv_Extensions[hv_i]))+"$").TupleConcat("ignore_case"), 
									&hv_Selection);
								hv_ImageFilesTmp = hv_ImageFilesTmp.TupleConcat(hv_Selection);
							}
						}
						TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("\\\\").Append("replace_all")), 
							"/", &hv_ImageFilesTmp);
						if (0 != (HTuple(hv_NetworkDrive[hv_Index])))
						{
							TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")), 
								"/", &hv_ImageFilesTmp);
							hv_ImageFilesTmp = "/"+hv_ImageFilesTmp;
						}
						else
						{
							TupleRegexpReplace(hv_ImageFilesTmp, (HTuple("//").Append("replace_all")), 
								"/", &hv_ImageFilesTmp);
						}
						break;
					}
				}
			}
			//Concatenate the output image paths.
			(*hv_ImageFiles) = (*hv_ImageFiles).TupleConcat(hv_ImageFilesTmp);
		}
	}
	return;
}

void CMLPDlg::get_features (HObject ho_Region, HObject ho_Image, HTuple hv_GrayFeaturesName, 
	HTuple hv_RegionFeaturesName, HTuple *hv_Features)
{

	// Local iconic variables

	// Local control variables
	HTuple  hv_RegionFeatures, hv_GrayFeatures;



	RegionFeatures(ho_Region, hv_RegionFeaturesName, &hv_RegionFeatures);
	GrayFeatures(ho_Region, ho_Image, hv_GrayFeaturesName, &hv_GrayFeatures);
	(*hv_Features).Clear();
	(*hv_Features).Append(hv_GrayFeatures);
	(*hv_Features).Append(hv_RegionFeatures);
	return;
}


void CMLPDlg::add_samples (HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName, 
	HTuple hv_RegionFeatureName, HTuple hv_Class)
{

	// Local iconic variables
	HObject  ho_Region;

	// Local control variables
	HTuple  hv_Number, hv_J, hv_Features;

	CountObj(ho_Regions, &hv_Number);
	{
		HTuple end_val1 = hv_Number;
		HTuple step_val1 = 1;
		for (hv_J=1; hv_J.Continue(end_val1, step_val1); hv_J += step_val1)
		{
			SelectObj(ho_Regions, &ho_Region, hv_J);
			get_features(ho_Region, ho_Image, hv_GrayFeatureName, hv_RegionFeatureName, &hv_Features);
			AddSampleClassMlp(hv_MLPHandle, hv_Features, hv_Class);
		}
	}
	return;
}

void CMLPDlg::classify (HObject ho_Regions, HObject ho_Image, HTuple hv_MLPHandle, HTuple hv_GrayFeatureName, 
	HTuple hv_RegionFeatureName, HTuple *hv_Classes, HTuple *hv_Confidence)
{

	// Local iconic variables
	HObject  ho_Region;

	// Local control variables
	HTuple  hv_Number, hv_J, hv_Features, hv_Class,hv_Probility;
	HTuple hv_TempConfidence;
	CountObj(ho_Regions, &hv_Number);
	(*hv_Classes) = HTuple();
	(*hv_Confidence) = HTuple();
	{
		HTuple end_val2 = hv_Number;
		HTuple step_val2 = 1;
		for (hv_J=1; hv_J.Continue(end_val2, step_val2); hv_J += step_val2)
		{
			SelectObj(ho_Regions, &ho_Region, hv_J);
			get_features(ho_Region, ho_Image, hv_GrayFeatureName, hv_RegionFeatureName, &hv_Features);
			EvaluateClassMlp(hv_MLPHandle,hv_Features,&hv_Probility);
			ClassifyClassMlp(hv_MLPHandle, hv_Features, 1, &hv_Class, &hv_TempConfidence);
			(*hv_Classes) = (*hv_Classes).TupleConcat(hv_Class);
			(*hv_Confidence)=(*hv_Confidence).TupleConcat(hv_Probility[hv_Class.I()]);
		}
	}
	return;
}




void CMLPDlg::segment (HObject ho_Image, HObject *ho_Regions, HTuple hv_Type)
{

	// Local iconic variables
	//HObject  ho_Region, ho_ConnectedRegions, ho_ImageMean;
	//HObject  ho_RegionDynThresh;

	//// Local control variables
	//HTuple  hv_UsedThreshold;

	//hv_Type=1;
	//if (0 != (int(hv_Type==0)))
	//{
	//	Threshold(ho_Image, &ho_Region, 120, 255);
	//	Connection(ho_Region, &(*ho_Regions));

	//}
	//else
	//{
	//	if (0 != (int(hv_Type==1)))
	//	{
	//		MeanImage(ho_Image, &ho_ImageMean, 31, 31);
	//		DynThreshold(ho_Image, ho_ImageMean, &ho_RegionDynThresh, m_EDT_nDynThresh, "not_equal");
	//		ClosingRectangle1(ho_RegionDynThresh, &ho_RegionDynThresh, 21, 21);
	//		Connection(ho_RegionDynThresh, &ho_ConnectedRegions);
	//		FillUp(ho_ConnectedRegions, &ho_ConnectedRegions);
	//		Connection(ho_ConnectedRegions, &ho_ConnectedRegions);
	//		SelectShape(ho_ConnectedRegions, &(*ho_Regions), "area", "and", 30, INT_MAX);
	//	}
	//	else
	//	{
	//		BinaryThreshold(ho_Image, &ho_Region, "max_separability", "dark", &hv_UsedThreshold);
	//		Connection(ho_Region, &(*ho_Regions));
	//	}

	//}


	return;
}

void CMLPDlg::OnBnClickedBtnTrain()
{
	// TODO: 在此添加控件通知处理程序代码

	for (int nClassIndex=0;nClassIndex<ClassTotalNum;nClassIndex++)
	{
		CString strKey;
		HTuple hv_Num;
		strKey.Format("%s_Total_tuple",g_strClassName[nClassIndex]);
		try
		{
			GetDictTuple(m_hvDataDict,(HTuple)strKey,&hv_Num);
		}catch(HException& e)
		{
			hv_Num=0;
		}
		for (int nChildDataIndex=0;nChildDataIndex<hv_Num;nChildDataIndex++)
		{
			try
			{
				HObject hoRegion,hoImage;
				strKey.Format("%s_%d_region",g_strClassName[nClassIndex],nChildDataIndex);
				GetDictObject(&hoRegion,m_hvDataDict,(HTuple)strKey);
				strKey.Format("%s_%d_Image",g_strClassName[nClassIndex],nChildDataIndex);
				GetDictObject(&hoImage,m_hvDataDict,(HTuple)strKey);
				add_samples(hoRegion, hoImage, m_MLPHandle, m_hvGrayFeature, m_hvRegionFeature,nClassIndex);
			}catch(HException& e)
			{
				AfxMessageBox(e.ErrorMessage().Text());
			}
			
		}
	}
	HTuple hv_Error,hv_ErrorLog;
	TrainClassMlp(m_MLPHandle, 200, 1, 0.01, &hv_Error, &hv_ErrorLog);
	ClearSamplesClassMlp(m_MLPHandle);

	AfxMessageBox("训练完毕");
}


void CMLPDlg::OnBnClickedBtnVerify()
{
	// TODO: 在此添加控件通知处理程序代码
	 UpdateData(TRUE);



	HTuple  hv_Classes,hv_Confidence;

	HTuple hv_StartTime,hv_EndTime;
	CountSeconds(&hv_StartTime);
	HTuple hv_Height;
	GetImageSize(m_hoCurrentImage, NULL, &hv_Height);

	if (1)
	{
		try
		{

			SetTposition(m_pParentDlg->m_hvWindowHandle,12,12);
			WriteString(m_pParentDlg->m_hvWindowHandle,"请绘制目标区域");
			DrawRegion(&m_hoLabelRegion,m_pParentDlg->m_hvWindowHandle);
		}catch(HException& e)
		{

		}
		classify(m_hoLabelRegion, m_hoCurrentImage, m_MLPHandle, m_hvGrayFeature, m_hvRegionFeature, &hv_Classes, &hv_Confidence);
		if (m_hoLabelRegion.CountObj()>0)
		{

			HTuple hv_NumRegion;
			HTuple hv_Row,hv_Column;
			HObject hoObjected;
			CountObj(m_hoLabelRegion,&hv_NumRegion);
			DispObj(m_hoCurrentImage,m_pParentDlg->m_hvWindowHandle);
			for (int k=0;k<hv_NumRegion;k++)
			{

				SelectObj(m_hoLabelRegion,&hoObjected,k+1);
				AreaCenter(hoObjected,NULL,&hv_Row,&hv_Column);
				SetColor(m_pParentDlg->m_hvWindowHandle,"red");
				DispObj(hoObjected,m_pParentDlg->m_hvWindowHandle);

				SetColor(m_pParentDlg->m_hvWindowHandle,"green");
				SetTposition(m_pParentDlg->m_hvWindowHandle,hv_Row,hv_Column);
				CString strName;
				strName.Format("%s 概率:%.1f",g_strClassName[hv_Classes[k].I()],hv_Confidence[k].D());
				WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strName);

			}
			CountSeconds(&hv_EndTime);

			CString strTime;
			strTime.Format("耗时:%.2fms",(hv_EndTime.D()-hv_StartTime.D())*1000);
			SetTposition(m_pParentDlg->m_hvWindowHandle,12,12);
			WriteString(m_pParentDlg->m_hvWindowHandle,(HTuple)strTime);

			
		}
	}else
	{

		m_pParentDlg->Test(1);


	}


	



	
}


void CMLPDlg::OnBnClickedBtnSaveall()
{
	// TODO: 在此添加控件通知处理程序代码

	WriteParam();
	
}


BOOL CMLPDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}





HBRUSH CMLPDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
