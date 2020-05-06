#pragma once

#include "CComponentDlg.h"

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshRenderDlg();

public:
	void Update(CGameObj* _pObj);
	void Init();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditMesh;
	CEdit m_EditMaterial;
	afx_msg void OnBnClickedMesh();
	afx_msg void OnBnClickedMaterial();
	afx_msg void OnBnAddTexture();
	afx_msg void OnBnClickedBlend();
private:
	CEdit m_Blend;
};