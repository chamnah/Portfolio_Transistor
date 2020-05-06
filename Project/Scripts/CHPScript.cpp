#include "CHPScript.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CUnitScript.h"
#include "CGameObj.h"
#include "CTransForm.h"
#include "CMeshRender.h"
#include "CMaterial.h"
#include "CTexture.h"
#include "CScriptMgr.h"

CHPScript::CHPScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::CHPSCRIPT);
}

CHPScript::~CHPScript()
{
}

int CHPScript::Update()
{
	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(L"Player");

	if (pLayer == nullptr)
		return 0;

	vector<CGameObj*>& vecObj = pLayer->GetGameObj();

	if (pLayer->GetGameObj().empty())
		return 0;
	int iCurHP = 0;
	int iMaxHP = 0;

	vector<CScript*>& vecScript = vecObj[0]->GetScript();

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