#include "CPrefab.h"
#include "CGameObj.h"

CPrefab::CPrefab(CGameObj* _Obj)
	:m_pObj(_Obj)
{
}

CPrefab::~CPrefab()
{
	SAFE_DELETE(m_pObj);
}

CGameObj* CPrefab::Instantiate()
{
	return m_pObj->Clone();
}