#pragma once
#include "CScript.h"
class CCollisionScript :
	public CScript
{
private:
	Vec3   m_vCollMesh[4];
	Vec3   m_vPrePos;
	Vec3   m_vNextPos;
	CGameObj* m_pCollTile;
	Mtx    m_mtxColl;

public:
	int Update();
	CLONE(CCollisionScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);

public:
	void Bounce(CGameObj* _pObj);
	bool Coll();
	int FinalUpdate();

public:
	CCollisionScript();
	~CCollisionScript();
};