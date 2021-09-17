// CScenarioDialog.cpp : implementation file
//

#include "pch.h"
#include "PaperImage.h"
#include "CScenarioDialog.h"
#include "afxdialogex.h"
#include "Util.h"
#include "CQObject.h"
#include "CQArea.h"
#include "RuleDlg.h"

#define WM_ADDOBJECT	(WM_USER + 1)
#define WM_ENDOBJECT	(WM_USER + 2)
// CScenarioDialog dialog

IMPLEMENT_DYNAMIC(CScenarioDialog, CDialogEx)

CScenarioDialog::CScenarioDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCQDIALOG1, pParent)
	, m_pWnd(pParent)
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

CScenarioDialog::~CScenarioDialog()
{
}

void CScenarioDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScenarioDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CScenarioDialog::OnBnClickedOk)
	//ON_BN_CLICKED(IDC_CHECK_ALL, &CScenarioDialog::OnClickedCheckAll)
	//ON_BN_CLICKED(IDC_CHECK_RAO, &CScenarioDialog::OnClickedCheckRao)
	//ON_BN_CLICKED(IDC_CHECK_RSVO, &CScenarioDialog::OnClickedCheckRsvo)
	//ON_BN_CLICKED(IDC_CHECK_RFVO, &CScenarioDialog::OnClickedCheckRfvo)
	//ON_BN_CLICKED(IDC_CHECK_RSMO, &CScenarioDialog::OnClickedCheckRsmo)
	//ON_BN_CLICKED(IDC_CHECK_RAO_RLSO, &CScenarioDialog::OnClickedCheckRaoRlso)
	//ON_BN_CLICKED(IDC_CHECK_RAO_RMSO, &CScenarioDialog::OnClickedCheckRaoRmso)
	//ON_BN_CLICKED(IDC_CHECK_RAO_RHSO, &CScenarioDialog::OnClickedCheckRaoRhso)
	//ON_BN_CLICKED(IDC_CHECK_RSVO_RLSO, &CScenarioDialog::OnClickedCheckRsvoRlso)
	//ON_BN_CLICKED(IDC_CHECK_RSVO_RMSO, &CScenarioDialog::OnClickedCheckRsvoRmso)
	//ON_BN_CLICKED(IDC_CHECK_RSVO_RHSO, &CScenarioDialog::OnClickedCheckRsvoRhso)
	//ON_BN_CLICKED(IDC_CHECK_RFVO_RLSO, &CScenarioDialog::OnClickedCheckRfvoRlso)
	//ON_BN_CLICKED(IDC_CHECK_RFVO_RMSO, &CScenarioDialog::OnClickedCheckRfvoRmso)
	//ON_BN_CLICKED(IDC_CHECK_RFVO_RHSO, &CScenarioDialog::OnClickedCheckRfvoRhso)
	//ON_BN_CLICKED(IDC_CHECK_RSMO_RLSO, &CScenarioDialog::OnClickedCheckRsmoRlso)
	//ON_BN_CLICKED(IDC_CHECK_RSMO_RMSO, &CScenarioDialog::OnClickedCheckRsmoRmso)
	//ON_BN_CLICKED(IDC_CHECK_RSMO_RHSO, &CScenarioDialog::OnClickedCheckRsmoRhso)
	//ON_BN_CLICKED(IDC_CHECK_REO, &CScenarioDialog::OnClickedCheckReo)
	//ON_BN_CLICKED(IDC_CHECK_RFO, &CScenarioDialog::OnClickedCheckRfo)
	//ON_BN_CLICKED(IDC_CHECK_PATH, &CScenarioDialog::OnClickedCheckPath)
END_MESSAGE_MAP()

BOOL CScenarioDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setCountVariable();

	//pCbo->AddString(_T("RADAR"));
	//pCbo->AddString(_T("SONAR"));
	//pCbo->AddString(_T("RECTANGLE"));
	//pCbo->AddString(_T("SONAR * 4"));
	//pCbo->AddString(_T("RADAR + SONAR * 4"));
	//pCbo->AddString(_T("(RADAR + SONAR * 4) * 2"));
	//pCbo->SetCurSel(0);

	return TRUE;
}

// CScenarioDialog message handlers
BOOL CScenarioDialog::PreTranslateMessage(MSG* pMsg)
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

void CScenarioDialog::setCountVariable()
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
//void CScenarioDialog::setDrawOptions()
//{
//	
//	CPaperImageView* pView = (CPaperImageView*)m_pWnd->;
//
//	UpdateData(TRUE);
//	pView->m_bREO = m_bREO;
//	pView->m_bRFO = m_bRFO;
//
//	pView->m_bRAO = m_bRAO;
//	pView->m_bRAO_RLSO = m_bRAO_RLSO;
//	pView->m_bRAO_RLSO = m_bRAO_RLSO;
//	pView->m_bRAO_RHSO = m_bRAO_RHSO;
//
//	pView->m_bRSVO = m_bRSVO;
//	pView->m_bRSVO_RLSO = m_bRSVO_RLSO;
//	pView->m_bRSVO_RMSO = m_bRSVO_RMSO;
//	pView->m_bRSVO_RHSO = m_bRSVO_RHSO;
//
//	pView->m_bRFVO = m_bRFVO;
//	pView->m_bRFVO_RLSO = m_bRFVO_RLSO;
//	pView->m_bRFVO_RMSO = m_bRFVO_RMSO;
//	pView->m_bRFVO_RHSO = m_bRFVO_RHSO;
//
//	pView->m_bRSMO = m_bRSMO;
//	pView->m_bRSMO_RLSO = m_bRSMO_RLSO;
//	pView->m_bRSMO_RMSO = m_bRSMO_RMSO;
//	pView->m_bRSMO_RHSO = m_bRSMO_RHSO;
//
//	pView->Invalidate(FALSE);
//}

void CScenarioDialog::OnBnClickedOk()
{
	RuleDlg* pDlg = (RuleDlg*)m_pWnd;

	//double typeOptions[4];	// RAO, RSVO, RFVO, RSMO
	pDlg->typeOptions[RAO] = (double)m_RAO_PERCENT / 100.0;
	pDlg->typeOptions[RSVO] = (double)m_RSVO_PERCENT / 100.0;
	pDlg->typeOptions[RFVO] = (double)m_RFVO_PERCENT / 100.0;
	pDlg->typeOptions[RSMO] = (double)m_RSMO_PERCENT / 100.0;

	//double speedOptions[3];	// RLSO, RMSO, RHSO
	pDlg->speedOptions[RLSO] = (double)m_RLSO_PERCENT / 100.0;
	pDlg->speedOptions[RMSO] = (double)m_RMSO_PERCENT / 100.0;
	pDlg->speedOptions[RHSO] = (double)m_RHSO_PERCENT / 100.0;

	//double cqOptions[2];		// 적군 객체에만 적용 (CQ접근, CQ무관)
	pDlg->cqOptions[0] = (double)m_REOCQ_PERCENT / 100.0;
	pDlg->cqOptions[1] = (double)m_REONCQ_PERCENT / 100.0;

	pDlg->CntF = (int)round(m_NO_COUNT * m_RFO_PERCENT / 100.0);	// 전체 아군객체수
	pDlg->CntE = (int)round(m_NO_COUNT * m_REO_PERCENT / 100.0);	// 전체 적군객체수

	this->CloseWindow();
}
