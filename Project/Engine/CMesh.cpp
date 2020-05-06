#include "CMesh.h"
#include "CDirect.h"
#include "CShader.h"

CMesh * CMesh::CreateMesh(void * _pVTX, int _iVTXSize, int _iVTXCount, void * _pITX, int _iITXSize, int _iITXCount,D3D11_PRIMITIVE_TOPOLOGY _Topol)
{
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iVTXSize * _iVTXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu에서 가변적으로 gpu에 접근해서 변경할때 쓰인다.
	tBufferDesc.CPUAccessFlags = 0; // cpu가 접근 안하려면 0 , 접근해서 갱신하려면 write
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 이녀석을 어떤 녀석인지 표기 여기선 VertexBuffer라고 알림
	tBufferDesc.MiscFlags = 0; // 정점버퍼에서는 굳이 지정할 필요는 없다.
	tBufferDesc.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 구조적버퍼를 위해서만 필요 그외에는 0

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVTX;

	ID3D11Buffer* pVB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pVB);

	tBufferDesc = {};
	tBufferDesc.ByteWidth = _iITXSize * _iITXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu에서 가변적으로 gpu에 접근해서 변경할때 쓰인다.
	tBufferDesc.CPUAccessFlags = 0; // cpu가 접근 안하려면 0 , 접근해서 갱신하려면 write
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // 이녀석을 어떤 녀석인지 표기 여기선 VertexBuffer라고 알림
	tBufferDesc.MiscFlags = 0; // 정점버퍼에서는 굳이 지정할 필요는 없다.
	tBufferDesc.StructureByteStride = 0; // 구조적 버퍼에 저장된 원소 하나의 크기 구조적버퍼를 위해서만 필요 그외에는 0

	tSubData = {};
	tSubData.pSysMem = _pITX;

	ID3D11Buffer* pIB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pIB);


	// Input할 자료형 구조체에 대한 정보를 담는다.
	// directx 헤더에는 pragma pack(1) 이 적용되어서 구조체가 그 크기 그대로이다.

	D3D11_INPUT_ELEMENT_DESC tLayoutDesc[3] = {};
	tLayoutDesc[0].SemanticName = "POSITION";
	tLayoutDesc[0].SemanticIndex = 0; // 같은 시멘틱에 대한 인덱스 POSITION 0 ~ ....
	tLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tLayoutDesc[0].InputSlot = 0; //총 16개 0 ~ 15에 대한 슬롯이 준비되어 있으며 하나의 입력슬롯으로 파이프라인에 공급할수도 있고 나누어서 공급할수도 있다.
	tLayoutDesc[0].AlignedByteOffset = 0; // 이 성분이 구조체에서 바이트시작 위치가 어디인지
	tLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명
	tLayoutDesc[0].InstanceDataStepRate = 0; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명

	tLayoutDesc[1].SemanticName = "COLOR";
	tLayoutDesc[1].SemanticIndex = 0; // 같은 시멘틱에 대한 인덱스 POSITION 0 ~ ....
	tLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 각각 비트단위
	tLayoutDesc[1].InputSlot = 0; //총 16개 0 ~ 15에 대한 슬롯이 준비되어 있으며 하나의 입력슬롯으로 파이프라인에 공급할수도 있고 나누어서 공급할수도 있다.
	tLayoutDesc[1].AlignedByteOffset = 12; // 이 성분이 구조체에서 바이트시작 위치가 어디인지
	tLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명
	tLayoutDesc[1].InstanceDataStepRate = 0; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명

	tLayoutDesc[2].SemanticName = "TEXCOORD";
	tLayoutDesc[2].SemanticIndex = 0; // 같은 시멘틱에 대한 인덱스 POSITION 0 ~ ....
	tLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT; // 각각 비트단위
	tLayoutDesc[2].InputSlot = 0; //총 16개 0 ~ 15에 대한 슬롯이 준비되어 있으며 하나의 입력슬롯으로 파이프라인에 공급할수도 있고 나누어서 공급할수도 있다.
	tLayoutDesc[2].AlignedByteOffset = 28; // 이 성분이 구조체에서 바이트시작 위치가 어디인지
	tLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명
	tLayoutDesc[2].InstanceDataStepRate = 0; // 인스턴싱에 쓰이는 기법 일단 이렇게 설정만 해두고 다음에 설명

	CMesh* pMesh = new CMesh;
	pMesh->m_pVB = pVB;
	pMesh->m_pIB = pIB;
	pMesh->m_iIdxSize = _iITXSize;
	pMesh->m_iIdxCount = _iITXCount;
	pMesh->m_iVtxSize = _iVTXSize;
	pMesh->m_iVtxCount = _iVTXCount;
	pMesh->m_Topol = _Topol;

	for(int i = 0; i < 3; ++i)
		pMesh->m_vecLayout.push_back(tLayoutDesc[i]);

	return pMesh;
}

void CMesh::SetShader(CShader* _pShader)
{
	if (m_pVSBlob == _pShader->GetVSBlob())
		return;

	m_pVSBlob = _pShader->GetVSBlob();
	SAFE_RELEASE(m_pLayout);
	DEVICE->CreateInputLayout(&m_vecLayout[0], m_vecLayout.size(), m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pLayout);
}

void CMesh::Render()
{
	CONTEXT->IASetInputLayout(m_pLayout);

	UINT iOffset = 0; // 몇번째 정점부터 시작할지
	UINT iStride = sizeof(VTX); // 각 정점에 크기, 다음 정점으로 간격

	CONTEXT->IASetVertexBuffers(0, // 정점 버퍼들을 붙이기 시작할 입력슬롯의 색인 Layout설정에서 설정한 입력슬롯을 말하는거 같다.
		1, // 입력 슬롯에다가 붙일 버퍼 갯수 
		&m_pVB, &iStride, &iOffset);

	CONTEXT->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->IASetPrimitiveTopology(m_Topol);

	CONTEXT->DrawIndexed(m_iIdxCount * (m_iIdxSize / 4), // 내가 그릴 인덱스 갯수
		0, // 그리기 시작할 첫 인덱스
		0); // 그리기 시작할 첫 정점
}

CMesh::CMesh()
	:m_pVSBlob(nullptr)
	, m_iIdxSize(0)
	, m_iIdxCount(0)
	, m_Topol(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}


CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pLayout);

	m_vecLayout.clear();
}