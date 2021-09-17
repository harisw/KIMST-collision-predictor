#include "pch.h"
#include "CQArea.h"
#include "resource.h"
#include "Util.h"
#include "PaperImageView.h"

//------------------------------------------------------------------------------------

#pragma region CQArea

CCQArea::CCQArea(int CQType, RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL fRadius2)
	: m_CQType(CQType)
	, m_mapRect(mapRect)
	, m_fPt(PointF(x, y))
	, m_fRadius{ fRadius1, fRadius2 }
{
	if (CQType != _RECT_)
	{
		m_fRadius[BFZ] = fRadius1;									// 버퍼반지름
	}
}

// CQ영역의 함선위치에 이미지를 그린다.
void CCQArea::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	SolidBrush br(Color(0, 0, 0));
	Pen pen(Color(0, 0, 0));
	pen.SetWidth(2.0f);

	POINT pt = pV->Map2Scr(m_fPt.X, m_fPt.Y);
	int w = pV->Map2Scr(SHIPWIDTH);
	int h = pV->Map2Scr(SHIPLENGTH);

	// 함선 폭이 너무 작으면 그리지 않는다.
	if (0 < w)	g.FillEllipse(&br, pt.x - w / 2, pt.y - h / 2, w, h);

	// 함선의 위치를 십자표시한다.
	g.DrawLine(&pen, pt.x - 20, pt.y, pt.x + 20, pt.y);
	g.DrawLine(&pen, pt.x, pt.y - 20, pt.x, pt.y + 20);
}

// tp에서 CQArea mbr중 가장 가까운 거리를 리턴
double CCQArea::getMinDistance(PointF& tp)
{
	double x[4], y[4];
	x[0] = m_rectf.X;
	y[0] = m_rectf.Y;
	x[1] = m_rectf.X + m_rectf.Width;
	y[1] = m_rectf.Y;
	x[2] = m_rectf.X + m_rectf.Width;
	y[2] = m_rectf.Y + m_rectf.Height;
	x[3] = m_rectf.X;
	y[3] = m_rectf.Y + m_rectf.Height;

	double mindist = CUtil::distance(tp.X, tp.Y, x[0], y[0]);
	for (int i = 1 ; i < 4 ; i++)
	{
		double dist = CUtil::distance_line_point(tp.X, tp.Y, x[i - 1], y[i - 1], x[i], y[i]);
		mindist = dist < mindist ? dist : mindist;
	}
	return mindist;
}

// MBR의 중심점을 리턴한다. (_RADAR_ 타입은 m_fPt와 같다)
PointF CCQArea::getMBRCenterPoint()
{
	PointF pt;
	pt.X = m_rectf.X + m_rectf.Width / 2.0f;
	pt.Y = m_rectf.Y + m_rectf.Height / 2.0f;
	return pt;
}

#pragma endregion

//------------------------------------------------------------------------------------

#pragma region CQAreaCircle

CCQAreaCircle::CCQAreaCircle(RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL fRadius2)
	:CCQArea(_RADAR_, mapRect, x, y, fRadius1, fRadius2)
{
	m_rectf.X = x - m_fRadius[BFZ];
	m_rectf.Y = y - m_fRadius[BFZ];
	m_rectf.Width = m_fRadius[BFZ] * 2.0f;
	m_rectf.Height = m_fRadius[BFZ] * 2.0f;
}

void CCQAreaCircle::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	POINT cp = pV->Map2Scr(m_fPt.X, m_fPt.Y);
	int r;
	RectF rect;
	for (int i = 0; i < 1; i++)
	{
		r = pV->Map2Scr(m_fRadius[i]);

		rect.X = (REAL)(cp.x - r);
		rect.Y = (REAL)(cp.y - r);
		rect.Width = (REAL)(r * 2);
		rect.Height = (REAL)(r * 2);

		SolidBrush br(Color(96, 225, 225, 225));
		g.FillEllipse(&br, rect);

		Pen pen(Color(0, 0, 0), 2.0);
		if (i == 0)	pen.SetDashStyle(DashStyleDash);
		else		pen.SetDashStyle(DashStyleSolid);
		g.DrawEllipse(&pen, rect);

		Gdiplus::Font myFont(L"Tahoma", 10);
		RectF layoutRect(cp.x, cp.y - (REAL)(rect.Height/2)+15.0f, 23.0f, 18.0f);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		g.DrawString(
			m_RuleID.c_str(),
			3,
			&myFont,
			layoutRect,
			&format,
			&blackBrush);

		// Draw layoutRect.
		g.DrawRectangle(&Pen(Color::Red, 3), layoutRect);


	}
	CCQArea::draw(g, pView);
}

BOOL CCQAreaCircle::isIn(PointF pt)
{
	double dist = CUtil::distance(pt.X, pt.Y, m_fPt.X, m_fPt.Y);
	
	// 중심점에서의 거리가 원의 버퍼반지름보다 작으면
	if (dist <= m_fRadius[BFZ])	return TRUE;
	return FALSE;
}
// TRJTYPE1 : pt에서 내부 버퍼존과 CQ존 사이로 진입하는 점을 찾는다.
PointF CCQAreaCircle::getApproachPoint(PointF pt)
{
	double dist = CUtil::distance(pt.X, pt.Y, m_fPt.X, m_fPt.Y) - m_fRadius[1];

	// pt를 중심으로 반지름 dist인 원에 있으면서 pArea의 버퍼존에도 있는 점을 찾기 위해 우선 두 원의 교점을 구한다.
	PointF ip[2];
	CUtil::circle_circle_Intersect(pt.X, pt.Y, dist, m_fPt.X, m_fPt.Y, m_fRadius[0], ip);

	// 두 점을 이은 선 위의 1/4지점과 3/4지점 사이의 임의의 점을 선택한다.
	long td = CUtil::getRandomLong(0, 50) + 25;

	return CUtil::getOffsetPoint(ip[0], ip[1], td);
}

// bBound가 TRUE면 현재 화면 지도 MBR과 만나는 점을 리턴
// 그렇지 않으면 Area외부 임의의 점을 리턴
PointF CCQAreaCircle::getRetreatPoint(PointF pt, double angle, BOOL bBound)
{
	if (bBound)	return CUtil::getPoint(pt, m_mapRect, angle);

	double rate = 2.0 + (double)CUtil::getRandomLong(1, 100) / 100.0;
	double dist = m_fRadius[BFZ] * rate;

	return CUtil::getPoint(pt, dist, angle);
}

// TRJTYPE5
PointF CCQAreaCircle::getDetourPoint(vector<PointF>& vec)
{
	int size = (int)vec.size();
	if (size == 1)
	{
		// 접점중의 하나를 선택해서 리턴
		PointF tp[2];
		CUtil::getTangent(tp, vec[0], m_fPt, m_fRadius[BFZ]);
		int idx = CUtil::getRandomLong(0, 1);
		double angle = CUtil::getAngle(m_fPt, tp[idx]) + (CUtil::getRandomLong(0, 20) - 10);
		double rate = 1.0 + (double)CUtil::getRandomLong(1, 5) / 10.0;
		double dist = m_fRadius[BFZ] * rate;
		return CUtil::getPoint(m_fPt, dist, angle);
	}

	double dist = CUtil::distance(vec[size - 2].X, vec[size - 2].Y, vec[size - 1].X, vec[size - 1].Y);
	double ang = CUtil::getAngle(vec[size - 2], vec[size - 1]);
	ang += (CUtil::getRandomLong(0, 40) - 20);	// +-20도 이내의 각도 변경을 준다.

	PointF ret = CUtil::getPoint(vec[size - 1], dist, ang);

	return ret;
}

#pragma endregion

//------------------------------------------------------------------------------------

#pragma region CQAreaArc

CCQAreaArc::CCQAreaArc(RectF mapRect, REAL x, REAL y, REAL fAngle1, REAL fAngle2, REAL fRadius1, REAL fRadius2)
	:CCQArea(_SONAR_, mapRect, x, y, fRadius1, fRadius2)
	, m_fAngle1(fAngle1)
	, m_fAngle2(fAngle2)
{
	REAL minx, miny, maxx, maxy;
	minx = maxx = x;
	miny = maxy = y;

	PointF p[2];
	p[0] = CUtil::getPoint(m_fPt, fRadius1, fAngle1 - fAngle2 / 2.0f);
	p[1] = CUtil::getPoint(m_fPt, fRadius1, fAngle1 + fAngle2 / 2.0f);

	for (int i = 0; i < 2; i++)
	{
		if (p[i].X < minx) minx = p[i].X;
		if (p[i].Y < miny) miny = p[i].Y;
		if (maxx < p[i].X) maxx = p[i].X;
		if (maxy < p[i].Y) maxy = p[i].Y;
	}
	m_rectf.X = minx;
	m_rectf.Y = miny;
	m_rectf.Width = maxx - minx;
	m_rectf.Height = maxy - miny;
}

void CCQAreaArc::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	/*REAL ang1 = (REAL)m_fAngle1 - m_fAngle2 / 2.0);
	REAL ang2 = (REAL)m_fAngle2;*/
	REAL ang1 = (REAL)m_fAngle1;
	REAL ang2 = (REAL)(m_fAngle2 - m_fAngle1);
	if (m_fAngle1 < 0) {
		REAL ang1 = (REAL)(m_fAngle1 * (-1.0));
		REAL ang2 = (REAL)(m_fAngle2 - m_fAngle1);
	}


	POINT cp = pV->Map2Scr(m_fPt.X, m_fPt.Y);
	RectF rect;
	for (int i = 0; i < 1; i++)
	{
		int r = pV->Map2Scr(m_fRadius[i]);

		rect.X = (REAL)(cp.x - r);
		rect.Y = (REAL)(cp.y - r);
		rect.Width = (REAL)(r * 2);
		rect.Height = (REAL)(r * 2);

		SolidBrush br(Color(96, 225, 225, 225));
		g.FillPie(&br, rect, ang1, ang2);

		Pen pen(Color(0, 0, 0), 2.0);
		if (i == 0)	pen.SetDashStyle(DashStyleDash);
		else		pen.SetDashStyle(DashStyleSolid);
		g.DrawPie(&pen, rect, ang1, ang2);

		if ((ang1 < 0 || ang1 > 270) && ang1 < 90)
			cp.x += 25;
		else if (ang1 >= 90 && ang1 < 180)
			cp.y += 25;
		else if (ang1 >= 180 && ang1 < 270)
			cp.x -= 45;
		else
			cp.y -= 45;
		Gdiplus::Font myFont(L"Tahoma", 10);
		RectF layoutRect(cp.x, cp.y, 23.0f, 18.0f);
		StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		SolidBrush blackBrush(Color(255, 0, 0, 0));
		g.DrawString(
			m_RuleID.c_str(),
			3,
			&myFont,
			layoutRect,
			&format,
			&blackBrush);

		// Draw layoutRect.
		g.DrawRectangle(&Pen(Color::Red, 3), layoutRect);
	}
	CCQArea::draw(g, pView);
}

double CCQAreaArc::getStartAngle() {	return (double)(m_fAngle1 - m_fAngle2 / 2.0f);	}
double CCQAreaArc::getEndAngle() {		return (double)(m_fAngle1 + m_fAngle2 / 2.0f);	}

BOOL CCQAreaArc::isIn(PointF pt)
{
	double dist = CUtil::distance(pt.X, pt.Y, m_fPt.X, m_fPt.Y);

	if (dist < m_fRadius[BFZ])
	{
		double ang = CUtil::getAngle(m_fPt, pt);
		//if (getStartAngle() <= ang && ang <= getEndAngle())
		if (m_fAngle1 <= ang && ang <= m_fAngle2)
			return TRUE;
	}
	return FALSE;
}

PointF CCQAreaArc::getApproachPoint(PointF pt)
{
	return CUtil::getRandomPoint(m_fPt, (int)m_fRadius[CQZ], (int)m_fRadius[BFZ], (int)getStartAngle(), (int)getEndAngle());
}

// bBound가 TRUE면 현재 화면 지도 MBR과 만나는 점을 리턴
// 그렇지 않으면 Area외부 임의의 점을 리턴
PointF CCQAreaArc::getRetreatPoint(PointF pt, double angle, BOOL bBound)
{
	if (bBound)	return CUtil::getPoint(pt, m_mapRect, angle);

	double rate = 1.0 + (double)CUtil::getRandomLong(1, 100) / 100.0;
	double dist = m_fRadius[BFZ] * rate;

	return CUtil::getPoint(pt, dist, angle);
}

// TRJTYPE5
PointF CCQAreaArc::getDetourPoint(vector<PointF>& vec)
{
	double td = sqrt(pow(m_rectf.Width / 2.0f, 2) + pow(m_rectf.Height / 2.0f, 2));
	PointF cp = getMBRCenterPoint();

	int size = (int)vec.size();
	if (size == 1)
	{
		// 접점중의 하나를 선택해서 리턴
		PointF tp[2];
		CUtil::getTangent(tp, vec[0], cp, m_fRadius[BFZ]/2.0f);
		int idx = CUtil::getRandomLong(0, 1);
		double angle = CUtil::getAngle(m_fPt, tp[idx]) + (CUtil::getRandomLong(0, 20) - 10);
		double rate = 1.0 + (double)CUtil::getRandomLong(1, 5) / 10.0;
		double dist = m_fRadius[BFZ] * rate;
		return CUtil::getPoint(m_fPt, dist, angle);
	}

	double dist = CUtil::distance(vec[size - 2].X, vec[size - 2].Y, vec[size - 1].X, vec[size - 1].Y);
	double ang = CUtil::getAngle(vec[size - 2], vec[size - 1]);
	ang += (CUtil::getRandomLong(0, 40) - 20);	// +-20도 이내의 각도 변경을 준다.

	return CUtil::getPoint(vec[size - 1], dist, ang);
}

#pragma endregion

//------------------------------------------------------------------------------------

#pragma region CQAreaRect

// x, y는 Rectagnle의 중심좌표
CCQAreaRect::CCQAreaRect(RectF mapRect, REAL x, REAL y, REAL BFZw, REAL BFZh)
	:CCQArea(_RECT_, mapRect, x, y)
{
	m_rectf.X = x - BFZw / 2.0f;
	m_rectf.Y = y - BFZh / 2.0f;
	m_rectf.Width = BFZw;
	m_rectf.Height = BFZh;
}

void CCQAreaRect::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	POINT pt = pV->Map2Scr(m_rectf.X, m_rectf.Y);

	RectF rect;
	rect.X = (REAL)pt.x;
	rect.Y = (REAL)pt.y;
	rect.Width = (REAL)pV->Map2Scr(m_rectf.Width);
	rect.Height = (REAL)pV->Map2Scr(m_rectf.Height);

	SolidBrush br(Color(96, 225, 225, 225));
	g.FillRectangle(&br, rect);

	Pen pen(Color(0, 0, 0), 2.0);
	pen.SetDashStyle(DashStyleDash);
	g.DrawRectangle(&pen, rect);
}

BOOL CCQAreaRect::isIn(PointF pt)
{
	if (pt.X < m_rectf.X)	return FALSE;
	if (pt.Y < m_rectf.Y)	return FALSE;
	if (m_rectf.X + m_rectf.Width < pt.X)	return FALSE;
	if (m_rectf.Y + m_rectf.Height < pt.Y)	return FALSE;
	return TRUE;
}

// MBR의 임의의 지점을 그냥 선택한다.
PointF CCQAreaRect::getApproachPoint(PointF pt)
{
	PointF tp;
	int tx = CUtil::getRandomLong(0, (int)m_rectf.Width);
	int ty = CUtil::getRandomLong(0, (int)m_rectf.Height);

	TRACE("%.2f,%.2f - %d,%d\n", m_rectf.Width, m_rectf.Height, tx, ty);

	tp.X = m_rectf.X + tx;
	tp.Y = m_rectf.Y + ty;

	return tp;
}

// bBound가 TRUE면 현재 화면 지도 MBR과 만나는 점을 리턴
// 그렇지 않으면 Area외부 임의의 점을 리턴
PointF CCQAreaRect::getRetreatPoint(PointF pt, double angle, BOOL bBound)
{
	if (bBound)	return CUtil::getPoint(pt, m_mapRect, angle);

	double dist = 0.0;
	if (m_rectf.Width < m_rectf.Height)	dist = m_rectf.Height;
	else								dist = m_rectf.Width;

	double rate = 1.0 + (double)CUtil::getRandomLong(1, 100) / 100.0;
	dist *= rate;

	return CUtil::getPoint(pt, dist, angle);
}

PointF CCQAreaRect::getDetourPoint(vector<PointF>& vec)
{
	double dist = 0.0, ang = 0.0;
	int size = (int)vec.size();
	if (size == 1)
	{
		// 네점과의 각도중 제일 작거나 제일 큰 각도를 선택
		// 선택된 점과의 거리를 구해서 거리를 추가
		double maxangle, minangle;
		int maxidx, minidx;
		PointF pt[4];

		for (int i = 0; i < 4; i++)
		{
			if (i == 0)
			{
				pt[i].X = m_rectf.X, pt[i].Y = m_rectf.Y;
				maxangle = minangle = CUtil::getAngle(vec[0], pt[i]);
				maxidx = minidx = 0;
				continue;
			}
			else if (i == 1)
			{
				pt[i].X = m_rectf.X + m_rectf.Width, pt[i].Y = m_rectf.Y;
			}
			else if (i == 2)
			{
				pt[i].X = m_rectf.X + m_rectf.Width, pt[i].Y = m_rectf.Y + m_rectf.Height;
			}
			else
			{
				pt[i].X = m_rectf.X + m_rectf.Width, pt[i].Y = m_rectf.Y + m_rectf.Height;
			}
			ang = CUtil::getAngle(vec[0], pt[i]);
			if (maxangle < ang)
			{
				maxangle = ang;
				maxidx = i;
			}
			if (ang < minangle)
			{
				minangle = ang;
				minidx = i;
			}
		}
		int fg = CUtil::getRandomLong(0, 1);
		if (fg == 0)
		{
			double td = 1.0 + (double)CUtil::getRandomLong(1, 5) / 10.0;
			dist = CUtil::distance(vec[0].X, vec[0].Y, pt[minidx].X, pt[minidx].Y) * td;
			double ta = (double)CUtil::getRandomLong(1, 10);
			ang = minangle - ta;
		}
		else
		{
			double td = 1.0 + (double)CUtil::getRandomLong(1, 5) / 10.0;
			dist = CUtil::distance(vec[0].X, vec[0].Y, pt[maxidx].X, pt[maxidx].Y) * td;
			double ta = (double)CUtil::getRandomLong(1, 10);
			ang = maxangle + ta;
		}
		return CUtil::getPoint(vec[0], dist, ang);
	}

	dist = CUtil::distance(vec[size - 2].X, vec[size - 2].Y, vec[size - 1].X, vec[size - 1].Y);
	ang = CUtil::getAngle(vec[size - 2], vec[size - 1]);
	ang += (CUtil::getRandomLong(0, 40) - 20);	// +-20도 이내의 각도 변경을 준다.

	return CUtil::getPoint(vec[size - 1], dist, ang);
}

#pragma endregion