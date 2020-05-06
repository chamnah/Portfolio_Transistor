#pragma once
#include "CScript.h"
class CTest2Script :
	public CScript
{
public:

	int Update() { return 0; }
	CLONE(CTest2Script)


public:
	CTest2Script();
	~CTest2Script();
};

