
// PaperImageView.h: CPaperImageView 클래스의 인터페이스
//

#pragma once

class CPaperImageDoc;
class CCQDialog;
class CSimulDlg;
class RuleDlg;
class ObjListDlg;
class SimuOutDlg;
class TPRTree;
class CPaperImageView : public CView
{
protected: // serialization에서만 만들어집니다.
	CPaperImageView() noexcept;
	DECLARE_DYNCREATE(CPaperImageView)

// 특성입니다.
public:
	CPaperImageDoc* GetDocument() const;

	CDC*		m_pDC;
	CBitmap*	m_pBitmap;

	CCQDialog*	m_pCQDlg;
	CSimulDlg*	m_pSimulDlg;
	RuleDlg* m_pRuleDlg;
	ObjListDlg* m_objListDlg;
	SimuOutDlg* m_simuOutDlg;

	CRect		m_rect;		// 현재 화면 사이즈 (pixel)
	double		m_dRate;	// 실제거리(meter) / pixel
							// 최초 : 900000meter / 900 pixel ==> 1000.0 (지도 좌표를 m_dRate으로 나누면 화면 좌표가 나온다는 의미)
	PointF		m_centerPt;	// 화면 중심 지도 좌표

	double		m_dMap_Meter;
	double		m_dCell_Meter;

	//---------------------------------------------------------------------------------------
	// Draw Option
	//BOOL		m_bAbstract;	// Abstract를 그릴건지 Concrete를 그릴건지
	BOOL		m_bShowGrid;	// Grid를 그릴지 말지
	BOOL		m_bShowDot;		// 객체 경로의 정점을 그릴지 말지

	BOOL		m_bREO;
	BOOL		m_bRFO;
	BOOL		m_bALL;
	BOOL		m_bRAO;
	BOOL		m_bRAO_RLSO;
	BOOL		m_bRAO_RMSO;
	BOOL		m_bRAO_RHSO;
	BOOL		m_bRSVO;
	BOOL		m_bRSVO_RLSO;
	BOOL		m_bRSVO_RMSO;
	BOOL		m_bRSVO_RHSO;
	BOOL		m_bRFVO;
	BOOL		m_bRFVO_RLSO;
	BOOL		m_bRFVO_RMSO;
	BOOL		m_bRFVO_RHSO;
	BOOL		m_bRSMO;
	BOOL		m_bRSMO_RLSO;
	BOOL		m_bRSMO_RMSO;
	BOOL		m_bRSMO_RHSO;
	//---------------------------------------------------------------------------------------
	int obj_num;
	int currentT = -1;
	TPRTree* myTPR;
// 작업입니다.
public:
	void ReleaseResources();
	void AllocResources(CDC* pDC, int _w, int _h);

	void initScale();

	RectF getMapRect();
	POINT Map2Scr(REAL x, REAL y);
	int Map2Scr(REAL d);
	PointF Scr2Map(int x, int y);
	REAL Scr2Map(int d);
	void UpdateStatusBar();

	void DrawGrid(Graphics& g);
	void updateQueue();
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.

// 구현입니다.
public:
	virtual ~CPaperImageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void ButtonC(int type);	// Radar
	void ButtonA(int type);	// Sonar
	void ButtonR(int type);	// Rectangle

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonC();

	void SetCQType(int cqTYPE);

	void startSimulation();

protected:
	afx_msg LRESULT OnSimul(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // PaperImageView.cpp의 디버그 버전
inline CPaperImageDoc* CPaperImageView::GetDocument() const
   { return reinterpret_cast<CPaperImageDoc*>(m_pDocument); }
#endif

