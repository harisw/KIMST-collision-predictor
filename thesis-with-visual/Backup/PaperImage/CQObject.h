#pragma once

class CCQArea;
class CCQObject
{
public:
	int m_id;
	int m_trjtype;
	PointF m_initPoint;
	int m_kmh;
	int m_vx;
	int m_vy;
	RectF m_mbr;						// 지도좌표로 경로의 MBR
	wstring m_charID;
	BOOL m_isFocused = false;
	BOOL m_isDetected = false;
	int secToCheck;
private:
	vector<PointF> m_AbstractPath;		// m_ConcretePath를 만들기 위한 기본 경로
	vector<PointF> m_ConcreteDrawPath;	// m_AbstractPath에 회전을 위한 좌표를 추가한 경로 (그리기에 사용)
	vector<PointF> m_ConcreteMovePath;	// m_ConcreteDrawPath를 시간 단위로 분할한 경로 (시뮬레이션에 사용)

	void setAbstractPath(PointF pt, CCQArea* pArea, int type);
	void setConcreteDrawPath(double speed);
	void setConcreteMovePath();
	void setMbr();

	Pen* m_pFocusedPen;
	Pen* m_pPathPen;			// 객체 경로 펜
	Pen* m_pPen;				// 객체 원을 그릴때 테두리 펜, 화살표 테두리 펜
	SolidBrush* m_pBrush;		// 객체 원을 그릴때 채우기 브러시
	SolidBrush* m_pArrowBrush;	// 화살표 채우기 브러시
	SolidBrush* m_pDetectedBrush;
	void setResource();
	void drawarrow(Graphics& g, CView* pView, vector<PointF>* path);
	void drawobject(Graphics& g, CView* pView, int idx);

public:
	CCQObject(int _id, PointF pt, CCQArea* pArea, int trjtype, double _vx, double _vy);
	~CCQObject();

	void draw(Graphics& g, CView* pView);

	int getMoveCount();
	vector<PointF>* getDrawPath()
	{
		return &m_ConcreteDrawPath;
	}
	vector<PointF>* getMovePath()
	{
		return &m_ConcreteMovePath;
	}
	BOOL isCollide(CCQArea* ourVessel, CView* pView);

};