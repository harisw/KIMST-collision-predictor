#include "pch.h"
#include "CQObject.h"
#include "CQArea.h"
#include "PaperImageView.h"
#include "PaperImageDoc.h"
#include "CQDialog.h"
#include "SimulDlg.h"
#include "Util.h"

#pragma region CCQObject

CCQObject::CCQObject(int objtype, int speedtype, PointF pt, CCQArea* pArea, int trjtype, double _vx, double _vy)
	: m_objtype(objtype)
	, m_speedtype(speedtype)
	, m_initPoint(PointF(pt.X, pt.Y))
	, m_trjtype(trjtype)
	, m_vx(_vx)
	, m_vy(_vy)
{
	setAbstractPath(pt, pArea, trjtype);

	// 속도는 meter단위로 변경
	//setConcreteDrawPath(ms);
	setConcreteMovePath();

	setResource();
	setMbr();
}

CCQObject::~CCQObject()
{
	delete m_pPathPen;
	delete m_pPen;
	delete m_pBrush;
	delete m_pArrowBrush;
	delete m_pFocusedPen;
}

//----------------------------------------------------------------------------------------------------------------------

// m_ConcreteDrawPath를 이용해서 경로의 MBR을 설정한다.
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
	m_pPathPen = new Pen(Color(128, 0, 0));
	m_pBrush = new SolidBrush(Color(128, 0, 0));

	m_pPathPen->SetDashStyle(DashStyle::DashStyleDot);

	m_pPen = new Pen(Color(128, 128, 128));
	m_pArrowBrush = new SolidBrush(Color(255, 255, 255));
	m_pDetectedBrush = new SolidBrush(Color(255, 0, 0));
	m_pFocusedPen = new Pen(Color(0, 0, 228));
}

// 화살표 그리기
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

	// 시작 지점부터 화살표를 그리지 않도록 스킵하기 위함.
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

		// 이전에 그려진 화살표중 겹쳐지는 데이터가 있으면 그리지 않음.
		if (j < rs)	continue;
		rectVector.push_back(rect);

		double rad = atan2(vec[i].y - vec[i - 1].y, vec[i].x - vec[i - 1].x);

		PointF arrow_pos[3];
		arrow_pos[0] = pt;
		double thetha = PI / 8.0; //화살표 끝을 그릴 때 필요한 각도. 30도

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
	if (m_isDetected) {
		pV->updateQueue();
		g.FillEllipse(m_pDetectedBrush, p1.x - r, p1.y - r, r * 2, r * 2);
	}
	else
		g.FillEllipse(m_pBrush, p1.x - r, p1.y - r, r * 2, r * 2);
	g.DrawEllipse(m_pPen, p1.x - r, p1.y - r, r * 2, r * 2);
	g.DrawEllipse(m_pPen, p1.x - r * 2, p1.y - r * 2, r * 4, r* 4);
	
	Gdiplus::Font myFont(L"Tahoma", 10);
	RectF layoutRect(p1.x+5, p1.y-15, 23.0f, 18.0f);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	SolidBrush blackBrush(Color(255, 0, 0, 0));

	// Draw string.
	g.DrawString(
		m_charID.c_str(),
		3,
		&myFont,
		layoutRect,
		&format,
		&blackBrush);

	// Draw layoutRect.
	g.DrawRectangle(&Pen(Color::Black, 3), layoutRect);
}

void CCQObject::draw(Graphics& g, CView* pView)
{
	CPaperImageView* pV = (CPaperImageView*)pView;
	CPaperImageDoc* pDoc = pV->GetDocument();
	m_isDetected = false;
	if (!pV->m_pSimulDlg->m_bPlay)	// 시뮬레이션 모드가 아닌 경우
	{
		string strID = to_string(m_id);
		this->m_charID = wstring(strID.begin(), strID.end());
		int size = (int)m_AbstractPath.size();

		if (pV->m_pCQDlg->m_bDrawPath)	// 경로 그리기 모드
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
	else	// 시뮬레이션 모드인 경우
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
	if (m_isFocused) {
		for (int i = 1; i < m_AbstractPath.size(); i++) {
			POINT p1 = pV->Map2Scr(m_AbstractPath[i - 1].X, m_AbstractPath[i - 1].Y);
			POINT p2 = pV->Map2Scr(m_AbstractPath[i].X, m_AbstractPath[i].Y);

			g.DrawLine(m_pPathPen, p1.x, p1.y, p2.x, p2.y);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

// 출발점 - 경유점 - 도착점을 일정간격으로 분할해서 점을 생성
// 이때 경유점은 임의의 반지름 R을 가지는 원
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

	double r = CUtil::getRadius(speed, gf);		// speed일때 g_Force가 gf인 경우 회전 반경 (meter)
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
		// 중점 m_AbstractPath[i], 반지름 r인 원과 점 m_AbstractPath[i-1]과의 접점을 tp에 저장
		CUtil::getTangent(tp, m_AbstractPath[i-1], m_AbstractPath[i], r);

		// 끝점과 거리가 먼 좌표가 추가 대상임.
		double d0 = CUtil::distance(m_AbstractPath[i+1].X, m_AbstractPath[i+1].Y, tp[0].X, tp[0].Y);
		double d1 = CUtil::distance(m_AbstractPath[i+1].X, m_AbstractPath[i+1].Y, tp[1].X, tp[1].Y);

		PointF ap1 = (d0 < d1) ? tp[1] : tp[0];

		// 끝과 경유점
		CUtil::getTangent(tp, m_AbstractPath[i+1], m_AbstractPath[i], r);

		// 시작점과 거리가 먼 좌표가 추가 대상임.
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

void CCQObject::setConcreteMovePath()
{
	m_ConcreteMovePath.clear();
	m_ConcreteMovePath.push_back(m_ConcreteDrawPath[0]);

	//double offsetlen = speed * (double)MOVEINTERVAL;	// 60초간격

	//double pathlen = CUtil::distance(m_ConcreteDrawPath);
	//int count = (int)(pathlen / offsetlen) + 1;

	//if (offsetlen == 0.0)
	//	int ddd = 0;

	for (int i = 1; i < SIMU_TIME; i++)
	{
		PointF pt;
		pt.X = m_initPoint.X + (i * m_vx);
		pt.Y = m_initPoint.Y + (i * m_vy);
		m_ConcreteMovePath.push_back(pt);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void CCQObject::setAbstractPath(PointF pt, CCQArea* pArea, int trjtype)
{
	// m_AbstractPath를 초기화하고 시작점 pt를 m_AbstractPath에 추가한다.
	m_AbstractPath.clear();
	m_AbstractPath.push_back(pt);

	PointF finalP;
	finalP.X = m_initPoint.X + (SIMU_TIME * m_vx);
	finalP.Y = m_initPoint.Y + (SIMU_TIME * m_vy);
	
	m_AbstractPath.push_back(finalP);
	//PointF tp;
	//if (trjtype != TRJTYPE5)
	//{
	//	// 우회가 아니면 일단 시작점 pt와 영역내 진입점 tp를 m_AbstractPath에 추가한다.
	//	tp = pArea->getApproachPoint(pt);
	//	m_AbstractPath.push_back(tp);
	//}

	//if (trjtype == TRJTYPE1)	// 진입인 경우 그냥 리턴
	//	return;
	//else if (trjtype == TRJTYPE2) // 통과인 경우
	//{
	//	// 직선의 각도에 -20 ~ 20도 사이의 랜덤 값을 더해서 각도를 설정
	//	double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
	//	tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, TRUE);	// 화면 Map MBR과 만나는 점
	//	m_AbstractPath.push_back(tp);
	//}
	//else if (trjtype == TRJTYPE3) // U-Turn인 경우m_char
	//{
	//	// 직선의 각도에 180도를 더한 뒤 -20 ~ 20도 사이의 랜덤 값을 추가로 더해서 각도를 설정
	//	double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
	//	angle += 180.0;
	//	if (360.0 <= angle)	angle -= 360.0;

	//	tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, TRUE);	// 화면 Map MBR과 만나는 점
	//	m_AbstractPath.push_back(tp);
	//}
	//else if (trjtype == TRJTYPE4)	// 통과 - U-Turn - 통과
	//{
	//	// 직선의 각도에 -20 ~ 20도 사이의 랜덤 값을 더해서 각도를 설정
	//	double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);
	//	tp = pArea->getRetreatPoint(m_AbstractPath[1], angle, FALSE);	// CQ 영역 외부의 임의의 점
	//	m_AbstractPath.push_back(tp);

	//	// 외부점 tp를 시작점으로 CQ 진입점 설정
	//	tp = pArea->getApproachPoint(tp);
	//	m_AbstractPath.push_back(tp);

	//	// 직선의 각도에 -20 ~ 20도 사이의 랜덤 값을 더해서 각도를 설정
	//	angle = CUtil::getAngle(m_AbstractPath[2], m_AbstractPath[3]) + (CUtil::getRandomLong(0, 40) - 20);
	//	tp = pArea->getRetreatPoint(m_AbstractPath[3], angle, TRUE);	// 화면 Map MBR과 만나는 점
	//	m_AbstractPath.push_back(tp);
	//}
	//else	// 우회 (TRJTYPE5)
	//{
	//	tp = pArea->getDetourPoint(m_AbstractPath);
	//	m_AbstractPath.push_back(tp);

	//	// 직선의 각도에 -20 ~ 20도 사이의 랜덤 값을 더해서 각도를 설정
	//	double angle = CUtil::getAngle(m_AbstractPath[0], m_AbstractPath[1]) + (CUtil::getRandomLong(0, 40) - 20);

	//	tp = pArea->getRetreatPoint(tp, angle, TRUE);	// 화면 Map MBR과 만나는 점
	//	m_AbstractPath.push_back(tp);
	//}
}

//----------------------------------------------------------------------------------------------------------------------

#pragma endregion