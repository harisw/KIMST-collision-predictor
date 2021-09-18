#pragma once

class CCQArea
{
public:
	int		m_CQType;		// Radar, Sonar, Rect
	RectF	m_mapRect;		// ���� ȭ�鿡 ���̴� ��ü MBR�� ���� ��ǥ
	PointF	m_fPt;			// CQ�� ��ġ�ϴ� �߽���ǥ
	REAL	m_fRadius[2];	// ���� ������(0:BFZ), CQ ������(1:CQZ) ==> ���������� 0.0�̸� ���� �������� 0.7�� ����
	RectF	m_rectf;		// CQArea�� MBR
	wstring	m_RuleID;
	REAL	m_vx;
	REAL	m_vy;
public:
	CCQArea(int CQType, RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy);

	double getMinDistance(PointF& tp);
	PointF getMBRCenterPoint();
	virtual void draw(Graphics& g, CView* pView);

	// �־��� ��pt�� ���� ���� �����ϴ��� �˻�
	virtual BOOL isIn(PointF pt) = 0;

	// pt���� ���� �������� CQ�� ���̷� �����ϴ� ���� ã�´�.
	virtual PointF getApproachPoint(PointF pt) = 0;								// TRJTYPE1
	
	// p1-p2�� �մ� ������ ���� ���� �����ϴ� p2���� ���� �ܺη� ������ ���� ã�´�.
	virtual PointF getRetreatPoint(PointF pt, double angle, BOOL bBound) = 0;
	
	// �� �迭 vec���� ���� �ܺθ� Ÿ�� ���� ���� ã�´�.
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
//	REAL m_fAngle1;	// Arc�� �߽ɰ�
//	REAL m_fAngle2;	// Arc�� ���̰�
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
//	// x, y�� Rectagnle�� �߽���ǥ
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