#pragma once
#include "global.h"

struct TURN
{
	ANIM_TYPE eType;
	Vec3      vPos; // 즉 여기에는 내가 이동할 좌표하고 내가 공격을 한 좌표 즉 방향을 담도록 하자.

	TURN(){}
	TURN(ANIM_TYPE _eType, Vec3 _vPos)
		:eType(_eType),vPos(_vPos)
	{}
};

class CGameObj;
class CScene;
class CGameMgr
{
private:
	bool m_bTurn;
	list<TURN> m_listTurn;
	float m_fMaxPoint;
	float m_fCurPoint;
	float m_fCurAllPoint;
	float m_fAccTime;
	bool m_bPlayTurn;
	int  m_iIconCount;
	bool m_bCoolTime;
	bool m_bTileRender;
	CGameObj* m_pMouse;
	bool m_bCinematic;
	bool m_bUIRender;
	CScene* m_pSecondScene;
	bool m_bEnd;

public:
	void SetTurn(bool _bTurn) { m_bTurn = _bTurn; }
	bool GetTurn() { return m_bTurn; }
	void SetPlayTurn(bool _bPlayTurn) { m_bPlayTurn = _bPlayTurn; }
	bool GetPlayTurn() { return m_bPlayTurn; }
	void Reset();
	int GetCount() { return m_iIconCount; }
	void SetCount(int _iCount) { m_iIconCount = _iCount; }
	void SubCount(int _iCount) { m_iIconCount -= _iCount; }

	void SetCoolTime(bool _bCoolTime) { m_bCoolTime = _bCoolTime; }
	bool GetCoolTime() { return m_bCoolTime; }

	float GetCurPoint() { return m_fCurPoint; }
	void SetCurAllPoint(float _fPoint) { m_fCurAllPoint = _fPoint; }
	float GetCurAllPoint() { return m_fCurAllPoint; }
	void SubCurAllPoint(float _fPoint) { m_fCurAllPoint -= _fPoint; }

	void SetTileRender(bool _bTile) { m_bTileRender = _bTile; }
	bool GetTileRender() { return m_bTileRender; }
	void ReversalTileRender() { m_bTileRender = !m_bTileRender; }

	void SetCinematic(bool _bCinematic) { m_bCinematic = _bCinematic; }
	bool GetCinematic() { return m_bCinematic; }

	void SetUIRender(bool _bUIRender) { m_bUIRender = _bUIRender; }
	bool GetUIRender() { return m_bUIRender; }

	void SetScene(CScene* _pScene) { m_pSecondScene = _pScene; }
	CScene* GetScene() {return m_pSecondScene;}

	void SetEnd(bool _bEnd) {m_bEnd = _bEnd;}
	bool GetEnd() { return m_bEnd; }

	void AddTurn(ANIM_TYPE _eType, Vec3 _vPos);
	list<TURN>& GetListTurn() { return m_listTurn; }

	void MouseInit();

	void Update();
	void Render();

public:
	SINGLETON(CGameMgr);
};