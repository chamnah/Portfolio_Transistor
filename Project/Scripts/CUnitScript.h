#pragma once
#include <CScript.h>
class CUnitScript :
	public CScript
{
protected:
	float      m_fCurHP;
	float      m_fMaxHP;
	OBJ_STATE m_eState;
	float     m_fAccTime;
	Vec3      m_vKnockbackDir;
	Vec3      m_vKnockbackPos;

public:
	void SetCurHP(float _fHP) { m_fCurHP = _fHP; }
	float  GetCurHP() { return m_fCurHP; }
	void SetMaxHP(float _fHP) { m_fMaxHP = _fHP; }
	float  GetMaxHP() { return m_fMaxHP; }

	void SetDamage(float _fDamage);
	void SetPlayerDamage(float _fDamage);

	bool Death();

	void SetState(OBJ_STATE _eState) { m_eState = _eState; }
	OBJ_STATE GetState() { return m_eState; }
	void SetKnockback(Vec3 _vDir);
	Vec3 GetKnockbackDir() { return m_vKnockbackDir; }
	Vec3 GetKnockbackPos() { return m_vKnockbackPos; }

public:
	CUnitScript();
	~CUnitScript();
};