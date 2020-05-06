// CColliderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CColliderDlg.h"
#include "afxdialogex.h"
#include "CGameObj.h"
#include "CCollider.h"
#include "MainFrm.h"
// CColliderDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderDlg, CDialogEx)

CColliderDlg::CColliderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_COLLIDER, pParent)
{

}

CColliderDlg::~CColliderDlg()
{
}

void CColliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	for (UINT i = IDC_EDIT1; i <= IDC_EDIT3; ++i)
		DDX_Control(pDX, i, m_arrPos[i - IDC_EDIT1]);

	for (UINT i = IDC_EDIT4; i <= IDC_EDIT6; ++i)
		DDX_Control(pDX, i, m_arrScale[i - IDC_EDIT4]);

	DDX_Control(pDX, IDC_EDIT7, m_Radius);

	DDX_Control(pDX, IDC_COMBO1, m_cbType);
}

void CColliderDlg::Init()
{
	m_cbType.InsertString(0, L"Rect2D");
	m_cbType.InsertString(1, L"Circle2D");
	m_cbType.InsertString(2, L"IsoMetrix2D");
}

void CColliderDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_bFocus || m_cbFocus)
		return;


	m_bClick = false;

	if (m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_BOX)
	{
		for (UINT i = 0; i < 3; ++i)
			m_arrScale[i].EnableWindow(true);
		
		m_Radius.EnableWindow(false);

		m_cbType.SetCurSel(0);
	}
	else if (m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_ISO)
	{
		for (UINT i = 0; i < 3; ++i)
			m_arrScale[i].EnableWindow(true);

		m_Radius.EnableWindow(false);

		m_cbType.SetCurSel(2);
	}
	else if(m_pTargetObj->GetCollider()->GetColliderType() == COLLIDER_TYPE::COLL_CIRCLE)
	{
		for (UINT i = 0; i < 3; ++i)
			m_arrScale[i].EnableWindow(false);
		
		m_Radius.EnableWindow(true);
		m_cbType.SetCurSel(1);
	}

	Vec3 vPos = m_pTargetObj->GetCollider()->GetPos();
	Vec3 vScale = m_pTargetObj->GetCollider()->GetScale();
	float fRad = m_pTargetObj->GetCollider()->GetRadius();
	CString strTemp;
	
	
	for (UINT i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vPos[i]);
		m_arrPos[i].SetWindowTextW(strTemp);
	}

	for (UINT i = 0; i < 3; ++i)
	{
		strTemp.Format(L"%f", vScale[i]);
		m_arrScale[i].SetWindowTextW(strTemp);
	}

	strTemp.Format(L"%f", fRad);
	m_Radius.SetWindowTextW(strTemp);

	m_bClick = true;
}

void CColliderDlg::OnChangeEdit(UINT _iId)
{
	if (!m_bClick)
		return;

	CString strTemp;
	Vec3 vEdit;
	float fRad;

	for (UINT i = 0; i < 3; ++i)
	{
		m_arrPos[i].GetWindowTextW(strTemp);
		vEdit[i] = _wtof(strTemp);
		m_pTargetObj->GetCollider()->SetPos(vEdit);
	}

	for (UINT i = 0; i < 3; ++i)
	{
		m_arrScale[i].GetWindowTextW(strTemp);
		vEdit[i] = _wtof(strTemp);
		m_pTargetObj->GetCollider()->SetScale(vEdit);
	}

	m_Radius.GetWindowTextW(strTemp);
	fRad = _wtof(strTemp);
	m_pTargetObj->GetCollider()->SetRadius(fRad);
}

BEGIN_MESSAGE_MAP(CColliderDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT7, &CComponentDlg::OnEditKillfocus)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT7, &CComponentDlg::OnEditSetfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT7, &CColliderDlg::OnChangeEdit)

	ON_CBN_KILLFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CComponentDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CColliderDlg::OnCbnSelchange)
END_MESSAGE_MAP()

void CColliderDlg::OnCbnSelchange()
{
	if (m_cbType.GetCurSel() == 0)
	{
		m_pTargetObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_BOX);
	}
	else if (m_cbType.GetCurSel() == 2)
	{
		m_pTargetObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_ISO);
	}
	else if (m_cbType.GetCurSel() == 1)
	{
		CString strTemp;
		float fRad = 0.f;
		m_Radius.GetWindowTextW(strTemp);
		fRad = _wtof(strTemp);

		m_pTargetObj->GetCollider()->SetColliderType(COLLIDER_TYPE::COLL_CIRCLE);
		m_pTargetObj->GetCollider()->SetScale(Vec3(fRad, fRad,0.f));
	}

	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameHwnd());
}