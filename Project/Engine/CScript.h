#pragma once
#include "CComponent.h"

class CPrefab;
class CCollider;
class CScript :
	public CComponent
{
private:
	UINT m_iScriptType;

public:
	void Instantiate(CPrefab* _pPrefab,const wstring& _wcsKey,const Vec3& _vPos);
	virtual void Save(FILE* _pFile) {}
	virtual void Load(FILE* _pFile) {}

	virtual void CollisionEnter(CCollider* _pColl) {}
	virtual void Collision(CCollider* _pColl) {}
	virtual void CollisionExit(CCollider* _pColl) {}

	UINT GetScriptType() { return m_iScriptType; }
	void SetScriptType(UINT iType);

protected:
	wstring AnimAngleToNum(int _iDir, float _fAngle,int* _pInt = nullptr);
	wstring AnimAngleNoRot(int _iDir, float _fAngle, int* _pInt = nullptr);
	float   GetAngleDegree(Vec3& _vDest, Vec3& _vSrc); 
	float   GetAngleRadian(Vec3& _vDest, Vec3& _vSrc);
	Vec3    GetDirVec3(Vec3 _vStart, Vec3 _vEnd);
	float    GetDistanceVec3(Vec3 _vStart, Vec3 _vEnd);

public:
	CScript();
	~CScript();
};