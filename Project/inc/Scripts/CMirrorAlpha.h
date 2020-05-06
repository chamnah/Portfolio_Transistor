#pragma once
#include <CScript.h>
class CMirrorAlpha :
	public CScript
{
private:
	CGameObj*   m_pMirror;

public:
	int Update();
	CLONE(CMirrorAlpha);
public:
	CMirrorAlpha();
	~CMirrorAlpha();
};