#include "CToolScript.h"
#include "CCollider.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "CScriptMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CPrefab.h"
#include "CAnimator2D.h"
#include "CEffectScript.h"
#include "CMeshRender.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CTileMgrScript.h"
#include "CGameObj.h"
#include "CTile.h"

CToolScript::CToolScript()
	:m_bFrame(false)
	, m_bEvent(false)
	, m_eCollType(COLL_TYPE::NOCOLL)
	, m_bCollStop(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CTOOLSCRIPT);
}

CToolScript::~CToolScript()
{
}

void CToolScript::CollisionEnter(CCollider * _pColl)
{
}

void CToolScript::Collision(CCollider * _pColl)
{
	if (m_bEvent)
		return;

	if (KEYTAB(KEY_TYPE::KEY_RBTN))
	{
		if (!m_bFrame && m_pTargetObj != _pColl->GetOwner())
		{
			m_bFrame = true;
			m_pTargetObj = _pColl->GetOwner();
			CSceneMgr::GetInst()->FrontSorting(m_pTargetObj);
		}
	}

	if (m_eCollType == COLL_TYPE::DRAWCOLL)
	{
		if (KEYTAB(KEY_TYPE::KEY_LBTN))
		{
			if (!m_bFrame && m_pTargetObj != _pColl->GetOwner())
			{
				m_bFrame = true;
				m_pTargetObj = _pColl->GetOwner();
			}
		}
		else if (KEYHOLD(KEY_TYPE::KEY_LBTN))
		{
			if (m_pTargetObj == nullptr)
				return;

			if (m_pTargetObj != _pColl->GetOwner())
				return;

			Vec3 vMousePos = CKeyMgr::GetInst()->GetMousePos();
			vMousePos -= g_World.mtxView.Translation();
			vMousePos.z = m_pTargetObj->GetTransForm()->GetPos().z;
			m_pTargetObj->GetTransForm()->SetPos(vMousePos);
		}
	}
}

void CToolScript::CollisionExit(CCollider * _pColl)
{
}

int CToolScript::Update()
{
	m_bFrame = false;

	if (m_pTargetObj != nullptr && m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;

	
	Vec3 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	vMousePos -= g_World.mtxView.Translation();
	TransForm()->SetPos(vMousePos);

	if (m_bEvent)
		return 0;

	if (KEYTAB(KEY_TYPE::KEY_1))
		m_eCollType = COLL_TYPE::NOCOLL;
	else if (KEYTAB(KEY_TYPE::KEY_2))
		m_eCollType = COLL_TYPE::COLL;
	else if (KEYTAB(KEY_TYPE::KEY_3))
		m_eCollType = COLL_TYPE::NONE;
	else if (KEYTAB(KEY_TYPE::KEY_0))
		m_eCollType = COLL_TYPE::DRAWCOLL;

	if (m_eCollType == COLL_TYPE::NONE || m_eCollType == COLL_TYPE::DRAWCOLL)
		return 0;

	if (KEYHOLD(KEY_TYPE::KEY_LBTN))
	{
		Vec3 vPos = CKeyMgr::GetInst()->GetMousePos();
		POINT pt;
		CGameObj* pTile = CTileMgrScript::GetInst()->GetPickingTile(vPos, pt);

		if (pTile == nullptr)
			return 0;

		int i = (int)m_eCollType;
		pTile->GetTile()->SetCollType(m_eCollType);
		pTile->GetMeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);
	}

	return 0;
}

void CToolScript::ClearObj()
{
	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecObj[i] ,0 };
		CEventMgr::GetInst()->AddEvent(event);
	}
	m_vecObj.clear();
}