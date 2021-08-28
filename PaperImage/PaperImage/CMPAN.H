// CMPAN.h : interface of the SelectTool class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMPAN_H_B4FCAE0D_2092_11D2_8476_0000B43382FE__INCLUDED_)
#define AFX_CMPAN_H_B4FCAE0D_2092_11D2_8476_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CMTools.h"

class CCMPan : public CCMTools
{
	void AllocResources(CWnd* pWnd, int _w, int _h);
	void ReleaseResources();
	void draw(CWnd* pWnd, int dx, int dy);

	CDC*	 m_pDC;
	CBitmap* m_pBitmap;

public:
	CCMPan(ToolMode nMode);
	~CCMPan();


// Overridables
public:
	virtual void OnRButtonDown(CWnd* pWnd, UINT nFlags, const CPoint& point);
};

#endif