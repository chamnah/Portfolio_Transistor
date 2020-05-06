#pragma once
#include <CScript.h>
class CGameObj;
class CToolScript :
	public CScript
{
private:
	CGameObj* m_pTargetObj;
	bool      m_bFrame;
	bool      m_bEvent;
	vector<CGameObj*>  m_vecObj;
	bool      m_bStop;
	COLL_TYPE m_eCollType;
	bool      m_bCollStop;

public:
	virtual void CollisionEnter(CCollider* _pColl);
	virtual void Collision(CCollider* _pColl);
	virtual void CollisionExit(CCollider* _pColl);

	CLONE(CToolScript);
	int Update();

	vector<CGameObj*>& GetVecObj() { return m_vecObj; }

	CGameObj* GetTargetObj() { return m_pTargetObj; }
	void SetEvent(bool _bEvent) { m_bEvent = _bEvent; }


	void ClearObj();
public:
	CToolScript();
	~CToolScript();
};