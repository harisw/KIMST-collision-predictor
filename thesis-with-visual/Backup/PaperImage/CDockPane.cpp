
#include "pch.h"
#include "framework.h"
#include "mainfrm.h"
#include "CDockPane.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockPane

CDockPane::CDockPane() noexcept
{
}

CDockPane::~CDockPane()
{
}

BEGIN_MESSAGE_MAP(CDockPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	//ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CDockPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CRect rectDummy;
	//rectDummy.SetRectEmpty();

	//// Create view:
	//const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	//if (!m_wndDockPane.Create(dwViewStyle, rectDummy, this, 4))
	//{
	//	TRACE0("Failed to create file view\n");
	//	return -1;      // fail to create
	//}

	//// Load view images:
	//m_DockPaneImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	//m_wndDockPane.SetImageList(&m_DockPaneImages, TVSIL_NORMAL);

	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	//m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* Is locked */);

	//OnChangeVisualStyle();

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	//m_wndToolBar.SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// Fill in some static tree view data (dummy code, nothing magic here)
	//FillDockPane();
	//AdjustLayout();
	dialog_box.Create(IDD_SIMULOUTDLG, this);
	dialog_box.ShowWindow(SW_SHOW);
	return 0;
}

//void CDockPane::OnSize(UINT nType, int cx, int cy)
//{
//	CDockablePane::OnSize(nType, cx, cy);
//	AdjustLayout();
//}

//void CDockPane::FillDockPane()
//{
	//HTREEITEM hRoot = m_wndDockPane.InsertItem(_T("FakeApp files"), 0, 0);
	//m_wndDockPane.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	//HTREEITEM hSrc = m_wndDockPane.InsertItem(_T("FakeApp Source Files"), 0, 0, hRoot);

	//m_wndDockPane.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);
	//m_wndDockPane.InsertItem(_T("FakeApp.rc"), 1, 1, hSrc);
	//m_wndDockPane.InsertItem(_T("FakeAppDoc.cpp"), 1, 1, hSrc);
	//m_wndDockPane.InsertItem(_T("FakeAppView.cpp"), 1, 1, hSrc);
	//m_wndDockPane.InsertItem(_T("MainFrm.cpp"), 1, 1, hSrc);
	//m_wndDockPane.InsertItem(_T("pch.cpp"), 1, 1, hSrc);

	//HTREEITEM hInc = m_wndDockPane.InsertItem(_T("FakeApp Header Files"), 0, 0, hRoot);

	//m_wndDockPane.InsertItem(_T("FakeApp.h"), 2, 2, hInc);
	//m_wndDockPane.InsertItem(_T("FakeAppDoc.h"), 2, 2, hInc);
	//m_wndDockPane.InsertItem(_T("FakeAppView.h"), 2, 2, hInc);
	//m_wndDockPane.InsertItem(_T("Resource.h"), 2, 2, hInc);
	//m_wndDockPane.InsertItem(_T("MainFrm.h"), 2, 2, hInc);
	//m_wndDockPane.InsertItem(_T("pch.h"), 2, 2, hInc);

	//HTREEITEM hRes = m_wndDockPane.InsertItem(_T("FakeApp Resource Files"), 0, 0, hRoot);

	//m_wndDockPane.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
	//m_wndDockPane.InsertItem(_T("FakeApp.rc2"), 2, 2, hRes);
	//m_wndDockPane.InsertItem(_T("FakeAppDoc.ico"), 2, 2, hRes);
	//m_wndDockPane.InsertItem(_T("FakeToolbar.bmp"), 2, 2, hRes);

	//m_wndDockPane.Expand(hRoot, TVE_EXPAND);
	//m_wndDockPane.Expand(hSrc, TVE_EXPAND);
	//m_wndDockPane.Expand(hInc, TVE_EXPAND);
//}

//void CDockPane::OnContextMenu(CWnd* pWnd, CPoint point)
//{
	//CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndDockPane;
	//ASSERT_VALID(pWndTree);

	//if (pWnd != pWndTree)
	//{
	//	CDockablePane::OnContextMenu(pWnd, point);
	//	return;
	//}

	//if (point != CPoint(-1, -1))
	//{
	//	// Select clicked item:
	//	CPoint ptTree = point;
	//	pWndTree->ScreenToClient(&ptTree);

	//	UINT flags = 0;
	//	HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
	//	if (hTreeItem != nullptr)
	//	{
	//		pWndTree->SelectItem(hTreeItem);
	//	}
	//}

	//pWndTree->SetFocus();
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
//}

//void CDockPane::AdjustLayout()
//{
//	if (GetSafeHwnd() == nullptr)
//	{
//		return;
//	}
//
//	CRect rectClient;
//	GetClientRect(rectClient);
//
////	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
//
//	//m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
//	//m_wndDockPane.SetWindowPos(nullptr, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
//}
//
//void CDockPane::OnProperties()
//{
//	AfxMessageBox(_T("Properties...."));
//
//}
//
//void CDockPane::OnFileOpen()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnFileOpenWith()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnDummyCompile()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnEditCut()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnEditCopy()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnEditClear()
//{
//	// TODO: Add your command handler code here
//}
//
//void CDockPane::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//
//	CRect rectTree;
//	ScreenToClient(rectTree);
//
//	rectTree.InflateRect(1, 1);
//	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
//}
//
//void CDockPane::OnSetFocus(CWnd* pOldWnd)
//{
//	CDockablePane::OnSetFocus(pOldWnd);
//}
//
//void CDockPane::OnChangeVisualStyle()
//{
//	m_wndToolBar.CleanUpLockedImages();
//	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* Locked */);
//
//	m_DockPaneImages.DeleteImageList();
//
//	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;
//
//	CBitmap bmp;
//	if (!bmp.LoadBitmap(uiBmpId))
//	{
//		TRACE(_T("Can't load bitmap: %x\n"), uiBmpId);
//		ASSERT(FALSE);
//		return;
//	}
//
//	BITMAP bmpObj;
//	bmp.GetBitmap(&bmpObj);
//
//	UINT nFlags = ILC_MASK;
//
//	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;
//
//	m_DockPaneImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
//	m_DockPaneImages.Add(&bmp, RGB(255, 0, 255));
//
//	m_wndDockPane.SetImageList(&m_DockPaneImages, TVSIL_NORMAL);
//}