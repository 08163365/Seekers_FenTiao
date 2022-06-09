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
	 BOOL   m_bOperatorPower;//操作员权限
	 BOOL   m_bEngineerPower;//工程师权限
	 BOOL   m_bSuperEngineerPower;//超级工程师权限
	 BOOL   m_bFactoryPower;//厂家权限
public:
	BOOL addData(PowerInfo data);
	void ReadParam();//读取权限信息
	void WriteParam();//写入权限
	BOOL MatchPower(const PowerInfo & TempPowerInfo);//匹配权限
	void  SetStatus(CString strInfo);
	CString GetStatus();
private:
	CString m_strCurrentStatus;//当前状态
public:
	CString m_strFilePath;
	int m_nEngineerPowerList;
	std::vector<PowerInfo>m_vec_EngineerPowerList;
	CRITICAL_SECTION m_csPowChange;//当该类的对象参数发生更改的时候，需要锁住，防止出现系统那边正在检查，这边正好去更改参数
};

