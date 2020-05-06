#pragma once
#include <CScript.h>
class CHPScript :
	public CScript
{
public:
	int Update();
	CLONE(CHPScript)

public:
	CHPScript();
	~CHPScript();
};