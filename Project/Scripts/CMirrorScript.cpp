#include "CMirrorScript.h"
#include "CSceneMgr.h"
#include "CTransForm.h"
#include "CAnimator2D.h"
#include "CScriptMgr.h"

CMirrorScript::CMirrorScript()
	: m_pMirror(nullptr)
	, m_pPlayer(nullptr)
	, m_eAnimType(ANIM_TYPE::NONE)
{
	SetScriptType((UINT)SCRIPT_TYPE::CMIRRORSCRIPT);
}

CMirrorScript::~CMirrorScript()
{
}

void CMirrorScript::Start()
{
}

int CMirrorScript::Update()
{
	if (Animator2D() == nullptr)
		return 0;

	if (Animator2D()->GetAnimation().empty())
		return 0;

	if (m_pMirror == nullptr)
	{
		if (!CSceneMgr::GetInst()->GetObjOfLayer(L"Mirror", 0, &m_pMirror))
			return 0;
	}

	if (m_pPlayer == nullptr)
	{
		if (!CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &m_pPlayer))
			return 0;
	}

	Vec3 vMirrorPos = m_pMirror->GetTransForm()->GetPos();
	Vec3 vMirrorScale = m_pMirror->GetTransForm()->GetScale();
	Vec3 vPlayerPos = m_pPlayer->GetTransForm()->GetPos();
	wstring wcsKey = m_pPlayer->GetAnimator2D()->GetCurKey();

	
	float fGra = Gradient(Vec2{ vMirrorPos.x - vMirrorScale.x * 0.5f,vMirrorPos.y - 30.f }, Vec2{ vMirrorPos.x + vMirrorScale.x * 0.5f,vMirrorPos.y - vMirrorScale.y * 0.5f });

	float fMoveY = vMirrorPos.y - vMirrorScale.y * 0.5f; // 선위에 좌표Y
	float Y = -(fGra * vMirrorPos.x) + fMoveY; // Y절편 구하기
	float fGradient = fGra * vPlayerPos.x + Y; // 최종 방정식

	if (wcsKey == L"")
		return 0;

	wstring wcsNum = wcsKey.substr(wcsKey.length() - 2, 2);
	wstring wcsClip = wcsKey.substr(0, wcsKey.length() - 2);
	int iNum = 0;
	if (wcsClip == L"Run")
	{
		iNum = _wtoi(wcsNum.c_str());
		if (iNum != 0)
			iNum = 32 - iNum;
	}
	else
	{
		iNum = _wtoi(wcsNum.c_str());
		if (iNum != 0)
			iNum = 16 - iNum;
	}

	wchar_t szNum[10];
	_itow_s(iNum, szNum,10);
	wcsNum = szNum;
	if (iNum < 10)
		wcsNum = L"0" + wcsNum;

	Animator2D()->PlayAnim(wcsClip + wcsNum, true);

	float fY = vPlayerPos.y - fGradient;

	Vec3 vPos;
	vPos.x = vPlayerPos.x;
	vPos.y = fGradient - fY + 250;//fGradient.y ;
	vPos.z = vMirrorPos.z - 10.f;
	TransForm()->SetPos(vPos);
	TransForm()->SetRotaion(m_pPlayer->GetTransForm()->GetRotaion());

	return 0;
}