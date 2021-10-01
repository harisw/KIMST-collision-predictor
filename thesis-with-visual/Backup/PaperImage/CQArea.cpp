#include "pch.h"
#include "CQArea.h"
#include "resource.h"
#include "Util.h"
#include "PaperImageView.h"
#include <math.h>

//------------------------------------------------------------------------------------

#pragma region CQArea

CCQArea::CCQArea(int CQType, RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy)
	: m_CQType(CQType)
	, m_mapRect(mapRect)
	, m_fPt(PointF(x, y))
	, m_fRadius{ fRadius1}
{
	if (CQType != _RECT_)
	{
		m_fRadius[BFZ] = fRadius1;									// ���۹�����
	}
	m_fCurrentPt = m_fPt;
	m_vx = _vx;
	m_vy = _vy;
	//m_vy = 0;
	getEndPoint();
	getMovingAngle();
	getBTPoints();
}

// CQ������ �Լ���ġ�� �̹����� �׸���.
void CCQArea::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	SolidBrush br(Color(0, 0, 0));
	Pen pen(Color(0, 0, 0));
	pen.SetWidth(2.0f);

	POINT pt = pV->Map2Scr(m_fCurrentPt.X, m_fCurrentPt.Y);
	int w = pV->Map2Scr(SHIPWIDTH);
	int h = pV->Map2Scr(SHIPLENGTH);

	// �Լ� ���� �ʹ� ������ �׸��� �ʴ´�.
	if (0 < w)	g.FillEllipse(&br, pt.x - w / 2, pt.y - h / 2, w, h);

	// �Լ��� ��ġ�� ����ǥ���Ѵ�.
	g.DrawLine(&pen, pt.x - 20, pt.y, pt.x + 20, pt.y);
	g.DrawLine(&pen, pt.x, pt.y - 20, pt.x, pt.y + 20);
}

// tp���� CQArea mbr�� ���� ����� �Ÿ��� ����
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

// MBR�� �߽����� �����Ѵ�. (_RADAR_ Ÿ���� m_fPt�� ����)
PointF CCQArea::getMBRCenterPoint()
{
	PointF pt;
	pt.X = m_rectf.X + m_rectf.Width / 2.0f;
	pt.Y = m_rectf.Y + m_rectf.Height / 2.0f;
	return pt;
}

void CCQArea::moveOurVessel()
{
	m_fCurrentPt.X += m_vx;
	m_fCurrentPt.Y += m_vy;
	return;
}

void CCQArea::getEndPoint()
{
	m_fEndPt.X = m_fPt.X + ((SIMU_TIME+1) * m_vx);
	m_fEndPt.Y = m_fPt.Y + ((SIMU_TIME + 1) * m_vy);
	return;
}

void CCQArea::getBTPoints()
{
	PointF p1, p2, p3, p4;
	double d = BUFFER_DIST;
	//if (m_vx == 0) {
		if (m_fEndPt.Y > m_fPt.Y) {
			p1.X = m_fPt.X - d;
			p1.Y = m_fPt.Y - d;

			p2.X = m_fPt.X + d;
			p2.Y = m_fPt.Y - d;

			p3.X = m_fEndPt.X - d;
			p3.Y = m_fEndPt.Y + d;

			p4.X = m_fEndPt.X + d;
			p4.Y = m_fEndPt.Y + d;
		}
		else {
			p1.X = m_fPt.X - d;
			p1.Y = m_fPt.Y + d;

			p2.X = m_fPt.X + d;
			p2.Y = m_fPt.Y + d;

			p3.X = m_fEndPt.X - d;
			p3.Y = m_fEndPt.Y - d;

			p4.X = m_fEndPt.X + d;
			p4.Y = m_fEndPt.Y - d;
		}
	/*}
	else if (m_vy == 0) {
		if (m_fEndPt.X > m_fPt.X) {
			p1.X = m_fPt.X - d;
			p1.Y = m_fPt.Y + d;

			p2.X = m_fPt.X - d;
			p2.Y = m_fPt.Y - d;

			p3.X = m_fEndPt.X + d;
			p3.Y = m_fEndPt.Y + d;

			p4.X = m_fEndPt.X + d;
			p4.Y = m_fEndPt.Y - d;
		}
		else {
			p1.X = m_fPt.X + d;
			p1.Y = m_fPt.Y - d;

			p2.X = m_fPt.X + d;
			p2.Y = m_fPt.Y + d;

			p3.X = m_fEndPt.X - d;
			p3.Y = m_fEndPt.Y - d;

			p4.X = m_fEndPt.X - d;
			p4.Y = m_fEndPt.Y + d;
		}
	}*/
	m_BTPoints.insert(m_BTPoints.end(), { p1, p2, p3, p4 });

	if (m_vx != 0 && m_vy != 0) {
		if (theta > 0) {
			for (int j = 0; j < m_BTPoints.size(); j++) {
				m_BTPoints[j].X = m_BTPoints[j].X * cos(theta) - m_BTPoints[j].Y * sin(theta) + 100.0f;
				m_BTPoints[j].Y = m_BTPoints[j].X * sin(theta) + m_BTPoints[j].Y * cos(theta);
			}
		}
	}
	
}

void CCQArea::getMovingAngle()
{
	double xGap = m_fEndPt.X - m_fPt.X;
	double yGap = m_fEndPt.Y - m_fPt.Y;
	theta = atan2(yGap, xGap);
}

#pragma endregion

//------------------------------------------------------------------------------------

#pragma region CQAreaCircle

CCQAreaCircle::CCQAreaCircle(RectF mapRect, REAL x, REAL y, REAL fRadius1, REAL _vx, REAL _vy)
	:CCQArea(_RADAR_, mapRect, x, y, fRadius1, _vx, _vy)
{
	m_rectf.X = x - m_fRadius[BFZ];
	m_rectf.Y = y - m_fRadius[BFZ];
	m_rectf.Width = m_fRadius[BFZ] * 2.0f;
	m_rectf.Height = m_fRadius[BFZ] * 2.0f;

}

void CCQAreaCircle::drawBT(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;
	int r = pV->Map2Scr(m_fRadius[0]);

	POINT cp1 = pV->Map2Scr(m_BTPoints[0].X, m_BTPoints[0].Y);
	POINT cp2 = pV->Map2Scr(m_BTPoints[3].X, m_BTPoints[3].Y);

	RectF layoutRect(cp1.x, cp1.y , abs(cp2.x - cp1.x), abs(cp2.y - cp1.y));

	// Draw layoutRect.
	//g.TranslateTransform(100.0f, 0.0f, MatrixOrder::MatrixOrderAppend);
	//g.TranslateTransform(m_fPt.X, m_fPt.Y);
	//g.RotateTransform(45.0f);
	//g.TranslateTransform(m_BTPoints[0].X, m_BTPoints[0].Y);

	g.DrawRectangle(&Pen(Color::Red, 2.0f), layoutRect);
}
void CCQAreaCircle::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	POINT cp = pV->Map2Scr(m_fCurrentPt.X, m_fCurrentPt.Y);
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

	}

	rect.Width = 5;
	rect.Height = 5;

	SolidBrush br(Color(96, 225, 0, 0));
	for (int j = 0; j < m_BTPoints.size(); j++) {
		cp = pV->Map2Scr(m_BTPoints[j].X, m_BTPoints[j].Y);
		rect.X = (REAL)(cp.x);
		rect.Y = (REAL)(cp.y);
		
		g.FillEllipse(&br, rect);
		Pen pen(Color(0, 0, 0), 2.0);
		pen.SetDashStyle(DashStyleSolid);
		g.DrawEllipse(&pen, rect);
	}
	
	//drawBT(g, pView);
	CCQArea::draw(g, pView);
}

BOOL CCQAreaCircle::isIn(PointF pt)
{
	double dist = CUtil::distance(pt.X, pt.Y, m_fCurrentPt.X, m_fCurrentPt.Y);
	
	// �߽��������� �Ÿ��� ���� ���۹��������� ������
	if (dist <= m_fRadius[BFZ])	return TRUE;
	return FALSE;
}
// TRJTYPE1 : pt���� ���� �������� CQ�� ���̷� �����ϴ� ���� ã�´�.
PointF CCQAreaCircle::getApproachPoint(PointF pt)
{
	double dist = CUtil::distance(pt.X, pt.Y, m_fPt.X, m_fPt.Y) - m_fRadius[1];

	// pt�� �߽����� ������ dist�� ���� �����鼭 pArea�� ���������� �ִ� ���� ã�� ���� �켱 �� ���� ������ ���Ѵ�.
	PointF ip[2];
	CUtil::circle_circle_Intersect(pt.X, pt.Y, dist, m_fPt.X, m_fPt.Y, m_fRadius[0], ip);

	// �� ���� ���� �� ���� 1/4������ 3/4���� ������ ������ ���� �����Ѵ�.
	long td = CUtil::getRandomLong(0, 50) + 25;

	return CUtil::getOffsetPoint(ip[0], ip[1], td);
}

// bBound�� TRUE�� ���� ȭ�� ���� MBR�� ������ ���� ����
// �׷��� ������ Area�ܺ� ������ ���� ����
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
		// �������� �ϳ��� �����ؼ� ����
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
	ang += (CUtil::getRandomLong(0, 40) - 20);	// +-20�� �̳��� ���� ������ �ش�.

	PointF ret = CUtil::getPoint(vec[size - 1], dist, ang);

	return ret;
}

#pragma endregion