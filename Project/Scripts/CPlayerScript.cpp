#include "CPlayerScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CGameMgr.h"
#include "CAnimator2D.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CResMgr.h"
#include "CEffectScript.h"

CPlayerScript::CPlayerScript()
	:m_fHitTime(0.f)
	, m_bHit(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CPLAYERSCRIPT);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Awake()
{
}

int CPlayerScript::Update()
{
	//Death();

	if (m_eState == OBJ_STATE::HIT)
	{
		if (!m_bHit)
		{
			CGameObj* pHit = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"PlayerHit");
			Vec3 vPos = TransForm()->GetPos();
			vPos.z = 100.f;
			pHit->GetTransForm()->SetPos(vPos);
			pHit->GetAnimator2D()->PlayAnim(L"PlayerHit", false);
			pHit->AddComponent(new CEffectScript);
			m_eState = OBJ_STATE::IDLE;
		}
		m_bHit = true;
	}

	if (m_bHit)
	{
		m_fHitTime += DT;
		if (m_fHitTime > 0.3f)
		{
			m_fHitTime = 0.f;
			m_bHit = false;
		}
	}

	Vec3 vPos = TransForm()->GetPos(); // 이것을 이제 충돌체에 좌표로 하고 적용시킬 예정
	//vPos.z = (vPos.y + 10000) / 1000.f ;

	if (CGameMgr::GetInst()->GetTurn())
	{
		if (KEYHOLD(KEY_TYPE::KEY_A) || KEYHOLD(KEY_TYPE::KEY_D) || KEYHOLD(KEY_TYPE::KEY_W)
			|| KEYHOLD(KEY_TYPE::KEY_S))
			CGameMgr::GetInst()->AddTurn(ANIM_TYPE::MOVE, vPos);
		else if(KEYTAB(KEY_TYPE::KEY_RBTN))
		{
			//CGameMgr::GetInst()->AddTurn(ANIM_TYPE::CRASH, CKeyMgr::GetInst()->GetMousePos());
		}
	}
	else
	{
		/*if (KEYHOLD(KEY_TYPE::KEY_A))
			vPos.x -= 100 * DT;
		if (KEYHOLD(KEY_TYPE::KEY_D))
			vPos.x += 100 * DT;
		if (KEYHOLD(KEY_TYPE::KEY_W))
			vPos.y += 100 * DT;
		if (KEYHOLD(KEY_TYPE::KEY_S))
			vPos.y -= 100 * DT;*/
	}

		TransForm()->SetPos(vPos);
	//TransForm()->SetAddPos(Vec3{-g_World.mtxView._41,-g_World.mtxView._42,0.f});

	return 0;
}

void CPlayerScript::CollisionEnter(CCollider* _pColl)
{
	//wstring wcsName = CSceneMgr::GetInst()->GetLayerName(_pColl->GetOwner()->GetID());// 다음으로 할일 몬스터 총알에 부딪히면 데미지가 가고 그 총알을 삭제 시킨다.

	//if (wcsName == L"Skill")
	//{
	//}
}

void CPlayerScript::Collision(CCollider * _pColl)
{
}

void CPlayerScript::CollisionExit(CCollider * _pColl)
{
}