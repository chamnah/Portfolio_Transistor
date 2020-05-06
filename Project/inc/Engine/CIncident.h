#pragma once
#include "CComponent.h"

class CIncident :
	public CComponent
{
	CLONE(CIncident);

private:
	vector<EventObj> m_vecObj;
	vector<POINT>    m_vecTile;
	vector<CGameObj*> m_vecGlow;
	bool             m_bIncident;
	int				 m_iMonsterCount;
	bool		     m_bDelete;
	bool			 m_bHide;
	float		     m_fDeathTime;
	CGameObj*		 m_pResult;
	CGameObj*		 m_pScore;

public:
	int Update();

	vector<EventObj>& GetVecObj() { return m_vecObj; }
	vector<POINT>& GetTilePos() { return m_vecTile; }
	void AddObj(CGameObj* _pObj);
	void AddTile(POINT _ptPos) { m_vecTile.push_back(_ptPos); }
	void AddGlow(CGameObj* _pObj) { m_vecGlow.push_back(_pObj); }
	void SetMonsterCount(int _iCount) { m_iMonsterCount = _iCount; }


	void SetIncident(bool _bIncident) { m_bIncident = _bIncident; }
	bool GetIncident() { return m_bIncident; }
public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

	
public:
	CIncident();
	~CIncident();
};