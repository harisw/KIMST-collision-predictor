// CMTools.h : interface of the SelectTool class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMTools_H__B4FCAE0D_2092_11D2_8476_0000B43382FE__INCLUDED_)
#define AFX_CMTools_H__B4FCAE0D_2092_11D2_8476_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define WM_PANNING	(WM_USER+100)
#define WM_ZOOMIN	(WM_USER+101)
#define WM_ZOOMOUT	(WM_USER+102)

#define NORMAL_MOUSE		10
#define PAN_MOUSE			11
#define ZOOMIN_MOUSE        12
#define ZOOMOUT_MOUSE       13

enum ToolMode {
	TM_NONE = 1000,
	PANTOOL,
	USERZOOMTOOL,
};

class CCMTools
{
public:
	CCMTools(ToolMode nMode);
	virtual ~CCMTools();

// Overridables
public:
	virtual void OnDraw(CWnd* pWnd, CDC* pDC) {}
	virtual void OnLButtonDown(CWnd* pWnd, UINT nFlags, const CPoint& point) {}
	virtual void OnRButtonDown(CWnd* pWnd, UINT nFlags, const CPoint& point) {}
	virtual void OnLButtonDblClk(CWnd* pWnd, UINT nFlags, const CPoint& point) {}

// Attribute
	ToolMode	m_eToolMode;
	CPoint		m_down;
	int			m_nCursorOrg;

// Static Functions
	static void			SetCmdMode(ToolMode nMode);
	static CCMTools*	FindTool (ToolMode nMode);

// Static Attributes
	static CPtrList		c_Tools;
	static ToolMode		c_eToolMode;
};

#endif