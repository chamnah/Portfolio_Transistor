#pragma once
#include "CScript.h"
class CTestScript :
	public CScript
{
	int Update() { return 0; }

	CLONE(CTestScript)

public:
	CTestScript();
	~CTestScript();
};