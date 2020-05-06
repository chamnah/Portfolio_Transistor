#include "CGameMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "CResMgr.h"
#include "CTransForm.h"

CGameMgr::CGameMgr()
	:m_bTurn(false)
	, m_fMaxPoint(16.f)
	, m_fCurPoint(0.f)
	, m_fAccTime(0.f)
	, m_bPlayTurn(false)
	, m_iIconCount(0)
	, m_bCoolTime(false)
	, m_bTileRender(false)
	, m_bCinematic(false)
	, m_bUIRender(false)
	, m_pSecondScene(nullptr)
	, m_bEnd(false)
{
}

CGameMgr::~CGameMgr()
{
}

void CGameMgr::MouseInit()
{
	CResMgr::GetInst()->AddResource<CTexture>(L"Mouse", CTexture::CreateTexture(L"..\\content\\Texture\\Pointer.png"));
	m_pMouse = new CGameObj;
	m_pMouse->AddComponent(new CMeshRender);
	m_pMouse->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pMouse->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
	m_pMouse->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(L"Mouse"));
}

void CGameMgr::Update()
{
	if (KEYTAB(KEY_TYPE::KEY_TAB))
		m_bTileRender = !m_bTileRender;

	if (KEYTAB(KEY_TYPE::KEY_F1))
		m_bUIRender = !m_bUIRender;

	if (nullptr == m_pMouse)
		return;

	Vec3 vPos = CKeyMgr::GetInst()->GetMousePos() - g_World.mtxView.Translation();
	m_pMouse->GetTransForm()->SetPos(vPos);
	m_pMouse->Update();
	m_pMouse->FinalUpdate();
}

void CGameMgr::Render()
{
	if (nullptr == m_pMouse)
		return;
	m_pMouse->Render();
}

void CGameMgr::Reset()
{
	m_iIconCount = 0;
	m_fCurPoint = 0;
}

void CGameMgr::AddTurn(ANIM_TYPE _eType, Vec3 _vPos)
{
	if (!m_listTurn.empty())
	{
		if (m_listTurn.back().eType == ANIM_TYPE::MOVE &&
			_eType == ANIM_TYPE::MOVE)
		{
			if (m_fCurAllPoint + DT >= m_fMaxPoint)
				return;

			m_fCurPoint += DT;
			m_fCurAllPoint += DT;
			m_fAccTime += DT;

			if (m_fAccTime > 0.1f)
			{
				m_fAccTime = 0.f;
				m_listTurn.push_back(TURN(_eType, _vPos));
			}
			return;
		}
	}

	float fTemp = 0.f;
	m_fAccTime = 0.f;

	switch (_eType)
	{
	case ANIM_TYPE::MOVE:
		fTemp = DT;
		break;
	case ANIM_TYPE::CRASH:
		fTemp = 3.f;
		break;
	case ANIM_TYPE::BREACH:
		fTemp = 5.f;
		break;
	}

	if (m_fCurAllPoint + fTemp >= m_fMaxPoint)
		return;

	m_fCurPoint = fTemp;
	m_fCurAllPoint += m_fCurPoint;

	++m_iIconCount;
	m_listTurn.push_back(TURN(_eType,_vPos));
}