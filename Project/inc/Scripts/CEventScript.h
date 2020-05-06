#pragma once
#include <CScript.h>
class CEventScript :
	public CScript
{
private:
	CCollider*  m_pColl;

public:
	virtual int Update();
	virtual void Awake();
	virtual void Start();
	CLONE(CEventScript);

public:
	virtual void CollisionEnter(CCollider* _pColl);

public:
	CEventScript();
	~CEventScript();
};