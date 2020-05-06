#pragma once
#include <CScript.h>
class CMHPScript :
	public CScript
{
public:
	int Update();
	CLONE(CMHPScript)

public:
	CMHPScript();
	~CMHPScript();
};

