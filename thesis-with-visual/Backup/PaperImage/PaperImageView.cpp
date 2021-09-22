
// PaperImageView.cpp: CPaperImageView 클래스의 구현
//
#pragma region Includes
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "PaperImage.h"
#endif

#include "MainFrm.h"
#include "PaperImageDoc.h"
#include "PaperImageView.h"

#include "CMPAN.H"

#include "Util.h"
#include "CQArea.h"
#include "CQObject.h"

#include "CQDialog.h"
#include "SimulDlg.h"
#include "RuleDlg.h"
#include "ObjListDlg.h"
#include "SimuOutDlg.h"
#include "TPRTree.h"
#include <sstream>
#pragma endregion

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPaperImageView

IMPLEMENT_DYNCREATE(CPaperImageView, CView)

BEGIN_MESSAGE_MAP(CPaperImageView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_C, &CPaperImageView::OnButtonC)
	ON_MESSAGE(WM_SIMUL, &CPaperImageView::OnSimul)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//==================================================================================================

#pragma region Create-Destructor
// CPaperImageView 생성/소멸
CPaperImageView::CPaperImageView() noexcept
{
//	m_nSimulIndex = -1;

	m_dRate = 0.0;
	m_dMap_Meter = 0.0;
	m_dCell_Meter = 0.0;

	m_pCQDlg = NULL;
	m_pSimulDlg = NULL;
	m_pRuleDlg = NULL;
	m_objListDlg = NULL;

	m_pDC = NULL;
	m_pBitmap = NULL;

	m_bShowGrid = TRUE;	// 화면에 Grid 보여줄지 말지
	m_bShowDot = FALSE;	// 화면에 경로점을 보여줄지 말지
}

CPaperImageView::~CPaperImageView()
{
	if (m_pDC || m_pBitmap)	ReleaseResources();

	m_pCQDlg->DestroyWindow();
	delete m_pCQDlg;

	m_pSimulDlg->DestroyWindow();
	delete m_pSimulDlg;

	m_pRuleDlg->DestroyWindow();
	delete m_pRuleDlg;

	m_objListDlg->DestroyWindow();
	delete m_objListDlg;

	delete myTPR;
}

void CPaperImageView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pCQDlg = new CCQDialog(this);
	m_pCQDlg->Create(IDD_CCQDIALOG);

	m_pSimulDlg = new CSimulDlg(this);
	m_pSimulDlg->Create(IDD_SIMULDLG);
	
	m_pRuleDlg = new RuleDlg(this);
	m_pRuleDlg->Create(IDD_RULEDLG);
	m_pRuleDlg->ShowWindow(SW_SHOW);

	m_objListDlg = new ObjListDlg(this);
	m_objListDlg->Create(IDD_OBJLISTDIALOG);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	m_simuOutDlg = &pMainWnd->m_DockPane.dialog_box;
	myTPR = new TPRTree();
}

#pragma endregion

//==================================================================================================

#pragma region _DEBUG
// CPaperImageView 진단
#ifdef _DEBUG
void CPaperImageView::AssertValid() const
{
	CView::AssertValid();
}

void CPaperImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPaperImageDoc* CPaperImageView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPaperImageDoc)));
	return (CPaperImageDoc*)m_pDocument;
}
#endif //_DEBUG
#pragma endregion

//==================================================================================================

#pragma region User-Defined-Functions
//==================================================================================================

void CPaperImageView::ReleaseResources()
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

void CPaperImageView::AllocResources(CDC* pDC, int _w, int _h)
{
	m_pDC = new CDC;
	m_pDC->CreateCompatibleDC(pDC);
	m_pBitmap = new CBitmap;
	m_pBitmap->CreateCompatibleBitmap(pDC, _w, _h);
	m_pDC->SelectObject(m_pBitmap);
}

void CPaperImageView::initScale()
{
	// 최초 화면 가로 거리를 2000km로 설정 ==> 늘리거나 줄이고 싶으면 이 값을 변경하면 됨.
	/*if (m_pCQDlg->m_CQTYPE == _SONAR_ || m_pCQDlg->m_CQTYPE == _SONAR4_)
	{
		m_dMap_Meter = SONAR_MAP_METER;
		m_dCell_Meter = SONAR_CELL_METER;
	}
	else
	{*/
		m_dMap_Meter = MAP_METER;
		m_dCell_Meter = CELL_METER;
	//}
	double temp = M_RECT_W;
	m_dRate = m_dMap_Meter / M_RECT_W;
	m_centerPt.X = (REAL)(m_dMap_Meter / 2.0f);
	m_centerPt.Y = (REAL)(m_dMap_Meter / 2.0f);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString str;
	str.Format(_T("R:%.4f"), m_dRate);
	pFrame->m_wndStatusBar.SetPaneText(4, str);
}

// 현재 화면의 지도좌표를 리턴한다.
RectF CPaperImageView::getMapRect()
{
	RectF ret;
	GetClientRect(m_rect);
	PointF p1 = Scr2Map(0, 0);
	PointF p2 = Scr2Map(M_RECT_W, M_RECT_H);

	ret.X = p1.X;
	ret.Y = p1.Y;
	ret.Width = p2.X - p1.X;
	ret.Height = p2.Y - p1.Y;

	return ret;
}

POINT CPaperImageView::Map2Scr(REAL x, REAL y)
{
	REAL left = m_centerPt.X - (REAL)((M_RECT_W /2.0f * m_dRate)) - 500;
	REAL top = m_centerPt.Y - (REAL)((M_RECT_H /2.0f * m_dRate)) - 500;

	REAL dx = x - left;
	REAL dy = y - top;

	POINT pt;
	pt.x = (int)(dx / m_dRate);
	pt.y = (int)(dy / m_dRate);
	return pt;
}

int CPaperImageView::Map2Scr(REAL d)
{
	return (int)(d / m_dRate);
}

PointF CPaperImageView::Scr2Map(int x, int y)
{
	REAL left = m_centerPt.X - (REAL)((M_RECT_W / 1.65f * m_dRate));
	REAL top = m_centerPt.Y - (REAL)((M_RECT_H / 1.75f * m_dRate));

	PointF pt;
	pt.X = (REAL)(x * m_dRate) + left;
	pt.Y = (REAL)(y * m_dRate) + top;
	return pt;
}

REAL CPaperImageView::Scr2Map(int d)
{
	return (REAL)((double)d * m_dRate);
}

void CPaperImageView::UpdateStatusBar()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// 상태바에 변경된 스케일 표시
	CString str;
	str.Format(_T("R:%.4f"), m_dRate);
	pFrame->m_wndStatusBar.SetPaneText(4, str);

	// 상태바에 변경된 중심좌표 표시
	str.Format(_T("CM:%.2f,%.2f"), m_centerPt.X, m_centerPt.Y);
	pFrame->m_wndStatusBar.SetPaneText(3, str);
}

#pragma endregion

//==================================================================================================

#pragma region Draw-Functions
//==================================================================================================
//
// CPaperImageView 그리기

void CPaperImageView::DrawGrid(Graphics& g)
{
	POINT p1, p2;
	int cnt = (int)(m_dMap_Meter/m_dCell_Meter)+1;
	Pen pen(Color(225, 225, 225), 1.0f);
	for (int i = 0; i < cnt; i++)
	{
		REAL t = (REAL)(i * m_dCell_Meter);

		// 가로줄
		p1 = Map2Scr(0.0f, t);
		p2 = Map2Scr((REAL)m_dMap_Meter, t);
		g.DrawLine(&pen, p1.x, p1.y, p2.x, p2.y);

		// 세로줄
		p1 = Map2Scr(t, 0.0f);
		p2 = Map2Scr(t, (REAL)m_dMap_Meter);
		g.DrawLine(&pen, p1.x, p1.y, p2.x, p2.y);
	}
}

void CPaperImageView::updateQueue()
{
	m_simuOutDlg->SendMessage(WM_UPDATE_EVENT);
}

void CPaperImageView::OnDraw(CDC* pDC)
{
	CPaperImageDoc* pDoc = GetDocument();

	GetClientRect(m_rect);

	if (!m_pDC)
	{
		initScale();
		AllocResources(pDC, M_RECT_W +1, M_RECT_H +1);
	}

	m_pDC->FillSolidRect(0, 0, m_rect.Width()+1, m_rect.Height()+1, RGB(255, 255, 255));

	Graphics g(m_pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	if (m_bShowGrid)	DrawGrid(g);

	int size = (int)pDoc->m_CQObjects.size();

	BOOL hasCollide = false;
	//Check for naive collision
	for (int i = 0; i < size; i++) {
		stringstream output_stream;
		if (pDoc->m_CQObjects[i]->isCollide(pDoc->m_CQAreas[0], this)) {
			output_stream << "NAIVE Object #" << to_string(i) << ", at " << to_string(currentT) << endl;
			queue_ev.push(output_stream.str());
			hasCollide = true;
		}
	}
	//check for TPR collision
	vector<CEntry> queryResult;
	CCQArea* ourVessel = pDoc->m_CQAreas[0];
	myTPR->rangeQueryKNN4(ourVessel->m_fCurrentPt.X, ourVessel->m_fCurrentPt.Y, 0.0, (double)ourVessel->m_fRadius[0], queryResult, currentT);
	if (!queryResult.empty()) {
		for (int i = 0; i < queryResult.size(); i++) {
			stringstream output_stream;
			output_stream << "TPR Object #" << to_string(queryResult[i].m_id) << ", at " << to_string(currentT) << endl;
			queue_ev.push(output_stream.str());
			hasCollide = true;
		}
	}
	//------------------------------------------------------------------------
	// Draw Object

	{
		PointF p1 = Scr2Map(m_rect.left, m_rect.top);
		PointF p2 = Scr2Map(m_rect.right, m_rect.bottom);
		RectF tr(p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);


		int cnt1 = 0;
		int cnt2 = 0;
		for (int i = 0; i < size; i++)
		{

			//// 그리기 대상 레이어가 아니면
			//if (!m_pCQDlg->isDrawObject(pDoc->m_CQObjects[i])) continue;

			//// 객체가 화면안에 완전히 들어오지 않으면
			//if (!tr.Contains(pDoc->m_CQObjects[i]->m_mbr))
			//{
			//	// 화면 MBR에서 벗어나면
			//	if (!tr.IntersectsWith(pDoc->m_CQObjects[i]->m_mbr)) continue;

			//	// 화면 MBR과 경로가 교차하지 않으면
			//	if (!CUtil::intersect(tr, pDoc->m_CQObjects[i]->getDrawPath()))	continue;
			//}
			pDoc->m_CQObjects[i]->draw(g, this);
		}
	}
	//------------------------------------------------------------------------
	// Draw CQArea
	if (currentT <= SIMU_TIME)
	{
		int size = (int)pDoc->m_CQAreas.size();
		for (int i = 0; i < size; i++)
		{
			pDoc->m_CQAreas[i]->draw(g, this);
			pDoc->m_CQAreas[i]->moveOurVessel();
		}
	pDC->BitBlt(0, 0, m_rect.Width()+1, m_rect.Height()+1, m_pDC, 0, 0, SRCCOPY);
	if (hasCollide)
		m_simuOutDlg->SendMessage(WM_UPDATE_EVENT);
	Sleep(150);
	currentT++;
	}
}
#pragma endregion

//==================================================================================================

#pragma region Message-Functions
//
// CPaperImageView 메시지 처리기
//
void CPaperImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 좌표를 상태바에 출력
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString str;
	str.Format(_T("S:%d,%d"), point.x, point.y);
	pFrame->m_wndStatusBar.SetPaneText(1, str);

	PointF mp = Scr2Map(point.x, point.y);
	str.Format(_T("M:%.2f,%.2f"), mp.X, mp.Y);
	pFrame->m_wndStatusBar.SetPaneText(2, str);

	str.Format(_T("CM:%.2f,%.2f"), m_centerPt.X, m_centerPt.Y);
	pFrame->m_wndStatusBar.SetPaneText(3, str);

	str.Format(_T("C:%d,%d"), (int)(mp.X/ m_dCell_Meter), (int)(mp.Y/ m_dCell_Meter));
	pFrame->m_wndStatusBar.SetPaneText(5, str);

	CView::OnMouseMove(nFlags, point);
}

//// pt를 기준으로 확대 축소
//BOOL CPaperImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	ScreenToClient(&pt);
//
//	POINT cp = Map2Scr(m_centerPt.X, m_centerPt.Y);	// 중심 지도좌표를 화면 좌표로 변환
//	int dx = pt.x - cp.x;	// 마우스 위치와 중심점과의 x좌표 거리
//	int dy = pt.y - cp.y;	// 마우스 위치와 중심점과의 y좌표 거리
//	PointF mpt = Scr2Map(pt.x, pt.y);				// 마우스 위치를 지도좌표로 변환
//
//	// 스케일 변경
//	if (0 < zDelta)		m_dRate /= 1.2;
//	else				m_dRate *= 1.2;
//
//	// 화면 중심 좌표 변경
//	REAL mx = Scr2Map(dx);
//	REAL my = Scr2Map(dy);
//
//	m_centerPt.X = mpt.X - mx;
//	m_centerPt.Y = mpt.Y - my;
//
//	UpdateStatusBar();
//
//	Invalidate(FALSE);
//
//	return CView::OnMouseWheel(nFlags, zDelta, pt);
//}

void CPaperImageView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CView::OnRButtonDown(nFlags, point);

	CCMPan* pTool = (CCMPan*)CCMTools::FindTool(PANTOOL);
	pTool->OnRButtonDown(this, nFlags, point);
}

afx_msg LRESULT CPaperImageView::OnSimul(WPARAM wParam, LPARAM lParam)
{
	Invalidate(FALSE);
	return 0;
}

#pragma endregion

//==================================================================================================

#pragma region Toolbar-Command-Functions
//==================================================================================================

//void CPaperImageView::OnButtonPathdot()							{	m_bShowDot = !m_bShowDot;	Invalidate(FALSE);}
//void CPaperImageView::OnUpdateButtonPathdot(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(m_bShowDot);	}
//void CPaperImageView::OnButtonGrid()							{	m_bShowGrid = !m_bShowGrid;	Invalidate(FALSE);}
//void CPaperImageView::OnUpdateButtonGrid(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(m_bShowGrid);	}

void CPaperImageView::OnButtonC()
{
	m_pCQDlg->ShowWindow(SW_SHOW);
}

//void CPaperImageView::SetCQType(int cqTYPE)
//{
//	m_pCQDlg->m_CQTYPE = cqTYPE;
//}

void CPaperImageView::startSimulation()
{
	CPaperImageDoc* pDoc = GetDocument();
	m_pSimulDlg->OnBnClickedPlay();
	
	m_objListDlg->ShowWindow(SW_SHOW);
	m_objListDlg->initObjects();
	for (int j = 0; j < pDoc->m_CQObjects.size(); j++) {

		CEntry* newEnt = new CEntry(pDoc->m_CQObjects[j]->m_id, 0, pDoc->m_CQObjects[j]->m_initPoint.X, pDoc->m_CQObjects[j]->m_initPoint.Y,
			0.0, pDoc->m_CQObjects[j]->m_vx, pDoc->m_CQObjects[j]->m_vy, 0.0);
		myTPR->Insert(*newEnt);
	}
	currentT = 0;
}
#pragma endregion

void CPaperImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int x = point.x; int y = point.y;
	PointF resPoint = Scr2Map(x, y);
	CString t;

	t.Format(_T("ori= %d, %d\n map= %f, %f"), x, y, resPoint.X, resPoint.Y);

	MessageBox(t, L"Debug", MB_OK);
	CView::OnLButtonDown(nFlags, point);
}
