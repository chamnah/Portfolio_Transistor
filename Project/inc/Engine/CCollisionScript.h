#pragma once
#include "CScript.h"
class CCollisionScript :
	public CScript
{

public:
	int Update();
	CLONE(CCollisionScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);

public:
	CCollisionScript();
	~CCollisionScript();
};