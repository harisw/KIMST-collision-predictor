// RuleDlg.cpp : implementation file
//

#include "pch.h"
#include "PaperImage.h"
#include "PaperImageDoc.h"
#include "PaperImageView.h"
#include "RuleDlg.h"
#include "afxdialogex.h"
#include "Util.h"
#include "CQObject.h"

#define WM_ADDOBJECT	(WM_USER + 1)
#define WM_ENDOBJECT	(WM_USER + 2)

// RuleDlg dialog

IMPLEMENT_DYNAMIC(RuleDlg, CDialogEx)

RuleDlg::RuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RULEDLG, pParent)
	, m_pWnd(pParent)
{

}

RuleDlg::~RuleDlg()
{
}

void RuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_objList);
}


BEGIN_MESSAGE_MAP(RuleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON7, &RuleDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &RuleDlg::OnBnClickedButton6)
	ON_MESSAGE(WM_GENERATE_OBJ, &RuleDlg::OnGenerateObj)
	/*ON_BN_CLICKED(IDC_BUTTON5, &RuleDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &RuleDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &RuleDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &RuleDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_INSERT_R, &RuleDlg::OnBnClickedInsertR)
	ON_BN_CLICKED(IDC_BUTTON9, &RuleDlg::OnBnClickedButton9)
	ON_EN_CHANGE(IDC_EDIT1, &RuleDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &RuleDlg::OnEnChangeEdit2)*/
END_MESSAGE_MAP()


// RuleDlg message handlers


BOOL RuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST2)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST2)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);

	dwStyle = GetDlgItem(IDC_LIST3)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST3)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	
	m_objList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 30);
	m_objList.InsertColumn(1, _T("Speed"), LVCFMT_LEFT, 100);
	m_objList.InsertColumn(2, _T("Init Position"), LVCFMT_LEFT, 1000);

	return TRUE;
}


void RuleDlg::OnBnClickedPreInsertR()
{

}

void RuleDlg::OnBnClickedButton7()
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	AfxGetMainWnd()->ShowWindow(SW_SHOW);
	AfxGetMainWnd()->UpdateWindow();
	pView->startSimulation();
	this->ShowWindow(SW_HIDE);
}

void RuleDlg::OnBnClickedButton6()		//Setting Scenario Button
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();
	pView->OnButtonC();
}

afx_msg LRESULT RuleDlg::OnGenerateObj(WPARAM wParam, LPARAM lParam)
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();
	CCQObject* temp = NULL;
	CString t;
	vector<PointF> tempVec;
	for (int j = lastObjId; j < pDoc->m_CQObjects.size(); j++) {
		temp = pDoc->m_CQObjects[j];
		t.Format(_T("%d"), temp->m_id);
		int nIndex = m_objList.InsertItem(j, t);
		switch (temp->m_speedtype)
		{
		case RLSO:
			m_objList.SetItemText(nIndex, 1, _T("Low"));
			break;
		case RMSO:
			m_objList.SetItemText(nIndex, 1, _T("Medium"));
			break;
		case RHSO:
			m_objList.SetItemText(nIndex, 1, _T("High"));
			break;
		default:
			break;
		}
		tempVec = *temp->getMovePath();
		t.Format(_T("%f,   %f"), tempVec[0].X, tempVec[0].Y);
		m_objList.SetItemText(nIndex, 2, t);

	}
	lastObjId = pDoc->m_CQObjects.size();
	return 0;
}