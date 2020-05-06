#pragma once
#include "CScript.h"

class CPlayerAnimScript :
	public CScript
{
private:
	float       m_fAnimAngle;
	ANIM_TYPE  m_eAnimType;
	SKILL_TYPE m_eSkillType;
	bool       m_bSkill;
	Vec3       m_vMousePos;
	bool       m_bRot;
	bool       m_bTurnSkill;
	Vec3       m_vStartPos;
	CGameObj*  m_pReload;
	float         m_fMoveDis;
	CGameObj*  m_pSpectrum;
	CGameObj*  m_pCharge;
	bool       m_bCharge;
	vector<CGameObj*> m_vecDot;
	Vec3       m_vPrePos;
	Vec3       m_vDir;

public:
	void Awake();
	int Update();
	void KeyInput();
	void MouseInput();
	void DirAnimPlay(wstring _wcsKey,Vec3 _vPos, Vec3 _vTargetPos);
	CLONE(CPlayerAnimScript);
	void SetSkillType(SKILL_TYPE _eType) { m_eSkillType = _eType; }

public:
	CPlayerAnimScript();
	~CPlayerAnimScript();
};