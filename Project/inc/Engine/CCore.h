#pragma once
#include "global.h"
class CCore
{
	SINGLETON(CCore)

private:
	ID3D11RasterizerState* m_pState;
	Vec3				   m_vViewPos;
	tResolution			   m_tResolution;
	vector<TextInfo>		   m_vecText;

	//// 폰트 띄우기용도
	ID2D1Factory			*factory = 0;
	ID2D1RenderTarget	*Rt = 0;

	IDWriteFactory *wFactory = 0;
	IDWriteTextFormat *textFormat = 0;

	//// 브러시도 필요함.
	ID2D1SolidColorBrush *brush = 0;

public:
	tResolution& GetResolution() { return m_tResolution; }

public:
	void Init(const tResolution& _res, HWND _hwnd,bool _bTool = false);
	int Update();
	void Render();

	void AddText(const TextInfo _Text) { m_vecText.push_back(_Text); }
	void ClearText() { m_vecText.clear(); }

	HRESULT	D2DInit(HWND hWnd, int width, int height, ID2D1Factory **factory, ID2D1RenderTarget **Rt);
	bool Setup();
	HRESULT DWInit(LPCWSTR fontname, float size, IDWriteFactory **factory, IDWriteTextFormat **format);
	void Clean();
};