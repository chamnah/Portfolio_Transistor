#include "CEventMgr.h"
#include "CGameObj.h"
#include "CSceneMgr.h"

void CEventMgr::Init()
{
}

int CEventMgr::Update()
{

	int i = 0;
	for (UINT i = 0; i < m_vecDel.size(); ++i)
	{
		m_vecDel[i]->LayerEscape();
		SAFE_DELETE(m_vecDel[i]);
		m_bReset = true;
	}
	m_vecDel.clear();

	for (UINT i = 0; i < m_vecEvent.size(); ++i)
	{
		Event(m_vecEvent[i]);
	}

	m_vecEvent.clear();

	if (m_bChangeScene)
	{
		m_bChangeScene = false;
		m_vecDel.clear();
	}

	return 0;
}

void CEventMgr::Event(tEvent& _event)
{
	if (EVENT_TYPE::DELETE_OBJ == _event.eType)
	{
		((CGameObj*)_event.pDesc)->IsDead();

		bool bFind = false;

		for (UINT i = 0; i < m_vecDel.size(); ++i)
		{
			if (m_vecDel[i] == (CGameObj*)_event.pDesc)
				bFind = true;
		}
		if(!bFind)
			m_vecDel.push_back((CGameObj*)_event.pDesc);
	}
	else if (EVENT_TYPE::ADD_CHILD == _event.eType)
	{
		((CGameObj*)_event.pDesc)->AddChild(((CGameObj*)_event.pSrc));
	}
	else if (EVENT_TYPE::DISCONNECT_PARENT == _event.eType)
	{
		((CGameObj*)_event.pDesc)->DisconnectParent();
	}
	else if (EVENT_TYPE::CHANGE_LAYER == _event.eType)
	{
		((CGameObj*)_event.pDesc)->ChangeLayer(_event.pSrc);
	}
	else if (EVENT_TYPE::CREATE_OBJ == _event.eType)
	{
		CSceneMgr::GetInst()->AddObject(_event.pDesc,(CGameObj*)_event.pSrc);
		m_bAddObj = true;
		m_pNewObj = (CGameObj*)_event.pSrc;
	}
	else if (EVENT_TYPE::CHANGE_SCENE == _event.eType)
		m_bChangeScene = true;
}

CEventMgr::CEventMgr()
	:m_bReset(false)
	, m_bChangeScene(false)
	, m_bAddObj(false)
	, m_pNewObj(nullptr)
{
}

CEventMgr::~CEventMgr()
{
}