#include "CChangeSceneScript.h"
#include "CScriptMgr.h"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CSaveLoadMgr.h"
#include "CPathMgr.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CTransForm.h"
#include "CRenderMgr.h"
#include "CGameMgr.h"

CChangeSceneScript::CChangeSceneScript()
	:m_pPage(nullptr)
	, m_bFirst(false)
	, m_pBlack(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCHANGESCENESCRIPT);
}

CChangeSceneScript::~CChangeSceneScript()
{
}

void CChangeSceneScript::CollisionEnter(CCollider * _pColl)
{
	if (CSceneMgr::GetInst()->GetLayerName(_pColl->GetOwner()->GetID()) == L"Player")
	{
		if (m_pPage == nullptr)
		{
			CTexture* pTex = nullptr;
			UINT iIdx = 0;
			Vec3 vPos = -g_World.mtxView.Translation();

			m_pBlack = new CGameObj;
			m_pBlack->AddComponent(new CMeshRender);
			m_pBlack->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
			m_pBlack->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
			vPos.z = 3.f;
			m_pBlack->GetTransForm()->SetPos(vPos);

			pTex = CResMgr::GetInst()->Load<CTexture>(L"Black");
			if (pTex == nullptr)
			{
				pTex = CTexture::CreateTexture(L"..\\content\\Texture\\Black.png");
				CResMgr::GetInst()->AddResource<CTexture>(L"Black", pTex);
			}
			m_pBlack->GetMeshRender()->SetTexture(pTex);
			m_pBlack->GetTransForm()->SetScale(1920.f, 1080.f, 1.f);

			iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Effect");
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,iIdx,(INT_PTR)m_pBlack });

			// 배경 까맣게 하고 알파블렌딩 하면 된다.
			m_pPage = new CGameObj;
			m_pPage->AddComponent(new CMeshRender);
			m_pPage->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
			m_pPage->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
			//CSceneMgr::GetInst()->SetStop(true);
			
			wstring wcsBlend = L"Alpha";
			m_pPage->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsBlend));

			vPos.z = 2.f;
			m_pPage->GetTransForm()->SetPos(vPos);

			pTex = CResMgr::GetInst()->Load<CTexture>(L"BackGround");
			if (pTex == nullptr)
			{
				pTex = CTexture::CreateTexture(L"..\\content\\Texture\\BackGround.png");
				CResMgr::GetInst()->AddResource<CTexture>(L"BackGround", pTex);
			}
			m_pPage->GetMeshRender()->SetTexture(pTex);

			iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Effect");
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJ,iIdx,(INT_PTR)m_pPage });

			CResMgr::GetInst()->Load<CSound>(L"Stage1")->Stop();

			wstring wcsPath = CPathMgr::GetInst()->GetPath();
			wcsPath += L"Sound\\End.mp3";
			CSound* sound = new CSound;
			sound->Load(wcsPath, L"End");
			CResMgr::GetInst()->AddResource<CSound>(L"End", sound);
			CResMgr::GetInst()->Load<CSound>(L"End")->Play(-1);

			CGameMgr::GetInst()->SetEnd(true);
		}
	}
}

void CChangeSceneScript::Start()
{
	GetOwner()->SetColl(true);
}

int CChangeSceneScript::Update()
{
	if (m_pPage != nullptr)
	{
		if (m_bFirst)
		{
			Vec3 vPos = -g_World.mtxView.Translation();
			vPos.z = 3.f;
			m_pBlack->GetTransForm()->SetPos(vPos);
			vPos.z = 2.f;
			m_pPage->GetTransForm()->SetPos(vPos);
			//CSceneMgr::GetInst()->SetStop(false);
			m_pPage = nullptr;
			CSceneMgr::GetInst()->SetDelObj(false);
			CSaveLoadMgr::SetLoadScene(true);
			wstring wcsContent = CPathMgr::GetInst()->GetPath();
			wcsContent += L"Scene\\Boss.scene";
			CSaveLoadMgr::SetPath(wcsContent);
		}
		m_bFirst = true;
	}

	return 0;
}
