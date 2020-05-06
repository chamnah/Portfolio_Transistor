#include "CMesh.h"
#include "CDirect.h"
#include "CShader.h"

CMesh * CMesh::CreateMesh(void * _pVTX, int _iVTXSize, int _iVTXCount, void * _pITX, int _iITXSize, int _iITXCount,D3D11_PRIMITIVE_TOPOLOGY _Topol)
{
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iVTXSize * _iVTXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = 0; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVTX;

	ID3D11Buffer* pVB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pVB);

	tBufferDesc = {};
	tBufferDesc.ByteWidth = _iITXSize * _iITXCount;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = 0; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	tSubData = {};
	tSubData.pSysMem = _pITX;

	ID3D11Buffer* pIB = nullptr;
	DEVICE->CreateBuffer(&tBufferDesc, &tSubData, &pIB);


	// Input�� �ڷ��� ����ü�� ���� ������ ��´�.
	// directx ������� pragma pack(1) �� ����Ǿ ����ü�� �� ũ�� �״���̴�.

	D3D11_INPUT_ELEMENT_DESC tLayoutDesc[3] = {};
	tLayoutDesc[0].SemanticName = "POSITION";
	tLayoutDesc[0].SemanticIndex = 0; // ���� �ø�ƽ�� ���� �ε��� POSITION 0 ~ ....
	tLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tLayoutDesc[0].InputSlot = 0; //�� 16�� 0 ~ 15�� ���� ������ �غ�Ǿ� ������ �ϳ��� �Է½������� ���������ο� �����Ҽ��� �ְ� ����� �����Ҽ��� �ִ�.
	tLayoutDesc[0].AlignedByteOffset = 0; // �� ������ ����ü���� ����Ʈ���� ��ġ�� �������
	tLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����
	tLayoutDesc[0].InstanceDataStepRate = 0; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����

	tLayoutDesc[1].SemanticName = "COLOR";
	tLayoutDesc[1].SemanticIndex = 0; // ���� �ø�ƽ�� ���� �ε��� POSITION 0 ~ ....
	tLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ���� ��Ʈ����
	tLayoutDesc[1].InputSlot = 0; //�� 16�� 0 ~ 15�� ���� ������ �غ�Ǿ� ������ �ϳ��� �Է½������� ���������ο� �����Ҽ��� �ְ� ����� �����Ҽ��� �ִ�.
	tLayoutDesc[1].AlignedByteOffset = 12; // �� ������ ����ü���� ����Ʈ���� ��ġ�� �������
	tLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����
	tLayoutDesc[1].InstanceDataStepRate = 0; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����

	tLayoutDesc[2].SemanticName = "TEXCOORD";
	tLayoutDesc[2].SemanticIndex = 0; // ���� �ø�ƽ�� ���� �ε��� POSITION 0 ~ ....
	tLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT; // ���� ��Ʈ����
	tLayoutDesc[2].InputSlot = 0; //�� 16�� 0 ~ 15�� ���� ������ �غ�Ǿ� ������ �ϳ��� �Է½������� ���������ο� �����Ҽ��� �ְ� ����� �����Ҽ��� �ִ�.
	tLayoutDesc[2].AlignedByteOffset = 28; // �� ������ ����ü���� ����Ʈ���� ��ġ�� �������
	tLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����
	tLayoutDesc[2].InstanceDataStepRate = 0; // �ν��Ͻ̿� ���̴� ��� �ϴ� �̷��� ������ �صΰ� ������ ����

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

	UINT iOffset = 0; // ���° �������� ��������
	UINT iStride = sizeof(VTX); // �� ������ ũ��, ���� �������� ����

	CONTEXT->IASetVertexBuffers(0, // ���� ���۵��� ���̱� ������ �Է½����� ���� Layout�������� ������ �Է½����� ���ϴ°� ����.
		1, // �Է� ���Կ��ٰ� ���� ���� ���� 
		&m_pVB, &iStride, &iOffset);

	CONTEXT->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);

	CONTEXT->IASetPrimitiveTopology(m_Topol);

	CONTEXT->DrawIndexed(m_iIdxCount * (m_iIdxSize / 4), // ���� �׸� �ε��� ����
		0, // �׸��� ������ ù �ε���
		0); // �׸��� ������ ù ����
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