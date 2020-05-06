#include "CTransForm.h"
#include "CConstBuffer.h"
#include "CDirect.h"

TranformMatrix g_World;

Vec3 g_vDir[(UINT)DIR_TYPE::END] = { Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f) };

CTransForm::CTransForm()
	:m_vLocalDir{Vec3::Right,Vec3::Up,Vec3::Front}
	, m_vWorldDir{ Vec3::Right,Vec3::Up,Vec3::Front }
	, m_vScale{1.f,1.f,1.f}
	, m_vAddPos{}
{
	m_matParent = XMMatrixIdentity();
	m_eType = COMPONENT_TYPE::TRANSFORM;
}

CTransForm::~CTransForm()
{
}

int CTransForm::Update()
{
	return 0;
}

int CTransForm::LateUpdate()
{
	return 0;
}

int CTransForm::FinalUpdate()
{
	Mtx matTrans = XMMatrixTranslation(m_vPos.x + m_vAddPos.x, m_vPos.y + m_vAddPos.y, m_vPos.z + m_vAddPos.z);
	Mtx matRotation = XMMatrixRotationX(m_vRot.x);
	matRotation *= XMMatrixRotationY(m_vRot.y);
	matRotation *= XMMatrixRotationZ(m_vRot.z);

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(g_vDir[i], matRotation);
		m_vLocalDir[i].Normalize();
		m_vWorldDir[i] = m_vLocalDir[i];
	}
	Mtx matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	if(GetParentOfOwner() == nullptr)
		m_matParent = XMMatrixIdentity();
	else
	{
		m_matParent = GetParentOfOwner()->GetTransForm()->GetWorldMtx();

		m_matParent._43 = 1.f;
		m_matParent._11 /= GetParentOfOwner()->GetTransForm()->GetScale().x;
		m_matParent._22 /= GetParentOfOwner()->GetTransForm()->GetScale().y;
		m_matParent._33 /= GetParentOfOwner()->GetTransForm()->GetScale().z;
	}
	
	m_matWorld = matScale * matRotation * matTrans * m_matParent;

	matScale = XMMatrixIdentity();

	m_matRT =  matRotation * matTrans * m_matParent;

	return 0;
}

void CTransForm::UpdateData()
{
	g_World.mtxWorld = m_matWorld;
	g_World.mtxWVP = g_World.mtxWorld * g_World.mtxView * g_World.mtxProj;

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"TransForm");

	pBuffer->UpdateData(&g_World.mtxWVP,sizeof(Mtx));
	pBuffer->UpdateRegister();
}

void CTransForm::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_vScale,sizeof(Vec3),1,_pFile);
	fwrite(&m_vPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRot, sizeof(Vec3), 1, _pFile);
}

void CTransForm::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vRot, sizeof(Vec3), 1, _pFile);
}