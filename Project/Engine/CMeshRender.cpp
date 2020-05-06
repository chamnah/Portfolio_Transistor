#include "CMeshRender.h"
#include "CShader.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CTransForm.h"
#include "CMaterial.h"
#include "CAnimator2D.h"
#include "CResMgr.h"
#include "CRenderMgr.h"

CMeshRender::CMeshRender()
	:m_bView(true)
{
	m_eType = COMPONENT_TYPE::MESH_RENDER;
}

CMeshRender::CMeshRender(const CMeshRender & _Other)
{
	*this = _Other;
	m_pMtrl = _Other.m_pMtrl->Clone();
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
	TransForm()->UpdateData();

	if(Animator2D())	
		Animator2D()->UpdateData();
	else
		Animator2D()->ClearData();

	if (!m_bView)
		return;

	if (m_pMesh != nullptr && m_pMtrl != nullptr)
	{
		m_pMtrl->UpdateData();
		m_pMesh->SetShader(m_pMtrl->GetShader());
		m_pMesh->Render();
	}
}

void CMeshRender::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	bool bMesh = false;
	bool bMtrl = false;

	if (m_pMesh != nullptr)
		bMesh = true;
	
	if (m_pMtrl != nullptr)
		bMtrl = true;
	
	fwrite(&bMesh, sizeof(bool), 1, _pFile);
	fwrite(&bMtrl, sizeof(bool), 1, _pFile);

	if(bMesh)
		SaveWString(m_pMesh->GetName(),_pFile);

	if (bMtrl)
	{
		SaveWString(m_pMtrl->GetName(), _pFile);

		if(m_pMtrl->GetArrTexture() != nullptr)
			SaveWString(m_pMtrl->GetArrTexture()->GetName(),_pFile);
		else
			SaveWString(L"", _pFile);

		SaveWString(CRenderMgr::GetInst()->GetBlendName(m_pMtrl->GetBlend()),_pFile);
	}
}

void CMeshRender::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	bool bMesh = false;
	bool bMtrl = false;

	fread(&bMesh, sizeof(bool), 1, _pFile);
	fread(&bMtrl, sizeof(bool), 1, _pFile);

	if (bMesh)
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(LoadWString(_pFile));
	
	if (bMtrl)
	{
		SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(LoadWString(_pFile)));
		wstring wcsTex = LoadWString(_pFile);
		if(wcsTex != L"")
			m_pMtrl->SetParam(CResMgr::GetInst()->Load<CTexture>(wcsTex), SHADER_PARAM::TEX_0);

		wstring wcsBlend = LoadWString(_pFile);
		m_pMtrl->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsBlend));
	}
}

void CMeshRender::SetMtrlClone(CMaterial* _pMtrl)
{
	if (m_pMtrl != nullptr)
	{
		CResMgr::GetInst()->GetDeleteClone(m_pMtrl);
		m_pMtrl = nullptr;
	}
	m_pMtrl = _pMtrl->Clone();
}

void CMeshRender::SetTexture(CTexture * pTex)
{
	m_pMtrl->SetParam(pTex, SHADER_PARAM::TEX_0);

	if(pTex == nullptr)
		TransForm()->SetScale(1.f, 1.f, 1.f);
	else
		TransForm()->SetScale(pTex->GetWidth(),pTex->GetHeight(),1.f);
}