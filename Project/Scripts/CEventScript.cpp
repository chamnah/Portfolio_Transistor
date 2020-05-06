#include "CEventScript.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CIncident.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CGameObj.h"
#include "CGrid.h"
#include "CTile.h"
#include "CTileMgrScript.h"
#include "CMeshRender.h"
#include "CCollider.h"
#include "CRenderMgr.h"

CEventScript::CEventScript()
	:m_pColl(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CEVENTSCRIPT);
	CResMgr::GetInst()->AddResource<CTexture>(L"Glow", CTexture::CreateTexture(L"..\\content\\Texture\\green_glow.png"));
}

CEventScript::~CEventScript()
{
}

int CEventScript::Update()
{
	if (m_pColl != nullptr)
	{
		delete m_pColl;
		m_pColl = nullptr;
		GetOwner()->SetComponent(COMPONENT_TYPE::COLLIDER,nullptr);
	}
	return 0;
}

void CEventScript::Awake()
{
}

void CEventScript::Start()
{
	GetOwner()->SetColl(true);
}

void CEventScript::CollisionEnter(CCollider* _pColl)
{
	if (Incident() == nullptr)
		return;

	// 최적화 방법 미리 알고 있다가 호출을 해주면 될꺼 같다.
	if (CSceneMgr::GetInst()->GetLayerName(_pColl->GetLayerID()) == L"Player")
	{
		m_pColl = Collider();
		Incident()->SetIncident(true);
		vector<EventObj>& vecObj = Incident()->GetVecObj();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			CGameObj* pObj = CResMgr::GetInst()->PrefabToCreateObj(L"Monster", vecObj[i].wcsName);
			pObj->GetTransForm()->SetPos(vecObj[i].vPos);
			pObj->Start();
		}

		Incident()->SetMonsterCount(vecObj.size());
		
		// 벽 만들기 이벤트
		CGameObj* pObj = nullptr; 
		vector<POINT>& vecTile = Incident()->GetTilePos();
		for (size_t i = 0; i < vecTile.size(); i++)
		{
			if (CSceneMgr::GetInst()->GetObjOfLayer(L"Tile", 0, &pObj))
			{
				pObj->GetGrid()->GetVecObj()[vecTile[i].y][vecTile[i].x]->GetTile()->SetCollType(COLL_TYPE::COLL);
				int iColl = 1;
				pObj->GetGrid()->GetVecObj()[vecTile[i].y][vecTile[i].x]->GetMeshRender()->GetMtrl()->SetParam(&iColl, SHADER_PARAM::INT_0);
			}
		}

		vector<POINT> vecPos;
		CTileMgrScript::GetInst()->TileOutLineSearch(TransForm()->GetPos() + g_World.mtxView.Translation(), vecPos);

		pObj = nullptr;
		wstring wcsBlend = L"Alpha";
		if (CSceneMgr::GetInst()->GetObjOfLayer(L"Tile", 0, &pObj))
		{
			vector<vector<CGameObj*>>& vecTile = pObj->GetGrid()->GetVecObj();
			UINT iBack = CSceneMgr::GetInst()->GetLayerIndex(L"Effect");

			for (size_t i = 0; i < vecPos.size(); i++)
			{
				CGameObj* pGlow = new CGameObj;
				pGlow->AddComponent(new CMeshRender);
				Vec3 vPos = vecTile[vecPos[i].y][vecPos[i].x]->GetTransForm()->GetPos();
				vPos.z = 8000.f;
				pGlow->GetTransForm()->SetPos(vPos);
				pGlow->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
				pGlow->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
				pGlow->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(L"Glow"));
				pGlow->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsBlend));
				pGlow->GetTransForm()->SetScale(100.f, 100.f, 1.f);

				tEvent event = tEvent{ EVENT_TYPE::CREATE_OBJ,iBack,(INT_PTR)pGlow };
				CEventMgr::GetInst()->AddEvent(event);
				Incident()->AddGlow(pGlow);
				//vecTile[vecPos[i].y][vecPos[i].x]->GetTile()->SetScale(true);
			}
		}
	}
}