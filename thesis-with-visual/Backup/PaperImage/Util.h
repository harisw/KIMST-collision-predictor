#pragma once
#include <queue>
//==============================================================================

#define TOTALCOUNT		30
#define MOVEINTERVAL	60	// 이동체 표시 간격 (초)
#define SIMU_TIME		50

#define WINDOW_W	1200
#define WINDOW_H	760
//==============================================================================

#define BFZ		0	// 버퍼존
#define CQZ		1	// CQ존

#define _RADAR_				0	// CQType : Radar ==> Circle
#define _SONAR_				1	// CQType : Sonar ==> Arc
//#define _RECT_				2	// CQType : Rect  ==> rectangle
#define _SONAR4_			2	// CQType : Sonar * 4
#define _RADAR_SONAR4_2_	3	// CQType : _RADAR_SONAR4_ * 2
#define _RECT_				4	// CQType : Rect  ==> rectangle
#define _FREE_CQ_			5

#define SHIPWIDTH		19.0f	// 함선 폭 19m
#define SHIPLENGTH		156.0f	// 함선 길이 156m
#define VESSEL_RAD		200
//#define SHIPWIDTH		15.0f	// 함선 폭 19m
//#define SHIPLENGTH		136.0f	// 함선 길이 156m
//==============================================================================

#define ROCQ	0	// CQ에 접근하는 객체
#define RONCQ	1	// CQ와 무관한 객체

#define RAO		0	// 공중
#define RSVO	1	// 군함
#define RFVO	2	// 어선
#define RSMO	3	// 잠수함

#define RLSO	0	// 저속
#define RMSO	1	// 중속
#define RHSO	2	// 고속

//==============================================================================

#define PI		3.141592
#define DTR(deg) ((double)(deg)*PI/180.0)
#define RTD(rad) ((double)(rad)*180.0/PI)

//==============================================================================
//Original
#define MAP_METER		20000.0	// 화면에 표시되는 전체 지도 거리(2000km)
#define R_METER	3000.0	// 레이더 반경 (300km)
#define CELL_METER		250.0		// 셀의 가로 간격 (25km)

#define ARROWWIDTH		8			// 경로를 그려줄 때 화살표 크기

#define WINDOW_X_START	75
#define WINDOW_Y_START	20
//==============================================================================

// 경로 설정을 위한 기본 이동 타입
#define CQAPPROACH		1			// 진입	(외부점에서 CQ(버퍼) 내부점으로 이동)
#define CQRETREAT		2			// 진출	(내부점에서 CQ(버퍼) 외부점으로 이동)
#define CQHOVER			3			// 정지 (CQ(버퍼 내에서 호버링)
#define CQDETOUR		4			// 우회 (CQ과 접하지 않으면서 버퍼존에서 버퍼존으로 이동)

// 경로 설정 타입
#define TRJTYPE1		101			// 진입	(CQAPPROACH) - 정지
#define TRJTYPE2		102			// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> 통과(BYPASS) (각도가 둔각)
#define TRJTYPE3		103			// 진입 - 진출 (CQAPPROACH - CQRETREAT)	-> U-Turn (각도가 예각)
#define TRJTYPE4		104			// (진입 - 진출) - (진입 - 진출) ==> TRJTYPE2 - TRJTYPE2 (TRJ 사이에는 예각)
#define TRJTYPE5		105			// 우회

//==============================================================================
#define SIMU_LONG_SLEEP 50
#define SIMU_SLEEP 5
#define DISTANCE_RATIO	20
//#define SONAR_DISTANCE_RATIO	10
#define DECIMAL_ROUND 1000
#define M_RECT_W 1200.0
#define M_RECT_H 760.0
//==============================================================================	CUSTOM MESSAGE
#define WM_GENERATE_OBJ (WM_USER + 500)
#define WM_UPDATE_EVENT (WM_USER + 501)
extern queue<string> queue_ev;

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

	// sp-ep 라인을 100으로 해서 sp에서 경로를 따라서 percent만큼 떨어진 거리의 좌표를 리턴한다.
	static CPoint getOffsetPoint(CPoint sp, CPoint ep, int percent);
	static PointF getOffsetPoint(PointF sp, PointF ep, int percent);

	// path의 시작점에서 경로를 따라서 dist만큼 떨어진 거리의 좌표를 리턴한다.
	static CPoint getOffsetPoint(vector<POINT>& path, double dist);
	static PointF getOffsetPoint(vector<PointF>& path, double dist);

	// cp를 중심으로 하는 p1-p2 호를 cnt개수 만큼 분할해서 retArr에 저장
	static void getRotatePoint(POINT cp, POINT p1, POINT p2, double area, int cnt, vector<POINT>& retArr);
	static void getRotatePoint(PointF cp, PointF p1, PointF p2, double area, int cnt, vector<PointF>& retArr);

	// i과 j사이의 정수 랜덤값을 리턴한다.
	static long	getRandomLong(long i, long j);
	static PointF getRandomPoint(PointF& pt, int mindist, int maxdist, int minang, int maxang);
	static PointF getRandomPoint(REAL minx, REAL miny, REAL maxx, REAL maxy);

	// 점 pt에서 d만큼 떨어져서 a만큼 회전한 점의 좌표를 리턴
	// 3시가 0도, 반시계방향으로 증가
	static CPoint getPoint(CPoint& pt, double dist, double angle);
	static PointF getPoint(PointF& pt, double dist, double angle);

	// pt에서 angle방향으로 선을 그어서 rect와 교차하는 점을 리턴
	static PointF getPoint(PointF& pt, RectF rect, double angle);

	// 두 원의 교점을 ret에 담아서 리턴한다.
	static void circle_circle_Intersect(double x1, double y1, double r1, double x2, double y2, double r2, PointF ret[2]);
	// 원과 직선의 교점을 ret에 담아서 리턴한다.
	static void circle_line_Intersect(double x, double y, double r, double x1, double y1, double x2, double y2, PointF ret[2]);

	// 주어진 속도에서 주어진 G-Force에 도달하면서 회전을 하려면 필요한 반지름을 meter로 구한다.
	static double getRadius(double speed, int gForce);
};