#pragma once
#include "CScript.h"

class CPrefab;
class CPlayerScript :
	public CScript
{
private:
	CPrefab*      m_pBullet;

public:
	void Awake();
	int Update();
	CLONE(CPlayerScript)

public:
	void CollisionEnter(CCollider* _pColl);
	void Collision(CCollider* _pColl);
	void CollisionExit(CCollider* _pColl);

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};