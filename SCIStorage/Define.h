#pragma once
class CriticalSectionHelper
{
public:
	CriticalSectionHelper(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	}

	~CriticalSectionHelper()
	{
		LeaveCriticalSection(m_pcs);
		m_pcs = NULL;
	}

private:
	CRITICAL_SECTION *m_pcs;
};
typedef struct tagTWRITEDISK
{
	BOOL bSaveProcessInfo;//写入过程信息
	BOOL bSmallImage;//写入小图
	BOOL bErrorRegion;//写入错误Region
	BOOL bSize;//写入尺寸
	BOOL bDeleteOK;//换卷的时候删除正常图片
	int  nType;//当发生错误的时候 如果为0只存图 如果为1大图小图一起存
	int  nSaveDays;//缓存天数
	CString strImageFormat;//图像格式
	BOOL bSaveAllGrabberImage;//存储所有原图
	BOOL bWriteInLocation;//启用本地存储
	BOOL bSaveMarkImage;//保存Mark图像

	int     nSaveImageType;//0:NG 1:全部
	CString StrImageSavePath;//图像存储路径
	BOOL bDefinedPath;	//0：默认膜卷路径 1：自定义路径
	CString StrMachineSN;//机台号

	CString StrUpLoadImagePath;//蓝标图像上传路径
	BOOL    bEnableUpLoadImage;//启用蓝标图像上传


}WRITEDISK,*PWRITEDISK;