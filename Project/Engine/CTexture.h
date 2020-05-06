#pragma once
#include "CRes.h"
class CTexture :
	public CRes
{
private:
	ID3D11ShaderResourceView* m_pResView;
	ScratchImage			  m_image;

public:
	static CTexture* CreateTexture(const wstring& _wcsPath);
	static CTexture* CreateTexture(ID3D11ShaderResourceView* _pRV);
	void SetRegisterAll(int _iIndex);
	static void RegisterClear(int _iIndex);

	UINT GetWidth() { return m_image.GetMetadata().width; }
	UINT GetHeight() { return m_image.GetMetadata().height; }

	void Save(FILE* _pFile);
	static void Load(FILE* _pFile);

public:
	CTexture();
	virtual ~CTexture();
};