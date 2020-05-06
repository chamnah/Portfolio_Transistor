#include "TimeMgr.h"

CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llCurCount{}
	, m_llOldCount{}
	, m_dt(0.)
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init(HWND _hwnd)
{
	QueryPerformanceFrequency(&m_llFrequency); // 1초에 타이머가 셀수있는 진동수	
	QueryPerformanceCounter(&m_llOldCount);	

	m_DC = GetDC(_hwnd);
}

int CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dt = (double)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (double)m_llFrequency.QuadPart;
	
	m_llOldCount = m_llCurCount;

	// 1초에 1번 계산
	static double accTime = 0.f;
	accTime += m_dt;
	if (accTime > 1.)
	{
		accTime = 0.;
		m_iFPS = (UINT)(1. / m_dt);
	}	

	return 0;
}

void CTimeMgr::render()
{	
	wchar_t szFPS[10];
	_itow_s(m_iFPS, szFPS, 10);

	TextOutW(m_DC,100 ,100 ,szFPS ,5);
}
