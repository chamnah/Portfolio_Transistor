#pragma once
#include "global.h"

struct tKey
{
	KEY_TYPE	eType;
	KEY_STATE	eState;
};

class CKeyMgr
{
	SINGLETON(CKeyMgr);
private:
	vector<tKey>	m_vecKey;
	Vec3			m_vMousePos;
	HWND			m_hWnd;

public:
	bool GetKeyState(KEY_TYPE _eType, KEY_STATE _eState)
	{
		if (_eState == m_vecKey[(UINT)_eType].eState)
			return true;
		else
			return false;
	};
	const Vec3& GetMousePos();
	void SetHandle(HWND _hWnd) { m_hWnd = _hWnd; }
	POINT TilePicking(){}

public:
	void init();
	void update();
};
