#include "stdafx.h"
#include "CComponentDlg.h"

CComponentDlg::CComponentDlg(UINT _id, CWnd * _pParent)
	:CDialogEx(_id, _pParent)
	, m_pTargetObj(nullptr)
	, m_bFocus(false)
	, m_bClick(false)
	, m_cbFocus(false)
{
}

CComponentDlg::~CComponentDlg()
{
}

void CComponentDlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;

	CDialogEx::PostNcDestroy();
}

void CComponentDlg::OnEditSetfocus(UINT _iId)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bFocus = true;
}


void CComponentDlg::OnEditKillfocus(UINT _iId)
{
	m_bFocus = false;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CComponentDlg::OnCbnKillfocus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_cbFocus = false;
}


void CComponentDlg::OnCbnSetfocus()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_cbFocus = true;
}