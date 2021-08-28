﻿
// PaperImageView.h: CPaperImageView 클래스의 인터페이스
//

#pragma once

class CPaperImageDoc;
class CCQDialog;
class CSimulDlg;

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
	afx_msg void OnButtonPathdot();
	afx_msg void OnUpdateButtonPathdot(CCmdUI* pCmdUI);
	afx_msg void OnButtonGrid();
	afx_msg void OnUpdateButtonGrid(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnButtonInit();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonC1();
	afx_msg void OnButtonC2();
	afx_msg void OnButtonC3();
	afx_msg void OnButtonC4();
	afx_msg void OnButtonC5();
	afx_msg void OnButtonA1();
	afx_msg void OnButtonA2();
	afx_msg void OnButtonA3();
	afx_msg void OnButtonA4();
	afx_msg void OnButtonA5();
	afx_msg void OnButtonR1();
	afx_msg void OnButtonR2();
	afx_msg void OnButtonR3();
	afx_msg void OnButtonR4();
	afx_msg void OnButtonR5();
	afx_msg void OnButtonC();
	afx_msg void OnButtonS();
protected:
	afx_msg LRESULT OnSimul(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnButtonZe();
	virtual void OnInitialUpdate();
	afx_msg void On32803();
};

#ifndef _DEBUG  // PaperImageView.cpp의 디버그 버전
inline CPaperImageDoc* CPaperImageView::GetDocument() const
   { return reinterpret_cast<CPaperImageDoc*>(m_pDocument); }
#endif

