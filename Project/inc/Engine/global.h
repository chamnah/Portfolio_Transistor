#pragma once
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "D2d1.lib")
//#pragma comment(lib, "libfbxsdk")

#include <d3d11.h>
#include <d3dcompiler.h>

#include "define.h"

#include <assert.h>
//#include <fbxsdk.h>

#include <array>
#include "SimpleMath.h"

using namespace DirectX;

typedef DirectX::SimpleMath::Vector2 Vec2;
typedef DirectX::SimpleMath::Vector3 Vec3;
typedef DirectX::SimpleMath::Vector4 Vec4;
typedef DirectX::SimpleMath::Matrix Mtx;

#include <string>
using std::wstring;
using std::string;

#include <list>
#include <vector>
#include <map>

using std::list;
using std::vector;
using std::map;

using std::make_pair;

#include "struct.h"
#include "func.h"
//#include <WICTextureLoader.h>
//#include <CommonStates.h>
//#include <DirectXHelpers.h>
//#include <Effects.h>
//#include <Keyboard.h>
//#include <SpriteFont.h>

#include <dwrite.h>//나중에 텍스트 추가할때 사용한다.
#include <d2d1.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug")
#else
#pragma comment(lib, "DirectXTex")
#endif

#include <DirectXTex.h>

using namespace DirectX;

extern TranformMatrix g_World;