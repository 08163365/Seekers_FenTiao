#pragma once

#define  LabelNum 1

enum EMCount
{
	
	   LabelReceiveNumAddress,//�����ź���Ŀ260
	   LabelActionNumAddress,//������Ŀ20
	   SensorNumAddress,//610
	   SiLianBiaoNumAddress,//152
	   SiLianBiaoDaBiaoNumAddress,//156
	   PingBiNumAddress,//52
};



struct LabelWarnAddress
{

		int   nLabelEnableLabelAddress;//���ñ�� ��ַ��4212 ����д1 �ر�д0

		
		int   nLabelMissAlaramAddress;//©����4300
	    int   nLabelSettingFailedAddress;//����ʧ�ܱ���4316

		


		int   nLabelSumAlarmAddress;//�ۼƴ�곬�ޱ���4336
		int   nLabelContinueAlarmAddress;//��������
	    int   nLocateRightAddress;///��λ�ź� 1:D4356
		int   nLabelBreakAlarmAddress;//�ϴ�����4372

		int   nLabelCuoWeiAddress;//��ǩ��λ4302
		int   nLabelEmptyAddress;//�ޱ��������ʧ�ܳ��걨��4408
		int   nLabelNOUseAddress;//����δ����4356

	    
		int  nFourLabelReceieveAddress;//��������յ�ַ D4432
		int  nFourLabelBackAddress;//   �����귴����ַ D4450

		///count
		int   nLabelReceiveNumAddress;//�����ź���Ŀ260
		int   nLabelActionNumAddress;//������Ŀ20
		int   nSensorNumAddress;//610
		int   nSiLianBiaoNumAddress;//152
		int   nSiLianBiaoDaBiaoNumAddress;//156
		int   nPingBiNumAddress;//52
		

};





typedef struct tagTLabellerInfo
{

	int nLabelPort;//������˿�
	CString strLabelIpAddress;//�����IP��ַ
	int  nEnableLabelName;//ʹ�������������

	int  nHeartbeatAddress;//������ַ��4200 ����200ms
	int  nStartBatchAddress;//Ĥ��ʼ����//��ʼд1 ����д0 ��ַ��4204
	int  nResetAlarmAddress;//������λ//��ַ��4208

	int  nShouDongChuBiaoMoshi;//4734
	int nLabelMotorAlarmAddress;//������ŷ����� ��ַ��4332  1Ϊ��״̬
	int nLabelSumAlarmAddress;//�ܴ���������� ��ַ 4336
	int nManual1_ZD_SD_Address;//D502ֵ��1��Ϊ�����1��ǩ���� 2��Ϊ�����2��ǩ���� 101��Ϊ�����1�ֶ����� 102��Ϊ�����2�ֶ�����
	int nQiGangYaXiaYiChangAddr;//����ѹ���쳣D4630
	int nQiGangTaiQiYiChangAddr;//����̧���쳣D4632
	//Param
	int nDuoLunZhiJingAddress;//100
	int nBianMaQiMaiChongLiangAddr;//106
	int nJiPianEALengthAddr;//504
	int nBiaoQianLengthAddr;//320
	int nPingBiXinHaoLengthAddr;//330
	int nDangeLeijiAddr;//160
	int nZongDabiaoLeijiAddr;//168
	int nLianXuDabiaoBaoJingAddr;//145
	int nSiLianBiaoWeiZhiAddr;//4802
	int nJiGuang1JuLiAddr;//4804
	int nJiGuang2JuLiAddr;//4806
	int nSiLianBiaoJianGeAddr;//4550
	int nBiaoQianDuanDaiShuAddr;//166
	//Control
	int nLianJiJianCeEnableAddr;//4750
	int nLeiJiaDaBiaoBaoJingEnableAddr;//4754
	int nLeiXuBaoJingEnableAddr;//4758
	int nMarkBiaoZhiEnableAddr;//4760
	int nSiLianBiaoEnableAddr;//4762

	



	////������
	int  nFourLabelEanbleFlagAddress;//�����꿪��D4762
	LabelWarnAddress tLabelWarnAddress[LabelNum];





}LabellerInfo,*PLabellerInfo;
