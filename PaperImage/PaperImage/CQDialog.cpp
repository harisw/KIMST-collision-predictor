// CQDialog.cpp: 구현 파일
//

#include "pch.h"
#include "PaperImage.h"
#include "PaperImageDoc.h"
#include "PaperImageView.h"
#include "CQDialog.h"
#include "afxdialogex.h"
#include "Util.h"
#include "CQObject.h"
#include "CQArea.h"

#define WM_ADDOBJECT	(WM_USER + 1)
#define WM_ENDOBJECT	(WM_USER + 2)

UINT ThreadFunc(LPVOID pParam);
UINT ImportThreadFunc(LPVOID pParam);

// CCQDialog 대화 상자

IMPLEMENT_DYNAMIC(CCQDialog, CDialogEx)

CCQDialog::CCQDialog(CWnd* pParent)
	: CDialogEx(IDD_CCQDIALOG, pParent)
	, m_pWnd(pParent)
	, m_CQTYPE(0)
	, m_NO_COUNT(TOTALCOUNT)
	, m_REO_PERCENT(30)
	, m_RFO_PERCENT(70)
	, m_RLSO_PERCENT(40)
	, m_RMSO_PERCENT(59)
	, m_RHSO_PERCENT(1)
	, m_REOCQ_PERCENT(15)
	, m_REONCQ_PERCENT(85)
	, m_RFOCQ_PERCENT(15)
	, m_RFONCQ_PERCENT(85)
	, m_RAO_PERCENT(10)
	, m_RSVO_PERCENT(20)
	, m_RFVO_PERCENT(65)
	, m_RSMO_PERCENT(5)
	, m_bALL(TRUE), m_bREO(TRUE), m_bRFO(TRUE)
	, m_bRAO(TRUE), m_bRAO_RLSO(TRUE), m_bRAO_RMSO(TRUE), m_bRAO_RHSO(TRUE)
	, m_bRSVO(TRUE), m_bRSVO_RLSO(TRUE), m_bRSVO_RMSO(TRUE), m_bRSVO_RHSO(TRUE)
	, m_bRFVO(TRUE), m_bRFVO_RLSO(TRUE), m_bRFVO_RMSO(TRUE), m_bRFVO_RHSO(TRUE)
	, m_bRSMO(TRUE), m_bRSMO_RLSO(TRUE), m_bRSMO_RMSO(TRUE), m_bRSMO_RHSO(TRUE)
	, m_bDrawPath(TRUE)
{
	m_bool1[RAO] = &m_bRAO;
	m_bool2[RAO][RLSO] = &m_bRAO_RLSO;
	m_bool2[RAO][RMSO] = &m_bRAO_RMSO;
	m_bool2[RAO][RHSO] = &m_bRAO_RHSO;
	m_bool1[RSVO] = &m_bRSVO;
	m_bool2[RSVO][RLSO] = &m_bRSVO_RLSO;
	m_bool2[RSVO][RMSO] = &m_bRSVO_RMSO;
	m_bool2[RSVO][RHSO] = &m_bRSVO_RHSO;
	m_bool1[RFVO] = &m_bRFVO;
	m_bool2[RFVO][RLSO] = &m_bRFVO_RLSO;
	m_bool2[RFVO][RMSO] = &m_bRFVO_RMSO;
	m_bool2[RFVO][RHSO] = &m_bRFVO_RHSO;
	m_bool1[RSMO] = &m_bRSMO;
	m_bool2[RSMO][RLSO] = &m_bRSMO_RLSO;
	m_bool2[RSMO][RMSO] = &m_bRSMO_RMSO;
	m_bool2[RSMO][RHSO] = &m_bRSMO_RHSO;
}

CCQDialog::~CCQDialog()
{
}

void CCQDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_CQTYPE, m_CQTYPE);
	DDX_Text(pDX, IDC_EDIT_NO_COUNT, m_NO_COUNT);
	DDX_Text(pDX, IDC_EDIT_REO_PERCENT, m_REO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_REO_COUNT, m_REO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RFO_PERCENT, m_RFO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RFO_COUNT, m_RFO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RLSO_PERCENT, m_RLSO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RMSO_PERCENT, m_RMSO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RHSO_PERCENT, m_RHSO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_REOCQ_PERCENT, m_REOCQ_PERCENT);
	DDX_Text(pDX, IDC_EDIT_REOCQ_COUNT, m_REOCQ_COUNT);
	DDX_Text(pDX, IDC_EDIT_REONCQ_PERCENT, m_REONCQ_PERCENT);
	DDX_Text(pDX, IDC_EDIT_REONCQ_COUNT, m_REONCQ_COUNT);
	DDX_Text(pDX, IDC_EDIT_RFOCQ_PERCENT, m_RFOCQ_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RFOCQ_COUNT, m_RFOCQ_COUNT);
	DDX_Text(pDX, IDC_EDIT_RFONCQ_PERCENT, m_RFONCQ_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RFONCQ_COUNT, m_RFONCQ_COUNT);
	DDX_Text(pDX, IDC_EDIT_RAO_PERCENT, m_RAO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RAO_REO_COUNT, m_RAO_REO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RAO_RFO_COUNT, m_RAO_RFO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RAO_REO_RLSO, m_RAO_REO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RAO_REO_RMSO, m_RAO_REO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RAO_REO_RHSO, m_RAO_REO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RAO_RFO_RLSO, m_RAO_RFO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RAO_RFO_RMSO, m_RAO_RFO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RAO_RFO_RHSO, m_RAO_RFO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_PERCENT, m_RSVO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RSVO_REO_COUNT, m_RSVO_REO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RSVO_RFO_COUNT, m_RSVO_RFO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RSVO_REO_RLSO, m_RSVO_REO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_REO_RMSO, m_RSVO_REO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_REO_RHSO, m_RSVO_REO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_RFO_RLSO, m_RSVO_RFO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_RFO_RMSO, m_RSVO_RFO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RSVO_RFO_RHSO, m_RSVO_RFO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_PERCENT, m_RFVO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RFVO_REO_COUNT, m_RFVO_REO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RFVO_RFO_COUNT, m_RFVO_RFO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RFVO_REO_RLSO, m_RFVO_REO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_REO_RMSO, m_RFVO_REO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_REO_RHSO, m_RFVO_REO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_RFO_RLSO, m_RFVO_RFO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_RFO_RMSO, m_RFVO_RFO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RFVO_RFO_RHSO, m_RFVO_RFO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_PERCENT, m_RSMO_PERCENT);
	DDX_Text(pDX, IDC_EDIT_RSMO_REO_COUNT, m_RSMO_REO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RSMO_RFO_COUNT, m_RSMO_RFO_COUNT);
	DDX_Text(pDX, IDC_EDIT_RSMO_REO_RLSO, m_RSMO_REO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_REO_RMSO, m_RSMO_REO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_REO_RHSO, m_RSMO_REO_RHSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_RFO_RLSO, m_RSMO_RFO_RLSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_RFO_RMSO, m_RSMO_RFO_RMSO);
	DDX_Text(pDX, IDC_EDIT_RSMO_RFO_RHSO, m_RSMO_RFO_RHSO);
	DDX_Check(pDX, IDC_CHECK_REO, m_bREO);
	DDX_Check(pDX, IDC_CHECK_RFO, m_bRFO);
	DDX_Check(pDX, IDC_CHECK_ALL, m_bALL);
	DDX_Check(pDX, IDC_CHECK_RAO, m_bRAO);
	DDX_Check(pDX, IDC_CHECK_RAO_RLSO, m_bRAO_RLSO);
	DDX_Check(pDX, IDC_CHECK_RAO_RMSO, m_bRAO_RMSO);
	DDX_Check(pDX, IDC_CHECK_RAO_RHSO, m_bRAO_RHSO);
	DDX_Check(pDX, IDC_CHECK_RSVO, m_bRSVO);
	DDX_Check(pDX, IDC_CHECK_RSVO_RLSO, m_bRSVO_RLSO);
	DDX_Check(pDX, IDC_CHECK_RSVO_RMSO, m_bRSVO_RMSO);
	DDX_Check(pDX, IDC_CHECK_RSVO_RHSO, m_bRSVO_RHSO);
	DDX_Check(pDX, IDC_CHECK_RFVO, m_bRFVO);
	DDX_Check(pDX, IDC_CHECK_RFVO_RLSO, m_bRFVO_RLSO);
	DDX_Check(pDX, IDC_CHECK_RFVO_RMSO, m_bRFVO_RMSO);
	DDX_Check(pDX, IDC_CHECK_RFVO_RHSO, m_bRFVO_RHSO);
	DDX_Check(pDX, IDC_CHECK_RSMO, m_bRSMO);
	DDX_Check(pDX, IDC_CHECK_RSMO_RLSO, m_bRSMO_RLSO);
	DDX_Check(pDX, IDC_CHECK_RSMO_RMSO, m_bRSMO_RMSO);
	DDX_Check(pDX, IDC_CHECK_RSMO_RHSO, m_bRSMO_RHSO);
	DDX_Check(pDX, IDC_CHECK_PATH, m_bDrawPath);
}

BEGIN_MESSAGE_MAP(CCQDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCQDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CCQDialog::OnClickedClose)
	ON_MESSAGE(WM_ADDOBJECT, &CCQDialog::OnAddobject)
	ON_MESSAGE(WM_ENDOBJECT, &CCQDialog::OnEndobject)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CCQDialog::OnClickedCheckAll)
	ON_BN_CLICKED(IDC_CHECK_RAO, &CCQDialog::OnClickedCheckRao)
	ON_BN_CLICKED(IDC_CHECK_RSVO, &CCQDialog::OnClickedCheckRsvo)
	ON_BN_CLICKED(IDC_CHECK_RFVO, &CCQDialog::OnClickedCheckRfvo)
	ON_BN_CLICKED(IDC_CHECK_RSMO, &CCQDialog::OnClickedCheckRsmo)
	ON_BN_CLICKED(IDC_CHECK_RAO_RLSO, &CCQDialog::OnClickedCheckRaoRlso)
	ON_BN_CLICKED(IDC_CHECK_RAO_RMSO, &CCQDialog::OnClickedCheckRaoRmso)
	ON_BN_CLICKED(IDC_CHECK_RAO_RHSO, &CCQDialog::OnClickedCheckRaoRhso)
	ON_BN_CLICKED(IDC_CHECK_RSVO_RLSO, &CCQDialog::OnClickedCheckRsvoRlso)
	ON_BN_CLICKED(IDC_CHECK_RSVO_RMSO, &CCQDialog::OnClickedCheckRsvoRmso)
	ON_BN_CLICKED(IDC_CHECK_RSVO_RHSO, &CCQDialog::OnClickedCheckRsvoRhso)
	ON_BN_CLICKED(IDC_CHECK_RFVO_RLSO, &CCQDialog::OnClickedCheckRfvoRlso)
	ON_BN_CLICKED(IDC_CHECK_RFVO_RMSO, &CCQDialog::OnClickedCheckRfvoRmso)
	ON_BN_CLICKED(IDC_CHECK_RFVO_RHSO, &CCQDialog::OnClickedCheckRfvoRhso)
	ON_BN_CLICKED(IDC_CHECK_RSMO_RLSO, &CCQDialog::OnClickedCheckRsmoRlso)
	ON_BN_CLICKED(IDC_CHECK_RSMO_RMSO, &CCQDialog::OnClickedCheckRsmoRmso)
	ON_BN_CLICKED(IDC_CHECK_RSMO_RHSO, &CCQDialog::OnClickedCheckRsmoRhso)
	ON_BN_CLICKED(IDC_CHECK_REO, &CCQDialog::OnClickedCheckReo)
	ON_BN_CLICKED(IDC_CHECK_RFO, &CCQDialog::OnClickedCheckRfo)
	ON_BN_CLICKED(IDC_CHECK_PATH, &CCQDialog::OnClickedCheckPath)
	ON_BN_CLICKED(IDC_BUTTON1, &CCQDialog::OnBnClickedButton1)
END_MESSAGE_MAP()

//===================================================================================================

void CCQDialog::setCountVariable()
{
	UpdateData(TRUE);
	//-----------------------------------------------------------------------------
	// Enemy	// reo를 반올림
	double reo = (double)m_NO_COUNT * (double)m_REO_PERCENT / 100.0;
	m_REO_COUNT = (int)round(reo);
	{
		m_REOCQ_COUNT = (int)round(m_REO_COUNT * (double)m_REOCQ_PERCENT / 100.0);
		m_REONCQ_COUNT = (int)floor(m_REO_COUNT * (double)m_REONCQ_PERCENT / 100.0);

		double rao = reo * (double)m_RAO_PERCENT / 100.0;
		m_RAO_REO_RLSO = (int)round(rao * (double)m_RLSO_PERCENT / 100.0);
		m_RAO_REO_RMSO = (int)round(rao * (double)m_RMSO_PERCENT / 100.0);
		m_RAO_REO_RHSO = (int)round(rao * (double)m_RHSO_PERCENT / 100.0);
		m_RAO_REO_COUNT = m_RAO_REO_RLSO + m_RAO_REO_RMSO + m_RAO_REO_RHSO;

		double rsvo = reo * (double)m_RSVO_PERCENT / 100.0;
		m_RSVO_REO_RLSO = (int)round(rsvo * (double)m_RLSO_PERCENT / 100.0);
		m_RSVO_REO_RMSO = (int)round(rsvo * (double)m_RMSO_PERCENT / 100.0);
		m_RSVO_REO_RHSO = (int)round(rsvo * (double)m_RHSO_PERCENT / 100.0);
		m_RSVO_REO_COUNT = m_RSVO_REO_RLSO + m_RSVO_REO_RMSO + m_RSVO_REO_RHSO;

		double rfvo = reo * (double)m_RFVO_PERCENT / 100.0;
		m_RFVO_REO_RLSO = (int)round(rfvo * (double)m_RLSO_PERCENT / 100.0);
		m_RFVO_REO_RMSO = (int)round(rfvo * (double)m_RMSO_PERCENT / 100.0);
		m_RFVO_REO_RHSO = (int)round(rfvo * (double)m_RHSO_PERCENT / 100.0);
		m_RFVO_REO_COUNT = m_RFVO_REO_RLSO + m_RFVO_REO_RMSO + m_RFVO_REO_RHSO;

		double rsmo = reo * (double)m_RSMO_PERCENT / 100.0;
		m_RSMO_REO_RLSO = (int)round(rsmo * m_RLSO_PERCENT / 100.0);
		m_RSMO_REO_RMSO = (int)round(rsmo * m_RMSO_PERCENT / 100.0);
		m_RSMO_REO_RHSO = (int)round(rsmo * m_RHSO_PERCENT / 100.0);
		m_RSMO_REO_COUNT = m_RSMO_REO_RLSO + m_RSMO_REO_RMSO + m_RSMO_REO_RHSO;
	}
	//-----------------------------------------------------------------------------
	// Friend	// rfo를 버림
	double rfo = (double)m_NO_COUNT * (double)m_RFO_PERCENT / 100.0;
	m_RFO_COUNT = (int)floor(rfo);
	{
		m_RFOCQ_COUNT = (int)round(m_RFO_COUNT * (double)m_RFOCQ_PERCENT / 100.0);
		m_RFONCQ_COUNT = (int)floor(m_RFO_COUNT * (double)m_RFONCQ_PERCENT / 100.0);

		double rao = rfo * (double)m_RAO_PERCENT / 100.0;
		m_RAO_RFO_RLSO = (int)round(rao * (double)m_RLSO_PERCENT / 100.0);
		m_RAO_RFO_RMSO = (int)round(rao * (double)m_RMSO_PERCENT / 100.0);
		m_RAO_RFO_RHSO = (int)round(rao * (double)m_RHSO_PERCENT / 100.0);
		m_RAO_RFO_COUNT = m_RAO_RFO_RLSO + m_RAO_RFO_RMSO + m_RAO_RFO_RHSO;

		double rsvo = rfo * (double)m_RSVO_PERCENT / 100.0;
		m_RSVO_RFO_RLSO = (int)round(rsvo * (double)m_RLSO_PERCENT / 100.0);
		m_RSVO_RFO_RMSO = (int)round(rsvo * (double)m_RMSO_PERCENT / 100.0);
		m_RSVO_RFO_RHSO = (int)round(rsvo * (double)m_RHSO_PERCENT / 100.0);
		m_RSVO_RFO_COUNT = m_RSVO_RFO_RLSO + m_RSVO_RFO_RMSO + m_RSVO_RFO_RHSO;

		double rfvo = rfo * (double)m_RFVO_PERCENT / 100.0;
		m_RFVO_RFO_RLSO = (int)round(rfvo * (double)m_RLSO_PERCENT / 100.0);
		m_RFVO_RFO_RMSO = (int)round(rfvo * (double)m_RMSO_PERCENT / 100.0);
		m_RFVO_RFO_RHSO = (int)round(rfvo * (double)m_RHSO_PERCENT / 100.0);
		m_RFVO_RFO_COUNT = m_RFVO_RFO_RLSO + m_RFVO_RFO_RMSO + m_RFVO_RFO_RHSO;

		double rsmo = rfo * (double)m_RSMO_PERCENT / 100.0;
		m_RSMO_RFO_RLSO = (int)round(rsmo * m_RLSO_PERCENT / 100.0);
		m_RSMO_RFO_RMSO = (int)round(rsmo * m_RMSO_PERCENT / 100.0);
		m_RSMO_RFO_RHSO = (int)round(rsmo * m_RHSO_PERCENT / 100.0);
		m_RSMO_RFO_COUNT = m_RSMO_RFO_RLSO + m_RSMO_RFO_RMSO + m_RSMO_RFO_RHSO;
	}
	UpdateData(FALSE);
}

// 레이어 on/off 옵션 설정
void CCQDialog::setDrawOptions()
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;

	UpdateData(TRUE);
	pView->m_bREO = m_bREO;
	pView->m_bRFO = m_bRFO;

	pView->m_bRAO = m_bRAO;
	pView->m_bRAO_RLSO = m_bRAO_RLSO;
	pView->m_bRAO_RLSO = m_bRAO_RLSO;
	pView->m_bRAO_RHSO = m_bRAO_RHSO;

	pView->m_bRSVO = m_bRSVO;
	pView->m_bRSVO_RLSO = m_bRSVO_RLSO;
	pView->m_bRSVO_RMSO = m_bRSVO_RMSO;
	pView->m_bRSVO_RHSO = m_bRSVO_RHSO;

	pView->m_bRFVO = m_bRFVO;
	pView->m_bRFVO_RLSO = m_bRFVO_RLSO;
	pView->m_bRFVO_RMSO = m_bRFVO_RMSO;
	pView->m_bRFVO_RHSO = m_bRFVO_RHSO;

	pView->m_bRSMO = m_bRSMO;
	pView->m_bRSMO_RLSO = m_bRSMO_RLSO;
	pView->m_bRSMO_RMSO = m_bRSMO_RMSO;
	pView->m_bRSMO_RHSO = m_bRSMO_RHSO;

	pView->Invalidate(FALSE);
}

BOOL CCQDialog::isDrawObject(CCQObject* pObj)
{
	if (pObj->m_bFriend && !m_bRFO)		return FALSE;
	if (!pObj->m_bFriend && !m_bREO)	return FALSE;

	return *(m_bool2[pObj->m_objtype][pObj->m_speedtype]);
}

//===================================================================================================
// CCQDialog 메시지 처리기

BOOL CCQDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setCountVariable();

	CComboBox* pCbo = (CComboBox*)GetDlgItem(IDC_COMBO_CQTYPE);
	pCbo->ResetContent();
	pCbo->AddString(_T("RADAR"));
	pCbo->AddString(_T("SONAR"));
	pCbo->AddString(_T("RECTANGLE"));
	pCbo->AddString(_T("SONAR * 4"));
	pCbo->AddString(_T("RADAR + SONAR * 4"));
	pCbo->AddString(_T("(RADAR + SONAR * 4) * 2"));
	pCbo->SetCurSel(0);

	return TRUE;
}

BOOL CCQDialog::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam)
		{
			ShowWindow(SW_HIDE);
			return TRUE;
		}

		if (VK_RETURN == pMsg->wParam)
		{
			setCountVariable();
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//Generate Object in CQWork
afx_msg LRESULT CCQDialog::OnAddobject(WPARAM wParam, LPARAM lParam)
{
	double total = (double)wParam;
	double pos = (double)lParam;
	double rate = (pos / total) * 100.0;

	CString str;
	str.Format(_T("%.2f%%"), rate);

	CButton* pBtn = (CButton*)GetDlgItem(IDOK);
	pBtn->SetWindowTextW(str);

	return 0;
}

afx_msg LRESULT CCQDialog::OnEndobject(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(TRUE);

	CButton* pBtn = (CButton*)GetDlgItem(IDOK);
	pBtn->SetWindowTextW(_T("Generate"));

	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	pView->Invalidate(FALSE);

	return 0;
}

void CCQDialog::OnClickedClose()
{
	CDialogEx::OnCancel();
}

// 주어진 조건으로 데이터 생성
void CCQDialog::OnBnClickedOk()
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	UpdateData(TRUE);

	// 기존 데이터 모두 제거
	pDoc->resetData();

	// 화면을 초기 설정으로 전환
	pView->initScale();

	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);

	AfxBeginThread(&ThreadFunc, this);
}

//================================================================================================================================

PointF CCQDialog::makeStartPoint(CCQArea* pArea, int w, int h)
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;

	PointF tp;

	int tx = CUtil::getRandomLong(0, w);
	int ty = CUtil::getRandomLong(0, h);

	tp = pView->Scr2Map(tx, ty);

	// 영역 내부의 점이 아니면
	if (!pArea->isIn(tp))	return tp;

	// 영역 내부의 점이면 영역 외부로 이동
	REAL x1 = (REAL)fabs(tp.X - pArea->m_rectf.X);
	REAL x2 = (REAL)fabs(tp.X - (pArea->m_rectf.X + pArea->m_rectf.Width));
	if (x2 < x1)	tp.X += (x2 * 1.5f);
	else			tp.X -= (x1 * 1.5f);

	return tp;
}

PointF CCQDialog::makeStartPoint(vector<CCQArea*>& vec, int w, int h)
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;

	PointF tp;
	int i, size = (int)vec.size();
	while (1)
	{
		int tx = CUtil::getRandomLong(0, w);
		int ty = CUtil::getRandomLong(0, h);

		tp = pView->Scr2Map(tx, ty);

		// 영역 내부의 점이 아니면
		for (i = 0; i < size; i++)
		{
			if (vec[i]->isIn(tp))	break;
		}
		if (size <= i)	return tp;
	}
	return PointF(0.0f, 0.0f);
}

CCQArea* CCQDialog::findNearestArea(vector<CCQArea*>& vec, PointF& tp)
{
	int size = (int)vec.size();
	if (size <= 0)	return NULL;

	int minidx = 0;
	double mindist = vec[0]->getMinDistance(tp);

	for (int i = 1; i < size; i++)
	{
		double dist = vec[i]->getMinDistance(tp);
		if (dist < mindist)
		{
			mindist = dist;
			minidx = i;
		}
	}
	return vec[minidx];
}

// #define RAO	0	// 공중
// #define RSVO	1	// 군함
// #define RFVO	2	// 어선
// #define RSMO	3	// 잠수함
int _getTargetCQAreas(CPaperImageDoc* pDoc, int type, vector<CCQArea*>& vec)
{
	int size = (int)pDoc->m_CQAreas.size();
	for (int i = 0; i < size; i++)
	{
		if (pDoc->m_CQAreas[i]->m_CQType == _RADAR_ && type == RSMO)	continue;
		if (pDoc->m_CQAreas[i]->m_CQType == _SONAR_ && type != RSMO)	continue;

		vec.push_back(pDoc->m_CQAreas[i]);
	}
	return (int)vec.size();
}

UINT ThreadFunc(LPVOID pParam)
{
	CCQDialog* pDlg = (CCQDialog*)pParam;
	CPaperImageView* pView = (CPaperImageView*)pDlg->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	// 쓰레드마다 별도로 랜덤 초기화해야 함.
	srand((unsigned int)time(0));

	RectF mapRect = pView->getMapRect();

	CCQArea* pArea = NULL;
	//---------------------------------------------------------------------------------
	// CQ 객체 생성
	//---------------------------------------------------------------------------------
	// 레이더 중심 좌표 or 소나 원뿔 좌표 or Rectangle 중심좌표 or 함선의 중심 좌표
	PointF mp = pView->Scr2Map((int)(pView->m_rect.Width() / 2.0), (int)(pView->m_rect.Height() * 3.0 / 5.0));

	if (pDlg->m_CQTYPE == _RADAR_)
	{
		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _SONAR_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _RECT_)
	{
		pArea = new CCQAreaRect(mapRect, mp.X, mp.Y, RADAR_R_METER, RADAR_R_METER / 2.0);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _SONAR4_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _RADAR_SONAR4_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else	// _RADAR_SONAR4_2_
	{
		mp.X -= 200000.0f;
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		mp.X += 500000.0f;
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}

	//---------------------------------------------------------------------------------
	// MO 객체 생성 (시작점은 여기에서 설정해준다.)
	//---------------------------------------------------------------------------------
	int rec = 0;
	int w = pView->m_rect.Width();
	int h = pView->m_rect.Height();

	double typeOptions[4];	// RAO, RSVO, RFVO, RSMO
	typeOptions[RAO] = (double)pDlg->m_RAO_PERCENT / 100.0;
	typeOptions[RSVO] = (double)pDlg->m_RSVO_PERCENT / 100.0;
	typeOptions[RFVO] = (double)pDlg->m_RFVO_PERCENT / 100.0;
	typeOptions[RSMO] = (double)pDlg->m_RSMO_PERCENT / 100.0;

	double speedOptions[3];	// RLSO, RMSO, RHSO
	speedOptions[RLSO] = (double)pDlg->m_RLSO_PERCENT / 100.0;
	speedOptions[RMSO] = (double)pDlg->m_RMSO_PERCENT / 100.0;
	speedOptions[RHSO] = (double)pDlg->m_RHSO_PERCENT / 100.0;

	double cqOptions[2];		// 적군 객체에만 적용 (CQ접근, CQ무관)
	cqOptions[0] = (double)pDlg->m_REOCQ_PERCENT / 100.0;
	cqOptions[1] = (double)pDlg->m_REONCQ_PERCENT / 100.0;

	int CntF = (int)round(pDlg->m_NO_COUNT * pDlg->m_RFO_PERCENT / 100.0);	// 전체 아군객체수
	int CntE = (int)round(pDlg->m_NO_COUNT * pDlg->m_REO_PERCENT / 100.0);	// 전체 적군객체수

	// { RLSOmin_kmh, RLSOmax_kmh, RMSOmin_kmh, RMSOmax_kmh, RHSOmin_kmh, RHSOmax_kmht }
	int divOptions[][8] = {
		{ 1, 300, 600, 1200, 1500, 2400	},	// RAO 속도 분포
		{ 1,  20,  30,   50,   60,  100 },	// RSVO 속도 분포
		{ 1,  10,  20,   30,   40,   50 },	// RFVO 속도 분포
		{ 1,  10,  20,   40,   60,  100 }	// RSMO 속도 분포
	};

	PointF tp;
	// 아군 객체 추가
	int loop = sizeof(divOptions) / sizeof(divOptions[0]);
	for (int i = 0; i < loop; i++)									// 항공, 군함, 어선, 잠수함
	{
		vector<CCQArea*> vec;
		int cnt = _getTargetCQAreas(pDoc, i, vec);
		if (cnt <= 0)	continue;	// 대상 CQ가 없으면

		for (int j = 0; j < 3; j++)									// 저속, 중속, 고속
		{
			int mins = divOptions[i][j * 2];
			int maxs = divOptions[i][j * 2 + 1];
			int count = (int)round(CntF * typeOptions[i] * speedOptions[j]);
			for (int k = 0; k < count; k++)
			{
				int trjtype = CUtil::getRandomLong(102, 105);		// 진입만 있는 객체는 없으므로 TRJTYPE1(101)타입은 생략
				int speed = CUtil::getRandomLong(mins, maxs);

				tp = pDlg->makeStartPoint(vec, w, h);
				CCQArea* tArea = pDlg->findNearestArea(vec, tp);

				// speed km/h 속도를 가지고 trjtype의 경로를 따라 이동하는 tp위치의 divs[i][0] 객체 생성
				CCQObject* pObj = new CCQObject(i, j, tp, tArea, trjtype, speed, TRUE);
				pDoc->m_CQObjects.push_back(pObj);
				pDlg->SendMessage(WM_ADDOBJECT, pDlg->m_NO_COUNT, rec++);
			}
		}
	}

	// 적군 객체 추가
	for (int cq = 0; cq < 2; cq++)
	{
		for (int i = 0; i < loop; i++)								// 항공, 군함, 어선, 잠수함
		{
			vector<CCQArea*> vec;
			int cnt = _getTargetCQAreas(pDoc, i, vec);
			if (cnt <= 0)	continue;	// 대상 CQ가 없으면

			for (int j = 0; j < 3; j++)								// 저속, 중속, 고속
			{
				int mins = divOptions[i][j * 2];
				int maxs = divOptions[i][j * 2 + 1];
				int count = (int)round(CntE * typeOptions[i] * speedOptions[j] * cqOptions[cq]);
				for (int k = 0; k < count; k++)
				{
					int trjtype = TRJTYPE5;;
					if (k == 0)	trjtype = CUtil::getRandomLong(102, 104);
					int speed = CUtil::getRandomLong(mins, maxs);

					tp = pDlg->makeStartPoint(vec, w, h);
					CCQArea* tArea = pDlg->findNearestArea(vec, tp);

					// speed km/h 속도를 가지고 trjtype의 경로를 따라 이동하는 tp위치의 divs[i][0] 객체 생성
					CCQObject* pObj = new CCQObject(i, j, tp, tArea, trjtype, speed, FALSE);
					pDoc->m_CQObjects.push_back(pObj);
					pDlg->SendMessage(WM_ADDOBJECT, pDlg->m_NO_COUNT, rec++);
				}
			}
		}
	}

	pDlg->SendMessage(WM_ENDOBJECT);
	return 0;
}

//================================================================================================================================

void CCQDialog::InvalidateView()
{
	UpdateData(TRUE);
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	pView->Invalidate(FALSE);
}

void CCQDialog::OnClickedCheckReo(){ InvalidateView(); }
void CCQDialog::OnClickedCheckRfo(){ InvalidateView(); }

void CCQDialog::OnClickedCheckAll()
{
	UpdateData(TRUE);

	for (int i = 0; i < 4; i++)
	{
		*m_bool1[i] = m_bALL;
		for (int j = 0; j < 3; j++)
		{
			*m_bool2[i][j] = m_bALL;
			*m_bool2[i][j] = m_bALL;
			*m_bool2[i][j] = m_bALL;
		}
	}
	UpdateData(FALSE);
	InvalidateView();
}

void CCQDialog::ClickedCheck1(int type)
{
	UpdateData(TRUE);
	BOOL val;
	if (type == RAO)		val = m_bRAO;
	else if (type == RSVO)	val = m_bRSVO;
	else if (type == RFVO)	val = m_bRFVO;
	else					val = m_bRSMO;

	*m_bool1[type] = val;
	for (int j = 0; j < 3; j++)
	{
		*m_bool2[type][j] = val;
		*m_bool2[type][j] = val;
		*m_bool2[type][j] = val;
	}

	UpdateData(FALSE);
	InvalidateView();
}

void CCQDialog::OnClickedCheckRao() { ClickedCheck1(RAO); }
void CCQDialog::OnClickedCheckRsvo(){ ClickedCheck1(RSVO); }
void CCQDialog::OnClickedCheckRfvo(){ ClickedCheck1(RFVO); }
void CCQDialog::OnClickedCheckRsmo(){ ClickedCheck1(RSMO); }

void CCQDialog::ClickedCheck2(int type, int spd)
{
	UpdateData(TRUE);
	BOOL val;

	if (type == RAO)
	{
		switch (spd)
		{
		case RLSO:	val = m_bRAO_RLSO;	break;
		case RMSO:	val = m_bRAO_RMSO;	break;
		default:	val = m_bRAO_RHSO;	break;
		}
	}
	else if (type == RSVO)
	{
		switch (spd)
		{
		case RLSO:	val = m_bRSVO_RLSO;	break;
		case RMSO:	val = m_bRSVO_RMSO;	break;
		default:	val = m_bRSVO_RHSO;	break;
		}
	}
	else if (type == RFVO)
	{
		switch (spd)
		{
		case RLSO:	val = m_bRFVO_RLSO;	break;
		case RMSO:	val = m_bRFVO_RMSO;	break;
		default:	val = m_bRFVO_RHSO;	break;
		}
	}
	else
	{
		switch (spd)
		{
		case RLSO:	val = m_bRSMO_RLSO;	break;
		case RMSO:	val = m_bRSMO_RMSO;	break;
		default:	val = m_bRSMO_RHSO;	break;
		}
	}

	*m_bool2[type][spd] = val;

	UpdateData(FALSE);
	InvalidateView();
}
void CCQDialog::OnClickedCheckRaoRlso(){	ClickedCheck2(RAO, RLSO);	}
void CCQDialog::OnClickedCheckRaoRmso(){	ClickedCheck2(RAO, RMSO);	}
void CCQDialog::OnClickedCheckRaoRhso(){	ClickedCheck2(RAO, RHSO);	}
void CCQDialog::OnClickedCheckRsvoRlso(){	ClickedCheck2(RSVO, RLSO);	}
void CCQDialog::OnClickedCheckRsvoRmso(){	ClickedCheck2(RSVO, RMSO);	}
void CCQDialog::OnClickedCheckRsvoRhso(){	ClickedCheck2(RSVO, RHSO);	}
void CCQDialog::OnClickedCheckRfvoRlso(){	ClickedCheck2(RFVO, RLSO);	}
void CCQDialog::OnClickedCheckRfvoRmso(){	ClickedCheck2(RFVO, RMSO);	}
void CCQDialog::OnClickedCheckRfvoRhso(){	ClickedCheck2(RFVO, RHSO);	}
void CCQDialog::OnClickedCheckRsmoRlso(){	ClickedCheck2(RSMO, RLSO);	}
void CCQDialog::OnClickedCheckRsmoRmso(){	ClickedCheck2(RSMO, RMSO);	}
void CCQDialog::OnClickedCheckRsmoRhso(){	ClickedCheck2(RSMO, RHSO);	}
void CCQDialog::OnClickedCheckPath() { InvalidateView(); }


void CCQDialog::OnBnClickedButton1()
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	UpdateData(TRUE);

	// 기존 데이터 모두 제거
	pDoc->resetData();

	// 화면을 초기 설정으로 전환
	pView->initScale();

	UpdateData(TRUE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_CLOSE)->EnableWindow(FALSE);

	AfxBeginThread(&ImportThreadFunc, this);
}

UINT ImportThreadFunc(LPVOID pParam)
{
	CCQDialog* pDlg = (CCQDialog*)pParam;
	CPaperImageView* pView = (CPaperImageView*)pDlg->m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	// 쓰레드마다 별도로 랜덤 초기화해야 함.
	srand((unsigned int)time(0));

	RectF mapRect = pView->getMapRect();

	CCQArea* pArea = NULL;
	//---------------------------------------------------------------------------------
	// CQ 객체 생성
	//---------------------------------------------------------------------------------
	// 레이더 중심 좌표 or 소나 원뿔 좌표 or Rectangle 중심좌표 or 함선의 중심 좌표
	PointF mp = pView->Scr2Map((int)(pView->m_rect.Width() / 2.0), (int)(pView->m_rect.Height() * 3.0 / 5.0));

	if (pDlg->m_CQTYPE == _RADAR_)
	{
		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _SONAR_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
		if (pDlg->m_NO_COUNT < 25)
			pDlg->m_NO_COUNT = 25;
	}
	else if (pDlg->m_CQTYPE == _RECT_)
	{
		pArea = new CCQAreaRect(mapRect, mp.X, mp.Y, RADAR_R_METER, RADAR_R_METER / 2.0);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else if (pDlg->m_CQTYPE == _SONAR4_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
		if (pDlg->m_NO_COUNT < 25)
			pDlg->m_NO_COUNT = 25;
	}
	else if (pDlg->m_CQTYPE == _RADAR_SONAR4_)
	{
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}
	else	// _RADAR_SONAR4_2_
	{
		mp.X -= 200000.0f;
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		mp.X += 500000.0f;
		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 270.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 90.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 0.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaArc(mapRect, mp.X, mp.Y, 180.0, 100.0, SONAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);

		pArea = new CCQAreaCircle(mapRect, mp.X, mp.Y, RADAR_R_METER);
		pDoc->m_CQAreas.push_back(pArea);
	}

	//---------------------------------------------------------------------------------
	// MO 객체 생성 (시작점은 여기에서 설정해준다.)
	//---------------------------------------------------------------------------------
	int rec = 0;
	int w = pView->m_rect.Width();
	int h = pView->m_rect.Height();

	double typeOptions[4];	// RAO, RSVO, RFVO, RSMO
	typeOptions[RAO] = (double)pDlg->m_RAO_PERCENT / 100.0;
	typeOptions[RSVO] = (double)pDlg->m_RSVO_PERCENT / 100.0;
	typeOptions[RFVO] = (double)pDlg->m_RFVO_PERCENT / 100.0;
	typeOptions[RSMO] = (double)pDlg->m_RSMO_PERCENT / 100.0;

	double speedOptions[3];	// RLSO, RMSO, RHSO
	speedOptions[RLSO] = (double)pDlg->m_RLSO_PERCENT / 100.0;
	speedOptions[RMSO] = (double)pDlg->m_RMSO_PERCENT / 100.0;
	speedOptions[RHSO] = (double)pDlg->m_RHSO_PERCENT / 100.0;

	double cqOptions[2];		// 적군 객체에만 적용 (CQ접근, CQ무관)
	cqOptions[0] = (double)pDlg->m_REOCQ_PERCENT / 100.0;
	cqOptions[1] = (double)pDlg->m_REONCQ_PERCENT / 100.0;

	int CntF = (int)round(pDlg->m_NO_COUNT * pDlg->m_RFO_PERCENT / 100.0);	// 전체 아군객체수
	int CntE = (int)round(pDlg->m_NO_COUNT * pDlg->m_REO_PERCENT / 100.0);	// 전체 적군객체수

	// { RLSOmin_kmh, RLSOmax_kmh, RMSOmin_kmh, RMSOmax_kmh, RHSOmin_kmh, RHSOmax_kmht }
	int divOptions[][8] = {
		{ 1, 300, 600, 1200, 1500, 2400	},	// RAO 속도 분포
		{ 1,  20,  30,   50,   60,  100 },	// RSVO 속도 분포
		{ 1,  10,  20,   30,   40,   50 },	// RFVO 속도 분포
		{ 1,  10,  20,   40,   60,  100 }	// RSMO 속도 분포
	};

	std::ifstream infile("events.txt");
	string line, token;
	vector<int> ids;
	vector<int> objTypes;
	vector<int> speedTypes;
	vector<PointF> initPoints;
	vector<int> trjTypes;
	vector<int> kmhs;
	vector<int> friendTypes;
	int objCounter = 0;
	while (getline(infile, line)) {
		std::istringstream tokenizer(line);

		getline(tokenizer, token, '|'); ids.push_back(stoi(token));
		getline(tokenizer, token, '|'); objTypes.push_back(stoi(token));
		getline(tokenizer, token, '|'); speedTypes.push_back(stoi(token));

		getline(tokenizer, token, '|');
		istringstream tokenizer2(token);
		string x, y;
		getline(tokenizer2, x, ','); getline(tokenizer2, y, ',');
		initPoints.push_back(PointF(stoi(x), stoi(y)));

		getline(tokenizer, token, '|'); trjTypes.push_back(stoi(token));
		getline(tokenizer, token, '|'); kmhs.push_back(stoi(token));
		getline(tokenizer, token); friendTypes.push_back(stoi(token));
		objCounter++;
	}
	infile.close();
	int obj_id = 0;
	PointF tp;
	// 아군 객체 추가
	int loop = sizeof(divOptions) / sizeof(divOptions[0]);
	for (int i = 0; i < loop; i++)									// 항공, 군함, 어선, 잠수함
	{
		vector<CCQArea*> vec;
		int cnt = _getTargetCQAreas(pDoc, i, vec);
		if (cnt <= 0)	continue;	// 대상 CQ가 없으면

		for (int j = 0; j < 3; j++)									// 저속, 중속, 고속
		{
			int mins = divOptions[i][j * 2];
			int maxs = divOptions[i][j * 2 + 1];
			int count = (int)round(CntF * typeOptions[i] * speedOptions[j]);
			for (int k = 0; k < count; k++)
			{
				tp = pDlg->makeStartPoint(vec, w, h);
				CCQArea* tArea = pDlg->findNearestArea(vec, tp);

				// speed km/h 속도를 가지고 trjtype의 경로를 따라 이동하는 tp위치의 divs[i][0] 객체 생성
				CCQObject* pObj = new CCQObject(objTypes[obj_id], speedTypes[obj_id], initPoints[obj_id],
					tArea, trjTypes[obj_id], kmhs[obj_id], friendTypes[obj_id]);
				pObj->m_id = obj_id;
				pDoc->m_CQObjects.push_back(pObj);
				pDlg->SendMessage(WM_ADDOBJECT, pDlg->m_NO_COUNT, rec++);
				obj_id++;
			}
		}
	}

	// 적군 객체 추가
	for (int cq = 0; cq < 2; cq++)
	{
		for (int i = 0; i < loop; i++)								// 항공, 군함, 어선, 잠수함
		{
			vector<CCQArea*> vec;
			int cnt = _getTargetCQAreas(pDoc, i, vec);
			if (cnt <= 0)	continue;	// 대상 CQ가 없으면

			for (int j = 0; j < 3; j++)								// 저속, 중속, 고속
			{
				int mins = divOptions[i][j * 2];
				int maxs = divOptions[i][j * 2 + 1];
				int count = (int)round(CntE * typeOptions[i] * speedOptions[j] * cqOptions[cq]);
				for (int k = 0; k < count; k++)
				{
					int trjtype = TRJTYPE5;;
					if (k == 0)	trjtype = CUtil::getRandomLong(102, 104);
					int speed = CUtil::getRandomLong(mins, maxs);

					tp = pDlg->makeStartPoint(vec, w, h);
					CCQArea* tArea = pDlg->findNearestArea(vec, tp);

					// speed km/h 속도를 가지고 trjtype의 경로를 따라 이동하는 tp위치의 divs[i][0] 객체 생성
					CCQObject* pObj = new CCQObject(objTypes[obj_id], speedTypes[obj_id], initPoints[obj_id],
						tArea, trjTypes[obj_id], kmhs[obj_id], friendTypes[obj_id]);
					pObj->m_id = obj_id;
					pDoc->m_CQObjects.push_back(pObj);
					pDlg->SendMessage(WM_ADDOBJECT, pDlg->m_NO_COUNT, rec++);
					obj_id++;
				}
			}
		}
	}
	pDlg->SendMessage(WM_ENDOBJECT);
	pDlg->ShowWindow(SW_HIDE);
	return 0;
}