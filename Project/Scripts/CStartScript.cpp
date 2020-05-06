#include "CStartScript.h"
#include "CPathMgr.h"
#include "CSaveLoadMgr.h"
#include "KeyMgr.h"
#include "CScriptMgr.h"
#include "CCore.h"
#include "CGameMgr.h"
#include "CResMgr.h"
#include "CSound.h"

CStartScript::CStartScript()
	:m_pScene(nullptr)
	, m_bFirst(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::CSTARTSCRIPT);
}

CStartScript::~CStartScript()
{
}

int CStartScript::Update()
{
	if (!m_bFirst && m_pScene == nullptr)
	{
		wstring strPath = CPathMgr::GetInst()->GetPath();
		strPath += L"\\Scene\\Stage1Final.scene";
		CGameMgr::GetInst()->SetScene(CSaveLoadMgr::LoadSceneNoChange(strPath));

		strPath = CPathMgr::GetInst()->GetPath();
		strPath += L"\\Scene\\Cinematic.scene";
		m_pScene = CSaveLoadMgr::LoadSceneNoChange(strPath);

		wstring wcsStart = L"PRESS BUTTON";
		CCore::GetInst()->AddText(TextInfo{ wcsStart,Vec2{760,900},Vec2{1560,1200} });
	}
	else
	{
		m_bFirst = false;
		if (KEYTAB(KEY_TYPE::KEY_SPACE))
		{
			CResMgr::GetInst()->Load<CSound>(L"Start")->Stop();
			CSaveLoadMgr::SetChangeScene(true);
			CSaveLoadMgr::SetScene(m_pScene);
			CCore::GetInst()->ClearText();
		}
	}

	return 0;
}