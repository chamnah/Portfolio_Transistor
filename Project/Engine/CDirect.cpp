#include "CDirect.h"
#include "CConstBuffer.h"
#include "CStateMgr.h"
CDirect::CDirect()
	:m_bSampling(true)
	, m_pDevice(nullptr)
	, m_pContext(nullptr)
	, m_pSwap(nullptr)
	, m_pView(nullptr)
	, m_pTexDepth(nullptr)
	, m_pDepthView(nullptr)
{
}

CDirect::~CDirect()
{
	SafeMapDelete(m_mapConstBuffer);
	SAFE_RELEASE(m_pView);
	SAFE_RELEASE(m_pTexDepth);
	SAFE_RELEASE(m_pSwap);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);

//#if defined(DEBUG) | defined(_DEBUG)
//	ID3D11Debug* pDebug = nullptr;
//	m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&pDebug));
//
//	SAFE_RELEASE(m_pDevice);
//	pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//	SAFE_RELEASE(pDebug);
//#endif
}

void CDirect::CreateConstBuffer(const wstring& _wcsKey,UINT _iSize,UINT _iRegister)
{
	CConstBuffer* pBuffer = CConstBuffer::Create(_iSize, _iRegister);
	
	m_mapConstBuffer.insert(make_pair(_wcsKey,pBuffer));
}

void CDirect::CreateBlurTexture()
{
	DXGI_SAMPLE_DESC sd;
	sd.Quality = 0;
	sd.Count = 1;

	D3D11_TEXTURE2D_DESC desc;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.CPUAccessFlags = 0;
	desc.SampleDesc = sd;

	m_pDevice->CreateTexture2D(&desc,0, &m_pBlurTex);


	m_pDevice->CreateRenderTargetView(m_pBlurTex,0,&m_pBlurView);
}

CConstBuffer * CDirect::FindConstBuffer(const wstring & _wcsKey)
{
	map<wstring,CConstBuffer*>::iterator iter = m_mapConstBuffer.find(_wcsKey);

	if (iter == m_mapConstBuffer.end())
	{
		assert(false);
	}

	return iter->second;
}

void CDirect::Init(const tResolution& _res, HWND _hwnd)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;// ( 디버그 모드에서 VC++ 출력창 디버그 메시지 출력 )
#endif
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL eLevel;
	HRESULT hr = D3D11CreateDevice(
		0, // 디스플레이 어댑터 설정 ( 0 == 기본어댑터)
		D3D_DRIVER_TYPE_HARDWARE, // 출력장치선택 (그래픽을 하드웨어로 출력)
		0, // 소프트웨어 구동기 지정 없으면 0
		iFlag, // 추가적인 플래그
		0, // 지원되는 기능 수준점검 D3D_FEATURE_LEVEL형식 원소 배열을 넣으면 그중에서 점검( 0 == 지원되는 최고수준기능 선택) 
		0, // 지원 배열원소 갯수 배열에 0 넣었으면 여기도 0
		D3D11_SDK_VERSION, //D3D11을 사용하기 위함
		&m_pDevice,
		&eLevel,
		&m_pContext);

	if (FAILED(hr))
		assert(NULL);

	// 각 변수에 어떤 값이 들어갔는지 확인해보자

	UINT Quality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Quality);
	assert(Quality > 0);

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = _res.x; // 후면버퍼 너비
	sd.BufferDesc.Height = _res.y; // 후면버퍼 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 디스플레이 화면 갱신율
	sd.BufferDesc.RefreshRate.Denominator = 1; // 디스플레이 화면 갱신율
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 후면버퍼 픽셀 형식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 스캔라인모드 어떤식으로 모니터 출력할지 위에서 부터 한줄씩 할지 등등
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // 비례모드

	if (m_bSampling) // 멀티샘플링 사용시
	{
		sd.SampleDesc.Count = 4; // 멀티샘플링할때 추출할 픽셀 갯수
		sd.SampleDesc.Quality = Quality - 1; // 수준
	}
	else // 사용 안할시
	{
		sd.SampleDesc.Count = 1; // 멀티샘플링할때 추출할 픽셀 갯수
		sd.SampleDesc.Quality = 0; // 수준
	}
	// 런타임중 샘플링 변경하려면 삭제하고 다시 생성하는 방법이 있다.

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼를 렌더 대상으로한다.(현재 후면버퍼를 렌더링)
	sd.BufferCount = 1; // 후면버퍼의 갯수
	sd.OutputWindow = _hwnd;// 렌더링을 할 창의 핸들값
	sd.Windowed = true; // 창모드 true 전체화면모드 false
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//교환 효과 (디스플레이 구동기가 가장 효율적인 방법으로 선택)
	sd.Flags = 0; // 디스플레이모드 플래그 책은 잘돌아가서 0으로 선택함 DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH를 지정하면 전체화면 모드에서 디스플레이모드 자동선택 이거 실험!!


	// 스왑체인을 위한 질의 과정 스왑체인을 사용하려면 장치에 쓰인 Factory인스턴스를 이용해야한다. 그냥 생성하면 오류가 발생한다.
	IDXGIDevice* dxgiDevice = 0;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	// __uuidof = 클래스명이나 인터페이스명에대한 GUID값을 리턴해준다.

	assert(dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	assert(dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory); // Factory를 얻음

	assert(dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwap);

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// 파이프라인에 연결할 후면버퍼 뷰를 생성
	ID3D11Texture2D* back;
	m_pSwap->GetBuffer(0, // 후면버퍼의 색인 몇번째 녀석인지 하나일땐 0
		__uuidof(ID3D11Texture2D), (void**)&back);

	m_pDevice->CreateRenderTargetView(back, // 렌더대상으로 사용할 자원
		0, //자원에 담긴 원소들의 자료형식 무형식 자원이 아니면 널값 넣어도 상관없다.
		&m_pView);

	//m_pDevice->CreateShaderResourceView(back,0,&m_pRV);

	back->Release(); // Getbuffer 호출시 참조횟수 증가됨 그러므로 해제를한다.

	// 깊이 스텐실 버퍼 설정
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = _res.x;
	td.Height = _res.y;
	td.MipLevels = 1; // 밉맵수준의 갯수 깊이버퍼 텍스쳐는 하나면 된다.
	td.ArraySize = 1; // 텍스쳐 배열의 텍스쳐갯수 깊이 버퍼는 하나면된다.
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 4.1.5
	if (m_bSampling) // 멀티샘플링 사용시
	{
		td.SampleDesc.Count = 4; // 멀티샘플링할때 추출할 픽셀 갯수
		td.SampleDesc.Quality = Quality - 1; // 수준
	}
	else // 사용 안할시
	{
		td.SampleDesc.Count = 1; // 멀티샘플링할때 추출할 픽셀 갯수
		td.SampleDesc.Quality = 0; // 수준
	}

	td.Usage = D3D11_USAGE_DEFAULT; // 텍스쳐의 용도결정 (GPU가 읽고 쓰기를 한다. CPU 접근 불가)
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL; // 자원을 파이프라인에 어떤식으로 묶을것인지 지정한다. 플래그로써 하나 이상 조합 지정 가능
	td.CPUAccessFlags = 0;// CPU가 자원에 접근하는 방식을 결정한다. 접근 못하게 하려면 0이다.
	td.MiscFlags = 0; // 기타 플래그들이다.

	m_pDevice->CreateTexture2D(&td,
		0, // 텍스쳐에 채울 초기자료이다. 깊이 스텐실버퍼 텍스쳐는 필요없다.
		&m_pTexDepth); // 깊이 스텐실 텍스쳐를 생성하고 그 버퍼를 반환받는다.
	m_pDevice->CreateDepthStencilView(m_pTexDepth,
		0,// 자원의 원소 자료형식 무형식 자원이 아니면 널값을 넣어도 된다. 지금 경우는 형식을 지정햇다. td에서
		&m_pDepthView); // 그 버퍼를 토대로 파이프라인과 연결할 뷰를 생성

	// 뷰와 파이프라인의 출력 병합기 단계에 묶기
	m_pContext->OMSetRenderTargets(1, // 묶고자하는 렌더 대상의 개수
		&m_pView, m_pDepthView);

	// 내가 출력할 영역인 뷰포트
	D3D11_VIEWPORT vp = {}; // 클리어 하는것은 뷰포트에 영향을 받지 않는다.
	vp.TopLeftX = 0.f; // 출력할 시작 좌표
	vp.TopLeftY = 0.f;
	vp.Width = _res.x; // 출력할 크기 시작좌표로부터 크기만큼 
	vp.Height = _res.y;
	vp.MinDepth = 0.f; // 최소 깊이버퍼
	vp.MaxDepth = 1.f; // 최대 깊이버퍼 Direct3d가 사용하는 깊이버퍼 범위는 0 ~ 1이다. 특수한 상황이 아니면 맞춰주는게 좋다.

	CONTEXT->RSSetViewports(1, // 뷰포트 갯수
		&vp);

	D3D11_SAMPLER_DESC desc = {};
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	
	ID3D11SamplerState* pSS;
	m_pDevice->CreateSamplerState(&desc, &pSS);
	m_pContext->PSSetSamplers(0,1,&pSS);

	pSS->Release();

	D3D11_DEPTH_STENCIL_DESC DSD = {};

	DSD.DepthEnable = true;
	DSD.DepthFunc = D3D11_COMPARISON_LESS;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ZERO는 깊이버퍼에 쓰기(갱신)를 하지 않고 깊이 판정은 한다. // ALL은 쓰기와 판정 다 한다. 
	DSD.StencilEnable = false;
	m_pDevice->CreateDepthStencilState(&DSD, &DSS);
	CStateMgr::GetInst()->AddDepthState(L"Default", DSS);

	DSD = {};

	DSD.DepthEnable = true;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DSD.DepthFunc = D3D11_COMPARISON_LESS;
	DSD.StencilEnable = true;
	DSD.StencilReadMask = 0xff;
	DSD.StencilWriteMask = 0xff;

	DSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	DSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // 내가 만약 후면컬링해서 안그리게끔 하면 의미 없는 설정이 된다.
	DSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	DSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_pDevice->CreateDepthStencilState(&DSD, &DSS);

	CStateMgr::GetInst()->AddDepthState(L"Stencil", DSS);

	DSD = {};

	DSD.DepthEnable = true;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSD.DepthFunc = D3D11_COMPARISON_LESS;
	DSD.StencilEnable = true;
	DSD.StencilReadMask = 0xff;
	DSD.StencilWriteMask = 0xff;

	DSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	m_pDevice->CreateDepthStencilState(&DSD, &DSS);

	CStateMgr::GetInst()->AddDepthState(L"StencilObj", DSS);

	//CStateMgr::GetInst()->DepthRender(L"Default");
}

void CDirect::Clear()
{
	float arrColor[4] = { 0.f,0.f,0.f,1.f};
	m_pContext->ClearRenderTargetView(m_pView,arrColor);
	m_pContext->ClearDepthStencilView(m_pDepthView,D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.f,0);
	m_pContext->OMSetDepthStencilState(0, 0); // 초기화 하기
}

/*DSD = {};

	DSD.DepthEnable = true;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DSD.DepthFunc = D3D11_COMPARISON_LESS;
	DSD.StencilEnable = true;
	DSD.StencilReadMask = 0xff;
	DSD.StencilWriteMask = 0xff;

	DSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	DSD.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // 내가 만약 후면컬링해서 안그리게끔 하면 의미 없는 설정이 된다.
	DSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	DSD.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	m_pDevice->CreateDepthStencilState(&DSD, &DSS);

	CStateMgr::GetInst()->AddDepthState(L"Stencil", DSS); 

	DSD = {};

	DSD.DepthEnable = true;
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSD.DepthFunc = D3D11_COMPARISON_LESS;
	DSD.StencilEnable = true;
	DSD.StencilReadMask = 0xff;
	DSD.StencilWriteMask = 0xff;

	DSD.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DSD.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	DSD.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	m_pDevice->CreateDepthStencilState(&DSD, &DSS);

	CStateMgr::GetInst()->AddDepthState(L"StencilObj", DSS);*/

	//m_pContext->OMSetDepthStencilState(DSS, 0);