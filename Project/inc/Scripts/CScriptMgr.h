#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	CBEAMSCRIPT,
	CBOSSSCRIPT,
	CBULLETSCRIPT,
	CCAMERASCRIPT,
	CCHANGESCENESCRIPT,
	CCINEMATICSCRIPT,
	CCOLLISIONSCRIPT,
	CEFFECTSCRIPT,
	CEVENTSCRIPT,
	CHPSCRIPT,
	CMHPSCRIPT,
	CMIRRORALPHA,
	CMIRRORSCRIPT,
	CPLAYERANIMSCRIPT,
	CPLAYERSCRIPT,
	CSTARTSCRIPT,
	CSUPPRESSORSCRIPT,
	CTILEMGRSCRIPT,
	CTOOLSCRIPT,
	CTURNSCRIPT,
	CUISCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
