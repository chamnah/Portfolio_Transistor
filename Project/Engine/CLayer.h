#pragma once
#include "CParent.h"

class CGameObj;
class CLayer :
	public CParent
{
private:
	vector<CGameObj*> m_vecObj;
	int     m_iLayerID;

public:
	void Awake();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void Render();
	void AddGameObject(CGameObj* _pObj);
	void PopGameObject(CGameObj* _pObj);
public:
	void SetID(int _iID) { m_iLayerID = _iID;}
	int  GetID() { return m_iLayerID; }
	vector<CGameObj*>& GetGameObj() { return m_vecObj; }

public:
	CLayer();
	~CLayer();
};