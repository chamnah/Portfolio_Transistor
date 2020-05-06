#pragma once
#include <CScript.h>
class CBeamScript :
	public CScript
{
private:
	Vec3* m_vStart;
	Vec3* m_vEnd;
	float m_fDis;

public:
	int Update();

	CLONE(CBeamScript)

public:
	void SetStartEnd(Vec3* _vStart, Vec3* _vEnd) { m_vStart = _vStart; m_vEnd = _vEnd; }

public:
	CBeamScript();
	~CBeamScript();
};