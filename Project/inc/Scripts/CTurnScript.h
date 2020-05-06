#pragma once
#include <CScript.h>
class CGameObj;
class CTurnScript :
	public CScript
{
private:
	UINT m_iUICount;
	list<CGameObj*> m_listUI;
	float m_fIconSize;

public:
	CLONE(CTurnScript);
	void Start();
	int Update();
	void SortUI();    // 매번 정렬 연산이 필요할때
	void OneSortUI(); // 딱 한번만 정렬하고 안쓸때

public:
	CTurnScript();
	~CTurnScript();
};