#include "CTexture.h"
#include "CDirect.h"
#include "CResMgr.h"

CTexture * CTexture::CreateTexture(const wstring& _wcsPath)
{
	ID3D11ShaderResourceView* pView;
	CTexture* pTex = new CTexture;

	LoadFromWICFile(_wcsPath.c_str(),WIC_FLAGS::WIC_FLAGS_NONE,NULL, pTex->m_image);
	CreateShaderResourceView(DEVICE, pTex->m_image.GetImages(), pTex->m_image.GetImageCount(), pTex->m_image.GetMetadata(),&pView);
	
	pTex->m_pResView = pView;
	pTex->SetPath(_wcsPath);

	return pTex;
}

CTexture * CTexture::CreateTexture(ID3D11ShaderResourceView* _pRV)
{
	CTexture* pTex = new CTexture;
	pTex->m_pResView = _pRV;

	return pTex;
}

void CTexture::SetRegisterAll(int _iIndex)
{
	CONTEXT->VSSetShaderResources(_iIndex, 1, &m_pResView); // 1번째 slot이 레지스터 번호이다. 텍스쳐 레지스터 t0 ~ ...
	CONTEXT->HSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->DSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->GSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->CSSetShaderResources(_iIndex, 1, &m_pResView);
	CONTEXT->PSSetShaderResources(_iIndex, 1, &m_pResView);

	//CONTEXT->CSSetUnorderedAccessViews(0,1,,);
}

void CTexture::RegisterClear(int _iIndex)
{
	ID3D11ShaderResourceView* pView = nullptr;
	CONTEXT->VSSetShaderResources(_iIndex, 1, &pView); // 1번째 slot이 레지스터 번호이다. 텍스쳐 레지스터 t0 ~ ...
	CONTEXT->HSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->DSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->GSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->CSSetShaderResources(_iIndex, 1, &pView);
	CONTEXT->PSSetShaderResources(_iIndex, 1, &pView);
}

void CTexture::Save(FILE* _pFile)
{
	// 키값
	SaveWString(GetName(), _pFile);

	// 상대경로
	SaveWString(m_wcsPath, _pFile);
}

void CTexture::Load(FILE * _pFile)
{
	wstring strKey, strPath;
	strKey = LoadWString(_pFile);

	// 상대경로
	strPath = LoadWString(_pFile);

	// 해당 키로 등록된게 없으면 로딩
	if (nullptr == CResMgr::GetInst()->Load<CTexture>(strKey))
		CResMgr::GetInst()->AddResource<CTexture>(strKey, CreateTexture(strPath));
}
CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pResView);
	m_image.Release();
}