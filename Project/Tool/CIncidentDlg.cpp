// CEventDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CIncidentDlg.h"
#include "afxdialogex.h"
#include "CTestMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CResMgr.h"
#include "CToolScript.h"
#include "CIncident.h"
#include "KeyMgr.h"
#include "CTransForm.h"
#include "CSceneMgr.h"
#include "CEventMgr.h"
#include "CCore.h"
#include "CTile.h"
#include "CLayer.h"
#include "CGrid.h"
#include "CCollider.h"
#include "CTileMgrScript.h"

// CEventDlg 대화 상자

IMPLEMENT_DYNAMIC(CIncidentDlg, CDialogEx)

CIncidentDlg::CIncidentDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_EVENT_DLG, pParent)
{
	m_eMode = EVENT_MODE::NONE;
}

CIncidentDlg::~CIncidentDlg()
{
}

void CIncidentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbKinds);
	DDX_Control(pDX, IDC_LIST2, m_listObjKinds);
	DDX_Control(pDX, IDC_LIST1, m_listEvent);
}

void CIncidentDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
	if (m_cbFocus)
		return;
	m_listEvent.ResetContent();

	Vec3 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	if (m_eMode == EVENT_MODE::WALL)
	{
		if (KEYTAB(KEY_TYPE::KEY_LBTN))
		{
			POINT ptIdx = {};

			CGameObj* pTile = CTileMgrScript::GetInst()->GetPickingTile(vMousePos, ptIdx);
			if (pTile == nullptr)
				return;
			pTile->GetTile()->SetCollType(COLL_TYPE::COLL);
			int i = 1;
			pTile->GetMeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);
			m_vecTilePos.push_back(ptIdx);
		}
		else if (KEYTAB(KEY_TYPE::KEY_RBTN))
		{
			if (m_vecTilePos.empty())
				return;

			POINT ptIdx = m_vecTilePos.back();
			vector<CGameObj*>& pGrid = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();
			CGameObj* pTile = pGrid[0]->GetGrid()->GetVecObj()[ptIdx.y][ptIdx.x];
			pTile->GetTile()->SetCollType(COLL_TYPE::NOCOLL);
			int i = 0;
			pTile->GetMeshRender()->GetMtrl()->SetParam(&i, SHADER_PARAM::INT_0);
			m_vecTilePos.pop_back();
		}
	}
	else if (m_eMode == EVENT_MODE::MONSTER)
	{
		if (KEYTAB(KEY_TYPE::KEY_LBTN))
		{
			if (vMousePos.x < -CCore::GetInst()->GetResolution().x / 2.f || vMousePos.x > CCore::GetInst()->GetResolution().x / 2.f
				|| vMousePos.y < -CCore::GetInst()->GetResolution().y / 2.f || vMousePos.y > CCore::GetInst()->GetResolution().y / 2.f)
				return;

			vMousePos -= g_World.mtxView.Translation();
			CGameObj* pObj = new CGameObj;
			pObj->SetName(m_strObj.GetBuffer());
			pObj->AddComponent(new CMeshRender);
			pObj->GetTransForm()->SetPos(vMousePos);
			pObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
			pObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(L"Std"));
			pObj->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(m_strObj.GetBuffer()));

			m_vecObj.push_back(pObj);
			CSceneMgr::GetInst()->AddObject(1, pObj);
		}
		else if (KEYTAB(KEY_TYPE::KEY_RBTN))
		{
			if (m_vecObj.empty())
				return ;
			tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecObj.back() ,0 };
			CEventMgr::GetInst()->AddEvent(event);

			m_vecObj.pop_back();
		}
	}

	vector<EventObj>& vecObj = m_pTargetObj->GetIncident()->GetVecObj();

	for(UINT i = 0; i < vecObj.size(); ++i)
		m_listEvent.InsertString(i , vecObj[i].wcsName.c_str());
}

BEGIN_MESSAGE_MAP(CIncidentDlg, CDialogEx)


	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CIncidentDlg::OnCbnSelchangeCombo)
	ON_BN_CLICKED(IDC_BUTTON6, &CIncidentDlg::OnBnClickedObjPlace)
	ON_BN_CLICKED(IDC_BUTTON1, &CIncidentDlg::OnBnClickedApply)
END_MESSAGE_MAP()

// CEventDlg 메시지 처리기
BOOL CIncidentDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_cbKinds.InsertString(0,L"Monster");
	m_cbKinds.InsertString(1, L"Wall");

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CIncidentDlg::OnCbnSelchangeCombo()
{    
	m_listObjKinds.ResetContent();

	int iSel = m_cbKinds.GetCurSel();

	if (iSel == 0)
		m_listObjKinds.InsertString(0,L"Suppressor");

	else if (iSel == 1)
		m_listObjKinds.InsertString(0, L"Wall");
}

void CIncidentDlg::OnBnClickedObjPlace()
{
	int icbSel = m_cbKinds.GetCurSel();
	int iSel = m_listObjKinds.GetCurSel();

	if (iSel == LB_ERR)
		return;

	if (m_pTargetObj == nullptr || m_pTargetObj->GetIncident() == nullptr)
		return;

	m_listObjKinds.GetText(iSel, m_strObj);
	
	CGameObj* pObj = CTestMgr::GetInst()->GetMouseObj();
	((CToolScript*)pObj->GetScript()[0])->SetEvent(true);

	if (icbSel == 0)
	{
		m_eMode = EVENT_MODE::MONSTER;
		pObj->GetMeshRender()->SetTexture(CResMgr::GetInst()->Load<CTexture>(m_strObj.GetBuffer()));
	}
	else if (icbSel == 1)
		m_eMode = EVENT_MODE::WALL;
}

void CIncidentDlg::OnBnClickedApply() // 다음으로 할일 이제 타일 좌표를 이벤트 컴포넌트에게 전달 해줘야 한다.0
{
	CGameObj* pObj = CTestMgr::GetInst()->GetMouseObj();

	for (size_t i = 0; i < m_vecObj.size(); i++)
		m_pTargetObj->GetIncident()->AddObj(m_vecObj[i]);

	for (size_t i = 0; i < m_vecTilePos.size(); i++)
		m_pTargetObj->GetIncident()->AddTile(m_vecTilePos[i]);
	
	ClearObj();
	pObj->GetMeshRender()->SetTexture(nullptr);
	((CToolScript*)pObj->GetScript()[0])->SetEvent(false);

	m_eMode = EVENT_MODE::NONE;
}

void CIncidentDlg::ClearObj()
{
	for (size_t i = 0; i < m_vecObj.size(); i++)
	{
		tEvent event = tEvent{ EVENT_TYPE::DELETE_OBJ,(INT_PTR)m_vecObj[i] ,0 };
		CEventMgr::GetInst()->AddEvent(event);
	}
	m_vecObj.clear();

	for (size_t i = 0; i < m_vecTilePos.size(); i++)
	{
		vector<CGameObj*>& pGrid = CSceneMgr::GetInst()->GetLayer(L"Tile")->GetGameObj();
		CGameObj* pTile = pGrid[0]->GetGrid()->GetVecObj()[m_vecTilePos[i].y][m_vecTilePos[i].x];
		pTile->GetTile()->SetCollType(COLL_TYPE::NOCOLL);
		int iType = 0;
		pTile->GetMeshRender()->GetMtrl()->SetParam(&iType, SHADER_PARAM::INT_0);
	}

	m_vecTilePos.clear();
}