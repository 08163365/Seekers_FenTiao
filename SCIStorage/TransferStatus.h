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
	BOOL  m_bTransferStaus;//���ݵ�״̬ 0��δ��� 1�������
};

