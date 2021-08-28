
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
	ON_COMMAND(ID_BUTTON_PATHDOT, &CPaperImageView::OnButtonPathdot)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PATHDOT, &CPaperImageView::OnUpdateButtonPathdot)
	ON_COMMAND(ID_BUTTON_GRID, &CPaperImageView::OnButtonGrid)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_GRID, &CPaperImageView::OnUpdateButtonGrid)
	ON_COMMAND(ID_BUTTON_INIT, &CPaperImageView::OnButtonInit)
	ON_COMMAND(ID_BUTTON_C1, &CPaperImageView::OnButtonC1)
	ON_COMMAND(ID_BUTTON_C2, &CPaperImageView::OnButtonC2)
	ON_COMMAND(ID_BUTTON_C3, &CPaperImageView::OnButtonC3)
	ON_COMMAND(ID_BUTTON_C4, &CPaperImageView::OnButtonC4)
	ON_COMMAND(ID_BUTTON_C5, &CPaperImageView::OnButtonC5)
	ON_COMMAND(ID_BUTTON_A1, &CPaperImageView::OnButtonA1)
	ON_COMMAND(ID_BUTTON_A2, &CPaperImageView::OnButtonA2)
	ON_COMMAND(ID_BUTTON_A3, &CPaperImageView::OnButtonA3)
	ON_COMMAND(ID_BUTTON_A4, &CPaperImageView::OnButtonA4)
	ON_COMMAND(ID_BUTTON_A5, &CPaperImageView::OnButtonA5)
	ON_COMMAND(ID_BUTTON_R1, &CPaperImageView::OnButtonR1)
	ON_COMMAND(ID_BUTTON_R2, &CPaperImageView::OnButtonR2)
	ON_COMMAND(ID_BUTTON_R3, &CPaperImageView::OnButtonR3)
	ON_COMMAND(ID_BUTTON_R4, &CPaperImageView::OnButtonR4)
	ON_COMMAND(ID_BUTTON_R5, &CPaperImageView::OnButtonR5)
	ON_COMMAND(ID_BUTTON_C, &CPaperImageView::OnButtonC)
	ON_COMMAND(ID_BUTTON_S, &CPaperImageView::OnButtonS)
	ON_MESSAGE(WM_SIMUL, &CPaperImageView::OnSimul)
	ON_COMMAND(ID_BUTTON_ZE, &CPaperImageView::OnButtonZe)
	ON_COMMAND(ID_32803, &CPaperImageView::On32803)
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
}

void CPaperImageView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pCQDlg = new CCQDialog(this);
	m_pCQDlg->Create(IDD_CCQDIALOG);

	m_pSimulDlg = new CSimulDlg(this);
	m_pSimulDlg->Create(IDD_SIMULDLG);
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
	if (m_pCQDlg->m_CQTYPE == _SONAR_ || m_pCQDlg->m_CQTYPE == _SONAR4_)
	{
		m_dMap_Meter = SONAR_MAP_METER;
		m_dCell_Meter = SONAR_CELL_METER;
	}
	else
	{
		m_dMap_Meter = RADAR_MAP_METER;
		m_dCell_Meter = RADAR_CELL_METER;
	}

	m_dRate = m_dMap_Meter / (double)m_rect.Width();
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
	PointF p2 = Scr2Map(m_rect.Width(), m_rect.Height());

	ret.X = p1.X;
	ret.Y = p1.Y;
	ret.Width = p2.X - p1.X;
	ret.Height = p2.Y - p1.Y;

	return ret;
}

POINT CPaperImageView::Map2Scr(REAL x, REAL y)
{
	REAL left = m_centerPt.X - (REAL)((m_rect.Width()/2.0f * m_dRate));
	REAL top = m_centerPt.Y - (REAL)((m_rect.Height()/2.0f * m_dRate));

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
	REAL left = m_centerPt.X - (REAL)((m_rect.Width() / 2.0f * m_dRate));
	REAL top = m_centerPt.Y - (REAL)((m_rect.Height() / 2.0f * m_dRate));

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

void CPaperImageView::OnDraw(CDC* pDC)
{
	CPaperImageDoc* pDoc = GetDocument();

	GetClientRect(m_rect);

	if (!m_pDC)
	{
		initScale();
		AllocResources(pDC, m_rect.Width()+1, m_rect.Height()+1);
	}

	m_pDC->FillSolidRect(0, 0, m_rect.Width()+1, m_rect.Height()+1, RGB(255, 255, 255));

	Graphics g(m_pDC->m_hDC);
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	if (m_bShowGrid)	DrawGrid(g);

	//------------------------------------------------------------------------
	// Draw Object
	{
		PointF p1 = Scr2Map(m_rect.left, m_rect.top);
		PointF p2 = Scr2Map(m_rect.right, m_rect.bottom);
		RectF tr(p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);

		int size = (int)pDoc->m_CQObjects.size();

		int cnt1 = 0;
		int cnt2 = 0;
		for (int i = 0; i < size; i++)
		{
			// 그리기 대상 레이어가 아니면
			if (!m_pCQDlg->isDrawObject(pDoc->m_CQObjects[i])) continue;

			// 객체가 화면안에 완전히 들어오지 않으면
			if (!tr.Contains(pDoc->m_CQObjects[i]->m_mbr))
			{
				// 화면 MBR에서 벗어나면
				if (!tr.IntersectsWith(pDoc->m_CQObjects[i]->m_mbr)) continue;
			
				// 화면 MBR과 경로가 교차하지 않으면
				if (!CUtil::intersect(tr, pDoc->m_CQObjects[i]->getDrawPath()))	continue;
			}
			pDoc->m_CQObjects[i]->draw(g, this);
		}
	}
	//------------------------------------------------------------------------
	// Draw CQArea
	{
		int size = (int)pDoc->m_CQAreas.size();
		for (int i = 0; i < size; i++)
		{
			pDoc->m_CQAreas[i]->draw(g, this);
		}
	}

	pDC->BitBlt(0, 0, m_rect.Width()+1, m_rect.Height()+1, m_pDC, 0, 0, SRCCOPY);
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

// pt를 기준으로 확대 축소
BOOL CPaperImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	ScreenToClient(&pt);

	POINT cp = Map2Scr(m_centerPt.X, m_centerPt.Y);	// 중심 지도좌표를 화면 좌표로 변환
	int dx = pt.x - cp.x;	// 마우스 위치와 중심점과의 x좌표 거리
	int dy = pt.y - cp.y;	// 마우스 위치와 중심점과의 y좌표 거리
	PointF mpt = Scr2Map(pt.x, pt.y);				// 마우스 위치를 지도좌표로 변환

	// 스케일 변경
	if (0 < zDelta)		m_dRate /= 1.2;
	else				m_dRate *= 1.2;

	// 화면 중심 좌표 변경
	REAL mx = Scr2Map(dx);
	REAL my = Scr2Map(dy);

	m_centerPt.X = mpt.X - mx;
	m_centerPt.Y = mpt.Y - my;

	UpdateStatusBar();

	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

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

void CPaperImageView::OnButtonPathdot()							{	m_bShowDot = !m_bShowDot;	Invalidate(FALSE);}
void CPaperImageView::OnUpdateButtonPathdot(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(m_bShowDot);	}
void CPaperImageView::OnButtonGrid()							{	m_bShowGrid = !m_bShowGrid;	Invalidate(FALSE);}
void CPaperImageView::OnUpdateButtonGrid(CCmdUI* pCmdUI)		{	pCmdUI->SetCheck(m_bShowGrid);	}

void CPaperImageView::OnButtonInit()
{
	CPaperImageDoc* pDoc = GetDocument();

	initScale();
	//---------------------------------------------------------------------------------
	// 기존 데이터 모두 제거
	pDoc->resetData();

	Invalidate(FALSE);
}

void CPaperImageView::OnButtonZe()
{
	initScale();
	Invalidate(FALSE);
}

//==================================================================================================

void CPaperImageView::ButtonC(int type)
{
	CPaperImageDoc* pDoc = GetDocument();

	//---------------------------------------------------------------------------------
	// 기존 데이터 모두 제거
	pDoc->resetData();

	m_dRate = 2222.2222;
	UpdateStatusBar();

	//---------------------------------------------------------------------------------
	// CQ 객체 생성
	//---------------------------------------------------------------------------------
	// 레이더 중심 좌표
	int x = (int)(m_rect.Width() / 2.0);
	int y = (int)(m_rect.Height() * 3.0 / 5.0);
	PointF mp = Scr2Map(x, y);

	CCQAreaCircle* pArea = new CCQAreaCircle(getMapRect(), mp.X, mp.Y, RADAR_R_METER);
	pDoc->m_CQAreas.push_back(pArea);

	//---------------------------------------------------------------------------------
	// MO 객체 생성 (시작점은 여기에서 설정해준다.)
	//---------------------------------------------------------------------------------
	// 경로의 시작점 설정 ==> CQArea 밖의 임의의 점에서 출발하는 것으로 수정이 필요
	POINT tp[4] = { { 700, 100 }, { 200, 100 }, { 50, 850 }, { 850, 850 } };

	for (int i = 0; i < 4; i++)
//	int i = 0;
	{
		PointF tmp = Scr2Map(tp[i].x, tp[i].y);
		CCQObject* pObj = new CCQObject(RAO, RHSO, tmp, pArea, type, 1000);	// 1000 km/h 속도를 가진 tmp위치의 객체 생성 ==> 임의의 속도로 수정이 필요
		pDoc->m_CQObjects.push_back(pObj);
		Sleep(1);
	}
}

void CPaperImageView::OnButtonC1(){	ButtonC(TRJTYPE1);	Invalidate(FALSE);}// 진입 - 정지
void CPaperImageView::OnButtonC2(){	ButtonC(TRJTYPE2);	Invalidate(FALSE);}// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> 통과(BYPASS) (각도가 둔각)
void CPaperImageView::OnButtonC3(){	ButtonC(TRJTYPE3);	Invalidate(FALSE);}// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> U-Turn (각도가 예각)
void CPaperImageView::OnButtonC4(){	ButtonC(TRJTYPE4);	Invalidate(FALSE);}// (진입 - 진출) - (진입 - 진출) ==> TRJTYPE2 - TRJTYPE2 (TRJ 사이에는 예각)
void CPaperImageView::OnButtonC5(){	ButtonC(TRJTYPE5);	Invalidate(FALSE);}// 우회

//==================================================================================================

void CPaperImageView::ButtonA(int type)
{
	CPaperImageDoc* pDoc = GetDocument();

	//---------------------------------------------------------------------------------
	// 기존 데이터 모두 제거
	pDoc->resetData();

	m_dRate = 84.4690;
	UpdateStatusBar();

	//---------------------------------------------------------------------------------
	// CQ 객체 생성
	//---------------------------------------------------------------------------------
	// 소나 원뿔 좌표
	int x = (int)(m_rect.Width() / 2.0);
	int y = (int)(m_rect.Height() * 3.0 / 5.0);
	PointF mp = Scr2Map(x, y);

	CCQAreaArc* pArea = new CCQAreaArc(getMapRect(), mp.X, mp.Y, 270.0f, 90.0f, SONAR_R_METER);
	pDoc->m_CQAreas.push_back(pArea);

	//---------------------------------------------------------------------------------
	// MO 객체 생성 (시작점은 여기에서 설정해준다.)
	//---------------------------------------------------------------------------------
	// 경로의 시작점 설정 ==> CQArea 밖의 임의의 점에서 출발하는 것으로 수정이 필요
	POINT tp[4] = { { 700, 100 }, { 200, 100 }, { 50, 850 }, { 850, 850 } };

	for (int i = 0; i < 4; i++)
		//	int i = 0;
	{
		PointF tmp = Scr2Map(tp[i].x, tp[i].y);
		CCQObject* pObj = new CCQObject(RAO, RHSO, tmp, pArea, type, 1000);	// 1000 km/h 속도를 가진 tmp위치의 객체 생성 ==> 임의의 속도로 수정이 필요
		pDoc->m_CQObjects.push_back(pObj);
		Sleep(1);
	}
}

void CPaperImageView::OnButtonA1() { ButtonA(TRJTYPE1);	Invalidate(FALSE); }// 진입 - 정지
void CPaperImageView::OnButtonA2() { ButtonA(TRJTYPE2);	Invalidate(FALSE); }// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> 통과(BYPASS) (각도가 둔각)
void CPaperImageView::OnButtonA3() { ButtonA(TRJTYPE3);	Invalidate(FALSE); }// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> U-Turn (각도가 예각)
void CPaperImageView::OnButtonA4() { ButtonA(TRJTYPE4);	Invalidate(FALSE); }// (진입 - 진출) - (진입 - 진출) ==> TRJTYPE2 - TRJTYPE2 (TRJ 사이에는 예각)
void CPaperImageView::OnButtonA5() { ButtonA(TRJTYPE5);	Invalidate(FALSE); }// 우회

//==================================================================================================

void CPaperImageView::ButtonR(int type)
{
	CPaperImageDoc* pDoc = GetDocument();

	//---------------------------------------------------------------------------------
	// 기존 데이터 모두 제거
	pDoc->resetData();

	m_dRate = 2222.2222;
	UpdateStatusBar();

	//---------------------------------------------------------------------------------
	// CQ 객체 생성
	//---------------------------------------------------------------------------------
	// Rectangle 좌표
	int x = (int)(m_rect.Width() / 2.0);
	int y = (int)(m_rect.Height() * 3.0 / 5.0);
	PointF mp = Scr2Map(x, y);

	REAL xdist = RADAR_R_METER * 2.0f;
	REAL ydist = RADAR_R_METER;

	CCQAreaRect* pArea = new CCQAreaRect(getMapRect(), mp.X, mp.Y, xdist, ydist);
	pDoc->m_CQAreas.push_back(pArea);

	//---------------------------------------------------------------------------------
	// MO 객체 생성 (시작점은 여기에서 설정해준다.)
	//---------------------------------------------------------------------------------
	// 경로의 시작점 설정 ==> CQArea 밖의 임의의 점에서 출발하는 것으로 수정이 필요
	POINT tp[4] = { { 700, 100 }, { 200, 100 }, { 50, 850 }, { 850, 850 } };

	for (int i = 0; i < 4; i++)
	{
		PointF tmp = Scr2Map(tp[i].x, tp[i].y);
		CCQObject* pObj = new CCQObject(RAO, RHSO, tmp, pArea, type, 1000);	// 1000 km/h 속도를 가진 tmp위치의 객체 생성 ==> 임의의 속도로 수정이 필요
		pDoc->m_CQObjects.push_back(pObj);
		Sleep(1);
	}
}

void CPaperImageView::OnButtonR1() { ButtonR(TRJTYPE1);	Invalidate(FALSE); }// 진입 - 정지
void CPaperImageView::OnButtonR2() { ButtonR(TRJTYPE2);	Invalidate(FALSE); }// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> 통과(BYPASS) (각도가 둔각)
void CPaperImageView::OnButtonR3() { ButtonR(TRJTYPE3);	Invalidate(FALSE); }// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> U-Turn (각도가 예각)
void CPaperImageView::OnButtonR4() { ButtonR(TRJTYPE4);	Invalidate(FALSE); }// (진입 - 진출) - (진입 - 진출) ==> TRJTYPE2 - TRJTYPE2 (TRJ 사이에는 예각)
void CPaperImageView::OnButtonR5() { ButtonR(TRJTYPE5);	Invalidate(FALSE); }// 우회

void CPaperImageView::OnButtonC()
{
	m_pCQDlg->ShowWindow(SW_SHOW);
}

void CPaperImageView::OnButtonS()
{
	m_pSimulDlg->ShowWindow(SW_SHOW);
}

#pragma endregion


void CPaperImageView::On32803()
{
	CPaperImageDoc* pDoc = GetDocument();
	vector<CCQObject*> m_CQObjects = pDoc->m_CQObjects;

	ofstream outfile;
	outfile.open("events.txt", ofstream::trunc); // opens the file
	if (!outfile) { // file couldn't be opened
		cerr << "Error: file could not be opened" << endl;
		exit(1);
	}

	for (int i = 0; i < pDoc->m_CQObjects.size(); ++i) {
		stringstream pX, pY;
		pX << fixed << setprecision(4) << m_CQObjects[i]->m_initPoint.X;
		pY << fixed << setprecision(4) << m_CQObjects[i]->m_initPoint.Y;

		outfile << m_CQObjects[i]->m_id << "|" << m_CQObjects[i]->m_objtype << "|" << m_CQObjects[i]->m_speedtype
			<< "|" << pX.str() << "," << pY.str()
			<< "|" << m_CQObjects[i]->m_trjtype
			<< "|" << m_CQObjects[i]->m_kmh << "|" << m_CQObjects[i]->m_bFriend;
		if (i + 1 < m_CQObjects.size())
			outfile << endl;
	}
	outfile.close();
	// TODO: Add your command handler code here
}
