#include "Vessel.h"
#include "Util.h"

void Vessel::setFinalLoc()
{
	double xH, yH;
	xH = loc0->x + (queryWind * vx);
	yH = loc0->y + (queryWind * vy);
	locH = new PointF(xH, yH);
}

void Vessel::setTrajectoryBuffer()
{
	PointF *p1, *p2, *p3, *p4;
	double x, y;

	p1->x = loc0->x - (d / 2);		//GOING UP CASE
	p1->y = loc0->y - d;

	p2->x = loc0->x + (d / 2);
	p2->y = loc0->y - d;

	p3->x = loc0->x - (d / 2);
	p3->y = loc0->y + d;

	p4->x = loc0->x + (d / 2);
	p4->y = loc0->y + d;

	bt.insert(bt.end(), { new Line(p1, p2), new Line(p1, p3),
		new Line(p2, p4), new Line(p3, p4) });
}