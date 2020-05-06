#include "CCollider.h"
#include "CMaterial.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CTransForm.h"
#include "CConstBuffer.h"
#include "CDirect.h"
#include "CCollisionMgr.h"
#include "CScript.h"
#include "CSceneMgr.h"

void CCollider::SetColliderType(COLLIDER_TYPE _eType)
{
	m_eCollType = _eType;

	if (m_eCollType == COLLIDER_TYPE::COLL_BOX)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollRect"); //CollRect RectMesh
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_CIRCLE)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollCircle");
	}
	else if (m_eCollType == COLLIDER_TYPE::COLL_ISO)
	{
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"IsoMetricColl"); //IsoMetric  IsoMetricColl
	}
}

Vec3 CCollider::GetWorldPos()
{
	Vec3 vPos = XMVector3TransformCoord(m_vPos,m_matWorld);
	return vPos;
}

DWORD CCollider::g_dwID = 0;

CCollider::CCollider()
	:m_dwID(g_dwID++)
	, m_fRadius(1.f)
{
	m_eType = COMPONENT_TYPE::COLLIDER;
	m_pMaterial = CResMgr::GetInst()->Load<CMaterial>(L"Coll")->Clone();

	//m_vPos = Vec3(1,1,1);
	m_vScale = Vec3(1,1,1.f);
	//m_vRot = Vec3(0, 0, XM_PIDIV4);

	m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CollRect");
}

CCollider::CCollider(const CCollider & _Coll)
{
	*this = _Coll;
	m_dwID = g_dwID++;
}

CCollider::~CCollider()
{
}

int CCollider::Update()
{
	return 0;
}

int CCollider::FinalUpdate()
{
	MatrixUpdate();

	CCollisionMgr::GetInst()->AddCollider(this,GetLayerID());
	
	return 0;
}

void CCollider::Render()
{
	g_World.mtxWorld = m_matWorld;
	g_World.mtxWVP = g_World.mtxWorld * g_World.mtxView * g_World.mtxProj;

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"TransForm");

	pBuffer->UpdateData(&g_World.mtxWVP, sizeof(Mtx));
	pBuffer->UpdateRegister();

	m_pMaterial->UpdateData();
	m_pMesh->SetShader(m_pMaterial->GetShader());
	m_pMesh->Render();
}

void CCollider::MatrixUpdate()
{
	Mtx   matTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	Mtx   matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	Mtx   matRot = XMMatrixIdentity();
	
	/*Mtx   p_matTrans = XMMatrixTranslation(TransForm()->GetPos().x, TransForm()->GetPos().y, TransForm()->GetPos().z);
	Mtx   p_matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
	Mtx   p_matRot = XMMatrixRotationZ(TransForm()->GetRotaion().z);

	Mtx ParentMtx = p_matScale * p_matRot * p_matTrans;*/

	m_matWorld = matScale * matRot * matTrans;
	
	Mtx mtxParent = TransForm()->GetRTMtx();

	m_matWorld *= mtxParent;
}

void CCollider::Save(FILE* _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_eCollType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_fRadius, sizeof(float), 1, _pFile);
}

void CCollider::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_eCollType, sizeof(UINT), 1, _pFile);
	fread(&m_vPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fRadius, sizeof(float), 1, _pFile);

	SetColliderType(m_eCollType);
}

int CCollider::CollisionEnter(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
		Script->CollisionEnter(_pColl);

	return 0;
}

int CCollider::Collision(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
	{
		Script->Collision(_pColl);
	}

	return 0;
}

int CCollider::CollisionExit(CCollider* _pColl)
{
	const vector<CScript*>& vecScript = Script();
	for (auto Script : vecScript)
	{
		Script->CollisionExit(_pColl);
	}

	return 0;
}