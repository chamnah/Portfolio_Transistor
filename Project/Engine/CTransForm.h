#pragma once
#include "CComponent.h"
class CTransForm :
	public CComponent
{
private:
	Vec3       m_vScale;
	Vec3	   m_vPos;
	Vec3       m_vRot;
	Mtx		   m_matWorld;
	Mtx        m_matParent;
	Mtx        m_matRT;
	Vec3       m_vAddPos;

	Vec3       m_vLocalDir[(UINT)DIR_TYPE::END];
	Vec3       m_vWorldDir[(UINT)DIR_TYPE::END];

public:
	int Update();
	int LateUpdate();
	int FinalUpdate();
	void UpdateData();

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

	CLONE(CTransForm)

public:
	void SetPos(const Vec3& _vPos) { m_vPos = _vPos; }
	void SetPos(float _fPosX, float _fPosY, float _fPosZ) { m_vPos.x = _fPosX; m_vPos.y = _fPosY; m_vPos.z = _fPosZ; }
	Vec3& GetPos() { return m_vPos; }
	void SetScale(const Vec3& _vScale) { m_vScale = _vScale; }
	void SetScale(float _fScaleX, float _fScaleY, float _fScaleZ) { m_vScale.x = _fScaleX; m_vScale.y = _fScaleY; m_vScale.z = _fScaleZ; }
	Vec3& GetScale() { return m_vScale; }
	void SetRotaion(const Vec3& _vRot) { m_vRot = _vRot; }
	Vec3& GetRotaion() { return m_vRot; }
	void SetParentMtx(const Mtx& _mtxParent) { m_matParent = _mtxParent; }
	const Mtx& GetWorldMtx() { return m_matWorld; }
	const Mtx& GetRTMtx() { return m_matRT; }
	void SetAddPos(Vec3 _vPos) { m_vAddPos = _vPos; }

	Vec3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	Vec3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }

public:
	CTransForm();
	virtual ~CTransForm();
};