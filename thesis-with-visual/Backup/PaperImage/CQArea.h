#pragma once

class CCQArea
{
public:
	int		m_CQType;		// Radar, Sonar, Rect
	RectF	m_mapRect;		// 현재 화면에 보이는 전체 MBR의 지도 좌표
	PointF	m_fPt;			// CQ가 위치하는 중심좌표
	REAL	m_fRadius[2];	// 버퍼 반지름(0:BFZ), CQ 반지름(1:CQZ) ==> 경고반지름이 0.0이면 버퍼 반지름의 0.7로 설정
	RectF	m_rectf;		// CQArea의 MBR
	wstring	m_RuleID;
	REAL	m_vx;
	REAL	m_vy;
public:
	CCQArea(int CQType, RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy);

	double getMinDistance(PointF& tp);
	PointF getMBRCenterPoint();
	virtual void draw(Graphics& g, CView* pView);

	// 주어진 점pt가 영역 내에 존재하는지 검사
	virtual BOOL isIn(PointF pt) = 0;

	// pt에서 내부 버퍼존과 CQ존 사이로 진입하는 점을 찾는다.
	virtual PointF getApproachPoint(PointF pt) = 0;								// TRJTYPE1
	
	// p1-p2를 잇는 선에서 영역 내에 존재하는 p2에서 영역 외부로 나가는 점을 찾는다.
	virtual PointF getRetreatPoint(PointF pt, double angle, BOOL bBound) = 0;
	
	// 점 배열 vec에서 영역 외부를 타고 도는 점을 찾는다.
	virtual PointF getDetourPoint(vector<PointF>& vec) = 0;						// TRJTYPE5
};

class CCQAreaCircle : public CCQArea
{
public:
	CCQAreaCircle(RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy);

	void	draw(Graphics& g, CView* pView);
	BOOL isIn(PointF pt);

	PointF	getApproachPoint(PointF pt);
	PointF	getRetreatPoint(PointF pt, double angle, BOOL bBound);
	PointF	getDetourPoint(vector<PointF>& vec);
};

//class CCQAreaArc : public CCQArea
//{
//private:
//	double getStartAngle();
//	double getEndAngle();
//
//public:
//	REAL m_fAngle1;	// Arc의 중심각
//	REAL m_fAngle2;	// Arc의 사이각
//
//public:
//	CCQAreaArc(RectF mapRect, REAL x, REAL y, REAL fAngle1, REAL fAngle2, REAL fRadius1, REAL fRadius2 = 0.0f);
//
//	void	draw(Graphics& g, CView* pView);
//
//	BOOL isIn(PointF pt);
//
//	PointF	getApproachPoint(PointF pt);
//	PointF	getRetreatPoint(PointF pt, double angle, BOOL bBound);
//	PointF	getDetourPoint(vector<PointF>& vec);
//};
//
//class CCQAreaRect : public CCQArea
//{
//public:
//	// x, y는 Rectagnle의 중심좌표
//	CCQAreaRect(RectF mapRect, REAL x, REAL y, REAL BFZw, REAL BFZh);
//
//	void	draw(Graphics& g, CView* pView);
//
//	BOOL isIn(PointF pt);
//
//	PointF	getApproachPoint(PointF pt);
//	PointF	getRetreatPoint(PointF pt, double angle, BOOL bBound);
//	PointF	getDetourPoint(vector<PointF>& vec);
//};