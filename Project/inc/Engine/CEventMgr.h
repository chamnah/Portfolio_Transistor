#pragma once

#include "global.h"
class CGameObj;
class CEventMgr
{

	SINGLETON(CEventMgr);
private:
	vector<tEvent> m_vecEvent;
	vector<CGameObj*> m_vecDel;
	bool            m_bReset;
	bool            m_bAddObj;
	bool           m_bChangeScene;
	CGameObj*       m_pNewObj;


public:
	void Init();
	int Update();

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }
	void Event(tEvent& _event);
	bool IsReset() { return m_bReset; }
	bool IsAddObj() { return m_bAddObj; }
	CGameObj* GetNewObj() { return m_pNewObj; }
	void SetNewObj(CGameObj* _pObj) { m_pNewObj = _pObj; }
	void SetReset(bool _bReset) { m_bReset = _bReset; }
	void SetAddObj(bool _bAddObj) { m_bAddObj = _bAddObj; }
};