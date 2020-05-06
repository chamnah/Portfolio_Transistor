#pragma once
#include <CScript.h>
class CCinematicScript :
	public CScript
{
private:
	float m_fAccTime;
	int   m_iCheck;
	float m_fTwinkleTime;
	vector<CGameObj*>* m_vecObj;
	bool  m_bEnd;

public:
	CLONE(CCinematicScript);
	int Update();
	
public:
	CCinematicScript();
	~CCinematicScript();
};