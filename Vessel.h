#pragma once
#include"Util.h"

class PointF;
class Line;
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