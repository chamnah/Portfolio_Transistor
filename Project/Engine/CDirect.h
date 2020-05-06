#pragma once
#include "global.h"

class CConstBuffer;
class CDirect
{
	SINGLETON(CDirect)

private:
	bool m_bSampling;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;
	IDXGISwapChain*	m_pSwap;
	ID3D11RenderTargetView* m_pView;
	ID3D11ShaderResourceView* m_pRV;
	ID3D11Texture2D* m_pTexDepth;
	ID3D11DepthStencilView* m_pDepthView;
	ID3D11DepthStencilState*  DSS;

	map<wstring, CConstBuffer*> m_mapConstBuffer;

	ID3D11Texture2D* m_pBlurTex;
	ID3D11RenderTargetView* m_pBlurView;
	ID3D11UnorderedAccessView* m_pBlurAccessView;	

public:

	ID3D11DepthStencilView* GetDepthView() { return m_pDepthView; }

	void CreateConstBuffer(const wstring& _wcsKey, UINT _iSize, UINT _iRegister);
	void CreateBlurTexture();
	CConstBuffer* FindConstBuffer(const wstring& _wcsKey);
	void Init(const tResolution& _res,HWND _hwnd);
	void Clear();
	void Present() { m_pSwap->Present(0,0); }
	IDXGISwapChain* GetSwapChain() { return m_pSwap; }
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }
};