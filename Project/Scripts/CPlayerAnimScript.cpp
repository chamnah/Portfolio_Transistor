#include "CPlayerAnimScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "CAnimator2D.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CEffectScript.h"
#include "CEventMgr.h"
#include "CMeshRender.h"
#include "CBulletScript.h"
#include "CGameMgr.h"
#include "CShaderMgr.h"
#include "CRenderMgr.h"
#include "CTileMgrScript.h"
#include "CPathMgr.h"

CPlayerAnimScript::CPlayerAnimScript()
	:m_fAnimAngle(0.f)
	, m_bSkill(false)
	, m_bTurnSkill(true)
	, m_pReload(nullptr)
	, m_fMoveDis(0.f)
	, m_pSpectrum(nullptr)
	, m_pCharge(nullptr)
	, m_eSkillType(SKILL_TYPE::CRASH)
	, m_bCharge(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CPLAYERANIMSCRIPT);
	m_eAnimType = ANIM_TYPE::NONE;
}

CPlayerAnimScript::~CPlayerAnimScript()
{
}

void CPlayerAnimScript::Awake()
{
}

int CPlayerAnimScript::Update()
{
	if (Animator2D() == nullptr)
		return 0;

	if (Animator2D()->GetAnimation().empty())
		return 0;

	if (CGameMgr::GetInst()->GetCoolTime())
	{
		CGameMgr::GetInst()->SubCurAllPoint(DT);
		float fPoint = CGameMgr::GetInst()->GetCurAllPoint();
		if (fPoint <= 0.f)
		{
			CGameMgr::GetInst()->SetCurAllPoint(0.f);
			CGameMgr::GetInst()->SetCoolTime(false);
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ, (INT_PTR)m_pReload, 0 };
			CEventMgr::GetInst()->AddEvent(event);
		}
	}

	if (Animator2D()->GetAnimFinish())
	{
		if ((m_eAnimType == ANIM_TYPE::BREACH || m_eAnimType == ANIM_TYPE::CRASH) && CGameMgr::GetInst()->GetPlayTurn())
			m_bTurnSkill = false;

		m_bSkill = false;

		Animator2D()->AnimReset(); // 나중에 죽는 애니메이션 등등을 위해 안에 두고 상황 봐서 바꿀 예정

		m_eAnimType = ANIM_TYPE::NONE;
		wstring wcsKey = L"Idle";
		wstring wcsNum = AnimAngleToNum(16, m_fAnimAngle);
		wcsKey += wcsNum;
		Animator2D()->PlayAnim(wcsKey, true);
	}

	if (!CGameMgr::GetInst()->GetTurn()) // 턴스킬을 쓴 상태가 아닐경우
	{
		if (m_eAnimType == ANIM_TYPE::CRASH)
		{
			if (!m_bSkill && Animator2D()->GetCurFrame() >= 28)
			{
				m_bSkill = true;
				Vec3 vObjPos = TransForm()->GetPos();

				Vec3 vDir = m_vMousePos - vObjPos;
				vDir.Normalize();

				Vec3 vPos = TransForm()->GetPos();
				vPos.y -= 40.f;
				vPos += vDir * 110.f;

				CGameObj*  pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Skill", L"Crash");
				pObj->AddComponent(new CBulletScript);
				((CBulletScript*)pObj->GetScript()[0])->SetMousePos(m_vMousePos);
				pObj->GetTransForm()->SetPos(vPos);
				pObj->Awake();

				pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Slam");
				pObj->AddComponent(new CEffectScript);
				pObj->GetTransForm()->SetPos(vPos);
				
				pObj->GetAnimator2D()->PlayAnim(L"Slam", false);

			}
		}
		else if (m_eAnimType == ANIM_TYPE::BREACH)
		{
			if (!m_bSkill && Animator2D()->GetCurFrame() >= 28)
			{
				m_bCharge = false;
				m_bSkill = true;
				Vec3 vObjPos = TransForm()->GetPos();

				Vec3 vDir = m_vMousePos - vObjPos;
				vDir.Normalize();

				Vec3 vPos = TransForm()->GetPos();
				vPos.y -= 40.f;
				vPos += vDir * 110.f;

				CGameObj*  pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Skill", L"Breach");
				pObj->AddComponent(new CBulletScript);
				((CBulletScript*)pObj->GetScript()[0])->SetMousePos(m_vMousePos);
				pObj->GetTransForm()->SetPos(vPos);
				pObj->Awake();

				/*pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Slam");
				pObj->AddComponent(new CEffectScript);
				pObj->GetTransForm()->SetPos(vPos);

				pObj->GetAnimator2D()->PlayAnim(L"Slam", false);*/

			}

			if (!m_bCharge && !m_bSkill && Animator2D()->GetCurFrame() >= 17)
			{
				CResMgr::GetInst()->SoundPlay(L"Snipe",1);
				m_bCharge = true;
				m_bSkill = true;
				Animator2D()->IsPlay(false);
				m_pCharge = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"Charge");

				Vec3 vPos = TransForm()->GetPos();
				vPos.y += 200.f;
				m_pCharge->GetTransForm()->SetPos(vPos);
				m_pCharge->GetAnimator2D()->PlayAnim(L"Charge", false);
				m_pCharge->AddComponent(new CEffectScript);
			}

			if (m_pCharge != nullptr && m_pCharge->GetDead())
			{
				m_bSkill = false;
				m_pCharge = nullptr;
				Animator2D()->IsPlay(true);
				Animator2D()->SetMoveFrm(18);
			}
		}
	}
	
	if (CGameMgr::GetInst()->GetPlayTurn())  // 턴스킬로 공격 중
	{
		list<TURN>& listTurn = CGameMgr::GetInst()->GetListTurn();
		
		if (listTurn.empty()) // 스킬이 다 떨어지면
		{
			m_pReload = CResMgr::GetInst()->Load<CPrefab>(L"Reload")->Instantiate();
			m_pReload->GetAnimator2D()->PlayAnim(L"Reload",true);
			m_pReload->GetTransForm()->SetPos(Vec3(0.f,70.f,1.f));
			GetOwner()->AddChild(m_pReload);

			CGameMgr::GetInst()->SetCoolTime(true);
			CGameMgr::GetInst()->SetCount(0);
			CGameMgr::GetInst()->SetPlayTurn(false);
			CSceneMgr::GetInst()->SetStop(false);
			CGameMgr::GetInst()->Reset();
			return 0;
		}

		TURN turn = listTurn.front();
		Vec3 vPos = TransForm()->GetPos();
		// 플레이어 기준
		int iDir = 0;

		bool bPop = false;

		
		m_fAnimAngle = GetAngleDegree(vPos, turn.vPos);

		if (turn.eType == ANIM_TYPE::MOVE)
		{
			m_vDir = GetDirVec3(vPos, turn.vPos);
			vPos += m_vDir * 300.f * DT;
			TransForm()->SetPos(vPos);

			wstring wcsNum = AnimAngleToNum(32, m_fAnimAngle);
			wstring wcsKey = L"Run";

			wcsKey += wcsNum;

			Animator2D()->PlayAnim(wcsKey, false);

			if (m_vPrePos.x <= turn.vPos.x && m_vPrePos.y <= turn.vPos.y)// 왼쪽 아래
			{
				if (vPos.x > turn.vPos.x || vPos.y > turn.vPos.y)
					bPop = true;
			}
			else if (m_vPrePos.x <= turn.vPos.x && m_vPrePos.y > turn.vPos.y) // 왼쪽 위
			{
				if (vPos.x > turn.vPos.x || vPos.y <= turn.vPos.y)
					bPop = true;
			}
			else if (m_vPrePos.x > turn.vPos.x && m_vPrePos.y <= turn.vPos.y) // 오른쪽 아래
			{
				if (vPos.x <= turn.vPos.x || vPos.y > turn.vPos.y)
					bPop = true;
			}
			else if (m_vPrePos.x > turn.vPos.x && m_vPrePos.y > turn.vPos.y) // 오른쪽 위
			{
				if (vPos.x <= turn.vPos.x || vPos.y <= turn.vPos.y)
					bPop = true;
			}
			else
				bPop = true;

			if (GetDistanceVec3(vPos, turn.vPos) <= 50.f || bPop) // 이건 나중에 수정해야한다. 내 캐릭터가 지정한 위치보다 작았을경우는 커지면,컷을경우는 작아지면으로  GetDistanceVec3(vPos, turn.vPos) < 30.f
			{
				listTurn.pop_front();
				m_vPrePos = vPos;
				
				if (listTurn.empty())
					CGameMgr::GetInst()->SetCount(0);
				else if (listTurn.front().eType != ANIM_TYPE::MOVE)
					CGameMgr::GetInst()->SubCount(1); // 아이콘 갯수 하나를 뺀다.
			}
		}
		else if (turn.eType == ANIM_TYPE::CRASH)
		{
			if (!m_bTurnSkill)
			{
				m_bTurnSkill = true;
				listTurn.pop_front();
				CGameMgr::GetInst()->SubCount(1);

				return 0;
			}

			m_vMousePos = turn.vPos;

			m_eAnimType = ANIM_TYPE::CRASH;
			
			wstring wcsNum = AnimAngleToNum(16, m_fAnimAngle);
			wstring wcsKey = L"Crash";

			wcsKey += wcsNum;
			Animator2D()->PlayAnim(wcsKey, false);
		}
		else if (turn.eType == ANIM_TYPE::BREACH)
		{
			if (!m_bTurnSkill)
			{
				m_bTurnSkill = true;
				listTurn.pop_front();
				CGameMgr::GetInst()->SubCount(1);

				return 0;
			}

			m_vMousePos = turn.vPos;
			m_eAnimType = ANIM_TYPE::BREACH;

			wstring wcsNum = AnimAngleToNum(16, m_fAnimAngle);
			wstring wcsKey = L"Crash";

			wcsKey += wcsNum;
			Animator2D()->PlayAnim(wcsKey, false);
		}
	}
	else
	{
		if (m_eAnimType != ANIM_TYPE::CRASH && m_eAnimType != ANIM_TYPE::BREACH)
		{
			MouseInput();
			KeyInput();
		}
	}
	return 0;
}

void CPlayerAnimScript::KeyInput()
{
	Vec3 vPos = TransForm()->GetPos();
	Vec3 vCurPos = vPos;

	m_fMoveDis += Vec3::Distance(vCurPos, vPos);

	if (m_fMoveDis > 50.f)
	{
		m_fMoveDis = 0.f;
		CGameObj* pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Back", L"Decal");
		pObj->AddComponent(new CEffectScript);
		pObj->GetTransForm()->SetPos(vCurPos);
		pObj->GetAnimator2D()->PlayAnim(L"Decal", false);
	}

	if (KEYHOLD(KEY_TYPE::KEY_A) && KEYHOLD(KEY_TYPE::KEY_S))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		Vec3 vDir = Vec3(vPos.x - 2.f, vPos.y - 1.f, vPos.z) - vPos;
		vDir.Normalize();
		vPos += vDir * 100.f * DT;
		m_fAnimAngle = 45.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Run04", true);
	}

	else if (KEYHOLD(KEY_TYPE::KEY_D) && KEYHOLD(KEY_TYPE::KEY_S))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		Vec3 vDir = Vec3(vPos.x + 2.f, vPos.y - 1.f, vPos.z) - vPos;
		vDir.Normalize();
		vPos += vDir * 100.f * DT;
		m_fAnimAngle = 135.f;
		TransForm()->SetRotaion(Vec3(0.f, XM_PI, 0.f));
		Animator2D()->PlayAnim(L"Run04", true);
	}

	else if (KEYHOLD(KEY_TYPE::KEY_D) && KEYHOLD(KEY_TYPE::KEY_W))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		Vec3 vDir = Vec3(vPos.x + 2.f, vPos.y + 1.f, vPos.z) - vPos;
		vDir.Normalize();
		vPos += vDir * 100.f * DT;
		m_fAnimAngle = 225.f;
		TransForm()->SetRotaion(Vec3(0.f, XM_PI, 0.f));
		Animator2D()->PlayAnim(L"Run28", true);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_A) && KEYHOLD(KEY_TYPE::KEY_W))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		Vec3 vDir = Vec3(vPos.x - 2.f, vPos.y + 1.f, vPos.z) - vPos;
		vDir.Normalize();
		vPos += vDir * 100.f * DT;
		m_fAnimAngle = 315.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Run28", true);
	}
	else if (KEYHOLD(KEY_TYPE::KEY_A))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		vPos.x -= 100.f * DT;
		m_fAnimAngle = 0.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Run00", true);
	}
	else if (KEYAWAY(KEY_TYPE::KEY_A))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		m_fAnimAngle = 0.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Stop00", false);
	}

	else if (KEYHOLD(KEY_TYPE::KEY_D))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		vPos.x += 100.f * DT;
		m_fAnimAngle = 180.f;
		TransForm()->SetRotaion(Vec3(0.f, XM_PI, 0.f));
		Animator2D()->PlayAnim(L"Run00", true);
	}
	else if (KEYAWAY(KEY_TYPE::KEY_D))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		m_fAnimAngle = 180.f;
		TransForm()->SetRotaion(Vec3(0.f, XM_PI, 0.f));
		Animator2D()->PlayAnim(L"Stop00", false);
	}

	else if (KEYHOLD(KEY_TYPE::KEY_W))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		vPos.y += 100.f * DT;
		m_fAnimAngle = 270.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Run24", true);
	}
	else if (KEYAWAY(KEY_TYPE::KEY_W))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		m_fAnimAngle = 270.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Stop12", false);
	}

	else if (KEYHOLD(KEY_TYPE::KEY_S))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		vPos.y -= 100.f * DT;
		m_fAnimAngle = 90.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Run08", true);
	}
	else if (KEYAWAY(KEY_TYPE::KEY_S))
	{
		m_eAnimType = ANIM_TYPE::MOVE;
		m_fAnimAngle = 90.f;
		TransForm()->SetRotaion(Vec3(0.f, 0.f, 0.f));
		Animator2D()->PlayAnim(L"Stop04", false);
	}

	TransForm()->SetPos(vPos);

	if (KEYTAB(KEY_TYPE::KEY_SPACE) && !CGameMgr::GetInst()->GetCoolTime())
	{
		//Vec3 vPos = (Collider()->GetPos() * Collider()->GetScale());

		if (!CGameMgr::GetInst()->GetTurn())
		{
			CResMgr::GetInst()->SoundPlay(L"FocusSkill",1);
			m_pSpectrum = new CGameObj;
			m_pSpectrum->AddComponent(new CMeshRender);

			m_pSpectrum->GetTransForm()->SetPos(TransForm()->GetPos());
			m_pSpectrum->GetMeshRender()->SetMesh(MeshRender()->GetMesh());
			m_pSpectrum->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Spectrum"));
			m_pSpectrum->GetMeshRender()->SetTexture(Animator2D()->GetCurAnimTexture());
			int iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Effect");
			tEvent event = tEvent{ EVENT_TYPE::CREATE_OBJ,iIdx,(INT_PTR)m_pSpectrum };
			CEventMgr::GetInst()->AddEvent(event);
			m_pSpectrum->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));

			//MeshRender()->GetMtrl()->SetShader(CShaderMgr::GetInst()->FindShader(L"Spectrum"));

			CGameObj* pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Mouse",L"Transition");
			pObj->AddComponent(new CEffectScript);
			Vec3 vPos = TransForm()->GetPos();
			vPos.z = 1.f;
			pObj->GetTransForm()->SetPos(vPos);
			pObj->GetAnimator2D()->PlayAnim(L"Transition", false);
			pObj->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));

			bool bStop = CSceneMgr::GetInst()->GetStop();
			CSceneMgr::GetInst()->SetStop(!bStop);
			CGameMgr::GetInst()->SetTurn(true);
			m_vStartPos = TransForm()->GetPos();
			m_vPrePos = m_vStartPos;
			vector<TURNTILE>  vecPos;
			CTileMgrScript::GetInst()->TurnTile(TransForm()->GetPos() + g_World.mtxView.Translation(), vecPos);

			for (size_t i = 0; i < vecPos.size(); i++)
			{
				CGameObj* pDot = CResMgr::GetInst()->PrefabToCreateObj(L"Effect", L"DotInOut");
				
				Vec3 vChildPos = pDot->GetChild()[0]->GetTransForm()->GetPos();
				vChildPos.z = 9998.f;
				pDot->GetChild()[0]->GetTransForm()->SetPos(vChildPos);

				pDot->GetTransForm()->SetPos(vecPos[i].vPos);
				Vec3 vScale = pDot->GetTransForm()->GetScale();
				float fIdx = 40 - vecPos[i].iIdx;
				if (fIdx < 0)
					fIdx = 0;
				fIdx *= 0.025f;
				vScale.x *= fIdx;
				vScale.y *= fIdx;
				pDot->GetTransForm()->SetScale(vScale);

				Vec3 vChildScale = pDot->GetChild()[0]->GetTransForm()->GetScale();
				vChildScale.x *= fIdx;
				pDot->GetChild()[0]->GetTransForm()->SetScale(vChildScale);

				m_vecDot.push_back(pDot);
			}

		}
		else
		{
			if (m_pSpectrum != nullptr)
			{
				CResMgr::GetInst()->SoundPlay(L"FocusStart", 1);
				tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pSpectrum,0 };
				CEventMgr::GetInst()->AddEvent(event);
				m_pSpectrum = nullptr;
			}

			for (size_t i = 0; i < m_vecDot.size(); i++)
			{
				tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecDot[i],0 };
				CEventMgr::GetInst()->AddEvent(event);
			}
			m_vecDot.clear();

			//MeshRender()->GetMtrl()->SetShader(CShaderMgr::GetInst()->FindShader(L"Std"));
			CGameMgr::GetInst()->SetTurn(false);
			CGameMgr::GetInst()->SetPlayTurn(true);
			TransForm()->SetPos(m_vStartPos);
		}
	}
}

void CPlayerAnimScript::MouseInput()
{
	Vec3 vObjPos = TransForm()->GetPos();
	

	/*
	if (KEYHOLD(KEY_TYPE::KEY_LBTN) || KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		Vec3 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		Vec3 vPos = Vec3(vMousePos.x - vObjPos.x, vMousePos.y - vObjPos.y, vMousePos.z - vObjPos.z);

		vPos.Normalize();

		vObjPos += vPos * 100.f * DT;
	}
	*/
	if (KEYTAB(KEY_TYPE::KEY_RBTN) && !CGameMgr::GetInst()->GetCoolTime())
	{


		if (m_eAnimType != ANIM_TYPE::NONE)
			return;

		m_vMousePos = CKeyMgr::GetInst()->GetMousePos();

		m_vMousePos -= g_World.mtxView.Translation();

		if (m_eSkillType == SKILL_TYPE::CRASH)
		{
			CResMgr::GetInst()->SoundPlay(L"Slam", 1);
			m_eAnimType = ANIM_TYPE::CRASH;
		}
		else if(m_eSkillType == SKILL_TYPE::BREACH)
			m_eAnimType = ANIM_TYPE::BREACH;

		if (CGameMgr::GetInst()->GetTurn())
			CGameMgr::GetInst()->AddTurn(m_eAnimType, m_vMousePos);

		m_fAnimAngle = GetAngleDegree(TransForm()->GetPos(), m_vMousePos);
		
		wstring wcsNum = AnimAngleToNum(16, m_fAnimAngle);
		wstring wcsKey = L"Crash";

		wcsKey += wcsNum;
		Animator2D()->IsPlay(true);
		Animator2D()->PlayAnim(wcsKey, false);
	}
	TransForm()->SetPos(vObjPos);
}

void CPlayerAnimScript::DirAnimPlay(wstring _wcsKey, Vec3 _vPos, Vec3 _vTargetPos)
{
}