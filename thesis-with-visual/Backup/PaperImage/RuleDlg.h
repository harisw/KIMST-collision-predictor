#pragma once
#include "CQArea.h"

class RuleObj;
// RuleDlg dialog
class RuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RuleDlg)

public:
	RuleDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~RuleDlg();

	CWnd* m_pWnd;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RULEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int ruleCount = 0;
	CListCtrl m_insertedList;
	int lastObjId = 0;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton6();
public:
	CListCtrl m_objList;
protected:
	afx_msg LRESULT OnGenerateObj(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_vesselList;
	afx_msg void OnBnClickedPreInsertR();
	CEdit m_vesselX_edit;
	CEdit m_vesselY_edit;
	CEdit m_vesselRad_edit;
	CEdit m_vesselVX_edit;
	CEdit m_vesselVY_edit;
};