#pragma once
#include "CScript.h"
class CPlayerAnimScript :
	public CScript
{
private:
	Vec3       m_vRot;
public:
	void Awake();
	int Update();
	void KeyInput();
	void MouseInput();
	CLONE(CPlayerAnimScript);

public:
	CPlayerAnimScript();
	~CPlayerAnimScript();
};