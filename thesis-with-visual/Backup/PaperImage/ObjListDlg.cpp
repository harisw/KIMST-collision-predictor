// ObjListDlg.cpp : implementation file
//

#include "pch.h"
#include "PaperImage.h"
#include "ObjListDlg.h"
#include "afxdialogex.h"
#include "PaperImageDoc.h"
#include "PaperImageView.h"
#include "CQObject.h"
#include "Util.h"
// ObjListDlg dialog

IMPLEMENT_DYNAMIC(ObjListDlg, CDialogEx)

ObjListDlg::ObjListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBJLISTDIALOG, pParent)
	, m_pWnd(pParent)
{

}

ObjListDlg::~ObjListDlg()
{
}

void ObjListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}


BEGIN_MESSAGE_MAP(ObjListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// ObjListDlg message handlers


void ObjListDlg::OnBnClickedButton1()
{
	CPaperImageView* pView = (CPaperImageView*)this->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	int selected = m_listCtrl.GetSelectionMark();
	int lastFocused = currObjFocused;
	if (lastFocused != -1) {
		pDoc->m_CQObjects[lastFocused]->m_isFocused = false;
	}
	pDoc->m_CQObjects[selected]->m_isFocused = true;
	currObjFocused = selected;
}

BOOL ObjListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void ObjListDlg::initObjects()
{
	CPaperImageView* pView = (CPaperImageView*)this->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST1)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST1)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	m_listCtrl.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
	m_listCtrl.InsertColumn(1, _T("Speed Type"), LVCFMT_LEFT, 200);

	CString t;
	for (int j = 0; j < pDoc->m_CQObjects.size(); j++) {
		t.Format(_T("%d"), pDoc->m_CQObjects[j]->m_id);
		int nIndex = m_listCtrl.InsertItem(j, t);
		t.Format(_T("%d, %d"), pDoc->m_CQObjects[j]->m_vx, pDoc->m_CQObjects[j]->m_vy);
		m_listCtrl .SetItemText(nIndex, 1, t);
	}
}