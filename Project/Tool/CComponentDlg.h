#pragma once

class CGameObj;
class CComponentDlg :
	public CDialogEx
{

protected:
	CGameObj*   m_pTargetObj;
	bool		m_bFocus;
	bool		m_bClick; // ������ update �߰��� change�Լ��� ȣ��ȴ�.
	bool	  m_cbFocus;

public:
	virtual void Update(CGameObj* _pObj) = 0;
	virtual void Init(){}

protected:
	virtual void OnCancel() {}
	virtual void OnOK() {}
	virtual void PostNcDestroy();

public:
	afx_msg void OnEditSetfocus(UINT _iId);
	afx_msg void OnEditKillfocus(UINT _iId);

	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbnSetfocus();

public:
	CComponentDlg(UINT _id, CWnd* _pParent);
	~CComponentDlg();
};