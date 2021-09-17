#pragma once
#include <afxdockablepane.h>
#include "SimuOutDlg.h"
//class CDockPane :
//	public CDockablePane
//{
//
//public:
//	DECLARE_MESSAGE_MAP()
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//};


class CDockPane : public CDockablePane
{
	// Construction
public:
	CDockPane() noexcept;

	//void AdjustLayout();
	//void OnChangeVisualStyle();

	// Attributes
protected:

	//CImageList m_DockPaneImages;

protected:
	//void FillDockPane();

	// Implementation
public:
	virtual ~CDockPane();
	SimuOutDlg dialog_box;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//afx_msg void OnEditCut();
	//afx_msg void OnEditCopy();
	//afx_msg void OnEditClear();
	//afx_msg void OnPaint();
	//afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
};