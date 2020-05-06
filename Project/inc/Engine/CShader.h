#pragma once
#include "CParent.h"
class CShader :
	public CParent
{
private:
	ID3DBlob* m_pVSBlob;
	ID3DBlob* m_pPSBlob;
	ID3DBlob* m_pGSBlob;
	ID3DBlob* m_pCSBlob;
	ID3DBlob* m_pErrBlob;

	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3D11GeometryShader* m_pGS;
	ID3D11ComputeShader * m_pCS;

public:
	ID3DBlob* GetVSBlob() { return m_pVSBlob; }

	void CreateVertexShader(const string& _strFunc, const string& _strVersion);
	void CreatePixelShader(const string& _strFunc, const string& _strVersion);
	void CreateGeoMetryShader(const string& _strFunc, const string& _strVersion);
	void CreateComputeShader(const string& _strFunc, const string& _strVersion);

	void UpdateData();

public:
	CShader();
	~CShader();
};