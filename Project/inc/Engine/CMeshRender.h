#pragma once
#include "CComponent.h"

class CMesh;
class CMaterial;
class CMeshRender :
	public CComponent
{
public:
	CMesh* m_pMesh;
	CMaterial* m_pMtrl;
	bool m_bView;

public:
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetMtrl(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }
	void SetMtrlClone(CMaterial* _pMtrl);
	CMaterial* GetMtrl() { return m_pMtrl;}
	CMesh* GetMesh() { return m_pMesh; }
	void SetTexture(CTexture* pTex);
	void SetView(bool _bView) { m_bView = _bView; }

	CLONE(CMeshRender)

public:
	void Render();

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CMeshRender();
	CMeshRender(const CMeshRender& _Other);
	virtual ~CMeshRender();
};