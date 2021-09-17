#include "pch.h"
#include "Util.h"

//==========================================================================================================
queue<string> queue_ev;
queue<string> own_queue_ev;

BOOL CUtil::isEqual(double a, double b, int prec)
{
	int a1, b1, a2, b2;

	a1 = (int)a;
	b1 = (int)b;

	if (a1 != b1) return FALSE;

	a2 = (int)((a - (double)a1) * pow(10, prec) + 0.5);
	b2 = (int)((b - (double)b1) * pow(10, prec) + 0.5);

	if (a2 != b2) return FALSE;

	return TRUE;
}

BOOL CUtil::isEqual(PointF a, PointF b, int prec)
{
	if (isEqual(a.X, b.X, prec) && isEqual(a.Y, b.Y, prec))	return TRUE;
	return FALSE;
}

//==========================================================================================================
// 폴리곤의 면적을 리턴
// 폴리곤이 반시계방향이면 (-), 시계방향이면 (+)가 된다.
double CUtil::getArea(vector<POINT>& ptArray)
{
	long area = 0;
	int size = (int)ptArray.size();
	for (int i = 0; i < size; i++)
	{
		int j = i + 1;
		if (size <= j)	j = 0;

		if (ptArray[i].x == ptArray[j].x && ptArray[i].y == ptArray[j].y)	continue;

		//area += (pX[i]*pY[j] - pY[i]*pX[j]);
		area += (ptArray[i].x * ptArray[j].y - ptArray[i].y * ptArray[j].x);
	}
	return (double)area / 2.0f;
}
double CUtil::getArea(vector<PointF>& ptArray)
{
	double area = 0.0;
	int size = (int)ptArray.size();
	for (int i = 0; i < size; i++)
	{
		int j = i + 1;
		if (size <= j)	j = 0;

		if (isEqual(ptArray[i], ptArray[j])) continue;

		area += (ptArray[i].X * ptArray[j].Y - ptArray[i].Y * ptArray[j].X);
	}
	return (area / 2.0);
}

// 중심점이 cp이고 반지름이 r인 원에 sp로부터 접선을 그었을 때 만나는 두점을 tp에 저장
void CUtil::getTangent(POINT* tp, POINT sp, POINT cp, int r)
{
	// 계산의 편의를 위해서 원의 중심을 0,0으로 잠시 이동
	int x1 = sp.x - cp.x;
	int y1 = sp.y - cp.y;

	if (y1 == 0)
	{
		double x2 = (double)(r * r) / (double)x1;
		double y2 = sqrt(r * r - x2 * x2);
		double y3 = -y2;

		tp[0].x = (int)x2 + cp.x;
		tp[0].y = (int)y2 + cp.y;

		tp[1].x = (int)x2 + cp.x;
		tp[1].y = (int)y3 + cp.y;

		return;
	}
	// ax^2 + bx + c = 0
	double a = (double)(x1 * x1 + y1 * y1);
	double b = -2.0 * x1 * r * r;
	double c = (double)(pow(r, 4) - (r * r * y1 * y1));

	double D = b * b - 4.0 * a * c;	// 판별식

	double x2 = (-b + sqrt(D)) / (2 * a);
	double x3 = (-b - sqrt(D)) / (2 * a);

	double y2 = (r * r - x2 * x1) / y1;
	double y3 = (r * r - x3 * x1) / y1;

	tp[0].x = (int)x2 + cp.x;
	tp[0].y = (int)y2 + cp.y;

	tp[1].x = (int)x3 + cp.x;
	tp[1].y = (int)y3 + cp.y;
}

void CUtil::getTangent(PointF* tp, PointF sp, PointF cp, double r)
{
	// 계산의 편의를 위해서 원의 중심을 0,0으로 잠시 이동
	REAL x1 = sp.X - cp.X;
	REAL y1 = sp.Y - cp.Y;

	if (y1 == 0)
	{
		double x2 = (r * r) / (double)x1;
		double y2 = sqrt(r * r - x2 * x2);
		double y3 = -y2;

		tp[0].X = (int)x2 + cp.X;
		tp[0].Y = (int)y2 + cp.Y;

		tp[1].X = (int)x2 + cp.X;
		tp[1].Y = (int)y3 + cp.Y;

		return;
	}
	// ax^2 + bx + c = 0
	double a = (double)(x1 * x1 + y1 * y1);
	double b = -2.0 * x1 * r * r;
	//double c = (double)(pow(r, 4) - (r * r * y1 * y1));
	double c = (double)(pow(r, 2) * (pow(r, 2) - (y1 * y1)));

	double D = b * b - 4.0 * a * c;	// 판별식

	if (D < 0.0)
	{
		double ddd = distance(0.0f, 0.0f, x1, y1);
		int kkk = 0;
	}

	double x2 = (-b + sqrt(D)) / (2 * a);
	double x3 = (-b - sqrt(D)) / (2 * a);

	double y2 = (r * r - x2 * x1) / y1;
	double y3 = (r * r - x3 * x1) / y1;

	tp[0].X = (int)x2 + cp.X;
	tp[0].Y = (int)y2 + cp.Y;

	tp[1].X = (int)x3 + cp.X;
	tp[1].Y = (int)y3 + cp.Y;
}

double CUtil::getAngle(CPoint p1, CPoint p2)
{
	double ang = RTD(atan2(p2.y - p1.y, p2.x - p1.x));
	if (ang < 0.0)	ang += 360.0;
	return ang;
}
double CUtil::getAngle(PointF p1, PointF p2)
{
	double ang = RTD(atan2(p2.Y - p1.Y, p2.X - p1.X));
	if (ang < 0.0)	ang += 360.0;
	return ang;
}

//==========================================================================================================

double CUtil::distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
}
double CUtil::distance(vector<POINT>& path)
{
	double dist = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		dist += distance(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
	}
	return dist;
}
double CUtil::distance(vector<PointF>& path)
{
	double dist = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		dist += distance(path[i].X, path[i].Y, path[i + 1].X, path[i + 1].Y);
	}
	return dist;
}
double CUtil::distance_line_point(double ptx, double pty, double sptx, double spty, double eptx, double epty, double* pIntersx, double* pIntersy)
{
	double a, b, c, d;
	double tx, ty;
	if (isEqual(sptx, eptx, 4) && isEqual(spty, epty, 4)) // 주어진 선이 점일때
	{
		if (pIntersx) { *pIntersx = sptx, * pIntersy = spty; }
		return distance(sptx, spty, ptx, pty);
	}

	if (isEqual(sptx, eptx, 4) && !isEqual(spty, epty, 4)) // 수직선이면
	{
		tx = sptx;
		if (spty < epty)
		{
			if (pty < spty) ty = spty;
			else if (epty < pty) ty = epty;
			else ty = pty;
		}
		else
		{
			if (pty < epty) ty = epty;
			else if (spty < pty) ty = spty;
			else ty = pty;
		}
		if (pIntersx) { *pIntersx = tx, * pIntersy = ty; }
		return distance(tx, ty, ptx, pty);
	}
	if (!isEqual(sptx, eptx, 4) && isEqual(spty, epty, 4)) // 수평선이면
	{
		ty = spty;
		if (sptx < eptx)
		{
			if (ptx < sptx) tx = sptx;
			else if (eptx < ptx) tx = eptx;
			else tx = ptx;
		}
		else
		{
			if (ptx < eptx) tx = eptx;
			else if (sptx < ptx) tx = sptx;
			else tx = ptx;
		}
		if (pIntersx) { *pIntersx = tx, * pIntersy = ty; }
		return distance(tx, ty, ptx, pty);
	}

	a = (epty - spty) / (eptx - sptx); // 기울기(spt, ept)
	c = -1.0 / a;						// 기울기(pt를 지나며 spt, ept에 직교)

	b = spty - a * sptx;	// y = ax + b
	d = pty - c * ptx;	// y = cx + d

	tx = (d - b) / (a - c);
	ty = a * tx + b;

	if (sptx < eptx)
	{
		if (tx < sptx) { tx = sptx, ty = spty; }
		if (eptx < tx) { tx = eptx, ty = epty; }
	}
	else
	{
		if (tx < eptx) { tx = eptx, ty = epty; }
		if (sptx < tx) { tx = sptx, ty = spty; }
	}
	if (pIntersx) { *pIntersx = tx, * pIntersy = ty; }
	return distance(tx, ty, ptx, pty);
}

//==========================================================================================================

// sp-ep 라인을 100으로 해서 sp에서 경로를 따라서 percent만큼 떨어진 거리의 좌표를 리턴한다.
CPoint CUtil::getOffsetPoint(CPoint sp, CPoint ep, int percent)
{
	int dx = (int)((double)(ep.x - sp.x) * (double)percent / 100.0);
	int dy = (int)((double)(ep.y - sp.y) * (double)percent / 100.0);

	return CPoint((sp.x+dx), (sp.y+dy));
}

PointF CUtil::getOffsetPoint(PointF sp, PointF ep, int percent)
{
	REAL dx = (REAL)((double)(ep.X - sp.X) * (double)percent / 100.0);
	REAL dy = (REAL)((double)(ep.Y - sp.Y) * (double)percent / 100.0);

	return PointF((sp.X + dx), (sp.Y + dy));
}

// path의 시작점에서 경로를 따라서 dist만큼 떨어진 거리의 좌표를 리턴한다.
CPoint CUtil::getOffsetPoint(vector<POINT>& path, double dist)
{
	double d = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		d = distance(path[i].x, path[i].y, path[i + 1].x, path[i + 1].y);
		if (dist < d)
		{
			double rate = dist / d;
			double dx = path[i + 1].x - path[i].x;
			double dy = path[i + 1].y - path[i].y;
			double tdx = dx * rate;
			double tdy = dy * rate;

			return CPoint(path[i].x + (int)tdx, path[i].y + (int)tdy);
		}
		dist -= d;
	}
	if (-0.000001 < dist && dist < 0.000001)
		return path.back();
	return CPoint(0, 0);
}

PointF CUtil::getOffsetPoint(vector<PointF>& path, double dist)
{
	double d = 0.0;
	int size = (int)path.size();
	for (int i = 0; i < size - 1; i++)
	{
		d = distance(path[i].X, path[i].Y, path[i + 1].X, path[i + 1].Y);
		if (dist < d)
		{
			double rate = dist / d;
			double dx = path[i + 1].X - path[i].X;
			double dy = path[i + 1].Y - path[i].Y;
			double tdx = dx * rate;
			double tdy = dy * rate;

			return PointF(path[i].X + (REAL)tdx, path[i].Y + (REAL)tdy);
		}
		dist -= d;
	}
	if (-0.000001 < dist && dist < 0.000001)
		return path.back();
	return PointF(0.0f, 0.0f);
}

void CUtil::getRotatePoint(POINT cp, POINT p1, POINT p2, double area, int cnt, vector<POINT>& retArr)
{
	POINT tp1, tp2;
	tp1.x = p1.x - cp.x;
	tp1.y = p1.y - cp.y;
	tp2.x = p2.x - cp.x;
	tp2.y = p2.y - cp.y;

	double ang1 = RTD(atan2(tp1.y, tp1.x));
	if (ang1 < 0.0f)	ang1 = 360.0f + ang1;
	double ang2 = RTD(atan2(tp2.y, tp2.x));
	if (ang2 < 0.0f)	ang2 = 360.0f + ang2;

	POINT tp;
	double diff = ang2 - ang1;
	if (0.0 < area && diff < 0.0)		diff += 360.0;
	else if (area < 0.0 && 0.0 < diff)	diff -= 360.0;

	double td = diff / (double)(cnt - 1);
	double tr = DTR(td);
	for (int i = 0; i < cnt; i++)
	{
		tp.x = (long)((p1.x - cp.x) * cos(tr * i) - (p1.y - cp.y) * sin(tr * i)) + cp.x;
		tp.y = (long)((p1.x - cp.x) * sin(tr * i) + (p1.y - cp.y) * cos(tr * i)) + cp.y;

		retArr.push_back(tp);
	}
}

void CUtil::getRotatePoint(PointF cp, PointF p1, PointF p2, double area, int cnt, vector<PointF>& retArr)
{
	PointF tp1, tp2;
	tp1.X = p1.X - cp.X;
	tp1.Y = p1.Y - cp.Y;
	tp2.X = p2.X - cp.X;
	tp2.Y = p2.Y - cp.Y;

	double ang1 = RTD(atan2(tp1.Y, tp1.X));
	if (ang1 < 0.0f)	ang1 = 360.0f + ang1;
	double ang2 = RTD(atan2(tp2.Y, tp2.X));
	if (ang2 < 0.0f)	ang2 = 360.0f + ang2;

	PointF tp;
	double diff = ang2 - ang1;
	if (0.0 < area && diff < 0.0)		diff += 360.0;
	else if (area < 0.0 && 0.0 < diff)	diff -= 360.0;

	double td = diff / (double)(cnt - 1);
	double tr = DTR(td);
	for (int i = 0; i < cnt; i++)
	{
		tp.X = (REAL)((p1.X - cp.X) * cos(tr * i) - (p1.Y - cp.Y) * sin(tr * i)) + cp.X;
		tp.Y = (REAL)((p1.X - cp.X) * sin(tr * i) + (p1.Y - cp.Y) * cos(tr * i)) + cp.Y;

		retArr.push_back(tp);
	}
}

// i와 j사이의 정수 랜덤값을 리턴한다.
// 값의 차이가 RAND_MAX(32767)보다 크면 비율로 처리한다.
long CUtil::getRandomLong(long i, long j)
{
	if (i == j)	return i;

	long min = i;
	long max = j;
	if (j < i)
	{
		min = j;
		max = i;
	}

	long d = (max - min) + 1;
	long ret;
	if (RAND_MAX < d)
	{
		double rate = (double)d / (double)RAND_MAX;
		ret = (long)((rand() % RAND_MAX) * rate) + min;
	}
	else
	{
		ret = rand() % d + min;
	}
	return ret;
}
// 거리 범위와 각도 범위를 주면 임의의 점을 리턴
//
//				270
//
//		180				0
//
//				90
//
PointF CUtil::getRandomPoint(PointF& pt, int mindist, int maxdist, int minang, int maxang)
{
	long d = getRandomLong(mindist, maxdist);
	long a = getRandomLong(minang, maxang);

	return getPoint(pt, d, a);
}
// 직사각형 영역을 주면 임의의 점을 리턴
PointF CUtil::getRandomPoint(REAL minx, REAL miny, REAL maxx, REAL maxy)
{
	long x = getRandomLong((long)minx, (long)maxx);
	long y = getRandomLong((long)miny, (long)maxy);

	return PointF((REAL)x, (REAL)y);
}

// 점 pt에서 dist 만큼 떨어져서 a만큼 회전한 점의 좌표를 리턴. 3시가 0도, 반시계방향으로 증가 (동:0, 남:90, 서:180, 북:270)
CPoint CUtil::getPoint(CPoint& pt, double dist, double angle)
{
	double rad = DTR(angle);
	int x = (int)(dist * cos(rad));
	int y = (int)(dist * sin(rad));

	return CPoint(pt.x+x, pt.y+y);
}
PointF CUtil::getPoint(PointF& pt, double dist, double angle)
{
	double rad = DTR(angle);
	int x = (int)(dist * cos(rad));
	int y = (int)(dist * sin(rad));

	return PointF(pt.X + x, pt.Y + y);
}

// pt에서 angle방향으로 선을 그어서 rect와 교차하는 점을 리턴
PointF CUtil::getPoint(PointF& pt, RectF rect, double angle)
{
	PointF tp = getPoint(pt, 100000000.0, angle);	// angle방향의 임의의 점을 생성

	double ix = 0.0;
	double iy = 0.0;
	double p1x = rect.X;
	double p1y = rect.Y;
	double p2x = rect.X + rect.Width;
	double p2y = rect.Y + rect.Height;
	PointF ret = { 0.0f, 0.0f };
	if (intersect(pt.X, pt.Y, tp.X, tp.Y, p1x, p1y, p2x, p1y, &ix, &iy)) ret = PointF((REAL)ix, (REAL)iy);// pt-tp 선과 rect의 윗선이 교차하면
	if (intersect(pt.X, pt.Y, tp.X, tp.Y, p1x, p2y, p2x, p2y, &ix, &iy)) ret = PointF((REAL)ix, (REAL)iy);// pt-tp 선과 rect의 아래선이 교차하면
	if (intersect(pt.X, pt.Y, tp.X, tp.Y, p1x, p1y, p1x, p2y, &ix, &iy)) ret = PointF((REAL)ix, (REAL)iy);// pt-tp 선과 rect의 왼선이 교차하면
	if (intersect(pt.X, pt.Y, tp.X, tp.Y, p2x, p1y, p2x, p2y, &ix, &iy)) ret = PointF((REAL)ix, (REAL)iy);// pt-tp 선과 rect의 오른선이 교차하면

	return ret;
}

// 주어진 속도에서 주어진 G-Force에 도달하면서 회전을 하려면 필요한 반지름을 meter로 구한다.
double CUtil::getRadius(double speed, int gForce)
{
	double r = 0.001118 * pow(speed, 2) / (gForce * pow((2 * PI), 2));

	return r;
}

//==========================================================================================================
// 경로 vec이 경계 Bound r을 통과하는지 확인 ==> 확대시 그릴지 말지를 판단하기 위해 사용
BOOL CUtil::intersect(RectF r, vector<PointF>* vec)
{
	PointF p1, p2;
	int size = (int)vec->size();
	for (int i = 0; i < size - 1; i++)
	{
		p1 = vec->at(i);
		p2 = vec->at(i+1);

		if (intersect(p1.X, p1.Y, p2.X, p2.Y, r.X, r.Y, r.X + r.Width, r.Y))	return TRUE;
		if (intersect(p1.X, p1.Y, p2.X, p2.Y, r.X, r.Y + r.Height, r.X + r.Width, r.Y + r.Height))	return TRUE;
		if (intersect(p1.X, p1.Y, p2.X, p2.Y, r.X, r.Y, r.X, r.Y + r.Height))	return TRUE;
		if (intersect(p1.X, p1.Y, p2.X, p2.Y, r.X + r.Width, r.Y, r.X + r.Width, r.Y + r.Height))	return TRUE;
	}
	return FALSE;
}

// 실제 선분 사이에서 교차하면 TRUE를 리턴하고 그렇지 않으면 FALSE를 리턴, 평행인 경우에도 FALSE를 리턴한다.
BOOL CUtil::intersect(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy, double* tx, double* ty)
{
	double	s, t;
	double	num, denom;
	int		bOver = FALSE;
	int		prec = 6;
	BOOL	ret = FALSE;

	if (100000 < ax || 100000 < ay || 100000 < bx || 100000 < by ||
		100000 < cx || 100000 < cy || 100000 < dx || 100000 < dy)
	{
		ax /= 100.0;
		ay /= 100.0;
		bx /= 100.0;
		by /= 100.0;
		cx /= 100.0;
		cy /= 100.0;
		dx /= 100.0;
		dy /= 100.0;

		bOver = TRUE;
		prec = 8;
	}

	denom = ax * (dy - cy) + bx * (cy - dy) + dx * (by - ay) + cx * (ay - by);

	// 평행인 경우
	if (isEqual(denom, 0, prec))	return FALSE;

	num = ax * (dy - cy) + cx * (ay - dy) + dx * (cy - ay);
	if (isEqual(num, 0.0, prec) || isEqual(num, denom, prec)) ret = TRUE;
	s = num / denom;

	s = (double)((int)(s * 1000000.0)) / 1000000.0;

	num = -(ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
	if (isEqual(num, 0.0, prec) || isEqual(num, denom, prec)) ret = TRUE;
	t = num / denom;

	t = (double)((int)(t * 1000000.0)) / 1000000.0;

	if ((0.0 < s) && (s < 1.0) && (0.0 < t) && (t < 1.0))		ret = TRUE;
	else if ((0.0 > s) || (s > 1.0) || (0.0 > t) || (t > 1.0))	ret = FALSE;

	if (tx && ty)
	{
		*tx = ax + s * (bx - ax);
		*ty = ay + s * (by - ay);

		if (bOver)
		{
			*tx *= 100.0;
			*ty *= 100.0;
		}
	}

	return ret;
}

// 두 원의 교점을 ret에 담아서 리턴한다.
void CUtil::circle_circle_Intersect(double x1, double y1, double r1, double x2, double y2, double r2, PointF ret[2])
{
	double X = x2 - x1;
	double Y = y2 - y1;

	double D = sqrt((X * X) + (Y * Y));
	double T1 = acos((r1 * r1 - r2 * r2 + D * D) / (2 * r1 * D));
	double T2 = atan2(Y, X);

	ret[0].X = (REAL)(x1 + r1 * cos(T2 + T1));
	ret[0].Y = (REAL)(y1 + r1 * sin(T2 + T1));

	ret[1].X = (REAL)(x1 + r1 * cos(T2 - T1));
	ret[1].Y = (REAL)(y1 + r1 * sin(T2 - T1));
}

void CUtil::circle_line_Intersect(double x, double y, double r, double x1, double y1, double x2, double y2, PointF ret[2])
{
	// 계산을 쉽게하기 위해 원의 중심을 (0,0)으로 이동
	double tx1 = x1 - x;
	double ty1 = y1 - y;
	double tx2 = x2 - x;
	double ty2 = y2 - y;

	// y = Ax - B;
	double A = (ty2 - ty1) / (tx2 - tx1);
	double B = A * tx1 - ty1;

	// ax^2 + bx + c = 0;
	double a = A * A + 1.0;
	double b = -2.0 * A * B;
	double c = B * B - r * r;

	double D = b * b - 4.0 * a * c;

	ret[0].X = (REAL)((-b + sqrt(D)) / (2.0 * a));
	ret[0].Y = (REAL)(A * ret[0].X - B);

	ret[1].X = (REAL)((-b - sqrt(D)) / (2.0 * a));
	ret[1].Y = (REAL)(A * ret[1].X - B);

	// 원 중심을 이동한 만큼 복원
	ret[0].X += (REAL)x;
	ret[0].Y += (REAL)y;
	ret[1].X += (REAL)x;
	ret[1].Y += (REAL)y;
}
