#include "CGameObj.h"
#include "CTransForm.h"
#include "CMeshRender.h"
#include "CScript.h"
#include "CStateMgr.h"
#include "CCamera.h"
#include "CCollider.h"
#include "CSceneMgr.h"
#include "CLayer.h"
#include "CGrid.h"
#include "CAnimator2D.h"
#include "CTile.h"
#include "CIncident.h"
#include "CCore.h"
#include "CGameMgr.h"

CGameObj::CGameObj()
	:m_arrComponent{}
	,m_iLayerID(-1)
	, m_bColl(false)
	, m_bRender(false)
{
	AddComponent(new CTransForm);
}

CGameObj::CGameObj(const CGameObj& _pObj)
{
	SetName(_pObj.GetName());
	m_iLayerID = _pObj.m_iLayerID;
	m_bColl = _pObj.m_bColl;
	
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(_pObj.m_arrComponent[i] != NULL)
			AddComponent(_pObj.m_arrComponent[i]->Clone());
	}
	for (UINT i = 0; i < _pObj.m_vecScript.size(); ++i)
		AddComponent(_pObj.m_vecScript[i]->Clone());

	for (UINT i = 0; i < _pObj.m_vecChild.size(); ++i)
	{
		AddChild(_pObj.m_vecChild[i]->Clone());
	}

	if(_pObj.m_arrComponent[(UINT)COMPONENT_TYPE::ANIMATION_2D] != NULL)
	((CAnimator2D*)m_arrComponent[(UINT)COMPONENT_TYPE::ANIMATION_2D])->AnimSetOwner();
}

CGameObj::~CGameObj()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		SAFE_DELETE(m_arrComponent[i]);
	}
	//SAFE_ARRAY_DELETE(m_arrComponent, (UINT)COMPONENT_TYPE::END);
	SAFE_ARRAY_DELETE(m_vecScript, m_vecScript.size());

	SafeArrayDelete(m_vecChild);
}

void CGameObj::AddComponent(CComponent * _pComponent)
{
	COMPONENT_TYPE eType = _pComponent->GetType();

	if (eType == COMPONENT_TYPE::SCRIPT)
		m_vecScript.push_back((CScript*)_pComponent);
	else
	{
		if (m_arrComponent[(UINT)eType] != nullptr)
			assert(false && "이미 객체에 컴포넌트가 존재합니다.");
		m_arrComponent[(UINT)eType] = _pComponent;
	}
	_pComponent->SetParentObj(this);
}

bool CGameObj::AddComponent(CComponent * _pComponent, COMPONENT_TYPE _eType)
{
	if (_eType == COMPONENT_TYPE::SCRIPT)
		m_vecScript.push_back((CScript*)_pComponent);
	else
	{
		if (m_arrComponent[(UINT)_eType] != nullptr)
		{
			delete _pComponent;
			_pComponent = nullptr;
			MessageBox(NULL,L"이미 Component가 존재 합니다.",L"에러",MB_OK);
			//assert(false && "이미 객체에 컴포넌트가 존재합니다.");
			return false;
		}
		m_arrComponent[(UINT)_eType] = _pComponent;
	}
	_pComponent->SetParentObj(this);
	return true;
}

void CGameObj::AddChild(CGameObj * _pObj)
{
	if (_pObj->GetParent() != nullptr)
	{
		_pObj->DisconnectParent();
		AddChild(_pObj);
	}
	else
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(_pObj->GetID());

		if (pLayer != nullptr)
			pLayer->PopGameObject(_pObj);

		m_vecChild.push_back(_pObj);
		_pObj->SetParent(this);
		_pObj->SetID(m_iLayerID);
	}
}

void CGameObj::DisconnectParent()
{
	if (!m_pParent)
		return;

	CSceneMgr::GetInst()->GetLayer(L"Default")->AddGameObject(this);
	vector<CGameObj*>::iterator iter = m_pParent->GetChild().begin();

	for (; iter != m_pParent->GetChild().end(); ++iter)
	{
		if (*iter == this)
		{
			m_pParent->GetChild().erase(iter);
			m_pParent = nullptr;
			return;
		}
	}

	assert(NULL);
}

void CGameObj::ChangeLayer(UINT _iID)
{
	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(m_iLayerID);

	vector<CGameObj*>& vecObj = pLayer->GetGameObj();
	vector<CGameObj*>::iterator iter = vecObj.begin();
	CSceneMgr::GetInst()->GetLayer(_iID)->AddGameObject(this);

	for (size_t i = 0; i < m_vecChild.size(); i++)
	{
		m_vecChild[i]->SetID(_iID);
	}

	for (; iter != vecObj.end(); ++iter)
	{
		if (*iter == this)
		{
			vecObj.erase(iter);
			m_iLayerID = _iID;
			return ;
		}
	}

	assert(NULL);
}

void CGameObj::Awake()
{
	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Awake();

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Awake();
	}
}

CGameObj * CGameObj::GetChild(wstring _wcsName)
{
	for (size_t i = 0; i < m_vecChild.size(); i++)
	{
		if (m_vecChild[i]->GetName() == _wcsName)
		{
			return m_vecChild[i];
		}
	}

	return nullptr;
}

void CGameObj::Start()
{
	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Start();

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->SetID(m_iLayerID);
		m_vecChild[i]->Start();
	}
}

void CGameObj::Update()
{
	if (m_pParent != nullptr && m_wcsLayerName != L"Skill")
	{
		Vec3 vPos = GetTransForm()->GetPos();
		Vec3 vParentPos = m_pParent->GetTransForm()->GetPos();
		vPos.z = vParentPos.z - 5.f;
		GetTransForm()->SetPos(vPos);
	}
	else if (m_wcsLayerName == L"Monster" ||
		m_wcsLayerName == L"Default" || m_wcsLayerName == L"Skill" || m_wcsLayerName == L"Mirror")
	{
		Vec3 vPos = GetTransForm()->GetPos();
		vPos.z = vPos.y - g_World.mtxView._42 + (CCore::GetInst()->GetResolution().y / 2.f) + 100.f;
		GetTransForm()->SetPos(vPos);
	}
	else if (m_wcsLayerName == L"Mouse")
	{
		Vec3 vPos = GetTransForm()->GetPos();
		Vec3 vScale = GetTransForm()->GetScale();
		vPos.z = (vPos.y - vScale.y * 0.5f) - g_World.mtxView._42 + (CCore::GetInst()->GetResolution().y / 2.f) + 100.f;
		GetTransForm()->SetPos(vPos);
	}
	else if (m_wcsLayerName == L"Player")
	{
		float fOffset = 100.f;
		if (CGameMgr::GetInst()->GetEnd())
			fOffset = 700.f;

		Vec3 vPos = GetTransForm()->GetPos();
		if (m_arrComponent[(UINT)COMPONENT_TYPE::COLLIDER] != nullptr)
			vPos.z = (vPos.y + GetCollider()->GetPos().y) - g_World.mtxView._42 + (CCore::GetInst()->GetResolution().y / 2.f) + fOffset;
		else
			vPos.z = vPos.y - g_World.mtxView._42 + (CCore::GetInst()->GetResolution().y / 2.f) + fOffset;
		GetTransForm()->SetPos(vPos);
	}
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if(m_arrComponent[i] != NULL)
		m_arrComponent[i]->Update();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->Update();

	for (UINT i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->Update();
}

void CGameObj::LateUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != NULL)
		m_arrComponent[i]->LateUpdate();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->LateUpdate();
	}
}

void CGameObj::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != NULL)
		m_arrComponent[i]->FinalUpdate();
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->FinalUpdate();

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->FinalUpdate();
	}
}

void CGameObj::Render()
{
	if(m_arrComponent[(UINT)COMPONENT_TYPE::MESH_RENDER] != NULL)
		GetMeshRender()->Render();

	if (m_arrComponent[(UINT)COMPONENT_TYPE::COLLIDER] != NULL && CGameMgr::GetInst()->GetTileRender())
		GetCollider()->Render();

	if (m_arrComponent[(UINT)COMPONENT_TYPE::GRID] != NULL)
	{
			GetGrid()->Render();
	}
	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Render();
	}
}

bool CGameObj::IsFamily(CGameObj* _pObj)
{
	CGameObj* pParent = m_pParent;

	while (pParent)
	{
		if (pParent == _pObj)
			return true;
		pParent = pParent->GetParent();
	}

	if (_pObj->GetParent() == this)
		return true;

	return false;
}

void CGameObj::LayerEscape()
{
	if (m_pParent != nullptr)
	{
		vector<CGameObj*>& vecChild = m_pParent->GetChild();
		vector<CGameObj*>::iterator iter = vecChild.begin();

		for (; iter != vecChild.end(); ++iter)
		{
			if (*iter == this)
			{
				vecChild.erase(iter);
				return;
			}
		}
		assert(NULL);
	}

	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(m_iLayerID);

	if (pLayer != nullptr)
		pLayer->PopGameObject(this);
}

void CGameObj::Save(FILE* _pFile)
{
	wstring wcsName = GetName();
	SaveWString(GetName(),_pFile);

	fwrite(&m_bColl,sizeof(bool),1,_pFile);

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComponent[i] != nullptr)
		{
			if (i == (UINT)COMPONENT_TYPE::COLLIDER)
			{
				if (m_arrComponent[(UINT)COMPONENT_TYPE::TILE] != nullptr && ((CTile*)m_arrComponent[(UINT)COMPONENT_TYPE::TILE])->GetCollType() == COLL_TYPE::COLL)
				{
					fwrite(&i, sizeof(UINT), 1, _pFile);
					m_arrComponent[i]->Save(_pFile);
				}
				else
				{
					fwrite(&i, sizeof(UINT), 1, _pFile);
					m_arrComponent[i]->Save(_pFile);
				}
			}
			else if (i != (UINT)COMPONENT_TYPE::COLLIDER)
			{
				fwrite(&i, sizeof(UINT), 1, _pFile);
				m_arrComponent[i]->Save(_pFile);
			}
		}
	}

	UINT iEnd = (UINT)COMPONENT_TYPE::END;
	fwrite(&iEnd, sizeof(UINT), 1, _pFile);
}

void CGameObj::Load(FILE * _pFile)
{
	SetName(LoadWString(_pFile));

	fread(&m_bColl, sizeof(bool), 1, _pFile);

	bool bEnd = false;
	CComponent* pCom = nullptr;

	while (!bEnd)
	{
		UINT iIdx = 0;
		fread(&iIdx, sizeof(UINT), 1, _pFile);

		switch (COMPONENT_TYPE(iIdx))
		{
		case COMPONENT_TYPE::TRANSFORM:
			m_arrComponent[iIdx]->Load(_pFile);
			break;
		case COMPONENT_TYPE::MESH_RENDER:
			pCom = new CMeshRender;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::COLLIDER:
			pCom = new CCollider;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::ANIMATION_2D:
			pCom = new CAnimator2D;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::GRID:
			pCom = new CGrid;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::INCIDENT:
			pCom = new CIncident;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::TILE:
			pCom = new CTile;
			AddComponent(pCom);
			pCom->Load(_pFile);
			break;
		case COMPONENT_TYPE::END:
			bEnd = true;
			continue;
			break;
		default:
			assert(NULL);
			break;
		}
	}
}

void CGameObj::SetColl(bool _bColl)
{
	m_bColl = _bColl;
}