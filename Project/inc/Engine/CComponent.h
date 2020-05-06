#pragma once
#include "CParent.h"
#include "CGameObj.h"

class CComponent :
	public CParent
{
private:
	CGameObj* m_pParentObj;

protected:
	COMPONENT_TYPE m_eType;

public:
	void SetParentObj(CGameObj* pObj) { m_pParentObj = pObj; }
	virtual int Update() { return 0; }
	virtual int LateUpdate() { return 0; }
	virtual int FinalUpdate() { return 0; }
	virtual void Render() {}
	virtual void Awake() { }
	virtual void Start() { }
	virtual void Save(FILE* _pFile) { fwrite(&m_eType, sizeof(UINT), 1, _pFile); }
	virtual void Load(FILE* _pFile) { fread(&m_eType, sizeof(UINT), 1, _pFile); }

	virtual COMPONENT_TYPE GetType();

public:
	virtual CComponent* Clone() = 0;

public:
	CTransForm* TransForm() { return m_pParentObj->GetTransForm(); }
	CMeshRender* MeshRender() { return m_pParentObj->GetMeshRender(); }
	CAnimator2D* Animator2D() { return m_pParentObj->GetAnimator2D(); }
	CCollider* Collider() { return m_pParentObj->GetCollider(); }
	CTile* Tile() { return m_pParentObj->GetTile(); }
	CGrid* Grid() { return m_pParentObj->GetGrid(); }
	CIncident* Incident() { return m_pParentObj->GetIncident(); }
	const vector<CScript*>& Script() { return m_pParentObj->GetScript(); }
	int GetLayerID() { return m_pParentObj->GetID(); }
	CGameObj* GetOwner() { return m_pParentObj; }
	CGameObj* GetParentOfOwner() { return m_pParentObj->GetParent(); }

public:
	CComponent();
	CComponent(const CComponent& _Com);
	virtual ~CComponent();

	friend class CGameObj;
};