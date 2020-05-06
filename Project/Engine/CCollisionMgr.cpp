#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CCollider.h"
#include "CLayer.h"

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CollisionCheck(UINT _iFirst, UINT _iSecond)
{
	UINT iColl = _iFirst;
	UINT iRow = _iSecond;

	if (iColl > iRow)
	{
		iColl = _iSecond;
		iRow = _iFirst;
	}

	UINT iCheck = 1 << iRow;
	m_arrCollCheck[iColl] |= iCheck;
}

void CCollisionMgr::CollisionCheck(wstring _wcsFirst, wstring _wcsSecond)
{
	UINT iFirst = CSceneMgr::GetInst()->GetLayer(_wcsFirst)->GetID();
	UINT iSecond = CSceneMgr::GetInst()->GetLayer(_wcsSecond)->GetID();

	CollisionCheck(iFirst,iSecond);
}

int CCollisionMgr::Update()
{
	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
	{
		for (UINT j = i; j < MAX_LAYER_COUNT; ++j)
		{
			if (m_arrCollCheck[i] & (1 << j))
			{
				Collision(i,j);
			}
		}
	}

	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
		m_arrColl[i].clear();

	return 0;
}

void CCollisionMgr::Collision(UINT _iFirst, UINT _iSecond)
{
	for (UINT i = 0; i < m_arrColl[_iFirst].size(); ++i)
	{
		for (UINT j = 0; j < m_arrColl[_iSecond].size(); ++j)
		{
			DiscID id;
			id.dwFirst = m_arrColl[_iFirst][i]->GetID();
			id.dwSecond = m_arrColl[_iSecond][j]->GetID();

			map<ULONGLONG,bool>::iterator iter = m_mapID.find(id.ID);

			if (IsCollision(m_arrColl[_iFirst][i], m_arrColl[_iSecond][j])) // 충돌
			{
				if (iter == m_mapID.end()) // 처음 충돌한 상태이다.
				{
					m_arrColl[_iFirst][i]->CollisionEnter(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionEnter(m_arrColl[_iFirst][i]);
					m_mapID.insert(make_pair(id.ID,true));
				}
				else if (iter->second == false) // 이전에 충돌을 안한 상태에서 충돌 했다.
				{
					iter->second = true;
					m_arrColl[_iFirst][i]->CollisionEnter(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionEnter(m_arrColl[_iFirst][i]);
				}
				else // 나머지 즉 충돌 중인 상태에서 또 충돌을 했다.
				{
					m_arrColl[_iFirst][i]->Collision(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->Collision(m_arrColl[_iFirst][i]);
				}
			}
			else if(iter != m_mapID.end()) // 충돌 안함
			{
				if (iter->second == true)
				{
					iter->second = false;

					m_arrColl[_iFirst][i]->CollisionExit(m_arrColl[_iSecond][j]);
					m_arrColl[_iSecond][j]->CollisionExit(m_arrColl[_iFirst][i]);
				}
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider * _pFirst, CCollider * _pSecond)
{
	static Vec3 CollRect[2][4];
	if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_BOX && 
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_BOX)
	{
		for (int i = 0; i < 2; ++i)
		{
			CollRect[i][0] = Vec3(-0.5f, 0.5f, 0.f);
			CollRect[i][1] = Vec3(0.5f, 0.5f, 0.f);
			CollRect[i][2] = Vec3(0.5f, -0.5f, 0.f);
			CollRect[i][3] = Vec3(-0.5f, -0.5f, 0.f);
		}
	}
	else if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_ISO &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_ISO)
	{
		for (int i = 0; i < 2; ++i)
		{
			CollRect[i][0] = Vec3(-0.5f, 0.f, 0.f);
			CollRect[i][1] = Vec3(0.f, 0.5f, 0.f);
			CollRect[i][2] = Vec3(0.5f, 0.f, 0.f);
			CollRect[i][3] = Vec3(0.f, -0.5f, 0.f);
		}
	}
	else if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_BOX &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_ISO)
	{
		CollRect[0][0] = Vec3(-0.5f, 0.5f, 0.f);
		CollRect[0][1] = Vec3(0.5f, 0.5f, 0.f);
		CollRect[0][2] = Vec3(0.5f, -0.5f, 0.f);
		CollRect[0][3] = Vec3(-0.5f, -0.5f, 0.f);

		CollRect[1][0] = Vec3(-0.5f, 0.f, 0.f);
		CollRect[1][1] = Vec3(0.f, 0.5f, 0.f);
		CollRect[1][2] = Vec3(0.5f, 0.f, 0.f);
		CollRect[1][3] = Vec3(0.f, -0.5f, 0.f);
	}
	else if (_pFirst->GetColliderType() == COLLIDER_TYPE::COLL_ISO &&
		_pSecond->GetColliderType() == COLLIDER_TYPE::COLL_BOX)
	{
		CollRect[0][0] = Vec3(-0.5f, 0.f, 0.f);
		CollRect[0][1] = Vec3(0.f, 0.5f, 0.f);
		CollRect[0][2] = Vec3(0.5f, 0.f, 0.f);
		CollRect[0][3] = Vec3(0.f, -0.5f, 0.f);

		CollRect[1][0] = Vec3(-0.5f, 0.5f, 0.f);
		CollRect[1][1] = Vec3(0.5f, 0.5f, 0.f);
		CollRect[1][2] = Vec3(0.5f, -0.5f, 0.f);
		CollRect[1][3] = Vec3(-0.5f, -0.5f, 0.f);

	}

	/*static Vec3 CollRect[4] =
	{
		Vec3(-0.5f,0.f,0.f)
		Vec3(0.f,0.5f,0.f)
		Vec3(0.5f,0.f,0.f)
		Vec3(0.f,-0.5f,0.f)
	};*/
	Vec3 vFirstPoint[3] = {};

	Vec3 vPoint[3] = {};
	Vec3 vSource[4] = {};
	Vec3 vProj[4] = {};

	//_pFirst->MeshRender();

	Mtx mtxFirst = _pFirst->GetWorldMtx();
	Mtx mtxSecond = _pSecond->GetWorldMtx();

	vPoint[0] = XMVector3TransformCoord(CollRect[0][3], _pFirst->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(CollRect[0][0], _pFirst->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(CollRect[0][2], _pFirst->GetWorldMtx());

	vFirstPoint[0] = XMVector3TransformCoord(CollRect[0][3], _pFirst->GetWorldMtx());
	vFirstPoint[1] = XMVector3TransformCoord(CollRect[0][0], _pFirst->GetWorldMtx());
	vFirstPoint[2] = XMVector3TransformCoord(CollRect[0][2], _pFirst->GetWorldMtx());


	vSource[0] = vPoint[2] - vPoint[0];
	vSource[1] = vPoint[1] - vPoint[0];

	vProj[0] = vSource[0]; vProj[0].Normalize();
	vProj[1] = vSource[1]; vProj[1].Normalize();

	vPoint[0] = XMVector3TransformCoord(CollRect[1][3], _pSecond->GetWorldMtx());
	vPoint[1] = XMVector3TransformCoord(CollRect[1][0], _pSecond->GetWorldMtx());
	vPoint[2] = XMVector3TransformCoord(CollRect[1][2], _pSecond->GetWorldMtx());

	vSource[2] = vPoint[2] - vPoint[0];
	vSource[3] = vPoint[1] - vPoint[0];

	vProj[2] = vSource[2]; vProj[2].Normalize();
	vProj[3] = vSource[3]; vProj[3].Normalize();

	Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, _pFirst->GetWorldMtx()) - XMVector3TransformCoord(Vec3::Zero, _pSecond->GetWorldMtx());

	for (int i = 0; i < 4; ++i)
	{
		float fDis = 0.f;
		float fCenter = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDis += abs(vSource[j].Dot(vProj[i]));
		}

		fDis *= 0.5f;
		fCenter = abs(vCenter.Dot(vProj[i]));

		if (fCenter >= fDis)
			return false;
	}

	return true;
}