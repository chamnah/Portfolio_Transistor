#include "CCinematicScript.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "TimeMgr.h"
#include "CMeshRender.h"
#include "CSound.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGameMgr.h"
#include "CSaveLoadMgr.h"
#include "CCore.h"

CCinematicScript::CCinematicScript()
	:m_fAccTime(0.f)
	, m_iCheck(0)
	, m_fTwinkleTime(0.f)
	, m_bEnd(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCINEMATICSCRIPT);
}

CCinematicScript::~CCinematicScript()
{
}

int CCinematicScript::Update()
{
	m_fAccTime += DT;
	
	if (m_fAccTime > 3.f && !(m_iCheck & ONE))
	{
		CCore::GetInst()->AddText(TextInfo{L"ÀÌºÁ, ·¹µå",Vec2(700,50),Vec2(1200,200)});
		CGameObj* pBlack = CResMgr::GetInst()->PrefabToCreateObj(L"Back",L"Black");
		Vec3 vPos;
		Vec3 vScale = {1920.f,1080.f,1.f};
		vPos.z = 595.f;
		pBlack->GetTransForm()->SetPos(vPos);
		pBlack->GetTransForm()->SetScale(vScale);
		float fAlpha = 0.5f;
		pBlack->GetMeshRender()->GetMtrl()->SetParam(&fAlpha,SHADER_PARAM::MTRL_3);
		m_iCheck |= ONE;
		CResMgr::GetInst()->SoundPlay(L"HeyRed", 1);
	}
	else if (m_fAccTime > 8.f && !(m_iCheck & TWO))
	{
		CCore::GetInst()->ClearText();
		CCore::GetInst()->AddText(TextInfo{ L"µµ¸ÁÄ¥¼ö ¾ø¾î",Vec2(700,50),Vec2(1200,200) });
		m_iCheck |= TWO;
		CResMgr::GetInst()->SoundPlay(L"NotEscape", 1);
	}

	if (m_iCheck & ONE && ((CResMgr::GetInst()->Load<CSound>(L"HeyRed"))->IsPlaying())
		|| m_iCheck & TWO && ((CResMgr::GetInst()->Load<CSound>(L"NotEscape"))->IsPlaying()))
	{
		float fColor = 0.2f;
		float fAll = 1.f;
		MeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_0);
		MeshRender()->GetMtrl()->SetParam(&fAll, SHADER_PARAM::MTRL_1);
		MeshRender()->GetMtrl()->SetParam(&fAll, SHADER_PARAM::MTRL_2);
		MeshRender()->GetMtrl()->SetParam(&fAll, SHADER_PARAM::MTRL_3);
	}
	else
	{
		float fColor = 0.f;
		MeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_0);
		MeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_1);
		MeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_2);
		MeshRender()->GetMtrl()->SetParam(&fColor, SHADER_PARAM::MTRL_3);
	}

	if (m_iCheck & TWO && !((CResMgr::GetInst()->Load<CSound>(L"NotEscape"))->IsPlaying()))
	{
		if (!m_bEnd)
		{
			CLayer* pBack = CSceneMgr::GetInst()->GetLayer(L"Back");
			m_vecObj = &pBack->GetGameObj();
			m_bEnd = true;
			CCore::GetInst()->ClearText();
		}
	}

	if (m_bEnd)
	{
		Vec3 vPos;
		for (size_t i = 0; i < m_vecObj->size(); i++)
		{
			vPos = (*m_vecObj)[i]->GetTransForm()->GetPos();
			vPos.y -= 300 * DT;
			(*m_vecObj)[i]->GetTransForm()->SetPos(vPos);
		}

		if (vPos.y < -1200.f)
		{
			CScene* pScene = CGameMgr::GetInst()->GetScene();
			if (pScene != nullptr)
			{
				CSaveLoadMgr::SetChangeScene(true);
				CSaveLoadMgr::SetScene(pScene);
				CGameMgr::GetInst()->SetScene(nullptr);
				CGameMgr::GetInst()->SetCinematic(false);
				CResMgr::GetInst()->SoundPlay(L"Stage1",-1);
			}
		}
	}
	return 0;
}