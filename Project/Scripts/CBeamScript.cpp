#include "CBeamScript.h"
#include "CTransForm.h"
#include "CAnimator2D.h"
#include "CTexture.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"

CBeamScript::CBeamScript()
	:m_fDis(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CBEAMSCRIPT);
}

CBeamScript::~CBeamScript()
{
}

int CBeamScript::Update()
{
	if (CSceneMgr::GetInst()->GetStop())
		return 0;

	Animator2D()->SetAutoSize(false);

	static float fScaleX = TransForm()->GetScale().x;

	if (fScaleX != TransForm()->GetScale().x)
	{
		fScaleX = TransForm()->GetScale().x;
	}

	if (m_vStart == nullptr || m_vEnd == nullptr)
		return 0;

	Vec3 vStart = *m_vStart;
	vStart.z = 0.f;
	Vec3 vEnd = *m_vEnd;
	vEnd.z = 0.f;

	float fDis = Vec3::Distance(vStart, vEnd);
	fDis -= 50.f;
	
	if (m_fDis != fabsf(fDis))
		m_fDis = fabsf(fDis);

	float fAnimSize = fDis / Animator2D()->GetCurAnimTexture()->GetWidth();
	Animator2D()->SetAnimSize(fAnimSize);

	TransForm()->SetScale(fDis, Animator2D()->GetCurAnimTexture()->GetHeight(),1.f);

	return 0;
}