// SimulDlg.cpp: 구현 파일
//

#include "pch.h"
#include "PaperImage.h"
#include "SimulDlg.h"
#include "afxdialogex.h"

#include "PaperImageDoc.h"
#include "PaperImageView.h"

// CSimulDlg 대화 상자

IMPLEMENT_DYNAMIC(CSimulDlg, CDialogEx)

CSimulDlg::CSimulDlg(CWnd* pParent)
	: CDialogEx(IDD_SIMULDLG, pParent)
	, m_pWnd(pParent)
	, m_strPercent(_T("[0.00%]"))
	, m_strCount(_T("0"))
	, m_bDrawPath(TRUE)
	, m_dInterval(0.0001)
{
	m_nIdx = 0;
	m_nMax = 0;
	m_bPlay = FALSE;
}

CSimulDlg::~CSimulDlg()
{
}

void CSimulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PERCENT, m_strPercent);
	DDX_Text(pDX, IDC_COUNT, m_strCount);
	DDX_Check(pDX, IDC_CHECK_DRAWPATH, m_bDrawPath);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_dInterval);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSlider);
}

BEGIN_MESSAGE_MAP(CSimulDlg, CDialogEx)
	ON_BN_CLICKED(ID_PLAY, &CSimulDlg::OnBnClickedPlay)
	ON_BN_CLICKED(ID_RESET, &CSimulDlg::OnBnClickedReset)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CSimulDlg::OnReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_CHECK_DRAWPATH, &CSimulDlg::OnClickedCheckDrawpath)
	ON_BN_CLICKED(ID_CLOSE, &CSimulDlg::OnBnClickedClose)
END_MESSAGE_MAP()

//======================================================================================

int CSimulDlg::getMaxCount()
{
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	CPaperImageDoc* pDoc = pView->GetDocument();

	return pDoc->getMaxCount();
}

//======================================================================================
// CSimulDlg 메시지 처리기

BOOL CSimulDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_strCount.Format(_T("[%d-0]"), m_nMax);
	m_strPercent = _T("[0.0%]");

	m_ctrlSlider.SetRange(0, 100);

	UpdateData(FALSE);

	return TRUE;
}

void CSimulDlg::OnBnClickedPlay()
{
	UpdateData(TRUE);

	if (m_dInterval <= 0.0)	return;

	CButton* pBtn = (CButton*)GetDlgItem(ID_PLAY);
	if (!m_bPlay)
	{
		m_nMax = getMaxCount();

		m_bPlay = TRUE;
		pBtn->SetWindowTextW(_T("Stop"));
		SetTimer(100, (int)round(m_dInterval * 1000.0), NULL);
	}
	else
	{
		m_bPlay = FALSE;
		pBtn->SetWindowTextW(_T("Play"));
		KillTimer(100);
	}
}

void CSimulDlg::OnBnClickedReset()
{
	m_nIdx = 0;
	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	pView->SendMessage(WM_SIMUL, m_nIdx, m_bDrawPath);
}

void CSimulDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);

	KillTimer(100);

	UpdateData(TRUE);

	CPaperImageView* pView = (CPaperImageView*)m_pWnd;

	double rate = (double)m_nIdx * 100.0 / (double)m_nMax;

	m_strPercent.Format(_T("[%.2f%%]"), rate);
	m_strCount.Format(_T("[%d-%d]"), m_nIdx, m_nMax);
	UpdateData(FALSE);

	m_ctrlSlider.SetPos((int)round(rate));

	pView->SendMessage(WM_SIMUL, m_nIdx, m_bDrawPath);

	m_nIdx++;

	SetTimer(100, (int)round(m_dInterval * 1000.0), NULL);
}

void CSimulDlg::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	int pos = m_ctrlSlider.GetPos();

	m_nIdx = (int)round((double)m_nMax * (double)pos / 100.0);;

	*pResult = 0;
}

void CSimulDlg::OnClickedCheckDrawpath()
{
	if (m_bPlay)	return;

	UpdateData(TRUE);

	CPaperImageView* pView = (CPaperImageView*)m_pWnd;
	pView->Invalidate(FALSE);
}

void CSimulDlg::OnBnClickedClose()
{
	if (m_bPlay)
	{
		CButton* pBtn = (CButton*)GetDlgItem(ID_PLAY);
		pBtn->SetWindowTextW(_T("Play"));
		KillTimer(100);
		m_bPlay = FALSE;
	}
	ShowWindow(SW_HIDE);
}
