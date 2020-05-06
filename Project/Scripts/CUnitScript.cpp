#include "CUnitScript.h"
#include "CEventMgr.h"
#include "CTransForm.h"
#include "CGameMgr.h"

void CUnitScript::SetDamage(float _fDamage)
{
	if (CGameMgr::GetInst()->GetPlayTurn())
	{
		m_fCurHP -= _fDamage;
		m_eState = OBJ_STATE::HIT;
	}
	else
	{
		if (m_eState != OBJ_STATE::BIRTH && m_eState != OBJ_STATE::HIT)
		{
			m_fCurHP -= _fDamage;
			m_eState = OBJ_STATE::HIT;
		}
	}
}

void CUnitScript::SetPlayerDamage(float _fDamage)
{
	m_fCurHP -= _fDamage;
	m_eState = OBJ_STATE::HIT;
}

bool CUnitScript::Death()
{
	if (!GetOwner()->GetDead())
	{
		if (m_fCurHP <= 0.f)
		{
			GetOwner()->IsDead();
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 };
			CEventMgr::GetInst()->AddEvent(event);
			return true;
		}
	}
	return false;
}

void CUnitScript::SetKnockback(Vec3 _vDir)
{
	if (m_eState != OBJ_STATE::BIRTH && m_eState != OBJ_STATE::HIT)
	{
		m_vKnockbackDir = _vDir;
		m_vKnockbackPos = TransForm()->GetPos() + (100.f  * _vDir);
	}
}

CUnitScript::CUnitScript()
	:m_fCurHP(100.f)
	, m_fAccTime(0.f)
	, m_fMaxHP(100.f)
{
}

CUnitScript::~CUnitScript()
{
	
}