#pragma once
#include "global.h"

class CBlendState;
class CRenderMgr
{
	SINGLETON(CRenderMgr);

private:
	CBlendState*  m_arrBlend[(UINT)BLEND_TYPE::END];

public:
	void CreateBlend(BLEND_TYPE _eType);

	CBlendState* GetBlend(BLEND_TYPE _eType) { return m_arrBlend[(UINT)_eType]; }
	CBlendState* GetBlend(wstring& _wcsName);
	wstring GetBlendName(CBlendState* _pState);

};