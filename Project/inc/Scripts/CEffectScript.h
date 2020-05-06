#pragma once
#include <CScript.h>
class CEffectScript :
	public CScript
{

private:
	bool m_bDelete;

public:
	int Update();

	CLONE(CEffectScript)
public:
	CEffectScript();
	~CEffectScript();
};