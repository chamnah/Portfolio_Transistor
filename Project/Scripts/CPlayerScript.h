#pragma once
#include "CUnitScript.h"

class CPrefab;
class CPlayerScript :
	public CUnitScript
{
private:
	CPrefab*      m_pBullet;
	float         m_fHitTime;
	bool          m_bHit;

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