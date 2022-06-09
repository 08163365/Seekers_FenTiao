#pragma once

/********************************************************************
    ("INITIALIZE" == strMessage)//��ʼ��
	("ACTION" == strMessage)//���
	("SHOW" == strMessage)//���öԻ���
	��Seekersϵͳ���õ�ͷ�ļ��������������ṹ�����
/***********************************************************************/
typedef struct tagTDETECTINIT//SCIDetect�㷨��̬���ʼ���ṹ��
{
	CString strProductName;//Ʒ������
	int     nGrabberSN;//������ 0��ʾǰ�������1��ʾ�������
	PVOID   pLogView;
	int     nProductType;//��Ʒ����
	int     nCoatType;//Ϳ������

}DETECTINIT,*PDETECTINIT;
typedef struct tagTDETECTINPUT//SCIDetect�㷨��̬������ṹ��
{
	int     nBreakType;
	HObject hoDetectImage;//�ɼ�����ͼƬ
}DETECTINPUT,*PDETECTINPUT;
typedef struct tagTDETECTOUTPUT//SCIDetect�㷨��̬������ṹ��
{
	HTuple  hv_Flag;//������������ TRUEΪok FALSE��NG 2�ǲ���� ÿƬ���
	HTuple  hv_ErrorType;//��������         5
	HTuple  hv_ErrorLevel;//����ȼ�        0
	HTuple  hv_ErrorBelongStrip;//��������һ������0��ʼ

	HTuple  hv_CoatWidth; //Ϳ����  
	HTuple  hv_CoatHeight;//Ϳ����


	HTuple  hv_FoilWidth;    //���Ŀ�
	HTuple  hv_FoilHeight;   //���ĸ�
	HTuple  hv_LocateMissing;//��ⶨλ����ֵ
	HTuple  hv_BaiBian;//�������ڵ��մɱ�
	HTuple  hv_JuanRaoMarkRow;//Mark��������� �����-1 ����û��Mark�� 2017/7/1

	HTuple  hv_SortWidth;//�������ʾ�Ŀ��˳��
	HTuple  hv_Name;//���ݿ������������
	HTuple  hv_AT9EdageToProEdage;//AT9��λ�ߴ�
	HTuple  hv_CeramicWidth;//AT9�ߴ�
	HTuple hv_CpkEdageToProEdage;	//����Ϳ��CPK����,��¼ÿ��Ϳ�������ڱ�


	//��ЪͿ��
	HTuple  hv_LeftEadgePixel;      //�������
	HTuple  hv_RightEadgePixel;     //�Ҳ�����
	HTuple  hv_TopEdagePixel;       //��������
	HTuple  hv_BotEdagePixel;       //�ײ�����
	HTuple  hv_HeadCuoWei;          //ͷ����λ
	HTuple  hv_TailCuoWei;          //β����λ

	HTuple  hv_TripSNTuple;         //��λ����������
	HTuple  hv_CaoWeiWidthTuple;    //��λ���
	HTuple  hv_CaoWeiHeightTuple;   //��λ�߶�
	HTuple  hv_CaoWeiXPositionTuple;//��λX����
	HTuple  hv_CaoWeiYpositionTuple;//��λY����
	
	HTuple  hv_JXHeight;            //��Ъ�߶�
	HTuple  hv_MPLength;            //Ĥ��
	HTuple  hv_SumLength;           //�ܳ�
	HObject ho_JXRegion;            //��Ъ����
	HObject ho_ExtractCaoWeiRegions;//��ȡ�Ĳ�λ����

	//HTuple  hv_Reserve1;//Ԥ���ֶ�1   ������Ъ��Ĥ��
	//HTuple  hv_Reserve2;//Ԥ���ֶ�2   ��Ъ�߶� 
	//HTuple  hv_Reserve3;//Ԥ���ֶ�3   �׼������
	//HTuple  hv_Reserve4;//Ԥ���ֶ�4   ������Ŀ
	//HTuple  hv_Reserve5;//Ԥ���ֶ�5   ��Բ���߶�
	//HTuple  hv_Reserve6;//Ԥ���ֶ�6   ��Բ���߶�
	//HTuple  hv_Reserve7;//Ԥ���ֶ�6    ��©���߶�




	HObject ho_ErrorRegion;//�������� ����   5
	HObject ho_Coat;//Ϳ��
	HObject ho_Foil;//����
	void InitVariable()
	{
		TupleGenConst(2,TRUE,&hv_Flag);//Ĭ��1��OK 0��NG
		hv_ErrorType.Clear();
		hv_ErrorLevel.Clear();
		hv_ErrorBelongStrip.Clear();
		ho_ErrorRegion.GenEmptyObj();
		ho_ExtractCaoWeiRegions.GenEmptyObj();
		ho_JXRegion.GenEmptyObj();

	}
}DETECTOUTPUT,*PDETECTOUTPUT;

typedef struct tagTSPOTCHECKOUTPUT//����㷨����ṹ��
{

	HTuple hv_LeftSpotArea;//���
	HTuple hv_LeftSpotWidth;//���
	HTuple hv_LeftSpotHeight;//���

	HTuple hv_RightSpotArea;//�Ҳ������
	HTuple hv_RightSpotWidth;//�Ҳ������
	HTuple hv_RightSpotHeight;//�Ҳ������

}SPOTCHECKOUTPUT,*PSPOTCHECKOUTPUT;