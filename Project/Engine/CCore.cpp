#include "CCore.h"
#include "CDirect.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CResMgr.h"
#include "CShaderMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CEventMgr.h"
#include "CGameMgr.h"
//CGameObj* g_pGameObj = nullptr;

CCore::CCore()
	:m_pState(nullptr)
{
}

CCore::~CCore()
{
	SAFE_RELEASE(textFormat);
	SAFE_RELEASE(wFactory);
	SAFE_RELEASE(Rt);
	SAFE_RELEASE(brush);
	SAFE_RELEASE(factory);
	m_pState->Release();
}

void CCore::Init(const tResolution& _res, HWND _hwnd, bool _bTool)
{
	//FbxManager* pFbx;
	//if (_bTool == true)
	//{
	//	RECT rt = {};
	//	GetClientRect(_hwnd, &rt);
	//	m_tResolution.x = rt.right - rt.left;
	//	m_tResolution.y = rt.bottom - rt.top; // 툴용으로 만듬
	//}
	//else

	m_tResolution = _res;
	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init(_hwnd);
	CDirect::GetInst()->Init(_res,_hwnd);
	CShaderMgr::GetInst()->Init();
	CKeyMgr::GetInst()->SetHandle(_hwnd);

	CResMgr::GetInst()->GridMesh(50,50,160,160);
	CResMgr::GetInst()->DefaultRectMesh();
	CResMgr::GetInst()->CreateDefaultMaterial();
	
	CDirect::GetInst()->CreateConstBuffer(L"TransForm",sizeof(Mtx),0);
	CDirect::GetInst()->CreateConstBuffer(L"ShaderParam",sizeof(ShaderParam),1);
	CDirect::GetInst()->CreateConstBuffer(L"AnimParam", sizeof(tAnimParam), 2);

	CSceneMgr::GetInst()->Init();

	D3D11_RASTERIZER_DESC Desc = {};

	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, &m_pState);

	if (FAILED(D2DInit(_hwnd, 1920, 1080, &factory, &Rt)))
	{
		MessageBox(_hwnd, TEXT("Direct2D 초기화 실패"), TEXT("에러"), MB_ICONERROR);
		Clean();
		return;
	}
	
	if (!Setup())
	{
		MessageBox(_hwnd, TEXT("Setup 실패"), TEXT("에러"), MB_ICONERROR);
		Clean();
		return;
	}
}

int CCore::Update()
{
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();
	CGameMgr::GetInst()->Update();
	CSceneMgr::GetInst()->Update();
	CEventMgr::GetInst()->Update();
	return 0;
}

void CCore::Render()
{
	CDirect::GetInst()->Clear();
	CONTEXT->RSSetState(m_pState);
	CSceneMgr::GetInst()->Render();
	CGameMgr::GetInst()->Render();

	Rt->BeginDraw();
	for (size_t i = 0; i < m_vecText.size(); i++)
	{
		Rt->DrawTextW(
			m_vecText[i].wcsText.c_str(),
			m_vecText[i].wcsText.length(),
			textFormat,
			D2D1::RectF(m_vecText[i].vLeftTop.x, m_vecText[i].vLeftTop.y, m_vecText[i].vRightBottom.x, m_vecText[i].vRightBottom.y),
			brush
		);
	}
	Rt->EndDraw();

	CDirect::GetInst()->Present();
}

HRESULT CCore::D2DInit(HWND hWnd, int width, int height,
	ID2D1Factory **factory, ID2D1RenderTarget **Rt)
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, factory);
	if (FAILED(hr))
		return hr;
	//(*factory)->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height)), Rt);
	//(*factory)->CreateDxgiSurfaceRenderTarget()
	IDXGISurface* pBackBuffer = nullptr;
	CDirect::GetInst()->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	FLOAT dpiX;
	FLOAT dpiY;
	(*factory)->GetDesktopDpi(&dpiX, &dpiY);

	D2D1_RENDER_TARGET_PROPERTIES props =
		D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

	hr = (*factory)->CreateDxgiSurfaceRenderTarget(
		pBackBuffer,
		&props,
		Rt
	);

	pBackBuffer->Release();

	return hr;
}

bool CCore::Setup()
{
	if (FAILED(DWInit(L"BusanBada", 60.0f, &wFactory, &textFormat)))
		return false;

	Rt->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &brush);

	return true;
}


HRESULT CCore::DWInit(LPCWSTR fontname, float size,
	IDWriteFactory **factory, IDWriteTextFormat **format)
{
	HRESULT hr;

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(*factory),
		(IUnknown**)factory
	);
	if (FAILED(hr))
		return hr;

	hr = (*factory)->CreateTextFormat(
		fontname,
		0,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size,
		L"ko",
		format
	);

	return hr;
}

void CCore::Clean()
{
	SAFE_RELEASE(textFormat);
	SAFE_RELEASE(wFactory);
	SAFE_RELEASE(Rt);
	SAFE_RELEASE(brush);
	SAFE_RELEASE(factory);
}

/*
	Rt->BeginDraw();
	Rt->Clear();
	for (size_t i = 0; i < m_vecText.size(); i++)
	{
		Rt->DrawTextW(
			m_vecText[i].wcsText.c_str(),
			m_vecText[i].wcsText.length(),
			textFormat,
			D2D1::RectF(m_vecText[i].vLeftTop.x, m_vecText[i].vLeftTop.y, m_vecText[i].vRightBottom.x, m_vecText[i].vRightBottom.y),
			brush
		);
	}
	Rt->EndDraw();

*/