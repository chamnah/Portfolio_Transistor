#include "CRenderMgr.h"
#include "CBlendState.h"

CRenderMgr::CRenderMgr()
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		CreateBlend((BLEND_TYPE)i);
	}
}

CRenderMgr::~CRenderMgr()
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		delete m_arrBlend[i];
		m_arrBlend[i] = nullptr;
	}
}

void CRenderMgr::CreateBlend(BLEND_TYPE _eType)
{
	D3D11_BLEND_DESC  desc;
	wstring wcsName = L"";

	if (_eType == BLEND_TYPE::ALPHA)
	{
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		wcsName = L"Alpha";
	}
	else if (_eType == BLEND_TYPE::NONE)
	{
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		wcsName = L"NONE";
	}
	else if (_eType == BLEND_TYPE::LIGHTING) // 이름 바꿔야 된다. 단순한 가산 Blending이 아니라 Lighting Blend이다.
	{
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		wcsName = L"Lighting";
	}

	else if (_eType == BLEND_TYPE::ADD) // 이름 바꿔야 된다. 단순한 가산 Blending이 아니라 Lighting Blend이다.
	{
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;

		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		wcsName = L"ADD";
	}

	m_arrBlend[(UINT)_eType] = CBlendState::CreateBlend(desc);
	m_arrBlend[(UINT)_eType]->SetName(wcsName);
}

CBlendState * CRenderMgr::GetBlend(wstring & _wcsName)
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		if (m_arrBlend[i]->GetName() == _wcsName)
			return m_arrBlend[i];
	}

	return nullptr;
}

wstring CRenderMgr::GetBlendName(CBlendState * _pState)
{
	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		if (m_arrBlend[i] == _pState)
			return m_arrBlend[i]->GetName();
	}

	return L"NONE";
}