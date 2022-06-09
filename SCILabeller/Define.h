#pragma once

#define  LabelNum 1

enum EMCount
{
	
	   LabelReceiveNumAddress,//接收信号数目260
	   LabelActionNumAddress,//出标数目20
	   SensorNumAddress,//610
	   SiLianBiaoNumAddress,//152
	   SiLianBiaoDaBiaoNumAddress,//156
	   PingBiNumAddress,//52
};



struct LabelWarnAddress
{

		int   nLabelEnableLabelAddress;//启用标机 地址：4212 启用写1 关闭写0

		
		int   nLabelMissAlaramAddress;//漏贴标4300
	    int   nLabelSettingFailedAddress;//整定失败报警4316

		


		int   nLabelSumAlarmAddress;//累计打标超限报警4336
		int   nLabelContinueAlarmAddress;//连续报警
	    int   nLocateRightAddress;///到位信号 1:D4356
		int   nLabelBreakAlarmAddress;//断带报警4372

		int   nLabelCuoWeiAddress;//标签错位4302
		int   nLabelEmptyAddress;//无标或者整定失败出标报警4408
		int   nLabelNOUseAddress;//打标机未启用4356

	    
		int  nFourLabelReceieveAddress;//四连标接收地址 D4432
		int  nFourLabelBackAddress;//   四连标反馈地址 D4450

		///count
		int   nLabelReceiveNumAddress;//接收信号数目260
		int   nLabelActionNumAddress;//出标数目20
		int   nSensorNumAddress;//610
		int   nSiLianBiaoNumAddress;//152
		int   nSiLianBiaoDaBiaoNumAddress;//156
		int   nPingBiNumAddress;//52
		

};





typedef struct tagTLabellerInfo
{

	int nLabelPort;//贴标机端口
	CString strLabelIpAddress;//贴标机IP地址
	int  nEnableLabelName;//使用贴标机的名称

	int  nHeartbeatAddress;//心跳地址：4200 周期200ms
	int  nStartBatchAddress;//膜卷开始结束//开始写1 结束写0 地址：4204
	int  nResetAlarmAddress;//报警复位//地址：4208

	int  nShouDongChuBiaoMoshi;//4734
	int nLabelMotorAlarmAddress;//贴标机伺服报警 地址：4332  1为有状态
	int nLabelSumAlarmAddress;//总打标数量报警 地址 4336
	int nManual1_ZD_SD_Address;//D502值：1，为贴标机1标签整定 2，为贴标机2标签整定 101，为贴标机1手动出标 102，为贴标机2手动出标
	int nQiGangYaXiaYiChangAddr;//气缸压下异常D4630
	int nQiGangTaiQiYiChangAddr;//气缸抬起异常D4632
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

	



	////四连标
	int  nFourLabelEanbleFlagAddress;//四连标开关D4762
	LabelWarnAddress tLabelWarnAddress[LabelNum];





}LabellerInfo,*PLabellerInfo;
