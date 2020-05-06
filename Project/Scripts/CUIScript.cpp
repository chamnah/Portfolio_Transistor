#include "CUIScript.h"
#include "CScriptMgr.h"
#include "KeyMgr.h"
#include "CMeshRender.h"
#include "CAnimator2D.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CGameObj.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CTransForm.h"
#include "CTexture.h"
#include "CPlayerAnimScript.h"

CUIScript::CUIScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CUISCRIPT);
	m_eSkill = SKILL_TYPE::CRASH;
}

CUIScript::~CUIScript()
{
}

void CUIScript::Awake()
{
	TransForm()->SetPos(-150.f, -412.f, 0.f);

	/*m_pActive = new CGameObj;
	m_pActive->SetName(L"Active");
	m_pActive->AddComponent(new CMeshRender);
	m_pActive->AddComponent(new CAnimator2D);

	m_pActive->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pActive->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
	m_pActive->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));

	m_pActive->GetAnimator2D()->AddAnimation(L"..\\content\\Texture\\ActivePowerHighlight\\", L"Active");
	m_pActive->GetAnimator2D()->PlayAnim(L"Active", true);

	m_pActive->GetTransForm()->SetPos(-150.f,-10.f,0.f);

	m_pSkill = new CGameObj;
	m_pSkill->SetName(L"CrashIcon");
	m_pSkill->AddComponent(new CMeshRender);

	m_pSkill->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pSkill->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
	CResMgr::GetInst()->AddResource<CTexture>(L"CrashIcon", CTexture::CreateTexture(L"..\\content\\Texture\\SlamSlot.png"));
	m_pSkill->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(L"CrashIcon"));

	m_pSkill->GetTransForm()->SetPos(-150.f, -10.f, 0.f);

	CGameObj* pObj = GetOwner();
	pObj->AddChild(m_pSkill);

	CSceneMgr::GetInst()->AddObject(L"UI", m_pActive);

	CEventMgr::GetInst()->SetReset(true);*/
}

int CUIScript::Update()
{
	if (Animator2D() != nullptr)
		Animator2D()->PlayAnim(L"Active", true);
	if (KEYTAB(KEY_TYPE::KEY_1))
	{
		CGameObj* pObj = nullptr;
		if (CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pObj))
		{
			vector<CScript*>& vecScript = pObj->GetScript();
			for (UINT i = 0; i < vecScript.size(); ++i)
			{
				CPlayerAnimScript* pScript = dynamic_cast<CPlayerAnimScript*>(vecScript[i]);

				if (pScript != nullptr)
				{
					pScript->SetSkillType(SKILL_TYPE::CRASH);
					break;
				}
			}
		}
		m_eSkill = SKILL_TYPE::CRASH;
		TransForm()->SetPos(-150.f, -412.f, 5.f);
	}
	else if (KEYTAB(KEY_TYPE::KEY_2))
	{
		CGameObj* pObj = nullptr;
		if (CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pObj))
		{
			vector<CScript*>& vecScript = pObj->GetScript();
			for (UINT i = 0; i < vecScript.size(); ++i)
			{
				CPlayerAnimScript* pScript = dynamic_cast<CPlayerAnimScript*>(vecScript[i]);

				if (pScript != nullptr)
				{
					pScript->SetSkillType(SKILL_TYPE::BREACH);
					break;
				}
			}
		}

		m_eSkill = SKILL_TYPE::BREACH;
		TransForm()->SetPos(-50.f, -412.f, 5.f);
	}
	else if (KEYTAB(KEY_TYPE::KEY_3))
	{
		TransForm()->SetPos(50.f, -412.f, 5.f);
	}
	else if (KEYTAB(KEY_TYPE::KEY_4))
	{
		TransForm()->SetPos(150.f, -412.f, 5.f);
	}
	return 0;
}