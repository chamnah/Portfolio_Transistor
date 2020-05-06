#pragma once
#include "CUnitScript.h"

enum class ATTACK_TYPE
{
	BEAM,
	TAIL
};

class CBossScript :
	public CUnitScript
{
private:
	float  m_fNum;
	float  m_fCoolTime;
	float  m_fAttackTime; // 공격 시간 이 시간동안 공격함
	float  m_fSpacing; // 총알 간격 시간
	float  m_fHitTime;
	ATTACK_TYPE m_eAttackType;
	bool   m_bTail;
	CGameObj* m_pTarget;

	CLONE(CBossScript);

public:
	void Start();
	void Awake();
	int Update();

public:
	CBossScript();
	~CBossScript();
};