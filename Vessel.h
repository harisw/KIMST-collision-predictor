#pragma once
#include"Util.h"
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

class Vessel
{
	PointF* loc0;
	PointF* locH;
	double vx;
	double vy;
	double d;
	vector<Line*> bt;
	int queryWind;
	Vessel(PointF* _loc0, double _vx, double _vy, double _d) {
		loc0 = _loc0;
		vx = _vx;
		vy = _vy;
		d = _d;
	}
	
	void setFinalLoc();
	void setTrajectoryBuffer();

};