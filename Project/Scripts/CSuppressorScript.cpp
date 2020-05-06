#include "CSuppressorScript.h"
#include "CAnimator2D.h"
#include "CScriptMgr.h"
#include "CGameObj.h";
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CCollider.h"
#include "TimeMgr.h"
#include "CTexture.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CBeamScript.h"
#include "CEffectScript.h"
#include "CMHPScript.h"

CSuppressorScript::CSuppressorScript()
	:m_pBeamCenter(nullptr)
	, m_bGravity(false)
	, m_pHPbar(nullptr)
	, m_pHP(nullptr)
	, m_bLegAnim(true)
	, m_bLeg(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CSUPPRESSORSCRIPT);
}   

CSuppressorScript::~CSuppressorScript()
{
}

void CSuppressorScript::Awake()
{
	if (Animator2D() == nullptr)
		return;

	Animator2D()->IsPlay(true);

	m_pLeg = new CGameObj;
	m_pLeg->SetName(L"Leg");
	m_pLeg->AddComponent(new CAnimator2D);
	m_pLeg->AddComponent(new CMeshRender);
	
	m_pLeg->GetTransForm()->SetPos(Vec3(4.f,-68.f,0.f));

	m_pLeg->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pLeg->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));

	m_pLeg->GetAnimator2D()->AddAnimation(L"..\\content\\Texture\\SuppressorLv1\\Walk\\", L"Walk");
	m_pLeg->GetAnimator2D()->SetDurationTime(0.06f);
	m_pLeg->GetAnimator2D()->IsPlay(true);
	
	GetOwner()->AddChild(m_pLeg);
	
	Animator2D()->PlayAnim(L"Birth", false);
	m_pLeg->GetAnimator2D()->PlayAnim(L"Walk", true);
	m_pLeg->GetMeshRender()->SetView(false);
	m_eState = OBJ_STATE::BIRTH;

	//_vSize.x / (float)_pTex->GetWidth(), _vSize.y / (float)_pTex->GetHeight();
}

void CSuppressorScript::Start()
{
	Animator2D()->PlayAnim(L"Birth", false);
	vector<CGameObj*>& vecChild = GetOwner()->GetChild();

	if (!vecChild.empty())
	{
		m_bLeg = true;
		m_pLeg = vecChild[0];
	}

	for (size_t i = 0; i < vecChild.size(); i++)
	{
		vecChild[i]->GetAnimator2D()->AddAnimation(L"..\\content\\Texture\\SuppressorLv1\\Walk\\", L"Walk");
		vecChild[i]->GetAnimator2D()->SetDurationTime(0.06f);
		vecChild[i]->GetAnimator2D()->IsPlay(true);

		vecChild[i]->GetAnimator2D()->PlayAnim(L"Walk", true);
		vecChild[i]->GetMeshRender()->SetView(false);
		m_eState = OBJ_STATE::BIRTH;
	}
}

int CSuppressorScript::Update()
{
	/*if (!m_bLeg)
	{
		vector<CGameObj*>& vecChild = GetOwner()->GetChild();

		if (!vecChild.empty())
		{
			m_bLeg = true;
			m_pLeg = vecChild[0];
		}
		else
		{
			return 0;
		}
	}*/
	if (CSceneMgr::GetInst()->GetStop())
	{
		m_bLegAnim = false;
		GetOwner()->GetChild()[0]->GetAnimator2D()->IsPlay(false);
		if (m_eState == OBJ_STATE::HIT)
		{
			if (m_pBeamCenter != nullptr)
			{
				tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pBeamCenter,0 };
				CEventMgr::GetInst()->AddEvent(event);
				m_pBeamCenter = nullptr;
			}
			//TransForm()->SetPos(Lerf(m_vStartPos, m_vKnockbackPos,DT));

			if (m_pHPbar == nullptr)
			{
				float fColor = 0.f;
				m_pHP = CResMgr::GetInst()->Load<CPrefab>(L"MHP")->Instantiate();
				m_pHP->AddComponent(new CMHPScript);
				GetOwner()->AddChild(m_pHP);
				m_pHP->GetTransForm()->SetPos(0.f, 0.f, 0.f);
				m_pHP->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_0);
				m_pHP->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_2);
				m_pHP->GetTransForm()->SetRotaion(TransForm()->GetRotaion());

				m_pHPbar = CResMgr::GetInst()->Load<CPrefab>(L"MHP")->Instantiate();
				GetOwner()->AddChild(m_pHPbar);
				m_pHPbar->GetTransForm()->SetPos(0.f, 0.f, 0.f);
				m_pHPbar->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_1);
				m_pHPbar->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_2);
				m_pHPbar->GetTransForm()->SetRotaion(TransForm()->GetRotaion());
			}
		}
		return 0;
	}

	if (!m_bLegAnim)
	{
		m_bLegAnim = true;
		GetOwner()->GetChild()[0]->GetAnimator2D()->IsPlay(true);
	}
	if (Death())
	{
		CGameObj* pPop = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Pop");
		pPop->GetTransForm()->SetPos(TransForm()->GetPos());
		pPop->AddComponent(new CEffectScript);
		pPop->GetAnimator2D()->PlayAnim(L"Pop", false);
	}

	if (Animator2D() == nullptr)
		return 0;

	if (Animator2D()->GetAnimFinish())
	{
		if (Animator2D()->GetCurKey() == L"Birth")
		{
			Animator2D()->PlayAnim(L"Unpack", false);
		}
		else if (Animator2D()->GetCurKey() == L"Glitch")
		{
			Animator2D()->PlayAnim(L"Eye", false);
			Animator2D()->SetMoveFrm(10);
		}
		else if (Animator2D()->GetCurKey() == L"Unpack")
		{
			m_eState = OBJ_STATE::IDLE;
			m_pLeg->GetMeshRender()->SetView(true);
			m_pLeg->GetAnimator2D()->IsPlay(true);
			
			Animator2D()->PlayAnim(L"Eye", false);
			Animator2D()->IsPlay(false);
			m_pLeg->GetAnimator2D()->IsPlay(false);
		}
	}

	if (m_eState == OBJ_STATE::HIT)
	{
		if (m_pBeamCenter != nullptr)
		{
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pBeamCenter,0 };
			CEventMgr::GetInst()->AddEvent(event);
			m_pBeamCenter = nullptr;
		}
		//TransForm()->SetPos(Lerf(m_vStartPos, m_vKnockbackPos,DT));

		if (m_pHPbar == nullptr)
		{
			float fColor = 0.f;
			m_pHP = CResMgr::GetInst()->Load<CPrefab>(L"MHP")->Instantiate();
			m_pHP->AddComponent(new CMHPScript);
			GetOwner()->AddChild(m_pHP);
			m_pHP->GetTransForm()->SetPos(0.f, 0.f, 0.f);
			m_pHP->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_0);
			m_pHP->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_2);
			m_pHP->GetTransForm()->SetRotaion(TransForm()->GetRotaion());

			m_pHPbar = CResMgr::GetInst()->Load<CPrefab>(L"MHP")->Instantiate();
			GetOwner()->AddChild(m_pHPbar);
			m_pHPbar->GetTransForm()->SetPos(0.f, 0.f, 0.f);
			m_pHPbar->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_1);
			m_pHPbar->GetMeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_2);
			m_pHPbar->GetTransForm()->SetRotaion(TransForm()->GetRotaion());
		}

		m_fAccTime += DT;
		Animator2D()->IsPlay(true);
		Animator2D()->PlayAnim(L"Glitch", true);
		m_pLeg->GetMeshRender()->SetView(false);

		if (m_fAccTime < 0.3f)
		{
			Vec3 vKnock = m_vKnockbackPos;
			vKnock.y += 50.f;
			Vec3 vPos = Lerf(m_vStartPos, vKnock, m_fAccTime * 3.3);
			TransForm()->SetPos(vPos);
		}
		else if (m_fAccTime < 0.5f)
		{
			if (!m_bGravity)
			{
				m_bGravity = true;
				m_vStartPos = TransForm()->GetPos();
			}
			Vec3 vPos = Lerf(m_vStartPos, m_vKnockbackPos, (m_fAccTime - 0.3f) * 5);
			TransForm()->SetPos(vPos);
		}

		if (m_fAccTime >= 0.5f)
		{
			m_bGravity = false;
			TransForm()->SetPos(m_vKnockbackPos);
			m_eState = OBJ_STATE::IDLE;
			m_fAccTime = 0.f;

			m_pLeg->GetMeshRender()->SetView(true);

			Animator2D()->IsPlay(false);
			Animator2D()->PlayAnim(L"Eye", false);
		}
		return 0;
	}


	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Player");

	if (pLayer != nullptr)
	{
		if (pLayer->GetGameObj().size() > 0)
		{
			m_vPlayerPos = pLayer->GetGameObj()[0]->GetTransForm()->GetPos();
			m_eState = OBJ_STATE::MOVE;
		}
	}

	if (m_eState == OBJ_STATE::MOVE && Animator2D()->GetCurKey() == L"Eye")
	{
		Vec3 vPos = TransForm()->GetPos();
		Vec3 vDir = GetDirVec3(vPos, m_vPlayerPos);

		vPos += vDir * 100.f * DT;

		TransForm()->SetPos(vPos);
		float fDis = Vec3::Distance(vPos, m_vPlayerPos);
		m_pLeg->GetAnimator2D()->IsPlay(true);

		if (fDis <= 400.f)
		{
			m_pBeamCenter = new CGameObj;
			m_pBeamCenter->GetTransForm()->SetPos(TransForm()->GetPos());
			CGameObj* pBeam = CResMgr::GetInst()->Load<CPrefab>(L"Beam")->Instantiate();
			pBeam->GetAnimator2D()->IsPlay(true);
			pBeam->GetAnimator2D()->PlayAnim(L"Beam",true);

			pBeam->GetTransForm()->SetPos(Vec3(-50.f,0.f,0.f));
			CBeamScript* pBeamScript = new CBeamScript;
			pBeam->AddComponent(pBeamScript);
			pBeamScript->SetStartEnd(&TransForm()->GetPos(),&m_vPlayerPos);

			m_pBeamCenter->AddChild(pBeam);
			CSceneMgr::GetInst()->AddObject(L"Skill", m_pBeamCenter);

			Animator2D()->PlayAnim(L"Attack", false);

			m_pLeg->GetAnimator2D()->IsPlay(false);
		}
	}
	else if (m_eState == OBJ_STATE::MOVE && Animator2D()->GetCurKey() == L"Attack")
	{
		float fDis = Vec3::Distance(TransForm()->GetPos(), m_vPlayerPos);

		CGameObj* pObj = nullptr;
		if (CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pObj))
		{
			vector<CScript*>& vecScript = pObj->GetScript();

			for (UINT i = 0; i < vecScript.size(); ++i)
			{
				CUnitScript* pUnit = dynamic_cast<CUnitScript*>(vecScript[i]);

				if (pUnit != nullptr)
				{
					pUnit->SetPlayerDamage(DT);
					break;
				}
			}
		}

		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Player");
		vector<CGameObj*>& vecObj = pLayer->GetGameObj();

		if (fDis >= 500.f)
		{
			if (m_pBeamCenter != nullptr)
			{
				tEvent event = tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pBeamCenter,0};
				CEventMgr::GetInst()->AddEvent(event);
				m_pBeamCenter = nullptr;
			}

			Animator2D()->PlayAnim(L"Eye", false);
			m_pLeg->GetAnimator2D()->IsPlay(true);
		}
	}

	if (Animator2D()->GetCurKey() == L"Eye" || Animator2D()->GetCurKey() == L"Attack")
	{
		float fAngle = GetAngleDegree(TransForm()->GetPos(), m_vPlayerPos);
		int iNum = 0;
		AnimAngleToNum(64, fAngle, &iNum);
		if (iNum > 16)
			iNum -= 31;

		fAngle = GetAngleRadian(TransForm()->GetPos(), m_vPlayerPos);

		if(m_pBeamCenter != nullptr)
		m_pBeamCenter->GetTransForm()->SetRotaion(Vec3(0.f,0.f, fAngle));

		Animator2D()->SetMoveFrm(iNum);
	}

	if (m_pHPbar != nullptr)
	{
		m_pHPbar->GetTransForm()->SetRotaion(TransForm()->GetRotaion());
		m_pHP->GetTransForm()->SetRotaion(TransForm()->GetRotaion());
	}

	return 0;
}

void CSuppressorScript::CollisionEnter(CCollider * _pColl)
{
	wstring wcsName = CSceneMgr::GetInst()->GetLayerName(_pColl->GetOwner()->GetID());

	if (wcsName == L"Skill")
	{
		m_vStartPos = TransForm()->GetPos();
		m_eState == OBJ_STATE::HIT;
	}
}

void CSuppressorScript::Collision(CCollider * _pColl)
{
}

void CSuppressorScript::CollisionExit(CCollider * _pColl)
{
}