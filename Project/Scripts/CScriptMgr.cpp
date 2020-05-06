#include "stdafx.h"
#include "CScriptMgr.h"

#include "CBeamScript.h"
#include "CBossScript.h"
#include "CBulletScript.h"
#include "CCameraScript.h"
#include "CChangeSceneScript.h"
#include "CCinematicScript.h"
#include "CCollisionScript.h"
#include "CEffectScript.h"
#include "CEventScript.h"
#include "CHPScript.h"
#include "CMHPScript.h"
#include "CMirrorAlpha.h"
#include "CMirrorScript.h"
#include "CPlayerAnimScript.h"
#include "CPlayerScript.h"
#include "CStartScript.h"
#include "CSuppressorScript.h"
#include "CTileMgrScript.h"
#include "CToolScript.h"
#include "CTurnScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBeamScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraScript");
	_vec.push_back(L"CChangeSceneScript");
	_vec.push_back(L"CCinematicScript");
	_vec.push_back(L"CCollisionScript");
	_vec.push_back(L"CEffectScript");
	_vec.push_back(L"CEventScript");
	_vec.push_back(L"CHPScript");
	_vec.push_back(L"CMHPScript");
	_vec.push_back(L"CMirrorAlpha");
	_vec.push_back(L"CMirrorScript");
	_vec.push_back(L"CPlayerAnimScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CStartScript");
	_vec.push_back(L"CSuppressorScript");
	_vec.push_back(L"CTileMgrScript");
	_vec.push_back(L"CToolScript");
	_vec.push_back(L"CTurnScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBeamScript" == _strScriptName)
		return new CBeamScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"CChangeSceneScript" == _strScriptName)
		return new CChangeSceneScript;
	if (L"CCinematicScript" == _strScriptName)
		return new CCinematicScript;
	if (L"CCollisionScript" == _strScriptName)
		return new CCollisionScript;
	if (L"CEffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"CEventScript" == _strScriptName)
		return new CEventScript;
	if (L"CHPScript" == _strScriptName)
		return new CHPScript;
	if (L"CMHPScript" == _strScriptName)
		return new CMHPScript;
	if (L"CMirrorAlpha" == _strScriptName)
		return new CMirrorAlpha;
	if (L"CMirrorScript" == _strScriptName)
		return new CMirrorScript;
	if (L"CPlayerAnimScript" == _strScriptName)
		return new CPlayerAnimScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CStartScript" == _strScriptName)
		return new CStartScript;
	if (L"CSuppressorScript" == _strScriptName)
		return new CSuppressorScript;
	if (L"CTileMgrScript" == _strScriptName)
		return new CTileMgrScript;
	if (L"CToolScript" == _strScriptName)
		return new CToolScript;
	if (L"CTurnScript" == _strScriptName)
		return new CTurnScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CBEAMSCRIPT:
		return L"CBeamScript";
		break;

	case SCRIPT_TYPE::CBOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::CBULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::CCAMERASCRIPT:
		return L"CCameraScript";
		break;

	case SCRIPT_TYPE::CCHANGESCENESCRIPT:
		return L"CChangeSceneScript";
		break;

	case SCRIPT_TYPE::CCINEMATICSCRIPT:
		return L"CCinematicScript";
		break;

	case SCRIPT_TYPE::CCOLLISIONSCRIPT:
		return L"CCollisionScript";
		break;

	case SCRIPT_TYPE::CEFFECTSCRIPT:
		return L"CEffectScript";
		break;

	case SCRIPT_TYPE::CEVENTSCRIPT:
		return L"CEventScript";
		break;

	case SCRIPT_TYPE::CHPSCRIPT:
		return L"CHPScript";
		break;

	case SCRIPT_TYPE::CMHPSCRIPT:
		return L"CMHPScript";
		break;

	case SCRIPT_TYPE::CMIRRORALPHA:
		return L"CMirrorAlpha";
		break;

	case SCRIPT_TYPE::CMIRRORSCRIPT:
		return L"CMirrorScript";
		break;

	case SCRIPT_TYPE::CPLAYERANIMSCRIPT:
		return L"CPlayerAnimScript";
		break;

	case SCRIPT_TYPE::CPLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::CSTARTSCRIPT:
		return L"CStartScript";
		break;

	case SCRIPT_TYPE::CSUPPRESSORSCRIPT:
		return L"CSuppressorScript";
		break;

	case SCRIPT_TYPE::CTILEMGRSCRIPT:
		return L"CTileMgrScript";
		break;

	case SCRIPT_TYPE::CTOOLSCRIPT:
		return L"CToolScript";
		break;

	case SCRIPT_TYPE::CTURNSCRIPT:
		return L"CTurnScript";
		break;

	case SCRIPT_TYPE::CUISCRIPT:
		return L"CUIScript";
		break;

	}
	return nullptr;
}