#pragma once
#include "CComponent.h"
class CLayer;
class CCamera :
	public CComponent
{
private:
	Mtx    m_mtxView;
	bool   m_bPerspective;
	float  m_fFOV;
	float  m_fNear;
	float  m_fFar;
	float  m_fScale;
	UINT m_iLayerMask;

public:
	void Init();
	int Update();
	virtual int LateUpdate();
	int FinalUpdate();
	void Render(CLayer** _pLayer);

	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	void SetPerspective(bool _bPers) { m_bPerspective = _bPers; }
	float GetScale() { return m_fScale; }

	bool IsPerspective() { return m_bPerspective; }

	void CheckLayerMask(UINT _iMask);
	void LayerMaskClear() { m_iLayerMask = 0; }
	void AllCheckLayerMask() { m_iLayerMask = 0xffff; }
	UINT GetLayerMask() { return m_iLayerMask; }

	float GetFar() {return m_fFar;}
	float GetFOV() { return m_fFOV; }
	void SetFar(float _fFar) { m_fFar = _fFar; }
	void SetFOV(float _fFOV) { m_fFOV = _fFOV; }
	void SetScale(float _fScale) { m_fScale = _fScale; }

	CLONE(CCamera)

public:
	CCamera();
	~CCamera();
};