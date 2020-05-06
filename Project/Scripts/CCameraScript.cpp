#include "CCameraScript.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "TimeMgr.h"
#include "CScriptMgr.h"
#include "CSceneMgr.h"
#include "CTransForm.h"
#include "CGameMgr.h"

CCameraScript::CCameraScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CCAMERASCRIPT);
}

CCameraScript::~CCameraScript()
{
}

int CCameraScript::Update()
{
	Vec3 vPos = TransForm()->GetPos();

	if (KEYTAB(KEY_TYPE::KEY_CTRL) || KEYHOLD(KEY_TYPE::KEY_CTRL))
		return 0;

	Vec3 vUp = TransForm()->GetLocalDir(DIR_TYPE::UP);
	Vec3 vRight = TransForm()->GetLocalDir(DIR_TYPE::RIGHT);

	if (KEYHOLD(KEY_TYPE::KEY_UP))
		vPos += vUp * 200 * DT;
	else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		vPos -= vUp * 200 * DT;

	if (KEYHOLD(KEY_TYPE::KEY_LEFT))
		vPos -= vRight * 200 * DT;

	else if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
		vPos += vRight * 200 * DT;

	TransForm()->SetPos(vPos);

	if (CGameMgr::GetInst()->GetCinematic())
		return 0;

	CGameObj* pPlayer = nullptr;
	if (!CSceneMgr::GetInst()->GetObjOfLayer(L"Player", 0, &pPlayer))
		return 0;

	Vec3 vCam = Lerf(vPos, pPlayer->GetTransForm()->GetPos(),DT);

	TransForm()->SetPos(vCam);
	return 0;
}