#include "CEffectScript.h"
#include "CAnimator2D.h"
#include "CEventMgr.h"
#include "CScriptMgr.h"

CEffectScript::CEffectScript()
	:m_bDelete(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::CEFFECTSCRIPT);
}

CEffectScript::~CEffectScript()
{
}

int CEffectScript::Update()
{
	if (Animator2D() == nullptr)
		return 0;

	if (Animator2D()->GetAnimFinish() && !m_bDelete)
	{
		m_bDelete = true;
		tEvent evnet = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)GetOwner(),0 };
		CEventMgr::GetInst()->AddEvent(evnet);
	}
	
	return 0;
}