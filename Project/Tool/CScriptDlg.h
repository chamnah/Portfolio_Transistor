#pragma once

#include "CComponentDlg.h"

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScriptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void Init();
	void Update(CGameObj* _pObj);
	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lbScript;
	CComboBox m_cbScript;
public:
	afx_msg void OnBnAddScript();
};
