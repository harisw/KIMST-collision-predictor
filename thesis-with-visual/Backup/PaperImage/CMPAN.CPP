#include "pch.h"
#include "Cmpan.h"
#include "PaperImageView.h"
/////////////////////////////////////////////////////////////////////////////
// CCMPan implementation

CCMPan::CCMPan(ToolMode nMode):CCMTools(nMode)
{
	m_pDC = NULL;
	m_pBitmap = NULL;
}

CCMPan::~CCMPan()
{
	ReleaseResources();
	CCMTools::~CCMTools();
}
/*
void CCMPan::OnRButtonDown(CWnd* pWnd, UINT nFlags, const CPoint& point)
{
	CPaperImageView* pView = (CPaperImageView*)pWnd;

	m_down = point;

	pView->SetCapture();		// 마우스 Capture 모드로 설정

	int dx, dy;
	CPoint down;
	BOOL bWork = TRUE;
	while(bWork)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		HWND hWnd = ::GetCapture();
		if (hWnd != pView->m_hWnd)
		{
			pView->Invalidate();
			break;
		}
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			{
				down.x = (int)(short)LOWORD(msg.lParam);
				down.y = (int)(short)HIWORD(msg.lParam);

				dx = down.x - m_down.x;
				dy = down.y - m_down.y;

				if (dx != 0 || dy != 0)	pView->SendMessage(WM_PANNING, (WPARAM)dx, (LPARAM)dy);

				m_down = down;
			}
			break;
		case WM_RBUTTONUP:
			{
				down.x = (int)(short)LOWORD(msg.lParam);
				down.y = (int)(short)HIWORD(msg.lParam);

				dx = down.x - m_down.x;
				dy = down.y - m_down.y;

				if (dx != 0 || dy != 0)	pView->SendMessage(WM_PANNING, (WPARAM)dx, (LPARAM)dy);

				m_down = down;

				bWork = FALSE;
			}
			break;
		default:
			::DispatchMessage(&msg);
			break;
		}
	}
	ReleaseCapture();
	m_down = CPoint(-1, -1);
}
*/

void CCMPan::ReleaseResources()
{
	if (m_pDC)
	{
		m_pDC->DeleteDC();
		delete m_pDC;
	}
	if (m_pBitmap)
	{
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
	}
	m_pDC = NULL;
	m_pBitmap = NULL;
}

void CCMPan::AllocResources(CWnd* pWnd, int _w, int _h)
{
	CDC* pDC = pWnd->GetDC();

	m_pDC = new CDC;
	m_pDC->CreateCompatibleDC(pDC);
	m_pBitmap = new CBitmap;
	m_pBitmap->CreateCompatibleBitmap(pDC, _w, _h);
	m_pDC->SelectObject(m_pBitmap);

	pWnd->ReleaseDC(pDC);
}

void CCMPan::draw(CWnd* pWnd, int dx, int dy)
{
	CPaperImageView* pView = (CPaperImageView*)pWnd;

	int w = pView->m_rect.Width();
	int h = pView->m_rect.Height();

	CDC* pDC = pView->GetDC();
	m_pDC->FillSolidRect(0, 0, w, h, RGB(255, 255, 255));
	m_pDC->BitBlt(0, 0, w, h, pView->m_pDC, dx, dy, SRCCOPY);
	pDC->BitBlt(0, 0, w, h, m_pDC, 0, 0, SRCCOPY);
	pView->ReleaseDC(pDC);
}

void CCMPan::OnRButtonDown(CWnd* pWnd, UINT nFlags, const CPoint& point)
{
	CPaperImageView* pView = (CPaperImageView*)pWnd;

	m_down = point;

	pView->SetCapture();		// 마우스 Capture 모드로 설정
	AllocResources(pWnd, pView->m_rect.Width(), pView->m_rect.Height());

	CPoint move;
	BOOL bWork = TRUE;
	while (bWork)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));
		HWND hWnd = ::GetCapture();
		if (hWnd != pView->m_hWnd)
		{
			pView->Invalidate(FALSE);
			break;
		}
		if (msg.message == WM_MOUSEMOVE || msg.message == WM_RBUTTONUP)
		{
			move.x = (int)(short)LOWORD(msg.lParam);
			move.y = (int)(short)HIWORD(msg.lParam);

			int dx = m_down.x - move.x;
			int dy = m_down.y - move.y;

			if (dx != 0 || dy != 0)	draw(pWnd, dx, dy);

			if (msg.message == WM_RBUTTONUP)
			{
				bWork = FALSE;

				if (dx != 0 || dy != 0)
				{
					pView->m_centerPt.X += pView->Scr2Map(dx);
					pView->m_centerPt.Y += pView->Scr2Map(dy);
					pView->Invalidate(FALSE);
				}
			}
		}
		else
		{
			::DispatchMessage(&msg);
		}
	}
	ReleaseResources();
	ReleaseCapture();
}
