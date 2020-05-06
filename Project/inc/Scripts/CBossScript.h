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
	float  m_fAttackTime; // ���� �ð� �� �ð����� ������
	float  m_fSpacing; // �Ѿ� ���� �ð�
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