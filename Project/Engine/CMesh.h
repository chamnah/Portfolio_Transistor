#pragma once
#include "CRes.h"
class CShader;
class CMesh :
	public CRes
{
private:
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;
	ID3D11InputLayout* m_pLayout;

	ID3DBlob* m_pVSBlob;

	D3D11_PRIMITIVE_TOPOLOGY m_Topol;

	int m_iIdxSize;
	int m_iIdxCount;
	int m_iVtxSize;
	int m_iVtxCount;
	void* m_pVtx;

	vector<D3D11_INPUT_ELEMENT_DESC> m_vecLayout;

public:
	static CMesh* CreateMesh(void* _pVTX,int iVTXSize,int iVTXCount,
		void* _pITX, int iITXSize, int iITXCount, D3D11_PRIMITIVE_TOPOLOGY _Topol = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void SetShader(CShader* _pShader);
	void Render();
	
public:
	CMesh();
	virtual ~CMesh();
};