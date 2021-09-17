#pragma once

class CCQArea;
class CCQObject
{
public:
	BOOL m_bFriend;						// TRUE�� �Ʊ�, FALSE�� ����
	int m_objtype;						// ����, �Լ�, �, �����
	int m_speedtype;					// ����, �߼�, ���
	int m_id;
	int m_trjtype;
	PointF m_initPoint;
	int m_kmh;
	RectF m_mbr;						// ������ǥ�� ����� MBR
	wstring m_charID;
	BOOL m_isFocused = false;
	BOOL m_isDetected = false;
	int secToCheck;
private:
	vector<PointF> m_AbstractPath;		// m_ConcretePath�� ����� ���� �⺻ ���
	vector<PointF> m_ConcreteDrawPath;	// m_AbstractPath�� ȸ���� ���� ��ǥ�� �߰��� ��� (�׸��⿡ ���)
	vector<PointF> m_ConcreteMovePath;	// m_ConcreteDrawPath�� �ð� ������ ������ ��� (�ùķ��̼ǿ� ���)

	void setAbstractPath(PointF pt, CCQArea* pArea, int type);
	void setConcreteDrawPath(double speed);
	void setConcreteMovePath(double speed);
	void setMbr();

	Pen* m_pFocusedPen;
	Pen* m_pPathPen;			// ��ü ��� ��
	Pen* m_pPen;				// ��ü ���� �׸��� �׵θ� ��, ȭ��ǥ �׵θ� ��
	SolidBrush* m_pBrush;		// ��ü ���� �׸��� ä��� �귯��
	SolidBrush* m_pArrowBrush;	// ȭ��ǥ ä��� �귯��
	SolidBrush* m_pDetectedBrush;
	void setResource();
	void drawarrow(Graphics& g, CView* pView, vector<PointF>* path);
	void drawobject(Graphics& g, CView* pView, int idx);

public:
	CCQObject(int objtype, int speedtype, PointF pt, CCQArea* pArea, int trjtype, int kmh, BOOL bFriend = TRUE);
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
};