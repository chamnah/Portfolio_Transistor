#include "CShaderMgr.h"
#include "CShader.h"
#include "CRenderMgr.h"

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
	SafeMapDelete(m_mapShader);
	for (UINT i = 0; i < m_vecShaderClone.size(); ++i)
	{
		delete m_vecShaderClone[i];
	}
	m_vecShaderClone.clear();
}

void CShaderMgr::Init()
{
	CShader* pShader = new CShader;
	pShader->CreateVertexShader("VS_Color", "vs_5_0");
	pShader->CreatePixelShader("PS_Color", "ps_5_0");
	//pShader->CreateGeoMetryShader("GS_Color", "gs_5_0");
	//pShader->CreateComputeShader("CS_HorizenBlur","cs_5_0");

	pShader->SetName(L"Color");
	AddShader(L"Color",pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Coll", "vs_5_0");
	pShader->CreatePixelShader("PS_Coll", "ps_5_0");

	pShader->SetName(L"Coll");
	AddShader(L"Coll", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0");
	pShader->CreatePixelShader("PS_Std_2D", "ps_5_0");

	//pShader->SetBlend(CRenderMgr::GetInst()->GetBlend(BLEND_TYPE::ALPHA));
	pShader->SetName(L"Std");
	AddShader(L"Std", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0");
	pShader->CreatePixelShader("PS_Std_Spectrum", "ps_5_0");

	pShader->SetName(L"Spectrum");
	AddShader(L"Spectrum", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0");
	pShader->CreatePixelShader("PS_Std_Hit", "ps_5_0");

	pShader->SetName(L"Hit");
	AddShader(L"Hit", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0");
	pShader->CreatePixelShader("PS_Std_Green", "ps_5_0");

	pShader->SetName(L"Green");
	AddShader(L"Green", pShader);

	pShader = new CShader;
	pShader->CreateVertexShader("VS_Std_2D", "vs_5_0");
	pShader->CreatePixelShader("PS_Std_Color", "ps_5_0");

	pShader->SetName(L"TexColor");
	AddShader(L"TexColor", pShader);
}

void CShaderMgr::AddShader(wstring _wcsKey, CShader * _pShader)
{
	if (m_mapShader.find(_wcsKey) != m_mapShader.end()) // 이미 같은 셰이더가 존재한다면 오류!!!
		assert(false);
	m_mapShader.insert(make_pair(_wcsKey,_pShader));
}

CShader * CShaderMgr::FindShader(wstring _wcsKey)
{
	map<wstring,CShader*>::iterator iter = m_mapShader.find(_wcsKey);

	if (iter == m_mapShader.end())
		assert(NULL);

	return iter->second;
}