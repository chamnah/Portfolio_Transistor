#include "stdafx.h"
#include "CTestMgr.h"
#include "CGameObj.h"
#include "CCamera.h"
#include "CSceneMgr.h"
#include "CCollider.h"
#include "CTransform.h"
#include "KeyMgr.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGrid.h"
#include "CMaterial.h"
#include "CToolScript.h"
#include "CTile.h"
#include "CCameraScript.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CTileMgrScript.h"
#include "CSound.h"
#include "CPathMgr.h"

void CTestMgr::Init()
{
	CResMgr::GetInst()->AddResource<CTexture>(L"Suppressor",CTexture::CreateTexture(L"..\\content\\Texture\\Suppressor.png"));

	CGameObj* pObj = new CGameObj;
	pObj->AddComponent(new CCamera);
	pObj->AddComponent(new CCameraScript);

	pObj->SetName(L"MainCamera");
	CSceneMgr::GetInst()->AddObject(L"Camera",pObj);
	pObj->GetCamera()->SetPerspective(false);

	m_pMouse = new CGameObj; // Mouse Layer에다가 충돌체만 쓰려고 만들어서 실제로 레이어에 넣지는 않앗다.
	m_pMouse->SetName(L"Mouse");
	m_pMouse->SetID(CSceneMgr::GetInst()->GetLayerIndex(L"Mouse"));
	m_pMouse->AddComponent(new CMeshRender);
	m_pMouse->AddComponent(new CCollider);
	m_pMouse->AddComponent(new CToolScript);
	
	m_pMouse->GetCollider()->SetScale(Vec3(50.f,50.f,1.f));
	m_pMouse->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_ISO);

	m_pMouse->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	m_pMouse->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));

	//CSceneMgr::GetInst()->AddObject(L"Mouse", m_pMouse);
	// 사운드 
}

void CTestMgr::Update()
{
	m_pTarget = nullptr;
	m_pMouse->Update();
}

void CTestMgr::Render()
{
	m_pMouse->Render();
}

void CTestMgr::UpdateData()
{
	if(!m_bStop)
	m_pMouse->FinalUpdate();
}

CTestMgr::CTestMgr()
	:m_pTarget(nullptr)
	, m_eCollType(COLL_TYPE::NOCOLL)
	, m_bStop(false)
{
}

CTestMgr::~CTestMgr()
{
	delete m_pMouse;
}

/*

Vec3 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	Vec3 vViewPos = g_World.mtxView.Translation();

	Vec3 vMouseRealPos = CKeyMgr::GetInst()->GetMousePos();

	if(CSceneMgr::GetInst()->GetLayer(L"Tile") == nullptr)
		return;

	vector<CGameObj*>& pTile = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();

	if (pTile.empty())
		return;

	CGameObj* pObj = pTile[0];
	if (pObj->GetGrid() == nullptr)
		return;

	vector<vector<CGameObj*>>& vecObj = pObj->GetGrid()->GetVecObj();

	if (vecObj.empty())
		return;

	Vec2 vScale = pObj->GetTransForm()->GetScale();

	vMousePos += pObj->GetGrid()->GetTransformed() - vViewPos;

	int iX = int((vMousePos.x + vScale.x / 2.f) / vScale.x);
	int iY = int((vMousePos.y + vScale.y / 2.f) / vScale.y) * 2;


	if (iX < 0)
		return;//iX = 0;
	if (iY < 0)
		return; //iY = 0;
	if (iX >= pObj->GetGrid()->GetCountX())
		return;//iX = pObj->GetGrid()->GetCountX() - 1;
	if (iY >= pObj->GetGrid()->GetCountY())
		return; //iY = pObj->GetGrid()->GetCountY() - 1;

	Vec2 vTargetPos = vecObj[iY][iX]->GetTransForm()->GetPos() + vViewPos;
	Vec2 vStart;
	Vec2 vEnd;
	float fGradient = 0.f;

	//y = ax + b
	if (vMouseRealPos.x < vTargetPos.x && vMouseRealPos.y < vTargetPos.y) // 왼쪽 아래
	{
		vStart = Vec2(vTargetPos.x - vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y - vScale.y / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y - vScale.y / 2.f; // 선위에 좌표Y
		float Y = -(fGra * vTargetPos.x) + fMoveY; // Y절편 구하기
		fGradient = fGra * vMouseRealPos.x + Y; // 최종 방정식

		if (vMouseRealPos.y < fGradient)
		{
			--iY;
			--iX;
		}
	}
	else if (vMouseRealPos.x < vTargetPos.x && vMouseRealPos.y > vTargetPos.y) // 왼쪽 위
	{
		vStart = Vec2(vTargetPos.x - vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y + vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y + vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * vMouseRealPos.x + Y;

		if (vMouseRealPos.y > fGradient)
		{
			--iX;
			++iY;
		}
	}
	else if (vMouseRealPos.x > vTargetPos.x && vMouseRealPos.y > vTargetPos.y) // 오른쪽 위
	{
		vStart = Vec2(vTargetPos.x + vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y + vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y + vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * vMouseRealPos.x + Y;

		if (vMouseRealPos.y > fGradient)
		{
			++iY;
		}
	}
	else if (vMouseRealPos.x > vTargetPos.x && vMouseRealPos.y < vTargetPos.y) //오른쪽 아래
	{
		vStart = Vec2(vTargetPos.x + vScale.x / 2.f, vTargetPos.y);
		vEnd = Vec2(vTargetPos.x, vTargetPos.y - vScale.x / 2.f);
		float fGra = Gradient(vStart, vEnd);
		float fMoveY = vTargetPos.y - vScale.y / 2.f;
		float Y = -(fGra * vTargetPos.x) + fMoveY;
		fGradient = fGra * vMouseRealPos.x + Y;

		if (vMouseRealPos.y < fGradient)
		{
			--iY;
		}
	}

	if (iX < 0)
		return;//iX = 0;
	if (iY < 0)
		return; //iY = 0;
	if (iX >= pObj->GetGrid()->GetCountX())
		return;//iX = pObj->GetGrid()->GetCountX() - 1;
	if (iY >= pObj->GetGrid()->GetCountY())
		return; //iY = pObj->GetGrid()->GetCountY() - 1;

	//m_pTarget = vecObj[iY][iX];
*/