#include "StdAfx.h"
#include "AutoGRR.h"





CAutoGRR::CAutoGRR(void)
{
}


CAutoGRR::~CAutoGRR(void)
{
}
void CAutoGRR::ReadParam()
{

	CString strFileDirectory;
	strFileDirectory.Format("%s\\Config\\Public\\自动GRR\\%d",GetCurrentAppPath(),m_nSide);
	CreateMultilevelPath(strFileDirectory);
	m_StrAutoGRRFilePath.Format("%s\\AotuGRRParam.ini",strFileDirectory);
	m_tGRRParamInfo.bEnableUseAutoGRR=GetPrivateProfileInt("GRRInfo","bEnableUseAutoGRR",0,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nCameraIndex=GetPrivateProfileInt("GRRInfo","nCameraIndex",0,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nFLPLeftColumn=GetPrivateProfileInt("GRRInfo","nFLPLeftColumn",0,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nFLPRightColumn=GetPrivateProfileInt("GRRInfo","nFLPRightColumn",500,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nFLPSizeLimit=GetPrivateProfileInt("GRRInfo","nFLPSizeLimit",15,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nScaleFactor=GetPrivateProfileInt("GRRInfo","nScaleFactor",2,m_StrAutoGRRFilePath);
	CString StrDefault;
	StrDefault.Format("%s\\GRR自动点检",GetCurrentAppPath());
	m_tGRRParamInfo.strGRRImagePath=GetPrivateProfileCString("GRRInfo","strGRRImagePath",StrDefault,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nYPLeftColumn=GetPrivateProfileInt("GRRInfo","nYPLeftColumn",3000,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nYPRightColumn=GetPrivateProfileInt("GRRInfo","nYPRightColumn",4000,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nYPLowThresh=GetPrivateProfileInt("GRRInfo","nYPLowThresh",40,m_StrAutoGRRFilePath);
	m_tGRRParamInfo.nYPHighThresh=GetPrivateProfileInt("GRRInfo","nYPHighThresh",80,m_StrAutoGRRFilePath);
	

}
void CAutoGRR::WriteParam()
{
	WritePrivateProfileInt("GRRInfo","bEnableUseAutoGRR",m_tGRRParamInfo.bEnableUseAutoGRR,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nCameraIndex",m_tGRRParamInfo.nCameraIndex,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nFLPLeftColumn",m_tGRRParamInfo.nFLPLeftColumn,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nFLPRightColumn",m_tGRRParamInfo.nFLPRightColumn,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nFLPSizeLimit",m_tGRRParamInfo.nFLPSizeLimit,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nScaleFactor",m_tGRRParamInfo.nScaleFactor,m_StrAutoGRRFilePath);

	WritePrivateProfileString("GRRInfo","strGRRImagePath",m_tGRRParamInfo.strGRRImagePath,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nYPLeftColumn",m_tGRRParamInfo.nYPLeftColumn,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nYPRightColumn",m_tGRRParamInfo.nYPRightColumn,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nYPLowThresh",m_tGRRParamInfo.nYPLowThresh,m_StrAutoGRRFilePath);
	WritePrivateProfileInt("GRRInfo","nYPHighThresh",m_tGRRParamInfo.nYPHighThresh,m_StrAutoGRRFilePath);
}
void CAutoGRR::Action()
{
	     HTuple hv_ImageFiles,hvLength;
		 list_image_files((HTuple)(m_tGRRParamInfo.strGRRImagePath), "default", HTuple(), &hv_ImageFiles);
		 TupleLength(hv_ImageFiles,&hvLength);
		 HObject ho_Image;
		 HTuple hv_Width, hv_Height;
		 HTuple hvPointNum,hv_RepeatArea;
		 HObject hoFLPRectangle,hoFLPObj;
		 HObject ho_YPRegions;
		 int i = 0;
		 while (i < hvLength)
		 {
			 hoFLPRectangle.GenEmptyObj();
			 hoFLPObj.GenEmptyObj();
			 ho_YPRegions.GenEmptyObj();

			 ReadImage(&ho_Image, HTuple(hv_ImageFiles[i]));
			 GetImageSize(ho_Image, &hv_Width, &hv_Height);
			 GetFLPDetectInfo(ho_Image,&hoFLPRectangle,&hoFLPObj,hv_Height,&hvPointNum);
			 GetYPInfo(ho_Image, &ho_YPRegions, hv_Height, &hv_RepeatArea);
			 HObject hoConCatRegions, hoUnionRegion;
			 hoConCatRegions.GenEmptyObj();
			 ConcatObj(hoFLPObj, ho_YPRegions, &hoConCatRegions);
			 Union1(hoConCatRegions, &hoUnionRegion);
			 if (hvPointNum.Length() == 3 && (hvPointNum[0] + hvPointNum[2] == 11))
				WriteFLPImage(hoUnionRegion, ho_Image);
			 else
				WriteFLPImage(hoUnionRegion, ho_Image, 0);
			 i++;
			 Sleep(2000);
		 }
		 WriteResult(hv_RepeatArea);
}
void CAutoGRR::WriteResult(HTuple& hvYPArea)//自动存储点检的数据
{
	if (hvYPArea.Length()%10==0 && hvYPArea.Length()>0)
	{

		CString strFilePath, strFileDirecor, strItem;
		SYSTEMTIME tSystemTime;
		GetLocalTime(&tSystemTime);
		strFileDirecor.Format("%s\\GRR结果", m_tGRRParamInfo.strGRRImagePath);
		strFilePath.Format("%s\\GRR瑕疵重复性检测记录-%02d.%02d.%02d.%02d.csv", strFileDirecor,tSystemTime.wYear,tSystemTime.wMonth,tSystemTime.wDay,tSystemTime.wHour);

		HTuple bExists;
		CString strName = "";
		FileExists(HTuple(strFilePath), &bExists);
		if (0 == bExists)
		{
			CreateMultilevelPath(strFileDirecor);
			ofstream outFile;
			outFile.open(strFilePath, ios::out | ios::app);
			CString strTemp;
			for (size_t i = 1; i < 11; i++)
			{
				strTemp.Format("第%d个点(mm),", i);
				strName += strTemp;
			}
			strItem.Format("%s,%s", "时间", strName);
			outFile << strItem << endl;
			outFile.close();
		}
		try
		{
			CString strTime;
			CString strTemp;
			strTime.Format("%02d%02d%02d %02d:%02d:%02d:%03d", tSystemTime.wYear, tSystemTime.wMonth, tSystemTime.wDay, tSystemTime.wHour, tSystemTime.wMinute, tSystemTime.wSecond, tSystemTime.wMilliseconds);
			CString strValue;
			HTupleVector  YPDataVector(1);
			YPDataVector = HTupleVector(hvYPArea, 10);
			HTuple hv_DataVectorLength;
			hv_DataVectorLength = HTuple(YPDataVector.Length());

			for (int i = 0; i<hv_DataVectorLength.I(); i++)
			{
				strValue="";
				for (int j=0;j<10;j++)
				{

					strTemp.Format("%.3f,", ((const HTuple&)YPDataVector[0].T())[j].D() + (((const HTuple&)YPDataVector[i].T())[j].D() - ((const HTuple&)YPDataVector[0].T())[j].D()) / MAX(m_tGRRParamInfo.nScaleFactor,1));
					strValue+=strTemp;
				}
				strItem.Format("%s,%s", strTime, strValue);
				ofstream outFile;
				CString strLeftInfo, StrRightInfo;
				outFile.open(strFilePath, ios::out | ios::app);
				outFile << strItem << endl;
				outFile.close();
			}

		}
		catch (HException& e)
		{

		}
	}
}

void CAutoGRR::list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
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

// Local procedures 
void CAutoGRR::GetFLPDetectInfo (HObject ho_Image, HObject *ho_FLPRectangle, HObject *ho_FLPPointObj, 
	HTuple hv_Height, HTuple *hv_PointNumber)
{

	try
	{
		// Local iconic variables
		HObject  ho_ImageReduced, ho_Region, ho_ConnectedRegions;
		HObject  ho_WhiteBackground, ho_ImageReduced1, ho_Region1;
		HObject  ho_ConnectedRegions1, ho_SelectedRegions, ho_SortedRegions;
		HObject  ho_ObjectSelected, ho_RectangleDetectRegion, ho_ImageReduced2;
		HObject  ho_Region2, ho_ConnectedRegions2, ho_SelectedRegions1;

		// Local control variables
		HTuple  hv_dColumnResolution, hv_dStandardArea;
		HTuple  hv_strFilePath, hv_UsedThreshold, hv_UsedThreshold1;
		HTuple  hv_RowTop, hv_ColumnTop, hv_RowBot, hv_Number1;
		HTuple  hv_j, hv_Row0, hv_Column0, hv_Row1, hv_Column1;
		HTuple  hv_UsedThreshold2, hv_Number;
		HTuple hvFLPLeftColumn,hvFLPRightColumn;
		(*hv_PointNumber) = HTuple();
		GenEmptyObj(&(*ho_FLPPointObj));
		hv_dColumnResolution =g_vec_ColumnResolution[m_nSide];;
		hv_dStandardArea = m_tGRRParamInfo.nFLPSizeLimit;
		hv_strFilePath = (HTuple)m_tGRRParamInfo.strGRRImagePath;
		hvFLPLeftColumn=m_tGRRParamInfo.nFLPLeftColumn;
		hvFLPRightColumn=m_tGRRParamInfo.nFLPRightColumn;

		GenRectangle1(&(*ho_FLPRectangle), 200, hvFLPLeftColumn, hv_Height-200,  hvFLPRightColumn);
		ReduceDomain(ho_Image, (*ho_FLPRectangle), &ho_ImageReduced);
		BinaryThreshold(ho_ImageReduced, &ho_Region, "max_separability", "light", &hv_UsedThreshold);
		ClosingRectangle1(ho_Region, &ho_Region, 11, 11);
		Connection(ho_Region, &ho_ConnectedRegions);
		SelectShapeStd(ho_ConnectedRegions, &ho_WhiteBackground, "max_area", 70);
		ErosionRectangle1(ho_WhiteBackground, &ho_WhiteBackground, 21, 21);

		//**筛选出符合要求的0.25 0.2的点

		ReduceDomain(ho_Image, ho_WhiteBackground, &ho_ImageReduced1);
		BinaryThreshold(ho_ImageReduced1, &ho_Region1, "max_separability", "dark", &hv_UsedThreshold1);
		ClosingRectangle1(ho_Region1, &ho_Region1, 5/hv_dColumnResolution, 11);
		Connection(ho_Region1, &ho_ConnectedRegions1);
		SelectShape(ho_ConnectedRegions1, &ho_SelectedRegions, (HTuple("width").Append("height")), 
			"and", (5/hv_dColumnResolution).TupleConcat(5), (15/hv_dColumnResolution).TupleConcat(30/hv_dColumnResolution));
		SortRegion(ho_SelectedRegions, &ho_SortedRegions, "first_point", "true", "column");
		//***计算顶部的坐标
		SmallestRectangle1(ho_WhiteBackground, &hv_RowTop, &hv_ColumnTop, &hv_RowBot, &hv_ColumnTop);
		CountObj(ho_SortedRegions, &hv_Number1);

		{
			HTuple end_val25 = hv_Number1;
			HTuple step_val25 = 1;
			for (hv_j=1; hv_j.Continue(end_val25, step_val25); hv_j += step_val25)
			{
				SelectObj(ho_SortedRegions, &ho_ObjectSelected, hv_j);
				SmallestRectangle1(ho_ObjectSelected, &hv_Row0, &hv_Column0, &hv_Row1, &hv_Column1);
				//**下面
				if (0 != (int((hv_RowTop+hv_RowBot)<(hv_Row0+hv_Row1))))
				{
					GenRectangle1(&ho_RectangleDetectRegion, hv_RowTop, hv_Column0, hv_Row0-15, 
						hv_Column1);
				}
				else
				{
					GenRectangle1(&ho_RectangleDetectRegion, hv_Row1+15, hv_Column0, hv_RowBot, 
						hv_Column1);
				}
				ReduceDomain(ho_ImageReduced1, ho_RectangleDetectRegion, &ho_ImageReduced2);

				BinaryThreshold(ho_ImageReduced2, &ho_Region2, "max_separability", "dark", &hv_UsedThreshold2);
				Connection(ho_Region2, &ho_ConnectedRegions2);
				SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", hv_dStandardArea, 
					3*hv_dStandardArea);
				CountObj(ho_SelectedRegions1, &hv_Number);
				ConcatObj((*ho_FLPPointObj), ho_SelectedRegions1, &(*ho_FLPPointObj));
				(*hv_PointNumber) = (*hv_PointNumber).TupleConcat(hv_Number);


			}




		}

	}catch(HException& e)
	{



	}
	return;
}

void CAutoGRR::GetYPInfo (HObject ho_Image, HObject *ho_SelectedRegions2, HTuple hv_Height, 
    HTuple *hv_RepeatArea)
{
	try
	{
		// Local iconic variables
		HObject  ho_TTLBROI_0, ho_ImageReduced3, ho_Region3;
		HObject  ho_RegionFillUp, ho_RegionOpening, ho_RegionErosion;
		HObject  ho_ConnectedRegions4, ho_SelectedRegions3, ho_ImageReduced4;
		HObject  ho_ImageOpening, ho_Regions, ho_ConnectedRegions3;
		HObject  ho_RegionUnion1;

		// Local control variables
		HTuple  hv_dRowResolution, hv_dColumnResolution;
		HTuple  hv_Row1, hv_Column1, hv_Row2, hv_Column2, hv_Area;
		HTuple  hv_Row, hv_Column;
		HTuple hvYPLeftColumn,hvYPRightColumn,hvHighThresh,hv_Lowthresh;
		hv_dRowResolution = g_vec_RowResolution[m_nSide];
		hv_dColumnResolution = g_vec_ColumnResolution[m_nSide];

		hvYPLeftColumn=m_tGRRParamInfo.nYPLeftColumn;
		hvYPRightColumn=m_tGRRParamInfo.nYPRightColumn;
		hv_Lowthresh=m_tGRRParamInfo.nYPLowThresh;
		hvHighThresh=m_tGRRParamInfo.nYPHighThresh;
		GenRectangle1(&ho_TTLBROI_0, 0, hvYPLeftColumn, hv_Height-1, hvYPRightColumn);
		ReduceDomain(ho_Image, ho_TTLBROI_0, &ho_ImageReduced3);
		Threshold(ho_ImageReduced3, &ho_Region3, hv_Lowthresh, hvHighThresh);
		FillUp(ho_Region3, &ho_RegionFillUp);
		OpeningRectangle1(ho_RegionFillUp, &ho_RegionOpening, 31, 11);
		ErosionRectangle1(ho_RegionOpening, &ho_RegionErosion, 31, 31);
		Connection(ho_RegionErosion, &ho_ConnectedRegions4);
		SelectShapeStd(ho_ConnectedRegions4, &ho_SelectedRegions3, "max_area", 70);
		SmallestRectangle1(ho_SelectedRegions3, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
		if (0 != (HTuple(int(hv_Row1>20)).TupleAnd(int((hv_Row2+20)<(hv_Height-1)))))
		{
			ReduceDomain(ho_ImageReduced3, ho_SelectedRegions3, &ho_ImageReduced4);
			GrayOpeningShape(ho_ImageReduced4, &ho_ImageOpening, 11, 11, "octagon");
			AutoThreshold(ho_ImageOpening, &ho_Regions, 2);
			Connection(ho_Regions, &ho_ConnectedRegions3);
			SelectShape(ho_ConnectedRegions3, &(*ho_SelectedRegions2), "area", "and", 11,1000);
			AreaCenter((*ho_SelectedRegions2), &hv_Area, &hv_Row, &hv_Column);

			if (hv_Area.Length()==10)
			{
				(*hv_RepeatArea) = (*hv_RepeatArea).TupleConcat(((hv_Area*hv_dRowResolution)*hv_dRowResolution)*hv_dColumnResolution);
			}else
			{
				GenEmptyObj(&(*ho_SelectedRegions2));
			}


		}

	}catch(HException& e)
	{

	}

  return;
}

void CAutoGRR::WriteFLPImage (HObject ho_SelectedRegions1, HObject ho_Image,int nOK)
{

  // Local iconic variables
  HObject  ho_RegionUnion, ho_Image1, ho_MultiChannelImage;

  // Local control variables

  Union1(ho_SelectedRegions1, &ho_RegionUnion);
  CopyImage(ho_Image, &ho_Image1);
  OverpaintRegion(ho_Image1, ho_RegionUnion, 255, "fill");
  Compose3(ho_Image1, ho_Image, ho_Image, &ho_MultiChannelImage);
  CString strImageSaveName,strImageSaveFile;
  strImageSaveFile.Format("%s\\GRR结果",m_tGRRParamInfo.strGRRImagePath);
  CreateMultilevelPath(strImageSaveFile);
  SYSTEMTIME tTime;
  GetLocalTime(&tTime);
  if (nOK==0)
	  strImageSaveName.Format("%s\\%02d%02d%02d-%02d-%02d-%02d-NG",strImageSaveFile,tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
 else
  strImageSaveName.Format("%s\\%02d%02d%02d-%02d-%02d-%02d-OK",strImageSaveFile,tTime.wYear,tTime.wMonth,tTime.wDay,tTime.wHour,tTime.wMinute,tTime.wSecond);
  WriteImage(ho_MultiChannelImage,"jpg",0,(HTuple)strImageSaveName);
  return;
}
