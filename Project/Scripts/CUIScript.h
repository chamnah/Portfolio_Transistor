#pragma once
#include <CScript.h>
class CUIScript :
	public CScript
{
private:
	SKILL_TYPE m_eSkill;
	//CGameObj*  m_pActive;
	//CGameObj*  m_pSkill;

public:
	CLONE(CUIScript);

	void Awake();
	int Update();

	SKILL_TYPE GetSkillType() { return m_eSkill; }

public:
	CUIScript();
	~CUIScript();
};