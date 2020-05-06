#include "CBulletScript.h"
#include "CTransForm.h"
#include "CScriptMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CEventMgr.h"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CUnitScript.h"

void CBulletScript::CollisionEnter(CCollider * _pColl)
{
	if (m_eTeamType == TEAM_TYPE::PLAYER && CSceneMgr::GetInst()->GetLayerName(_pColl->GetLayerID()) == L"Monster")
	{
		vector<CScript*> vecScript = _pColl->GetOwner()->GetScript();
		for (int i = 0; i < vecScript.size(); ++i)
		{
			CUnitScript* pUnit = dynamic_cast<CUnitScript*>(vecScript[i]);

			if (pUnit != nullptr)
			{
				pUnit->SetKnockback(m_vDir);
				pUnit->SetDamage(30);
				break;
			}
		}
	}
	else if (m_eTeamType == TEAM_TYPE::MONSTER && CSceneMgr::GetInst()->GetLayerName(_pColl->GetLayerID()) == L"Player")
	{
		vector<CScript*> vecScript = _pColl->GetOwner()->GetScript();
		for (int i = 0; i < vecScript.size(); ++i)
		{
			CUnitScript* pUnit = dynamic_cast<CUnitScript*>(vecScript[i]);

			if (pUnit != nullptr)
			{
				pUnit->SetPlayerDamage(1);

				if (m_bDead == false)
				{
					tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 };
					CEventMgr::GetInst()->AddEvent(event);
				}
				m_bDead = true;

				break;
			}
		}
	}
}

void CBulletScript::Collision(CCollider * _pColl)
{
}

void CBulletScript::CollisionExit(CCollider * _pColl)
{
}


CBulletScript::CBulletScript()
	:m_bDead(false)
	, m_fRange(200.f)
	, m_eTeamType(TEAM_TYPE::PLAYER)
	, m_fZSort(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CBULLETSCRIPT);
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::Awake()
{
	m_vTargetPos = TransForm()->GetPos();
	m_vMousePos.z = m_vTargetPos.z;
	Vec3 vDir = m_vMousePos - m_vTargetPos;
	vDir.Normalize();
	TransForm()->SetRotaion(Vec3(0.f, 0.f, atan2f(vDir.y, vDir.x)));
}

int CBulletScript::Update()
{
	if (CSceneMgr::GetInst()->GetStop() && m_eTeamType == TEAM_TYPE::MONSTER)
		return 0;

	m_vDir = m_vMousePos - m_vTargetPos;
	m_vDir.Normalize();

	TransForm()->SetRotaion(Vec3(0.f,0.f, atan2f(m_vDir.y, m_vDir.x)));
	Vec3 vPos = TransForm()->GetPos();
	vPos += m_vDir * 800 * DT;
	vPos.z = m_vTargetPos.z;
	float fDistance = Vec3::Distance(m_vTargetPos,vPos);
	if (fDistance >= m_fRange)
	{
		if (m_bDead == false)
		{
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 };
			CEventMgr::GetInst()->AddEvent(event);
		}
		m_bDead = true;
	}

	TransForm()->SetPos(vPos);
	return 0;
}