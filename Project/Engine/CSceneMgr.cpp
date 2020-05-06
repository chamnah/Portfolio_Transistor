#include "CSceneMgr.h"
#include "CScene.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CTransForm.h"
#include "CResMgr.h"
#include "CShaderMgr.h"
#include "CScript.h"
#include "CCamera.h"
#include "CPrefab.h"
#include "CMaterial.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CCamera.h"
#include "CAnimator2D.h"
#include "CLayer.h"
#include "CStateMgr.h"

CGameObj* pMonster = nullptr;

CSceneMgr::CSceneMgr()
	:m_bDelObj(true)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::RegisterCamera(CCamera* _pCam)
{
	if (_pCam == nullptr)
		assert(NULL);
	m_pCurScene->RegisterCamera(_pCam);
}

void CSceneMgr::AddObject(const wstring& _wcsLayer, CGameObj * _pObj)
{
	m_pCurScene->AddGameObject(_wcsLayer, _pObj);
}

void CSceneMgr::AddObject(const UINT _iIdx, CGameObj * _pObj)
{
	m_pCurScene->AddGameObject(_iIdx, _pObj);
}

CLayer * CSceneMgr::GetLayer(int _iIdx)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_iIdx);
}

CLayer* CSceneMgr::GetLayer(const wstring & _wcsName)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_wcsName);
}
UINT CSceneMgr::GetLayerIndex(const wstring & _wcsName)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayerIndex(_wcsName);
}
CGameObj * CSceneMgr::FindObject(const wstring& _wcsName)
{
	return m_pCurScene->FindObject(_wcsName);
}
void CSceneMgr::AddLayer(const wstring & _wcsLayer)
{
	m_pCurScene->AddLayer(_wcsLayer);
}
const wstring & CSceneMgr::GetLayerName(UINT _iIdx)
{
	return m_pCurScene->GetLayerName(_iIdx);
}
void CSceneMgr::ChangeScene(CScene * pScene)
{
	//여기에서 기존에 있던 녀석을 삭제 시킨다.
	if (m_bDelObj)
	{
		SAFE_DELETE(m_pCurScene);
	}
	else
	{
		m_bDelObj = true;
		vector<CGameObj*>& vecPlayer = m_pCurScene->GetLayer(L"Player")->GetGameObj();
		vector<CGameObj*>& vecCamera = m_pCurScene->GetLayer(L"Camera")->GetGameObj();
		vector<CGameObj*>& vecUI = m_pCurScene->GetLayer(L"UI")->GetGameObj();

		vector<CGameObj*>& vecNewPlayer = pScene->GetLayer(L"Player")->GetGameObj();
		vector<CGameObj*>& vecNewCamera = pScene->GetLayer(L"Camera")->GetGameObj();
		vector<CGameObj*>& vecNewUI = pScene->GetLayer(L"UI")->GetGameObj();

		if (!vecNewPlayer.empty())
		{
			Vec3 NewPos = vecNewPlayer[0]->GetTransForm()->GetPos();
			vecPlayer[0]->GetTransForm()->SetPos(NewPos);
		}
		SafeArrayDelete(vecNewPlayer);
		SafeArrayDelete(vecNewCamera);
		SafeArrayDelete(vecNewUI);

		vecCamera[0]->GetTransForm()->SetPos(Vec3{ 0.f,0.f,0.f });

		for (size_t i = 0; i < vecPlayer.size(); i++)
			vecNewPlayer.push_back(vecPlayer[i]);
		
		for (size_t i = 0; i < vecCamera.size(); i++)
			vecNewCamera.push_back(vecCamera[i]);
		
		for (size_t i = 0; i < vecUI.size(); i++)
			vecNewUI.push_back(vecUI[i]);

		vecPlayer.clear();
		vecCamera.clear();
		vecUI.clear();

		SAFE_DELETE(m_pCurScene);
	}

	m_pCurScene = pScene;
}

UINT CSceneMgr::GetCurLayerCount()
{
	return m_pCurScene->GetCurLayerCount();
}

void CSceneMgr::FrontSorting(CGameObj * _pObj)
{
	if (_pObj->GetParent() != nullptr)
		return;

	CLayer* pLayer = m_pCurScene->GetLayer(_pObj->GetID());

	if (pLayer == nullptr)
		return;

	vector<CGameObj*>& vecObj = pLayer->GetGameObj();
	vector<CGameObj*>::iterator iter = vecObj.begin();

	bool bErase = false;

	for (; iter != vecObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			vecObj.erase(iter);
			bErase = true;
			break;
		}
	}

	if (!bErase)
		return;
	
	vecObj.push_back(_pObj);

	for (UINT i = vecObj.size() - 1; i > 0 ; --i)
		vecObj[i] = vecObj[i - 1];

	vecObj[0] = _pObj;
}

void CSceneMgr::SetStop(bool _bStop)
{
	m_pCurScene->SetStop(_bStop);
}

bool CSceneMgr::GetStop()
{
	return m_pCurScene->GetStop();
}

bool CSceneMgr::GetObjOfLayer(wstring _wcsLayer, int _iIdx, CGameObj** _pObj)
{
	CLayer* pLayer = m_pCurScene->GetLayer(_wcsLayer);

	if (pLayer == nullptr)
		return false;

	if (pLayer->GetGameObj().empty())
		return false;

	*_pObj = pLayer->GetGameObj()[_iIdx];

	if (*_pObj == nullptr)
		return false;

	return true;
}

void CSceneMgr::RenderSort(UINT _iMask)
{
	while (!m_queueRender.empty())
	{
		CGameObj* pTop = m_queueRender.top();
		if (_iMask & (1 << pTop->GetID()))
		{
			pTop->Render();
			m_queueRender.pop();
		}
		else
		{
			m_vecRender.push_back(m_queueRender.top());
			m_queueRender.pop();
		}
	}

	for (size_t i = 0; i < m_vecRender.size(); i++)
	{
		if (_iMask & (1 << m_vecRender[i]->GetID()))
		{
			if (m_vecRender[i]->GetLayerName() == L"Mirror")
				CStateMgr::GetInst()->DepthRender(L"Stencil");
			else if (m_vecRender[i]->GetLayerName() == L"MirrorObj")
				CStateMgr::GetInst()->DepthRender(L"StencilObj");
			else
				CStateMgr::GetInst()->DepthRender(L"Default");

			m_vecRender[i]->Render();
		}
	}

	/*if (pTop->GetLayerName() == L"Mirror")
		CStateMgr::GetInst()->DepthRender(L"Stencil");
	else if (pTop->GetLayerName() == L"MirrorObj")
		CStateMgr::GetInst()->DepthRender(L"StencilObj");
	else
		CStateMgr::GetInst()->DepthRender(L"Default");*/
}

void CSceneMgr::RenderClear()
{
	/*for (size_t i = 0; i < m_queueRender.size(); i++)
		m_queueRender.pop();*/
	
	m_queueRender = priority_queue<CGameObj*, vector<CGameObj*>, cmp>();

	m_vecRender.clear();
}

void CSceneMgr::Init()
{
	m_pCurScene = new CScene;
	
	AddLayer(L"Tile");
	AddLayer(L"Back");
	AddLayer(L"Default");
	//AddLayer(L"Event");
	AddLayer(L"Player");
	AddLayer(L"Monster");
	AddLayer(L"Skill");
	AddLayer(L"Effect");
	AddLayer(L"Mirror");
	AddLayer(L"MirrorObj");
	AddLayer(L"UI");
	AddLayer(L"Camera");
	AddLayer(L"Mouse");

	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Tile");
	//CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Default");
	CCollisionMgr::GetInst()->CollisionCheck(L"Mouse", L"Default");
	CCollisionMgr::GetInst()->CollisionCheck(L"Mouse", L"Player");
	CCollisionMgr::GetInst()->CollisionCheck(L"Mouse", L"UI");
	CCollisionMgr::GetInst()->CollisionCheck(L"Mouse", L"Back");
	CCollisionMgr::GetInst()->CollisionCheck(L"Skill", L"Monster");
	CCollisionMgr::GetInst()->CollisionCheck(L"Skill", L"Player");

	CResMgr::GetInst()->SoundInit();

	//CGameObj* pObj = new CGameObj;
	//pObj->SetName(L"Slam");
	//pObj->AddComponent(new CAnimator2D);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CCollider);
	//
	//pObj->GetAnimator2D()->AddAnimation(L"..\\content\\Texture\\Skill\\Slam\\SlamFire\\",L"Slam",Vec2(),0.2f);

	//pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));

	//pObj->GetCollider()->SetScale(Vec3(1.f,1.f,1.f));
	//pObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_BOX);

	//CResMgr::GetInst()->AddResource<CPrefab>(L"Slam",new CPrefab(pObj));

	//pObj = new CGameObj;
	//pObj->SetName(L"Crash");
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CCollider);

	//pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
	//
	//CResMgr::GetInst()->AddResource<CTexture>(L"Crash",CTexture::CreateTexture(L"..\\content\\Texture\\Skill\\Slam\\particle_green_bullet_01.png"));
	//pObj->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(L"Crash"));

	//pObj->GetCollider()->SetScale(Vec3(1.f, 1.f, 1.f));
	//pObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_BOX);

	//CResMgr::GetInst()->AddResource<CPrefab>(L"Crash", new CPrefab(pObj));


	//pObj = new CGameObj;
	//pObj->SetName(L"Suppesor");
	//
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CCollider);
	//pObj->AddComponent(new CAnimator2D);

	//pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));

	//CResMgr::GetInst()->AddResource<CPrefab>(L"Suppesor",new CPrefab(pObj));

	m_pCurScene->Awake();
}

void CSceneMgr::Update()
{
	m_pCurScene->Update();
	m_pCurScene->LateUpdate();
	m_pCurScene->FinalUpdate();
	CCollisionMgr::GetInst()->Update();
}

void CSceneMgr::Render()
{
	m_pCurScene->Render();
}

// 스텐실 만들기 object
	//CGameObj* pGameObj = new CGameObj;

	//pGameObj->AddComponent(new CMeshRender, COMPONENT_TYPE::MESH_RENDER);
	////pGameObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"Grid"));
	//pGameObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//pGameObj->GetMeshRender()->SetShader(CShaderMgr::GetInst()->FindShader(L"Color"));
	//pGameObj->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(L"Mirror"));

	//pGameObj->SetName(L"Mirror");

	//pGameObj->GetTransForm()->SetPos(0.f, 0.f, 200.f);
	//pGameObj->GetTransForm()->SetScale(5.f, 5.f, 5.f);

	//m_pCurScene->AddGameObject(L"Default", pGameObj);

bool cmp::operator()(CGameObj * t, CGameObj * u)
{
	return t->GetTransForm()->GetPos().z < u->GetTransForm()->GetPos().z;
}