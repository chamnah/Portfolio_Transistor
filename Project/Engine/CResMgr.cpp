#include "CResMgr.h"
#include "CMesh.h"
#include "CPtr.h"
#include "CShaderMgr.h"
#include "CResMgr.h"
#include "CComponent.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CPathMgr.h"

CResMgr::CResMgr()
{
}


CResMgr::~CResMgr()
{
	SAFE_ARRAY_MAP_DELETE(m_mapRes, (UINT)RESOURCE_TYPE::END);
	SafeArrayDelete(m_vecClone);
}

void CResMgr::DefaultRectMesh()
{
	VTX arrVTX[4] = {};

	arrVTX[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f,0.f);

	arrVTX[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f,1.f);

	IDX arrIndex[2] =
	{
		0,1,2,
		0,2,3
	};

	CMesh* Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"RectMesh",Mesh);

	UINT iIndex[5] ={	0,1,2,3,0};
	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, iIndex, sizeof(UINT), 5,D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddResource<CMesh>(L"CollRect", Mesh);

	arrVTX[0].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(1.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(1.f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"LeftRectMesh", Mesh);

	arrVTX[0].vPos = Vec3(-1.f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(-1.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"RightRectMesh", Mesh);

	arrVTX[0].vPos = Vec3(-0.5f, 0.f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vTex = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(0.f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[1].vTex = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.5f, 0.f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vTex = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(0.f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vTex = Vec2(0.f, 1.f);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, iIndex, sizeof(UINT), 5, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddResource<CMesh>(L"IsoMetricColl", Mesh);

	Mesh = CMesh::CreateMesh(arrVTX, sizeof(VTX), 4, arrIndex, sizeof(IDX), 2);
	AddResource<CMesh>(L"IsoMetric", Mesh);

	vector<VTX> vecVTX;
	vector<UINT> vecId;

	UINT iSlice = 64;
	float fAngle = XM_2PI / (float)iSlice;

	VTX vtx;
	vtx.vPos = Vec3(0.f, 0.f, 0.f);
	vtx.vColor = Vec4(1.f,0.f,0.f,1.f);

	vecVTX.push_back(vtx);

	for (UINT i = 0; i < iSlice; ++i)
	{
		vtx.vPos = Vec3(cosf(fAngle * i) * 0.5f,sinf(fAngle * i) * 0.5f,0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX.push_back(vtx);
	}

	for (int i = 0; i < iSlice - 1; ++i)
	{
		vecId.push_back(0);
		vecId.push_back(i + 2);
		vecId.push_back(i + 1);
	}

	vecId.push_back(0);
	vecId.push_back(1);
	vecId.push_back(iSlice);

	Mesh = CMesh::CreateMesh(&vecVTX[0], sizeof(VTX), vecVTX.size(), &vecId[0], sizeof(UINT), vecId.size());
	AddResource<CMesh>(L"Circle", Mesh);


	vector<VTX> vecVTX2;
	vector<UINT> vecId2;

	for (UINT i = 0; i < iSlice; ++i)
	{
		vtx.vPos = Vec3(cosf(fAngle * i) * 0.5f, sinf(fAngle * i) * 0.5f, 0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX2.push_back(vtx);
	}

	for (int i = 0; i < iSlice; ++i)
	{
		vecId2.push_back(i);
	}

	vecId2.push_back(0);

	Mesh = CMesh::CreateMesh(&vecVTX2[0], sizeof(VTX), vecVTX2.size(), &vecId2[0], sizeof(UINT), vecId2.size(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddResource<CMesh>(L"CollCircle", Mesh);
}
	
void CResMgr::GridMesh(int _iVertexX, int _iVertexZ, int _iWidth, int _iDepth)
{	
	UINT vertexCount = _iVertexX * _iVertexZ;           // ÃÑ Á¤Á¡ °¹¼ö
	UINT faceCount = (_iVertexX - 1)*(_iVertexZ - 1) * 2; // ÃÑ »ï°¢Çü °¹¼ö *2 ¾ÈÇÏ¸é »ç°¢Çü °¹¼ö

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*_iWidth;
	float halfDepth = 0.5f*_iDepth;

	float dx = _iWidth / (_iVertexX - 1); // °¡·Î °£°Ý
	float dz = _iDepth / (_iVertexZ - 1); // ±íÀÌ ¹æÇâ °£°Ý


	vector<VTX> vecVTX;
	vecVTX.resize(vertexCount);

	for (UINT i = 0; i < _iVertexZ; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < _iVertexX; ++j)
		{
			float x = -halfWidth + j * dx;
			float y = 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
			vecVTX[i*_iVertexX + j].vPos = Vec3(x, y, z);
			vecVTX[i*_iVertexX + j].vColor = Vec4(1.f, 0.f, 1.f,1.f);
			vecVTX[i*_iVertexX + j].vTex = Vec2(j,i);
		}
	}

	vecVTX[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	
	//
	// Create the indices.
	//

	vector<IDX> vecIDX;
	vecIDX.resize(faceCount);

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < _iVertexZ - 1; ++i)
	{
		for (UINT j = 0; j < _iVertexX - 1; ++j)
		{
			vecIDX[k].first = i * _iVertexX + j;
			vecIDX[k].second = i * _iVertexX + j + 1;
			vecIDX[k].third = (i + 1)*_iVertexX + j;

			vecIDX[k + 1].first = (i + 1)*_iVertexX + j;
			vecIDX[k + 1].second = i * _iVertexX + j + 1;
			vecIDX[k + 1].third = (i + 1)*_iVertexX + j + 1;

			k += 2; // next quad
		}
	}

	CMesh* Mesh = CMesh::CreateMesh(&vecVTX[0], sizeof(VTX), vecVTX.size(), &vecIDX[0], sizeof(IDX), vecIDX.size());
	AddResource<CMesh>(L"Grid", Mesh);
}

void CResMgr::GetDeleteClone(CMaterial * _pMtrl)
{
	vector<CMaterial*>::iterator iter = m_vecClone.begin();
	for (; iter != m_vecClone.end(); ++iter)
	{
		if (*iter == _pMtrl)
		{
			delete *iter;
			m_vecClone.erase(iter);
			break;
		}
	}

}

CGameObj * CResMgr::PrefabToCreateObj(wstring _wcsLayer, wstring _wcsPrefabName)
{
	CPrefab* pPrefab = Load<CPrefab>(_wcsPrefabName);
	CGameObj* pObj = pPrefab->Instantiate();

	UINT iIdx = CSceneMgr::GetInst()->GetLayerIndex(_wcsLayer);
	tEvent event = tEvent{ EVENT_TYPE::CREATE_OBJ,(INT_PTR)iIdx,(INT_PTR)pObj };
	CEventMgr::GetInst()->AddEvent(event);

	return pObj;
}
void CResMgr::RemoveResource(const wstring & _wcsFind, RESOURCE_TYPE _eType)
{
	map<wstring, CRes*>::iterator ResIter = m_mapRes[(UINT)_eType].find(_wcsFind);

	if (ResIter == m_mapRes[(UINT)_eType].end())
		assert(NULL);

	delete ResIter->second;
	ResIter->second = nullptr;
	m_mapRes[(UINT)_eType].erase(ResIter);
}


void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Color"));
	AddResource<CMaterial>(L"Color", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Coll"));
	AddResource<CMaterial>(L"Coll", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Std"));
	AddResource<CMaterial>(L"Std", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Spectrum"));
	AddResource<CMaterial>(L"Spectrum", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Green"));
	AddResource<CMaterial>(L"Green", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"TexColor"));
	AddResource<CMaterial>(L"TexColor", pMtrl);
}

void CResMgr::SoundInit()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::SoundPlay(const wstring & _wcsSound,int _iCount)
{
	CSound* pSound = CResMgr::GetInst()->Load<CSound>(_wcsSound);

	if (pSound != nullptr)
	{
		pSound->Play(1);
	}
	else
	{
		wstring wcsPath = CPathMgr::GetInst()->GetPath();
		wcsPath += L"Sound\\" + _wcsSound;
		wcsPath += L".wav";
		pSound = new CSound;
		pSound->Load(wcsPath, _wcsSound);
		CResMgr::GetInst()->AddResource<CSound>(_wcsSound, pSound);
		pSound->Play(_iCount);
	}
}