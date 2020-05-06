#include "CIncident.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGameObj.h"
#include "CGrid.h"
#include "CTile.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "CAnimator2D.h"
#include "CCore.h"
#include "TimeMgr.h"

int CIncident::Update()
{
	if (m_bDelete)
	{
		m_fDeathTime += DT;
		if (!m_bHide && m_fDeathTime > 5.f)
		{
			m_bHide = true;
			m_pResult->GetAnimator2D()->PlayAnim(L"ResultOut", false);
			CEventMgr::GetInst()->AddEvent(tEvent{EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pScore,0});
			m_pScore = nullptr;
			CCore::GetInst()->ClearText();
		}

		if (m_bHide && m_pResult->GetAnimator2D()->GetAnimFinish())
		{
			m_bDelete = false;
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 });
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_pResult,0 });
			m_pResult = nullptr;
		}

	}

	if (!m_bIncident)
		return 0;

	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Monster");
	vector<CGameObj*>& vecObj = pLayer->GetGameObj();

	if (!vecObj.empty())
		return 0;

	m_bDelete = true;

	CGameObj* pObj = nullptr;
	if (CSceneMgr::GetInst()->GetObjOfLayer(L"Tile", 0, &pObj))
	{
		for (size_t i = 0; i < m_vecTile.size(); i++)
		{
			pObj->GetGrid()->GetVecObj()[m_vecTile[i].y][m_vecTile[i].x]->GetTile()->SetCollType(COLL_TYPE::NOCOLL);
			int iColl = 0;
			pObj->GetGrid()->GetVecObj()[m_vecTile[i].y][m_vecTile[i].x]->GetMeshRender()->GetMtrl()->SetParam(&iColl, SHADER_PARAM::INT_0);
		}
	}

	for (UINT i = 0; i < m_vecGlow.size(); ++i)
	{
		tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecGlow[i], 0 };
		CEventMgr::GetInst()->AddEvent(event);
	}
	m_vecGlow.clear();

	m_pResult = CResMgr::GetInst()->PrefabToCreateObj(L"UI", L"ResultIn");
	m_pResult->GetAnimator2D()->PlayAnim(L"ResultIn", false);
	m_pResult->GetTransForm()->SetPos(Vec3(0.f,0.f,100.f));

	m_pScore = CResMgr::GetInst()->PrefabToCreateObj(L"UI", L"SuppressorIcon");
	m_pScore->GetTransForm()->SetPos(Vec3(-840,0,0));

	if (m_iMonsterCount != 0)
	{
		wchar_t szCount[10];
		_itow_s(m_iMonsterCount, szCount, 10);

		wstring wcsCount = L"x ";
		wcsCount += szCount;
		CCore::GetInst()->AddText(TextInfo{ wcsCount,Vec2{200,500},Vec2{300,600} });
	}


	m_bIncident = false;
	return 0;
}

void CIncident::AddObj(CGameObj * _pObj)
{
	m_vecObj.push_back(EventObj(_pObj->GetTransForm()->GetPos(),_pObj->GetName()));
}

CIncident::CIncident()
	:m_bIncident(false)
	, m_iMonsterCount(0)
	, m_bDelete(false)
	, m_fDeathTime(0.f)
	, m_pResult(nullptr)
	, m_pScore(nullptr)
	, m_bHide(false)
{
	m_eType = COMPONENT_TYPE::INCIDENT;
}

CIncident::~CIncident()
{
}

void CIncident::Save(FILE * _pFile)
{
	UINT iSize = m_vecObj.size();

	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		SaveWString(m_vecObj[i].wcsName, _pFile);
		fwrite(&m_vecObj[i].vPos,sizeof(Vec3),1,_pFile);
	}

	iSize = m_vecTile.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (size_t i = 0; i < m_vecTile.size(); i++)
	{
		fwrite(&m_vecTile[i], sizeof(POINT), 1, _pFile);
	}
}

void CIncident::Load(FILE * _pFile)
{
	UINT iSize = 0;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	EventObj eObj;

	for (size_t i = 0; i < iSize; i++)
	{
		eObj.wcsName = LoadWString(_pFile);
		fread(&eObj.vPos, sizeof(Vec3), 1, _pFile);
		m_vecObj.push_back(eObj);
	}

	fread(&iSize, sizeof(UINT), 1, _pFile);

	POINT ptPos = {};

	for (size_t i = 0; i < iSize; i++)
	{
		fread(&ptPos, sizeof(POINT), 1, _pFile);
		m_vecTile.push_back(ptPos);
	}
}