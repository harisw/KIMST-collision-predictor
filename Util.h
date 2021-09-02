#pragma once
#include<vector>

using namespace std;
#define STATIC_MODE 0
#define MOVING_MODE 1

#define PREDICTION_INTERVAL 3
#define QUERY_INTERVAL 6
#define QUERY_RADIUS 50

class PointF
{
public:
	double x;
	double y;
	PointF() {}
	PointF(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

class Line
{
public:
	PointF* p0;
	PointF* p1;
	Line() {}
	Line(PointF* _p0, PointF* _p1) {
		p0 = _p0;
		p1 = _p1;
	}
};

double getDistance(PointF* p, PointF* q)
{
	return sqrt(pow(p->x - q->x, 2) + pow(p->y - q->y, 2));
}