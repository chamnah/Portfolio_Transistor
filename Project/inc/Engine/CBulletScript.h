#pragma once
#include "CScript.h"
class CBulletScript :
	public CScript
{
private:
	Vec3 vPos;

public:
	int Update();

	CLONE(CBulletScript)

public:
	CBulletScript();
	~CBulletScript();
};