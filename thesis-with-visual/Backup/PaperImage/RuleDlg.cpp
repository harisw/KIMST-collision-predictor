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
	DDX_Control(pDX, IDC_LIST2, m_vesselList);
	DDX_Control(pDX, IDC_EDITVESSEL_X, m_vesselX_edit);
	DDX_Control(pDX, IDC_EDITVESSEL_Y, m_vesselY_edit);
	DDX_Control(pDX, IDC_EDITVESSEL_RAD, m_vesselRad_edit);
	DDX_Control(pDX, IDC_EDITVESSEL_VX, m_vesselVX_edit);
	DDX_Control(pDX, IDC_EDITVESSEL_VY, m_vesselVY_edit);
}


BEGIN_MESSAGE_MAP(RuleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON7, &RuleDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON6, &RuleDlg::OnBnClickedButton6)
	ON_MESSAGE(WM_GENERATE_OBJ, &RuleDlg::OnGenerateObj)
	/*ON_BN_CLICKED(IDC_BUTTON5, &RuleDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON8, &RuleDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &RuleDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &RuleDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON9, &RuleDlg::OnBnClickedButton9)
	ON_EN_CHANGE(IDC_EDIT1, &RuleDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &RuleDlg::OnEnChangeEdit2)*/
	ON_BN_CLICKED(IDC_PRE_INSERT_R, &RuleDlg::OnBnClickedPreInsertR)
END_MESSAGE_MAP()


// RuleDlg message handlers


BOOL RuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD dwStyle;
	dwStyle = GetDlgItem(IDC_LIST2)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST2)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	m_vesselList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 30);
	m_vesselList.InsertColumn(1, _T("Position"), LVCFMT_LEFT, 100);
	m_vesselList.InsertColumn(2, _T("Speed"), LVCFMT_LEFT, 100);
	m_vesselList.InsertColumn(3, _T("Radius"), LVCFMT_LEFT, 100);

	dwStyle = GetDlgItem(IDC_LIST3)->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_REPORT | LVS_EX_GRIDLINES;
	GetDlgItem(IDC_LIST3)->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, dwStyle);
	m_objList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 30);
	m_objList.InsertColumn(1, _T("Speed"), LVCFMT_LEFT, 100);
	m_objList.InsertColumn(2, _T("Position"), LVCFMT_LEFT, 1000);

	CPaperImageView* pView = (CPaperImageView*)this->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();
	// 쓰레드마다 별도로 랜덤 초기화해야 함.
	srand((unsigned int)time(0));
	pView->initScale();
	RectF mapRect = pView->getMapRect();
	PointF mp = pView->Scr2Map((int)(pView->m_rect.Width() / 2.0), (int)(pView->m_rect.Height() * 3.0 / 5.0));

	CString t;
	t.Format(_T("%f"), mp.X);
	GetDlgItem(IDC_EDITVESSEL_X)->SetWindowText(t);
	t.Format(_T("%f"), mp.Y);
	GetDlgItem(IDC_EDITVESSEL_Y)->SetWindowText(t);
	int rad = VESSEL_RAD;
	int vx = 50;
	int vy = 30;
	t.Format(_T("%d"), rad);
	GetDlgItem(IDC_EDITVESSEL_RAD)->SetWindowText(t);
	t.Format(_T("%d"), vx);
	GetDlgItem(IDC_EDITVESSEL_VX)->SetWindowText(t);
	t.Format(_T("%d"), vy);
	GetDlgItem(IDC_EDITVESSEL_VY)->SetWindowText(t);

	return TRUE;
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
		t.Format(_T("%d, %d"), temp->m_vx, temp->m_vy);
		m_objList.SetItemText(nIndex, 1, t);
		//switch (temp->m_speedtype)
		//{
		//case RLSO:
		//	m_objList.SetItemText(nIndex, 1, _T("Low"));
		//	break;
		//case RMSO:
		//	m_objList.SetItemText(nIndex, 1, _T("Medium"));
		//	break;
		//case RHSO:
		//	m_objList.SetItemText(nIndex, 1, _T("High"));
		//	break;
		//default:
		//	break;
		//}
		tempVec = *temp->getMovePath();
		t.Format(_T("%f,   %f"), tempVec[0].X, tempVec[0].Y);
		m_objList.SetItemText(nIndex, 2, t);

	}
	lastObjId = pDoc->m_CQObjects.size();
	return 0;
}

void RuleDlg::OnBnClickedPreInsertR()
{
	CString path;
	string strX, strY, strRad, strVX, strVY;
	LPTSTR str_line;
	int len;
	len = m_vesselX_edit.LineLength(m_vesselX_edit.LineIndex(0));
	str_line = path.GetBuffer(len);
	m_vesselX_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	strX = CT2A(str_line);

	len = m_vesselY_edit.LineLength(m_vesselY_edit.LineIndex(0));
	str_line = path.GetBuffer(len);
	m_vesselY_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	strY = CT2A(str_line);

	/*len = m_vesselRad_edit.LineLength(m_vesselRad_edit.LineIndex(0));
	str_line = path.GetBuffer(len);
	m_vesselRad_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	strRad = CT2A(str_line);

	len = m_vesselVX_edit.LineLength(m_vesselVX_edit.LineIndex(0));
	str_line = path.GetBuffer(len);
	m_vesselVX_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	strVX = CT2A(str_line);

	len = m_vesselVY_edit.LineLength(m_vesselVY_edit.LineIndex(0));
	str_line = path.GetBuffer(len);
	m_vesselVY_edit.GetLine(0, str_line, len);
	path.ReleaseBuffer();
	strVY = CT2A(str_line);*/
	float vx, vy, rad;
	BOOL bWorked = FALSE;

	vx = GetDlgItemInt(IDC_EDITVESSEL_VX, &bWorked);
	vy = GetDlgItemInt(IDC_EDITVESSEL_VY, &bWorked);
	rad = GetDlgItemInt(IDC_EDITVESSEL_RAD, &bWorked);

	CPaperImageView* pView = (CPaperImageView*)this->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	srand((unsigned int)time(0));
	pView->initScale();
	RectF mapRect = pView->getMapRect();
	CCQArea* newVessel = new CCQAreaCircle(mapRect, stof(strX), stof(strY), rad, vx, vy);
	pDoc->m_CQAreas.push_back(newVessel);

	CString t;
	int nIndex = m_vesselList.InsertItem(0, _T("0"));
	t.Format(_T("%.2f, %.2f"), stof(strX), stof(strY));
	m_vesselList.SetItemText(nIndex, 1, t);

	t.Format(_T("%.2f, %.2f"), vx, vy);
	m_vesselList.SetItemText(nIndex, 2, t);

	t.Format(_T("%.2f"), rad);
	m_vesselList.SetItemText(nIndex, 3, t);


}