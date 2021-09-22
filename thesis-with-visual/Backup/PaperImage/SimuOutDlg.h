#pragma once
#define IDT_TIMER_UPDATE_RESULT WM_USER + 201
#define IDT_TIMER_UPDATE_EVENT WM_USER + 202
// SimuOutDlg dialog
#include <map>

class SimuOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SimuOutDlg)

public:
	SimuOutDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SimuOutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULOUTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_resultList;
	int matchCount = 0;
	int matchTimeOut = 0;
private:
protected:
	afx_msg LRESULT OnUpdateEvent(WPARAM wParam, LPARAM lParam);
};
