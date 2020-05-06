#include "stdafx.h"
#include "CDockView.h"

CDockView::CDockView()
: m_pTargetObj(nullptr)

{
}

CDockView::~CDockView()
{
}

void CDockView::PostNcDestroy()
{
	CDockablePane::PostNcDestroy();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}BEGIN_MESSAGE_MAP(CDockView, CDockablePane)
ON_WM_PAINT()
ON_WM_SIZE()
END_MESSAGE_MAP()


void CDockView::OnPaint()
{
	CPaintDC dc(this);

	CRect rt;
	GetWindowRect(&rt);
	rt.left = 0.f;
	rt.top = 0.f;
	dc.FillSolidRect(&rt, RGB(240, 240, 240));
}


void CDockView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	Invalidate();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
