#pragma once
class CRes;
class CComponent;

#include "global.h"
#include "CMesh.h"
#include "CTexture.h"
#include "CPrefab.h"
#include "CMaterial.h"
#include "CSound.h"

class CResMgr
{
	SINGLETON(CResMgr)

private:
	map<wstring, CRes*> m_mapRes[(UINT)RESOURCE_TYPE::END];
	vector<CMaterial*> m_vecClone;

public:
	void DefaultRectMesh();
	void CreateDefaultMaterial();
	void GridMesh(int _iVertexX, int _iVertexZ, int _iWidth,int _iDepth);
	map<wstring, CRes*>& GetResource(RESOURCE_TYPE _eType) { return m_mapRes[(UINT)_eType]; }
	void AddClone(CMaterial* _pMtrl) { m_vecClone.push_back(_pMtrl); }
	void GetDeleteClone(CMaterial* _pMtrl);
	CGameObj* PrefabToCreateObj(wstring _wcsLayer, wstring _wcsPrefabName);
	void SoundInit();
	void SoundPlay(const wstring& _wcsSound,int _iCount);

public:
	void RemoveResource(const wstring& _wcsFind, RESOURCE_TYPE _eType);

	template<typename T>
	T* Load(const wstring& _wcsFind);

	template<typename T>
	void AddResource(const wstring& _wcsFind,CRes* _pRes);

	template<typename T>
	void ChangeResource(const wstring& _wcsFind, CRes* _pRes);
};

template<typename T>
inline T * CResMgr::Load(const wstring & _wcsFind)
{
	// static���� �� ������ type_infoŸ���� ���ϴ� �뵵�θ� ���δ�.
	// �ٸ� Ÿ���� ������ ���ο� ���ø� �Լ��� �����Ǿ� ���ο� static�̹Ƿ� �̷��� �ʱ�ȭ �ص� ������ ���� �ʴ´�.
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	
	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &sound)
		eType = RESOURCE_TYPE::SOUND;

	map<wstring,CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter == m_mapRes[(UINT)eType].end())
		return nullptr;

	return (T*)iter->second;
}

template<typename T>
inline void CResMgr::AddResource(const wstring & _wcsFind, CRes * _pRes)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &sound)
		eType = RESOURCE_TYPE::SOUND;

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter != m_mapRes[(UINT)eType].end())
	{
		delete _pRes;
		return;
	}
	//assert(false && L"�̹� �ش� ���ҽ��� ���� �մϴ�.");

	_pRes->SetName(_wcsFind);
	_pRes->SetPath(_pRes->GetPath());

	m_mapRes[(UINT)eType].insert(make_pair(_wcsFind,_pRes));
}

template<typename T>
inline void CResMgr::ChangeResource(const wstring & _wcsFind, CRes * _pRes)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& prefab = typeid(CPrefab);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
		eType = RESOURCE_TYPE::MESH;
	else if (&info == &texture)
		eType = RESOURCE_TYPE::TEXTURE;
	else if (&info == &prefab)
		eType = RESOURCE_TYPE::PREFAB;
	else if (&info == &material)
		eType = RESOURCE_TYPE::MATERIAL;
	else if (&info == &material)
		eType = RESOURCE_TYPE::SOUND;

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_wcsFind);

	if (iter != m_mapRes[(UINT)eType].end())
	{
		delete iter->second;
		iter->second = nullptr;
		m_mapRes[(UINT)eType].erase(iter);
	}
	//assert(false && L"�̹� �ش� ���ҽ��� ���� �մϴ�.");

	_pRes->SetName(_wcsFind);
	_pRes->SetPath(_pRes->GetPath());

	m_mapRes[(UINT)eType].insert(make_pair(_wcsFind, _pRes));
}
