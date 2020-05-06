#pragma once

#include "CDockView.h"

class CComponentDlg;
class CAddComponentDlg;
class CManageDlg;
class CLayerDlg;

enum class COMDLG_TYPE
{
	TRANSFORM,
	MESH_RENDER,
	CAMERA,
	COLLIDER,
	ANIMATOR_2D,
	GRID,
	INCIDENT,
	TILE,
	SCRIPT,
	END,
};

class CComView : public CDockView
{
	DECLARE_DYNAMIC(CComView)
	
private:
	CComponentDlg*   m_arrComDlg[(UINT)COMDLG_TYPE::END];
	CAddComponentDlg* m_pAddCom;
	CManageDlg*      m_Scroll;
	CLayerDlg*       m_Layer;


public:
	CComView();
	virtual ~CComView();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void Init();
	void Update();
	void DialogSort();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};