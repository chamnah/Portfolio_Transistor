// CMeshRenderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CMeshRenderDlg.h"
#include "afxdialogex.h"
#include "CDlgList.h"
#include "CResMgr.h"
#include "CGameObj.h"
#include "CMeshRender.h"
#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTransForm.h"
#include "CTexture.h"
#include "CRenderMgr.h"
#include "CBlendState.h"

// CMeshRenderDlg 대화 상자

IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_MESHRENDER, pParent)
{
}

CMeshRenderDlg::~CMeshRenderDlg()
{
}

void CMeshRenderDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;

	if (m_pTargetObj->GetMeshRender()->GetMesh() != nullptr)
	{
		CString strMesh = m_pTargetObj->GetMeshRender()->GetMesh()->GetName().c_str();
		m_EditMesh.SetWindowTextW(strMesh);
	}
	else
		m_EditMesh.SetWindowTextW(L"");

	if (m_pTargetObj->GetMeshRender()->GetMtrl() != nullptr)
	{
		CString strMtrl = m_pTargetObj->GetMeshRender()->GetMtrl()->GetName().c_str();
		m_EditMaterial.SetWindowTextW(strMtrl);
	}
	else
		m_EditMaterial.SetWindowTextW(L"");


	if (m_pTargetObj->GetMeshRender()->GetMtrl() != nullptr && m_pTargetObj->GetMeshRender()->GetMtrl()->GetBlend() != nullptr)
	{
		CString strBlend = m_pTargetObj->GetMeshRender()->GetMtrl()->GetBlend()->GetName().c_str();
		m_Blend.SetWindowTextW(strBlend);
	}
	else
		m_Blend.SetWindowTextW(L"NONE");
}

void CMeshRenderDlg::Init()
{
}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditMesh);
	DDX_Control(pDX, IDC_EDIT2, m_EditMaterial);
	DDX_Control(pDX, IDC_EDIT4, m_Blend);
}

BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnBnClickedMesh)
	ON_BN_CLICKED(IDC_BUTTON5, &CMeshRenderDlg::OnBnClickedMaterial)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshRenderDlg::OnBnAddTexture)
	ON_BN_CLICKED(IDC_BUTTON9, &CMeshRenderDlg::OnBnClickedBlend)
END_MESSAGE_MAP()

/*wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"", L"",
		OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = L"D:\\TransistorExport(split)";

	if (dlg.DoModal() != IDOK)
		return;

	CString strPath = dlg.GetFolderPath();
	dlg.SetEditBoxText();
	strPath;*/

void CMeshRenderDlg::OnBnClickedMesh()
{
	CDlgList List;

	List.SetCaption(L"Mesh");

	map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::MESH);

	for (auto pair : mapRes)
	{
		List.AddData(pair.first.c_str());
	}

	UINT id = List.DoModal();


	if (!id)
	{
		wstring wcsSelect = List.GetSelect();
		if(wcsSelect != L"")
		m_pTargetObj->GetMeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(wcsSelect));
	}
}

void CMeshRenderDlg::OnBnClickedMaterial()
{
	CDlgList List;

	List.SetCaption(L"Material");

	map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResource(RESOURCE_TYPE::MATERIAL);

	for (auto pair : mapRes)
	{
		List.AddData(pair.first.c_str());
	}

	UINT id = List.DoModal();

	if (!id)
	{
		wstring wcsSelect = List.GetSelect();
		if (wcsSelect != L"")
		m_pTargetObj->GetMeshRender()->SetMtrlClone(CResMgr::GetInst()->Load<CMaterial>(wcsSelect));
	}
}

void CMeshRenderDlg::OnBnAddTexture()
{
	wstring strPath = CPathMgr::GetInst()->GetPath();

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"png", L"*.png",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	wstring wcsPath = dlg.GetPathName();
	wcsPath = CPathMgr::GetInst()->AbsoluteToRelativeResPath(wcsPath);

	wstring wcsKey = dlg.GetFileName();

	CResMgr::GetInst()->AddResource<CTexture>(wcsKey, CTexture::CreateTexture(wcsPath.c_str()));
	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(wcsKey);
	m_pTargetObj->GetTransForm()->SetScale(Vec3(pTex->GetWidth(), pTex->GetHeight(),1.f));
	m_pTargetObj->GetMeshRender()->SetTexture(pTex);
}

void CMeshRenderDlg::OnBnClickedBlend()
{
	CDlgList List;

	List.SetCaption(L"Blend");

	for (UINT i = 0; i < (UINT)BLEND_TYPE::END; ++i)
	{
		wstring strName = ((CBlendState*)CRenderMgr::GetInst()->GetBlend((BLEND_TYPE)i))->GetName();
		List.AddData(strName.c_str());
	}

	UINT id = List.DoModal();


	if (!id)
	{
		wstring wcsSelect = List.GetSelect();

		if (wcsSelect != L"")
			m_pTargetObj->GetMeshRender()->GetMtrl()->SetBlend(CRenderMgr::GetInst()->GetBlend(wcsSelect));
	}
}