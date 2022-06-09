#pragma once




struct GRRParamInfo
{
	BOOL bEnableUseAutoGRR;//是否启用自动GRR
	int  nCameraIndex;//相机的序号
	CString strGRRImagePath;//加载GRR图片的路径
	int   nFLPLeftColumn;//菲林片左侧坐标
	int   nFLPRightColumn;//菲林片右侧坐标
	int   nFLPSizeLimit;//菲林片提取限制尺寸

	int   nYPLeftColumn;//样品左侧坐标
	int   nYPRightColumn;//样品右侧坐标
	int   nYPLowThresh;//样品低阈值
	int   nYPHighThresh;//样品高阈值
	int   nScaleFactor;
};
class CAutoGRR
{
public:
	CAutoGRR(void);
	~CAutoGRR(void);

public:
	void list_image_files (HTuple hv_ImageDirectory, HTuple hv_Extensions, HTuple hv_Options, 
		HTuple *hv_ImageFiles);
	// Local procedures 
	void GetFLPDetectInfo (HObject ho_Image, HObject *ho_FLPRectangle, HObject *ho_FLPPointObj, 
		HTuple hv_Height, HTuple *hv_PointNumber);
	void GetYPInfo (HObject ho_Image, HObject *ho_SelectedRegions2, HTuple hv_Height, 
		HTuple *hv_RepeatArea);
	void WriteFLPImage (HObject ho_SelectedRegions1, HObject ho_Image,int nOK=1);

public:
	void ReadParam();
	void WriteParam();
	void Action();
	void WriteResult(HTuple& hvYPArea);
	CString      m_StrAutoGRRFilePath;
	int          m_nSide;//内侧还是外侧
	GRRParamInfo m_tGRRParamInfo;//GRR初始化参数信息
};

