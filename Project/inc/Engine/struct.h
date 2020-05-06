#pragma once

#include "define.h"

struct tResolution
{
	float x, y;

	tResolution()
		:x(0.f),y(0.f)
	{}
	tResolution(float _fX,float _fY)
		:x(_fX),y(_fY)
	{}
};

struct TranformMatrix
{
	Mtx     mtxWorld;
	Mtx     mtxView;
	Mtx     mtxProj;
	Mtx     mtxWVP;
};

// 정점 구조체
struct VTX
{
	Vec3	vPos;
	Vec4	vColor;
	Vec2    vTex;
};

struct IDX
{
	UINT first, second, third;
};

struct ShaderParam
{
	int		arrInt[4];
	float	arrFloat[4];
	float   arrMtrl[4];
};

struct tAnimParam
{
	Vec2 g_fLT;
	Vec2 g_fSize;
	int   g_iUse;
	Vec2 g_vOffset;
	int g_iPadding[1];
};

struct tEvent
{
	EVENT_TYPE eType;
	INT_PTR    pDesc;
	INT_PTR    pSrc;
};

struct TileInfo
{
	POINT ptPos;
	int   iDir;
};

struct EventObj
{
	Vec3 vPos;
	wstring wcsName;

	EventObj()
	{}
	EventObj(Vec3 _vPos, wstring _wcsName)
		:vPos(_vPos), wcsName(_wcsName)
	{}
};

struct TextInfo
{
	wstring wcsText;
	Vec2    vLeftTop;
	Vec2    vRightBottom;
};

struct TURNTILE
{
	Vec3 vPos;
	int  iIdx;
};