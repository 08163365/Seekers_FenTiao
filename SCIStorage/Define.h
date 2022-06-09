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
	BOOL bSaveProcessInfo;//д�������Ϣ
	BOOL bSmallImage;//д��Сͼ
	BOOL bErrorRegion;//д�����Region
	BOOL bSize;//д��ߴ�
	BOOL bDeleteOK;//�����ʱ��ɾ������ͼƬ
	int  nType;//�����������ʱ�� ���Ϊ0ֻ��ͼ ���Ϊ1��ͼСͼһ���
	int  nSaveDays;//��������
	CString strImageFormat;//ͼ���ʽ
	BOOL bSaveAllGrabberImage;//�洢����ԭͼ
	BOOL bWriteInLocation;//���ñ��ش洢
	BOOL bSaveMarkImage;//����Markͼ��

	int     nSaveImageType;//0:NG 1:ȫ��
	CString StrImageSavePath;//ͼ��洢·��
	BOOL bDefinedPath;	//0��Ĭ��Ĥ��·�� 1���Զ���·��
	CString StrMachineSN;//��̨��

	CString StrUpLoadImagePath;//����ͼ���ϴ�·��
	BOOL    bEnableUpLoadImage;//��������ͼ���ϴ�


}WRITEDISK,*PWRITEDISK;