#pragma once
#include "CComponent.h"

class CMaterial;
class CMesh;
class CCollider :
	public CComponent
{
private:
	static DWORD  g_dwID;

private:
	Vec3   m_vPos;
	Vec3   m_vScale;
	Vec3   m_vRot;
	Mtx	   m_matWorld;

	float  m_fRadius;

	CMaterial*  m_pMaterial;
	CMesh*      m_pMesh;

	COLLIDER_TYPE  m_eCollType;

	DWORD   m_dwID;

public:
	int Update();
	int FinalUpdate();
	void Render();
	void MatrixUpdate();
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
public:
	int CollisionEnter(CCollider* _pColl);
	int Collision(CCollider* _pColl);
	int CollisionExit(CCollider* _pColl);

	CLONE(CCollider);

	void SetColliderType(COLLIDER_TYPE _eType);
	COLLIDER_TYPE GetColliderType() { return m_eCollType; }
	Mtx& GetWorldMtx() { return m_matWorld; }
	DWORD GetID() { return m_dwID; }
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetPos(Vec3 _vPos) { m_vPos = _vPos; }
	Vec3 GetPos() { return m_vPos; }
	Vec3 GetWorldPos();
	void SetScale(Vec3 _vScale) { m_vScale = _vScale; }
	Vec3 GetScale() { return m_vScale; }
	void SetRadius(float _fRad) { m_fRadius = _fRad; }
	float GetRadius() { return m_fRadius; }
	//Vec3 GetPos() { return XMVector3TransformCoord(Vec3::Zero, m_matWorld); }

public:
	CCollider();
	CCollider(const CCollider& _Coll);
	virtual ~CCollider();
};