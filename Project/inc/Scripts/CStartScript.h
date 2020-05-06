#pragma once
#include <CScript.h>
class CScene;
class CStartScript :
	public CScript
{
private:
	CScene* m_pScene;
	bool    m_bFirst;

public:
	CLONE(CStartScript);
	int Update();

public:
	CStartScript();
	~CStartScript();
};