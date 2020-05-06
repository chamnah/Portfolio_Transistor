#pragma once
#include "CUnitScript.h"
class CGameObj;
class CSuppressorScript :
	public CUnitScript
{
private:
	CGameObj* m_pLeg;
	Vec3      m_vPlayerPos;
	CGameObj* m_pBeamCenter;
	Vec3      m_vStartPos;
	bool      m_bGravity;
	CGameObj* m_pHPbar;
	CGameObj* m_pHP;
	bool      m_bLegAnim;
	bool      m_bLeg;

public:
	void Awake();
	void Start();
	int Update();

	CLONE(CSuppressorScript)

public:
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CSuppressorScript();
	~CSuppressorScript();
};