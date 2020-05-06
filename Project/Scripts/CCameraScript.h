#pragma once
#include "CScript.h"

class CCameraScript :
	public CScript
{
public:
	int Update();
	CLONE(CCameraScript)

public:
	CCameraScript();
	~CCameraScript();
};