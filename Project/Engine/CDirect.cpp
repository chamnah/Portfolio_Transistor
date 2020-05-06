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
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;// ( ����� ��忡�� VC++ ���â ����� �޽��� ��� )
#endif
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	D3D_FEATURE_LEVEL eLevel;
	HRESULT hr = D3D11CreateDevice(
		0, // ���÷��� ����� ���� ( 0 == �⺻�����)
		D3D_DRIVER_TYPE_HARDWARE, // �����ġ���� (�׷����� �ϵ����� ���)
		0, // ����Ʈ���� ������ ���� ������ 0
		iFlag, // �߰����� �÷���
		0, // �����Ǵ� ��� �������� D3D_FEATURE_LEVEL���� ���� �迭�� ������ ���߿��� ����( 0 == �����Ǵ� �ְ���ر�� ����) 
		0, // ���� �迭���� ���� �迭�� 0 �־����� ���⵵ 0
		D3D11_SDK_VERSION, //D3D11�� ����ϱ� ����
		&m_pDevice,
		&eLevel,
		&m_pContext);

	if (FAILED(hr))
		assert(NULL);

	// �� ������ � ���� ������ Ȯ���غ���

	UINT Quality = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &Quality);
	assert(Quality > 0);

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = _res.x; // �ĸ���� �ʺ�
	sd.BufferDesc.Height = _res.y; // �ĸ���� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ���÷��� ȭ�� ������
	sd.BufferDesc.RefreshRate.Denominator = 1; // ���÷��� ȭ�� ������
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �ĸ���� �ȼ� ����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // ��ĵ���θ�� ������� ����� ������� ������ ���� ���پ� ���� ���
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // ��ʸ��

	if (m_bSampling) // ��Ƽ���ø� ����
	{
		sd.SampleDesc.Count = 4; // ��Ƽ���ø��Ҷ� ������ �ȼ� ����
		sd.SampleDesc.Quality = Quality - 1; // ����
	}
	else // ��� ���ҽ�
	{
		sd.SampleDesc.Count = 1; // ��Ƽ���ø��Ҷ� ������ �ȼ� ����
		sd.SampleDesc.Quality = 0; // ����
	}
	// ��Ÿ���� ���ø� �����Ϸ��� �����ϰ� �ٽ� �����ϴ� ����� �ִ�.

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���۸� ���� ��������Ѵ�.(���� �ĸ���۸� ������)
	sd.BufferCount = 1; // �ĸ������ ����
	sd.OutputWindow = _hwnd;// �������� �� â�� �ڵ鰪
	sd.Windowed = true; // â��� true ��üȭ���� false
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;//��ȯ ȿ�� (���÷��� �����Ⱑ ���� ȿ������ ������� ����)
	sd.Flags = 0; // ���÷��̸�� �÷��� å�� �ߵ��ư��� 0���� ������ DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH�� �����ϸ� ��üȭ�� ��忡�� ���÷��̸�� �ڵ����� �̰� ����!!


	// ����ü���� ���� ���� ���� ����ü���� ����Ϸ��� ��ġ�� ���� Factory�ν��Ͻ��� �̿��ؾ��Ѵ�. �׳� �����ϸ� ������ �߻��Ѵ�.
	IDXGIDevice* dxgiDevice = 0;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	// __uuidof = Ŭ�������̳� �������̽������� GUID���� �������ش�.

	assert(dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetAdapter(&dxgiAdapter);

	assert(dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory); // Factory�� ����

	assert(dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwap);

	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// ���������ο� ������ �ĸ���� �並 ����
	ID3D11Texture2D* back;
	m_pSwap->GetBuffer(0, // �ĸ������ ���� ���° �༮���� �ϳ��϶� 0
		__uuidof(ID3D11Texture2D), (void**)&back);

	m_pDevice->CreateRenderTargetView(back, // ����������� ����� �ڿ�
		0, //�ڿ��� ��� ���ҵ��� �ڷ����� ������ �ڿ��� �ƴϸ� �ΰ� �־ �������.
		&m_pView);

	//m_pDevice->CreateShaderResourceView(back,0,&m_pRV);

	back->Release(); // Getbuffer ȣ��� ����Ƚ�� ������ �׷��Ƿ� �������Ѵ�.

	// ���� ���ٽ� ���� ����
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = _res.x;
	td.Height = _res.y;
	td.MipLevels = 1; // �Ӹʼ����� ���� ���̹��� �ؽ��Ĵ� �ϳ��� �ȴ�.
	td.ArraySize = 1; // �ؽ��� �迭�� �ؽ��İ��� ���� ���۴� �ϳ���ȴ�.
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 4.1.5
	if (m_bSampling) // ��Ƽ���ø� ����
	{
		td.SampleDesc.Count = 4; // ��Ƽ���ø��Ҷ� ������ �ȼ� ����
		td.SampleDesc.Quality = Quality - 1; // ����
	}
	else // ��� ���ҽ�
	{
		td.SampleDesc.Count = 1; // ��Ƽ���ø��Ҷ� ������ �ȼ� ����
		td.SampleDesc.Quality = 0; // ����
	}

	td.Usage = D3D11_USAGE_DEFAULT; // �ؽ����� �뵵���� (GPU�� �а� ���⸦ �Ѵ�. CPU ���� �Ұ�)
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL; // �ڿ��� ���������ο� ������� ���������� �����Ѵ�. �÷��׷ν� �ϳ� �̻� ���� ���� ����
	td.CPUAccessFlags = 0;// CPU�� �ڿ��� �����ϴ� ����� �����Ѵ�. ���� ���ϰ� �Ϸ��� 0�̴�.
	td.MiscFlags = 0; // ��Ÿ �÷��׵��̴�.

	m_pDevice->CreateTexture2D(&td,
		0, // �ؽ��Ŀ� ä�� �ʱ��ڷ��̴�. ���� ���ٽǹ��� �ؽ��Ĵ� �ʿ����.
		&m_pTexDepth); // ���� ���ٽ� �ؽ��ĸ� �����ϰ� �� ���۸� ��ȯ�޴´�.
	m_pDevice->CreateDepthStencilView(m_pTexDepth,
		0,// �ڿ��� ���� �ڷ����� ������ �ڿ��� �ƴϸ� �ΰ��� �־ �ȴ�. ���� ���� ������ �����޴�. td����
		&m_pDepthView); // �� ���۸� ���� ���������ΰ� ������ �並 ����

	// ��� ������������ ��� ���ձ� �ܰ迡 ����
	m_pContext->OMSetRenderTargets(1, // �������ϴ� ���� ����� ����
		&m_pView, m_pDepthView);

	// ���� ����� ������ ����Ʈ
	D3D11_VIEWPORT vp = {}; // Ŭ���� �ϴ°��� ����Ʈ�� ������ ���� �ʴ´�.
	vp.TopLeftX = 0.f; // ����� ���� ��ǥ
	vp.TopLeftY = 0.f;
	vp.Width = _res.x; // ����� ũ�� ������ǥ�κ��� ũ�⸸ŭ 
	vp.Height = _res.y;
	vp.MinDepth = 0.f; // �ּ� ���̹���
	vp.MaxDepth = 1.f; // �ִ� ���̹��� Direct3d�� ����ϴ� ���̹��� ������ 0 ~ 1�̴�. Ư���� ��Ȳ�� �ƴϸ� �����ִ°� ����.

	CONTEXT->RSSetViewports(1, // ����Ʈ ����
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
	DSD.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ZERO�� ���̹��ۿ� ����(����)�� ���� �ʰ� ���� ������ �Ѵ�. // ALL�� ����� ���� �� �Ѵ�. 
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

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // ���� ���� �ĸ��ø��ؼ� �ȱ׸��Բ� �ϸ� �ǹ� ���� ������ �ȴ�.
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
	m_pContext->OMSetDepthStencilState(0, 0); // �ʱ�ȭ �ϱ�
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

	DSD.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // ���� ���� �ĸ��ø��ؼ� �ȱ׸��Բ� �ϸ� �ǹ� ���� ������ �ȴ�.
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