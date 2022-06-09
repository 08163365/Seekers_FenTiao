#include "StdAfx.h"
#include "Define.h"
#include "TransferStatus.h"


CTransferStatus::CTransferStatus(void)
{
	m_bTransferStaus=FALSE;
	InitializeCriticalSection(&m_csStaus);
}


CTransferStatus::~CTransferStatus(void)
{
	DeleteCriticalSection(&m_csStaus);
}
BOOL CTransferStatus::GetStaus()
{
	{
		CriticalSectionHelper Lock(&m_csStaus);
		return m_bTransferStaus;
	}

}
void CTransferStatus::SetStauts(BOOL bFlag)
{
	{
		CriticalSectionHelper Lock(&m_csStaus);
		m_bTransferStaus=bFlag;
	}
	
}