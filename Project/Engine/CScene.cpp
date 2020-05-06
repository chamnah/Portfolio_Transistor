#include "CScene.h"
#include "CLayer.h"
#include "CStateMgr.h"
#include "CDirect.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CTransForm.h"
#include "CSceneMgr.h"

CScene::CScene()
	:m_iCurLayerCount(0)
	, m_arrLayer{}
	, pParent(nullptr)
	, m_bStop(false)
{
}

CScene::~CScene()
{
	SAFE_ARRAY_DELETE(m_arrLayer, MAX_LAYER_COUNT);
}

void CScene::Awake()
{
	for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		m_arrLayer[i]->Awake();
	}
}

void CScene::Update()
{
	if (!m_bStop) //���� ���̾ ������ ������ �غ���
	{
		for (UINT i = 0; i < m_iCurLayerCount; ++i)
			m_arrLayer[i]->Update();
	}
	else
	{
		UINT iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Player");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"UI");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Effect");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Mouse");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Skill");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Camera");
		m_arrLayer[iIdx]->Update();

		iIdx = CSceneMgr::GetInst()->GetLayerIndex(L"Monster");
		m_arrLayer[iIdx]->Update();
	}
}

void CScene::LateUpdate()
{
	m_vecCam.clear();

	for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		m_arrLayer[i]->LateUpdate();
	}
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		m_arrLayer[i]->FinalUpdate();
	}
}

void CScene::Render()
{
	for (int i = m_vecCam.size() - 1; i >= 0; --i)
	{
		m_vecCam[i]->Render(m_arrLayer);
	}

	m_vecCam[m_vecCam.size() - 1]->FinalUpdate();
	CSceneMgr::GetInst()->RenderClear();

	/*for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		if (i == 0)
		{
			CStateMgr::GetInst()->BlendRender(L"NoRender");
			CONTEXT->OMSetDepthStencilState(CStateMgr::GetInst()->FindDepthState(L"Stencil"), 1);
		}
		else
		{
			CStateMgr::GetInst()->BlendRender(L"Default");
			CONTEXT->OMSetDepthStencilState(CStateMgr::GetInst()->FindDepthState(L"StencilObj"), 1);
		}
		m_arrLayer[i]->Render();
	}*/
}

void CScene::AddLayer(wstring _wcsName)
{
	CLayer* pLayer = new CLayer;
	pLayer->SetName(_wcsName);
	pLayer->SetID(m_iCurLayerCount);
	m_arrLayer[m_iCurLayerCount++] = pLayer;
}

void CScene::AddLayer(CLayer * _pLayer)
{
	_pLayer->SetID(m_iCurLayerCount);
	m_arrLayer[m_iCurLayerCount++] = _pLayer;
}

void CScene::AddGameObject(wstring _wcsLayer, CGameObj* _pObj)
{
	for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		if (m_arrLayer[i]->GetName() == _wcsLayer)
		{
			m_arrLayer[i]->AddGameObject(_pObj);
			return;
		}
	}

	assert(false && "���� ���̾ ���ӿ�����Ʈ�� �߰��Ϸ� �޽��ϴ�.");
}

void CScene::AddGameObject(UINT _iIdx, CGameObj* _pObj)
{
	m_arrLayer[_iIdx]->AddGameObject(_pObj);
}

void CScene::RegisterCamera(CCamera * _pCam)
{
	m_vecCam.push_back(_pCam);
}

CLayer * CScene::GetLayer(int _iIdx)
{
	if (_iIdx == -1)
		return nullptr;

	if (_iIdx >= MAX_LAYER_COUNT)
		assert(NULL);

	return m_arrLayer[_iIdx];
}

CLayer * CScene::GetLayer(const wstring & _wcsName)
{
	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
	{
		if (m_arrLayer[i]->GetName() == _wcsName)
			return m_arrLayer[i];
	}

	return nullptr; 
}

CGameObj * CScene::FindObject(const wstring & _wcsName)
{
	for (UINT i = 0; i < MAX_LAYER_COUNT; ++i)
	{
		if (m_arrLayer[i] == nullptr)
			continue;

		vector<CGameObj*>& vecObj = m_arrLayer[i]->GetGameObj();

		for (auto Obj : vecObj)
		{
			if (Obj->GetName() == _wcsName)
			{
				return Obj;
			}
		}
	}
	return nullptr;
}

UINT CScene::GetLayerIndex(const wstring & _wcsName)
{
	for (UINT i = 0; i < m_iCurLayerCount; ++i)
	{
		if (m_arrLayer[i]->GetName() == _wcsName)
		{
			return i;
		}
	}

	assert(NULL);
	return 0;
}

const wstring & CScene::GetLayerName(UINT _iIdx)
{
	if (_iIdx >= MAX_LAYER_COUNT)
		assert(NULL);
	return m_arrLayer[_iIdx]->GetName();
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
}

/*
void CScene::CreateTile(UINT _iCountX, UINT _iCountY,bool _bIsoTile)
{
	SafeArrayDelete(m_vecTileObj);

	if (pParent == nullptr)
	{
		pParent = new CGameObj;
		AddGameObject(L"Default", pParent);
	}

	for (UINT i = 0; i < _iCountX; ++i)
	{
		for (UINT j = 0; j < _iCountY; ++j)
		{
			CGameObj* pGameObj = new CGameObj;
			pGameObj->GetTransForm()->SetPos(Vec3(0.f,0.f,1.f));
			CCollider* pColl = new CCollider;
			pColl->SetColliderType(COLLIDER_TYPE::COLL_ISO);
			pGameObj->AddComponent(pColl);
			pGameObj->SetParent(pParent);
			m_vecTileObj.push_back(pGameObj);
		}
	}
}
*/