#pragma once
#include "CRes.h"
class CGameObj;
class CPrefab :
	public CRes
{
private:
	CGameObj*  m_pObj;

public:
	CGameObj* Instantiate();
	CGameObj* GetOriginalObj() { return m_pObj; }

public:
	CPrefab(CGameObj* _Obj);
	~CPrefab();
};
