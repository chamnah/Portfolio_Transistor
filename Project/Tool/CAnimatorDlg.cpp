﻿// CAnimaotrDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CAnimatorDlg.h"
#include "afxdialogex.h"
#include "CPathMgr.h"
#include "CGameObj.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

// CAnimaotrDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimatorDlg, CDialogEx)

CAnimatorDlg::CAnimatorDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_ANIMATOR, pParent)
{

}

CAnimatorDlg::~CAnimatorDlg()
{
}

void CAnimatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditName);
}

void CAnimatorDlg::Update(CGameObj * _pObj)
{
	m_pTargetObj = _pObj;
}

BEGIN_MESSAGE_MAP(CAnimatorDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimatorDlg::OnBnClickedLoad)
END_MESSAGE_MAP()

void CAnimatorDlg::OnBnClickedLoad()
{

	wstring strPath = CPathMgr::GetInst()->GetPath();

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"anim", L"*.anim",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	m_EditName.SetWindowTextW(dlg.GetFileName());

	strPath = dlg.GetPathName();

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	int iClipSize = 0;
	fread(&iClipSize, sizeof(int), 1, pFile);

	int iNameLength = 0;
	int iPathLength = 0;

	wstring wcsPath;
	wstring wcsName;

	Vec2 vOffset = {};
	float fDuration = 0.f;
	int iFrmSize = 0;

	m_pTargetObj->GetAnimator2D()->ClearAnimation();
	m_pTargetObj->GetAnimator2D()->SetPath(strPath);

	for (int i = 0; i < iClipSize; ++i)
	{
		wchar_t wcsName[MAX_PATH] = L"";
		wchar_t wcsPath[MAX_PATH] = L"";

		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(&wcsName, sizeof(wchar_t), iNameLength, pFile);

		fread(&iPathLength, sizeof(int), 1, pFile);
		fread(&wcsPath, sizeof(wchar_t), iPathLength, pFile);

		fread(&iFrmSize, sizeof(int), 1, pFile);
		
		CAnimation2D* pAnim = m_pTargetObj->GetAnimator2D()->AddAnimation(wcsPath, wcsName);
		vector<tAnim>& vecFrm = pAnim->GetAllFrame();

		for (UINT i = 0; i < iFrmSize; ++i)
		{
			fread(&vOffset.x, sizeof(float), 1, pFile);
			fread(&vOffset.y, sizeof(float), 1, pFile);

			fread(&fDuration, sizeof(float), 1, pFile);

			vecFrm[i].vOffset = vOffset;
			vecFrm[i].fDuration = fDuration;
		}
	}

	fclose(pFile);

	m_pTargetObj->GetAnimator2D()->IsPlay(true);
	m_pTargetObj->GetAnimator2D()->ChangeAnimation(nullptr);
}