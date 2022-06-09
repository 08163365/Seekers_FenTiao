#pragma once



class CPowerLimit
{
public:
	CPowerLimit(void);
	~CPowerLimit(void);
public:
	 BOOL  IsAllContinue();
	 BOOL  IsEngineAndFactoryContinue();
	 BOOL  IsJustFactoryContinue();
	  void ClearFlag();
	  int  GetUserIndex();
	  BOOL GetUserPower(CString strUserName,CString strJobNum,CString strPassWord);
public:
	 BOOL   m_bOperatorPower;//����ԱȨ��
	 BOOL   m_bEngineerPower;//����ʦȨ��
	 BOOL   m_bSuperEngineerPower;//��������ʦȨ��
	 BOOL   m_bFactoryPower;//����Ȩ��
public:
	BOOL addData(PowerInfo data);
	void ReadParam();//��ȡȨ����Ϣ
	void WriteParam();//д��Ȩ��
	BOOL MatchPower(const PowerInfo & TempPowerInfo);//ƥ��Ȩ��
	void  SetStatus(CString strInfo);
	CString GetStatus();
private:
	CString m_strCurrentStatus;//��ǰ״̬
public:
	CString m_strFilePath;
	int m_nEngineerPowerList;
	std::vector<PowerInfo>m_vec_EngineerPowerList;
	CRITICAL_SECTION m_csPowChange;//������Ķ�������������ĵ�ʱ����Ҫ��ס����ֹ����ϵͳ�Ǳ����ڼ�飬�������ȥ���Ĳ���
};

