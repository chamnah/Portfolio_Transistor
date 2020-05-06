#include "CConstBuffer.h"
#include "CDirect.h"


CConstBuffer* CConstBuffer::Create(UINT _iSize,UINT _iRegister)
{
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = _iSize;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // cpu���� ���������� gpu�� �����ؼ� �����Ҷ� ���δ�.
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu�� ���� ���Ϸ��� 0 , �����ؼ� �����Ϸ��� write
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // �̳༮�� � �༮���� ǥ�� ���⼱ VertexBuffer��� �˸�
	tBufferDesc.MiscFlags = 0; // �������ۿ����� ���� ������ �ʿ�� ����.
	tBufferDesc.StructureByteStride = 0; // ������ ���ۿ� ����� ���� �ϳ��� ũ�� ���������۸� ���ؼ��� �ʿ� �׿ܿ��� 0

	ID3D11Buffer* pCB = nullptr;

	DEVICE->CreateBuffer(&tBufferDesc, NULL, &pCB);

	CConstBuffer* pBuffer = new CConstBuffer;
	pBuffer->m_pCB = pCB;
	pBuffer->m_iRegister = _iRegister;

	return pBuffer;
}

void CConstBuffer::UpdateData(void* _pData,UINT _iSize)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pData, _iSize);
	CONTEXT->Unmap(m_pCB, 0);
}

void CConstBuffer::ClearData()
{
	ID3D11Buffer* pBuffer = nullptr;

	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &pBuffer);
}

void CConstBuffer::UpdateRegister()
{
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

void CConstBuffer::UpdateRegisterAll()
{
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

CConstBuffer::CConstBuffer()
{
}

CConstBuffer::~CConstBuffer()
{
	SAFE_RELEASE(m_pCB);
}