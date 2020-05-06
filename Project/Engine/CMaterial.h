#pragma once
#include "CRes.h"
class CShader;
class CTexture;
class CBlendState;
class CMaterial :
	public CRes
{
private:
	CShader*   m_pShader;
	CTexture*  m_arrTexture[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	ShaderParam  m_Param;
	CBlendState*  m_pBlend;

public:

	void UpdateData();
	void SetShader(CShader* _pShader) { m_pShader = _pShader; }
	CShader* GetShader() { return m_pShader; }
	void SetParam(void* _pData, SHADER_PARAM _eType);
	CMaterial* Clone();
	CTexture* GetArrTexture() { return m_arrTexture[0]; }
	void SetBlend(CBlendState* _pBlend) { m_pBlend = _pBlend; }
	CBlendState* GetBlend() { return m_pBlend; }

public:
	CMaterial();
	CMaterial(const CMaterial& _Other);
	~CMaterial();
};