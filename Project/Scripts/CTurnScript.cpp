#include "CTurnScript.h"
#include "CScriptMgr.h"
#include "CMeshRender.h"
#include "CGameMgr.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CTransForm.h"

CTurnScript::CTurnScript()
	: m_iUICount(0)
	, m_fIconSize(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CTURNSCRIPT);
}

CTurnScript::~CTurnScript()
{
}

void CTurnScript::Start()
{
}

int CTurnScript::Update()
{
	bool bTurn = CGameMgr::GetInst()->GetTurn();
	bool bPlay = CGameMgr::GetInst()->GetPlayTurn();
	bool bCoolTime = CGameMgr::GetInst()->GetCoolTime();
	
	if (bTurn || bPlay || bCoolTime)  // 이거는 고쳐야 한다.  이제 턴인상태가 되면 당연히 보여야 겟지만 쿨타임이 다 끝나면 사라져야한다. View bool변수를 매니져에 하나 추가 해놓자
		MeshRender()->SetView(true);
	else
		MeshRender()->SetView(false);

	list <TURN>& listTurn = CGameMgr::GetInst()->GetListTurn();
	if (listTurn.empty() && m_iUICount == 0)
		return 0;
	    
	if (bTurn && CGameMgr::GetInst()->GetCount() > m_iUICount)
	{
		CGameObj* pUI = nullptr;
		if (listTurn.back().eType == ANIM_TYPE::MOVE)
		{
			pUI = CResMgr::GetInst()->Load<CPrefab>(L"MoveUI")->Instantiate();
			m_fIconSize = 100.f;
		}
		if (listTurn.back().eType == ANIM_TYPE::CRASH)
		{
			m_fIconSize = 80.f;
			pUI = CResMgr::GetInst()->Load<CPrefab>(L"CrashIcon")->Instantiate();
		}
		if (listTurn.back().eType == ANIM_TYPE::BREACH)
		{
			m_fIconSize = 80.f;
			pUI = CResMgr::GetInst()->Load<CPrefab>(L"BreachIcon")->Instantiate();
		}
		//CSceneMgr::GetInst()->AddObject(L"UI",pUI);

		if (m_iUICount == 0)
			pUI->GetTransForm()->SetPos(-820, 0, 0.f);
		
		else
		{
			Vec3 vPos = m_listUI.back()->GetTransForm()->GetPos();
			vPos.x += m_listUI.back()->GetTransForm()->GetScale().x + 1;
			pUI->GetTransForm()->SetPos(vPos);
		}

		GetOwner()->AddChild(pUI);
		m_listUI.push_back(pUI);
		OneSortUI();

	}
	else if (!bTurn && CGameMgr::GetInst()->GetCount() < m_iUICount)
	{
		int iCount = m_iUICount - CGameMgr::GetInst()->GetCount();

		for (UINT i = 0; i < iCount; ++i)
		{
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_listUI.front(),0 };
			CEventMgr::GetInst()->AddEvent(event); // 마지막꺼 지우면 됨
			m_listUI.pop_front();
		}
	}

	if (bTurn && listTurn.back().eType == ANIM_TYPE::MOVE)
		SortUI();
	
	m_iUICount = CGameMgr::GetInst()->GetCount();

	return 0;
}

void CTurnScript::SortUI()
{
	vector<CGameObj*>& vecChild = m_listUI.back()->GetChild();
	float fScale[3] = {};

	for (size_t i = 0; i < vecChild.size(); i++)
	{
		if (vecChild[i]->GetName() == L"Icon")
		{
			float ScaleX = m_listUI.back()->GetTransForm()->GetScale().x * 0.5f;
			Vec3 vPos = vecChild[i]->GetTransForm()->GetPos();
			vPos.x = ScaleX;
			vecChild[i]->GetTransForm()->SetPos(vPos);
		}
		else if (vecChild[i]->GetName() == L"Left")
			fScale[0] = vecChild[i]->GetTransForm()->GetScale().x;

		else if (vecChild[i]->GetName() == L"Center")
		{
			Vec3 vScale = vecChild[i]->GetTransForm()->GetScale();
			vScale.x = CGameMgr::GetInst()->GetCurPoint() * 100.f;
			vecChild[i]->GetTransForm()->SetScale(vScale);
			fScale[1] = vecChild[i]->GetTransForm()->GetScale().x;
		}
		else if (vecChild[i]->GetName() == L"Right")
		{
			float fPosX = m_listUI.back()->GetTransForm()->GetScale().x - vecChild[i]->GetTransForm()->GetScale().x;
			Vec3 vPos = vecChild[i]->GetTransForm()->GetPos();
			vPos.x = fPosX;
			vecChild[i]->GetTransForm()->SetPos(vPos);
			fScale[2] = vecChild[i]->GetTransForm()->GetScale().x;
		}
	}

	Vec3 vScale = m_listUI.back()->GetTransForm()->GetScale();
	vScale.x = fScale[0] + fScale[1] + fScale[2];
	m_listUI.back()->GetTransForm()->SetScale(vScale);
}

void CTurnScript::OneSortUI() // 나중에 Icon일때를 넣을수도 있다.
{
	vector<CGameObj*>& vecChild = m_listUI.back()->GetChild();
	float fScale[3] = {};
	CGameObj* pRight = nullptr;
	CGameObj* pIcon = nullptr;

	for (size_t i = 0; i < vecChild.size(); i++)
	{
		if (vecChild[i]->GetName() == L"Icon")
			pIcon = vecChild[i];
		
		else if (vecChild[i]->GetName() == L"Left")
			fScale[0] = vecChild[i]->GetTransForm()->GetScale().x;

		else if (vecChild[i]->GetName() == L"Center")
		{
			Vec3 vScale = vecChild[i]->GetTransForm()->GetScale();
			vScale.x = CGameMgr::GetInst()->GetCurPoint() * m_fIconSize;
			vecChild[i]->GetTransForm()->SetScale(vScale);
			fScale[1] = vecChild[i]->GetTransForm()->GetScale().x;
		}
		else if (vecChild[i]->GetName() == L"Right")
		{
			pRight = vecChild[i];
			fScale[2] = pRight->GetTransForm()->GetScale().x;
		}
	}

	Vec3 vScale = m_listUI.back()->GetTransForm()->GetScale();
	vScale.x = fScale[0] + fScale[1] + fScale[2];
	m_listUI.back()->GetTransForm()->SetScale(vScale);

	if (pIcon != nullptr)
	{
		float ScaleX = m_listUI.back()->GetTransForm()->GetScale().x * 0.5f;
		Vec3 vPos = pIcon->GetTransForm()->GetPos();
		vPos.x = ScaleX;
		pIcon->GetTransForm()->SetPos(vPos);
	}

	if (pRight != nullptr)
	{
		float fPosX = m_listUI.back()->GetTransForm()->GetScale().x - pRight->GetTransForm()->GetScale().x;
		Vec3 vPos = pRight->GetTransForm()->GetPos();
		vPos.x = fPosX;
		pRight->GetTransForm()->SetPos(vPos);
	}
}

//float fPosX = 0.f;
//Vec3 vScale;
//
//for (UINT i = 0; i < m_vecUI.size(); ++i)
//{
//
//	vScale = m_vecUI[i]->GetTransForm()->GetScale();
//	vScale.x = CGameMgr::GetInst()->GetCurPoint() * 100;
//	m_vecUI[i]->GetTransForm()->SetScale(vScale);
//	for (UINT i = 0; i < m_vecUI[i]->GetChild().size(); ++i)
//	{
//		if (m_vecUI[i]->GetName() == L"LEFT")
//		{
//			fPosX = m_vecUI[i]->GetTransForm()->GetScale().x;
//		}
//		else if (m_vecUI[i]->GetName() == L"RIGHT")
//		{
//			Vec3 vChild = m_vecUI[i]->GetTransForm()->GetScale();
//			vChild.x = vScale.x;
//			m_vecUI[i]->GetTransForm()->SetPos(vChild);
//		}
//		else if (m_vecUI[i]->GetName() == L"ICON")
//		{
//			Vec3 vChild = m_vecUI[i]->GetTransForm()->GetScale();
//			vChild.x = vScale.x / 2.f;
//			m_vecUI[i]->GetTransForm()->SetPos(vChild);
//		}
//	}
//}