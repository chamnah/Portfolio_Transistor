#pragma once
#include <CScript.h>
class CChangeSceneScript :
	public CScript
{
private:
	CGameObj* m_pPage;
	CGameObj* m_pBlack;
	bool      m_bFirst;

public:
	void CollisionEnter(CCollider* _pColl);
	CLONE(CChangeSceneScript);
	virtual void Start();
	int Update();

public:
	CChangeSceneScript();
	~CChangeSceneScript();
};