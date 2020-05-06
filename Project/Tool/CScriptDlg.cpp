// CScriptDlg.cpp: 구현 파일
#include "stdafx.h"
#include "Tool.h"
#include "CScriptDlg.h"
#include "afxdialogex.h"
#include "CScriptMgr.h"
#include "CGameObj.h"
#include "CScript.h"


// CScriptDlg 대화 상자

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_SCRIPT, pParent)
{

}

CScriptDlg::~CScriptDlg()
{
}

void CScriptDlg::Update(CGameObj* _pObj)
{
		m_pTargetObj = _pObj;

		if (m_pTargetObj != nullptr)
		{
			m_lbScript.ResetContent();
			const vector<CScript*>& vecScript = m_pTargetObj->GetScript();

			for (UINT i = 0; i < vecScript.size(); ++i)
			{
				wstring strName = CScriptMgr::GetScriptName(vecScript[i]);
				m_lbScript.AddString(strName.c_str());
			}
		}
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lbScript);
	DDX_Control(pDX, IDC_COMBO1, m_cbScript);
}

void CScriptDlg::Init()
{
	vector<wstring> vecScript;
	CScriptMgr::GetScriptInfo(vecScript);

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		m_cbScript.InsertString(i,vecScript[i].c_str());
	}

}

BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDlg::OnBnAddScript)
END_MESSAGE_MAP()

void CScriptDlg::OnBnAddScript()
{


	if (m_pTargetObj == nullptr)
		return;

	if (m_pTargetObj->GetDead())
		m_pTargetObj = nullptr;

	int iSel = m_cbScript.GetCurSel();
	if (iSel == LB_ERR)
		return;
	
	const vector<CScript*>& vecScript = m_pTargetObj->GetScript();

	CString Script;
	m_cbScript.GetLBText(iSel, Script);

	CScript* pCurScript = CScriptMgr::GetScript(Script.GetBuffer());

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		if (vecScript[i]->GetScriptType() == pCurScript->GetScriptType())
		{
			delete pCurScript;
			return;
		}
	}

	m_pTargetObj->AddComponent(pCurScript);
}