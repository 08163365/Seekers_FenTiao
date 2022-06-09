#pragma once




struct GRRParamInfo
{
	BOOL bEnableUseAutoGRR;//�Ƿ������Զ�GRR
	int  nCameraIndex;//��������
	CString strGRRImagePath;//����GRRͼƬ��·��
	int   nFLPLeftColumn;//����Ƭ�������
	int   nFLPRightColumn;//����Ƭ�Ҳ�����
	int   nFLPSizeLimit;//����Ƭ��ȡ���Ƴߴ�

	int   nYPLeftColumn;//��Ʒ�������
	int   nYPRightColumn;//��Ʒ�Ҳ�����
	int   nYPLowThresh;//��Ʒ����ֵ
	int   nYPHighThresh;//��Ʒ����ֵ
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
	int          m_nSide;//�ڲ໹�����
	GRRParamInfo m_tGRRParamInfo;//GRR��ʼ��������Ϣ
};

