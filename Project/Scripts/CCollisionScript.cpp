#include "CCollisionScript.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CScriptMgr.h"
#include "CTransForm.h"
#include "CTile.h"
#include "CTileMgrScript.h"
#include "CMesh.h"

CCollisionScript::CCollisionScript()
	:m_pCollTile(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CCOLLISIONSCRIPT);

	m_vCollMesh[0] = Vec3(-0.5f, 0.f, 0.f);
	m_vCollMesh[1] = Vec3(0.f, 0.5f, 0.f);
	m_vCollMesh[2] = Vec3(0.5f, 0.f, 0.f);
	m_vCollMesh[3] = Vec3(0.f, -0.5f, 0.f);
}

CCollisionScript::~CCollisionScript()
{
}

int CCollisionScript::Update()
{
	return 0;
}

void CCollisionScript::CollisionEnter(CCollider * _pColl)
{
}
     
void CCollisionScript::Collision(CCollider * _pColl)
{
	//CollisionEnter(_pColl);
}

void CCollisionScript::Bounce(CGameObj* _pObj)
{
	//Vec3 vPos = TransForm()->GetPos();
	Vec3 vPos;
	Vec3 vCollPos = Collider()->GetPos();

	Vec3 vTarget = _pObj->GetTransForm()->GetPos();// + g_World.mtxView.Translation();
	Vec3 vPlayer = Collider()->GetPos() + TransForm()->GetPos();

	POINT pt = {};
	CGameObj* pTile = CTileMgrScript::GetInst()->GetPickingTile(vPlayer, pt);

	if (pTile != nullptr &&  pTile->GetTile() != nullptr && pTile->GetTile()->GetCollType() == COLL_TYPE::NOCOLL)
	{
		vPos = pTile->GetTransForm()->GetPos();
		TransForm()->SetPos(vPos);
	}

	/*Vec3 vMove;
	vMove.x = (int)vPos.x;
	vMove.y = (int)vPos.y;
	vMove.z = (int)vPos.z;*/

	//TransForm()->SetPos(vPos);
}

bool CCollisionScript::Coll()
{
	POINT pt = {};
	Vec3 vPos;
	bool bColl = false;
	m_mtxColl = Collider()->GetWorldMtx() * g_World.mtxView;

	for (UINT i = 0; i < 4; ++i)
	{
		vPos = XMVector3TransformCoord(m_vCollMesh[i], m_mtxColl); // 여기서 쓰이는 매트릭스 자체가 이전 좌표로 계산을 한것이다.
		m_pCollTile = CTileMgrScript::GetInst()->GetPickingTile(vPos, pt);
		if (m_pCollTile != nullptr &&  m_pCollTile->GetTile() != nullptr && m_pCollTile->GetTile()->GetCollType() == COLL_TYPE::COLL)
		{
			bColl = true;
			break;
		}
	}

	return bColl;
}

int CCollisionScript::FinalUpdate()
{
	Vec3 vPos = TransForm()->GetPos();

	if (Collider() == nullptr)
		return 0;

	if (Coll())
	{
		TransForm()->SetPos(m_vPrePos);
		TransForm()->FinalUpdate();
		Collider()->MatrixUpdate();
	}
	else
		m_pCollTile = nullptr;

	m_vPrePos = TransForm()->GetPos();

	return 0;
}

	//if (vTarget.x <= vPlayer.x && // 내가 오른쪽 아래
	//	vTarget.y > vPlayer.y)
	//{
	//	while (true)
	//	{
	//		vPos += Vec3(0.5f, -0.5f, 0);
	//		TransForm()->SetPos(vPos);
	//		TransForm()->FinalUpdate();
	//		Collider()->MatrixUpdate();

	//		if (!Coll())
	//			break;
	//	}
	//}
	//else if (vTarget.x <= vPlayer.x &&  // 내가 오른쪽 위
	//	vTarget.y <= vPlayer.y)
	//{
	//	while (true)
	//	{
	//		vPos += Vec3(0.5f, 0.5f, 0);
	//		TransForm()->SetPos(vPos);
	//		TransForm()->FinalUpdate();
	//		Collider()->MatrixUpdate();

	//		if (!Coll())
	//			break;
	//	}
	//}
	//else if (vTarget.x > vPlayer.x && // 내가 왼쪽 아래
	//	vTarget.y > vPlayer.y)
	//{
	//	while (true)
	//	{
	//		vPos += Vec3(-0.5f, -0.5f, 0);
	//		TransForm()->SetPos(vPos);
	//		TransForm()->FinalUpdate();
	//		Collider()->MatrixUpdate();

	//		if (!Coll())
	//			break;
	//	}
	//}
	//else if (vTarget.x > vPlayer.x && // 내가 왼쪽 위
	//	vTarget.y <= vPlayer.y)
	//{
	//	while (true)
	//	{
	//		vPos += Vec3(-0.5f, 0.5f, 0);
	//		TransForm()->SetPos(vPos);
	//		TransForm()->FinalUpdate();
	//		Collider()->MatrixUpdate();

	//		if (!Coll())
	//			break;
	//	}
	//}