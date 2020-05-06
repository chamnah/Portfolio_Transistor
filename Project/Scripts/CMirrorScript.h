#pragma once
#include <CScript.h>
class CMirrorScript :
	public CScript
{
private:
	CGameObj* m_pMirror;
	CGameObj* m_pPlayer;
	ANIM_TYPE m_eAnimType;

public:
	void Start();
	int Update();
	CLONE(CMirrorScript);
public:
	CMirrorScript();
	~CMirrorScript();
};

