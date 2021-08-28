#pragma once

//==============================================================================

#define TOTALCOUNT		10
#define MOVEINTERVAL	60	// �̵�ü ǥ�� ���� (��)

//==============================================================================

#define BFZ		0	// ������
#define CQZ		1	// CQ��

#define _RADAR_				0	// CQType : Radar ==> Circle
#define _SONAR_				1	// CQType : Sonar ==> Arc
#define _RECT_				2	// CQType : Rect  ==> rectangle
#define _SONAR4_			3	// CQType : Sonar * 4
#define _RADAR_SONAR4_		4	// CQType : _RADAR_ + _SONAR4_
#define _RADAR_SONAR4_2_	5	// CQType : _RADAR_SONAR4_ * 2

#define SHIPWIDTH		19.0f	// �Լ� �� 19m
#define SHIPLENGTH		156.0f	// �Լ� ���� 156m

//==============================================================================

#define ROCQ	0	// CQ�� �����ϴ� ��ü
#define RONCQ	1	// CQ�� ������ ��ü

#define RAO		0	// ����
#define RSVO	1	// ����
#define RFVO	2	// �
#define RSMO	3	// �����

#define RLSO	0	// ����
#define RMSO	1	// �߼�
#define RHSO	2	// ���

//==============================================================================

#define PI		3.141592
#define DTR(deg) ((double)(deg)*PI/180.0)
#define RTD(rad) ((double)(rad)*180.0/PI)

//==============================================================================

#define RADAR_MAP_METER		2000000.0	// ȭ�鿡 ǥ�õǴ� ��ü ���� �Ÿ�(2000km)
#define SONAR_MAP_METER		100000.0	// ȭ�鿡 ǥ�õǴ� ��ü ���� �Ÿ�(100km)

#define RADAR_R_METER	300000.0	// ���̴� �ݰ� (300km)
#define SONAR_R_METER	20000.0		// �ҳ� �ݰ� (20km)

#define RADAR_CELL_METER		25000.0		// ���� ���� ���� (25km)
#define SONAR_CELL_METER		2500.0		// ���� ���� ���� (0.25km)

#define ARROWWIDTH		8			// ��θ� �׷��� �� ȭ��ǥ ũ��

//==============================================================================

// ��� ������ ���� �⺻ �̵� Ÿ��
#define CQAPPROACH		1			// ����	(�ܺ������� CQ(����) ���������� �̵�)
#define CQRETREAT		2			// ����	(���������� CQ(����) �ܺ������� �̵�)
#define CQHOVER			3			// ���� (CQ(���� ������ ȣ����)
#define CQDETOUR		4			// ��ȸ (CQ�� ������ �����鼭 ���������� ���������� �̵�)

// ��� ���� Ÿ��
#define TRJTYPE1		101			// ����	(CQAPPROACH) - ����
#define TRJTYPE2		102			// ���� - ���� (CQAPPROACH - CQRETREAT)	-> ���(BYPASS) (������ �а�)
#define TRJTYPE3		103			// ���� - ���� (CQAPPROACH - CQRETREAT)	-> U-Turn (������ ����)
#define TRJTYPE4		104			// (���� - ����) - (���� - ����) ==> TRJTYPE2 - TRJTYPE2 (TRJ ���̿��� ����)
#define TRJTYPE5		105			// ��ȸ

//==============================================================================

class CUtil
{
public:
	CUtil() {}

	static BOOL isEqual(double a, double b, int prec = 4);
	static BOOL isEqual(PointF a, PointF b, int prec = 4);

	static BOOL intersect(RectF r, vector<PointF>* vec);
	static BOOL intersect(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double* tx = NULL, double* ty = NULL);

	static double distance_line_point(double ptx, double pty, double sptx, double spty, double eptx, double epty, double* pIntersx = NULL, double* pIntersy = NULL);
	static double distance(double x1, double y1, double x2, double y2);
	static double distance(vector<POINT>& path);
	static double distance(vector<PointF>& path);

	static double getArea(vector<POINT>& ptArray);
	static double getArea(vector<PointF>& ptArray);
	static void getTangent(POINT* tp, POINT sp, POINT cp, int r);
	static void getTangent(PointF* tp, PointF sp, PointF cp, double r);

	static double getAngle(CPoint p1, CPoint p2);
	static double getAngle(PointF p1, PointF p2);

	// sp-ep ������ 100���� �ؼ� sp���� ��θ� ���� percent��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
	static CPoint getOffsetPoint(CPoint sp, CPoint ep, int percent);
	static PointF getOffsetPoint(PointF sp, PointF ep, int percent);

	// path�� ���������� ��θ� ���� dist��ŭ ������ �Ÿ��� ��ǥ�� �����Ѵ�.
	static CPoint getOffsetPoint(vector<POINT>& path, double dist);
	static PointF getOffsetPoint(vector<PointF>& path, double dist);

	// cp�� �߽����� �ϴ� p1-p2 ȣ�� cnt���� ��ŭ �����ؼ� retArr�� ����
	static void getRotatePoint(POINT cp, POINT p1, POINT p2, double area, int cnt, vector<POINT>& retArr);
	static void getRotatePoint(PointF cp, PointF p1, PointF p2, double area, int cnt, vector<PointF>& retArr);

	// i�� j������ ���� �������� �����Ѵ�.
	static long	getRandomLong(long i, long j);
	static PointF getRandomPoint(PointF& pt, int mindist, int maxdist, int minang, int maxang);
	static PointF getRandomPoint(REAL minx, REAL miny, REAL maxx, REAL maxy);

	// �� pt���� d��ŭ �������� a��ŭ ȸ���� ���� ��ǥ�� ����
	// 3�ð� 0��, �ݽð�������� ����
	static CPoint getPoint(CPoint& pt, double dist, double angle);
	static PointF getPoint(PointF& pt, double dist, double angle);

	// pt���� angle�������� ���� �׾ rect�� �����ϴ� ���� ����
	static PointF getPoint(PointF& pt, RectF rect, double angle);

	// �� ���� ������ ret�� ��Ƽ� �����Ѵ�.
	static void circle_circle_Intersect(double x1, double y1, double r1, double x2, double y2, double r2, PointF ret[2]);
	// ���� ������ ������ ret�� ��Ƽ� �����Ѵ�.
	static void circle_line_Intersect(double x, double y, double r, double x1, double y1, double x2, double y2, PointF ret[2]);

	// �־��� �ӵ����� �־��� G-Force�� �����ϸ鼭 ȸ���� �Ϸ��� �ʿ��� �������� meter�� ���Ѵ�.
	static double getRadius(double speed, int gForce);
};