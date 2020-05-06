#include "CMHPScript.h"
#include "CGameObj.h"
#include "CUnitScript.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTransForm.h"
#include "CTexture.h"
#include "CScriptMgr.h"

CMHPScript::CMHPScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CMHPSCRIPT);
}


CMHPScript::~CMHPScript()
{
}

int CMHPScript::Update()
{
	CGameObj* pObj = GetOwner()->GetParent();

	if (pObj == nullptr)
		return 0;

	int iCurHP = 0;
	int iMaxHP = 0;

	vector<CScript*>& vecScript = pObj->GetScript();

	bool bScript = false;

	for (int i = 0; i < vecScript.size(); ++i)
	{
		CUnitScript* pUnit = dynamic_cast<CUnitScript*>(vecScript[i]);

		if (pUnit != nullptr)
		{
			iCurHP = pUnit->GetCurHP();
			iMaxHP = pUnit->GetMaxHP();
			bScript = true;
			break;
		}
	}

	if (!bScript)
		return 0;

	CTexture* pTex = MeshRender()->GetMtrl()->GetArrTexture();

	if (pTex == nullptr)
		return 0;

	Vec3& vScale = TransForm()->GetScale();

	float fPercent = (float)iCurHP / (float)iMaxHP;

	vScale.x = pTex->GetWidth() * fPercent;

	if (vScale.x <= 0.f)
		vScale.x = 0.f;
	if (fPercent <= 0.f)
		fPercent = 0.f;

	MeshRender()->GetMtrl()->SetParam(&fPercent, SHADER_PARAM::FLOAT_0);

	TransForm()->SetScale(vScale);

	return 0;
}