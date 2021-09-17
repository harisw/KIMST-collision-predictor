// SimuOutDlg.cpp : implementation file
//

#include "pch.h"
#include "PaperImage.h"
#include "SimuOutDlg.h"
#include "afxdialogex.h"

#include "Util.h"
// SimuOutDlg dialog

IMPLEMENT_DYNAMIC(SimuOutDlg, CDialogEx)

SimuOutDlg::SimuOutDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIMULOUTDLG, pParent)
{

}

SimuOutDlg::~SimuOutDlg()
{
}

void SimuOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_resultList);
}


BEGIN_MESSAGE_MAP(SimuOutDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// SimuOutDlg message handlers


BOOL SimuOutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyle(WS_SYSMENU, 0);

	// TODO:  Add extra initialization here
	m_resultList.InsertColumn(0, _T("Result Out"), LVCFMT_LEFT, 1000);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void SimuOutDlg::OnTimer(UINT_PTR nIDEvent)
{
	
	switch (nIDEvent)
	{
	case IDT_TIMER_UPDATE_EVENT:
		while (!queue_ev.empty()) {
			string topE = queue_ev.front();
			m_resultList.InsertItem(matchCount, wstring(topE.begin(), topE.end()).c_str());
			matchCount++;
			queue_ev.pop();
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

afx_msg LRESULT SimuOutDlg::OnUpdateEvent(WPARAM wParam, LPARAM lParam)
{
	while (!queue_ev.empty()) {
		string topE = queue_ev.front();
		m_resultList.InsertItem(matchCount, wstring(topE.begin(), topE.end()).c_str());
		matchCount++;
		queue_ev.pop();
	}
	m_resultList.EnsureVisible(matchCount - 1, true);
	return 0;
}
