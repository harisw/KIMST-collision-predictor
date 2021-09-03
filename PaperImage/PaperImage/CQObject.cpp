#include "pch.h"
#include "CQObject.h"
#include "CQArea.h"
#include "PaperImageView.h"
#include "CQDialog.h"
#include "SimulDlg.h"
#include "Util.h"

#pragma region CCQObject

CCQObject::CCQObject(int objtype, int speedtype, PointF pt, CCQArea* pArea, int trjtype, int kmh, BOOL bFriend)
	: m_objtype(objtype)
	, m_speedtype(speedtype)
	, m_bFriend(bFriend)
	, m_kmh(kmh)
	, m_initPoint(PointF(pt.X, pt.Y))
	, m_trjtype(trjtype)
{
	setAbstractPath(pt, pArea, trjtype);

	// �ӵ��� meter������ ����
	double ms = (double)kmh * 1000.0 / 3600.0;	// m/s
	setConcreteDrawPath(ms);
	setConcreteMovePath(ms);

	setResource();
	setMbr();
}

CCQObject::~CCQObject()
{
	delete m_pPathPen;
	delete m_pPen;
	delete m_pBrush;
	delete m_pArrowBrush;
}

//----------------------------------------------------------------------------------------------------------------------

// m_ConcreteDrawPath�� �̿��ؼ� ����� MBR�� �����Ѵ�.
void CCQObject::setMbr()
{
	int size = (int)m_ConcreteDrawPath.size();
	if (size <= 0)	return;

	REAL minx, miny, maxx, maxy;
	minx = maxx = m_ConcreteDrawPath[0].X;
	miny = maxy = m_ConcreteDrawPath[0].Y;

	for (int i = 1; i < size; i++)
	{
		if (m_ConcreteDrawPath[i].X < minx)	minx = m_ConcreteDrawPath[i].X;
		if (m_ConcreteDrawPath[i].Y < miny)	miny = m_ConcreteDrawPath[i].Y;
		if (maxx < m_ConcreteDrawPath[i].X)	maxx = m_ConcreteDrawPath[i].X;
		if (maxy < m_ConcreteDrawPath[i].Y)	maxy = m_ConcreteDrawPath[i].Y;
	}
	m_mbr = RectF(minx, miny, maxx - minx, maxy - miny);
}

int CCQObject::getMoveCount()
{
	return (int)m_ConcreteMovePath.size();
}

void CCQObject::setResource()
{
	if (m_objtype == RAO)
	{
		if (m_bFriend)
		{
			m_pPathPen = new Pen(Color(0, 0, 255));
			m_pBrush = new SolidBrush(Color(0, 0, 255));
		}
		else
		{
			m_pPathPen = new Pen(Color(255, 0, 0));
			m_pBrush = new SolidBrush(Color(255, 0, 0));
		}
		m_pPathPen->SetDashStyle(DashStyle::DashStyleSolid);
		//m_pPathPen->SetWidth(2.0f);
	}
	else if (m_objtype == RSVO)
	{
		if (m_bFriend)
		{
			m_pPathPen = new Pen(Color(0, 0, 228));
			m_pBrush = new SolidBrush(Color(0, 0, 228));
		}
		else
		{
			m_pPathPen = new Pen(Color(228, 0, 0));
			m_pBrush = new SolidBrush(Color(228, 0, 0));
		}
		m_pPathPen->SetDashStyle(DashStyle::DashStyleDash);
	}
	else if (m_objtype == RFVO)
	{
		if (m_bFriend)
		{
			m_pPathPen = new Pen(Color(0, 0, 192));
			m_pBrush = new SolidBrush(Color(0, 0, 192));
		}
		else
		{
			m_pPathPen = new Pen(Color(192, 0, 0));
			m_pBrush = new SolidBrush(Color(192, 0, 0));
		}
		m_pPathPen->SetDashStyle(DashStyle::DashStyleDashDotDot);
	}
	else
	{
		if (m_bFriend)
		{
			m_pPathPen = new Pen(Color(0, 0, 128));
			m_pBrush = new SolidBrush(Color(0, 0, 128));
		}
		else
		{
			m_pPathPen = new Pen(Color(128, 0, 0));
			m_pBrush = new SolidBrush(Color(128, 0, 0));
		}
		m_pPathPen->SetDashStyle(DashStyle::DashStyleDot);
	}
	m_pPen = new Pen(Color(128, 128, 128));
	m_pArrowBrush = new SolidBrush(Color(255, 255, 255));
}

// ȭ��ǥ �׸���
void CCQObject::drawarrow(Graphics& g, CView* pView, vector<PointF>* path)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	int size = (int)path->size();
	if (size < 2)	return;

	vector<POINT> vec;
	for (int i = 0; i < size; i++)
	{
		PointF t = path->at(i);
		vec.push_back(pV->Map2Scr(t.X, t.Y));
	}

	int offset = ARROWWIDTH * 2;

	vector <CRect> rectVector;
	CRect rect, tr;

	// ���� �������� ȭ��ǥ�� �׸��� �ʵ��� ��ŵ�ϱ� ����.
	rect.left = vec[0].x - offset;
	rect.right = vec[0].x + offset;
	rect.top = vec[0].y - offset;
	rect.bottom = vec[0].y+ offset;
	rect.NormalizeRect();
	rectVector.push_back(rect);

	PointF pt;
	for (int i = 1; i < size; i++)
	{
		pt.X = (REAL)((vec[i].x + vec[i - 1].x) / 2);
		pt.Y = (REAL)((vec[i].y + vec[i - 1].y) / 2);

		rect.left = (int)(pt.X - offset);
		rect.right = (int)(pt.X + offset);
		rect.top = (int)(pt.Y - offset);
		rect.bottom = (int)(pt.Y + offset);
		rect.NormalizeRect();

		tr.SetRectEmpty();
		int j, rs = (int)rectVector.size();
		for (j = 0; j < rs; j++)
		{
			if (tr.IntersectRect(rectVector[j], rect) != 0)	break;
		}

		// ������ �׷��� ȭ��ǥ�� �������� �����Ͱ� ������ �׸��� ����.
		if (j < rs)	continue;
		rectVector.push_back(rect);

		double rad = atan2(vec[i].y - vec[i - 1].y, vec[i].x - vec[i - 1].x);

		PointF arrow_pos[3];
		arrow_pos[0] = pt;
		double thetha = PI / 8.0; //ȭ��ǥ ���� �׸� �� �ʿ��� ����. 30��

		arrow_pos[1].X = (REAL)(pt.X - ARROWWIDTH * sin(PI / 2.0 - (thetha + rad)));
		arrow_pos[1].Y = (REAL)(pt.Y - ARROWWIDTH * cos(PI / 2.0 - (thetha + rad)));
		arrow_pos[2].X = (REAL)(pt.X - ARROWWIDTH * cos(rad - thetha));
		arrow_pos[2].Y = (REAL)(pt.Y - ARROWWIDTH * sin(rad - thetha));

		g.FillPolygon(m_pArrowBrush, arrow_pos, 3);
		g.DrawPolygon(m_pPen, arrow_pos, 3);
	}
}

void CCQObject::drawobject(Graphics& g, CView* pView, int idx)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	int r = 2;

	int size = (int)m_ConcreteMovePath.size();
	if (size <= idx)	idx = size - 1;

	POINT p1 = pV->Map2Scr(m_ConcreteMovePath[idx].X, m_ConcreteMovePath[idx].Y);
	g.FillEllipse(m_pBrush, p1.x - r, p1.y - r, r * 2, r * 2);
	g.DrawEllipse(m_pPen, p1.x - r, p1.y - r, r * 2, r * 2);
	g.DrawEllipse(m_pPen, p1.x - r * 2, p1.y - r * 2, r * 4, r* 4);
}

void CCQObject::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;

	if (!pV->m_pSimulDlg->m_bPlay)	// �ùķ��̼� ��尡 �ƴ� ���
	{
		int size = (int)m_AbstractPath.size();

		if (pV->m_pCQDlg->m_bDrawPath)	// ��� �׸��� ���
		{
			for (int i = 0; i < size - 1; i++)
			{
				POINT p1 = pV->Map2Scr(m_AbstractPath[i].X, m_AbstractPath[i].Y);
				POINT p2 = pV->Map2Scr(m_AbstractPath[i + 1].X, m_AbstractPath[i + 1].Y);

				g.DrawLine(m_pPathPen, p1.x, p1.y, p2.x, p2.y);
			}
			if (pV->m_bShowDot)
			{
				for (int i = 1; i < size; i++)
				{
					POINT p1 = pV->Map2Scr(m_AbstractPath[i].X, m_AbstractPath[i].Y);
					g.FillEllipse(m_pBrush, p1.x - 3, p1.y - 3, 6, 6);
				}
			}
			drawarrow(g, pView, &m_AbstractPath);
		}
		drawobject(g, pView, 0);
	}
	else	// �ùķ��̼� ����� ���
	{
		int size = (int)m_ConcreteMovePath.size();

		if (!pV->m_pSimulDlg->m_bDrawPath)
		{
			if (size <= pV->m_pSimulDlg->m_nIdx)	return;
			drawobject(g, pView, pV->m_pSimulDlg->m_nIdx);
			return;
		}

		int s = pV->m_pSimulDlg->m_nIdx - 10;
		if (s < 0)	s = 0;

		int e = pV->m_pSimulDlg->m_nIdx;
		if (size <= e)	e = size - 1;

		for (int i = s; i <= e; i++)
		{
			if (i == 0) continue;

			POINT p1 = pV->Map2Scr(m_ConcreteMovePath[i - 1].X, m_ConcreteMovePath[i - 1].Y);
			POINT p2 = pV->Map2Scr(m_ConcreteMovePath[i].X, m_ConcreteMovePath[i].Y);

			g.DrawLine(m_pPathPen, p1.x, p1.y, p2.x, p2.y);
		}
		drawobject(g, pView, e);
	}
}

//----------------------------------------------------------------------------------------------------------------------

// ����� - ������ - �������� ������������ �����ؼ� ���� ����
// �̶� �������� ������ ������ R�� ������ ��
void CCQObject::setConcreteDrawPath(double speed)
{
	m_ConcreteDrawPath.clear();

	int size = (int)m_AbstractPath.size();
	if (size <= 2)
	{
		m_ConcreteDrawPath.assign(m_AbstractPath.begin(), m_AbstractPath.end());
		return;
	}

	int gf = 1;
	if (m_objtype == RAO) gf = CUtil::getRandomLong(1, 5);

	double r = CUtil::getRadius(speed, gf);		// speed�϶� g_Force�� gf�� ��� ȸ�� �ݰ� (meter)
	if (r <= 0.0)
	{
		m_ConcreteDrawPath.assign(m_AbstractPath.begin(), m_AbstractPath.end());
		return;
	}

	m_ConcreteDrawPath.push_back(m_AbstractPath[0]);
	for (int i = 1; i < size - 1; i++)
	{
		double ad0 = CUtil::distance(m_AbstractPath[i - 1].X, m_AbstractPath[i - 1].Y, m_AbstractPath[i].X, m_AbstractPath[i].Y);
		double ad1 = CUtil::distance(m_AbstractPath[i].X, m_AbstractPath[i].Y, m_AbstractPath[i + 1].X, m_AbstractPath[i + 1].Y);
		if (ad0 <= r || ad1 <= r)
		{
			m_ConcreteDrawPath.push_back(m_AbstractPath[i]);
			continue;
		}

		double td1 = CUtil::distance(m_AbstractPath[i - 1].X, m_AbstractPath[i - 1].Y, m_AbstractPath[i].X, m_AbstractPath[i].Y);
		if (r <= td1)
		{
			m_ConcreteDrawPath.push_back(m_AbstractPath[i]);
			continue;
		}

		PointF tp[2];
		// ���� m_AbstractPath[i], ������ r�� ���� �� m_AbstractPath[i-1]���� ������ tp�� ����
		CUtil::getTangent(tp, m_AbstractPath[i-1], m_AbstractPath[i], r);

		// ������ �Ÿ��� �� ��ǥ�� �߰� �����.
		double d0 = CUtil::distance(m_AbstractPath[i+1].X, m_AbstractPath[i+1].Y, tp[0].X, tp[0].Y);
		double d1 = CUtil::distance(m_AbstractPath[i+1].X, m_AbstractPath[i+1].Y, tp[1].X, tp[1].Y);

		PointF ap1 = (d0 < d1) ? tp[1] : tp[0];

		// ���� ������
		CUtil::getTangent(tp, m_AbstractPath[i+1], m_AbstractPath[i], r);

		// �������� �Ÿ��� �� ��ǥ�� �߰� �����.
		double d2 = CUtil::distance(m_AbstractPath[i-1].X, m_AbstractPath[i-1].Y, tp[0].X, tp[0].Y);
		double d3 = CUtil::distance(m_AbstractPath[i-1].X, m_AbstractPath[i-1].Y, tp[1].X, tp[1].Y);

		PointF ap2 = (d2 < d3) ? tp[1] : tp[0];

		vector<PointF> tarr;
		tarr.push_back(m_AbstractPath[i - 1]);
		tarr.push_back(m_AbstractPath[i]);
		tarr.push_back(m_AbstractPath[i + 1]);
		double area = CUtil::getArea(tarr);

		tarr.clear();
		CUtil::getRotatePoint(m_AbstractPath[1], ap1, ap2, area, 10, tarr);

		int tsize = (int)tarr.size();
		for (int i = 0; i < tsize; i++)
		{
			m_ConcreteDrawPath.push_back(tarr[i]);
		}
	}
	m_ConcreteDrawPath.push_back(m_AbstractPath[size-1]);
}

void CCQObject::setConcreteMovePath(double speed)
{
	m_ConcreteMovePath.clear();
	m_ConcreteMovePath.push_back(m_ConcreteDrawPath[0]);

	double offsetlen = speed * (double)MOVEINTERVAL;	// 60�ʰ���

	double pathlen = CUtil::distance(m_ConcreteDrawPath);
	int count = (int)(pathlen / offsetlen) + 1;

	if (offsetlen == 0.0)
		int ddd = 0;

	for (int i = 1; i < count; i++)
	{
		PointF pt = CUtil::getOffsetPoint(m_ConcreteDrawPath, offsetlen * i);
		m_ConcreteMovePath.push_back(pt);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void CCQObject::setAbstractPath(PointF pt, CCQArea* pArea, int trjtype)
{
	// m_AbstractPath�� �ʱ�ȭ�ϰ� ������ pt�� m_AbstractPath�� �߰��Ѵ�.
	m_AbstractPath.clear();
	m_AbstractPath.push_back(pt);

	PointF tp;
	if (trjtype != TRJTYPE5)
	{
		// ��ȸ�� �ƴϸ� �ϴ� ������ pt�� ������ ������ tp�� m_AbstractPath�� �߰��Ѵ�.
		tp = pArea->getApproachPoint(pt);
		m_AbstractPath.push_back(tp);
	}

	if (trjtype == TRJTYPE1)	// ������ ��� �׳� ����
		return;
	else if (trjtype == TRJTYPE2) // ����� ���
	{
		// ������ ������ -20 ~ 20�� ������ ���� ���� ���ؼ� ������ ����
		double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
		tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, TRUE);	// ȭ�� Map MBR�� ������ ��
		m_AbstractPath.push_back(tp);
	}
	else if (trjtype == TRJTYPE3) // U-Turn�� ���
	{
		// ������ ������ 180���� ���� �� -20 ~ 20�� ������ ���� ���� �߰��� ���ؼ� ������ ����
		double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
		angle += 180.0;
		if (360.0 <= angle)	angle -= 360.0;

		tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, TRUE);	// ȭ�� Map MBR�� ������ ��
		m_AbstractPath.push_back(tp);
	}
	else if (trjtype == TRJTYPE4)	// ��� - U-Turn - ���
	{
		// ������ ������ -20 ~ 20�� ������ ���� ���� ���ؼ� ������ ����
		double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
		tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, FALSE);	// CQ ���� �ܺ��� ������ ��
		m_AbstractPath.push_back(tp);

		// �ܺ��� tp�� ���������� CQ ������ ����
		tp = pArea->getApproachPoint(tp);
		m_AbstractPath.push_back(tp);

		// ������ ������ -20 ~ 20�� ������ ���� ���� ���ؼ� ������ ����
		angle = CUtil::getAngle(m_AbstractPath[2], m_AbstractPath[3]) + (CUtil::getRandomLong(0, 40) - 20);
		tp = pArea->getRetreatPoint(m_AbstractPath[3], angle, TRUE);	// ȭ�� Map MBR�� ������ ��
		m_AbstractPath.push_back(tp);
	}
	else	// ��ȸ (TRJTYPE5)
	{
		tp = pArea->getDetourPoint(m_AbstractPath);
		m_AbstractPath.push_back(tp);

		// ������ ������ -20 ~ 20�� ������ ���� ���� ���ؼ� ������ ����
		double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);

		tp = pArea->getRetreatPoint(tp, angle, TRUE);	// ȭ�� Map MBR�� ������ ��
		m_AbstractPath.push_back(tp);
	}
}

//----------------------------------------------------------------------------------------------------------------------

#pragma endregion