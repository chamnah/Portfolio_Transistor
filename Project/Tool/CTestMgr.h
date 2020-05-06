#pragma once

class CGameObj;
class CTestMgr
{
private:
	CGameObj* m_pMouse;
	CGameObj* m_pTarget;
	COLL_TYPE m_eCollType;
	bool      m_bStop;

public:
	void Init();
	void Update();
	void Render();
	void UpdateData();
	CGameObj* GetTargetObj() { return m_pTarget; }
	CGameObj* GetMouseObj() { return m_pMouse; }
	void SetStop(bool _bStop) { m_bStop = _bStop; }

SINGLETON(CTestMgr)
};