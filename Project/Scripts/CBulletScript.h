#pragma once
#include "CScript.h"
class CBulletScript :
	public CScript
{
private:
	Vec3 m_vTargetPos;
	Vec3 m_vMousePos;
	Vec3 m_vDistance;
	bool m_bDead;
	Vec3 m_vDir;
	float  m_fRange;
	TEAM_TYPE m_eTeamType;
	float m_fZSort;

public:
	void Awake();
	int Update();
	void SetMousePos(Vec3& _vPos) { m_vMousePos = _vPos; }
	void SetRange(float _fRange) { m_fRange = _fRange; }
	void ZSort(float _z) { m_fZSort = _z; }

	void SetTeamType(TEAM_TYPE _eType) { m_eTeamType = _eType; }
	TEAM_TYPE GetTeamType() { return m_eTeamType; }

	CLONE(CBulletScript)

public:
	void Collision(CCollider* _pColl);
	void CollisionExit(CCollider* _pColl);
	void CollisionEnter(CCollider* _pColl);

public:
	CBulletScript();
	~CBulletScript();
};