#include "CBossScript.h"
#include "CSceneMgr.h"
#include "CTransForm.h"
#include "CAnimator2D.h"
#include "CScriptMgr.h"	
#include "TimeMgr.h"
#include "CResMgr.h"
#include "CBulletScript.h"
#include "CMeshRender.h"
#include "CShaderMgr.h"
#include "time.h"
#include "CEffectScript.h"
#include "CEventMgr.h"

CBossScript::CBossScript()
	:m_fNum(68.f)
	, m_fCoolTime(5.f)
	, m_fAttackTime(0.f)
	, m_fSpacing(0.f)
	, m_fHitTime(0.f)
	, m_eAttackType(ATTACK_TYPE::BEAM)
	, m_bTail(false)
	, m_pTarget(nullptr)
{	 
	SetScriptType((UINT)SCRIPT_TYPE::CBOSSSCRIPT);
}	
	
CBossScript::~CBossScript()
{	
}	
	
void CBossScript::Start()
{
	Animator2D()->IsPlay(false);
	Animator2D()->PlayAnim(L"Head", false);
}

void CBossScript::Awake()
{
	Animator2D()->IsPlay(false);
	Animator2D()->PlayAnim(L"Head", false);
}	
	
int CBossScript::Update()
{
	srand(time(0));

	if (CSceneMgr::GetInst()->GetStop())
		return 0;

	if (m_eState == OBJ_STATE::HIT)
	{
		m_fHitTime += DT;
		MeshRender()->GetMtrl()->SetShader(CShaderMgr::GetInst()->FindShader(L"Hit"));

		if (m_fHitTime > 0.5f)
		{
			MeshRender()->GetMtrl()->SetShader(CShaderMgr::GetInst()->FindShader(L"Std"));
			m_fHitTime = 0.f;
			m_eState = OBJ_STATE::IDLE;
		}
	}

	if (m_fCurHP <= 0)
	{
		Vec3 vScale = TransForm()->GetScale();
		Animator2D()->IsPlay(true);
		m_fCurHP = 0.f;
		Animator2D()->PlayAnim(L"Death", false);
		Animator2D()->SetAutoSize(false);
		TransForm()->SetScale(Vec3(1500.f, 750.f,1.f));

		if (Animator2D()->GetAnimFinish())
			Death();

		return 0;
	}

	CGameObj* pPlayer = nullptr;
	if (!CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pPlayer))
		return 0;
	CGameObj* pParent = GetParentOfOwner();
	Vec3 vBossPos;
	if(pParent != nullptr)
		vBossPos = GetParentOfOwner()->GetTransForm()->GetPos() + TransForm()->GetPos();
	else
		vBossPos = TransForm()->GetPos();
	float fDegree = GetAngleDegree(vBossPos, pPlayer->GetTransForm()->GetPos());
	int iNum = 0;
	AnimAngleNoRot(256, fDegree,&iNum);

	if ((int)m_fNum > iNum)
		m_fNum -= DT * 3;
	else if ((int)m_fNum < iNum)
		m_fNum += DT * 3;
	else
		m_fNum = iNum;

	if (m_fNum <= 0)
		m_fNum = 0.f;
	else if (m_fNum >= 129)
		m_fNum = 128.f;

	iNum = m_fNum;

	if (m_fCoolTime <= 0.f) // 5초에 한번씩 공격을  한다.
	{
		if (m_eAttackType == ATTACK_TYPE::BEAM)
		{
			m_fAttackTime += DT;
			m_fSpacing += DT;
			if (m_fAttackTime >= 3.f)
			{
				m_fSpacing = 0.f;
				m_fAttackTime = 0.f;
				m_fCoolTime = 5.f;

				m_eAttackType = ATTACK_TYPE(rand() % 2);
			}
			if (m_fSpacing >= 0.2f)
			{
				m_fSpacing = 0.f;
				float fRad = DegreeToRadian((iNum * 1.40625f) - 180.f);
				CGameObj* pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Skill", L"BossBeam");
				Vec3 vPos = vBossPos;

				pObj->GetTransForm()->SetPos(vPos);
				vPos.x += cosf(fRad) * 100.f;
				vPos.y += sinf(fRad) * 100.f;

				CBulletScript* pBullet = new CBulletScript;
				pBullet->SetMousePos(vPos);
				pBullet->SetTeamType(TEAM_TYPE::MONSTER);
				pObj->AddComponent(pBullet);
				pBullet->Awake();
				pBullet->SetRange(1000.f);
				pObj->GetAnimator2D()->PlayAnim(L"BossBeam", true);
			}
		}
		else if (m_eAttackType == ATTACK_TYPE::TAIL)
		{
			m_fAttackTime += DT;
			if (!m_bTail)
			{
				m_bTail = true;
				m_pTarget = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Targeting");
				CGameObj* pPlayer = nullptr;
				m_pTarget->GetAnimator2D()->PlayAnim(L"Targeting",false);
				if (CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pPlayer))
				{
					Vec3 vPos = pPlayer->GetTransForm()->GetPos();
					vPos.y -= pPlayer->GetTransForm()->GetScale().y * 0.5f;
					vPos.z += 30.f;
					m_pTarget->GetTransForm()->SetPos(vPos);
				}
			}

			if (m_fAttackTime > 1.f)
			{
				CGameObj* pExplpsion = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Explosion");
				pExplpsion->GetTransForm()->SetPos(m_pTarget->GetTransForm()->GetPos());
				pExplpsion->GetAnimator2D()->PlayAnim(L"Explosion", false);
				pExplpsion->AddComponent(new CEffectScript);

				m_fSpacing = 0.f;
				m_fAttackTime = 0.f;
				m_fCoolTime = 5.f;
				CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pTarget ,0});
				m_pTarget = nullptr;

				m_eAttackType = ATTACK_TYPE(rand() % 2);
				m_bTail = false;
			}
		}
	}
	else
		m_fCoolTime -= DT;

	Animator2D()->SetMoveFrm(iNum);

	/*Vec3 vDir = pPlayer->GetTransForm()->GetPos() - TransForm()->GetPos();
	vDir.Normalize();*/

	return 0;
}