#include "CCamera.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "CLayer.h"
#include "CSceneMgr.h"
#include "CCore.h"
#include "CTransForm.h"
#include "CStateMgr.h"
#include "CGameMgr.h"

CCamera::CCamera()
	:m_bPerspective(true)
	,m_fNear(1.f)
	,m_fFar(10000.f)
	,m_fFOV(XM_PI / 4.f)
	,m_fScale(1.0f)
	, m_iLayerMask(0xffff)
{
	m_eType = COMPONENT_TYPE::CAMERA;
}

CCamera::~CCamera()
{
}

void CCamera::Init()
{
}

int CCamera::Update()
{
	return 0;
}

int CCamera::LateUpdate()
{
	CSceneMgr::GetInst()->RegisterCamera(this);
	return 0;
}

int CCamera::FinalUpdate()
{
	Vec3 vPos = -TransForm()->GetPos();

	m_mtxView = XMMatrixIdentity();

	m_mtxView.Right(TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
	m_mtxView.Up(TransForm()->GetWorldDir(DIR_TYPE::UP));
	m_mtxView.Backward(TransForm()->GetWorldDir(DIR_TYPE::FRONT));

	m_mtxView = m_mtxView.Transpose();

	m_mtxView._41 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::RIGHT));
	m_mtxView._42 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::UP));
	m_mtxView._43 = vPos.Dot(TransForm()->GetWorldDir(DIR_TYPE::FRONT));

	g_World.mtxView = m_mtxView;//XMMatrixTranslation(m_vViewPos.x, m_vViewPos.y, m_vViewPos.z) * XMMatrixTranspose(XMMatrixRotationY(XMConvertToRadians(TransForm()->GetRotaion().x)));

	if (m_bPerspective)
		g_World.mtxProj = XMMatrixPerspectiveFovLH(m_fFOV, CCore::GetInst()->GetResolution().x / CCore::GetInst()->GetResolution().y, m_fNear, m_fFar);
	else
	{
		float fHalfWidth = CCore::GetInst()->GetResolution().x * 0.5f * m_fScale;
		float fHalfHeight = CCore::GetInst()->GetResolution().y * 0.5f * m_fScale;

		if (0 != fHalfWidth && 0 != fHalfHeight)
		g_World.mtxProj = XMMatrixOrthographicOffCenterLH(-fHalfWidth , fHalfWidth,  -fHalfHeight, fHalfHeight, m_fNear, m_fFar);
	}

	return 0;
}

void CCamera::Render(CLayer** _pLayer)
{
	if (GetOwner()->GetName() == L"UICamera" && !CGameMgr::GetInst()->GetUIRender())
		return;
	FinalUpdate();

	UINT iMirror = CSceneMgr::GetInst()->GetLayerIndex(L"Mirror");
	UINT iMirrorObj = CSceneMgr::GetInst()->GetLayerIndex(L"MirrorObj");
	UINT iDefault = CSceneMgr::GetInst()->GetLayerIndex(L"Default");
	UINT iBack = CSceneMgr::GetInst()->GetLayerIndex(L"Back");
	bool bStop = CSceneMgr::GetInst()->GetStop();
	UINT iLayerCount = CSceneMgr::GetInst()->GetCurLayerCount();

	if (bStop)
	{
		for (UINT i = 0; i < iLayerCount; ++i)
		{
			if (iDefault == i || iMirror == i || iMirrorObj == i || iBack == i)
				continue;

			if (_pLayer[i] != NULL && m_iLayerMask & (1 << i))
				_pLayer[i]->Render();
		}
	}
	else
	{
		if (GetOwner()->GetName() == L"UICamera")
		{
			int aaa = 0;
		}
		CSceneMgr::GetInst()->RenderSort(m_iLayerMask);
		/*for (UINT i = 0; i <= iMirrorObj; ++i)
		{
			if (iMirror == i)
				CStateMgr::GetInst()->DepthRender(L"Stencil");
			else if (iMirrorObj == i)
				CStateMgr::GetInst()->DepthRender(L"StencilObj");

			if (_pLayer[i] != NULL && m_iLayerMask & (1 << i))
				_pLayer[i]->Render();
		}

		CStateMgr::GetInst()->DepthRender(L"Default");

		for (UINT i = iMirrorObj + 1; i < MAX_LAYER_COUNT; ++i)
		{
			if (_pLayer[i] != NULL && m_iLayerMask & (1 << i))
				_pLayer[i]->Render();
		}*/
	}
}

void CCamera::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_bPerspective, sizeof(bool), 1, _pFile);
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _pFile);
}

void CCamera::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_bPerspective, sizeof(bool), 1, _pFile);
	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);
	fread(&m_iLayerMask, sizeof(UINT), 1, _pFile);
}

void CCamera::CheckLayerMask(UINT _iMask)
{
	m_iLayerMask ^= (1 << _iMask);
}