#include "CMaterial.h"
#include "CShader.h"
#include "CTexture.h"
#include "CShaderMgr.h"
#include "CResMgr.h"
#include "CDirect.h"
#include "CConstBuffer.h"
#include "CBlendState.h"

CMaterial::CMaterial()
	:m_Param{}
	, m_arrTexture{}
	, m_pBlend(nullptr)
{
	for (size_t i = 0; i < 4; i++)
	{
		m_Param.arrMtrl[i] = 1.f;
	}

	m_Param.arrFloat[0] = 1.f;
	m_Param.arrFloat[1] = 1.f;
}

CMaterial::CMaterial(const CMaterial & _Other)
{
	*this = _Other;
	
	for (size_t i = 0; i < 4; i++)
		m_Param.arrMtrl[i] = 1.f;
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	m_pShader->UpdateData();

	if (m_pBlend != nullptr)
		m_pBlend->SetShader();
	else
		m_pBlend->DefaultSetShader();

	CConstBuffer* pBuffer = CDirect::GetInst()->FindConstBuffer(L"ShaderParam");
	pBuffer->UpdateData(&m_Param,sizeof(m_Param));
	pBuffer->UpdateRegisterAll();

	UINT iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;

	for (UINT i = 0; i < iCount; ++i)
	{
		CTexture::RegisterClear(i);
	}

	for (UINT i = 0; i < iCount; ++i)
	{
		if (m_arrTexture[i] != nullptr) // 널이 아닐때만 들어가므로 이미 들어 있는 녀석에 영향이 생긴다.
			m_arrTexture[i]->SetRegisterAll(i);
	}
}

void CMaterial::SetParam(void * _pData, SHADER_PARAM _eType)
{
	switch (_eType)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_Param.arrInt[(UINT)_eType - (UINT)SHADER_PARAM::INT_0] = *((int*)_pData);
		break;

	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_Param.arrFloat[(UINT)_eType - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)_pData);
		break;

	case SHADER_PARAM::MTRL_0:
	case SHADER_PARAM::MTRL_1:
	case SHADER_PARAM::MTRL_2:
	case SHADER_PARAM::MTRL_3:
		m_Param.arrMtrl[(UINT)_eType - (UINT)SHADER_PARAM::MTRL_0] = *((float*)_pData);
		break;

	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
		m_arrTexture[(UINT)_eType - (UINT)SHADER_PARAM::TEX_0] = (CTexture*)_pData;
		break;
	}
}

CMaterial * CMaterial::Clone()
{
	CMaterial* pClone = new CMaterial(*this);
	CResMgr::GetInst()->AddClone(pClone);
	return pClone;
}