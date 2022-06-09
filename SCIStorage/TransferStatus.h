#pragma once
class CTransferStatus
{
public:
	CTransferStatus(void);
	~CTransferStatus(void);
public:
	BOOL GetStaus();
	void SetStauts(BOOL bFlag=TRUE);
	CRITICAL_SECTION m_csStaus;
private:
	BOOL  m_bTransferStaus;//传递的状态 0是未完成 1是已完成
};

