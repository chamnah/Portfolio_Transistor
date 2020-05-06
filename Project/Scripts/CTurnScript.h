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
	void SortUI();    // �Ź� ���� ������ �ʿ��Ҷ�
	void OneSortUI(); // �� �ѹ��� �����ϰ� �Ⱦ���

public:
	CTurnScript();
	~CTurnScript();
};