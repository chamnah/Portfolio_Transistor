#include "CMirrorAlpha.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CTransForm.h"

int CMirrorAlpha::Update()
{
	if (m_pMirror == nullptr)
	{
		if (!CSceneMgr::GetInst()->GetObjOfLayer(L"Mirror", 0, &m_pMirror))
			return 0;
	}
	
	Vec3 vPos = TransForm()->GetPos();
	vPos.z = m_pMirror->GetTransForm()->GetPos().z - 20.f;
	TransForm()->SetPos(vPos);

 	return 0;
}

CMirrorAlpha::CMirrorAlpha()
	:m_pMirror(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::CMIRRORALPHA);
}

CMirrorAlpha::~CMirrorAlpha()
{
}