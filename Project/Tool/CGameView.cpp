// CGameView.cpp: 구현 파일
//

#include "stdafx.h"
#include "Tool.h"
#include "CGameView.h"
#include "CTestMgr.h"
#include "MainFrm.h"
#include "CComponentView.h"
#include "CGameObj.h"
#include "CToolScript.h"

// CGameView
IMPLEMENT_DYNCREATE(CGameView, CFormView)

CGameView::CGameView()
	:CFormView(IDD_GAME)
{

}

CGameView::~CGameView()
{
}

BEGIN_MESSAGE_MAP(CGameView, CFormView)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CGameView 그리기
void CGameView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}
// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CGameView 메시지 처리기

void CGameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
	CGameObj* pTarget = ((CToolScript*)CTestMgr::GetInst()->GetMouseObj()->GetScript()[0])->GetTargetObj();

	if (pTarget == nullptr)
		return;

	((CMainFrame*)AfxGetMainWnd())->GetView(VIEW_TYPE::COMPONENT)->SetTargetObj(pTarget);

	CFormView::OnLButtonDown(nFlags, point);
}