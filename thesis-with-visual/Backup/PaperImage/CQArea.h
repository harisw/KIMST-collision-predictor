#pragma once

class CCQArea
{
public:
	int		m_CQType;		// Radar, Sonar, Rect
	RectF	m_mapRect;		// ���� ȭ�鿡 ���̴� ��ü MBR�� ���� ��ǥ
	PointF	m_fPt;			// CQ�� ��ġ�ϴ� �߽���ǥ
	PointF m_fCurrentPt;
	PointF m_fEndPt;
	vector<PointF> m_BTPoints;
	REAL	m_fRadius[2];	// ���� ������(0:BFZ), CQ ������(1:CQZ) ==> ���������� 0.0�̸� ���� �������� 0.7�� ����
	RectF	m_rectf;		// CQArea�� MBR
	wstring	m_RuleID;
	REAL	m_vx;
	REAL	m_vy;
	double theta;
public:
	CCQArea(int CQType, RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy);

	double getMinDistance(PointF& tp);
	PointF getMBRCenterPoint();
	void moveOurVessel();
	void getEndPoint();
	void getBTPoints();
	void getMovingAngle();
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

	void drawBT(Graphics& g, CView* pView);

	void	draw(Graphics& g, CView* pView);
	BOOL isIn(PointF pt);

	PointF	getApproachPoint(PointF pt);
	PointF	getRetreatPoint(PointF pt, double angle, BOOL bBound);
	PointF	getDetourPoint(vector<PointF>& vec);
};