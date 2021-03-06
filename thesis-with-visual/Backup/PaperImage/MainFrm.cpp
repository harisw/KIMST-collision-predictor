
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "PaperImage.h"

#include "MainFrm.h"
#include "Util.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_XY,		// 화면 좌표 (x, y)
	ID_INDICATOR_MXY,		// 지도 좌표 (x, y)
	ID_INDICATOR_CXY,		// 지도 중심 좌표 (x, y)
	ID_INDICATOR_RATE,		// 스케일
	ID_INDICATOR_RC,		// Cell 좌표 (row, col)
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	/////////////////////////////////////////////////////////////////
	// 상태바 생성
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// 상태바 크기 조절 시작
	UINT nID, nStyle;
	int  cxWidth;

	m_wndStatusBar.GetPaneInfo(0, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(0, nID, nStyle, (int)(cxWidth * 0.5));

	// 화면좌표 X,Y
	m_wndStatusBar.GetPaneInfo(1, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(1, nID, nStyle, cxWidth * 3);

	// 지도 좌표 (x, y)
	m_wndStatusBar.GetPaneInfo(2, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(2, nID, nStyle, cxWidth * 4);

	// 지도 중심 좌표 (x, y)
	m_wndStatusBar.GetPaneInfo(3, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(3, nID, nStyle, cxWidth * 4);

	// 스케일
	m_wndStatusBar.GetPaneInfo(4, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(4, nID, nStyle, cxWidth * 4);

	// CELL좌표 COL,ROW
	m_wndStatusBar.GetPaneInfo(5, nID, nStyle, cxWidth);
	m_wndStatusBar.SetPaneInfo(5, nID, nStyle, cxWidth * 2);
	/////////////////////////////////////////////////////////////////
	CDockingManager::SetDockingMode(DT_SMART);
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	m_DockPane.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_DockPane);
	return 0;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	const wchar_t* temp= L"Output Dock";
	// Create file view
	CString strDockView;
	//bNameValid = strDockView.LoadString(IDS_OUTPUT_DOCK);
	bNameValid = strDockView.LoadStringW(IDS_OUTPUT_DOCK);
	//bNameValid = strDockView.LoadString(temp);
	//ASSERT(bNameValid);
	if (!m_DockPane.Create(strDockView, this, CRect(0, 0, 450, 200), TRUE, ID_OUTPUT_DOCK, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI | CBRS_HIDE_INPLACE))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	//SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

	// Multi
	cs.cx = M_RECT_W;
	cs.cy = M_RECT_H;
	/*cs.cx = 920;
	cs.cy = 1006;*/

	cs.x = 0;
	cs.y = 0;

	// 프로그램 타이틀에 "제목없음" 없애기
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

