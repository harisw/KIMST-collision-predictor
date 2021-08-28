#pragma once

#define WM_SIMUL	(WM_USER + 201)

// CSimulDlg 대화 상자

class CSimulDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSimulDlg)

public:
	CSimulDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSimulDlg();

	BOOL m_bPlay;
	int m_nIdx;

	CWnd* m_pWnd;
	int m_nMax;

	int getMaxCount();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPercent;
	CString m_strCount;
	BOOL m_bDrawPath;
	double m_dInterval;
	CSliderCtrl m_ctrlSlider;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedReset();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickedCheckDrawpath();
	afx_msg void OnBnClickedClose();
};
