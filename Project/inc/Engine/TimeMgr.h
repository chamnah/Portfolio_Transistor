#pragma once
#include "global.h"

class CTimeMgr
{
	SINGLETON(CTimeMgr);
private:
	LARGE_INTEGER	m_llFrequency;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llOldCount;

	double			m_dt;	// 1프레임 시간
	UINT			m_iFPS;
	HDC				m_DC;

public:
	float DeltaTime()
	{
		if (m_dt >= 0.1f)
			return 0.1f;
		return (float)m_dt;
	}

public:
	void init(HWND _hwnd);
	int update();
	void render();
};

